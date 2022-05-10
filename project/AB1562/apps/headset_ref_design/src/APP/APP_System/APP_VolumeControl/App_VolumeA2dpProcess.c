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
#include "App_VolumeA2dpProcess.h"
#include "App_VolumeSetting.h"
#include "App_ResourceManager.h"
#include "App_LinkKey.h"
#include "App_Interface.h"
#include "AudioDSP_StreamManager.h"
#include "App_Avrcp.h"
#include "App_A2dp_NVKeyHandler.h"
#include "App_Media.h"
#include "Volume_NvkeyHandler.h"
#include "App_System.h"

/**************************************************************************************************
* Define
**************************************************************************************************/
#define CURRENT_A2DP_SOUND_LEVEL(pBdAddr)	APP_GetAppLinkByBdAddr(pBdAddr)->linkPara.soundLevelA2DP
#define CURRENT_AVRCP_VOLUME(pBdAddr)		APP_GetAppLinkByBdAddr(pBdAddr)->linkPara.avrcpVolume

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static void app_VolA2dp_SetLink(BD_ADDR_T *pBdAddr, U8 soundLevel, bool isLocalTriggered);
static void app_VolA2dp_SendVolumeToDSP(BD_ADDR_T *pBdAddr, U8 soundLevel);
//static void app_VolA2dp_SendMuteOnOffToDSP(BD_ADDR_T *pBdAddr, bool spkMuteOn);

/**************************************************************************************************
* Static Functions
**************************************************************************************************/
static void app_VolA2dp_SetLink(BD_ADDR_T *pBdAddr, U8 soundLevel, bool isLocalTriggered)
{
	if(APP_IsSetAppLink(pBdAddr))
	{
		APP_VolA2dp_SetCurrentAbsVolume(pBdAddr, APP_VolA2dp_GetAbsVolumeBySoundLevel(soundLevel));
		APP_VolA2dp_SetCurrentSoundLevel(pBdAddr, soundLevel);
		if(isLocalTriggered)
		{
<<<<<<< HEAD
			#ifdef AVRCP_Profile
=======
			#ifdef AIR_AVRCP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
			APP_Avrcp_SendVolumeChangeNotification(pBdAddr, soundLevel);
			#endif
		}
	}
}

static void app_VolA2dp_SendVolumeToDSP(BD_ADDR_T *pBdAddr, U8 soundLevel)
{
	if(!APP_IsSetAppLink(pBdAddr))
	{
		return;
	}

	if(APP_VolProcess_IsSpeakerMuteByLink(pBdAddr))
	{
        DBG_LOG_APP_SYSTEM( "[APP_A2DP] A2DP speaker mute by link", 0);
		soundLevel = 0;
	}
	else
	{
		//if(DRIVER_IsMiscCtrlTypeEnabled(DRIVER_IS_LOW_BATTERY) && (soundLevel > APP_NvramType_SoundCtl_GetA2doDecSoundLevelsBase()))
		//{
		//	soundLevel -= APP_NvramType_SoundCtl_GetA2dpDecSoundLevelsDec();
		//}
	}

	if(APP_AudioDspIsStreaming(pBdAddr, AUDIO_A2DP))
	{
		APP_VolSetting_SetSoundlevelToDSP(soundLevel, COMPONENT_A2DP);
	}
}

/*
static void app_VolA2dp_SendMuteOnOffToDSP(BD_ADDR_T *pBdAddr, bool spkMuteOn)
{
	spkMuteOn ? APP_VolSetting_SetSpeakerMuteOn() : APP_VolSetting_SetSpeakerMuteOff() ;
}
*/

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void APP_VolA2dp_VolumeUpDown(BD_ADDR_T *pBdAddr, BOOL isVolumeUp, BOOL isFollowerCmd)
{
	U8 soundLevel;

	soundLevel = APP_VolA2dp_GetCurrentSoundLevel(pBdAddr);

    DBG_LOG_APP_SYSTEM( "[Volume]A2DP Volup:%d, IsFollower:%d, CurSoundlevel:%d", 3, isVolumeUp, isFollowerCmd, soundLevel);

	if(isVolumeUp)
	{
		if(soundLevel < APP_MAX_A2DP_SOUND_LEVEL)
		{
			(soundLevel)++;
		}
		else if(APP_A2dp_NVKeyIsFeatureOn(APP_A2DP_CYCLIC_VOLUME_ENABLED_FEAT))
		{
            DBG_LOG_APP_SYSTEM( "[APP_A2DP] A2DP cyclic volume up feature", 0);
			soundLevel = 0;
		}
		else
		{
            DBG_LOG_APP_SYSTEM( "[Volume]Vol reach Max", 0);
			APP_Media_PushMediaEvent(MEDIA_EVT_VOLUME_REACH_MAX);
			return;
		}
	}
	else
	{
		if(soundLevel > 0)
		{
			soundLevel--;
		}
		else if(APP_A2dp_NVKeyIsFeatureOn(APP_A2DP_CYCLIC_VOLUME_ENABLED_FEAT))
		{
            DBG_LOG_APP_SYSTEM( "[APP_A2DP] A2DP cyclic volume down feature", 0);
			soundLevel = APP_MAX_A2DP_SOUND_LEVEL;
		}
		else
		{
            DBG_LOG_APP_SYSTEM( "[Volume]Vol reach Min", 0);
			APP_Media_PushMediaEvent(MEDIA_EVT_VOLUME_REACH_MIN);
			return;
		}
	}

	app_VolA2dp_SetLink(pBdAddr, soundLevel, TRUE);
	app_VolA2dp_SendVolumeToDSP(pBdAddr, soundLevel);

	//Beep!
	if(!isFollowerCmd)
	{
		APP_VolProcess_Beep(soundLevel, AUDIO_DEVICE_MUSIC);
	}
}

void APP_VolA2dp_RemoteHandler(BD_ADDR_T *pBdAddr, U8 soundLevel)
{
    DBG_LOG_APP_SYSTEM( "[Volume]A2DP Remote set soundlevel:%d", 1, soundLevel);
	app_VolA2dp_SetLink(pBdAddr, soundLevel, FALSE);
<<<<<<< HEAD
	#ifdef A2DP_Profile
=======
	#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
	app_VolA2dp_SendVolumeToDSP(pBdAddr, soundLevel);
	#endif
}

void APP_VolA2dp_SetCurrentSoundLevel(BD_ADDR_T *pBdAddr, U8 soundLevel)
{
	if(APP_IsSetAppLink(pBdAddr))
	{
		CURRENT_A2DP_SOUND_LEVEL(pBdAddr) = soundLevel;

		if(APP_REMEMBER_SOUND_LEVEL_CHANGE_IN_RUNTIME_FEAT)//APP_Hfp_IsFeatureOn(APP_REMEMBER_SOUND_LEVEL_CHANGE_IN_RUNTIME_FEAT))
		{
            DBG_LOG_APP_SYSTEM( "[APP_A2DP] A2DP save history in run time", 0);
			APP_LinkKey_SaveHistoryInRunTime(pBdAddr);
		}
	}
}

void APP_VolA2dp_SetCurrentAbsVolume(BD_ADDR_T *pBdAddr, U8 absVolume)
{
	if(APP_IsSetAppLink(pBdAddr))
	{
		CURRENT_AVRCP_VOLUME(pBdAddr) = absVolume;
	}
}

U8 APP_VolA2dp_GetCurrentSoundLevel(BD_ADDR_T *pBdAddr)
{
	if(APP_IsSetAppLink(pBdAddr))
	{
		return CURRENT_A2DP_SOUND_LEVEL(pBdAddr);
	}
	
	return 0;
}

U8 APP_VolA2dp_GetCurrentAbsVolume(BD_ADDR_T *pBdAddr)
{
	if(APP_IsSetAppLink(pBdAddr))
	{
		return CURRENT_AVRCP_VOLUME(pBdAddr);
	}
	return 0;

}

U8 APP_VolA2dp_GetSoundLevelByAbsVolume(U8 absVolume)
{
	U8 soundLevel;
	for(soundLevel = 0; soundLevel <= APP_MAX_A2DP_SOUND_LEVEL; soundLevel++)
	{
		if(absVolume <= APP_VolA2dp_GetAbsVolumeBySoundLevel(soundLevel))
		{
			break;
		}
	}
	return soundLevel;
}

U8 APP_VolA2dp_GetAbsVolumeBySoundLevel(U8 soundLevel)
{
	return APP_VolumeNvkey_GetAvrcpVolumeBySoundLevel(soundLevel);
}
