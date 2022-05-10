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

#ifndef _MCSYNC_H_
#define _MCSYNC_H_


#include "bt.h"


/*************************************************************************************************
* Definition
**************************************************************************************************/
#define MCSYNC_INQUIRY_FEATURE

/*************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
	PICO_EVENT_START = 0,
	PICO_EVENT_LED,
	PICO_EVENT_PEQ,
	PICO_EVENT_NUM
};

#if SINGLE_HEADSET_MODE_ENABLE
enum
{
	NORMAL_HEADSET_MODE,
	SINGLE_CONVERTING_MODE,
	SINGLE_HEADSET_MODE,
};
#endif

enum
{
	AIR_PAIRING_STATE_NONE,
	AIR_PAIRING_STATE_PREPARE,
	AIR_PAIRING_STATE_READY,
};

enum
{
	MCSYNC_LINK_SAWS_DISCONNECTED,
	MCSYNC_LINK_SAWS_CONNECTING,
	MCSYNC_LINK_SAWS_DISCONNECTING,
	MCSYNC_LINK_SAWS_READY,
	MCSYNC_LINK_DISCONNECTED = MCSYNC_LINK_SAWS_READY,
	MCSYNC_LINK_CONNECTING,
	MCSYNC_LINK_CONNECTED, //5
	MCSYNC_LINK_DISCONNECTING,
};

<<<<<<< HEAD
=======
#ifdef AIR_TAKE_OVER_LINK_ENABLE
>>>>>>> db20e11 (second commit)
enum
{
	MCSYNC_TAKE_OVER_CONNECTION_IDLE_STATE,
	MCSYNC_TAKE_OVER_CONNECTION_DISCONNECTING_LOW_PRIO_LINK_AWS_STATE,
	MCSYNC_TAKE_OVER_CONNECTION_DISCONNECTED_LOW_PRIO_LINK_AWS_STATE,
	MCSYNC_TAKE_OVER_CONNECTION_DISCONNECTING_LOW_PRIO_LINK_ACL_STATE,
	MCSYNC_TAKE_OVER_CONNECTION_DISCONNECTING_LOW_PRIO_LINK_PROFILE_STATE,
	MCSYNC_TAKE_OVER_CONNECTION_DISCONNECTED_LOW_PRIO_COMPLETE_STATE_AND_CONNECT_HIGH_PRIO_AWS, //5
	MCSYNC_TAKE_OVER_CONNECTION_SWITCH_SPECIAL_TO_HIGH_PRIO_LINK_STATE,
<<<<<<< HEAD
};
=======
	MCSYNC_TAKE_OVER_CONNECTION_ADD_HIGH_PRIO_LINK_TO_SERVICE_STATE,
	MCSYNC_TAKE_OVER_CONNECTION_TAKE_OVER_COMPLETE_STATE,
};
#endif
>>>>>>> db20e11 (second commit)

enum
{
	ROLE_MODE_CHANGE_STATE_NONE,
	ROLE_MODE_CHANGE_STATE_TO_PARTNER_DISABLE_LS,
	ROLE_MODE_CHANGE_STATE_TO_PARTNER_DISCONNECT_SAWS,
	ROLE_MODE_CHANGE_STATE_TO_PARTNER_CHANGE_BDADDR,

	ROLE_MODE_CHANGE_STATE_TO_AGENT_DISCONNECT_SAWS,
	ROLE_MODE_CHANGE_STATE_TO_AGENT_CHANGE_BDADDR,
	ROLE_MODE_CHANGE_STATE_TO_AGENT_CONNECT_SAWS
};

enum
{
	SET_ROLE_SLAVE_NONE,
	SET_ROLE_SLAVE_FAIL,
};

enum
{
	MCSYNC_SHARE_PAIRING_STATE_INIT,
	MCSYNC_SHARE_PAIRING_STATE_START,
};

<<<<<<< HEAD
=======
enum
{
	MCSYNC_PARTNER_SET_TAKE_OVER_STATE,
};
>>>>>>> db20e11 (second commit)
/*************************************************************************************************
* Public function
**************************************************************************************************/
void 		MCSYNC_SetHandle(Handler app);
void 		MCSYNC_SetServiceState(U8 serviceState);
U8 			MCSYNC_GetServiceState(void);
Handler 	MCSYNC_GetAppHandler(void);
BOOL		MCSYNC_IsMCSyncRegistered(void);
BOOL		MCSYNC_IsDefaultRoleAgent(void);
BOOL 		MCSYNC_IsDefaultRolePartner(void);
BOOL 		MCSYNC_IsDefaultRoleNone(void);
void		MCSYNC_SetIsRoleChanging(BOOL value);
BOOL		MCSYNC_IsRoleChanging(void);
#ifdef MCSYNC_INQUIRY_FEATURE
void 		MCSYNC_SetBestRSSIAddr(BD_ADDR_T *pBdAddr);
BD_ADDR_T* 	MCSYNC_GetBestRSSIAddr(void);
#endif
void 		MCSYNC_SetSlaveRetryCount(U8 rtyCnt);
U8		 	MCSYNC_GetSlaveRetryCount(void);
void 		MCSYNC_PushPicoBtClockEvent(U8 event);
U8 			MCSYNC_PopPicoBtClockEvent(void);
void		MCSYNC_SetMpTestMode(BOOL isMpTestMode);
BOOL		MCSYNC_IsMpTestMode(void);
<<<<<<< HEAD
=======
BOOL MCSYNC_IsDiscoverableEnabled(void);
void MCSYNC_SetDiscoverableEnable(BOOL isEnable);
>>>>>>> db20e11 (second commit)
#if SINGLE_HEADSET_MODE_ENABLE
U8			MCSYNC_GetSingleHSModeState(void);
BOOL		MCSYNC_IsSingleHSMode(void);
void		MCSYNC_SetSingleHSModeState(U8 state);
#endif
BOOL		MCSYNC_IsInAirPairing(void);
U8			MCSYNC_GetAirPairingState(void);
void		MCSYNC_SetAirPairingState(U8 state);
void		MCSYNC_SetMcsyncState(U8 state);
U8			MCSYNC_GetMcsyncState(void);
<<<<<<< HEAD
#ifdef TAKE_OVER_LINK
=======
#ifdef AIR_TAKE_OVER_LINK_ENABLE
>>>>>>> db20e11 (second commit)
void 		MCSYNC_SetMcsyncTakeOverConnState(U8 state);
U8 			MCSYNC_GetMcsyncTakeOverConnState(void);
#endif
U8			MCSYNC_GetRoleRecoveryMode(void); //?
void		MCSYNC_ClearRoleRecoveryMode(void);
void 	MCSYNC_SetCurrentRole(U8 role);
U8	 	MCSYNC_GetCurrentRole(void);
BOOL 	MCSYNC_IsCurrentRoleAgent(void);
BOOL 	MCSYNC_IsCurrentRolePartner(void);
void	MCSYNC_SetRoleModeChangeState(U8 state);
U8		MCSYNC_GetRoleModeChangeState(void);

void mcsync_set_dsp_sync_timer(U32 clockToStart, U8 *pData, U32 length);
void mcsync_dsp_init(void);

<<<<<<< HEAD
=======
U16 MCSYNC_CheckTimer(U16 timerId);
void MCSYNC_ReleaseTimer(U16 timerId);
void MCSYNC_SetTimer(U16 timerId, U32 timeout);


>>>>>>> db20e11 (second commit)
#endif //_MCSYNC_H_
