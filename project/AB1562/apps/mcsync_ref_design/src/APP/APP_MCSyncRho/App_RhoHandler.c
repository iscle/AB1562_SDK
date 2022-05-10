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
#include "App_RhoHandler.h"
#include "App_LinkKey.h"
#include "BtMCSync.h"
#include "App_State.h"
#include "AudioDSP_StreamManager.h"
#include "APP_AirApp.h"
#include "App_MCSync_Message.h"
#include "APP_HfpScoHandler.h"
#include "DrvSmartCase.h"
#include "App_InEarDetection.h"
<<<<<<< HEAD
#ifdef PROFILE_GFP_ENABLE
=======
#ifdef AIR_GFP_ENABLE
>>>>>>> db20e11 (second commit)
#include "App_Gfp.h"
#endif
#include "App_Ble.h"

/**************************************************************************************************
* Definition
**************************************************************************************************/
#define APP_RHO_MSG_RECORD_HEADER_LENGTH (sizeof(APP_RHO_MSG_RECORD_STRU) - 1)

log_create_module(APP_MCS_RHO, PRINT_LEVEL_INFO);

/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct
{
	BOOL isLock;
	U16 totalQueueLen;
}APP_RHO_HANDLER_STRU;

typedef struct
{
	U16 msgId;
	U16 msgLen; //only data length
	U8 pMsgData[1];
}PACKED APP_RHO_MSG_RECORD_STRU;

/**************************************************************************************************
* Variable
**************************************************************************************************/
static APP_RHO_HANDLER_STRU gAppRhoHandler;

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
BOOL APP_Lock(void)
{
	gAppRhoHandler.isLock = TRUE;

	return TRUE;
}

BOOL APP_Unlock(void)
{
	gAppRhoHandler.isLock = FALSE;
	//clear queue

	return TRUE;
}

BOOL APP_IsLock(void)
{
	return gAppRhoHandler.isLock;
}

BOOL APP_RecordMessage(U16 recordId, void *pRecordData, U16 length)
{
	APP_RHO_MSG_RECORD_STRU *pMsgData;
	U8 pushStatus;
	U16 dataSize;

	DBG_LOG_APP_McsyncRho( "[RHO_APP] Old Agent Record Msg length:%d, recordData:%d", 2, length, pRecordData ? TRUE : FALSE);

	if(!length || !pRecordData)
		return FALSE;

	dataSize = APP_RHO_MSG_RECORD_HEADER_LENGTH + length;

	if( !(pMsgData = (APP_RHO_MSG_RECORD_STRU *)FW_GetMemory(dataSize)) )
	{
		DBG_LOG_APP_McsyncRho( "[RHO_APP] Old Agent Record Msg error: no memory:%d", 1, dataSize);
		return FALSE;
	}

	pMsgData->msgId = recordId;
	pMsgData->msgLen = length;
	FW_Memcpy(pMsgData->pMsgData, (U8 *)pRecordData, length);

	if( SUCCESS != (pushStatus = FW_Queue_Push(pMsgData, QUEUE_TYPE_APP)) )
	{
		DBG_LOG_APP_McsyncRho( "[RHO_APP] Old Agent Record Msg error: push fail:%d", 1, pushStatus);
		FW_Assert(FALSE);
	}

	gAppRhoHandler.totalQueueLen += dataSize;
	DBG_LOG_APP_McsyncRho( "[RHO_APP] Old Agent Record Msg total Qlength:%d", 1, gAppRhoHandler.totalQueueLen);

	return TRUE;
}

BOOL APP_IsAllow(void)
{
	return APP_IsAllowHandler();
}

U16 APP_GetRhoDataLength(void)
{
	U16 length = sizeof(APP_RHO_DATA_STRU) + gAppRhoHandler.totalQueueLen;

	DBG_LOG_APP_McsyncRho( "[RHO_APP] Old Agent Get Data length:%d", 1, length);

	return length;
}

BOOL APP_GetRhoData(U8 *pBuffer)
{
	U8 *pCopyPosition;
	U16 queueDataLen;
	APP_RHO_MSG_RECORD_STRU *pQueueData;
	APP_RHO_DATA_STRU appRhoData;

	if(!pBuffer)
	{
		DBG_LOG_APP_McsyncRho( "[RHO_APP] Old Agent Get Data error: No buffer", 0);
		return FALSE;
	}

	if( !(APP_FillRhoData(&appRhoData)) )
	{
		DBG_LOG_APP_McsyncRho( "[RHO_APP] Old Agent Get Data error: Fill data fail", 0);
		return FALSE;
	}

	//data
	pCopyPosition = pBuffer;
	FW_Memcpy(pCopyPosition, (U8 *)&appRhoData, sizeof(APP_RHO_DATA_STRU));
	pCopyPosition += sizeof(APP_RHO_DATA_STRU);

	//message
	while( (pQueueData = (APP_RHO_MSG_RECORD_STRU *)FW_Queue_Pop(QUEUE_TYPE_APP)) )
	{
		queueDataLen = APP_RHO_MSG_RECORD_HEADER_LENGTH + pQueueData->msgLen;
		DBG_LOG_APP_McsyncRho( "[RHO_APP] Old Agent Get Data popQ:%d, %d", 2, queueDataLen, gAppRhoHandler.totalQueueLen);
		FW_Memcpy(pCopyPosition, pQueueData, queueDataLen);
		pCopyPosition += queueDataLen;
		gAppRhoHandler.totalQueueLen -= queueDataLen;

		FW_FreeMemory(pQueueData);
	}

	if(gAppRhoHandler.totalQueueLen)
	{
		DBG_LOG_APP_McsyncRho( "[RHO_APP] Old Agent Get Data error: remaining Qlength:%d", 1, gAppRhoHandler.totalQueueLen);
		FW_Assert(FALSE);
	}

	return TRUE;
}

BOOL APP_ApplyAgentRhoData(BD_ADDR_T *pBdAddr, U8 *pAppData, U16 length)
{
	U8 *pApplyPos, msgNum;
	U16 len = length;

	UNUSED(pBdAddr);

	DBG_LOG_APP_McsyncRho( "[RHO_APP] New Agent apply length:%d, appData:0x%x", 2, length, pAppData ? TRUE : FALSE);

	if(!length || !pAppData)
	{
		return FALSE;
	}

	pApplyPos = pAppData;

	//data
	if( !APP_AssignRhoData((APP_RHO_DATA_STRU *)pApplyPos))
	{
		DBG_LOG_APP_McsyncRho( "[RHO_APP] New Agent assign data error", 0);
		return FALSE;
	}
	pApplyPos += sizeof(APP_RHO_DATA_STRU);
	len -= sizeof(APP_RHO_DATA_STRU);

	//message
	msgNum = 0;
	while(len)
	{
		U8 *msg;
		APP_RHO_MSG_RECORD_STRU *pRhoMsg = (APP_RHO_MSG_RECORD_STRU *)pApplyPos;

		if( !(msg = FW_GetMemory(pRhoMsg->msgLen)) )
		{
			DBG_LOG_APP_McsyncRho( "[RHO_APP] New Agent send msg no memory:%d", 1, pRhoMsg->msgLen);
			return FALSE;
		}

		FW_Memcpy(msg, pRhoMsg->pMsgData, pRhoMsg->msgLen);
		FW_MessageSend(PM_GAP_GetHandle(), pRhoMsg->msgId, msg);

		pApplyPos += (APP_RHO_MSG_RECORD_HEADER_LENGTH + pRhoMsg->msgLen);
		len -= (APP_RHO_MSG_RECORD_HEADER_LENGTH + pRhoMsg->msgLen);
		msgNum++;
	}

	if(len)
	{
		DBG_LOG_APP_McsyncRho( "[RHO_APP] New Agent msg length error:%d", 1, len);
		return FALSE;
	}

<<<<<<< HEAD
=======
	#ifdef AIR_TAKE_OVER_LINK_ENABLE
	//APP_Conn_SetTimer(TIMER_ID_ACL_LINK_TIMER, 30000);
	#endif
	
>>>>>>> db20e11 (second commit)
	DBG_LOG_APP_McsyncRho( "[RHO_APP] New Agent apply OK:%d", 1, msgNum);
	return TRUE;
}

BOOL APP_ApplyPartnerRhoData(BD_ADDR_T *pBdAddr, U8 *pAppData, U16 length)
{
	UNUSED(pBdAddr);
	UNUSED(pAppData);
	UNUSED(length);

	BD_ADDR_T *pAgentAddr = BtMCSync_ReadAgentBdAddr();

	APP_SetNewPartnerConfig(APP_GetServiceBdAddr(), pAgentAddr);
	//APP_LinkKey_ResetLinkHistory();
	APP_State_RmvStateAllForce();
	APP_State_AddTopState(pAgentAddr, APP_FAKEON);
	APP_State_AddTopState(pAgentAddr, APP_CONNECTABLE);
	APP_State_AddTopState(pAgentAddr, APP_CONNECTED);

    APP_AudioDSP_SetBDA(pAgentAddr);
	APP_HfpSco_DisableLightMonoMode(pAgentAddr);
    APP_Ble_DisableAdv(BLE_ADV_PRIMARY_MASK);
<<<<<<< HEAD

	#ifdef PROFILE_GFP_ENABLE
    APP_Gfp_DisableAdv();
=======
#ifdef AIR_GFP_ENABLE
    APP_Ble_DisableAdv(BLE_ADV_GFP_MASK);
#endif

	#ifdef AIR_TAKE_OVER_LINK_ENABLE
	//APP_Conn_ReleaseTimer(TIMER_ID_ACL_LINK_TIMER);
>>>>>>> db20e11 (second commit)
	#endif

	APP_MCSYNC_SendSyncSmartChargeCaseStateInfo(DrvCharger_GetSmartCaseState());
	APP_AudioReg_SetCurrentAudioDevice(NULL, AUDIO_DEVICE_NONE);


	if(APP_InEarDetection_IsEnable())
	{
		APP_MCSYNC_SendSyncPartnerInEarDetectionStatus(APP_InEarDetection_GetStatus());
	}

	DBG_LOG_APP_McsyncRho( "[RHO_APP] New Partner applied", 0);

	return TRUE;
}

