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
#include "App_VolumeProcess.h"
#include "App_VolumeScoMicProcess.h"
#include "App_VolumeSetting.h"
#include "App_ResourceManager.h"
#include "App_LinkKey.h"
#include "App_Interface.h"
#include "App_Media.h"
#include "App_Hfp.h"
#include "AudioDSP_StreamManager.h"
#include "Volume_NvkeyHandler.h"
#include "App_State.h"
#include "App_MCSync_Message.h"
#include "BtMCSync.h"
#include "App_System.h"

/**************************************************************************************************
* Define
**************************************************************************************************/
#define GET_SCO_VGM_BY_LEVEL(A)					APP_VolumeNvkey_GetScoMicVgmBySoundLevel(A)
#define CURRENT_SCO_VGM(pBdAddr)				APP_GetAppLinkByBdAddr(pBdAddr)->linkPara.vgm
#define CURRENT_SCO_MIC_SOUND_LEVEL(pBdAddr)	APP_GetAppLinkByBdAddr(pBdAddr)->linkPara.micSoundLevelSco

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static void app_VolScoMic_SetLink(BD_ADDR_T *pBdAddr, U8 soundLevel, bool isLocalTriggered);
static void app_VolScoMic_SendVolumeToDSP(BD_ADDR_T *pBdAddr, U8 soundLevel);
static void app_VolScoMic_SendMuteOnOffToDSP(bool micMuteOn);
static U32 app_VolScoMicHandler(Handler handler, U16 id, void *msg, U32 handler_id);
static void app_VolScoMic_MuteReminder(void);

/**************************************************************************************************
* Variable
**************************************************************************************************/
static const HandlerData gAppVolScoMicHandle = { app_VolScoMicHandler };

/**************************************************************************************************
* Static Functions
**************************************************************************************************/
static void app_VolScoMic_SetLink(BD_ADDR_T *pBdAddr, U8 soundLevel, bool isLocalTriggered)
{
	if(APP_IsSetAppLink(pBdAddr))
	{
		APP_VolScoMic_SetCurrentSoundLevel(pBdAddr, soundLevel);
		APP_VolScoMic_SetCurrentVgm(pBdAddr, GET_SCO_VGM_BY_LEVEL(soundLevel));

		if(isLocalTriggered)
		{
			APP_HFP_SendATCmdByLink(pBdAddr, HFP_ATCMD_ADJUST_MIC_GAIN);
		}
	}
}

static void app_VolScoMic_SendVolumeToDSP(BD_ADDR_T *pBdAddr, U8 soundLevel)
{
	if(!APP_IsSetAppLink(pBdAddr) || !APP_AudioDspIsStreaming(pBdAddr, AUDIO_SCO))
	{
		return;
	}

	if(APP_VolProcess_IsScoMicMute(pBdAddr))
	{
        DBG_LOG_APP_SYSTEM( "[APP_SCOMIC] SCO Mic mute by link", 0);
		soundLevel = 0;
	}
	else
	{
		//if(DRIVER_IsMiscCtrlTypeEnabled(DRIVER_IS_LOW_BATTERY) && (soundLevel > APP_NvramType_SoundCtl_GetScoDecSoundLevelsBase()))
		//{
		//	soundLevel -= APP_NvramType_SoundCtl_GetScoDecSoundLevelsDec();
		//}
	}

    APP_VolSetting_SetSoundlevelToDSP(soundLevel, COMPONENT_MIC_SCO);
}

static void app_VolScoMic_SendMuteOnOffToDSP(bool micMuteOn)
{
	micMuteOn ? APP_VolSetting_SetMicMuteOn() : APP_VolSetting_SetMicMuteOff() ;
}

static U32 app_VolScoMicHandler(Handler handler, U16 id, void *msg, U32 handler_id)
{
	UNUSED(handler);
	UNUSED(handler_id);
	UNUSED(msg);

	switch(id)
	{
		case TIMER_MIC_MUTE_REMIND:
			app_VolScoMic_MuteReminder();
			break;
		default:
			break;
	}
	return 0;
}

static void app_VolScoMic_MuteReminder(void)
{
	if(APP_State_AllLinksInCallActiveState())
	{
		APP_Media_PushMediaEvent(MEDIA_EVT_MUTE_REMINDER);
	}

	FW_SetTimer((Handler)&gAppVolScoMicHandle, TIMER_MIC_MUTE_REMIND, NULL, 0, APP_VolumeNvkey_GetTimer(TIMER_MIC_MUTE_REMIND) * ONE_SEC);
}

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void APP_VolScoMic_VolumeUpDown(BD_ADDR_T *pBdAddr, BOOL isVolumeUp)
{
	U8 soundLevel;

	soundLevel = APP_VolScoMic_GetCurrentSoundLevel(pBdAddr);

	//Reversed Volume Control
//	isVolumeUp = (gMMI_driver_variation_nvram.misc_para.init.isMiscEnabled & IS_VOLUME_ORIENTATION_REVERSED) ? !isVolumeUp : isVolumeUp;

    DBG_LOG_APP_SYSTEM( "[Volume]ScoMic Volup:%d, CurSoundlevel:%d", 2, isVolumeUp, soundLevel);

	if(isVolumeUp)
	{
		if(soundLevel < APP_MAX_SCO_MIC_SOUND_LEVEL)
		{
			(soundLevel)++;
		}
		else if(APP_CYCLIC_VOLUME_ENABLED_FEAT)
		{
            DBG_LOG_APP_SYSTEM( "[APP_SCOMIC] SCO Mic cyclic volume up feature", 0);
			soundLevel = 0;
		}
		else
		{
            DBG_LOG_APP_SYSTEM( "[Volume]Vol reach Max", 0);
			return;
		}
	}
	else
	{
		if(soundLevel > 0)
		{
			soundLevel--;
		}
		else if(APP_CYCLIC_VOLUME_ENABLED_FEAT)
		{
            DBG_LOG_APP_SYSTEM( "[APP_SCOMIC] SCO Mic cyclic volume down feature", 0);
			soundLevel = APP_MAX_SCO_MIC_SOUND_LEVEL;
		}
		else
		{
            DBG_LOG_APP_SYSTEM( "[Volume]Vol reach Min", 0);
			return;
		}
	}

	app_VolScoMic_SetLink(pBdAddr, soundLevel, TRUE);

	app_VolScoMic_SendVolumeToDSP(pBdAddr, soundLevel);

	//Beep!
	if(!PM_IsProfileConnected(pBdAddr, PROFILE_MCSYNC))
	{
		APP_VolProcess_Beep(soundLevel, AUDIO_DEVICE_SCO, TRUE);
	}
	else
	{
		APP_Media_PushMediaEvent(MEDIA_EVT_PLAYING_BEEP_SYNC);
	}
}

void APP_VolScoMic_RemoteHandler(BD_ADDR_T *pBdAddr, U8 vgm)
{
	U8 soundLevel = APP_VolScoMic_GetSoundLevelByVgm(vgm);

    DBG_LOG_APP_SYSTEM( "[Volume]ScoMic Remote set Vgm:%d, soundlevel:%d", 2, vgm, soundLevel);

	if(BtAwsMce_IsDefaultRoleAgent())
	{
		app_VolScoMic_SetLink(pBdAddr, soundLevel, FALSE);
	}

	app_VolScoMic_SendVolumeToDSP(pBdAddr, soundLevel);
}

void APP_VolScoMic_SetScoMicMuteOnOff(BD_ADDR_T *pBdAddr)
{
	if(APP_IsSetAppLink(pBdAddr))
	{
		bool micMuteOn = APP_VolProcess_IsScoMicMute(pBdAddr);
        DBG_LOG_APP_SYSTEM( "[APP_SCOMIC] Set SCO Mic MuteOn:%d", 1, micMuteOn);
		app_VolScoMic_SendMuteOnOffToDSP(micMuteOn);

		if(micMuteOn)
		{
			FW_SetTimer((Handler)&gAppVolScoMicHandle, TIMER_MIC_MUTE_REMIND, NULL, 0, APP_VolumeNvkey_GetTimer(TIMER_MIC_MUTE_REMIND) * ONE_SEC);
		}
		else
		{
			FW_ReleaseTimer((Handler)&gAppVolScoMicHandle, TIMER_MIC_MUTE_REMIND, 0);
		}
	}
}

void APP_VolScoMic_SetCurrentSoundLevel(BD_ADDR_T *pBdAddr, U8 soundLevel)
{
	if(APP_IsSetAppLink(pBdAddr))
	{
		CURRENT_SCO_MIC_SOUND_LEVEL(pBdAddr) = soundLevel;

		if(APP_REMEMBER_SOUND_LEVEL_CHANGE_IN_RUNTIME_FEAT)//APP_Hfp_IsFeatureOn(APP_REMEMBER_SOUND_LEVEL_CHANGE_IN_RUNTIME_FEAT))
		{
            DBG_LOG_APP_SYSTEM( "[APP_SCOMIC] SCO Mic save history in run time", 0);
			APP_LinkKey_SaveHistoryInRunTime(pBdAddr);
		}
	}
}

void APP_VolScoMic_SetCurrentVgm(BD_ADDR_T *pBdAddr, U8 vgm)
{
	if(APP_IsSetAppLink(pBdAddr))
	{
		CURRENT_SCO_VGM(pBdAddr) = vgm;
	}
}

void APP_VolScoMic_SyncBeep(BD_ADDR_T *pBdAddr)
{
	APP_VolProcess_Beep(APP_VolScoMic_GetCurrentSoundLevel(pBdAddr), AUDIO_DEVICE_SCO, FALSE);
}

U8 APP_VolScoMic_GetCurrentSoundLevel(BD_ADDR_T *pBdAddr)
{
	if(APP_IsSetAppLink(pBdAddr))
	{
		return CURRENT_SCO_MIC_SOUND_LEVEL(pBdAddr);
	}
	return 0;
}

U8 APP_VolScoMic_GetCurrentVgm(BD_ADDR_T *pBdAddr)
{
	if(APP_IsSetAppLink(pBdAddr))
	{
		return CURRENT_SCO_VGM(pBdAddr);
	}
	return 0;
}

U8 APP_VolScoMic_GetSoundLevelByVgm(U8 vgm)
{
	U8 soundLevel;
	for(soundLevel = 0; soundLevel < APP_MAX_SCO_MIC_SOUND_LEVEL; soundLevel++)
	{
		if(GET_SCO_VGM_BY_LEVEL(soundLevel) >= vgm)
		{
			return soundLevel;
		}
	}
	return APP_MAX_SCO_MIC_SOUND_LEVEL;
}

U8 APP_VolScoMic_GetVgmBySoundLevel(U8 soundLevel)
{
	return GET_SCO_VGM_BY_LEVEL(soundLevel);
}
