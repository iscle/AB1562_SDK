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
#include "Peq_NvkeyHandler.h"
#include "Peq_Nvkey.h"
#include "App_PeqProcess.h"
#include "App_PeqMount.h"
#include "App_A2dp_NVKeyHandler.h"
#include "App_LinkKey.h"
#include "drv_audio_nvkey.h"

/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct
{
	U8 PEQGroup[PEQ_PATH_NUM];
	U8 PEQNvkeyMode[PEQ_PATH_NUM];
}APP_PEQ_CTL_STRU;

/**************************************************************************************************
* Variables
**************************************************************************************************/
static APP_PEQ_CTL_STRU gAppPeqCtl;

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static void app_PeqProc_PeqGroupCalculation(U8 peqComponent);
static void app_PeqProc_SetPeqGroup(U8 groupIndex, U8 peqComponent);
static void app_PeqProc_SetNvkeyMode(U8 mode, U8 peqComponent);

/**************************************************************************************************
* Static functions
**************************************************************************************************/
static void app_PeqProc_PeqGroupCalculation(U8 peqComponent)
{
	U8 numOfPEQ, groupIndex, nvkeyMode;

	nvkeyMode = APP_PeqProc_GetNvkeyMode(peqComponent);
	numOfPEQ = APP_PeqNvkey_GetPEQGroupNum(peqComponent, nvkeyMode);

	if(numOfPEQ == 0 && nvkeyMode == PEQ_LOAD_DEFAULT)
	{
		return;
	}

	groupIndex = APP_PeqProc_GetPeqGroup(peqComponent);

	if(numOfPEQ < groupIndex)
	{
		app_PeqProc_SetNvkeyMode(PEQ_LOAD_DEFAULT, peqComponent);
		if(APP_A2dp_NVKeyIsFeatureOn(APP_MUSIC_FORCE_USE_PEQ_FEAT))
		{
			DBG_LOG_APP_PEQ( "[APP_PEQ]Force to use PEQ Feature", 0);
			//printf("FORCE to use PEQ feature");
			//1->2->3->1->2->3
			groupIndex = 1;
		}
		else
		{
			DBG_LOG_APP_PEQ( "[APP_PEQ]Not Force to use PEQ Feature", 0);
			//printf("NOT FORCE to use PEQ feature");
			//0->1->2->3->0->1->2->3
			groupIndex = 0;
		}
	}
	else if(numOfPEQ == groupIndex)
	{
		if(nvkeyMode == PEQ_LOAD_CUSTOM)
		{
			app_PeqProc_SetNvkeyMode(PEQ_LOAD_DEFAULT, peqComponent);
		}
		else if((nvkeyMode == PEQ_LOAD_DEFAULT) && !APP_PeqNvkey_IsCustomNvkeyEmpty(peqComponent))
		{
			app_PeqProc_SetNvkeyMode(PEQ_LOAD_CUSTOM, peqComponent);
		}

		if(APP_A2dp_NVKeyIsFeatureOn(APP_MUSIC_FORCE_USE_PEQ_FEAT))
		{
			DBG_LOG_APP_PEQ( "[APP_PEQ]Force to use PEQ Feature", 0);
			//printf("FORCE to use PEQ feature");
			//1->2->3->1->2->3
			groupIndex = 1;
		}
		else
		{
			DBG_LOG_APP_PEQ( "[APP_PEQ]Not Force to use PEQ Feature", 0);
			//printf("NOT FORCE to use PEQ feature");
			//0->1->2->3->0->1->2->3
			groupIndex = 0;
		}
	}
	else
	{
		groupIndex += 1;
	}

	app_PeqProc_SetPeqGroup(groupIndex, peqComponent);
	//printf("PEQ cal index:%d, mode:%d", APP_PeqProc_GetPeqGroup(peqComponent), APP_PeqProc_GetNvkeyMode(peqComponent));
}

static void app_PeqProc_SetPeqGroup(U8 groupIndex, U8 peqComponent)
{
	gAppPeqCtl.PEQGroup[peqComponent] = groupIndex;
	APP_LinkKey_SavePeqIndex(peqComponent, groupIndex);
    
    if(peqComponent == PEQ_A2DP)
        race_hostaudio_a2dp_peq_changed(groupIndex);
    else if(peqComponent == PEQ_LINE_IN)
        race_hostaudio_line_in_peq_changed(groupIndex);
        
}

static void app_PeqProc_SetNvkeyMode(U8 mode, U8 peqComponent)
{
	gAppPeqCtl.PEQNvkeyMode[peqComponent] = mode;
}

/**************************************************************************************************
* Public functions
**************************************************************************************************/
void APP_PeqProc_Load(U8 peq_path)
{
	U16 phaseIndex, phaseNumber;
	
	if(!drv_audio_nvkey_peq_is_peq_feature_on())
		return;

	APP_PeqNvkey_Init();

	U8 nvkeyMode = APP_PeqProc_GetNvkeyMode(peq_path);
	U8 groupIndex = APP_PeqProc_GetPeqGroup(peq_path);

	if(groupIndex == 0)
	{
		if(APP_A2dp_NVKeyIsFeatureOn(APP_MUSIC_FORCE_USE_PEQ_FEAT))
		{
			DBG_LOG_APP_PEQ( "[APP_PEQ]Force to use PEQ Feature", 0);
			app_PeqProc_PeqGroupCalculation(peq_path);
			groupIndex = APP_PeqProc_GetPeqGroup(peq_path);
		}
		else
		{
			APP_PeqMount_ResetFuncUser();
			return;
		}
	}

	DBG_LOG_APP_PEQ( "[APP_PEQ]PEQ Group:%d, Mode:%d", 2, groupIndex, nvkeyMode);

	if(groupIndex == 0)
	{
		return;
	}

	groupIndex -= 1; //mapping array index
	phaseNumber = APP_PeqNvkey_GetPhaseNum(peq_path, groupIndex, nvkeyMode);
	//printf("PhaseNumber:%d", phaseNumber);

	for(phaseIndex = 0; phaseIndex < phaseNumber; phaseIndex++)
	{
		//printf("PEQPhase:%d", APP_PeqNvkey_GetPhase(peq_path, groupIndex, phaseIndex));
		//printf("NvkeyID:%X", APP_PeqNvkey_GetNvkeyId(peq_path, groupIndex, phaseIndex));
		APP_PeqMount_SendParaToDSP(APP_PeqNvkey_GetPhase(peq_path, groupIndex, phaseIndex, nvkeyMode), APP_PeqNvkey_GetNvkeyId(peq_path, groupIndex, phaseIndex, nvkeyMode));
	}
}

void APP_PeqProc_ChangeToNext(U8 peq_path)
{
	app_PeqProc_PeqGroupCalculation(peq_path);

/*
	if(MMI_REMEMBER_PEQ_CHANGE_FEAT && !MMI_DEFAULT_PEQ_ASSIGNMENT_FEAT)
	{
		gMMI_driver_variation_nvram.misc_para.PEQRecord[PEQ_A2DP] =	PEQ_GetModeIndex(PEQ_A2DP);
		gMMI_driver_variation_nvram.misc_para.PEQRecord[PEQ_LINE_IN] = PEQ_GetModeIndex(PEQ_LINE_IN);
		gMMI_driver_variation_nvram.misc_para.PEQNvkeyMode[PEQ_A2DP] = PEQ_GetSectorMode(PEQ_A2DP);
		gMMI_driver_variation_nvram.misc_para.PEQNvkeyMode[PEQ_LINE_IN] = PEQ_GetSectorMode(PEQ_LINE_IN);
		MMI_SECTOR_SetUpdateFlag(TRUE);
	}
*/
}

void APP_PeqProc_SetIndex(U8 peq_path, U8 groupIndex) //groupIndex: pPeqCtl index, [path, several sets of PEQ]
{
	U8 numOfPEQ;

	numOfPEQ = APP_PeqNvkey_GetPEQGroupNum(peq_path, APP_PeqProc_GetNvkeyMode(peq_path));

	if((groupIndex > numOfPEQ) || (APP_A2dp_NVKeyIsFeatureOn(APP_MUSIC_FORCE_USE_PEQ_FEAT) && groupIndex == 0))
	{
		return;
	}

	app_PeqProc_SetPeqGroup(groupIndex, peq_path);

/*
	if(MMI_REMEMBER_PEQ_CHANGE_FEAT && !MMI_DEFAULT_PEQ_ASSIGNMENT_FEAT)
	{
		gMMI_driver_variation_nvram.misc_para.PEQRecord[PEQ_A2DP] =	PEQ_GetModeIndex(PEQ_A2DP);
		gMMI_driver_variation_nvram.misc_para.PEQRecord[PEQ_LINE_IN] = PEQ_GetModeIndex(PEQ_LINE_IN);
		gMMI_driver_variation_nvram.misc_para.PEQNvkeyMode[PEQ_A2DP] = PEQ_GetSectorMode(PEQ_A2DP);
		gMMI_driver_variation_nvram.misc_para.PEQNvkeyMode[PEQ_LINE_IN] = PEQ_GetSectorMode(PEQ_LINE_IN);
		MMI_SECTOR_SetUpdateFlag(TRUE);
	}
*/
}

U8 APP_PeqProc_GetPeqGroup(U8 peqComponent)
{
	return gAppPeqCtl.PEQGroup[peqComponent]; //several sets of PEQ parameter for a component
}

U8 APP_PeqProc_GetNvkeyMode(U8 peqComponent)
{
	return gAppPeqCtl.PEQNvkeyMode[peqComponent];
}

void APP_PeqProc_RealtimeUpdate(U8 peqPhase, U8 *pPeqPara, U16 length)
{
	DBG_LOG_APP_PEQ( "[APP_PEQ]PEQ PHASE: %d, Length:%d", 2, peqPhase, length);

	for(U8 i = 0; i < 10 ; i++)
	{
		DBG_LOG_APP_PEQ( "[APP_PEQ]realtime: %d - Realtime data:%d %d %d %d %d %d", 7, i, pPeqPara[6*i+0],
			pPeqPara[6*i+1], pPeqPara[6*i+2], pPeqPara[6*i+3], pPeqPara[6*i+4], pPeqPara[6*i+5]);
	}

	APP_PeqMount_RealtimeToDSP(peqPhase, pPeqPara, length);
}



void APP_Peq_Init(void)//must init after app linkKey APP_LinkKey_Init()
{
	U8 component;
	for(component=0; component < PEQ_PATH_NUM; component++)
	{
		app_PeqProc_SetPeqGroup(APP_LinkKey_LoadPeqIndex(component), component);
	}
}

