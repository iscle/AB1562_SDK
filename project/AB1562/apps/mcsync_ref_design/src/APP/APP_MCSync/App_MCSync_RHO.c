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
#include "bt_avctp.h"

#include "BtMCSync.h"
#include "App_MCSync_RHO.h"
#include "App_ResourceManager.h"
#include "App_Pairing.h"
#include "App_SniffHandler.h"
#include "Avrcp_Interface.h"
#include "A2dp_Interface.h"
#include "Hfp_Interface.h"
#include "PM_Interface.h"
#include "App_RhoHandler.h"
#include "App_MCSync_Message.h"
#include "MCSync_LinkManager.h"
#include "App_PowerOff.h"
#include "App_ChargeBattery.h"
#include "App_SmartChgCase.h"
#include "AudioDSP_StreamManager.h"
#include "ChargerBatteryCaseNvkeyHandler.h"
#include "App_System.h"
#include "AirApp.h"
#include "App_Battery.h"
#include "APP_AirApp.h"
#include "App_Customer_Func.h"
#include "DeviceID_CustomerSdpHandler.h"
#include "drv_a2dp.h"
#include "App_Ble.h"
<<<<<<< HEAD
=======
#ifdef AIR_GATT_OVER_BREDR_ENABLE
#include "bt_att_rho.h"
#endif
>>>>>>> db20e11 (second commit)

#ifdef PROFILE_AMA_ENABLE
#include "BtAma.h"
#include "BtIap2.h"
#include "App_Ama.h"
#endif
#include "App_Customer.h"
<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
#include "App_MCSync_Share.h"
#endif

#ifdef XIAOAI_DEVELOPMENT
#include "bt_xiaoai.h"
#include "App_XiaoaiAdv.h"
#include "App_XiaoaiKey.h"
#include "drv_xiaoai.h"
#include "Battery.h"
#include "DrvSmartCase.h"
#endif

<<<<<<< HEAD
/*
#ifdef PROFILE_GFP_ENABLE
#include "App_Gfp.h"
#include "App_Gfp_RhoHandler.h"
#endif*/

#ifdef TAKE_OVER_LINK
=======
#ifdef AIR_GFP_ENABLE
#include "gfp_features.h"
#include "App_Gfp.h"
#ifdef GFP_MCSYNC_ENABLE
#include "App_Gfp_RhoHandler.h"
#endif
#endif

#if defined(XIAOAI_DEVELOPMENT) || defined(AIR_GFP_ENABLE)
#include "Battery.h"
#include "DrvSmartCase.h"
#endif

#ifdef AIR_TAKE_OVER_LINK_ENABLE
>>>>>>> db20e11 (second commit)
#include "App_State.h"
#endif

/**************************************************************************************************
* Definition
**************************************************************************************************/
#define INVALID_MODULE_ID_INDEX 		0xFF
#define APP_MCSYNC_RHO_ACTION_FLOW_NUM	sizeof(gAppMCSyncRhoActionFlow)


#define RHO_DELAY_TIME					15 		//ms
#define RHO_RETRY_COUNT					200
#define RHO_CONFIRM_TIMEOUT_TIME		2500 	//ms
#define RHO_END_CHGCASE_CHECK_TIME		500		//ms

#define SYSTEM_CLOCK_UNIT_US			HALF_SLOT * 4 //us

<<<<<<< HEAD
=======
#define STREAM_UNLOCK  0
#define AGENT_LOCK 1
#define PARTNER_LOCK 2
>>>>>>> db20e11 (second commit)
/**************************************************************************************************
* Enumeration
**************************************************************************************************/
enum
{
	RHO_CHECK_IDLE,
	RHO_CHECK_FAIL,
	RHO_CHECK_BUSY,
};

enum
{
	APP_MCSYNC_RHO_ACTION_NONE = 0,

	APP_MCSYNC_RHO_ACTION_SNIFF_LOCK,
	APP_MCSYNC_RHO_ACTION_CONNECTABLE_LOCK,
<<<<<<< HEAD
	APP_MCSYNC_RHO_ACTION_DISCONNECT_BLE,
	APP_MCSYNC_RHO_ACTION_DISCONNECT_SPP,
	APP_MCSYNC_RHO_ACTION_DISCONNECT_CUSTAPP,
#ifdef PROFILE_GFP_ENABLE
=======
#ifdef AIR_TILE_ADV_ENABLE	
	APP_MCSYNC_RHO_ACTION_NOTIFY_PARTNER_DISCONNECT_BLE,//add for tile
#endif
	APP_MCSYNC_RHO_ACTION_DISCONNECT_BLE,
	APP_MCSYNC_RHO_ACTION_DISCONNECT_SPP,
	APP_MCSYNC_RHO_ACTION_DISCONNECT_CUSTAPP,
#ifdef AIR_GFP_ENABLE
>>>>>>> db20e11 (second commit)
	APP_MCSYNC_RHO_ACTION_DISCONNECT_GFPSPP,
#endif
	APP_MCSYNC_RHO_ACTION_IS_ALLOW,
	APP_MCSYNC_RHO_ACTION_SEND_RHO_PREPARE,
	APP_MCSYNC_RHO_ACTION_LOCK,

	APP_MCSYNC_RHO_ACTION_GET_DATA,

	APP_MCSYNC_RHO_ACTION_SEND_RHO_PACKET,
	APP_MCSYNC_RHO_ACTION_APP_PROFILE_PROTOCOL_UNLOCK,

	APP_MCSYNC_RHO_ACTION_NUM,
};

enum
{
	APP_MCSYNC_RHO_FLOW_SUCCESS,
	APP_MCSYNC_RHO_FLOW_WAIT,
	APP_MCSYNC_RHO_FLOW_ERROR,
};

enum
{
	APP_MCSYNC_RHO_ID_PROCESS,
	APP_MCSYNC_RHO_ID_PROCESS_NEXT,
	APP_MCSYNC_RHO_ID_CONFIRM_TIMER,
};

enum
{
	APP_MCSYNC_RHO_ERRCODE_SNIFF_CFM_FAIL,
	APP_MCSYNC_RHO_ERRCODE_CONNECTABLE_CFM_FAIL,
<<<<<<< HEAD
	APP_MCSYNC_RHO_ERRCODE_BLE_DISCONNECT_CFM_FAIL,
	APP_MCSYNC_RHO_ERRCODE_SPP_DISCONNECT_CFM_FAIL,
	APP_MCSYNC_RHO_ERRCODE_CUSTAPP_DISCONNECT_CFM_FAIL,
	#ifdef PROFILE_GFP_ENABLE
=======
#ifdef AIR_TILE_ADV_ENABLE	
	APP_MCSYNC_RHO_ERRCODE_NOTIFY_PARTNER_BLE_DISCONNECT_CFM_FAIL,//add for tile
#endif
	APP_MCSYNC_RHO_ERRCODE_BLE_DISCONNECT_CFM_FAIL,
	APP_MCSYNC_RHO_ERRCODE_SPP_DISCONNECT_CFM_FAIL,
	APP_MCSYNC_RHO_ERRCODE_CUSTAPP_DISCONNECT_CFM_FAIL,
	#ifdef AIR_GFP_ENABLE
>>>>>>> db20e11 (second commit)
	APP_MCSYNC_RHO_ERRCODE_GFPSPP_DISCONNECT_CFM_FAIL,
	#endif
	APP_MCSYNC_RHO_ERRCODE_IS_ALLOW_FAIL,
	APP_MCSYNC_RHO_ERRCODE_RHO_PREPARE_CFM_FAIL,
	APP_MCSYNC_RHO_ERRCODE_LOCK_FAIL,
	APP_MCSYNC_RHO_ERRCODE_GET_DATA_FAIL,
	APP_MCSYNC_RHO_ERRCODE_ROLE_CHANGE_IND_FAIL, //9
	APP_MCSYNC_RHO_ERRCODE_APP_PROFILE_PROTOCOL_UNLOCK_FAIL,
	APP_MCSYNC_RHO_ERRCODE_SET_RHO_FINISH_FAIL,
	APP_MCSYNC_RHO_ERRCODE_CONFIRM_TIMEOUT,
	APP_MCSYNC_RHO_ERRCODE_PARTNER_DISCONNECTED,
};

/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct
{
	BD_ADDR_T agBdAddr;
	U8 curFlowIndex;

	MCSYNC_RHO_PACKET_T *pTxRhoPacket;
	U16 rhoTxPacketLen;

	MCSYNC_RHO_PACKET_T *pRxRhoPacket;
	U16 rhoRxPacketLen;

	U8 retryCnt;
	U8 rhoState;
	U8 isForceStop;

	U32 rhoStartClock;
	U32 rhoEndClock;
}APP_MCSYNC_RHO_CTL_STRU;

typedef struct
{
	U8 moduleId;
	BOOL (*funcLock)(void);
	BOOL (*funcUnlock)(void);
	BOOL (*funcIsAllow)(void);
	U16 (*funcGetLength)(void);
	BOOL (*funcGetData)(U8 *pBuffer);
	BOOL (*funcApplyAgentData)(BD_ADDR_T *pBdAddr, U8 *pData, U16 length);
	BOOL (*funcApplyPartnerData)(BD_ADDR_T *pBdAddr, U8 *pData, U16 length);
}APP_MCSYNC_RHO_MODULE_CTL_STRU;

/**************************************************************************************************
* Protorype (General)
**************************************************************************************************/
static void app_MCSync_Rho_Init(void);
static U8 app_MCSync_Rho_GetCurAction(void);
static U8 app_MCSync_Rho_ShiftAndGetNextAction(void);
static BOOL app_MCSync_Rho_SetFlowIdxByAction(U8 action);
static void app_MCSync_Rho_ClearRhoTxPacketInfo(void);
#if RHO_DEVELOPMENT
static void app_MCSync_Rho_ClearRhoRxPacketInfo(void);
#endif
static void app_MCSync_Rho_FlowErrorHandler(U8 errorCode);
static BOOL app_MCSync_Rho_IsAllow(void);
#ifdef DO_RHO_IMMEDIATELY
static BOOL app_MCSync_Rho_Lock(void);
static BOOL app_MCSync_Rho_Unlock(void);
#endif
#if RHO_DEVELOPMENT
static U8 app_MCSync_Rho_FindModuleIndex(U8 moduleId);
static BOOL app_MCSync_Rho_ApplyData(U8 newRole, MCSYNC_RHO_PACKET_T *pRhoData, U16 length);
#endif

<<<<<<< HEAD
extern void StreamL2capLockSink(BOOL isLock);
=======
extern void StreamL2capLockSink(U8 isLock);
>>>>>>> db20e11 (second commit)
extern void StreamL2capPopQueueToNewAgent(void);
extern void StreamL2capPopQueueToHci(void);

/**************************************************************************************************
* Protorype (Check)
**************************************************************************************************/
#if RHO_DEVELOPMENT
static U8 app_MCSync_Rho_StartCheck(void);
static void app_MCSync_Rho_CheckActionAfterRho(void);
#endif

/**************************************************************************************************
* Protorype (Cfm / Ind Handler)
**************************************************************************************************/
static void app_MCSync_Rho_SniffCfmHandler(U8 mode);
#if RHO_DEVELOPMENT
static void app_MCSync_Rho_NonConnectableCfmHandler(BOOL status);
#endif
static void app_MCSync_Rho_BleDisconnectCfmHandler(U8 status);
#if RHO_DEVELOPMENT
static void app_MCSync_Rho_SppDisconnectCfmHandler(void);
#endif
#ifdef AIROHA_BT_SPP_ENABLE
static void app_MCSync_Rho_CustAppDisconnectCfmHandler(void);
#endif
static void app_MCSync_Rho_PrepareCfmHandler(U16 id, BT_AWS_MCE_PREPARE_ROLE_HANDOVER_CFM_T *cfm);
#if RHO_DEVELOPMENT
static U32 app_MCSync_Rho_SetFinishHandler(Handler handler, U16 id, void *msg, U32 id_ext);
#endif
static U32 app_MCSync_Rho_FlowWaitHandler(Handler handler, U16 id, void *msg, U32 id_ext);
static U32 app_MCSync_Rho_PrintRoleHandler(Handler handler, U16 id, void *msg, U32 id_ext);
static void app_MCSync_Rho_CancelRhoCfmHandler(U16 id, BT_AWS_MCE_CANCEL_ROLE_HANDOVER_CFM_T *cfm);
static U32 app_MCSync_Rho_FlowMessageHandler(Handler handler, U16 id, void *msg, U32 id_ext);

/**************************************************************************************************
* Protorype (Flow Handler)
**************************************************************************************************/
static U8 app_MCSync_Rho_SniffLockHandler(void);
static U8 app_MCSync_Rho_ConnectableLockHandler(void);
static U8 app_MCSync_Rho_DisconnectBleHandler(void);
static U8 app_MCSync_Rho_DisconnectSppHandler(void);
static U8 app_MCSync_Rho_DisconnectCustAppHandler(void);
<<<<<<< HEAD
#ifdef PROFILE_GFP_ENABLE
=======
#ifdef AIR_GFP_ENABLE
>>>>>>> db20e11 (second commit)
static U8 app_MCSync_Rho_DisconnectGfpSppHandler(void);
#endif
#ifndef DO_RHO_IMMEDIATELY
static U8 app_MCSync_Rho_IsAllowHandler(void);
#endif
static U8 app_MCSync_Rho_SendRhoPrepareHandler(void);
static U8 app_MCSync_Rho_LockHandler(void);
static U8 app_MCSync_Rho_GetDataHandler(void);
static U8 app_MCSync_Rho_SendRhoPacketHandler(void);
static U8 app_MCSync_Rho_AppProfileProtocolUnlockHandler(void);
static void app_MCSync_Rho_ActionFlowHandler(U8 action);
static void app_MCSync_Rho_FlowProcessHandler(U16 id);

static void app_MCSync_Rho_PrintCurrentRole();
static void app_MCSync_Rho_SetAndSyncRhoState(U8 state);
static void app_MCSync_Rho_SetStartClock(void);
static void app_MCSync_Rho_SetEndClock(void);

/**************************************************************************************************
* Variable
**************************************************************************************************/
<<<<<<< HEAD
=======
extern HandlerData gAppImgCallMediaTimerHandler;
>>>>>>> db20e11 (second commit)
extern BOOL g_is_fota_ongoing;
extern APP_POWER_CTL_STRU gAppPowerCtl;
U8 gRhoMsgPrint = 0;
static APP_MCSYNC_RHO_CTL_STRU gAppMCSyncRhoCtl;
#if RHO_DEVELOPMENT
static HandlerData gAppMCSyncSetRhoFinishHandle = { app_MCSync_Rho_SetFinishHandler };
#endif
static HandlerData gAppMCSyncRhoFlowWaitHandle = { app_MCSync_Rho_FlowWaitHandler };
static HandlerData gAppMCSyncRhoPrintRoleHandle = { app_MCSync_Rho_PrintRoleHandler };
static HandlerData gAppMCSyncRhoFlowMessageHandle = { app_MCSync_Rho_FlowMessageHandler };

static const U8 gAppMCSyncRhoActionFlow[] =
{
	APP_MCSYNC_RHO_ACTION_NONE,

	//adjustable
	APP_MCSYNC_RHO_ACTION_SNIFF_LOCK,
	APP_MCSYNC_RHO_ACTION_CONNECTABLE_LOCK,
<<<<<<< HEAD
	APP_MCSYNC_RHO_ACTION_DISCONNECT_BLE,
	APP_MCSYNC_RHO_ACTION_DISCONNECT_SPP,
	APP_MCSYNC_RHO_ACTION_DISCONNECT_CUSTAPP,
#ifdef PROFILE_GFP_ENABLE
=======
#ifdef AIR_TILE_ADV_ENABLE	
	APP_MCSYNC_RHO_ACTION_NOTIFY_PARTNER_DISCONNECT_BLE,//add for tile
#endif
	APP_MCSYNC_RHO_ACTION_DISCONNECT_BLE,
	APP_MCSYNC_RHO_ACTION_DISCONNECT_SPP,
	APP_MCSYNC_RHO_ACTION_DISCONNECT_CUSTAPP,
#ifdef AIR_GFP_ENABLE
>>>>>>> db20e11 (second commit)
	APP_MCSYNC_RHO_ACTION_DISCONNECT_GFPSPP,
#endif
	//fixed
	#ifndef DO_RHO_IMMEDIATELY
	APP_MCSYNC_RHO_ACTION_IS_ALLOW,
	#endif
	APP_MCSYNC_RHO_ACTION_SEND_RHO_PREPARE,
	APP_MCSYNC_RHO_ACTION_LOCK,
	APP_MCSYNC_RHO_ACTION_GET_DATA,
	APP_MCSYNC_RHO_ACTION_SEND_RHO_PACKET,
	APP_MCSYNC_RHO_ACTION_APP_PROFILE_PROTOCOL_UNLOCK,
};

static const APP_MCSYNC_RHO_MODULE_CTL_STRU gAppMCSyncRhoModuleCtl[AWS_MCE_RHO_MODULE_NUM] =
{
	{ AWS_MCE_RHO_MODULE_GAP, BtGapRhoLock, BtGapRhoLock, BtGapIsRhoAllow, BtGapGetRhoDataLength, BtGapGetRhoData, BtGapApplyAgentRhoData, BtGapApplyPartnerRhoData },
    { AWS_MCE_RHO_MODULE_L2CAP, BtGapRhoLock, BtGapRhoLock, bt_l2cap_is_rho_allow, bt_l2cap_get_rho_data_length, bt_l2cap_get_rho_data, bt_l2cap_apply_agent_rho_data, bt_l2cap_apply_partner_rho_data },
    { AWS_MCE_RHO_MODULE_SDP, BtGapRhoLock, BtGapRhoLock, BtSdpIsRhoAllow, BtSdpGetRhoDataLength, BtSdpGetRhoData, BtSdpApplyAgentRhoData, BtSdpApplyPartnerRhoData },
    { AWS_MCE_RHO_MODULE_RFCOMM, BtGapRhoLock, BtGapRhoLock, BtRfcommIsRhoAllow, BtRfcommGetRhoDataLength, BtRfcommGetRhoData, BtRfcommApplyAgentRhoData, BtRfcommApplyPartnerRhoData },
	{ AWS_MCE_RHO_MODULE_AVDTP, BtAvdtpLock, BtAvdtpUnlock, BtAvdtpIsAllow, BtAvdtpGetRhoDataLength, BtAvdtpGetRhoData, BtAvdtpApplyAgentRhoData, BtAvdtpApplyPartnerRhoData },
	{ AWS_MCE_RHO_MODULE_AVCTP, BtAvctpLock, BtAvctpUnlock, BtAvctpIsAllow, BtAvctpGetRhoDataLength, BtAvctpGetRhoData, BtAvctpApplyAgentRhoData, BtAvctpApplyPartnerRhoData },
	{ AWS_MCE_RHO_MODULE_AWSMCE, BtAwsMceLock, BtAwsMceUnlock, BtAwsMceIsAllow, BtAwsMceGetRhoDataLength, BtAwsMceGetRhoData, BtAwsMceApplyAgentRhoData, BtAwsMceApplyPartnerRhoData },
	{ AWS_MCE_RHO_MODULE_AVRCP, BtAvrcpLock, BtAvrcpUnlock, BtAvrcpIsAllow, BtAvrcpGetRhoDataLength, BtAvrcpGetRhoData, BtAvrcpApplyAgentRhoData, BtAvrcpApplyPartnerRhoData },
	{ AWS_MCE_RHO_MODULE_A2DP, bt_a2dp_lock, bt_a2dp_unlock, bt_a2dp_is_allow, bt_a2dp_get_rho_data_length, bt_a2dp_get_rho_data, bt_a2dp_apply_agent_rho_data, bt_a2dp_apply_partner_rho_data },
	{ AWS_MCE_RHO_MODULE_HFP, BtHfpLock, BtHfpUnlock, BtHfpIsAllow, BtHfpGetRhoDataLength, BtHfpGetRhoData, BtHfpApplyAgentRhoData, BtHfpApplyPartnerRhoData },
	#ifdef PROFILE_AMA_ENABLE
	{ AWS_MCE_RHO_MODULE_AMA, BtAmaLock, BtAmaUnlock, BtAmaIsAllow, BtAmaGetRhoDataLength, BtAmaGetRhoData, BtAmaApplyAgentRhoData, BtAmaApplyPartnerRhoData },
	#endif
    #ifdef XIAOAI_DEVELOPMENT
    { AWS_MCE_RHO_MODULE_XIAOAI, xiaoai_lock, xiaoai_unlock, xiaoai_is_allow_rho, xiaoai_get_rho_data_length, xiaoai_get_rho_data, xiaoai_apply_agent_rho_data, xiaoai_apply_partner_rho_data},
    #endif
	{ AWS_MCE_RHO_MODULE_PM, PM_Lock, PM_Unlock, PM_IsAllow, PM_GetRhoDataLength, PM_GetRhoData, PM_ApplyAgentRhoData, PM_ApplyPartnerRhoData },
	{ AWS_MCE_RHO_MODULE_APP, APP_Lock, APP_Unlock, APP_IsAllow, APP_GetRhoDataLength, APP_GetRhoData, APP_ApplyAgentRhoData, APP_ApplyPartnerRhoData },
	#ifdef PROFILE_AMA_ENABLE
	{AWS_MCE_RHO_MODULE_IAP2, BtIAP2Lock, BtIAP2Unlock, BtIAP2IsAllow, BtIAP2GetRhoDataLength, BtIAP2GetRhoData, BtIAP2ApplyAgentRhoData, BtIAP2ApplyPartnerRhoData},
	#endif
<<<<<<< HEAD
//#ifdef PROFILE_GFP_ENABLE
//    { AWS_MCE_RHO_MODULE_GFP, gfp_rho_lock, gfp_rho_unlock, gfp_rho_is_allow, BtGfpGetRhoDataLength, BtGfpGetRhoData, BtGfpApplyAgentRhoData, BtGfpApplyPartnerRhoData },
//#endif
=======
#ifdef AIR_GATT_OVER_BREDR_ENABLE
	{ AWS_MCE_RHO_MODULE_ATT, BtAttLock, BtAttUnlock, BtAttIsAllow, BtAttGetRhoDataLength, BtAttGetRhoData, BtAttApplyAgentRhoData, BtAttApplyPartnerRhoData },
#endif	
#if defined(AIR_GFP_ENABLE) && defined(GFP_MCSYNC_ENABLE)
    {AWS_MCE_RHO_MODULE_GFP, APP_Gfp_RhoLock, APP_Gfp_RhoUnlock, APP_Gfp_RhoIsAllow, APP_Gfp_RhoGetDataLength, APP_Gfp_RhoGetData, APP_Gfp_RhoNewAgentProcess, APP_Gfp_RhoNewPartnerProcess},
#endif
>>>>>>> db20e11 (second commit)
};

/**************************************************************************************************
* Static Function (General)
**************************************************************************************************/
static void app_MCSync_Rho_Init(void)
{
    BD_ADDR_T agBdAddr; //For release RHO cfm 
    
	DBG_LOG_APP_McsyncRho( "[APP RHO] Init", 0);
    
    FW_Memcpy(&agBdAddr, &gAppMCSyncRhoCtl.agBdAddr, sizeof(BD_ADDR_T)); //For release RHO cfm 

	app_MCSync_Rho_PrintCurrentRole();
	app_MCSync_Rho_ClearRhoRxPacketInfo();
	FW_Memset(&gAppMCSyncRhoCtl, 0, sizeof(APP_MCSYNC_RHO_CTL_STRU));
    
    FW_Memcpy(&gAppMCSyncRhoCtl.agBdAddr, &agBdAddr, sizeof(BD_ADDR_T)); //For release RHO cfm 

	app_MCSync_Rho_CheckActionAfterRho();
}

static U8 app_MCSync_Rho_GetCurAction(void)
{
	return gAppMCSyncRhoActionFlow[gAppMCSyncRhoCtl.curFlowIndex];
}

static U8 app_MCSync_Rho_ShiftAndGetNextAction(void)
{
	if(gAppMCSyncRhoCtl.curFlowIndex >= APP_MCSYNC_RHO_ACTION_FLOW_NUM - 1)
	{
		DBG_LOG_APP_McsyncRho( "[APP RHO] GetNextAction reach end", 0);
		gAppMCSyncRhoCtl.curFlowIndex = 0;
	}
	else
	{
		gAppMCSyncRhoCtl.curFlowIndex++;
	}

	return gAppMCSyncRhoActionFlow[gAppMCSyncRhoCtl.curFlowIndex];
}

static BOOL app_MCSync_Rho_SetFlowIdxByAction(U8 action)
{
	U8 flowIndex;

	for(flowIndex = 0; flowIndex < APP_MCSYNC_RHO_ACTION_FLOW_NUM; flowIndex++)
	{
		if(gAppMCSyncRhoActionFlow[flowIndex] == action)
		{
			gAppMCSyncRhoCtl.curFlowIndex = flowIndex;
			return TRUE;
		}
	}

	return FALSE;
}

static void app_MCSync_Rho_ClearRhoTxPacketInfo(void)
{
	DBG_LOG_APP_McsyncRho( "[APP RHO] Clear RHO Tx packet info", 0);
	gAppMCSyncRhoCtl.rhoTxPacketLen = 0;

	if(gAppMCSyncRhoCtl.pTxRhoPacket)
	{
		//FW_FreeMemory(gAppMCSyncRhoCtl.pTxRhoPacket); //free in controller when calling "BtAwsMceSendRHOPacket"
		gAppMCSyncRhoCtl.pTxRhoPacket = NULL;
	}
}

#if RHO_DEVELOPMENT
static void app_MCSync_Rho_ClearRhoRxPacketInfo(void)
{
	DBG_LOG_APP_McsyncRho( "[APP RHO] Clear RHO Rx packet info", 0);
	gAppMCSyncRhoCtl.rhoRxPacketLen = 0;

	if(gAppMCSyncRhoCtl.pRxRhoPacket)
	{
		FW_FreeMemory(gAppMCSyncRhoCtl.pRxRhoPacket);
		gAppMCSyncRhoCtl.pRxRhoPacket = NULL;
	}
}
#endif

static void app_MCSync_Rho_FlowErrorHandler(U8 errorCode)
{
	DBG_LOG_APP_McsyncRho( "[APP RHO] FlowError: %d", 1, errorCode);

	switch(errorCode)
	{
		case APP_MCSYNC_RHO_ERRCODE_SNIFF_CFM_FAIL:
		case APP_MCSYNC_RHO_ERRCODE_CONNECTABLE_CFM_FAIL:
<<<<<<< HEAD
		case APP_MCSYNC_RHO_ERRCODE_BLE_DISCONNECT_CFM_FAIL:
		case APP_MCSYNC_RHO_ERRCODE_SPP_DISCONNECT_CFM_FAIL:
		case APP_MCSYNC_RHO_ERRCODE_CUSTAPP_DISCONNECT_CFM_FAIL:
		#ifdef PROFILE_GFP_ENABLE
=======
#ifdef AIR_TILE_ADV_ENABLE	
		case APP_MCSYNC_RHO_ERRCODE_NOTIFY_PARTNER_BLE_DISCONNECT_CFM_FAIL:
#endif
		case APP_MCSYNC_RHO_ERRCODE_BLE_DISCONNECT_CFM_FAIL:
		case APP_MCSYNC_RHO_ERRCODE_SPP_DISCONNECT_CFM_FAIL:
		case APP_MCSYNC_RHO_ERRCODE_CUSTAPP_DISCONNECT_CFM_FAIL:
		#ifdef AIR_GFP_ENABLE
>>>>>>> db20e11 (second commit)
        case APP_MCSYNC_RHO_ERRCODE_GFPSPP_DISCONNECT_CFM_FAIL:
		#endif
		case APP_MCSYNC_RHO_ERRCODE_IS_ALLOW_FAIL:
		case APP_MCSYNC_RHO_ERRCODE_RHO_PREPARE_CFM_FAIL:
		case APP_MCSYNC_RHO_ERRCODE_LOCK_FAIL:
		case APP_MCSYNC_RHO_ERRCODE_ROLE_CHANGE_IND_FAIL:
		case APP_MCSYNC_RHO_ERRCODE_APP_PROFILE_PROTOCOL_UNLOCK_FAIL:
		case APP_MCSYNC_RHO_ERRCODE_CONFIRM_TIMEOUT:
		case APP_MCSYNC_RHO_ERRCODE_PARTNER_DISCONNECTED:
			break;

		case APP_MCSYNC_RHO_ERRCODE_GET_DATA_FAIL:
			BtAwsMceCancelRoleHandoverReq(&gAppMCSyncRhoFlowWaitHandle, &gAppMCSyncRhoCtl.agBdAddr.addr);
			break;

		default:
			break;
	}

	app_MCSync_Rho_SetEndClock();
	//unlock sniff, connectable, ble & spp connection, APP & profile & profile
	App_SniffEnable(&gAppMCSyncRhoCtl.agBdAddr, APP_SNIFF_MCSYNC);
	APP_SmtChgCse_SetCheckStateTimer(RHO_END_CHGCASE_CHECK_TIME);
	app_MCSync_Rho_SetAndSyncRhoState(RHO_IDLE);
	APP_Ble_EnableAdv(BLE_ADV_PRIMARY_MASK);
<<<<<<< HEAD
=======
	#ifdef AIR_GFP_ENABLE
        APP_Ble_EnableAdv(BLE_ADV_GFP_MASK);
        #endif
>>>>>>> db20e11 (second commit)
	app_MCSync_Rho_Init();
    
    #ifdef XIAOAI_DEVELOPMENT
    APP_Ble_EnableAdv(BLE_ADV_XIAOAI_MASK);
    #endif
<<<<<<< HEAD
	#ifdef PROFILE_GFP_ENABLE
    APP_Gfp_EnableAdv();
=======

    #ifdef AIR_TILE_ADV_ENABLE
        APP_Tile_Rho_done(FALSE);
>>>>>>> db20e11 (second commit)
    #endif
    StreamL2capPopQueueToHci();
}

static BOOL app_MCSync_Rho_IsAllow(void)
{
	U8 moduleIndex;

	for(moduleIndex = 0; moduleIndex < AWS_MCE_RHO_MODULE_NUM; moduleIndex++)
	{
		if(gAppMCSyncRhoModuleCtl[moduleIndex].funcIsAllow && !gAppMCSyncRhoModuleCtl[moduleIndex].funcIsAllow())
		{
			DBG_LOG_APP_McsyncRho( "[APP RHO] Not allow. ID:%d", 1, gAppMCSyncRhoModuleCtl[moduleIndex].moduleId);
			return FALSE;
		}
	}

	return TRUE;
}

#ifdef DO_RHO_IMMEDIATELY
static BOOL app_MCSync_Rho_Lock(void)
{
	U8 moduleIndex;

	for(moduleIndex = 0; moduleIndex < AWS_MCE_RHO_MODULE_NUM; moduleIndex++)
	{
		if(gAppMCSyncRhoModuleCtl[moduleIndex].funcLock && !gAppMCSyncRhoModuleCtl[moduleIndex].funcLock())
		{
			DBG_LOG_APP_McsyncRho( "[APP RHO] Lock failed. ID:%d", 1, gAppMCSyncRhoModuleCtl[moduleIndex].moduleId);
			return FALSE;
		}
	}

	return TRUE;
}

static BOOL app_MCSync_Rho_Unlock(void)
{
	U8 moduleIndex;

	for(moduleIndex = 0; moduleIndex < AWS_MCE_RHO_MODULE_NUM; moduleIndex++)
	{
		if(gAppMCSyncRhoModuleCtl[moduleIndex].funcUnlock && !gAppMCSyncRhoModuleCtl[moduleIndex].funcUnlock())
		{
			DBG_LOG_APP_McsyncRho( "[APP RHO] Unlock failed. ID:%d", 1, gAppMCSyncRhoModuleCtl[moduleIndex].moduleId);
			return FALSE;
		}
	}

	return TRUE;
}
#endif

#if RHO_DEVELOPMENT
static U8 app_MCSync_Rho_FindModuleIndex(U8 moduleId)
{
	U8 moduleIndex;

	for(moduleIndex = 0; moduleIndex < AWS_MCE_RHO_MODULE_NUM; moduleIndex++)
	{
		if(gAppMCSyncRhoModuleCtl[moduleIndex].moduleId == moduleId)
		{
			return moduleIndex;
		}
	}

	return INVALID_MODULE_ID_INDEX;
}
static void app_MCSync_Rho_Done(void)
{
	BD_ADDR_T *pBdAddr;

	app_MCSync_Rho_SetAndSyncRhoState(RHO_IDLE);
	APP_SmtChgCse_SetIsNeedRHO(FALSE);
	APP_SmtChgCse_SetCheckStateTimer(RHO_END_CHGCASE_CHECK_TIME);

	if(BtAwsMce_IsDefaultRoleAgent() && (pBdAddr = APP_GetServiceBdAddr()))
	{
		DBG_LOG_APP_McsyncRho( "[APP RHO] New Agent Done", 0);
		App_SniffEnable(pBdAddr, APP_SNIFF_MCSYNC);

		AiroKey_NewAgentTimerSet();

		switch(APP_ChgCase_GetCaseType())
		{
			case CASE_TYPE_NONE:
				if(APP_ChgBat_IsFeatureSupported(APP_CHARGER_OUT_THEN_POWER_ON_FEAT) && APP_ChgBat_IsChargerIn() && (MCSYNC_GetMcsyncState() == MCSYNC_LINK_CONNECTED))
				{
					DBG_LOG_APP_McsyncRho( "[APP RHO] RHO Done, but Agent in case", 0);
					gAppPowerCtl.powerOff.isNeedToPowerOff = TRUE;
					APP_MCSync_Rho_Start();
				}
				break;
			default:
				break;
		}
<<<<<<< HEAD
		#ifdef TAKE_OVER_LINK
		APP_State_ConnectabilityHandle(CMD_SET_CONNECTABLE);
    	APP_State_DiscoverabilityHandle(CMD_SET_NON_DISCOVERABLE);
		#endif
		
=======
		#ifdef AIR_TAKE_OVER_LINK_ENABLE
		APP_State_ConnectabilityHandle(CMD_SET_CONNECTABLE);
    	APP_State_DiscoverabilityHandle(CMD_SET_NON_DISCOVERABLE);
		#endif
	    #ifdef AIR_TILE_ADV_ENABLE
                APP_Tile_Rho_done(TRUE);
            #endif
>>>>>>> db20e11 (second commit)
	}
	else
	{
		if(BtAwsMce_IsDefaultRolePartner())
		{
			DBG_LOG_APP_McsyncRho( "[APP RHO] New Partner Done", 0);
			#ifdef PROFILE_AMA_ENABLE
        	APP_Ble_DisableAdv(BLE_ADV_AMA_MASK);
        	#endif
<<<<<<< HEAD
			#ifdef XIAOAI_DEVELOPMENT
			BtMCSync_SendSyncBatteryLevel(Battery_GetLevelInPercent());
			APP_MCSYNC_SendSyncSmartChargeCaseStateInfo(DrvCharger_GetSmartCaseState());
			#endif	
			#ifdef PROFILE_GFP_ENABLE
            APP_Gfp_DisableAdv();
            #endif
			APP_Customer_Rho_Done();
=======
			#if defined(XIAOAI_DEVELOPMENT) || defined(AIR_GFP_ENABLE)
			BtMCSync_SendSyncBatteryLevel(Battery_GetLevelInPercent());
			APP_MCSYNC_SendSyncSmartChargeCaseStateInfo(DrvCharger_GetSmartCaseState());
			#endif
			#ifdef AIR_GFP_ENABLE
               APP_Ble_DisableAdv(BLE_ADV_GFP_MASK);
            #endif
			#ifdef AIR_TILE_ADV_ENABLE
        	APP_Tile_Rho_done(TRUE);
        	#endif
			APP_Customer_Rho_Done();
			FW_ReleaseTimer((Handler)&gAppImgCallMediaTimerHandler, 0, 0);
            APP_RhoDomn_StartMonitorTimer();
>>>>>>> db20e11 (second commit)
		}
	}

	if(!AirApp_IsFOTASuccess())
		APP_ReleaseDspSuspend();

    drv_a2dp_rho_done();
	gAppMCSyncRhoCtl.retryCnt = 0;

}

static BOOL app_MCSync_Rho_ApplyData(U8 newRole, MCSYNC_RHO_PACKET_T *pRhoData, U16 length)
{
	U8 *pModuleAddr, moduleIdx, *pModuleData;
	U16 moduleDataLen, hostDataLen, totalShiftLen = 0, shiftLen;

	if(newRole == ROLE_PARTNER)
	{
		DBG_LOG_APP_McsyncRho( "[APP RHO] Apply partner data", 0);
		//to partner
		//change role variables
		for(moduleIdx = 0; moduleIdx < AWS_MCE_RHO_MODULE_NUM; moduleIdx++)
		{
			if(gAppMCSyncRhoModuleCtl[moduleIdx].funcApplyPartnerData)
				gAppMCSyncRhoModuleCtl[moduleIdx].funcApplyPartnerData(NULL, NULL, 0);
		}
        StreamL2capPopQueueToNewAgent();
		app_MCSync_Rho_Done();
	}
	else if(newRole == ROLE_SLAVE_AGENT_WITH_PARTNER)
	{
		DBG_LOG_APP_McsyncRho( "[APP RHO] Apply agent data", 0);
		//to agent

		if(!length || !pRhoData)
		{
			DBG_LOG_APP_McsyncRho( "[APP RHO] Apply data failed. pData:0x%x length:%d", 2, (U32)pRhoData, length);
			return FALSE;
		}

		pModuleAddr = pRhoData->pHostData;
		hostDataLen = length - (sizeof(MCSYNC_RHO_PACKET_T) - 1);

		while(totalShiftLen < hostDataLen)
		{
			moduleIdx = app_MCSync_Rho_FindModuleIndex(((BT_AWS_MCE_RHO_MODULE_STUR *)pModuleAddr)->moduleId);
			moduleDataLen = ((BT_AWS_MCE_RHO_MODULE_STUR *)pModuleAddr)->length;
			pModuleData = ((BT_AWS_MCE_RHO_MODULE_STUR *)pModuleAddr)->pData;
			DBG_LOG_APP_McsyncRho( "[APP RHO] Apply data Module ID/Idx:%d/%d Len:%d", 3, ((BT_AWS_MCE_RHO_MODULE_STUR *)pModuleAddr)->moduleId, moduleIdx, moduleDataLen);

			if(moduleIdx != INVALID_MODULE_ID_INDEX && gAppMCSyncRhoModuleCtl[moduleIdx].funcApplyAgentData)
			{
				gAppMCSyncRhoModuleCtl[moduleIdx].funcApplyAgentData(NULL, pModuleData, moduleDataLen);
			}

			shiftLen = (sizeof(BT_AWS_MCE_RHO_MODULE_STUR) - 1 + moduleDataLen);
			totalShiftLen += shiftLen;
			pModuleAddr += shiftLen;
		}

		if(totalShiftLen != hostDataLen)
		{
			DBG_LOG_APP_McsyncRho( "[APP RHO] Apply data Warning!! %d/%d", 2, totalShiftLen, hostDataLen);
		}

		app_MCSync_Rho_Done();
	}

	return TRUE;
}
#endif

/**************************************************************************************************
* Static Function (Check)
**************************************************************************************************/
#if RHO_DEVELOPMENT
static U8 app_MCSync_Rho_StartCheck(void)
{
<<<<<<< HEAD
	BD_ADDR_T *pBdAddr = APP_GetServiceBdAddr();
=======
	BD_ADDR_T *pBdAddr = APP_GetNormalLinkAddr();
>>>>>>> db20e11 (second commit)
#ifdef XIAOAI_DEVELOPMENT
	BD_ADDR_T *pXiaoPlayingAddr = drv_xiaoai_get_playing_bda();
#endif	

<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
	U8 shareMode = BtMCSync_GetShareMode(); 
	UNUSED(shareMode);
#endif

	if(g_is_fota_ongoing || APP_AirApp_FOTA_Reconnect())
	{
		DBG_LOG_APP_McsyncRho( "[APP RHO] Start fail: FOTAing", 0);
		return RHO_CHECK_FAIL;
	}
	
	if(!pBdAddr || !BtAwsMce_IsDefaultRoleAgent() || !BtAwsMce_IsWithPartner(pBdAddr)) //Is with partner
	{
		DBG_LOG_APP_McsyncRho( "[APP RHO] IsAgent:%d, withPartner:%d", 2, BtAwsMce_IsDefaultRoleAgent(), BtAwsMce_IsWithPartner(pBdAddr));
		return RHO_CHECK_FAIL;
	}

	if(PM_GetNormalLinkNum() != 1) //With phone
	{
		DBG_LOG_APP_McsyncRho( "[APP RHO] Link error. Num:%d", 1, PM_GetNormalLinkNum());
		return RHO_CHECK_FAIL;
	}

<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
	if(APP_MCSync_Share_IsEnable() || APP_MCSync_Share_GetLinkLoss())
	{
		DBG_LOG_APP_McsyncRho("[APP RHO] share mode enabled",0 );
		return RHO_CHECK_FAIL;
	}
#endif

	#ifdef XIAOAI_DEVELOPMENT
	if(xiaoai_get_speech_state() != XIAOAI_SPEECH_STOPPED)
	{
		DBG_LOG_APP_McsyncRho( "[APP RHO] stop XIAOAI due to RHO", 0);

		if(!APP_Xiaoai_SpeechStop(pXiaoPlayingAddr))
		{
			DBG_LOG_APP_McsyncRho( "[APP RHO] stop XIAOAI Fail due to RHO", 0);
			return RHO_CHECK_FAIL;
		}
	}
	#endif
	
	if(app_MCSync_Rho_GetCurAction() != APP_MCSYNC_RHO_ACTION_NONE) //RHO idle state
	{
		DBG_LOG_APP_McsyncRho( "[APP RHO] Start fail: Processing action:%d", 1, app_MCSync_Rho_GetCurAction());
		return RHO_CHECK_BUSY;
	}

	FW_Memcpy(&gAppMCSyncRhoCtl.agBdAddr, pBdAddr, sizeof(BD_ADDR_T));
	return RHO_CHECK_IDLE;
}

static void app_MCSync_Rho_CheckActionAfterRho(void)
{
	DBG_LOG_APP_McsyncRho( "[APP RHO] Start Checking Actions Related to RHO ", 0);

	if(APP_PowerOff_IsPowerOffAfterRHO())
	{
		DBG_LOG_APP_McsyncRho( "[APP RHO] Need to Power off after RHO", 0);
		APP_PowerOff_Loader(POWEROFF_REASON_AWS_RHO_FINISHED);
	}
	else if(APP_ChgCase_GetCaseType() == CASE_TYPE_GPIOXY)
	{
		if(APP_ChgBat_IsDSleepNeededAfterRHO())
		{
			DBG_LOG_APP_McsyncRho( "[APP RHO] Need to Enter Deep sleep after RHO", 0);
			APP_ChgBat_EnterDSleepAfterRHO();
		}
		else if(APP_ChgBat_IsRHOForChgCase())
		{
			DBG_LOG_APP_McsyncRho( "[APP RHO] Need to Power off for charger case", 0);
			APP_PowerOff_Loader(POWEROFF_REASON_CHARGING_CASE);
		}
	}

	APP_Customer_CheckKeyStatusAfterRho();
	APP_ChgBat_SetRHOFinished();
}
#endif


/**************************************************************************************************
* Static Function (Cfm / Ind Handler)
**************************************************************************************************/
static void app_MCSync_Rho_SniffCfmHandler(U8 mode)
{
	if(APP_MCSYNC_RHO_ACTION_SNIFF_LOCK != app_MCSync_Rho_GetCurAction())
	{
		return;
	}

	FW_ReleaseTimer(&gAppMCSyncRhoFlowMessageHandle, APP_MCSYNC_RHO_ID_CONFIRM_TIMER, 0);

	if((mode == LINK_MODE_ACTIVE) /*&& (status == HCI_STATUS_SUCCESS)*/)
	{
		app_MCSync_Rho_FlowProcessHandler(APP_MCSYNC_RHO_ID_PROCESS_NEXT);
	}
	else
	{
		app_MCSync_Rho_FlowErrorHandler(APP_MCSYNC_RHO_ERRCODE_SNIFF_CFM_FAIL);
	}
}

#if RHO_DEVELOPMENT
static void app_MCSync_Rho_NonConnectableCfmHandler(BOOL status)
{
	if(APP_MCSYNC_RHO_ACTION_CONNECTABLE_LOCK != app_MCSync_Rho_GetCurAction())
	{
		return;
	}

	FW_ReleaseTimer(&gAppMCSyncRhoFlowMessageHandle, APP_MCSYNC_RHO_ID_CONFIRM_TIMER, 0);

	if(status)
	{
		app_MCSync_Rho_FlowProcessHandler(APP_MCSYNC_RHO_ID_PROCESS_NEXT);
	}
	else
	{
		app_MCSync_Rho_FlowErrorHandler(APP_MCSYNC_RHO_ERRCODE_CONNECTABLE_CFM_FAIL);
	}
}
#endif

<<<<<<< HEAD
=======
#ifdef AIR_TILE_ADV_ENABLE	
static void app_MCSync_Rho_NotifyPartnerBleDisconnectCfmHandler(U8 status) //add for tile
{
	/*Check the current state. */
	if(APP_MCSYNC_RHO_ACTION_NOTIFY_PARTNER_DISCONNECT_BLE != app_MCSync_Rho_GetCurAction()) {
		return;
	}
	DBG_LOG_APP_McsyncRho( "[APP RHO] app_MCSync_Rho_NotifyPartnerBleDisconnectCfmHandler status: %d", 1, status);

	FW_ReleaseTimer(&gAppMCSyncRhoFlowMessageHandle, APP_MCSYNC_RHO_ID_CONFIRM_TIMER, 0);

	if (BT_STATUS_SUCCESS == status) {
		/*Successful, Goto next action*/
		app_MCSync_Rho_FlowProcessHandler(APP_MCSYNC_RHO_ID_PROCESS_NEXT);		
	} else {
	    /*Fail, Resume the BLE ADV*/
		app_MCSync_Rho_FlowErrorHandler(APP_MCSYNC_RHO_ERRCODE_NOTIFY_PARTNER_BLE_DISCONNECT_CFM_FAIL);
	}
}
#endif
>>>>>>> db20e11 (second commit)
static void app_MCSync_Rho_BleDisconnectCfmHandler(U8 status) //not used, reserved
{
	if(APP_MCSYNC_RHO_ACTION_DISCONNECT_BLE != app_MCSync_Rho_GetCurAction())
	{
		return;
	}

	FW_ReleaseTimer(&gAppMCSyncRhoFlowMessageHandle, APP_MCSYNC_RHO_ID_CONFIRM_TIMER, 0);

	if(BT_STATUS_SUCCESS == status)
	{
		app_MCSync_Rho_FlowProcessHandler(APP_MCSYNC_RHO_ID_PROCESS_NEXT);
	}
	else
	{
		app_MCSync_Rho_FlowErrorHandler(APP_MCSYNC_RHO_ERRCODE_BLE_DISCONNECT_CFM_FAIL);
	}
}

#if RHO_DEVELOPMENT
static void app_MCSync_Rho_SppDisconnectCfmHandler(void)
{
	if(APP_MCSYNC_RHO_ACTION_DISCONNECT_SPP != app_MCSync_Rho_GetCurAction())
	{
		return;
	}

	FW_ReleaseTimer(&gAppMCSyncRhoFlowMessageHandle, APP_MCSYNC_RHO_ID_CONFIRM_TIMER, 0);

	app_MCSync_Rho_FlowProcessHandler(APP_MCSYNC_RHO_ID_PROCESS_NEXT);

	//if disconnect failed
	//app_MCSync_Rho_FlowErrorHandler(APP_MCSYNC_RHO_ERRCODE_SPP_DISCONNECT_CFM_FAIL);
}
#endif

#ifdef AIROHA_BT_SPP_ENABLE
static void app_MCSync_Rho_CustAppDisconnectCfmHandler(void)
{
	if(APP_MCSYNC_RHO_ACTION_DISCONNECT_CUSTAPP != app_MCSync_Rho_GetCurAction())
	{
		return;
	}
	FW_ReleaseTimer(&gAppMCSyncRhoFlowMessageHandle, APP_MCSYNC_RHO_ID_CONFIRM_TIMER, 0);

	app_MCSync_Rho_FlowProcessHandler(APP_MCSYNC_RHO_ID_PROCESS_NEXT);

	//if disconnect failed
	//app_MCSync_Rho_FlowErrorHandler(APP_MCSYNC_RHO_ERRCODE_CUSTAPP_DISCONNECT_CFM_FAIL);
}
#endif

<<<<<<< HEAD
#ifdef PROFILE_GFP_ENABLE
=======
#ifdef AIR_GFP_ENABLE
>>>>>>> db20e11 (second commit)
static void app_MCSync_Rho_GfpSppDisconnectCfmHandler(void)
{
	if(APP_MCSYNC_RHO_ACTION_DISCONNECT_GFPSPP != app_MCSync_Rho_GetCurAction())
	{
		return;
	}
	FW_ReleaseTimer(&gAppMCSyncRhoFlowMessageHandle, APP_MCSYNC_RHO_ID_CONFIRM_TIMER, 0);

	app_MCSync_Rho_FlowProcessHandler(APP_MCSYNC_RHO_ID_PROCESS_NEXT);

	//if disconnect failed
	//app_MCSync_Rho_FlowErrorHandler(APP_MCSYNC_RHO_ERRCODE_GFPSPP_DISCONNECT_CFM_FAIL);
}
#endif

static void app_MCSync_Rho_PrepareCfmHandler(U16 id, BT_AWS_MCE_PREPARE_ROLE_HANDOVER_CFM_T *cfm)
{
	FW_ReleaseTimer(&gAppMCSyncRhoFlowMessageHandle, APP_MCSYNC_RHO_ID_CONFIRM_TIMER, 0);

	if((id == BT_AWS_MCE_PREPARE_ROLE_HANDOVER_CFM) && (cfm->status == AWS_MCE_RHO_PREPARE_SUCCESS))
	{
		if(!gAppMCSyncRhoCtl.isForceStop)
			app_MCSync_Rho_FlowProcessHandler(APP_MCSYNC_RHO_ID_PROCESS_NEXT);
		else
			BtAwsMceCancelRoleHandoverReq(&gAppMCSyncRhoFlowWaitHandle, &gAppMCSyncRhoCtl.agBdAddr.addr);
	}
	else
	{
		app_MCSync_Rho_FlowErrorHandler(APP_MCSYNC_RHO_ERRCODE_RHO_PREPARE_CFM_FAIL);
	}
}

#if RHO_DEVELOPMENT
static U32 app_MCSync_Rho_SetFinishHandler(Handler handler, U16 id, void *msg, U32 id_ext)
{
	BT_AWS_MCE_SET_ROLE_HANDOVER_FINISH_CFM_T *cfm = (BT_AWS_MCE_SET_ROLE_HANDOVER_FINISH_CFM_T *)msg;

	UNUSED(handler);
	UNUSED(id_ext);

	if(!BtAwsMce_IsDefaultRolePartner())
	{
		return 0;
	}

	switch(id)
	{
		case BT_AWS_MCE_SET_ROLE_HANDOVER_FINISH_CFM:
			FW_SetTimer(&gAppMCSyncRhoFlowMessageHandle, APP_MCSYNC_RHO_ID_CONFIRM_TIMER, NULL, 0, RHO_CONFIRM_TIMEOUT_TIME); //reset timer

			if(cfm->status == AWS_MCE_RHO_SET_FINISH_SUCCESS)
			{
				DBG_LOG_APP_McsyncRho( "[APP RHO] Set RHO finish OK", 0);
			}
			else
			{
				DBG_LOG_APP_McsyncRho( "[APP RHO] Set RHO finish failed", 0);
				app_MCSync_Rho_FlowErrorHandler(APP_MCSYNC_RHO_ERRCODE_SET_RHO_FINISH_FAIL);
				if(gAppPowerCtl.powerOff.isNeedToPowerOff)
				{
					APP_PowerOff_Loader(gAppPowerCtl.powerOff.reason);
					gAppPowerCtl.powerOff.isNeedToPowerOff = FALSE;
					APP_MCSync_Rho_SetRhoState(RHO_IDLE);
				}
			}
			break;
	}

	return 0;
}
#endif

static void app_MCSync_Rho_CancelRhoCfmHandler(U16 id, BT_AWS_MCE_CANCEL_ROLE_HANDOVER_CFM_T *cfm)
{
	FW_ReleaseTimer(&gAppMCSyncRhoFlowMessageHandle, APP_MCSYNC_RHO_ID_CONFIRM_TIMER, 0);

	if((id == BT_AWS_MCE_CANCEL_ROLE_HANDOVER_CFM) && (cfm->status == AWS_MCE_RHO_PREPARE_SUCCESS))
	{
		if(gAppMCSyncRhoCtl.retryCnt < RHO_RETRY_COUNT && !gAppMCSyncRhoCtl.isForceStop)
		{
			app_MCSync_Rho_SetFlowIdxByAction(APP_MCSYNC_RHO_ACTION_IS_ALLOW);
			FW_SetTimer(&gAppMCSyncRhoFlowWaitHandle, APP_MCSYNC_RHO_ID_PROCESS, NULL, 0, RHO_DELAY_TIME);
			gAppMCSyncRhoCtl.retryCnt++;
			DBG_LOG_APP_McsyncRho( "[APP RHO] Cancel RHO cfm. Do after:%d ms, retry:%d", 2, RHO_DELAY_TIME, gAppMCSyncRhoCtl.retryCnt);
		}
		else
		{
			DBG_LOG_APP_McsyncRho( "[APP RHO] Cancel RHO cfm. Stop retry RHO", 0);
			app_MCSync_Rho_FlowErrorHandler(APP_MCSYNC_RHO_ERRCODE_LOCK_FAIL);
		}
	}
	else
	{
		app_MCSync_Rho_FlowErrorHandler(APP_MCSYNC_RHO_ERRCODE_LOCK_FAIL);
	}
}

static U32 app_MCSync_Rho_FlowWaitHandler(Handler handler, U16 id, void *msg, U32 id_ext)
{
	UNUSED(handler);
	UNUSED(id_ext);
	U8 action = app_MCSync_Rho_GetCurAction();

	DBG_LOG_APP_McsyncRho( "[APP RHO] RhoFlowWaitHandler id:%d, action:%d", 2, id, action);

	switch(action)
	{
		case APP_MCSYNC_RHO_ACTION_SEND_RHO_PREPARE:
			app_MCSync_Rho_PrepareCfmHandler(id, (BT_AWS_MCE_PREPARE_ROLE_HANDOVER_CFM_T *)msg);
			break;

		case APP_MCSYNC_RHO_ACTION_LOCK:
			app_MCSync_Rho_CancelRhoCfmHandler(id, (BT_AWS_MCE_CANCEL_ROLE_HANDOVER_CFM_T *)msg);
			break;

		case APP_MCSYNC_RHO_ACTION_IS_ALLOW:
<<<<<<< HEAD
			app_MCSync_Rho_FlowProcessHandler(id);
=======
			if(id != BT_AWS_MCE_CANCEL_ROLE_HANDOVER_CFM)
			{
				app_MCSync_Rho_FlowProcessHandler(id);
			}
>>>>>>> db20e11 (second commit)
			break;

		default:
            if(id == BT_AWS_MCE_PREPARE_ROLE_HANDOVER_CFM)
            {
                BT_AWS_MCE_PREPARE_ROLE_HANDOVER_CFM_T *cfm = (BT_AWS_MCE_PREPARE_ROLE_HANDOVER_CFM_T *)msg;

                if(cfm->status == AWS_MCE_RHO_PREPARE_SUCCESS)
                    BtAwsMceCancelRoleHandoverReq(&gAppMCSyncRhoFlowWaitHandle, &gAppMCSyncRhoCtl.agBdAddr.addr);
            }
			break;
	}

	return 0;
}

/**************************************************************************************************
* Static Function (Flow Handler)
**************************************************************************************************/
static U8 app_MCSync_Rho_SniffLockHandler(void)
{
	//Disable sniff
	App_SniffDisable(&gAppMCSyncRhoCtl.agBdAddr, APP_SNIFF_MCSYNC);
	if(!PM_IsInActive(APP_GetServiceBdAddr()))
	{
		FW_SetTimer(&gAppMCSyncRhoFlowMessageHandle, APP_MCSYNC_RHO_ID_CONFIRM_TIMER, NULL, 0, RHO_CONFIRM_TIMEOUT_TIME);

		return APP_MCSYNC_RHO_FLOW_WAIT;
	}

	return APP_MCSYNC_RHO_FLOW_SUCCESS;
}

static U8 app_MCSync_Rho_ConnectableLockHandler(void)
{
	//APP_Pairing_DiscoverableHandler(NON_DISCOVERABLE_MODE);
	APP_Pairing_ConnectableHandler(NON_CONNECTABLE_MODE);

	FW_SetTimer(&gAppMCSyncRhoFlowMessageHandle, APP_MCSYNC_RHO_ID_CONFIRM_TIMER, NULL, 0, RHO_CONFIRM_TIMEOUT_TIME);

	return APP_MCSYNC_RHO_FLOW_WAIT;
}
<<<<<<< HEAD
=======
#ifdef AIR_TILE_ADV_ENABLE	
static U8 app_MCSync_Rho_NotifyPartnerDisconnectBleHandler(void)
{
	/*Notify Patner to stop adv and disconnect BLE. */
	APP_MCSYNC_SendSyncTileInfo(TRUE);
	FW_SetTimer(&gAppMCSyncRhoFlowMessageHandle, APP_MCSYNC_RHO_ID_CONFIRM_TIMER, NULL, 0, RHO_CONFIRM_TIMEOUT_TIME);
	return APP_MCSYNC_RHO_FLOW_WAIT;
}
#endif
>>>>>>> db20e11 (second commit)

static U8 app_MCSync_Rho_DisconnectBleHandler(void)
{
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
	#endif

	#ifdef AIR_TILE_ADV_ENABLE
	APP_Ble_DisableAdv(BLE_ADV_TILE_MASK);
>>>>>>> db20e11 (second commit)
	#endif

    if(!APP_Ble_Disconnect(NULL))
	{
		FW_SetTimer(&gAppMCSyncRhoFlowMessageHandle, APP_MCSYNC_RHO_ID_CONFIRM_TIMER, NULL, 0, RHO_CONFIRM_TIMEOUT_TIME);
		return APP_MCSYNC_RHO_FLOW_WAIT;
	}
    else
	{
		return APP_MCSYNC_RHO_FLOW_SUCCESS;
	}
	//ble only connected with airapp. ble disconnect automatically when airapp is disconnected
}

static U8 app_MCSync_Rho_DisconnectSppHandler(void)
{
	BOOL status = PM_DisconnectSpecificProfile(&gAppMCSyncRhoCtl.agBdAddr, PROFILE_AIR_APP);

	if(status)
	{
		//no spp connection
		return APP_MCSYNC_RHO_FLOW_SUCCESS;
	}

	FW_SetTimer(&gAppMCSyncRhoFlowMessageHandle, APP_MCSYNC_RHO_ID_CONFIRM_TIMER, NULL, 0, RHO_CONFIRM_TIMEOUT_TIME);

	//sent spp disconnect request
	return APP_MCSYNC_RHO_FLOW_WAIT;
	//return APP_MCSYNC_RHO_FLOW_SUCCESS;
}

static U8 app_MCSync_Rho_DisconnectCustAppHandler(void)
{
	#ifdef AIROHA_BT_SPP_ENABLE
	if(!PM_DisconnectSpecificProfile(&gAppMCSyncRhoCtl.agBdAddr, PROFILE_SERIAL_PORT))
	{
		FW_SetTimer(&gAppMCSyncRhoFlowMessageHandle, APP_MCSYNC_RHO_ID_CONFIRM_TIMER, NULL, 0, RHO_CONFIRM_TIMEOUT_TIME);
		return APP_MCSYNC_RHO_FLOW_WAIT;
	}
	#endif
	return APP_MCSYNC_RHO_FLOW_SUCCESS;
}

<<<<<<< HEAD
#ifdef PROFILE_GFP_ENABLE
=======
#ifdef AIR_GFP_ENABLE
>>>>>>> db20e11 (second commit)
static U8 app_MCSync_Rho_DisconnectGfpSppHandler(void)
{
    if(!PM_DisconnectSpecificProfile(&gAppMCSyncRhoCtl.agBdAddr, PROFILE_GFP))
    {
        FW_SetTimer(&gAppMCSyncRhoFlowMessageHandle, APP_MCSYNC_RHO_ID_CONFIRM_TIMER, NULL, 0, RHO_CONFIRM_TIMEOUT_TIME);
        return APP_MCSYNC_RHO_FLOW_WAIT;
    }
    return APP_MCSYNC_RHO_FLOW_SUCCESS;
}
#endif

#ifndef DO_RHO_IMMEDIATELY
static U8 app_MCSync_Rho_IsAllowHandler(void)
{
	if(!app_MCSync_Rho_IsAllow())
	{
		if(gAppMCSyncRhoCtl.retryCnt < RHO_RETRY_COUNT)
		{
			FW_SetTimer(&gAppMCSyncRhoFlowWaitHandle, APP_MCSYNC_RHO_ID_PROCESS, NULL, 0, RHO_DELAY_TIME);
			gAppMCSyncRhoCtl.retryCnt++;
			DBG_LOG_APP_McsyncRho( "[APP RHO] Is allow retry:%d after %d ms", 2, gAppMCSyncRhoCtl.retryCnt, RHO_DELAY_TIME);
			return APP_MCSYNC_RHO_FLOW_WAIT;
		}
		else
		{
			DBG_LOG_APP_McsyncRho( "[APP RHO] Is allow stop retry", 0);
		}

		app_MCSync_Rho_FlowErrorHandler(APP_MCSYNC_RHO_ERRCODE_IS_ALLOW_FAIL);
		return APP_MCSYNC_RHO_FLOW_ERROR;
	}

	return APP_MCSYNC_RHO_FLOW_SUCCESS;
}
#endif

static U8 app_MCSync_Rho_SendRhoPrepareHandler(void)
{
	//send RHO_Prepare command to controller
<<<<<<< HEAD
=======
	app_MCSync_Rho_SetAndSyncRhoState(RHO_CONTROLLER_STARTING);
>>>>>>> db20e11 (second commit)
	BtAwsMcePrepareRoleHandoverReq(&gAppMCSyncRhoFlowWaitHandle, &gAppMCSyncRhoCtl.agBdAddr.addr, &APP_AgentGetPartnerInfo()->addr);

	FW_SetTimer(&gAppMCSyncRhoFlowMessageHandle, APP_MCSYNC_RHO_ID_CONFIRM_TIMER, NULL, 0, RHO_CONFIRM_TIMEOUT_TIME);

	return APP_MCSYNC_RHO_FLOW_WAIT;
}

static U8 app_MCSync_Rho_LockHandler(void)
{
	//status = lock API. APP > Profile > Protocol
	//status = is allowed. APP > Profile > Protocol

	#ifdef DO_RHO_IMMEDIATELY
	if(!app_MCSync_Rho_Lock())
	{
		app_MCSync_Rho_FlowErrorHandler(APP_MCSYNC_RHO_ERRCODE_LOCK_FAIL);
		return APP_MCSYNC_RHO_FLOW_ERROR;
	}
	#endif

	if(!app_MCSync_Rho_IsAllow())
	{
		#ifndef DO_RHO_IMMEDIATELY
		//send rho prepare stop
		DBG_LOG_APP_McsyncRho( "[APP RHO] Have to stop prepare and do again later", 0);
		BtAwsMceCancelRoleHandoverReq(&gAppMCSyncRhoFlowWaitHandle, &gAppMCSyncRhoCtl.agBdAddr.addr);

		FW_SetTimer(&gAppMCSyncRhoFlowMessageHandle, APP_MCSYNC_RHO_ID_CONFIRM_TIMER, NULL, 0, RHO_CONFIRM_TIMEOUT_TIME);
		#else
		FW_MessageSend(&gAppMCSyncRhoFlowWaitHandle, APP_MCSYNC_RHO_ID_PROCESS, NULL);
		#endif

		return APP_MCSYNC_RHO_FLOW_WAIT;
	}

	return APP_MCSYNC_RHO_FLOW_SUCCESS;
}

static U8 app_MCSync_Rho_GetDataHandler(void)
{
	U8 moduleIndex;
	MCSYNC_RHO_PACKET_T *pTxRhoPkt;
	U8 *pDataAddr;
	U16 totalLen = 0;
	U16 moduleLength[AWS_MCE_RHO_MODULE_NUM];

	//call API to get data length from APP, Profile, Protocol
	//get memory
	//call API to get data from APP, Profile, Protocol
	//copy data to memory (assign module, length)

	for(moduleIndex = 0; moduleIndex < AWS_MCE_RHO_MODULE_NUM; moduleIndex++)
	{
		U16 len;

		if(gAppMCSyncRhoModuleCtl[moduleIndex].funcGetLength)
		{
			len = gAppMCSyncRhoModuleCtl[moduleIndex].funcGetLength();
			moduleLength[moduleIndex] = len;
			if(len)
			{
				totalLen += (len + sizeof(BT_AWS_MCE_RHO_MODULE_STUR) - 1);
			}
			DBG_LOG_APP_McsyncRho( "[APP RHO] Get data module ID:%d Size:%d", 2, gAppMCSyncRhoModuleCtl[moduleIndex].moduleId, len);
		}
		else
		{
			moduleLength[moduleIndex] = 0;
		}
	}

	totalLen += (sizeof(MCSYNC_RHO_PACKET_T) - 1);
	DBG_LOG_APP_McsyncRho( "[APP RHO] Get data Size:%d", 1, totalLen);

	//if(!(pTxRhoPkt = FW_GetMemory(totalLen)))
	if(!(pTxRhoPkt = (MCSYNC_RHO_PACKET_T *)BtAwsMceClaimRHOPacket(totalLen)))
	{
		DBG_LOG_APP_McsyncRho( "[APP RHO] Get data no memory. Size:%d", 1, totalLen);
		app_MCSync_Rho_FlowErrorHandler(APP_MCSYNC_RHO_ERRCODE_GET_DATA_FAIL);
		return APP_MCSYNC_RHO_FLOW_ERROR;
	}

	pTxRhoPkt->rpCount = 2;
	pDataAddr = pTxRhoPkt->pHostData;

	for(moduleIndex = 0; moduleIndex < AWS_MCE_RHO_MODULE_NUM; moduleIndex++)
	{
		if(moduleLength[moduleIndex] && gAppMCSyncRhoModuleCtl[moduleIndex].funcGetData)
		{
			if(!gAppMCSyncRhoModuleCtl[moduleIndex].funcGetData(((BT_AWS_MCE_RHO_MODULE_STUR *)pDataAddr)->pData))
			{
				DBG_LOG_APP_McsyncRho( "[APP RHO] Get data failed. ModuleId:%d", 1, gAppMCSyncRhoModuleCtl[moduleIndex].moduleId);
				break;
			}

			((BT_AWS_MCE_RHO_MODULE_STUR *)pDataAddr)->moduleId = gAppMCSyncRhoModuleCtl[moduleIndex].moduleId;
			((BT_AWS_MCE_RHO_MODULE_STUR *)pDataAddr)->length = moduleLength[moduleIndex];

			pDataAddr += (sizeof(BT_AWS_MCE_RHO_MODULE_STUR) - 1 + moduleLength[moduleIndex]);
		}
	}

	if(pDataAddr != (pTxRhoPkt->pHostData + totalLen))
	{
		DBG_LOG_APP_McsyncRho( "[APP RHO] Get data Warning!! 0x%x/%x", 2, (U32)pDataAddr, (U32)(pTxRhoPkt->pHostData + totalLen));
	}

	gAppMCSyncRhoCtl.pTxRhoPacket = pTxRhoPkt;
	gAppMCSyncRhoCtl.rhoTxPacketLen = totalLen;

<<<<<<< HEAD
    StreamL2capLockSink(TRUE);
=======
    StreamL2capLockSink(AGENT_LOCK);
>>>>>>> db20e11 (second commit)

	return APP_MCSYNC_RHO_FLOW_SUCCESS;
}

static U8 app_MCSync_Rho_SendRhoPacketHandler(void)
{
	if(gAppMCSyncRhoCtl.rhoTxPacketLen && gAppMCSyncRhoCtl.pTxRhoPacket)
	{
		DBG_LOG_APP_McsyncRho( "[APP RHO] Send RHO packet len %d", 1, gAppMCSyncRhoCtl.rhoTxPacketLen);
		//MCSYNC_SendRhoIfPkt((U8 *)gAppMCSyncRhoCtl.pTxRhoPacket, gAppMCSyncRhoCtl.rhoTxPacketLen);
		BtAwsMceSendRHOPacket((U8 *)gAppMCSyncRhoCtl.pTxRhoPacket);

		FW_SetTimer(&gAppMCSyncRhoFlowMessageHandle, APP_MCSYNC_RHO_ID_CONFIRM_TIMER, NULL, 0, RHO_CONFIRM_TIMEOUT_TIME);
		app_MCSync_Rho_ClearRhoTxPacketInfo();
	}
	else
	{
		DBG_LOG_APP_McsyncRho( "[APP RHO] Send RHO packet NULL", 0);
	}

	return APP_MCSYNC_RHO_FLOW_WAIT; //wait for role change indication
}

static U8 app_MCSync_Rho_AppProfileProtocolUnlockHandler(void)
{
#ifdef DO_RHO_IMMEDIATELY
	U8 status = app_MCSync_Rho_Unlock();

	//only NEW partner need unlock
	//status = call API to unlock APP, Profile, Protocol

	if(status)
	{
		return APP_MCSYNC_RHO_FLOW_SUCCESS;
	}
	else
	{
		app_MCSync_Rho_FlowErrorHandler(APP_MCSYNC_RHO_ERRCODE_APP_PROFILE_PROTOCOL_UNLOCK_FAIL);
		return APP_MCSYNC_RHO_FLOW_ERROR;
	}
#else
    return APP_MCSYNC_RHO_FLOW_SUCCESS; 
#endif
}

static void app_MCSync_Rho_ActionFlowHandler(U8 action)
{
	U8 status = APP_MCSYNC_RHO_FLOW_ERROR;

	while(action != APP_MCSYNC_RHO_ACTION_NONE)
	{
		DBG_LOG_APP_McsyncRho( "[APP RHO] Action Flow Handler: Flow:%d, Action:%d", 2, gAppMCSyncRhoCtl.curFlowIndex, action);

		switch(action)
		{
			case APP_MCSYNC_RHO_ACTION_SNIFF_LOCK:
				status = app_MCSync_Rho_SniffLockHandler();
				break;

			case APP_MCSYNC_RHO_ACTION_CONNECTABLE_LOCK:
				status = app_MCSync_Rho_ConnectableLockHandler();
				break;
<<<<<<< HEAD

=======
#ifdef AIR_TILE_ADV_ENABLE	
			case APP_MCSYNC_RHO_ACTION_NOTIFY_PARTNER_DISCONNECT_BLE:
				status = app_MCSync_Rho_NotifyPartnerDisconnectBleHandler();
				break;
#endif
>>>>>>> db20e11 (second commit)
			case APP_MCSYNC_RHO_ACTION_DISCONNECT_BLE:
				status = app_MCSync_Rho_DisconnectBleHandler();
				break;

			case APP_MCSYNC_RHO_ACTION_DISCONNECT_SPP:
				status = app_MCSync_Rho_DisconnectSppHandler();
				break;

			case APP_MCSYNC_RHO_ACTION_DISCONNECT_CUSTAPP:
				status = app_MCSync_Rho_DisconnectCustAppHandler();
				break;
<<<<<<< HEAD
#ifdef PROFILE_GFP_ENABLE
=======
#ifdef AIR_GFP_ENABLE
>>>>>>> db20e11 (second commit)
            case APP_MCSYNC_RHO_ACTION_DISCONNECT_GFPSPP:
                status = app_MCSync_Rho_DisconnectGfpSppHandler();
                break;
#endif
			#ifndef DO_RHO_IMMEDIATELY
			case APP_MCSYNC_RHO_ACTION_IS_ALLOW:
				status = app_MCSync_Rho_IsAllowHandler();
				break;
			#endif

			case APP_MCSYNC_RHO_ACTION_SEND_RHO_PREPARE:
				status = app_MCSync_Rho_SendRhoPrepareHandler();
				break;

			case APP_MCSYNC_RHO_ACTION_LOCK:
				status = app_MCSync_Rho_LockHandler();
				break;

			case APP_MCSYNC_RHO_ACTION_GET_DATA:
				status = app_MCSync_Rho_GetDataHandler();
				break;

			case APP_MCSYNC_RHO_ACTION_SEND_RHO_PACKET:
				status = app_MCSync_Rho_SendRhoPacketHandler();
				break;

			case APP_MCSYNC_RHO_ACTION_APP_PROFILE_PROTOCOL_UNLOCK:
				status = app_MCSync_Rho_AppProfileProtocolUnlockHandler();
				break;
		}

		if(status == APP_MCSYNC_RHO_FLOW_WAIT)
		{
			DBG_LOG_APP_McsyncRho( "[APP RHO] Action Flow Handler: Wait", 0);
			break;
		}
		else if(status == APP_MCSYNC_RHO_FLOW_ERROR)
		{
			DBG_LOG_APP_McsyncRho( "[APP RHO] Action Flow Handler: Error", 0);
			break;
		}

		action = app_MCSync_Rho_ShiftAndGetNextAction();
	}
}

static void app_MCSync_Rho_FlowProcessHandler(U16 id)
{
	U8 action;

	switch(id)
	{
		case APP_MCSYNC_RHO_ID_PROCESS:
			DBG_LOG_APP_McsyncRho( "[APP RHO] RhoFlowProcessHandler", 0);
			action = app_MCSync_Rho_GetCurAction();
			break;

		case APP_MCSYNC_RHO_ID_PROCESS_NEXT:
			DBG_LOG_APP_McsyncRho( "[APP RHO] RhoFlowProcessHandler Next", 0);
			action = app_MCSync_Rho_ShiftAndGetNextAction();
			break;

		case APP_MCSYNC_RHO_ID_CONFIRM_TIMER:
			break;

		default:
			FW_Assert(FALSE);
            return;
	}

	DBG_LOG_APP_McsyncRho( "[APP RHO] RhoFlowProcessHandler id:%d action:%d", 2, id, action);

	app_MCSync_Rho_ActionFlowHandler(action);
}

/**************************************************************************************************
* Static Function
**************************************************************************************************/
static void app_MCSync_Rho_PrintCurrentRole()
{
	switch(BtAwsMce_GetDefaultRole())
	{
		case ROLE_NONE:
			DBG_LOG_APP_McsyncRho( "[APP RHO] Current role NONE", 0);
			break;
		case ROLE_PARTNER:
			DBG_LOG_APP_McsyncRho( "[APP RHO] Current role PARTNER", 0);
			break;
		case ROLE_AGENT:
			DBG_LOG_APP_McsyncRho( "[APP RHO] Current role AGENT", 0);
			break;

		default:
			DBG_LOG_APP_McsyncRho( "[APP RHO] Current role Unknown", 0);
			break;
	}
}

static U32 app_MCSync_Rho_PrintRoleHandler(Handler handler, U16 id, void *msg, U32 id_ext)
{
	UNUSED(handler);
	UNUSED(msg);
	UNUSED(id_ext);

	switch(id)
	{
		case 0:
			app_MCSync_Rho_PrintCurrentRole();
			FW_SetTimer(&gAppMCSyncRhoPrintRoleHandle, 0, NULL, 0, 5000); //print role every 5 sec
			break;
	}

	return 0;
}

static U32 app_MCSync_Rho_FlowMessageHandler(Handler handler, U16 id, void *msg, U32 id_ext)
{
	UNUSED(handler);
	UNUSED(id_ext);
	UNUSED(msg);

	switch(id)
	{
		case APP_MCSYNC_RHO_ID_PROCESS:
		case APP_MCSYNC_RHO_ID_PROCESS_NEXT:
			app_MCSync_Rho_FlowProcessHandler(id);
			break;

		case APP_MCSYNC_RHO_ID_CONFIRM_TIMER:
			DBG_LOG_APP_McsyncRho( "[APP RHO] Confirm timeout", 0);
			app_MCSync_Rho_FlowErrorHandler(APP_MCSYNC_RHO_ERRCODE_CONFIRM_TIMEOUT);
			break;
	}

	return 0;
}

static void app_MCSync_Rho_SetAndSyncRhoState(U8 state)
{
	#if RHO_DEVELOPMENT
	DBG_LOG_APP_McsyncRho( "[APP RHO] Set state:%d", 1, state);
	gAppMCSyncRhoCtl.rhoState = state;
	BtAwsMceSetRhoState(state);

<<<<<<< HEAD
	if(state == RHO_STARTING)
=======
	if(state >= RHO_STARTING)
>>>>>>> db20e11 (second commit)
	{
		gRhoMsgPrint = 1;
	}
	else if(state == RHO_IDLE)
	{
		gRhoMsgPrint = 0;
<<<<<<< HEAD
		StreamL2capLockSink(FALSE);
=======
		StreamL2capLockSink(STREAM_UNLOCK);
>>>>>>> db20e11 (second commit)
	}

	#else
	UNUSED(state);
	#endif
}

static void app_MCSync_Rho_SetStartClock(void)
{
	gAppMCSyncRhoCtl.rhoStartClock = FW_GetSysClk();
	DBG_LOG_APP_McsyncRho( "[APP RHO] Start Clock:%d", 1, gAppMCSyncRhoCtl.rhoStartClock);
}

static void app_MCSync_Rho_SetEndClock(void)
{
	U32 processingTime;

	gAppMCSyncRhoCtl.rhoEndClock = FW_GetSysClk();

	if(!gAppMCSyncRhoCtl.rhoStartClock || gAppMCSyncRhoCtl.rhoEndClock < gAppMCSyncRhoCtl.rhoStartClock)
	{
		DBG_LOG_APP_McsyncRho( "[APP RHO] Time calculate error", 0);
		return;
	}

	processingTime = (gAppMCSyncRhoCtl.rhoEndClock - gAppMCSyncRhoCtl.rhoStartClock) * SYSTEM_CLOCK_UNIT_US / 1000; //ms
	DBG_LOG_APP_McsyncRho( "[APP RHO] End Clock:%d, Processing time:%d ms", 2, gAppMCSyncRhoCtl.rhoEndClock, processingTime);
}

/**************************************************************************************************
* Public Function
**************************************************************************************************/
void APP_MCSync_Rho_Init(void)
{
	#if RHO_DEVELOPMENT
	app_MCSync_Rho_Init();
	FW_SetTimer(&gAppMCSyncRhoPrintRoleHandle, 0, NULL, 0, 5000); //print role every 5 sec
	#endif
}

BOOL APP_MCSync_Rho_Start(void)
{
	#if RHO_DEVELOPMENT
	U8 status = app_MCSync_Rho_StartCheck();

	if(RHO_CHECK_IDLE == status
<<<<<<< HEAD
	#ifdef MCSYNC_SHARE_MODE
=======
	#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
		&& MCSYNC_SHARE_MODE_DISABLE == BtMCSync_GetShareMode()
	#endif
	)
	{
		DBG_LOG_APP_McsyncRho( "[APP RHO] Start", 0);

		app_MCSync_Rho_SetStartClock();
		PM_ResetTimer();
		BtHfpResetTimer();
		FW_SetTimer(&gAppMCSyncRhoPrintRoleHandle, 0, NULL, 0, 5000); //print role every 5 sec
		app_MCSync_Rho_SetAndSyncRhoState(RHO_STARTING);
		FW_MessageSend(&gAppMCSyncRhoFlowMessageHandle, APP_MCSYNC_RHO_ID_PROCESS_NEXT, NULL); //wait IF packet (RHO state) has sent
		return TRUE;
	}
	else if(RHO_CHECK_BUSY == status)
		return TRUE;
	#endif
	return FALSE;
}

void APP_MCSync_Rho_AirAppDisconnectNotify(void)
{
	#if RHO_DEVELOPMENT
	app_MCSync_Rho_SppDisconnectCfmHandler();
	#endif
}

void APP_MCSync_Rho_BleDisconnectNotify(U8 status)
{
	#if RHO_DEVELOPMENT
	app_MCSync_Rho_BleDisconnectCfmHandler(status);
	#endif
}

#ifdef AIROHA_BT_SPP_ENABLE
void APP_MCSync_Rho_CustAppDisconnectNotify(void)
{
	app_MCSync_Rho_CustAppDisconnectCfmHandler();
}
#endif

<<<<<<< HEAD
#ifdef PROFILE_GFP_ENABLE
=======
#ifdef AIR_GFP_ENABLE
>>>>>>> db20e11 (second commit)
void APP_MCSync_Rho_GfpSppDisconnectNotify(void)
{
    app_MCSync_Rho_GfpSppDisconnectCfmHandler();
}
#endif

<<<<<<< HEAD
=======
#ifdef AIR_TILE_ADV_ENABLE	
void app_MCSync_Rho_PartnerBleDisconnectNotify(U8 status) //add for tile
{
	app_MCSync_Rho_NotifyPartnerBleDisconnectCfmHandler(status);
}
#endif

>>>>>>> db20e11 (second commit)
void APP_MCSync_Rho_PacketHandler(MCSYNC_RHO_PACKET_T *pRxPacket, U16 length)
{
	#if RHO_DEVELOPMENT
	BD_ADDR_T *pAgentBdAddr = BtMCSync_ReadAgentBdAddr();
	//record RHO packet, release after applying data when receiving role change indication
	DBG_LOG_APP_McsyncRho( "[APP RHO] Record Rx RHO packet. Length:%d", 1, length);
	app_MCSync_Rho_SetStartClock();

	if(!pAgentBdAddr)
	{
<<<<<<< HEAD
		DBG_LOG_APP_McsyncRho( "[APP RHO] Record Rx RHO packet. Null Agent BdAddr", 0);
=======
		DBG_LOG_APP_McsyncRho( "[APP RHO] Record Rx RHO packet. Null Agent BdAddr pRxPacket:0x%x", 1, pRxPacket);
		if(pRxPacket)
			FW_FreeMemory(pRxPacket);

>>>>>>> db20e11 (second commit)
		return;
	}

	gAppMCSyncRhoCtl.pRxRhoPacket = pRxPacket;
	gAppMCSyncRhoCtl.rhoRxPacketLen = length;
	BtAwsMceSetRoleHandoverFinish(&gAppMCSyncSetRhoFinishHandle, &pAgentBdAddr->addr);

	FW_SetTimer(&gAppMCSyncRhoFlowMessageHandle, APP_MCSYNC_RHO_ID_CONFIRM_TIMER, NULL, 0, RHO_CONFIRM_TIMEOUT_TIME);
	FW_SetTimer(&gAppMCSyncRhoPrintRoleHandle, 0, NULL, 0, 5000); //print role every 5 sec
	APP_MCSync_Rho_SetRhoState(RHO_PARTNER_RECEIVE_PKT);
<<<<<<< HEAD
=======
    StreamL2capLockSink(PARTNER_LOCK);
>>>>>>> db20e11 (second commit)
	#else
	UNUSED(pRxPacket);
	UNUSED(length);
	#endif
}

void APP_MCSync_Rho_RoleChangeIndHandler(U8 newRole, BOOL status)
{
	#if RHO_DEVELOPMENT
	if(newRole == ROLE_PARTNER) //agent to partner
	{
		if(APP_MCSYNC_RHO_ACTION_SEND_RHO_PACKET != app_MCSync_Rho_GetCurAction())
			return;

<<<<<<< HEAD
        StreamL2capLockSink(FALSE);
=======
        StreamL2capLockSink(STREAM_UNLOCK);
>>>>>>> db20e11 (second commit)

		FW_ReleaseTimer(&gAppMCSyncRhoFlowMessageHandle, APP_MCSYNC_RHO_ID_CONFIRM_TIMER, 0);

		DBG_LOG_APP_McsyncRho( "[APP RHO] RoleChangeInd: Agent to Partner. status:%d", 1, status);
		if(status)
		{
			app_MCSync_Rho_ApplyData(newRole, NULL, 0);
			app_MCSync_Rho_FlowProcessHandler(APP_MCSYNC_RHO_ID_PROCESS_NEXT);
			DBG_LOG_APP_McsyncRho( "[APP RHO] Agent change to Partner OK", 0);
			app_MCSync_Rho_SetEndClock();
		}
		else
		{
			//fail, keep original role
			app_MCSync_Rho_FlowErrorHandler(APP_MCSYNC_RHO_ERRCODE_ROLE_CHANGE_IND_FAIL);
		}

		if(gAppPowerCtl.powerOff.isNeedToPowerOff)
		{
			APP_PowerOff_Loader(gAppPowerCtl.powerOff.reason);
			gAppPowerCtl.powerOff.isNeedToPowerOff = FALSE;

			if(status != BT_STATUS_SUCCESS)
			{
				APP_MCSync_Rho_SetRhoState(RHO_IDLE);
			}
		}
	}
	else if(newRole == ROLE_SLAVE_AGENT_WITH_PARTNER) //partner to agent
	{
		FW_ReleaseTimer(&gAppMCSyncRhoFlowMessageHandle, APP_MCSYNC_RHO_ID_CONFIRM_TIMER, 0);

		DBG_LOG_APP_McsyncRho( "[APP RHO] RoleChangeInd: Partner to Agent. status:%d", 1, status);
		if(status)
		{
			if(gAppMCSyncRhoCtl.rhoRxPacketLen)
			{
				app_MCSync_Rho_ApplyData(newRole, (MCSYNC_RHO_PACKET_T *)gAppMCSyncRhoCtl.pRxRhoPacket, gAppMCSyncRhoCtl.rhoRxPacketLen);
				app_MCSync_Rho_ClearRhoRxPacketInfo();
				DBG_LOG_APP_McsyncRho( "[APP RHO] Partner change to Agent OK", 0);
				app_MCSync_Rho_SetEndClock();
			}
		}
		else
		{
			//fail, keep original role
			app_MCSync_Rho_FlowErrorHandler(APP_MCSYNC_RHO_ERRCODE_ROLE_CHANGE_IND_FAIL);
		}

		if(gAppPowerCtl.powerOff.isNeedToPowerOff)
		{
			APP_PowerOff_Loader(gAppPowerCtl.powerOff.reason);
			gAppPowerCtl.powerOff.isNeedToPowerOff = FALSE;

			if(status != BT_STATUS_SUCCESS)
			{
				APP_MCSync_Rho_SetRhoState(RHO_IDLE);
			}
		}
	}
    

	app_MCSync_Rho_CheckActionAfterRho();
<<<<<<< HEAD
=======
    #ifdef AIR_GFP_ENABLE
    APP_Gfp_RhoDoneHandle(newRole, status);
    #endif
>>>>>>> db20e11 (second commit)

	#else
	UNUSED(newRole);
	UNUSED(status);
	#endif
}

void APP_MCSync_Rho_ModeChangeIndHandler(BD_ADDR_T *pBdAddr, U8 mode)
{
	if(PM_IsSniffingOrUnsniffingState(pBdAddr))
	{
		return;
	}

	app_MCSync_Rho_SniffCfmHandler(mode);
}

void APP_MCSync_Rho_NonConnectableNotify(void)
{
	#if RHO_DEVELOPMENT
	app_MCSync_Rho_NonConnectableCfmHandler(TRUE);
	#endif
}

BOOL APP_MCSync_Rho_IsRhoing(void)
{
	#if RHO_DEVELOPMENT
<<<<<<< HEAD
	return (gAppMCSyncRhoCtl.rhoState == RHO_STARTING) ? TRUE : FALSE;
=======
	return (gAppMCSyncRhoCtl.rhoState >= RHO_STARTING) ? TRUE : FALSE;
>>>>>>> db20e11 (second commit)
	#endif

	return FALSE;
}

BOOL APP_MCSync_RHO_IsReceiveRHOPkt(void)
{
	#if RHO_DEVELOPMENT
	return (gAppMCSyncRhoCtl.rhoState == RHO_PARTNER_RECEIVE_PKT) ? TRUE : FALSE;
	#endif

	return FALSE;
}

void APP_MCSync_Rho_SetRhoState(U8 state)
{
	app_MCSync_Rho_SetAndSyncRhoState(state);
}

BOOL APP_MCSync_Rho_IsAbleToStartRho(void)
{
	#if RHO_DEVELOPMENT
	U8 status = app_MCSync_Rho_StartCheck();
	return (status == RHO_CHECK_IDLE || status == RHO_CHECK_BUSY) ? TRUE:FALSE;
	#else
	return FALSE;
	#endif
}

void APP_MCSync_Rho_StatusChangeIndHandler(U8 role)
{
	if(!APP_MCSync_Rho_IsRhoing())
		return;

	#if RHO_DEVELOPMENT
	if(role == ROLE_AGENT)
	{
		FW_ReleaseTimer(&gAppMCSyncRhoFlowMessageHandle, APP_MCSYNC_RHO_ID_CONFIRM_TIMER, 0);
		app_MCSync_Rho_FlowErrorHandler(APP_MCSYNC_RHO_ERRCODE_PARTNER_DISCONNECTED);
	}
	#else
	UNUSED(role);
	#endif
}

BOOL APP_MCSync_Rho_Stop(void)
{
	#if RHO_DEVELOPMENT
	U8 action = app_MCSync_Rho_GetCurAction();

	if(!APP_MCSync_Rho_IsRhoing())
		return FALSE;

	DBG_LOG_APP_McsyncRho( "[APP RHO] Force Stop. action:%d", 1, action);

	gAppMCSyncRhoCtl.isForceStop = TRUE;

	switch(action)
	{
		case APP_MCSYNC_RHO_ACTION_SNIFF_LOCK:
			app_MCSync_Rho_FlowErrorHandler(APP_MCSYNC_RHO_ERRCODE_SNIFF_CFM_FAIL);
			return TRUE;

		case APP_MCSYNC_RHO_ACTION_CONNECTABLE_LOCK:
			app_MCSync_Rho_FlowErrorHandler(APP_MCSYNC_RHO_ERRCODE_CONNECTABLE_CFM_FAIL);
			return TRUE;

<<<<<<< HEAD
=======
#ifdef AIR_TILE_ADV_ENABLE	
		case APP_MCSYNC_RHO_ACTION_NOTIFY_PARTNER_DISCONNECT_BLE:
			app_MCSync_Rho_FlowErrorHandler(APP_MCSYNC_RHO_ERRCODE_NOTIFY_PARTNER_BLE_DISCONNECT_CFM_FAIL);
			break;
#endif

>>>>>>> db20e11 (second commit)
		case APP_MCSYNC_RHO_ACTION_DISCONNECT_BLE:
			app_MCSync_Rho_FlowErrorHandler(APP_MCSYNC_RHO_ERRCODE_BLE_DISCONNECT_CFM_FAIL);
			return TRUE;

		case APP_MCSYNC_RHO_ACTION_DISCONNECT_SPP:
			app_MCSync_Rho_FlowErrorHandler(APP_MCSYNC_RHO_ERRCODE_SPP_DISCONNECT_CFM_FAIL);
			return TRUE;

		case APP_MCSYNC_RHO_ACTION_DISCONNECT_CUSTAPP:
			app_MCSync_Rho_FlowErrorHandler(APP_MCSYNC_RHO_ERRCODE_CUSTAPP_DISCONNECT_CFM_FAIL);
			return TRUE;

		case APP_MCSYNC_RHO_ACTION_IS_ALLOW:
			app_MCSync_Rho_FlowErrorHandler(APP_MCSYNC_RHO_ERRCODE_IS_ALLOW_FAIL);
			return TRUE;

		case APP_MCSYNC_RHO_ACTION_SEND_RHO_PREPARE:
			//have to wait cfm. if prepare successful, cancel prepare.
			return TRUE;

		case APP_MCSYNC_RHO_ACTION_LOCK:
			//stop at cancel prepare cfm
			return TRUE;

		default:
			//can not stop
			gAppMCSyncRhoCtl.isForceStop = FALSE;
			return FALSE;
	}
	#else
	return FALSE;
	#endif
}
