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
#include "App_ReConnection.h"
#include "App_ResourceManager.h"
#include "App_LinkKey.h"
#include "App_Nvram_Type.h"
#include "App_Interface.h"
#include "App_Connection_Nvkey.h"
#include "App_Connection_NVKeyHandler.h"
#include "App_PowerOff.h"
#include "App_Media.h"
#include "PM_Interface.h"
#include "APP_AirApp.h"
#include "App_Pairing.h"
#include "bt_sdp.h"

/**************************************************************************************************
* Enum
**************************************************************************************************/
#define INVALID_RECONNECT_INDEX	0xFF

#define RECONNECT_MAX_LINK_CNT	4

#define LIST_HIGH_PRI_DATA 	FALSE
#define LIST_NORMAL_DATA	TRUE

/**************************************************************************************************
* Define
**************************************************************************************************/
typedef struct
{
	BD_ADDR_T bdAddr;
	U8 reconnectType;
	U8 retryCnt;
	U8 reconnProfileMask;
}APP_RECONNECT_DATA_STRU;//MMIReonnectDataType

typedef struct
{
	U8 currentHighPriorityIndex;
	APP_RECONNECT_DATA_STRU reconLinkLossWatchDogData[RECONNECT_MAX_LINK_CNT];
	U8 currentTargetIndex;
	APP_RECONNECT_DATA_STRU reconnectData[RECONNECT_MAX_LINK_CNT];
}APP_RECONNECT_LIST_STRU;//MMIReconnectListType


typedef struct
{
	APP_RECONNECT_LIST_STRU *pReconnectList;//reconnectListPtr
	U8 type;
	//U8 linkLossLoopCnt;
}APP_RECONNECT_CTRL_STRU;//MMIReconnectCtlType

static U32 app_Reconn_Handler(Handler handler, U16 id, void *payload, U32 id_ext);
/**************************************************************************************************
* Variables
**************************************************************************************************/
APP_RECONNECT_CTRL_STRU gAppReconnectCtl;
static const HandlerData gAppReconnHandler = { app_Reconn_Handler };

/**************************************************************************************************
* Prototype
**************************************************************************************************/

/**************************************************************************************************
* Static Functions
**************************************************************************************************/
static BOOL app_Reconn_CheckBdAddrList(BD_ADDR_T *pBdAddr)
{
    U8 index;
    if(gAppReconnectCtl.pReconnectList != NULL)
    {
        for(index = 0; index < RECONNECT_MAX_LINK_CNT; index++)
        {
            if(FW_CmpBdAddr(pBdAddr, &gAppReconnectCtl.pReconnectList->reconLinkLossWatchDogData[index].bdAddr))
                return TRUE;

            if(FW_CmpBdAddr(pBdAddr, &gAppReconnectCtl.pReconnectList->reconnectData[index].bdAddr))
                return TRUE;
        }
    }
    return FALSE;
}

static U8 app_Reconn_GetCurrentTargetIndex(void)
{
	if(gAppReconnectCtl.pReconnectList)
		return gAppReconnectCtl.pReconnectList->currentTargetIndex;

	return INVALID_RECONNECT_INDEX;
}

static APP_RECONNECT_DATA_STRU * app_Reconn_GetCtlDataPtr(BD_ADDR_T *pBdAddr, BOOL isHighPriority)
{
	U8 index;
	APP_RECONNECT_DATA_STRU * pDataCtl;

	if(gAppReconnectCtl.pReconnectList == (APP_RECONNECT_LIST_STRU *)NULL)
		return (APP_RECONNECT_DATA_STRU *)NULL;

	if(isHighPriority)
	{
		pDataCtl = &gAppReconnectCtl.pReconnectList->reconLinkLossWatchDogData[0];
	}
	else
	{
		pDataCtl = &gAppReconnectCtl.pReconnectList->reconnectData[0];
	}

	//Find the duplicated data
	for(index = 0; index < RECONNECT_MAX_LINK_CNT; index++)
	{
		if(FW_CmpBdAddr(pBdAddr, &pDataCtl[index].bdAddr))
			break;
	}


	//New data! Allocate the new data space
	if(index == RECONNECT_MAX_LINK_CNT)
	{
		for(index = 0; index < RECONNECT_MAX_LINK_CNT; index++)
		{
			if(pDataCtl[index].reconnectType == RECONNECT_TYPE_NULL)
			{
				break;
			}
		}
	}

	//No empty space, delete the last one
	if(index == RECONNECT_MAX_LINK_CNT)
		index = (RECONNECT_MAX_LINK_CNT-1);

    DBG_LOG_APP_Connection( "[APP_Reconn] GetCtlDataPtr: index:%d, BdAddr:0x%x%x", 3, index, FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE));

	return &pDataCtl[index];
}

static void app_Reconn_SetData(APP_RECONNECT_DATA_STRU * pDataCtl, BD_ADDR_T *pBdAddr, U8 type, U8 retryCnt)
{
	FW_Memcpy(&pDataCtl->bdAddr, pBdAddr, sizeof(BD_ADDR_T));
	pDataCtl->reconnectType = type;
	pDataCtl->retryCnt = retryCnt;
	pDataCtl->reconnProfileMask = 0;
}

static void app_Reconn_SetProfileMask(APP_RECONNECT_DATA_STRU * pDataCtl, U8 profileMask)
{
	pDataCtl->reconnProfileMask = profileMask;
}

static void app_Reconn_SetReconnectType(U8 type)
{
	DBG_LOG_APP_Connection( "[APP_ReConn] Set reconnect type, before:%d, after:%d", 2, gAppReconnectCtl.type, type);

	//if(gAppReconnectCtl.type != RECONNECT_LINK_LOSS && gAppReconnectCtl.type != RECONNECT_WATCHDOG)
	{
		gAppReconnectCtl.type = type;
	}
}

static void app_Reconn_MoveDataToTop(BD_ADDR_T *pBdAddr, BOOL isNormalData)
{
	U8 index;
	APP_RECONNECT_DATA_STRU * pDataCtl;
	APP_RECONNECT_DATA_STRU * pBackUpData;

	if(gAppReconnectCtl.pReconnectList == (APP_RECONNECT_LIST_STRU *)NULL)
		return;

	if(!isNormalData)
	{
		pDataCtl = &gAppReconnectCtl.pReconnectList->reconLinkLossWatchDogData[0];
	}
	else
	{
		pDataCtl = &gAppReconnectCtl.pReconnectList->reconnectData[0];
	}

	for(index = 0; index < RECONNECT_MAX_LINK_CNT; index++)
	{
		if(FW_CmpBdAddr(pBdAddr, &pDataCtl[index].bdAddr))
			break;
	}

	if(index && index != RECONNECT_MAX_LINK_CNT)
	{
		pBackUpData = (APP_RECONNECT_DATA_STRU *)FW_GetMemory(sizeof(APP_RECONNECT_DATA_STRU));
		if(pBackUpData)
		{
			if(!isNormalData)
			{
				if(gAppReconnectCtl.pReconnectList->currentHighPriorityIndex < index)
					gAppReconnectCtl.pReconnectList->currentHighPriorityIndex++;
			}

			FW_Memcpy((U8 *)pBackUpData, (U8 *)&pDataCtl[index], sizeof(APP_RECONNECT_DATA_STRU));
			for(; index; index--)
			{
				FW_Memcpy((U8 *)&pDataCtl[index], (U8 *)&pDataCtl[index-1], sizeof(APP_RECONNECT_DATA_STRU));
			}
			FW_Memcpy((U8 *)&pDataCtl[0], (U8 *)pBackUpData, sizeof(APP_RECONNECT_DATA_STRU));
			FW_FreeMemory((U8 *) pBackUpData);
		}
	}
}

static void app_ReConn_AddReconnectList(U8 type, BD_ADDR_T *pBdAddr)
{
	APP_RECONNECT_DATA_STRU * pCtlData;

	if(pBdAddr == (BD_ADDR_T *)NULL)
	{
		DBG_LOG_APP_Connection( "[APP_Reconn] invalid BdAddr", 0);
		return;
	}

	DBG_LOG_APP_Connection( "[APP_Reconn] Add list type:%d, BdAddr:0x%x%x, pReconnectList:0x%x", 4, type, FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE), gAppReconnectCtl.pReconnectList);

	//1. Prepare reconnectList Memory
	if(gAppReconnectCtl.pReconnectList == (APP_RECONNECT_LIST_STRU *)NULL)
	{
		gAppReconnectCtl.pReconnectList = (APP_RECONNECT_LIST_STRU *)FW_GetMemory(sizeof(APP_RECONNECT_LIST_STRU));
		if(gAppReconnectCtl.pReconnectList)
		{
			FW_Memset((U8 *)gAppReconnectCtl.pReconnectList, 0, sizeof(APP_RECONNECT_LIST_STRU));
			gAppReconnectCtl.pReconnectList->currentHighPriorityIndex = INVALID_RECONNECT_INDEX;
			gAppReconnectCtl.pReconnectList->currentTargetIndex = INVALID_RECONNECT_INDEX;
		}
		else
		{
			DBG_LOG_APP_Connection( "[APP_Reconn] Allocate Memory fail, no reconnection!", 0);
			return;
		}
	}
    
    //2. Check duplicate
    if(app_Reconn_CheckBdAddrList(pBdAddr))
    {
        DBG_LOG_APP_Connection( "[APP_Reconn] Duplicated in list!!", 0);
        return;
    }

	//3. Set Parameters
	if(type == RECONNECT_LINK_LOSS || type == RECONNECT_WATCHDOG)
	{
		U8 retryCnt;

		if((pCtlData = app_Reconn_GetCtlDataPtr(pBdAddr, TRUE)) == (APP_RECONNECT_DATA_STRU *)NULL)
		{
			DBG_LOG_APP_Connection( "[APP_Reconn] 1. Get CtlData Fail, no reconnection!", 0);
			return;
		}

		if(type == RECONNECT_LINK_LOSS)
		{
			retryCnt = App_ReConnNvkey_GetLinkLossReTryCnt();

			//if(App_ReConnNvkey_GetReconnectLinkLossTimer())
			//{
				//gAppReconnectCtl.pReconnectList->isDurationTimerSet = TRUE;
				//OST_SetTimer(&gAppReconnectCtl.pReconnectList->reconnectDurationTimer,App_ReConnNvkey_GetReconnectLinkLossTimer());
			//}
		}
		else
		{
			retryCnt = App_ReConnNvkey_GetWatchDogReTryCnt();
		}

		app_Reconn_SetData(pCtlData, pBdAddr, type, retryCnt);

		app_Reconn_MoveDataToTop(pBdAddr, FALSE);
	}
	else
	{
		if((pCtlData = app_Reconn_GetCtlDataPtr(pBdAddr, FALSE)) == (APP_RECONNECT_DATA_STRU *)NULL)
		{
			DBG_LOG_APP_Connection( "[APP_Reconn] 1. Get CtlData Fail, no reconnection!", 0);
			return;
		}

		app_Reconn_SetData(pCtlData, pBdAddr, type, App_ReConnNvkey_GetAutoReTryCnt());
	}
}

static void app_ReConn_AddListByLinkKeyIndex(U8 type, U8 index)
{
	if(APP_LinkKey_CheckKeyStatusOK(index))
	{
		app_ReConn_AddReconnectList(type, APP_LinkKey_GetNormalLinkBdAddr(index));
	}
}


static void app_ReConn_AddListByFastDongle(U8 type, U8 index)
{
    UNUSED(index);
	if(APP_LinkKey_CheckFCDKeyState())
	{
		app_ReConn_AddReconnectList(type, APP_LinkKey_GetFcdBdAddr());
	}
}

static void app_ReConn_AddListByBdAddr(U8 type, BD_ADDR_T *pBdAddr)
{
	if(pBdAddr && APP_LinkKey_IsBdAddrInHistory(pBdAddr))
	{
		app_ReConn_AddReconnectList(type, pBdAddr);
	}
}

static void app_ReConn_AddListByMode(U8 type, U8 reconnectMode)
{
	U8 i, cnt;
	switch(reconnectMode)
	{
		case RECONNECT_FAST_DONGLE:
			app_ReConn_AddListByFastDongle(type, 0);
			break;
		case RECONNECT_LAST_TWO:
			app_ReConn_AddListByLinkKeyIndex(type, 0);
			app_ReConn_AddListByLinkKeyIndex(type, 1);
			break;
		case RECONNECT_LAST_ONE:
			app_ReConn_AddListByLinkKeyIndex(type, 0);
			break;
		case RECONNECT_AG_IND_0:
			app_ReConn_AddListByBdAddr(type, APP_LinkKey_GetNormalLinkBdAddr(0));
			break;
		case RECONNECT_AG_IND_0_1:
			app_ReConn_AddListByBdAddr(type, APP_LinkKey_GetNormalLinkBdAddr(0));
		case RECONNECT_AG_IND_1:
			app_ReConn_AddListByBdAddr(type, APP_LinkKey_GetNormalLinkBdAddr(1));
			break;

		case RECONNECT_LAST_DISC:
			for(i = 0;  i < APP_Conn_GetSupportDeviceNo(); i++)
			{
				if(!APP_GetAppLinkByBdAddr(APP_LinkKey_GetNormalLinkBdAddr(i)))
				{
					app_ReConn_AddListByBdAddr(type, APP_LinkKey_GetNormalLinkBdAddr(i));
					break;
				}
			}
			break;

		case RECONNECT_LIST:
			cnt = APP_LinkKey_GetNormalPDLNumber();
			for(i = 0;  i < cnt; i++)
				app_ReConn_AddListByLinkKeyIndex(type, i);
			break;

		case RECONNECT_ALL_DROPPED_LINKS:

			cnt = APP_Conn_GetSupportDeviceNo() - PM_GetCtlInfoNum();

			for(i = 0; i < cnt; i++)
			{
				if((APP_LinkKey_IsNormalLinkWatchdog(i) && !APP_GetAppLinkByBdAddr(APP_LinkKey_GetNormalLinkBdAddr(i))) || APP_AirApp_FOTA_Reconnect())
				{
					app_ReConn_AddListByLinkKeyIndex(type, i);
				}
			}
			break;

		default:
			break;
	}
}

static BOOL app_ReConn_SearchList(U8 dataCnt, U8 * pIndex)
{
	if(dataCnt)
	{
		if(*pIndex == INVALID_RECONNECT_INDEX)
		{
			*pIndex = 0;
		}
		else if(*pIndex == (dataCnt-1))
		{
			*pIndex = INVALID_RECONNECT_INDEX;
			return FALSE;
		}
		else
		{
			(*pIndex)++;
		}
		return TRUE;
	}
	return FALSE;
}

static U8 app_ReConn_GetListCnt(U8 isNormalData)
{
	U8 index;
	U8 cnt = 0;

	if(!gAppReconnectCtl.pReconnectList)
		return 0;

	for(index = 0; index < RECONNECT_MAX_LINK_CNT; index++)
	{
		if(isNormalData)
		{
			if(gAppReconnectCtl.pReconnectList->reconnectData[index].reconnectType != RECONNECT_TYPE_NULL)
			{
				cnt++;
			}
		}
		else
		{
			if(gAppReconnectCtl.pReconnectList->reconLinkLossWatchDogData[index].reconnectType != RECONNECT_TYPE_NULL)
			{
				cnt++;
			}
		}
	}
	return cnt;
}

static APP_RECONNECT_DATA_STRU *app_ReConn_GetTarget(void)
{
	APP_RECONNECT_DATA_STRU * pTarget = (APP_RECONNECT_DATA_STRU *)NULL;

	if(app_ReConn_SearchList(app_ReConn_GetListCnt(LIST_HIGH_PRI_DATA), &gAppReconnectCtl.pReconnectList->currentHighPriorityIndex))
		pTarget = &gAppReconnectCtl.pReconnectList->reconLinkLossWatchDogData[gAppReconnectCtl.pReconnectList->currentHighPriorityIndex];
	else if(app_ReConn_SearchList(app_ReConn_GetListCnt(LIST_NORMAL_DATA), &gAppReconnectCtl.pReconnectList->currentTargetIndex))
		pTarget = &gAppReconnectCtl.pReconnectList->reconnectData[gAppReconnectCtl.pReconnectList->currentTargetIndex];
	else if(app_ReConn_SearchList(app_ReConn_GetListCnt(LIST_HIGH_PRI_DATA), &gAppReconnectCtl.pReconnectList->currentHighPriorityIndex))
		pTarget = &gAppReconnectCtl.pReconnectList->reconLinkLossWatchDogData[gAppReconnectCtl.pReconnectList->currentHighPriorityIndex];
	else if(app_ReConn_SearchList(app_ReConn_GetListCnt(LIST_NORMAL_DATA), &gAppReconnectCtl.pReconnectList->currentTargetIndex))
		pTarget = &gAppReconnectCtl.pReconnectList->reconnectData[gAppReconnectCtl.pReconnectList->currentTargetIndex];

	return pTarget;
}

static U8 app_Reconn_GetProfileMask(APP_RECONNECT_DATA_STRU * pDataCtl)
{
	return pDataCtl->reconnProfileMask;
}

static void app_ReConn_Start(U8 type)
{
	APP_RECONNECT_DATA_STRU * pTarget;
	U8 listCount = app_ReConn_GetListCnt(LIST_HIGH_PRI_DATA) + app_ReConn_GetListCnt(LIST_NORMAL_DATA);

	FIND_AGAIN:

	if(	!gAppReconnectCtl.pReconnectList || !APP_PowerOff_IsPowerOn() ||
		(0 == app_ReConn_GetListCnt(LIST_NORMAL_DATA) && 0 == app_ReConn_GetListCnt(LIST_HIGH_PRI_DATA)))
	{
		DBG_LOG_APP_Connection( "[APP_Reconn] STOP_RECONNECT no list:%d is poweron :%d normal cnt:%d highpri cnt:%d", 4,
			!gAppReconnectCtl.pReconnectList, !APP_PowerOff_IsPowerOn(), app_ReConn_GetListCnt(LIST_NORMAL_DATA), app_ReConn_GetListCnt(LIST_HIGH_PRI_DATA));

		goto STOP_RECONNECT;
	}

	if((pTarget = app_ReConn_GetTarget()) == (APP_RECONNECT_DATA_STRU *)NULL)
	{
		STOP_RECONNECT:
		APP_ReConn_ClearCtl();

		if(App_ReConnNvkey_IsFeatureOn(APP_DISCOVER_AFTER_POWERON_RECONNECT_FAIL_FEAT) && type == RECONNECT_PWR_ON)
		{
			APP_Pairing_EnterDiscoverable();
		}
		DBG_LOG_APP_Connection( "[APP_Reconn] STOP_RECONNECT, feature:%d, type:%d", 2, App_ReConnNvkey_IsFeatureOn(APP_DISCOVER_AFTER_POWERON_RECONNECT_FAIL_FEAT), type);

		return;
	}

	if(!APP_Conn_IsProfilesReady())
	{
		DBG_LOG_APP_Connection( "[APP_Reconn] ----- Profile Not Ready!! return -----", 0);
		return;
	}

	DBG_LOG_APP_Connection( "[APP_Reconn] app_ReConn_Start listCount:%d, normalCnt:%d, isActiveLink:%d, profileCnt:%d, targetBda:0x%x%x", 6,
		listCount, app_ReConn_GetListCnt(LIST_NORMAL_DATA), APP_IsActiveLink(&pTarget->bdAddr), PM_GetConnectedProfileCount(&pTarget->bdAddr, TRUE), FW_bdaddr_to_2U32(&pTarget->bdAddr, TRUE), FW_bdaddr_to_2U32(&pTarget->bdAddr, FALSE));

	if(APP_IsActiveLink(&pTarget->bdAddr))
	{
		APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(&pTarget->bdAddr);
		if(pLinkInfo->linkPara.miscMask & APP_LINK_DETACHING)
		{
			if(type == RECONNECT_USER_INIT)
			{
				APP_Reconn_SetDelayTimer(RECONNECT_USER_INIT, App_ReConnNvKey_GetReconnectDelayTime() * ONE_SEC);
			}
		}
		if(!PM_GetConnectedProfileCount(&pTarget->bdAddr, TRUE))
		{
			DBG_LOG_APP_Connection( "[APP_Reconn] ACL Exist Reconnect", 0);
			APP_Conn_CreateActiveProfileConnection(&pTarget->bdAddr);
		}
		APP_ReConn_DeleteInvalidList();

		if((pTarget = app_ReConn_GetTarget()) == (APP_RECONNECT_DATA_STRU *)NULL)
		{
			DBG_LOG_APP_Connection( "[APP_Reconn] STOP_RECONNECT no target", 0);
			goto STOP_RECONNECT;
		}
	}

	if(pTarget->retryCnt)
	{
		if(!app_Reconn_GetProfileMask(pTarget))
		{
			DBG_LOG_APP_Connection( "[APP_Reconn] Start Reconnecting, Priority:%d, targetIdx:%d, rtyCnt:%d", 2, (U8)gAppReconnectCtl.pReconnectList->currentHighPriorityIndex, (U8)app_Reconn_GetCurrentTargetIndex());
			app_Reconn_SetProfileMask(pTarget, RECONNECT_PROFILE_HFP|RECONNECT_PROFILE_A2DP|RECONNECT_PROFILE_AVRCP/*|RECONNECT_PROFILE_CUSTAPP*/);
			APP_Conn_CreateActiveProfileConnection(&pTarget->bdAddr);
			app_Reconn_SetReconnectType(type);

			if(pTarget->retryCnt != 0xFF)
				pTarget->retryCnt--;
		}
		else
		{
			DBG_LOG_APP_Connection( "[APP_Reconn] Reconnect profile mask exist", 0);
		}
	}
	else
	{
		DBG_LOG_APP_Connection( "[APP_ReConn] Reconn stop retry", 0);
		APP_ReConn_DeleteListByBdAddr(&pTarget->bdAddr);
	}

	if(listCount)
	{
		listCount--;
		if(listCount == 0)
		{
			DBG_LOG_APP_Connection( "[APP_Reconn] app_ReConn_Start No reconnect list count", 0);
			return;
		}
	}
	goto FIND_AGAIN;
}

static U8 app_ReConn_ProfileIDToMask(U8 profileID)
{
	switch(profileID)
	{
		case PROFILE_A2DP:
			return RECONNECT_PROFILE_A2DP;
			break;
		case PROFILE_AVRCP:
			return RECONNECT_PROFILE_AVRCP;
			break;
		case PROFILE_HANDSFREE:
			return RECONNECT_PROFILE_HFP;
			break;
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE            
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE            
>>>>>>> db20e11 (second commit)
		case PROFILE_HEADSET:
			return RECONNECT_PROFILE_HSP;
			break;
#endif            
		case PROFILE_SERIAL_PORT:
			return RECONNECT_PROFILE_CUSTAPP;
		default:
			break;
	}
	return 0;
}

static U32 app_Reconn_Handler(Handler handler, U16 id, void *payload, U32 id_ext)
{
	UNUSED(handler);
	UNUSED(id);
	UNUSED(payload);
	UNUSED(id_ext);

	switch(id)
	{
		case RECONNECT_LINK_LOSS:
			APP_ReConn_Reconnect(RECONNECT_LINK_LOSS);
			break;
		case RECONNECT_CHECK:
			APP_ReConn_Reconnect(RECONNECT_CHECK);
			break;
		case RECONNECT_WATCHDOG:
			APP_ReConn_Reconnect(RECONNECT_WATCHDOG);
			break;
        case ESD_RECONNECT_CHECK:
			if(!CURRENT_ACTIVE_LINK_CNT)
			{
				APP_ReConn_Reconnect(RECONNECT_CHECK);
			}
			break;
	}

	return 0;
}

/**************************************************************************************************
* Public Function
**************************************************************************************************/
void APP_ReConn_DeleteProfileMask(BD_ADDR_T *pBdAddr, U8 profileID, U8 reason)
{
	APP_RECONNECT_DATA_STRU *pDataCtl;
	U8 listCount = app_ReConn_GetListCnt(LIST_HIGH_PRI_DATA) + app_ReConn_GetListCnt(LIST_NORMAL_DATA);

	DBG_LOG_APP_Connection( "[APP_ReConn] DeleteProfileMask profileId:0x%x, reason:%d, list cnt:%d, bdAddr:0x%x%x", 5,
		profileID, reason, listCount, FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE));
	for(; listCount > 0 ; listCount--)
	{
		if((pDataCtl = app_ReConn_GetTarget()) != NULL && FW_CmpBdAddr(pBdAddr, &pDataCtl->bdAddr))
		{
			DBG_LOG_APP_Connection( "[APP_ReConn] DeleteProfileMask mask:0x%x, isActiveLink:%d", 2, pDataCtl->reconnProfileMask, APP_IsActiveLink(pBdAddr));
			pDataCtl->reconnProfileMask &= ~app_ReConn_ProfileIDToMask(profileID);
			if(!pDataCtl->reconnProfileMask && !APP_IsActiveLink(pBdAddr))
			{
                APP_Media_PushMediaEvent(MEDIA_EVT_RECONNECT_FAILED);
				APP_Reconn_SetDelayTimer( RECONNECT_CHECK, ((reason == SDP_OPEN_SEARCH_ACL_LINK_ALREADY_EXIST) ? ONE_SEC : App_ReConnNvKey_GetReconnectDelayTime() * ONE_SEC));
			}
			break;
		}
	}
}

U8 APP_ReConn_GetProfileMaskByBdAddr(BD_ADDR_T *pBdAddr)
{
	APP_RECONNECT_DATA_STRU *pDataCtl;
	U8 listCount = app_ReConn_GetListCnt(LIST_HIGH_PRI_DATA) + app_ReConn_GetListCnt(LIST_NORMAL_DATA);

	for(; listCount > 0 ; listCount--)
	{
		if((pDataCtl = app_ReConn_GetTarget()) != NULL && FW_CmpBdAddr(pBdAddr, &pDataCtl->bdAddr))
		{
			return app_Reconn_GetProfileMask(pDataCtl);
		}
	}
	return 0;
}

void APP_ReConn_DeleteInvalidList(void)
{
	U8 linkIndex;
	BD_ADDR_T *pBdAddr;

	DBG_LOG_APP_Connection( "[APP_ReConn] Delete Invalid List", 0);

	for(linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		pBdAddr = APP_GetBdAddrByLinkIndex(linkIndex);
		if(pBdAddr)
		{
			APP_ReConn_DeleteListByBdAddr(pBdAddr);
		}
	}
}

void APP_ReConn_ClearCtl(void)
{
	if(gAppReconnectCtl.pReconnectList)
	{
		FW_FreeMemory(gAppReconnectCtl.pReconnectList);
		gAppReconnectCtl.pReconnectList = NULL;
	}
}

BOOL APP_ReConn_ToUARTIndex(U8 index)
{
	if(index >= APP_LinkKey_GetNormalPDLNumber())
	{
		index -= APP_LinkKey_GetNormalPDLNumber();

        app_ReConn_AddListByFastDongle(RECONNECT_USER_INIT, index);
	}
	else
	{
		if(!APP_GetAppLinkByBdAddr(APP_LinkKey_GetNormalLinkBdAddr(index)))
		{
			app_ReConn_AddListByLinkKeyIndex(RECONNECT_USER_INIT, index);
		}
		else
		{
			return FALSE;
		}
	}

	app_ReConn_Start(RECONNECT_USER_INIT);

	return TRUE;
}

void APP_Reconn_SetDelayTimer(U8 type,U32 delayTime)
{
	FW_SetTimer((Handler)&gAppReconnHandler, type, NULL, 0, delayTime);
}

void APP_Reconn_SetLinkLossBdAddr(BD_ADDR_T *pBdAddr)
{
	if(pBdAddr && APP_LinkKey_IsBdAddrInHistory(pBdAddr))
	{
		if(RECONNECT_UNNECESSARY == App_ReConnNvkey_GetActionAfterLinkLoss())
		{
			return;
		}
		app_ReConn_AddListByBdAddr(RECONNECT_LINK_LOSS, pBdAddr);
	}
}

void APP_ReConn_Reconnect(U8 type)
{
	// there are 3 stages, add reconnect data, check if reconnected is needed, reconnect

	U8 settings;

    DBG_LOG_APP_Connection( "[APP_ReConn] Reconnect. type:%d, isPowerOn:%d", 2, type, APP_PowerOff_IsPowerOn());

	if(!APP_PowerOff_IsPowerOn())
	{
		APP_ReConn_ClearCtl();
		return;
	}

	if(!APP_Conn_IsProfilesReady())
	{
		goto PROFILE_NOT_READY;
	}

	if(CURRENT_ACTIVE_LINK_CNT >= APP_Conn_GetSupportDeviceNo())
	{
		return;
	}

	PROFILE_NOT_READY:

	if(!APP_LinkKey_GetNormalPDLNumber() && !APP_LinkKey_CheckFCDKeyState())
	{
		APP_ReConn_ClearCtl();
		return;
	 }

	//add reconnect data
	switch(type)
	{
		case RECONNECT_PWR_ON:
			settings = App_ReConnNvkey_GetActionAfterPowerOn();
			goto KEY_RECONNECT;
		case RECONNECT_USER_INIT:
			settings = App_ReConnNvkey_GetActionUserInitiated();

			KEY_RECONNECT:
			if(0 == APP_LinkKey_GetNormalPDLNumber())
			{
				//app_ReConn_AddListByMode(type, RECONNECT_FAST_DONGLE);
			}
			if(APP_LinkKey_CheckFCDKeyState())
				settings = RECONNECT_FAST_DONGLE;
			app_ReConn_AddListByMode(type, settings);	//TO DO
			break;

		case RECONNECT_LAST_ONCE_AUDIOTRANSFER:
		case RECONNECT_LAST_ONCE_VOICE_DIAL:
			app_ReConn_AddListByMode(type, RECONNECT_LAST_DISC);
			break;

		case RECONNECT_FIRST_ONCE:
		case RECONNECT_FIRST_ONCE_VOICE_DIAL:
			app_ReConn_AddListByMode(type, RECONNECT_AG_IND_0);
			break;

		case RECONNECT_SECONDARY_ONCE:
		case RECONNECT_SECONDARY_ONCE_VOICE_DIAL:
			app_ReConn_AddListByMode(type, RECONNECT_AG_IND_1);
			break;

		case RECONNECT_LINK_LOSS:
			DBG_LOG_APP_Connection( "[APP_Reconn] ReConn OOR:%d,%d", 2, (U8)App_ReConnNvkey_GetActionAfterLinkLoss(), (U8)App_ReConnNvkey_GetLinkLossReTryCnt());
			break;

		case RECONNECT_WATCHDOG:
			app_ReConn_AddListByMode(type, RECONNECT_ALL_DROPPED_LINKS);
			break;

		case RECONNECT_CHECK:
			type = gAppReconnectCtl.type;
			app_ReConn_Start(type);
			return;
			break;

		case RECONNECT_AWSMCE:
			app_ReConn_AddListByMode(type, RECONNECT_AWSMCE_CONNECT);
			break;
	}
	if(type > RECONNECT_TYPE_NULL && type < RECONNECT_LINK_LOSS)
	{
		if(INVALID_RECONNECT_INDEX != app_Reconn_GetCurrentTargetIndex())
		{
			return;
		}
	}
	app_ReConn_Start(type);
}

void APP_ReConn_DeleteListByBdAddr(BD_ADDR_T * pBdAddr)
{
	U8 * pIndex;
	U8 isNormalData = TRUE;
	U8 index, i;
	APP_RECONNECT_DATA_STRU *pCtlData;

	//PM_ClearAllReconnectProfiles(pBdAddr);

	if(!gAppReconnectCtl.pReconnectList)
		return;

	DELETE_START:
	index = RECONNECT_MAX_LINK_CNT;
	if(isNormalData)
		pIndex = &gAppReconnectCtl.pReconnectList->currentTargetIndex;
	else
		pIndex = &gAppReconnectCtl.pReconnectList->currentHighPriorityIndex;

	//delete list by bdaddr
	while(index--)
	{
		if(isNormalData)
			pCtlData = &gAppReconnectCtl.pReconnectList->reconnectData[index];
		else
			pCtlData = &gAppReconnectCtl.pReconnectList->reconLinkLossWatchDogData[index];

		if(pCtlData->reconnectType != RECONNECT_TYPE_NULL)
		{
			if(FW_CmpBdAddr(&pCtlData->bdAddr, pBdAddr))
			{
				goto DELETE_RECONNECT_DATA;
			}

			if(pCtlData->retryCnt)
			{
				continue;
			}

			DELETE_RECONNECT_DATA:
			DBG_LOG_APP_Connection( "[APP_ReConn] Delete list bdAddr:0x%x%x", 2, FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE));

			FW_Memset(pCtlData, 0, sizeof(APP_RECONNECT_DATA_STRU));
			if(index <= *pIndex)
			{
				if(index)
					*pIndex = *pIndex-1;
				else
					*pIndex = INVALID_RECONNECT_INDEX;
			}
		}
	}

	//move list to head
	for(index = 0; index < RECONNECT_MAX_LINK_CNT ; index++)
	{
		if(isNormalData)
			pCtlData = &gAppReconnectCtl.pReconnectList->reconnectData[index];
		else
			pCtlData = &gAppReconnectCtl.pReconnectList->reconLinkLossWatchDogData[index];

		if(pCtlData->reconnectType == RECONNECT_TYPE_NULL)
		{
			for(i = index+1; i < RECONNECT_MAX_LINK_CNT; i++)
			{
				APP_RECONNECT_DATA_STRU * pBackUpData;

				if(isNormalData)
					pBackUpData = &gAppReconnectCtl.pReconnectList->reconnectData[i];
				else
					pBackUpData = &gAppReconnectCtl.pReconnectList->reconLinkLossWatchDogData[i];

				if(pBackUpData->reconnectType != RECONNECT_TYPE_NULL)
				{
					FW_Memcpy(pCtlData, pBackUpData, sizeof(APP_RECONNECT_DATA_STRU));
                    FW_Memset(pBackUpData, 0, sizeof(APP_RECONNECT_DATA_STRU));
					break;
				}
			}
		}
	}

	if(isNormalData)
	{
		isNormalData = FALSE;
		goto DELETE_START;
	}
}

BOOL App_IsProfileInReconnectList(BD_ADDR_T *pBdAddr, U8 profileId)
{
	return PM_IsProfileWaitingReconnect(pBdAddr, profileId);
}

void App_ClearProfileInReconnectList(BD_ADDR_T *pBdAddr, U8 profileId)
{
	PM_ClearOneReconnectProfile(pBdAddr, profileId);
}

bool APP_IsHFPConnecting(BD_ADDR_T *pBdAddr)
{
	if(app_ReConn_ProfileIDToMask(PROFILE_HANDSFREE) & APP_ReConn_GetProfileMaskByBdAddr(pBdAddr))
	{
		return TRUE;
	}
	return FALSE;
}
