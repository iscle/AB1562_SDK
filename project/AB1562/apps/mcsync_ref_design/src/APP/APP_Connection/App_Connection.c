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
#include "App_Connection.h"
#include "App_ResourceManager.h"
#include "AudioDSP_StreamManager.h"
#include "App_Nvram_Type.h"
#include "App_ReConnection.h"
#include "App_Interface.h"
#include "App_State.h"
#include "App_LinkKey.h"
#include "App_PowerOff.h"
#include "App_Media.h"
#include "App_Hfp.h"
#include "App_SniffHandler.h"
#include "APP_HfpScoHandler.h"
#include "APP_HfpIncomingCallMediaHandler.h"
#include "APP_Qos.h"
#include "App_EventOpcode.h"
#include "PM_Interface.h"
#include "App_System.h"
#include "APP_AirApp.h"
#include "system.h"
#include "App_Fcd.h"
#ifdef AIROHA_WWE_ENABLE
#include "App_Vad.h"
#endif
#include "drv_audio_nvkey.h"
#include "App_Ble.h"
#include "App_SmartChgCase.h"
#include "App_Customer_Func.h"
#ifdef MODULE_LOG_FEATURE
#include "BtModule_log.h"
#include "BtModule_log_table_module.h"
#endif

#include "BtMCSync.h"
#include "App_MCSync_Message.h"
#include "App_MCSync_RHO.h"
#include "App_VpRtControl.h"
#include "App_RhoHandler.h"
#include "App_Mcsync_LinkKey.h"
#include "App_Connection.h"
#include "drv_a2dp.h"
#include "App_InEarDetection.h"

<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
#include "App_MCSync_Share.h"
#endif
#include "App_Mcsync_Reconnection.h"
#include "App_Customer.h"
#ifdef PROFILE_AMA_ENABLE
#include "App_Ama.h"
#endif

#include "App_Mcsync_Reconnection.h"
#include "App_A2dp.h"

#define TIMER_ID_POWER_OFF_IN_ROLE_CHANNGING_DELAY      3000


/**************************************************************************************************
* Definition
**************************************************************************************************/
log_create_module(APP_CONN, PRINT_LEVEL_INFO);

/**************************************************************************************************
* Enumeration
**************************************************************************************************/

/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct
{
	U8 currentSupportDeviceNo;
	U8 isProfileRegistered;
}APP_CONNECTION_CTL_STRU;

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static U32 app_Connection_Handler(Handler handler, U16 id, void *msg, U32 handler_id);
static U32 app_ConnectionTimer_Handler(Handler handler, U16 id, void *msg, U32 handler_id);

/**************************************************************************************************
* Variables
**************************************************************************************************/
static const HandlerData gAppConnectionHandle = { app_Connection_Handler };
static const HandlerData gAppConnectionTimerHandle = { app_ConnectionTimer_Handler };
static APP_CONNECTION_CTL_STRU gAppConnCtl;
<<<<<<< HEAD
=======
extern APP_VPRT_INFO_STRU gVpRtInfo;
>>>>>>> db20e11 (second commit)

/**************************************************************************************************
* Static Functions
**************************************************************************************************/
static void app_Conn_SetConnected(BD_ADDR_T *pBdAddr)
{
	BOOL isInConnected;
	U8 role = BtAwsMce_GetDefaultRole();
	APP_SetAppLinkBdAddr(pBdAddr);
	APP_AddServiceBdAddr(pBdAddr);

	////Load link history info which was set in EEPROM
	APP_LinkKey_LoadHistory(pBdAddr);
	//if(FW_CmpBdAddr(pBdAddr, APP_Reconn_GetReconnectingBdAddr()))
		//APP_Reconn_SetReconnectingBdAddr(NULL);

	#ifdef LINEIN_ENABLE
	if(APP_EXIT_LINE_IN_WHEN_BT_CONNECED_FEAT && APP_State_CheckInLineInState())
	{
		APP_State_RmvState(APP_EOF, APP_LINE_IN);
	}
	#endif

	isInConnected = APP_State_CheckNestStateByLink(pBdAddr, APP_CONNECTED) ? TRUE : FALSE;
	APP_State_InsertState(pBdAddr, APP_CONNECTED, APP_State_CheckNestStateByLink(pBdAddr, APP_CONDISCABLE) ? APP_CONDISCABLE : APP_CONNECTABLE);

	DBG_LOG_APP_Connection( "[APP_CONN] PM_CONNECT_CFM isInConn:%d ConnDevCnt:%d", 2, isInConnected, APP_State_CountConnectedDev());

	if(APP_State_CountConnectedDev() >= APP_Conn_GetSupportDeviceNo())
	{
		if(APP_State_CheckNestStateByLink(APP_EOF, APP_CONDISCABLE))
		{
			APP_State_DiscoverabilityHandle(CMD_SET_NON_DISCOVERABLE);
		}
	}

	App_SniffEnable(pBdAddr, APP_SNIFF_ACL_READY_SLC_NOT_YET);

	if(!isInConnected)
	{
		if((BtAwsMce_IsDefaultRoleAgent() && BtAwsMce_IsSpecialLink(pBdAddr)) ||
		   (BtAwsMce_IsDefaultRolePartner() && (APP_GetAGNum() == 0)))
		{
			APP_Media_PushMediaEvent((U16)(MEDIA_EVT_SPECIAL_CONNECTED));
			DBG_LOG_APP_Connection( "[AUDIO DSP A2DP][Partner Later Join] Partner Set Later Join log string one", 0);
			drv_a2dp_set_partner_later_join(FALSE);
		}
		else if(role == ROLE_AGENT || (role == ROLE_NONE && APP_SUPPORT_ALL_ROLE_ENTER_DISCOVER_FEAT))
		{
<<<<<<< HEAD
			APP_Media_PushMediaEvent((U16)(MEDIA_EVT_SLC_CONNECTED + APP_State_CountConnectedDev()-1));
			APP_SetAGNum(APP_State_CountConnectedDev());

=======
			/*
			if(APP_Conn_CheckTimer(TIMER_ID_DISCONNECTED_VP) )
				APP_Conn_ReleaseTimer(TIMER_ID_DISCONNECTED_VP);
			*/
			//APP_Media_PushMediaEvent((U16)(MEDIA_EVT_SLC_CONNECTED + APP_State_CountConnectedDev()-1));
			#ifdef AIR_TAKE_OVER_LINK_ENABLE
			if(BtMCSync_GetMcsyncTakeOverConnState() == MCSYNC_TAKE_OVER_CONNECTION_IDLE_STATE)
			#endif
			{
				APP_Conn_SetTimer(TIMER_ID_AGENT_SLC_CONNECTED, 2500);//OPPOX20E1-178_0121
			}
			APP_SetAGNum(APP_State_CountConnectedDev());
			
			#ifdef AIR_TAKE_OVER_LINK_ENABLE
			if(APP_State_CountConnectedDev() == 2)
			{
				gVpRtInfo.isVpRtSecondDevice = TRUE;
				//APP_MCSYNC_SendSyncNumOfAGInfo(APP_GetAGNum());
			}
			#endif
>>>>>>> db20e11 (second commit)
			APP_Customer_ConnectionNoiseControl();
			APP_Customer_SetConnected_Control();
			APP_Customer_BatteryLevelVP();
		}

		APP_Customer_CancelConnectedFilter();

        FW_SetTimer((Handler)&gAppConnectionTimerHandle, TIMER_ID_QOS, NULL, (U32)APP_GetAppLinkByBdAddr(pBdAddr), 5*ONE_SEC);
	}

	//MMI_HCI_SendVCmdAudioQosSetup(linkIndex, SERVICE_TYPE_BEST_EFFORT, FALSE, FALSE);
}

static void app_Conn_ConnectCfmHandler(PM_CONNECT_CFM_T * cfm)
{
<<<<<<< HEAD
	DBG_LOG_APP_Connection( "[APP_CONN] PM_CONNECT_CFM PID:0x%x ConnProfileCnt:%d bda:0x%x%x", 4, cfm->profileID, cfm->profileCntConnectedEvt, FW_bdaddr_to_2U32(&cfm->bdAddr, TRUE), FW_bdaddr_to_2U32(&cfm->bdAddr, FALSE));
#ifdef MCSYNC_SHARE_MODE
=======
	DBG_LOG_APP_Connection( "[APP_CONN] PM_CONNECT_CFM PID:0x%x ConnProfileCnt:%d bda:0x%x%x, mcsync state:%d", 5, 
		cfm->profileID, cfm->profileCntConnectedEvt, FW_bdaddr_to_2U32(&cfm->bdAddr, TRUE), 
		FW_bdaddr_to_2U32(&cfm->bdAddr, FALSE), BtAwsMce_GetMcsyncState());

#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
	if(APP_MCSync_Share_FollowerEventHandler(APP_SHARE_FOLLOWER_EVENT_AG_CONNECT_CFM, &cfm->bdAddr, NULL))
	{
		APP_ReConn_DeleteInvalidListByBdAddr(&cfm->bdAddr);
		return;
	}
#endif
	//memcpy((U8*)&gAppCtl.latestConnectedBdaddr.addr[0], (U8*)&cfm->bdAddr, sizeof(BD_ADDR_T)); //Read rssi
	if(APP_SmtChgCse_GetIsNeedRHO() && cfm->profileID != PROFILE_MCSYNC)
	{
		DBG_LOG_APP_Connection( "[APP_CONN] SmtChgCse need to RHO delay", 0);
		APP_SmtChgCse_SetCheckStateTimer(3000);
	}

	APP_ReConn_DeleteInvalidListByBdAddr(&cfm->bdAddr);
<<<<<<< HEAD
	APP_ReConn_PourQueue();
=======
	APP_ReConn_PourQueue(0);
>>>>>>> db20e11 (second commit)

	if(cfm->profileCntConnectedEvt && cfm->profileID == PROFILE_A2DP)
	{
		U16 timer = 1000;

		if(BtAwsMce_IsDefaultRoleAgent() && APP_GetAGNum())
		{
			if(APP_IsScoConnected(APP_GetServiceBdAddr()))
				timer = 4000; //wait for phone to connect AVRCP with us
		}

		PM_SetReconnectProfile(&cfm->bdAddr, PROFILE_AVRCP, APP_ReConn_GetProfileMaskByBdAddr(&cfm->bdAddr) ? 0 : timer);
	}

#ifdef AIROHA_WWE_ENABLE
    if(cfm->profileID == PROFILE_HANDSFREE && drv_audio_nvkey_is_feature_on(AUDIO_DSP_VAD_ALWAYS_ON_FEATURE))
    {
        APP_VAD_On();
    }
#endif

	// Any profile is connected we call this is in connected state
	if (cfm->profileCntConnectedEvt == 1)
	{
		app_Conn_SetConnected(&cfm->bdAddr);
	}
//	MMI_TIMER_ReleaseDetachTimer(linkIndex);

<<<<<<< HEAD
	if(!APP_System_IsMpTestMode() && !APP_FCD_IsFcdReset() && !FW_IsBdAddrZero(BtMCSync_ReadAgentBdAddr()))
	{
		if(!APP_LinkKey_IsBdAddrInHistory(&cfm->bdAddr))
		{
			APP_LinkKey_UpdateHistory(&cfm->bdAddr, TRUE);
=======
	#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
	if(cfm->profileCntConnectedEvt >=2 && BtAwsMce_GetMcsyncState() == MCSYNC_LINK_CONNECTED 
		&& BtAwsMce_IsDefaultRoleAgent() && !BtAwsMce_IsSpecialLink(&cfm->bdAddr))
	{
		APP_MCSync_Share_AgentEventHandler(APP_SHARE_AGENT_EVENT_PARTNER_CONNECTED_CFM);
	}
	#endif

	if(!APP_System_IsMpTestMode() && !APP_FCD_IsFcdReset() && !FW_IsBdAddrZero(BtMCSync_ReadAgentBdAddr()))
	{
		if(!APP_LinkKey_IsBdAddrInHistory(&cfm->bdAddr) || APP_LinkKey_SearchLinkKeyIndex(&cfm->bdAddr, TRUE) != 0 )
		{
            //need update & sync local key to remote again when AG profile is not the first connected profile because PROFILE_MCSYNC delete the link history below.
		    APP_LinkKey_UpdateHistory(&cfm->bdAddr, TRUE);
            APP_Mcsync_GetLocalLinkKey();
>>>>>>> db20e11 (second commit)
		}

		if(cfm->profileID == PROFILE_MCSYNC)
		{
			#ifdef MODULE_LOG_FEATURE
			BtModuleLog_SendLogHandler(MODULE_LOG_MODULE_APP_MCSYNC,
				MODULE_LOG_MODULE_APP_MCSYNC_SUB_MODULE_CONNECTION, 1);
			#endif
<<<<<<< HEAD
			BD_ADDR_T *pBdAddr = (BtAwsMce_IsDefaultRoleAgent()) ? BtAwsMce_ReadLocalBdAddr() : BtMCSync_ReadAgentBdAddr();
			if(FW_CmpBdAddr(&cfm->bdAddr, pBdAddr))
			{
				APP_LinkKey_DeleteLinkHistory(&cfm->bdAddr); // remove agent from phone list
=======
			
			BD_ADDR_T *pBdAddr = (BtAwsMce_IsDefaultRoleAgent()) ? BtAwsMce_ReadLocalBdAddr() : BtMCSync_ReadAgentBdAddr();
			if(FW_CmpBdAddr(&cfm->bdAddr, pBdAddr) || ((cfm->profileCntConnectedEvt == 1) && !BtAwsMce_IsSpecialLink(&cfm->bdAddr)))
			{
				APP_LinkKey_DeleteLinkHistory(&cfm->bdAddr); // remove agent from phone list, or remove link info when only mcsync profile connected on normal aws link for avoiding reconnect sp.
>>>>>>> db20e11 (second commit)
			}

			APP_Mcsync_SyncLinkKey();
		}
	}

	if(APP_State_CheckNestStateByLink(APP_EOF, APP_AWS_PAIRING))
		APP_Conn_ReleaseMultiLink();
}

void APP_Conn_RemoveConnectedState(BD_ADDR_T *pBdAddr, U8 profileCnt)
{
	U8 state;
<<<<<<< HEAD
=======

	#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
	U8 shareMode = BtMCSync_GetShareMode();
	#endif
	
>>>>>>> db20e11 (second commit)
	if(!profileCnt)
	{
		state = APP_State_GetTopState(pBdAddr);

<<<<<<< HEAD
=======
		DBG_LOG_APP_Connection( "[APP_CONN] RemoveConnectedState, BDA:0x%x%x, agNum:%d, role:0x%x, special link:%d, state:%d", 6,
			FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE), APP_GetAGNum(),
			BtAwsMce_GetDefaultRole(), BtAwsMce_IsSpecialLink(pBdAddr), state);

        if(APP_State_CheckLinkConnected(pBdAddr))
        {
            if(BtAwsMce_IsDefaultRoleAgent())
            {
                if(!BtAwsMce_IsSpecialLink(pBdAddr)
					#ifdef AIR_TAKE_OVER_LINK_ENABLE
					&&
					BtMCSync_GetMcsyncTakeOverConnState() == MCSYNC_TAKE_OVER_CONNECTION_IDLE_STATE
					#endif
				  )
                {

					#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
					if(shareMode == MCSYNC_SHARE_MODE_DISABLE)
					#endif
					{
	                    APP_Conn_SetTimer(TIMER_ID_DISCONNECTED_VP, 3500);

						if(APP_Conn_CheckTimer(TIMER_ID_ACL_LINK_TIMER))
						{
							APP_Conn_ReleaseTimer(TIMER_ID_ACL_LINK_TIMER);
						}
					}
                }
            }
            else if(BtAwsMce_IsDefaultRolePartner()) // partner disconnect with phone
            {
            	#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
				if(shareMode == MCSYNC_SHARE_MODE_DISABLE)
				#endif
				{
	                if(APP_GetAGNum() > 0)
	                {
	                    APP_Conn_SetTimer(TIMER_ID_DISCONNECTED_VP, 3500);
	                }
				}
            }
        }

>>>>>>> db20e11 (second commit)
		if(state >= APP_HFP_INCOMMING && state <= APP_HFP_CAMULTY)
		{
			APP_State_RmvState(pBdAddr, state);
		}
		APP_State_RmvState(pBdAddr, APP_CONNECTED);
		APP_ClearAppLinkBdAddr(pBdAddr);
	}
}

static void app_Conn_DisconnectCfmHandler(PM_DISCONNECT_CFM_T *cfm)
{
	BD_ADDR_T *pBdAddr = &cfm->bdAddr;

	DBG_LOG_APP_Connection("[APP_CONN] PM_DISCONNECT_CFM PID:0x%x, profile cnt:%d, isActice Disc:%d status:%x", 4, cfm->profileID,
				cfm->profileCntConnectedEvt, APP_IsLinkActiveDisconnecting(pBdAddr), cfm->status);

#ifdef AIROHA_WWE_ENABLE
	if(cfm->profileID == PROFILE_HANDSFREE && drv_audio_nvkey_is_feature_on(AUDIO_DSP_VAD_ALWAYS_ON_FEATURE))
	{
		APP_VAD_Off();
	}
#endif

<<<<<<< HEAD
#ifdef PROFILE_AMA_ENABLE
    if (!PM_IsProfileConnected(pBdAddr, PROFILE_A2DP) && !PM_IsProfileConnected(pBdAddr, PROFILE_HANDSFREE) && !PM_IsProfileConnected(pBdAddr, PROFILE_AVRCP))
    {
=======
#if defined(PROFILE_AMA_ENABLE) || defined(AIR_GFP_ENABLE)
    if (!PM_IsProfileConnected(pBdAddr, PROFILE_A2DP) && !PM_IsProfileConnected(pBdAddr, PROFILE_HANDSFREE) && !PM_IsProfileConnected(pBdAddr, PROFILE_AVRCP))
    {
#ifdef PROFILE_AMA_ENABLE
>>>>>>> db20e11 (second commit)
        if (PM_IsProfileConnected(pBdAddr, PROFILE_AMA))
        {
            APP_Ama_Disconnect(pBdAddr);
            PM_DisconnectSpecificProfile(pBdAddr, PROFILE_AMA);
        }
<<<<<<< HEAD
=======
#endif

#ifdef AIR_GFP_ENABLE
        if (PM_IsProfileConnected(pBdAddr, PROFILE_GFP))
        {
            PM_DisconnectSpecificProfile(pBdAddr, PROFILE_GFP);
        }
#endif
>>>>>>> db20e11 (second commit)
    }
#endif

	if(cfm->profileID == PROFILE_MCSYNC && BtAwsMce_IsDefaultRolePartner())
	{
		DBG_LOG_APP_Connection("[APP_CONN] close a2dp transform", 0);
		APP_A2dpDspStop(pBdAddr);
	}

	// active release link
	if(!APP_PowerOff_IsPowerOn() || APP_IsLinkActiveDisconnecting(pBdAddr))
	{
		APP_Conn_RemoveConnectedState(pBdAddr, cfm->profileCntConnectedEvt);

		if(cfm->profileID != PROFILE_MCSYNC)
		{
			DBG_LOG_APP_Connection("[APP_CONN] release and detach 1", 0);
			APP_Conn_ReleaseProfileLinkAndDetach(pBdAddr);
		}
	}
	else
	{
		if(cfm->profileID == PROFILE_AIR_APP)
			APP_MCSync_Rho_AirAppDisconnectNotify();

		if (!APP_IsActiveLink(pBdAddr))
			goto REMOVE_CONNECTED_IMMEDIATLY;

        if(BtAwsMce_IsDefaultRolePartner() && cfm->profileID == PROFILE_MCSYNC && cfm->status == HCI_STATUS_CONNECTION_TIMEOUT) {
            APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);
            DBG_LOG_APP_Connection("[APP_CONN] Partner Linkloss:%x%x", 2, FW_bdaddr_to_2U32(&cfm->bdAddr, TRUE), FW_bdaddr_to_2U32(&cfm->bdAddr, FALSE));
            if(pLinkInfo != NULL) {
                pLinkInfo->linkPara.miscMask = APP_LINK_LOSS;
            }
        }

		if(cfm->profileCntConnectedEvt)
<<<<<<< HEAD
		{			
=======
		{
>>>>>>> db20e11 (second commit)
			return;
		}

		//if(gMMI_ctl.mmiTimerInfo.connTimer[pBdAddr].delayRemoveConnectedTimer == (OST XDATA_PTR)NULL) //asus zenfone 5
		//{
		//	SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.connTimer[pBdAddr].delayRemoveConnectedTimer, 640L);
		//	return;
		//}

		REMOVE_CONNECTED_IMMEDIATLY:
		PM_ClearAllReconnectProfiles(pBdAddr);
		BtSdpServerDisconnectReq((U8 *)pBdAddr, SDP_SERVER_DISCON_SINGLE);
		APP_Conn_RemoveConnectedState(pBdAddr, cfm->profileCntConnectedEvt);
	}
}

static U32 app_Connection_Handler(Handler handler, U16 id, void *msg, U32 handler_id)
{
	UNUSED(handler);
	UNUSED(handler_id);

	switch (id)
	{
		case PM_REGISTER_APP_CFM:
			DBG_LOG_APP_Connection( "[APP_CONN] PM_REGISTER_APP_CFM", 0);
			break;

		case PM_CONNECT_CFM:
			app_Conn_ConnectCfmHandler((PM_CONNECT_CFM_T *)msg);
			break;

		case PM_DISCONNECT_CFM:
			app_Conn_DisconnectCfmHandler((PM_DISCONNECT_CFM_T *)msg);
			break;

		case PM_REGISTER_ALL_PROFILE_CFM:
			DBG_LOG_APP_Connection( "[APP_CONN] All Profile Registered Cfm: isPowerOn:%d, isFOTA:%d", 2, APP_PowerOff_IsPowerOn(), APP_AirApp_FOTA_Reconnect() );

			gAppConnCtl.isProfileRegistered = TRUE;
<<<<<<< HEAD
			APP_ReConn_PourQueue();
            
            APP_Ble_StartGattService();
            
=======
			APP_ReConn_PourQueue(0);

            APP_Ble_StartGattService();

>>>>>>> db20e11 (second commit)
			if(
				(APP_State_CheckNestStateByLink(APP_EOF, APP_CONDISCABLE) || APP_State_CheckNestStateByLink(APP_EOF, APP_CONNECTABLE)) ||
				( APP_AirApp_FOTA_Reconnect() || (SYS_BOOT_REASON_STATUS() == BOOT_CODE_FROM_DEEP_SLEEP) || !APP_PowerOff_IsPowerOn())
			)
			{
				APP_State_SendMsgToHandler(NULL, APP_CHECK_POWERON_TIMER, APP_CHECK_POWERON_TIMER, FALSE);
			}
			else
			{
				return 2;
			}
			break;

		case BT_GAP_CANCEL_CONN_CFM:
			break;

		default:
			DBG_LOG_APP_Connection( "[APP_CONN] Undefined message = %d", 1, id);
			break;
	}
	return 0;
}

<<<<<<< HEAD
#ifdef DISCONNECT_AGENT_RECOVERY
=======
#ifdef AIR_AGENT_RECOVERY_MODE_AFTER_DISCONNECTION_FROM_PHONE_ENABLE
>>>>>>> db20e11 (second commit)
static void app_connection_time_power_off_aws_check_handler(void)
{
    U8 timer_1 = app_mcsync_reconnect_check_super_reconnect_timer(RECONNECT_LINK_LOSS);
    U8 timer_2 = app_mcsync_reconnect_check_super_reconnect_timer(RECONNECT_PWR_ON);
    U8 timer_3 = app_mcsync_reconnect_check_super_reconnect_timer(RECONNECT_USER_INIT);
    DBG_LOG_APP_Connection( "[APP_CONN] POWER_OFF_AWS_CHECK:%x,%x,%x", 3, timer_1, timer_2, timer_3);
    if(timer_1 == 0 && timer_2 == 0 && timer_3 == 0) {
        APP_ReConn_Reconnect(RECONNECT_LINK_LOSS);
    }
}
#endif

static U32 app_ConnectionTimer_Handler(Handler handler, U16 id, void *msg, U32 handler_id)
{
    APP_INFO_STRU *pLinkInfo;
<<<<<<< HEAD
	UNUSED(handler);
	UNUSED(msg);

	switch(id)
	{
		case TIMER_ID_PARTNER_PLAY_SLC_CONNECTED_VP: 		
			
			//APP_Media_PushMediaEvent(MEDIA_EVT_SLC_CONNECTED);
		case TIMER_ID_DISCONNECTED_VP:
			DBG_LOG_APP_Connection( "[APP_CONN] timer handler, push slc disc media event", 0);
			APP_Media_PushMediaEvent(MEDIA_EVT_SLC_DISCONNECTED);
=======
	U32 value = 0;
	U32 *pMsg;
	U8 state;

	#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
	U8 shareMode = BtMCSync_GetShareMode();
	#endif

	UNUSED(handler);
	UNUSED(msg);

	#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
   	DBG_LOG_APP_Connection( "[APP_CONN] timer handler, id:%d, isPowerOn:%d, share mode:%d", 3, id, APP_PowerOff_IsPowerOn(), shareMode);
	#else
	DBG_LOG_APP_Connection( "[APP_CONN] timer handler, id:%d, isPowerOn:%d", 2, id, APP_PowerOff_IsPowerOn());
	
	#endif
	
	switch(id)
	{
		case TIMER_ID_AGENT_SLC_CONNECTED:
			APP_Media_PushMediaEvent((U16)(MEDIA_EVT_SLC_CONNECTED + APP_State_CountConnectedDev()-1));
			break;

		#ifdef AIR_TAKE_OVER_LINK_ENABLE

		case TIMER_ID_TAKE_OVER_SLC_CONNECTED:
			if(APP_GetAGNum() > 0)
			{
				if(APP_Conn_CheckTimer(TIMER_ID_DISCONNECTED_VP))
					APP_Conn_ReleaseTimer(TIMER_ID_DISCONNECTED_VP);

				APP_MCSYNC_SendSyncTakeOverStateInfo();
				APP_Media_PushMediaEvent(MEDIA_EVT_SLC_CONNECTED);
				BtMCSync_SetMcsyncTakeOverConnState(MCSYNC_TAKE_OVER_CONNECTION_IDLE_STATE);
				APP_MCSYNC_SendSyncTakeOverStateInfo();
			}
			break;
		case TIMER_ID_CHECK_TAKE_OVER_RECONNECT:
			state = BtMCSync_GetMcsyncTakeOverConnState();

			printf( "[APP_CONN] TIMER_ID_CHECK_TAKE_OVER_RECONNECT, %d %d %d",
				APP_GetAGNum(), BtAwsMce_IsDefaultRoleAgent(), state);

			if(!APP_GetAGNum() && BtAwsMce_IsDefaultRoleAgent() &&
				state == MCSYNC_TAKE_OVER_CONNECTION_DISCONNECTED_LOW_PRIO_COMPLETE_STATE_AND_CONNECT_HIGH_PRIO_AWS)
			{
				APP_ReConn_Reconnect(RECONNECT_USER_INIT);
				BtMCSync_SetMcsyncTakeOverConnState(MCSYNC_TAKE_OVER_CONNECTION_IDLE_STATE);
			}

			break;
		#endif

		case TIMER_ID_DISCONNECTED_VP:

			#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
			if(shareMode != MCSYNC_SHARE_MODE_DISABLE)
			{
				return 0;
			}
			#endif
			
            if(APP_PowerOff_IsPowerOn())
                APP_Media_PushMediaEvent(MEDIA_EVT_SLC_DISCONNECTED);
			break;

		case TIMER_ID_SLC_CONNECTED_VP:

			#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
			if(shareMode != MCSYNC_SHARE_MODE_DISABLE)
			{
				return 0;
			}
			#endif
			
			pMsg = (U32*)msg;
			if(pMsg)
			{
				value = *pMsg;
				if(value != TIMER_PARTNER_VP_IF_SLC_TIMER)
				{
					if(APP_PowerOff_IsPowerOn())
	                	APP_Media_PushMediaEvent(MEDIA_EVT_SLC_CONNECTED);
				}
			}
			else
			{
	            if(APP_PowerOff_IsPowerOn())
	                APP_Media_PushMediaEvent(MEDIA_EVT_SLC_CONNECTED);
			}
>>>>>>> db20e11 (second commit)
			break;

		case TIMER_ID_CLEAR_LINK_KEY:
			APP_LinkKey_ClearAllHistory();
			break;

		case TIMER_ID_WAIT_DISCONNECTED_CLEAR_LINK_KEY:
			if(LINK_DISCONNECTED == APP_Conn_GetAgLinkState())
				FW_SetTimer((Handler)&gAppConnectionTimerHandle, TIMER_ID_CLEAR_LINK_KEY, NULL, 0, 500); //wait for partner connecting
			else
				APP_Conn_SetTimer(TIMER_ID_WAIT_DISCONNECTED_CLEAR_LINK_KEY, 0);
			break;
        case TIMER_ID_POWER_OFF_IN_ROLE_CHANNGING:
            DBG_LOG_APP_Connection( "[APP_SYS] TIMER_ID_POWER_OFF_IN_ROLE_CHANNGING:%x", 1, APP_PowerOff_IsPowerOn());
            if(!APP_PowerOff_IsPowerOn())
            {
                APP_PowerOff_Proce();
            }
            break;

		case TIMER_ID_PIN_OR_KEY_MISSING_AWS_CHECK:
			{
				U8 mcsyncState = MCSYNC_GetMcsyncState();
				U8 connState = APP_Conn_GetAgLinkState();
				DBG_LOG_APP_Connection( "[APP_CONN] TIMER_ID_PIN_OR_KEY_MISSING_AWS_CHECK:%x,%x", 2, mcsyncState, connState);
				if(mcsyncState == MCSYNC_LINK_CONNECTING && connState == LINK_DISCONNECTED)
				{
					APP_Mcsync_ExitSuperReconnectMode(FALSE);
					APP_Mcsync_EnterSuperReconnectMode(RECONNECT_PWR_ON);
				}
			}
			break;

        case TIMER_ID_QOS:
            pLinkInfo = (APP_INFO_STRU *)handler_id;
            DBG_LOG_APP_Connection( "[APP_CONN] TIMER_ID_QOS, aclState:%d, BdAddr:0x%x%x, isRhoing:%d", 4, pLinkInfo->aclState, FW_bdaddr_to_2U32(&pLinkInfo->bdAddr, TRUE), FW_bdaddr_to_2U32(&pLinkInfo->bdAddr, FALSE), APP_MCSync_Rho_IsRhoing());
            if(pLinkInfo->aclState == ACL_STATE_ON)
            {
                if(APP_MCSync_Rho_IsRhoing())
                {
                    FW_SetTimer((Handler)&gAppConnectionTimerHandle, TIMER_ID_QOS, NULL, (U32)pLinkInfo, 5*ONE_SEC);
                }
                else if(!APP_AudioDspIsStreaming(&pLinkInfo->bdAddr, AUDIO_A2DP))
                {
                    APP_Qos_SetQos(&pLinkInfo->bdAddr, QOS_SERVICE_BEST_EFFORT);
                }
            }
			break;

<<<<<<< HEAD
#ifdef DISCONNECT_AGENT_RECOVERY
=======
        case TIMER_ID_DISCONNECT_SPECIAL_LINK:
            DBG_LOG_APP_Connection( "[APP_CONN] TIMER_ID_DISCONNECT_SPECIAL_LINK: power on=%d, mcsync= %d", 2, APP_PowerOff_IsPowerOn(), BtAwsMce_GetMcsyncState());
            if (!APP_PowerOff_IsPowerOn() && BtAwsMce_GetMcsyncState() >= MCSYNC_LINK_SAWS_READY)
            {
                BtAwsMce_DisconnectAwsLink();
            }
            break;

#ifdef AIR_AGENT_RECOVERY_MODE_AFTER_DISCONNECTION_FROM_PHONE_ENABLE
>>>>>>> db20e11 (second commit)
        case TIMER_ID_POWER_OFF_AWS_CHECK:
            app_connection_time_power_off_aws_check_handler();
            break;
#endif
	}

	return 0;
}

static void app_Conn_InitSupportDeviceNo(void)
{
	gAppConnCtl.currentSupportDeviceNo = APP_MAX_PAGE_DEVICE;
	PM_SetMaximumEDRLinkNum(gAppConnCtl.currentSupportDeviceNo);
}

/**************************************************************************************************
* Public Functions (Connection API)
**************************************************************************************************/
void APP_RemoveServiceBdAddr(BD_ADDR_T *pBdAddr)
{
	BD_ADDR_T *pNextBdAddr;

	if(!APP_PowerOff_IsPowerOn() || !pBdAddr)
		return;

	if(APP_IsServiceBdAddr(pBdAddr))
	{
		switch(APP_GetCurrentAudioDevice())
		{
			case AUDIO_DEVICE_SCO:
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE                
			case AUDIO_DEVICE_SCO_HSP:
#endif                
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE
			case AUDIO_DEVICE_SCO_HSP:
#endif
>>>>>>> db20e11 (second commit)
			case AUDIO_DEVICE_SCO_IDLE:
			case AUDIO_DEVICE_MUSIC:
			case AUDIO_DEVICE_MUSIC_CALL_NOTIFY:
			case AUDIO_DEVICE_CALL_STATE:
			case AUDIO_DEVICE_AVRCP_PLAY:
			case AUDIO_DEVICE_KEY_AT_CMD:
				DBG_LOG_APP_Connection( "[APP_SYS] Zombie Audio Device", 0);
				/*fallthrough*/
			case AUDIO_DEVICE_LINE_IN:
			case AUDIO_DEVICE_FM:
			case AUDIO_DEVICE_MIC_TO_SPK:
#ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL
			case AUDIO_DEVICE_MP3:
#endif//MP3_LOCAL_PLAYBACK_MMI_CTRL
#ifdef PROFILE_AMA_ENABLE
            case AUDIO_DEVICE_AMA:
#endif
			case AUDIO_DEVICE_NONE:
                if((pNextBdAddr = APP_GetNextNormalLink(pBdAddr)) == APP_EOF)
                {
                    APP_ClearServiceBdAddr();
                }
                if(pNextBdAddr)
                    APP_SetServiceBdAddr(pNextBdAddr);
                else
                    APP_ClearServiceBdAddr();
				break;

			default:
				FW_Assert(FALSE);
                break;
		}
	}
}

void APP_AddServiceBdAddr(BD_ADDR_T *pBdAddr)
{
	BD_ADDR_T *pNextBdAddr;

	if(!APP_PowerOff_IsPowerOn() || !pBdAddr)
		return;

	if(	APP_IsLinkInHighAudioPriority(pBdAddr))
	{
		pNextBdAddr = APP_GetNextNormalLink(pBdAddr);

		if(BtAwsMce_IsDefaultRoleAgent() && pNextBdAddr && BtAwsMce_IsSpecialLink(pNextBdAddr))
		{
			APP_SetServiceBdAddr(pBdAddr);
			return;
		}

		if(pNextBdAddr && APP_State_CheckNestStateByLink(pNextBdAddr, APP_CONNECTED))
			return;

		APP_SetServiceBdAddr(pBdAddr);
	}
}

void APP_Conn_CreateActiveProfileConnection(BD_ADDR_T *pBdAddr,U8 profilemask)
{
<<<<<<< HEAD
=======
	DBG_LOG_APP_Connection( "[APP_CONN] CreateActiveProfileConnection mask:0x%x, bdAddr:0x%x%x", 3, profilemask, FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE));
>>>>>>> db20e11 (second commit)
	//PM_ConnectProfile(pBdAddr, PROFILE_AVRCP);		//Sony IOT Issue. Reconnect AVRCP After A2DP Connect
	PM_ConnectProfile(pBdAddr, PROFILE_MCSYNC);

	#ifdef PROFILE_AMA_ENABLE
	if((profilemask & RECONNECT_PROFILE_A2DP)
		&& (profilemask & RECONNECT_PROFILE_HFP)
		&& (profilemask & RECONNECT_PROFILE_AVRCP))
	{
		PM_ConnectProfile(pBdAddr, PROFILE_A2DP);
		PM_ConnectProfile(pBdAddr, PROFILE_HANDSFREE);
		PM_ConnectProfile(pBdAddr, PROFILE_AVRCP);
        PM_ConnectProfile(pBdAddr, PROFILE_IAP2);
		//APP_Reconn_SetReconnectingBdAddr(pBdAddr);
	}
	#else
	if((profilemask & RECONNECT_PROFILE_A2DP)
		&& (profilemask & RECONNECT_PROFILE_HFP)
		&& (profilemask & RECONNECT_PROFILE_AVRCP))
	{
		PM_ConnectProfile(pBdAddr, PROFILE_A2DP);
		PM_ConnectProfile(pBdAddr, PROFILE_HANDSFREE);
		PM_ConnectProfile(pBdAddr, PROFILE_AVRCP);
		//APP_Reconn_SetReconnectingBdAddr(pBdAddr);
	}
	#endif

	#ifdef AIROHA_BT_SPP_ENABLE
	//if(profilemask & RECONNECT_PROFILE_CUSTAPP)
	//	PM_ConnectProfile(pBdAddr, PROFILE_SERIAL_PORT);
	#endif
}

void APP_Conn_CancelCreateConnection(BD_ADDR_T *pBdAddr)
{
	if(!(BtAwsMce_IsDefaultRoleAgent() && FW_CmpBdAddr(pBdAddr, BtAwsMce_ReadLocalBdAddr())) &&
		!(BtAwsMce_IsDefaultRolePartner() && FW_CmpBdAddr(pBdAddr, BtMCSync_ReadAgentBdAddr())))
	{
		BtCancelCreateConnection((Handler)&gAppConnectionHandle, &pBdAddr->addr[0]);
	}
}

void APP_Conn_ReleaseProfileLinkAndDetach(BD_ADDR_T *pBdAddr)
{
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	if (!pLinkInfo)
	{
		if(!APP_PowerOff_IsPowerOn())
		{
            if(!MCSYNC_IsRoleChanging() && APP_Conn_CheckTimer(TIMER_ID_POWER_OFF_IN_ROLE_CHANNGING) != 0)
            {
                APP_PowerOff_Proce();
            }
		}
		DBG_LOG_APP_Connection( "[APP_CONN] No Link to Detach:%x,%x,%x", 3, APP_PowerOff_IsPowerOn(), MCSYNC_IsRoleChanging(), APP_Conn_CheckTimer(TIMER_ID_POWER_OFF_IN_ROLE_CHANNGING));
		return;
	}

<<<<<<< HEAD
 
=======

>>>>>>> db20e11 (second commit)
	if (pLinkInfo->linkPara.miscMask & APP_LINK_DETACHING)
		return;

	pLinkInfo->linkPara.miscMask |= APP_ACTIVE_DISCONNECT;
	App_SniffDisable(pBdAddr, APP_SNIFF_DISC);
	APP_ReConn_DeleteListByBdAddr(pBdAddr);

	APP_ImgMedia_AudioVoiceHandler(pBdAddr, IMG_AUDIO_VOICE_STOP);
	APP_HfpSco_DisconnectRequest(pBdAddr);
	//release one profile at a time
	//force detach and power off if timeout
	PM_ClearAllReconnectProfiles(pBdAddr);
	BtSdpServerDisconnectReq((U8 *)pBdAddr, SDP_SERVER_DISCON_SINGLE);
	DBG_LOG_APP_Connection( "[APP_CONN] PM_DisconnectLoadedProfile", 0);
	if (!PM_DisconnectLoadedProfile(pBdAddr) || APP_SCO_EXISTS(pBdAddr))
	{

	}
	else if (APP_IsActiveLink(pBdAddr))
	{
		if(!APP_State_GetConnectedCount() && !APP_PowerOff_IsPowerOn())
		{
			APP_State_AddTopState(APP_EOF, APP_DETACHING_LINK);
<<<<<<< HEAD

=======
#if 0
>>>>>>> db20e11 (second commit)
			if(BtAwsMce_IsDefaultRoleAgent())
			{
				APP_SetACLState(pLinkInfo, ACL_STATE_OFF);
				APP_ClearAppLinkBdAddr(pBdAddr);
			}
<<<<<<< HEAD

=======
#endif
>>>>>>> db20e11 (second commit)
		}
	}
}

void APP_Conn_DetachAllLinks(void)
{
	U8 linkIndex;

	for(linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		BD_ADDR_T *pBdAddr = APP_GetBdAddrByLinkIndex(linkIndex);

		if(pBdAddr)
		{
			BtDetchAclLink((Handler)APP_GetSystemHandle(), pBdAddr->addr);
		}
	}
}

void APP_Conn_ReleaseMultiLink(void)
{
	U8 linkIndex;

	for(linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		BD_ADDR_T *pBdAddr = APP_GetBdAddrByLinkIndex(linkIndex);

		if(pBdAddr)
		{
			DBG_LOG_APP_Connection("[APP_CONN] release and detach 2", 0);
			APP_Conn_ReleaseProfileLinkAndDetach(pBdAddr);
		}
	}
}

BOOL APP_Conn_CheckProfileOnAllLinks(void)
{
	U8 linkIndex;
	for(linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		BD_ADDR_T *pBdAddr = APP_GetBdAddrByLinkIndex(linkIndex);

		if(pBdAddr && !BtAwsMce_IsSpecialLink(pBdAddr) && PM_GetConnectedProfileCount(pBdAddr, TRUE))  // not include SDP
		{
			return TRUE;
		}
	}
	return FALSE;
}

void APP_Conn_ReleaseAWSLink(void)
{
	U8 linkIndex;

	for(linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		BD_ADDR_T *pBdAddr = APP_GetBdAddrByLinkIndex(linkIndex);

		if(pBdAddr && !FW_CmpBdAddr(BtAwsMce_ReadLocalBdAddr(), pBdAddr))
		{
			DBG_LOG_APP_Connection("[APP_CONN] release and detach 3", 0);
			APP_Conn_ReleaseProfileLinkAndDetach(pBdAddr);
		}
	}
}

BOOL APP_Conn_IsOnlySpecialAWS(void)
{
	U8 linkIndex;
	U8 cnt = 0;
	for(linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		if(gAppCtl.appInfo[linkIndex].aclState == ACL_STATE_ON)
		{
			cnt++;
		}
	}

	if(cnt == 1)
	{
		for(linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
		{
			if(BtAwsMce_IsSpecialLink(&gAppCtl.appInfo[linkIndex].bdAddr))
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}


/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void APP_Conn_Register(void)
{
	PM_RegisterApp((Handler)&gAppConnectionHandle);
	app_Conn_InitSupportDeviceNo();
}

U8 APP_Conn_GetSupportDeviceNo(void)
{
	return gAppConnCtl.currentSupportDeviceNo;
}

void APP_Conn_SetSupportDeviceNo(BOOL isIncrease)
{
	if(isIncrease)
	{
		if(gAppConnCtl.currentSupportDeviceNo < APP_MAX_PAGE_DEVICE)
		{
			gAppConnCtl.currentSupportDeviceNo++;
			PM_SetMaximumEDRLinkNum(gAppConnCtl.currentSupportDeviceNo);
		}
	}
	else
	{
		if (gAppConnCtl.currentSupportDeviceNo > 1)
		{
			gAppConnCtl.currentSupportDeviceNo--;
			PM_SetMaximumEDRLinkNum(gAppConnCtl.currentSupportDeviceNo);
		}
	}
}

BOOL APP_Conn_PowerOffHandler(void)
{
	APP_ReConn_ClearCtl();

	if(APP_Conn_IsOnlySpecialAWS())
	{
<<<<<<< HEAD
		BtAwsMce_DisconnectAwsLink();
=======
	    if (APP_Conn_CheckTimer(TIMER_ID_DISCONNECT_SPECIAL_LINK))
        {
            APP_Conn_ReleaseTimer(TIMER_ID_DISCONNECT_SPECIAL_LINK);
        }
        if (BtAwsMce_IsDefaultRoleAgent() &&
            (MCSYNC_GetMcsyncState() == MCSYNC_LINK_CONNECTING || MCSYNC_GetMcsyncState() == MCSYNC_LINK_CONNECTED) &&
            PM_IsProfileIdLoaded(BtAwsMce_ReadLocalBdAddr(), PROFILE_MCSYNC))
        {
            DBG_LOG_APP_Connection("APP_Conn_PowerOffHandler. mcsync state:%d", 1, MCSYNC_GetMcsyncState());
            AWSMCE_AgentDisconnAwsLink(BtAwsMce_ReadLocalBdAddr());
        }
        APP_Conn_SetTimer(TIMER_ID_DISCONNECT_SPECIAL_LINK, 3000);
>>>>>>> db20e11 (second commit)
	}

	if(CURRENT_ACTIVE_LINK_CNT == 2)
	{
		if(BtAwsMce_IsDefaultRoleAgent() && APP_GetAGNum() && APP_GetServiceBdAddr())
		{
			BtSetAclDetachTimeout(APP_GetServiceBdAddr()->addr, 0);
		}
		APP_Conn_ReleaseMultiLink();
		return TRUE;
	}
	else if(BtAwsMce_IsDefaultRolePartner())
	{
		BtAwsMce_DisconnectAwsLink();
	}
	return FALSE;
}

BOOL APP_Conn_IsProfilesReady(void)
{
	return (gAppConnCtl.isProfileRegistered) ? TRUE : FALSE;
}


void APP_Conn_NotifyPartnerMediaEvt(U8 oldNumOfAg, U8 numOfAG, U8 isA2dpPlaying)
{
	U8 numIndex;

	DBG_LOG_APP_Connection("APP_Conn_NotifyPartnerMediaEvt. oldNum:%d, newNum:%d, isPlaying:%d", 3, oldNumOfAg, numOfAG, isA2dpPlaying);
	for(numIndex = oldNumOfAg; numIndex < numOfAG; numIndex++)
	{
		if(numIndex == oldNumOfAg && isA2dpPlaying)
		{
			DBG_LOG_APP_Connection( "[AUDIO DSP A2DP][Partner Later Join] Partner Set Later Join log string two", 0);
			drv_a2dp_set_partner_later_join(TRUE);
		}

<<<<<<< HEAD
		APP_Media_PushMediaEvent((U16)(MEDIA_EVT_SLC_CONNECTED) + numIndex);
=======
		//APP_Media_PushMediaEvent((U16)(MEDIA_EVT_SLC_CONNECTED) + numIndex);
>>>>>>> db20e11 (second commit)
	}
}

BOOL APP_Conn_FillRhoData(void *pData)
{
	APP_RHO_CONN_STRU *pDataToFill = &((APP_RHO_DATA_STRU *)pData)->appConn;

	if(!pDataToFill)
	{
		DBG_LOG_APP_Connection( "[RHO_APP_CONN] Old Agent fill data error", 0);
		return FALSE;
	}

	pDataToFill->currentSupportDeviceNo = gAppConnCtl.currentSupportDeviceNo;
    pDataToFill->isQosTimerSet = FW_CheckTimer((Handler)&gAppConnectionTimerHandle, TIMER_ID_QOS, 0);
	return TRUE;
}

BOOL APP_Conn_AssignRhoData(void *pData)
{
	APP_RHO_CONN_STRU *pDataToGet = &((APP_RHO_DATA_STRU *)pData)->appConn;

	if(!pDataToGet)
	{
		DBG_LOG_APP_Connection( "[RHO_APP_CONN] New Agent Assign data error", 0);
		return FALSE;
	}

	gAppConnCtl.currentSupportDeviceNo = pDataToGet->currentSupportDeviceNo;
    if(pDataToGet->isQosTimerSet)
    {
        BD_ADDR_T *pBdAddr = APP_GetServiceBdAddr();
        APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);
        if(pLinkInfo)
            FW_SetTimer((Handler)&gAppConnectionTimerHandle, TIMER_ID_QOS, NULL, (U32)pLinkInfo, 5 * ONE_SEC);
    }
	return TRUE;
}

void APP_Conn_NewPartner(BD_ADDR_T *pOldAgentAddr, BD_ADDR_T *pNewAgentAddr)
{
    DBG_LOG_APP_Connection( "[RHO_APP_CONN] RHO NewPartner: oldAgent:0x%x%x, NewAgent:0x%x%x", 4, FW_bdaddr_to_2U32(pOldAgentAddr, TRUE), FW_bdaddr_to_2U32(pOldAgentAddr, FALSE), FW_bdaddr_to_2U32(pNewAgentAddr, TRUE), FW_bdaddr_to_2U32(pNewAgentAddr, FALSE));
}

U8 APP_Conn_GetMsgNumInQueue(void)
{
	return MSG_MessageSearchAllHandler((Handler)&gAppConnectionHandle);
}

void APP_Conn_AllowAllRoleConnSPInit(void)
{
	if(APP_SUPPORT_ALL_ROLE_ENTER_DISCOVER_FEAT)
	{
		BtMCSync_AllowAllRoleConnSP(TRUE);
	}
	else
	{
		BtMCSync_AllowAllRoleConnSP(FALSE);
	}
}


U8 APP_Conn_GetAgLinkState(void)
{
	BD_ADDR_T *pBdAddr;
	U8 agState = PM_Conn_GetAgLinkState();

	if(!BtAwsMce_IsDefaultRoleAgent())
		return LINK_DISCONNECTED;

	if(agState == LINK_DISCONNECTED)
	{
		pBdAddr = APP_GetServiceBdAddr();
		if(!BtAwsMce_IsSpecialLink(pBdAddr) && APP_IsActiveLink(pBdAddr))
			return LINK_DISCONNECTING;
	}

	return agState;
}

U16 APP_Conn_CheckTimer(U16 timerId)
{
	return FW_CheckTimer((Handler)&gAppConnectionTimerHandle, timerId, 0);
}

void APP_Conn_ReleaseTimer(U16 timerId)
{
	FW_ReleaseTimer((Handler)&gAppConnectionTimerHandle, timerId, 0);
}

void APP_Conn_SetTimer(U16 timerId, U32 timeout)
{
<<<<<<< HEAD
	DBG_LOG_APP_Connection("[APP_CONN] Set Timer, timer id:%d, timeout:%d", 2, timerId, timeout);
	FW_SetTimer((Handler)&gAppConnectionTimerHandle, timerId, NULL, 0, timeout);
=======
	U32 *pMsg = NULL;
    if(timerId == TIMER_ID_DISCONNECTED_VP)
    {
        if(APP_Conn_CheckTimer(TIMER_ID_DISCONNECTED_VP))
            return;
    }

	if(timerId == TIMER_ID_SLC_CONNECTED_VP)
	{
		pMsg = FW_GetMemory(sizeof(U32));
		*pMsg = timeout;
	}

	DBG_LOG_APP_Connection("[APP_CONN] Set Timer, timer id:%d, timeout:%d", 2, timerId, timeout);
	FW_SetTimer((Handler)&gAppConnectionTimerHandle, timerId, pMsg, 0, timeout);
>>>>>>> db20e11 (second commit)
}

void APP_Conn_ReleaseQosTimer(APP_INFO_STRU * pLinkInfo)
{
    FW_ReleaseTimer((Handler)&gAppConnectionTimerHandle, TIMER_ID_QOS, (U32)pLinkInfo);
}

