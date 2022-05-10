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
#include "bt_gatt.h"
#include "bt_gap.h"
#include "App_ResourceManager.h"
#include "AirApp.h"
#include "APP_AirApp.h"
#include "Pairing_NvkeyHandler.h"
#include "App_Interface.h"
#include "App_State.h"
#include "App_Pairing.h"
#include "PM_Interface.h"
#include "nvkey_list.h"
<<<<<<< HEAD
#include "gfps_api.h"
=======
>>>>>>> db20e11 (second commit)
#include "PM_Interface.h"
#include "App_Avrcp.h"
#include "App_A2dp.h"
#include "AudioDSP_StreamManager.h"
#include "App_Ble.h"

#define DEFAULT_MTU_SIZE          273
#define DEFAULT_AD_SIZE           28

log_create_module(APP_AIRAPP, PRINT_LEVEL_INFO);

#define DBG_LOG_APP_AIRAPP(_message, arg_cnt, ...)     LOG_MSGID_I(APP_AIRAPP, _message, arg_cnt, ##__VA_ARGS__)

static U32 APP_AirAppHandler(Handler handler, U16 id, void *msg, U32 handler_id);
BOOL APP_AirApp_DisconnectReq(BD_ADDR_T* bdAddr);
U32 App_AirApp_AdvEnableHandler(Handler t, U16 id, void *payload, U32 id_ext);
static BOOL app_AirAppGattServerReady(void);
static BOOL app_AirAppEnableAdv(void);
static BOOL app_AirAppDisableAdv(void);

static const APP_BLE_ADV_HANDLE_DESC gAppAirAppDesc =
{
    BLE_ADV_PRIMARY_MASK,
    app_AirAppGattServerReady,
    app_AirAppEnableAdv,
    app_AirAppDisableAdv,
};

static APP_BLE_ADV_HANDLE_NODE gAppAirAppNode = {
    &gAppAirAppDesc,
    NULL,
};

static const HandlerData gAppAirAppHandler = {APP_AirAppHandler};
static const HandlerData gAirAppAdvEnableHandler = {App_AirApp_AdvEnableHandler};



typedef struct AIRAPP_CTRL_T
{
    PROFILE_INFO SppInfo;
    PROFILE_INFO BleInfo;
    BD_ADDR_T SppAddr;
    BD_ADDR_T BleAddr;
    BOOL isSppConnected;
    BOOL isBleConnected;
}AIRAPP_CTRL_T;

static AIRAPP_CTRL_T gApp_AirAppCtrl =
{
    .SppAddr = {{0}},
    .BleAddr = {{0}},
    .isSppConnected = 0,
    .isBleConnected = 0,
};

const PROFILE_DESC gApp_AirAppProfileDesc =
{
	PROFILE_AIR_APP,
	NULL,
	APP_AirApp_DisconnectReq,
};

static PROFILE_NODE gApp_AirAppProfileNode =
{
	&gApp_AirAppProfileDesc,
	NULL,
};

static const BLE_EXT_ADV_PARAM_T gAppAirAppAdvParam =
{
    .advType = BLE_ADV_IND,
    .ownAddrType = RANDOM_DEVICE_ADDR,
    .AdvParms = 
    {
        .undirectedAdvParams = 
        {
            .advIntervalMin = 0x0320,
            .advIntervalMax = 0x0320,
            .filterPolicy = BLE_FILTER_NONE,
         }
    },
    .channelMap = 0x07,
};

static BOOL app_AirAppGattServerReady(void)
{
    DBG_LOG_APP_AIRAPP( "[APP_AIRAPP] APP_AirApp_NotifyGattServerReady", 0);

    BLE_EXT_ADV_PARAM_EX_T advParamEx;
    U8 pAdvData[BLE_MAX_DATA_SIZE] = {0};
    U8* ptr;

    /* check if enable AirApp ADV */
	ptr = (U8*)NVKEY_GetPayloadFlashAddress(NVKEYID_BT_BLE_BLE_ADV_ENABLE);
	if ((NULL == ptr) || (FALSE == *ptr))
	{
        DBG_LOG_APP_AIRAPP( "[APP_AIRAPP] NVKEYID ADV NOT ENABLE", 0);
		 return TRUE;
	}

    memset(&advParamEx, 0, sizeof(BLE_EXT_ADV_PARAM_EX_T));
    /* adv params */
    advParamEx.setAdvParam = TRUE;
    memset(&advParamEx, 0, sizeof(BLE_EXT_ADV_PARAM_EX_T));
    advParamEx.setAdvParam = TRUE;
    memcpy(&advParamEx.extAdvParam, (U8 *)&gAppAirAppAdvParam, sizeof(BLE_EXT_ADV_PARAM_T));

    /* adv data */
    advParamEx.setAdvData = TRUE;
<<<<<<< HEAD
    advParamEx.sizeAdvData = NVKEY_GetPayloadLength(NVKEYID_BT_BLE_BLE_ADV_DATA);
=======
    advParamEx.sizeAdvData = PM_Nvkey_GetBleAdvDataLength();
>>>>>>> db20e11 (second commit)
    if (advParamEx.sizeAdvData > BLE_MAX_DATA_SIZE)
    {
        advParamEx.sizeAdvData = BLE_MAX_DATA_SIZE;
    }
<<<<<<< HEAD
    memcpy(pAdvData, (U8 *)NVKEY_GetPayloadFlashAddress(NVKEYID_BT_BLE_BLE_ADV_DATA), advParamEx.sizeAdvData);
=======
    memcpy(pAdvData, (U8 *)PM_Nvkey_GetBleAdvData(), advParamEx.sizeAdvData);
>>>>>>> db20e11 (second commit)
    advParamEx.advData = pAdvData;

    /* adv random addr */
    advParamEx.setRandomAddr = TRUE;
    advParamEx.extRadomAddr.randomAddrType = BLE_ADDR_WRITE_STATIC;
    memcpy(&advParamEx.extRadomAddr.RandomAddr, (U8 *)NVKEY_GetPayloadFlashAddress(NVKEYID_BT_CON_BLE_BDADDR), 6);

    BleSetExtendedAdvParamsExReq((Handler)&gAppAirAppHandler, BLE_ADV_PRIMARY, &advParamEx);
    return TRUE;
}

static BOOL app_AirAppEnableBleAdv(BOOL enable)
{
    U8 *ptr;

    ptr = (U8 *)NVKEY_GetPayloadFlashAddress(NVKEYID_BT_BLE_BLE_ADV_ENABLE);
    if (NULL == ptr || FALSE == *ptr)
    {
        return FALSE;
        
    }
    BleSetAdvEnableReq((Handler)&gAppAirAppHandler, enable);
    return TRUE;
}

static BOOL app_AirAppEnableAdv(void)
{
    return app_AirAppEnableBleAdv(TRUE);
}

static BOOL app_AirAppDisableAdv(void)
{
    return app_AirAppEnableBleAdv(FALSE);
}

BOOL APP_AirApp_FOTA_Reconnect(void)
{

    U8* ptr;
    /*NvkeyDefine NVKEYID_FOTA_FOTA_PARA_RECONNECT*/
    ptr = (U8*)NVKEY_GetPayloadFlashAddress(NVKEYID_FOTA_FOTA_PARA_RECONNECT);

    if(*ptr) {
        DBG_LOG_APP_AIRAPP( "[APP_AIRAPP] FOTA Reconnect Byte: %x", 1, *ptr);
    }

    return *ptr;

}

void APP_AirApp_FOTA_DisableReconnect(void)
{
	U8 val = 0x00;
	U16 ErrorCode;

    ErrorCode = NVKEY_WriteFullKey(NVKEYID_FOTA_FOTA_PARA_RECONNECT, &val, 1);

	DBG_LOG_APP_AIRAPP( "[APP_AIRAPP] FOTA Disable Recon Error code:%x", 1, ErrorCode);

}

void APP_AirApp_RegisterProfile(void)
{
	PM_SetProfileRegisterState(TRUE, PM_REGISTER_AIR_APP);
    AirApp_Init((Handler)&gAppAirAppHandler, AIRAPP_INIT_SPP_BLE);
    APP_Ble_RegisterAdvHandle(&gAppAirAppNode);
}

BOOL APP_AirApp_DisconnectReq(BD_ADDR_T* bdAddr)
{
    if(bdAddr && !memcmp(&gApp_AirAppCtrl.SppAddr, bdAddr, sizeof(BD_ADDR_T)))
    {
        DBG_LOG_APP_AIRAPP( "[APP_AIRAPP] DisconnectReq SPP", 0);

		gApp_AirAppCtrl.SppInfo.state = PROFILE_DISCONNECTING;
        AirApp_Spp_Disconn(bdAddr);

        return FALSE;

    }else{

        /*ble temp code*/
        DBG_LOG_APP_AIRAPP( "[APP_AIRAPP] DisconnectReq BLE. isBleConnected:%d", 1, gApp_AirAppCtrl.isBleConnected);

        if(gApp_AirAppCtrl.isBleConnected)
        {
            BleDisconnecReq((Handler)&gAppAirAppHandler, gApp_AirAppCtrl.BleAddr.addr, BT_DISCONNECT_REASON_REMOTE_USER_TERMINATED_CONNECTION);
			return FALSE;
        }
		else
		{
			return TRUE;
		}
    }
}

BOOL APP_AirApp_IsBLEConnected(void)
{
	return gApp_AirAppCtrl.isBleConnected;
}

static void APP_AirApp_HandleInitCfm(AIRAPP_INIT_CFM_T* cfm)
{
    if(cfm->status == AIRAPP_SUCCESS)
    {

        DBG_LOG_APP_AIRAPP( "[APP_AIRAPP] HandleInitCfm", 0);

        PM_RegisterProfile(&gApp_AirAppProfileNode);

    }

	PM_SetProfileRegisterState(FALSE, PM_REGISTER_AIR_APP);
}

static void APP_AirApp_HandleConnectInd(AIRAPP_CONNECT_IND_T* ind)
{
    DBG_LOG_APP_AIRAPP( "[APP_AIRAPP] HandleConnectInd bearer %d", 1, ind->bearer);

    PROFILE_INFO *pProfileInfo;

    if(ind->bearer == AIRAPP_SPP)
    {
        gApp_AirAppCtrl.isSppConnected =TRUE;

        memcpy(&gApp_AirAppCtrl.SppAddr,&ind->bdAddr,sizeof(BD_ADDR_T));

        pProfileInfo = &gApp_AirAppCtrl.SppInfo;

        PM_LoadProfile(&ind->bdAddr, pProfileInfo, &gApp_AirAppProfileDesc);

        PM_ActivateProfile(&ind->bdAddr,pProfileInfo);

    }else{

        gApp_AirAppCtrl.isBleConnected = TRUE;

        memcpy(&gApp_AirAppCtrl.BleAddr,&ind->bdAddr,sizeof(BD_ADDR_T));

        pProfileInfo = &gApp_AirAppCtrl.BleInfo;

        APP_Pairing_CancelTimer(TIMER_PAIRING_MODE_TIMEOUT);
		APP_Pairing_CancelTimer(TIMER_PAIRING_MODE_PDL_EMPTY_TIMEOUT);

    }

    if(PM_IsRejectConnectInd(&ind->bdAddr))
    {
        APP_AirApp_DisconnectReq(&ind->bdAddr);
    }

}


static void APP_AirApp_HandleDisconnectInd(AIRAPP_DISCONNECT_IND_T* ind)
{
    DBG_LOG_APP_AIRAPP( "[APP_AIRAPP] HandleDisconnectInd bearer %d", 1, ind->bearer);

    PROFILE_INFO * pProfileInfo;
    BD_ADDR_T *pBdAddr;

    if(ind->bearer == AIRAPP_SPP)
    {
        gApp_AirAppCtrl.isSppConnected =FALSE;

        memset(&gApp_AirAppCtrl.SppAddr,0,sizeof(BD_ADDR_T));

        pProfileInfo = &gApp_AirAppCtrl.SppInfo;

        PM_UnloadProfile(&ind->bdAddr, pProfileInfo, HCI_STATUS_SUCCESS);

    }else{

        gApp_AirAppCtrl.isBleConnected = FALSE;

        memset(&gApp_AirAppCtrl.BleAddr,0,sizeof(BD_ADDR_T));

        pProfileInfo = &gApp_AirAppCtrl.BleInfo;

        APP_Ble_EnableAdv(BLE_ADV_PRIMARY_MASK);

        APP_Pairing_SetPairModeTimer();
    }

    pBdAddr = APP_GetServiceBdAddr();
    if(pBdAddr)
        PM_Sniff_Enable(pBdAddr, PM_SNIFF_OTA);
	
	if(!AirApp_IsFOTASuccess())
		APP_ReleaseDspSuspend();


    memset(pProfileInfo,0,sizeof(PROFILE_INFO));

}


static void APP_AirApp_HandleDisconnectCfm(AIRAPP_DISCONNECT_CFM_T* cfm)
{
    DBG_LOG_APP_AIRAPP( "[APP_AIRAPP] HandleDisconnectCfm bearer %d", 1, cfm->bearer);
    BD_ADDR_T *pBdAddr;

    if(cfm->bearer == AIRAPP_SPP)
    {
        gApp_AirAppCtrl.isSppConnected =FALSE;

        memset(&gApp_AirAppCtrl.SppAddr,0,sizeof(BD_ADDR_T));

        PM_UnloadProfile(&cfm->bdAddr, &gApp_AirAppCtrl.SppInfo, HCI_STATUS_SUCCESS);

        memset(&gApp_AirAppCtrl.SppInfo,0,sizeof(PROFILE_INFO));
    }

    pBdAddr = APP_GetServiceBdAddr();
    if(pBdAddr)
        PM_Sniff_Enable(pBdAddr, PM_SNIFF_OTA);
    
	if(!AirApp_IsFOTASuccess())
		APP_ReleaseDspSuspend();
}

<<<<<<< HEAD
void cb_set_bt_discoverable(U8 action)
{
  if (action) {
    APP_State_AddTopState(APP_EOF, APP_CONDISCABLE);
  } else {
    APP_State_RmvState(APP_EOF, APP_CONDISCABLE);
  }
}

U8 cb_is_bt_discoverable()
{
  U8 state, retval;

  state = APP_State_GetTopState(APP_EOF);
  if (state == APP_CONDISCABLE) {
    retval = TRUE;
  } else {
    retval = FALSE;
  }

  return retval;
}

U8 cb_get_bt_connected_cnt()
{
  return CURRENT_ACTIVE_LINK_CNT;
}

U8 cb_get_bt_maximum_link()
{
  return MAX_MULTI_POINT_NO;
}

#ifdef PROFILE_GFP_ENABLE
static void APP_AirApp_EnableAdvForGFPSv2()
{
  app_cbfunc_module_t module;

  module.set_bt_discoverable = cb_set_bt_discoverable;
  module.is_bt_discoverable = cb_is_bt_discoverable;
  module.get_bt_connected_cnt = cb_get_bt_connected_cnt;
  module.get_bt_maximum_link = cb_get_bt_maximum_link;

  GFPSv2_Enable(TRUE, &module);
}
#endif
=======
>>>>>>> db20e11 (second commit)

static void APP_AirApp_HandleBleDisconnectRsp(BT_BLE_DISCONNECT_REQ_RSP_T* rsp)
{
    DBG_LOG_APP_AIRAPP( "[APP_AIRAPP] HandleBleDisconnectRsp status %d", 1, rsp->status);

    gApp_AirAppCtrl.isBleConnected = FALSE;

    memset(&gApp_AirAppCtrl.BleAddr, 0, sizeof(BD_ADDR_T));

    memset(&gApp_AirAppCtrl.BleInfo, 0, sizeof(PROFILE_INFO));
}



static U32 APP_AirAppHandler(Handler handler, U16 id, void *msg, U32 handler_id)
{
    UNUSED(handler);
	UNUSED(handler_id);

	switch(id)
	{
        case AIRAPP_INIT_CFM:
            APP_AirApp_HandleInitCfm((AIRAPP_INIT_CFM_T*)msg);
            break;

        case AIRAPP_CONNECT_IND:
            APP_AirApp_HandleConnectInd((AIRAPP_CONNECT_IND_T*)msg);
            break;

        case AIRAPP_DISCONNECT_IND:
            APP_AirApp_HandleDisconnectInd((AIRAPP_DISCONNECT_IND_T*) msg);
            break;

        case AIRAPP_DISCONNECT_CFM:
            APP_AirApp_HandleDisconnectCfm((AIRAPP_DISCONNECT_CFM_T*)msg);
            break;

        /* BLE control */
        case BT_BLE_SET_ADV_PARAMS_EX_CFM:
        {
            DBG_LOG_APP_AIRAPP("[APP_AIRAPP] BT_BLE_SET_ADV_PARAMS_EX_CFM status: %d", 1, ((BT_BLE_SET_ADV_PARAMS_EX_CFM_T *)msg)->status);
            if (BT_STATUS_SUCCESS == ((BT_BLE_SET_ADV_PARAMS_EX_CFM_T *)msg)->status)
            {
                APP_Ble_EnableAdv(BLE_ADV_PRIMARY_MASK);
<<<<<<< HEAD
#ifdef PROFILE_GFP_ENABLE
                if (GFPSv2_IsEnable())
                {
                    APP_AirApp_EnableAdvForGFPSv2();
                }
#endif
=======
>>>>>>> db20e11 (second commit)
            }
        }
        break;

        case BT_BLE_DISCONNECT_REQ_RSP:
            APP_AirApp_HandleBleDisconnectRsp((BT_BLE_DISCONNECT_REQ_RSP_T*)msg);
            break;

        default:

            break;

	}

    return 0;

}




