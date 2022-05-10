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

<<<<<<< HEAD
#include "bt.h"

#if defined(PROFILE_GFP_ENABLE)
=======

#include "bt.h"
#if defined(AIR_GFP_ENABLE)
#include "gfp_features.h"
>>>>>>> db20e11 (second commit)
#include "BtMCSync.h"
#include "Gfp_Bt.h"
#include "App_Gfp.h"
#include "gfps_api.h"
#include "App_Media.h"
#include "PM_Interface.h"
<<<<<<< HEAD
//#include "Gfp_LinkManager.h"
#include "PM_ProfileId.h"
#include "App_MCSync_RHO.h"

extern U8 usSPPStatus;
extern U8 usBleStatus;

#define F_GFPS_ADV_ENABLE_REQ      0x08


#define F_SPP_GFP_CONNECTED         0x01
#define F_SPP_GFP_START_SER         0x02
#define F_SPP_GFP_DISCONNECTED      0x04
=======
#include "PM_ProfileId.h"
#ifdef GFP_MCSYNC_ENABLE
#include "App_MCSync_RHO.h"
#endif
#include "App_Ble.h"
#include "log.h"
#include "App_ResourceManager.h"
#ifdef GFP_EXTENSION_BATTERY_NOTIFICATION_ENABLE
#include "App_Battery.h"
#endif
#include "nvkey.h"
#include "App_State.h"
#include "App_Interface.h"


log_create_module(APP_GFP, PRINT_LEVEL_INFO);
>>>>>>> db20e11 (second commit)


#define F_GFP_LEFT_EAR          0x01
#define F_GFP_RIGHT_EAR         0x02

<<<<<<< HEAD
#define GFPS_TID_RESERVE        11
#define MIN_PACKET_LEN  5

static BOOL bNextPacket = FALSE;
static U8 tempLen = 0;
static U8 storeBuf[10]={0};

#define MAX_MULTI_POINT_NO      1

#if 1
#define DBG_LOG_PROFILE_GFP(_message, arg_cnt, ...) LOG_MSGID_I(GFP, _message, arg_cnt, ##__VA_ARGS__)
#else
#define DBG_LOG_PROFILE_GFP(_message, arg_cnt, ...) printf( _message, ##__VA_ARGS__)
#endif
=======
#define APP_GFP_TIMER_REPEAT_RING_INTERVAL    (2000)

#define MIN_PACKET_LEN  5


typedef enum
{
    APP_GFP_TIMER_ID_REPEAT_RING,

    APP_GFP_TIMER_ID_REPEAT_MAX = 0xFF
}app_gfp_timer_id_enum;

>>>>>>> db20e11 (second commit)

/**************************************************************************************************
* Prototype
**************************************************************************************************/
<<<<<<< HEAD
static U32 APP_GfpHandler(Handler handler, U16 id, void *msg, U32 handler_id);


/**************************************************************************************************
* Extern function  
**************************************************************************************************/
extern void LeGFPSv2_Init(void);
extern uint8_t* reg_get_model_id();
extern uint8_t* reg_get_ble_random_bdaddr();
extern void reg_timer_start(uint8_t id, uint32_t ms, void (*cb)(void *parm));
extern void reg_timer_stop(uint8_t id);


extern U8 BatteryLevel[3];

extern const PROFILE_DESC gGfpProfileDesc;

/**************************************************************************************************
* Static Variables
**************************************************************************************************/
typedef struct{
    unsigned char addr[6];
}gfp_bdaddr_t;


#if 0
typedef struct
{
    SINK sink;
    PROFILE_INFO profileInfo;
    gfp_bdaddr_t  bdAddr;
    U8 state;
    U8 channel;
} gfp_spp_link_info_t;
#endif
    
=======
static U32 APP_Gfp_SppHandler(Handler handler, U16 id, void *msg, U32 handler_id);
static BOOL APP_Gfp_GattServerReadyHandler(void);
static BOOL APP_Gfp_BleAdvEnableHandler(void);
static BOOL APP_Gfp_BleAdvDisableHandler(void);
static U32 APP_GfpTimerHandler(Handler handler, U16 id, void *msg, U32 handler_id);


/**************************************************************************************************
* Extern function
**************************************************************************************************/
static void APP_Gfp_SetBtDiscoverable(BOOL discoverable);
static U8 APP_Gfp_IsBtDiscoverable(void);
static U8 APP_Gfp_GetBTAGNum(void);


/**************************************************************************************************
* Static Variables
**************************************************************************************************/
>>>>>>> db20e11 (second commit)
typedef struct
{
    GFP_LINK_STRU linkInfo[MAX_MULTI_POINT_NO];
    U8 rfcommSinkId[MAX_MULTI_POINT_NO];
<<<<<<< HEAD
}gfp_spp_rho_t;  //IAP2_RHO_SPP_STRU_T

=======
}gfp_spp_rho_t;
>>>>>>> db20e11 (second commit)


typedef struct APP_GFP_CTRL_T
{
    BD_ADDR_T SppAddr;
    BOOL isSppConnected;

    gfp_spp_cb_t *callbackTable;
    GFP_LINK_STRU linkInfo[2];
    U32 sdpServiceHandle;
    U8 state;
<<<<<<< HEAD


}APP_GFP_CTRL_T;

=======
}APP_GFP_CTRL_T;


>>>>>>> db20e11 (second commit)
static APP_GFP_CTRL_T gGfpSppCtl =
{
    .SppAddr = {{0}},
    .isSppConnected = 0,
};

<<<<<<< HEAD
//extern GFP_CTL_STRU gGfpCtl;

static const HandlerData gAppGfpHandler = {APP_GfpHandler};

static BOOL gIsGfpAdvEnable;
=======
static BOOL bNextPacket = FALSE;
static U8 tempLen = 0;
static U8 storeBuf[10] = {0};
static const HandlerData gAppGfpSppHandler = {APP_Gfp_SppHandler};
static const HandlerData gAppGfpTimerHandler = {APP_GfpTimerHandler};

static const APP_BLE_ADV_HANDLE_DESC gAppGfpDesc = {
    BLE_ADV_GFP_MASK,
    APP_Gfp_GattServerReadyHandler,
    APP_Gfp_BleAdvEnableHandler,
    APP_Gfp_BleAdvDisableHandler,
};

static APP_BLE_ADV_HANDLE_NODE gAppGfpNode = {
    &gAppGfpDesc,
    NULL,
};

>>>>>>> db20e11 (second commit)

/**************************************************************************************************
* Static Functions
**************************************************************************************************/
static void APP_Gfp_DeviceInfo(GFP_MESSAGE_STREAM_STRU *payload)
{

    switch (payload->header.code)
    {
        case GFP_CODE_CAPABILITIES:
<<<<<<< HEAD
            DBG_LOG_PROFILE_GFP("[APP_Gfp] GFP_CODE_PLATFORM_TYPE:%x", 1, payload->data.deviceInfo.capabilities );
            break;

        case GFP_CODE_PLATFORM_TYPE:
            DBG_LOG_PROFILE_GFP("[APP_Gfp] GFP_CODE_CAPABILITIES:%x %x", 2, payload->data.deviceInfo.platformType[0],payload->data.deviceInfo.platformType[0]);
=======
            APP_GFP_LOG_MSGID_I("GFP_CODE_PLATFORM_TYPE:%x", 1, payload->data.deviceInfo.capabilities );
            break;

        case GFP_CODE_PLATFORM_TYPE:
            APP_GFP_LOG_MSGID_I("GFP_CODE_CAPABILITIES:%x %x", 2, payload->data.deviceInfo.platformType[0],payload->data.deviceInfo.platformType[0]);
>>>>>>> db20e11 (second commit)
            break;

        case GFP_CODE_ACTIVE_COMPONENTS_REQ:
        case GFP_CODE_ACTIVE_COMPONENTS_RESP:
            break;
    }
<<<<<<< HEAD

}

static void tcb_action_hdl(void *parm)
{
    UNUSED(parm);
    APP_Media_PushMediaEvent( MEDIA_EVT_RINGTONE2_IN_CALL );
    reg_timer_start( GFPS_TID_RESERVE, 2000, tcb_action_hdl );
}

=======
}


static void tcb_action_hdl(void *parm)
{
    UNUSED(parm);
    APP_Media_PushMediaEvent( MEDIA_EVT_RINGTONE_IN_CALL );

    FW_SetTimer((Handler)&gAppGfpTimerHandler, APP_GFP_TIMER_ID_REPEAT_RING, NULL, 0, APP_GFP_TIMER_REPEAT_RING_INTERVAL);
}


>>>>>>> db20e11 (second commit)
void APP_Gfp_ActionHandle(U8 act )
{
    if ( act & F_GFP_LEFT_EAR )
    {
        APP_Media_PushMediaEvent( MEDIA_EVT_RINGTONE_IN_CALL );
<<<<<<< HEAD
        reg_timer_start( GFPS_TID_RESERVE, 2000, tcb_action_hdl );
    }
    else if ( act & F_GFP_RIGHT_EAR )
    {
        APP_Media_PushMediaEvent( MEDIA_EVT_RINGTONE2_IN_CALL );
        reg_timer_start( GFPS_TID_RESERVE, 2000, tcb_action_hdl );
    }
    else
    {
        reg_timer_stop( GFPS_TID_RESERVE );
=======

        FW_SetTimer((Handler)&gAppGfpTimerHandler, APP_GFP_TIMER_ID_REPEAT_RING, NULL, 0, APP_GFP_TIMER_REPEAT_RING_INTERVAL);
    }
    else if ( act & F_GFP_RIGHT_EAR )
    {
        APP_Media_PushMediaEvent( MEDIA_EVT_RINGTONE_IN_CALL );

        FW_SetTimer((Handler)&gAppGfpTimerHandler, APP_GFP_TIMER_ID_REPEAT_RING, NULL, 0, APP_GFP_TIMER_REPEAT_RING_INTERVAL);
    }
    else
    {
        FW_ReleaseTimer((Handler)&gAppGfpTimerHandler, APP_GFP_TIMER_ID_REPEAT_RING, 0);
>>>>>>> db20e11 (second commit)
    }

    APP_Gfp_SendMessageStream( GFP_GROUP_ACKNOWLEDGEMENT, GFP_CODE_ACK );
}


static void APP_Gfp_MessageStreamHandler(GFP_MESSAGE_STREAM_STRU *payload)
{
<<<<<<< HEAD
    DBG_LOG_PROFILE_GFP("[APP_Gfp] Rx MessageStream Group:%x Code:%x Length:%x", 3, payload->header.group, payload->header.code, ENDIAN_TRANSFORM_U16(payload->header.length));
    switch(payload->header.group)
    {
        case GFP_GROUP_DEVICE_INFORMATION_EVENT:
            DBG_LOG_PROFILE_GFP("[APP_Gfp] Rx DeviceInformation Data:%x", 1, payload->data.deviceInfo.platformType[0]);
=======
    APP_GFP_LOG_MSGID_I("Rx MessageStream Group:%x Code:%x Length:%x", 3, payload->header.group, payload->header.code, ENDIAN_TRANSFORM_U16(payload->header.length));
    switch(payload->header.group)
    {
        case GFP_GROUP_DEVICE_INFORMATION_EVENT:
            APP_GFP_LOG_MSGID_I("Rx DeviceInformation Data:%x", 1, payload->data.deviceInfo.platformType[0]);
>>>>>>> db20e11 (second commit)
            APP_Gfp_DeviceInfo( payload );
            break;

        case GFP_GROUP_DEVICE_ACTION_EVENT:
<<<<<<< HEAD
            DBG_LOG_PROFILE_GFP("[APP_Gfp] Rx DeviceAction 0x%X", 1, payload->data.deviceInfo.activeComponentsResp );
=======
            APP_GFP_LOG_MSGID_I("Rx DeviceAction 0x%X", 1, payload->data.deviceInfo.activeComponentsResp );
>>>>>>> db20e11 (second commit)
            APP_Gfp_ActionHandle( payload->data.deviceInfo.activeComponentsResp );
            break;

        case GFP_GROUP_ACKNOWLEDGEMENT:
<<<<<<< HEAD
            DBG_LOG_PROFILE_GFP("[APP_Gfp] Rx Acknowledgement Data:%x %x", 2, payload->data.acknowledgement[0], payload->data.acknowledgement[1]);
=======
            APP_GFP_LOG_MSGID_I("Rx Acknowledgement Data:%x %x", 2, payload->data.acknowledgement[0], payload->data.acknowledgement[1]);
>>>>>>> db20e11 (second commit)
            break;
    }
}

<<<<<<< HEAD
static void APP_Gfp_SetMessageStream(U8 group, U8 code, GFP_MESSAGE_STREAM_STRU *gfpMessage)
{
    DBG_LOG_PROFILE_GFP("[APP_Gfp] APP_Gfp_SetMessageStream Group:%x Code:%x", 2, group, code);
=======

static void APP_Gfp_SetMessageStream(U8 group, U8 code, GFP_MESSAGE_STREAM_STRU *gfpMessage)
{
    APP_GFP_LOG_MSGID_I("APP_Gfp_SetMessageStream Group:%x Code:%x", 2, group, code);
>>>>>>> db20e11 (second commit)
    gfpMessage->header.group = group;
    gfpMessage->header.code = code;
    switch(group)
    {
        case GFP_GROUP_BLUETOOTH_EVENT:
            gfpMessage->header.length = ENDIAN_TRANSFORM_U16(0);
<<<<<<< HEAD
            if ( code == 0x01 ) //
            {
                DBG_LOG_PROFILE_GFP("[APP_Gfp] SetBluetoothEvent ,Enable Silent", 0);
            }
            if ( code == 0x02 ) //
            {
                DBG_LOG_PROFILE_GFP("[APP_Gfp] SetBluetoothEvent ,Disable Silent", 0);
            }
            
            break;
        case GFP_GROUP_COMPANION_APP_EVENT:
            gfpMessage->header.length = ENDIAN_TRANSFORM_U16(0);
            DBG_LOG_PROFILE_GFP("[APP_Gfp] SetCompanionAppEvent", 0);
            break;
        case GFP_GROUP_DEVICE_INFORMATION_EVENT:
            DBG_LOG_PROFILE_GFP("[APP_Gfp] SetDeviceInfo", 0);
            if(code == GFP_CODE_MODEL_ID)
            {
                gfpMessage->header.length = ENDIAN_TRANSFORM_U16(3);

                memcpy( &gfpMessage->data.deviceInfo.modelID[0], reg_get_model_id() , 3);

                DBG_LOG_PROFILE_GFP("[APP_Gfp] ModelID:%x %x %x", 3, gfpMessage->data.deviceInfo.modelID[0], gfpMessage->data.deviceInfo.modelID[1], gfpMessage->data.deviceInfo.modelID[2]);
            }
            if(code == GFP_CODE_BLE_ADDRESS_UPDATED)
            {
                gfpMessage->header.length = ENDIAN_TRANSFORM_U16(6);
                memcpy( gfpMessage->data.deviceInfo.bleAddr, reg_get_ble_random_bdaddr() , 6 );
            }
=======
            if ( code == 0x01 )
            {
                APP_GFP_LOG_MSGID_I("SetBluetoothEvent ,Enable Silent", 0);
            }
            if ( code == 0x02 )
            {
                APP_GFP_LOG_MSGID_I("[APP_Gfp] SetBluetoothEvent ,Disable Silent", 0);
            }

            break;
        case GFP_GROUP_COMPANION_APP_EVENT:
            gfpMessage->header.length = ENDIAN_TRANSFORM_U16(0);
            APP_GFP_LOG_MSGID_I("SetCompanionAppEvent", 0);
            break;
        case GFP_GROUP_DEVICE_INFORMATION_EVENT:
            APP_GFP_LOG_MSGID_I("SetDeviceInfo", 0);
            if(code == GFP_CODE_MODEL_ID)
            {
                gfpMessage->header.length = ENDIAN_TRANSFORM_U16(3);
                if (gfp_get_model_id())
                {
                    memcpy((void *)&gfpMessage->data.deviceInfo.modelID[0], (void *)gfp_get_model_id() , 3);
                }
                else
                {
                    memset((void *)&gfpMessage->data.deviceInfo.modelID[0], 0, 3);
                }

                APP_GFP_LOG_MSGID_I("ModelID:%x %x %x", 3, gfpMessage->data.deviceInfo.modelID[0], gfpMessage->data.deviceInfo.modelID[1], gfpMessage->data.deviceInfo.modelID[2]);
            }
            if(code == GFP_CODE_BLE_ADDRESS_UPDATED)
            {
                U8 *reverse_rpa = gfp_ble_rpa_get_reverse_rpa();

                gfpMessage->header.length = ENDIAN_TRANSFORM_U16(6);
                if (reverse_rpa)
                {
                    memcpy(gfpMessage->data.deviceInfo.bleAddr, reverse_rpa, 6);
                }
                else
                {
                    memset(gfpMessage->data.deviceInfo.bleAddr, 0, 6);
                }
            }
#ifdef GFP_EXTENSION_BATTERY_NOTIFICATION_ENABLE
>>>>>>> db20e11 (second commit)
            if(code == GFP_CODE_BATTERY_UPDATED)
            {
                gfpMessage->header.length = ENDIAN_TRANSFORM_U16(3);

<<<<<<< HEAD
                gfpMessage->data.deviceInfo.battery[0] = BatteryLevel[0];
                gfpMessage->data.deviceInfo.battery[1] = BatteryLevel[1];
                gfpMessage->data.deviceInfo.battery[2] = BatteryLevel[2];
                
                DBG_LOG_PROFILE_GFP("[APP_Gfp] Battery:%x %x %x", 3, gfpMessage->data.deviceInfo.battery[0], gfpMessage->data.deviceInfo.battery[1], gfpMessage->data.deviceInfo.battery[2]);
=======
                gfpMessage->data.deviceInfo.battery[0] = gfp_adv_data_battery_value_get(GFP_ADV_DATA_BATTERY_COMPONENT_TYPE_LEFT_BUD);
                gfpMessage->data.deviceInfo.battery[1] = gfp_adv_data_battery_value_get(GFP_ADV_DATA_BATTERY_COMPONENT_TYPE_RIGHT_BUD);
                gfpMessage->data.deviceInfo.battery[2] = gfp_adv_data_battery_value_get(GFP_ADV_DATA_BATTERY_COMPONENT_TYPE_CASE);

                APP_GFP_LOG_MSGID_I("Battery:%x %x %x", 3, gfpMessage->data.deviceInfo.battery[0], gfpMessage->data.deviceInfo.battery[1], gfpMessage->data.deviceInfo.battery[2]);
>>>>>>> db20e11 (second commit)
            }
            if(code == GFP_CODE_REMAINING_BATTERY_TIME)
            {
                gfpMessage->header.length = ENDIAN_TRANSFORM_U16(1);
                gfpMessage->data.deviceInfo.remainingTime = 0x1E;
<<<<<<< HEAD
                DBG_LOG_PROFILE_GFP("[APP_Gfp] Remaining Time", 1, gfpMessage->data.deviceInfo.remainingTime);
            }
=======
                APP_GFP_LOG_MSGID_I("Remaining Time", 1, gfpMessage->data.deviceInfo.remainingTime);
            }
#endif
>>>>>>> db20e11 (second commit)
            if(code == GFP_CODE_ACTIVE_COMPONENTS_RESP)
            {
                gfpMessage->header.length = ENDIAN_TRANSFORM_U16(1);
                gfpMessage->data.deviceInfo.activeComponentsResp = 0x03;
<<<<<<< HEAD
                DBG_LOG_PROFILE_GFP("[APP_Gfp] Active Components Resp:%x", 1, gfpMessage->data.deviceInfo.activeComponentsResp);
=======
                APP_GFP_LOG_MSGID_I("Active Components Resp:%x", 1, gfpMessage->data.deviceInfo.activeComponentsResp);
>>>>>>> db20e11 (second commit)
            }
            if(code == GFP_CODE_CAPABILITIES)
            {
                gfpMessage->header.length = ENDIAN_TRANSFORM_U16(1);
<<<<<<< HEAD
                DBG_LOG_PROFILE_GFP("[APP_Gfp] Capabilities:%x", 1, gfpMessage->data.deviceInfo.capabilities);
=======
                APP_GFP_LOG_MSGID_I("Capabilities:%x", 1, gfpMessage->data.deviceInfo.capabilities);
>>>>>>> db20e11 (second commit)
            }
            if(code == GFP_CODE_PLATFORM_TYPE)
            {
                gfpMessage->header.length = ENDIAN_TRANSFORM_U16(2);
<<<<<<< HEAD
                DBG_LOG_PROFILE_GFP("[APP_Gfp] PlayformType:%x", 3, gfpMessage->data.deviceInfo.platformType[0],gfpMessage->data.deviceInfo.platformType[1]);
            }
            break;
            
=======
                APP_GFP_LOG_MSGID_I("PlayformType:%x", 3, gfpMessage->data.deviceInfo.platformType[0],gfpMessage->data.deviceInfo.platformType[1]);
            }
            break;

>>>>>>> db20e11 (second commit)
        case GFP_GROUP_DEVICE_ACTION_EVENT:
            gfpMessage->header.length = ENDIAN_TRANSFORM_U16(2);
            gfpMessage->data.deviceRing.ringing[0] = 0x03;
            gfpMessage->data.deviceRing.ringing[1] = 0x3C;
            if ( code == GFP_CODE_RING )
            {
<<<<<<< HEAD
                DBG_LOG_PROFILE_GFP("[APP_Gfp] DeviceAction , Code =%x", 1 , gfpMessage->header.code );
            }
            break;
            
=======
                APP_GFP_LOG_MSGID_I("DeviceAction , Code =%x", 1 , gfpMessage->header.code );
            }
            break;

>>>>>>> db20e11 (second commit)
        case GFP_GROUP_ACKNOWLEDGEMENT:
        {
            GFP_ACK_T ack;

            gfpMessage->header.length = ENDIAN_TRANSFORM_U16(2);
<<<<<<< HEAD
            DBG_LOG_PROFILE_GFP("[APP_Gfp] DeviceAction , Code =%x", 1 , gfpMessage->header.code );
=======
            APP_GFP_LOG_MSGID_I("DeviceAction , Code =%x", 1 , gfpMessage->header.code );
>>>>>>> db20e11 (second commit)

            ack.action = GFP_GROUP_DEVICE_ACTION_EVENT;
            ack.groupID = group;
            ack.codeID = GFP_CODE_ACK;
<<<<<<< HEAD
            
=======

>>>>>>> db20e11 (second commit)
            if ( gfpMessage->header.code == GFP_CODE_ACK )
            {
                gfpMessage->data.acknowledgement[0] = ack.action;
                gfpMessage->data.acknowledgement[1] = ack.codeID;
            }
            else if ( gfpMessage->header.code == GFP_CODE_NAK )
            {
                gfpMessage->header.length = ENDIAN_TRANSFORM_U16(3);
                ack.reason = GFP_ACKNOLEGEMENT_NAK_REASON_NOT_SUPPORTED;
                gfpMessage->data.acknowledgement[0] = ack.reason;
                gfpMessage->data.acknowledgement[1] = ack.groupID;
                gfpMessage->data.acknowledgement[2] = ack.codeID;
            }

<<<<<<< HEAD
            DBG_LOG_PROFILE_GFP("[APP_Gfp] Acknowledgement:%x %x %x", 3, gfpMessage->data.acknowledgement[0], gfpMessage->data.acknowledgement[1],gfpMessage->data.acknowledgement[2]);
=======
            APP_GFP_LOG_MSGID_I("Acknowledgement:%x %x %x", 3, gfpMessage->data.acknowledgement[0], gfpMessage->data.acknowledgement[1],gfpMessage->data.acknowledgement[2]);
>>>>>>> db20e11 (second commit)
        }
        break;
    }
}

<<<<<<< HEAD
static void APP_Gfp_RxDataHandler(GFP_RX_DATA_IND_T * ind)
{

=======

static void APP_Gfp_RxDataHandler(GFP_RX_DATA_IND_T * ind)
{
>>>>>>> db20e11 (second commit)
    if ( ( ind->length < MIN_PACKET_LEN ) && ( tempLen < MIN_PACKET_LEN ))
    {
        if ( bNextPacket )
        {
            memcpy( &storeBuf[tempLen], &ind->data[0], ind->length );
<<<<<<< HEAD
            DBG_LOG_PROFILE_GFP("[APP_Gfp] Rx2 Len:%d", 1, ind->length );
=======
            APP_GFP_LOG_MSGID_I("Rx2 Len:%d", 1, ind->length );
>>>>>>> db20e11 (second commit)
            APP_Gfp_MessageStreamHandler((GFP_MESSAGE_STREAM_STRU *)&storeBuf[0] );
            tempLen = 0;
            bNextPacket = FALSE;
            return;
        }
        else
        {
            bNextPacket = TRUE;
            tempLen = ind->length;
<<<<<<< HEAD
            DBG_LOG_PROFILE_GFP("[APP_Gfp] Rx1 Len:%d", 1, ind->length );
=======
            APP_GFP_LOG_MSGID_I("Rx1 Len:%d", 1, ind->length );
>>>>>>> db20e11 (second commit)
            memcpy( &storeBuf[0], &ind->data[0], tempLen);
            return;
        }
    }

<<<<<<< HEAD
    
    if(ind->length >= sizeof(GFP_MESSAGE_STREAM_HEADER_T))
    {
        tempLen = 0;
        bNextPacket = 0;
        DBG_LOG_PROFILE_GFP("[APP_Gfp] Rx Data:%x %x %x %x", 4, ind->data[0], ind->data[1], ind->data[2], ind->data[3]);
=======
    if(ind->length >= sizeof(GFP_MESSAGE_STREAM_HEADER_T))
    {
        tempLen = 0;
        bNextPacket = FALSE;
        APP_GFP_LOG_MSGID_I("Rx Data:%x %x %x %x", 4, ind->data[0], ind->data[1], ind->data[2], ind->data[3]);
>>>>>>> db20e11 (second commit)
        APP_Gfp_MessageStreamHandler((GFP_MESSAGE_STREAM_STRU *)&ind->data);
    }
}

<<<<<<< HEAD
static void APP_Gfp_TxAvailableIndHandler(GFP_TX_AVAILABLE_IND_T * ind)
{
    UNUSED(ind);
//	DBG_LOG_PROFILE_GFP("[APP_Gfp] Tx Available Ind: BdAddr:0x%x%x", 2, FW_bdaddr_to_2U32(&ind->bdAddr, TRUE), FW_bdaddr_to_2U32(&ind->bdAddr, FALSE));
}

static void APP_Gfp_StartServiceCfmHandler(GFP_START_SERVICE_CFM_T * cfm)
{
    DBG_LOG_PROFILE_GFP("[APP_Gfp] Start Service Cfm: %d", 1, cfm->status);
}

static void APP_Gfp_StopServiceCfmHandler(GFP_STOP_SERVICE_CFM_T * cfm)
{
    DBG_LOG_PROFILE_GFP("[APP_Gfp] Stop Service Cfm: %d", 1, cfm->status);
}


void APP_Gfp_InformationUpdate(uint8_t gfp_code )
{
    if ( !gGfpSppCtl.isSppConnected )  return;

    APP_Gfp_SendMessageStream(GFP_GROUP_DEVICE_INFORMATION_EVENT, gfp_code);
}

static void APP_Gfp_ConnectCfmHandler(GFP_CONNECT_CFM_T * cfm)
{
    DBG_LOG_PROFILE_GFP("[APP_Gfp] Connect Cfm: BdAddr:0x%x%x, status: %d", 3, FW_bdaddr_to_2U32(&cfm->bdAddr, TRUE), FW_bdaddr_to_2U32(&cfm->bdAddr, FALSE), cfm->status);

    gGfpSppCtl.isSppConnected = TRUE;
    memcpy(&gGfpSppCtl.SppAddr, &cfm->bdAddr, sizeof(BD_ADDR_T));
 
    // For Retroactively Writing Account Key
    APP_Gfp_SendMessageStream(GFP_GROUP_DEVICE_INFORMATION_EVENT, GFP_CODE_MODEL_ID);
    APP_Gfp_SendMessageStream(GFP_GROUP_DEVICE_INFORMATION_EVENT, GFP_CODE_BLE_ADDRESS_UPDATED);
//    APP_Gfp_SendMessageStream(GFP_GROUP_DEVICE_INFORMATION_EVENT, GFP_CODE_CAPABILITIES);

    APP_Gfp_SendMessageStream(GFP_GROUP_DEVICE_INFORMATION_EVENT, GFP_CODE_BATTERY_UPDATED);

    APP_Gfp_SendMessageStream( GFP_GROUP_DEVICE_ACTION_EVENT, GFP_CODE_RING );

    App_GfpsAdv_DataParmUpdate();
}

static void APP_Gfp_DisconnectCfmHandler(GFP_DISCONNECT_CFM_T * cfm)
{
    DBG_LOG_PROFILE_GFP("[APP_Gfp] Disconnect Cfm: BdAddr:0x%x%x, status: %d", 3, FW_bdaddr_to_2U32(&cfm->bdAddr, TRUE), FW_bdaddr_to_2U32(&cfm->bdAddr, FALSE), cfm->status);

    gGfpSppCtl.isSppConnected = FALSE;
    memset(&gGfpSppCtl.SppAddr, 0, sizeof(BD_ADDR_T));
    App_Gfps_Spp_Disconnect();
}

static U32 APP_GfpHandler(Handler handler, U16 id, void *msg, U32 handler_id)
=======

static void APP_Gfp_TxAvailableIndHandler(GFP_TX_AVAILABLE_IND_T * ind)
{
    UNUSED(ind);
//	APP_GFP_LOG_MSGID_I("[APP_Gfp] Tx Available Ind: BdAddr:0x%x%x", 2, FW_bdaddr_to_2U32(&ind->bdAddr, TRUE), FW_bdaddr_to_2U32(&ind->bdAddr, FALSE));
}


static void APP_Gfp_StartServiceCfmHandler(GFP_START_SERVICE_CFM_T * cfm)
{
    APP_GFP_LOG_MSGID_I("Start Service Cfm: %d", 1, cfm->status);
}


static void APP_Gfp_StopServiceCfmHandler(GFP_STOP_SERVICE_CFM_T * cfm)
{
    APP_GFP_LOG_MSGID_I("Stop Service Cfm: %d", 1, cfm->status);
}


static void APP_Gfp_ConnectCfmHandler(GFP_CONNECT_CFM_T * cfm)
{
    APP_GFP_LOG_MSGID_I("Connect Cfm: BdAddr:0x%x%x, status: %d", 3, FW_bdaddr_to_2U32(&cfm->bdAddr, TRUE), FW_bdaddr_to_2U32(&cfm->bdAddr, FALSE), cfm->status);

    gGfpSppCtl.isSppConnected = TRUE;
    memcpy(&gGfpSppCtl.SppAddr, &cfm->bdAddr, sizeof(BD_ADDR_T));

    // For Retroactively Writing Account Key
    APP_Gfp_SendMessageStream(GFP_GROUP_DEVICE_INFORMATION_EVENT, GFP_CODE_MODEL_ID);
    APP_Gfp_SendMessageStream(GFP_GROUP_DEVICE_INFORMATION_EVENT, GFP_CODE_BLE_ADDRESS_UPDATED);
    // APP_Gfp_SendMessageStream(GFP_GROUP_DEVICE_INFORMATION_EVENT, GFP_CODE_CAPABILITIES);

#ifdef GFP_EXTENSION_BATTERY_NOTIFICATION_ENABLE
    APP_Gfp_SendMessageStream(GFP_GROUP_DEVICE_INFORMATION_EVENT, GFP_CODE_BATTERY_UPDATED);
#endif
    APP_Gfp_SendMessageStream( GFP_GROUP_DEVICE_ACTION_EVENT, GFP_CODE_RING );

    gfp_spp_connect_handle();
    gfp_ble_adv_update(GFP_UPDATE_BLE_ADV_REASON_FORCE);
}


static void APP_Gfp_DisconnectCfmHandler(GFP_DISCONNECT_CFM_T * cfm)
{
    APP_GFP_LOG_MSGID_I("Disconnect Cfm: BdAddr:0x%x%x, status: %d", 3, FW_bdaddr_to_2U32(&cfm->bdAddr, TRUE), FW_bdaddr_to_2U32(&cfm->bdAddr, FALSE), cfm->status);

    gGfpSppCtl.isSppConnected = FALSE;
    memset(&gGfpSppCtl.SppAddr, 0, sizeof(BD_ADDR_T));
    gfp_spp_disconnect_handle();
}


static U32 APP_Gfp_SppHandler(Handler handler, U16 id, void *msg, U32 handler_id)
>>>>>>> db20e11 (second commit)
{
    UNUSED(handler);
    UNUSED(handler_id);

    switch(id)
    {
        case GFP_RX_DATA_IND:
            APP_Gfp_RxDataHandler((GFP_RX_DATA_IND_T *)msg);
            break;
        case GFP_TX_AVAILABLE_IND:
            APP_Gfp_TxAvailableIndHandler((GFP_TX_AVAILABLE_IND_T *)msg);
            break;
        case GFP_START_SERVICE_CFM:
            APP_Gfp_StartServiceCfmHandler((GFP_START_SERVICE_CFM_T *)msg);
            break;
        case GFP_STOP_SERVICE_CFM:
            APP_Gfp_StopServiceCfmHandler((GFP_STOP_SERVICE_CFM_T *)msg);
            break;
        case GFP_CONNECT_CFM:
<<<<<<< HEAD
            usSPPStatus |= F_SPP_GFP_CONNECTED;
            usSPPStatus &= ~F_SPP_GFP_DISCONNECTED;
            APP_Gfp_ConnectCfmHandler((GFP_CONNECT_CFM_T *)msg);
            break;
        case GFP_DISCONNECT_CFM:
            usSPPStatus &= ~F_SPP_GFP_CONNECTED ;
            usSPPStatus |= F_SPP_GFP_DISCONNECTED;
            usBleStatus &= ~F_GFPS_ADV_ENABLE_REQ;
            APP_Gfp_DisconnectCfmHandler((GFP_DISCONNECT_CFM_T *)msg);
			APP_MCSync_Rho_GfpSppDisconnectNotify();
			break;
        default:
            break;
    }
=======
            APP_Gfp_ConnectCfmHandler((GFP_CONNECT_CFM_T *)msg);
            break;
        case GFP_DISCONNECT_CFM:
            APP_Gfp_DisconnectCfmHandler((GFP_DISCONNECT_CFM_T *)msg);
#ifdef GFP_MCSYNC_ENABLE
            APP_MCSync_Rho_GfpSppDisconnectNotify();
#endif
            break;
        default:
            break;
    }
    return 0;
}


static U32 APP_GfpTimerHandler(Handler handler, U16 id, void *msg, U32 ext_id)
{
    UNUSED(handler);
    UNUSED(ext_id);
    UNUSED(msg);

    switch (id)
    {
        case APP_GFP_TIMER_ID_REPEAT_RING:
        {
            tcb_action_hdl(NULL);
            break;
        }

        default:
            break;
    }

>>>>>>> db20e11 (second commit)
    return 0;
}


<<<<<<< HEAD
=======
/* The return value does not matter actually. */
static BOOL APP_Gfp_GattServerReadyHandler(void)
{
    APP_GFP_LOG_MSGID_I("APP_Gfp_GattServerReadyHandler()", 0);
    /* Only agent need enable gfp ble adv. */
    gfp_ble_adv_set_gatt_server_status(TRUE);
    gfp_ble_adv_enable();
    return TRUE;
}


static BOOL APP_Gfp_BleAdvEnableHandler(void)
{
    APP_GFP_LOG_MSGID_I("APP_Gfp_BleAdvEnableHandler()", 0);
    return gfp_ble_adv_enable();
}


static BOOL APP_Gfp_BleAdvDisableHandler(void)
{
    APP_GFP_LOG_MSGID_I("APP_Gfp_BleAdvDisableHandler()", 0);
    return gfp_ble_adv_disable();
}


>>>>>>> db20e11 (second commit)
/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void APP_Gfp_RegisterProfile(void)
{
<<<<<<< HEAD
    if ( GFPSv2_IsEnable() == 0 ) 
    {
        DBG_LOG_PROFILE_GFP("[APP_Gfp] RegisterService return", 0);
        return;
    }

    Gfp_StartService((Handler)&gAppGfpHandler, GFP_CHANNEL);
    DBG_LOG_PROFILE_GFP("[APP_Gfp] RegisterService", 0);
    //BLE  service Init
    LeGFPSv2_Init();
=======
    gfp_reg_app_func_struct app_func;
    gfp_ret_enum ret = GFP_RET_FAIL;

    APP_GFP_LOG_MSGID_I("RegisterProfile", 0);

    app_func.set_bt_discoverable = APP_Gfp_SetBtDiscoverable;
    app_func.is_bt_discoverable = APP_Gfp_IsBtDiscoverable;
    app_func.get_acl_link_count = APP_GetACLLinkCount;
    app_func.get_ag_num = APP_Gfp_GetBTAGNum;
    app_func.reconn_clear_ctl = APP_ReConn_ClearCtl;
    app_func.conn_release_multilink = APP_Conn_ReleaseMultiLink;
#ifdef GFP_EXTENSION_BATTERY_NOTIFICATION_ENABLE
    app_func.get_smartcase_battery_level = APP_Battery_GetSmartCaseBatLevel;
#else
    app_func.get_smartcase_battery_level = NULL;
#endif

    ret = gfp_init(&app_func);
    if (GFP_RET_SUCCESS == ret && gfp_is_enabled())
    {
        Gfp_StartService((Handler)&gAppGfpSppHandler, GFP_CHANNEL);

        //BLE  service Init
        LeGFPSv2_Init();
        APP_Ble_RegisterAdvHandle(&gAppGfpNode);
    }
    else
    {
        APP_GFP_LOG_MSGID_I("GFP is disable. ret:%d", ret);
    }
>>>>>>> db20e11 (second commit)
}

void APP_Gfp_SendMessageStream(U8 group, U8 code)
{
    U16 length;
    U8 serviceState = Gfp_GetServiceState();
<<<<<<< HEAD
    U8 enableState = App_GfpEnableStat();

    if (( serviceState != PROFILE_SERVICE_STATE_INITIATED )
        || ( enableState != 0x01 )) // GFPS_ENABLE_ON
    {
        DBG_LOG_PROFILE_GFP( "[APP_Gfp] SendMessageStream_RET %d %d\n", 2, serviceState, enableState );
        return;
    }
        
=======

    if (( serviceState != PROFILE_SERVICE_STATE_INITIATED )
        || !gfp_is_enabled())
    {
        APP_GFP_LOG_MSGID_I( "SendMessageStream_RET %d %d\n", 2, serviceState, gfp_is_enabled() );
        return;
    }

>>>>>>> db20e11 (second commit)
    GFP_MESSAGE_STREAM_STRU *gfpMessage = (GFP_MESSAGE_STREAM_STRU *)FW_GetMemory(sizeof(GFP_MESSAGE_STREAM_STRU));

    APP_Gfp_SetMessageStream(group, code, gfpMessage);
    length  = sizeof(GFP_MESSAGE_STREAM_HEADER_T) + ENDIAN_TRANSFORM_U16(gfpMessage->header.length);
    if(Gfp_TxData(&gGfpSppCtl.SppAddr, length, (U8 *)gfpMessage))
<<<<<<< HEAD
        DBG_LOG_PROFILE_GFP("[APP_Gfp] Send MessageStream Group:%x Code:%x Length:%x SUCCESS", 3, group, code, length);
    else
        DBG_LOG_PROFILE_GFP("[APP_Gfp] Send MessageStream Group:%x Code:%x Length:%x FAIL", 3, group, code, length);

    FW_FreeMemory( gfpMessage );
    
}
BOOL APP_Gfp_EnableAdv(void)
{
    if(!BtAwsMce_IsDefaultRoleAgent() || !GFPSv2_IsEnable())
        return FALSE;

    DBG_LOG_PROFILE_GFP("[APP_Gfp] APP_Gfp_EnableAdv !!" , 0);
    App_GfpsAdv_Update(1);

    gIsGfpAdvEnable = TRUE;
    return TRUE;
}

BOOL APP_Gfp_DisableAdv(void)
{
    if( (!BtAwsMce_IsDefaultRoleAgent()) /*|| (!gIsGfpAdvEnable) */)
        return FALSE;

    DBG_LOG_PROFILE_GFP("[APP_Gfp] APP_Gfp_DisableAdv !!" , 0);
    App_GfpsAdv_Disable();

    gIsGfpAdvEnable = FALSE;
    return TRUE;
}

#endif
=======
        APP_GFP_LOG_MSGID_I("Send MessageStream Group:%x Code:%x Length:%x SUCCESS", 3, group, code, length);
    else
        APP_GFP_LOG_MSGID_I("Send MessageStream Group:%x Code:%x Length:%x FAIL", 3, group, code, length);

    FW_FreeMemory( gfpMessage );
}


void APP_Gfp_ResetToFactory(void)
{
    if (!gfp_is_enabled())
    {
        return;
    }

    gfp_acct_key_reset();
    gfp_personalized_name_clear();
    gfp_ble_adv_update(GFP_UPDATE_BLE_ADV_REASON_ACCOUNT_KEY_LIST_CHANGES);
}



/* No matter it is SAWS or AWS connected, send the req to the partner. */
void APP_Gfp_AwsConnectedHandle(void)
{
#ifdef GFP_MCSYNC_ENABLE
    if (!gfp_is_enabled())
    {
        return;
    }

    if (BtAwsMce_IsDefaultRoleAgent())
    {
        gfp_mcsync_send_merge_acct_key_list_req(TRUE);
        gfp_mcsync_send_update_personalized_name_req(TRUE);
    }
#endif
}


void APP_Gfp_RhoDoneHandle(U8 newRole, BOOL status)
{
#ifndef GFP_MCSYNC_ENABLE
    UNUSED(newRole);
    UNUSED(status);
#else
    if (!gfp_is_enabled())
    {
        return;
    }

    if (status)
    {
        if (ROLE_AGENT == newRole)
        {
            gfp_mcsync_send_merge_acct_key_list_req(FALSE);
            /* The personalized name is synced in the rho data during rho. */
        }
    }
#endif
}


void APP_Gfp_McsyncDataHandle(U16 length, U8 *p_data)
{
#ifndef GFP_MCSYNC_ENABLE
    UNUSED(length);
    UNUSED(p_data);
#else
    U8 cmd_id = p_data ? p_data[0] : GFP_MCSYNC_CMD_ID_NONE;

    if (!gfp_is_enabled() || !length || !p_data)
    {
        APP_GFP_LOG_MSGID_I("mcsync_data_handle fail. gfp_enable:%d cmd_id:%d length:%d p_data:%x", 4, gfp_is_enabled(), cmd_id, length, p_data);
        return;
    }

    APP_GFP_LOG_MSGID_I("mcsync_data_handle cmd_id:%d length:%d", 2, cmd_id, length);
    switch (cmd_id)
    {
        case GFP_MCSYNC_CMD_ID_MERGE_ACCT_KEY_LIST_REQ:
        {
            gfp_mcsync_handle_merge_acct_key_list_req(length, p_data);
            break;
        }

        case GFP_MCSYNC_CMD_ID_MERGE_ACCT_KEY_LIST_RSP:
        {
            gfp_mcsync_handle_merge_acct_key_list_rsp(length, p_data);
            break;
        }

        case GFP_MCSYNC_CMD_ID_UPDATE_PERSONALIZED_NAME_REQ:
        {
            gfp_mcsync_handle_update_personalized_name_req(length, p_data);
            break;
        }

        case GFP_MCSYNC_CMD_ID_UPDATE_PERSONALIZED_NAME_RSP:
        {
            gfp_mcsync_handle_update_personalized_name_rsp(length, p_data);
            break;
        }

        default:
            break;
    }
#endif
}


void APP_Gfp_McsyncPartnerSmartChargeCaseStateIndHandle(void)
{
#ifdef GFP_MCSYNC_ENABLE
#ifdef GFP_EXTENSION_BATTERY_NOTIFICATION_ENABLE
    U8 charging_status = gfp_get_partner_charging_status();

    if (0 == charging_status || 1 == charging_status)
    {
        /* New partner will report its charging status to new agent after RHO.
               * Set the charging status maintained by GFP to be invalid to let GFP change to use charger API
               * to get the partner charger status when start/update the BLE ADV. */
        gfp_set_partner_charging_status(0xFF);
    }
#endif
#endif
}


BOOL APP_Gfp_SppIsAllowRho(void)
{
#ifdef GFP_MCSYNC_ENABLE
    return (MSG_MessageSearchAllHandler((Handler)&gAppGfpSppHandler)) ? FALSE : TRUE;
#else
    return FALSE;
#endif
}


static void APP_Gfp_SetBtDiscoverable(BOOL discoverable)
{
  BD_ADDR_T bd;
  U8 *pAddr;

  pAddr = (U8*)NVKEY_GetPayloadFlashAddress(NVKEYID_BT_CON_BR_EDR_BDADDR);
  memcpy(bd.addr, pAddr, 6);

  if (discoverable) {
    APP_State_AddTopState(&bd, APP_CONDISCABLE);
  } else {
    APP_State_RmvState(&bd, APP_CONDISCABLE);
  }
}


static U8 APP_Gfp_IsBtDiscoverable(void)
{
  U8 state, retval;

  if (BtAwsMce_GetDefaultRole() == ROLE_AGENT) {
    BD_ADDR_T bd;
    U8 *pAddr;
    U8 is_mcsync;

    pAddr = (U8*)NVKEY_GetPayloadFlashAddress(NVKEYID_BT_CON_BR_EDR_BDADDR);
    memcpy(bd.addr, pAddr, 6);
    state = APP_State_GetTopState(&bd);

    // Check the special linke exist or not ...
    is_mcsync = BtAwsMce_IsSpecialLink(&bd);
    if (is_mcsync) {
      if (APP_State_CheckNestStateByLink(&bd, APP_CONDISCABLE)) {
        state = APP_CONDISCABLE;
      }
    }
  } else {
    state = APP_State_GetTopState(APP_EOF);
  }

  if (state == APP_CONDISCABLE) {
    retval = TRUE;
  } else {
    retval = FALSE;
  }

  return retval;
}


static U8 APP_Gfp_GetBTAGNum(void)
{
#ifdef GFP_MCSYNC_ENABLE
    return APP_GetAGNum();
#else
    return APP_GetACLLinkCount();
#endif
}

#endif /* AIR_GFP_ENABLE */

>>>>>>> db20e11 (second commit)
