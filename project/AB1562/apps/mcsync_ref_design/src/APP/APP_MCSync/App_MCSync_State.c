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
#include "App_ResourceManager.h"
#include "App_ReConnection.h"
#include "PM_Interface.h"
#include "App_State.h"
#include "App_EventOpcode.h"
#include "App_MCSync.h"
#include "App_MCSync_State.h"
#include "App_Interface.h"
#include "App_MCSync_System.h"
#include "App_MCSync_NvkeyHandler.h"
#include "log.h"
#include "BtMCSync.h"
#include "App_System.h"
#include "App_LinkKey.h"
#include "App_Mcsync_Reconnection.h"
#include "App_ReConnection.h"
#ifdef PROFILE_AMA_ENABLE
#include "App_Ama.h"
#endif

#include "APP_AirApp.h"
#include "App_RhoHandler.h"
#include "App_Ble.h"
#include "hal_captouch.h"

<<<<<<< HEAD
#ifdef PROFILE_GFP_ENABLE
=======
#ifdef AIR_GFP_ENABLE
>>>>>>> db20e11 (second commit)
#include "App_Gfp.h"
#endif


/**************************************************************************************************
* Definition
**************************************************************************************************/
#define APP_MCSYNC_EXIT_INQUIRY_TIME	2000 //ms, buffer time for the other side to find us

#define MASK_AG_DETACHED		0x01
#define MASK_ROLE_MODE_LEAVED	0x02
#define MASK_READY				(MASK_AG_DETACHED | MASK_ROLE_MODE_LEAVED)

/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct
{
	U8 procedureMask;
	U8 isNeedReconn;
}APP_MCS_AIR_PAIRING_STRU;

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static void app_mcsync_InquiryDataInit(void);
static void app_mcsync_PairingStart(void);
static void app_mcsync_CheckAirPairingReady(void);
static void app_mcsync_SetAirPairingMask(U8 mask);
static BOOL app_mcsync_IsAirPairingMaskReady(void);
static void app_mcsync_PairingEntry(BD_ADDR_T *pBdAddr);
static void app_mcsync_PairingHandler(BD_ADDR_T *pBdAddr, APP_MSG_TYPE_STRU *pMsg);
static void app_mcsync_PairingExit(BD_ADDR_T *pBdAddr);
static U32 app_mcsync_StateTimerHandler(Handler handler, U16 id, void *payload, U32 id_ext);

/**************************************************************************************************
* Variable
**************************************************************************************************/
APP_STATE_HANDLER_SET_TYPE_STRU const gAppStateMCSyncHandler =
{
	app_mcsync_PairingEntry, app_mcsync_PairingHandler, app_mcsync_PairingExit, //APP_AWS_PAIRING
};

HandlerData gAppMCSyncStateTimerHandle = { app_mcsync_StateTimerHandler };
extern HandlerData gAppMCSyncSystemHandle;

APP_MCS_AIR_PAIRING_STRU gAppAirPairingCtl;

/*************************************************************************************************
* Extern Variable
***************************************************************************************************/
extern APP_MCSYNC_STRU gAppMCSyncCtl;

/**************************************************************************************************
* Static function
**************************************************************************************************/
static void app_mcsync_InquiryDataInit(void)
{
	gAppMCSyncCtl.inquiryPara.rssi = -128;
	gAppMCSyncCtl.inquiryPara.isFoundRssi = 0;
	gAppAirPairingCtl.procedureMask = 0;
	gAppAirPairingCtl.isNeedReconn = TRUE;
}

static void app_mcsync_PairingStart(void)
{
	DBG_LOG_APP_McsyncRho( "[MCSync] Air Pairing Start", 0);

	APP_MCSYNC_SystemAirPairingStart();

	if(APP_MCSync_Nvkey_GetAirPairingTime())
		FW_SetTimer(&gAppMCSyncStateTimerHandle, APP_MCSYNC_STATE_TIMEOUT, NULL, 0, APP_MCSync_Nvkey_GetAirPairingTime());
}

static void app_mcsync_CheckAirPairingReady(void)
{
	if(app_mcsync_IsAirPairingMaskReady())
	{
		BtAwsMce_DisconnectAwsLink();
	}
}

static void app_mcsync_SetAirPairingMask(U8 mask)
{
	gAppAirPairingCtl.procedureMask |= mask;
}

static BOOL app_mcsync_IsAirPairingMaskReady(void)
{
	if((gAppAirPairingCtl.procedureMask & MASK_AG_DETACHED) && (gAppAirPairingCtl.procedureMask & MASK_ROLE_MODE_LEAVED))
		return TRUE;
	else
		return FALSE;
}

static void app_mcsync_PairingEntry(BD_ADDR_T *pBdAddr)
{
<<<<<<< HEAD
=======
    DBG_LOG_APP_McsyncRho( "[MCSync] MCSync Pairing Enter", 0);

>>>>>>> db20e11 (second commit)
    UNUSED(pBdAddr);
    #ifdef PROFILE_AMA_ENABLE
    APP_Ble_DisableAdv(BLE_ADV_AMA_MASK);
    #endif
    #ifdef XIAOAI_DEVELOPMENT
    APP_Ble_DisableAdv(BLE_ADV_XIAOAI_MASK);
    #endif
    APP_Ble_DisableAdv(BLE_ADV_PRIMARY_MASK);
<<<<<<< HEAD
	
	#ifdef PROFILE_GFP_ENABLE
	APP_Gfp_DisableAdv();
=======
	#ifdef AIR_GFP_ENABLE
	APP_Ble_DisableAdv(BLE_ADV_GFP_MASK);
>>>>>>> db20e11 (second commit)
	#endif

	APP_Ble_Disconnect(NULL);

    BtWriteScanEnable(&gAppMCSyncSystemHandle, (SCAN_ENABLE_T)SCAN_ENABLE_OFF);
    BtAwsMce_SetAirPairing(TRUE);
    APP_ReConn_ClearCtl();
    app_mcsync_InquiryDataInit();
    APP_Media_PushMediaEvent(MEDIA_EVT_TWS_ENTER_PAIRING); // add vp rt

<<<<<<< HEAD
	if(BtAwsMce_IsDefaultRoleAgent() && PM_GetNormalLinkNum())
=======
	if(BtAwsMce_IsDefaultRoleAgent() && APP_GetNormalLinkNum())
>>>>>>> db20e11 (second commit)
	{
		DBG_LOG_APP_McsyncRho( "[MCSync] Air Pairing. Release AG", 0);
		APP_Conn_ReleaseMultiLink();
	}
	else
	{
		DBG_LOG_APP_McsyncRho( "[MCSync] Air Pairing. No AG", 0);
		app_mcsync_SetAirPairingMask(MASK_AG_DETACHED);
	}

	if(!APP_System_RoleModeLeave(FALSE, TRUE))
	{
		DBG_LOG_APP_McsyncRho( "[MCSync] Air Pairing. No role mode", 0);
		app_mcsync_SetAirPairingMask(MASK_ROLE_MODE_LEAVED);
	}

	app_mcsync_CheckAirPairingReady();
}

static void app_mcsync_PairingHandler(BD_ADDR_T *pBdAddr, APP_MSG_TYPE_STRU *pMsg)
{
	gAppCtl.msgHandled = PROCD_RELEASE;

	switch(pMsg->eventCode)
	{
		case APP_LINK_DETACHED_EVT:
			if(BtAwsMce_IsDefaultRoleAgent() && !BtAwsMce_IsSpecialLink(pBdAddr))
			{
				DBG_LOG_APP_McsyncRho( "[MCSync] Air Pairing. AG detached", 0);
				app_mcsync_SetAirPairingMask(MASK_AG_DETACHED);
				app_mcsync_CheckAirPairingReady();
			}
			break;

		default:
			gAppCtl.msgHandled = NOTPROCD;
			break;
	}
}

static void app_mcsync_PairingExit(BD_ADDR_T *pBdAddr)
{
	U8 defaultRole;

	UNUSED(pBdAddr);

	DBG_LOG_APP_McsyncRho( "[MCSync] MCSync Pairing Exit State, pairing result:%d", 1, gAppMCSyncCtl.inquiryPara.isFoundRssi);

<<<<<<< HEAD
=======
#ifdef AIR_GFP_ENABLE
    if (!gAppMCSyncCtl.inquiryPara.isFoundRssi)
    {
        APP_Ble_EnableAdv(BLE_ADV_GFP_MASK);
    }
#endif
>>>>>>> db20e11 (second commit)
	BtAwsMce_SetAirPairing(FALSE);
	APP_MCSYNC_SystemStopAirPairingInquiry();

	if(gAppMCSyncCtl.inquiryPara.isFoundRssi)
	{
		APP_AWSMCE_DecideRole((U8 *)APP_AWSMCE_GetBestRSSIAddr());
		gAppMCSyncCtl.inquiryPara.isFoundRssi = 0;
		#ifdef HAL_CAPTOUCH_MODULE_ENABLED
		hal_captouch_disable_sw_tune();
		#endif
	}
	else
	{
		BtMCSync_SetDefaultRoleByLocalBdAddr();
	}

	defaultRole = BtAwsMce_GetDefaultRole();
	//APP_LinkKey_ClearAllHistory(); //BTA-3686

	if(gAppAirPairingCtl.isNeedReconn && defaultRole != ROLE_NONE)
	{
		APP_Mcsync_SetReconnStartRole(defaultRole);
		APP_ReConn_Reconnect(RECONNECT_AWSMCE);

		if(defaultRole == ROLE_AGENT)
		{
<<<<<<< HEAD
			#ifdef MCSYNC_SHARE_MODE
=======
			#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
			FW_SetTimer(&gAppMCSyncStateTimerHandle, 
				APP_MCSYNC_STATE_ENTER_DISCOVERABLE, NULL, 0, 3000);			
			#endif
			APP_ReConn_PutQueue(RECONNECT_PWR_ON);
			APP_State_ConnectabilityHandle(CMD_SET_CONNECTABLE);
		}
	}

	APP_Media_PushMediaEvent(MEDIA_EVT_TWS_EXIT_PAIRING);
	FW_ReleaseTimer(&gAppMCSyncStateTimerHandle, APP_MCSYNC_STATE_TIMEOUT, 0);
}

/**************************************************************************************************
* Static (Timer Handler)
**************************************************************************************************/
static U32 app_mcsync_StateTimerHandler(Handler handler, U16 id, void *payload, U32 id_ext)
{
	UNUSED(payload);
	UNUSED(handler);
	UNUSED(id_ext);

	switch(id)
	{
<<<<<<< HEAD
		#ifdef MCSYNC_SHARE_MODE
=======
		#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
		case APP_MCSYNC_STATE_ENTER_DISCOVERABLE:
			break;
		#endif
		case APP_MCSYNC_STATE_TIMEOUT:
			APP_MCSYNC_StateStopPairing();
			break;

		case APP_MCSYNC_STATE_SET_DEFAULT_DEVICE_BDA:
			BtMCSync_SetDefaultDeviceBdAddr();
			break;

		default:
			FW_Assert(FALSE);
			break;
	}
	return 0;
}

/**************************************************************************************************
* Public function
**************************************************************************************************/
void APP_MCSYNC_StateStopPairing(void)
{
	if(APP_State_CheckNestStateByLink(APP_EOF, APP_AWS_PAIRING))
	{
		APP_State_RmvState(APP_EOF, APP_AWS_PAIRING);
	}
}

void APP_MCSYNC_StateStartPairing(void)
{
	APP_State_AddTopState(APP_EOF, APP_AWS_PAIRING);
}

void APP_MCSYNC_StateKeyExitPairing(void)
{
	APP_MCSYNC_StateStopPairing();
}

void APP_MCSYNC_StateGetAirPairingInquiryResultWithRSSI(S8 rssi, BD_ADDR_T *pBdAddr)
{
	DBG_LOG_APP_McsyncRho( "[MCSync] Inquiry Result BdAddr:%x %x, is found RSSI:%d ", 3, FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE), gAppMCSyncCtl.inquiryPara.isFoundRssi);

	DBG_LOG_APP_McsyncRho( "[MCSync] Inquiry Result Found RSSI Value:%d, Exist RSSI Value:%d, RSSI threshold:%d", 3, rssi, gAppMCSyncCtl.inquiryPara.rssi, APP_MCSync_Nvkey_RSSIThreshold());

	if(gAppMCSyncCtl.inquiryPara.isFoundRssi == 0)
	{
		#if !FPGA
		#if !BRINGUP_1562	
		if(rssi > APP_MCSync_Nvkey_RSSIThreshold())
		#endif
		#endif
		{
			APP_AWSMCE_SetRSSIInfo(rssi, pBdAddr);
			FW_ReleaseTimer(&gAppMCSyncStateTimerHandle, APP_MCSYNC_STATE_TIMEOUT, 0);
			FW_SetTimer(&gAppMCSyncStateTimerHandle, APP_MCSYNC_STATE_TIMEOUT, NULL, 0, APP_MCSYNC_EXIT_INQUIRY_TIME); //2s define time
		}
	}
}

void APP_MCSYNC_StateSetAirPairingCfmHandler(U8 status)
{
	DBG_LOG_APP_McsyncRho( "[MCSync] Air Pairing, cfm:%d", 1, status);

	if(status == HCI_STATUS_SUCCESS)
	{
		app_mcsync_PairingStart();
	}
	else
	{
		FW_SetTimer(&gAppMCSyncStateTimerHandle, APP_MCSYNC_STATE_SET_DEFAULT_DEVICE_BDA, NULL, 0, 500);
	}
}

void APP_MCSYNC_StateRoleModeLeaveCfmHandler(void)
{
	if(APP_State_CheckNestStateByLink(APP_EOF, APP_AWS_PAIRING))
	{
		DBG_LOG_APP_McsyncRho( "[MCSync] Air Pairing. Role mode leaved", 0);
		app_mcsync_SetAirPairingMask(MASK_ROLE_MODE_LEAVED);
		app_mcsync_CheckAirPairingReady();
	}
}

void APP_MCSYNC_SetAirPairingExitReconnect(BOOL isNeedReconn)
{
	gAppAirPairingCtl.isNeedReconn = isNeedReconn;
}

