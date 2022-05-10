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
 
 
#ifndef _PM_SDP_CLIENT_CENTER_H_
#define _PM_SDP_CLIENT_CENTER_H_

#include "PM_Message.h"

/**************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
	SDP_OPEN_CHANNEL_CFM = MSG_MESSAGE_BT_SDP,
	SDP_CLOSE_CHANNEL_CFM,
	
	SDP_INTERNAL_SERVICE_SEARCH_MSG,
	SDP_INTERNAL_ATTRIBUTE_SEARCH_MSG,
	SDP_INTERNAL_SERVICE_SEARCH_ATTRIBUTE_MSG,
};

/**************************************************************************************************
* Structure
**************************************************************************************************/	
typedef struct SDP_OPEN_CHANNEL_CFM_T
{
	BT_GENERIC_STATUS_E status;
	BD_ADDR_T bdAddr;
}SDP_OPEN_CHANNEL_CFM_T;

typedef struct SDP_CLOSE_CHANNEL_CFM_T
{
	BT_GENERIC_STATUS_E status;
	BD_ADDR_T bdAddr;
}SDP_CLOSE_CHANNEL_CFM_T;


typedef struct SDP_INTERNAL_SERVICE_SEARCH_MSG_T
{
	BD_ADDR_T bdAddr;
	Handler app;
	BT_UUID_T uuid;
}SDP_INTERNAL_SERVICE_SEARCH_MSG_T;

typedef struct SDP_INTERNAL_ATTRIBUTE_SEARCH_MSG_T
{
	BD_ADDR_T bdAddr;
	Handler app;
	U32 serviceRecordHandle;
	U16 startAttId;
	U16 endAttId;
}SDP_INTERNAL_ATTRIBUTE_SEARCH_MSG_T;

typedef struct SDP_INTERNAL_SERVICE_SEARCH_ATTRIBUTE_MSG_T
{
	BD_ADDR_T bdAddr;
	Handler app;
	BT_UUID_T uuid;
	U16 startAttId;
	U16 endAttId;
}SDP_INTERNAL_SERVICE_SEARCH_ATTRIBUTE_MSG_T;
	
/**************************************************************************************************
* Public functions
**************************************************************************************************/
void PM_SDPClient_OpenChannel(Handler app, BD_ADDR_T *pBdAddr);
void PM_SDPClient_CloseChannel(Handler app, BD_ADDR_T *pBdAddr);
void PM_SDPClient_ServiceSearch(Handler app, BD_ADDR_T *pBdAddr, BT_UUID_T uuid);
void PM_SDPClient_AttributeSearch(Handler app, BD_ADDR_T *pBdAddr, U32 serviceRecordHandle, U16 startAttId, U16 endAttId);
void PM_SDPClient_ServiceSearchAttribute(Handler app, BD_ADDR_T *pBdAddr, BT_UUID_T uuid, U16 startAttId, U16 endAttId);

#endif//_PM_SDP_CLIENT_CENTER_H_
