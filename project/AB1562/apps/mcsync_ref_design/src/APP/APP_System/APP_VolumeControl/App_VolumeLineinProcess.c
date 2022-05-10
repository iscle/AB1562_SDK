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
#ifdef LINEIN_ENABLE
#include "App_VolumeLineinProcess.h"
#endif
#include "App_VolumeSetting.h"
#include "App_Nvram_Type.h"
#include "App_Interface.h"
#include "App_Media.h"
#include "App_ResourceManager.h"
#include "App_VolumeManage.h"


#ifdef LINEIN_ENABLE
/**************************************************************************************************
* Define
**************************************************************************************************/
#define CURRENT_LINEIN_SOUND_LEVEL		gAppCtl.lineinSoundLevel

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static void app_VolLinein_SendVolumeToDSP(U8 soundLevel);

/**************************************************************************************************
* Static functions
**************************************************************************************************/
static void app_VolLinein_SendVolumeToDSP(U8 soundLevel)
{
	//if(DRIVER_IsMiscCtrlTypeEnabled(DRIVER_IS_LOW_BATTERY) && (soundLevel > APP_NvramType_SoundCtl_GetScoDecSoundLevelsBase()))
	//{
	//	soundLevel -= APP_NvramType_SoundCtl_GetScoDecSoundLevelsDec();
	//}
	if(APP_GetCurrentAudioDevice() == AUDIO_DEVICE_LINE_IN)
	{
		APP_VolSetting_SetSoundlevelToDSP(soundLevel, COMPONENT_LINEIN);
	}
}

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void APP_VolLinein_VolumeUpDown(BOOL isVolumeUp)
{
	U8 soundLevel;

	soundLevel = APP_VolLinein_GetCurrentSoundLevel();

	DBG_LOG_APP_SYSTEM( "[APP_LINEIN] Line in Volume Up:%d, CurSoundLevel:%d", 2, isVolumeUp, soundLevel);

	if(isVolumeUp)
	{
		if(soundLevel < APP_MAX_LINE_IN_SOUND_LEVEL)
		{
			(soundLevel)++;
		}
		else if(APP_LINE_IN_CYCLIC_VOLUME_ENABLED_FEAT)
		{
			DBG_LOG_APP_SYSTEM( "[APP_LINEIN] Line in cyclic volume up feature", 0);
			soundLevel = 0;
		}
		else
		{
			DBG_LOG_APP_SYSTEM( "[APP_LINEIN] Line in volume reach max volume", 0);
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
		else if(APP_LINE_IN_CYCLIC_VOLUME_ENABLED_FEAT)
		{
			DBG_LOG_APP_SYSTEM( "[APP_LINEIN] Line in volume down feature", 0);
			soundLevel = APP_MAX_LINE_IN_SOUND_LEVEL;
		}
		else
		{
			DBG_LOG_APP_SYSTEM( "[APP_LINEIN] Line in volume reach min volume", 0);
			APP_Media_PushMediaEvent(MEDIA_EVT_VOLUME_REACH_MIN);
			return;
		}
	}

	APP_VolLinein_SetCurrentSoundLevel(soundLevel);
	app_VolLinein_SendVolumeToDSP(soundLevel);
	APP_VolProcess_Beep(soundLevel, AUDIO_DEVICE_LINE_IN, TRUE);
}

void APP_VolLinein_RemoteHandler(U8 soundLevel)
{
	APP_VolLinein_SetCurrentSoundLevel(soundLevel);
	app_VolLinein_SendVolumeToDSP(soundLevel);
}

U8 APP_VolLinein_GetCurrentSoundLevel(void)
{
	return CURRENT_LINEIN_SOUND_LEVEL;
}

void APP_VolLinein_SetCurrentSoundLevel(U8 soundLevel)
{
	CURRENT_LINEIN_SOUND_LEVEL = soundLevel;
}

void APP_VolLinein_SetMuteOnOff(BOOL isMuteOn)
{
	isMuteOn ? APP_VolSetting_SetSpeakerMuteOn(SPKR_MUTE_MASK_LINEIN) : APP_VolSetting_SetSpeakerMuteOff(SPKR_MUTE_MASK_LINEIN);
}
#endif

