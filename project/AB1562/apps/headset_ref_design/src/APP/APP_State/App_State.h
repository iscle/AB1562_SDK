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
 
 
#ifndef _APP_STATE_H_
#define _APP_STATE_H_


/**************************************************************************************************
* Define
**************************************************************************************************/
#define APP_EXIT_IMGCALL_STATE				0x01
#define APP_EXIT_CALLACTIVE_STATE			0x04
#define APP_EXIT_OUTGOING_STATE				0x08

/**************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
	CMD_SYSTEM_INIT,
	CMD_SECURITY_MODE_COMPLETE,
	CMD_SYSTEM_OFF,

	CMD_SET_CONNECTABLE,
	CMD_SET_NON_CONNECTABLE,
	CMD_SET_NON_CONNECTABLE_ANYWAY,
	CMD_SET_RESUME_CONNECTABLE,
	
	CMD_SPECIAL_LIMITED_DISCOVERABLE,
	CMD_LIMITED_DISCOVERABLE,
	CMD_SET_NORMAL_DISCOVERABLE,
	CMD_SET_NON_DISCOVERABLE,
	CMD_SET_NON_DISCOVERABLE_ANYWAY,
	CMD_SET_RESUME_DISCOVERABLE,
};

enum
{
	TYPE_ONE_LINK, //add/remove on one link
	TYPE_ALL_LINK, //add/remove on all links
};

enum
{
	APP_OFF,							//0
	APP_CONDISCABLE,
	APP_CONNECTABLE,
	APP_CONNECTED,		
	APP_HFP_INCOMMING,	
	APP_HFP_OUTGOING,					//5
	APP_HFP_CALLACTIVE,
	APP_HFP_CALLACTIVE_WITHOUT_SCO,
	APP_HFP_CAIMG,		
	APP_HFP_CAOGG,		
	APP_HFP_CAMULTY,					//10
	APP_FAKEON, 	  
	APP_FAKEOFF,
	APP_DETACHING_LINK,
	APP_TEST_MODE,
	APP_FM,								//15
	APP_LINE_IN,
	APP_UPDATING,
	APP_UPDATED_OK,
	APP_UPDATED_FAIL,
	APP_VOICE_PROMPT_LANG_SELECT,		//20
	APP_AWS_PAIRING,
	APP_INQUIRY,
	APP_MP_STATE,
	APP_MP3,
	APP_STATE_RESERVED2,				//25
	APP_STATE_RESERVED3,
	APP_STATE_RESERVED4,
	APP_STATE_RESERVED5,
	APP_STATE_RESERVED6,
	APP_STATE_RESERVED7,				//30
	APP_TOTAL_STATE_NO
};

enum
{
	PREVENT_AUTO_POWER_ON_TIMER,
	DETACH_LINK_TIMER,
	APP_READY_SYSTEM_OFF_TIMER,
	WATCHDOG_REBOOT_NO_MEDIA_TIMER,
	RECONN_FAIL_ENTER_DISC_TIMER,
};

enum
{
	TIMER_ID_VP_SEL,
};

/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct
{
	void (*InitAction)(BD_ADDR_T *pBdAddr);
	void (*MsgHandler)(BD_ADDR_T *pBdAddr, APP_MSG_TYPE_STRU *pMsg);
	void (*ExitAction)(BD_ADDR_T *pBdAddr);
}APP_STATE_HANDLER_SET_TYPE_STRU;//MmiHandlerSetType;

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void APP_State_CancelTimer(U8 timerType);
void APP_State_SetTimer(U8 timerType, U32 time);
BOOL APP_State_CheckTimer(U8 timerType);
void APP_State_Init(void);//MMI_STATE_Init
void APP_State_SetAutoPowerOnTimer(void);
void APP_State_PassThruMsg(BD_ADDR_T *pBdAddr, APP_MSG_TYPE_STRU *pMsg);//MMI_STATE_PassThruMsg
U8 APP_State_GetTopState(BD_ADDR_T *pBdAddr);
void APP_State_AddTopState(BD_ADDR_T *pBdAddr, U8 state);//MMI_AddTopState
void APP_State_InsertState(BD_ADDR_T *pBdAddr, U8 state, U8 beforeState);//MMI_InsertState
void APP_State_RmvStateAllForce();
void APP_State_RmvState(BD_ADDR_T *pBdAddr, U8 state);//MMI_RmvState
void APP_State_ReplaceStateWithEntry(BD_ADDR_T *pBdAddr, U8 newState, U8 oldState, bool withEntry);//MMI_ReplaceStateWithEntry //MMI_ReplaceState
void APP_State_CleanNestState(BD_ADDR_T *pBdAddr);//MMI_CleanNestState
bool APP_State_CheckNestStateByLink(BD_ADDR_T *pBdAddr, U8 state);//MMI_CheckNestState, MMI_CheckNestStateByLink
bool APP_State_CheckLinkConnected(BD_ADDR_T *pBdAddr);
bool APP_State_CheckLinkInCallRelatedState(BD_ADDR_T *pBdAddr);//MMI_CheckLinkInCallRelatedState
BD_ADDR_T * APP_State_CheckOtherLinksInCallRelatedState(BD_ADDR_T *pBdAddr);
bool APP_State_AllLinksInCallRelatedState(void);//MMI_AllLinksInCallRelatedState
<<<<<<< HEAD
=======
bool APP_State_IsAnyLinkInSpecifiedState(U8 state);
>>>>>>> db20e11 (second commit)
bool APP_State_CheckLinkInCallActiveState(BD_ADDR_T *pBdAddr);//MMI_CheckLinkInCallActiveState
bool APP_State_CheckOtherLinksInCallActiveState(BD_ADDR_T *pBdAddr);
bool APP_State_AllLinksInCallActiveState(void);//MMI_AllLinksInCallActiveState
bool APP_State_CheckLinkInCallNotifyState(BD_ADDR_T *pBdAddr);//MMI_CheckLinkInCallNotifyState
bool APP_State_CheckLinkInOutGoingState(BD_ADDR_T *pBdAddr);//MMI_CheckLinkInOutGoingState
bool APP_State_CheckOtherLinksInOutGoingState(BD_ADDR_T *pBdAddr);
bool APP_State_CheckLinkInIncomingState(BD_ADDR_T *pBdAddr);//MMI_CheckLinkInIncomingState
BD_ADDR_T * APP_State_CheckOtherLinksInIncomingState(BD_ADDR_T *pBdAddr);
bool APP_State_CheckLinkInImgAndCaImgState(BD_ADDR_T *pBdAddr);//isHFPImgEntry
bool APP_State_CheckInFMState(void);//MMI_CheckInFMState
bool APP_State_CheckInLineInState(void);//MMI_CheckInLineInState
bool APP_State_CheckInMP3State(void);//MMI_CheckInMP3State
BOOL APP_State_IsInSystemAudioState(void);
void APP_State_ConnectabilityHandle(U8 action);//MMI_ConnectabilityHandle
void APP_State_DiscoverabilityHandle(U8 action);//MMI_DiscoverabilityHandle
U8 APP_State_CountConnectedDev(void);//MMI_CountConnectedDev
void APP_State_ConnectedEntry(BD_ADDR_T *pBdAddr);//MMI_ConnectedEntry
void APP_State_ConnectedExit(BD_ADDR_T *pBdAddr);//MMI_ConnectedExit
void APP_State_PowerOffHandle(void);
U8 APP_State_GetConnectedCount(void);
void APP_State_ReleaseTimerSystemOff(void);
void APP_State_SendMsgToHandler(BD_ADDR_T *pBdAddr, U16 msgCode, U16 evtCode, BOOL isSetMsgHabdled);


#endif //_APP_STATE_H_
