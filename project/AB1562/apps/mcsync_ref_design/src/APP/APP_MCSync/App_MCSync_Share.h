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


#ifndef _APP_MCSYNC_PLUS_H_
#define _APP_MCSYNC_PLUS_H_

<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)

#define SHARE_MODE_SHARE_AGENT 		0x01
#define SHARE_MODE_FOLLOWER_AGENT 	0x02
#define	SHARE_MODE_NONE				0xFF
/*************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
    APP_SHARE_AGENT_EVENT_PARTNER_CONNECTED_CFM = 1,
};

enum
{
<<<<<<< HEAD
=======
    APP_SHARE_STATE_NONE,
    APP_SHARE_STATE_PREPARING,
    APP_SHARE_STATE_ENTERED_MODE,
    APP_SHARE_STATE_LEAVING,
    APP_SHARE_STATE_SHARE_PAIRING_START,
    APP_SHARE_STATE_PARTNER_GET_PAIRING_DATA,
};

enum
{
    APP_SHARE_SUBSTATE_NONE,
    APP_SHARE_SUBSTATE_WAITING_CFM,
    APP_SHARE_SUBSTATE_WAITING_TIMEOUT,
};

enum
{
>>>>>>> db20e11 (second commit)
	APP_SHARE_PARTNER_EVENT_SYNC_INFO,
	APP_SHARE_PARTNER_EVENT_MCSYNC_DISCONNECT_IND,
	APP_SHARE_PARTNER_EVENT_ACL_CLOSE,
};
 
enum
{
	APP_SHARE_FOLLOWER_EVENT_NON_CONNECTABLE,
	APP_SHARE_FOLLOWER_EVENT_ACL_CLOSE,
	APP_SHARE_FOLLOWER_EVENT_ACL_OPEN,
	APP_SHARE_FOLLOWER_EVENT_MCSYNC_CONNECT_CFM,
	APP_SHARE_FOLLOWER_EVENT_AG_CONNECT_CFM, //  4
	APP_SHARE_FOLLOWER_EVENT_MCSYNC_DISCONNECT_IND,
	APP_SHARE_FOLLOWER_EVENT_MCSYNC_WITH_PARTNER, //6
    
    APP_SHARE_FOLLOWER_EVENT_LEAVE_MODE_IND, //from agent
};

<<<<<<< HEAD
=======
enum
{
    APP_SHARE_MODE_OFF,
    APP_SHARE_MODE_FOLLOWER,
    APP_SHARE_MODE_AGENT_PARTNER,
};
>>>>>>> db20e11 (second commit)


/*************************************************************************************************
* Public function
**************************************************************************************************/
void APP_MCSync_Share_Init(void);
void APP_MCSync_Share_KeyFollowerStart(void);
void APP_MCSync_Share_KeyFollowerStop(void);
void APP_MCSync_Share_KeyAgentStart(void);
void APP_MCSync_Share_KeyAgentStop(void);
void APP_MCSync_Share_KeySharePairingStart(void);
void APP_MCSync_Share_KeySharePairingStop(void);


void APP_MCSync_Share_EnterMode(BOOL isFollow);
void APP_MCSync_Share_ExitMode(void);
void APP_MCSync_Share_SetMode(U8 mode);
U8   APP_MCSync_Share_IsEnable(void);
void APP_MCSync_Share_RhoAssign(U8 mode);


void APP_MCSync_Share_AgentEventHandler(U8 event);
void APP_MCSync_Share_PartnerEventHandler(U8 event, U8 *pMsg);
BOOL APP_MCSync_Share_FollowerEventHandler(U8 event, BD_ADDR_T *pBdAddr, U8 *pMsg);
U8   APP_MCSync_Share_StatusChangeHandler(BD_ADDR_T *pBdAddr, U8 role);
void APP_MCSync_Share_InquiryParaIndHandler(S8 rssi, BD_ADDR_T *pBdAddr);
void APP_MCSync_Share_WriteInquiryModeCfmHandler(U8 status);
void APP_MCSync_Share_WriteCurentIACCfmHandler(U8 status);
void APP_MCSync_Share_SyncFollowerDiscStatusHandler(U8 isFollowerIFPkt, U8 isOnlyForFollower);
void APP_MCSync_Share_SyncOtherAgentStatusHandler(BD_ADDR_T *pBdAddr);
void APP_MCSync_Share_SyncAgentRetransmitDataHandler(U8 *msg);


BOOL APP_MCSync_Share_IsKeyNeedSync(U16 keyEventCode);
BOOL APP_MCSync_Share_IsKeyEventAllowed(U16 keyEventCode);


BOOL APP_MCSync_Share_IsGetIFPacketAllowed(U8 event);
void APP_MCSyncPlus_FollowerStop(void);
U8 APP_MCSync_Share_FollowerFilter(BD_ADDR_T *pBdAddr, U8 role);

void APP_MCSync_Share_SetLinkLoss(U8 isValue);

U8 APP_MCSync_Share_GetLinkLoss(void);
<<<<<<< HEAD
=======
U8 APP_MCSync_Share_GetState(void);
bool APP_MCSync_IsShareConn(void);
>>>>>>> db20e11 (second commit)


#endif

#endif
