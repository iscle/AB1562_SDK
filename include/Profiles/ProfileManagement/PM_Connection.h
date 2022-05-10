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

#ifndef _PM_CONNECTION_H_
#define _PM_CONNECTION_H_

#include "PM_LinkManager.h"
#include "PM_Service.h"

#define PROFILE_DISCONNECT_TIME 1000


typedef enum
{
    PM_CONN_CONFLICT_SDP_CONN_NOT_ALLOW_EVT,
    PM_CONN_CONFLICT_SDP_CONN_EXIST_EVT,
    PM_CONN_CONFLICT_NOT_ALLOW_TIMER_EXPIRE_EVT,
    PM_CONN_CONFLICT_EXIST_TIMER_EXPIRE_EVT,
    PM_CONN_CONFLICT_ACL_OPEN_EVT,
    PM_CONN_CONFLICT_ENCRIPTION_CHANGE_EVT,
    PM_CONN_CONFLICT_ACL_IDLE_TIMER_EXPIRE_EVT,
    PM_CONN_CONFLICT_ACL_CLOSE_EVT,
    PM_CONN_CONFLICT_ACTIVE_RECONNECT_EVT,
    PM_CONN_CONFLICT_CLEAR_INFO_EVT,
}PM_CONN_CONFLICT_EVENT;


/**************************************************************************************************
* Public functions
**************************************************************************************************/
U8 PM_GetConnectedProfileCount(BD_ADDR_T * pBdAddr, BOOL connectedEvtOnly);
BOOL PM_IsProfileIdLoaded(BD_ADDR_T * pBdAddr, U8 profileId);
void PM_LoadProfile(BD_ADDR_T * pBdAddr, PROFILE_INFO * pProfileInfo, PROFILE_DESC const * profileDesc);
void PM_ActivateProfile(BD_ADDR_T * pBdAddr, PROFILE_INFO *  pProfileInfo);
void PM_RhoActivateProfile(BD_ADDR_T *pBdAddr, PROFILE_INFO *pProfileInfo);
void PM_UnloadProfile(BD_ADDR_T * pBdAddr, PROFILE_INFO *  pProfileInfo, U8 status);
void PM_RhoUnloadProfile(BD_ADDR_T *pBdAddr, PROFILE_INFO *pProfileInfo);
void PM_RegisterProfile(PROFILE_NODE * profileNode);
void PM_ConnectProfile(BD_ADDR_T *pBdAddr, U8 profileId);
void PM_ProfileDisconnected(BD_ADDR_T *pBdAddr);
U8 PM_DisconnectLoadedProfile(BD_ADDR_T *pBdAddr);
U8 PM_DisconnectSpecificProfile(BD_ADDR_T *pBdAddr, U8 profileId);
BOOL PM_IsProfileConnected(BD_ADDR_T *pBdAddr, U8 profileId);
BOOL PM_IsAllProfileDisconnected(BD_ADDR_T *pBdAddr);
void PM_RegisterApp(Handler app);
void PM_SetMaximumEDRLinkNum(U8 num);
void PM_SetProfileRegisterState(BOOL isRegistering, U8 regProfileId);
void PM_Conn_SecurityEventIndHandler(BT_SM_SECURITY_EVENT_IND_T *ind);
void PM_Conne_GetRoleHandler(BT_GET_ROLE_CFM_T *cfm);
void PM_Conne_SetRoleHandler(BT_SET_ROLE_CFM_T *cfm);
U8 PM_Conn_GetLinkState(BD_ADDR_T *pBdAddr);
U8 PM_Conn_GetAgLinkState(void);
BOOL PM_Conn_CheckTimer(PM_INFO_STRU *pLinkInfo, U16 id);
void PM_ConnectionConflictStateMachine(BD_ADDR_T *pBdAddr, PM_CONN_CONFLICT_EVENT evt);
void PM_Conn_ConflictClearStateInfo(void);
void PM_DeviceConnectedInd(BD_ADDR_T *pBdAddr);
<<<<<<< HEAD
U8 pm_connection_get_block_role_slave(void);
void pm_connection_set_block_role_slave(bool is_block);
=======

U8 PM_Conn_GetBlockRoleSlave(void);
void PM_Conn_SetBlockRoleSlave(bool is_block);
BOOL PM_IsProfileInfoLoaded(BD_ADDR_T * pBdAddr, PROFILE_INFO *  pProfileInfo);


>>>>>>> db20e11 (second commit)

#endif //_PM_CONNECTION_H_
