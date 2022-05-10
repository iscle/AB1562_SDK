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

#ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL

#include "drv_mp3.h"
#include "App_EventOpcode.h"
#include "App_Interface.h"
#include "App_State.h"
#include "App_EventKeyHandler.h"
#include "AudioDSP_Registration.h"
#include "App_VolumeManage.h"
#include "App_System.h"
#include "AudioDSP_StreamManager.h"


static U32 app_MP3_Handler(Handler handler, U16 id, void *msg, U32 ext_id);
static U16 app_Mp3Key_CheckKeyMp3PowerOn(U16 keyEventCode);
static void app_MP3_KeyPowerOn(BD_ADDR_T *pBdAddr);
static void app_MP3_KeyPowerOff(BD_ADDR_T *pBdAddr);
static void app_MP3_KeyPlay(BD_ADDR_T *pBdAddr);
static void app_MP3_KeyPause(BD_ADDR_T *pBdAddr);
static void app_MP3_KeyStop(BD_ADDR_T *pBdAddr);
static void app_MP3_KeyForward(BD_ADDR_T *pBdAddr);
static void app_MP3_KeyBackward(BD_ADDR_T *pBdAddr);


static HandlerData const gAppMP3Handle = { app_MP3_Handler };
static APP_KEY_CHECK_STRU const APP_MP3_KeyCheckValidHandler[MP3_KEY_EVENT_NO] =
{
	{app_Mp3Key_CheckKeyMp3PowerOn}, //MP3_KEY_EVENT_KEY_MP3_POWERON}
	{NULL}, //MP3_KEY_EVENT_KEY_MP3_POWEROFF //Wait until voice prompt is over
	{NULL}, //MP3_KEY_EVENT_KEY_MP3_PLAY //Wait until voice prompt is over
	{NULL}, //MP3_KEY_EVENT_KEY_MP3_STOP
	{NULL}, //MP3_KEY_EVENT_KEY_MP3_PAUSE,
	{NULL}, //MP3_KEY_EVENT_KEY_MP3_FORWARD,
	{NULL}, //MP3_KEY_EVENT_KEY_MP3_BACKWARD,
};

static APP_KEY_EVT_STRU const gMp3KeyEvtHandler[MP3_KEY_EVENT_NO] =
{
	{app_MP3_KeyPowerOn}, 			//MP3_KEY_EVENT_KEY_MP3_POWERON
	{app_MP3_KeyPowerOff}, 				//MP3_KEY_EVENT_KEY_MP3_POWEROFF,
	{app_MP3_KeyPlay}, 			//MP3_KEY_EVENT_KEY_MP3_PLAY,
	{app_MP3_KeyStop}, 	//MP3_KEY_EVENT_KEY_MP3_STOP,
	{app_MP3_KeyPause}, 	//MP3_KEY_EVENT_KEY_MP3_PAUSE,   //0x05
	{app_MP3_KeyForward}, 			//MP3_KEY_EVENT_KEY_MP3_FORWARD,
	{app_MP3_KeyBackward},			//MP3_KEY_EVENT_KEY_MP3_BACKWARD,
};

static U32 app_MP3_Handler(Handler handler, U16 id, void *msg, U32 ext_id)
{
    UNUSED(handler);
    UNUSED(msg);
    UNUSED(ext_id);
    switch(id)
    {
        case DRV_MP3_ID_INIT_OK:
            APP_Media_PushMediaEvent(MEDIA_EVT_MP3_POWERON);
            break;
        case DRV_MP3_ID_POWER_OFF_REQ:
            app_MP3_KeyPowerOff(NULL);
            break;
    }
    return 0;
}
static U16 app_Mp3Key_CheckKeyMp3PowerOn(U16 keyEventCode)
{
	if(APP_IsAudioDeviceRegistered(NULL, AUDIO_DEVICE_MP3))
	{
		DBG_LOG_APP_SYSTEM( "[    APP][DSP] MP3 Start Fail: Already Registered", 0);
		keyEventCode = KEY_INVALID;
	}
	else if(!APP_AudioDeviceRegister(NULL, AUDIO_DEVICE_MP3))
	{
		APP_AudioDeviceDeRegister(NULL, AUDIO_DEVICE_MP3);
		DBG_LOG_APP_SYSTEM( "[    APP][DSP] MP3 Start Fail", 0);
		keyEventCode = KEY_INVALID;
	}

	return keyEventCode;
}

static void app_MP3_KeyPowerOn(BD_ADDR_T *pBdAddr)
{
    UNUSED(pBdAddr);
	drv_mp3_init_drive((Handler)&gAppMP3Handle, DRIVE_SD_CARD);
	APP_State_AddTopState(APP_EOF, APP_MP3);
}

static void app_MP3_KeyPowerOff(BD_ADDR_T *pBdAddr)
{
	AUDIO_DSP_STOP_PARA_STUR audioDspPara;
	FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_STOP_PARA_STUR));
	audioDspPara.audioType = AUDIO_MP3;
	APP_AudioDspStop(&audioDspPara);
	drv_mp3_deinit_drive();
	APP_State_RmvState(APP_EOF, APP_MP3);
	APP_AudioDeviceDeRegister(NULL, AUDIO_DEVICE_MP3);
	APP_ResumeOtherAudioDevice(NULL, AUDIO_DEVICE_MP3);
    UNUSED(pBdAddr);
}

static void app_MP3_KeyPlay(BD_ADDR_T *pBdAddr)
{
	AUDIO_DSP_START_PARA_STUR audioDspPara;
	UNUSED(pBdAddr);
	if(!drv_mp3_is_dsp_playing())
	{
		FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_START_PARA_STUR));
		audioDspPara.audioType = AUDIO_MP3;
		if(APP_AudioDspPlay(&audioDspPara))
			APP_VolManage_SpkVolumeRemoteHandler(APP_EOF, APP_VolManage_GetCurrentMp3SoundLevel(), COMPONENT_MP3);
	}
	else if(!drv_mp3_is_user_playing())
		drv_mp3_resume();
	else if(1/*APP_MP3_PLAY_PAUSE_ONE_KEY_FEATURE*/)
		drv_mp3_pause();
}

static void app_MP3_KeyPause(BD_ADDR_T *pBdAddr)
{
	AUDIO_DSP_START_PARA_STUR audioDspPara;
	UNUSED(pBdAddr);
	if(drv_mp3_is_dsp_playing() && drv_mp3_is_user_playing())
		drv_mp3_pause();
	else if(1/*APP_MP3_PLAY_PAUSE_ONE_KEY_FEATURE*/)
	{
		if(!drv_mp3_is_dsp_playing())
		{
			FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_START_PARA_STUR));
			audioDspPara.audioType = AUDIO_MP3;
			if(APP_AudioDspPlay(&audioDspPara))
				APP_VolManage_SpkVolumeRemoteHandler(APP_EOF, APP_VolManage_GetCurrentMp3SoundLevel(), COMPONENT_MP3);
		}
		else if(!drv_mp3_is_user_playing())
			drv_mp3_resume();
	}
}

static void app_MP3_KeyStop(BD_ADDR_T *pBdAddr)
{
	AUDIO_DSP_STOP_PARA_STUR audioDspPara;
	FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_STOP_PARA_STUR));
	audioDspPara.audioType = AUDIO_MP3;
	APP_AudioDspStop(&audioDspPara);
    UNUSED(pBdAddr);
}

static void app_MP3_KeyForward(BD_ADDR_T *pBdAddr)
{
	drv_mp3_track_next();
    UNUSED(pBdAddr);
}

static void app_MP3_KeyBackward(BD_ADDR_T *pBdAddr)
{
	drv_mp3_track_prev();
    UNUSED(pBdAddr);
}


U16 APP_MP3_CheckValidKey(U8 index, U16 keyCode)
{
	if(APP_MP3_KeyCheckValidHandler[index].fKeyCheckValid)
	{
		keyCode = APP_MP3_KeyCheckValidHandler[index].fKeyCheckValid(keyCode);
	}
	return keyCode;
}

void APP_MP3_ProcessKeyEvent(BD_ADDR_T *pBdAddr, U8 eventIndex)
{
	if(gMp3KeyEvtHandler[eventIndex].fKeyEvt)
	{
		gMp3KeyEvtHandler[eventIndex].fKeyEvt(pBdAddr);
	}
}

#endif
