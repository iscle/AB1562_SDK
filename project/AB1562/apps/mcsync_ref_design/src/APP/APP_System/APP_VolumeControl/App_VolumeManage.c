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
#include "App_VolumeManage.h"
#include "App_VolumeProcess.h"
#include "App_VolumeScoProcess.h"
#include "App_VolumeScoMicProcess.h"
#include "App_VolumeA2dpProcess.h"
#include "App_VolumeMp3Process.h"
#include "App_VolumeVpRtProcess.h"
#include "App_VolumeAiroThruProcess.h"
#include "App_Interface.h"
#include "AudioDSP_Registration.h"
#include "App_ResourceManager.h"
#include "App_EventOpcode.h"
#include "App_Media.h"
#include "App_Hfp.h"
#include "APP_AudioDriver.h"
#include "App_A2dp.h"
#include "Volume_NvkeyHandler.h"
#include "AudioDSP_StreamManager.h"
#include "APP_AiroThru.h"
#ifdef LINEIN_ENABLE
#include "App_VolumeLineinProcess.h"
#endif
#include "App_System.h"
#include "BtMCSync.h"
#include "App_MCSync_Message.h"

/**************************************************************************************************
* Static Functions
**************************************************************************************************/
static void app_VolManage_SetMicMuteByLink(BD_ADDR_T *pBdAddr, U8 muteCmd)
{
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	if(muteCmd == SET_MUTE_ON)
	{
		pLinkInfo->linkPara.isMicMute = TRUE;
	}
	else if(muteCmd == SET_MUTE_OFF)
	{
		pLinkInfo->linkPara.isMicMute = FALSE;
	}
	else if(muteCmd == TOGGLE_MUTE_ON_OFF)
	{
		pLinkInfo->linkPara.isMicMute = !pLinkInfo->linkPara.isMicMute;
	}

	if(APP_MUTE_SPEAKER_AS_MICROPHONE_FEAT)
	{
		pLinkInfo->linkPara.isSpkMuteForMicMute = pLinkInfo->linkPara.isMicMute;
		APP_VolSco_SetScoSpkMuteOnOff(pBdAddr);
	}

	APP_VolScoMic_SetScoMicMuteOnOff(pBdAddr);
}

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void APP_VolManage_SetSpkMuteByLink(BD_ADDR_T *pBdAddr, U8 muteCmd)
{
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	if(muteCmd == SET_MUTE_ON)
	{
		pLinkInfo->linkPara.isSpkMuteForSCO = TRUE;
	}
	else if(muteCmd == SET_MUTE_OFF)
	{
		pLinkInfo->linkPara.isSpkMuteForSCO = FALSE;
	}
	else if(muteCmd == TOGGLE_MUTE_ON_OFF)
	{
		pLinkInfo->linkPara.isSpkMuteForSCO = !pLinkInfo->linkPara.isSpkMuteForSCO;
	}

	APP_VolSco_SetScoSpkMuteOnOff(pBdAddr);
}

void APP_VolManage_Init(void)
{
	APP_VolManage_SetCurrentVpRtSoundLevel(APP_DEFAULT_VPRT_SOUND_LEVEL);
	#ifdef LINEIN_ENABLE
	APP_VolManage_SetCurrentLineInSoundLevel(APP_DEFAULT_LINE_IN_SOUND_LEVEL);
	#endif
	#ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL
	APP_VolManage_SetCurrentMp3SoundLevel(APP_DEFAULT_MP3_SOUND_LEVEL);
	#endif //MP3_LOCAL_PLAYBACK_MMI_CTRL
	APP_VolAiroThru_SetCurrentSoundLevel(APP_DEFAULT_AIROTHRU_SOUND_LEVEL);
}

void APP_VolManage_SpkVolumeUpDownHandler(BD_ADDR_T *pBdAddr, U16 keyCode, BOOL isFollowerCmd)
{
	BOOL isVolumeUp;

	#if !defined(HFP_Profile)
<<<<<<< HEAD
	#ifndef A2DP_Profile
=======
	#ifndef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
	UNUSED(pBdAddr);
	#endif
	#endif

	isVolumeUp = (keyCode == KEY_VOICEUP) ? TRUE : FALSE;
//	isVolumeUp	= (gMMI_driver_variation_nvram.misc_para.init.isMiscEnabled & IS_VOLUME_ORIENTATION_REVERSED) ? !isVolumeUp : isVolumeUp;
	switch(APP_GetCurrentAudioDevice())
	{
		case AUDIO_DEVICE_SCO:
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE            
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE            
>>>>>>> db20e11 (second commit)
		case AUDIO_DEVICE_SCO_HSP:
#endif            
		case AUDIO_DEVICE_SCO_IDLE:
		case AUDIO_DEVICE_KEY_AT_CMD:
			if(isFollowerCmd)
			{
				pBdAddr = APP_GetNextNormalLink(pBdAddr);
			}
			#ifdef HFP_Profile
			if(APP_Hfp_IsConnected(pBdAddr))
			{
				if(!APP_VolProcess_IsScoMicMute(pBdAddr))
				{
					APP_VolSco_VolumeUpDown(pBdAddr, isVolumeUp, isFollowerCmd);
				}
				else if(APP_MUTE_CANCEL_WHEN_VOL_CHANGE_FEAT)
				{
					APP_VolManage_AutoMICMuteOff(pBdAddr);
					APP_VolSco_VolumeUpDown(pBdAddr, isVolumeUp, isFollowerCmd);
				}
				else if(APP_ALLOW_VOL_ADJUST_WHEN_MUTED_FEAT)
				{
					APP_VolSco_VolumeUpDown(pBdAddr, isVolumeUp, isFollowerCmd);
				}
			}
			else
			#endif
			{
				APP_VolVpRt_VolumeUpDown(isVolumeUp);
			}
			break;
		case AUDIO_DEVICE_MUSIC:
		case AUDIO_DEVICE_MUSIC_CALL_NOTIFY:
		case AUDIO_DEVICE_AVRCP_PLAY:
<<<<<<< HEAD
			#ifdef A2DP_Profile
=======
			#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
			if(isFollowerCmd)
			{
				pBdAddr = APP_GetNextNormalLink(pBdAddr);
			}
			APP_VolA2dp_VolumeUpDown(pBdAddr, isVolumeUp, isFollowerCmd);
			#endif
			break;
		case AUDIO_DEVICE_LINE_IN:
		case AUDIO_DEVICE_MIC_TO_SPK:
			#ifdef LINEIN_ENABLE
			APP_VolLinein_VolumeUpDown(isVolumeUp);
			#endif
			break;
		case AUDIO_DEVICE_FM:
			#ifdef FM_ENABLE
//			MMI_FM_VolumeControl(isVolumeUp);
			#endif
			break;

		#ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL
		case AUDIO_DEVICE_MP3:
			APP_VolMp3_VolumeUpDown(isVolumeUp);
			break;
		#endif //MP3_LOCAL_PLAYBACK_MMI_CTRL

		default:
			APP_VolVpRt_VolumeUpDown(isVolumeUp);
			break;
	}
	if(APP_AiroThru_CheckFeatureEnable(APP_AIROTHRU_ENABLE_KEY_CHANGE_AIROTHRU_SOUND_LEVEL) &&
		APP_AudioDspIsStreaming(pBdAddr, AUDIO_ANC))
	{
		APP_VolAiroThru_VolumeUpDown(isVolumeUp);
	}
}

void APP_VolManage_SpkVolumeRemoteHandler(BD_ADDR_T *pBdAddr, U8 soundLevel, U8 component)
{
    DBG_LOG_APP_SYSTEM( "[VolumeManager]Remote Set Speaker soundlevel:%d, component:%s", 2, soundLevel, APP_VolComponentLogString[component]);
	switch(component)
	{
		case COMPONENT_A2DP:
			APP_VolA2dp_RemoteHandler(pBdAddr, soundLevel);
			break;
		case COMPONENT_SCO_WB:
		case COMPONENT_SCO_NB:
			APP_VolSco_RemoteHandler(pBdAddr, soundLevel, component);
			break;
		case COMPONENT_VPRT:
			UNUSED(pBdAddr);
			APP_VolVpRt_RemoteHandler(soundLevel);
			break;
		#ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL
		case COMPONENT_MP3:
			UNUSED(pBdAddr);
			APP_VolMp3_RemoteHandler(soundLevel);
			break;
		#endif

		#ifdef LINEIN_ENABLE
		case COMPONENT_LINEIN:
			UNUSED(pBdAddr);
			APP_VolLinein_RemoteHandler(soundLevel);
			break;
		#endif
	}
}

void APP_VolManage_MicVolumeUpDownHandler(BD_ADDR_T *pBdAddr, U16 keyCode)
{
	BOOL isVolumeUp;
	isVolumeUp = (keyCode == KEY_MICUP) ? TRUE : FALSE;

<<<<<<< HEAD
	#ifdef A2DP_Profile
=======
	#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
//	if(MUSIC_EXISTS(pBdAddr))
//	{
//		MMI_DRV_MicVolA2DPHandler(pBdAddr, isVolumeUp);
//
//	}
//	else
	#endif
	{
		if(APP_IsActiveAudioDevInCall())
		{
			APP_VolScoMic_VolumeUpDown(pBdAddr, isVolumeUp);
			APP_Media_PushMediaEvent(MEDIA_EVT_MIC_UPDN);
		}
//		else
//		{
//		}
	}
	//APP_Media_PushMediaEvent(MEDIA_EVT_MIC_UPDN);
}

void APP_VolManage_MicVolumeRemoteHandler(BD_ADDR_T *pBdAddr, U8 vgm, U8 component)
{
    DBG_LOG_APP_SYSTEM( "[VolumeManager]Remote Set Mic vgm:%d, component:%s", 2, vgm, APP_VolComponentLogString[component]);
	switch(component)
	{
		case COMPONENT_MIC_SCO:
			APP_VolScoMic_RemoteHandler(pBdAddr, vgm);
			break;
		case COMPONENT_MIC_A2DP:
			UNUSED(pBdAddr);
			break;
	}
}

void APP_VolManage_AutoMICMuteOff(BD_ADDR_T *pBdAddr)
{
	if(APP_VolProcess_IsScoMicMute(pBdAddr))
	{
		APP_Media_PushMediaEvent(MEDIA_EVT_KEY_MIC_MUTE_OFF);
		APP_VolManage_MicMuteHandler(pBdAddr, SET_MUTE_OFF);
	}
}

void APP_VolManage_MicMuteHandler(BD_ADDR_T *pBdAddr, U8 muteCmd)
{
	if(APP_IsSetAppLink(pBdAddr))
	{
		if (APP_ENABLE_SYNC_MUTE_MIC_TO_ALL_LINK_FEAT)
		{
			BD_ADDR_T *pOtherBdAddr = APP_GetNextNormalLink(pBdAddr);
			if(pOtherBdAddr)
			{
				app_VolManage_SetMicMuteByLink(pOtherBdAddr, muteCmd);
			}
		}
		app_VolManage_SetMicMuteByLink(pBdAddr, muteCmd);

		if(muteCmd == TOGGLE_MUTE_ON_OFF)
		{
			if (APP_VolProcess_IsScoMicMute(pBdAddr))
			{
				APP_Media_PushMediaEvent(MEDIA_EVT_KEY_MIC_MUTE_ON);
			}
			else
			{
				APP_Media_PushMediaEvent(MEDIA_EVT_KEY_MIC_MUTE_OFF);
			}
		}
	}
}

void APP_VolManage_SyncBeep(void)
{
	BD_ADDR_T *pBdAddr = APP_GetServiceBdAddr();
	if(APP_AudioDspIsStreaming(pBdAddr, AUDIO_A2DP))
	{
		APP_VolA2dp_SyncBeep(pBdAddr);
	}
	else if(APP_AudioDspIsStreaming(pBdAddr, AUDIO_SCO))
	{
		APP_VolSco_SyncBeep(pBdAddr);
	}
	#ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL
	else if(APP_AudioDspIsStreaming(pBdAddr, AUDIO_MP3))
	{
		APP_VolMp3_SyncBeep();
	}
	#endif
	else
	{
		APP_VolVpRt_SyncBeep();
	}

}

void APP_VolManage_SetCurrentScoSoundLevel(BD_ADDR_T *pBdAddr, U8 soundLevel)
{
	APP_VolSco_SetCurrentSoundLevel(pBdAddr, soundLevel);
}

void APP_VolManage_SetCurrentVgs(BD_ADDR_T *pBdAddr, U8 vgs)
{
	APP_VolSco_SetCurrentVgs(pBdAddr, vgs);
}

U8 APP_VolManage_GetCurrentScoSoundLevel(BD_ADDR_T *pBdAddr)
{
	return APP_VolSco_GetCurrentSoundLevel(pBdAddr);
}

U8 APP_VolManage_GetCurrentScoVgs(BD_ADDR_T *pBdAddr)
{
	return APP_VolSco_GetCurrentVgs(pBdAddr);
}

U8 APP_VolManage_GetScoSoundLevelByVgs(U8 vgs)
{
	return APP_VolSco_GetSoundLevelByVgs(vgs);
}

U8 APP_VolManage_GetVgsByScoSoundLevel(U8 soundLevel)
{
	return APP_VolSco_GetVgsBySoundLevel(soundLevel);
}

void APP_VolManage_SetCurrentA2dpSoundLevel(BD_ADDR_T *pBdAddr, U8 soundLevel)
{
	APP_VolA2dp_SetCurrentSoundLevel(pBdAddr, soundLevel);
}

void APP_VolManage_SetCurrentAbsoluteVolume(BD_ADDR_T *pBdAddr, U8 absVol)
{
	APP_VolA2dp_SetCurrentAbsVolume(pBdAddr, absVol);
}

U8 APP_VolManage_GetCurrentA2dpSoundLevel(BD_ADDR_T *pBdAddr)
{
	return APP_VolA2dp_GetCurrentSoundLevel(pBdAddr);
}

U8 APP_VolManage_GetCurrentAbsoluteVolume(BD_ADDR_T *pBdAddr)
{
	return APP_VolA2dp_GetCurrentAbsVolume(pBdAddr);
}

U8 APP_VolManage_GetSoundLevelByAbsoluteVolume(U8 absVol)
{
	return APP_VolA2dp_GetSoundLevelByAbsVolume(absVol);
}

U8 APP_VolManage_GetAbsoluteVolumeBySoundLevel(U8 soundLevel)
{
	return APP_VolA2dp_GetAbsVolumeBySoundLevel(soundLevel);
}

void APP_VolManage_SetCurrentScoMicSoundLevel(BD_ADDR_T *pBdAddr, U8 soundLevel)
{
	APP_VolScoMic_SetCurrentSoundLevel(pBdAddr, soundLevel);
}

void APP_VolManage_SetCurrentScoVgm(BD_ADDR_T *pBdAddr, U8 vgm)
{
	APP_VolScoMic_SetCurrentVgm(pBdAddr, vgm);
}

U8 APP_VolManage_GetCurrentScoMicSoundLevel(BD_ADDR_T *pBdAddr)
{
	return APP_VolScoMic_GetCurrentSoundLevel(pBdAddr);
}

U8 APP_VolManage_GetCurrentScoVgm(BD_ADDR_T *pBdAddr)
{
	return APP_VolScoMic_GetCurrentVgm(pBdAddr);
}

U8 APP_VolManage_GetScoMicSoundLevelByVgm(U8 vgm)
{
	return APP_VolScoMic_GetSoundLevelByVgm(vgm);
}

U8 APP_VolManage_GetVgmByScoMicSoundLevel(U8 soundLevel)
{
	return APP_VolScoMic_GetVgmBySoundLevel(soundLevel);
}

#ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL
void APP_VolManage_SetCurrentMp3SoundLevel(U8 soundLevel)
{
	APP_VolMp3_SetCurrentSoundLevel(soundLevel);
}

U8 APP_VolManage_GetCurrentMp3SoundLevel(void)
{
	return APP_VolMp3_GetCurrentSoundLevel();
}
#endif //MP3_LOCAL_PLAYBACK_MMI_CTRL

void APP_VolManage_SetCurrentVpRtSoundLevel(U8 soundLevel)
{
	APP_VolVpRt_SetCurrentSoundLevel(soundLevel);
}

U8 APP_VolManage_GetCurrentVpRtSoundLevel(void)
{
	return APP_VolVpRt_GetCurrentSoundLevel();
}

#ifdef LINEIN_ENABLE
void APP_VolManage_SetCurrentLineInSoundLevel(U8 soundLevel)
{
	APP_VolLinein_SetCurrentSoundLevel(soundLevel);
}

U8 APP_VolManage_GetCurrentLineInSoundLevel(void)
{
	return APP_VolLinein_GetCurrentSoundLevel();
}
#endif
