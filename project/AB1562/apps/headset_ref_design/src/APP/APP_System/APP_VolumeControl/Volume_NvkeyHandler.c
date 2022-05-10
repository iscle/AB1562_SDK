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
#include "Volume_NvkeyHandler.h"
#include "Volume_Nvkey.h"
#include "nvkey_hdl.h"

/**************************************************************************************************
* Define
**************************************************************************************************/
// Transfer to NVKEYID_APP_AIROTHRU_SOUND_LEVEL_SETTING

/**************************************************************************************************
* Variables
**************************************************************************************************/
static VOLUME_SCO_SETTING_STRU *pScoSetting;
static VOLUME_A2DP_SETTING_STRU *pA2dpSetting;
static VOLUME_SCO_MIC_SETTING_STRU *pScoMicSetting;
static VOLUME_VPRT_SETTING_STRU *pVpRtSetting;
#ifdef LINEIN_ENABLE
static VOLUME_LINEIN_SETTING_STRU *pLineinSetting;
#endif
#ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL
static VOLUME_MP3_SETTING_STRU *pMp3Setting;
#endif
static VOLUME_AIROTHRU_SETTING_STRU *pAiroThruSetting;
static VOLUME_INFOR_STRU *pVolumeInfo;

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static void app_VolumeNvkey_ReadScoConfig(void);
static void app_VolumeNvkey_ReadA2dpConfig(void);
static void app_VolumeNvkey_ReadScoMicConfig(void);
static void app_VolumeNvkey_ReadVpRtConfig(void);
#ifdef LINEIN_ENABLE
static void app_VolumeNvkey_ReadLineinConfig(void);
#endif
#ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL
static void app_VolumeNvkey_ReadMp3Config(void);
#endif //MP3_LOCAL_PLAYBACK_MMI_CTRL
static void app_VolumeNvkey_ReadAiroThruConfig(void);
static void app_VolumeNvkey_ReadVolumeInfoConfig(void);
static U8 app_VolumeNvkey_GetScoVgs(U8 soundLevel);
static U8 app_VolumeNvkey_GetScoMicVgm(U8 soundLevel);
static U8 app_VolumeNvkey_GetScoBeepTone(U8 soundLevel);
static U8 app_VolumeNvkey_GetA2dpBeepTone(U8 soundLevel);
#ifdef LINEIN_ENABLE
static U8 app_VolumeNvkey_GetLineinBeepTone(U8 soundLevel);
#endif
static U8 app_VolumeNvkey_GetScoPercentage(U8 soundLevel);
static U8 app_VolumeNvkey_GetA2dpPercentage(U8 soundLevel);
#ifdef LINEIN_ENABLE
static U8 app_VolumeNvkey_GetLineinPercentage(U8 soundLevel);
#endif
static U8 app_VolumeNvkey_GetScoMicPercentage(U8 soundLevel);
static U8 app_VolumeNvkey_GetVpRtPercentage(U8 soundLevel);
#ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL
static U8 app_VolumeNvkey_GetMp3Percentage(U8 soundLevel);
#endif
static U8 app_VolumeNvkey_GetAvrcpVolume(U8 soundLevel);
static U8 app_VolumeNvkey_GetAiroThruSpkPercentage(U8 soundLevel);
static U8 app_VolumeNvkey_GetAiroThruMicPercentage(U8 soundLevel);

/**************************************************************************************************
* Static functions
**************************************************************************************************/
static void app_VolumeNvkey_ReadScoConfig(void)
{
	U8* pAddr;
	U8 dataSize;

	pAddr = (U8*)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_VOLUME_SCO_SETTING);
	dataSize = NVKEY_GetPayloadLength(NVKEYID_APP_VOLUME_SCO_SETTING);

	if(pAddr == NULL || dataSize <= 0)
	{
		FW_Assert(FALSE);
	}
	else
	{
		pScoSetting = (VOLUME_SCO_SETTING_STRU *)pAddr;
	}

	return ;
}

static void app_VolumeNvkey_ReadA2dpConfig(void)
{
	U8* pAddr;
	U8 dataSize;

	pAddr = (U8*)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_VOLUME_A2DP_SETTING);
	dataSize = NVKEY_GetPayloadLength(NVKEYID_APP_VOLUME_A2DP_SETTING);

	if(pAddr == NULL || dataSize <= 0)
	{
		FW_Assert(FALSE);
	}
	else
	{
		pA2dpSetting = (VOLUME_A2DP_SETTING_STRU *)pAddr;
	}

	return ;
}


static void app_VolumeNvkey_ReadScoMicConfig(void)
{
	U8* pAddr;
	U8 dataSize;

	pAddr = (U8*)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_VOLUME_SCOMIC_SETTING);
	dataSize = NVKEY_GetPayloadLength(NVKEYID_APP_VOLUME_SCOMIC_SETTING);

	if(pAddr == NULL || dataSize <= 0)
	{
		FW_Assert(FALSE);
	}
	else
	{
		pScoMicSetting = (VOLUME_SCO_MIC_SETTING_STRU *)pAddr;
	}

	return ;
}


static void app_VolumeNvkey_ReadVpRtConfig(void)
{
	U8* pAddr;
	U8 dataSize;

	pAddr = (U8*)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_VOLUME_VPRT_SETTING);
	dataSize = NVKEY_GetPayloadLength(NVKEYID_APP_VOLUME_VPRT_SETTING);

	if(pAddr == NULL || dataSize <= 0)
	{
		FW_Assert(FALSE);
	}
	else
	{
		pVpRtSetting = (VOLUME_VPRT_SETTING_STRU *)pAddr;
	}

	return ;
}

#ifdef LINEIN_ENABLE
static void app_VolumeNvkey_ReadLineinConfig(void)
{
	U8* pAddr;
	U8 dataSize;

	pAddr = (U8*)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_VOLUME_LINEIN_SETTING);
	dataSize = NVKEY_GetPayloadLength(NVKEYID_APP_VOLUME_LINEIN_SETTING);

	if(pAddr == NULL || dataSize <= 0)
	{
		FW_Assert(FALSE);
	}
	else
	{
		 pLineinSetting = (VOLUME_LINEIN_SETTING_STRU *)pAddr;
	}

	return ;
}
#endif

#ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL
static void app_VolumeNvkey_ReadMp3Config(void)
{
	U8* pAddr;
	U8 dataSize;

	pAddr = (U8*)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_VOLUME_MP3_SETTING);
	dataSize = NVKEY_GetPayloadLength(NVKEYID_APP_VOLUME_MP3_SETTING);

	if(pAddr == NULL || dataSize <= 0)
	{
		FW_Assert(FALSE);
	}
	else
	{
		 pMp3Setting = (VOLUME_MP3_SETTING_STRU *)pAddr;
	}

	return ;
}
#endif

static void app_VolumeNvkey_ReadAiroThruConfig(void)
{
	U8* pAddr;
	U8 dataSize;

	pAddr = (U8*)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_AIROTHRU_SOUND_LEVEL_SETTING);
	dataSize = NVKEY_GetPayloadLength(NVKEYID_APP_AIROTHRU_SOUND_LEVEL_SETTING);

	if(pAddr == NULL || dataSize <= 0)
	{
		FW_Assert(FALSE);
	}
	else
	{
		 pAiroThruSetting = (VOLUME_AIROTHRU_SETTING_STRU *)pAddr;
	}

	return ;
}

static void app_VolumeNvkey_ReadVolumeInfoConfig(void)
{
	U8* pAddr;
	U8 dataSize;

	pAddr = (U8*)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_VOLUME_INFORMATION);
	dataSize = NVKEY_GetPayloadLength(NVKEYID_APP_VOLUME_INFORMATION);

	if(pAddr == NULL || dataSize <= 0)
	{
		FW_Assert(FALSE);
	}
	else
	{
		 pVolumeInfo = (VOLUME_INFOR_STRU *)pAddr;
	}

	return ;
}

static U8 app_VolumeNvkey_GetScoVgs(U8 soundLevel)
{
	U8 *pAddr = (U8 *)pScoSetting;

	pAddr += sizeof(APP_VOL_PARA_STRU);    //scoVolPara

	return *(pAddr + soundLevel);
}

static U8 app_VolumeNvkey_GetScoMicVgm(U8 soundLevel)
{
	U8 *pAddr = (U8 *)pScoMicSetting;

	pAddr += sizeof(APP_VOL_PARA_STRU);    //scoMicVolPara

	return *(pAddr + soundLevel);
}

static U8 app_VolumeNvkey_GetScoBeepTone(U8 soundLevel)
{
	U8 *pAddr = (U8 *)pScoSetting;
	U8 totalSoundLevel = *(pAddr);

	pAddr += sizeof(APP_VOL_PARA_STRU);    //scoVolPara
	pAddr += sizeof(U8) * totalSoundLevel; //scoSoundLevelToVgs

	return *(pAddr + soundLevel);
}

static U8 app_VolumeNvkey_GetA2dpBeepTone(U8 soundLevel)
{
	U8 *pAddr = (U8 *)pA2dpSetting;
	U8 totalSoundLevel = *(pAddr);

	pAddr += sizeof(APP_VOL_PARA_STRU);    //a2dpVolPara
	pAddr += sizeof(U8) * totalSoundLevel; //a2dpSoundlevelToAvrcpVolume

	return *(pAddr + soundLevel);
}

#ifdef LINEIN_ENABLE
static U8 app_VolumeNvkey_GetLineinBeepTone(U8 soundLevel)
{
	U8 *pAddr = (U8 *)pLineinSetting;

	pAddr += sizeof(APP_VOL_PARA_STRU);    //lineinVolPara

	return *(pAddr + soundLevel);
}
#endif

static U8 app_VolumeNvkey_GetScoPercentage(U8 soundLevel)
{
	U8 *pAddr = (U8 *)pScoSetting;
	U8 totalSoundLevel = *(pAddr);

	pAddr += sizeof(APP_VOL_PARA_STRU);    //scoVolPara
	pAddr += sizeof(U8) * totalSoundLevel; //scoSoundLevelToVgs
	pAddr += sizeof(U8) * totalSoundLevel; //scoSoundLevelToBeepTone

	return *(pAddr + soundLevel);
}

static U8 app_VolumeNvkey_GetA2dpPercentage(U8 soundLevel)
{
	U8 *pAddr = (U8 *)pA2dpSetting;
	U8 totalSoundLevel = *(pAddr);

	pAddr += sizeof(APP_VOL_PARA_STRU);    //a2dpVolPara
	pAddr += sizeof(U8) * totalSoundLevel; //a2dpSoundlevelToAvrcpVolume
	pAddr += sizeof(U8) * totalSoundLevel; //musicSoundLevelToBeepTone

	return *(pAddr + soundLevel);
}

#ifdef LINEIN_ENABLE
static U8 app_VolumeNvkey_GetLineinPercentage(U8 soundLevel)
{
	U8 *pAddr = (U8 *)pLineinSetting;
	U8 totalSoundLevel = *(pAddr);

	pAddr += sizeof(APP_VOL_PARA_STRU);    //lineinVolPara
	pAddr += sizeof(U8) * totalSoundLevel; //lineInSoundLevelToBeepTone

	return *(pAddr + soundLevel);
}
#endif

static U8 app_VolumeNvkey_GetScoMicPercentage(U8 soundLevel)
{
	U8 *pAddr = (U8 *)pScoMicSetting;
	U8 totalSoundLevel = *(pAddr);

	pAddr += sizeof(APP_VOL_PARA_STRU);    //scoMicVolPara
	pAddr += sizeof(U8) * totalSoundLevel; //scoMicSoundLevelToVgm

	return *(pAddr + soundLevel);
}

static U8 app_VolumeNvkey_GetVpRtPercentage(U8 soundLevel)
{
	U8 *pAddr = (U8 *)pVpRtSetting;

	pAddr += sizeof(APP_VOL_PARA_STRU);    //vprtVolPara

	return *(pAddr + soundLevel);
}

#ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL
static U8 app_VolumeNvkey_GetMp3Percentage(U8 soundLevel)
{
	U8 *pAddr = (U8 *)pMp3Setting;

	pAddr += sizeof(APP_VOL_PARA_STRU);    //mp3VolPara

	return *(pAddr + soundLevel);
}
#endif //MP3_LOCAL_PLAYBACK_MMI_CTRL

static U8 app_VolumeNvkey_GetAvrcpVolume(U8 soundLevel)
{
	U8 *pAddr = (U8 *)pA2dpSetting;

	pAddr += sizeof(APP_VOL_PARA_STRU);    //a2dpVolPara

	return *(pAddr + soundLevel);
}

static U8 app_VolumeNvkey_GetAiroThruSpkPercentage(U8 soundLevel)
{
	U8 *pAddr = (U8 *)pAiroThruSetting;
	U8 totalSoundLevel = *(pAddr);

	pAddr += sizeof(APP_VOL_PARA_STRU);    //airoThruVolPara
	pAddr += sizeof(U8) * totalSoundLevel; //airoThruMiclevelToPercentage

	return *(pAddr + soundLevel);
}

static U8 app_VolumeNvkey_GetAiroThruMicPercentage(U8 soundLevel)
{
	U8 *pAddr = (U8 *)pAiroThruSetting;

	pAddr += sizeof(APP_VOL_PARA_STRU);    //airoThruVolPara

	return *(pAddr + soundLevel);
}

static void app_VolumeNvkey_SetAiroThruDefaultSoundLevel(U8 soundLevel)
{
	VOLUME_AIROTHRU_SETTING_STRU *pData;
	U16 dataSize = sizeof(APP_VOL_PARA_STRU) + pAiroThruSetting->airoThruVolPara.totalSoundLevel * (sizeof(U8) + sizeof(U8));

	if((pData = FW_GetMemory(dataSize)) != NULL)
	{
		FW_Memcpy(pData, pAiroThruSetting, dataSize);
		pData->airoThruVolPara.defaultSoundLevel = soundLevel;
		nvkey_hdl_save_nvkey(NVKEYID_APP_AIROTHRU_SOUND_LEVEL_SETTING, dataSize, pData);
		FW_FreeMemory(pData);
	}
}

/**************************************************************************************************
* Public functions
**************************************************************************************************/
void APP_VolumeNvkey_Init(void)
{
	app_VolumeNvkey_ReadScoConfig();
	app_VolumeNvkey_ReadA2dpConfig();
	app_VolumeNvkey_ReadScoMicConfig();
	app_VolumeNvkey_ReadVpRtConfig();
	#ifdef LINEIN_ENABLE
	app_VolumeNvkey_ReadLineinConfig();
	#endif
	#ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL
	app_VolumeNvkey_ReadMp3Config();
	#endif //MP3_LOCAL_PLAYBACK_MMI_CTRL
	app_VolumeNvkey_ReadAiroThruConfig();
	app_VolumeNvkey_ReadVolumeInfoConfig();
}

U8 APP_VolumeNvkey_GetScoVgsBySoundLevel(U8 soundLevel)
{
	return app_VolumeNvkey_GetScoVgs(soundLevel);
}

U8 APP_VolumeNvkey_GetScoMicVgmBySoundLevel(U8 soundLevel)
{
	return app_VolumeNvkey_GetScoMicVgm(soundLevel);
}

U8 APP_VolumeNvkey_GetBeepToneBySoundLevel(U8 component, U8 soundLevel)
{
	switch(component)
	{
		case COMPONENT_SCO_WB:
		case COMPONENT_SCO_NB:
			return app_VolumeNvkey_GetScoBeepTone(soundLevel);
		case COMPONENT_A2DP:
			return app_VolumeNvkey_GetA2dpBeepTone(soundLevel);
		#ifdef LINEIN_ENABLE
		case COMPONENT_LINEIN:
			return app_VolumeNvkey_GetLineinBeepTone(soundLevel);
		#endif
		default:
			FW_Assert(FALSE);
			break;
	}
	return 0;
}

U8 APP_VolumeNvkey_GetSoundLevelsBase(U8 component)
{
	switch(component)
	{
		case COMPONENT_SCO_WB:
		case COMPONENT_SCO_NB:
			return pScoSetting->scoVolPara.DecreaseSoundLevelCtl.baseSoundLevel;
		case COMPONENT_MIC_SCO:
			return pScoMicSetting->scoMicVolPara.DecreaseSoundLevelCtl.baseSoundLevel;
		case COMPONENT_A2DP:
			return pA2dpSetting->a2dpVolPara.DecreaseSoundLevelCtl.baseSoundLevel;
		#ifdef LINEIN_ENABLE
		case COMPONENT_LINEIN:
			return pLineinSetting->lineinVolPara.DecreaseSoundLevelCtl.baseSoundLevel;
		#endif
		case COMPONENT_VPRT:
			return pVpRtSetting->vprtVolPara.DecreaseSoundLevelCtl.baseSoundLevel;
		#ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL
		case COMPONENT_MP3:
			return pMp3Setting->mp3VolPara.DecreaseSoundLevelCtl.baseSoundLevel;
		#endif
		case COMPONENT_AIROTHRU_SPK:
		case COMPONENT_AIROTHRU_MIC:
			return pAiroThruSetting->airoThruVolPara.DecreaseSoundLevelCtl.baseSoundLevel;
		default:
			FW_Assert(FALSE);
			break;
	}
	return 0;
}

U8 APP_VolumeNvkey_GetSoundLevelsDec(U8 component)
{
	switch(component)
	{
		case COMPONENT_SCO_WB:
		case COMPONENT_SCO_NB:
			return pScoSetting->scoVolPara.DecreaseSoundLevelCtl.decreaseSoundLevel;
		case COMPONENT_MIC_SCO:
			return pScoMicSetting->scoMicVolPara.DecreaseSoundLevelCtl.decreaseSoundLevel;
		case COMPONENT_A2DP:
			return pA2dpSetting->a2dpVolPara.DecreaseSoundLevelCtl.decreaseSoundLevel;
		#ifdef LINEIN_ENABLE
		case COMPONENT_LINEIN:
			return pLineinSetting->lineinVolPara.DecreaseSoundLevelCtl.decreaseSoundLevel;
		#endif
		case COMPONENT_VPRT:
			return pVpRtSetting->vprtVolPara.DecreaseSoundLevelCtl.decreaseSoundLevel;
		#ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL
		case COMPONENT_MP3:
			return pMp3Setting->mp3VolPara.DecreaseSoundLevelCtl.decreaseSoundLevel;
		#endif
		case COMPONENT_AIROTHRU_SPK:
		case COMPONENT_AIROTHRU_MIC:
			return pAiroThruSetting->airoThruVolPara.DecreaseSoundLevelCtl.decreaseSoundLevel;
		default:
			FW_Assert(FALSE);
			break;
	}
	return 0;
}

U8 APP_VolumeNvkey_GetTotalSoundLevel(U8 component)
{
	switch(component)
	{
		case COMPONENT_SCO_WB:
		case COMPONENT_SCO_NB:
			return pScoSetting->scoVolPara.totalSoundLevel;
		case COMPONENT_MIC_SCO:
			return pScoMicSetting->scoMicVolPara.totalSoundLevel;
		case COMPONENT_A2DP:
			return pA2dpSetting->a2dpVolPara.totalSoundLevel;
		#ifdef LINEIN_ENABLE
		case COMPONENT_LINEIN:
			return pLineinSetting->lineinVolPara.totalSoundLevel;
		#endif
		case COMPONENT_VPRT:
			return pVpRtSetting->vprtVolPara.totalSoundLevel;
		#ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL
		case COMPONENT_MP3:
			return pMp3Setting->mp3VolPara.totalSoundLevel;
		#endif //MP3_LOCAL_PLAYBACK_MMI_CTRL
		case COMPONENT_AIROTHRU_SPK:
		case COMPONENT_AIROTHRU_MIC:
			return pAiroThruSetting->airoThruVolPara.totalSoundLevel;
		default:
			FW_Assert(FALSE);
			break;
	}
	return 0;
}

U8 APP_VolumeNvkey_GetDefaultSoundLevel(U8 component)
{
	switch(component)
	{
		case COMPONENT_SCO_WB:
		case COMPONENT_SCO_NB:
			return pScoSetting->scoVolPara.defaultSoundLevel;
		case COMPONENT_MIC_SCO:
			return pScoMicSetting->scoMicVolPara.defaultSoundLevel;
		case COMPONENT_A2DP:
			return pA2dpSetting->a2dpVolPara.defaultSoundLevel;
		#ifdef LINEIN_ENABLE
		case COMPONENT_LINEIN:
			return pLineinSetting->lineinVolPara.defaultSoundLevel;
		#endif
		case COMPONENT_VPRT:
			return pVpRtSetting->vprtVolPara.defaultSoundLevel;
		#ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL
		case COMPONENT_MP3:
			return pMp3Setting->mp3VolPara.defaultSoundLevel;
		#endif
		case COMPONENT_AIROTHRU_SPK:
		case COMPONENT_AIROTHRU_MIC:
			return pAiroThruSetting->airoThruVolPara.defaultSoundLevel;
		default:
			FW_Assert(FALSE);
			break;
	}
	return 0;
}

U8 APP_VolumeNvkey_GetPercentageBySoundLevel(U8 component, U8 soundLevel)
{
	switch(component)
	{
		case COMPONENT_SCO_WB:
		case COMPONENT_SCO_NB:
			return app_VolumeNvkey_GetScoPercentage(soundLevel);
		case COMPONENT_MIC_SCO:
			return app_VolumeNvkey_GetScoMicPercentage(soundLevel);
		case COMPONENT_A2DP:
			return app_VolumeNvkey_GetA2dpPercentage(soundLevel);
		#ifdef LINEIN_ENABLE
		case COMPONENT_LINEIN:
			return app_VolumeNvkey_GetLineinPercentage(soundLevel);
		#endif
		case COMPONENT_VPRT:
			return app_VolumeNvkey_GetVpRtPercentage(soundLevel);
		#ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL
		case COMPONENT_MP3:
			return app_VolumeNvkey_GetMp3Percentage(soundLevel);
		#endif //MP3_LOCAL_PLAYBACK_MMI_CTRL
		case COMPONENT_AIROTHRU_SPK:
			return app_VolumeNvkey_GetAiroThruSpkPercentage(soundLevel);
		case COMPONENT_AIROTHRU_MIC:
			return app_VolumeNvkey_GetAiroThruMicPercentage(soundLevel);
		default:
			FW_Assert(FALSE);
			break;
	}
	return 0;
}

U8 APP_VolumeNvkey_GetAvrcpVolumeBySoundLevel(U8 soundLevel)
{
	return app_VolumeNvkey_GetAvrcpVolume(soundLevel);
}

void APP_VolumeNvkey_WriteDefaultSoundLevel(U8 component, U8 soundLevel)
{
	switch(component)
	{
		case COMPONENT_AIROTHRU_SPK:
		case COMPONENT_AIROTHRU_MIC:
			app_VolumeNvkey_SetAiroThruDefaultSoundLevel(soundLevel);
			break;
		default:
			break;
	}
}

U16 APP_VolumeNvkey_GetTimer(U8 timerType)
{
	switch(timerType)
	{
		case TIMER_MIC_MUTE_REMIND:
			return pVolumeInfo->timer.MuteReminderTime;
		default:
			FW_Assert(FALSE);
			break;
	}

	return 0;
}
