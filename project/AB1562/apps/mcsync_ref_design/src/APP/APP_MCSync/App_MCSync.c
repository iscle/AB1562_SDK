/* Copyright Statement:
 *
 * (C) 2018  Airoha Technology Corp. All rights reserved.
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
#include "BtMCSync.h"
#include "App_ReConnection.h"
#include "PM_Interface.h"
#include "App_MCSync_Message.h"
#include "App_VolumeManage.h"
#include "Volume_NvkeyHandler.h"
#include "App_PeqProcess.h"
#include "Peq_NvkeyHandler.h"
#include "AudioDSP_StreamManager.h"
#include "App_EventKeyHandler.h"
#include "App_State.h"
#include "App_EventOpcode.h"
#include "App_CheckKeyValid.h"
#include "App_EventKeyHandler.h"
#include "App_MCSync.h"
#include "DSP_Control.h"
#include "AudioDSP_Vp.h"
#include "App_Hfp.h"
#include "App_EventOpcode.h"
#include "key_nvkey.h"
#include "key.h"
#include "App_Connection_NVKeyHandler.h"
#include "App_MCSync_NvkeyHandler.h"
#include "App_LedControl.h"
#include "App_VpRtControl.h"
#include "Hfp_atCommand.h"
#include "App_MCSync_RHO.h"
#include "BtA2dp_Settings.h"
#include "App_PeqSync.h"
#include "A2dp_Interface.h"
#include "APP_AirApp.h"
#include "App_MCSync_System.h"
#include "App_RhoHandler.h"
#include "App_RhoDominator.h"
#include "Power_NvkeyHandler.h"
#include "App_PowerOff.h"
#include "PM_Interface.h"
#include "App_Nvkey_KeymapHandler.h"
#include "App_System.h"
#include "App_SmartChgCase.h"
#include "BtMCSync.h"
#include "DrvSmartCase.h"
#include "App_Mcsync_Reconnection.h"
#include "App_Mcsync_LinkKey.h"
#include "App_Interface.h"
#include "App_MCSync_State.h"
#include "Pairing_NvkeyHandler.h"
#include "App_ChargeBattery.h"
#include "chargercase_common.h"
#include "App_LinkKey.h"
#include "AudioDSP_AncPassThru.h"
#include "App_Fcd.h"
#include "App_PeqManage.h"
#ifdef PROFILE_AMA_ENABLE
#include "App_Ama.h"
#include "BtAma.h"
#endif
#ifdef XIAOAI_DEVELOPMENT
#include "App_XiaoaiAdv.h"
#endif
<<<<<<< HEAD
#ifdef APP_TILE_ENABLE
=======
#ifdef AIR_TILE_ADV_ENABLE
>>>>>>> db20e11 (second commit)
#include "App_Tile.h"
#endif
#include "App_VolumeSetting.h"

#include "App_InEarDetection.h"
#include "InEarDetection_common.h"
#include "InEarDetection_DrvHandler.h"
#include "App_Customer_Func.h"
#include "system.h"
#include "App_SniffHandler.h"
#include "App_Customer.h"
#include "AirApp.h"
#include "drv_anc.h"
#include "App_A2dp.h"
#include "drv_a2dp.h"
#include "Battery.h"
#include "ChargerSmartCase.h"

<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
#include "App_MCSync_Share.h"
#include "App_VolumeA2dpProcess.h"
#endif

#include "App_MCSync_Data.h"
#include "App_Ble.h"
#include "ChargerSmartCase.h"
#include "ota.h"

<<<<<<< HEAD
#ifdef PROFILE_GFP_ENABLE
#include "App_Gfp.h"
#endif

=======
#ifdef AIR_GFP_ENABLE
#include "App_Gfp.h"
#endif

#include "APP_HfpIncomingCallMediaHandler.h"

>>>>>>> db20e11 (second commit)
log_create_module(APP_MCSYNC, PRINT_LEVEL_INFO);

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static void app_AwsMce_StatusChangeIndHandler(AWSMCE_STATUS_CHANGE_IND_T* ind);
static void app_AwsMce_AgentA2dpEvtIndHandler(AWSMCE_AGENT_A2DP_EVT_IND_T* ind);
static void app_AwsMce_AgentScoEvtIndHandler(AWSMCE_AGENT_SCO_EVT_IND_T* ind);
static void app_AwsMce_ConnectCfmHandler(AWSMCE_CONNECT_CFM_T * cfm);
static void app_AwsMce_DisconnectIndHandler(AWSMCE_DISCONNECT_IND_T *ind);
static void app_AwsMce_ReconnectIndHandler(void);
static void app_AwsMce_ReconnectPowerOnIndHandler(void);
static void app_AwsMce_PicoClockCfmHandler(AWS_MCE_PICO_CLOCK_CFM_T* cfm);
static void app_AwsMce_SyncPartnerKeyCodeIndHandler(AWSMCE_SYNC_KEYCODE_IND_T *ind);

static U32 app_AwsMce_SyncAgentVpRtIndHandler(AWSMCE_SYNC_VPRT_IND_T* ind);
static void app_AwsMce_SyncAgentStopVpRtIndHandler(AWSMCE_SYNC_STOP_VPRT_IND_T *ind);
static void app_AwsMce_SyncAgentVpRtPlayLaterIndHandler(AWSMCE_SYNC_VPRT_PLAY_LATER_IND_T *ind);
static void app_AwsMce_SyncAgentCallerIdIndHandler(AWSMCE_SYNC_CALLER_ID_IND_T* ind);
static void app_AwsMce_SyncAgentPEQRealTimeIndHandler(AWSMCE_SYNC_PEQREALTIME_IND_T* ind);
static void app_AwsMce_RhoPacketIndHandler(AWSMCE_RHO_PACKET_IND_T* ind);

static void app_AwsMce_PartnerDiscHandler(void);
static BOOL app_AwsMce_IsSystemKeyNeedSync(U16 keyEventCode);
static void app_AwsMce_SyncDSPParaHandler(BD_ADDR_T* pBdAddr);
static U32 app_AwsMce_IndHandler(Handler handler, U16 id, void *msg, U32 handler_id);
static U32 app_AwsMce_TimerHandler(Handler handler, U16 id, void *msg, U32 handler_id);

static void app_AwsMce_SyncPowerOffHandler(AWSMCE_SYNC_POWEROFF_IND_T *ind);


/**************************************************************************************************
* Variable
**************************************************************************************************/
static HandlerData gAppAwsMceHandle = { app_AwsMce_IndHandler };
static HandlerData gAppMcsTimerHandle = { app_AwsMce_TimerHandler };

APP_MCSYNC_STRU gAppMCSyncCtl;

extern APP_PEQ_REALTIME_SYNC_STRU gAppPeqRealTimeSyncCtl;
extern MEDIA_EVT_CTL_STRU gMediaCtl;
<<<<<<< HEAD
#ifdef PROFILE_GFP_ENABLE
extern void GFPSv2_mcsync_accountkey(uint8_t *pData, uint16_t length);
extern uint8_t App_gfp_get_agent_charge();
extern uint8_t App_Gfps_Partner_Check_Enable();
#endif
#ifdef MCSYNC_SHARE_MODE
extern HandlerData gAppMCSyncStateTimerHandle ;
#endif
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
extern HandlerData gAppMCSyncStateTimerHandle ;
#endif
extern APP_VPRT_INFO_STRU gVpRtInfo;
extern HandlerData gAppImgCallMediaTimerHandler;
extern APP_HFP_IMG_MEDIA_STRU gAppHfpImgMediaCtl;

>>>>>>> db20e11 (second commit)
/**************************************************************************************************
* Static function
**************************************************************************************************/
static U32 app_AwsMce_TimerHandler(Handler handler, U16 id, void *msg, U32 handler_id)
{
	UNUSED(handler);
	UNUSED(msg);
	UNUSED(handler_id);

	switch(id)
	{
		case APP_MCS_TIMER_ID_CHECK_AUTO_DISCOVERABLE_WHEN_PARTNER_CONNECTED:
			if(APP_ChgBat_IsFeatureSupported(APP_POWER_OFF_WHEN_CHARGING_IN_INTR) || APP_SmtChgCse_IsConnToAGNeeded())
			{
				if(0 == APP_GetAGNum()
<<<<<<< HEAD
					#ifndef TAKE_OVER_LINK
=======
					#ifndef AIR_TAKE_OVER_LINK_ENABLE
>>>>>>> db20e11 (second commit)
					&& APP_GetACLLinkCount() < APP_Conn_GetSupportDeviceNo()
					#endif
					)
				{
					APP_Media_PushMediaEvent(MEDIA_EVT_ENTER_DISCOVERABLE);
					APP_State_DiscoverabilityHandle(CMD_SET_NORMAL_DISCOVERABLE);
				}
			}
			break;

		case APP_MCS_TIMER_AIR_PAIRING:
			if(!PM_GetNormalLinkNum() && (BtAwsMce_GetMcsyncState() != MCSYNC_LINK_CONNECTED) && APP_PowerOff_IsPowerOn())
			{
				APP_MCSYNC_StateStartPairing();
			}
			break;

		case APP_MCS_TIMER_CHECK_RHO:
			if(BtAwsMce_IsDefaultRoleAgent() && PM_IsProfileConnected(APP_GetServiceBdAddr(), PROFILE_MCSYNC) &&
				STATE_LID_OPEN == DrvCharger_GetSmartCaseState() && STATE_OUT_OF_CASE == APP_GetPartnerSmartChargeCaseState())
			{
				APP_MCSync_Rho_Start();
			}
			break;
<<<<<<< HEAD

        #ifdef PROFILE_GFP_ENABLE
        case APP_MCS_TIMER_SYNC_BATT:
            {
                U8 buf[2];
                buf[0] = Battery_GetLevelInPercent();
                buf[1] = App_gfp_get_agent_charge();
                APP_MCSYNC_SendSyncGeneralData( &buf[0], 2 );
                APP_AWSMCE_SetTimer(APP_MCS_TIMER_SYNC_BATT, 60000);
            }
            break;
        #endif
=======
>>>>>>> db20e11 (second commit)
	}

	return 0;
}

static void app_AwsMce_StatusChangeIndHandler(AWSMCE_STATUS_CHANGE_IND_T *ind)
{
<<<<<<< HEAD
	#ifdef TAKE_OVER_LINK
	MCSYNC_LINK_STRU *pHighPrioLink = BtMCSync_GetHighPrioLink();
	MCSYNC_LINK_STRU *pLowPrioLink = BtMCSync_GetHighPrioLink();
	#endif
	
#ifdef MCSYNC_SHARE_MODE
	ind->role = APP_MCSync_Share_FollowerFilter(&ind->bdAddr, ind->role);
	APP_MCSync_Share_StatusChangeHandler(&ind->bdAddr, ind->role);
#endif

	DBG_LOG_APP_MCSYNC( "[APP_MCSync]Status Change, role:0x%x", 1, ind->role);
	switch(ind->role)
	{
		case ROLE_SLAVE_AGENT_WITH_PARTNER:
			#ifdef MCSYNC_SHARE_MODE
=======
	#ifdef AIR_TAKE_OVER_LINK_ENABLE
	MCSYNC_LINK_STRU *pHighPrioLink = BtMCSync_GetHighPrioLink();
	MCSYNC_LINK_STRU *pLowPrioLink = BtMCSync_GetLowPrioLink();
	#endif

	#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
	ind->role = APP_MCSync_Share_FollowerFilter(&ind->bdAddr, ind->role);
	APP_MCSync_Share_StatusChangeHandler(&ind->bdAddr, ind->role);
	#endif

	DBG_LOG_APP_MCSYNC( "[APP_MCSync]Status Change, role:0x%x", 1, ind->role);

	switch(ind->role)
	{
		case ROLE_SLAVE_AGENT_WITH_PARTNER:
			#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
			if(APP_MCSync_Share_FollowerEventHandler(APP_SHARE_FOLLOWER_EVENT_MCSYNC_WITH_PARTNER, &ind->bdAddr, NULL))
			{
				break;
			}
<<<<<<< HEAD
            APP_MCSync_Share_AgentEventHandler(APP_SHARE_AGENT_EVENT_PARTNER_CONNECTED_CFM);
			#endif

			#if 0
			DBG_LOG_APP_MCSYNC( "[APP_MCSync]status change, check disc vp timer:%d, ag num:%d", 2,
				APP_Conn_CheckTimer(TIMER_ID_DISCONNECTED_VP), APP_GetAGNum());

			if(APP_Conn_CheckTimer(TIMER_ID_DISCONNECTED_VP) && BtAwsMce_IsSpecialLink(&ind->bdAddr) /*&&
				!APP_GetAGNum()*/) //special link
			{
				APP_Conn_ReleaseTimer(TIMER_ID_DISCONNECTED_VP);
				APP_Media_PushMediaEvent(MEDIA_EVT_SLC_DISCONNECTED);
			}
			#ifdef TAKE_OVER_LINK
			else if(APP_Conn_CheckTimer(TIMER_ID_DISCONNECTED_VP))
			{
				if((pHighPrioLink && FW_Memcmp(&pHighPrioLink->bdAddr, &ind->bdAddr, sizeof(BD_ADDR_T)))
					|| (pLowPrioLink && FW_Memcmp(&pLowPrioLink->bdAddr, &ind->bdAddr, sizeof(BD_ADDR_T))))
				{
					APP_Conn_ReleaseTimer(TIMER_ID_DISCONNECTED_VP);
				}
			}
			#endif
			#endif
=======

            //APP_MCSync_Share_AgentEventHandler(APP_SHARE_AGENT_EVENT_PARTNER_CONNECTED_CFM);
			#endif

			DBG_LOG_APP_MCSYNC( "[APP_MCSync]status change, check disc vp timer:%d, ag num:%d, ind BDA:0x%x%x", 4,
				APP_Conn_CheckTimer(TIMER_ID_DISCONNECTED_VP), APP_GetAGNum(), FW_bdaddr_to_2U32(&ind->bdAddr, TRUE),
				FW_bdaddr_to_2U32(&ind->bdAddr, FALSE));

            if(APP_Conn_CheckTimer(TIMER_ID_DISCONNECTED_VP))
            {
                if(BtAwsMce_IsSpecialLink(&ind->bdAddr))
                {
                    APP_Conn_ReleaseTimer(TIMER_ID_DISCONNECTED_VP);
                    APP_Media_PushMediaEvent(MEDIA_EVT_SLC_DISCONNECTED);
                }
                #ifdef AIR_TAKE_OVER_LINK_ENABLE
                else
                {
                    if(pHighPrioLink != NULL)
                    {
                        DBG_LOG_APP_MCSYNC( "[APP_MCSync][Take Over] high prio link:0x%x%x", 2,
                            FW_bdaddr_to_2U32(&pHighPrioLink->bdAddr, TRUE), FW_bdaddr_to_2U32(&pHighPrioLink->bdAddr, FALSE));
                    }

                    if(pLowPrioLink != NULL)
                    {
                        DBG_LOG_APP_MCSYNC( "[APP_MCSync][Take Over] low prio link:0x%x%x", 2,
                            FW_bdaddr_to_2U32(&pLowPrioLink->bdAddr, TRUE), FW_bdaddr_to_2U32(&pLowPrioLink->bdAddr, FALSE));
                    }

                    if((pHighPrioLink && FW_Memcmp(&pHighPrioLink->bdAddr, &ind->bdAddr, sizeof(BD_ADDR_T)))
                        || (pLowPrioLink && FW_Memcmp(&pLowPrioLink->bdAddr, &ind->bdAddr, sizeof(BD_ADDR_T)))
                        || gVpRtInfo.isVpRtSecondDevice)
                    {
                        APP_Conn_ReleaseTimer(TIMER_ID_DISCONNECTED_VP);
                        gVpRtInfo.isVpRtSecondDevice = FALSE;
                    }
                }
                #endif
            }
>>>>>>> db20e11 (second commit)

			if(APP_SmtChgCse_MCSyncConnCfmCheck())
			{
				app_AwsMce_SendSyncAllInfo(&ind->bdAddr);
				APP_MCSYNC_SendSyncNumOfAGInfo(APP_GetAGNum());
				APP_MCSYNC_SendSyncA2dpInfo(&ind->bdAddr, MCSYNC_SHARE_SPECIFIC_ROLE_NOT_USED);
				app_AwsMce_SyncDSPParaHandler(&ind->bdAddr);
				drv_a2dp_update_audio_channel(AUDIO_CH_MONO);
				APP_Mcsync_ClearNotReconnMask(MCS_RECONN_MASK_PARTNER_DISC);
				APP_Mcsync_ReconnectNotify(MCS_RECONN_DEVICE_PARTNER);
<<<<<<< HEAD
				APP_AWSMCE_SetTimer(APP_MCS_TIMER_ID_CHECK_AUTO_DISCOVERABLE_WHEN_PARTNER_CONNECTED, 10);
				#ifdef SMART_CHG_CSE_AUTO_AIR_PAIRING
				APP_AWSMCE_ReleaseTimer(APP_MCS_TIMER_AIR_PAIRING);
				#endif
=======
#ifdef AIR_GFP_ENABLE
                /* When GFP is enabled, do not enter pairing mode when partner is connected and SP is not. Otherwise,
                            * gfp end to end test will fail when test the subsequence pairing because it enters discoverable mode
                            * after unpair the initial pairng and subsequence pairing should be tested under non discoverable mode.
                            */
                if (!gfp_is_enabled())
#endif
                {
                    APP_AWSMCE_SetTimer(APP_MCS_TIMER_ID_CHECK_AUTO_DISCOVERABLE_WHEN_PARTNER_CONNECTED, 10);
                }

				#ifdef SMART_CHG_CSE_AUTO_AIR_PAIRING
				APP_AWSMCE_ReleaseTimer(APP_MCS_TIMER_AIR_PAIRING);
				#endif

				#ifdef AIR_TAKE_OVER_LINK_ENABLE

				if(BtAwsMce_IsDefaultRoleAgent() &&
					BtAwsMce_IsSpecialLink(&ind->bdAddr) &&
					APP_GetAGNum() == 0 &&
					BtMCSync_GetMcsyncTakeOverConnState() == MCSYNC_TAKE_OVER_CONNECTION_IDLE_STATE)
				{
					APP_MCSYNC_SendSyncTakeOverStateInfo();
				}
				#endif
>>>>>>> db20e11 (second commit)
			}
			#ifdef XIAOAI_DEVELOPMENT
			APP_XiaoaiResetAdv();
			#endif
<<<<<<< HEAD
=======
            #ifdef AIR_GFP_ENABLE
            APP_Gfp_AwsConnectedHandle();
            #endif
>>>>>>> db20e11 (second commit)
			break;

		case ROLE_AGENT:

<<<<<<< HEAD
			#ifdef TAKE_OVER_LINK
=======
			#ifdef AIR_TAKE_OVER_LINK_ENABLE
>>>>>>> db20e11 (second commit)

			if(PM_GetConnectedProfileCount((BD_ADDR_T *)&ind->bdAddr, TRUE))
			{
				if(BtMCSync_GetMcsyncTakeOverConnState() == MCSYNC_TAKE_OVER_CONNECTION_DISCONNECTED_LOW_PRIO_LINK_AWS_STATE)
				{
					APP_Conn_ReleaseProfileLinkAndDetach((BD_ADDR_T *)&ind->bdAddr);
<<<<<<< HEAD
=======

					if(FW_CmpBdAddr(&gAppHfpImgMediaCtl.bdAddr, (BD_ADDR_T *)&ind->bdAddr) && FW_CheckTimer((Handler)&gAppImgCallMediaTimerHandler, 0, 0))
					{
						FW_ReleaseTimer((Handler)&gAppImgCallMediaTimerHandler, 0, 0);
					}

>>>>>>> db20e11 (second commit)
					BtMCSync_SetMcsyncTakeOverConnState(MCSYNC_TAKE_OVER_CONNECTION_DISCONNECTING_LOW_PRIO_LINK_PROFILE_STATE);
				}
			}
			else
			{
				if(BtAwsMce_IsDefaultRoleAgent() &&
					!BtAwsMce_IsSpecialLink((BD_ADDR_T *)&ind->bdAddr) &&
					BtMCSync_CheckIsLowPrioLink((BD_ADDR_T *)&ind->bdAddr) &&
					BtMCSync_GetMcsyncTakeOverConnState() == MCSYNC_TAKE_OVER_CONNECTION_DISCONNECTED_LOW_PRIO_LINK_AWS_STATE)
				{
					APP_System_DetachAclLink(&ind->bdAddr);
					BtMCSync_SetMcsyncTakeOverConnState(MCSYNC_TAKE_OVER_CONNECTION_DISCONNECTING_LOW_PRIO_LINK_ACL_STATE);
				}
			}
			#endif

			drv_a2dp_update_audio_channel(AUDIO_CH_STEREO);
			APP_SetPartnerSmartChargeCaseState(STATE_CASE_OFF);
			APP_InEarDetection_SetPartnerStatus(IN_EAR_DETECTION_OUT);
			app_AwsMce_PartnerDiscHandler();
			APP_Customer_SetCloseAdvTimer();
			APP_Customer_McsynDisconnect();
			break;
<<<<<<< HEAD
		
		case ROLE_SLAVE_CONNECTABLE_AGENT:
			
			#ifdef TAKE_OVER_LINK
			DBG_LOG_APP_MCSYNC( "[APP_MCSync][Take Over] take over state:%d", 1, BtMCSync_GetMcsyncTakeOverConnState());
=======

		case ROLE_SLAVE_CONNECTABLE_AGENT:

			#ifdef AIR_TAKE_OVER_LINK_ENABLE
			DBG_LOG_APP_MCSYNC( "[APP_MCSync][Take Over] take over state:%d, ag num:%d", 2,
				BtMCSync_GetMcsyncTakeOverConnState(), APP_GetAGNum());

>>>>>>> db20e11 (second commit)
			if(BtMCSync_GetMcsyncTakeOverConnState() == MCSYNC_TAKE_OVER_CONNECTION_ADD_HIGH_PRIO_LINK_TO_SERVICE_STATE)
			{
				if(pHighPrioLink)
				{
					APP_SetAppLinkBdAddr(&pHighPrioLink->bdAddr);
					APP_AddServiceBdAddr(&pHighPrioLink->bdAddr);
<<<<<<< HEAD
				}
				BtMCSync_SetMcsyncTakeOverConnState(MCSYNC_TAKE_OVER_CONNECTION_IDLE_STATE);
=======

					if(APP_CheckRedialBdaddrisZero())
					{
						APP_SetRedialBdAddr(&pHighPrioLink->bdAddr);
					}
				}

				BtMCSync_SetMcsyncTakeOverConnState(MCSYNC_TAKE_OVER_CONNECTION_TAKE_OVER_COMPLETE_STATE);

				if(pHighPrioLink && APP_GetAGNum() > 0)
				{
					APP_Conn_SetTimer(TIMER_ID_TAKE_OVER_SLC_CONNECTED, 3000);	//OPPOX20E1-178_0120
				}
				else
				{
					BtMCSync_SetMcsyncTakeOverConnState(MCSYNC_TAKE_OVER_CONNECTION_IDLE_STATE);
				}

>>>>>>> db20e11 (second commit)
			}
			#endif
			break;
	}

	APP_MCSync_Rho_StatusChangeIndHandler(ind->role);
}

static void app_AwsMce_AgentA2dpEvtIndHandler(AWSMCE_AGENT_A2DP_EVT_IND_T *ind)
{
	AUDIO_DSP_START_PARA_STUR audioDspStartPara;
	AUDIO_DSP_STOP_PARA_STUR audioDspStopPara;
	U8 codecType = ind->a2dpInfo.a2dpCodecConfig.codecType;
	U8 enableSCMS = ind->a2dpInfo.airohaA2dpConfig.enableSCMS;
	U8 volume = ind->a2dpInfo.volume;
	U8 sampleRateIndex;
	U8 absVolume =  APP_VolManage_GetSoundLevelByAbsoluteVolume(volume);
	U8 peqIndex = ind->a2dpInfo.airohaA2dpConfig.peqIndex;
	BD_ADDR_T *pBdAddr = &ind->bdAddr;

    DBG_LOG_APP_MCSYNC( "[APP_MCSync][Partner] : Sync A2DP, absVol(%d), sound(%d), peqIndex(%d), codec(%d), isGameMode(%d), numofAg(%d)", 6, volume, absVolume,
																		 peqIndex, codecType, ind->a2dpInfo.airohaA2dpConfig.isGameMode,
																		 ind->a2dpInfo.airohaA2dpConfig.numOfAg);
    DBG_LOG_APP_MCSYNC( "[APP_MCSync][Partner] : Sync A2DP, state(%d), streaming(%d), Addr(0x%x%x)", 4, ind->a2dpInfo.state, APP_AudioDspIsStreaming(&ind->bdAddr, AUDIO_A2DP),
																		 FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE));

<<<<<<< HEAD
=======
    if(ind->a2dpInfo.airohaA2dpConfig.isGameMode & 0x04) //Check ap mode flag before check gamemode
    {
        drv_a2dp_set_ap_mode(TRUE);
    }
    else
    {
    	drv_a2dp_set_ap_mode(FALSE);
    }

>>>>>>> db20e11 (second commit)
	if(ind->a2dpInfo.airohaA2dpConfig.isGameMode & 0x01)
	{
		APP_A2dp_GameModeOn();
	}
	else
	{
		APP_A2dp_GameModeOff();
	}
#ifdef A2DP_LOW_LATENCY
    if(ind->a2dpInfo.airohaA2dpConfig.isGameMode & 0x02)
    {
        drv_a2dp_set_low_latency_mode(pBdAddr, TRUE);
    }
    else
    {
        drv_a2dp_set_low_latency_mode(pBdAddr, FALSE);
    }
#endif
<<<<<<< HEAD
=======

>>>>>>> db20e11 (second commit)
	if(codecType == SBC_SNK_SEID)
	{
        DBG_LOG_APP_MCSYNC( "[APP_MCSync][Partner] : Sync A2DP, SBC sampleRate:%x", 1, ind->a2dpInfo.a2dpCodecConfig.codecSetting.sbc.content1);
	}
	else if(codecType == AAC_SNK_SEID)
	{
        DBG_LOG_APP_MCSYNC( "[APP_MCSync][Partner] : Sync A2DP, AAC sampleRate1:%x, sampleRate2:%x", 2, ind->a2dpInfo.a2dpCodecConfig.codecSetting.aac.sampleRate1,
			ind->a2dpInfo.a2dpCodecConfig.codecSetting.aac.sampleRate2);
	}

	if(ind->a2dpInfo.state == MCSYNC_A2DP_STATE_PLAYING)
	{
		if(codecType == SBC_SNK_SEID)
		{
			bt_a2dp_update_client_codec_info(SBC_SNK_SEID, ind->a2dpInfo.a2dpCodecConfig.codecSetting.sbc.content1);
		}
		else if(codecType == AAC_SNK_SEID)
		{
			bt_a2dp_update_client_codec_info(AAC_SNK_SEID, (ind->a2dpInfo.a2dpCodecConfig.codecSetting.aac.sampleRate1 << 8) +
				ind->a2dpInfo.a2dpCodecConfig.codecSetting.aac.sampleRate2);
		}

		if(!APP_AudioDspIsStreaming(pBdAddr, AUDIO_A2DP))
		{
			sampleRateIndex = bt_a2dp_get_sample_rate_index(&ind->bdAddr, codecType);
			APP_VolManage_SetCurrentA2dpSoundLevel(pBdAddr, absVolume);
			APP_VolManage_SetCurrentAbsoluteVolume(pBdAddr, volume);

            DBG_LOG_APP_MCSYNC( "[APP_MCSync][Partner] : Sync A2DP, sampleRate Index :%d", 1, sampleRateIndex);

			FW_Memset(&audioDspStartPara, 0, sizeof(AUDIO_DSP_START_PARA_STUR));
			FW_Memcpy(&audioDspStartPara.bdAddr, &ind->bdAddr, sizeof(BD_ADDR_T));
			audioDspStartPara.audioType = AUDIO_A2DP;
			audioDspStartPara.codec = codecType;
			audioDspStartPara.a2dpPara.scms = enableSCMS;
			audioDspStartPara.a2dpPara.sampleRateIndex = sampleRateIndex;
			if(APP_AudioDspPlay(&audioDspStartPara))
			{
<<<<<<< HEAD
#ifdef APP_TILE_ENABLE
=======
#ifdef AIR_TILE_ADV_ENABLE
>>>>>>> db20e11 (second commit)
                APP_Tile_NotifyPartnerA2dpState(TRUE);
#endif
			}
		}
		else
		{
			APP_VolManage_SpkVolumeRemoteHandler(pBdAddr, absVolume, COMPONENT_A2DP);
			APP_PeqManage_SetPeqGroup(PEQ_A2DP, peqIndex);
		}
	}
	else if(APP_AudioDspIsStreaming(pBdAddr, AUDIO_A2DP))
	{
		FW_Memset(&audioDspStopPara, 0, sizeof(AUDIO_DSP_STOP_PARA_STUR));
		FW_Memcpy(&audioDspStopPara.bdAddr, pBdAddr, sizeof(BD_ADDR_T));
		audioDspStopPara.audioType = AUDIO_A2DP;
		APP_AudioDspStop(&audioDspStopPara);
<<<<<<< HEAD
#ifdef APP_TILE_ENABLE
=======
#ifdef AIR_TILE_ADV_ENABLE
>>>>>>> db20e11 (second commit)
        APP_Tile_NotifyPartnerA2dpState(FALSE);
#endif
	}
}

static void app_AwsMce_AgentScoEvtIndHandler(AWSMCE_AGENT_SCO_EVT_IND_T *ind)
{
	AUDIO_DSP_START_PARA_STUR audioDspPara;
	APP_INFO_STRU * pLinkInfo = APP_GetAppLinkByBdAddr(&ind->bdAddr);

    DBG_LOG_APP_MCSYNC( "[APP_MCSync][Partner] : Sync SCO, speakSound(%d), micSound(%d), scoState(%d), topState(%d)", 4, ind->scoInfo.speakerSoundLevel, ind->scoInfo.micSoundLevel, ind->scoInfo.scoState, ind->scoInfo.topState);

	APP_VolManage_SetCurrentScoSoundLevel(&ind->bdAddr, ind->scoInfo.speakerSoundLevel);
	APP_VolManage_SetCurrentScoMicSoundLevel(&ind->bdAddr, ind->scoInfo.micSoundLevel);

	APP_SetAgentScoTopState(ind->scoInfo.topState);

	if(ind->scoInfo.scoState == SCO_STATE_CONNECTED)
	{
		if(APP_GetAgentScoState() != SCO_STATE_CONNECTED && APP_GetScoState(pLinkInfo) == SCO_STATE_CONNECTED)
		{
            DBG_LOG_APP_MCSYNC( "[APP_MCSync][Partner] SCO evt ind A2dp stream handler Not exist", 0);
            FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_START_PARA_STUR));
            FW_Memcpy(&audioDspPara.bdAddr, &ind->bdAddr, sizeof(BD_ADDR_T));
            audioDspPara.audioType = AUDIO_SCO;
            audioDspPara.codec = APP_GetScoCodec(&ind->bdAddr);
            if (APP_AudioDspPlay(&audioDspPara))
            {
<<<<<<< HEAD
#ifdef APP_TILE_ENABLE
=======
#ifdef AIR_TILE_ADV_ENABLE
>>>>>>> db20e11 (second commit)
                APP_Tile_NotifyPartnerCallState(TRUE);
#endif
            }
		}
		else
		{
			APP_VolManage_SpkVolumeRemoteHandler(&ind->bdAddr, ind->scoInfo.speakerSoundLevel, APP_Hfp_IsCodecWideBand(APP_GetScoCodec(&ind->bdAddr)) ? COMPONENT_SCO_WB : COMPONENT_SCO_NB);
		}
	}
    else if (ind->scoInfo.scoState == SCO_STATE_DISC)
    {
<<<<<<< HEAD
#ifdef APP_TILE_ENABLE
=======
#ifdef AIR_TILE_ADV_ENABLE
>>>>>>> db20e11 (second commit)
        APP_Tile_NotifyPartnerCallState(FALSE);
#endif
    }

	APP_SetAgentScoState(ind->scoInfo.scoState);
}

static void app_AwsMce_ConnectCfmHandler(AWSMCE_CONNECT_CFM_T *cfm)
{
	if(cfm->status != BT_STATUS_BUSY)
		APP_ReConn_DeleteProfileMask(&cfm->bdAddr, PROFILE_MCSYNC, cfm->status);

<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
	if(APP_MCSync_Share_FollowerEventHandler(APP_SHARE_FOLLOWER_EVENT_MCSYNC_CONNECT_CFM, &cfm->bdAddr, (U8 *)cfm))
	{
		return;
	}
#endif
	if(cfm->status == BT_STATUS_SUCCESS)
	{
		if(BtAwsMce_IsDefaultRolePartner())
		{
<<<<<<< HEAD
			#ifdef XIAOAI_DEVELOPMENT
			BtMCSync_SendSyncBatteryLevel(Battery_GetLevelInPercent());
			APP_MCSYNC_SendSyncSmartChargeCaseStateInfo(DrvCharger_GetSmartCaseState());
			#endif
			if(APP_SmtChgCse_MCSyncConnCfmCheck())
			{
				APP_MCSYNC_SendSyncBdAddrInfo();
	            #ifdef PROFILE_GFP_ENABLE
	            if ( App_Gfps_Partner_Check_Enable() )
	            {
	        		APP_AWSMCE_SetTimer(APP_MCS_TIMER_SYNC_BATT, 100);
	            }
	            #endif
=======
			#if defined(XIAOAI_DEVELOPMENT) || defined(AIR_GFP_ENABLE)
			BtMCSync_SendSyncBatteryLevel(Battery_GetLevelInPercent());
			APP_MCSYNC_SendSyncSmartChargeCaseStateInfo(DrvCharger_GetSmartCaseState());
			#endif

            if(APP_Conn_CheckTimer(TIMER_ID_DISCONNECTED_VP))
            {
                APP_Conn_ReleaseTimer(TIMER_ID_DISCONNECTED_VP);
                APP_Conn_SetTimer(TIMER_ID_DISCONNECTED_VP, 3500);
            }

			if(APP_SmtChgCse_MCSyncConnCfmCheck())
			{
				APP_MCSYNC_SendSyncBdAddrInfo();
>>>>>>> db20e11 (second commit)

				APP_Customer_SendSyncGeneralData();
				APP_MCSYNC_SendSyncSmartChargeCaseStateInfo(DrvCharger_GetSmartCaseState());
				drv_a2dp_update_audio_channel(AUDIO_CH_MONO);
				APP_Mcsync_ReconnectNotify(MCS_RECONN_DEVICE_AGENT);
				#ifdef SMART_CHG_CSE_AUTO_AIR_PAIRING
				APP_AWSMCE_ReleaseTimer(APP_MCS_TIMER_AIR_PAIRING);
				#endif
				if(APP_InEarDetection_IsEnable())
				{
					APP_MCSYNC_SendSyncPartnerInEarDetectionStatus(APP_InEarDetection_GetStatus());
				}
			}

			//BtInquiryCancel(&gAppMCSyncSystemHandle);
		}
		else if(BtAwsMce_IsDefaultRoleAgent())
		{
			if(!FW_CmpBdAddr(&cfm->bdAddr, BtAwsMce_ReadLocalBdAddr())) //not special link
			{
				APP_Mcsync_ReconnectNotify(MCS_RECONN_DEVICE_AG);
				APP_AWSMCE_SetTimer(APP_MCS_TIMER_CHECK_RHO, 4000);
			}
			else
			{
<<<<<<< HEAD
				APP_ReConn_PourQueue();
=======
				APP_ReConn_PourQueue(0);
>>>>>>> db20e11 (second commit)
			}
		}
	}
}

static void app_AwsMce_DisconnectIndHandler(AWSMCE_DISCONNECT_IND_T *ind)
{
	UNUSED(ind);

<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
	if(APP_MCSync_Share_FollowerEventHandler(APP_SHARE_FOLLOWER_EVENT_MCSYNC_DISCONNECT_IND, &ind->bdAddr, NULL))
	{
		return;
	}
#endif
	if(BtAwsMce_IsDefaultRolePartner())
	{
<<<<<<< HEAD
		if(APP_GetAGNum() > 0)
		{
			APP_SetAGNum(APP_GetAGNum()-1);
=======
        DBG_LOG_APP_MCSYNC( "[APP_MCSync][Partner] DisconnectInd, AGNum:%d", 1, APP_GetAGNum());
		if(APP_GetAGNum() > 0)
		{
            APP_SetAGNum(APP_GetAGNum()-1);
            APP_Conn_SetTimer(TIMER_ID_DISCONNECTED_VP, 3500);
>>>>>>> db20e11 (second commit)
		}
		else
		{
			APP_SetAGNum(0);
		}

		APP_LED_ReleaseSyncTimer();
		APP_State_LoadDefaultTopState();
		if(!AirApp_IsFOTASuccess())
        {
			APP_ReleaseDspSuspend();
            ota_abort();
        }

		drv_a2dp_update_audio_channel(AUDIO_CH_STEREO);
<<<<<<< HEAD
		#ifdef MCSYNC_SHARE_MODE
=======
		#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
		APP_MCSync_Share_PartnerEventHandler(APP_SHARE_PARTNER_EVENT_MCSYNC_DISCONNECT_IND, NULL);
		#endif
	}

	APP_Customer_Captouch_MCSYNCDisconnect();
}

static void app_AwsMce_ReconnectIndHandler(void)
{
	APP_ReConn_Reconnect(RECONNECT_AWSMCE);
}

static void app_AwsMce_ReconnectPowerOnIndHandler(void)
{
	if(APP_AUTO_RECONN_AFTER_PWR_ON_FEAT && APP_SmtChgCse_IsOutOfChargerCase())
	{
		APP_ReConn_Reconnect(RECONNECT_PWR_ON);
	}
}

static void app_AwsMce_PicoClockCfmHandler(AWS_MCE_PICO_CLOCK_CFM_T * cfm)
{
	if(cfm->status == BT_STATUS_SUCCESS)
	{
		switch(cfm->event)
		{
			case PICO_EVENT_LED:
				APP_LED_SetSyncTimer(cfm->clock);
				break;

			case PICO_EVENT_PEQ:
				APP_Peq_SetSyncTimer(cfm->clock);
				break;

			default:
				break;
		}
	}
}

static void app_AwsMce_SyncPartnerKeyCodeIndHandler(AWSMCE_SYNC_KEYCODE_IND_T *ind)
{
	APP_Media_PushMediaEvent(ind->keyCode);
	APP_EvtKey_KeyEventHandler(APP_GetServiceBdAddr(), ind->keyCode);
}

<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
APP_MCSYNC_VPRT_LOG_RECORD_STRU gVpRtLog;
#endif
static U32 app_AwsMce_SyncAgentVpRtIndHandler(AWSMCE_SYNC_VPRT_IND_T *ind)
{
<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
	U8 shareMode = BtMCSync_GetShareMode();

	if(gVpRtLog.picoClock != ind->currentPicoClock
		|| (gVpRtLog.index % 50 == 0))
	{
        DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] SyncAgentVpRtIndHandler: eventIndex: 0x%x, currentPicoClock:%d, shareMode:%d", 3, ind->eventIndex, ind->currentPicoClock, shareMode);
        DBG_LOG_APP_MCSYNC("[APP_MCSync][Share] SyncAgentVpRtIndHandler: rtIndex: 0x%x, vpIndex:0x%x, overwriteTime:%d", 3, ind->rtIndex, ind->vpIndex, ind->overwriteTime);
		gVpRtLog.picoClock = ind->currentPicoClock;
		gVpRtLog.index = 1;
	}
	else
	{
		gVpRtLog.index++;
	}

	if(shareMode == MCSYNC_SHARE_MODE_FOLLOWER_ENABLE)
	{
		printf("[APP_MCSync] Followe role, not to do VPRT sync");
		return 0;
	}
#else
    DBG_LOG_APP_MCSYNC( "[APP_MCSync][Partner] : Sync VPRT, eventIndex(0x%x), currentPicoClock(%d)", 2, ind->eventIndex, ind->currentPicoClock);
    DBG_LOG_APP_MCSYNC( "[APP_MCSync][Partner] : Sync VPRT, rtIndex(0x%x), vpIndex(0x%x), overwriteTime(%d)", 3, ind->rtIndex, ind->vpIndex, ind->overwriteTime);
#endif

	if(gMediaCtl.mediaState != MEDIA_STATE_END)
	{
		U8 *pData = FW_GetMemory(sizeof(AWSMCE_SYNC_VPRT_IND_T));

		APP_VpRt_StopSystemVpUnderHFPState(ind->eventIndex);
		APP_VpRt_StopHFPVpUnderHFPState(ind->eventIndex);
		if(!pData)
		{
			return 2;
		}
		else
		{
<<<<<<< HEAD
			#ifdef MCSYNC_SHARE_MODE
=======
			#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
			gVpRtLog.picoClock = 0;
			gVpRtLog.index = 0;
			#endif
			FW_Memcpy(pData, (U8*)ind, sizeof(AWSMCE_SYNC_VPRT_IND_T));
			FW_SetTimer(&gAppAwsMceHandle, AWSMCE_MSG_SLAVE_VPRT_DELAY_IND, pData, 0, 10);
			return 0;
		}
	}
	else
	{
<<<<<<< HEAD
		if(ind->rtIndex != 0xFF || ind->vpIndex != 0xFF || ind->eventIndex == MEDIA_EVT_PLAYING_BEEP_SYNC)
			APP_VpRt_SyncTimerSlaveSetting(ind->rtIndex, ind->vpIndex, ind->eventIndex, ind->overwriteTime, ind->currentPicoClock);

		#ifdef MCSYNC_SHARE_MODE
=======
		if(ind->eventIndex == MEDIA_EVT_SLC_DISCONNECTED)
		{
			DBG_LOG_APP_MCSYNC("[APP_MCSync][Partner] DISCONNECTED VP Exist:%d", 1, APP_Conn_CheckTimer(TIMER_ID_DISCONNECTED_VP));

            APP_Conn_ReleaseTimer(TIMER_ID_SLC_CONNECTED_VP);
			if(APP_Conn_CheckTimer(TIMER_ID_DISCONNECTED_VP))
			{
				APP_Conn_ReleaseTimer(TIMER_ID_DISCONNECTED_VP);
			}
			else
			{
				//return 0;
			}
		}
		else if(ind->eventIndex == MEDIA_EVT_SLC_CONNECTED)
		{

			DBG_LOG_APP_MCSYNC("[APP_MCSync][Partner] set SLC VP 5s", 0);
			APP_Conn_ReleaseTimer(TIMER_ID_SLC_CONNECTED_VP);
			APP_Conn_SetTimer(TIMER_ID_SLC_CONNECTED_VP, TIMER_PARTNER_VP_IF_SLC_TIMER);
		}

		if(ind->rtIndex != 0xFF || ind->vpIndex != 0xFF || ind->eventIndex == MEDIA_EVT_PLAYING_BEEP_SYNC)
			APP_VpRt_SyncTimerSlaveSetting(ind->rtIndex, ind->vpIndex, ind->eventIndex, ind->overwriteTime, ind->currentPicoClock);

		#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
		gVpRtLog.picoClock = 0;
		gVpRtLog.index = 0;
		#endif
		return 0;
	}
}

static void app_AwsMce_SyncAgentStopVpRtIndHandler(AWSMCE_SYNC_STOP_VPRT_IND_T *ind)
{
	APP_VpRt_SetSyncStopTimerSlave(BtAwsMce_GetCurrentBtClock(), ind->currentPicoClock, ind->eventIndex);
}

static void app_AwsMce_SyncAgentVpRtPlayLaterIndHandler(AWSMCE_SYNC_VPRT_PLAY_LATER_IND_T *ind)
{
    DBG_LOG_APP_MCSYNC( "[APP_MCSync] : Sync VPRT Play Later Ind. eventIndex(%d), clockToStart(%d)", 2, ind->eventIndex, ind->clockToStart);
	APP_VpRt_SyncPlayLaterHandler(ind->eventIndex, ind->clockToStart);
}

static void app_AwsMce_SyncAgentCallerIdIndHandler(AWSMCE_SYNC_CALLER_ID_IND_T* ind)
{
	drv_vp_set_caller_id((U8*)&ind->callerIdData[0], ind->callerIdLength);
}

static void app_AwsMce_SyncAgentPEQRealTimeIndHandler(AWSMCE_SYNC_PEQREALTIME_IND_T* ind)
{
	APP_Peq_SyncAgentParameter(ind);
	APP_Peq_SetSyncTimer(BtAwsMce_GetCurrentBtClock());
}

static void app_AwsMce_RhoPacketIndHandler(AWSMCE_RHO_PACKET_IND_T *ind)
{
	if(!APP_PowerOff_IsPowerOn())
	{
<<<<<<< HEAD
=======
		DBG_LOG_APP_MCSYNC("[APP_MCSYNC] RhoPacketIndHandler Poweroff", 0);
>>>>>>> db20e11 (second commit)
		return;
	}

	MCSYNC_RHO_PACKET_T *pRxPacket = (MCSYNC_RHO_PACKET_T *)FW_GetMemory(ind->length);

	FW_Memcpy(pRxPacket, &ind->rhoRxPkt, ind->length);
	APP_MCSync_Rho_PacketHandler(pRxPacket, ind->length);
}

static void app_AwsMce_SyncGeneralDataIndHandler(AWSMCE_SYNC_GENERAL_DATA_IND_T* ind)
{
	//for AE use
	APP_Customer_Sync_Parnter_Data( &ind->data[0] );
	APP_Customer_SyncHandler(ind);
}

static void app_AwsMce_PartnerDiscHandler(void)
{
	U8 connState = PM_GetAgLinkState();

<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
	U8 shareMode = BtMCSync_GetShareMode();
	BOOL isPowerOn = APP_PowerOff_IsPowerOn();

	printf("[APP_MCSYNC] Partner Disc Handler. connState:%d, isPwrOn:%d, shareMode:%d", connState, isPowerOn, shareMode);

	if(!isPowerOn || shareMode != MCSYNC_SHARE_MODE_DISABLE)
	{
		return;
	}
#else
	if(!APP_PowerOff_IsPowerOn())
		return;
#endif

	//                               check no AG's ACL, only SAWS exist
	if(connState != LINK_CONNECTED && CURRENT_ACTIVE_LINK_CNT == 1)
	{
		if(connState != LINK_CONNECTING && APP_SmtChgCse_IsOutOfChargerCase())
		{
			APP_Mcsync_SetNotReconnMask(MCS_RECONN_MASK_PARTNER_DISC);
		}
		else
		{
			APP_ReConn_ClearCtl();
		}

		APP_ReConn_Reconnect(RECONNECT_PWR_ON);
	}
}

static BOOL app_AwsMce_IsSystemKeyNeedSync(U16 keyEventCode)
{
	BD_ADDR_T *pServiceBdAddr = (BD_ADDR_T *)APP_GetServiceBdAddr();

	switch(keyEventCode)
	{
		case KEY_MIC_MUTE_TOGGLE:
		case KEY_MIC_MUTE_ON:
		case KEY_MIC_MUTE_OFF:
		case KEY_RECONNECT_USER_INIT:
		case KEY_CONNECTED_TO_DISCOVERABLE:
		case KEY_VOICEUP:
		case KEY_VOICEDN:
		case KEY_AIROTHRU:
		case KEY_PEQ_GROUP_CHANGE:
		case KEY_GAME_MODE_ON:
		case KEY_GAME_MODE_OFF:
		case KEY_GAME_MODE_TOGGLE:
		case KEY_MEDIA_TRIGGER_1:
		case KEY_MEDIA_TRIGGER_2:
		case KEY_RESET_PAIRED_DEVICES:
		case KEY_ANC_SWITCH:
		case KEY_TOGGLE_LED:
		case KEY_VOICE_PROMPT_LANG_CHANGE:
			if(PM_IsProfileConnected(pServiceBdAddr, PROFILE_MCSYNC))
			{
				return TRUE;
			}
			break;

        case KEY_DISCOVERABLE:
        	return TRUE;

<<<<<<< HEAD
		#ifdef MCSYNC_SHARE_MODE
		case KEY_SHARE_PAIRING_START:
		case KEY_SHARE_PAIRING_STOP:
		case KEY_AGENT_STARTS_SHARE_MODE:
		case KEY_AGENT_STOPS_SHARE_MODE:
		case KEY_FOLLOWER_STARTS_SHARE_MODE:
		case KEY_AGENT_TOGGLE_SHARE_MODE:
		case KEY_FOLLOWER_TOGGLE_SHARE_MODE:
        case KEY_FOLLOWER_STOPS_SHARE_MODE:
=======
		#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
		case KEY_SHARE_PAIRING_START:
		case KEY_SHARE_PAIRING_STOP:
		case KEY_MASTER_SHARE_MODE_START:
		case KEY_MASTER_SHARE_MODE_STOP:
		case KEY_FOLLOWER_SHARE_MODE_START:
		case KEY_FOLLOWER_SHARE_MODE_STOP:
		case KEY_MASTER_SHARE_MODE_TOGGLE:
		case KEY_FOLLOWER_SHARE_MODE_TOGGLE:
>>>>>>> db20e11 (second commit)
            return APP_MCSync_Share_IsKeyNeedSync(keyEventCode);
		#endif

		default:
			return FALSE;
	}

	return FALSE;
}

static void app_AwsMce_SyncDSPParaHandler(BD_ADDR_T* pBdAddr)
{
	if(APP_AudioDspIsStreaming(pBdAddr, AUDIO_SCO))
	{
		MCE_TrigParaSync();
	}
}

static void app_AwsMce_RhoRoleChangeIndHandler(MCSYNC_RHO_ROLE_CHANGE_IND_T *ind)
{
	if(ind->status == HCI_STATUS_SUCCESS)
		APP_MCSync_Rho_RoleChangeIndHandler(ind->role, TRUE);
	else
		APP_MCSync_Rho_RoleChangeIndHandler(ind->role, FALSE);
}

static void app_AwsMce_CancelConnCfmHandler(MCSYNC_CANCEL_CONN_CFM_T *cfm)
{
	UNUSED(cfm);
}

static void app_AwsMce_AwsIdleTimerCtlIndHandler(AWSMCE_AWS_IDLE_TIMER_CTL_IND_T *ind)
{
    if(BtAwsMce_IsDefaultRoleAgent())
    {
        DBG_LOG_APP_MCSYNC( "[APP_MCSync][Agent] AWS Idle Timer Ctl (%d)", 1, ind->cmd);

        if(ind->cmd == MCSYNC_AWS_IDLE_TIMER_SET
<<<<<<< HEAD
		#ifdef MCSYNC_SHARE_MODE
=======
		#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
			&& MCSYNC_SHARE_MODE_FOLLOWER_ENABLE != BtMCSync_GetShareMode()
		#endif
		)
        {
            APP_Conn_ReleaseTimer(TIMER_ID_POWER_OFF_IN_ROLE_CHANNGING);
            APP_PowerOff_SetTimer(TIMER_AWS_IDLE_POWEROFF);
        }
        else if(ind->cmd == MCSYNC_AWS_IDLE_TIMER_CANCEL)
        {
        	APP_Conn_SetTimer(TIMER_ID_POWER_OFF_IN_ROLE_CHANNGING, TIMER_ID_POWER_OFF_IN_ROLE_CHANNGING_DELAY);
            APP_PowerOff_CancelTimer(TIMER_AWS_IDLE_POWEROFF);
        }
    }
}

static void app_Mcsync_RoleModeChangeCfmHandler(MCSYNC_ROLEMODECHANGE_CFM_T *cfm)
{
    DBG_LOG_APP_MCSYNC( "[APP_MCSync] Role Mode Change Cfm. status:%d, defaultRole:0x%x", 2, cfm->status, BtAwsMce_GetDefaultRole());

    if(cfm->status == HCI_STATUS_SUCCESS)
    {
        U8 role = BtAwsMce_GetDefaultRole();
        if(role == ROLE_AGENT)
        {
            APP_Ble_EnableAdv(BLE_ADV_PRIMARY_MASK);
            #ifdef PROFILE_AMA_ENABLE
            APP_Ble_EnableAdv(BLE_ADV_AMA_MASK);
            #endif
            #ifdef XIAOAI_DEVELOPMENT
            APP_XiaoaiResetAdv();
            #endif
<<<<<<< HEAD
			#ifdef PROFILE_GFP_ENABLE
            APP_Gfp_EnableAdv();
=======
			#ifdef AIR_GFP_ENABLE
            APP_Ble_EnableAdv(BLE_ADV_GFP_MASK);
>>>>>>> db20e11 (second commit)
			#endif

			if(APP_InEarDetection_IsEnable())
			{
				U8 twoEarStatus = (APP_InEarDetection_GetPartnerStatus()<<1) | APP_InEarDetection_GetStatus();
				DBG_LOG_APP_MCSYNC( "[InEarDetection] a:0x%x / p:0x%x / two 0x%x", APP_InEarDetection_GetStatus(), APP_InEarDetection_GetPartnerStatus(), twoEarStatus);
				APP_InEarDetection_SetLastTwoEarStatus(twoEarStatus);
			}
        }else if(role == ROLE_PARTNER){
            APP_Ble_DisableAdv(BLE_ADV_PRIMARY_MASK);
            #ifdef PROFILE_AMA_ENABLE
            APP_Ble_DisableAdv(BLE_ADV_AMA_MASK);
            APP_Ble_Disconnect(NULL);
            #endif
            #ifdef XIAOAI_DEVELOPMENT
            APP_Ble_DisableAdv(BLE_ADV_XIAOAI_MASK);
            #endif
<<<<<<< HEAD
			#ifdef PROFILE_GFP_ENABLE
            APP_Gfp_DisableAdv();
=======
			#ifdef AIR_GFP_ENABLE
            APP_Ble_DisableAdv(BLE_ADV_GFP_MASK);
>>>>>>> db20e11 (second commit)
            #endif
        }

    }

	APP_System_RoleModeChangeInd(cfm->status);
}

static void app_AwsMce_SetAirParingCfmHandler(MCSYNC_SETAIRPAIRING_CFM_T *cfm)
{
	if(APP_State_CheckNestStateByLink(APP_EOF, APP_AWS_PAIRING))
	{
		APP_MCSYNC_StateSetAirPairingCfmHandler(cfm->status);
	}
}

static void app_Mcsync_NeedDisconnectHandler(AWSMCE_SYNC_NEED_DISCONNECT_T *ind)
{
	if(BtAwsMce_IsDefaultRoleAgent())
	{
		APP_AWSMCE_SetReasonForDisconnect(ind->type);
		APP_Conn_ReleaseMultiLink();
	}
}

static void app_AwsMce_SyncPowerOffHandler(AWSMCE_SYNC_POWEROFF_IND_T *ind)
{
	DBG_LOG_APP_MCSYNC( "[APP_MCSync] Sync Power Off Handler: %d %d %d", 3,
		BtAwsMce_GetDefaultRole(), ind->isFollowerIFPKT, ind->isOnlyForFollower);

<<<<<<< HEAD
	#ifdef MCSYNC_SHARE_MODE
=======
	#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
	DBG_LOG_APP_MCSYNC( "[APP_MCSync] Sync Power Off Handler, share mode:%d", 1, BtMCSync_GetShareMode());

	if(ind->isFollowerIFPKT)
	{
		APP_MCSYNC_SendSyncPowerOff(ind->reason, FALSE, TRUE);
		APP_MCSYNC_SendSyncPowerOff(ind->reason, FALSE, TRUE);
	}
	else if(BtMCSync_GetShareMode() == MCSYNC_SHARE_MODE_FOLLOWER_ENABLE && ind->isOnlyForFollower)
	{
		APP_PowerOff_Loader(ind->reason);
	}
	else if((BtMCSync_GetShareMode() == MCSYNC_SHARE_MODE_NORMAL_ENABLE && BtAwsMce_IsDefaultRolePartner() && ind->isOnlyForFollower)
		||(BtMCSync_GetShareMode() == MCSYNC_SHARE_MODE_FOLLOWER_ENABLE && !ind->isOnlyForFollower))
	{
		//Do nothing
	}
	else
	#endif
	{
        DBG_LOG_APP_MCSYNC( "[APP_MCSync]  Sync Agent - PowerOff, reason(%d)", 1, ind->reason);
		APP_PowerOff_Loader(ind->reason);
	}

}

static void app_Mcsync_SetCurrentRoleCfmHandler(MCSYNC_SET_CURRENT_ROLE_CFM_T *cfm)
{
#ifdef PROFILE_AMA_ENABLE
	if(cfm->role == ROLE_AGENT)
	{
		APP_Ble_EnableAdv(BLE_ADV_AMA_MASK);
	}
	else if(cfm->role == ROLE_PARTNER)
	{
		APP_Ble_DisableAdv(BLE_ADV_AMA_MASK);
	}
#else
    UNUSED(cfm);
#endif
}

<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
static void app_Mcsync_SyncShareModeInfoHandler(MCSYNC_SYNC_SHARE_MODE_INFO_IND_T *ind)
{
	APP_MCSync_Share_PartnerEventHandler(APP_SHARE_PARTNER_EVENT_SYNC_INFO, (U8 *)ind);
}

static void app_Mcsync_SyncFollowerSoundLevelStatusHandler(AWSMCE_SYNC_FOLLOWER_SOUNDLEVEL_IND_T *ind)
{
	if(ind)
	{
		if(BtAwsMce_IsDefaultRoleAgent())
		{
			APP_MCSYNC_SendSyncFollowerSoundLevel(ind->soundLevel, ind->isFollowerIFPkt);
		}
		else if(BtMCSync_GetShareMode() == MCSYNC_SHARE_MODE_FOLLOWER_ENABLE)
		{
			BD_ADDR_T *pBdAddr = APP_GetServiceBdAddr();
			if(pBdAddr)
			{
				APP_VolA2dp_RemoteHandler(pBdAddr, ind->soundLevel);
			}
		}
	}
}

static void app_Mcsync_SyncFollowerRetriggerA2dpTransformStatusHandler(AWSMCE_SYNC_FOLLOWER_RETRIGGER_A2DP_TRANSFORM_IND_T *ind)
{
	U8 shareMode = BtMCSync_GetShareMode();
	BD_ADDR_T *pBdAddr = APP_GetServiceBdAddr();
	U8 specificRole;

	if(ind == NULL || pBdAddr == NULL)
	{
		DBG_LOG_APP_MCSYNC("[APP_MCSync] SyncFollowerRetriggerA2dp is null", 0);
		return;
	}

	 DBG_LOG_APP_MCSYNC("[APP_MCSync] SyncFollowerRetriggerA2dp, share mode:%d, specify role:%d", 2, shareMode, ind->specificRole);
	specificRole = ind->specificRole;

	if(BtAwsMce_IsDefaultRoleAgent() && shareMode == MCSYNC_SHARE_MODE_NORMAL_ENABLE)
	{
		APP_MCSYNC_SendSyncA2dpInfo(pBdAddr, specificRole);
	}

}

static void app_Mcsync_SyncOtherAgentStatusHandler(AWSMCE_SYNC_OTHER_AGENT_BDA_IND_T *ind)
{
    APP_MCSync_Share_SyncOtherAgentStatusHandler(&ind->bdAddr);
}

static void app_Mcsync_SyncFollowerConnStatusHandler(AWSMCE_SYNC_FOLLOWER_CONN_IND_T *ind)
{
	if(ind->doConn)
	{
		APP_MCSync_Share_KeyFollowerStart();
	}
	else
	{
		APP_MCSync_Share_KeyFollowerStop();
	}
}

static void app_Mcsync_SyncFollowerDiscStatusHandler(AWSMCE_SYNC_FOLLOWER_DISC_IND_T *ind)
{
	if(ind)
	{
        APP_MCSync_Share_SyncFollowerDiscStatusHandler(ind->isFollowerIFPkt, ind->isOnlyForFollower);
	}

}


static void app_Mcsync_SyncAgentRetransmitFollowerDataStatusHandler(AWSMCE_SYNC_AGENT_RETRANSMIT_FOLLOWER_DATA_IND_T *ind)
{
    APP_MCSync_Share_SyncAgentRetransmitDataHandler((U8 *)ind);
}

static void app_Mcsync_SyncAgentNoticeFollowerLeaveStatusHandler(AWSMCE_SYNC_AGENT_NOTICE_FOLLOWER_LEAVE_IND_T *ind)
{
    APP_MCSync_Share_FollowerEventHandler(APP_SHARE_FOLLOWER_EVENT_LEAVE_MODE_IND, NULL, (U8 *)ind);
}

#endif

/**************************************************************************************************
* Static (Message Handler)
**************************************************************************************************/
static U32 app_AwsMce_IndHandler(Handler handler, U16 id, void *msg, U32 handler_id)
{
	UNUSED(handler);
	UNUSED(handler_id);
	U32 value = 0;
<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
	if(!APP_MCSync_Share_IsGetIFPacketAllowed(id))
	{
		return 0;
	}
#endif

	switch(id)
	{
		case AWSMCE_START_SERVICE_CFM:
			break;
		case AWSMCE_CONNECT_CFM:
			app_AwsMce_ConnectCfmHandler((AWSMCE_CONNECT_CFM_T * )msg);
			break;
		case AWSMCE_DISCONNECT_IND:
			app_AwsMce_DisconnectIndHandler((AWSMCE_DISCONNECT_IND_T *)msg);
			break;
		case AWSMCE_RECONNECT_IND:
			app_AwsMce_ReconnectIndHandler();
			break;
		case AWSMCE_RECONNECT_POWER_ON_IND:
			app_AwsMce_ReconnectPowerOnIndHandler();
			break;
		case AWSMCE_PICO_CLOCK_CFM:
			app_AwsMce_PicoClockCfmHandler((AWS_MCE_PICO_CLOCK_CFM_T * )msg);
			break;
		case AWSMCE_STATUS_CHANGE_IND:
			app_AwsMce_StatusChangeIndHandler((AWSMCE_STATUS_CHANGE_IND_T *)msg);
			break;
		case AWSMCE_AGENT_A2DP_EVT_IND:
			app_AwsMce_AgentA2dpEvtIndHandler((AWSMCE_AGENT_A2DP_EVT_IND_T *)msg);
			break;
		case AWSMCE_AGENT_SCO_EVT_IND:
			app_AwsMce_AgentScoEvtIndHandler((AWSMCE_AGENT_SCO_EVT_IND_T *)msg);
			break;

		case AWSMCE_SYNC_SMART_CHARGE_CASE_STATE_IND:
			APP_MCSync_SyncSmartChargeCaseStateIndHandler(((AWSMCE_SYNC_SMART_CHARGE_CASE_STATE_IND_T *)msg)->smartChargeCaseState);
			break;

		case AWSMCE_SYNC_KEY_IND:
			APP_MCSync_SyncPartnerKeyIndHandler(((AWSMCE_SYNC_KEY_ACTION_IND_T *)msg)->keyEventCode, ((AWSMCE_SYNC_KEY_ACTION_IND_T *)msg)->supportedState);
			break;

		case AWSMCE_SYNC_KEYCODE_IND:
			app_AwsMce_SyncPartnerKeyCodeIndHandler((AWSMCE_SYNC_KEYCODE_IND_T*)msg);
			break;

		case AWSMCE_SYNC_VPRT_IND:
			value = app_AwsMce_SyncAgentVpRtIndHandler((AWSMCE_SYNC_VPRT_IND_T*)msg);
			break;

		case AWSMCE_SYNC_STOP_VPRT_IND:
			app_AwsMce_SyncAgentStopVpRtIndHandler((AWSMCE_SYNC_STOP_VPRT_IND_T *)msg);
			break;

		case AWSMCE_SYNC_VPRT_PLAY_LATER_IND:
			app_AwsMce_SyncAgentVpRtPlayLaterIndHandler((AWSMCE_SYNC_VPRT_PLAY_LATER_IND_T *)msg);
			break;

		case AWSMCE_SYNC_CALLER_ID_IND:
			app_AwsMce_SyncAgentCallerIdIndHandler((AWSMCE_SYNC_CALLER_ID_IND_T*)msg);
			break;
		case AWSMCE_SYNC_PEQREALTIME_IND:
			app_AwsMce_SyncAgentPEQRealTimeIndHandler((AWSMCE_SYNC_PEQREALTIME_IND_T*)msg);
			break;
		case AWSMCE_RHO_PACKET_IND:
			app_AwsMce_RhoPacketIndHandler((AWSMCE_RHO_PACKET_IND_T *)msg);
			break;

		case AWSMCE_RHO_ROLE_CHANGE_IND:
			app_AwsMce_RhoRoleChangeIndHandler((MCSYNC_RHO_ROLE_CHANGE_IND_T *)msg);
			break;

		case AWSMCE_CANCEL_CONN_CFM:
			app_AwsMce_CancelConnCfmHandler((MCSYNC_CANCEL_CONN_CFM_T *)msg);
			break;

		case AWSMCE_MSG_SLAVE_VPRT_DELAY_IND:
			app_AwsMce_SyncAgentVpRtIndHandler((AWSMCE_SYNC_VPRT_IND_T*)msg);
			break;

		case AWSMCE_AWS_IDLE_TIMER_CTL_IND:
			app_AwsMce_AwsIdleTimerCtlIndHandler((AWSMCE_AWS_IDLE_TIMER_CTL_IND_T *)msg);
			break;

		case AWSMCE_SYNC_POWEROFF_IND:
			app_AwsMce_SyncPowerOffHandler((AWSMCE_SYNC_POWEROFF_IND_T *)msg);
            break;

		case MCSYNC_ROLEMODECHANGE_CFM:
			app_Mcsync_RoleModeChangeCfmHandler((MCSYNC_ROLEMODECHANGE_CFM_T *)msg);
<<<<<<< HEAD
=======
#ifdef AIR_TILE_ADV_ENABLE
            APP_Tile_NotifyMcsyncRoleModeChange(((MCSYNC_ROLEMODECHANGE_CFM_T *)msg)->status);
#endif
>>>>>>> db20e11 (second commit)
			break;

		case MCSYNC_SETAIRPAIRING_CFM:
			app_AwsMce_SetAirParingCfmHandler((MCSYNC_SETAIRPAIRING_CFM_T *)msg);
			break;

		case AWSMCE_NEED_DISCONNECT_IND:
			app_Mcsync_NeedDisconnectHandler((AWSMCE_SYNC_NEED_DISCONNECT_T *)msg);
			break;

        case AWSMCE_SYNC_GENERAL_DATA_IND:
			app_AwsMce_SyncGeneralDataIndHandler((AWSMCE_SYNC_GENERAL_DATA_IND_T *)msg);
			break;

        case MCSYNC_SET_CURRENT_ROLE_CFM:
            app_Mcsync_SetCurrentRoleCfmHandler((MCSYNC_SET_CURRENT_ROLE_CFM_T *)msg);
            break;

<<<<<<< HEAD
#ifdef APP_TILE_ENABLE
=======
#ifdef AIR_TILE_ADV_ENABLE
>>>>>>> db20e11 (second commit)
        case AWSMCE_SYNC_STATE_CHANGED_IND:
            APP_Tile_NotifyMcsyncState(((AWSMCE_SYNC_STATE_CHANGED_IND_T *)msg)->newState);
            break;
#endif

<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
		case MCSYNC_SYNC_SHARE_MODE_INFO_IND:
			app_Mcsync_SyncShareModeInfoHandler((MCSYNC_SYNC_SHARE_MODE_INFO_IND_T *)msg);
			break;
		case AWSMCE_SYNC_FOLLOWER_SOUNDLEVEL_IND:
			app_Mcsync_SyncFollowerSoundLevelStatusHandler((AWSMCE_SYNC_FOLLOWER_SOUNDLEVEL_IND_T*)msg);
			break;

		case AWSMCE_SYNC_FOLLOWER_RETRIGGER_A2DP_TRANSFORM_IND:
			if(APP_AWSMCE_CheckTimer(APP_MCS_TIMER_A2DP_RETRIGGER))
			{

			}
			else
			{
				APP_AWSMCE_SetTimer(APP_MCS_TIMER_A2DP_RETRIGGER, 2000);
				app_Mcsync_SyncFollowerRetriggerA2dpTransformStatusHandler((AWSMCE_SYNC_FOLLOWER_RETRIGGER_A2DP_TRANSFORM_IND_T*)msg);
			}
			break;

		case AWSMCE_SYNC_OTHER_AGENT_BDA_IND:
			app_Mcsync_SyncOtherAgentStatusHandler((AWSMCE_SYNC_OTHER_AGENT_BDA_IND_T*)msg);
			break;

		case AWSMCE_SYNC_FOLLOWER_CONN_IND:
			app_Mcsync_SyncFollowerConnStatusHandler((AWSMCE_SYNC_FOLLOWER_CONN_IND_T*)msg);
			break;

		case AWSMCE_SYNC_FOLLOWER_DISC_IND:
			app_Mcsync_SyncFollowerDiscStatusHandler((AWSMCE_SYNC_FOLLOWER_DISC_IND_T*)msg);
			break;

		case AWSMCE_SYNC_AGENT_RETRANSMIT_FOLLOWER_DATA_IND:
			app_Mcsync_SyncAgentRetransmitFollowerDataStatusHandler((AWSMCE_SYNC_AGENT_RETRANSMIT_FOLLOWER_DATA_IND_T*)msg);
			break;

		case AWSMCE_SYNC_AGENT_NOTICE_FOLLOWER_LEAVE_IND:
			app_Mcsync_SyncAgentNoticeFollowerLeaveStatusHandler((AWSMCE_SYNC_AGENT_NOTICE_FOLLOWER_LEAVE_IND_T*)msg);
			break;

        case AWSMCE_SYNC_PARTNER_REQUEST_EXIT_MODE_IND:
            APP_MCSync_Share_ExitMode();
            break;
#endif

		default:
			break;
	}
	return value;
}



/**************************************************************************************************
* Public function
**************************************************************************************************/
void APP_AWSMCE_RegisterProfile(void)
{
	BtAwsMce_StartService(&gAppAwsMceHandle);
}

BOOL APP_AWSMCE_KeyEvent(U8 keyIndex, U8 keyAction)
{
	U16 keyEventCode;
	U8 state;
	U32 supportedState;

	if(BtAwsMce_GetDefaultRole() == ROLE_PARTNER)
	{
        if(BtAwsMce_GetMcsyncState() != MCSYNC_LINK_CONNECTED)
        {
            DBG_LOG_APP_MCSYNC( "[APP_MCSync] Keyevent : Only Partner!!", 0);
            return FALSE;
        }

        if(keyAction == EVENT_KEY_DOWN)
        {
            APP_MCSYNC_SendSyncKeyInfo(KEY_INVALID, 0);
        }
		switch(APP_GetAgentState())
		{
			case APP_LINE_IN:
			case APP_CONNECTED:
			case APP_HFP_INCOMMING:
			case APP_HFP_OUTGOING:
			case APP_HFP_CALLACTIVE:
			case APP_HFP_CALLACTIVE_WITHOUT_SCO:
			case APP_HFP_CAIMG:
			case APP_HFP_CAOGG:
			case APP_HFP_CAMULTY:
				state = APP_GetAgentState();
				keyEventCode = APP_NVKEY_KeyParseEvent(keyIndex, keyAction, state);
				supportedState = APP_NVKEY_GetSupportedStateByEventCode(APP_Customer_IsSpecialEventCode(keyEventCode));
				switch(APP_ChkKeyVld_GetKeyEventGroup(keyEventCode))
				{
					case NOT_KEY_EVENT:
						state = APP_State_GetTopState(APP_GetServiceBdAddr());
						keyEventCode = APP_NVKEY_KeyParseEvent(keyIndex, keyAction, state);
						switch(keyEventCode)
						{
							case KEY_VOICE_COMMAND_ENABLE:
							case KEY_VOICE_PROMPT_LANG_CHANGE:
							case KEY_ANC_SWITCH:
							case KEY_AIROTHRU:
								APP_NVKEY_KeyParseEventPrintLog(keyIndex, keyAction, state);
								return TRUE; //if agent not allow, so does partner
						}

						switch(APP_ChkKeyVld_GetKeyEventGroup(keyEventCode))
						{
							case HID_KEY_EVENT:
							case HFP_KEY_EVENT:
							case AVRCP_KEY_EVENT:
								APP_NVKEY_KeyParseEventPrintLog(keyIndex, keyAction, state);
								return TRUE; //if agent not allow, so does partner
						}

						return FALSE;
					case HID_KEY_EVENT:
					case HFP_KEY_EVENT:
					case AVRCP_KEY_EVENT:
						APP_NVKEY_KeyParseEventPrintLog(keyIndex, keyAction, state);
                        DBG_LOG_APP_MCSYNC( "[APP_MCSync][Partner] Sync Key Event:%s", 1, APP_KeyEventLogString[keyEventCode]);
						APP_MCSYNC_SendSyncKeyInfo(keyEventCode, supportedState);
						return TRUE;
					case SYSTEM_KEY_EVENT:
						if(app_AwsMce_IsSystemKeyNeedSync(keyEventCode))
						{
                            DBG_LOG_APP_MCSYNC( "[APP_MCSync][Partner] Sync Key Event:%s", 1, APP_KeyEventLogString[keyEventCode]);
							APP_MCSYNC_SendSyncKeyInfo(keyEventCode, supportedState);
							if(keyEventCode == KEY_POWER_OFF) //partner tell agent to power off and partner power off immediately
							{
								return FALSE;
							}
							APP_NVKEY_KeyParseEventPrintLog(keyIndex, keyAction, state);
							return TRUE;
						}
						break;

					case AMA_KEY_EVENT:
						DBG_LOG_APP_MCSYNC( "[APP_MCSync][Partner] Sync Key Event:%s", 1, APP_KeyEventLogString[keyEventCode]);
						APP_MCSYNC_SendSyncKeyInfo(keyEventCode, supportedState);
						APP_NVKEY_KeyParseEventPrintLog(keyIndex, keyAction, state);
						return TRUE;

					#ifdef XIAOAI_DEVELOPMENT
					case XIAOAI_KEY_EVENT:
						DBG_LOG_APP_MCSYNC( "[APP_MCSync][Partner] Sync Key Event:%s", 1, APP_KeyEventLogString[keyEventCode]);
						APP_MCSYNC_SendSyncKeyInfo(keyEventCode, supportedState);
						APP_NVKEY_KeyParseEventPrintLog(keyIndex, keyAction, state);
						return TRUE;
					#endif

					default:
						return FALSE;
				}

			default:
				state = APP_State_GetTopState(APP_GetServiceBdAddr());
				keyEventCode = APP_NVKEY_KeyParseEvent(keyIndex, keyAction, state);
				supportedState = APP_NVKEY_GetSupportedStateByEventCode(APP_Customer_IsSpecialEventCode(keyEventCode));
				switch(keyEventCode)
				{
					case KEY_DISCOVERABLE:
						if(MCSYNC_LINK_CONNECTED != BtAwsMce_GetMcsyncState())
							return FALSE;
					case KEY_RECONNECT_USER_INIT:
					case KEY_NONDISCOVERABLE:
					case KEY_AIROTHRU:
					case KEY_PEQ_GROUP_CHANGE:
<<<<<<< HEAD
					case KEY_FOLLOWER_STOPS_SHARE_MODE:
=======
					case KEY_FOLLOWER_SHARE_MODE_STOP:
					case KEY_AVRCP_PLAY:
>>>>>>> db20e11 (second commit)
						APP_NVKEY_KeyParseEventPrintLog(keyIndex, keyAction, state);

						if(MCSYNC_LINK_CONNECTED != BtAwsMce_GetMcsyncState())
							return FALSE;

						DBG_LOG_APP_MCSYNC( "[APP_MCSync][Partner] Sync Key Event:%s", 1, APP_KeyEventLogString[keyEventCode]);
						APP_MCSYNC_SendSyncKeyInfo(keyEventCode, supportedState);
						return TRUE;
				}
				break;
		}
	}
	else
	{
		DBG_LOG_APP_MCSYNC( "[APP_MCSync] Keyevent : it's not Partner!!", 0);
	}
	return FALSE;
}

BOOL APP_AWSMCE_KeyCode(U16 keyCode)
{
	if(BtAwsMce_GetDefaultRole() == ROLE_PARTNER)
	{
		switch(APP_GetAgentState())
		{
			case APP_LINE_IN:
			case APP_CONNECTED:
			case APP_HFP_INCOMMING:
			case APP_HFP_OUTGOING:
			case APP_HFP_CALLACTIVE:
			case APP_HFP_CALLACTIVE_WITHOUT_SCO:
			case APP_HFP_CAIMG:
			case APP_HFP_CAOGG:
			case APP_HFP_CAMULTY:
				switch(APP_ChkKeyVld_GetKeyEventGroup(keyCode))
				{
					case NOT_KEY_EVENT:

						switch(keyCode)
						{
							case KEY_VOICE_COMMAND_ENABLE:
							case KEY_VOICE_PROMPT_LANG_CHANGE:
								return TRUE;
							default:
								switch(APP_ChkKeyVld_GetKeyEventGroup(keyCode))
								{
									case HID_KEY_EVENT:
									case HFP_KEY_EVENT:
									case AVRCP_KEY_EVENT:
										return TRUE;
								}
						}
						return FALSE;
					case HID_KEY_EVENT:
					case HFP_KEY_EVENT:
					case AVRCP_KEY_EVENT:
						APP_MCSYNC_SendSyncKeyCodeInfo(keyCode);
						return TRUE;
					case SYSTEM_KEY_EVENT:
						if(app_AwsMce_IsSystemKeyNeedSync(keyCode))
						{
							APP_MCSYNC_SendSyncKeyCodeInfo(keyCode);
							return TRUE;
						}
					default:
						return FALSE;
				}
                /* fallthrough */
			default:
				switch(keyCode)
				{
					case KEY_RECONNECT_USER_INIT:
					case KEY_DISCOVERABLE:
					case KEY_NONDISCOVERABLE:
					case KEY_AIROTHRU:
					case KEY_PEQ_GROUP_CHANGE:
						APP_MCSYNC_SendSyncKeyCodeInfo(keyCode);
						return TRUE;
				}
				break;
		}
	}
	return FALSE;
}

U8 APP_AWSMCE_GetMsgNumInQueue(void)
{
	U8 totalMsgNum = 0, msgNum;

	if((msgNum = APP_MCSYNC_SystemGetMsgNumInQueue()))
	{
		totalMsgNum += msgNum;
		DBG_LOG_APP_MCSYNC( "[APP_MCSync] AwsSystem RHO Not Allow Msg Number:%d", 1, msgNum);
	}

	return (totalMsgNum + MSG_MessageSearchAllHandler(&gAppAwsMceHandle));
}

BOOL APP_AWSMCE_FillRhoData(void *pData)
{
	APP_RHO_MCSYNC_STRU *pDataToFill = &((APP_RHO_DATA_STRU *)pData)->appMCSync;

	if(!pDataToFill)
	{
		DBG_LOG_APP_MCSYNC( "[APP_MCSync] Fill RHO data error", 0);
		return FALSE;
	}

	pDataToFill->rssi = gAppMCSyncCtl.inquiryPara.rssi;
	FW_Memcpy(&pDataToFill->deviceAddr, &gAppMCSyncCtl.inquiryPara.deviceAddr, sizeof(BD_ADDR_T));
	pDataToFill->isFoundRssi = gAppMCSyncCtl.inquiryPara.isFoundRssi;

<<<<<<< HEAD
	#ifdef MCSYNC_SHARE_MODE
=======
	#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
	pDataToFill->isShareMode = APP_MCSync_Share_IsEnable();
	#else
	pDataToFill->isShareMode = FALSE;
	#endif

	return TRUE;
}

BOOL APP_AWSMCE_AssignRhoData(void *pData)
{
	APP_RHO_MCSYNC_STRU *pDataToGet = &((APP_RHO_DATA_STRU *)pData)->appMCSync;

	if(!pDataToGet)
	{
		DBG_LOG_APP_MCSYNC( "[APP_MCSync] Assign RHO data error", 0);
		return FALSE;
	}

	gAppMCSyncCtl.inquiryPara.rssi = pDataToGet->rssi;
	FW_Memcpy(&gAppMCSyncCtl.inquiryPara.deviceAddr, &pDataToGet->deviceAddr, sizeof(BD_ADDR_T));
	gAppMCSyncCtl.inquiryPara.isFoundRssi = pDataToGet->isFoundRssi;

<<<<<<< HEAD
    #ifdef MCSYNC_SHARE_MODE
=======
    #ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
    APP_MCSync_Share_RhoAssign(pDataToGet->isShareMode);
    #endif

	return TRUE;
}

void APP_AWSMCE_SetRSSIInfo(S8 rssi, BD_ADDR_T* pBdAddr)
{
	FW_Memcpy((U8*)&gAppMCSyncCtl.inquiryPara.deviceAddr, (U8*)pBdAddr, sizeof(BD_ADDR_T));
	gAppMCSyncCtl.inquiryPara.rssi = rssi;
	gAppMCSyncCtl.inquiryPara.isFoundRssi = 1;
}

BD_ADDR_T* APP_AWSMCE_GetBestRSSIAddr(void)
{
	if(gAppMCSyncCtl.inquiryPara.isFoundRssi)
	{
		return (BD_ADDR_T*)&gAppMCSyncCtl.inquiryPara.deviceAddr.addr[0];
	}
	else
	{
		return (BD_ADDR_T*)NULL;
	}
}

BOOL APP_AWSMCE_DecideRole(U8* pTargetBdAddr)
{
	U8* pLocalBdAddr = (U8*)BtAwsMce_ReadLocalBdAddr();
	U8 i = 0;

	if(pTargetBdAddr == NULL)
		return FALSE;

	for(i = 0; i < sizeof(BD_ADDR_T); i++)
	{
		if(pLocalBdAddr[i] != pTargetBdAddr[i])
			break;
	}

	if(i == sizeof(BD_ADDR_T))
	{
		return FALSE;
	}

	if(pLocalBdAddr[i] > pTargetBdAddr[i])
	{
		DBG_LOG_APP_MCSYNC( "[MCSync] Decide Role: Agent", 0);
		BtAwsMce_WriteDefaultInfo(ROLE_AGENT, (BD_ADDR_T *)pLocalBdAddr, (BD_ADDR_T *)pTargetBdAddr);
		#ifdef AIRAPP_Profile
        APP_Ble_EnableAdv(BLE_ADV_PRIMARY_MASK);
		#endif
        #ifdef PROFILE_AMA_ENABLE
        APP_Ble_EnableAdv(BLE_ADV_AMA_MASK);
        #endif
		#ifdef XIAOAI_DEVELOPMENT
		APP_Ble_EnableAdv(BLE_ADV_XIAOAI_MASK);
		#endif
<<<<<<< HEAD
		#ifdef PROFILE_GFP_ENABLE
        APP_Gfp_EnableAdv();
=======
		#ifdef AIR_GFP_ENABLE
        APP_Ble_EnableAdv(BLE_ADV_GFP_MASK);
>>>>>>> db20e11 (second commit)
        #endif
	}
	else
	{
		DBG_LOG_APP_MCSYNC( "[MCSync] Decide Role: Partner", 0);
		BtAwsMce_WriteDefaultInfo(ROLE_PARTNER, (BD_ADDR_T *)pTargetBdAddr, (BD_ADDR_T *)pLocalBdAddr);
		#ifdef AIRAPP_Profile
        APP_Ble_DisableAdv(BLE_ADV_PRIMARY_MASK);
		#endif
        #ifdef PROFILE_AMA_ENABLE
        APP_Ble_DisableAdv(BLE_ADV_AMA_MASK);
        #endif
		#ifdef XIAOAI_DEVELOPMENT
		APP_Ble_DisableAdv(BLE_ADV_XIAOAI_MASK);
		#endif
<<<<<<< HEAD
		#ifdef PROFILE_GFP_ENABLE
        APP_Gfp_DisableAdv();
=======
		#ifdef AIR_GFP_ENABLE
        APP_Ble_DisableAdv(BLE_ADV_GFP_MASK);
>>>>>>> db20e11 (second commit)
        #endif
	}

	return TRUE;
}

void app_AwsMce_SendSyncAllInfo(BD_ADDR_T *pBdAddr)
{
	APP_MCSYNC_SendSyncScoInfo(pBdAddr);
	APP_MCSYNC_SendSyncStateInfo(pBdAddr);
    APP_MCSync_SendAncInfo(0, AudioDsp_GetANCPassThruRegister());
	APP_LED_SyncAllInfo();
	APP_MCSYNC_SendSyncVPLangIndex(APP_VpGetCurrentLangId());
	APP_MCSYNC_SendReconnectMask(APP_Mcsync_GetNotReconnMask());
	APP_MCSYNC_SendSyncPartnerInEarDetectionStatus(APP_InEarDetection_GetStatus());
	APP_Customer_SendSyncInfo();
}

void APP_AWSMCE_SetReasonForDisconnect(U8 reason)
{
	gAppMCSyncCtl.reasonForDisconnect = reason;
}

U8 APP_AWSMCE_GetReasonForDisconnect(void)
{
	return gAppMCSyncCtl.reasonForDisconnect;
}

void APP_AWSMCE_SetTimer(U8 type, U32 time)
{
	FW_SetTimer(&gAppMcsTimerHandle, type, NULL, 0, time);
}

void APP_AWSMCE_ReleaseTimer(U8 type)
{
	FW_ReleaseTimer(&gAppMcsTimerHandle, type, 0);
}

U16 APP_AWSMCE_CheckTimer(U8 type)
{
	return FW_CheckTimer(&gAppMcsTimerHandle, type, 0);
}


void APP_MCSync_SendAncInfo(U32 clockToStart, U8 userRegister)
{
	BD_ADDR_T *pBdAddr = APP_GetServiceBdAddr();

	if(pBdAddr)
		App_SniffDisable(pBdAddr, APP_SNIFF_ANC_IF_PKT);

	APP_MCSYNC_SendSyncANCPassThruInfo( userRegister,
                                        APP_ANCPassThru_GetAncSwitch(),
                                        drv_anc_get_flash_no(),
                                        drv_anc_get_type(),
                                        drv_anc_get_runtime_gain(),
                                        clockToStart);
	if(pBdAddr)
		App_SniffEnable(pBdAddr, APP_SNIFF_ANC_IF_PKT);
}

/**************************************************************************************************
* Public function (MCSync Data)
**************************************************************************************************/
void APP_MCSync_SyncAgentStateIndHandler(U8 agentState)
{
	DBG_LOG_APP_MCSYNC( "[APP_MCSync][Partner] Sync Agent State(%d)(%s)", 2, agentState, APP_StateLogString[agentState]);
	APP_SetAgentState(agentState);
}

void APP_MCSync_SyncRhoStateIndHandler(U8 state)
{
    DBG_LOG_APP_MCSYNC( "[APP_MCSync][Partner] Sync RHO State(%d)", 1, state);
	APP_MCSync_Rho_SetRhoState(state);
}

void APP_MCSync_SyncAgentVPIsEnableIndHandler(BOOL isEnable)
{
	if(isEnable)
		APP_VpEnableVoicePrompt();
	else
		APP_VpDisableVoicePrompt();
}

void APP_MCSync_SyncAgentANCPassThruIndHandler(U8 userRegister, U8 enable, U8 flash_no, U8 type, S16 runtime_gain)
{
<<<<<<< HEAD
	AUDIO_DSP_START_PARA_STUR audioDspStartPara;
	AUDIO_DSP_STOP_PARA_STUR audioDspStopPara;
    DBG_LOG_APP_MCSYNC( "[APP_MCSync][Partner] : Sync ANCPassThru, userRegister(%d) enable(%d) flash_no(%d) type(%d) gain(%d)", 5, userRegister, enable, flash_no, type, runtime_gain);

	AudioDsp_SetANCPassThruRegister(userRegister);
	drv_anc_set_para(flash_no, type, runtime_gain);

	if(enable)
	{
		FW_Memset(&audioDspStartPara, 0, sizeof(AUDIO_DSP_START_PARA_STUR));
		audioDspStartPara.audioType = AUDIO_ANC;
		APP_AudioDspPlay(&audioDspStartPara);
	}
	else
	{
		FW_Memset(&audioDspStopPara, 0, sizeof(AUDIO_DSP_STOP_PARA_STUR));
		audioDspStopPara.audioType = AUDIO_ANC;
		APP_AudioDspStop(&audioDspStopPara);
	}
=======
	APP_ANCPASSTHRU_PARA_STRU para;

    DBG_LOG_APP_MCSYNC("[APP_MCSync][Partner]:Sync ANCPassThru, userRegister(%d) enable(%d) flash_no(%d) type(%d) gain(%d)", 5, userRegister, enable, flash_no, type, runtime_gain);

	if(enable)
	{
		para.action = ACTION_ON;
	}
	else
	{
		para.action = ACTION_OFF;
	}

	para.sync = FALSE;
	para.userRegister = userRegister;
	para.flash_no = flash_no;
	para.type = type;
	para.runtime_gain = runtime_gain;

	AudioDsp_ANCPassThruHandler(para);
    APP_PeqManage_LoadPeqGroup(PEQ_A2DP);
>>>>>>> db20e11 (second commit)
}

void APP_MCSync_SyncLEDIndHandler(U8 patternIndex, U8* patternType, U8 overwriteTime, BOOL isOverwrite, U32 clockToStart, U8 action)
{
<<<<<<< HEAD
    DBG_LOG_APP_MCSYNC( "[APP_MCSync][Partner] : Sync LED, patternIndex(%d), patternType(%d), loopTimes(%d)", 3, patternIndex, patternType[0], action);
    DBG_LOG_APP_MCSYNC( "[APP_MCSync][Partner] : Sync LED, overwrtieTime(%d), isOverwrite(%d), clock(%d)", 3, overwriteTime, isOverwrite, clockToStart);
=======
>>>>>>> db20e11 (second commit)

	if(BtAwsMce_IsDefaultRoleAgent() && action == MCSYNC_LED_PARTNER_ASK_RESUME_BG)
	{
		APP_LED_ResumeBGHandler();
	}
	else if(BtAwsMce_IsDefaultRolePartner() && action != MCSYNC_LED_PARTNER_ASK_RESUME_BG)
	{
		APP_LED_SyncParameter(patternIndex, (U8*)&patternType[0], overwriteTime, isOverwrite, clockToStart, action);
		APP_LED_PushAction(action);
		BtAwsMce_GetCurrentPiconetBtClock(BtMCSync_ReadAgentBdAddr(), PICO_EVENT_LED);
	}
}

void APP_MCSync_SyncSmartChargeCaseStateIndHandler(U8 smartChargeCaseState)
{
    DBG_LOG_APP_MCSYNC( "[APP_MCSync][Agent] Sync Partner SmartChgCase State(%d)", 1, smartChargeCaseState);

	APP_SetPartnerSmartChargeCaseState(smartChargeCaseState);

	if(BtAwsMce_IsDefaultRoleAgent() && smartChargeCaseState == STATE_OUT_OF_CASE)
	{
        APP_MCSync_SendAncInfo(0, AudioDsp_GetANCPassThruRegister());
#ifdef SMART_CHG_CSE_LIDON_RECONNECT
        DBG_LOG_APP_MCSYNC( "[APP_MCSync][Agent] agent case state (%d), SP state(%d)", 2, DrvCharger_GetSmartCaseState(), APP_Conn_GetAgLinkState());
        if((DrvCharger_GetSmartCaseState() == STATE_LID_OPEN)&&(APP_Conn_GetAgLinkState() == LINK_CONNECTED))
        {
            APP_SmtChgCse_SetCheckStateTimer(1000);
        }
<<<<<<< HEAD
#endif        
	}
	if(BtAwsMce_IsDefaultRoleAgent())
		ChargerSmartCase_SetPartnerCaseState(smartChargeCaseState);
=======
#endif
	}
	if(BtAwsMce_IsDefaultRoleAgent())
		ChargerSmartCase_SetPartnerCaseState(smartChargeCaseState);
#ifdef AIR_GFP_ENABLE
    APP_Gfp_McsyncPartnerSmartChargeCaseStateIndHandle();
#endif
>>>>>>> db20e11 (second commit)
}

void APP_MCSync_SyncPartnerKeyIndHandler(U16 keyEventCode, U32 supportedState)
{
	U32 stateBitMask;

	DBG_LOG_APP_MCSYNC( "[APP_MCSync][Agent] Sync Key Code : Partner keyEventCode(0x%x)(%s), Partner supportedState(0x%x)", 3, keyEventCode, APP_KeyEventLogString[keyEventCode], supportedState);

	if(keyEventCode == KEY_INVALID && supportedState == 0)
	{
		App_SniffDisable(APP_GetServiceBdAddr(), APP_SNIFF_PARTNER_KEY);
		App_SniffEnable(APP_GetServiceBdAddr(), APP_SNIFF_PARTNER_KEY);
	}

	stateBitMask = 1L << APP_State_GetTopState(&gAppCtl.serviceBdAddr);

	if(stateBitMask & supportedState)
	{
		APP_Customer_SyncPartnerKeyEventHandler(keyEventCode);
		APP_EvtKey_KeyHandler(keyEventCode);
	}
	/*
		U8 keyIndex = ind->keyIndex, keyAction = ind->keyAction;

#if (Boomtech_MMI)
		U8 state = APP_State_GetTopState(APP_GetServiceBdAddr());
		if((ind->keyAction == EVENT_KEY_LONGPRESS_RELEASE) && (ind->keyIndex == KEY1) && (state == APP_CONNECTED))
		{
			keyIndex = KEY2;
			keyAction = EVENT_KEY_TRIPLE;
		}
#endif

		APP_EvtKey_KeyEvent(keyIndex, keyAction);
	*/

}
void APP_MCSync_SyncRhoParaInfoIndHandler(U16 batPercent, S8 rssi)
{
<<<<<<< HEAD
    DBG_LOG_APP_MCSYNC( "[APP_MCSync][Agent] : Sync RHO Para, batPercent(%d), rssi(%d)", 2, batPercent, rssi);
=======
>>>>>>> db20e11 (second commit)

    APP_RhoDomn_SetPartnerRssiPara(rssi);
    APP_RhoDomn_SetPartnerBatPara(batPercent);

    BD_ADDR_T * pBda  = APP_GetServiceBdAddr();

    if(pBda != NULL)
    {
	    BtReadRssi(APP_RhoDomn_GetHandle(), &pBda->addr);
    }
}

void APP_MCSync_SyncSmtChgCseInfoIndHandler(BOOL isTimerSet)
{
    APP_SmtChgCse_AgentRhoTimerHandler(isTimerSet);
}

void APP_MCSync_InEarDetectionStatusIndHandler(U8 status)
{
    if(BtAwsMce_IsDefaultRoleAgent())
    {
        APP_InEarDetection_SetPartnerStatus(status);
        if(!APP_MCSync_Rho_IsRhoing())
            APP_InEarDetection_SetDebounceTimer();
    }
}

void APP_MCSync_SyncPowerOffIndHandler(U8 reason, BOOL isFollowerIFPKT, BOOL isOnlyForFollower)
{
<<<<<<< HEAD
	#ifdef MCSYNC_SHARE_MODE
=======
	#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
	DBG_LOG_APP_MCSYNC( "[APP_MCSync] Sync Power Off Handler, share mode:%d", 1, BtMCSync_GetShareMode());

	if(isFollowerIFPKT)
	{
		APP_MCSYNC_SendSyncPowerOff(reason, FALSE, TRUE);
	}
	else if(BtMCSync_GetShareMode() == MCSYNC_SHARE_MODE_FOLLOWER_ENABLE && isOnlyForFollower)
	{
		APP_PowerOff_Loader(reason);
	}
	else if((BtMCSync_GetShareMode() == MCSYNC_SHARE_MODE_NORMAL_ENABLE && BtAwsMce_IsDefaultRolePartner() && isOnlyForFollower)
		||(BtMCSync_GetShareMode() == MCSYNC_SHARE_MODE_FOLLOWER_ENABLE && ! isOnlyForFollower))
	{
		//Do nothing
	}
	else
	#endif
	{
		UNUSED(isFollowerIFPKT); UNUSED(isOnlyForFollower);
        DBG_LOG_APP_MCSYNC( "[APP_MCSync]  Sync Agent - PowerOff, reason(%d)", 1, reason);
		APP_PowerOff_Loader(reason);
	}
}

void APP_MCSync_LinkListIndHandler(U8 * pData, U32 length)
{
    APP_Mcsync_SyncLinkKeyInd(length, pData);
}

void APP_MCSync_ReconnectStateIndHandler(U8 type, BD_ADDR_T *pBdAddr)
{
    DBG_LOG_APP_MCSYNC("[APP_MCSync] Reconnect State Ind. type:%d, BdAddr:0x%x%x", 3, type,
            FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE));

    if(BtAwsMce_IsDefaultRoleAgent())
    {
        APP_Reconn_SetCheckReconnect(type, pBdAddr);
    }
}

void APP_MCSync_ReconnectMaskIndHandler(U8 notReconnMask)
{
    DBG_LOG_APP_MCSYNC( "[APP_MCSync] Reconnect Mask Ind. notReconnMask:0x%x", 1, notReconnMask);

    if(BtAwsMce_IsDefaultRolePartner())
    {
        APP_Mcsync_SyncNotReconnMaskIndHandler(notReconnMask);
    }
}

void APP_MCSync_SyncKeyCodeInfoIndHandler(U16 keyCode)
{
    APP_Media_PushMediaEvent(keyCode);
    APP_EvtKey_KeyEventHandler(APP_GetServiceBdAddr(), keyCode);
}

<<<<<<< HEAD
void APP_MCSync_SyncGFPInfoIndHandler(void *pData, U32 length)
{
    #if defined(PROFILE_GFP_ENABLE)
    GFPSv2_mcsync_accountkey(pData, length);
    #else
    UNUSED(pData);
    UNUSED(length);
    #endif
}
=======
#ifdef AIR_TILE_ADV_ENABLE
void APP_MCSync_SyncTileInfoIndHandler(U8 opcode, U8 streamState)
{
    DBG_LOG_APP_MCSYNC("[APP_MCSync] Sync Tile info Ind. opcode:%d, streamState: %d", 2, opcode, streamState);

	if (BtAwsMce_IsDefaultRoleAgent() && (APP_MCSYNC_TILE_INFO_OPCODE_RSP == opcode)) {
		/* notify Agent to do next RHO action. */
		app_MCSync_Rho_PartnerBleDisconnectNotify(BT_STATUS_SUCCESS);
		APP_Tile_Set_Partner_StreamState(streamState);
	} else if(BtAwsMce_IsDefaultRolePartner() && (APP_MCSYNC_TILE_INFO_OPCODE_CMD == opcode)) {
		/* to Stop BLE adv and disconnect BLE */
		APP_Ble_DisableAdv(BLE_ADV_TILE_MASK);
		APP_Ble_Disconnect(NULL);
		/*RSP to Agent to do Next RHO Action. */
		APP_MCSYNC_SendSyncTileInfo(FALSE);
		/*Enable a timer to wait RHO Done event. */

	}
}
#endif
>>>>>>> db20e11 (second commit)

/**************************************************************************************************
* Public function (MCSync Init)
**************************************************************************************************/

void APP_MCSync_Init(void)
{
	APP_MCSync_Nvkey_Init();
	APP_MCSync_Data_Callback_Init();

}

