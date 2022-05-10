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
#include "App_VolumeScoProcess.h"
#include "App_VolumeProcess.h"
#include "App_VolumeSetting.h"
#include "App_ResourceManager.h"
#include "App_LinkKey.h"
#include "App_Interface.h"
#include "App_Media.h"
#include "App_Hfp.h"
#include "AudioDSP_StreamManager.h"
#include "Volume_NvkeyHandler.h"
#include "APP_HfpScoHandler.h"
#include "App_State.h"
#include "App_System.h"

/**************************************************************************************************
* Define
**************************************************************************************************/
#define CURRENT_SCO_SOUND_LEVEL(pBdAddr)	APP_GetAppLinkByBdAddr(pBdAddr)->linkPara.soundLevelSco
#define CURRENT_SCO_VGS(pBdAddr)			APP_GetAppLinkByBdAddr(pBdAddr)->linkPara.vgs
#define GET_SCO_VGS_BY_LEVEL(A)				APP_VolumeNvkey_GetScoVgsBySoundLevel(A)

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static void app_VolSco_SetLink(BD_ADDR_T *pBdAddr, U8 soundLevel, bool isLocalTriggered);
static void app_VolSco_SendVolumeToDSP(BD_ADDR_T *pBdAddr, U8 soundLevel, U8 component);
static void app_VolSco_SendSpkMuteOnOffToDSP(bool spkMuteOn);

/**************************************************************************************************
* Static Functions
**************************************************************************************************/
static void app_VolSco_SetLink(BD_ADDR_T *pBdAddr, U8 soundLevel, bool isLocalTriggered)
{
	if(APP_IsSetAppLink(pBdAddr))
	{
		APP_VolSco_SetCurrentSoundLevel(pBdAddr, soundLevel);
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE        
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE        
>>>>>>> db20e11 (second commit)
		if(isLocalTriggered && (APP_Hfp_IsConnected(pBdAddr) || APP_Hfp_IsHspConnected(pBdAddr)))
#else
        if(isLocalTriggered && APP_Hfp_IsConnected(pBdAddr)
#endif
		{
			APP_HFP_SendATCmdByLink(pBdAddr, HFP_ATCMD_ADJUST_VOLUME);
		}
	}
}

static void app_VolSco_SendVolumeToDSP(BD_ADDR_T *pBdAddr, U8 soundLevel, U8 component)
{
	if(!APP_IsSetAppLink(pBdAddr))
	{
		return;
	}

	if(APP_VolProcess_IsSpeakerMuteByLink(pBdAddr))
	{
		DBG_LOG_APP_SYSTEM( "[APP_SCO] SCO speaker mute by link", 0);
		soundLevel = 0;
	}
	else
	{
		//if(DRIVER_IsMiscCtrlTypeEnabled(DRIVER_IS_LOW_BATTERY) && (soundLevel > APP_NvramType_SoundCtl_GetScoDecSoundLevelsBase()))
		//{
		//	soundLevel -= APP_NvramType_SoundCtl_GetScoDecSoundLevelsDec();
		//}
	}

	if(APP_AudioDspIsStreaming(pBdAddr, AUDIO_SCO))
	{
		APP_VolSetting_SetSoundlevelToDSP(soundLevel, component);
	}
}

static void app_VolSco_SendSpkMuteOnOffToDSP(bool spkMuteOn)
{
	spkMuteOn ? APP_VolSetting_SetSpeakerMuteOn() : APP_VolSetting_SetSpeakerMuteOff() ;
}

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void APP_VolSco_VolumeUpDown(BD_ADDR_T *pBdAddr, BOOL isVolumeUp, BOOL isFollowerCmd)
{
	U8 soundLevel;

	soundLevel = APP_VolSco_GetCurrentSoundLevel(pBdAddr);

	DBG_LOG_APP_SYSTEM( "[Volume]Sco Volup:%d, IsFollower:%d, CurSoundlevel:%d", 3, isVolumeUp, isFollowerCmd, soundLevel);

	if(isVolumeUp)
	{
		if(soundLevel < APP_MAX_SCO_SOUND_LEVEL)
		{
			(soundLevel)++;
		}
		else if(APP_CYCLIC_VOLUME_ENABLED_FEAT)
		{
			DBG_LOG_APP_SYSTEM( "[APP_SCO] SCO cyclic volume up feature", 0);
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
		else if(APP_CYCLIC_VOLUME_ENABLED_FEAT)
		{
			DBG_LOG_APP_SYSTEM( "[APP_SCO] SCO cyclic volume down feature", 0);
			soundLevel = APP_MAX_SCO_SOUND_LEVEL;
		}
		else
		{
			DBG_LOG_APP_SYSTEM( "[Volume]Vol reach Min", 0);
			APP_Media_PushMediaEvent(MEDIA_EVT_VOLUME_REACH_MIN);
			return;
		}
	}

	app_VolSco_SetLink(pBdAddr, soundLevel, TRUE);
	app_VolSco_SendVolumeToDSP(pBdAddr, soundLevel, APP_HfpSco_IsSupportCodecWideBand(pBdAddr) ? COMPONENT_SCO_WB : COMPONENT_SCO_NB);

	//Beep!
	if(!isFollowerCmd)
	{
		APP_VolProcess_Beep(soundLevel, AUDIO_DEVICE_SCO);
	}
}

void APP_VolSco_RemoteHandler(BD_ADDR_T *pBdAddr, U8 soundLevel, U8 component)
{
	DBG_LOG_APP_SYSTEM( "[Volume]Sco Remote set soundlevel:%d", 1, soundLevel);

	app_VolSco_SetLink(pBdAddr, soundLevel, FALSE);
	#ifdef HFP_Profile
	app_VolSco_SendVolumeToDSP(pBdAddr, soundLevel, component);
	#endif
}

void APP_VolSco_SetScoSpkMuteOnOff(BD_ADDR_T *pBdAddr)
{
	if(APP_IsSetAppLink(pBdAddr) && APP_IsDSPInSCO(pBdAddr))
	{
		bool spkMuteOn = APP_VolProcess_IsSpeakerMuteByLink(pBdAddr);
		DBG_LOG_APP_SYSTEM( "[APP_SCO] Set SCO Spk MuteOn:%d", 1, spkMuteOn);
		app_VolSco_SendSpkMuteOnOffToDSP(spkMuteOn);
	}
}

void APP_VolSco_SetCurrentSoundLevel(BD_ADDR_T *pBdAddr, U8 soundLevel)
{
	if(APP_IsSetAppLink(pBdAddr))
	{
		CURRENT_SCO_SOUND_LEVEL(pBdAddr) = soundLevel;

		if(APP_REMEMBER_SOUND_LEVEL_CHANGE_IN_RUNTIME_FEAT)//APP_Hfp_IsFeatureOn(APP_REMEMBER_SOUND_LEVEL_CHANGE_IN_RUNTIME_FEAT))
		{
			DBG_LOG_APP_SYSTEM( "[APP_SCO] SCO save history in run time", 0);
			APP_LinkKey_SaveHistoryInRunTime(pBdAddr);
		}
	}
}

void APP_VolSco_SetCurrentVgs(BD_ADDR_T *pBdAddr, U8 vgs)
{
	if(APP_IsSetAppLink(pBdAddr))
	{
		CURRENT_SCO_VGS(pBdAddr) = vgs;
	}
}

U8 APP_VolSco_GetCurrentSoundLevel(BD_ADDR_T *pBdAddr)
{
	if(APP_IsSetAppLink(pBdAddr))
	{
		return CURRENT_SCO_SOUND_LEVEL(pBdAddr);
	}
	return 0;
}

U8 APP_VolSco_GetCurrentVgs(BD_ADDR_T *pBdAddr)
{
	if(APP_IsSetAppLink(pBdAddr))
	{
		return CURRENT_SCO_VGS(pBdAddr);
	}
	return 0;
}

U8 APP_VolSco_GetSoundLevelByVgs(U8 vgs)
{
	U8 soundLevel;
	for(soundLevel = 0; soundLevel < APP_MAX_SCO_SOUND_LEVEL; soundLevel++)
	{
		if(GET_SCO_VGS_BY_LEVEL(soundLevel) >= vgs)
		{
			return soundLevel;
		}
	}
	return APP_MAX_SCO_SOUND_LEVEL;
}

U8 APP_VolSco_GetVgsBySoundLevel(U8 soundLevel)
{
	return GET_SCO_VGS_BY_LEVEL(soundLevel);
}

