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


#include "stdlib.h"
#include "App_LinkKey.h"
#include "App_ResourceManager.h"
#include "App_Nvram_Type.h"
#include "App_Interface.h"
#include "App_A2dp.h"
#include "App_VolumeManage.h"
#include "App_A2dp_NVKeyHandler.h"
#include "App_VolumeScoMicProcess.h"
#include "App_System.h"
#include "bt_gap.h"
#include "App_Fcd.h"

/**************************************************************************************************
* Enum
**************************************************************************************************/


/**************************************************************************************************
* Define
**************************************************************************************************/
typedef struct
{
    U8 normalLinkNum;
    APP_MISC_PARA_TYPE_STRU miscPara;
    LINK_HISTORY_STRU* pHistoryInfo;
    LINK_HISTORY_STRU* pFcdHistoryInfo;
}LINK_KEY_CTL_STRU;

/**************************************************************************************************
* Variables
**************************************************************************************************/
static LINK_KEY_CTL_STRU gLinkKeyCtl;

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static void app_linkkey_set_connected(BD_ADDR_T *pBdAddr);

/**************************************************************************************************
* Static Functions
**************************************************************************************************/
static void app_linkkey_write_nvkey_normal_link(void)
{
	APP_NvramDrvVar_WritePayload(gLinkKeyCtl.pHistoryInfo, (sizeof(LINK_HISTORY_STRU) * gLinkKeyCtl.normalLinkNum));
}

static void app_linkkey_write_nvkey_fcd_link(void)
{
	APP_NvramDrvVar_WriteFCDPayload(gLinkKeyCtl.pFcdHistoryInfo);
}

static void app_linkkey_write_nvkey_misc(void)
{
	APP_NvramDrvVar_WriteMiscPara(&(gLinkKeyCtl.miscPara));
}

static void app_linkkey_set_connected(BD_ADDR_T *pBdAddr)
{
	LINK_HISTORY_STRU *pLinkList = APP_LinkKey_SearchHistoryByBdAddr(pBdAddr, FALSE, TRUE);

	if(pLinkList)
	{
		pLinkList->miscMask |= NVKEY_LINK_KEY_MISC_MASK_CONNECTED;
		app_linkkey_write_nvkey_normal_link();
		app_linkkey_write_nvkey_fcd_link();
	}
}

static U8 app_linkkey_search_fcd_link_history(BD_ADDR_T *pBdAddrr, U8 needKeyOk)
{
	if(FW_CmpBdAddr(&gLinkKeyCtl.pFcdHistoryInfo->linkData, pBdAddrr))
	{
		if(!needKeyOk || gLinkKeyCtl.pFcdHistoryInfo->key_status == LINK_KEY_OK)
			return 0;
	}
	return INDEX_EOF;
}

static void app_linkkey_save_history(BD_ADDR_T *pBdAddr, BOOL setDisconnected)
{
	LINK_HISTORY_STRU *linkListPtr = APP_LinkKey_SearchHistoryByBdAddr(pBdAddr, FALSE, FALSE);

	if(linkListPtr != (LINK_HISTORY_STRU *)NULL) // New link
	{
		if (setDisconnected)
		{
			linkListPtr->miscMask &= (~NVKEY_LINK_KEY_MISC_MASK_CONNECTED);
		}
		linkListPtr->scoSoundlevel = APP_VolManage_GetCurrentScoSoundLevel(pBdAddr);
		linkListPtr->scoMicSoundlevel = APP_VolScoMic_GetCurrentSoundLevel(pBdAddr);
		linkListPtr->a2dpSoundLevel = APP_VolManage_GetCurrentA2dpSoundLevel(pBdAddr);
		app_linkkey_write_nvkey_normal_link();
	}
}

static void app_linkkey_init_nvkey_normal_link(void)
{
	U8 *tempPtr;

	if((gLinkKeyCtl.pHistoryInfo = (LINK_HISTORY_STRU *)malloc(sizeof(LINK_HISTORY_STRU) * gLinkKeyCtl.normalLinkNum)) == NULL)
	{
		FW_Assert(FALSE);
	}

	tempPtr = (U8 *)APP_NvramDrvVar_GetPayloadAddr();
	if(tempPtr == NULL)
	{
		DBG_LOG_APP_Connection( "[APP_LINKKEY] Normal Read Fail", 0);
		FW_Memset(gLinkKeyCtl.pHistoryInfo, 0, sizeof(LINK_HISTORY_STRU) * gLinkKeyCtl.normalLinkNum);
	}
	else
	{
		FW_Memcpy(gLinkKeyCtl.pHistoryInfo, tempPtr, sizeof(LINK_HISTORY_STRU) * gLinkKeyCtl.normalLinkNum);
	}
}

static void app_linkkey_init_nvkey_fcd_link(void)
{
    U8 *tempPtr;

    if((gLinkKeyCtl.pFcdHistoryInfo = (LINK_HISTORY_STRU *)malloc(sizeof(LINK_HISTORY_STRU))) == NULL)
    {
        FW_Assert(FALSE);
    }

    tempPtr = (U8 *)APP_NvramDrvVar_GetFCDPayloadAddr();
    if(tempPtr == NULL)
    {
        DBG_LOG_APP_Connection( "[APP_LINKKEY] FCD Read Fail", 0);
        FW_Memset(gLinkKeyCtl.pFcdHistoryInfo, 0, sizeof(LINK_HISTORY_STRU));
    }
    else
    {
        FW_Memcpy(gLinkKeyCtl.pFcdHistoryInfo, tempPtr, sizeof(LINK_HISTORY_STRU));
    }
}

static void app_linkkey_init_nvkey_misc(void)
{
	U8 *tempPtr;

	tempPtr = (U8 *)APP_NvramDrvVar_GetMiscParaAddr();
	if(tempPtr == NULL)
	{
		DBG_LOG_APP_Connection( "[APP_LINKKEY] MISC Para Read Fail", 0);
		return;
	}
	FW_Memcpy(&gLinkKeyCtl.miscPara, tempPtr,sizeof(APP_MISC_PARA_TYPE_STRU));
}

static void app_linkkey_clear_normal_link(U8 index)
{
    LINK_HISTORY_STRU* linkListPtr = (LINK_HISTORY_STRU *)gLinkKeyCtl.pHistoryInfo;
    U8 maxCount = gLinkKeyCtl.normalLinkNum;

    //Remove Index LinkHistory and Sequence LinkHistory
    for( ; index < maxCount - 1; index++)
    {
        FW_Memcpy(&linkListPtr[index], &linkListPtr[index + 1], sizeof(LINK_HISTORY_STRU));
    }
    FW_Memset(&linkListPtr[maxCount - 1], 0, sizeof(LINK_HISTORY_STRU));
}

/*
static void app_linkkey_clear_fcd_link(void)
{
	FW_Memset(gLinkKeyCtl.pFcdHistoryInfo, 0, sizeof(LINK_HISTORY_STRU));
}*/

/*
static void app_linkkey_clear_misc(void)
{
	FW_Memset(&(gLinkKeyCtl.miscPara), 0, sizeof(APP_MISC_PARA_TYPE_STRU));
}*/

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
U8 APP_LinkKey_LoadPeqIndex(U8 component)
{
	return gLinkKeyCtl.miscPara.init.peqIndex[component];
}

void APP_LinkKey_SavePeqIndex(U8 component, U8 peqIndex)
{
	gLinkKeyCtl.miscPara.init.peqIndex[component] = peqIndex;
	app_linkkey_write_nvkey_misc();
}

void APP_LinkKey_LoadHistory(BD_ADDR_T *pBdAddr)
{
	app_linkkey_set_connected(pBdAddr);
}

BD_ADDR_T *APP_LinkKey_GetNormalLinkBdAddr(U8 index)
{
    return (index < gLinkKeyCtl.normalLinkNum) ? &gLinkKeyCtl.pHistoryInfo[index].linkData : NULL;
}

BD_ADDR_T *APP_LinkKey_GetFcdBdAddr(void)
{
    return &gLinkKeyCtl.pFcdHistoryInfo->linkData;
}

U8 APP_LinkKey_GetNormalPDLNumber(void)
{
	U8 i;
	U8 linkKeyCount = 0;

	for(i = 0 ; i < gLinkKeyCtl.normalLinkNum ; i++)
	{
	 	if(gLinkKeyCtl.pHistoryInfo[i].key_status == LINK_KEY_OK)
			++linkKeyCount;
	}
	return linkKeyCount;
}

BOOL APP_LinkKey_CheckKeyStatusOK(U8 i)
{
	return (gLinkKeyCtl.pHistoryInfo[i].key_status == LINK_KEY_OK);
}

BOOL APP_LinkKey_CheckFCDKeyState(void)
{
	return (gLinkKeyCtl.pFcdHistoryInfo->key_status & LINK_KEY_OK);
}

void APP_LinkKey_ClearConnectedFlag(void)
{
	U8 index;
	for(index = 0; index < gLinkKeyCtl.normalLinkNum; index++)
	{
		gLinkKeyCtl.pHistoryInfo[index].miscMask &= (~NVKEY_LINK_KEY_MISC_MASK_CONNECTED);
	}
	app_linkkey_write_nvkey_normal_link();
}

void APP_LinkKey_ClearAllHistory(void)
{
	DBG_LOG_APP_Connection( "[    APP][LinkHistory]: Clear Entry", 0);
    FW_Memset(gLinkKeyCtl.pHistoryInfo, 0, sizeof(LINK_HISTORY_STRU) * gLinkKeyCtl.normalLinkNum);
	BtSmDeleteAllAuthDevice((Handler)APP_GetSystemHandle());

	FW_Memset(gLinkKeyCtl.pFcdHistoryInfo, 0, sizeof(LINK_HISTORY_STRU));


	app_linkkey_write_nvkey_normal_link();
	app_linkkey_write_nvkey_fcd_link();
}

void APP_LinkKey_DeleteLinkHistory(BD_ADDR_T *pbdAddr)
{
	U8 i = APP_LinkKey_SearchLinkKeyIndex(pbdAddr, FALSE);
	if(i != INDEX_EOF)
	{
		app_linkkey_clear_normal_link(i);
		app_linkkey_write_nvkey_normal_link();
	}
}

BOOL APP_LinkKey_IsBdAddrInHistory(BD_ADDR_T *pBdAddr)
{
	return (APP_LinkKey_SearchHistoryByBdAddr(pBdAddr, TRUE, FALSE) != NULL ? TRUE : FALSE);
}

BOOL APP_LinkKey_IsNormalLinkWatchdog(U8 index)
{
	return (gLinkKeyCtl.pHistoryInfo[index].miscMask & NVKEY_LINK_KEY_MISC_MASK_CONNECTED) ? TRUE : FALSE;
}

void APP_LinkKey_ResetLinkHistory(void)
{
	U8 count;

	for(count = 0 ; count < gLinkKeyCtl.normalLinkNum ; count++)
	{
		app_linkkey_clear_normal_link(count);
	}
	app_linkkey_write_nvkey_normal_link();
}

void APP_LinkKey_SaveHistoryInRunTime(BD_ADDR_T *pbdAddr)
{
	app_linkkey_save_history(pbdAddr, FALSE);
}

void APP_LinkKey_SaveHistoryWhenDisconnect(BD_ADDR_T *pbdAddr)
{
	app_linkkey_save_history(pbdAddr, TRUE);
}

LINK_HISTORY_STRU *APP_LinkKey_SearchHistoryByBdAddr(BD_ADDR_T *pBdAddr, bool isNeedOk, bool isNeedShiftTop)
{
	U8 i;

	if((i = APP_LinkKey_SearchLinkKeyIndex(pBdAddr, isNeedOk)) == INDEX_EOF)   //cant find
	{
        if((i = app_linkkey_search_fcd_link_history(pBdAddr, isNeedOk)) != INDEX_EOF)
        {
            return (gLinkKeyCtl.pFcdHistoryInfo);
        }
	}
	else
	{
		if(isNeedShiftTop)
			i = APP_LinkKey_ShiftLinkKeyToTop(i);

		return &(gLinkKeyCtl.pHistoryInfo[i]);
	}
	return (LINK_HISTORY_STRU *)NULL;
}

U8 APP_LinkKey_SearchLinkKeyIndex(BD_ADDR_T *pBdAddr, U8 needKeyOk)
{
	U8 i;

	for (i = 0 ; i < gLinkKeyCtl.normalLinkNum ; i++)
	{
		if(FW_CmpBdAddr(&gLinkKeyCtl.pHistoryInfo[i].linkData, pBdAddr))
		{
			if(!needKeyOk)
				break;

			if(gLinkKeyCtl.pHistoryInfo[i].key_status == LINK_KEY_OK)
				break;
		}
	}

	if(i ==  gLinkKeyCtl.normalLinkNum)
		i = INDEX_EOF;

	return i;
}

void APP_LinkKey_SetLinkSoundLevelByHistory(BD_ADDR_T *pBdAddr)
{
	U8 scoSoundLevel, scoMicSoundlevel;
<<<<<<< HEAD
	#ifdef A2DP_Profile
=======
	#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
	U8 a2dpSoundLevel;
	#endif
	LINK_HISTORY_STRU *linkListPtr = APP_LinkKey_SearchHistoryByBdAddr(pBdAddr, FALSE, FALSE);

	if(linkListPtr == (LINK_HISTORY_STRU *)NULL)
	{
		scoSoundLevel = APP_DEFAULT_SCO_SOUND_LEVEL;
		scoMicSoundlevel = APP_DEFAULT_SCO_MIC_SOUND_LEVEL;
<<<<<<< HEAD
		#ifdef A2DP_Profile
=======
		#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
		a2dpSoundLevel = APP_DEFAULT_A2DP_SOUND_LEVEL;
		#endif
	}
	else
	{
		scoSoundLevel = linkListPtr->scoSoundlevel;
		scoMicSoundlevel = linkListPtr->scoMicSoundlevel;
<<<<<<< HEAD
		#ifdef A2DP_Profile
=======
		#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
		a2dpSoundLevel = linkListPtr->a2dpSoundLevel;
		#endif
	}

	DBG_LOG_APP_Connection( "[APP_LINKKEY] linkkey scosdlv:%d, a2dpsdlv:%d, micScosdlv:%d", 3, scoSoundLevel, a2dpSoundLevel, scoMicSoundlevel);
	CURRENT_A2DP_MIC_SOUND_LEVEL(pBdAddr) = a2dpSoundLevel;

	APP_VolManage_SetCurrentVgs(pBdAddr, APP_VolManage_GetVgsByScoSoundLevel(scoSoundLevel));
	APP_VolManage_SetCurrentScoSoundLevel(pBdAddr, scoSoundLevel);

	APP_VolManage_SetCurrentScoMicSoundLevel(pBdAddr, scoMicSoundlevel);
	APP_VolManage_SetCurrentScoVgm(pBdAddr, APP_VolManage_GetVgmByScoMicSoundLevel(scoMicSoundlevel));

<<<<<<< HEAD
	#ifdef A2DP_Profile
=======
	#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
	APP_VolManage_SetCurrentA2dpSoundLevel(pBdAddr, a2dpSoundLevel);
	#endif
	APP_VolManage_SetCurrentAbsoluteVolume(pBdAddr, APP_VolManage_GetAbsoluteVolumeBySoundLevel(a2dpSoundLevel));

	app_linkkey_write_nvkey_normal_link();
}

void APP_LinkKey_UpdateHistory(BD_ADDR_T *pBdAddr, U8 isSuccessful)
{
	U8 i;
	LINK_HISTORY_STRU *linkListPtr = APP_LinkKey_SearchHistoryByBdAddr(pBdAddr, FALSE, TRUE);

	DBG_LOG_APP_Connection( "[APP_LINKKEY] Update Link History:%d", 1, isSuccessful);

	if(isSuccessful)
	{
		if(linkListPtr == (LINK_HISTORY_STRU *)NULL) // New link
		{
			i = gLinkKeyCtl.normalLinkNum - 1; // choose the oldest link
			app_linkkey_clear_normal_link(i);
			i = APP_LinkKey_ShiftLinkKeyToTop(i);

			if(i < gLinkKeyCtl.normalLinkNum)
			{
				linkListPtr = &gLinkKeyCtl.pHistoryInfo[i];
				linkListPtr->scoSoundlevel = APP_DEFAULT_SCO_SOUND_LEVEL;
				linkListPtr->scoMicSoundlevel = APP_DEFAULT_SCO_MIC_SOUND_LEVEL;
				linkListPtr->a2dpSoundLevel = APP_DEFAULT_A2DP_SOUND_LEVEL;
			}
            else
            {
                FW_Assert(FALSE);
                return;
            }
		}

		linkListPtr->profile = 0;
		FW_Memcpy(&linkListPtr->linkData, pBdAddr, sizeof(BD_ADDR_T));
		linkListPtr->key_status = LINK_KEY_OK;

		if(PM_GetConnectedProfileCount(pBdAddr, TRUE))
		{
			//write flash until it gets connected
			app_linkkey_write_nvkey_normal_link();
			app_linkkey_write_nvkey_fcd_link();
			app_linkkey_write_nvkey_misc();
		}
	}
	else
	{
		if(linkListPtr != (LINK_HISTORY_STRU *)NULL)
		{
			FW_Memset(&linkListPtr, 0, sizeof(LINK_HISTORY_STRU));
			app_linkkey_write_nvkey_normal_link();
			app_linkkey_write_nvkey_fcd_link();
			app_linkkey_write_nvkey_misc();
		}
	}
}

U8 APP_LinkKey_ShiftLinkKeyToTop (U8 targetIndex)
{
	// Just connected and paired
	LINK_HISTORY_STRU *tempHistory;

	if(targetIndex < gLinkKeyCtl.normalLinkNum)
	{
		tempHistory = (LINK_HISTORY_STRU *)FW_GetMemory(sizeof(LINK_HISTORY_STRU));
		if(tempHistory != (LINK_HISTORY_STRU *)NULL)
		{
			FW_Memcpy(tempHistory, &gLinkKeyCtl.pHistoryInfo[targetIndex], sizeof(LINK_HISTORY_STRU));
			for(; targetIndex > 0; targetIndex--)
			{
				FW_Memcpy(&gLinkKeyCtl.pHistoryInfo[targetIndex], &gLinkKeyCtl.pHistoryInfo[targetIndex - 1], sizeof(LINK_HISTORY_STRU));
			}
			FW_Memcpy(&gLinkKeyCtl.pHistoryInfo[targetIndex],tempHistory, sizeof(LINK_HISTORY_STRU));

			app_linkkey_write_nvkey_normal_link();
			FW_FreeMemory(tempHistory);
		}
		targetIndex = 0;
	}
	return targetIndex;
}


void APP_LinkKey_Init(void)
{
	U8 linkCount;

	if((linkCount = APP_NvramDrvVar_GetLinkHistoryNum()) != 0)
	{
		gLinkKeyCtl.normalLinkNum = linkCount;
		app_linkkey_init_nvkey_normal_link();
	}

	app_linkkey_init_nvkey_fcd_link();
	app_linkkey_init_nvkey_misc();
}

