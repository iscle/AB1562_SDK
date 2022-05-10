/* Copyright Statement:
 *
 * (C) 2017	 Airoha Technology Corp. All rights reserved.
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

<<<<<<< HEAD
#ifdef APP_TILE_ENABLE
=======
#ifdef AIR_TILE_ADV_ENABLE
>>>>>>> db20e11 (second commit)
#include "BtMCSync.h"
#include "APP_AirApp.h"
#include "App_Media.h"
#include "App_Ble.h"
#include "App_Avrcp.h"
#include "App_A2dp.h"
#include "AudioDSP_Registration.h"
<<<<<<< HEAD
=======
#include "App_Tile.h"
#include "App_RhoHandler.h"
>>>>>>> db20e11 (second commit)

log_create_module(APP_TILE, PRINT_LEVEL_INFO);
/**************************************************************************************************
* Type define
**************************************************************************************************/
#define DBG_LOG_APP_TILE(_message, arg_cnt, ...)      LOG_MSGID_I(APP_TILE, _message, arg_cnt, ##__VA_ARGS__)
#define APP_TILE_ADV_TYPE_FLAG_LEN                  2
#define APP_TILE_ADV_TYPE_LE_BD_ADDR_LEN            8
#define APP_TILE_ADV_TYPE_NUM                       2
#define APP_TILE_ADV_DATA_TOTAL_LEN                 (APP_TILE_ADV_TYPE_FLAG_LEN + APP_TILE_ADV_TYPE_LE_BD_ADDR_LEN + APP_TILE_ADV_TYPE_NUM)

#define APP_TILE_NAME               "LE-TILE"
#define APP_TILE_NAME_LEN           (sizeof(APP_TILE_NAME) - 1) /* minus '0/' */
#define APP_TILE_SR_DATA_LEN        (APP_TILE_NAME_LEN + 1)     /* plus dataType len */

#define APP_TILE_IA_SERVICE_UUID    {0x02,0x18}
#define APP_TILE_ALTER_CHARC_UUID   {0x06,0x2A}

#define APP_TILE_CONNECT_PARAM_UPDATE_ENABLE

typedef struct
{
    U8 srDataLen;
    U8 dataType;
    U8 name[APP_TILE_NAME_LEN];
} APP_TILE_SR_DATA_T;

#define APP_TILE_SR_DATA_TOTAL_LEN sizeof(APP_TILE_SR_DATA_T)

typedef enum
{
    APP_TILE_ACTION_DISCONNECT_BLE,
    APP_TILE_ACTION_ENABLE_ADV,
} APP_TILE_ACTION_E;

typedef enum
{
    APP_TILE_MCSYNC_STATE_DISCONNECTED,      /* agent does NOT connect with partner, or only special link(a+p without SP) */
    APP_TILE_MCSYNC_STATE_CONNECTED,         /* agent connected with partner, without A2DP and eSCO */
} APP_TILE_MCSYNC_STATE_E;

typedef enum
{
    APP_TILE_BLE_STATE_IDLE,
    APP_TILE_BLE_STATE_ADV_IND,
    APP_TILE_BLE_STATE_ADV_NON_CONN_IND,
    APP_TILE_BLE_CONNECTED,

    APP_TILE_BLE_DISCONNECTING,
} APP_TILE_BLE_STATE_E;

typedef struct
{
    APP_TILE_MCSYNC_STATE_E mcsyncState;
    APP_TILE_BLE_STATE_E bleState;
    BD_ADDR_T peerBleBdAddr;
    U16 alertLevelHdl;
    U8 callIsExist:1;
    U8 a2dpIsPlay:1;
} APP_TILE_CTL_T;

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static U32 app_TileHandler(Handler handler, U16 id, void *msg, U32 handler_id);
static BOOL app_TileGattServerReady(void);
static BOOL app_TileEnableAdv(void);
static BOOL app_TileDisableAdv(void);


/**************************************************************************************************
* Static Variables
**************************************************************************************************/
static APP_TILE_CTL_T gAppTileCtl;

static const HandlerData gAppTileHandle = { app_TileHandler };

static const APP_BLE_ADV_HANDLE_DESC gAppTileDesc =
{
    BLE_ADV_TILE_MASK,
    app_TileGattServerReady,
    app_TileEnableAdv,
    app_TileDisableAdv,    
};

static APP_BLE_ADV_HANDLE_NODE gAppTileNode = {
    &gAppTileDesc,
    NULL,
};

/* IA Service*/
static const BT_GATT_ADD_SERVICE_T gAppTileIasServicePara =
{
    .serviceType = PRIMARY_SERVICE,
    .uuidType = UUID_TYPE_16,
    //.uuid = {0x02,0x18},
    .uuid = APP_TILE_IA_SERVICE_UUID,
    .registerLinkStatusEvt = 1,
};

static const BT_GATT_ADD_CHARACTERISTIC_T gAppTileIasAlertLevel =
{
    .uuidType = UUID_TYPE_16,
    //.uuid = {0x06,0x2A},
    .uuid = APP_TILE_ALTER_CHARC_UUID,
    .property = (C_WRITE_NO_RSP),
    .permission = BT_ATT_RB_WP,
    .data_initial_length = 1,
    .data_maximum_length = 0,
    .char_data = NULL,
};

static const U8 gAppTileAdvData[] =
{
    /* ADV TYPE: FLAGS */
    APP_TILE_ADV_TYPE_FLAG_LEN,
    EIR_TYPE_FLAGS,
    BT_GAP_LE_AD_FLAG_BR_EDR_NOT_SUPPORTED | BT_GAP_LE_AD_FLAG_GENERAL_DISCOVERABLE,

    /* ADV TYPE: LE BLUETOOTH DEVICE ADDRESS  */
    APP_TILE_ADV_TYPE_LE_BD_ADDR_LEN,
    EIR_TYPE_LE_BLUETOOTH_DEVICE_ADDRESS,
    BLE_ADDR_TYPE_RANDOM_DEVICE_ADDR,
};

#define APP_TILE_ADV_DATA_LEN   (sizeof(gAppTileAdvData))

static const APP_TILE_SR_DATA_T gAppTileRspData =
{
    .srDataLen = APP_TILE_SR_DATA_LEN,
    .dataType = EIR_TYPE_COMPLETE_LOCAL_NAME,
    .name = APP_TILE_NAME,
};

static const BLE_EXT_ADV_PARAM_T gAppTileExtAdvParam =
{
    .advType = BLE_ADV_IND,
    .ownAddrType = RANDOM_DEVICE_ADDR,
    .AdvParms = 
    {
        .undirectedAdvParams = 
        {
            .advIntervalMin = 0x0C80,           /* 2s */
            .advIntervalMax = 0x0C80,           /* 2s */
            .filterPolicy = BLE_FILTER_NONE,
         }
    },
    .channelMap = 0x07,
};

#ifdef APP_TILE_CONNECT_PARAM_UPDATE_ENABLE
static const BLE_CONN_PARAMS_UPDATE_T gAppTileConnParamUpdate =
{
    .addrType = BLE_ADDR_TYPE_RANDOM_DEVICE_ADDR,
    .bdAddr.addr = {0, 0, 0, 0, 0, 0},
    .connIntervalMin = 108,     /* unit: 1.25 ms, iOS: 15ms *n */
    .connIntervalMax = 120,    /* unit: 1.25 ms, iOS: interval min +15ms <= interval max OR interval min = interval max = 15ms */
    .connLatency = 1,           /* 0 ~ ((supervisonTimeout/(connIntervalMax*2))-1, < 500*/
    .supervisonTimeout = 600,   /* unit: 10 ms, iOS: 2s~6s */
};
#endif

/**************************************************************************************************
* Static Functions
**************************************************************************************************/
static BOOL app_TileGattServerReady(void)
{
    DBG_LOG_APP_TILE("[APP_TILE] APP_Tile_NotifyGattServerReady", 0);

    BLE_EXT_ADV_PARAM_EX_T advParamEx;
    U8 pAdvData[BLE_MAX_DATA_SIZE] = {0};
        
    memset(&advParamEx, 0, sizeof(BLE_EXT_ADV_PARAM_EX_T));
    advParamEx.setAdvParam = TRUE;
    memcpy(&advParamEx.extAdvParam, (U8 *)&gAppTileExtAdvParam, sizeof(BLE_EXT_ADV_PARAM_T));

    advParamEx.setAdvData = TRUE;
    memcpy(pAdvData, gAppTileAdvData, APP_TILE_ADV_DATA_LEN);
    memcpy(&pAdvData[APP_TILE_ADV_DATA_LEN], (U8 *)NVKEY_GetPayloadFlashAddress(NVKEYID_BT_CON_BLE_BDADDR), 6);
    advParamEx.sizeAdvData = APP_TILE_ADV_DATA_TOTAL_LEN;
    advParamEx.advData = pAdvData;
    
    advParamEx.setScanRspData = TRUE;
    advParamEx.scanRspData = (U8 *)&gAppTileRspData;
    advParamEx.sizeRspData = APP_TILE_SR_DATA_TOTAL_LEN;
    DBG_LOG_APP_TILE("[APP_TILE] sizeRspData:%d", 1, advParamEx.sizeRspData);
    
    advParamEx.setRandomAddr = TRUE;
    advParamEx.extRadomAddr.randomAddrType = BLE_ADDR_WRITE_STATIC;
    memcpy(&advParamEx.extRadomAddr.RandomAddr, (U8 *)NVKEY_GetPayloadFlashAddress(NVKEYID_BT_CON_BLE_BDADDR), 6);
    
    BleSetExtendedAdvParamsExReq((Handler)&gAppTileHandle, BLE_ADV_TILE, &advParamEx);
    return TRUE;
}

static BOOL app_TileEnableAdv(void)
{
    BleSetExtendedAdvEnableReq((Handler)&gAppTileHandle, BLE_ADV_TILE, TRUE);
    return TRUE;
}

static BOOL app_TileDisableAdv(void)
{
    BleSetExtendedAdvEnableReq((Handler)&gAppTileHandle, BLE_ADV_TILE, FALSE);
    return TRUE;
}

static void app_TileSetAdvParam(BOOL isConnectable)
{
    DBG_LOG_APP_TILE("[APP_TILE] app_TileSetAdvParam connectable:%d", 1, isConnectable);
    
    BLE_EXT_ADV_PARAM_EX_T advParamEx;

    memset(&advParamEx, 0, sizeof(BLE_EXT_ADV_PARAM_EX_T));
    advParamEx.setAdvParam = TRUE;
    memcpy(&advParamEx.extAdvParam, (U8 *)&gAppTileExtAdvParam, sizeof(BLE_EXT_ADV_PARAM_T));
    if (FALSE == isConnectable)
    {
        advParamEx.extAdvParam.advType = BLE_ADV_NON_CONN_IND;
    }
    BleSetExtendedAdvParamsExReq((Handler)&gAppTileHandle, BLE_ADV_TILE, &advParamEx);
}

static bool app_TileIsPartnerStreamingMode(void)
{
<<<<<<< HEAD
    DBG_LOG_APP_TILE("[APP_TILE] isPartner:%d mcsycstate:%d callIsExist:%d a2dpIsPlay:%d", 4, BtAwsMce_IsDefaultRolePartner(), gAppTileCtl.mcsyncState, gAppTileCtl.callIsExist, gAppTileCtl.a2dpIsPlay);
=======
    DBG_LOG_APP_TILE("[APP_TILE] isPartner:%d mcsycstate:%d callIsExist:%d a2dpIsPlay:%d", 4, BtAwsMce_IsDefaultRolePartner(),
		gAppTileCtl.mcsyncState, gAppTileCtl.callIsExist, gAppTileCtl.a2dpIsPlay);
>>>>>>> db20e11 (second commit)
    if (BtAwsMce_IsDefaultRolePartner() && gAppTileCtl.mcsyncState && 
        (gAppTileCtl.callIsExist || gAppTileCtl.a2dpIsPlay))
    {
        return TRUE;
    }
    return FALSE;
}

static void app_TileCheckNextState(void)
{
    DBG_LOG_APP_TILE("[APP_TILE] app_TileCheckNextState oldTileState:%d isPartnerStreaming:%d gattReady:%d", 3, gAppTileCtl.bleState, app_TileIsPartnerStreamingMode(), APP_Ble_IsGattServerReady());

    if (!APP_Ble_IsGattServerReady())
    {
        return;
    }
    
    switch (gAppTileCtl.bleState)
    {
        case APP_TILE_BLE_STATE_IDLE:
        {
            if (app_TileIsPartnerStreamingMode())
            {
                app_TileSetAdvParam(FALSE);
                gAppTileCtl.bleState = APP_TILE_BLE_STATE_ADV_NON_CONN_IND;
            }
            else
            {
                app_TileSetAdvParam(TRUE);
                gAppTileCtl.bleState = APP_TILE_BLE_STATE_ADV_IND;
            }
        }
        break;

        case APP_TILE_BLE_STATE_ADV_IND:
        {
            if (app_TileIsPartnerStreamingMode())
            {
                app_TileSetAdvParam(FALSE);
                gAppTileCtl.bleState = APP_TILE_BLE_STATE_ADV_NON_CONN_IND;
            }
        }
        break;

        case APP_TILE_BLE_STATE_ADV_NON_CONN_IND:
        {
            if (!app_TileIsPartnerStreamingMode())
            {
                app_TileSetAdvParam(TRUE);
                gAppTileCtl.bleState = APP_TILE_BLE_STATE_ADV_IND;
            }
        }
        break;

        case APP_TILE_BLE_CONNECTED:
        {
            if (app_TileIsPartnerStreamingMode())
            {
                BleDisconnecReq((Handler)&gAppTileHandle, gAppTileCtl.peerBleBdAddr.addr, BT_DISCONNECT_REASON_REMOTE_USER_TERMINATED_CONNECTION);
                gAppTileCtl.bleState = APP_TILE_BLE_DISCONNECTING;
            }
        }
        break;

        default:
            break;
    }
    DBG_LOG_APP_TILE("[APP_TILE] app_TileCheckNextState newState: %d", 1, gAppTileCtl.bleState);
}

static U32 app_TileHandler(Handler handler, U16 id, void *msg, U32 handler_id)
{
	UNUSED(handler);
	UNUSED(handler_id);

	switch(id)
	{
        case BT_BLE_SET_ADV_PARAMS_EX_CFM:
        {
            DBG_LOG_APP_TILE("[APP_TILE] BT_BLE_SET_ADV_PARAMS_EX_CFM status: %d", 1, ((BT_BLE_SET_ADV_PARAMS_EX_CFM_T *)msg)->status);
            if (BT_STATUS_SUCCESS == ((BT_BLE_SET_ADV_PARAMS_EX_CFM_T *)msg)->status)
            {
                APP_Ble_EnableAdv(BLE_ADV_TILE_MASK);
            }
        }
        break;

#ifdef APP_TILE_CONNECT_PARAM_UPDATE_ENABLE
        case BT_BLE_CONNECTION_PARAMS_UPDATE_CFM:
        {
            BT_BLE_CONNECTION_PARAMS_UPDATE_CFM_T *cfm = (BT_BLE_CONNECTION_PARAMS_UPDATE_CFM_T *)msg;
            
            DBG_LOG_APP_TILE("[APP_TILE] BT_BLE_CONNECTION_PARAMS_UPDATE_CFM status:%d connInterval:%d supervisionTimeout:%d", 3, cfm->status, cfm->connInterval, cfm->supervisionTimeout);
        }
        break;
#endif        
        
        case BT_ATT_LINK_STATUS_IND:
        {
            DBG_LOG_APP_TILE("[APP_TILE] app_TileHandler BT_ATT_LINK_STATUS_IND status:%d", 1, ((BT_ATT_LINK_STATUS_IND_T *)msg)->status);
            
            BT_ATT_LINK_STATUS_IND_T *ind = (BT_ATT_LINK_STATUS_IND_T *)msg;
            
            if (ATT_LINK_CONNECTED == ind->status)
            {
                if (gAppTileCtl.bleState != APP_TILE_BLE_CONNECTED)
                {
                    gAppTileCtl.bleState = APP_TILE_BLE_CONNECTED;
                    FW_Memcpy(&gAppTileCtl.peerBleBdAddr, ind->bdAddr, 6);

#ifdef APP_TILE_CONNECT_PARAM_UPDATE_ENABLE
                    BLE_CONN_PARAMS_UPDATE_T connParams;
                    FW_Memcpy(&connParams, &gAppTileConnParamUpdate, sizeof(BLE_CONN_PARAMS_UPDATE_T));
                    connParams.addrType = ind->type;
                    FW_Memcpy(&connParams.bdAddr, ind->bdAddr, 6);
                    BleConnectionParamsUpdateReq((Handler)&gAppTileHandle, &connParams);
#endif                    
                    
                    app_TileCheckNextState();
                }
            }
            else
            {
                if (gAppTileCtl.bleState != APP_TILE_BLE_STATE_IDLE)
                {
                    gAppTileCtl.bleState = APP_TILE_BLE_STATE_IDLE;
                    app_TileCheckNextState();
                }
            }
        }
        break;
        
        case BT_ATT_ACCESS_IND:
        {
            BT_ATT_ACCESS_IND_T *ind = (BT_ATT_ACCESS_IND_T *)msg;
            
            if (gAppTileCtl.alertLevelHdl == ind->handle)
            {
                DBG_LOG_APP_TILE("[APP_TILE] app_TileHandler BT_ATT_ACCESS_IND flag:%d len:%d value:%d", 3, ind->flags, ind->valueLen, ind->value[0]);
                
                if (ATT_ACCESS_WRITE_CMD == ind->flags && ind->valueLen && ind->value[0])
                {
                    APP_Media_PushMediaEvent(MEDIA_EVT_KEY_FIND_MY_SINGLE_ACCESSORY);
                    
                }
            }
        }
        break;
        
        default:
            break;
	}
    return 0;
}

/**************************************************************************************************
* Public Variables
**************************************************************************************************/
void APP_Tile_Init(void)
{
    DBG_LOG_APP_TILE("[APP_TILE] APP_Tile_Init", 0);
    /* IA Service (immediate alert service), the customer can replace by other service */
    BtGattAddService((Handler)&gAppTileHandle, &gAppTileIasServicePara);
    gAppTileCtl.alertLevelHdl = BtGattAddCharacteristic(&gAppTileIasAlertLevel);
    //BtAwsMceDisableAdvHandle();
    APP_Ble_RegisterAdvHandle(&gAppTileNode);
}

void APP_Tile_NotifyMcsyncState(U8 newState)
{
    DBG_LOG_APP_TILE("[APP_TILE] APP_Tile_NotifyMcsyncState old:%d new:%d mcsyncState:%d", 3, gAppTileCtl.mcsyncState, (MCSYNC_LINK_CONNECTED == newState), newState);
    if (MCSYNC_LINK_CONNECTED == newState)
    {
        if (gAppTileCtl.mcsyncState != APP_TILE_MCSYNC_STATE_CONNECTED)
        {
            gAppTileCtl.mcsyncState = APP_TILE_MCSYNC_STATE_CONNECTED;
            app_TileCheckNextState();
        }
    }
    else
    {
        if (gAppTileCtl.mcsyncState != APP_TILE_MCSYNC_STATE_DISCONNECTED)
        {
            gAppTileCtl.mcsyncState = APP_TILE_MCSYNC_STATE_DISCONNECTED;
            app_TileCheckNextState();
        }
    }
}

void APP_Tile_NotifyPartnerCallState(BOOL isExist)
{
    DBG_LOG_APP_TILE("[APP_TILE] APP_Tile_NotifyPartnerCallState old:%d new:%d", 2, gAppTileCtl.callIsExist, isExist);
    if (gAppTileCtl.callIsExist != isExist)
    {
        gAppTileCtl.callIsExist = isExist;
        app_TileCheckNextState();
    }
}

void APP_Tile_NotifyPartnerA2dpState(BOOL isPlay)
{
    DBG_LOG_APP_TILE("[APP_TILE] APP_Tile_NotifyPartnerA2dpState old:%d new:%d", 2, gAppTileCtl.a2dpIsPlay, isPlay);
    if (gAppTileCtl.a2dpIsPlay != isPlay)
    {
        gAppTileCtl.a2dpIsPlay = isPlay;
        app_TileCheckNextState();
    }
}

<<<<<<< HEAD
=======
/**************************************************************************************************
* Public API (RHO)
**************************************************************************************************/
BOOL APP_Tile_FillRhoData(void *pData)
{
	APP_RHO_TILE_STRU *pDataToFill = &((APP_RHO_DATA_STRU *)pData)->appTile;

	if (!pDataToFill) {
		DBG_LOG_APP_TILE("[APP_TILE] Fill RHO data error", 0);
		return FALSE;
	}

	pDataToFill->a2dpIsPlay = gAppTileCtl.a2dpIsPlay;
	pDataToFill->callIsExist = gAppTileCtl.callIsExist;

	DBG_LOG_APP_TILE("[APP_TILE] Fill RHO data OK, a2dpIsPlay is %d, callIsExist is %d", 2, pDataToFill->a2dpIsPlay, pDataToFill->callIsExist);
	return TRUE;
}

BOOL APP_Tile_AssignRhoData(void *pData)
{
	APP_RHO_TILE_STRU *pDataToGet = &((APP_RHO_DATA_STRU *)pData)->appTile;

	if(!pDataToGet)
	{
		DBG_LOG_APP_TILE("[APP_TILE] Assign RHO data error", 0);
		return FALSE;
	}

	gAppTileCtl.a2dpIsPlay = pDataToGet->a2dpIsPlay;
	gAppTileCtl.callIsExist = pDataToGet->callIsExist;


	DBG_LOG_APP_TILE("[APP_TILE] Assign RHO data OK, a2dpIsPlay is %d, callIsExist is %d", 2, gAppTileCtl.a2dpIsPlay, gAppTileCtl.callIsExist);
	return TRUE;
}

APP_RHO_TILE_STRU g_tempPartnerStatus = {0, 0};
void APP_Tile_Set_Partner_StreamState(U8 state)
{
    memset(&g_tempPartnerStatus, 0, sizeof(APP_RHO_TILE_STRU));
	if (state & 0x1) {
		g_tempPartnerStatus.a2dpIsPlay = 1;
	}
	if (state & 0x2) {
		g_tempPartnerStatus.callIsExist = 1;
	}
    DBG_LOG_APP_TILE("[APP_TILE] APP_Tile_Set_Partner_StreamState, a2dpIsPlay:%d, callIsExist:%d", 2, g_tempPartnerStatus.a2dpIsPlay, g_tempPartnerStatus.callIsExist);
}

U8 APP_Tile_Get_Partner_StreamState(void)
{
	U8 streamState = 0x0;

	if (gAppTileCtl.a2dpIsPlay) {
		streamState |= 0x1;
	}

	if (gAppTileCtl.callIsExist) {
		streamState |= 0x2;
	}
    DBG_LOG_APP_TILE("[APP_TILE] APP_Tile_Get_Partner_StreamState, streamState:%d", 1, streamState);
	return streamState;
}

void APP_Tile_Rho_done(BOOL isSuccess)
{
	if(isSuccess && BtAwsMce_IsDefaultRolePartner()) {//Assign new partner's data
		gAppTileCtl.a2dpIsPlay = g_tempPartnerStatus.a2dpIsPlay;
		gAppTileCtl.callIsExist = g_tempPartnerStatus.callIsExist;
	}
	DBG_LOG_APP_TILE("[APP_TILE] isPartner:%d mcsycstate:%d callIsExist:%d a2dpIsPlay:%d", 4, BtAwsMce_IsDefaultRolePartner(),
		gAppTileCtl.mcsyncState, gAppTileCtl.callIsExist, gAppTileCtl.a2dpIsPlay);

    if (BtAwsMce_IsDefaultRoleAgent()) {
	    app_TileSetAdvParam(TRUE);
		gAppTileCtl.bleState = APP_TILE_BLE_STATE_ADV_IND;
	} else if (app_TileIsPartnerStreamingMode()) {
		app_TileSetAdvParam(FALSE);
		gAppTileCtl.bleState = APP_TILE_BLE_STATE_ADV_NON_CONN_IND;
	} else {
	    app_TileSetAdvParam(TRUE);
		gAppTileCtl.bleState = APP_TILE_BLE_STATE_ADV_IND;
	}
    DBG_LOG_APP_TILE("[APP_TILE] APP_Tile_Rho_done, restart adv, adv type is %d", 1, gAppTileCtl.bleState);
}

>>>>>>> db20e11 (second commit)
#endif

