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
#include "App_VolumeMp3Process.h"
#include "App_VolumeProcess.h"
#include "App_VolumeSetting.h"
#include "App_ResourceManager.h"
#include "AudioDSP_StreamManager.h"
#include "App_Nvram_Type.h"
#include "App_Interface.h"
#include "App_Media.h"
#include "App_System.h"

#ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL
/**************************************************************************************************
* Define
**************************************************************************************************/
#define CURRENT_MP3_SOUND_LEVEL				gAppCtl.mp3SoundLevel

/**************************************************************************************************
* Prototype
**************************************************************************************************/
void app_VolMp3_SendVolumeToDSP(U8 soundLevel);

/**************************************************************************************************
* Static Functions
**************************************************************************************************/
void app_VolMp3_SendVolumeToDSP(U8 soundLevel)
{
	//if(DRIVER_IsMiscCtrlTypeEnabled(DRIVER_IS_LOW_BATTERY) && (soundLevel > APP_NvramType_SoundCtl_GetA2doDecSoundLevelsBase()))
	//{
	//	soundLevel -= APP_NvramType_SoundCtl_GetA2dpDecSoundLevelsDec();
	//}

	APP_VolSetting_SetSoundlevelToDSP(soundLevel, COMPONENT_MP3);
}

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void APP_VolMp3_VolumeUpDown(BOOL isVolumeUp)
{
	U8 soundLevel;

	soundLevel = APP_VolMp3_GetCurrentSoundLevel();

	DBG_LOG_APP_SYSTEM( "[Volume]MP3 Volup:%d, CurSoundlevel:%d", 1, soundLevel);

	if(isVolumeUp)
	{
		if(soundLevel < APP_MAX_MP3_SOUND_LEVEL)
		{
			(soundLevel)++;
		}
		else if(APP_CYCLIC_VOLUME_ENABLED_FEAT)
		{
			DBG_LOG_APP_SYSTEM( "[APP_MP3] MP3 cyclic volume up feature", 0);
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
			DBG_LOG_APP_SYSTEM( "[APP_MP3] MP3 cyclic volume down feature", 0);
			soundLevel = APP_MAX_MP3_SOUND_LEVEL;
		}
		else
		{
			DBG_LOG_APP_SYSTEM( "[Volume]Vol reach Min", 0);
			APP_Media_PushMediaEvent(MEDIA_EVT_VOLUME_REACH_MIN);
			return;
		}
	}

	APP_VolMp3_SetCurrentSoundLevel(soundLevel);
	app_VolMp3_SendVolumeToDSP(soundLevel);

	//Beep!
	APP_VolProcess_Beep(soundLevel, AUDIO_DEVICE_MP3);
}

void APP_VolMp3_RemoteHandler(U8 soundLevel)
{
	APP_VolMp3_SetCurrentSoundLevel(soundLevel);
	app_VolMp3_SendVolumeToDSP(soundLevel);
}

void APP_VolMp3_SetCurrentSoundLevel(U8 soundLevel)
{
	CURRENT_MP3_SOUND_LEVEL = soundLevel;
}

U8 APP_VolMp3_GetCurrentSoundLevel(void)
{
	return CURRENT_MP3_SOUND_LEVEL;
}
#endif//MP3_LOCAL_PLAYBACK_MMI_CTRL
