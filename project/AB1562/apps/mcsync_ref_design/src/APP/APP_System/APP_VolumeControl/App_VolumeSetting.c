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
#include "App_VolumeSetting.h"
#include "Volume_NvkeyHandler.h"
#include "DSP_Control.h"
#include "AudioDSP_Registration.h"
#include "AudioDSP_StreamManager.h"
#include "App_ResourceManager.h"
#include "App_MCSync_Message.h"
#include "App_System.h"
#include "App_Interface.h"
#include "chargercase_common.h"
#include "ChargerSmartCase.h"

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static void app_VolSetting_OutLevelUpdate(U8 component, U8 percentage);
static void app_VolSetting_InLevelUpdate(U8 component, U8 percentage);

/**************************************************************************************************
* Static Functions
**************************************************************************************************/
static void app_VolSetting_OutLevelUpdate(U8 component, U8 percentage)
{
	if((component != AUDIO_GAIN_VP && component != AUDIO_GAIN_RT) || APP_IsAbleToLoadVpRtGain())
	{
		DSP_GC_SetAnalogOutLevel(percentage);
		DSP_GC_LoadAnalogOutLevelToAfe();
	}
	DSP_GC_SetDigitalOutLevel(component, percentage);
}

static void app_VolSetting_InLevelUpdate(U8 component, U8 percentage)
{
	DSP_GC_SetAfeGainLevelByPercentage(percentage);
	DSP_GC_LoadDigitalInLevelToDfe();
	#if 0
	DSP_GC_LoadAnalogInLevelToAfeConcurrently();
	#else
	DSP_GC_LoadAnalogInLevelToAfeByScenario(component);
	#endif
}

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void APP_VolSetting_LoadScoWBGainTable(void)
{
	DSP_GC_SetDefaultLevelByGainTable(AUDIO_GAIN_SCO);

	DSP_GC_LoadDefaultGainToAfe(AUDIO_GAIN_SCO);
}

void APP_VolSetting_LoadScoNBGainTable(void)
{
	DSP_GC_SetDefaultLevelByGainTable(AUDIO_GAIN_SCO_NB);

	DSP_GC_LoadDefaultGainToAfe(AUDIO_GAIN_SCO_NB);
}

void APP_VolSetting_LoadLineinGainTable(void)
{
	DSP_GC_SetDefaultLevelByGainTable(AUDIO_GAIN_LINE);
	DSP_GC_LoadDefaultGainToAfe(AUDIO_GAIN_LINE);
}

void APP_VolSetting_LoadA2dpGainTable(void)
{
	DSP_GC_SetAnalogOutScaleByGainTable(AUDIO_GAIN_A2DP);
	DSP_GC_LoadDefaultGainToAfe(AUDIO_GAIN_A2DP);
}

void APP_VolSetting_LoadVCGainTable(void)
{
    DBG_LOG_APP_SYSTEM( "[Volume]Load VC gain table", 0);
	DSP_GC_SetDefaultLevelByGainTable(AUDIO_GAIN_VC);
	DSP_GC_SetAnalogInLevelByGainTable(AUDIO_GAIN_VC);
	DSP_GC_SetDigitalInLevelByGainTable(AUDIO_GAIN_VC);
	DSP_GC_LoadDefaultGainToAfe(AUDIO_GAIN_VC);
}

void APP_VolSetting_LoadVPGainTable(void)
{
	DSP_GC_SetAnalogOutScaleByGainTable(AUDIO_GAIN_VP);
	DSP_GC_LoadDefaultGainToAfe(AUDIO_GAIN_VP);
}

void APP_VolSetting_LoadRTGainTable(void)
{
	DSP_GC_SetAnalogOutScaleByGainTable(AUDIO_GAIN_RT);
	DSP_GC_LoadDefaultGainToAfe(AUDIO_GAIN_RT);
}

void APP_VolSetting_LoadATGainTable(void)
{
	DBG_LOG_APP_SYSTEM( "[Volume]Load AT gain table", 0);
	DSP_GC_SetDefaultLevelByGainTable(AUDIO_GAIN_AT);
	DSP_GC_SetAnalogInLevelByGainTable(AUDIO_GAIN_AT);
	DSP_GC_SetDigitalInLevelByGainTable(AUDIO_GAIN_AT);
	DSP_GC_LoadDefaultGainToAfe(AUDIO_GAIN_AT);
}

void APP_VolSetting_SetSoundlevelToDSP(U8 soundLevel, U8 component)
{

	if((component < COMPONENT_NUMBER) && (soundLevel > APP_GET_MAX_SOUND_LEVEL(component)))
	{
			DBG_LOG_APP_SYSTEM( "[Volume] Error! soundLevel(%d) > MAX_SOUND_LEVEL(%d)", 2, soundLevel, APP_GET_MAX_SOUND_LEVEL(component));
			soundLevel = APP_GET_MAX_SOUND_LEVEL(component);
	}

	DBG_LOG_APP_SYSTEM( "[Volume] Set Volume to DSP SoundLevel:%d, Percentage:%d, Component:%s", 3, soundLevel, APP_VolumeNvkey_GetPercentageBySoundLevel(component, soundLevel), APP_VolComponentLogString[component]);

	switch(component)
	{
		case COMPONENT_A2DP:
			app_VolSetting_OutLevelUpdate(AUDIO_GAIN_A2DP, APP_VolumeNvkey_GetPercentageBySoundLevel(component, soundLevel));
			break;
		#ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL
		case COMPONENT_MP3:
			app_VolSetting_OutLevelUpdate(AUDIO_GAIN_A2DP, APP_VolumeNvkey_GetPercentageBySoundLevel(component, soundLevel));
			break;
		#endif
		case COMPONENT_SCO_WB:
			app_VolSetting_OutLevelUpdate(AUDIO_GAIN_SCO, APP_VolumeNvkey_GetPercentageBySoundLevel(component, soundLevel));
			break;
		case COMPONENT_SCO_NB:
			app_VolSetting_OutLevelUpdate(AUDIO_GAIN_SCO_NB, APP_VolumeNvkey_GetPercentageBySoundLevel(component, soundLevel));
			break;
		case COMPONENT_VPRT:
			app_VolSetting_OutLevelUpdate(APP_AudioDspIsStreaming(NULL, AUDIO_VP) ? AUDIO_GAIN_VP : AUDIO_GAIN_RT, APP_VolumeNvkey_GetPercentageBySoundLevel(component, soundLevel));
			break;
		#ifdef LINEIN_ENABLE
		case COMPONENT_LINEIN:
			app_VolSetting_OutLevelUpdate(AUDIO_GAIN_LINE, APP_VolumeNvkey_GetPercentageBySoundLevel(component, soundLevel));
			break;
		#endif
		case COMPONENT_MIC_SCO:
			app_VolSetting_InLevelUpdate(AUDIO_GAIN_SCO, APP_VolumeNvkey_GetPercentageBySoundLevel(component, soundLevel));
			break;
		case COMPONENT_AIROTHRU_SPK:
			DSP_GC_SetDigitalOutLevel(AUDIO_GAIN_AT, APP_VolumeNvkey_GetPercentageBySoundLevel(component, soundLevel));
			break;
		case COMPONENT_AIROTHRU_MIC:
			DSP_GC_UpdateAfeGains(AUDIO_GAIN_AT, APP_VolumeNvkey_GetPercentageBySoundLevel(component, soundLevel));
			break;
		default:
			break;
	}
}

void APP_VolSetting_SetSpeakerMuteOn(U32 mask)
{
	U32 muteMask = APP_GetSpeakerMuteMask();

	DBG_LOG_APP_SYSTEM("[Volume] Speaker Mute On, setMask:%d, currentMask:%d", 2, mask, muteMask);
	if(!muteMask)
		DSP_GC_MuteAudioSink(TRUE);

    if(mask != SPKR_MUTE_MASK_CHG_CASE)
        APP_MCSYNC_SendSyncSpeakerMute(TRUE);

	APP_SetSpeakerMuteMask(mask);
}

void APP_VolSetting_SetSpeakerMuteOff(U32 mask)
{
	U32 muteMask;

	APP_RmvSpeakerMuteMask(mask);
	muteMask = APP_GetSpeakerMuteMask();
	DBG_LOG_APP_SYSTEM("[Volume] Speaker Mute Off, setMask:%d, afterMask:%d", 2, mask, muteMask);

	if(!muteMask)
	{
		//DBG_LOG_APP_SYSTEM("[Volume] chargeState:%d, smartCaseState:%d", 2, ChargerSmartCase_GetChargerState(), ChargerSmartCase_GetSmartCaseState());
		if(mask != SPKR_MUTE_MASK_CHG_CASE)
			APP_MCSYNC_SendSyncSpeakerMute(FALSE);
		if((ChargerSmartCase_GetChargerState() == STATE_CHARGER_OUT)
			&&(ChargerSmartCase_GetSmartCaseState() == STATE_OUT_OF_CASE))
			DSP_GC_MuteAudioSink(FALSE);
	}
}

void APP_VolSetting_SetMicMuteOn(void)
{
	DSP_GC_MuteAudioSource(TRUE);
	DBG_LOG_APP_SYSTEM( "[    APP][VolumeSetting]Mic Mute On", 0);
}

void APP_VolSetting_SetMicMuteOff(void)
{
	DSP_GC_MuteAudioSource(FALSE);
	DBG_LOG_APP_SYSTEM( "[    APP][VolumeSetting]Mic Mute Off", 0);
}
