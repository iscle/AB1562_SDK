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
 
 
#ifndef _APP_CONNECTION_H_
#define _APP_CONNECTION_H_


/**************************************************************************************************
* Definition
**************************************************************************************************/
#if 0 /* Unused LOG_PRINT_156X */
#define DBG_LOG_APP_Connection(_message, arg_cnt, ...)   printf(_message, ##__VA_ARGS__)
#else
#define DBG_LOG_APP_Connection(_message, arg_cnt, ...)   LOG_MSGID_I(APP_CONN, _message, arg_cnt, ##__VA_ARGS__)
#endif

#define TIMER_ID_POWER_OFF_IN_ROLE_CHANNGING_DELAY      3000

<<<<<<< HEAD
enum
{
	TIMER_ID_PARTNER_PLAY_SLC_CONNECTED_VP,
=======
#define TIMER_PARTNER_VP_IF_SLC_TIMER 5000
#define TIMER_PARTNER_AG_NUM_IF_SLC_TIMER 3000
enum
{
>>>>>>> db20e11 (second commit)
	TIMER_ID_SLC_CONNECTED_VP,
	TIMER_ID_DISCONNECTED_VP,
    TIMER_ID_CLEAR_LINK_KEY,
    TIMER_ID_WAIT_DISCONNECTED_CLEAR_LINK_KEY,
    TIMER_ID_POWER_OFF_IN_ROLE_CHANNGING,
    TIMER_ID_PIN_OR_KEY_MISSING_AWS_CHECK,
    TIMER_ID_QOS,
<<<<<<< HEAD
#ifdef DISCONNECT_AGENT_RECOVERY
    TIMER_ID_POWER_OFF_AWS_CHECK,
#endif
    TIMER_ID_ACL_LINK_TIMER,

=======
#ifdef AIR_AGENT_RECOVERY_MODE_AFTER_DISCONNECTION_FROM_PHONE_ENABLE
    TIMER_ID_POWER_OFF_AWS_CHECK,
#endif
    TIMER_ID_ACL_LINK_TIMER,
    TIMER_ID_DISCONNECT_SPECIAL_LINK,
    TIMER_ID_CHECK_TAKE_OVER_RECONNECT,
    TIMER_ID_TAKE_OVER_SLC_CONNECTED,
    TIMER_ID_AGENT_SLC_CONNECTED,
>>>>>>> db20e11 (second commit)
};

/**************************************************************************************************
* Public Functions (Connection API)
**************************************************************************************************/
void APP_RemoveServiceBdAddr(BD_ADDR_T *pBdAddr);
void APP_AddServiceBdAddr(BD_ADDR_T *pBdAddr);

void APP_Conn_ReleaseProfileLinkAndDetach(BD_ADDR_T *pBdAddr);
void APP_Conn_ReleaseMultiLink(void);//MMI_ReleaseMultiLink
void APP_Conn_ReleaseAWSLink(void);
BOOL APP_Conn_CheckProfileOnAllLinks(void);//MMI_CheckProfileOnAllLinks

void APP_Conn_CreateActiveProfileConnection(BD_ADDR_T *pBdAddr,U8 profilemask);
void APP_Conn_CancelCreateConnection(BD_ADDR_T *pBdAddr);

/**************************************************************************************************
* Public Functions (Notify)
**************************************************************************************************/
void APP_Conn_Register(void);

U8 APP_Conn_GetSupportDeviceNo(void);
void APP_Conn_SetSupportDeviceNo(BOOL isIncrease);

BOOL APP_Conn_PowerOffHandler(void);
void APP_Conn_DetachAllLinks(void);


void APP_Conn_NotifyPartnerMediaEvt(U8 oldNumOfAg, U8 numOfAG, U8 isA2dpPlaying);
BOOL APP_Conn_FillRhoData(void *pData);
BOOL APP_Conn_AssignRhoData(void *pData);
void APP_Conn_NewPartner(BD_ADDR_T *pOldAgentAddr, BD_ADDR_T *pNewAgentAddr);
U8 APP_Conn_GetMsgNumInQueue(void);

U8 APP_Conn_GetAgLinkState(void);

BOOL APP_Conn_IsProfilesReady(void);
void APP_Conn_AllowAllRoleConnSPInit(void);

U16 APP_Conn_CheckTimer(U16 timerId);
void APP_Conn_ReleaseTimer(U16 timerId);
void APP_Conn_SetTimer(U16 timerId, U32 timeout);


#endif //_APP_CONNECTION_H_
