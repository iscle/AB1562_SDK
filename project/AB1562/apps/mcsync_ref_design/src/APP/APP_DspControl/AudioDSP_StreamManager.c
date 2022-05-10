/* Copyright Statement:
 *
 * (C) 2017  Airoha Technology Corp. All rights reserved.
 *
 * This software/firmware and related documentation ("Airoha Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to Airoha Technology Corp. ("Airoha") and/or its licensors.
 * Without the prior written permission of Airoha and/or its licensors,
 * any reproduction, modification, use or disclosure of Airoha Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 * You may only use, reproduce, modify, or distribute (as applicable) Airoha Software
 * if you have agreed to and been bound by the applicable license agreement with
 * Airoha ("License Agreement") and been granted explicit permission to do so within
 * the License Agreement ("Permitted User").  If you are not a Permitted User,
 * please cease any access or use of Airoha Software immediately.
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT AIROHA SOFTWARE RECEIVED FROM AIROHA AND/OR ITS REPRESENTATIVES
 * ARE PROVIDED TO RECEIVER ON AN "AS-IS" BASIS ONLY. AIROHA EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES AIROHA PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH AIROHA SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN AIROHA SOFTWARE. AIROHA SHALL ALSO NOT BE RESPONSIBLE FOR ANY AIROHA
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND AIROHA'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO AIROHA SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT AIROHA'S OPTION, TO REVISE OR REPLACE AIROHA SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * AIROHA FOR SUCH AIROHA SOFTWARE AT ISSUE.
 */


#include "bt.h"
#include "transform.h"
#include "App_ResourceManager.h"
#include "AudioDSP_StreamManager.h"
#include "AudioDSP_AncPassThru.h"
#include "A2dp_Interface.h"
#include "App_VolumeSetting.h"
#include "App_PeqManage.h"
#include "Peq_NvkeyHandler.h"
#include "nvkey_hdl.h"
#include "APP_HfpScoHandler.h"
#include "App_VolumeManage.h"
#include "Volume_NvkeyHandler.h"
#include "App_Interface.h"
#include "App_Hfp.h"
#include "App_Media.h"
#include "drv_audio_nvkey.h"
#include "drv_rt.h"
#include "drv_mp3.h"
#include "drv_sidetone.h"
#include "drv_encoder.h"
#ifdef LINEIN_ENABLE
#include "drv_line_in.h"
#endif
#include "drv_sco.h"
#include "drv_a2dp.h"
#include "drv_anc.h"
#include "drv_ama.h"
#include "drv_vad.h"
#if LAB_TUNNING_APP
#include "drv_recorder.h"
#endif
#ifdef AIROHA_WWE_ENABLE
#include "App_Vad.h"
#endif
#include "AudioDSP_Vp.h"
#include "App_VolumeScoMicProcess.h"
#include "App_VolumeScoProcess.h"
#include "App_PowerOff.h"

#include "App_VpRtControl.h"
#include "App_RhoHandler.h"
#include "App_MCSync_Message.h"
#include "BtMCSync.h"
#include "AudioDSP_Registration.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "App_A2dp.h"
#ifdef XIAOAI_DEVELOPMENT
#include "drv_xiaoai.h"
#endif
#ifdef APP_OPUS_ENCODER
#include "APP_Encoder.h"
#endif
#include "App_Customer.h"
#include "APP_AiroThru.h"
#include "drv_calibration.h"

log_create_module(APP_DSP_CONTROL, PRINT_LEVEL_INFO);

/******************************************************************************
* Structure
*******************************************************************************/
typedef struct
{
	BOOL isSuspended;
}AUDIODSP_CTL_STRU;

typedef struct
{
    U8 path_type;
    U8 audio_mode;
    BOOL (*f_is_playing_sys)(void);
    BOOL (*f_is_playing_link)(BD_ADDR_T *pBdAddr);
    BOOL (*f_stop_sys)(BOOL is_suspend);
    BOOL (*f_stop_link)(BD_ADDR_T *pBdAddr);
}AUDIO_CTL_TABLE_T;

typedef struct
{
    U8 codec;
    U8 audio_type;
    BD_ADDR_T bdAddr;
	U8 scms;
	U8 sampleRateIndex;
}PATH_MAIN_CTL;

typedef struct
{
    U8 audio_type;
}PATH_PROMT_CTL;

typedef struct
{
    U8 audio_type;
    BD_ADDR_T bdAddr;
    U32 bit_rate;
}PATH_ASSISTANT_CTL;

typedef struct
{
    U8 audio_type;
}PATH_BG_EFFECT_CTL;

typedef struct
{
    PATH_MAIN_CTL main_ctl;
    PATH_PROMT_CTL prompt_ctl;
    PATH_ASSISTANT_CTL assistant_ctl;
    PATH_BG_EFFECT_CTL bg_effect_ctl;
}AUDIO_PATH_CTL;

/******************************************************************************
* Enumeration
*******************************************************************************/
enum
{
	AUDIO_PATH_NON_DEFINE,
    AUDIO_PATH_MAIN_OUT_IN,
    AUDIO_PATH_MAIN_OUT,
    AUDIO_PATH_PROMPT,
    AUDIO_PATH_ASSISTANT,
    AUDIO_PATH_BG_EFFECT,
}; //Audio Path Group

enum
{
    AUDIO_MODE_NONE,
    AUDIO_MODE_SYSTEM,
    AUDIO_MODE_LINK,
}; //Audio Type

/******************************************************************************
* Static Functions prototype
*******************************************************************************/
static BOOL app_AudioDsp_PlayCheck(U8 audioType, BOOL is_resume);
static U32 app_audioDsp_handler(Handler handler, U16 id, void *msg, U32 handler_id);

/******************************************************************************
* Variable
*******************************************************************************/
static AUDIODSP_CTL_STRU gAudioDspCtl;
static AUDIO_PATH_CTL g_audio_path_ctl;
static const HandlerData gAppAudioDspHandle = { app_audioDsp_handler };

extern APP_VPRT_INFO_STRU gVpRtInfo;
extern void *pVPRT_semaphore;
extern unsigned int port_OsStart;

static const AUDIO_CTL_TABLE_T gAudioTable[AUDIO_STREAM_NO] =
{
    /*AUDIO_NONE*/  {AUDIO_PATH_NON_DEFINE,     AUDIO_MODE_NONE,    NULL,                       NULL,                   NULL,       NULL},
    /*AUDIO_SCO*/   {AUDIO_PATH_MAIN_OUT_IN,    AUDIO_MODE_LINK,    NULL,                       drv_sco_is_playing,     NULL,       drv_sco_stop},
    /*AUDIO_A2DP*/  {AUDIO_PATH_MAIN_OUT,       AUDIO_MODE_LINK,    NULL,                       drv_a2dp_is_playing,    NULL,       drv_a2dp_stop},
    /*AUDIO_ADDA*/  {AUDIO_PATH_BG_EFFECT,      AUDIO_MODE_SYSTEM,  drv_sidetone_is_playing,    NULL,                   NULL,       NULL},
    /*AUDIO_VP*/    {AUDIO_PATH_PROMPT,         AUDIO_MODE_SYSTEM,  drv_vp_is_playing,          NULL,                   drv_vp_stop,NULL},
    /*AUDIO_RT*/    {AUDIO_PATH_PROMPT,         AUDIO_MODE_SYSTEM,  drv_rt_is_playing,          NULL,                   drv_rt_stop,NULL},
#ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL
    /*AUDIO_MP3*/   {AUDIO_PATH_MAIN_OUT,       AUDIO_MODE_SYSTEM,  drv_mp3_is_dsp_playing,     NULL,                   drv_mp3_stop,NULL},
#else
    /*AUDIO_MP3*/   {AUDIO_PATH_NON_DEFINE,     AUDIO_MODE_NONE,    NULL,                       NULL,                   NULL,       NULL},
#endif
    /*AUDIO_ANC*/   {AUDIO_PATH_BG_EFFECT,      AUDIO_MODE_SYSTEM,  drv_anc_is_playing,         NULL,                   drv_anc_stop,NULL},
#ifdef LINEIN_ENABLE
    /*AUDIO_LINEIN*/{AUDIO_PATH_MAIN_OUT_IN,    AUDIO_MODE_SYSTEM,  drv_line_in_is_playing,     NULL,                   drv_line_in_stop,NULL},
#else
    /*AUDIO_LINEIN*/{AUDIO_PATH_NON_DEFINE,     AUDIO_MODE_NONE,    NULL,                       NULL,                   NULL,       NULL},
#endif
#ifdef PROFILE_AMA_ENABLE
    /*AUDIO_AMA*/   {AUDIO_PATH_ASSISTANT,      AUDIO_MODE_LINK,    NULL,                       drv_ama_is_playing,     NULL,       drv_ama_stop},
#else
    /*AUDIO_AMA*/   {AUDIO_PATH_NON_DEFINE,     AUDIO_MODE_NONE,    NULL,                       NULL,                   NULL,       NULL},
#endif
#ifdef AIROHA_WWE_ENABLE
    /*AUDIO_VAD*/   {AUDIO_PATH_ASSISTANT,      AUDIO_MODE_SYSTEM,  drv_vad_is_playing,         NULL,                   drv_vad_stop,NULL},
#else
    /*AUDIO_VAD*/   {AUDIO_PATH_NON_DEFINE,     AUDIO_MODE_NONE,    NULL,                       NULL,                   NULL,       NULL},
#endif
#ifdef APP_OPUS_ENCODER
    /*AUDIO_ENCODER*/{AUDIO_PATH_ASSISTANT,     AUDIO_MODE_LINK,    NULL,                       drv_encoder_is_playing, NULL,       drv_encoder_stop},
#else
    /*AUDIO_ENCODER*/{AUDIO_PATH_NON_DEFINE,     AUDIO_MODE_NONE,    NULL,                       NULL,                   NULL,       NULL},
#endif
#if LAB_TUNNING_APP
    /*AUDIO_STREAM_RECORD*/{AUDIO_PATH_ASSISTANT, AUDIO_MODE_SYSTEM,drv_recorder_is_streaming,  NULL,                   drv_recorder_stop,NULL},
#else
	/*AUDIO_STREAM_RECORD*/{AUDIO_PATH_NON_DEFINE,AUDIO_MODE_NONE,  NULL,                       NULL,                   NULL,           NULL},
#endif
#ifdef XIAOAI_DEVELOPMENT
	/*AUDIO_XIAOAI*/      {AUDIO_PATH_ASSISTANT,      AUDIO_MODE_LINK,    NULL,               drv_xiaoai_is_playing, NULL, drv_xiaoai_stop},
#else
	/*AUDIO_XIAOAI*/      {AUDIO_PATH_NON_DEFINE,		AUDIO_MODE_NONE,	NULL,	NULL,	NULL,	NULL},
#endif
#ifdef AUDIO_CALIBRATION_ENABLE
    /**AUDIO_CALIBRATION*/{AUDIO_PATH_MAIN_OUT_IN,  AUDIO_MODE_SYSTEM,  drv_calibration_is_playing,     NULL,  drv_calibration_stop,    NULL},
#else
    /**AUDIO_CALIBRATION*/{AUDIO_PATH_NON_DEFINE,   AUDIO_MODE_NONE,    NULL,   NULL,   NULL,   NULL},
#endif
};

/******************************************************************************
* Static Functions
*******************************************************************************/
static U8 app_Audio_GetPathType(U8 audio_type)
{
    if(audio_type > AUDIO_NONE &&  audio_type < AUDIO_STREAM_NO)
    {
        return gAudioTable[audio_type].path_type;
    }
    return AUDIO_PATH_NON_DEFINE;
}


/******************************************************************************
* Static Functions (AUDIO_PATH_PROMPT)
*******************************************************************************/
static void app_Audio_SetPathPromptInfo(U8 audio_type)
{
    DBG_LOG_APP_DSP_CONTROL( "[APP][DSPStreamManager]Set PathPromptInfo type:%d", 1, audio_type);
    g_audio_path_ctl.prompt_ctl.audio_type = audio_type;
}

static void app_Audio_ClearPathPromptInfo(void)
{
    DBG_LOG_APP_DSP_CONTROL( "[APP][DSPStreamManager]Clear PathPromptInfo type:%d", 1, g_audio_path_ctl.prompt_ctl.audio_type);
    g_audio_path_ctl.prompt_ctl.audio_type = AUDIO_NONE;
}

static BOOL app_Audio_ClosePathPrompt(U8 audio_type)
{
    DBG_LOG_APP_DSP_CONTROL( "[APP][DSPStreamManager]ClosePathPrompt type:%d,%d", 2, audio_type, g_audio_path_ctl.prompt_ctl.audio_type);
    if(g_audio_path_ctl.prompt_ctl.audio_type != AUDIO_NONE && g_audio_path_ctl.prompt_ctl.audio_type == audio_type)
    {
        if(gAudioTable[audio_type].path_type != AUDIO_PATH_PROMPT || gAudioTable[audio_type].audio_mode != AUDIO_MODE_SYSTEM)
        {
            FW_Assert(FALSE);
        }
        if(port_OsStart)
        {
            xSemaphoreTakeRecursive(pVPRT_semaphore, portMAX_DELAY);
        }
        gVpRtInfo.isVpRtSyncEvent = FALSE;
        drv_vp_set_sync(FALSE);
        drv_rt_set_sync(FALSE);
        if(port_OsStart)
        {
            xSemaphoreGiveRecursive(pVPRT_semaphore);
        }
        APP_VpRt_ReleaseTimer();

        if(gAudioTable[audio_type].f_stop_sys)
        {
            if(gAudioTable[audio_type].f_stop_sys(FALSE))
            {
                nvkey_hdl_remove_state(NVKEY_HDL_STATE_MASK_APP_DSP_PROMPT);
                app_Audio_ClearPathPromptInfo();
<<<<<<< HEAD
=======
#ifdef AIR_PEQ_SPEAKER_ENBALE
                APP_PeqManage_LoadPeqGroup(PEQ_A2DP);
#endif
>>>>>>> db20e11 (second commit)
                return TRUE;
            }
            return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

static BOOL app_Audio_OpenPathPrompt(U8 audio_type, U8 codec)
{
    BOOL isSuccessful = FALSE;

    DBG_LOG_APP_DSP_CONTROL( "[APP][DSPStreamManager]OpenPathPrompt type:%d -> %d, codec:%d", 3, g_audio_path_ctl.prompt_ctl.audio_type, audio_type, codec);
    if(g_audio_path_ctl.prompt_ctl.audio_type == AUDIO_NONE)
    {
        if(APP_IsAbleToLoadVpRtGain())
        {
            if(audio_type == AUDIO_VP)
                APP_VolSetting_LoadVPGainTable();
            else if(audio_type == AUDIO_RT)
                APP_VolSetting_LoadRTGainTable();
        }

        APP_VolManage_SpkVolumeRemoteHandler(NULL, APP_VolManage_GetCurrentVpRtSoundLevel(), COMPONENT_VPRT);
        if(audio_type == AUDIO_VP)
        {
            isSuccessful = drv_vp_start(codec);
        }
        else if(audio_type == AUDIO_RT)
        {
            isSuccessful = drv_rt_start(codec);
        }
        else
        {
            FW_Assert(FALSE);
        }
        if(isSuccessful)
        {
            nvkey_hdl_set_state(NVKEY_HDL_STATE_MASK_APP_DSP_PROMPT);
            app_Audio_SetPathPromptInfo(audio_type);
            #if defined AIROHA_WWE_ENABLE || defined AMAZON_AMA_ENABLE || defined GOOGLE_GVA_ENABLE
            //drv_vad_notify_dsp_on(TRUE); /*only ANC needs HW-VAD. */
            #endif
        }
    }
    return isSuccessful;
}
/******************************************************************************
* Static Functions (AUDIO_PATH_ASSISTANT)
*******************************************************************************/
static void app_Audio_SetPathAssistantInfo(U8 audio_type, BD_ADDR_T *pBdAddr)
{
    g_audio_path_ctl.assistant_ctl.audio_type = audio_type;
	if(pBdAddr)
		FW_Memcpy(&g_audio_path_ctl.assistant_ctl.bdAddr.addr[0], pBdAddr, sizeof(BD_ADDR_T));

    pBdAddr = &g_audio_path_ctl.assistant_ctl.bdAddr;
    DBG_LOG_APP_DSP_CONTROL( "[APP][DSPStreamManager]Set PathAssistantInfo type:%d, BdAddr: 0x%x%x", 3, audio_type, FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE));
}

static void app_Audio_ClearPathAssistantInfo(void)
{
    BD_ADDR_T *pBdAddr = &g_audio_path_ctl.assistant_ctl.bdAddr;
    DBG_LOG_APP_DSP_CONTROL( "[APP][DSPStreamManager]Clear PathAssistantInfo type:%d, BdAddr: 0x%x%x", 3, g_audio_path_ctl.assistant_ctl.audio_type, FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE));
    FW_Memset(&g_audio_path_ctl.assistant_ctl, 0, sizeof(PATH_ASSISTANT_CTL));
}

static BOOL app_Audio_ClosePathAssistant(U8 audio_type, BD_ADDR_T *pBdAddr, BOOL is_suspend)
{
    DBG_LOG_APP_DSP_CONTROL( "[APP][DSPStreamManager]ClosePathAssistant type:%d,%d, is_suspend:%d", 3, audio_type, g_audio_path_ctl.assistant_ctl.audio_type, is_suspend);
    if(g_audio_path_ctl.assistant_ctl.audio_type != AUDIO_NONE && g_audio_path_ctl.assistant_ctl.audio_type == audio_type)
    {
        U8 audio_mode;
        if(gAudioTable[audio_type].path_type != AUDIO_PATH_ASSISTANT)
        {
            FW_Assert(FALSE);
        }

        audio_mode = gAudioTable[audio_type].audio_mode;
        if(audio_mode == AUDIO_MODE_SYSTEM)
        {
            if(gAudioTable[audio_type].f_stop_sys)
            {
                if(gAudioTable[audio_type].f_stop_sys(is_suspend))
                {
                    nvkey_hdl_remove_state(NVKEY_HDL_STATE_MASK_APP_DSP_ASSISTANT);
                    if(!is_suspend)
                        app_Audio_ClearPathAssistantInfo();
                    return TRUE;
                }
                return FALSE;
            }
        }
        else if(audio_mode == AUDIO_MODE_LINK)
        {
            if(FW_CmpBdAddr(&g_audio_path_ctl.assistant_ctl.bdAddr, pBdAddr))
            {
                if(gAudioTable[audio_type].f_stop_link)
                {
                    if(gAudioTable[audio_type].f_stop_link(&g_audio_path_ctl.assistant_ctl.bdAddr))
                    {
                        nvkey_hdl_remove_state(NVKEY_HDL_STATE_MASK_APP_DSP_ASSISTANT);
                        if(!is_suspend)
                            app_Audio_ClearPathAssistantInfo();
                        return TRUE;
                    }
                    return FALSE;
                }
            }
        }
        return TRUE;
    }
    return FALSE;
}

static BOOL app_Audio_OpenPathAssistant(U8 audio_type, BD_ADDR_T *pBdAddr, U32 bit_rate)
{
    BOOL isSuccessful = FALSE;

    DBG_LOG_APP_DSP_CONTROL( "[APP][DSPStreamManager]OpenPathAssistant type:%d , BdAddr: 0x%x%x, bit_rate:%d", 4, audio_type, FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE), bit_rate);

    switch(audio_type)
    {
        #ifdef PROFILE_AMA_ENABLE
        case AUDIO_AMA:
            APP_VolSetting_LoadVCGainTable();
            isSuccessful = drv_ama_start(pBdAddr, bit_rate);
            break;
        #endif
        #ifdef APP_OPUS_ENCODER
        case AUDIO_ENCODER:
            APP_VolSetting_LoadVCGainTable();
            isSuccessful = drv_encoder_start(pBdAddr, bit_rate);
            break;
        #endif
        #ifdef AIROHA_WWE_ENABLE
        case AUDIO_VAD:
            APP_VolSetting_LoadVCGainTable();
            isSuccessful = drv_vad_start();
        #endif

        #ifdef XIAOAI_DEVELOPMENT
        case AUDIO_XIAOAI:
            APP_VolSetting_LoadVCGainTable();
            isSuccessful = drv_xiaoai_start(pBdAddr, bit_rate);
            break;
        #endif

        #if LAB_TUNNING_APP
        case AUDIO_STREAM_RECORD:
            APP_VolSetting_LoadVCGainTable();
            isSuccessful = drv_recorder_start(bit_rate /*pParameter->recordPara.mode*/);
            break;
        #endif
        default:
            break;
    }

    if(isSuccessful)
    {
        nvkey_hdl_set_state(NVKEY_HDL_STATE_MASK_APP_DSP_ASSISTANT);
        app_Audio_SetPathAssistantInfo(audio_type, pBdAddr);
        g_audio_path_ctl.assistant_ctl.bit_rate = bit_rate;
    }
    return isSuccessful;
}

static void app_Audio_ResumePathAssistant(void)
{
    U8 audio_type;

    DBG_LOG_APP_DSP_CONTROL( "[APP][DSPStreamManager]ResumePathAssistant type:%d, is_suspend:%d", 2, g_audio_path_ctl.assistant_ctl.audio_type, gAudioDspCtl.isSuspended);
    if(g_audio_path_ctl.assistant_ctl.audio_type == AUDIO_NONE || gAudioDspCtl.isSuspended)
    {
        return;
    }
    audio_type = g_audio_path_ctl.assistant_ctl.audio_type;
    if(gAudioTable[audio_type].path_type != AUDIO_PATH_ASSISTANT)
    {
        FW_Assert(FALSE);
        return;
    }

    if(!app_AudioDsp_PlayCheck(audio_type, TRUE))
        return;

    if(APP_AudioDspIsStreaming(&g_audio_path_ctl.assistant_ctl.bdAddr, audio_type))
        return;

    app_Audio_OpenPathAssistant(audio_type, &g_audio_path_ctl.assistant_ctl.bdAddr, g_audio_path_ctl.assistant_ctl.bit_rate);
}


/******************************************************************************
* Static Functions (AUDIO_PATH_BG_EFFECT)
*******************************************************************************/
static void app_Audio_SetPathBgEffectInfo(U8 audio_type)
{
    g_audio_path_ctl.bg_effect_ctl.audio_type = audio_type;
    DBG_LOG_APP_DSP_CONTROL( "[APP][DSPStreamManager]Set PathBgEffectInfo type:%d", 1, audio_type);
}

static void app_Audio_ClearPathBgEffectInfo(void)
{
    DBG_LOG_APP_DSP_CONTROL( "[APP][DSPStreamManager]Clear PathBgEffectInfo type:%d", 1, g_audio_path_ctl.bg_effect_ctl.audio_type);
    FW_Memset(&g_audio_path_ctl.bg_effect_ctl, 0, sizeof(PATH_BG_EFFECT_CTL));
}

static BOOL app_Audio_ClosePathBgEffect(U8 audio_type, BOOL is_suspend)
{
    BOOL isSuccessful = FALSE;
    DBG_LOG_APP_DSP_CONTROL( "[APP][DSPStreamManager]ClosePathBgEffect type:%d,%d, is_suspend:%d", 3, audio_type, g_audio_path_ctl.bg_effect_ctl.audio_type, is_suspend);

    if(g_audio_path_ctl.bg_effect_ctl.audio_type != AUDIO_NONE && g_audio_path_ctl.bg_effect_ctl.audio_type == audio_type)
    {
        if(gAudioTable[audio_type].path_type != AUDIO_PATH_BG_EFFECT && gAudioTable[audio_type].audio_mode != AUDIO_MODE_SYSTEM)
        {
            FW_Assert(FALSE);
        }

        if(gAudioTable[audio_type].f_stop_sys)
        {
            if(gAudioTable[audio_type].f_stop_sys(is_suspend))
            {
                isSuccessful = TRUE;
                if(!is_suspend)
                    app_Audio_ClearPathBgEffectInfo();
            }
        }
        else
        {
            isSuccessful = TRUE;
        }
    }
    if(isSuccessful)
    {
        APP_PowerOff_SetAutoSwitchOffBit(AUTO_SWITCH_OFF_BIT_ANCPASSTHRU, FALSE, APP_GetServiceBdAddr());
    }
    return isSuccessful;
}

static BOOL app_Audio_OpenPathBgEffect(U8 audio_type)
{
    BOOL isSuccessful = FALSE;

    DBG_LOG_APP_DSP_CONTROL( "[APP][DSPStreamManager]OpenPathBgEffect type:%d", 1, audio_type);

    switch(audio_type)
    {
        case AUDIO_ANC:
            isSuccessful = drv_anc_start();
<<<<<<< HEAD
=======
            DBG_LOG_APP_DSP_CONTROL( "[APP][DSPStreamManager] drv_anc_start()",0);
>>>>>>> db20e11 (second commit)
            if(isSuccessful)
            {
                APP_PowerOff_SetAutoSwitchOffBit(AUTO_SWITCH_OFF_BIT_ANCPASSTHRU, app_airothru_read_cancel_idle_timer_config(), APP_GetServiceBdAddr());
            }
            break;
        case AUDIO_ADDA:
            isSuccessful = drv_sidetone_start();
            break;
    }

    if(isSuccessful)
    {
        app_Audio_SetPathBgEffectInfo(audio_type);
        #if defined AIROHA_WWE_ENABLE || defined AMAZON_AMA_ENABLE || defined GOOGLE_GVA_ENABLE
        if(audio_type == AUDIO_ANC)
            drv_vad_notify_dsp_on(TRUE);
        #endif
    }

    return isSuccessful;
}

static void app_Audio_ResumePathBgEffect(void)
{
    U8 audio_type;

    DBG_LOG_APP_DSP_CONTROL( "[APP][DSPStreamManager]ResumePathBgEffect type:%d, is_suspend:%d", 2, g_audio_path_ctl.bg_effect_ctl.audio_type, gAudioDspCtl.isSuspended);
    if(g_audio_path_ctl.bg_effect_ctl.audio_type == AUDIO_NONE || gAudioDspCtl.isSuspended)
    {
        return;
    }
    audio_type = g_audio_path_ctl.bg_effect_ctl.audio_type;
    if(gAudioTable[audio_type].path_type != AUDIO_PATH_BG_EFFECT)
    {
        FW_Assert(FALSE);
        return;
    }

    if(!app_AudioDsp_PlayCheck(audio_type, TRUE))
        return;

    if(APP_AudioDspIsStreaming(NULL, audio_type))
        return;

    app_Audio_OpenPathBgEffect(audio_type);
}


/******************************************************************************
* Static Functions (AUDIO_PATH_MAIN_OUT | AUDIO_PATH_MAIN_OUT_IN)
*******************************************************************************/
static void app_Audio_SetPathMainInfo(BD_ADDR_T *pBdAddr, U8 audioType, U8 codec)
{
	g_audio_path_ctl.main_ctl.codec = codec;
	g_audio_path_ctl.main_ctl.audio_type = audioType;
	if(pBdAddr)
		FW_Memcpy(&g_audio_path_ctl.main_ctl.bdAddr.addr[0], pBdAddr, sizeof(BD_ADDR_T));

    pBdAddr = &g_audio_path_ctl.main_ctl.bdAddr;
    DBG_LOG_APP_DSP_CONTROL( "[APP][DSPStreamManager]Set PathMainInfo type:%d codec:%d, BdAddr: 0x%x%x", 4, audioType, codec, FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE));
}

static void app_Audio_ClearPathMainInfo(void)
{
    BD_ADDR_T *pBdAddr = &g_audio_path_ctl.main_ctl.bdAddr;
    DBG_LOG_APP_DSP_CONTROL( "[APP][DSPStreamManager]Clear PathMainInfo type:%d codec:%d, BdAddr: 0x%x%x", 4,
                                                                g_audio_path_ctl.main_ctl.audio_type,
                                                                g_audio_path_ctl.main_ctl.codec,
                                                                FW_bdaddr_to_2U32(pBdAddr, TRUE),
                                                                FW_bdaddr_to_2U32(pBdAddr, FALSE));
	FW_Memset(&g_audio_path_ctl.main_ctl, 0, sizeof(PATH_MAIN_CTL));
}

static BOOL app_Audio_CloseMainPath(U8 audio_type, BD_ADDR_T *pBdAddr, BOOL is_suspend)
{
    DBG_LOG_APP_DSP_CONTROL( "[APP][DSPStreamManager]CloseMainPath type:%d,%d, is_suspend:%d, BdAddr: 0x%x%x", 5,
                                                                audio_type,
                                                                g_audio_path_ctl.main_ctl.audio_type,
                                                                is_suspend,
                                                                FW_bdaddr_to_2U32(pBdAddr, TRUE),
                                                                FW_bdaddr_to_2U32(pBdAddr, FALSE));
    if(g_audio_path_ctl.main_ctl.audio_type != AUDIO_NONE && g_audio_path_ctl.main_ctl.audio_type == audio_type)
    {
        U8 audio_mode;
        if(gAudioTable[audio_type].path_type != AUDIO_PATH_MAIN_OUT && gAudioTable[audio_type].path_type != AUDIO_PATH_MAIN_OUT_IN)
        {
            FW_Assert(FALSE);
        }

        audio_mode = gAudioTable[audio_type].audio_mode;
        if(audio_mode == AUDIO_MODE_SYSTEM)
        {
            if(gAudioTable[audio_type].f_stop_sys)
            {
                if(gAudioTable[audio_type].f_stop_sys(is_suspend))
                {
                    nvkey_hdl_remove_state(NVKEY_HDL_STATE_MASK_APP_DSP_MAIN_PATH);
                    if(!is_suspend)
                        app_Audio_ClearPathMainInfo();
                    return TRUE;
                }
                return FALSE;
            }
        }
        else if(audio_mode == AUDIO_MODE_LINK)
        {
            if(FW_CmpBdAddr(&g_audio_path_ctl.main_ctl.bdAddr, pBdAddr))
            {
                if(gAudioTable[audio_type].f_stop_link)
                {
                    if(gAudioTable[audio_type].f_stop_link(&g_audio_path_ctl.main_ctl.bdAddr))
                    {
                        nvkey_hdl_remove_state(NVKEY_HDL_STATE_MASK_APP_DSP_MAIN_PATH);
                        if(audio_type == AUDIO_A2DP)
                        {
                            APP_Customer_MusicPlayingLed_Resume();
                            APP_A2dp_NotifyDSPStatus(&g_audio_path_ctl.main_ctl.bdAddr, FALSE);
                            APP_SetEsdInfo(FALSE, &g_audio_path_ctl.main_ctl.bdAddr);
                        }
                        if(audio_type == AUDIO_SCO)
                        {
                            APP_HfpSco_DeRegisterStopNotify();
                        }
                        if(!is_suspend)
                            app_Audio_ClearPathMainInfo();
                        return TRUE;
                    }
                    return FALSE;
                }
            }
        }
        return TRUE;
    }
    return FALSE;
}

static BOOL app_Audio_OpenMainPath(U8 audio_type, BD_ADDR_T *pBdAddr, U8 codec, U8 scms, U8 sampleRateIndex)
{
    BOOL isSuccessful = FALSE;

    DBG_LOG_APP_DSP_CONTROL( "[APP][DSPStreamManager]OpenMainPath type:%d, bdaddr:0x%x%x", 3, audio_type, FW_bdaddr_to_2U32(pBdAddr,TRUE),FW_bdaddr_to_2U32(pBdAddr,FALSE));

    switch(audio_type)
    {
        case AUDIO_SCO:
            if(APP_Hfp_IsCodecWideBand(codec))
            {
                APP_VolSetting_LoadScoWBGainTable();
                APP_VolSetting_SetSoundlevelToDSP(APP_VolManage_GetCurrentScoSoundLevel(pBdAddr), COMPONENT_SCO_WB);
            }
            else
            {
                APP_VolSetting_LoadScoNBGainTable();
                APP_VolSetting_SetSoundlevelToDSP(APP_VolManage_GetCurrentScoSoundLevel(pBdAddr), COMPONENT_SCO_NB);
            }

            APP_VolSetting_SetSoundlevelToDSP(APP_VolManage_GetCurrentScoMicSoundLevel(pBdAddr), COMPONENT_MIC_SCO);
            isSuccessful = drv_sco_start(pBdAddr, codec, APP_GetSCOHandleByBdAddr(pBdAddr), BtAwsMce_GetDefaultRole());

            if(isSuccessful)
            {
                APP_HfpSco_DeRegisterStopNotify();
                nvkey_hdl_set_state(NVKEY_HDL_STATE_MASK_APP_DSP_MAIN_PATH);
            }
            break;
        case AUDIO_A2DP:
            APP_VolSetting_LoadA2dpGainTable();
            APP_VolSetting_SetSoundlevelToDSP(APP_VolManage_GetCurrentA2dpSoundLevel(pBdAddr), COMPONENT_A2DP);

            isSuccessful = drv_a2dp_start(pBdAddr, codec, scms, sampleRateIndex);
            if(isSuccessful)
            {
                APP_A2dp_NotifyDSPStatus(pBdAddr, TRUE);
                APP_Customer_MusicPlayingLed_Set();
                nvkey_hdl_set_state(NVKEY_HDL_STATE_MASK_APP_DSP_MAIN_PATH);
                APP_PeqManage_LoadPeqGroup(PEQ_A2DP);
                APP_SetEsdInfo(TRUE, pBdAddr);
            }
            break;
        #ifdef LINEIN_ENABLE
        case AUDIO_LINEIN:
            APP_VolSetting_LoadLineinGainTable();
            APP_VolManage_SpkVolumeRemoteHandler(APP_EOF, APP_VolManage_GetCurrentLineInSoundLevel(), COMPONENT_LINEIN);

            isSuccessful = drv_line_in_start();
            if(isSuccessful)
            {
                nvkey_hdl_set_state(NVKEY_HDL_STATE_MASK_APP_DSP_MAIN_PATH);
                APP_PeqManage_LoadPeqGroup(PEQ_LINE_IN);
            }
            break;
        #endif
        #ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL
        case AUDIO_MP3:
            APP_VolSetting_LoadA2dpGainTable();
            APP_VolManage_SpkVolumeRemoteHandler(APP_EOF, APP_VolManage_GetCurrentMp3SoundLevel(), COMPONENT_MP3);

            isSuccessful = drv_mp3_start();
            if(isSuccessful)
            {
                nvkey_hdl_set_state(NVKEY_HDL_STATE_MASK_APP_DSP_MAIN_PATH);
                APP_PeqManage_LoadPeqGroup(PEQ_MP3);
            }
            break;
        #endif //MP3_LOCAL_PLAYBACK_MMI_CTRL
    }

    if(isSuccessful)
    {
        app_Audio_SetPathMainInfo(pBdAddr, audio_type, codec);
        if(audio_type == AUDIO_A2DP)
        {
            g_audio_path_ctl.main_ctl.scms = scms;
            g_audio_path_ctl.main_ctl.sampleRateIndex = sampleRateIndex;
        }
        #if defined AIROHA_WWE_ENABLE || defined AMAZON_AMA_ENABLE || defined GOOGLE_GVA_ENABLE
        //drv_vad_notify_dsp_on(TRUE); /*only ANC needs HW-VAD. */
        #endif
    }
    return isSuccessful;
}

static void app_Audio_ResumeMainPath(void)
{
    U8 audio_type;
    DBG_LOG_APP_DSP_CONTROL( "[APP][DSPStreamManager]ResumeMainPath type:%d, is_suspend:%d", 2, g_audio_path_ctl.main_ctl.audio_type, gAudioDspCtl.isSuspended);
    if(g_audio_path_ctl.main_ctl.audio_type == AUDIO_NONE || gAudioDspCtl.isSuspended)
    {
        return;
    }
    audio_type = g_audio_path_ctl.main_ctl.audio_type;
    if(gAudioTable[audio_type].path_type != AUDIO_PATH_MAIN_OUT && gAudioTable[audio_type].path_type != AUDIO_PATH_MAIN_OUT_IN)
    {
        FW_Assert(FALSE);
    }

    if(!app_AudioDsp_PlayCheck(audio_type, TRUE))
        return;

    if(APP_AudioDspIsStreaming(&g_audio_path_ctl.main_ctl.bdAddr, audio_type))
        return;

    app_Audio_OpenMainPath(audio_type, &g_audio_path_ctl.main_ctl.bdAddr, g_audio_path_ctl.main_ctl.codec, g_audio_path_ctl.main_ctl.scms, g_audio_path_ctl.main_ctl.sampleRateIndex);
}

static void app_AudioCloseAll(U8 audioType)
{
    BOOL is_suspend = (audioType == AUDIO_SUSPEND) ? TRUE : FALSE;

    DBG_LOG_APP_DSP_CONTROL( "[APP][DSPStreamManager]AudioCloseAll: is_suspend:%d", 1, is_suspend);
    app_Audio_CloseMainPath(g_audio_path_ctl.main_ctl.audio_type, &g_audio_path_ctl.main_ctl.bdAddr, is_suspend);
    app_Audio_ClosePathPrompt(g_audio_path_ctl.prompt_ctl.audio_type);
    app_Audio_ClosePathAssistant(g_audio_path_ctl.assistant_ctl.audio_type, &g_audio_path_ctl.assistant_ctl.bdAddr, is_suspend);
    app_Audio_ClosePathBgEffect(g_audio_path_ctl.bg_effect_ctl.audio_type, is_suspend);
    drv_sidetone_stop(is_suspend);

#if defined AIROHA_WWE_ENABLE || defined AMAZON_AMA_ENABLE || defined GOOGLE_GVA_ENABLE
    drv_vad_notify_dsp_busy(FALSE);
    drv_vad_notify_dsp_on(FALSE);
#endif
}

static BOOL app_AudioDsp_PlayCheck(U8 audioType, BOOL is_resume)
{
    BOOL isSuccessful = TRUE;
    DBG_LOG_APP_DSP_CONTROL( "[APP][DSPStreamManager]PlayCheck: audioType:%d, is_resume:%d", 2, audioType, is_resume);
    switch(app_Audio_GetPathType(audioType))
    {
        case AUDIO_PATH_MAIN_OUT_IN:
            app_Audio_ClosePathAssistant(g_audio_path_ctl.assistant_ctl.audio_type, &g_audio_path_ctl.assistant_ctl.bdAddr, TRUE);
<<<<<<< HEAD
            #ifdef MTK_INEAR_ENHANCEMENT_ENABLE
            app_Audio_ClosePathBgEffect(g_audio_path_ctl.bg_effect_ctl.audio_type, TRUE);
            #elif defined MTK_DUALMIC_INEAR
=======
            #ifdef AIR_TXNR_1_MIC_INEAR_ENABLE
            app_Audio_ClosePathBgEffect(g_audio_path_ctl.bg_effect_ctl.audio_type, TRUE);
            #elif defined AIR_TXNR_2_MIC_INEAR_ENABLE
>>>>>>> db20e11 (second commit)
            if(g_audio_path_ctl.bg_effect_ctl.audio_type == AUDIO_ANC)
            {
                if(USER_PASSTHRU_OPEN == AudioDsp_GetANCPassThruRegister())
                {
                    app_Audio_ClosePathBgEffect(g_audio_path_ctl.bg_effect_ctl.audio_type, TRUE);
                }
            }
            #endif
            /* fallthrough */
        case AUDIO_PATH_MAIN_OUT:
            if(audioType != g_audio_path_ctl.main_ctl.audio_type && !is_resume)
                app_Audio_CloseMainPath(g_audio_path_ctl.main_ctl.audio_type, &g_audio_path_ctl.main_ctl.bdAddr, FALSE);
            #if defined AIROHA_WWE_ENABLE || defined AMAZON_AMA_ENABLE || defined GOOGLE_GVA_ENABLE
            drv_vad_notify_dsp_busy(TRUE);
            #endif
            break;

        case AUDIO_PATH_ASSISTANT:
            if(AUDIO_PATH_MAIN_OUT_IN == app_Audio_GetPathType(g_audio_path_ctl.main_ctl.audio_type))
            {
                isSuccessful = FALSE;
                break;
            }
            if(g_audio_path_ctl.assistant_ctl.audio_type != AUDIO_NONE && !is_resume)
            {
                isSuccessful = FALSE;
                break;
            }
            break;

        case AUDIO_PATH_BG_EFFECT:
<<<<<<< HEAD
            #ifdef MTK_INEAR_ENHANCEMENT_ENABLE
=======
            #ifdef AIR_TXNR_1_MIC_INEAR_ENABLE
>>>>>>> db20e11 (second commit)
            if(AUDIO_PATH_MAIN_OUT_IN == app_Audio_GetPathType(g_audio_path_ctl.main_ctl.audio_type))
            {
                isSuccessful = FALSE;
                break;
            }
<<<<<<< HEAD
            #elif defined MTK_DUALMIC_INEAR
=======
            #elif defined AIR_TXNR_2_MIC_INEAR_ENABLE
>>>>>>> db20e11 (second commit)
            if(AUDIO_PATH_MAIN_OUT_IN == app_Audio_GetPathType(g_audio_path_ctl.main_ctl.audio_type))
            {
                if(USER_PASSTHRU_OPEN == AudioDsp_GetANCPassThruRegister())
                {
                    isSuccessful = FALSE;
                    break;
                }
            }
            #endif
            if(drv_sidetone_is_playing())
            {
                isSuccessful = FALSE;
                break;
            }
            if(g_audio_path_ctl.bg_effect_ctl.audio_type != AUDIO_NONE && !is_resume)
            {
                if(audioType != AUDIO_ANC || g_audio_path_ctl.bg_effect_ctl.audio_type != AUDIO_ANC)
                    isSuccessful = FALSE;
                break;
            }
            break;
        case AUDIO_PATH_PROMPT:
            if(g_audio_path_ctl.prompt_ctl.audio_type != AUDIO_NONE && !is_resume)
            {
                isSuccessful = FALSE;
                break;
            }
            break;
        default:
            break;
    }
    return isSuccessful;
}

static void app_AudioDsp_PlayFailRecover(U8 audioType)
{
    switch(app_Audio_GetPathType(audioType))
    {
        case AUDIO_PATH_MAIN_OUT_IN:
            #if defined AIROHA_WWE_ENABLE || defined AMAZON_AMA_ENABLE || defined GOOGLE_GVA_ENABLE
            drv_vad_notify_dsp_busy(FALSE);
            #endif
            app_Audio_ResumePathAssistant();
<<<<<<< HEAD
            #ifdef MTK_INEAR_ENHANCEMENT_ENABLE
            app_Audio_ResumePathBgEffect();
            #elif defined MTK_DUALMIC_INEAR
=======
            #ifdef AIR_TXNR_1_MIC_INEAR_ENABLE
            app_Audio_ResumePathBgEffect();
            #elif defined AIR_TXNR_2_MIC_INEAR_ENABLE
>>>>>>> db20e11 (second commit)
            if(g_audio_path_ctl.bg_effect_ctl.audio_type == AUDIO_ANC)
            {
                if(USER_PASSTHRU_OPEN == AudioDsp_GetANCPassThruRegister())
                {
                    app_Audio_ResumePathBgEffect();
                }
            }
            #endif
            /* fallthrough */
        case AUDIO_PATH_MAIN_OUT:
            #if defined AIROHA_WWE_ENABLE || defined AMAZON_AMA_ENABLE || defined GOOGLE_GVA_ENABLE
            drv_vad_notify_dsp_busy(FALSE);
            #endif
            break;
        default:

            break;
    }
}

#ifdef AIROHA_WWE_ENABLE
static void app_VAD_WakeEvtAppleSiri(void)
{
	BOOL isAGSiriOn, isAndroidOn;
	BD_ADDR_T *pBdAddr;

	isAGSiriOn = APP_Hfp_IsAGSiriSupported();
	isAndroidOn = drv_audio_nvkey_is_feature_on(AUDIO_DSP_VAD_ANDROID_ON_FEATURE);

	DBG_LOG_APP_DSP_CONTROL( "[APP_VAD] Feature, AGSiriOn:%d, AndroidOn:%d", 2, isAGSiriOn, isAndroidOn);

	if(isAGSiriOn || isAndroidOn)
	{
		pBdAddr = APP_GetServiceBdAddr();
		APP_AudioDeviceRegister(pBdAddr, AUDIO_DEVICE_KEY_AT_CMD);
		APP_HFP_SendATCmdByLink(pBdAddr, HFP_ATCMD_INIT_VOICE_DIAL);
	}
}
#endif

static U32 app_audioDsp_handler(Handler handler, U16 id, void *msg, U32 handler_id)
{
    AUDIO_DSP_STOP_PARA_STUR audioDspPara;
    UNUSED(handler);
    UNUSED(msg);
    UNUSED(handler_id);

    switch(id)
    {
        case DRV_RT_ID_REQUEST_CLOSE:
            FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_STOP_PARA_STUR));
            audioDspPara.audioType = AUDIO_RT;
            APP_AudioDspStop(&audioDspPara);
            break;
        case DRV_VP_ID_REQUEST_CLOSE:
            FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_STOP_PARA_STUR));
            audioDspPara.audioType = AUDIO_VP;
            APP_AudioDspStop(&audioDspPara);
            break;
    #ifdef AIROHA_WWE_ENABLE
        case DRV_VAD_ID_WAKE_UP:
            app_VAD_WakeEvtAppleSiri();
            break;
    #endif
    }
    return 0;
}

static BOOL audiodsp_scoStop_Prepare(BD_ADDR_T *pBdAddr)
{

	DBG_LOG_APP_DSP_CONTROL("[APP][DSPStreamManager]SCO Stop_Prepare - bdaddr : 0x%x%x", 2, FW_bdaddr_to_2U32(pBdAddr,TRUE),FW_bdaddr_to_2U32(pBdAddr,FALSE));

	if(pBdAddr == NULL || drv_sco_is_playing(pBdAddr))
	{
        DBG_LOG_APP_DSP_CONTROL("[APP][DSPStreamManager]SCO Stop_Prepare - result:0",0);
		APP_HfpSco_RegisterStopNotify();
		return FALSE;
	}
	else if(!drv_sco_is_sco_on())
    {
        DBG_LOG_APP_DSP_CONTROL("[APP][DSPStreamManager]SCO Stop_Prepare - result:1",0);
		return TRUE;
    }

	return TRUE;
}

/******************************************************************************
 * Public Functions
*******************************************************************************/
void APP_AudioDspInit()
{
	drv_rt_init((Handler)&gAppAudioDspHandle);
    drv_vp_init((Handler)&gAppAudioDspHandle, 0);

    APP_VpInit(); // drv_vp_init should be prior to VP language

    #if defined AIROHA_WWE_ENABLE || defined AMAZON_AMA_ENABLE || defined GOOGLE_GVA_ENABLE
    drv_vad_init((Handler)&gAppAudioDspHandle);
    #endif
    drv_a2dp_init();
    #ifdef APP_OPUS_ENCODER
    APP_Encoder_Init();
    #endif
}

/******************************************************************************
 * Public Functions (Stream)
*******************************************************************************/
BOOL APP_AudioDspPlay(AUDIO_DSP_START_PARA_STUR *pParameter)
{
	BOOL isSuccessful = FALSE;
	BD_ADDR_T *pBdAddr = &pParameter->bdAddr;
	U8 audioType = pParameter->audioType;
	U8 codec = pParameter->codec;
    U8 path_type = app_Audio_GetPathType(audioType);

	if(!gAudioDspCtl.isSuspended)
	{
        isSuccessful = app_AudioDsp_PlayCheck(audioType, FALSE);
        if(!isSuccessful)
            goto DSP_PLAY_FAIL;

        switch(path_type)
        {
            case AUDIO_PATH_PROMPT:
                isSuccessful = app_Audio_OpenPathPrompt(audioType, codec);
                break;
            case AUDIO_PATH_ASSISTANT:
                isSuccessful = app_Audio_OpenPathAssistant(audioType, pBdAddr, (audioType != LAB_TUNNING_APP) ? pParameter->bitRate : pParameter->recordPara.mode);
                break;
            case AUDIO_PATH_BG_EFFECT:
                isSuccessful = app_Audio_OpenPathBgEffect(audioType);
                break;
            case AUDIO_PATH_MAIN_OUT:
            case AUDIO_PATH_MAIN_OUT_IN:
                isSuccessful = app_Audio_OpenMainPath(audioType, pBdAddr, codec, pParameter->a2dpPara.scms, pParameter->a2dpPara.sampleRateIndex);
                break;
        }
	}

    if(!isSuccessful)
    {
        app_AudioDsp_PlayFailRecover(audioType);
    }

    DSP_PLAY_FAIL:
	DBG_LOG_APP_DSP_CONTROL( "[APP][DSPStreamManager] Start [%s] Result:%d. BdAddr: 0x%x%x", 4, APP_AudioTypeLogString[audioType], isSuccessful, FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE));
    if(!isSuccessful)
    {
        if(AUDIO_PATH_ASSISTANT == path_type)
        {
            if(g_audio_path_ctl.assistant_ctl.audio_type == AUDIO_NONE)
            {
                app_Audio_SetPathAssistantInfo(audioType, pBdAddr);
                g_audio_path_ctl.assistant_ctl.bit_rate = pParameter->bitRate;
            }
        }
        if(AUDIO_PATH_BG_EFFECT == path_type)
        {
            if(g_audio_path_ctl.bg_effect_ctl.audio_type == AUDIO_NONE)
            {
                app_Audio_SetPathBgEffectInfo(audioType);
            }
        }

    }
	return isSuccessful;
}

BOOL APP_AudioDspStop(AUDIO_DSP_STOP_PARA_STUR *pParameter)
{
	BOOL isSuccessful = FALSE;
    BOOL isResume = FALSE;
	BD_ADDR_T *pBdAddr = &pParameter->bdAddr;
	U8 audioType = pParameter->audioType;

    if(audioType == AUDIO_SCO)
    {
        if(!audiodsp_scoStop_Prepare(pBdAddr))
        {
            FW_Memcpy(&g_audio_path_ctl.main_ctl.bdAddr.addr[0], pBdAddr, sizeof(BD_ADDR_T));
            return TRUE;
        }
    }
    else if(audioType == AUDIO_SCO_DISC)
    {
        audioType = AUDIO_SCO;
        pBdAddr = &g_audio_path_ctl.main_ctl.bdAddr;
    }

    if(audioType==AUDIO_ANCPASSTHRU_SUSPEND)
    {
        audioType = AUDIO_ANC;
        isResume = TRUE;
    }

    if(audioType == AUDIO_ADDA)
    {
        isSuccessful = drv_sidetone_stop(FALSE);
    }
    else
    {
        switch(app_Audio_GetPathType(audioType))
        {
            case AUDIO_PATH_MAIN_OUT:
            case AUDIO_PATH_MAIN_OUT_IN:
                isSuccessful = app_Audio_CloseMainPath(audioType, pBdAddr, isResume);
                break;
            case AUDIO_PATH_PROMPT:
                isSuccessful = app_Audio_ClosePathPrompt(audioType);
                break;
            case AUDIO_PATH_ASSISTANT:
                isSuccessful = app_Audio_ClosePathAssistant(audioType, pBdAddr, isResume);
                break;
            case AUDIO_PATH_BG_EFFECT:
                isSuccessful = app_Audio_ClosePathBgEffect(audioType, isResume);
                break;
            default:
                break;
        }
    }

    if(isSuccessful)
    {
        if(g_audio_path_ctl.main_ctl.audio_type == AUDIO_NONE && g_audio_path_ctl.prompt_ctl.audio_type == AUDIO_NONE && g_audio_path_ctl.bg_effect_ctl.audio_type == AUDIO_NONE)
        {
            #if defined AIROHA_WWE_ENABLE || defined AMAZON_AMA_ENABLE || defined GOOGLE_GVA_ENABLE
            drv_vad_notify_dsp_on(FALSE);
            #endif
        }

        switch(app_Audio_GetPathType(audioType))
        {
            case AUDIO_PATH_MAIN_OUT_IN:
                #if defined AIROHA_WWE_ENABLE || defined AMAZON_AMA_ENABLE || defined GOOGLE_GVA_ENABLE
                drv_vad_notify_dsp_busy(FALSE);
                #endif
                app_Audio_ResumePathAssistant();
<<<<<<< HEAD
                #ifdef MTK_INEAR_ENHANCEMENT_ENABLE
                app_Audio_ResumePathBgEffect();
                #elif defined MTK_DUALMIC_INEAR
=======
                #ifdef AIR_TXNR_1_MIC_INEAR_ENABLE
                app_Audio_ResumePathBgEffect();
                #elif defined AIR_TXNR_2_MIC_INEAR_ENABLE
>>>>>>> db20e11 (second commit)
                if(g_audio_path_ctl.bg_effect_ctl.audio_type == AUDIO_ANC)
                {
                    if(USER_PASSTHRU_OPEN == AudioDsp_GetANCPassThruRegister())
                    {
                        app_Audio_ResumePathBgEffect();
                    }
                }
                #endif
                /* fallthrough */
            case AUDIO_PATH_MAIN_OUT:
                //if(app_IsAudioDSPInfoTheSame(pBdAddr, audioType))
                //    app_Audio_ClearPathMainInfo();
                #if defined AIROHA_WWE_ENABLE || defined AMAZON_AMA_ENABLE || defined GOOGLE_GVA_ENABLE
                drv_vad_notify_dsp_busy(FALSE);
                #endif
                break;
            case AUDIO_PATH_PROMPT:
                // do in close
                break;
            case AUDIO_PATH_ASSISTANT:
                // do in close
                break;
            case AUDIO_PATH_BG_EFFECT:
                #if defined AIROHA_WWE_ENABLE || defined AMAZON_AMA_ENABLE || defined GOOGLE_GVA_ENABLE
                drv_vad_notify_dsp_on(FALSE);  /*only ANC needs HW-VAD. */
                #endif
                break;
            default:

                break;
        }
    }

	DBG_LOG_APP_DSP_CONTROL("[APP][DSPStreamManager] Stop [%s] Result:%d. BdAddr: 0x%x%x", 4, APP_AudioTypeLogString[audioType], isSuccessful, FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE));
	return isSuccessful;
}


BOOL APP_AudioDspIsStreaming(BD_ADDR_T *pBdAddr, U8 audioType)
{
    if(audioType > AUDIO_NONE &&  audioType < AUDIO_STREAM_NO)
    {
        U8 audio_mode = gAudioTable[audioType].audio_mode;
        if(audio_mode == AUDIO_MODE_SYSTEM)
        {
            if(gAudioTable[audioType].f_is_playing_sys)
                return gAudioTable[audioType].f_is_playing_sys();
        }
        else if(audio_mode == AUDIO_MODE_LINK)
        {
            if(gAudioTable[audioType].f_is_playing_link)
                return gAudioTable[audioType].f_is_playing_link(pBdAddr);
        }
    }
    return FALSE;
}

BOOL APP_AudioDspInMainPathStreaming(void)
{
	return (g_audio_path_ctl.main_ctl.audio_type != AUDIO_NONE)?TRUE:FALSE;
}

U8 APP_AudioDspGetMainPath(void)
{
    return g_audio_path_ctl.main_ctl.audio_type;
}

void APP_AudioDspClose(void)
{
	DBG_LOG_APP_DSP_CONTROL( "[APP][DSPStreamManager] FW_SUSPEND_DSP", 0);
	app_AudioCloseAll(AUDIO_CLOSE);
}

void APP_AudioDSPRealtimeSuspend(void)
{
	DBG_LOG_APP_DSP_CONTROL( "[APP][DSPStreamManager] BT_APP_DSP_REALTIME_SUSPEND_DSP", 0);

    gAudioDspCtl.isSuspended = TRUE;
	app_AudioCloseAll(AUDIO_SUSPEND);
}

void APP_AudioDSPRealtimeResume(void)
{
	DBG_LOG_APP_DSP_CONTROL( "[APP][DSPStreamManager] BT_APP_DSP_REALTIME_RESUME_DSP", 0);

	gAudioDspCtl.isSuspended = FALSE;

    app_Audio_ResumeMainPath();
    app_Audio_ResumePathAssistant();
    app_Audio_ResumePathBgEffect();
}

BOOL APP_IsAbleToLoadVpRtGain(void)
{
	BD_ADDR_T *pBdAddr = (BD_ADDR_T *)APP_GetServiceBdAddr();

	return (!APP_AudioDspIsStreaming(pBdAddr, AUDIO_SCO) &&
			!APP_AudioDspIsStreaming(pBdAddr, AUDIO_A2DP) &&
			!APP_AudioDspIsStreaming(pBdAddr, AUDIO_MP3));
}

void APP_ReleaseDspSuspend(void)
{
	gAudioDspCtl.isSuspended = FALSE;
}

BOOL APP_AudioIsDspSuspend(void)
{
	return gAudioDspCtl.isSuspended;
}

BOOL APP_AudioIsPermittedToPlay(U8 audio_type)
{
    if(audio_type == AUDIO_ANC)
    {
<<<<<<< HEAD
        #ifdef MTK_INEAR_ENHANCEMENT_ENABLE
=======
        #ifdef AIR_TXNR_1_MIC_INEAR_ENABLE
>>>>>>> db20e11 (second commit)
        if(AUDIO_PATH_MAIN_OUT_IN == app_Audio_GetPathType(g_audio_path_ctl.main_ctl.audio_type))
        {
            return FALSE;
        }
<<<<<<< HEAD
        #elif defined MTK_DUALMIC_INEAR
=======
        #elif defined AIR_TXNR_2_MIC_INEAR_ENABLE
>>>>>>> db20e11 (second commit)
        if(AUDIO_PATH_MAIN_OUT_IN == app_Audio_GetPathType(g_audio_path_ctl.main_ctl.audio_type))
        {
            if(USER_PASSTHRU_OPEN == AudioDsp_GetANCPassThruRegister())
            {
                return FALSE;
            }
        }
        #endif
        if(drv_sidetone_is_playing())
        {
            return FALSE;
        }
    }

    return TRUE;
}


BOOL APP_AudioDSP_FillRhoData(void *pData)
{
	APP_RHO_STREAM_MANAGE_STRU *pDataToFill = &((APP_RHO_DATA_STRU *)pData)->appStreamManage;

	if(!pDataToFill)
	{
		DBG_LOG_APP_DSP_CONTROL( "[RHO_APP_Stream] Old Agent fill data error", 0);
		return FALSE;
	}

    pDataToFill->appVP.isMiscEnabled = APP_VpGetMiscEnabled();
    pDataToFill->appSideTone.isSideToneOn = drv_sidetone_is_playing();
    pDataToFill->appAiroThru.userRegister = AudioDsp_GetANCPassThruRegister();

    FW_Memcpy(&pDataToFill->appDSPCtl.mainCtlBdAddr, &g_audio_path_ctl.main_ctl.bdAddr.addr[0], sizeof(BD_ADDR_T));
    FW_Memcpy(&pDataToFill->appDSPCtl.assistantCtlBdAddr, &g_audio_path_ctl.assistant_ctl.bdAddr.addr[0], sizeof(BD_ADDR_T));
    pDataToFill->appDSPCtl.pathCtl_audioType = g_audio_path_ctl.assistant_ctl.audio_type;

    FW_Memcpy(&pDataToFill->appA2DP.bdAddr, drv_a2dp_get_bdaddr(), sizeof(BD_ADDR_T));

    if(drv_sco_get_bdaddr())
        FW_Memcpy(&pDataToFill->appSCO.bdAddr, drv_sco_get_bdaddr(), sizeof(BD_ADDR_T));
    else
        FW_Memset(&pDataToFill->appSCO.bdAddr, 0, sizeof(BD_ADDR_T));

	pDataToFill->appSCO.IsEcnrOff = drv_sco_is_ecnr_off();

	if(!APP_DspDevReg_FillRhoData(pDataToFill))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL APP_AudioDSP_AssignRhoData(void *pData)
{
	APP_RHO_STREAM_MANAGE_STRU *pDataToGet = &((APP_RHO_DATA_STRU *)pData)->appStreamManage;

	if(!pDataToGet)
	{
		DBG_LOG_APP_DSP_CONTROL( "[RHO_APP_Stream] New Agent Assign data error", 0);
		return FALSE;
	}

    FW_Memcpy(&g_audio_path_ctl.main_ctl.bdAddr.addr[0], &pDataToGet->appDSPCtl.mainCtlBdAddr, sizeof(BD_ADDR_T));
    FW_Memcpy(&g_audio_path_ctl.assistant_ctl.bdAddr.addr[0], &pDataToGet->appDSPCtl.assistantCtlBdAddr, sizeof(BD_ADDR_T));

    APP_VpSetMiscEnabled(pDataToGet->appVP.isMiscEnabled);

    if(pDataToGet->appSideTone.isSideToneOn)
    {
        drv_sidetone_start();
    }
    else
    {
        drv_sidetone_stop(FALSE);
    }

    AudioDsp_SetANCPassThruRegister(pDataToGet->appAiroThru.userRegister);
    FW_Memcpy(drv_a2dp_get_bdaddr(), &pDataToGet->appA2DP.bdAddr, sizeof(BD_ADDR_T));

    drv_sco_set_bdaddr(&pDataToGet->appSCO.bdAddr, CODEC_NO_CHANGE, SCO_HANDLE_NO_CHANGE, ROLE_AGENT);
    drv_sco_set_ecnr_off(pDataToGet->appSCO.IsEcnrOff);

	if(!APP_DspDevReg_AssignRhoData(pDataToGet))
	{
		return FALSE;
	}

#ifdef AIROHA_WWE_ENABLE
    if(pDataToGet->appDSPCtl.pathCtl_audioType == AUDIO_VAD)
    {
        APP_VAD_On();
    }
#endif
	return TRUE;
}

void APP_AudioDSP_SetBDA(BD_ADDR_T *pBdAddr)
{
    drv_a2dp_set_bdaddr(pBdAddr);
    drv_sco_set_bdaddr(pBdAddr, CODEC_NO_CHANGE, SCO_HANDLE_NO_CHANGE, ROLE_PARTNER);

    if(g_audio_path_ctl.main_ctl.audio_type != AUDIO_NONE)
        FW_Memcpy(&g_audio_path_ctl.main_ctl.bdAddr.addr[0], pBdAddr, sizeof(BD_ADDR_T));
    if(g_audio_path_ctl.assistant_ctl.audio_type != AUDIO_NONE)
        FW_Memcpy(&g_audio_path_ctl.assistant_ctl.bdAddr.addr[0], pBdAddr, sizeof(BD_ADDR_T));

#ifdef AIROHA_WWE_ENABLE
    if(g_audio_path_ctl.assistant_ctl.audio_type == AUDIO_VAD)
    {
        APP_VAD_Off();
    }
#endif
}

