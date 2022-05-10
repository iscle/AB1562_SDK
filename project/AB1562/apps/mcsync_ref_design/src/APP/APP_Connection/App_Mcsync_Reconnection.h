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


#ifndef _APP_MCSYNC_RECONNECTION_H_
#define _APP_MCSYNC_RECONNECTION_H_


/**************************************************************************************************
* Enumeration
**************************************************************************************************/
#define MCS_RECONN_MASK_CHARGER_CASE	0x01
#define MCS_RECONN_MASK_PARTNER_DISC	0x02
#define MCS_RECONN_MASK_AG_DISC			0x04


#if SUPER_RECONNECT_MODE
/**************************************************************************************************
* Enumeration
**************************************************************************************************/
enum
{
	MCS_RECONN_DEVICE_AGENT,
	MCS_RECONN_DEVICE_PARTNER,
	MCS_RECONN_DEVICE_AG,
};

enum
{
	MCS_RECONN_START_ORIGINAL,
	MCS_RECONN_START_AGENT,
	MCS_RECONN_START_PARTNER,
};

#endif //SUPER_RECONNECT_MODE

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
BOOL APP_Mcsync_EnterSuperReconnectMode(U8 type);
BOOL APP_Mcsync_IsInSuperReconnectMode(void);
void APP_Mcsync_ReconnectNotify(U8 connectDev);
void APP_Mcsync_ReconnectFailedNotify(U8 reason);
void APP_Mcsync_SetRoleNotify(U8 status);
void APP_Mcsync_SetReconnStartRole(U8 role);
void APP_Mcsync_ClearReconnStartRole(void);
void APP_Mcsync_ExitSuperReconnectMode(BOOL isNeedBackRole);
void APP_Mcsync_SetLinkLossBdAddr(BD_ADDR_T *pBdAddr);
void APP_Mcsync_ClearLinkLossBdAddr(void);
void APP_Mcsync_ReconnectInit(void);
BOOL APP_Mcsync_ReconnectIsFeatureOn(void);
void APP_Mcsync_SetNotReconnMask(U8 mask);
U8 APP_Mcsync_GetNotReconnMask(void);
void APP_Mcsync_ClearNotReconnMask(U8 mask);
void APP_Mcsync_ClearAllNotReconnMask(void);
void APP_Mcsync_SyncNotReconnMaskIndHandler(U8 mask);
void APP_Mcsync_ClearSuperReconnTimer(void);
BOOL APP_Mcsync_IsReconnectStateWaitToPartner(void);
void APP_Mcsync_SetReconnectLater(U8 type, U32 time);
<<<<<<< HEAD
#ifdef DISCONNECT_AGENT_RECOVERY
=======
#ifdef AIR_AGENT_RECOVERY_MODE_AFTER_DISCONNECTION_FROM_PHONE_ENABLE
>>>>>>> db20e11 (second commit)
U16 app_mcsync_reconnect_check_super_reconnect_timer(U32 reconnect_type);
#endif

#endif //_APP_MCSYNC_RECONNECTION_H_
