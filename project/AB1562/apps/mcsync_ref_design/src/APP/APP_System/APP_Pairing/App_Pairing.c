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
#include "App_ResourceManager.h"
#include "App_Pairing.h"
#include "App_System.h"
#include "App_State.h"
#include "App_Interface.h"
#include "App_PowerOff.h"
#include "App_Nvram_Type.h"
#include "App_LinkKey.h"
#include "Pairing_NvkeyHandler.h"
#include "App_Media.h"
#include "BtMCSync.h"
#include "App_MCSync_RHO.h"
#include "App_SmartChgCase.h"
<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
#include "App_MCSync_Share.h"
#endif

/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct
{
    U8 cmd_state;
    U8 connect_mode;
    U8 discoverable_mode;
}APP_PAIRING_CTL_STRU;

/**************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
    APP_SCAN_CMD_NONE,
    APP_SCAN_CMD_CONNECTABLE,
    APP_SCAN_CMD_NO_CONNECT_NO_DISCOVER,
    APP_SCAN_CMD_CONNECT_DISCOVER,
};

enum
{
	APP_PAIRING_ID_SET_CONNECTABLE,
	APP_PAIRING_ID_SET_DISCOVERABLE,
};

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static U32 app_pairing_gap_handler(Handler handler, U16 id, void *msg, U32 id_ext);
static U32 app_pairing_timer_handler(Handler handler, U16 id, void *msg, U32 id_ext);
static U32 app_pairing_set_conndiscover_handler(Handler handler, U16 id, void *msg, U32 id_ext);


/**************************************************************************************************
* Variable
**************************************************************************************************/
static const HandlerData gAppPairingTimerHandler = { app_pairing_timer_handler };
static const HandlerData gAppPairingGapHandler = { app_pairing_gap_handler };
static const HandlerData gAppPairingSetConnDiscHandler = { app_pairing_set_conndiscover_handler };
static APP_PAIRING_CTL_STRU gAppPairingCtl;


/**************************************************************************************************
* Static Functions (Message Handler)
**************************************************************************************************/
static void app_pairing_write_scan_enable_cfm_handler(BT_WRITE_SCAN_ENABLE_CFM_T* cfm)
{
    U8 state;
    if(cfm->hci_status == HCI_STATUS_SUCCESS)
    {
        switch(gAppPairingCtl.cmd_state)
        {
            case APP_SCAN_CMD_CONNECTABLE:
                APP_State_RmvState(APP_EOF, APP_CONDISCABLE);
                state = APP_State_GetTopState(APP_EOF);

                DBG_LOG_APP_SYSTEM( "[APP_Pairing] Scan enable, Inquiry disable, state:%d", 1, state);
<<<<<<< HEAD
                if(APP_FAKEOFF == state || APP_OFF == state)
=======
                if(APP_FAKEOFF == state || APP_OFF == state || APP_DETACHING_LINK == state)
>>>>>>> db20e11 (second commit)
                {
                    APP_Pairing_ConnectableHandler(NON_CONNECTABLE_MODE);
                }
                else
                {
                    APP_State_RmvState(APP_EOF, APP_CONDISCABLE);
                    APP_State_InsertState(APP_EOF, APP_CONNECTABLE, APP_FAKEON);
                }
                break;
            case APP_SCAN_CMD_NO_CONNECT_NO_DISCOVER:
                DBG_LOG_APP_SYSTEM( "[APP_Pairing] Scan disable, Inquiry disable", 0);
                APP_State_RmvState(APP_EOF, APP_CONDISCABLE);
                APP_MCSync_Rho_NonConnectableNotify();
<<<<<<< HEAD
                #ifdef MCSYNC_SHARE_MODE
=======
                #ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
                APP_MCSync_Share_FollowerEventHandler(APP_SHARE_FOLLOWER_EVENT_NON_CONNECTABLE, NULL, NULL);
                #endif
                break;
            case APP_SCAN_CMD_CONNECT_DISCOVER:
                DBG_LOG_APP_SYSTEM( "[APP_Pairing] Scan enable, Inquiry enable", 0);
                APP_State_InsertState(APP_EOF, APP_CONDISCABLE, APP_CONNECTABLE);
                break;
            default:
                break;
        }
    }
    gAppPairingCtl.cmd_state = APP_SCAN_CMD_NONE;
}

static U32 app_pairing_gap_handler(Handler handler, U16 id, void *msg, U32 id_ext)
{
	UNUSED(handler);
	UNUSED(id_ext);
    switch(id)
    {
        case BT_WRITE_SCAN_ENABLE_CFM:
            app_pairing_write_scan_enable_cfm_handler((BT_WRITE_SCAN_ENABLE_CFM_T*)msg);
            break;
    }
    return 0;
}

static void app_pairing_connectable_timeout_handler(void)
{
<<<<<<< HEAD
	#ifndef TAKE_OVER_LINK
=======
	#ifndef AIR_TAKE_OVER_LINK_ENABLE
>>>>>>> db20e11 (second commit)
    if (CURRENT_ACTIVE_LINK_CNT > 0)
    {
        APP_State_ConnectabilityHandle(CMD_SET_NON_CONNECTABLE);
        APP_Media_PushMediaEvent(MEDIA_EVT_CONNECTABLE_TIMEOUT);
    }
	#endif
}

static U32 app_pairing_timer_handler(Handler handler, U16 id, void *msg, U32 id_ext)
{
	UNUSED(msg);
	UNUSED(handler);
	UNUSED(id_ext);

	switch(id)
	{
		case TIMER_PAIRING_MODE_TIMEOUT:
		case TIMER_PAIRING_MODE_PDL_EMPTY_TIMEOUT:
			if(APP_State_CheckNestStateByLink(APP_EOF, APP_CONDISCABLE))
			{
				APP_State_DiscoverabilityHandle(CMD_SET_NON_DISCOVERABLE);
			}
			break;
		case TIMER_PAIRING_CONNECTABLE_TIMEOUT:
			app_pairing_connectable_timeout_handler();
			break;
		default:
			FW_Assert(FALSE);
			break;
	}
	return 0;
}


static void app_pairing_set_connectable(U8 connectMode)
{
    gAppPairingCtl.connect_mode = connectMode;
	if(connectMode == NON_CONNECTABLE_MODE)
	{
		gAppPairingCtl.cmd_state = APP_SCAN_CMD_NO_CONNECT_NO_DISCOVER;
<<<<<<< HEAD
=======
        gAppPairingCtl.discoverable_mode = NON_DISCOVERABLE_MODE;
>>>>>>> db20e11 (second commit)
		BtWriteScanEnable((Handler)&gAppPairingGapHandler, (SCAN_ENABLE_T)SCAN_ENABLE_OFF);
	}
	else if(connectMode == CONNECTABLE_MODE)
	{
		if(!APP_SmtChgCse_IsPhoneNeeded())
			return;

<<<<<<< HEAD
        if(gAppPairingCtl.discoverable_mode != NON_DISCOVERABLE_MODE)
=======
        if(gAppPairingCtl.discoverable_mode != NON_DISCOVERABLE_MODE            
        	#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
           	&& !(BtMCSync_GetShareMode() == MCSYNC_SHARE_MODE_FOLLOWER_ENABLE && APP_MCSync_IsShareConn())
            #endif
           )

>>>>>>> db20e11 (second commit)
        {
            gAppPairingCtl.cmd_state = APP_SCAN_CMD_CONNECT_DISCOVER;
            BtWriteScanEnable((Handler)&gAppPairingGapHandler, (SCAN_ENABLE_T)SCAN_ENABLE_INQ_AND_PAGE);
        }
        else
        {
            gAppPairingCtl.cmd_state = APP_SCAN_CMD_CONNECTABLE;
            BtWriteScanEnable((Handler)&gAppPairingGapHandler, (SCAN_ENABLE_T)SCAN_ENABLE_PAGE);
        }
	}
	else
	{
		DBG_LOG_APP_SYSTEM( "[APP_Pairing] No Match Connect Mode:0x%x", 1, connectMode);
	}
}

static void app_pairing_set_discoverable(U8 discoverMode)
{
    gAppPairingCtl.discoverable_mode = discoverMode;
	if(discoverMode == NON_DISCOVERABLE_MODE)
	{
        gAppPairingCtl.cmd_state = APP_SCAN_CMD_CONNECTABLE;
        if(gAppPairingCtl.connect_mode != NON_CONNECTABLE_MODE)
        {
            BtWriteScanEnable((Handler)&gAppPairingGapHandler, (SCAN_ENABLE_T)SCAN_ENABLE_PAGE);
        }
        else
        {
            BtWriteScanEnable((Handler)&gAppPairingGapHandler, (SCAN_ENABLE_T)SCAN_ENABLE_OFF);
        }
	}
	else if (discoverMode & (GENERAL_DISCOVERABLE_MODE | SPECIAL_LIMITED_DISCOVERABLE_MODE | LIMITED_DISCOVERABLE_MODE))
	{
		if(!APP_SmtChgCse_IsPhoneNeeded())
			return;

		gAppPairingCtl.cmd_state = APP_SCAN_CMD_CONNECT_DISCOVER;
		if(APP_System_IsMpTestMode())
		{
			BtWriteInquiryScanActivity((Handler)&gAppPairingGapHandler, MP_TEST_MODE_INQ_SCAN_INTERVAL, INQ_SCAN_DEFAULT_WINDOW);
		}
		BtWriteScanEnable((Handler)&gAppPairingGapHandler, (SCAN_ENABLE_T)SCAN_ENABLE_INQ_AND_PAGE);
	}
	else
	{
		DBG_LOG_APP_SYSTEM( "[APP_Pairing] No Match Discover Mode:0x%x", 1, discoverMode);
	}
}

static U32 app_pairing_set_conndiscover_handler(Handler handler, U16 id, void *msg, U32 id_ext)
{
	UNUSED(msg);
	UNUSED(handler);
	if(APP_State_CheckNestStateByLink(APP_EOF, APP_AWS_PAIRING))
	{
		DBG_LOG_APP_SYSTEM( "[APP_Pairing] SetConnDiscHandler. airPairState:1, checkTimer:%d", 1, APP_Conn_CheckTimer(TIMER_ID_CLEAR_LINK_KEY));
		return 0; //drop
	}

	if(gAppPairingCtl.cmd_state != APP_SCAN_CMD_NONE)
	{
		return 2; //wait to the next round
	}

	if(id == APP_PAIRING_ID_SET_DISCOVERABLE && !APP_SUPPORT_ALL_ROLE_ENTER_DISCOVER_FEAT && !BtAwsMce_IsDefaultRoleAgent()
		&& (!(BtAwsMce_IsDefaultRoleNone() && APP_LinkKey_CheckFCDKeyState()))
		)
	{
		DBG_LOG_APP_SYSTEM( "[APP_Pairing] SetConnDiscHandler. pairState:%d, role:0x%x, id:%d", 3, gAppPairingCtl.cmd_state, BtAwsMce_GetDefaultRole(), id);
		return 0;
	}
	if(APP_Conn_CheckTimer(TIMER_ID_CLEAR_LINK_KEY))
	{
		return 2;
	}

	switch(id)
	{
		case APP_PAIRING_ID_SET_CONNECTABLE:
			app_pairing_set_connectable(id_ext);
			break;

		case APP_PAIRING_ID_SET_DISCOVERABLE:
			app_pairing_set_discoverable(id_ext);
			break;
	}
	return 0;
}

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void APP_Pairing_DiscoverableHandler(U8 discoverMode)
{
	if(APP_SUPPORT_ALL_ROLE_ENTER_DISCOVER_FEAT)
	{
		MSG_MessageCancelAll((Handler)&gAppPairingSetConnDiscHandler, APP_PAIRING_ID_SET_DISCOVERABLE);
		if(MSG_MessageSendEx((Handler)&gAppPairingSetConnDiscHandler, APP_PAIRING_ID_SET_DISCOVERABLE, NULL, discoverMode))
			FW_Assert(FALSE);

	}
	else
	{
		if(BtAwsMce_IsDefaultRoleAgent())
		{
			MSG_MessageCancelAll((Handler)&gAppPairingSetConnDiscHandler, APP_PAIRING_ID_SET_DISCOVERABLE);
			if(MSG_MessageSendEx((Handler)&gAppPairingSetConnDiscHandler, APP_PAIRING_ID_SET_DISCOVERABLE, NULL, discoverMode))
				FW_Assert(FALSE);
		}
		else if(BtAwsMce_IsDefaultRoleNone() && !APP_State_CheckNestStateByLink(APP_EOF, APP_AWS_PAIRING) && APP_LinkKey_CheckFCDKeyState())
		{
			MSG_MessageCancelAll((Handler)&gAppPairingSetConnDiscHandler, APP_PAIRING_ID_SET_DISCOVERABLE);
			if(MSG_MessageSendEx((Handler)&gAppPairingSetConnDiscHandler, APP_PAIRING_ID_SET_DISCOVERABLE, NULL, discoverMode))
				FW_Assert(FALSE);
		}
	}
}

void APP_Pairing_ConnectableHandler(U8 connectMode)
{
	if(BtAwsMce_IsDefaultRolePartner() && (connectMode & CONNECTABLE_MODE) && !APP_State_CheckNestStateByLink(APP_EOF, APP_CONNECTABLE))
	{
		APP_State_InsertState(APP_EOF, APP_CONNECTABLE, APP_FAKEON);
	}
	else if(BtAwsMce_IsDefaultRoleAgent() || BtAwsMce_IsDefaultRoleNone() ||
		(BtAwsMce_IsDefaultRolePartner() && (connectMode & NON_CONNECTABLE_MODE)) )
	{
		MSG_MessageCancelAll((Handler)&gAppPairingSetConnDiscHandler, APP_PAIRING_ID_SET_CONNECTABLE);
		if(MSG_MessageSendEx((Handler)&gAppPairingSetConnDiscHandler, APP_PAIRING_ID_SET_CONNECTABLE, NULL, connectMode))
			FW_Assert(FALSE);
	}
}

void APP_Pairing_SetTimer(U8 timerType)
{
	switch(timerType)
	{
		case TIMER_PAIRING_MODE_TIMEOUT:
		case TIMER_PAIRING_MODE_PDL_EMPTY_TIMEOUT:
		case TIMER_PAIRING_CONNECTABLE_TIMEOUT:
			FW_SetTimer((Handler)&gAppPairingTimerHandler, timerType, NULL, 0, APP_PairNvkey_GetTimer(timerType)*ONE_SEC);
			break;
		default:
			FW_Assert(FALSE);
			break;
	}

	return;
}

void APP_Pairing_CancelTimer(U8 timerType)
{
	MSG_MessageCancelAll((Handler)&gAppPairingTimerHandler, timerType);
}

bool APP_Pairing_IsTimerSet(U8 timerType)
{
	return MSG_MessageSearchAllEx((Handler)&gAppPairingTimerHandler, timerType, 0);
}

void APP_Pairing_ConnectableStateChangeWhenConnected(void)
{
	if(CURRENT_ACTIVE_LINK_CNT == APP_Conn_GetSupportDeviceNo())
	{
		APP_State_DiscoverabilityHandle(CMD_SET_NON_DISCOVERABLE_ANYWAY);
		APP_State_ConnectabilityHandle(CMD_SET_NON_CONNECTABLE_ANYWAY);
	}
	else if(CURRENT_ACTIVE_LINK_CNT > APP_Conn_GetSupportDeviceNo())
	{
		FW_Assert(FALSE);
	}
	else
	{
		APP_State_ConnectabilityHandle(CMD_SET_CONNECTABLE);
	}
}

void APP_Pairing_EnterDiscoverable(void)
{
	if(!(APP_NO_DISCAVERABLE_IN_LINE_IN_FEAT && APP_State_CheckInLineInState()) && !(APP_NO_DISCAVERABLE_IN_FM_FEAT && APP_State_CheckInFMState()))
	{
		APP_State_DiscoverabilityHandle(CMD_SET_NORMAL_DISCOVERABLE);
	}
}

void APP_Pairing_PairModeTimeoutHandler(void)
{
	if(APP_PairNvkey_IsFeatureOn(APP_ALWAYS_ENA_DISCOVER_MODE_FEAT))
		return;

	switch(APP_PairNvkey_GetActionAfterPairingModeTimeout())
	{
		case POWER_OFF:
			CHECK_POWER_OFF_IN_PAIR_MODE_TO:
			if(APP_CONNECTABLE == APP_State_GetTopState(&gAppCtl.serviceBdAddr))
			{
				APP_PowerOff_Loader(POWEROFF_REASON_PAIRING_MODE_TO);
			}
			break;
		case RETURN_TO_CONNECTABLE:
			CHECK_CONNECTABLE_IN_PAIR_MODE_TO:
			APP_State_ConnectabilityHandle(CMD_SET_CONNECTABLE);
			break;
		case POWER_OFF_IF_PDL_IS_EMPTY:
			if(APP_LinkKey_GetNormalPDLNumber())
			{
				goto CHECK_CONNECTABLE_IN_PAIR_MODE_TO;
			}
			else
			{
				goto CHECK_POWER_OFF_IN_PAIR_MODE_TO;
			}
			break;
		default:
			FW_Assert(FALSE);
			break;
	}
}

void APP_Pairing_SetPairModeTimer(void)
{
    if(!APP_PairNvkey_IsFeatureOn(APP_ALWAYS_ENA_DISCOVER_MODE_FEAT))
    {
        APP_Pairing_SetTimer( (APP_LinkKey_GetNormalPDLNumber() == 0) ? TIMER_PAIRING_MODE_PDL_EMPTY_TIMEOUT : TIMER_PAIRING_MODE_TIMEOUT);
    }
    APP_Pairing_CancelTimer(TIMER_PAIRING_CONNECTABLE_TIMEOUT);
}

U8 APP_Pairing_GetMsgNumInQueue(void)
{
	return (MSG_MessageSearchAllHandler((Handler)&gAppPairingSetConnDiscHandler) + MSG_MessageSearchAllHandler((Handler)&gAppPairingGapHandler));
}
