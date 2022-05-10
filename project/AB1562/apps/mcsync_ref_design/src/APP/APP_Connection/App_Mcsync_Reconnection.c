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
#include "bt_aws_mce.h"
#include "BtMCSync.h"
#include "MCSync.h"
#include "App_Mcsync_Reconnection.h"
#include "App_ReConnection.h"
#include "PM_Interface.h"
#include "App_ResourceManager.h"
#include "app_mcsync_reconnect_nvstru.h"
#include "App_System.h"
#include "Power_NvkeyHandler.h"
#include "App_SmartChgCase.h"
#include "App_State.h"
#include "App_PowerOff.h"
#include "App_LinkKey.h"
#include "App_Interface.h"
#include "App_Connection.h"
#include "App_MCSync_Message.h"

#if SUPER_RECONNECT_MODE
/**************************************************************************************************
* Definition
**************************************************************************************************/
#define LONG_RECONNECT_TIME_MIN		gpNvkeyMcsReconn->longReconnTimeMin 	//3000 	//ms
#define LONG_RECONNECT_TIME_MAX		gpNvkeyMcsReconn->longReconnTimeMax 	//4000 	//ms
#define LONG_RECONNECT_TIME_DIFF	gpNvkeyMcsReconn->longReconnTimeDiff 	//200 	//ms

#define SHORT_RECONNECT_TIME_MIN	gpNvkeyMcsReconn->shortReconnTimeMin 	//1000 	//ms
#define SHORT_RECONNECT_TIME_MAX	gpNvkeyMcsReconn->shortReconnTimeMax 	//2000 	//ms
#define SHORT_RECONNECT_TIME_DIFF	gpNvkeyMcsReconn->shortReconnTimeDiff 	//300 	//ms

#define DELAY_RECONNECT_AG_TIME		6000 //ms

/**************************************************************************************************
* Enumeration
**************************************************************************************************/
enum
{
	MCS_RECONN_STATE_IDLE,							//0
	MCS_RECONN_STATE_WAIT_TO_PARTNER,
	MCS_RECONN_STATE_WAIT_TO_AGENT,
	MCS_RECONN_STATE_PARTNER_RECONNECTING,
	MCS_RECONN_STATE_AGENT_RECONNECTING,
	MCS_RECONN_STATE_AGENT_IDLE,					//5
	MCS_RECONN_STATE_AGENT_WAIT_RECONNECT_FAILED,
	MCS_RECONN_STATE_CANCELING,
};

enum
{
	MCS_RECONN_ACTION_SET_TO_PARTNER,
	MCS_RECONN_ACTION_SET_TO_AGENT,
	MCS_RECONN_ACTION_RECONNECT,
};

enum
{
	MCS_RECONN_EVENT_SET_PARTNER_SUCCESSFUL,	//0
	MCS_RECONN_EVENT_SET_PARTNER_FAILED,
	MCS_RECONN_EVENT_SET_AGENT_SUCCESSFUL,
	MCS_RECONN_EVENT_SET_AGENT_FAILED,
	MCS_RECONN_EVENT_RECONNECT_TIMEOUT,
	MCS_RECONN_EVENT_RECONNECT_FAILED,			//5
	MCS_RECONN_EVENT_RECONNECT_FAILED_ALREADY_EXIST,
	MCS_RECONN_EVENT_RECONNECT_FAILED_CONN_ACCEPT_TO,
	MCS_RECONN_EVENT_AGENT_WITH_PARTNER,
	MCS_RECONN_EVENT_AGENT_WITH_AG,
	MCS_RECONN_EVENT_CANCEL_MODE,
<<<<<<< HEAD
	MCS_RECONN_EVENT_RECONNECT_FAILED_LMP_RESPONSE_TIMEOUT,
=======
	MCS_RECONN_EVENT_RECONNECT_FAIL_LMP_RESPONSE_TIMEOUT,
>>>>>>> db20e11 (second commit)
};

enum
{
	ID_RECONNECT_TIMEOUT,
	ID_RECONNECT_NOTIFY,
	ID_ACTION_HANDLER,
	ID_STATE_HANDLER,
	ID_ENTER_SUPER_RECONNECT,
	ID_SEND_SYNC_NOT_RECONNECT_MASK,
};

/**************************************************************************************************
* Sturcture
**************************************************************************************************/
typedef struct
{
	U8 state;
	U32 randomTimeAgent;
	U32 randomTimePartner;
	U8 reconnType;
	U8 isNeedBackRole:1;
	U8 reserved:7;
	U8 reconnRole;
	U8 notReconnMask;
	BD_ADDR_T linkLossBdAddr;
}APP_MCS_RECONN_CTL_STRU;

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static BOOL app_Mcsync_IsConnToAGNeeded(void);
static void app_Mcsync_SetReconnectState(U8 state);
static U8 app_Mcsync_GetReconnectState(void);
static void app_Mcsync_SetReconnectRandomTime(U8 role, U32 time);
static U32 app_Mcsync_GetReconnectRandomTime(U8 role);
static void app_Mcsync_ClearReconnectRandomTime(void);
static void app_Mcsync_SetReconnType(U8 reconnType);
static U8 app_Mcsync_GetReconnType(void);
static U8 app_Mcsync_GetReconnStartRole(void);
static BD_ADDR_T *app_Mcsync_GetLinkLossBdAddr(void);
static void app_Mcsync_SetReconnRandomTime(void);
static void app_Mcsync_Reconnect(void);
static void app_Mcsync_SuperReconnectModeEnd(void);
static void app_Mcsync_ReconnectParaInit(void);
static void app_Mcsync_NvkeyInit(void);

static U32 app_Mcsync_SuperReconnectHandler(Handler handler, U16 id, void *msg, U32 id_ext);
static void app_Mcsync_ReconnectStateHandler(U8 event);
static void app_Mcsync_ReconnectActionHandler(U8 action);

/**************************************************************************************************
* Variable
**************************************************************************************************/
static APP_MCS_RECONN_CTL_STRU gAppMcsReconnCtl;
static APP_MCS_RECONNECT_NVKEY_STRU *gpNvkeyMcsReconn;
static const HandlerData gAppMcsSuperReconnectHandle = { app_Mcsync_SuperReconnectHandler };
extern HandlerData gAppMCSyncSystemHandle;

/**************************************************************************************************
* Static Functions
**************************************************************************************************/
static BOOL app_Mcsync_IsConnToAGNeeded(void)
{
	DBG_LOG_APP_Connection( "[APP MCS Reconn] Is Need Reconnect AG. Not Reconnect Mask:0x%x", 1, gAppMcsReconnCtl.notReconnMask);

	return (!gAppMcsReconnCtl.notReconnMask) ? TRUE : FALSE;
}

static void app_Mcsync_SetReconnectState(U8 state)
{
	DBG_LOG_APP_Connection( "[APP MCS Reconn] set reconnect state:%d -> %d", 2, gAppMcsReconnCtl.state, state);
	gAppMcsReconnCtl.state = state;
}

static U8 app_Mcsync_GetReconnectState(void)
{
	return gAppMcsReconnCtl.state;
}

static void app_Mcsync_SetReconnectRandomTime(U8 role, U32 time)
{
	if(role == ROLE_AGENT)
		gAppMcsReconnCtl.randomTimeAgent = time;
	else if(role == ROLE_PARTNER)
		gAppMcsReconnCtl.randomTimePartner = time;
}

static U32 app_Mcsync_GetReconnectRandomTime(U8 role)
{
	if(role == ROLE_AGENT)
		return gAppMcsReconnCtl.randomTimeAgent;
	else if(role == ROLE_PARTNER)
		return gAppMcsReconnCtl.randomTimePartner;
	else
		return 0;
}

static void app_Mcsync_ClearReconnectRandomTime(void)
{
	app_Mcsync_SetReconnectRandomTime(ROLE_AGENT, 0);
	app_Mcsync_SetReconnectRandomTime(ROLE_PARTNER, 0);
}

static void app_Mcsync_SetReconnType(U8 reconnType)
{
	gAppMcsReconnCtl.reconnType = reconnType;
}

static U8 app_Mcsync_GetReconnType(void)
{
	return gAppMcsReconnCtl.reconnType;
}

static U8 app_Mcsync_GetReconnStartRole(void)
{
	return gAppMcsReconnCtl.reconnRole;
}

static BD_ADDR_T *app_Mcsync_GetLinkLossBdAddr(void)
{
	if(!FW_IsBdAddrZero(&gAppMcsReconnCtl.linkLossBdAddr))
		return &gAppMcsReconnCtl.linkLossBdAddr;

	return NULL;
}

static void app_Mcsync_SetReconnRandomTime(void)
{
	BD_ADDR_T *pAgentBdAddr, *pDeviceBdAddr;
	U32 seed, agentTime, partnerTime;

	if(app_Mcsync_GetReconnectRandomTime(ROLE_AGENT) && app_Mcsync_GetReconnectRandomTime(ROLE_PARTNER))
		return;

	pAgentBdAddr = BtMCSync_ReadAgentBdAddr();
	pDeviceBdAddr = BtMCSync_ReadDeviceBdAddr();
	FW_Memcpy(&seed, &pDeviceBdAddr->addr[2], sizeof(U32));

	if(FW_CmpBdAddr(pAgentBdAddr, pDeviceBdAddr))
	{
		agentTime = FW_GetRandomNum(LONG_RECONNECT_TIME_MIN, LONG_RECONNECT_TIME_MAX, LONG_RECONNECT_TIME_DIFF, seed);
		partnerTime = FW_GetRandomNum(SHORT_RECONNECT_TIME_MIN, SHORT_RECONNECT_TIME_MAX, SHORT_RECONNECT_TIME_DIFF, seed);
	}
	else
	{
		partnerTime = FW_GetRandomNum(LONG_RECONNECT_TIME_MIN, LONG_RECONNECT_TIME_MAX, LONG_RECONNECT_TIME_DIFF, seed);
		agentTime = FW_GetRandomNum(SHORT_RECONNECT_TIME_MIN, SHORT_RECONNECT_TIME_MAX, SHORT_RECONNECT_TIME_DIFF, seed);
	}

	DBG_LOG_APP_Connection( "[APP MCS Reconn] Set Reconnect Time. Agent:%d ms, Partner:%d ms", 2, agentTime, partnerTime);

	app_Mcsync_SetReconnectRandomTime(ROLE_AGENT, agentTime);
	app_Mcsync_SetReconnectRandomTime(ROLE_PARTNER, partnerTime);
}

static void app_Mcsync_Reconnect(void)
{
	if(BtAwsMce_IsDefaultRolePartner())
	{
		if(!FW_CheckTimer((Handler)&gAppMcsSuperReconnectHandle, ID_RECONNECT_TIMEOUT, 0))
		{
			FW_SetTimer((Handler)&gAppMcsSuperReconnectHandle, ID_RECONNECT_TIMEOUT, NULL, 0, app_Mcsync_GetReconnectRandomTime(ROLE_PARTNER));
		}

		app_Mcsync_SetReconnectState(MCS_RECONN_STATE_PARTNER_RECONNECTING);
		APP_ReConn_Reconnect(RECONNECT_AWSMCE); //if already has sent AWS create connection???
	}
	else if(BtAwsMce_IsDefaultRoleAgent())
	{
		if(!FW_CheckTimer((Handler)&gAppMcsSuperReconnectHandle, ID_RECONNECT_TIMEOUT, 0))
		{
			FW_SetTimer((Handler)&gAppMcsSuperReconnectHandle, ID_RECONNECT_TIMEOUT, NULL, 0, app_Mcsync_GetReconnectRandomTime(ROLE_AGENT));
		}

		if(app_Mcsync_IsConnToAGNeeded())
		{
			if(APP_PairNvkey_IsFeatureOn(APP_DISCOVER_MODE_AFTER_RECOVERY_FEAT))
			{
				APP_State_DiscoverabilityHandle(CMD_SET_NORMAL_DISCOVERABLE);
			}
			else
			{
<<<<<<< HEAD
=======
                APP_State_DiscoverabilityHandle(CMD_SET_NON_DISCOVERABLE);
>>>>>>> db20e11 (second commit)
				APP_State_ConnectabilityHandle(CMD_SECURITY_MODE_COMPLETE);
			}

			app_Mcsync_SetReconnectState(MCS_RECONN_STATE_AGENT_RECONNECTING);

			if(RECONNECT_AWSMCE == app_Mcsync_GetReconnType())
			{
				APP_ReConn_Reconnect(RECONNECT_USER_INIT);
			}
			else if(RECONNECT_LINK_LOSS == app_Mcsync_GetReconnType())
			{
				APP_Reconn_SetLinkLossBdAddr(app_Mcsync_GetLinkLossBdAddr());
				APP_ReConn_Reconnect(RECONNECT_LINK_LOSS);
			}
			else
			{
				APP_ReConn_Reconnect(app_Mcsync_GetReconnType()); //reconnect AG if has link key
			}

			APP_ReConn_Reconnect(RECONNECT_CHECK);
		}
		else
		{
			if(APP_SmtChgCse_IsOutOfChargerCase())
			{
				if(APP_PairNvkey_IsFeatureOn(APP_DISCOVER_MODE_AFTER_RECOVERY_FEAT))
				{
					APP_State_DiscoverabilityHandle(CMD_SET_NORMAL_DISCOVERABLE);
				}
				else
				{
<<<<<<< HEAD
=======
                    APP_State_DiscoverabilityHandle(CMD_SET_NON_DISCOVERABLE);
>>>>>>> db20e11 (second commit)
					APP_State_ConnectabilityHandle(CMD_SET_CONNECTABLE);
				}
			}
			else
			{
<<<<<<< HEAD
=======
                APP_State_DiscoverabilityHandle(CMD_SET_NON_DISCOVERABLE);
>>>>>>> db20e11 (second commit)
				APP_State_ConnectabilityHandle(CMD_SET_NON_CONNECTABLE);
			}

			app_Mcsync_SetReconnectState(MCS_RECONN_STATE_AGENT_IDLE);
		}
	}
}

static void app_Mcsync_SuperReconnectModeEnd(void)
{
	DBG_LOG_APP_Connection( "[APP MCS Reconn] Super Reconnect End", 0);
	FW_ReleaseTimer((Handler)&gAppMcsSuperReconnectHandle, ID_RECONNECT_TIMEOUT, 0);
	if(MSG_MessageSearchAllEx3((Handler)&gAppMcsSuperReconnectHandle, ID_ENTER_SUPER_RECONNECT))
	{
		MSG_MessageCancelAllHandler((Handler)&gAppMcsSuperReconnectHandle);
<<<<<<< HEAD
		FW_SetTimer((Handler)&gAppMcsSuperReconnectHandle, ID_ENTER_SUPER_RECONNECT, NULL, RECONNECT_USER_INIT, 80);
=======
		FW_SetTimer((Handler)&gAppMcsSuperReconnectHandle, ID_ENTER_SUPER_RECONNECT, NULL, RECONNECT_USER_INIT, 10);
>>>>>>> db20e11 (second commit)
	}
	else
	{
		MSG_MessageCancelAllHandler((Handler)&gAppMcsSuperReconnectHandle);
	}
	app_Mcsync_ReconnectParaInit();
	APP_System_RoleModeLeaveInd(APP_ROLE_MODE_SUPER_RECONNECT);
}

static void app_Mcsync_ReconnectParaInit(void)
{
	U8 notReconnMask = gAppMcsReconnCtl.notReconnMask;

	FW_Memset(&gAppMcsReconnCtl, 0, sizeof(APP_MCS_RECONN_CTL_STRU));
	gAppMcsReconnCtl.reconnRole = MCS_RECONN_START_PARTNER;
	gAppMcsReconnCtl.notReconnMask = notReconnMask;
}

static void app_Mcsync_NvkeyInit(void)
{
	APP_MCS_RECONNECT_NVKEY_STRU *pNvkeyMcsReconn = (APP_MCS_RECONNECT_NVKEY_STRU *)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_SUPER_RECONNECT_MODE);

	if(!pNvkeyMcsReconn)
	{
		DBG_LOG_APP_Connection( "[APP MCS Reconn] Init No Nvkey", 0);
		FW_Assert(FALSE);
	}

	gpNvkeyMcsReconn = pNvkeyMcsReconn;
}

/**************************************************************************************************
* Static Functions (Handler)
**************************************************************************************************/
static U32 app_Mcsync_SuperReconnectHandler(Handler handler, U16 id, void *msg, U32 id_ext)
{
	U8 state = app_Mcsync_GetReconnectState();

	UNUSED(handler);
	UNUSED(msg);

	switch(id)
	{
		case ID_RECONNECT_TIMEOUT:
			DBG_LOG_APP_Connection( "[APP MCS Reconn] Reconnect Timeout. state:%s", 1, APP_McsReconn_State_String[state]);
<<<<<<< HEAD
			#ifdef MCSYNC_SHARE_MODE
=======
			#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
			if( MCSYNC_SHARE_MODE_DISABLE != BtMCSync_GetShareMode())
			{
				app_Mcsync_SuperReconnectModeEnd();
				break;
			}
			#endif
			app_Mcsync_ClearReconnectRandomTime();
			app_Mcsync_ReconnectStateHandler(MCS_RECONN_EVENT_RECONNECT_TIMEOUT);
			break;

		case ID_RECONNECT_NOTIFY:
			if(state == MCS_RECONN_STATE_PARTNER_RECONNECTING || state == MCS_RECONN_STATE_AGENT_RECONNECTING)
				APP_Mcsync_ReconnectNotify(id_ext);
			else
				return 2;
			break;

		case ID_ACTION_HANDLER:
			app_Mcsync_ReconnectActionHandler(id_ext);
			break;

		case ID_STATE_HANDLER:
			app_Mcsync_ReconnectStateHandler(id_ext);
			break;

		case ID_ENTER_SUPER_RECONNECT:
			//APP_Mcsync_EnterSuperReconnectMode(id_ext);
			APP_ReConn_Reconnect(id_ext);
			break;

		case ID_SEND_SYNC_NOT_RECONNECT_MASK:
			APP_MCSYNC_SendReconnectMask(id_ext);
			break;
	}

	return 0;
}

static void app_Mcsync_ReconnectStateHandler(U8 event)
{
	U8 state = app_Mcsync_GetReconnectState();

<<<<<<< HEAD
	DBG_LOG_APP_Connection( "[APP MCS Reconn] State Handler. event:%s, state:%s", 2, APP_McsReconn_Event_String[event], APP_McsReconn_State_String[state]);
=======
	DBG_LOG_APP_Connection( "[APP MCS Reconn] State Handler. event(%d):%s, state(%d):%s", 4, event, APP_McsReconn_Event_String[event], state, APP_McsReconn_State_String[state]);
>>>>>>> db20e11 (second commit)

	switch(state)
	{
		case MCS_RECONN_STATE_IDLE:
<<<<<<< HEAD
			if(event == MCS_RECONN_EVENT_RECONNECT_FAILED_LMP_RESPONSE_TIMEOUT)
			{
				if(APP_System_SetRoleMode(APP_ROLE_MODE_SUPER_RECONNECT, ROLE_PARTNER))
					app_Mcsync_SetReconnectState(MCS_RECONN_STATE_WAIT_TO_PARTNER);
				else
					app_Mcsync_SuperReconnectModeEnd();
			}
=======
>>>>>>> db20e11 (second commit)
			break;

		case MCS_RECONN_STATE_WAIT_TO_PARTNER:
			if(event == MCS_RECONN_EVENT_SET_PARTNER_SUCCESSFUL ||
				event == MCS_RECONN_EVENT_SET_PARTNER_FAILED)
			{
				if(LINK_CONNECTED == PM_Conn_GetAgLinkState() || MCSYNC_LINK_CONNECTED == BtAwsMce_GetMcsyncState())
					app_Mcsync_SuperReconnectModeEnd();
				else
				{
					APP_ReConn_ClearCtl();
					app_Mcsync_ReconnectActionHandler(MCS_RECONN_ACTION_RECONNECT);
				}
			}
			else if(event == MCS_RECONN_EVENT_AGENT_WITH_PARTNER ||
				event == MCS_RECONN_EVENT_AGENT_WITH_AG)
			{
				DBG_LOG_APP_Connection( "[APP MCS Reconn] isReconn:%d, AgLinkState:%d, isNeedAg:%d", 3, APP_Reconn_IsReconnecting(),
					PM_Conn_GetAgLinkState(), app_Mcsync_IsConnToAGNeeded());

				//if partner connected before the first time of role change mode, has to reconnect to AG.
				if(LINK_DISCONNECTED == PM_Conn_GetAgLinkState() && app_Mcsync_IsConnToAGNeeded())
				{
					APP_Reconn_SetDelayTimer(RECONNECT_USER_INIT, 0);
				}

				app_Mcsync_SuperReconnectModeEnd();
			}
			break;

		case MCS_RECONN_STATE_WAIT_TO_AGENT:
			if(event == MCS_RECONN_EVENT_SET_AGENT_SUCCESSFUL ||
				event == MCS_RECONN_EVENT_SET_AGENT_FAILED)
			{
				app_Mcsync_ReconnectActionHandler(MCS_RECONN_ACTION_RECONNECT);
			}
			else if(event == MCS_RECONN_EVENT_AGENT_WITH_PARTNER ||
				event == MCS_RECONN_EVENT_AGENT_WITH_AG)
			{
				app_Mcsync_SuperReconnectModeEnd();
			}
			break;

		case MCS_RECONN_STATE_PARTNER_RECONNECTING:
			if(event == MCS_RECONN_EVENT_RECONNECT_TIMEOUT)
			{
				//cancel reconnect agent, will be done at "BtMCSync_AgentMode".
				if(MCSYNC_LINK_CONNECTED != BtAwsMce_GetMcsyncState())
				{
					APP_ReConn_ClearCtl();
					app_Mcsync_ReconnectActionHandler(MCS_RECONN_ACTION_SET_TO_AGENT);
				}
				else
				{
					app_Mcsync_SuperReconnectModeEnd();
				}
			}
			else if(event == MCS_RECONN_EVENT_AGENT_WITH_PARTNER ||
				event == MCS_RECONN_EVENT_AGENT_WITH_AG)
			{
				app_Mcsync_SuperReconnectModeEnd();
			}
			break;

		case MCS_RECONN_STATE_AGENT_RECONNECTING:
			if(event == MCS_RECONN_EVENT_RECONNECT_TIMEOUT)
			{
				app_Mcsync_SetReconnectState(MCS_RECONN_STATE_AGENT_WAIT_RECONNECT_FAILED);

				if(APP_Reconn_IsReconnecting())
				{
					APP_Reconn_ReconnectSetSuspend(TRUE);
				}
				else
				{
					FW_MessageSendEx((Handler)&gAppMcsSuperReconnectHandle, ID_STATE_HANDLER, NULL, MCS_RECONN_EVENT_RECONNECT_FAILED);
				}
			}
			else if(event == MCS_RECONN_EVENT_AGENT_WITH_PARTNER ||
				event == MCS_RECONN_EVENT_AGENT_WITH_AG)
			{
				app_Mcsync_SuperReconnectModeEnd();
			}
			break;

		case MCS_RECONN_STATE_AGENT_IDLE:
			if(event == MCS_RECONN_EVENT_RECONNECT_TIMEOUT)
			{
				if(MCSYNC_LINK_CONNECTED != BtAwsMce_GetMcsyncState())
					app_Mcsync_ReconnectActionHandler(MCS_RECONN_ACTION_SET_TO_PARTNER);
				else
					app_Mcsync_ReconnectActionHandler(MCS_RECONN_ACTION_RECONNECT);
			}
			else if(event == MCS_RECONN_EVENT_AGENT_WITH_PARTNER ||
				event == MCS_RECONN_EVENT_AGENT_WITH_AG)
			{

				DBG_LOG_APP_Connection( "[APP MCS Reconn] AGENT_IDLE isReconn:%d, AgLinkState:%d, isNeedAg:%d", 3,
					APP_Reconn_IsReconnecting(), PM_Conn_GetAgLinkState(), app_Mcsync_IsConnToAGNeeded());

				if(event == MCS_RECONN_EVENT_AGENT_WITH_PARTNER && LINK_DISCONNECTED == PM_Conn_GetAgLinkState() && app_Mcsync_IsConnToAGNeeded())
				{
					APP_Reconn_SetDelayTimer(RECONNECT_USER_INIT, 0);
				}
				app_Mcsync_SuperReconnectModeEnd();
			}
			break;

		case MCS_RECONN_STATE_AGENT_WAIT_RECONNECT_FAILED:
			if(event == MCS_RECONN_EVENT_RECONNECT_FAILED || event == MCS_RECONN_EVENT_RECONNECT_FAILED_ALREADY_EXIST ||
				event == MCS_RECONN_EVENT_RECONNECT_FAILED_CONN_ACCEPT_TO)
			{
				APP_ReConn_ClearCtl();
				APP_Reconn_ReconnectSetSuspend(FALSE);

				if(MCSYNC_LINK_CONNECTED != BtAwsMce_GetMcsyncState())
				{
					if(event == MCS_RECONN_EVENT_RECONNECT_FAILED_CONN_ACCEPT_TO)
					{
						app_Mcsync_SetReconnectRandomTime(ROLE_AGENT, 1);
						app_Mcsync_SetReconnectRandomTime(ROLE_PARTNER, DELAY_RECONNECT_AG_TIME);
					}
					//cancel LS will be done at "BtMCSync_PartnerMode".
					/* base on BTA-5446 & BTA-5295 remove below code */
					/*app_Mcsync_ReconnectActionHandler(MCS_RECONN_ACTION_SET_TO_PARTNER);*/
                    FW_SetTimer((Handler)&gAppMcsSuperReconnectHandle, ID_ACTION_HANDLER, NULL, MCS_RECONN_ACTION_SET_TO_PARTNER, 800);
				}
				else
				{
					//have to stop?
					app_Mcsync_ReconnectActionHandler(MCS_RECONN_ACTION_RECONNECT);
				}
			}
			else if(event == MCS_RECONN_EVENT_AGENT_WITH_PARTNER ||
				event == MCS_RECONN_EVENT_AGENT_WITH_AG)
			{
				APP_Reconn_ReconnectSetSuspend(FALSE);
				app_Mcsync_SuperReconnectModeEnd();
<<<<<<< HEAD
=======
                if(event == MCS_RECONN_EVENT_AGENT_WITH_PARTNER && LINK_DISCONNECTED == PM_Conn_GetAgLinkState() && app_Mcsync_IsConnToAGNeeded())
                {
                    APP_Reconn_SetDelayTimer(RECONNECT_USER_INIT, 0);
                }
>>>>>>> db20e11 (second commit)
			}
			break;

		case MCS_RECONN_STATE_CANCELING:
			//back to original role
			if(event == MCS_RECONN_EVENT_CANCEL_MODE || event == MCS_RECONN_EVENT_RECONNECT_FAILED)
			{
				if(gAppMcsReconnCtl.isNeedBackRole &&
					MCSYNC_LINK_CONNECTED != BtAwsMce_GetMcsyncState() && LINK_DISCONNECTED == PM_Conn_GetAgLinkState())
				{
					if(ROLE_AGENT == BtAwsMce_GetNvKeyRole())
					{
						if(ROLE_AGENT == BtAwsMce_GetDefaultRole() || !APP_System_SetRoleMode(APP_ROLE_MODE_SUPER_RECONNECT, ROLE_AGENT))
							app_Mcsync_SuperReconnectModeEnd();
					}
					else if(ROLE_PARTNER == BtAwsMce_GetNvKeyRole())
					{
						if(ROLE_PARTNER == BtAwsMce_GetDefaultRole() || !APP_System_SetRoleMode(APP_ROLE_MODE_SUPER_RECONNECT, ROLE_PARTNER))
							app_Mcsync_SuperReconnectModeEnd();
					}
				}
				else
				{
					app_Mcsync_SuperReconnectModeEnd();
				}
			}
			break;
	}
}

static void app_Mcsync_ReconnectActionHandler(U8 action)
{
	DBG_LOG_APP_Connection( "[APP MCS Reconn] Action Handler. action:%s", 1, APP_McsReconn_Action_String[action]);

	switch(action)
	{
		case MCS_RECONN_ACTION_SET_TO_PARTNER:
			if(APP_System_SetRoleMode(APP_ROLE_MODE_SUPER_RECONNECT, ROLE_PARTNER))
				app_Mcsync_SetReconnectState(MCS_RECONN_STATE_WAIT_TO_PARTNER);
			else
				app_Mcsync_SuperReconnectModeEnd();
			break;

		case MCS_RECONN_ACTION_SET_TO_AGENT:
			if(APP_System_SetRoleMode(APP_ROLE_MODE_SUPER_RECONNECT, ROLE_AGENT))
				app_Mcsync_SetReconnectState(MCS_RECONN_STATE_WAIT_TO_AGENT);
			else
				app_Mcsync_SuperReconnectModeEnd();
			break;

		case MCS_RECONN_ACTION_RECONNECT:
			app_Mcsync_SetReconnRandomTime();
			app_Mcsync_Reconnect();
			break;
	}
}

#endif //SUPER_RECONNECT_MODE

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
BOOL APP_Mcsync_EnterSuperReconnectMode(U8 type)
{
	#if SUPER_RECONNECT_MODE
	//return TRUE:  Pass through.
	//       FALSE: Enter super reconnect mode.
	U8 defaultRole = BtAwsMce_GetDefaultRole();
	U8 state = app_Mcsync_GetReconnectState();
	U8 agState = PM_Conn_GetLinkState(APP_GetServiceBdAddr());
	U8 mcsState = BtAwsMce_GetMcsyncState();
	U8 reconnRole;

	DBG_LOG_APP_Connection( "[APP MCS Reconn] Super Reconnect Mode. defaultRole:0x%x, type:%d, state:%s, AG:%d", 4, defaultRole, type, APP_McsReconn_State_String[state], agState);
	DBG_LOG_APP_Connection( "[APP MCS Reconn] Super Reconnect Mode. MCS:%d, Feature:%d, SingleHS:%d", 3, mcsState, APP_Mcsync_ReconnectIsFeatureOn(), APP_System_IsSingleHSMode());

	if(!APP_Mcsync_ReconnectIsFeatureOn())
		return TRUE;

<<<<<<< HEAD
	#ifdef MCSYNC_SHARE_MODE
=======
	#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
	if(MCSYNC_SHARE_MODE_DISABLE != BtMCSync_GetShareMode())
		return TRUE;
	#endif

	#if SINGLE_HEADSET_MODE_ENABLE
	if(APP_System_IsSingleHSMode() || APP_State_CheckNestStateByLink(APP_EOF, APP_AWS_PAIRING))
		return TRUE;
	#endif

	if(BtAwsMce_IsDefaultRoleAgent() && type == RECONNECT_AWSMCE)
		return TRUE;

	if(mcsState < MCSYNC_LINK_SAWS_READY)
	{

		DBG_LOG_APP_Connection( "[APP MCS Reconn] Special not ready, reconnect later", 0);
		FW_SetTimer((Handler)&gAppMcsSuperReconnectHandle, ID_ENTER_SUPER_RECONNECT, NULL, type, 700);
		return FALSE;
	}

<<<<<<< HEAD
=======
    DBG_LOG_APP_Connection( "[APP MCS Reconn] ACL disconnecting check: ACL cnt:%d, diconnecting cnt:%d", 2, APP_GetACLLinkCount(), APP_GetLinkActiveDisconnectingCnt());
	if(APP_GetACLLinkCount() && APP_GetLinkActiveDisconnectingCnt())
	{
        DBG_LOG_APP_Connection( "[APP MCS Reconn] ACL disconnecting, reconnect later", 0);
		FW_SetTimer((Handler)&gAppMcsSuperReconnectHandle, ID_ENTER_SUPER_RECONNECT, NULL, type, 1000);
		return FALSE;
	}

>>>>>>> db20e11 (second commit)
	if(mcsState == MCSYNC_LINK_CONNECTED)
		return TRUE;

	if(type != RECONNECT_PWR_ON && type != RECONNECT_USER_INIT && type != RECONNECT_LINK_LOSS && type != RECONNECT_WATCHDOG &&
		(type != RECONNECT_AWSMCE || BtAwsMce_IsDefaultRoleAgent()))
		return TRUE;

	if(state == MCS_RECONN_STATE_PARTNER_RECONNECTING || state == MCS_RECONN_STATE_AGENT_RECONNECTING)
		return TRUE;

	if(APP_Mcsync_IsInSuperReconnectMode())
		return FALSE;

	reconnRole = app_Mcsync_GetReconnStartRole();
	app_Mcsync_SetReconnType(type);

	if(reconnRole == MCS_RECONN_START_AGENT)
	{
		FW_MessageSendEx((Handler)&gAppMcsSuperReconnectHandle, ID_ACTION_HANDLER, NULL, MCS_RECONN_ACTION_SET_TO_AGENT);
		DBG_LOG_APP_Connection( "[APP MCS Reconn] Enter Super Reconnect Mode:%d", 1, 1);
	}
	else if(reconnRole == MCS_RECONN_START_PARTNER)
	{
		FW_MessageSendEx((Handler)&gAppMcsSuperReconnectHandle, ID_ACTION_HANDLER, NULL, MCS_RECONN_ACTION_SET_TO_PARTNER);
		DBG_LOG_APP_Connection( "[APP MCS Reconn] Enter Super Reconnect Mode:%d", 1, 2);
	}
	else
	{
		FW_MessageSendEx((Handler)&gAppMcsSuperReconnectHandle, ID_ACTION_HANDLER, NULL, MCS_RECONN_ACTION_RECONNECT);
		DBG_LOG_APP_Connection( "[APP MCS Reconn] Enter Super Reconnect Mode:%d", 1, 3);
	}

	return FALSE;
	#else
	UNUSED(type);
	return TRUE;
	#endif //SUPER_RECONNECT_MODE
}

BOOL APP_Mcsync_IsInSuperReconnectMode(void)
{
	#if SUPER_RECONNECT_MODE
	if(!APP_Mcsync_ReconnectIsFeatureOn())
		return FALSE;

	return (MCS_RECONN_STATE_IDLE == app_Mcsync_GetReconnectState()) ? FALSE : TRUE;
	#else
	return FALSE;
	#endif
}

void APP_Mcsync_ReconnectNotify(U8 connectDev)
{
	#if SUPER_RECONNECT_MODE
	U8 defaultRole = BtAwsMce_GetDefaultRole();
	U8 state = app_Mcsync_GetReconnectState();

	if(!APP_Mcsync_ReconnectIsFeatureOn())
		return;

<<<<<<< HEAD
	#ifdef MCSYNC_SHARE_MODE
=======
	#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
	if(MCSYNC_SHARE_MODE_DISABLE != BtMCSync_GetShareMode())
		return;
	#endif

	DBG_LOG_APP_Connection( "[APP MCS Reconn] Reconnect Notify. state:%s, defaultRole:0x%x, connectDev:%d", 3, APP_McsReconn_State_String[state], defaultRole, connectDev);

    if(state == MCS_RECONN_STATE_IDLE)
    {
        if(connectDev == MCS_RECONN_DEVICE_PARTNER || connectDev == MCS_RECONN_DEVICE_AG)
        {
            app_Mcsync_SuperReconnectModeEnd();
        }
        return;
    }

	switch(connectDev)
	{
		case MCS_RECONN_DEVICE_AGENT:
			if(defaultRole == ROLE_PARTNER)
			{
				//send reconnect state to agent.
				if(app_Mcsync_GetLinkLossBdAddr())
				{
					APP_MCSYNC_SendReconnectState(RECONNECT_LINK_LOSS, app_Mcsync_GetLinkLossBdAddr());
					APP_Mcsync_ClearLinkLossBdAddr();
				}

				app_Mcsync_ReconnectStateHandler(MCS_RECONN_EVENT_AGENT_WITH_PARTNER);
			}
			break;

		case MCS_RECONN_DEVICE_PARTNER:
			if(defaultRole == ROLE_AGENT)
			{
				APP_Mcsync_ClearLinkLossBdAddr();
				app_Mcsync_ReconnectStateHandler(MCS_RECONN_EVENT_AGENT_WITH_PARTNER);
			}
			break;

		case MCS_RECONN_DEVICE_AG:
			if(defaultRole == ROLE_AGENT)
			{
				app_Mcsync_ReconnectStateHandler(MCS_RECONN_EVENT_AGENT_WITH_AG);
			}
			break;

		default:
			break;
	}

	#else
	UNUSED(connectDev);
	#endif //SUPER_RECONNECT_MODE
}

void APP_Mcsync_ReconnectFailedNotify(U8 reason)
{
	#if SUPER_RECONNECT_MODE
	U8 state = app_Mcsync_GetReconnectState();
	U8 eventId = MCS_RECONN_EVENT_RECONNECT_FAILED;

	if(!APP_Mcsync_ReconnectIsFeatureOn())
		return;
<<<<<<< HEAD
	#ifdef MCSYNC_SHARE_MODE
=======
	#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
	if(MCSYNC_SHARE_MODE_DISABLE != BtMCSync_GetShareMode())
		return;
	#endif

	DBG_LOG_APP_Connection( "[APP MCS Reconn] Reconnect Failed Notify. state:%s, reason:%d", 2, APP_McsReconn_State_String[state], reason);

	if(state == MCS_RECONN_STATE_AGENT_RECONNECTING)
	{
		switch(reason)
		{
			case SDP_OPEN_SEARCH_ACL_LINK_ALREADY_EXIST: //maybe agent has already connected with AG, change role to partner immediately.
				FW_ReleaseTimer((Handler)&gAppMcsSuperReconnectHandle, ID_RECONNECT_TIMEOUT, 0);
				app_Mcsync_SetReconnectState(MCS_RECONN_STATE_AGENT_WAIT_RECONNECT_FAILED);
				APP_Reconn_ReconnectSetSuspend(TRUE);
				eventId = MCS_RECONN_EVENT_RECONNECT_FAILED_ALREADY_EXIST;
				break;

			case SDP_OPEN_SEARCH_CONNECTION_ACCEPT_TIMEOUT: //maybe agent has already connected with AG or AG is not connection timeout yet, try again after a long time.
				FW_ReleaseTimer((Handler)&gAppMcsSuperReconnectHandle, ID_RECONNECT_TIMEOUT, 0);
				app_Mcsync_SetReconnectState(MCS_RECONN_STATE_AGENT_WAIT_RECONNECT_FAILED);
				APP_Reconn_ReconnectSetSuspend(TRUE);
				eventId = MCS_RECONN_EVENT_RECONNECT_FAILED_CONN_ACCEPT_TO;
				break;
		}
	}
<<<<<<< HEAD
	else if(state == MCS_RECONN_STATE_IDLE)
	{
		switch(reason)
		{
			case HCI_STATUS_LMP_RESPONSE_TIMEOUT:
				eventId = MCS_RECONN_EVENT_RECONNECT_FAILED_LMP_RESPONSE_TIMEOUT;
				break;
		}
	}
=======
>>>>>>> db20e11 (second commit)

	FW_MessageSendEx((Handler)&gAppMcsSuperReconnectHandle, ID_STATE_HANDLER, NULL, eventId);
	#else
	UNUSED(reason);
	#endif
}

void APP_Mcsync_SetRoleNotify(U8 status)
{
	#if SUPER_RECONNECT_MODE
	U8 state = app_Mcsync_GetReconnectState();
	U8 defaultRole = BtAwsMce_GetDefaultRole();

	if(!APP_Mcsync_ReconnectIsFeatureOn())
		return;

<<<<<<< HEAD
	#ifdef MCSYNC_SHARE_MODE
=======
	#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
	if(MCSYNC_SHARE_MODE_DISABLE != BtMCSync_GetShareMode())
		return;
	#endif

	DBG_LOG_APP_Connection( "[APP MCS Reconn] Set Role Notify. state:%s, defaultRole:0x%x, status:%d", 3, APP_McsReconn_State_String[state], defaultRole, status);

	if(state == MCS_RECONN_STATE_CANCELING)
	{
		app_Mcsync_ReconnectStateHandler(MCS_RECONN_EVENT_CANCEL_MODE);
		gAppMcsReconnCtl.isNeedBackRole = FALSE;
		return;
	}

	if(status == HCI_STATUS_SUCCESS)
	{
		if(defaultRole == ROLE_AGENT)
		{
			app_Mcsync_ReconnectStateHandler(MCS_RECONN_EVENT_SET_AGENT_SUCCESSFUL);
		}
		else if(defaultRole == ROLE_PARTNER)
		{
<<<<<<< HEAD
=======
            APP_State_DiscoverabilityHandle(CMD_SET_NON_DISCOVERABLE);
>>>>>>> db20e11 (second commit)
			APP_State_ConnectabilityHandle(CMD_SET_NON_CONNECTABLE);
			APP_PowerOff_CancelTimer(TIMER_AWS_IDLE_POWEROFF);
			app_Mcsync_ReconnectStateHandler(MCS_RECONN_EVENT_SET_PARTNER_SUCCESSFUL);
		}
	}
	else
	{
		if(defaultRole == ROLE_AGENT)
		{
			if(status == HCI_STATUS_ROLE_SWITCH_PENDING)
				FW_SetTimer((Handler)&gAppMcsSuperReconnectHandle, ID_ACTION_HANDLER, NULL, MCS_RECONN_ACTION_SET_TO_PARTNER, 200);
			else
				app_Mcsync_ReconnectStateHandler(MCS_RECONN_EVENT_SET_PARTNER_FAILED);
		}
		else if(defaultRole == ROLE_PARTNER)
		{
			if(status == HCI_STATUS_ROLE_SWITCH_PENDING)
				FW_SetTimer((Handler)&gAppMcsSuperReconnectHandle, ID_ACTION_HANDLER, NULL, MCS_RECONN_ACTION_SET_TO_AGENT, 200);
			else
				app_Mcsync_ReconnectStateHandler(MCS_RECONN_EVENT_SET_AGENT_FAILED);
		}
	}
	#else
	UNUSED(status);
	#endif //SUPER_RECONNECT_MODE
}

void APP_Mcsync_SetReconnStartRole(U8 role)
{
	#if SUPER_RECONNECT_MODE
	if(!APP_Mcsync_ReconnectIsFeatureOn())
		return;

	gAppMcsReconnCtl.reconnRole = role;
	#else
	UNUSED(role);
	#endif
}

void APP_Mcsync_ClearReconnStartRole(void)
{
	#if SUPER_RECONNECT_MODE
	if(!APP_Mcsync_ReconnectIsFeatureOn())
		return;

	gAppMcsReconnCtl.reconnRole = MCS_RECONN_START_ORIGINAL;
	#endif
}

void APP_Mcsync_ExitSuperReconnectMode(BOOL isNeedBackRole)
{
	#if SUPER_RECONNECT_MODE
	U8 state = app_Mcsync_GetReconnectState();

	if(!APP_Mcsync_ReconnectIsFeatureOn())
		return;

	DBG_LOG_APP_Connection( "[APP MCS Reconn] Exit Super Reconnect Mode. state:%s, isNeedToBackRole:%d", 2, APP_McsReconn_State_String[state], isNeedBackRole);

	if(state == MCS_RECONN_STATE_IDLE || state == MCS_RECONN_STATE_CANCELING)
		return;

	gAppMcsReconnCtl.isNeedBackRole = isNeedBackRole;
	APP_ReConn_ClearCtl();

	FW_ReleaseTimer((Handler)&gAppMcsSuperReconnectHandle, ID_RECONNECT_TIMEOUT, 0);
	app_Mcsync_SetReconnectState(MCS_RECONN_STATE_CANCELING);

	if(state != MCS_RECONN_STATE_WAIT_TO_PARTNER && state != MCS_RECONN_STATE_WAIT_TO_AGENT)
		app_Mcsync_ReconnectStateHandler(MCS_RECONN_EVENT_CANCEL_MODE);
	#endif //SUPER_RECONNECT_MODE
}

void APP_Mcsync_SetLinkLossBdAddr(BD_ADDR_T *pBdAddr)
{
	#if SUPER_RECONNECT_MODE
	if(!APP_Mcsync_ReconnectIsFeatureOn())
		return;

	if(pBdAddr)
	{
		DBG_LOG_APP_Connection( "[APP MCS Reconn] Set Link Loss BdAddr:0x%x%x", 2, FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE));
		FW_Memcpy(&gAppMcsReconnCtl.linkLossBdAddr, pBdAddr, sizeof(BD_ADDR_T));
	}
	#else
	UNUSED(pBdAddr);
	#endif
}

void APP_Mcsync_ClearLinkLossBdAddr(void)
{
	#if SUPER_RECONNECT_MODE
	if(!APP_Mcsync_ReconnectIsFeatureOn())
		return;

	FW_Memset(&gAppMcsReconnCtl.linkLossBdAddr, 0, sizeof(BD_ADDR_T));
	#endif
}

void APP_Mcsync_ReconnectInit(void)
{
	#if SUPER_RECONNECT_MODE
	app_Mcsync_NvkeyInit();
	app_Mcsync_ReconnectParaInit();
	#endif
}

BOOL APP_Mcsync_ReconnectIsFeatureOn(void)
{
	#if SUPER_RECONNECT_MODE
	return (gpNvkeyMcsReconn->isFeatureOn) ? TRUE : FALSE;
	#else
	return FALSE;
	#endif
}

void APP_Mcsync_SetNotReconnMask(U8 mask)
{
	#if SUPER_RECONNECT_MODE
	U8 tempMask = gAppMcsReconnCtl.notReconnMask;

	DBG_LOG_APP_Connection( "[APP MCS Reconn] Set Not Reconnect Mask:0x%x, original:0x%x", 2, mask, gAppMcsReconnCtl.notReconnMask);
	gAppMcsReconnCtl.notReconnMask |= mask;

	if(mask != MCS_RECONN_MASK_CHARGER_CASE && tempMask != gAppMcsReconnCtl.notReconnMask)
		FW_SetTimer((Handler)&gAppMcsSuperReconnectHandle, ID_SEND_SYNC_NOT_RECONNECT_MASK, NULL, gAppMcsReconnCtl.notReconnMask, 200);
	#else
	UNUSED(mask);
	#endif
}

U8 APP_Mcsync_GetNotReconnMask(void)
{
	#if SUPER_RECONNECT_MODE
	return gAppMcsReconnCtl.notReconnMask;
	#else
	return 0;
	#endif
}

void APP_Mcsync_ClearNotReconnMask(U8 mask)
{
	#if SUPER_RECONNECT_MODE
	U8 tempMask = gAppMcsReconnCtl.notReconnMask;

	DBG_LOG_APP_Connection( "[APP MCS Reconn] Clear Not Reconnect Mask:0x%x, original:0x%x", 2, mask, gAppMcsReconnCtl.notReconnMask);
	gAppMcsReconnCtl.notReconnMask &= ~mask;

	if(mask != MCS_RECONN_MASK_CHARGER_CASE && tempMask != gAppMcsReconnCtl.notReconnMask)
		FW_SetTimer((Handler)&gAppMcsSuperReconnectHandle, ID_SEND_SYNC_NOT_RECONNECT_MASK, NULL, gAppMcsReconnCtl.notReconnMask, 200);
	#else
	UNUSED(mask);
	#endif
}

void APP_Mcsync_ClearAllNotReconnMask(void)
{
	#if SUPER_RECONNECT_MODE
	DBG_LOG_APP_Connection( "[APP MCS Reconn] Clear All Not Reconnect Mask", 0);
	gAppMcsReconnCtl.notReconnMask = 0;
	#endif
}

void APP_Mcsync_SyncNotReconnMaskIndHandler(U8 mask)
{
	#if SUPER_RECONNECT_MODE
	DBG_LOG_APP_Connection( "[APP MCS Reconn] Sync Not Reconnect Mask:0x%x", 1, mask);

	if(mask & MCS_RECONN_MASK_PARTNER_DISC)
		gAppMcsReconnCtl.notReconnMask |= MCS_RECONN_MASK_PARTNER_DISC;
	else
		gAppMcsReconnCtl.notReconnMask &= ~MCS_RECONN_MASK_PARTNER_DISC;

	if(mask & MCS_RECONN_MASK_AG_DISC)
		gAppMcsReconnCtl.notReconnMask |= MCS_RECONN_MASK_AG_DISC;
	else
		gAppMcsReconnCtl.notReconnMask &= ~MCS_RECONN_MASK_AG_DISC;
	#else
	UNUSED(mask);
	#endif
}
//For NONE Role with FCD discoverable mode
void APP_Mcsync_ClearSuperReconnTimer(void)
{
	#if SUPER_RECONNECT_MODE
	U8 state = app_Mcsync_GetReconnectState();

	if(state == MCS_RECONN_STATE_IDLE)
		MSG_MessageCancelAllHandler((Handler)&gAppMcsSuperReconnectHandle);
	#endif
}

BOOL APP_Mcsync_IsReconnectStateWaitToPartner(void)
{
	return app_Mcsync_GetReconnectState() == MCS_RECONN_STATE_WAIT_TO_PARTNER ? TRUE : FALSE;
}

void APP_Mcsync_SetReconnectLater(U8 type, U32 time)
{
    FW_SetTimer((Handler)&gAppMcsSuperReconnectHandle, ID_ENTER_SUPER_RECONNECT, NULL, type, time);
}

<<<<<<< HEAD
#ifdef DISCONNECT_AGENT_RECOVERY
=======
#ifdef AIR_AGENT_RECOVERY_MODE_AFTER_DISCONNECTION_FROM_PHONE_ENABLE
>>>>>>> db20e11 (second commit)
U16 app_mcsync_reconnect_check_super_reconnect_timer(U32 reconnect_type)
{
    return FW_CheckTimer((Handler)&gAppMcsSuperReconnectHandle, ID_ENTER_SUPER_RECONNECT, reconnect_type);
}
#endif

