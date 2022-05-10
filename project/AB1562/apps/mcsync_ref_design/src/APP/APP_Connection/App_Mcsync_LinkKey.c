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
#include "BtMCSync.h"
#include "App_LinkKey.h"
#include "App_Mcsync_LinkKey.h"
#include "App_Interface.h"
#include "MCSync.h"
#ifdef PROFILE_AMA_ENABLE
#include "BtAma.h"
#include "App_Ama.h"
#endif
#include "App_Connection.h"
#include "App_MCSync_Message.h"

/**************************************************************************************************
* Definition
**************************************************************************************************/
#define INVALID_LINK_KEY 0xFF

/**************************************************************************************************
* Sturcture
**************************************************************************************************/
typedef struct
{
    BDADDR_T bdaddr;
    U8 bonded : 1;
    U8 isBle : 1;
    U8 isEdr : 1;
    U8 reserved : 5;
    U8 link_key_size;
    U8 key_type;
    U8 link_key[16];
}GAP_LINK_INFO_STRU;

typedef struct
{
	BT_LINK_INFO_T gapLinkInfo;
	LINK_HISTORY_STRU appLinkInfo;
}APP_MCS_LINKKEY_STRU;

typedef struct
{
	U8 linkKeyNum;
	APP_MCS_LINKKEY_STRU *pLinkKey;
}RECORD_LINKKEY_STRU;

typedef struct
{
	U8 recordCnt;
	RECORD_LINKKEY_STRU local;
	RECORD_LINKKEY_STRU remote;
}APP_MCS_LINKKEY_CTL_STRU;

#ifdef PROFILE_AMA_ENABLE
typedef struct
{
    U8 numOfList;
    U32 *pList;
    U8 isAddBleDone;
}APP_MCS_BLELINKKEY_CTL_STRU;
#endif

/**************************************************************************************************
* Variable
**************************************************************************************************/
static APP_MCS_LINKKEY_CTL_STRU gAppMcsLinkKeyCtl;
#ifdef PROFILE_AMA_ENABLE
static APP_MCS_BLELINKKEY_CTL_STRU gAppMcsBleLinkKeyCtl;
#endif
static void app_Mcsync_GetLocalBleLinkKey(void);
static U32 app_MCSync_LinkKeyHandler(Handler handler, U16 id, void *msg, U32 id_ext);
static const HandlerData gAppMCSyncLinkKeyHandle = { app_MCSync_LinkKeyHandler };

/**************************************************************************************************
* Static Function
**************************************************************************************************/
static void app_MCSync_UpdateRemoteLinkKey(U8 linkKeyNum, APP_MCS_LINKKEY_STRU *pLinkKey)
{
	BD_ADDR_T *pBdAddr;
	APP_MCS_LINKKEY_STRU * pCurrLinkInfo;
	U8 i, keyIdx, defaultRole = BtAwsMce_GetDefaultRole();

	DBG_LOG_APP_Connection( "[APPMcsLinkKey] Update Remote Link Key. defaultRole:0x%x Num:%d, pLinkKey:0x%x", 3, defaultRole, linkKeyNum, (U32)pLinkKey);


	if(!linkKeyNum || !pLinkKey)
		return;

	for(i = 0; i < linkKeyNum; i++)
	{
		if(BtAwsMce_IsDefaultRoleAgent())
		{
			pCurrLinkInfo = &pLinkKey[i];
			pBdAddr = (BD_ADDR_T *)pCurrLinkInfo->gapLinkInfo.bdaddr.addr;
			keyIdx = APP_LinkKey_SearchLinkKeyIndex(pBdAddr, TRUE);

<<<<<<< HEAD
			if(keyIdx == INDEX_EOF)
=======
			if(keyIdx == INDEX_EOF && !APP_IsActiveLink(pBdAddr))
>>>>>>> db20e11 (second commit)
			{

				DBG_LOG_APP_Connection( "[APPMcsLinkKey] Agent Add LinkKey. BdAddr:0x%x%x, a2dpVol:%d, scoVol:%d, scoMic:%d", 5,
					FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE), pCurrLinkInfo->appLinkInfo.a2dpSoundLevel, pCurrLinkInfo->appLinkInfo.scoSoundlevel, pCurrLinkInfo->appLinkInfo.scoMicSoundlevel);

				APP_LinkKey_AddNormalByHistoryInfo(&pCurrLinkInfo->appLinkInfo);
				BtSmAddAuthDevice((Handler)&gAppMCSyncLinkKeyHandle, pCurrLinkInfo->gapLinkInfo.bdaddr, pCurrLinkInfo->gapLinkInfo.link_key_size, pCurrLinkInfo->gapLinkInfo.key_type, pCurrLinkInfo->gapLinkInfo.link_key);
			}
		}
		else if(BtAwsMce_IsDefaultRolePartner())
		{
			pCurrLinkInfo = &pLinkKey[(linkKeyNum - 1) - i];
			pBdAddr = (BD_ADDR_T *)pCurrLinkInfo->gapLinkInfo.bdaddr.addr;
			keyIdx = APP_LinkKey_AddNormalByHistoryInfo(&pCurrLinkInfo->appLinkInfo);


			DBG_LOG_APP_Connection( "[APPMcsLinkKey] Partner Add LinkKey. BdAddr:0x%x%x, a2dpVol:%d, scoVol:%d, scoMic:%d", 5,

				FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE), pCurrLinkInfo->appLinkInfo.a2dpSoundLevel, pCurrLinkInfo->appLinkInfo.scoSoundlevel, pCurrLinkInfo->appLinkInfo.scoMicSoundlevel);

			BtSmAddAuthDevice((Handler)&gAppMCSyncLinkKeyHandle, pCurrLinkInfo->gapLinkInfo.bdaddr, pCurrLinkInfo->gapLinkInfo.link_key_size, pCurrLinkInfo->gapLinkInfo.key_type, pCurrLinkInfo->gapLinkInfo.link_key);
			APP_LinkKey_ShiftLinkKeyToTop(keyIdx);
		}
	}

	APP_Reconn_CheckReconnect();
	APP_Mcsync_LinkKeyInit();
}

static void app_MCSync_GetLocalLinkKeyDone(void)
{
	//partner send link key to agent
	if(BtAwsMce_IsDefaultRolePartner())
	{
		U8 invalidLinkKey = INVALID_LINK_KEY;


		DBG_LOG_APP_Connection( "[APPMcsLinkKey] Partner Send Link Key. local Num:%d", 1, gAppMcsLinkKeyCtl.local.linkKeyNum);


		if(gAppMcsLinkKeyCtl.local.linkKeyNum)
			APP_MCSYNC_SendLinkList((U8 *)gAppMcsLinkKeyCtl.local.pLinkKey, gAppMcsLinkKeyCtl.local.linkKeyNum * sizeof(APP_MCS_LINKKEY_STRU));
		else
			APP_MCSYNC_SendLinkList(&invalidLinkKey, sizeof(U8));
	}
	else if(BtAwsMce_IsDefaultRoleAgent())
	{

		DBG_LOG_APP_Connection( "[APPMcsLinkKey] Agent Send Link Key. local Num:%d, remote Num:%d", 2, gAppMcsLinkKeyCtl.local.linkKeyNum, gAppMcsLinkKeyCtl.remote.linkKeyNum);

		if(gAppMcsLinkKeyCtl.local.linkKeyNum)
			APP_MCSYNC_SendLinkList((U8 *)gAppMcsLinkKeyCtl.local.pLinkKey, gAppMcsLinkKeyCtl.local.linkKeyNum * sizeof(APP_MCS_LINKKEY_STRU));

		if(gAppMcsLinkKeyCtl.remote.linkKeyNum && gAppMcsLinkKeyCtl.remote.pLinkKey)
			app_MCSync_UpdateRemoteLinkKey(gAppMcsLinkKeyCtl.remote.linkKeyNum, gAppMcsLinkKeyCtl.remote.pLinkKey);
	}
}

static U32 app_MCSync_LinkKeyHandler(Handler handler, U16 id, void *msg, U32 id_ext)
{
	UNUSED(handler);
	UNUSED(id_ext);
	LINK_HISTORY_STRU *pAppLinkInfo;
	BD_ADDR_T *pBdAddr;

	switch(id)
	{
		case BT_SM_GET_AUTH_DEVICE_CFM:
			pBdAddr = (BD_ADDR_T *)&((BT_SM_GET_AUTH_DEVICE_CFM_T *)msg)->linkInfo.bdaddr.addr[0];

			DBG_LOG_APP_Connection( "[APPMcsLinkKey] Get Auth Device Cfm status:%d, BdAddr:0x%x%x, cnt:%d, Num:%d", 5, ((BT_SM_GET_AUTH_DEVICE_CFM_T *)msg)->status, FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE),
				gAppMcsLinkKeyCtl.recordCnt, gAppMcsLinkKeyCtl.local.linkKeyNum);

			if(!gAppMcsLinkKeyCtl.local.pLinkKey || gAppMcsLinkKeyCtl.recordCnt >= gAppMcsLinkKeyCtl.local.linkKeyNum)
				break;

			if(((BT_SM_GET_AUTH_DEVICE_CFM_T *)msg)->status == BT_STATUS_SUCCESS)
			{
				if((pAppLinkInfo = APP_LinkKey_SearchHistoryByBdAddr(pBdAddr, FALSE, FALSE)))
				{
					FW_Memcpy(&gAppMcsLinkKeyCtl.local.pLinkKey[gAppMcsLinkKeyCtl.recordCnt].gapLinkInfo, &((BT_SM_GET_AUTH_DEVICE_CFM_T *)msg)->linkInfo, sizeof(BT_LINK_INFO_T));
					FW_Memcpy(&gAppMcsLinkKeyCtl.local.pLinkKey[gAppMcsLinkKeyCtl.recordCnt].appLinkInfo, pAppLinkInfo, sizeof(LINK_HISTORY_STRU));
					gAppMcsLinkKeyCtl.recordCnt++;
				}
				else
				{
					DBG_LOG_APP_Connection( "[APPMcsLinkKey] Get NULL Link Key History. BdAddr:0x%x%x", 2,
						FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE));

					gAppMcsLinkKeyCtl.local.linkKeyNum--;
				}
			}
			else
			{
				gAppMcsLinkKeyCtl.local.linkKeyNum--;
			}

			if(gAppMcsLinkKeyCtl.recordCnt == gAppMcsLinkKeyCtl.local.linkKeyNum)
			{
				app_MCSync_GetLocalLinkKeyDone();
				APP_Mcsync_LinkKeyInit();
			}
			break;

		case BT_SM_ADD_AUTH_DEVICE_CFM:
			DBG_LOG_APP_Connection( "[APPMcsLinkKey] Add Auth Device Cfm status:%d", 1, ((BT_SM_ADD_AUTH_DEVICE_CFM_T *)msg)->status);
			break;

        case BT_SM_GET_AUTH_DEVICE_LIST_EX_CFM:
            DBG_LOG_APP_Connection( "[APPMcsLinkKey] SM_GET_AUTH_DEVICE_LIST_EX_CFM len:%d", 1, ((BT_SM_GET_AUTH_DEVICE_LIST_EX_CFM_T *)msg)->length);
            /* 2. agent send ble ltk to partner */
            /* 5. partner send ble ltk to agent */
            bt_mcsync_send_data_by_module_id(MCSYNC_MODULE_BT_BLE_LINK_LIST, ((BT_SM_GET_AUTH_DEVICE_LIST_EX_CFM_T *)msg)->length, ((BT_SM_GET_AUTH_DEVICE_LIST_EX_CFM_T *)msg)->list);
#ifdef PROFILE_AMA_ENABLE
            APP_Ama_GetData();
#endif
            break;

        case BT_SM_ADD_AUTH_DEVICE_LIST_CFM:
        {
            BT_SM_ADD_AUTH_DEVICE_LIST_CFM_T *cfm = (BT_SM_ADD_AUTH_DEVICE_LIST_CFM_T *)msg;

            DBG_LOG_APP_Connection( "[APPMcsLinkKey] SM_ADD_AUTH_DEVICE_LIST_CFM num:%d, data: 0x04%x 0x04%x", 3, cfm->numOfList, cfm->keyIdList[0].newKeyId, cfm->keyIdList[1].newKeyId);

#ifdef PROFILE_AMA_ENABLE
            if (NULL == gAppMcsBleLinkKeyCtl.pList)
            {
                gAppMcsBleLinkKeyCtl.pList = FW_GetMemory(cfm->numOfList * sizeof(U32));
            }
            if (gAppMcsBleLinkKeyCtl.pList)
            {
                FW_Memcpy((U8 *)gAppMcsBleLinkKeyCtl.pList, cfm->keyIdList, cfm->numOfList * sizeof(U32));
                gAppMcsBleLinkKeyCtl.numOfList = cfm->numOfList;
            }
            else
            {
                gAppMcsBleLinkKeyCtl.numOfList = 0;
            }
            gAppMcsBleLinkKeyCtl.isAddBleDone = TRUE;
#endif

            /* 4. partner get ble ltk */
            if (BtAwsMce_IsDefaultRolePartner())
            {
                app_Mcsync_GetLocalBleLinkKey();
            }
            break;
        }
	}

	return 0;
}

static void app_Mcsync_GetLocalLinkKey(void)
{
	BD_ADDR_T *pBdAddr;
	BDADDR_T authDev;
	APP_MCS_LINKKEY_STRU *pLinkKey;
	U8 i, normalCnt = APP_LinkKey_GetNormalLinkNum();

	for(i = 0; i < normalCnt; i++)
	{
		if(!APP_LinkKey_CheckKeyStatusOK(i))
			continue;

		if((pBdAddr = APP_LinkKey_GetNormalLinkBdAddr(i)) && !FW_IsBdAddrZero(pBdAddr) && !FW_CmpBdAddr(pBdAddr, BtMCSync_ReadAgentBdAddr()))
		{
			authDev.type = 0;
			FW_Memcpy(authDev.addr, pBdAddr, sizeof(BD_ADDR_T));
			BtSmGetAuthDevice((Handler)&gAppMCSyncLinkKeyHandle, authDev);
			gAppMcsLinkKeyCtl.local.linkKeyNum++;
		}
	}

	DBG_LOG_APP_Connection( "[APPMcsLinkKey] Get Local Link Key. Num:%d, Sent Num:%d", 2, normalCnt, gAppMcsLinkKeyCtl.local.linkKeyNum);

	if(gAppMcsLinkKeyCtl.local.linkKeyNum &&
		(pLinkKey = FW_GetMemory( gAppMcsLinkKeyCtl.local.linkKeyNum * sizeof(APP_MCS_LINKKEY_STRU) )))
	{
		gAppMcsLinkKeyCtl.local.pLinkKey = pLinkKey;
	}
	else
	{
		gAppMcsLinkKeyCtl.local.linkKeyNum = 0;
		app_MCSync_GetLocalLinkKeyDone();
	}
}

static void app_Mcsync_GetLocalBleLinkKey(void)
{
    if (BtAwsMce_IsDefaultRolePartner())
	{
        BtSmGetAuthDeviceListEx((Handler)&gAppMCSyncLinkKeyHandle, SEARCH_TYPE_BLE);
    }
    else if (BtAwsMce_IsDefaultRoleAgent())
    {
        BtSmGetAuthDeviceListEx((Handler)&gAppMCSyncLinkKeyHandle, SEARCH_TYPE_BLE);
    }
}

/**************************************************************************************************
* Public Function
**************************************************************************************************/
void APP_Mcsync_LinkKeyInit(void)
{
	if(gAppMcsLinkKeyCtl.local.pLinkKey)
		FW_FreeMemory(gAppMcsLinkKeyCtl.local.pLinkKey);

	if(gAppMcsLinkKeyCtl.remote.pLinkKey)
		FW_FreeMemory(gAppMcsLinkKeyCtl.remote.pLinkKey);

	FW_Memset(&gAppMcsLinkKeyCtl, 0, sizeof(APP_MCS_LINKKEY_CTL_STRU));
}

void APP_Mcsync_SyncLinkKey(void)
{
	if(BtAwsMce_IsDefaultRoleAgent())
	{
        /* 1. agent get local ble ltk */
        app_Mcsync_GetLocalBleLinkKey();
	}
	else if(BtAwsMce_IsDefaultRolePartner() && MCSYNC_LINK_CONNECTED == BtAwsMce_GetMcsyncState())
	{
		app_Mcsync_GetLocalLinkKey();
	}
}

<<<<<<< HEAD
=======
void APP_Mcsync_GetLocalLinkKey(void)
{
    app_Mcsync_GetLocalLinkKey();
}

>>>>>>> db20e11 (second commit)
void APP_Mcsync_SyncLinkKeyInd(U32 length, U8 *pData)
{
	APP_MCS_LINKKEY_STRU *pLinkKey;
	U32 linkKeyNum;
	//if agent, compare with local's and sort, send result to partner
	//if partner, save link key

	DBG_LOG_APP_Connection( "[APPMcsLinkKey] Sync Link Key Ind. length:%d, pData:0x%x", 2, length, (U32)pData);
	if(!length || !pData)
		return;

	linkKeyNum = length / sizeof(APP_MCS_LINKKEY_STRU);

	if(BtAwsMce_IsDefaultRoleAgent())
	{
		if(*pData != INVALID_LINK_KEY && linkKeyNum && (pLinkKey = (APP_MCS_LINKKEY_STRU *)FW_GetMemory( linkKeyNum * sizeof(APP_MCS_LINKKEY_STRU) )))
		{
			FW_Memcpy(pLinkKey, pData, linkKeyNum * sizeof(APP_MCS_LINKKEY_STRU));
			gAppMcsLinkKeyCtl.remote.linkKeyNum = linkKeyNum;
			gAppMcsLinkKeyCtl.remote.pLinkKey = pLinkKey;
		}

		app_Mcsync_GetLocalLinkKey();
	}
	else if(BtAwsMce_IsDefaultRolePartner())
	{
		app_MCSync_UpdateRemoteLinkKey(linkKeyNum, (APP_MCS_LINKKEY_STRU *)pData);
	}
}

void APP_Mcsync_SyncBleLinkKeyInd(U32 length, U8 *pData)
{
	//if agent, compare with local's and sort, send result to partner
	//if partner, save link key

	DBG_LOG_APP_Connection( "[APPMcsLinkKey] Sync BLE Link Key Ind. length:%d, pData:0x%x role %d %d", 4, length, (U32)pData, BtAwsMce_IsDefaultRoleAgent(), BtAwsMce_IsDefaultRolePartner());

    /* 6. agent add ble ltk of partner */
	if(BtAwsMce_IsDefaultRoleAgent())
	{
        if (length)
	    {
            BtSmAddAuthDeviceList((Handler)&gAppMCSyncLinkKeyHandle, length, pData);
        }
#ifdef PROFILE_AMA_ENABLE
        else
        {
            gAppMcsBleLinkKeyCtl.isAddBleDone = TRUE;
            if (gAppMcsBleLinkKeyCtl.pList)
            {
                FW_FreeMemory(gAppMcsBleLinkKeyCtl.pList);
                gAppMcsBleLinkKeyCtl.pList = NULL;
                gAppMcsBleLinkKeyCtl.numOfList = 0;
            }
        }
#endif
	}
	else if(BtAwsMce_IsDefaultRolePartner())
	{
        if (length)
	    {
            /* 3. partner add ble ltk of agent */
            BtSmAddAuthDeviceList((Handler)&gAppMCSyncLinkKeyHandle, length, pData);
        }
        else
        {
#ifdef PROFILE_AMA_ENABLE
            gAppMcsBleLinkKeyCtl.isAddBleDone = TRUE;
            if (gAppMcsBleLinkKeyCtl.pList)
            {
                FW_FreeMemory(gAppMcsBleLinkKeyCtl.pList);
                gAppMcsBleLinkKeyCtl.pList = NULL;
                gAppMcsBleLinkKeyCtl.numOfList = 0;
            }
#else 
            /* 4. partner get ble ltk */
            app_Mcsync_GetLocalBleLinkKey();
#endif
        }
	}
}

#ifdef PROFILE_AMA_ENABLE
void APP_Mcsync_SyncAmaData(void)
{
    if (MCSYNC_LINK_CONNECTED != MCSYNC_GetMcsyncState())
    {
        return;
    }

    app_Mcsync_GetLocalBleLinkKey();
}

void APP_Mcsync_SyncAmaDataFromPartner(void)
{
    if (MCSYNC_LINK_CONNECTED != MCSYNC_GetMcsyncState())
    {
        return;
    }

    if(BtAwsMce_IsDefaultRolePartner())
    {
        app_Mcsync_GetLocalBleLinkKey();
    }
}


void APP_Mcsync_SendAmaData(U8 *pData, U16 length)
{
    if (MCSYNC_LINK_CONNECTED != MCSYNC_GetMcsyncState())
    {
        return;
    }
    bt_mcsync_send_data_by_module_id(MCSYNC_MODULE_AMA_DATA, length, pData);
}

U8 APP_Mcsync_SyncAmaDataInd(U32 length, U8 *pData)
{
    DBG_LOG_APP_Connection( "[APPMcsLinkKey] Sync AMA data Ind. length:%d, pData:0x%x, isDone %d role %d %d", 5, length, (U32)pData, gAppMcsBleLinkKeyCtl.isAddBleDone, BtAwsMce_IsDefaultRoleAgent(), BtAwsMce_IsDefaultRolePartner());

    if (0 == gAppMcsBleLinkKeyCtl.isAddBleDone)
    {
        return 2;
    }

	if(BtAwsMce_IsDefaultRoleAgent())
	{
        AMA_Target_AddDataApplyAgent(pData, length, (U8 *)gAppMcsBleLinkKeyCtl.pList, gAppMcsBleLinkKeyCtl.numOfList);

    }
    else if(BtAwsMce_IsDefaultRolePartner())
	{
        AMA_Target_AddDataApplyPartner(pData, length, (U8 *)gAppMcsBleLinkKeyCtl.pList, gAppMcsBleLinkKeyCtl.numOfList);
    }

    gAppMcsBleLinkKeyCtl.isAddBleDone = 0;
    if (gAppMcsBleLinkKeyCtl.pList)
    {
        FW_FreeMemory(gAppMcsBleLinkKeyCtl.pList);
        gAppMcsBleLinkKeyCtl.pList = 0;
    }
    gAppMcsBleLinkKeyCtl.numOfList = 0;

    return 0;
}
#else
void APP_Mcsync_SyncBleLinkKey(void)
{
    if (MCSYNC_LINK_CONNECTED != MCSYNC_GetMcsyncState())
    {
        return;
    }

    app_Mcsync_GetLocalBleLinkKey();
}

#endif

