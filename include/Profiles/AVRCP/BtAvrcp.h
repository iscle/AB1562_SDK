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


#ifndef _BTAVRCP_H_
#define _BTAVRCP_H_

#include "bt.h"


/**************************************************************************************************
* API (Connection Relative)
**************************************************************************************************/
void AVRCP_StartService(Handler app);
void AVRCP_StopService(Handler app);
BOOL AVRCP_ConnectRequest(BD_ADDR_T *pBdAddr);
void AVRCP_ConnectResponse(BD_ADDR_T *pBdAddr, BOOL response, U16 channelId, U8 identifier);
BOOL AVRCP_DisconnectRequest(BD_ADDR_T *pBdAddr);
U8 AVRCP_GetProfileState(BD_ADDR_T *pBdAddr);

/**************************************************************************************************
* API
**************************************************************************************************/
void AVRCP_SendOperation(BD_ADDR_T *pBdAddr, U8 operationId, U8 operationValue);
void AVRCP_SetNotifyMask(BD_ADDR_T *pBdAddr, U8 notifyEventMask);

/**************************************************************************************************
* API (Send AVRCP Command)
**************************************************************************************************/
BOOL AVRCP_GetCapsRequest(Handler app, BD_ADDR_T *pBdAddr, U8 capabilityID);
BOOL AVRCP_GetElementAttributesRequest(Handler app, BD_ADDR_T *pBdAddr, U8 attributeId);
BOOL AVRCP_RegisterNotificationRequest(Handler app, BD_ADDR_T *pBdAddr, U8 eventId, U8 transactionLabel);
BOOL AVRCP_SetAbsoluteVolumeRequest(Handler app, BD_ADDR_T *pBdAddr, U8 absoluteVolume);
BOOL AVRCP_PassthroughRequest(Handler app, BD_ADDR_T *pBdAddr, U8 operationId, U8 buttonState);
BOOL AVRCP_VendorUniqueRequest(Handler app, BD_ADDR_T *pBdAddr, U8 operationId, U8 operationParam);
BOOL AVRCP_GetPlayStatusRequest(Handler app, BD_ADDR_T *pBdAddr);
BOOL AVRCP_GetAppValueRequest(Handler app, BD_ADDR_T *pBdAddr, U8 attributeId);
BOOL AVRCP_SetAppValueRequest(Handler app, BD_ADDR_T *pBdAddr, U8 attributeId, U8 attributeValue);
BOOL AVRCP_ContinueResponseRequest(Handler app, BD_ADDR_T *pBdAddr, U8 conti, U8 pduId);


/**************************************************************************************************
* API (Send AVRCP Response)
**************************************************************************************************/
BOOL AVRCP_UnitInfoResponse(BD_ADDR_T *pBdAddr, U8 transactionLabel, U8 resultCode, U8 subUnitID, U8 subUnitType);
BOOL AVRCP_SubunitInfoResponse(BD_ADDR_T *pBdAddr, U8 transactionLabel, U8 resultCode, U8 subUnitID, U8 subUnitType);
BOOL AVRCP_GetCapsResponse(BD_ADDR_T *pBdAddr, U8 transactionLabel, U8 resultCode, U32 companyID, U8 capabilityId);
BOOL AVRCP_GetElementAttributesResponse(BD_ADDR_T *pBdAddr, U8 transactionLabel, U8 resultCode, U32 companyID);
BOOL AVRCP_SendRegisterNotificationVolInterim(BD_ADDR_T *pBdAddr, U8 transactionLabel, U8 resultCode, U32 companyID, U8 currentAbsVolume);
BOOL AVRCP_SetAbsoluteVolumeResponse(BD_ADDR_T *pBdAddr, U8 transactionLabel, U8 resultCode, U32 companyID, U8 absVolume);
BOOL AVRCP_VendorDependentRejectResponse(BD_ADDR_T *pBdAddr, U8 transactionLabel, U8 resultCode, U32 companyID, U8 pduID);
BOOL AVRCP_PassthroughResponse(BD_ADDR_T *pBdAddr, U8 transactionLabel, U8 resultCode, U8 operandID, U8 buttonState, U8 operandLength);
BOOL AVRCP_VolumeChangedNotifyResponse(BD_ADDR_T *pBdAddr, U8 absVolume);

/**************************************************************************************************
* API (RHO)
**************************************************************************************************/
BOOL BtAvrcpLock();
BOOL BtAvrcpUnlock();
BOOL BtAvrcpIsAllow();
U16 BtAvrcpGetRhoDataLength();
BOOL BtAvrcpGetRhoData(U8 *pBuffer);
BOOL BtAvrcpApplyAgentRhoData(BD_ADDR_T *pBdAddr, U8 *pAvctpData, U16 length);
BOOL BtAvrcpApplyPartnerRhoData(BD_ADDR_T *pBdAddr, U8 *pAvctpData, U16 length);


#endif //_BTAVRCP_H_
