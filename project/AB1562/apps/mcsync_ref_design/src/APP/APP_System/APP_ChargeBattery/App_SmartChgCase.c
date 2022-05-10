/* Copyright Statement:
 *
 * (C) 2019  Airoha Technology Corp. All rights reserved.
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
#include "App_SmartChgCase.h"
#include "App_PowerOff.h"
#include "PM_Interface.h"
#include "App_MCSync_RHO.h"
#include "App_State.h"
#include "BtMCSync.h"
#include "App_ChargeBattery.h"
#include "App_System.h"
#include "App_Nvram_Type.h"
#include "App_MCSync_Message.h"
#include "BtMCSync.h"
#include "PM_Connection.h"
#include "App_PowerOn.h"
#include "App_MCSync_System.h"
#include "DrvSmartCase.h"
#include "App_SmartChgCase.h"
#include "App_LinkKey.h"
#include "App_Mcsync_Reconnection.h"
#include "DSP_Control.h"
#include "App_Media.h"
#include "App_Battery.h"
#include "ChargerBatteryCaseNvkeyHandler.h"
#include "chargercase_common.h"
#include "App_EventKeyHandler.h"
#include "nvkey_hdl.h"
#include "AudioDSP_AncPassThru.h"
#include "App_VolumeSetting.h"
#include "App_Customer_Func.h"
#include "App_Customer.h"
#include "App_Media.h"
<<<<<<< HEAD
#include "gfps_api.h"

#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_GFP_ENABLE
#include "gfps_api.h"
#include "App_Ble.h"
#endif

#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
#include "App_MCSync_Share.h"
#endif

#ifdef SMART_CHG_CSE_DEVELOPMENT

#define DBG_LOG_APP_SMART_CASE(_message, arg_cnt, ...)  LOG_MSGID_I(APP_SMART_CASE, _message, arg_cnt, ##__VA_ARGS__)


log_create_module(APP_SMART_CASE, PRINT_LEVEL_INFO);

/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct
{
	BOOL isNeedRho;
}APP_SMTCHGCSE_CTL_STRU;

/**************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
	TIMER_CHECK_STATE
};

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static U32 app_SmtChgCse_StateHandler(Handler handler, U16 id, void *msg, U32 ext_id);
static U32 app_SmtChgCse_TimerHandler(Handler handler, U16 id, void *msg, U32 ext_id);
static U32 app_SmtChgCse_RsvHandler(Handler handler, U16 id, void *msg, U32 ext_id);
static void app_SmtChgCse_ChgCaseOffHandler(void);
static void app_SmtChgCse_LidOnHandler(void);
static void app_SmtChgCse_LidOffHandler(void);
static void app_SmtChgCse_OutOfCaseHandler(void);
static void app_SmtChgCse_CheckStateTimerHandler(void);
#ifndef SMART_CHG_CSE_LIDON_RECONNECT
static void app_SmtChgCse_PartnerConnHandler(void);
#endif
static void app_SmtChgCse_AgentConnHandler(void);
static void app_SmtChgCse_DiscAGHandler(void);
#ifndef SMART_CHG_CSE_LIDON_RECONNECT
static void app_SmtChgCse_MCSyncConntingHandler(void);
#endif


<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
static U32 app_SmtChgCse_MCSyncShareHandler(Handler handler, U16 id, void *msg, U32 ext_id);
#endif

/**************************************************************************************************
* Variable
**************************************************************************************************/
HandlerData gAppSccStateHandle = { app_SmtChgCse_StateHandler };
static HandlerData gAppSccTimerHandle = { app_SmtChgCse_TimerHandler };
HandlerData gAppSccRsvHandle = { app_SmtChgCse_RsvHandler };
static APP_SMTCHGCSE_CTL_STRU gAppSccCtl;

<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
static HandlerData gAppMCSyncShareHandle = { app_SmtChgCse_MCSyncShareHandler };
#endif

/**************************************************************************************************
* Static Functions
**************************************************************************************************/
<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
static U32 app_SmtChgCse_MCSyncShareHandler(Handler handler, U16 id, void *msg, U32 ext_id)
{
	UNUSED(handler);UNUSED(msg); UNUSED(ext_id);
	switch(id)
	{
		case CASE_SHORT_PATTERN:
			DBG_LOG_APP_SMART_CASE( "[APP SMART V2] short pattern expired, trigger follower agent", 0);

			if(BtAwsMce_IsDefaultRoleAgent())
				APP_MCSync_Share_KeyFollowerStart();

			break;
		case CASE_LONG_PATTERN:
			DBG_LOG_APP_SMART_CASE( "[APP SMART V2] long pattern expired, trigger share agent", 0);
<<<<<<< HEAD
			
			if(BtAwsMce_IsDefaultRoleAgent())
				APP_MCSync_Share_KeyAgentStart();
			
=======

			if(BtAwsMce_IsDefaultRoleAgent())
				APP_MCSync_Share_KeyAgentStart();

>>>>>>> db20e11 (second commit)
			break;
	}

	return 0;
}
#endif

static U32 app_SmtChgCse_RsvHandler(Handler handler, U16 id, void *msg, U32 ext_id)
{
	UNUSED(handler);
	UNUSED(msg);;

<<<<<<< HEAD
	if(APP_ChgCase_GetCaseType() == CASE_TYPE_SMART)
=======
        if(APP_ChgCase_GetCaseType() == CASE_TYPE_SMART)
>>>>>>> db20e11 (second commit)
	{
		switch(id)
			{
				case SMART_RSV_0_IND:
					break;
				case SMART_RSV_1_IND:
					break;
				case SMART_RSV_2_IND:
					break;
				case SMART_RSV_3_IND:
					break;
				case SMART_RSV_4_IND:
					break;
				default:
					break;
			}

	}
	else if((APP_ChgCase_GetCaseType() == CASE_TYPE_SMART_V2) || (APP_ChgCase_GetCaseType() == CASE_TYPE_ONE_WIRE))
	{
		DBG_LOG_APP_SMART_CASE( "[APP SMART V2] app_SmtChgCse_RsvHandler-id:(%d), para:(%d)", 2, id, ext_id);

		//printf("[APP SMART V2] app_SmtChgCse_RsvHandler-id:(%d), para:(%d)", id, ext_id);

		switch(id)
			{
				case SMARTV2_CASE_BATTERY_IND: //5
					APP_Customer_LidOnBatteryHanler(ext_id);
<<<<<<< HEAD
=======
					APP_Battery_SetSmartCaseBatLevel(ext_id);
>>>>>>> db20e11 (second commit)
					break;
				case SMARTV2_KEY_IND:
					{
						U16 keyCode;
						U32 supportedState;
						U32 stateBitMask;
<<<<<<< HEAD
						#ifdef MCSYNC_SHARE_MODE
=======
						#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
						if(ext_id == 0)
                        {
							DBG_LOG_APP_SMART_CASE( "[APP SMART V2] set share mode short pattern, timer:13.5s", 0);
							APP_Media_PushMediaEvent(MEDIA_EVT_SHARE_MODE_SHORT_PATTERN);
							APP_Media_PushMediaEvent(MEDIA_EVT_SHARE_MODE_SHORT_PATTERN);
							APP_Media_PushMediaEvent(MEDIA_EVT_SHARE_MODE_SHORT_PATTERN);
<<<<<<< HEAD
									
							FW_SetTimer((Handler)&gAppMCSyncShareHandle, CASE_SHORT_PATTERN, NULL, 0, 13500);
            		
=======

							FW_SetTimer((Handler)&gAppMCSyncShareHandle, CASE_SHORT_PATTERN, NULL, 0, 13500);

>>>>>>> db20e11 (second commit)
                            return 0;
                        }
                        else if(ext_id == 1)
                        {
                        	DBG_LOG_APP_SMART_CASE( "[APP SMART V2] set share mode long pattern, timer:0.5s", 0);
                        	FW_ReleaseTimer((Handler)&gAppMCSyncShareHandle, CASE_SHORT_PATTERN, 0);
							FW_SetTimer((Handler)&gAppMCSyncShareHandle, CASE_LONG_PATTERN, NULL, 0, 500);

                            return 0;
                        }
						#else
						if(ext_id == 0)
						{
							keyCode = ChgCase_SmartV2_GetKey1EvtCode();
							supportedState = ChgCase_SmartV2_GetKey1SupportedState();
							APP_Customer_KeyIndex0Handler();
						}
						else if(ext_id ==1)
						{
							keyCode = ChgCase_SmartV2_GetKey2EvtCode();
							supportedState = ChgCase_SmartV2_GetKey2SupportedState();
							APP_Customer_KeyIndex1Handler();
						}
						#endif
<<<<<<< HEAD
                        #if 0//def MCSYNC_SHARE_MODE
=======
                        #if 0//def AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
                        else if(ext_id == 2)
                        {
                            if(BtAwsMce_IsDefaultRoleAgent())
                                APP_MCSync_Share_KeyAgentStart();
<<<<<<< HEAD
                            
=======

>>>>>>> db20e11 (second commit)
                            return 0;
                        }
                        else if(ext_id == 3)
                        {
                            if(BtAwsMce_IsDefaultRoleAgent())
                                APP_MCSync_Share_KeyFollowerStart();
<<<<<<< HEAD
                            
=======

>>>>>>> db20e11 (second commit)
                            return 0;
                        }
                        #endif
						else
						{
							return 0;
						}


						stateBitMask = 1L << APP_State_GetTopState(&gAppCtl.serviceBdAddr);

						DBG_LOG_APP_SMART_CASE( "[APP SMART V2] keyCode(%x) SupportedState(%x) stateBitMask(%x)", 3, keyCode, supportedState, stateBitMask);
						//printf("[APP SMART V2] keyCode(%x) suppotedState(%x) stateBitMask(%x)", keyCode, SupportedState, stateBitMask);

						if((stateBitMask & supportedState) && !APP_Customer_SmartKeyHandler())
						{
							APP_EvtKey_KeyHandler(keyCode);
						}
					}
					break;
				case SMARTV2_USR1_IND:
<<<<<<< HEAD
=======
                    #ifdef AIR_GFP_ENABLE
                    gfp_ble_adv_update(GFP_UPDATE_BLE_ADV_REASON_LID_OFF);
                    #endif
>>>>>>> db20e11 (second commit)
					APP_Customer_SmartUsr1IndHandler();
					break;
				case SMARTV2_USR2_IND:
					break;
				case SMARTV2_USR3_IND:
					break;
				default:
					break;
			}

	}

	return 0;

}

static U32 app_SmtChgCse_StateHandler(Handler handler, U16 id, void *msg, U32 ext_id)
{
	UNUSED(handler);
	UNUSED(msg);
	UNUSED(ext_id);

	DBG_LOG_APP_SMART_CASE( "[APP_SmtChgCse] app_SmtChgCse_StateHandler, State:%s(%d)", 2, LibDrv_SmartCase_StateString[id], id);

	APP_MCSYNC_SendSyncSmartChargeCaseStateInfo(id);

	switch(id)
	{
		case SMART_CASE_OFF_IND:
			app_SmtChgCse_ChgCaseOffHandler();
			break;

		case SMART_CASE_LID_OPEN_IND:
			app_SmtChgCse_LidOnHandler();
			break;

		case SMART_CASE_LID_CLOSE_IND:
			app_SmtChgCse_LidOffHandler();
			break;

		case SMART_CASE_OUT_OF_CASE_IND:
			app_SmtChgCse_OutOfCaseHandler();
			break;

		default:

			break;
	}

	return 0;
}

static U32 app_SmtChgCse_TimerHandler(Handler handler, U16 id, void *msg, U32 ext_id)
{
	UNUSED(handler);
	UNUSED(msg);
	UNUSED(ext_id);

	DBG_LOG_APP_SMART_CASE( "[APP_SmtChgCse] app_SmtChgCse_TimerHandler, id:%d", 1, id);

	switch(id)
	{
		case TIMER_CHECK_STATE:
			app_SmtChgCse_CheckStateTimerHandler();
			break;

		default:

			break;
	}

	return 0;
}

static void app_SmtChgCse_ChgCaseOffHandler(void)
{
    APP_VolSetting_SetSpeakerMuteOn(SPKR_MUTE_MASK_CHG_CASE);
	nvkey_hdl_remove_state(NVKEY_HDL_STATE_MASK_APP_SMART_CASE);
/*
	if(APP_ChgBat_GetBatStatus() >= APP_BAT_CHARGER_IN)
	{
		APP_ChgBat_UpdateBatStatus((APP_Battery_IsBelowLowLevel())? APP_BAT_LOW : APP_BAT_WELL);
		APP_Media_PushMediaEvent(MEDIA_EVT_BAT_CHGROUT);
	}
*/
	APP_PowerOff_Loader(POWEROFF_REASON_SMARTCASE_POWEROFF);
}

static void app_SmtChgCse_LidOnHandler(void)
{
	U8 mcsyncState = MCSYNC_GetMcsyncState();
	U8 connState = APP_Conn_GetAgLinkState();

	DBG_LOG_APP_SMART_CASE( "[APP_SmtChgCse] LidOn, McsState:%s(%d), ConnState:%s(%d), role:0x%x", 5, APP_McSyncLinkString[mcsyncState],
		mcsyncState, APP_LinkConnString[connState], connState, BtMCSync_GetCurrentRole());

	DBG_LOG_APP_SMART_CASE( "[APP_SmtChgCse]app_SmtChgCse_LidOnHandler checktimer:%d", 1,
		FW_CheckTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, 0));

<<<<<<< HEAD
	AudioDsp_ANCPassThruLidOnHandler();

#ifdef PROFILE_GFP_ENABLE
    App_gfp_set_agent_case_LidOn();
#endif 

=======
    #ifdef AIR_GFP_ENABLE
    if (!gfp_ble_adv_is_enabled())
    {
        APP_Ble_EnableAdv(BLE_ADV_GFP_MASK);
    }
    gfp_ble_adv_update(GFP_UPDATE_BLE_ADV_REASON_LID_ON);
    #endif

	AudioDsp_ANCPassThruLidOnHandler();
>>>>>>> db20e11 (second commit)

	if(!APP_SmtChgCse_IsConnToAGNeeded())
		APP_Mcsync_SetNotReconnMask(MCS_RECONN_MASK_CHARGER_CASE);
	else
		APP_Mcsync_ClearNotReconnMask(MCS_RECONN_MASK_CHARGER_CASE);
	#ifndef SMART_CHG_CSE_LIDON_RECONNECT
	PM_SetRejectConnectInd(TRUE);
	APP_State_ConnectabilityHandle(CMD_SET_NON_CONNECTABLE);
	#endif
	APP_VolSetting_SetSpeakerMuteOn(SPKR_MUTE_MASK_CHG_CASE);
	nvkey_hdl_set_state(NVKEY_HDL_STATE_MASK_APP_SMART_CASE);
<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
	APP_MCSync_Share_ExitMode();
#endif
	#ifdef SMART_CHG_CSE_LIDON_RECONNECT
	switch(mcsyncState)
	{
		case MCSYNC_LINK_CONNECTED:
			switch(connState)
			{
<<<<<<< HEAD
				case LINK_CONNECTED:
					FW_SetTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, NULL, 0, (APP_IsActiveAudioDevInCall()) ? 500 : ChgCase_Smart_GetCaseRHOTimer());
					break;
			}
			break;

=======
				//charger out -> lid on
				case LINK_CONNECTED:
				//lid off -> lid on, acl is disconnecting, need timer to reconnect phone after acl disc
				case LINK_DISCONNECTING:
					FW_SetTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, NULL, 0, (APP_IsActiveAudioDevInCall()) ? 500 : ChgCase_Smart_GetCaseRHOTimer());
			}
		//fall through, for lid off -> lid on, need power on.
>>>>>>> db20e11 (second commit)
		default:
			if(!APP_PowerOff_IsPowerOn())
			{
				APP_PowerOn_Loader();
			}
			break;
	}
	#else
	switch(mcsyncState)
	{
		case MCSYNC_LINK_SAWS_DISCONNECTED:
			switch(connState)
			{
				case LINK_DISCONNECTED:
					app_SmtChgCse_MCSyncConntingHandler();
					break;
				case LINK_CONNECTING:
					if(!FW_CheckTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, 0))
					{
						app_SmtChgCse_DiscAGHandler();
						APP_ReConn_ClearCtl();
						app_SmtChgCse_MCSyncConntingHandler();
					}
					FW_SetTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, NULL, 0, 1000);
					break;
				case LINK_CONNECTED:
					app_SmtChgCse_DiscAGHandler();
					app_SmtChgCse_MCSyncConntingHandler();
					break;
				case LINK_DISCONNECTING:
					FW_SetTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, NULL, 0, 1000);
					break;
				default:
					FW_Assert(FALSE);
					break;
			}
			break;
		case MCSYNC_LINK_SAWS_CONNECTING:
			switch(connState)
			{
				case LINK_DISCONNECTED:
					app_SmtChgCse_MCSyncConntingHandler();
					break;
				case LINK_CONNECTING:
					if(!FW_CheckTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, 0))
					{
						app_SmtChgCse_DiscAGHandler();
						APP_ReConn_ClearCtl();
					}
					FW_SetTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, NULL, 0, 1000);
					break;
				case LINK_CONNECTED:
					app_SmtChgCse_DiscAGHandler();
					break;
				case LINK_DISCONNECTING:
					FW_SetTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, NULL, 0, 1000);
					break;
				default:
					FW_Assert(FALSE);
					break;
			}
			break;
		case MCSYNC_LINK_SAWS_DISCONNECTING:
		case MCSYNC_LINK_DISCONNECTED:
			switch(connState)
			{
				case LINK_DISCONNECTED:
					app_SmtChgCse_MCSyncConntingHandler();
					break;
				case LINK_CONNECTING:
					if(!FW_CheckTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, 0))
					{
						app_SmtChgCse_DiscAGHandler();
						APP_ReConn_ClearCtl();
					}
					FW_SetTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, NULL, 0, 1000);
					break;
				case LINK_CONNECTED:
					app_SmtChgCse_DiscAGHandler();
					break;
				case LINK_DISCONNECTING:
					FW_SetTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, NULL, 0, 1000);
					break;
				default:
					FW_Assert(FALSE);
					break;
			}
			break;
		case MCSYNC_LINK_CONNECTING:
			switch(connState)
			{
				case LINK_DISCONNECTED:
					app_SmtChgCse_MCSyncConntingHandler();
					break;
				case LINK_CONNECTING:
					if(!FW_CheckTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, 0))
					{
						app_SmtChgCse_DiscAGHandler();
						APP_ReConn_ClearCtl();
					}
					FW_SetTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, NULL, 0, 1000);
					break;
				case LINK_CONNECTED:
					app_SmtChgCse_DiscAGHandler();
					break;
				case LINK_DISCONNECTING:
					FW_SetTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, NULL, 0, 1000);
					break;
				default:
					FW_Assert(FALSE);
					break;
			}
			break;
		case MCSYNC_LINK_CONNECTED:
			switch(connState)
			{
				case LINK_DISCONNECTED:
					break;
				case LINK_CONNECTING:
					if(!FW_CheckTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, 0))
					{
						if(!APP_SmtChgCse_IsConnToAGNeeded())
						{
							app_SmtChgCse_DiscAGHandler();
							APP_ReConn_ClearCtl();
						}
						FW_SetTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, NULL, 0, 1000);
					}
					break;
				case LINK_CONNECTED:
					FW_SetTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, NULL, 0, (APP_IsActiveAudioDevInCall()) ? 500 : ChgCase_Smart_GetCaseRHOTimer());
					break;
				case LINK_DISCONNECTING:
					FW_SetTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, NULL, 0, 1000);
					break;
				default:
					FW_Assert(FALSE);
					break;
			}
			break;
		case MCSYNC_LINK_DISCONNECTING:
			switch(connState)
			{
				case LINK_DISCONNECTED:
					break;
				case LINK_CONNECTING:
					if(!FW_CheckTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, 0))
					{
						app_SmtChgCse_DiscAGHandler();
						APP_ReConn_ClearCtl();
						FW_SetTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, NULL, 0, 1000);
					}
					break;
				case LINK_CONNECTED:
					app_SmtChgCse_DiscAGHandler();
					break;
				case LINK_DISCONNECTING:
					FW_SetTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, NULL, 0, 1000);
					break;
				default:
					FW_Assert(FALSE);
					break;
			}
			break;
		default:
			FW_Assert(FALSE);
			break;

	}
	#endif
}

static void app_SmtChgCse_LidOffHandler(void)
{
	U8 mcsyncState = BtAwsMce_GetMcsyncState();

	DBG_LOG_APP_SMART_CASE( "[APP_SmtChgCse] LidOff, McsState:%s(%d), role:0x%x", 3, APP_McSyncLinkString[mcsyncState], mcsyncState, BtMCSync_GetCurrentRole());

<<<<<<< HEAD
	APP_VolSetting_SetSpeakerMuteOn(SPKR_MUTE_MASK_CHG_CASE);
	nvkey_hdl_set_state(NVKEY_HDL_STATE_MASK_APP_SMART_CASE);

#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_GFP_ENABLE
    gfp_ble_adv_update(GFP_UPDATE_BLE_ADV_REASON_LID_OFF);
#endif

	APP_VolSetting_SetSpeakerMuteOn(SPKR_MUTE_MASK_CHG_CASE);
	nvkey_hdl_set_state(NVKEY_HDL_STATE_MASK_APP_SMART_CASE);

#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
	APP_MCSync_Share_ExitMode();
#endif
	if(!APP_SmtChgCse_IsConnToAGNeeded())
		APP_Mcsync_SetNotReconnMask(MCS_RECONN_MASK_CHARGER_CASE);
	else
		APP_Mcsync_ClearNotReconnMask(MCS_RECONN_MASK_CHARGER_CASE);

	APP_Customer_SetActionPowerOff(FALSE);
	APP_PowerOff_Loader(POWEROFF_REASON_SMARTCASE_POWEROFF);
}

static void app_SmtChgCse_OutOfCaseHandler(void)
{
	U8 mcsyncState = BtAwsMce_GetMcsyncState();
	U8 connState = PM_Conn_GetAgLinkState();

	DBG_LOG_APP_SMART_CASE( "[APP_SmtChgCse] OutOfCase, McsState:%s(%d), ConnState:%s(%d), role0x%x", 5, APP_McSyncLinkString[mcsyncState], mcsyncState, APP_LinkConnString[connState], connState, BtMCSync_GetCurrentRole());

	APP_Media_StopMediaEvt(MEDIA_EVT_SLC_DISCONNECTED);
<<<<<<< HEAD
=======
	APP_Conn_ReleaseTimer(TIMER_ID_DISCONNECTED_VP);
>>>>>>> db20e11 (second commit)

	#ifndef SMART_CHG_CSE_LIDON_RECONNECT
	PM_SetRejectConnectInd(FALSE);
	#endif
    APP_VolSetting_SetSpeakerMuteOff(SPKR_MUTE_MASK_CHG_CASE);
	nvkey_hdl_remove_state(NVKEY_HDL_STATE_MASK_APP_SMART_CASE);

	AudioDsp_ANCPassThruOutOfCaseHandler();

	if(!APP_SmtChgCse_IsConnToAGNeeded())
		APP_Mcsync_SetNotReconnMask(MCS_RECONN_MASK_CHARGER_CASE);
	else
		APP_Mcsync_ClearNotReconnMask(MCS_RECONN_MASK_CHARGER_CASE);

<<<<<<< HEAD
#ifdef PROFILE_GFP_ENABLE
    App_gfp_set_agent_case_LidOff();
#endif 

=======
>>>>>>> db20e11 (second commit)
	#ifdef SMART_CHG_CSE_LIDON_RECONNECT
	if(mcsyncState == MCSYNC_LINK_CONNECTED)
	{
		if(BtAwsMce_IsDefaultRolePartner())
		{
			APP_MCSYNC_SendSyncSmtChgCseInfo(TRUE);
		}
	}
	else if(!APP_PowerOff_IsPowerOn() && !APP_Customer_GetActionPowerOff())
	{
		APP_PowerOn_Loader();
	}
	APP_Customer_KeyActionPowerOff();
	#else
	switch(mcsyncState)
	{
		case MCSYNC_LINK_SAWS_DISCONNECTED:
			switch(connState)
			{
				case LINK_DISCONNECTED:
					app_SmtChgCse_MCSyncConntingHandler();
					break;
				case LINK_CONNECTING:
					break;
				case LINK_CONNECTED:
					app_SmtChgCse_MCSyncConntingHandler();
					break;
				case LINK_DISCONNECTING:
					FW_SetTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, NULL, 0, 1000);
					break;
				default:
					FW_Assert(FALSE);
					break;
			}
			break;
		case MCSYNC_LINK_SAWS_CONNECTING:
			switch(connState)
			{
				case LINK_DISCONNECTED:
					if(BtAwsMce_IsDefaultRoleAgent())
					{
						app_SmtChgCse_AgentConnHandler();
					}
					break;
				case LINK_CONNECTING:
					break;
				case LINK_CONNECTED:
					break;
				case LINK_DISCONNECTING:
					FW_SetTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, NULL, 0, 1000);
					break;
				default:
					FW_Assert(FALSE);
					break;
			}
			break;
		case MCSYNC_LINK_SAWS_DISCONNECTING:
		case MCSYNC_LINK_DISCONNECTED:
			switch(connState)
			{
				case LINK_DISCONNECTED:
					app_SmtChgCse_MCSyncConntingHandler();
					break;
				case LINK_CONNECTING:
					break;
				case LINK_CONNECTED:
					break;
				case LINK_DISCONNECTING:
					FW_SetTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, NULL, 0, 1000);
					break;
				default:
					FW_Assert(FALSE);
					break;
			}
			break;
		case MCSYNC_LINK_CONNECTING:
			switch(connState)
			{
				case LINK_DISCONNECTED:
					if(BtAwsMce_IsDefaultRoleAgent())
					{
						app_SmtChgCse_AgentConnHandler();
					}
					break;
				case LINK_CONNECTING:
					break;
				case LINK_CONNECTED:
					break;
				case LINK_DISCONNECTING:
					FW_SetTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, NULL, 0, 1000);
					break;
				default:
					FW_Assert(FALSE);
					break;
			}
			break;
		case MCSYNC_LINK_CONNECTED:
			switch(connState)
			{
				case LINK_DISCONNECTED:
					if(BtAwsMce_IsDefaultRolePartner())
					{
						APP_MCSYNC_SendSyncSmtChgCseInfo(TRUE);
					}
					else if(BtAwsMce_IsDefaultRoleAgent())
					{
						app_SmtChgCse_AgentConnHandler();
					}
					break;
				case LINK_CONNECTING:
					break;
				case LINK_CONNECTED:
					break;
				case LINK_DISCONNECTING:
					FW_SetTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, NULL, 0, 1000);
					break;
				default:
					FW_Assert(FALSE);
					break;
			}
			break;
		case MCSYNC_LINK_DISCONNECTING:
			switch(connState)
			{
				case LINK_DISCONNECTED:
					app_SmtChgCse_MCSyncConntingHandler();
					break;
				case LINK_CONNECTING:
					break;
				case LINK_CONNECTED:
					break;
				case LINK_DISCONNECTING:
					FW_SetTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, NULL, 0, 1000);
					break;
				default:
					FW_Assert(FALSE);
					break;
			}
			break;
		default:
			FW_Assert(FALSE);
			break;

	}
	#endif
}

static void app_SmtChgCse_CheckStateTimerHandler(void)
{
	U8 connState = APP_Conn_GetAgLinkState();
	U8 mcsyncState = BtAwsMce_GetMcsyncState();
	U8 sccPState = APP_GetPartnerSmartChargeCaseState();
	U8 sccAState = DrvCharger_GetSmartCaseState();

	DBG_LOG_APP_SMART_CASE( "[APP_SmtChgCse] RhoTimer, Pstate:%d, Astate:%d, connState:%d, role:0x%x, mcsState:%d", 5, sccPState, sccAState, connState, BtMCSync_GetCurrentRole(), mcsyncState);

	if(!BtAwsMce_IsDefaultRoleAgent())
		return;

	if(connState == LINK_DISCONNECTING)
	{
		FW_SetTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, NULL, 0, 1000);
	}
	else if(connState == LINK_CONNECTING)
	{
		FW_SetTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, NULL, 0, 1000);
	}
	else if(connState == LINK_CONNECTED)
	{
		if(sccAState != STATE_OUT_OF_CASE)
		{
			if(mcsyncState == MCSYNC_LINK_CONNECTED)
			{
				if(sccPState != STATE_OUT_OF_CASE)
				{
					app_SmtChgCse_DiscAGHandler();
				}
				else
				{
					APP_MCSync_Rho_Start();
					FW_SetTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, NULL, 0, 2000);
				}
			}
			else
			{
				app_SmtChgCse_DiscAGHandler();
			}
		}
	}
	else if(connState == LINK_DISCONNECTED)
	{
		if(APP_SmtChgCse_IsConnToAGNeeded())
		{
			app_SmtChgCse_AgentConnHandler();
		}
	}
}

#ifndef SMART_CHG_CSE_LIDON_RECONNECT
static void app_SmtChgCse_PartnerConnHandler(void)
{
	APP_ReConn_Reconnect(RECONNECT_AWSMCE);
}
#endif

static void app_SmtChgCse_AgentConnHandler(void)
{
	APP_ReConn_Reconnect(RECONNECT_PWR_ON);
	if(APP_System_IsMpTestMode() || (!(APP_NO_DISCAVERABLE_IN_LINE_IN_FEAT && APP_State_CheckInLineInState()) && !(APP_NO_DISCAVERABLE_IN_FM_FEAT && APP_State_CheckInFMState())))
	{
<<<<<<< HEAD
		if(APP_SmtChgCse_IsConnToAGNeeded())
			APP_State_DiscoverabilityHandle(CMD_SET_NORMAL_DISCOVERABLE);
=======
#if 0 // (337)entry disdcoveralbe mode will cancel re-connect SP
		if(APP_SmtChgCse_IsConnToAGNeeded())
			APP_State_DiscoverabilityHandle(CMD_SET_NORMAL_DISCOVERABLE);
#endif
>>>>>>> db20e11 (second commit)
	}
}

static void app_SmtChgCse_DiscAGHandler(void)
{
	#ifndef SMART_CHG_CSE_LIDON_RECONNECT
	if(PM_Conn_GetAgLinkState() == LINK_CONNECTED)
	{
		if(BtAwsMce_IsDefaultRoleAgent() && APP_GetAGNum() && APP_GetServiceBdAddr())
		{
			BtSetAclDetachTimeout(APP_GetServiceBdAddr()->addr, 0);
		}
		APP_State_ConnectabilityHandle(CMD_SET_NON_CONNECTABLE);
		APP_Conn_ReleaseMultiLink();
	}
	#endif
}

#ifndef SMART_CHG_CSE_LIDON_RECONNECT
static void app_SmtChgCse_MCSyncConntingHandler(void)
{
	if(!APP_PowerOff_IsPowerOn())
	{
		APP_Customer_MCSyncConntingHandler();
		APP_PowerOn_Loader();
	}
	else if(BtAwsMce_IsDefaultRolePartner())
	{
		app_SmtChgCse_PartnerConnHandler();
	}
	else if(BtAwsMce_IsDefaultRoleAgent())
	{
		app_SmtChgCse_AgentConnHandler();
	}
}
#endif //#if !SMART_CHG_CSE_LIDON_RECONNECT

#endif
/**************************************************************************************************
* Public Functions
**************************************************************************************************/

BOOL APP_SmtChgCse_MCSyncConnCfmCheck(void)
{
	#ifdef SMART_CHG_CSE_DEVELOPMENT
	U8 sccState = DrvCharger_GetSmartCaseState();

	if(APP_ChgCase_IsCaseSmartType())
	{
		switch(sccState)
		{
			case STATE_LID_CLOSE:
				PM_DisconnectSpecificProfile(BtMCSync_GetCurrentLinkBdAddr(),PROFILE_MCSYNC);
				return FALSE;

			case STATE_CASE_OFF:
				APP_PowerOff_Loader(POWEROFF_REASON_SMARTCASE_POWEROFF);
				return FALSE;

			case STATE_OUT_OF_CASE:
				if(BtAwsMce_IsDefaultRolePartner())
				{
					APP_MCSYNC_SendSyncSmtChgCseInfo(TRUE);
				}
				else
				{
					//app_SmtChgCse_AgentConnHandler(); //if connection terminated by AG, we shall not reconnect AG.
				}
				break;

			default:

				break;
		}
	}
	#endif

	return TRUE;
}

void APP_SmtChgCse_AgentRhoTimerHandler(BOOL isTimerSet)
{
	#ifdef SMART_CHG_CSE_DEVELOPMENT
	U8 sccState = DrvCharger_GetSmartCaseState();

	if(APP_ChgCase_IsCaseSmartType())
	{
		DBG_LOG_APP_SMART_CASE( "[APP_SmtChgCse] AgentRhoTimer, isTimerSet:%d, sccState:%d", 2, isTimerSet, sccState);

		if(isTimerSet && sccState == STATE_LID_OPEN)
		{
			FW_SetTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, NULL, 0, ChgCase_Smart_GetCaseRHOTimer());
		}
	}
	#else
	UNUSED(isTimerSet);
	#endif
}

BOOL APP_SmtChgCse_IsOutOfChargerCase(void)
{
	#ifndef SMART_CHG_CSE_LIDON_RECONNECT
	#ifdef SMART_CHG_CSE_DEVELOPMENT
	U8 sccState = DrvCharger_GetSmartCaseState();

	if(APP_ChgCase_IsCaseSmartType())
	{
		DBG_LOG_APP_SMART_CASE( "[APP_SmtChgCse] IsOutOfCase sccState:%d", 1, sccState);
		return (sccState == STATE_OUT_OF_CASE) ? TRUE : FALSE;
	}
	#endif
	#endif

	return TRUE;
}

void APP_SmtChgCse_SetCheckStateTimer(U32 time)
{
	#ifdef SMART_CHG_CSE_DEVELOPMENT
	if(APP_ChgCase_IsCaseSmartType())
	{
		FW_SetTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, NULL, 0, time);
	}
	#endif
}

void APP_SmtChgCse_ReleaseCheckStateTimer(void)
{
	#ifdef SMART_CHG_CSE_DEVELOPMENT
	if(APP_ChgCase_IsCaseSmartType())
	{
		FW_ReleaseTimer(&gAppSccTimerHandle, TIMER_CHECK_STATE, 0);
	}
	#endif
}

void APP_SmtChgCse_SetIsNeedRHO(BOOL need)
{
	#ifdef SMART_CHG_CSE_DEVELOPMENT
	if(APP_ChgCase_IsCaseSmartType())
	{
		gAppSccCtl.isNeedRho = need;
	}
	#else
	UNUSED(need);
	#endif
}

BOOL APP_SmtChgCse_GetIsNeedRHO(void)
{
	#ifdef SMART_CHG_CSE_DEVELOPMENT
	if(APP_ChgCase_IsCaseSmartType())
	{
		return gAppSccCtl.isNeedRho;
	}
	#endif

	return FALSE;
}

BOOL APP_SmtChgCse_IsConnToAGNeeded(void)
<<<<<<< HEAD
{	
=======
{
>>>>>>> db20e11 (second commit)
	#ifdef SMART_CHG_CSE_DEVELOPMENT

		U8 sccPState = APP_GetPartnerSmartChargeCaseState();
		U8 sccAState = DrvCharger_GetSmartCaseState();
		U8 mcsyncState = MCSYNC_GetMcsyncState();

		if(APP_ChgCase_IsCaseSmartType())
		{

			DBG_LOG_APP_SMART_CASE( "[APP_SmtChgCse] sccPState:%d, sccAState:%d, mcsyncState:%d, connState:%d", 3, sccPState, sccAState, mcsyncState);

<<<<<<< HEAD
			#ifndef SMART_CHG_CSE_LIDON_RECONNECT	
=======
			#ifndef SMART_CHG_CSE_LIDON_RECONNECT
>>>>>>> db20e11 (second commit)
				if((mcsyncState == MCSYNC_LINK_CONNECTED && sccPState == STATE_OUT_OF_CASE) || sccAState == STATE_OUT_OF_CASE)
				{
					return TRUE;
				}
			#else
<<<<<<< HEAD
				if((mcsyncState == MCSYNC_LINK_CONNECTED && (sccPState == STATE_OUT_OF_CASE || sccPState == STATE_LID_OPEN)) 
=======
				if((mcsyncState == MCSYNC_LINK_CONNECTED && (sccPState == STATE_OUT_OF_CASE || sccPState == STATE_LID_OPEN))
>>>>>>> db20e11 (second commit)
					||(sccAState == STATE_OUT_OF_CASE || sccAState == STATE_LID_OPEN))
				{
					return TRUE;
				}
<<<<<<< HEAD
			#endif 
			
			return FALSE;
		}
		
=======
			#endif

			return FALSE;
		}

>>>>>>> db20e11 (second commit)
	#endif

	return FALSE;
}

BOOL APP_SmtChgCse_IsPhoneNeeded(void)
{
	#ifndef SMART_CHG_CSE_LIDON_RECONNECT
	#ifdef SMART_CHG_CSE_DEVELOPMENT
	U8 sccPState = APP_GetPartnerSmartChargeCaseState();
	U8 sccAState = DrvCharger_GetSmartCaseState();
	U8 mcsyncState = MCSYNC_GetMcsyncState();
	U8 connState = PM_Conn_GetAgLinkState();

	if(APP_ChgCase_IsCaseSmartType())
	{
		DBG_LOG_APP_SMART_CASE( "[APP_SmtChgCse] APP_SmtChgCse_IsPhoneNeeded", 0);
		DBG_LOG_APP_SMART_CASE( "[APP_SmtChgCse] sccPState:%d, sccAState:%d, mcsyncState:%d, connState:%d", 4, sccPState,
			sccAState, mcsyncState, connState);

		if(mcsyncState == MCSYNC_LINK_CONNECTED && sccAState != STATE_OUT_OF_CASE && sccPState != STATE_OUT_OF_CASE)
		{
			return FALSE;
		}
		else if(mcsyncState != MCSYNC_LINK_CONNECTED && sccAState != STATE_OUT_OF_CASE)
		{
			return FALSE;
		}

		return TRUE;
	}
	#endif
	#endif

	return TRUE;
}

BOOL APP_SmtChgCse_IsAbleToRHO(void)
{
	#ifdef SMART_CHG_CSE_DEVELOPMENT
	U8 sccPState = APP_GetPartnerSmartChargeCaseState();
	U8 mcsyncState = MCSYNC_GetMcsyncState();

	if(APP_ChgCase_IsCaseSmartType())
	{
		DBG_LOG_APP_SMART_CASE( "[APP_SmtChgCse] IsAbleToRHO, sccPState:%d, mcsyncState:%d", 2, sccPState, mcsyncState);

		return (mcsyncState == MCSYNC_LINK_CONNECTED && sccPState == STATE_OUT_OF_CASE) ? TRUE : FALSE;
	}
	#endif

	return TRUE;
}
