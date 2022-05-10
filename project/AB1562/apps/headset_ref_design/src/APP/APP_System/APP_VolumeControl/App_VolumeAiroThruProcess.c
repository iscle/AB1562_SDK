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
#include "App_VolumeAiroThruProcess.h"
#include "App_VolumeProcess.h"
#include "App_VolumeSetting.h"
#include "App_Interface.h"
#include "App_Nvram_Type.h"
#include "App_ResourceManager.h"
#include "App_System.h"

/**************************************************************************************************
* Define
**************************************************************************************************/
#define CURRENT_AIROTHRU_SOUND_LEVEL				gAppCtl.airoThruSoundLevel

/**************************************************************************************************
* Prototype
**************************************************************************************************/

/**************************************************************************************************
* Static Functions
**************************************************************************************************/


/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void APP_VolAiroThru_SendVolumeToDSP(U8 soundLevel)
{
    DBG_LOG_APP_SYSTEM( "[APP_AIROTHRU] AiroThru send vol to dsp soundlevel:%d", 1, soundLevel);

	APP_VolSetting_SetSoundlevelToDSP(soundLevel, COMPONENT_AIROTHRU_SPK);
	APP_VolSetting_SetSoundlevelToDSP(soundLevel, COMPONENT_AIROTHRU_MIC);
}

void APP_VolAiroThru_VolumeUpDown(BOOL isVolumeUp)
{
	U8 soundLevel;

	soundLevel = APP_VolAiroThru_GetCurrentSoundLevel();

    DBG_LOG_APP_SYSTEM( "[Volume]AiroThru Volup, CurSoundlevel:%d", 1, soundLevel);

	if(isVolumeUp)
	{
		if(soundLevel < APP_MAX_AIROTHRU_SOUND_LEVEL)
		{
			(soundLevel)++;
		}
		else if(APP_CYCLIC_VOLUME_ENABLED_FEAT)
		{
            DBG_LOG_APP_SYSTEM( "[Volume]AiroThru Cycle Volume isUp:%d", 1, isVolumeUp);
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
            DBG_LOG_APP_SYSTEM( "[Volume]AiroThru Cycle Volume isUp:%d", 1, isVolumeUp);
			soundLevel = APP_MAX_AIROTHRU_SOUND_LEVEL;
		}
		else
		{
            DBG_LOG_APP_SYSTEM( "[Volume]Vol reach Min", 0);
			return;
		}
	}

	APP_VolAiroThru_SetCurrentSoundLevel(soundLevel);
	APP_VolAiroThru_SendVolumeToDSP(soundLevel);
}

U8 APP_VolAiroThru_GetCurrentSoundLevel(void)
{
	return CURRENT_AIROTHRU_SOUND_LEVEL;
}

void APP_VolAiroThru_SetCurrentSoundLevel(U8 soundLevel)
{
    DBG_LOG_APP_SYSTEM( "[APP_AIROTHRU] Set AiroThru Sound Level: %d", 1, soundLevel);
	CURRENT_AIROTHRU_SOUND_LEVEL = soundLevel;
}
