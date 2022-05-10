/* Copyright Statement:
 *
 * (C) 2018  Airoha Technology Corp. All rights reserved.
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
#include "App_Ble.h"
#include "App_PowerOff.h"
#include "App_Connection.h"
#include "App_MCSync_RHO.h"
<<<<<<< HEAD
=======
#include "APP_AirApp.h"
>>>>>>> db20e11 (second commit)

log_create_module(APP_BLE, PRINT_LEVEL_INFO);
/**************************************************************************************************
* Type define
**************************************************************************************************/
#define DBG_LOG_APP_BLE(_message, arg_cnt, ...)      LOG_MSGID_I(APP_BLE, _message, arg_cnt, ##__VA_ARGS__)
#define NUM_OF_LE_ACL_LINKS                         2   /* according to the number of controller support */
#define APP_BLE_GATT_EXCHANGE_MTU                   273

typedef enum
{
    APP_BLE_GATT_SERVER_STATE_IDLE,
    APP_BLE_GATT_SERVER_STATE_STARTING,
    APP_BLE_GATT_SERVER_STATE_STARTED,
} APP_BLE_GATT_SERVER_STATE_E;

typedef enum
{
    APP_BLE_ACTION_GATT_SERVER_READY,
    APP_BLE_ACTION_ENABLE_ADV,
    APP_BLE_ACTION_DISABLE_ADV,
} APP_BLE_ACTION_E;

typedef struct
{
    BD_ADDR_T bdAddr;
    U8 isConnected:1;
} APP_BLE_LINK_INFO_T;

typedef struct
{
    APP_BLE_LINK_INFO_T linkInfo[NUM_OF_LE_ACL_LINKS];
    U8 gattServerState;
    U8 advHandleMask;
} APP_BLE_CTL_T;

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static U32 app_BleHandler(Handler handler, U16 id, void *msg, U32 handler_id);


/**************************************************************************************************
* Static Variables
**************************************************************************************************/
static const HandlerData gAppBleHandle = { app_BleHandler };

static APP_BLE_CTL_T gAppBleCtl;
static APP_BLE_ADV_HANDLE_NODE *gAppBleAdvHandleNodeList;

/**************************************************************************************************
* Static Functions
**************************************************************************************************/
static APP_BLE_ADV_HANDLE_DESC const *app_BleAdvHandleIsExist(U8 advHandleMask)
{
    APP_BLE_ADV_HANDLE_DESC const *appBleAdvDesc = (APP_BLE_ADV_HANDLE_DESC CODE_PTR)NULL;
    APP_BLE_ADV_HANDLE_NODE *advNode = gAppBleAdvHandleNodeList;

    while (advNode)
    {
        if(!advNode->appBleAdvDesc)
        {
            FW_Assert(FALSE);
            return appBleAdvDesc;
        }
        if (advNode->appBleAdvDesc->advHandleMask == advHandleMask)
        {
            appBleAdvDesc = advNode->appBleAdvDesc;
            return appBleAdvDesc;
        }
        advNode = advNode->nextNode;
    }

    return appBleAdvDesc;
}

static BOOL app_BleAction(APP_BLE_ADV_HANDLE_DESC const *pDesc, U8 action)
{
    switch (action)
    {
        case APP_BLE_ACTION_GATT_SERVER_READY:
        {
            if (pDesc->appBleGattServerReady)
            {
                pDesc->appBleGattServerReady();
                return TRUE;
            }
        }
        break;

        case APP_BLE_ACTION_ENABLE_ADV:
        {
            if (pDesc->appBleAdvEnable)
            {
                if (APP_PowerOff_IsPowerOn())
                {
                    return pDesc->appBleAdvEnable();
                }
                else
                {
                    return TRUE;    /* if power off,  save the mask for later use */
                }
            }
        }
        break;

        case APP_BLE_ACTION_DISABLE_ADV:
        {
            if (pDesc->appBleAdvDisable)
            {
                return pDesc->appBleAdvDisable();
            }
        }
        break;

        default:
            break;
    }
    return FALSE;
}

static U8 app_BleSetAdvHandleAction(U8 advHandleMask, U8 action)
{
    U8 validMask = 0;
    APP_BLE_ADV_HANDLE_NODE *pAdvHandleNode = gAppBleAdvHandleNodeList;

    DBG_LOG_APP_BLE( "[APP_BLE] app_BleSetAdvHandleAction pAdvHandleNode:0x%X gAppBleAdvHandleNodeList:0x%X", 2, pAdvHandleNode, gAppBleAdvHandleNodeList);

    while (pAdvHandleNode)
    {
        if (!pAdvHandleNode->appBleAdvDesc)
        {
            FW_Assert(FALSE);
            return 0;
        }
        if (pAdvHandleNode->appBleAdvDesc->advHandleMask & advHandleMask)
        {
            if (app_BleAction(pAdvHandleNode->appBleAdvDesc, action))
            {
                validMask |= pAdvHandleNode->appBleAdvDesc->advHandleMask;
<<<<<<< HEAD
            }            
        }
        pAdvHandleNode = pAdvHandleNode->nextNode;
    }
    
=======
            }
        }
        pAdvHandleNode = pAdvHandleNode->nextNode;
    }

>>>>>>> db20e11 (second commit)
    return validMask;
}

static void app_BleHandleStartGattServerCfm(BT_GATT_START_SERVER_CFM_T *cfm)
{
    U8 validMask;
<<<<<<< HEAD
    
    DBG_LOG_APP_BLE( "[APP_BLE] APP_Ble_HandleStartServerCfm status:%d", 1, cfm->status);
    
=======

    DBG_LOG_APP_BLE( "[APP_BLE] APP_Ble_HandleStartServerCfm status:%d", 1, cfm->status);

>>>>>>> db20e11 (second commit)
    if (cfm->status == BT_STATUS_SUCCESS)
    {
        gAppBleCtl.gattServerState = APP_BLE_GATT_SERVER_STATE_STARTED;

        validMask = app_BleSetAdvHandleAction(BLE_ADV_ALL, APP_BLE_ACTION_GATT_SERVER_READY);
<<<<<<< HEAD
        
=======

>>>>>>> db20e11 (second commit)
        DBG_LOG_APP_BLE( "[APP_BLE] APP_Ble_HandleStartServerCfm validMask:%d", 1, validMask);

#ifdef PROFILE_GVA_ENABLE
        APP_Gva_Enable();
#endif
    }
}

static U32 app_BleHandler(Handler handler, U16 id, void *msg, U32 handler_id)
{
	UNUSED(handler);
	UNUSED(handler_id);

	switch(id)
	{
        case BT_ACL_OPENED_IND:
        {
            BT_ACL_OPENED_IND_T *ind = (BT_ACL_OPENED_IND_T *)msg;
<<<<<<< HEAD
            
=======

>>>>>>> db20e11 (second commit)
            DBG_LOG_APP_BLE("[APP_BLE] BT_ACL_OPENED_IND BdAddr:0x%x%x", 3, ind->status, FW_bdaddr_to_2U32((BD_ADDR_T *)ind->bdaddr, TRUE), FW_bdaddr_to_2U32((BD_ADDR_T *)ind->bdaddr, FALSE));
            if (!ind->status)
            {
                U8 i;
                for (i = 0; i < NUM_OF_LE_ACL_LINKS; i++)
                {
                    if (!gAppBleCtl.linkInfo[i].isConnected)
                    {
                        memcpy(&gAppBleCtl.linkInfo[i].bdAddr, ind->bdaddr, 6);
                        gAppBleCtl.linkInfo[i].isConnected = TRUE;
                        return 0;
                    }
                }
            }
        }
        break;

        case BT_ACL_CLOSED_IND:
        {
            BT_ACL_CLOSED_IND_T *ind = (BT_ACL_CLOSED_IND_T *)msg;
<<<<<<< HEAD
            
=======

>>>>>>> db20e11 (second commit)
            DBG_LOG_APP_BLE("[APP_BLE] BT_ACL_CLOSED_IND status:%d poweron:%d BdAddr:0x%x%x", 4, ind->status, APP_PowerOff_IsPowerOn(), FW_bdaddr_to_2U32((BD_ADDR_T *)ind->bdaddr, TRUE), FW_bdaddr_to_2U32((BD_ADDR_T *)ind->bdaddr, FALSE));
            if (!ind->status)
            {
                U8 i;
<<<<<<< HEAD
                if (APP_PowerOff_IsPowerOn())
                {
                    APP_Ble_EnableAdv(gAppBleCtl.advHandleMask);
                }
=======
                if (APP_PowerOff_IsPowerOn() && !APP_AirApp_FOTA_Reconnect())
                {
                    APP_Ble_EnableAdv(gAppBleCtl.advHandleMask);
                }
                else if (APP_PowerOff_IsPowerOn() && APP_AirApp_FOTA_Reconnect())
                {
                    APP_Ble_DisableAdv(BLE_ADV_ALL);
                }
                else if (!APP_MCSync_Rho_IsRhoing())
                {
                    APP_Ble_EnableAdv(BLE_ADV_PRIMARY_MASK);
                }
>>>>>>> db20e11 (second commit)

                for (i = 0; i < NUM_OF_LE_ACL_LINKS; i++)
                {
                    if (FW_CmpBdAddr(&gAppBleCtl.linkInfo[i].bdAddr, (BD_ADDR_T *)ind->bdaddr))
                    {
                        memset(&gAppBleCtl.linkInfo[i].bdAddr, 0, 6);
                        gAppBleCtl.linkInfo[i].isConnected = FALSE;
                    }
                }
            }
        }
        break;

        /* BLE control */
        case BT_GATT_START_SERVER_CFM:
            app_BleHandleStartGattServerCfm((BT_GATT_START_SERVER_CFM_T *)msg);
            break;
<<<<<<< HEAD
            
=======

>>>>>>> db20e11 (second commit)
        case BT_GATT_EXCHANGE_MTU_IND:
        {
            BT_GATT_EXCHANGE_MTU_IND_T *ind = (BT_GATT_EXCHANGE_MTU_IND_T *)msg;
            DBG_LOG_APP_BLE("[APP_BLE] BT_GATT_EXCHANGE_MTU_IND mtu:%d", 1, ind->peerMtu);
            BtGattExchangeMtuResponse(ind->bdAddr, APP_BLE_GATT_EXCHANGE_MTU);
        }
        break;

        case BT_BLE_DISCONNECT_REQ_RSP:
        {
            BT_BLE_DISCONNECT_REQ_RSP_T *rsp = (BT_BLE_DISCONNECT_REQ_RSP_T *)msg;
            DBG_LOG_APP_BLE("[APP_BLE] BT_BLE_DISCONNECT_REQ_RSP status:%d bdAddr:0x%x%x", 3, rsp->status, FW_bdaddr_to_2U32((BD_ADDR_T *)rsp->bdAddr, TRUE), FW_bdaddr_to_2U32((BD_ADDR_T *)rsp->bdAddr, FALSE));
            APP_MCSync_Rho_BleDisconnectNotify(rsp->status);
        }
        break;

        default:
            break;
	}
    return 0;
}

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void APP_Ble_Init(void)
{
    BleSetLinkInfoNotifyHandler((Handler)&gAppBleHandle);
}

void APP_Ble_StartGattService(void)
{
    if (APP_Conn_IsProfilesReady() && APP_PowerOff_IsPowerOn() && APP_BLE_GATT_SERVER_STATE_IDLE == gAppBleCtl.gattServerState)
    {
        gAppBleCtl.gattServerState = APP_BLE_GATT_SERVER_STATE_STARTING;
        BtGattStartServer((Handler)&gAppBleHandle);
    }
}

BOOL APP_Ble_IsGattServerReady(void)
{
    if (APP_BLE_GATT_SERVER_STATE_STARTED != gAppBleCtl.gattServerState)
    {
        return FALSE;
    }
    return TRUE;
}

BOOL APP_Ble_Disconnect(BD_ADDR_T *pBdAddr)
{
    U8 i;
    BOOL notDisconn = TRUE;
<<<<<<< HEAD
    
=======

>>>>>>> db20e11 (second commit)
    if (NULL == pBdAddr)
    {
        for (i = 0; i < NUM_OF_LE_ACL_LINKS; i++)
        {
            if (gAppBleCtl.linkInfo[i].isConnected)
            {
                BleDisconnecReq((Handler)&gAppBleHandle, (U8 *)&gAppBleCtl.linkInfo[i].bdAddr, BT_DISCONNECT_REASON_REMOTE_USER_TERMINATED_CONNECTION);
                notDisconn = FALSE;
            }
        }
    }
    else
    {
        for (i = 0; i < NUM_OF_LE_ACL_LINKS; i++)
        {
            if (gAppBleCtl.linkInfo[i].isConnected && FW_CmpBdAddr(&gAppBleCtl.linkInfo[i].bdAddr, pBdAddr))
            {
                BleDisconnecReq((Handler)&gAppBleHandle, (U8 *)pBdAddr, BT_DISCONNECT_REASON_REMOTE_USER_TERMINATED_CONNECTION);
                return FALSE;
            }
        }
    }
    return notDisconn;
}

void APP_Ble_RegisterAdvHandle(APP_BLE_ADV_HANDLE_NODE *pAdvHandleNode)
{
    if (NULL == pAdvHandleNode || NULL == pAdvHandleNode->appBleAdvDesc)
    {
        return;
    }

    DBG_LOG_APP_BLE("[APP_BLE] RegisterAdvHandle advHandleMask:0x%x", 1, pAdvHandleNode->appBleAdvDesc->advHandleMask);
    if (NULL != app_BleAdvHandleIsExist(pAdvHandleNode->appBleAdvDesc->advHandleMask))
    {
        DBG_LOG_APP_BLE("[APP_BLE] RegisterAdvHandle already!! advHandleMask:0x%x", 1, pAdvHandleNode->appBleAdvDesc->advHandleMask);
        return;
    }
<<<<<<< HEAD
    
=======

>>>>>>> db20e11 (second commit)
    pAdvHandleNode->nextNode = gAppBleAdvHandleNodeList;
    gAppBleAdvHandleNodeList = pAdvHandleNode;
}

BOOL APP_Ble_EnableAdv(U8 advHandleMask)
{
    U8 validMask = 0;
<<<<<<< HEAD
    
=======

>>>>>>> db20e11 (second commit)
    if (APP_BLE_GATT_SERVER_STATE_STARTED != gAppBleCtl.gattServerState)
    {
        DBG_LOG_APP_BLE("[APP_BLE] gattServer isn't ready! state:%d advHandleMask:0x%x", 2, gAppBleCtl.gattServerState, advHandleMask);
        return FALSE;
    }

    validMask = app_BleSetAdvHandleAction(advHandleMask, APP_BLE_ACTION_ENABLE_ADV);
    gAppBleCtl.advHandleMask |= validMask;

    DBG_LOG_APP_BLE("[APP_BLE] APP_Ble_EnableAdv actionMask:0x%x validMask:0x%x final:0x%x", 3, advHandleMask, validMask, gAppBleCtl.advHandleMask);

    if (validMask)
    {
        return TRUE;
    }
    return FALSE;

}

BOOL APP_Ble_DisableAdv(U8 advHandleMask)
{
    U8 validMask = 0;
<<<<<<< HEAD
    
=======

>>>>>>> db20e11 (second commit)
    if (APP_BLE_GATT_SERVER_STATE_STARTED != gAppBleCtl.gattServerState)
    {
        DBG_LOG_APP_BLE("[APP_BLE] gattServer isn't ready! state:%d advHandleMask:0x%x", 2, gAppBleCtl.gattServerState, advHandleMask);
        return FALSE;
    }

    validMask = app_BleSetAdvHandleAction(advHandleMask, APP_BLE_ACTION_DISABLE_ADV);
    gAppBleCtl.advHandleMask &= ~(validMask);

    DBG_LOG_APP_BLE("[APP_BLE] APP_Ble_DisableAdv actionMask:0x%x validMask:0x%x final:0x%x", 3, advHandleMask, validMask, gAppBleCtl.advHandleMask);

    if (validMask)
    {
        return TRUE;
    }
    return FALSE;
}

U8 APP_Ble_GetAdvMask(void)
{
    return gAppBleCtl.advHandleMask;
}

