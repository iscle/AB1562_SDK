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
 
#ifndef _GFP_BT_H_
#define _GFP_BT_H_

#include "bt.h"
#include "stream.h"
#include "PM_Connection.h"

#define GFP_CHANNEL		7
#define MSG_MESSAGE_BT_PROFILE_GFP	MSG_MESSAGE_BT_PROFILE_BASE+0xA000

typedef struct GFP_LINK_STRU 
{
	BD_ADDR_T bdAddr;
	PROFILE_INFO profileInfo;
	U8 remoteScn;
	U8 serverChannel;
	SINK sink;
    U8 notDrop:1;
    U8 handleRxDropByUpperLayer:1;
}GFP_LINK_STRU;

typedef enum
{
	GFP_START_SERVICE_CFM = MSG_MESSAGE_BT_PROFILE_GFP,
	GFP_STOP_SERVICE_CFM,
	GFP_CONNECT_CFM,
	GFP_DISCONNECT_CFM,
	GFP_RX_DATA_IND,
	GFP_TX_AVAILABLE_IND,
}GFP_MSG_E;

typedef struct
{
	BT_GENERIC_STATUS_E status;
}GFP_START_SERVICE_CFM_T;

typedef struct
{
	BT_GENERIC_STATUS_E status;
}GFP_STOP_SERVICE_CFM_T;

typedef struct GFP_CONNECT_CFM_T
{
	BT_GENERIC_STATUS_E status;
	BD_ADDR_T bdAddr;
}GFP_CONNECT_CFM_T;

typedef struct GFP_DISCONNECT_CFM_T
{
	BT_GENERIC_STATUS_E status;
	BD_ADDR_T bdAddr;
}GFP_DISCONNECT_CFM_T;

typedef struct GFP_RX_DATA_IND_T
{
	BD_ADDR_T bdAddr;
	U16 length;
	U8 data[1];
}GFP_RX_DATA_IND_T;

typedef struct GFP_TX_AVAILABLE_IND_T
{
	BD_ADDR_T bdAddr;
}GFP_TX_AVAILABLE_IND_T;


void Gfp_RegisterRfcommChannel(void);
void Gfp_UnregisterRfcommChannel(void);
void Gfp_RfcommConnectRequest(GFP_LINK_STRU *pLinkInfo);
void Gfp_RfcommDisconnectRequest(GFP_LINK_STRU *pLinkInfo);

void Gfp_StartService(Handler app, U8 ctrlChannel);
void Gfp_StopService(Handler app);
BOOL Gfp_DisconnectRequest(BD_ADDR_T *pBdAddr);

BOOL Gfp_TxData(BD_ADDR_T *pBdAddr, U16 length, const U8 *pData);
U16 Gfp_GetMtu(BD_ADDR_T *pBdAddr);
BOOL Gfp_RxDone(BD_ADDR_T *pBdAddr, U16 length);

BOOL Gfp_RfcommRxDone(GFP_LINK_STRU *pLinkInfo, U16 length);
//BOOL GFP_SetRxDropByUpperLayer(BD_ADDR_T *pBdAddr, BOOL enable);

#endif //_GFP_BT_H_
