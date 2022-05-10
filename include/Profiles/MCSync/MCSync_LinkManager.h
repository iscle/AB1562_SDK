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
#ifndef _MCSYNC_LINK_MANAGER_H_
#define _MCSYNC_LINK_MANAGER_H_

#include "bt.h"
#include "bt_aws_mce.h"
#include "PM_LinkManager.h"
#include "MCSync.h"

/*************************************************************************************************
* Define
**************************************************************************************************/
#if 0 /* Unused LOG_PRINT_156X */
#define DBG_LOG_PROFILE_MCSYNC(_message, arg_cnt, ...)	printf(_message, ##__VA_ARGS__)
#else
#define DBG_LOG_PROFILE_MCSYNC(_message, arg_cnt, ...)   LOG_MSGID_I(LIB_MCSYNC, _message, arg_cnt, ##__VA_ARGS__)
#endif

/*************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
	SPECIAL_LINK,
	NORMAL_LINK,
<<<<<<< HEAD
	#ifdef TAKE_OVER_LINK
=======
	#ifdef AIR_TAKE_OVER_LINK_ENABLE
>>>>>>> db20e11 (second commit)
	NORMAL_LINK2,
	#endif
	MCSYNC_MAX_LINK_NUM,
};

enum
{
	RHO_IDLE,
	RHO_STARTING,
<<<<<<< HEAD
=======
	RHO_CONTROLLER_STARTING,
>>>>>>> db20e11 (second commit)
	RHO_PARTNER_RECEIVE_PKT,
};
// 0:special, 1:normal 1, 2: normal 2
#define MAX_MCSYNC_LINK_IDX MCSYNC_MAX_LINK_NUM 

/*************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct
{
	BD_ADDR_T bdAddr;
	SINK sink;
	SINK urgent_sink;
	PROFILE_INFO profileInfo;
	U8 linkRole;
	U8 rhoState;
	BD_ADDR_T shareAgentBdAddr;
<<<<<<< HEAD
	#ifdef TAKE_OVER_LINK
=======
	#ifdef AIR_TAKE_OVER_LINK_ENABLE
>>>>>>> db20e11 (second commit)
	BOOL isLowPrioLink;
	#endif
}MCSYNC_LINK_STRU;

/*************************************************************************************************
* Public function
**************************************************************************************************/
void MCSYNC_SetLinkRole(MCSYNC_LINK_STRU *pLinkInfo, U8 role);
U8 MCSYNC_GetLinkRole(MCSYNC_LINK_STRU *pLinkInfo);
MCSYNC_LINK_STRU* MCSYNC_GetSpecialLink(void);
MCSYNC_LINK_STRU* MCSYNC_GetNormalLink(void);
BD_ADDR_T* MCSYNC_GetCurrentLinkBdAddr(void);
BD_ADDR_T* MCSYNC_GetSpecialLinkBdAddr(void);
BD_ADDR_T* MCSYNC_GetNormalLinkBdAddr(void);
MCSYNC_LINK_STRU* MCSYNC_GetLinkByBdAddr(BD_ADDR_T *pBdAddr);
MCSYNC_LINK_STRU* MCSYNC_GetLinkBySink(SINK sink);
void MCSYNC_ClearLinkInfo(MCSYNC_LINK_STRU * pLinkInfo);
void MCSYNC_SetLink(BD_ADDR_T *pBdAddr, SINK sink);
void MCSYNC_SetUrgentLink(BD_ADDR_T *pBdAddr, SINK sink);
SINK MCSYNC_GetLinkSink(MCSYNC_LINK_STRU *pLinkInfo);
void MCSYNC_SetLinkState(MCSYNC_LINK_STRU * pLinkInfo, U8 state);
U8 MCSYNC_GetLinkState(MCSYNC_LINK_STRU * pLinkInfo);
BOOL MCSYNC_IsWithPartner(BD_ADDR_T *pBdAddr);
void MCSYNC_SetRhoState(MCSYNC_LINK_STRU *pLinkInfo, U8 state);
U8 MCSYNC_GetRhoState(MCSYNC_LINK_STRU *pLinkInfo);

<<<<<<< HEAD
#ifdef TAKE_OVER_LINK
=======
#ifdef AIR_TAKE_OVER_LINK_ENABLE
>>>>>>> db20e11 (second commit)
MCSYNC_LINK_STRU *MCSYNC_GetHighPrioLink(void);
MCSYNC_LINK_STRU *MCSYNC_GetLowPrioLink(void);
void MCSYNC_SetLowPrioLink(BD_ADDR_T *pBdAddr);
void MCSYNC_ClearLowPrioLink(BD_ADDR_T *pBdAddr);
BOOL MCSYNC_CheckIsLowPrioLink(BD_ADDR_T *pBdAddr);
#endif

<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
void MCSYNC_SetShareAgentBdAddr(MCSYNC_LINK_STRU *pLinkInfo, BD_ADDR_T *pBdAddr);
BD_ADDR_T* MCSYNC_GetShareAgentBdAddr(MCSYNC_LINK_STRU *pLinkInfo);
#endif

#endif
