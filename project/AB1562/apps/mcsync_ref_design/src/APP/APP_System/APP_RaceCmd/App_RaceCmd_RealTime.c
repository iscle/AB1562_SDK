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
#include "AudioDSP_StreamManager.h"
#include "App_PeqManage.h"
#include "App_PeqProcess.h"
#include "Peq_NvkeyHandler.h"
#include "race_cmd.h"
#include "drv_recorder.h"
#include "App_Media.h"
#include "drv_leakage_detect.h"

#define DBG_LOG_APP_REALTIME(_message, arg_cnt, ...)  LOG_MSGID_I(APP_REALTIME, _message, arg_cnt, ##__VA_ARGS__)
log_create_module(APP_REALTIME, PRINT_LEVEL_INFO);

/******************************************************************************
* Structure
*******************************************************************************/

typedef struct
{
    U8  is_start;
    U8  mode;
    U8  bit_per_sample;
    U8  channel_num;
    U16 frame_size;
    U8  seq_num;
    U32 total_data_length;
    U16 data_length;
    U8  data[1];
}PACKED racecmd_realtime_anc_adaptive_check_rsp_t;

/******************************************************************************
* Static Functions
*******************************************************************************/

static U32 app_RaceCmd_RealTimeHandler(Handler app, U16 id, void *msg, U32 id_ext);
static const HandlerData gAppRaceCmdRealTimeHandle = { app_RaceCmd_RealTimeHandler };

#ifdef MTK_LEAKAGE_DETECTION_ENABLE
static U32 app_racecmd_realtime_ld_mode_callback(Handler app, U16 id, void *msg, U32 id_ext);
static const HandlerData g_app_racecmd_realtime_handle = {app_racecmd_realtime_ld_mode_callback};
static U8 g_app_racecmd_realtime_ld_channel;
#endif

#ifdef MTK_LEAKAGE_DETECTION_ENABLE
static BOOL app_racecmd_realtime_ld_mode_handle(void)
{
    if(!DrvLeakageDetect_StartLeakageDetect(LD_MODE_POWER_DB, (Handler) &g_app_racecmd_realtime_handle) || !APP_Media_PushMediaEvent(MEDIA_EVT_LEAKAGE_DETECTION))
    {
        return FALSE;
    }

    return TRUE;
}

static void app_racecmd_realtime_power_db_handle(S32 value)
{
    racecmd_realtime_anc_adaptive_check_rsp_t *rsp;
    U32 length = sizeof(racecmd_realtime_anc_adaptive_check_rsp_t) + sizeof(S32) - sizeof(U8);
    if( (rsp = FW_GetMemory(length)) != NULL )
    {
        FW_Memset(rsp, 0, length);
        rsp->mode = RACE_ADAPTIVE_ANC_CHECK_CMD_LD_MODE;
        rsp->total_data_length = sizeof(S32);
        rsp->data_length = sizeof(S32);
        FW_Memcpy(&rsp->data[0], &value, sizeof(S32));
        RACE_ANCAaptiveCheck_Send(g_app_racecmd_realtime_ld_channel, RACE_TYPE_NOTIFICATION, (U8 *)rsp, length);
    }
    g_app_racecmd_realtime_ld_channel = 0;
}

static U32 app_racecmd_realtime_ld_mode_callback(Handler app, U16 id, void *msg, U32 id_ext)
{
    UNUSED(app);
    UNUSED(msg);

    switch(id)
    {
        case LD_MSG_POWER_DB_VALUE:
            app_racecmd_realtime_power_db_handle((S32) id_ext);
            break;
    }
    return 0;
}
#endif

static void app_racecmd_realtime_anc_adaptive_check(BT_APP_DSP_REALTIME_ANC_ADAPTIVE_CHECK_T *msg)
{
    DBG_LOG_APP_REALTIME("[APP_REAL_TIME] BT_APP_DSP_REALTIME_ANC_ADAPTIVE_CHECK mode:%d, is_enable:%d", 2, msg->mode,msg->is_enable);
    BOOL isSuccessful = FALSE;
    U8 rsp[2];
    switch(msg->mode)
    {
#ifdef MTK_LEAKAGE_DETECTION_ENABLE
        case RACE_ADAPTIVE_ANC_CHECK_CMD_LD_MODE:
            if(msg->is_enable && g_app_racecmd_realtime_ld_channel == 0)
            {
                if( (isSuccessful = app_racecmd_realtime_ld_mode_handle()) )
                {
                    g_app_racecmd_realtime_ld_channel = msg->channelId;
                }
            }
            break;
        case RACE_ADAPTIVE_ANC_CHECK_CMD_IH_DETECT_MODE:
            break;
#endif
#if LAB_TUNNING_APP
        case RACE_ADAPTIVE_ANC_CHECK_CMD_RECORD_MODE_MIC_SZ:
        case RACE_ADAPTIVE_ANC_CHECK_CMD_RECORD_MODE_MIC_WZ:
        case RACE_ADAPTIVE_ANC_CHECK_CMD_RECORD_MODE_FBC:
            if(msg->is_enable)
            {
                if(!APP_AudioDspIsStreaming(NULL,AUDIO_STREAM_RECORD))
                {
                    AUDIO_DSP_START_PARA_STUR audioDspPara;
                    FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_START_PARA_STUR));
                    audioDspPara.audioType = AUDIO_STREAM_RECORD;
                    audioDspPara.recordPara.mode = msg->mode;

                    if(msg->mode == RACE_ADAPTIVE_ANC_CHECK_CMD_RECORD_MODE_MIC_SZ) {
                       audioDspPara.recordPara.mode = RECORD_MODE_MIC_SZ;
                    } else if(msg->mode == RACE_ADAPTIVE_ANC_CHECK_CMD_RECORD_MODE_MIC_WZ){
                       audioDspPara.recordPara.mode = RECORD_MODE_MIC_WZ;
                    } else if(msg->mode == RACE_ADAPTIVE_ANC_CHECK_CMD_RECORD_MODE_FBC){
                       audioDspPara.recordPara.mode = RECORD_MODE_FBC;
                    } else {
                        break;
                    }
                    drv_recorder_init(msg->channelId,RACE_ADAPTIVE_ANC_CHECK_RECORD_SIZE);
                    isSuccessful = APP_AudioDspPlay(&audioDspPara);
                    if((msg->mode == RACE_ADAPTIVE_ANC_CHECK_CMD_RECORD_MODE_MIC_SZ ||
                        msg->mode == RACE_ADAPTIVE_ANC_CHECK_CMD_RECORD_MODE_FBC)&&
                       isSuccessful)
                    {
                       drv_recorder_set_pause(TRUE);
                       if(!APP_Media_PushMediaEvent(MEDIA_EVT_ANC_CAL_SZ))
                          isSuccessful = FALSE;
                    }
                }
            }
            else
            {
                AUDIO_DSP_STOP_PARA_STUR audioDspPara;
                FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_STOP_PARA_STUR));
                audioDspPara.audioType = AUDIO_STREAM_RECORD;
                isSuccessful = APP_AudioDspStop(&audioDspPara);
            }
            break;
        case RACE_ADAPTIVE_ANC_CHECK_CMD_RECORD_MODE_CALIBRATION_SETTING:
            isSuccessful = drv_anc_set_fbc_calibration(msg->is_enable);
            break;
#endif
        default:
            break;
    }
    rsp[0] = (isSuccessful) ? RACE_ADAPTIVE_ANC_CHECK_RSP_SUCCESS : RACE_ADAPTIVE_ANC_CHECK_RSP_FAIL;
    rsp[1] = msg->mode;
    RACE_ANCAaptiveCheck_Send(msg->channelId,RACE_TYPE_RESPONSE,&rsp[0],sizeof(rsp));
}

static U32 app_RaceCmd_RealTimeHandler(Handler app, U16 id, void *msg, U32 id_ext)
{
    switch(id)
    {
        case BT_APP_DSP_REALTIME_SUSPEND_DSP:
            DBG_LOG_APP_REALTIME("[APP_REAL_TIME] BT_APP_DSP_REALTIME_SUSPEND_DSP", 0);
            APP_AudioDSPRealtimeSuspend();
            break;
        case BT_APP_DSP_REALTIME_RESUME_DSP:
            DBG_LOG_APP_REALTIME( "[APP_REAL_TIME] BT_APP_DSP_REALTIME_SUSPEND_DSP", 0);
            APP_AudioDSPRealtimeResume();
            break;
        case BT_APP_DSP_SOUND_EFFECT_UPDATE:
            // change NVKey
            //nvkey_hdl_save_nvkey(NVKEY_ID_DSP_ALG + DSP_A2DP_DBB_OFFSET, (U8*)&((BT_APP_DSP_SOUND_EFFECT_UPDATE_T *)msg)->data[0], ((BT_APP_DSP_SOUND_EFFECT_UPDATE_T *)msg)->length);
            break;
        case BT_APP_DSP_PEQ_UPDATE:
            DBG_LOG_APP_REALTIME( "[APP_REAL_TIME] PEQ realtime", 0);
            APP_PeqManage_RealtimeUpdate(((BT_APP_DSP_PEQ_UPDATE_T *)msg)->data, ((BT_APP_DSP_PEQ_UPDATE_T *)msg)->length);
            break;
        case BT_APP_DSP_PEQ_SAVE_STATUS:
            DBG_LOG_APP_REALTIME( "[APP_REAL_TIME] PEQ SAVE Status", 0);
            APP_PeqNvkey_Init();
            APP_PeqProc_SetIndex(PEQ_A2DP, APP_PeqNvkey_GetPEQGroupNum(PEQ_A2DP, PEQ_LOAD_DEFAULT));
            break;
        case BT_APP_DSP_REALTIME_ANC_ADAPTIVE_CHECK:
            DBG_LOG_APP_REALTIME( "[APP_REAL_TIME] BT_APP_DSP_REALTIME_ANC_ADAPTIVE_CHECK", 0);
            app_racecmd_realtime_anc_adaptive_check((BT_APP_DSP_REALTIME_ANC_ADAPTIVE_CHECK_T *)msg);
            break;
    }
    UNUSED(id_ext);
    UNUSED(app);
    return 0;
}

/******************************************************************************
* Public Functions
*******************************************************************************/

void APP_RaceCmd_RealTimeInit(void)
{
    BtRaceCommand_RealTime_RegisterHandler((Handler)&gAppRaceCmdRealTimeHandle);
}

