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
<<<<<<< HEAD
#ifdef PROFILE_SWIFT_ENABLE
=======
#ifdef AIR_SWIFT_ENABLE
>>>>>>> db20e11 (second commit)
#include "bt_gatt.h"
#include "bt_gap.h"
#include "App_ResourceManager.h"
#include "App_Swift.h"
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
#include "AudioDSP_StreamManager.h"
#include "BtMCSync.h"
#include "App_RhoHandler.h"
#include "nvkey_hdl.h"
#include "App_MCSync_RHO.h"
#include "App_Customer_Func.h"


log_create_module(APP_SWIFT, PRINT_LEVEL_INFO);

static U32 APP_Swift_Handler(Handler handler, U16 id, void *msg, U32 handler_id);

const HandlerData gAppSwiftHandler = {APP_Swift_Handler};

#define     SWIFT_ADV_DISABLE       0x01
static U8 bySwiftCheck = 0;

typedef struct SWIFTAPP_RHO_DATA_T
{
    U8 AdvData[31];
    U8 AdvSize;
}SWIFTAPP_RHO_DATA_T;

static void APP_Swift_HandleAdvDataCfm(BT_BLE_SET_ADV_DATA_CFM_T* cfm)
{
    if( cfm->status == BT_STATUS_SUCCESS )
    {
        BLE_EXT_RANDOM_ADDR_T extAddr;
        uint8_t *addr = extAddr.RandomAddr.addr;

        DBG_LOG_APP_SWIFT( "[APP_SWIFT] APP_Swift_HandleAdvDataCfm\n", 0 );

        addr[0] = get_random_num();
        addr[1] = get_random_num();
        addr[2] = get_random_num();
        addr[3] = get_random_num();
        addr[4] = get_random_num();
        addr[5] = (get_random_num() & 0x7F) | 0x40;

        extAddr.randomAddrType = BLE_ADDR_WRITE_STATIC;
        BleSetExtendedLocalRandomAddrReq((Handler)&gAppSwiftHandler, BLE_ADV_SWIFT, &extAddr);
    }
    else
    {
        DBG_LOG_APP_SWIFT( "[APP_SWIFT]\033[0;31m Swift_HandleAdvDataCfm fail!! 0x%X\n\033[m", 1, cfm->status );
    }

}
static void APP_Swift_HandleAdvParamCfm(BT_BLE_SET_ADV_PARAMS_CFM_T* cfm)
{
    if( cfm->status == BT_STATUS_SUCCESS )
    {
        BLE_EXT_ADV_DATA_T extAD;
        U8 *pAddr;
        U8 *pDeviceName;
        U8 lengthName;
        U8 pAdvData[31] = { 23, //Len
                            0xFF, 0x06, 0x00, //Company <0x0006> Micorsoft
                            0x03, 0x01,  //Type: Swift Pair Beacon
                            0x80, // RSSI
                            0x33,0x33,0x33, 0xFF, 0xFF, 0xFF,  //BD Addr
                            0x18, 0x04, 0x20, // Cod[3] http://bluetooth-pentest.narod.ru/software/bluetooth_class_of_device-service_generator.html
                            'A', 'B', '1','5','6','2','-','-' // Display Name
                          }; 

        DBG_LOG_APP_SWIFT( "[APP_SWIFT] APP_Swift_HandleAdvParamCfm\n", 0 );
        pAddr = (U8*)NVKEY_GetPayloadFlashAddress(NVKEYID_BT_CON_BR_EDR_BDADDR);
        memcpy( &pAdvData[7], pAddr, 6);
        lengthName =  NVKEY_GetPayloadLength(NVKEYID_APP_DEVICE_NAME_DEFAULT);

        if ( lengthName ) 
        {
            pDeviceName = (U8*)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_DEVICE_NAME_DEFAULT);
            memcpy( &pAdvData[16], pDeviceName, lengthName);

            if ( lengthName > (31-15) )
                DBG_LOG_APP_SWIFT( "[APP_SWIFT]\033[0;31m Name too long 0x%X\n\033[m", 1, lengthName );
        }

        pAdvData[0] = 15 + lengthName;
        extAD.advData = pAdvData;
        extAD.sizeAdvData =  pAdvData[0]+1; 

        BleSetExtendedAdvDataReq((Handler)&gAppSwiftHandler, BLE_ADV_SWIFT, &extAD);
        return;
    }
    else
    {
        DBG_LOG_APP_SWIFT( "[APP_SWIFT]\033[0;31m HandleAdvParamCfm fail!! 0x%X\n\033[m", 1, cfm->status );
    }
}

static void App_Swift_AdvSetPara()
{
    BLE_EXT_ADV_PARAM_T extAdvParams;
    DBG_LOG_APP_SWIFT( "[APP_SWIFT] App_Swift_AdvSetPara\n", 0);

    // Set adv param
    extAdvParams.advType = BLE_ADV_IND;
    extAdvParams.ownAddrType = RANDOM_DEVICE_ADDR;
    extAdvParams.channelMap = 0x07;

    extAdvParams.AdvParms.undirectedAdvParams.advIntervalMin = 0x0040;
    extAdvParams.AdvParms.undirectedAdvParams.advIntervalMax = 0x0040;

    BleSetExtendedAdvParamsReq((Handler)&gAppSwiftHandler, BLE_ADV_SWIFT, &extAdvParams);
}

static void App_Swift_HandleSetLocalRandomAddr( BT_BLE_SET_LOCAL_RANDOM_ADDR_CFM_T* cfm )
{
    BLE_EXT_SCAN_RSP_DATA_T extSP;
    U8 rspbuf[7] = { 6, BLE_AD_TYPE_COMPLETE_LOCAL_NAME, 0x00, '-', 'M', 'S', 'P' };
    
    if(cfm->status == BT_STATUS_SUCCESS)
    {
        extSP.sizeRspData = 7;
        extSP.scanRspData = rspbuf;
        BleSetExtendedScanResponseDataReq((Handler)&gAppSwiftHandler, BLE_ADV_SWIFT, &extSP);
    }
}

static void App_Swift_AdvEnable()
{
    DBG_LOG_APP_SWIFT( "[APP_SWIFT] BleSetAdvEnableReq\n", 0);
    BleSetExtendedAdvEnableReq((Handler)&gAppSwiftHandler, BLE_ADV_SWIFT, TRUE);
}

static void APP_Swift_HandleSetAdvEnableCfm(BT_BLE_SET_ADV_ENABLE_CFM_T* cfm)
{
    if(cfm->status == BT_STATUS_SUCCESS)
    {
        DBG_LOG_APP_SWIFT( "[APP_SWIFT] APP_Swift_HandleSetAdvEnableCfm \n", 0);
        if ( bySwiftCheck & SWIFT_ADV_DISABLE )
        {
            bySwiftCheck &= ~SWIFT_ADV_DISABLE;
            App_Swift_AdvSetPara();
        }
    }else
        DBG_LOG_APP_SWIFT( "[APP_SWIFT]\033[0;31m APP_Swift_HandleSetAdvEnableCfm fail\n\033[m", 0);
}


static void APP_Swift_HandleScanRespCfm(BT_BLE_SET_SCAN_RESPONSE_DATA_CFM_T* cfm)
{
    if(cfm->status == BT_STATUS_SUCCESS)
    {
        DBG_LOG_APP_SWIFT( "[APP_SWIFT] APP_Swift_HandleScanRespCfm\n", 0);
        BleSetExtendedAdvEnableReq((Handler)&gAppSwiftHandler, BLE_ADV_SWIFT, TRUE);
    }
}

static void App_Swift_AdvDsiable()
{
    bySwiftCheck |= SWIFT_ADV_DISABLE;
    BleSetExtendedAdvEnableReq((Handler)&gAppSwiftHandler, BLE_ADV_SWIFT, FALSE );  
}


void App_SwiftAdv_Update()
{
    uint8_t* pConfig = (uint8_t*)NVKEY_GetPayloadFlashAddress(NVKEYID_PROFILE_MCSYNC_CONFIG);

    if ( pConfig[0] == ROLE_PARTNER ) return;

    DBG_LOG_APP_SWIFT( "[APP_SWIFT] Ver.0.01\n", 0);
    App_Swift_AdvDsiable();
}

static U32 APP_Swift_Handler(Handler handler, U16 id, void *msg, U32 handler_id)
{
    UNUSED(handler);
	UNUSED(handler_id);

	switch(id)
	{
        /* BLE control */
        case BT_GATT_START_SERVER_CFM:
            PRINTA( "\033[0;33m[APP_SWIFT]BT_GATT_START_SERVER_CFM \n\033[m" );
            break;
            
        case BT_BLE_SET_LOCAL_RANDOM_ADDR_CFM:
            PRINTA( "\033[0;33m[APP_SWIFT]BT_BLE_SET_LOCAL_RANDOM_ADDR_CFM\n\033[m" );
            App_Swift_HandleSetLocalRandomAddr((BT_BLE_SET_LOCAL_RANDOM_ADDR_CFM_T*)msg );
            break;

        case BT_BLE_SET_ADV_PARAMS_CFM:
            PRINTA( "\033[0;33m[APP_SWIFT]BT_BLE_SET_ADV_PARAMS_CFM\n\033[m" );
            APP_Swift_HandleAdvParamCfm((BT_BLE_SET_ADV_PARAMS_CFM_T*)msg);
            break;

        case BT_BLE_SET_ADV_DATA_CFM:
            PRINTA( "\033[0;33m[APP_SWIFT]BT_BLE_SET_ADV_DATA_CFM\n\033[m" );
            APP_Swift_HandleAdvDataCfm((BT_BLE_SET_ADV_DATA_CFM_T*)msg);
            break;

        case BT_BLE_SET_ADV_ENABLE_CFM: //BleSetExtendedAdvEnableReq
            PRINTA( "\033[0;33m[APP_SWIFT]BT_BLE_SET_ADV_ENABLE_CFM\n\033[m" );
            APP_Swift_HandleSetAdvEnableCfm((BT_BLE_SET_ADV_ENABLE_CFM_T*)msg);
            break;

        case BT_BLE_SET_SCAN_RESPONSE_DATA_CFM: //0x677
            PRINTA( "\033[0;33m[APP_SWIFT]BT_BLE_SET_SCAN_RESPONSE_DATA_CFM\n\033[m" );
            APP_Swift_HandleScanRespCfm((BT_BLE_SET_SCAN_RESPONSE_DATA_CFM_T*)msg);
            break;


        case BT_GATT_EXCHANGE_MTU_IND:
        case BT_BLE_DISCONNECT_REQ_RSP:
        default:
            PRINTB( "\033[0;33m[APP_SWIFT]Others ud=0x%X\n\033[m" , id);
            break;

	}
    return 0;
}

#endif

