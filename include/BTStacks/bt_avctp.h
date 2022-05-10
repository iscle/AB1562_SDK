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

#ifndef	_BT_AVCTP_H_
#define _BT_AVCTP_H_


/**************************************************************************************************
* Enumeration
**************************************************************************************************/
typedef enum {
    AVCTP_CMD = 0,
    AVCTP_RSP
} AVCTP_MSG_TYPE_E; //DataHandler

/**************************************************************************************************
* Structure
**************************************************************************************************/
#ifdef WIN32
#pragma pack(push,1)
#endif

typedef struct {
    U8  status;
} PACKED BT_AVCTP_REGISTER_CFM_T;

typedef struct {
    U8  status;
} PACKED BT_AVCTP_UNREGISTER_CFM_T;

typedef struct {
    U8 status;
    BD_ADDR_T bdAddr;
} PACKED BT_AVCTP_CONNECT_CFM_T;

typedef struct {
    BD_ADDR_T bdAddr;
    U8 identifier;
    U16 channelId;
} PACKED BT_AVCTP_CONNECT_IND_T;

typedef struct {
    U8 status;
    BD_ADDR_T bdAddr;
} PACKED BT_AVCTP_DISCONNECT_CFM_T;

typedef struct {
    BD_ADDR_T bdAddr;
    U8 transactionLabel;
<<<<<<< HEAD
    U8 length;
=======
    U16 length;
>>>>>>> db20e11 (second commit)
    U8 data[1];
} PACKED BT_AVCTP_MORE_DATA_T;

#ifdef WIN32
#pragma pack(pop)
#endif

/*** AVCTP API ***/
/**************************************************************************************************
* Internal Public Functions (Message Send)
**************************************************************************************************/
void BtAvctpRegisterL2cap(Handler app, U16 pid);
void BtAvctpUnregisterL2cap(Handler app);
void BtAvctpConnectRequest(BD_ADDR_T *pBdAddr, U16 mtu);
void BtAvctpConnectResponse(BD_ADDR_T *pBdAddr, BOOL response, U16 channelID, U8 identifier, U16 mtu);
void BtAvctpDisconnectRequest(BD_ADDR_T *pBdAddr);

/**************************************************************************************************
* Internal Public Functions (Directly)
**************************************************************************************************/
BOOL BtAvctpSendData(BD_ADDR_T *pBdAddr, U8 *pData, U8 dataLength, U8 msgType, U8 transactionLabel);


/**************************************************************************************************
* API (RHO)
**************************************************************************************************/
BOOL BtAvctpLock();
BOOL BtAvctpUnlock();
BOOL BtAvctpIsAllow();
U16 BtAvctpGetRhoDataLength();
BOOL BtAvctpGetRhoData(U8 *pBuffer);
BOOL BtAvctpApplyAgentRhoData(BD_ADDR_T *pBdAddr, U8 *pAvctpData, U16 length);
BOOL BtAvctpApplyPartnerRhoData(BD_ADDR_T *pBdAddr, U8 *pAvctpData, U16 length);

#endif //_BT_AVCTP_H_
