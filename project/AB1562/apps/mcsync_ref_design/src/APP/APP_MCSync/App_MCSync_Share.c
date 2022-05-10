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

#include "bt.h"
#include "App_MCSync_Share.h"
#include "bt_aws_mce.h"
#include "BtMCSync.h"
#include "App_MCSync_Message.h"
#include "App_MCSync_NvkeyHandler.h"
#include "App_LinkKey.h"
#include "APP_HfpScoHandler.h"
#include "App_ResourceManager.h"
#include "App_Mcsync_Reconnection.h"
#include "App_SmartChgCase.h"
#include "App_ReConnection.h"
#include "App_Pairing.h"
#include "App_EventOpcode.h"
#include "App_PowerOff.h"
#include "Power_NvkeyHandler.h"
#include "App_A2dp.h"
#include "App_Media.h"
#include "App_MCSync_State.h"
#include "App_MCSync_System.h"
#include "App_System.h"
#include "AudioDSP_StreamManager.h"
#include "App_MCSync_RHO.h"
#include "drv_a2dp.h"
#include "App_Nvkey_KeymapHandler.h"
#include "app_mcsync_share_mode_nvstruc.h"
<<<<<<< HEAD


#ifdef MCSYNC_SHARE_MODE
=======
#include "Pairing_NvkeyHandler.h"
#include "App_State.h"



#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
/*************************************************************************************************
* Define
**************************************************************************************************/
#define	SHARE_FOLLOWER_NUMBER_1_FILTER	0x04
#define SHARE_FOLLOWER_NUMBER_2_FILTER	0x08

enum
{
	APP_SHARE_MODE_ROLE_NONE,
	APP_SHARE_MODE_ROLE_AGENT_TO_AGENT,
	APP_SHARE_MODE_ROLE_AGENT_TO_FOLLOWER,
	APP_SHARE_MODE_ROLE_PARTNER_TO_FOLLOWER,
	APP_SHARE_MODE_ROLE_INVALID,
};

enum
{
	APP_SHARE_TIMER_TO_CONNECT_FOLLOWER,
	APP_SHARE_TIMER_FOLLOWER_DISCONNECT,
	APP_SHARE_TIMER_FOLLOWER_RECONNECT,
	APP_SHARE_TIMER_SHARE_AGENT_RECONNECT,
	APP_SHARE_TIMER_TO_STOP_AGENT,
	APP_SHARE_TIMER_IN_SHARE_PAIRING_MODE,
	APP_SHARE_TIMER_PUSH_SHARE_PAIRING_MODE_MEDIA,
	APP_SHARE_TIMER_FOLLOWER_DISC_TIMER,
	APP_SHARE_TIMER_AGENT_SEND_FOLLOWER_DISC_IF_REPEAT_TIMER,
	APP_SHARE_TIMER_AGENT_NOTICE_FOLLOWER_TO_LEAVE_REPEAT_TIMER,
<<<<<<< HEAD
=======
	APP_SHARE_TIMER_PAGE_TIMEOUT_RETRY
>>>>>>> db20e11 (second commit)
};

enum
{
	APP_SHARE_AGENT_RETRANSMIT_FOLLOWER_DISC_IF = 1,
};

enum
{
    APP_SHARE_ACTION_NONE,
    APP_SHARE_ACTION_AGENT_START = 1,
    APP_SHARE_ACTION_AGENT_STOP,
    APP_SHARE_ACTION_SHARE_PAIRING,
    APP_SHARE_ACTION_FOLLOWER_START,
};

enum
{
<<<<<<< HEAD
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
=======
>>>>>>> db20e11 (second commit)
	FOLLOWER_1_CONNECTED,
	FOLLOWER_2_CONNECTED,
	FOLLOWER_1_DISCONNECTED,
	FOLLOWER_2_DISCONNECTED
};

<<<<<<< HEAD
enum
{
    APP_SHARE_MODE_OFF,
    APP_SHARE_MODE_FOLLOWER,
    APP_SHARE_MODE_AGENT_PARTNER,
};
=======

>>>>>>> db20e11 (second commit)

/*************************************************************************************************
* Prototype
**************************************************************************************************/
static void app_MCSync_Share_SetRole(U8 role);
static U8   app_MCSync_Share_GetRole(void);
static void app_MCSync_Share_SetState(U8 state, U8 subState);
<<<<<<< HEAD
static U8   app_MCSync_Share_GetState(void);
=======
>>>>>>> db20e11 (second commit)
static void app_MCSync_Share_SetFollowerConnectionTO(U8 isConnectTimeout);
static U8   app_MCSync_Share_GetFollowerConnectionTO(void);

static void app_MCSync_Share_AgentDoNextAction(void);

static void app_MCSync_Share_WriteSharePairingLIAC(void);
static void app_MCSync_Share_SharePairingInquiry(void);
static void app_MCSync_Share_SetSharePairingInfo(S8 rssi, BD_ADDR_T* pBdAddr);
static BD_ADDR_T* app_MCSync_Share_GetSharePairingInfo(void);
static void app_MCSync_Share_ResetPairingTimer(void);



static U32  app_MCSync_Share_TimerHandler(Handler handler, U16 id, void *msg, U32 handler_id);
static U32  app_MCSync_Share_MsgHandler(Handler handler, U16 id, void *msg, U32 handler_id);

static void app_MCSync_Share_SetFollowerConnected(U8 event);
static void app_MCSync_Share_FollowerBackNormalState(void);

static void app_MCSync_Share_StartSharePairing(void);
static void app_MCSync_Share_StopSharePairing(void);

static void app_MCSync_Share_AgentClearCtl(void);

static void app_MCSync_Share_StartAgentConnection(void);
static void app_MCSync_Share_StopAgentConnection(void);
static void app_MCSync_Share_StartFollowerConnection(void);
static void app_MCSync_Share_StopFollowerConnection(void);

<<<<<<< HEAD
static void app_MCSync_Share_AgentStart(void);
static void app_MCSync_Share_AgentStop(void);
static void app_MCSync_Share_FollowerStart(void);
=======
static void app_MCSync_Share_AgentStart(U8 isTriggerByUser);
static void app_MCSync_Share_AgentStop(void);
static void app_MCSync_Share_FollowerStart(U8 isTriggerByUser);
>>>>>>> db20e11 (second commit)
static void app_MCSync_Share_FollowerStop(void);
static void app_MCSync_Share_ClearCtl(void);

/**************************************************************************************************
* Variable
**************************************************************************************************/
static HandlerData gAppMCSyncShareHandle = { app_MCSync_Share_MsgHandler };
static HandlerData gAppMCSyncShareTimerHandle = { app_MCSync_Share_TimerHandler };

static U8 gAppIsShareModeOn;

extern HandlerData gAppMCSyncSystemHandle;
extern U8 g_start_share_mode;

/*************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct
{
	BD_ADDR_T otherAgentAddr;
	S8 rssi;
	U8 isFoundRssi:1;
	U8 reserved:7;
}SHARE_PAIRING_STRU;

typedef struct
{
    U8 action;
	U8 state;
    U8 subState;
	U8 shareRole;
	U8 countOfFollowerDiscIF;
	BOOL follower1Connected;
	BOOL follower2Connected;

	BOOL followerConnectionTimerout;

	SHARE_PAIRING_STRU pairingInfo;
	U8 splinkloss;
	U8 noticeFollowerToLeaveNum;

}APP_SHARE_MODE_CTRL_STRU;

/*************************************************************************************************
* Variables
**************************************************************************************************/
static APP_SHARE_MODE_CTRL_STRU gAppShareModeCtrl;

/*************************************************************************************************
* Static function
**************************************************************************************************/
static void app_MCSync_Share_SetRole(U8 role)
{
	if(role > APP_SHARE_MODE_ROLE_PARTNER_TO_FOLLOWER)
		return;
	
	DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Set share role: %d -> %d", 2, gAppShareModeCtrl.shareRole, role);
	
	gAppShareModeCtrl.shareRole = role;
}

static U8 app_MCSync_Share_GetRole(void)
{	
	DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Get share role: %d ", 1, gAppShareModeCtrl.shareRole);
	
	return gAppShareModeCtrl.shareRole;
}

static void app_MCSync_Share_SetState(U8 state, U8 subState)
{
	DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Set State: %d -> %d; subState:%d -> %d", 4, gAppShareModeCtrl.state, state, gAppShareModeCtrl.subState, subState);
	gAppShareModeCtrl.state = state;
    gAppShareModeCtrl.subState = subState;
}

<<<<<<< HEAD
static U8 app_MCSync_Share_GetState(void)
{
	return gAppShareModeCtrl.state;
}

=======
>>>>>>> db20e11 (second commit)
static U8 app_MCSync_Share_GetSubState(void)
{
	return gAppShareModeCtrl.subState;
}

static void app_MCSync_Share_SetAction(U8 action)
{
    DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Set Action: %d -> %d", 2, gAppShareModeCtrl.action, action);
	gAppShareModeCtrl.action = action;
}

static U8 app_MCSync_Share_GetAction(void)
{
	return gAppShareModeCtrl.action;
}

static void app_MCSync_Share_SetFollowerConnectionTO(U8 isConnectTimeout)
{
	DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Set Connect Timeout:%d -> %d", 2, gAppShareModeCtrl.followerConnectionTimerout, isConnectTimeout);
	gAppShareModeCtrl.followerConnectionTimerout = isConnectTimeout;
}

static U8 app_MCSync_Share_GetFollowerConnectionTO(void)
{
	return gAppShareModeCtrl.followerConnectionTimerout;
}


/**************************************************************************************************
* Static (Earbuds Share Pairing System Handler)
**************************************************************************************************/
static void app_MCSync_Share_WriteSharePairingLIAC(void)
{
	U32 LIAC = APP_MCSync_Nvkey_GetSharePairingLIAC();
	DBG_LOG_APP_MCSYNC( "[APP_MCSync][Share] WriteSharePairingLIAC:0x%x", 1, LIAC);
	BtWriteInquiryAccessCode(&gAppMCSyncSystemHandle, (const U32 *)&LIAC, 1);
}

static void app_MCSync_Share_SharePairingInquiry(void)
{
    U32 liacNvKey;

	BtWriteInquiryScanActivity(&gAppMCSyncSystemHandle, EARBUDS_SHARE_PAIRING_INQ_SCAN_INTERVAL, INQ_SCAN_DEFAULT_WINDOW);
	BtWriteScanEnable(&gAppMCSyncSystemHandle, (SCAN_ENABLE_T)SCAN_ENABLE_INQ);

    liacNvKey = APP_MCSync_Nvkey_GetSharePairingLIAC();
    DBG_LOG_APP_MCSYNC( "[APP_MCSync][Share] Inquiry LIAC:0x%x, Time:%d, Num:%d", 3, liacNvKey, APP_MCSync_Nvkey_SharePairingInquiryTime(), APP_MCSync_Nvkey_SharePairingInquiryDevNum());
    BtInquiry(&gAppMCSyncSystemHandle, liacNvKey, APP_MCSync_Nvkey_SharePairingInquiryTime(), APP_MCSync_Nvkey_SharePairingInquiryDevNum());
}

static void app_MCSync_Share_SetSharePairingInfo(S8 rssi, BD_ADDR_T* pBdAddr)
{
	U8 *pTemp = (U8*)&gAppShareModeCtrl.pairingInfo.otherAgentAddr;

	if(FW_Memcmp(pTemp, pBdAddr, sizeof(BD_ADDR_T)))
	{
		FW_Memcpy((U8*)&gAppShareModeCtrl.pairingInfo.otherAgentAddr, (U8*)pBdAddr, sizeof(BD_ADDR_T));
	}
	
	gAppShareModeCtrl.pairingInfo.rssi = rssi;
	gAppShareModeCtrl.pairingInfo.isFoundRssi = 1;
}

static BD_ADDR_T* app_MCSync_Share_GetSharePairingInfo(void)
{
	if(!FW_IsBdAddrZero(&gAppShareModeCtrl.pairingInfo.otherAgentAddr))
	{
		return &gAppShareModeCtrl.pairingInfo.otherAgentAddr;
	}
	return NULL;
}

static void app_MCSync_Share_ResetPairingTimer(void)
{
    FW_ReleaseTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_IN_SHARE_PAIRING_MODE, 0);
    FW_SetTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_IN_SHARE_PAIRING_MODE, NULL, 0, 2000); //2s define time    
}

static U32 app_MCSync_Share_TimerHandler(Handler handler, U16 id, void *msg, U32 handler_id)
{
	U8 plusState;
	UNUSED(handler); 
    UNUSED(handler_id); 
    UNUSED(msg);

	switch(id)
	{
<<<<<<< HEAD
		case APP_SHARE_TIMER_TO_STOP_AGENT:
			DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] APP_SHARE_TIMER_TO_STOP_AGENT", 0);
			app_MCSync_Share_StopAgentConnection();
=======
		case APP_SHARE_TIMER_PAGE_TIMEOUT_RETRY:
			DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] APP_SHARE_TIMER_PAGE_TIMEOUT_RETRY timeout", 0);
			APP_MCSync_Share_EnterMode(FALSE);
			break;
		case APP_SHARE_TIMER_TO_STOP_AGENT:
			DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] APP_SHARE_TIMER_TO_STOP_AGENT", 0);
			app_MCSync_Share_StopAgentConnection();
            app_MCSync_Share_AgentClearCtl();
>>>>>>> db20e11 (second commit)
			break;
		case APP_SHARE_TIMER_SHARE_AGENT_RECONNECT:
			DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] APP_SHARE_TIMER_SHARE_AGENT_RECONNECT", 0);
			app_MCSync_Share_SetAction(APP_SHARE_ACTION_SHARE_PAIRING);
            app_MCSync_Share_AgentDoNextAction();
			break;
		case APP_SHARE_TIMER_FOLLOWER_RECONNECT:
			
<<<<<<< HEAD
			plusState = app_MCSync_Share_GetState();
=======
			plusState = APP_MCSync_Share_GetState();
>>>>>>> db20e11 (second commit)
			
			DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] APP_SHARE_TIMER_FOLLOWER_RECONNECT,plus state:%d, handler_id:%d", 2, plusState, handler_id);

            if(handler_id == 1)
<<<<<<< HEAD
                app_MCSync_Share_SetFollowerConnectionTO(TRUE);
=======
            {
                app_MCSync_Share_SetFollowerConnectionTO(TRUE);
				app_MCSync_Share_FollowerStart(FALSE);
			}
>>>>>>> db20e11 (second commit)
			else
            {
                if(plusState != APP_SHARE_STATE_ENTERED_MODE)
                {
                    APP_MCSyncPlus_FollowerStop();
                    FW_SetTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_FOLLOWER_RECONNECT, NULL, 1, 1800);
                    app_MCSync_Share_SetFollowerConnectionTO(TRUE);
                }
            }
			break;
		case APP_SHARE_TIMER_TO_CONNECT_FOLLOWER:
			DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] APP_SHARE_TIMER_TO_CONNECT_FOLLOWER", 0);
			app_MCSync_Share_StartFollowerConnection();
			break;

		case APP_SHARE_TIMER_FOLLOWER_DISCONNECT:
			DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] TIMER_FOLLOWER_DISCONNECT:%d %d %d, %d", 4,
<<<<<<< HEAD
			BtMCSync_GetShareMode(),  app_MCSync_Share_GetState(), handler_id, gAppShareModeCtrl.countOfFollowerDiscIF);

			if(BtMCSync_GetShareMode() == MCSYNC_SHARE_MODE_FOLLOWER_ENABLE &&
				app_MCSync_Share_GetState() == APP_SHARE_STATE_ENTERED_MODE)
=======
			BtMCSync_GetShareMode(),  APP_MCSync_Share_GetState(), handler_id, gAppShareModeCtrl.countOfFollowerDiscIF);

			if(BtMCSync_GetShareMode() == MCSYNC_SHARE_MODE_FOLLOWER_ENABLE &&
				APP_MCSync_Share_GetState() == APP_SHARE_STATE_ENTERED_MODE)
>>>>>>> db20e11 (second commit)
			{
				if(gAppShareModeCtrl.countOfFollowerDiscIF >= 2)
				{
					DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] disc conn == 2, stop follower", 0);
					APP_MCSyncPlus_FollowerStop();
				}
				else
				{
					DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] disc conn < 2, continue send follower disc IF", 0);
					BtMCSync_SendSyncFollowerDisconn(TRUE, TRUE);
					gAppShareModeCtrl.countOfFollowerDiscIF++;

					FW_SetTimer(&gAppMCSyncShareTimerHandle, 
						APP_SHARE_TIMER_FOLLOWER_DISCONNECT, NULL, 0, 1800);
				}
			}
			else
			{
				DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] direct stop follower", 0);
				APP_MCSyncPlus_FollowerStop();
			}
			break;
            
		case APP_SHARE_TIMER_PUSH_SHARE_PAIRING_MODE_MEDIA:
			APP_Media_PushMediaEvent(MEDIA_EVT_SHARE_MODE_CONTINUE);
			FW_SetTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_PUSH_SHARE_PAIRING_MODE_MEDIA, 
				NULL, 0, 2500);
			break;
            
		case APP_SHARE_TIMER_IN_SHARE_PAIRING_MODE:
            DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] APP_SHARE_TIMER_IN_SHARE_PAIRING_MODE", 0);
			app_MCSync_Share_StopSharePairing();
			break;

		case APP_SHARE_TIMER_AGENT_SEND_FOLLOWER_DISC_IF_REPEAT_TIMER:
			APP_MCSYNC_SendSyncFollowerDisc(FALSE, TRUE);
			break;

		case APP_SHARE_TIMER_AGENT_NOTICE_FOLLOWER_TO_LEAVE_REPEAT_TIMER:
<<<<<<< HEAD
			 if(gAppShareModeCtrl.noticeFollowerToLeaveNum >= 2)	
=======
			 if(gAppShareModeCtrl.noticeFollowerToLeaveNum >= 3)	
>>>>>>> db20e11 (second commit)
			 {
			 	gAppShareModeCtrl.noticeFollowerToLeaveNum = 0;	
			 }
			 else
			 {
			 	BtMCSync_SendSyncAgentNoticeFollowerLeave(1, 0);
			 	FW_SetTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_AGENT_NOTICE_FOLLOWER_TO_LEAVE_REPEAT_TIMER, NULL, 0, 100);
				gAppShareModeCtrl.noticeFollowerToLeaveNum++;
			 }
			break;
		case APP_SHARE_TIMER_FOLLOWER_DISC_TIMER:
		default:
			break;
	}
	return 0;
}

static void app_MCSync_Share_AgentDoNextAction(void)
{
    U8 subState = app_MCSync_Share_GetSubState();
    U8 action = app_MCSync_Share_GetAction();
<<<<<<< HEAD
    U8 shareState = app_MCSync_Share_GetState();
=======
    U8 shareState = APP_MCSync_Share_GetState();
>>>>>>> db20e11 (second commit)
    
    DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Agent Do NextAction: action:%d, state:%d, subState:%d", 3, action, shareState, subState);
    
    if(subState != APP_SHARE_SUBSTATE_NONE || action == APP_SHARE_ACTION_NONE)
        return;

    if(action == APP_SHARE_ACTION_AGENT_START)
    {
        switch(shareState)
        {
            case APP_SHARE_STATE_NONE:
                app_MCSync_Share_StartAgentConnection();
                break;

            case APP_SHARE_STATE_LEAVING:
                if(gAppShareModeCtrl.follower1Connected && gAppShareModeCtrl.follower2Connected)
                    app_MCSync_Share_SetState(APP_SHARE_STATE_ENTERED_MODE, APP_SHARE_SUBSTATE_NONE);
                else
                    app_MCSync_Share_SetState(APP_SHARE_STATE_PREPARING, APP_SHARE_SUBSTATE_NONE);
                /* fallthrough */
            case APP_SHARE_STATE_PREPARING:
            case APP_SHARE_STATE_ENTERED_MODE:
                app_MCSync_Share_SetAction(APP_SHARE_ACTION_NONE);
                break;
            case APP_SHARE_STATE_SHARE_PAIRING_START:
            case APP_SHARE_STATE_PARTNER_GET_PAIRING_DATA:
                break;
        }
    }
    else if(action == APP_SHARE_ACTION_AGENT_STOP)
    {
        switch(shareState)
        {
            case APP_SHARE_STATE_NONE:
                app_MCSync_Share_AgentClearCtl();
                app_MCSync_Share_SetAction(APP_SHARE_ACTION_NONE);
                break;
            case APP_SHARE_STATE_PREPARING:
            case APP_SHARE_STATE_ENTERED_MODE:
            {
                U8 role = BtAwsMce_GetDefaultRole();
                U8 agState = APP_Conn_GetAgLinkState();
                
                DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Agent_DoNextAction: role:%d, agState:%d", 2, role, agState);
                app_MCSync_Share_SetState(APP_SHARE_STATE_LEAVING, APP_SHARE_SUBSTATE_WAITING_TIMEOUT);

                if(agState != LINK_CONNECTED)
                {
                    FW_SetTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_TO_STOP_AGENT, NULL, 0, 0);    
                }
                else
                {
                    if(role == ROLE_AGENT)
                    {
                        BtMCSync_SendSyncAgentNoticeFollowerLeave(1, 0);
						FW_SetTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_AGENT_NOTICE_FOLLOWER_TO_LEAVE_REPEAT_TIMER, NULL, 0, 100);
						gAppShareModeCtrl.noticeFollowerToLeaveNum = 1;
                    }
                    FW_SetTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_TO_STOP_AGENT, NULL, 0, 1000);        
                }
            }
                break;
            case APP_SHARE_STATE_LEAVING:
                app_MCSync_Share_StopAgentConnection();
                break;
            case APP_SHARE_STATE_SHARE_PAIRING_START:
            case APP_SHARE_STATE_PARTNER_GET_PAIRING_DATA:
                app_MCSync_Share_SetRole(APP_SHARE_MODE_ROLE_NONE);
                app_MCSync_Share_StopSharePairing();
                app_MCSync_Share_SetAction(APP_SHARE_ACTION_NONE);
<<<<<<< HEAD
=======
				FW_ReleaseTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_PUSH_SHARE_PAIRING_MODE_MEDIA, 0);
>>>>>>> db20e11 (second commit)
                break;
        }        
    }
    else if(action == APP_SHARE_ACTION_SHARE_PAIRING)
    {
        switch(shareState)
        {
            case APP_SHARE_STATE_NONE:
            case APP_SHARE_STATE_PREPARING:
				app_MCSync_Share_SetState(APP_SHARE_STATE_SHARE_PAIRING_START, APP_SHARE_SUBSTATE_NONE);
                app_MCSync_Share_StartSharePairing();
                app_MCSync_Share_SetAction(APP_SHARE_ACTION_NONE);
                break;

            case APP_SHARE_STATE_LEAVING:
                if(gAppShareModeCtrl.follower1Connected && gAppShareModeCtrl.follower2Connected)
<<<<<<< HEAD
                    app_MCSync_Share_SetState(APP_SHARE_STATE_ENTERED_MODE, APP_SHARE_SUBSTATE_NONE);
                else
                    app_MCSync_Share_SetState(APP_SHARE_STATE_PREPARING, APP_SHARE_SUBSTATE_NONE);
=======
                {
		        	PM_Sniff_SetLinkPolicyDisableAllMode(APP_GetServiceBdAddr());
                	app_MCSync_Share_SetState(APP_SHARE_STATE_ENTERED_MODE, APP_SHARE_SUBSTATE_NONE);
                }
				else
                {
                    app_MCSync_Share_SetState(APP_SHARE_STATE_PREPARING, APP_SHARE_SUBSTATE_NONE);
                }
>>>>>>> db20e11 (second commit)
                /* fallthrough */
            case APP_SHARE_STATE_ENTERED_MODE:
            case APP_SHARE_STATE_SHARE_PAIRING_START:
            case APP_SHARE_STATE_PARTNER_GET_PAIRING_DATA:
                app_MCSync_Share_SetAction(APP_SHARE_ACTION_NONE);
                break;
        }        
    }
}

static void app_MCSync_Share_FollowerDoNextAction(void)
{
    U8 subState = app_MCSync_Share_GetSubState();
    U8 action = app_MCSync_Share_GetAction();
<<<<<<< HEAD
    U8 shareState = app_MCSync_Share_GetState();
=======
    U8 shareState = APP_MCSync_Share_GetState();
>>>>>>> db20e11 (second commit)
    
    DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Follower Do Next Action: action:%d, state:%d, subState:%d", 3, action, shareState, subState);
    
    if(subState != APP_SHARE_SUBSTATE_NONE || action == APP_SHARE_ACTION_NONE)
        return;
    
    if(action == APP_SHARE_ACTION_FOLLOWER_START)
    {
        switch(shareState)
        {
            case APP_SHARE_STATE_NONE:
                app_MCSync_Share_SetState(APP_SHARE_STATE_NONE, APP_SHARE_SUBSTATE_WAITING_TIMEOUT);
                FW_SetTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_FOLLOWER_RECONNECT, NULL, 0, 20000);
<<<<<<< HEAD
                if(BtAwsMce_IsDefaultRoleAgent())
                {
                    
=======

				if(BtAwsMce_IsDefaultRoleAgent())
                {
>>>>>>> db20e11 (second commit)
                    BtMCSync_SendSyncFollowerConn(TRUE);
                    FW_SetTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_TO_CONNECT_FOLLOWER, NULL, 0, 1200);                     
                }
                else
                {
                    FW_SetTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_TO_CONNECT_FOLLOWER, NULL, 0, 0); 
                }
<<<<<<< HEAD
=======
				
>>>>>>> db20e11 (second commit)
                break;

            case APP_SHARE_STATE_LEAVING:
            case APP_SHARE_STATE_PREPARING:
            case APP_SHARE_STATE_ENTERED_MODE:
                app_MCSync_Share_SetAction(APP_SHARE_ACTION_NONE);
                break;
            case APP_SHARE_STATE_SHARE_PAIRING_START:
            case APP_SHARE_STATE_PARTNER_GET_PAIRING_DATA:
                break;
        }        
    }
}

static void app_MCSync_Share_Agent_Prepare_OK(void)
{
    app_MCSync_Share_SetState(APP_SHARE_STATE_PREPARING, APP_SHARE_SUBSTATE_NONE);
    app_MCSync_Share_AgentDoNextAction();
}

static void app_MCSync_Share_Agent_Prepare_Fail(void)
{
    APP_MCSYNC_SendSyncShareModeInfo(APP_SHARE_STATE_NONE, MCSYNC_SHARE_MODE_DISABLE);
    app_MCSync_Share_AgentClearCtl();
    app_MCSync_Share_AgentDoNextAction();
}

static void app_MCSync_Share_Agent_Leave_OK(void)
{
    app_MCSync_Share_AgentClearCtl();
    app_MCSync_Share_AgentDoNextAction();
}

static void app_MCSync_Share_Agent_Leave_Fail(void)
{
    app_MCSync_Share_SetState(APP_SHARE_STATE_LEAVING, APP_SHARE_SUBSTATE_NONE);
    app_MCSync_Share_AgentDoNextAction();
}


static U32 app_MCSync_Share_MsgHandler(Handler handler, U16 id, void *msg, U32 handler_id)
{
	UNUSED(handler);
	UNUSED(handler_id);
	
<<<<<<< HEAD
	U8 plusState = app_MCSync_Share_GetState();
=======
	U8 plusState = APP_MCSync_Share_GetState();
>>>>>>> db20e11 (second commit)
    U8 subState = app_MCSync_Share_GetSubState();
    U8 status;

	switch(id)
	{ 
		case BT_AWS_MCE_SHARE_MODE_ENABLE_CFM:
            status = ((BT_AWS_MCE_SHARE_MODE_ENABLE_CFM_T *)msg)->hci_status;
            DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] MsgHandler: BT_AWS_MCE_SHARE_MODE_ENABLE_CFM, status:%d, state:%d, subState:%d", 3, status, plusState, subState);
<<<<<<< HEAD
            if(plusState == APP_SHARE_STATE_NONE)
=======

			if(status != 0)
			{
				DBG_LOG_APP_MCSYNC("[APP_MCSync][Share]BT_AWS_MCE_SHARE_MODE_ENABLE_CFM debug:%d %d %d", 3, status, plusState, subState);
				//FW_Assert(FALSE);
			}

			if(plusState == APP_SHARE_STATE_NONE)
>>>>>>> db20e11 (second commit)
            {
                if(subState == APP_SHARE_SUBSTATE_WAITING_CFM)
                {
                    if(status == HCI_STATUS_SUCCESS)
                        app_MCSync_Share_Agent_Prepare_OK();
                    else
                        app_MCSync_Share_Agent_Prepare_Fail();
<<<<<<< HEAD
=======

					if(status == HCI_STATUS_PAGE_TIMEOUT)
					{
						FW_SetTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_PAGE_TIMEOUT_RETRY, NULL, 1, 5000);
					}
>>>>>>> db20e11 (second commit)
                }
                else
                {           
                    DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] MsgHandler: BT_AWS_MCE_SHARE_MODE_ENABLE_CFM: Start Error!!!", 0);
                }                
            }
            else if(plusState == APP_SHARE_STATE_LEAVING)
            {
                if(subState == APP_SHARE_SUBSTATE_WAITING_CFM)
                {
                    if(status == HCI_STATUS_SUCCESS)
                        app_MCSync_Share_Agent_Leave_OK();
                    else
                        app_MCSync_Share_Agent_Leave_Fail();
                }
                else
                {           
                    DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] MsgHandler: BT_AWS_MCE_SHARE_MODE_ENABLE_CFM: Stop Error!!!", 0);
                }
            }
            else
            {
                DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] MsgHandler: Warning!!!! Impossible state", 0);
            }
			break;
			
		default:
			break;
	}
	
	return 0;
}

static void app_MCSync_Share_SetFollowerConnected(U8 event)
{
	switch(event)
	{
		case FOLLOWER_1_CONNECTED:
			gAppShareModeCtrl.follower1Connected = TRUE;
			break;
			
		case FOLLOWER_2_CONNECTED:
			gAppShareModeCtrl.follower2Connected = TRUE;
			break;
			
		case FOLLOWER_1_DISCONNECTED:
			gAppShareModeCtrl.follower1Connected = FALSE;
			break;
			
		case FOLLOWER_2_DISCONNECTED:
			gAppShareModeCtrl.follower2Connected = FALSE;
			break;
			
		default:
			break;
	}

	DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Set Follower Connected, event:%d, f1:%d, f2:%d", 3, event, gAppShareModeCtrl.follower1Connected, gAppShareModeCtrl.follower2Connected);
}

static void app_MCSync_Share_FollowerBackNormalState(void)
{
	U8 role = BtAwsMce_GetDefaultRole();
	U8 shareMode = BtMCSync_GetShareMode();
<<<<<<< HEAD
	U8 plusState = app_MCSync_Share_GetState();
=======
	U8 plusState = APP_MCSync_Share_GetState();
>>>>>>> db20e11 (second commit)
	BOOL isOutofCase = APP_SmtChgCse_IsOutOfChargerCase();
	BOOL isPowerOn = APP_PowerOff_IsPowerOn();
	
	DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Follower Back Normal State String1, role:%d, isOutOfCase:%d, share mode:%d", 3, role, isOutofCase, shareMode);
	DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Follower Back Normal State String2, share state:%d, isPowerOn:%d", 2, plusState, isPowerOn);
	
	app_MCSync_Share_SetState(APP_SHARE_STATE_NONE, APP_SHARE_SUBSTATE_NONE);
	BtMCSync_SetShareMode(MCSYNC_SHARE_MODE_DISABLE);
	
	if(!isPowerOn)
	{
		return;
	}
	
	APP_PowerOff_SetTimer(TIMER_AUTO_SWITCH_OFF_TIMEOUT);
	APP_ReConn_ClearListInfo();
	
	if((role == ROLE_AGENT) || (role == ROLE_PARTNER && BtMCSync_IsFeatureSupport(AWSMCE_AUTO_RECONN_AFTER_PWR_ON_FEAT)))
	{
		APP_ReConn_Reconnect(RECONNECT_AWSMCE);
	}
	
	APP_Mcsync_ClearNotReconnMask(MCS_RECONN_MASK_AG_DISC);
    APP_Mcsync_ClearNotReconnMask(MCS_RECONN_MASK_PARTNER_DISC);

	app_MCSync_Share_SetFollowerConnectionTO(FALSE);

	if(isOutofCase)
	{
		APP_ReConn_Reconnect(RECONNECT_PWR_ON);
	}
}

static void app_MCSync_Share_StartSharePairing(void)
{
	U32 pairingTime;

<<<<<<< HEAD
	DBG_LOG_APP_MCSYNC( "[APP_MCSync][Share] Start Share Pairing, share state:%d", 1, app_MCSync_Share_GetState());
=======
	DBG_LOG_APP_MCSYNC( "[APP_MCSync][Share] Start Share Pairing, share state:%d", 1, APP_MCSync_Share_GetState());
>>>>>>> db20e11 (second commit)
	if(MCSYNC_LINK_CONNECTED != BtAwsMce_GetMcsyncState())
	{
		DBG_LOG_APP_MCSYNC( "[APP_MCSync][Share] Start Share Pairing: Mcsync Not connected", 0);
		app_MCSync_Share_FollowerStop();
		return;
	}

	if(BtAwsMce_IsInAirPairing())
	{
		DBG_LOG_APP_MCSYNC( "[APP_MCSync][Share] Start Share Pairing: Air Pairing Ongoing", 0);
		app_MCSync_Share_FollowerStop();
		return;
	}
    
	if(BtMCSync_IsInMCSyncSharePairing())
	{
		DBG_LOG_APP_MCSYNC( "[APP_MCSync][Share] Start Share Pairing: Share Pairing Ongoing", 0);
		app_MCSync_Share_FollowerStop();
		return;
	}
    
<<<<<<< HEAD
    if(app_MCSync_Share_GetState() != APP_SHARE_STATE_SHARE_PAIRING_START)
    {
		DBG_LOG_APP_MCSYNC( "[APP_MCSync][Share] Start Share Pairing: share state is wrong, state:%d", 1, app_MCSync_Share_GetState());
=======
    if(APP_MCSync_Share_GetState() != APP_SHARE_STATE_SHARE_PAIRING_START)
    {
		DBG_LOG_APP_MCSYNC( "[APP_MCSync][Share] Start Share Pairing: share state is wrong, state:%d", 1, APP_MCSync_Share_GetState());
>>>>>>> db20e11 (second commit)
		app_MCSync_Share_FollowerStop();
		return;        
    }
    
    app_MCSync_Share_SetState(APP_SHARE_STATE_SHARE_PAIRING_START, APP_SHARE_SUBSTATE_NONE);
    pairingTime = 100 * APP_MCSync_Nvkey_GetSharePairingTime();
	
	DBG_LOG_APP_MCSYNC( "[APP_MCSync][Share] StartSharePairing: Start!! pairingTime:%d ms", 1, pairingTime);
    
    
    gAppShareModeCtrl.pairingInfo.rssi = -128;
    gAppShareModeCtrl.pairingInfo.isFoundRssi = 0;

    BtMCSyunc_SetMCSyncSharePairing(TRUE);
	BtWriteScanEnable(&gAppMCSyncSystemHandle, (SCAN_ENABLE_T)SCAN_ENABLE_OFF);
	BtWriteInquiryMode(&gAppMCSyncSystemHandle, (INQUARY_MODE_E)INQUARY_MODE_RSSI);

	if(pairingTime)
	{
		FW_SetTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_IN_SHARE_PAIRING_MODE, NULL, 0, pairingTime);
	}
}

static void app_MCSync_Share_StopSharePairing(void)
{	
	BOOL isGetOtherAgent = FALSE;
    BD_ADDR_T* pBdAddr = app_MCSync_Share_GetSharePairingInfo();
    
    DBG_LOG_APP_MCSYNC( "[APP_MCSync][Share] StopSharePairing: isFound:%d, shareRole:%d", 2, gAppShareModeCtrl.pairingInfo.isFoundRssi, app_MCSync_Share_GetRole());

	BtMCSyunc_SetMCSyncSharePairing(FALSE);
	APP_MCSYNC_SystemStopAirPairingInquiry();

	if(gAppShareModeCtrl.pairingInfo.isFoundRssi && pBdAddr)
	{
		DBG_LOG_APP_MCSYNC( "[APP_MCSync][Share] StopSharePairing:BdAddr:0x%x%x", 2, FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE));
		BtMCSync_SetOtherAgentBDA(pBdAddr);
		APP_MCSYNC_SendSyncOtherAgentBDA(pBdAddr);
		gAppShareModeCtrl.pairingInfo.isFoundRssi = 0;
		isGetOtherAgent = TRUE;
	}

	APP_Media_PushMediaEvent(MEDIA_EVT_TWS_EXIT_MCSYNC_SHARE_PAIRING);
	FW_ReleaseTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_IN_SHARE_PAIRING_MODE, 0);
    
    app_MCSync_Share_SetState(APP_SHARE_STATE_NONE, APP_SHARE_SUBSTATE_NONE);

	switch(app_MCSync_Share_GetRole())
	{
		case APP_SHARE_MODE_ROLE_AGENT_TO_FOLLOWER:
			if(isGetOtherAgent)
			{
                if(BtAwsMce_IsDefaultRoleAgent())
                {
<<<<<<< HEAD
                    app_MCSync_Share_SetAction( APP_SHARE_ACTION_FOLLOWER_START);
=======
                    app_MCSync_Share_SetAction(APP_SHARE_ACTION_FOLLOWER_START);
>>>>>>> db20e11 (second commit)
                    app_MCSync_Share_FollowerDoNextAction();
                }
			}
			else
			{
				app_MCSync_Share_FollowerStop();
			}
			break;

		case APP_SHARE_MODE_ROLE_AGENT_TO_AGENT:
			app_MCSync_Share_SetAction( (isGetOtherAgent) ? APP_SHARE_ACTION_AGENT_START : APP_SHARE_ACTION_AGENT_STOP);
            app_MCSync_Share_AgentDoNextAction();
			break;

		default:
			break;
			
	}
}

static void app_MCSync_Share_AgentClearCtl(void)
{
    DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] AgentClearCtl", 0);
    app_MCSync_Share_SetFollowerConnected(FOLLOWER_1_DISCONNECTED);
    app_MCSync_Share_SetFollowerConnected(FOLLOWER_2_DISCONNECTED);
	app_MCSync_Share_SetRole(APP_SHARE_MODE_ROLE_NONE);
	app_MCSync_Share_StopSharePairing();
	FW_ReleaseTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_PUSH_SHARE_PAIRING_MODE_MEDIA, 0);
    FW_ReleaseTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_TO_STOP_AGENT, 0);
<<<<<<< HEAD

=======
	
	FW_ReleaseTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_PAGE_TIMEOUT_RETRY, 0);
		
>>>>>>> db20e11 (second commit)
	APP_Media_PushMediaEvent(MEDIA_EVT_SHARE_MODE_STOP);
	app_MCSync_Share_SetState(APP_SHARE_STATE_NONE, APP_SHARE_SUBSTATE_NONE);
	BtMCSync_SendSyncShareModeInfo(APP_SHARE_STATE_NONE, MCSYNC_SHARE_MODE_DISABLE);
	BtMCSync_SetShareMode(MCSYNC_SHARE_MODE_DISABLE);
	
	APP_MCSync_Share_SetLinkLoss(FALSE);
	g_start_share_mode = 0;
}

static void app_MCSync_Share_StartAgentConnection(void)
{
	U8 role = BtAwsMce_GetDefaultRole();
	U8 shareMode = BtMCSync_GetShareMode();
	U8 agState = APP_Conn_GetAgLinkState();
	U8 mcsyncState = BtAwsMce_GetMcsyncState();
<<<<<<< HEAD
	U8 shareState = app_MCSync_Share_GetState();
=======
	U8 shareState = APP_MCSync_Share_GetState();
>>>>>>> db20e11 (second commit)
    U8 subState = app_MCSync_Share_GetSubState();
	
	DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Start Agent Connection: role:%d, shareMode:%d, agState:%d, mcsyncState:%d", 4, role, shareMode, agState, mcsyncState);
    DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Start Agent Connection: share State:%d, subState:%d", 2, shareState, subState);
	
	if( shareMode == MCSYNC_SHARE_MODE_DISABLE && shareState == APP_SHARE_STATE_NONE && 
        agState == LINK_CONNECTED && mcsyncState == MCSYNC_LINK_CONNECTED /*&& link key change done*/)
	{
		APP_MCSYNC_SendSyncShareModeInfo(APP_SHARE_STATE_PREPARING, MCSYNC_SHARE_MODE_NORMAL_ENABLE);
		BtMCSync_AgentStartShareMode(&gAppMCSyncShareHandle);
		BtMCSync_SetShareMode(MCSYNC_SHARE_MODE_NORMAL_ENABLE);
        
        app_MCSync_Share_SetState(APP_SHARE_STATE_NONE, APP_SHARE_SUBSTATE_WAITING_CFM);
	}
    else
    {
        DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Start Agent Connection: Wrong Calling!!!", 0);
    }
}

static void app_MCSync_Share_StopAgentConnection(void)
{
	U8 role = BtAwsMce_GetDefaultRole();
	U8 shareMode = BtMCSync_GetShareMode();
<<<<<<< HEAD
	U8 shareState = app_MCSync_Share_GetState();
=======
	U8 shareState = APP_MCSync_Share_GetState();
>>>>>>> db20e11 (second commit)
    U8 subState = app_MCSync_Share_GetSubState();
	
	DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Stop Agent Connection: role:%d, shareMode:%d, plusState:%d, subState:%d", 4, role, shareMode, shareState, subState);
	
	if(role == ROLE_AGENT && shareMode == MCSYNC_SHARE_MODE_NORMAL_ENABLE /*&& shareState != APP_SHARE_STATE_LEAVING && subState != APP_SHARE_SUBSTATE_WAITING_CFM*/)
	{
        APP_MCSYNC_SendSyncShareModeInfo(APP_SHARE_STATE_LEAVING, MCSYNC_SHARE_MODE_NORMAL_ENABLE);
        app_MCSync_Share_SetState(APP_SHARE_STATE_LEAVING, APP_SHARE_SUBSTATE_WAITING_CFM);
        BtMCSync_AgentStopShareMode(&gAppMCSyncShareHandle);
	}
    else
    {
        DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Stop Agent Connection: Wrong Calling!!!", 0);
    }
}

static void app_MCSync_Share_StartFollowerConnection(void)
{
	U8 role = BtAwsMce_GetDefaultRole();
	U8 shareMode = BtMCSync_GetShareMode();
<<<<<<< HEAD
	U8 shareState = app_MCSync_Share_GetState();
=======
	U8 shareState = APP_MCSync_Share_GetState();
>>>>>>> db20e11 (second commit)
	U8 mcsyncState = BtAwsMce_GetMcsyncState();
	BD_ADDR_T *pBdAddr = drv_a2dp_get_bdaddr();
	
	DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Follower Start Conn, role:%d, share mode:%d, share state:%d, mcsync state:%d", 4, role, shareMode, shareState, mcsyncState);
			
	if(shareMode == MCSYNC_SHARE_MODE_DISABLE && shareState == APP_SHARE_STATE_NONE /*&& mcsyncState == MCSYNC_LINK_CONNECTED*/ /*&& link key change done*/)
	{
		if(pBdAddr != NULL)
		{
            APP_A2dp_StopDSP(pBdAddr);
		}
		
		APP_PowerOff_CancelTimer(TIMER_AWS_IDLE_POWEROFF);
		app_MCSync_Share_SetState(APP_SHARE_STATE_PREPARING, APP_SHARE_SUBSTATE_WAITING_CFM);
		BtMCSync_SetShareMode(MCSYNC_SHARE_MODE_FOLLOWER_ENABLE);
		APP_Pairing_ConnectableHandler(NON_CONNECTABLE_MODE);
	}
}

static void app_MCSync_Share_StopFollowerConnection(void)
{
	U8 role = BtAwsMce_GetDefaultRole();
	U8 shareMode = BtMCSync_GetShareMode();
<<<<<<< HEAD
	U8 plusState = app_MCSync_Share_GetState();
=======
	U8 plusState = APP_MCSync_Share_GetState();
>>>>>>> db20e11 (second commit)
	
	DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Follower Stop Conn:%d,%d,%d", 3, role, shareMode, plusState);

    
    FW_ReleaseTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_FOLLOWER_RECONNECT, 0);
	
	if(shareMode == MCSYNC_SHARE_MODE_FOLLOWER_ENABLE && plusState != APP_SHARE_STATE_LEAVING)
	{
		//BtMCSync_SendSyncFollowerDisconn(TRUE, TRUE);
		app_MCSync_Share_SetState(APP_SHARE_STATE_LEAVING, APP_SHARE_SUBSTATE_NONE);
		BtMCSync_FollowerStopShareMode();
	}
	else
	{
		if(plusState == APP_SHARE_STATE_LEAVING)
		{
			BtMCSync_FollowerStopShareMode(); 
		}
		
		app_MCSync_Share_SetState(APP_SHARE_STATE_NONE, APP_SHARE_SUBSTATE_NONE);
	}
}

<<<<<<< HEAD
static void app_MCSync_Share_AgentStart(void)
=======
static void app_MCSync_Share_AgentStart(U8 isTriggerByUser)
>>>>>>> db20e11 (second commit)
{	
	if(APP_MCSync_Rho_IsRhoing() == TRUE)
	{
		DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Agent Start: RHOing, wait until RHO is done", 0);
		return;
	}
    
    if(FW_CheckTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_PUSH_SHARE_PAIRING_MODE_MEDIA, 0))
    {
        DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Agent Start: has triggered already!", 0);
        return;       
    }
    
    if(!BtAwsMce_IsDefaultRoleAgent())
    {
        DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Agent Start: Not Agent!", 0);
        return;         
    }

	APP_Media_PushMediaEvent(MEDIA_EVT_SHARE_MODE_START);

	if(BtAwsMce_GetMcsyncState() != MCSYNC_LINK_CONNECTED)
	{
		APP_MCSync_Share_SetLinkLoss(FALSE);
		DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Agent Start: MCSYNC not connected", 0);
		return;
	}
    
<<<<<<< HEAD
    DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Agent Start: state:%d, linkloss:%d", 2, 
		app_MCSync_Share_GetState(), APP_MCSync_Share_GetLinkLoss());
=======
    DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Agent Start: state:%d, linkloss:%d, trigger by user:%d", 3, 
		APP_MCSync_Share_GetState(), APP_MCSync_Share_GetLinkLoss(), isTriggerByUser);
>>>>>>> db20e11 (second commit)
	
	FW_ReleaseTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_PUSH_SHARE_PAIRING_MODE_MEDIA, 0);
	FW_SetTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_PUSH_SHARE_PAIRING_MODE_MEDIA, NULL, 0, 2500);
	app_MCSync_Share_SetRole(APP_SHARE_MODE_ROLE_AGENT_TO_AGENT);

	if(!FW_IsBdAddrZero(BtMCSync_GetShareModeBdAddr()) && 
		APP_MCSync_Share_GetLinkLoss())
	{
		DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] execute share command", 0);
		app_MCSync_Share_SetState(APP_SHARE_STATE_NONE, APP_SHARE_SUBSTATE_NONE);
		BtMCSync_SetShareMode(MCSYNC_SHARE_MODE_DISABLE);
		app_MCSync_Share_StartAgentConnection();
	}
	
	if(!FW_IsBdAddrZero(BtMCSync_GetShareModeBdAddr()) &&
<<<<<<< HEAD
		(app_MCSync_Share_GetState() != APP_SHARE_STATE_PARTNER_GET_PAIRING_DATA))
=======
		(APP_MCSync_Share_GetState() != APP_SHARE_STATE_PARTNER_GET_PAIRING_DATA) &&
		!isTriggerByUser)
>>>>>>> db20e11 (second commit)
	{
		FW_SetTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_SHARE_AGENT_RECONNECT, NULL, 0, 2000 /*40000*/);
		DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Agent Start: set 2s timer", 0);
        app_MCSync_Share_SetAction(APP_SHARE_ACTION_AGENT_START);
	}
    else
    {
       app_MCSync_Share_SetAction(APP_SHARE_ACTION_SHARE_PAIRING);
    }
<<<<<<< HEAD
=======
	
>>>>>>> db20e11 (second commit)
    app_MCSync_Share_AgentDoNextAction();	
}

static void app_MCSync_Share_AgentStop(void)
{
    DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Agent Stop", 0);
    app_MCSync_Share_SetAction(APP_SHARE_ACTION_AGENT_STOP);
    app_MCSync_Share_AgentDoNextAction();
	
}

<<<<<<< HEAD
static void app_MCSync_Share_FollowerStart(void)
=======
static void app_MCSync_Share_FollowerStart(U8 isTriggerByUser)
>>>>>>> db20e11 (second commit)
{
	if(APP_MCSync_Rho_IsRhoing() == TRUE)
	{
		DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Follower Start: RHOing, wait until RHO is done", 0);
		return;
	}
    
    if(FW_CheckTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_PUSH_SHARE_PAIRING_MODE_MEDIA, 0))
    {
        DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Follower Start: has triggered already!", 0);
        return;       
    }
    
    if(!BtAwsMce_IsDefaultRoleAgent() && !BtAwsMce_IsDefaultRolePartner())
    {
		DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Follower Start: Not Agent && Not Partner!", 0);
		return;        
    }
    
	APP_Media_PushMediaEvent(MEDIA_EVT_SHARE_MODE_START);
	APP_Media_PushMediaEvent(MEDIA_EVT_SHARE_MODE_FOLLOWER);

    if(BtAwsMce_GetMcsyncState() != MCSYNC_LINK_CONNECTED)
	{
		DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Follower Start: MCSYNC not connected", 0);
		return;
	}
    
	FW_ReleaseTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_PUSH_SHARE_PAIRING_MODE_MEDIA, 0);
	FW_SetTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_PUSH_SHARE_PAIRING_MODE_MEDIA, NULL, 0, 2500);

<<<<<<< HEAD
	if(BtAwsMce_IsDefaultRoleAgent())
	{
		app_MCSync_Share_SetRole(APP_SHARE_MODE_ROLE_AGENT_TO_FOLLOWER);
        
        if( !FW_IsBdAddrZero(BtMCSync_GetShareModeBdAddr()) &&
            app_MCSync_Share_GetState() != APP_SHARE_STATE_PARTNER_GET_PAIRING_DATA  &&
            app_MCSync_Share_GetFollowerConnectionTO() == 0)
=======
	DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Follower Start: trigger by user:%d, ag state:%d", 
		2, isTriggerByUser, PM_GetAgLinkState());
	
	if(BtAwsMce_IsDefaultRoleAgent())
	{
		if(PM_GetAgLinkState() == LINK_CONNECTING)
		{
			DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Follower Start, cancel ag connection", 0);
			APP_ReConn_ClearCtl();
		}
		
		app_MCSync_Share_SetRole(APP_SHARE_MODE_ROLE_AGENT_TO_FOLLOWER);
        
        if(!FW_IsBdAddrZero(BtMCSync_GetShareModeBdAddr()) &&
            APP_MCSync_Share_GetState() != APP_SHARE_STATE_PARTNER_GET_PAIRING_DATA  &&
            app_MCSync_Share_GetFollowerConnectionTO() == 0 && !isTriggerByUser)
>>>>>>> db20e11 (second commit)
        {
            DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Follower Start: reconn share mode", 0);
            app_MCSync_Share_SetAction(APP_SHARE_ACTION_FOLLOWER_START);
            app_MCSync_Share_FollowerDoNextAction();
        }
        else
        {
            app_MCSync_Share_SetFollowerConnectionTO(FALSE);
            DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Follower Start, follower start share pairing", 0);
			app_MCSync_Share_SetState(APP_SHARE_STATE_SHARE_PAIRING_START, APP_SHARE_SUBSTATE_NONE);
			app_MCSync_Share_StartSharePairing();
        }
	}
	else
	{
		app_MCSync_Share_SetRole(APP_SHARE_MODE_ROLE_PARTNER_TO_FOLLOWER);
        app_MCSync_Share_SetFollowerConnectionTO(FALSE);
		DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Follower Start, MCSYNC follower start connection", 0);
        app_MCSync_Share_SetAction(APP_SHARE_ACTION_FOLLOWER_START);
		app_MCSync_Share_SetState(APP_SHARE_STATE_NONE, APP_SHARE_SUBSTATE_NONE);
		app_MCSync_Share_FollowerDoNextAction();       
	}
}

void APP_MCSyncPlus_FollowerStop(void)
{
	BD_ADDR_T *pBdAddr = BtMCSync_GetShareModeBdAddr();

<<<<<<< HEAD
	U8 plusState = app_MCSync_Share_GetState();
=======
	U8 plusState = APP_MCSync_Share_GetState();
>>>>>>> db20e11 (second commit)
	U8 shareMode  = BtMCSync_GetShareMode();

	DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Follower Stop, shareState:%d, shareMode:%d", 2, plusState, shareMode);
    
	if(BtAwsMce_IsDefaultRoleAgent())
	{
		BtMCSync_SendSyncFollowerConn(FALSE);
	}
	
    FW_ReleaseTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_TO_CONNECT_FOLLOWER, 0);

	FW_ReleaseTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_FOLLOWER_DISCONNECT, 0);

	gAppShareModeCtrl.countOfFollowerDiscIF = 0;
	
	app_MCSync_Share_StopFollowerConnection();

	if(pBdAddr)
	{
		AUDIO_DSP_STOP_PARA_STUR audioDspPara;

		FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_STOP_PARA_STUR));
		FW_Memcpy(&audioDspPara.bdAddr, pBdAddr, sizeof(BD_ADDR_T));
		audioDspPara.audioType = AUDIO_A2DP;
		APP_AudioDspStop(&audioDspPara);
	}
	
	app_MCSync_Share_SetRole(APP_SHARE_MODE_ROLE_NONE);
	app_MCSync_Share_SetFollowerConnectionTO(FALSE);

	if(plusState == APP_SHARE_STATE_SHARE_PAIRING_START)
	{
		app_MCSync_Share_StopSharePairing();
	}

	FW_ReleaseTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_PUSH_SHARE_PAIRING_MODE_MEDIA, 0);
	APP_Media_PushMediaEvent(MEDIA_EVT_SHARE_MODE_STOP);
	//app_MCSync_Share_SetState(APP_SHARE_STATE_NONE, APP_SHARE_SUBSTATE_NONE);
	//BtMCSync_SetShareMode(MCSYNC_SHARE_MODE_DISABLE);
	//app_MCSync_Share_SetRole(APP_SHARE_MODE_ROLE_NONE);
}

static void app_MCSync_Share_FollowerStop(void)
{
	U8 shareMode = BtMCSync_GetShareMode();
<<<<<<< HEAD
	U8 plusState = app_MCSync_Share_GetState();
=======
	U8 plusState = APP_MCSync_Share_GetState();
>>>>>>> db20e11 (second commit)
	
	DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Follower Stop, mode:%d, state:%d", 2, shareMode, plusState);
    
    if(shareMode == MCSYNC_SHARE_MODE_FOLLOWER_ENABLE && (plusState == APP_SHARE_STATE_ENTERED_MODE || plusState == APP_SHARE_STATE_PREPARING))
    {
 		if(!FW_CheckTimer(&gAppMCSyncShareTimerHandle, 
			APP_SHARE_TIMER_FOLLOWER_DISCONNECT, 0))
		{
			DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] set disc conn timer, 1.8s", 0);
			gAppShareModeCtrl.countOfFollowerDiscIF = 1;
			BtMCSync_SendSyncFollowerDisconn(TRUE, TRUE);
            
            FW_SetTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_FOLLOWER_DISCONNECT, NULL, 0, 1800);
		}       
    }
    else
    {
		DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] share mode , state wrong", 0);
		FW_ReleaseTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_PUSH_SHARE_PAIRING_MODE_MEDIA, 0);
        FW_ReleaseTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_FOLLOWER_RECONNECT, 1);
        FW_ReleaseTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_FOLLOWER_RECONNECT, 0);
		APP_MCSyncPlus_FollowerStop();        
    }
}

static void app_MCSync_Share_ClearCtl(void)
{
	app_MCSync_Share_SetFollowerConnectionTO(FALSE);
}

/*************************************************************************************************
* Public function
**************************************************************************************************/
void APP_MCSync_Share_Init(void)
{
	BtMCSync_SetShareMode(MCSYNC_SHARE_MODE_DISABLE);
}

void APP_MCSync_Share_KeyFollowerStart(void)
{
    APP_MCSync_Share_EnterMode(TRUE);
}

void APP_MCSync_Share_KeyFollowerStop(void)
{
    APP_MCSync_Share_ExitMode();
}

void APP_MCSync_Share_KeyAgentStart(void)
{	
    APP_MCSync_Share_EnterMode(FALSE);
}

void APP_MCSync_Share_KeyAgentStop(void)
{
	APP_MCSync_Share_ExitMode();
}

void APP_MCSync_Share_KeySharePairingStart(void)
{
    app_MCSync_Share_StartSharePairing();
}

void APP_MCSync_Share_KeySharePairingStop(void)
{
    app_MCSync_Share_StopSharePairing();
}

U8 APP_MCSync_Share_StatusChangeHandler(BD_ADDR_T *pBdAddr, U8 role)
{
    U8 agState = APP_Conn_GetAgLinkState();
<<<<<<< HEAD
    U8 plusState = app_MCSync_Share_GetState();
=======
    U8 plusState = APP_MCSync_Share_GetState();
>>>>>>> db20e11 (second commit)
    U8 shareMode = BtMCSync_GetShareMode();

	UNUSED(pBdAddr);
	DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] StatusChangeHandler info:%d %d %d, role:0x%x", 4, 
		shareMode, plusState, agState, role);
	
	if(plusState == APP_SHARE_STATE_LEAVING || shareMode == MCSYNC_SHARE_MODE_FOLLOWER_ENABLE)
	{
		return role;
	}
	
	if(shareMode == MCSYNC_SHARE_MODE_DISABLE)
	{
		APP_MCSYNC_SendSyncShareModeInfo(APP_SHARE_STATE_NONE, MCSYNC_SHARE_MODE_DISABLE);
		return role;
	}
	
	switch(role)
	{
		case ROLE_AGENT:
		case ROLE_SLAVE_CONNECTABLE_AGENT:
			switch(agState)
			{
				case LINK_CONNECTED:
					break;

				default:
					switch(plusState)
					{
						case APP_SHARE_STATE_PREPARING:
						case APP_SHARE_STATE_ENTERED_MODE:
							app_MCSync_Share_SetState(APP_SHARE_STATE_NONE, APP_SHARE_SUBSTATE_NONE);
							break;
						
						default:
							break;
					}
					break;
			}
			break;
			
		case ROLE_SLAVE_AGENT_WITH_PARTNER:
			switch(agState)
			{
				case LINK_CONNECTED:
					switch(plusState)
					{
						case APP_SHARE_STATE_NONE:
							APP_MCSYNC_SendSyncShareModeInfo(APP_SHARE_STATE_PREPARING, MCSYNC_SHARE_MODE_NORMAL_ENABLE);
							BtMCSync_AgentStartShareMode(&gAppMCSyncShareHandle);
							break;
							
						default:
							APP_MCSYNC_SendSyncShareModeInfo(plusState, MCSYNC_SHARE_MODE_NORMAL_ENABLE);
							break;
					}
					break;
				
				default:
					APP_MCSYNC_SendSyncShareModeInfo(APP_SHARE_STATE_NONE, MCSYNC_SHARE_MODE_NORMAL_ENABLE);
					break;
			}
			break;
		
		default:
			break;
	}
    
    return role;
}

void APP_MCSync_Share_AgentEventHandler(U8 event)
{
<<<<<<< HEAD
	DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] AgentEventHandler:%d %d", 2, APP_Conn_GetAgLinkState(), APP_MCSync_Share_GetLinkLoss());  
    switch(event)
    {
        case APP_SHARE_AGENT_EVENT_PARTNER_CONNECTED_CFM:
			if(APP_Conn_GetAgLinkState() == LINK_CONNECTED && APP_MCSync_Share_GetLinkLoss()) //Iverson, sp linkloss
			{
                if(APP_MCSync_Share_IsEnable())
                {
                    app_MCSync_Share_AgentStart();
=======
	DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] AgentEventHandler:%d %d %d %d", 4, 
		APP_GetAGNum(), APP_MCSync_Share_GetLinkLoss(), APP_MCSync_Share_IsEnable(), 
		event); 
	
    switch(event)
    {
        case APP_SHARE_AGENT_EVENT_PARTNER_CONNECTED_CFM:
			if(APP_GetAGNum() && APP_MCSync_Share_GetLinkLoss()) //Iverson, sp linkloss
			{
                //if(APP_MCSync_Share_IsEnable())
                {
                    app_MCSync_Share_AgentStart(FALSE);
>>>>>>> db20e11 (second commit)
					APP_MCSync_Share_SetLinkLoss(FALSE);
                }
			}	        
            break;
    }
}

void APP_MCSync_Share_PartnerEventHandler(U8 event, U8 *pMsg)
{
	U8 shareMode = BtMCSync_GetShareMode();
    
    DBG_LOG_APP_MCSYNC( "[APP_MCSync][Share] Partner Event Handler: event:%d, mode:%d", 2, event, shareMode);
	
	if(shareMode == MCSYNC_SHARE_MODE_FOLLOWER_ENABLE)
	{
		return;
	}
	
	switch(event)
	{
		case APP_SHARE_PARTNER_EVENT_SYNC_INFO:
			app_MCSync_Share_SetState(((MCSYNC_SYNC_SHARE_MODE_INFO_IND_T *)pMsg)->state, APP_SHARE_SUBSTATE_NONE);
			BtMCSync_SetShareMode(((MCSYNC_SYNC_SHARE_MODE_INFO_IND_T *)pMsg)->shareMode);
			break;
			
		case APP_SHARE_PARTNER_EVENT_ACL_CLOSE:
		case APP_SHARE_PARTNER_EVENT_MCSYNC_DISCONNECT_IND:
			if(shareMode == MCSYNC_SHARE_MODE_NORMAL_ENABLE)
			{
				app_MCSync_Share_SetState(APP_SHARE_STATE_NONE, APP_SHARE_SUBSTATE_NONE);
			}
			break;
		
		default:
			break;
	}
}

BOOL APP_MCSync_Share_FollowerEventHandler(U8 event, BD_ADDR_T *pBdAddr, U8 *pMsg)
{
	U8 shareMode = BtMCSync_GetShareMode();
<<<<<<< HEAD
	U8 plusState = app_MCSync_Share_GetState();
=======
	U8 plusState = APP_MCSync_Share_GetState();
>>>>>>> db20e11 (second commit)
	U8 mcsyncState = BtAwsMce_GetMcsyncState();
	U8 profilePlusState = BtMCSync_GetShareState();
	
	DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Follower Event Handler:share mode: %d, app share state:%d, profile share state:%d", 3, shareMode, plusState, profilePlusState);
	DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Follower Event Handler:event: %d, mcsyncState:%d, BdAddr:0x%x%x", 4, event, mcsyncState, FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE));

	if(event == APP_SHARE_FOLLOWER_EVENT_AG_CONNECT_CFM)
    {
        DBG_LOG_APP_Connection( "[APP_CONN] isSpecial:%d isAgent:%d connTO:%d", 3, BtAwsMce_IsSpecialLink(pBdAddr), BtAwsMce_IsDefaultRoleAgent(), app_MCSync_Share_GetFollowerConnectionTO());
        if(BtAwsMce_IsSpecialLink(pBdAddr) && BtAwsMce_IsDefaultRoleAgent() && app_MCSync_Share_GetFollowerConnectionTO())
        {
<<<<<<< HEAD
            app_MCSync_Share_FollowerStart();
=======
            app_MCSync_Share_FollowerStart(FALSE);
>>>>>>> db20e11 (second commit)
        }
    }

	if(shareMode != MCSYNC_SHARE_MODE_FOLLOWER_ENABLE)
	{
		if(BtMCSync_IsShareModeBdAddr(pBdAddr))
		{
			if(APP_AudioDspIsStreaming(pBdAddr, AUDIO_A2DP))
			{
				AUDIO_DSP_STOP_PARA_STUR audioDspPara;

				FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_STOP_PARA_STUR));
				FW_Memcpy(&audioDspPara.bdAddr, pBdAddr, sizeof(BD_ADDR_T));
				audioDspPara.audioType = AUDIO_A2DP;
				APP_AudioDspStop(&audioDspPara);
			}	
 
			APP_ClearAppLinkBdAddr(pBdAddr);
		}
		return FALSE;
	}

	switch(event)
	{
		case APP_SHARE_FOLLOWER_EVENT_NON_CONNECTABLE:
			if(plusState == APP_SHARE_STATE_PREPARING)
			{
                U8 agState = APP_Conn_GetAgLinkState();
                DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Follower Event Handler:agState: %d", 1, agState);
				switch(agState)
				{
					case LINK_CONNECTED:
						APP_Conn_ReleaseMultiLink();
						break;
					case LINK_CONNECTING:
						APP_Conn_ReleaseMultiLink();
					default:
						BtMCSync_FollowerScanOffEvent();
						break;
				}
			}
			break;
		
		case APP_SHARE_FOLLOWER_EVENT_ACL_CLOSE:
			APP_SetACLState(APP_GetAppLinkByBdAddr(pBdAddr), ACL_STATE_OFF);
			if(!BtMCSync_IsShareModeBdAddr(pBdAddr))
			{
				APP_LinkKey_SaveHistoryWhenDisconnect(pBdAddr);
				APP_HfpSco_DisconnectHandleByBdAddress(pBdAddr);
				APP_ClearAppLinkBdAddr(pBdAddr);
			}
			else
			{
                APP_A2dp_StopDSP(pBdAddr);
				if(BtMCSync_GetShareMode() == MCSYNC_SHARE_MODE_FOLLOWER_ENABLE &&
					BtMCSync_GetShareState() == MCSYNC_FOLLOWER_STATE_CONNECTING)
				{
					
				}
				else
				{
					app_MCSync_Share_SetState(APP_SHARE_STATE_NONE, APP_SHARE_SUBSTATE_NONE);
					BtMCSync_SetShareMode(MCSYNC_SHARE_MODE_DISABLE);
					APP_ClearAppLinkBdAddr(pBdAddr);
				}
			}
			
			break;
		
		case APP_SHARE_FOLLOWER_EVENT_ACL_OPEN:
			if(!BtMCSync_IsShareModeBdAddr(pBdAddr))
			{
				return FALSE;
			}
			FW_ReleaseTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_FOLLOWER_RECONNECT, 0);

			APP_SetAppLinkBdAddr(pBdAddr);
			break;
		
		case APP_SHARE_FOLLOWER_EVENT_MCSYNC_CONNECT_CFM:
			if(BtMCSync_IsShareModeBdAddr(pBdAddr))
			{
				if(((AWSMCE_CONNECT_CFM_T *)pMsg)->status == BT_STATUS_SUCCESS)
				{
<<<<<<< HEAD
=======
					drv_a2dp_update_audio_channel(AUDIO_CH_MONO);
>>>>>>> db20e11 (second commit)
					switch(plusState)
					{
						case APP_SHARE_STATE_PREPARING:
							FW_ReleaseTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_PUSH_SHARE_PAIRING_MODE_MEDIA, 0);
<<<<<<< HEAD
=======

							APP_Pairing_CancelTimer(TIMER_PAIRING_MODE_PDL_EMPTY_TIMEOUT);
							
>>>>>>> db20e11 (second commit)
							app_MCSync_Share_SetState(APP_SHARE_STATE_ENTERED_MODE, APP_SHARE_SUBSTATE_NONE);
							APP_PowerOff_CancelTimer(TIMER_AUTO_SWITCH_OFF_TIMEOUT);
							break;
							
						default:
							break;
					}
				}
				else
				{
					switch(plusState)
					{
						case APP_SHARE_STATE_LEAVING:
							app_MCSync_Share_FollowerBackNormalState();
							break;
							
						default:
							break;
					}
				}
			}
			break;
			
		case APP_SHARE_FOLLOWER_EVENT_AG_CONNECT_CFM:
			if(!BtAwsMce_IsSpecialLink(pBdAddr))
			{
				APP_Conn_ReleaseMultiLink();
			}
			break;
			
		case APP_SHARE_FOLLOWER_EVENT_MCSYNC_DISCONNECT_IND:
			if(!BtMCSync_IsShareModeBdAddr(pBdAddr))
			{
				return FALSE;
			}
			
			switch(plusState)
			{
				case APP_SHARE_STATE_LEAVING:
					app_MCSync_Share_FollowerBackNormalState();
					BtMCSync_ClearShareState();
					break;
				
				case APP_SHARE_STATE_ENTERED_MODE:
					app_MCSync_Share_SetState(APP_SHARE_STATE_PREPARING, APP_SHARE_SUBSTATE_NONE);
					APP_PowerOff_SetTimer(TIMER_AUTO_SWITCH_OFF_TIMEOUT);
					break;
				
				default:
					break;
			}
			break;
			
		case APP_SHARE_FOLLOWER_EVENT_MCSYNC_WITH_PARTNER:
			APP_Mcsync_ClearNotReconnMask(MCS_RECONN_MASK_PARTNER_DISC);
			break;
            
        case APP_SHARE_FOLLOWER_EVENT_LEAVE_MODE_IND:
            {
                AWSMCE_SYNC_AGENT_NOTICE_FOLLOWER_LEAVE_IND_T *ind = (AWSMCE_SYNC_AGENT_NOTICE_FOLLOWER_LEAVE_IND_T *)pMsg;
                if(ind && plusState == APP_SHARE_STATE_ENTERED_MODE)
                {
                    if(ind->isFollowerIFPkt == 0 && ind->isLeaveShareMode == 1)
                    {
                        APP_MCSyncPlus_FollowerStop();
                    }
                }
            }
            break;
			
		default:
			break;
	}
	
	return TRUE;
}

BOOL APP_MCSync_Share_IsGetIFPacketAllowed(U8 event)
{
	U8 shareMode = BtMCSync_GetShareMode();
	BOOL value = TRUE;
	
	if(shareMode == MCSYNC_SHARE_MODE_FOLLOWER_ENABLE)
	{
		switch(event)
		{
			case AWSMCE_CONNECT_CFM:
			case AWSMCE_DISCONNECT_IND:
			case AWSMCE_AGENT_A2DP_EVT_IND:
			case AWSMCE_SYNC_AGENT_RETRANSMIT_FOLLOWER_DATA_IND:
			case AWSMCE_SYNC_VPRT_IND:
			case AWSMCE_SYNC_AGENT_NOTICE_FOLLOWER_LEAVE_IND:
			case AWSMCE_SYNC_FOLLOWER_RETRIGGER_A2DP_TRANSFORM_IND:
			case AWSMCE_SYNC_POWEROFF_IND:
				value = TRUE;
				break;
				
			default:
				DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] IsGetIFPacketAllowed:%d", 1, event);
				value = FALSE;
                break;
		}
	}
	
	return value;
}


void APP_MCSync_Share_EnterMode(BOOL isFollow)
{
    if(isFollow)
    {
    	APP_MCSync_Share_SetMode(APP_SHARE_MODE_FOLLOWER);
<<<<<<< HEAD
        drv_a2dp_mute_spk();
        app_MCSync_Share_FollowerStart();        
=======
		APP_MCSYNC_SendNoticeMasterPartnerToShareMode(APP_SHARE_MODE_FOLLOWER);
        //drv_a2dp_mute_spk();
        app_MCSync_Share_FollowerStart(TRUE);        
>>>>>>> db20e11 (second commit)
    }
    else
    {
    	APP_MCSync_Share_SetMode(APP_SHARE_MODE_AGENT_PARTNER);
<<<<<<< HEAD
        if(BtAwsMce_IsDefaultRoleAgent())
        {
            drv_a2dp_mute_spk();
            app_MCSync_Share_AgentStart();
=======
		APP_MCSYNC_SendNoticeMasterPartnerToShareMode(APP_SHARE_MODE_AGENT_PARTNER);
        if(BtAwsMce_IsDefaultRoleAgent())
        {
            //drv_a2dp_mute_spk();
            app_MCSync_Share_AgentStart(TRUE);
>>>>>>> db20e11 (second commit)
        }        
    }
}

void APP_MCSync_Share_ExitMode(void)
{
    U8 shareRole = app_MCSync_Share_GetRole();
    U8 shareMode = BtMCSync_GetShareMode();
	U32 supportedState;
<<<<<<< HEAD
	
	DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Exit Mode, mcsync share mode:%d, share role:%d, state:%d", 3, shareMode, shareRole, app_MCSync_Share_GetState());

	APP_MCSync_Share_SetMode(APP_SHARE_MODE_OFF);
=======
	U8 shareState = APP_MCSync_Share_GetState();
	U8 appShareMode = APP_MCSync_Share_IsEnable();
	
	DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Exit Mode, mcsync share mode:%d, share role:%d, state:%d, app share mode:%d", 4, 
		shareMode, shareRole, shareState, APP_MCSync_Share_IsEnable());
>>>>>>> db20e11 (second commit)

	if(shareMode == MCSYNC_SHARE_MODE_NORMAL_ENABLE)
	{
        if(BtAwsMce_IsDefaultRolePartner())
        {
<<<<<<< HEAD
            supportedState = APP_NVKEY_GetSupportedStateByEventCode(KEY_AGENT_STOPS_SHARE_MODE);
            APP_MCSYNC_SendSyncKeyInfo(KEY_AGENT_STOPS_SHARE_MODE, supportedState);
=======
            supportedState = APP_NVKEY_GetSupportedStateByEventCode(KEY_MASTER_SHARE_MODE_STOP);
            APP_MCSYNC_SendSyncKeyInfo(KEY_MASTER_SHARE_MODE_STOP, supportedState);
>>>>>>> db20e11 (second commit)
        }
        else
        {
            app_MCSync_Share_AgentStop(); 
        }
	}
    else if(shareRole == APP_SHARE_MODE_ROLE_AGENT_TO_FOLLOWER || shareRole == APP_SHARE_MODE_ROLE_PARTNER_TO_FOLLOWER)
    {
        app_MCSync_Share_FollowerStop();        
    }
<<<<<<< HEAD
	else
	{
		app_MCSync_Share_ClearCtl();
	}
=======
	else if(appShareMode == APP_SHARE_MODE_FOLLOWER && BtAwsMce_IsDefaultRolePartner())
	{
       supportedState = APP_NVKEY_GetSupportedStateByEventCode(KEY_FOLLOWER_SHARE_MODE_STOP);
       APP_MCSYNC_SendSyncKeyInfo(KEY_FOLLOWER_SHARE_MODE_STOP, supportedState);
	}
	else if(appShareMode == APP_SHARE_MODE_AGENT_PARTNER)
	{
		if(BtAwsMce_IsDefaultRoleAgent() && shareState == APP_SHARE_STATE_SHARE_PAIRING_START)
		{
			APP_MCSYNC_SendNoticeMasterPartnerToShareMode(APP_SHARE_MODE_OFF);
			app_MCSync_Share_AgentStop(); 
		}
		else if(BtAwsMce_IsDefaultRolePartner())
		{
			supportedState = APP_NVKEY_GetSupportedStateByEventCode(KEY_MASTER_SHARE_MODE_STOP);

			APP_MCSYNC_SendSyncKeyInfo(KEY_MASTER_SHARE_MODE_STOP, supportedState);
		}
	}
	else
	{
		app_MCSync_Share_ClearCtl();
		app_MCSync_Share_AgentClearCtl();
	}

	APP_MCSync_Share_SetMode(APP_SHARE_MODE_OFF);
>>>>>>> db20e11 (second commit)
    
}

void APP_MCSync_Share_SetMode(U8 mode)
{
	DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Set app mode:%d -> %d", 2, gAppIsShareModeOn, mode);
	gAppIsShareModeOn = mode;
}

U8 APP_MCSync_Share_IsEnable(void)
{
    return gAppIsShareModeOn;
}

void APP_MCSync_Share_RhoAssign(U8 mode)
{
    if(mode)
    {
        APP_MCSync_Share_EnterMode( (mode == APP_SHARE_MODE_FOLLOWER) ? TRUE : FALSE);
    }
    else
    {
        APP_MCSync_Share_ExitMode();
    }
}


/**************************************************************************************************
* Public (Earbuds Air-pairing)
**************************************************************************************************/


BOOL APP_MCSync_Share_IsKeyNeedSync(U16 keyEventCode)
{
    switch(keyEventCode)
    {
		case KEY_SHARE_PAIRING_START:
		case KEY_SHARE_PAIRING_STOP:
<<<<<<< HEAD
		case KEY_AGENT_STARTS_SHARE_MODE:
		case KEY_AGENT_STOPS_SHARE_MODE:
		case KEY_FOLLOWER_STARTS_SHARE_MODE:
		case KEY_AGENT_TOGGLE_SHARE_MODE:
		case KEY_FOLLOWER_TOGGLE_SHARE_MODE:
			return TRUE;
		case KEY_FOLLOWER_STOPS_SHARE_MODE:
			DBG_LOG_APP_MCSYNC("[APP_MCSync] follower stop key, state:%d", 1, app_MCSync_Share_GetState());
			if(app_MCSync_Share_GetState() == APP_SHARE_STATE_ENTERED_MODE)
=======
		case KEY_MASTER_SHARE_MODE_START:
		case KEY_MASTER_SHARE_MODE_STOP:
		case KEY_FOLLOWER_SHARE_MODE_START:
		case KEY_MASTER_SHARE_MODE_TOGGLE:
		case KEY_FOLLOWER_SHARE_MODE_TOGGLE:
			return TRUE;
		case KEY_FOLLOWER_SHARE_MODE_STOP:
			DBG_LOG_APP_MCSYNC("[APP_MCSync] follower stop key, state:%d", 1, APP_MCSync_Share_GetState());
			if(APP_MCSync_Share_GetState() == APP_SHARE_STATE_ENTERED_MODE)
>>>>>>> db20e11 (second commit)
				return TRUE;
			else
				return FALSE;
        default:
            return FALSE;
    }    
}

BOOL APP_MCSync_Share_IsKeyEventAllowed(U16 keyEventCode)
{
	BOOL value = TRUE;
	U8 shareMode = BtMCSync_GetShareMode();
	
	if(shareMode == MCSYNC_SHARE_MODE_FOLLOWER_ENABLE)
	{
		switch(keyEventCode)
		{
			case KEY_POWER_OFF:
<<<<<<< HEAD
			case KEY_FOLLOWER_STOPS_SHARE_MODE:
			case KEY_FOLLOWER_TOGGLE_SHARE_MODE:
=======
			case KEY_FOLLOWER_SHARE_MODE_STOP:
			case KEY_FOLLOWER_SHARE_MODE_TOGGLE:
>>>>>>> db20e11 (second commit)
				value = TRUE;
				break;
				
			default:
				value = FALSE;
				DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] IsKeyEventAllowed:%d", 1, keyEventCode);
				break;//return FALSE;
		}
	}
	
	return value;
}


void APP_MCSync_Share_InquiryParaIndHandler(S8 rssi, BD_ADDR_T *pBdAddr)
{
	DBG_LOG_APP_MCSYNC( "[APP_MCSync][Share] InquiryParaIndHandler BdAddr:0x%x%x", 2, FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE));
	DBG_LOG_APP_MCSYNC( "[APP_MCSync][Share] rssi:%d, paraRSSI:%d, threshold:%d", 3, rssi, gAppShareModeCtrl.pairingInfo.rssi, APP_MCSync_Nvkey_SharePairingRssiThreshold());

	if(gAppShareModeCtrl.pairingInfo.isFoundRssi == 0)
	{
		if(rssi > APP_MCSync_Nvkey_SharePairingRssiThreshold())
		{
			FW_MakeSpecialBDA(pBdAddr);
			app_MCSync_Share_SetSharePairingInfo(rssi, pBdAddr);
            app_MCSync_Share_ResetPairingTimer();
		}
	}
}

void APP_MCSync_Share_WriteInquiryModeCfmHandler(U8 status)
{
    if(	status == BT_STATUS_SUCCESS)
    {
        app_MCSync_Share_WriteSharePairingLIAC();
    }
    else
    {
        app_MCSync_Share_StopSharePairing();
    }    
}

void APP_MCSync_Share_WriteCurentIACCfmHandler(U8 status)
{
    if(	status == BT_STATUS_SUCCESS)
    {
        app_MCSync_Share_SharePairingInquiry();
    }
    else
    {
        app_MCSync_Share_StopSharePairing();
    }    
}

void APP_MCSync_Share_SyncFollowerDiscStatusHandler(U8 isFollowerIFPkt, U8 isOnlyForFollower)
{
	U8 shareMode = BtMCSync_GetShareMode();
<<<<<<< HEAD
	U8 plusState = app_MCSync_Share_GetState();
=======
	U8 plusState = APP_MCSync_Share_GetState();
>>>>>>> db20e11 (second commit)

	DBG_LOG_APP_MCSYNC( "[APP_MCSync][Share] FollowerDiscStatusHandler, role:0x%x, share mode:%d, share state:%d, isFPkt:%d", 
		4, BtAwsMce_GetDefaultRole(), shareMode, plusState, isFollowerIFPkt);

	UNUSED(isOnlyForFollower);
	if(BtAwsMce_IsDefaultRoleAgent() && shareMode == MCSYNC_SHARE_MODE_NORMAL_ENABLE
        && plusState == APP_SHARE_STATE_ENTERED_MODE && isFollowerIFPkt == TRUE)
    {
<<<<<<< HEAD
    	DBG_LOG_APP_MCSYNC( "[APP_MCSync][Share] agent send follower disc 2 times", 0);
		APP_MCSYNC_SendSyncFollowerDisc(FALSE, TRUE);
		FW_SetTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_AGENT_SEND_FOLLOWER_DISC_IF_REPEAT_TIMER, 
			NULL, 0, 100);		
=======
    	DBG_LOG_APP_MCSYNC( "[APP_MCSync][Share] agent send follower disc 3 times", 0);
		APP_MCSYNC_SendSyncFollowerDisc(FALSE, TRUE);
		FW_SetTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_AGENT_SEND_FOLLOWER_DISC_IF_REPEAT_TIMER, 
			NULL, 0, 100);
		FW_SetTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_AGENT_SEND_FOLLOWER_DISC_IF_REPEAT_TIMER, 
			NULL, 0, 200);
>>>>>>> db20e11 (second commit)

	}
    else if(shareMode == MCSYNC_SHARE_MODE_FOLLOWER_ENABLE 
        && plusState == APP_SHARE_STATE_ENTERED_MODE 
        && isFollowerIFPkt == FALSE)
    {
    	if(!FW_CheckTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_FOLLOWER_DISC_TIMER, 0))
    	{
			FW_SetTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_FOLLOWER_DISC_TIMER, NULL, 0, 1800);					  
        	APP_MCSyncPlus_FollowerStop();
    	}
    }  
}

void APP_MCSync_Share_SyncOtherAgentStatusHandler(BD_ADDR_T *pBdAddr)
{
    DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] SyncOtherAgentStatusHandler: BdAddr: 0x%x%x", 2, FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE));
    
    BtMCSync_SetOtherAgentBDA(pBdAddr);
    app_MCSync_Share_SetState(APP_SHARE_STATE_PARTNER_GET_PAIRING_DATA, APP_SHARE_SUBSTATE_NONE);    
}

void APP_MCSync_Share_SyncAgentRetransmitDataHandler(U8 *msg)
{
    AWSMCE_SYNC_AGENT_RETRANSMIT_FOLLOWER_DATA_IND_T *ind = (AWSMCE_SYNC_AGENT_RETRANSMIT_FOLLOWER_DATA_IND_T *)msg;
    AWSMCE_SYNC_FOLLOWER_DISC_IND_T * ptr;
	if(ind)
	{
		switch(ind->type)
		{
			case APP_SHARE_AGENT_RETRANSMIT_FOLLOWER_DISC_IF:
                ptr = (AWSMCE_SYNC_FOLLOWER_DISC_IND_T *)&ind->value;
                APP_MCSync_Share_SyncFollowerDiscStatusHandler(ptr->isFollowerIFPkt, ptr->isOnlyForFollower);
				break;
			
		}
	}    
}

U8 APP_MCSync_Share_FollowerFilter(BD_ADDR_T *pBdAddr, U8 role)
{
	U8 plusState;
	BOOL f1Connected, f2Connected;
    U8 shareMode = BtMCSync_GetShareMode();
		
	if(shareMode != MCSYNC_SHARE_MODE_NORMAL_ENABLE)
	{
		return role;
	}
	
<<<<<<< HEAD
	plusState = app_MCSync_Share_GetState();
	
	DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Follower Filter: role:0x%x, plusState:%d", 2, role, plusState);
	

=======
	plusState = APP_MCSync_Share_GetState();
	
	DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Follower Filter: role:0x%x, plusState:%d, share linkloss:%d", 3, 
		role, plusState, APP_MCSync_Share_GetLinkLoss());
	
>>>>>>> db20e11 (second commit)
	if(role == 0x51 && APP_MCSync_Share_GetLinkLoss() &&
		APP_Conn_GetAgLinkState() != LINK_CONNECTED)
	{
		app_MCSync_Share_AgentStop();
		APP_MCSync_Share_SetLinkLoss(TRUE);
<<<<<<< HEAD
=======
		APP_State_ConnectabilityHandle(CMD_SET_CONNECTABLE);
>>>>>>> db20e11 (second commit)
	}
	
	if(role & SHARE_FOLLOWER_NUMBER_1_FILTER)
	{
		DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Follower Filter, release PUSH_SHARE_PAIRING_MODE_MEDIA timer 1", 0);
		FW_ReleaseTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_PUSH_SHARE_PAIRING_MODE_MEDIA, 0);

		
		FW_ReleaseTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_IN_SHARE_PAIRING_MODE, 0);

		BtMCSyunc_SetMCSyncSharePairing(FALSE);
		
		app_MCSync_Share_SetFollowerConnected(FOLLOWER_1_CONNECTED);
		f1Connected = TRUE;
		role = role & ~SHARE_FOLLOWER_NUMBER_1_FILTER;
	}
	else
	{
		app_MCSync_Share_SetFollowerConnected(FOLLOWER_1_DISCONNECTED);
		f1Connected = FALSE;
	}
	
	if(role & SHARE_FOLLOWER_NUMBER_2_FILTER)
	{
		DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] Follower Filter, release PUSH_SHARE_PAIRING_MODE_MEDIA timer 2", 0);
		FW_ReleaseTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_PUSH_SHARE_PAIRING_MODE_MEDIA, 0);
		FW_ReleaseTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_IN_SHARE_PAIRING_MODE, 0);

		BtMCSyunc_SetMCSyncSharePairing(FALSE);
		
		app_MCSync_Share_SetFollowerConnected(FOLLOWER_2_CONNECTED);
		f2Connected = TRUE;
		role = role & ~SHARE_FOLLOWER_NUMBER_2_FILTER;
	}
	else
	{
		app_MCSync_Share_SetFollowerConnected(FOLLOWER_2_DISCONNECTED);
		f2Connected = FALSE;
	}
	
	if((f1Connected == TRUE || f2Connected == TRUE) && 
		(plusState == APP_SHARE_STATE_PREPARING || plusState == APP_SHARE_STATE_SHARE_PAIRING_START
		|| plusState == APP_SHARE_STATE_ENTERED_MODE || plusState == APP_SHARE_STATE_NONE))
	{
		FW_ReleaseTimer(&gAppMCSyncShareTimerHandle, APP_SHARE_TIMER_SHARE_AGENT_RECONNECT, 0);
<<<<<<< HEAD
=======

		PM_Sniff_SetLinkPolicyDisableAllMode(APP_GetServiceBdAddr());
		BtSetAclDetachTimeout(APP_GetServiceBdAddr()->addr, 2500);
>>>>>>> db20e11 (second commit)
		
		app_MCSync_Share_SetState(APP_SHARE_STATE_ENTERED_MODE, APP_SHARE_SUBSTATE_NONE);
		
		APP_MCSYNC_SendSyncShareModeInfo(APP_SHARE_STATE_ENTERED_MODE, MCSYNC_SHARE_MODE_NORMAL_ENABLE);

		if(pBdAddr)
		{
			APP_MCSYNC_SendSyncA2dpInfo(pBdAddr, MCSYNC_SHARE_SPECIFIC_ROLE_NOT_USED);
		}
	}
	
	if(f1Connected == FALSE && f2Connected == FALSE && (plusState == APP_SHARE_STATE_ENTERED_MODE || plusState == APP_SHARE_STATE_LEAVING))
	{
		app_MCSync_Share_SetState(APP_SHARE_STATE_PREPARING, APP_SHARE_SUBSTATE_NONE);
		APP_MCSYNC_SendSyncShareModeInfo(APP_SHARE_STATE_PREPARING, MCSYNC_SHARE_MODE_NORMAL_ENABLE);
	}
	
	return role;
}

void APP_MCSync_Share_SetLinkLoss(U8 isValue)
{
	DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] sp linkloss:%d -> %d", 2, gAppShareModeCtrl.splinkloss, isValue);
	gAppShareModeCtrl.splinkloss = isValue;
}

U8 APP_MCSync_Share_GetLinkLoss(void)
{
	return gAppShareModeCtrl.splinkloss;
}


/**************************************************************************************************
* Public (Share NVKEY)
**************************************************************************************************/
<<<<<<< HEAD
=======
U8 APP_MCSync_Share_GetState(void)
{
	return gAppShareModeCtrl.state;
}

bool APP_MCSync_IsShareConn(void)
{
	if (APP_MCSync_Share_GetState()==APP_SHARE_STATE_ENTERED_MODE)
		return TRUE;
	else
		return FALSE;
}
>>>>>>> db20e11 (second commit)

#endif
