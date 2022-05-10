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


#ifndef _PM_SNIFF_HANDLER_H_
#define _PM_SNIFF_HANDLER_H_

#include "bt.h"
#include "PM_Message.h"
#include "message.h"

/**************************************************************************************************
* Definition
**************************************************************************************************/
#define PM_SNIFF_HFP						0
#define PM_SNIFF_A2DP_CONTROL				1
#define PM_SNIFF_AVRCP						2
#define PM_SNIFF_A2DP_MUSIC					3
#define PM_SNIFF_SDP						4
#define PM_SNIFF_DISC						5
#define PM_SNIFF_SPP						6
#define PM_SNIFF_AWSMCE						7
#define PM_SNIFF_ROLE_SWITCH				8
#define PM_SNIFF_AVDTP_SIGNAL				9
#define PM_SNIFF_SOUNDBAR_EST_SCO			10
#define PM_FORCE_UNSNIFF					11
#define PM_SNIFF_OPP						12
#ifdef HID_Profile
#define PM_SNIFF_HID						13
#endif
#define PM_SNIFF_OTA						14
#define PM_SNIFF_APP_LAYER					15
#define PM_SNIFF_RHO						16
#define PM_SNIFF_AMA						17

/**************************************************************************************************
* Enum
**************************************************************************************************/

enum
{
	PM_ACTIVE_STATE,
	PM_SNIFF_STATE,
	PM_SNIFFING_STATE,
	PM_UNSNIFFING_STATE,
	PM_INVALID_STATE,
};

/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct
{
    HandlerData HandlerData;
	const MSG_HANDLER_ENTRY_T *table;
}PM_SNIFF_HANDLER_STRU;

typedef struct
{
	U8 isUsed;
	BD_ADDR_T bdAddr;
	U32 sniffProfileMask;
	U8 sniffCount;
	HCI_STATUS_E status;
	U8 sniffStateMachine;
}PM_SNIFF_INFO_STRU;

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
BOOL PM_Sniff_Enable(BD_ADDR_T *pBdAddr, U32 sniffReason);
BOOL PM_Sniff_Disable(BD_ADDR_T *pBdAddr, U32 sniffReason);
void PM_Sniff_ClearTimer(PM_SNIFF_INFO_STRU *pLinkInfo);
void PM_Sniff_SetModeCfmHandler(BT_SET_MODE_CFM_T *cfm);
void PM_Sniff_ModeChangeIndHandler(BT_MODE_CHANGE_IND_T * ind);
BOOL PM_IsInSniff(BD_ADDR_T *pBdAddr);
BOOL PM_IsInActiveOrEmptyLink(BD_ADDR_T *pBdAddr);
BOOL PM_IsInActive(BD_ADDR_T *pBdAddr);
BOOL PM_IsSniffingOrUnsniffingState(BD_ADDR_T *pBdAddr);
BOOL PM_Sniff_CheckTimer(PM_SNIFF_INFO_STRU *pLinkInfo, U16 id);
void PM_Sniff_SetTimer(PM_SNIFF_INFO_STRU *pLinkInfo, U16 id);
<<<<<<< HEAD

=======
void PM_Sniff_SetLinkPolicyDisableAllMode(BD_ADDR_T *pBdAddr);
>>>>>>> db20e11 (second commit)
#endif
