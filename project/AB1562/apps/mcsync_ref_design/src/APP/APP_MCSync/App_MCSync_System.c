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
#include "BtMCSync.h"
#include "App_ResourceManager.h"
#include "App_ReConnection.h"
#include "PM_Interface.h"
#include "App_MCSync_Message.h"
#include "App_State.h"
#include "App_EventOpcode.h"
#include "App_MCSync.h"
#include "App_MCSync_State.h"
#include "App_System.h"
#include "App_Interface.h"
#include "App_MCSync_System.h"
#include "App_MCSync_NvkeyHandler.h"
#include "App_MCSync_System.h"
#include "BtMCSync.h"
#include "drv_a2dp.h"
#include "App_RhoHandler.h"
<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
#include "App_MCSync_Share.h"
#endif


enum
{
	AUDIO_CH_LR_FOR_INQUIRY,
	AUDIO_CH_L_FOR_INQUIRY,
	AUDIO_CH_R_FOR_INQUIRY
};

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static U32 	app_mcsync_SystemAirPairingHandler(Handler handler, U16 id, void *msg, U32 handler_id);
static void app_mcsync_SystemInquiryCfmHandler(BT_INQUIRY_REQ_CFM_T * cfm);
static void app_mcsync_SystemInquiryCancelCfmHandler(BT_INQUIRY_CANCEL_CFM_T * cfm);
static void app_mcsync_SystemWriteInquiryModeCfmHandler(BT_WRITE_INQUIRY_MODE_CFM_T * cfm);
static void app_mcsync_SystemInquiryParaIndHandler(BT_INQUIRY_RESULT_IND_T * ind);
static void app_mcsync_SystemInquiryStopIndHandler(BT_INQUIRY_STOP_IND_T * ind);
static void app_mcsync_SystemWriteCurentIACCfmHandler(BT_WRITE_INQUIRY_ACCESS_CODE_CFM_T * cfm);
static void app_mcsync_SystemWriteAirPairingLIAC(void);
static void app_mcsync_SystemWriteGIAC(void);
static void app_mcsync_SystemSendInquiryUnderAirPairingState(void);

/**************************************************************************************************
* Variable
**************************************************************************************************/
HandlerData gAppMCSyncSystemHandle = { app_mcsync_SystemAirPairingHandler };
<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
extern HandlerData gAppMCSyncStateTimerHandle;
#endif

/*************************************************************************************************
* Extern Variable
***************************************************************************************************/
extern APP_MCSYNC_STRU gAppMCSyncCtl;

const U32 gAppAwsMceGIAC = 0x9E8B33;

/**************************************************************************************************
* Static (System Handler)
**************************************************************************************************/
static U32 app_mcsync_SystemAirPairingHandler(Handler handler, U16 id, void *msg, U32 handler_id)
{
	UNUSED(handler);
	UNUSED(handler_id);

	switch(id)
	{
		case BT_INQUIRY_REQ_CFM:
			DBG_LOG_APP_MCSYNC( "[APP_MCSync] Request Inquiry Status confirmation:%d", 1, ((BT_INQUIRY_REQ_CFM_T *)msg)->status);
			app_mcsync_SystemInquiryCfmHandler((BT_INQUIRY_REQ_CFM_T *)msg);
			break;
		case BT_INQUIRY_CANCEL_CFM:
			DBG_LOG_APP_MCSYNC( "[APP_MCSync] Inquiry Cancel confirm", 0);
			app_mcsync_SystemInquiryCancelCfmHandler((BT_INQUIRY_CANCEL_CFM_T *)msg);
			break;
		case BT_WRITE_INQUIRY_MODE_CFM:
			app_mcsync_SystemWriteInquiryModeCfmHandler((BT_WRITE_INQUIRY_MODE_CFM_T *)msg);
			break;
		case BT_INQUIRY_RESULT_IND:
			DBG_LOG_APP_MCSYNC( "[APP_MCSync] System Inquiry Result", 0);
			app_mcsync_SystemInquiryParaIndHandler((BT_INQUIRY_RESULT_IND_T *)msg);
			break;

		case BT_INQUIRY_STOP_IND:
			DBG_LOG_APP_MCSYNC( "[APP_MCSync] System inquiry stop indication", 0);
			app_mcsync_SystemInquiryStopIndHandler((BT_INQUIRY_STOP_IND_T *)msg);
			break;

		case BT_WRITE_INQUIRY_ACCESS_CODE_CFM:
			DBG_LOG_APP_MCSYNC( "[APP_MCSync] System Write Inquiry Access Code confirmation", 0);
			app_mcsync_SystemWriteCurentIACCfmHandler((BT_WRITE_INQUIRY_ACCESS_CODE_CFM_T *)msg);
			break;

		default:
			DBG_LOG_APP_SYSTEM( "[APP_SYS] Rx Unknown Message:%x", 1, id);
			break;
	}
	return 0;
}

static void app_mcsync_SystemInquiryCfmHandler(BT_INQUIRY_REQ_CFM_T * cfm)
{
	if(cfm->hci_status != BT_STATUS_SUCCESS)
	{
		APP_MCSYNC_SystemAirPairingEnd();
	}
}

static void app_mcsync_SystemInquiryCancelCfmHandler(BT_INQUIRY_CANCEL_CFM_T * cfm)
{
	UNUSED(cfm);
}

static void app_mcsync_SystemWriteInquiryModeCfmHandler(BT_WRITE_INQUIRY_MODE_CFM_T * cfm)
{
<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
	DBG_LOG_APP_MCSYNC( "[APP_MCSync] Write Inquiry mode confirm: status:%d, isAir:%d, isShare:%d", 3, cfm->hci_status, BtAwsMce_IsInAirPairing(), BtMCSync_IsInMCSyncSharePairing());
#else
	DBG_LOG_APP_MCSYNC( "[APP_MCSync] Write Inquiry mode confirm: status:%d", 1, cfm->hci_status);
#endif
	if(APP_State_CheckNestStateByLink(APP_EOF, APP_AWS_PAIRING)
<<<<<<< HEAD
	#ifdef MCSYNC_SHARE_MODE
=======
	#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
		&& BtAwsMce_IsInAirPairing()
	#endif
	)
	{
		if(	cfm->hci_status == BT_STATUS_SUCCESS)
		{
			app_mcsync_SystemWriteAirPairingLIAC();
		}
		else
		{
			APP_MCSYNC_SystemAirPairingEnd();
		}
	}
<<<<<<< HEAD
	#ifdef MCSYNC_SHARE_MODE
=======
	#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
	else if(BtMCSync_IsInMCSyncSharePairing())
	{
        APP_MCSync_Share_WriteInquiryModeCfmHandler(cfm->hci_status);
	}	
	#endif
}

static void app_mcsync_SystemInquiryParaIndHandler(BT_INQUIRY_RESULT_IND_T * ind)
{
	if(APP_State_CheckNestStateByLink(APP_EOF, APP_AWS_PAIRING)
<<<<<<< HEAD
	#ifdef MCSYNC_SHARE_MODE
=======
	#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
		&& BtAwsMce_IsInAirPairing()
	#endif
	)
	{
		APP_MCSYNC_StateGetAirPairingInquiryResultWithRSSI(ind->rssi, (BD_ADDR_T*)&ind->bd_addr[0]);
	}
<<<<<<< HEAD
	#ifdef MCSYNC_SHARE_MODE
=======
	#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
	else if(BtMCSync_IsInMCSyncSharePairing())
	{
        APP_MCSync_Share_InquiryParaIndHandler(ind->rssi, (BD_ADDR_T*)&ind->bd_addr[0]);
	}	
	#endif
}

static void app_mcsync_SystemInquiryStopIndHandler(BT_INQUIRY_STOP_IND_T * ind)
{
	UNUSED(ind);

	app_mcsync_SystemSendInquiryUnderAirPairingState();
}

static void app_mcsync_SystemWriteCurentIACCfmHandler(BT_WRITE_INQUIRY_ACCESS_CODE_CFM_T * cfm)
{
	if(APP_State_CheckNestStateByLink(APP_EOF, APP_AWS_PAIRING)
<<<<<<< HEAD
	#ifdef MCSYNC_SHARE_MODE
=======
	#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
		&& BtAwsMce_IsInAirPairing()
	#endif	
	)
	{
		if(cfm->hci_status == BT_STATUS_SUCCESS)
		{
			APP_MCSYNC_SystemStartAirPairingInquiry();
		}
		else
		{
			APP_MCSYNC_SystemAirPairingEnd();
		}
	}
<<<<<<< HEAD
	#ifdef MCSYNC_SHARE_MODE
=======
	#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
	else if(BtMCSync_IsInMCSyncSharePairing())
	{
        APP_MCSync_Share_WriteCurentIACCfmHandler(cfm->hci_status);
	}	
	#endif
}

U8 app_mcsync_SystemGetAudioChannel(void)
{
	switch(drv_a2dp_get_audio_channel(FALSE))
	{
		case AU_DSP_CH_L:
			return AUDIO_CH_L_FOR_INQUIRY;
		case AU_DSP_CH_R:
			return AUDIO_CH_R_FOR_INQUIRY;
		default:
			return AUDIO_CH_LR_FOR_INQUIRY;
	}
	return AUDIO_CH_LR_FOR_INQUIRY;
}

static void app_mcsync_SystemWriteAirPairingLIAC(void)
{
	U32 LIAC = (AUDIO_CH_R_FOR_INQUIRY == app_mcsync_SystemGetAudioChannel()) ? APP_MCSync_Nvkey_GetInquiryLIAC() : APP_MCSync_Nvkey_GetLIAC(); //BTA-4431 request

	BtWriteInquiryAccessCode(&gAppMCSyncSystemHandle, (const U32 *)&LIAC, 1);
}

static void app_mcsync_SystemWriteGIAC(void)
{
	BtWriteInquiryAccessCode(&gAppMCSyncSystemHandle, &gAppAwsMceGIAC, 1);
}

static void app_mcsync_SystemSendInquiryUnderAirPairingState(void)
{
	if(APP_State_CheckNestStateByLink(APP_EOF, APP_AWS_PAIRING))
		APP_MCSYNC_SystemStartAirPairingInquiry(); // it will stop at "APP_MCSYNC_STATE_TIMEOUT" timer timeout
}


/**************************************************************************************************
* Public function
**************************************************************************************************/
U8 APP_MCSYNC_SystemGetMsgNumInQueue(void)
{
	return MSG_MessageSearchAllHandler(&gAppMCSyncSystemHandle);
}

void APP_MCSYNC_SystemAirPairingEnd(void)
{
	APP_MCSYNC_StateStopPairing();
}

void APP_MCSYNC_SystemAirPairingStart(void)
{
	BtWriteInquiryMode(&gAppMCSyncSystemHandle, (INQUARY_MODE_E)INQUARY_MODE_RSSI);
}

void APP_MCSYNC_SystemStartAirPairingInquiry(void)
{
    U32 liacNvKey;

	BtWriteInquiryScanActivity(&gAppMCSyncSystemHandle, AIR_PAIRING_INQ_SCAN_INTERVAL, INQ_SCAN_DEFAULT_WINDOW);
	BtWriteScanEnable(&gAppMCSyncSystemHandle, (SCAN_ENABLE_T)SCAN_ENABLE_INQ);

    liacNvKey = (AUDIO_CH_R_FOR_INQUIRY == app_mcsync_SystemGetAudioChannel()) ? APP_MCSync_Nvkey_GetLIAC() : APP_MCSync_Nvkey_GetInquiryLIAC(); //BTA-4431 request
    DBG_LOG_APP_MCSYNC( "[APP_MCSync] Inquiry LIAC value:%x, Time:%d, Num:%d", 3, liacNvKey, APP_MCSync_Nvkey_InquiryTime(), APP_MCSync_Nvkey_InquiryDevNum());
    BtInquiry(&gAppMCSyncSystemHandle, liacNvKey, APP_MCSync_Nvkey_InquiryTime(), APP_MCSync_Nvkey_InquiryDevNum());
}

void APP_MCSYNC_SystemStopAirPairingInquiry(void)
{
	BtInquiryCancel(&gAppMCSyncSystemHandle);
	BtWriteInquiryScanActivity(&gAppMCSyncSystemHandle, INQ_SCAN_DEFAULT_INTERVAL, INQ_SCAN_DEFAULT_WINDOW);
	BtWriteScanEnable(&gAppMCSyncSystemHandle, (SCAN_ENABLE_T)SCAN_ENABLE_OFF);
	app_mcsync_SystemWriteGIAC();
}

