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
#include "App_MCSync_Data.h"

<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
#include "App_MCSync_Share.h"
#include "App_VolumeA2dpProcess.h"
#endif
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
#include "App_MCSync_Share.h"
#include "App_VolumeA2dpProcess.h"
#endif
#ifdef AIR_GFP_ENABLE
#include "App_Gfp.h"
#endif

#include "BtMCSync.h"
>>>>>>> db20e11 (second commit)
/**************************************************************************************************
* Structure
**************************************************************************************************/

/**************************************************************************************************
* Prototype 	Static bt group(MCSync data init & callback)
**************************************************************************************************/
static void app_mcsync_bt_state_sync_callback(U16 length, U8 *p_data);
static void app_mcsync_bt_ag_num_sync_callback(U16 length, U8 *p_data);
static void app_mcsync_bt_caller_id_sync_callback(U16 length, U8 *p_data);
static void app_mcsync_bt_rho_para_sync_callback(U16 length, U8 *p_data);
static void app_mcsync_bt_rho_state_sync_callback(U16 length, U8 *p_data);
static void app_mcsync_bt_link_list_sync_callback(U16 length, U8 *p_data);
static void app_mcsync_bt_ble_link_list_sync_callback(U16 length, U8 *p_data);
static void app_mcsync_bt_reconnect_state_sync_callback(U16 length, U8 *p_data);
static void app_mcsync_bt_reconnect_mask_sync_callback(U16 length, U8 *p_data);
static void app_mcsync_bt_retransmit_follower_disc_sync_callback(U16 length, U8 *p_data);
static void app_mcsync_bt_l2cap_data_callback(U16 length, U8 *p_data);
<<<<<<< HEAD

=======
static void app_mcsync_bt_take_over_callback(U16 length, U8 *p_data);
static void app_mcsync_bt_notice_master_partner_to_share_mode_sync_callback(U16 length, U8 *p_data);
>>>>>>> db20e11 (second commit)
/**************************************************************************************************
* Prototype 	Static gva group(MCSync data init & callback)
**************************************************************************************************/
static void app_mcsync_gva_gfp_sync_callback(U16 length, U8 *p_data);

<<<<<<< HEAD
=======
/**************************************************************************************************
*Static Tile group(MCSync data init & callback)
**************************************************************************************************/
#ifdef AIR_TILE_ADV_ENABLE
static void app_mcsync_tile_data_sync_callback(U16 length, U8 *p_data);
#endif
>>>>>>> db20e11 (second commit)

/**************************************************************************************************
* Prototype 	Static ama group(MCSync data init & callback)
**************************************************************************************************/
#ifdef PROFILE_AMA_ENABLE
static void app_mcsync_ama_data_sync_callback(U16 length, U8 *p_data);
#endif
/**************************************************************************************************
* Prototype 	Static  audio group(MCSync data init & callback)
**************************************************************************************************/
static void app_mcsync_audio_vprt_is_enable_callback(U16 length, U8 *p_data);
static void app_mcsync_audio_anc_passthru_callback(U16 length, U8 *p_data);
static void app_mcsync_audio_vprt_sync_callback(U16 length, U8 *p_data);
static void app_mcsync_audio_stop_vprt_callback(U16 length, U8 *p_data);
static void app_mcsync_audio_vprt_play_later_callback(U16 length, U8 *p_data);
static void app_mcsync_audio_peq_realtime_callback(U16 length, U8 *p_data);
static void app_mcsync_audio_vp_lang_index_callback(U16 length, U8 *p_data);
static void app_mcsync_audio_dsp_callback(U16 length, U8 *p_data);
static void app_mcsync_audio_speaker_mute_callback(U16 length, U8 *p_data);
#ifdef MTK_LEAKAGE_DETECTION_ENABLE
static void app_mcsync_audio_leakage_detect_info_callback(U16 length, U8 *p_data);
#endif
/**************************************************************************************************
* Prototype 	Static sys group(MCSync data init & callback)
**************************************************************************************************/
static void app_mcsync_sys_key_action_sync_callback(U16 length, U8 *p_data);
static void app_mcsync_sys_device_name_sync_callback(U16 length, U8 *p_data);
static void app_mcsync_sys_led_sync_callback(U16 length, U8 *p_data);
static void app_mcsync_sys_bdaddr_sync_callback(U16 length, U8 *p_data);
static void app_mcsync_sys_poweroff_sync_callback(U16 length, U8 *p_data);
static void app_mcsync_sys_fcd_reset_sync_callback(U16 length, U8 *p_data);
static void app_mcsync_sys_battery_sync_callback(U16 length, U8 *p_data);
static void app_mcsync_sys_clear_link_key_sync_callback(U16 length, U8 *p_data);

/**************************************************************************************************
* Prototype 	Static pph group(MCSync data init & callback)
**************************************************************************************************/
static void app_mcsync_pph_smt_chg_cse_sync_callback(U16 length, U8 *p_data);
static void app_mcsync_pph_keycode_action_sync_callback(U16 length, U8 *p_data);
static void app_mcsync_pph_smart_charger_case_state_sync_callback(U16 length, U8 *p_data);
static void app_mcsync_pph_in_ear_detection_status_sync_callback(U16 length, U8 *p_data);


/**************************************************************************************************
* Prototype 	Static newton group(MCSync data init & callback)
**************************************************************************************************/
static void app_mcsync_newton_airokey_noise_sync_callback(U16 length, U8 *p_data);
static void app_mcsync_newton_airoha_key_sync_callback(U16 length, U8 *p_data);
static void app_mcsync_newton_airokey_setting_sync_callback(U16 length, U8 *p_data);

/**************************************************************************************************
* Prototype 	Static reserved group 1(MCSync data init & callback)
**************************************************************************************************/
static void app_mcsync_reserved1_group_ae_general_data_sync_callback(U16 length, U8 *p_data);

/**************************************************************************************************
*Variables
**************************************************************************************************/

const MODULE_TABLE_T gAppMCSyncIFDataBtCbTable[MCSYNC_MODULE_BT_GROUP_NUM] =
{
    {MCSYNC_MODULE_BT_STATE, 					app_mcsync_bt_state_sync_callback},
    {MCSYNC_MODULE_BT_AGNUM, 					app_mcsync_bt_ag_num_sync_callback},
    {MCSYNC_MODULE_BT_CALLER_ID, 				app_mcsync_bt_caller_id_sync_callback},
    {MCSYNC_MODULE_BT_RHO_PARA, 				app_mcsync_bt_rho_para_sync_callback},
    {MCSYNC_MODULE_BT_RHO_STATE, 				app_mcsync_bt_rho_state_sync_callback},
    {MCSYNC_MODULE_BT_LINK_LIST, 				app_mcsync_bt_link_list_sync_callback},
    {MCSYNC_MODULE_BT_BLE_LINK_LIST,            app_mcsync_bt_ble_link_list_sync_callback},
    {MCSYNC_MODULE_BT_RECONNECT_STATE, 			app_mcsync_bt_reconnect_state_sync_callback},
    {MCSYNC_MODULE_BT_RECONNECT_MASK, 			app_mcsync_bt_reconnect_mask_sync_callback},
    {MCSYNC_MODULE_BT_RETRANSMIT_FOLLOWER_DISC, app_mcsync_bt_retransmit_follower_disc_sync_callback},
    {MCSYNC_MODULE_BT_L2CAP_DATA,               app_mcsync_bt_l2cap_data_callback},
<<<<<<< HEAD
=======
	{MCSYNC_MODULE_BT_TAKE_OVER_STATE, 			app_mcsync_bt_take_over_callback},
    {MCSYNC_MODULE_BT_NOTICE_MASTER_PARTNER_TO_SHARE_MODE, app_mcsync_bt_notice_master_partner_to_share_mode_sync_callback},
>>>>>>> db20e11 (second commit)
};

const MODULE_TABLE_T gAppMCSyncIFDataGvaCbTable[MCSYNC_MODULE_GVA_GROUP_NUM] =
{
    {MCSYNC_MODULE_GVA_GFP,                     app_mcsync_gva_gfp_sync_callback},
};

#ifdef PROFILE_AMA_ENABLE
const MODULE_TABLE_T gAppMCSyncIFDataAmaCbTable[MCSYNC_MODULE_AMA_GROUP_NUM] =
{
    {MCSYNC_MODULE_AMA_DATA,                    app_mcsync_ama_data_sync_callback},
};
#endif

<<<<<<< HEAD
=======
#ifdef AIR_TILE_ADV_ENABLE
const MODULE_TABLE_T gAppMCSyncIFDataTileCbTable[MCSYNC_MODULE_TILE_GROUP_NUM] =
{
    {MCSYNC_MODULE_TILE_DATA,                    app_mcsync_tile_data_sync_callback},
};
#endif

>>>>>>> db20e11 (second commit)
const MODULE_TABLE_T gAppMCSyncIFDataAudioCbTable[MCSYNC_MODULE_AUDIO_GROUP_NUM] =
{
    {MCSYNC_MODULE_AUDIO_VP_IS_ENABLE,          app_mcsync_audio_vprt_is_enable_callback},
    {MCSYNC_MODULE_AUDIO_ANC_PASSTHRU,          app_mcsync_audio_anc_passthru_callback},
    {MCSYNC_MODULE_AUDIO_VPRT,                  app_mcsync_audio_vprt_sync_callback},
    {MCSYNC_MODULE_AUDIO_STOP_VPRT,             app_mcsync_audio_stop_vprt_callback},
    {MCSYNC_MODULE_AUDIO_VPRT_PLAY_LATER,       app_mcsync_audio_vprt_play_later_callback},
    {MCSYNC_MODULE_AUDIO_PEQ_REALTIME,          app_mcsync_audio_peq_realtime_callback},
    {MCSYNC_MODULE_AUDIO_VP_LANG_INDEX,         app_mcsync_audio_vp_lang_index_callback},
    {MCSYNC_MODULE_AUDIO_DSP,                   app_mcsync_audio_dsp_callback},
    {MCSYNC_MODULE_AUDIO_SPEAKER_MUTE,          app_mcsync_audio_speaker_mute_callback},
#ifdef MTK_LEAKAGE_DETECTION_ENABLE
    {MCSYNC_MODULE_AUDIO_LEAKAGE_DETECT_INFO,   app_mcsync_audio_leakage_detect_info_callback},
#else
    {MCSYNC_MODULE_AUDIO_LEAKAGE_DETECT_INFO,   NULL},
#endif
};

const MODULE_TABLE_T gAppMCSyncIFDataSysCbTable[MCSYNC_MODULE_SYS_GROUP_NUM] =
{
    {MCSYNC_MODULE_SYS_KEY_ACTION,              app_mcsync_sys_key_action_sync_callback},
    {MCSYNC_MODULE_SYS_DEVICE_NAME,             app_mcsync_sys_device_name_sync_callback},
    {MCSYNC_MODULE_SYS_LED,                     app_mcsync_sys_led_sync_callback},
    {MCSYNC_MODULE_SYS_BDADDR,                  app_mcsync_sys_bdaddr_sync_callback},
    {MCSYNC_MODULE_SYS_POWEROFF,                app_mcsync_sys_poweroff_sync_callback},
    {MCSYNC_MODULE_SYS_FCD_RESET,               app_mcsync_sys_fcd_reset_sync_callback},
    {MCSYNC_MODULE_SYS_BATTERY,                 app_mcsync_sys_battery_sync_callback},
    {MCSYNC_MODULE_SYS_CLEAR_LINK_KEY,          app_mcsync_sys_clear_link_key_sync_callback},
};

const MODULE_TABLE_T gAppMCSyncIFDataPPHCbTable[MCSYNC_MODULE_PPH_GROUP_NUM] =
{
    {MCSYNC_MODULE_PPH_SMT_CHG_CSE,             app_mcsync_pph_smt_chg_cse_sync_callback},
    {MCSYNC_MODULE_PPH_KEYCODE_ACTION,          app_mcsync_pph_keycode_action_sync_callback},
    {MCSYNC_MODULE_PPH_SMART_CHARGER_CASE_STATE, app_mcsync_pph_smart_charger_case_state_sync_callback},
    {MCSYNC_MODULE_PPH_IN_EAR_DETECTION_STATUS, app_mcsync_pph_in_ear_detection_status_sync_callback},
};

const MODULE_TABLE_T gAppMCSyncIFDataNewtonCbTable[MCSYNC_MODULE_NEWTON_GROUP_NUM] =
{
    {MCSYNC_MODULE_NEWTON_AIROKEY_NOISE,        app_mcsync_newton_airokey_noise_sync_callback},
    {MCSYNC_MODULE_NEWTON_AIROHA_KEY,           app_mcsync_newton_airoha_key_sync_callback},
    {MCSYNC_MODULE_NEWTON_AIROKEY_SETTING,      app_mcsync_newton_airokey_setting_sync_callback},
};

const MODULE_TABLE_T gAppMCSyncIFDataReserved1GroupCbTable[MCSYNC_MODULE_RESERVED1_GROUP_NUM] =
{
	{MCSYNC_MODULE_RESERVED1_GROUP_AE_GENEREAL_DATA, app_mcsync_reserved1_group_ae_general_data_sync_callback},
};

/**************************************************************************************************
* Static bt group(MCSync data init & callback)
**************************************************************************************************/
static void app_mcsync_bt_state_sync_callback(U16 length, U8 *p_data)
{
	APP_MCSYNC_DATA_STATE_T msg;
	FW_Memcpy(&msg, p_data, sizeof(APP_MCSYNC_DATA_STATE_T));
	APP_MCSync_SyncAgentStateIndHandler(msg.state);
	UNUSED(length);
}

static void app_mcsync_bt_ag_num_sync_callback(U16 length, U8 *p_data)
{
    APP_MCSYNC_DATA_NUM_AG_T *ind = (APP_MCSYNC_DATA_NUM_AG_T *)p_data;
    U8 oldNumOfAg = APP_GetAGNum();
<<<<<<< HEAD
    DBG_LOG_APP_MCSYNC( "[APP_MCSync][Partner] : Sync Num Of AG, numOfAG(%d), a2dpState:%d", 2, ind->numOfAG, ind->a2dpState);
=======

	#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
	U8 shareMode = BtMCSync_GetShareMode();
	#endif

	#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
	DBG_LOG_APP_MCSYNC( "[APP_MCSync][Partner] : Sync Num Of AG, numOfAG(%d), a2dpState:%d, share mode:%d", 3, ind->numOfAG, 
		ind->a2dpState, shareMode);
	#else
	DBG_LOG_APP_MCSYNC( "[APP_MCSync][Partner] : Sync Num Of AG, numOfAG(%d), a2dpState:%d", 2, ind->numOfAG, ind->a2dpState);
	#endif
>>>>>>> db20e11 (second commit)

	APP_SetAGNum(ind->numOfAG);
	APP_Conn_NotifyPartnerMediaEvt(oldNumOfAg, ind->numOfAG, (ind->a2dpState == APP_MCSYNC_A2DP_STATE_PLAYING) ? 1 : 0);
	APP_RhoDomn_MonitorTimerHandler();
    UNUSED(length);
<<<<<<< HEAD
=======

	#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
	if(shareMode != MCSYNC_SHARE_MODE_DISABLE)
	{
		return;
	}
	
	#endif
	if(BtAwsMce_IsDefaultRolePartner())
    {
        if(APP_GetAGNum() > 0) //ag + phone --> partner connected
        {
            APP_Conn_ReleaseTimer(TIMER_ID_DISCONNECTED_VP);
            if(!APP_Conn_CheckTimer(TIMER_ID_SLC_CONNECTED_VP))
            {
            	APP_Conn_SetTimer(TIMER_ID_SLC_CONNECTED_VP, TIMER_PARTNER_AG_NUM_IF_SLC_TIMER);
                //APP_Media_PushMediaEvent(MEDIA_EVT_SLC_CONNECTED);
            }
        }
        else
        {
            if(APP_Conn_CheckTimer(TIMER_ID_DISCONNECTED_VP))
            {
                APP_Conn_ReleaseTimer(TIMER_ID_DISCONNECTED_VP);
                APP_Conn_SetTimer(TIMER_ID_DISCONNECTED_VP, 3500);
            }
        }
    }
>>>>>>> db20e11 (second commit)
}

static void app_mcsync_bt_caller_id_sync_callback(U16 length, U8 *p_data)
{
	UNUSED(length); UNUSED(p_data);
}

static void app_mcsync_bt_rho_para_sync_callback(U16 length, U8 *p_data)
{
    UNUSED(length);
    APP_MCSYNC_DATA_RHO_PARA_T msg;
    FW_Memcpy(&msg, p_data, sizeof(APP_MCSYNC_DATA_RHO_PARA_T));
    APP_MCSync_SyncRhoParaInfoIndHandler(msg.batPercent, msg.rssi);
}

static void app_mcsync_bt_rho_state_sync_callback(U16 length, U8 *p_data)
{
	APP_MCSYNC_DATA_RHO_STATE_T msg;
	FW_Memcpy(&msg, p_data, sizeof(APP_MCSYNC_DATA_RHO_STATE_T));
	APP_MCSync_SyncRhoStateIndHandler(msg.state);
	UNUSED(length);
}

static void app_mcsync_bt_link_list_sync_callback(U16 length, U8 *p_data)
{
    APP_Mcsync_SyncLinkKeyInd(length, (U8*)p_data);
}

static void app_mcsync_bt_ble_link_list_sync_callback(U16 length, U8 *p_data)
{    
    APP_Mcsync_SyncBleLinkKeyInd(length, p_data);
}

static void app_mcsync_bt_reconnect_state_sync_callback(U16 length, U8 *p_data)
{
    UNUSED(length);
    APP_MCSYNC_DATA_RECONNECT_STATE_T msg;
    FW_Memcpy(&msg, p_data, sizeof(APP_MCSYNC_DATA_RECONNECT_STATE_T));
    APP_MCSync_ReconnectStateIndHandler(msg.type, &msg.bdAddr);
}

static void app_mcsync_bt_reconnect_mask_sync_callback(U16 length, U8 *p_data)
{
    UNUSED(length);
    APP_MCSYNC_DATA_RECONNECT_MASK_T msg;
    FW_Memcpy(&msg, p_data, sizeof(APP_MCSYNC_DATA_RECONNECT_MASK_T));
    APP_MCSync_ReconnectMaskIndHandler(msg.notReconnMask);
}

static void app_mcsync_bt_retransmit_follower_disc_sync_callback(U16 length, U8 *p_data)
{
	
<<<<<<< HEAD
	#ifdef MCSYNC_SHARE_MODE
=======
	#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
	UNUSED(length);
    APP_MCSYNC_DATA_FOLLOWER_DISC_T msg;
    FW_Memcpy(&msg, p_data, sizeof(APP_MCSYNC_DATA_FOLLOWER_DISC_T));
	APP_MCSync_Share_SyncFollowerDiscStatusHandler(msg.isFollowerIFPkt, msg.isOnlyForFollower);
	#else
	UNUSED(length); UNUSED(p_data);
	#endif
}

extern BOOL StreamL2capNewAgentFlush(U8 *req);
static void app_mcsync_bt_l2cap_data_callback(U16 length, U8 *p_data)
{
    UNUSED(length);
    DBG_LOG_APP_MCSYNC("[APP_MCSync][Partner] : app_mcsync_bt_l2cap_data_callback(0x%x), %d", 2, p_data, length);
    U8 * ptr = malloc(length);/* alignment issue */
    if(ptr)
    {
        FW_Memcpy(ptr, p_data, length); /* alignment issue */
        StreamL2capNewAgentFlush(ptr);
        free(ptr);
    }
}

<<<<<<< HEAD
=======
static void app_mcsync_bt_take_over_callback(U16 length, U8 *p_data)
{
	#ifdef AIR_TAKE_OVER_LINK_ENABLE
	APP_MCSYNC_TAKE_OVER_STATE_T msg;
	U8 oldState = BtMCSync_GetMcsyncTakeOverConnState();
	DBG_LOG_APP_MCSYNC("[APP_MCSync][Partner] : app_mcsync_bt_take_over_callback, %d %d", 2, p_data[0], length);

    FW_Memcpy(&msg, p_data, sizeof(APP_MCSYNC_TAKE_OVER_STATE_T));
	DBG_LOG_APP_MCSYNC("[APP_MCSync][Partner] : take over callback, old state:%d, new state:%d", 2, 
		oldState, msg.takeOverState);

	BtMCSync_SetMcsyncTakeOverConnState(msg.takeOverState);

	if(oldState == MCSYNC_TAKE_OVER_CONNECTION_TAKE_OVER_COMPLETE_STATE 
		&& msg.takeOverState == MCSYNC_TAKE_OVER_CONNECTION_IDLE_STATE)
	{
		APP_HFP_StopIncomingVPRing((BD_ADDR_T*)NULL);
	}
	UNUSED(length);
	#else
	UNUSED(length); UNUSED(p_data);
	#endif
}

static void app_mcsync_bt_notice_master_partner_to_share_mode_sync_callback(U16 length, U8 *p_data)
{
	#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
	APP_MCSYNC_NOTICE_MASTER_PARTNER_TO_ENTER_SHARE_MODE_T msg;

    FW_Memcpy(&msg, p_data, sizeof(APP_MCSYNC_NOTICE_MASTER_PARTNER_TO_ENTER_SHARE_MODE_T));

	DBG_LOG_APP_MCSYNC( "[APP_MCSync][Share] master_partner_to_share_mode_sync_callback", 1, msg.enterShareMode);

	if(APP_MCSync_Share_IsEnable() == APP_SHARE_MODE_FOLLOWER)
	{
		if(BtAwsMce_IsDefaultRolePartner())
		{
			APP_MCSync_Share_SetMode(msg.enterShareMode);
		}
	
		return;
	}

	if(BtAwsMce_IsDefaultRolePartner())
	{
		APP_MCSync_Share_SetMode(msg.enterShareMode);
	}
	
	#else
	UNUSED(length); UNUSED(p_data);
	#endif
}

>>>>>>> db20e11 (second commit)
/**************************************************************************************************
* Static gva group(MCSync data init & callback)
**************************************************************************************************/
static void app_mcsync_gva_gfp_sync_callback(U16 length, U8 *p_data)
{
<<<<<<< HEAD
    UNUSED(length);
    APP_MCSYNC_GFP_INFO_STRU *msg = NULL;
    msg = (APP_MCSYNC_GFP_INFO_STRU*)FW_GetMemory(length);
    if(msg){
        FW_Memcpy(msg, p_data, length);
        APP_MCSync_SyncGFPInfoIndHandler(&msg->data[0], msg->length);
        FW_FreeMemory(msg);
    }
=======
#ifdef AIR_GFP_ENABLE
    APP_Gfp_McsyncDataHandle(length, p_data);
#endif
>>>>>>> db20e11 (second commit)
}


/**************************************************************************************************
*Static ama group(MCSync data init & callback)
**************************************************************************************************/
#ifdef PROFILE_AMA_ENABLE
static void app_mcsync_ama_data_sync_callback(U16 length, U8 *p_data)
{
    APP_Mcsync_SyncAmaDataInd(length, p_data);
}
#endif

/**************************************************************************************************
<<<<<<< HEAD
=======
*Static Tile group(MCSync data init & callback)
**************************************************************************************************/
#ifdef AIR_TILE_ADV_ENABLE
static void app_mcsync_tile_data_sync_callback(U16 length, U8 *p_data)
{
    //APP_Mcsync_SyncAmaDataInd(length, p_data);
    UNUSED(length);
    APP_MCSYNC_TILE_INFO_STRU *msg = NULL;
    msg = (APP_MCSYNC_TILE_INFO_STRU*)FW_GetMemory(length);
    if (msg) {
        FW_Memcpy(msg, p_data, length);
        APP_MCSync_SyncTileInfoIndHandler(msg->opcode, msg->streamState);
        FW_FreeMemory(msg);
    }
}
#endif

/**************************************************************************************************
>>>>>>> db20e11 (second commit)
* Static audio group (MCSync data init & callback)
**************************************************************************************************/
static void app_mcsync_audio_vprt_is_enable_callback(U16 length, U8 *p_data)
{
	APP_MCSYNC_DATA_VP_IS_ENABLE_T msg;
	FW_Memcpy(&msg, p_data, sizeof(APP_MCSYNC_DATA_VP_IS_ENABLE_T));
	APP_MCSync_SyncAgentVPIsEnableIndHandler(msg.isEnable);
	UNUSED(length);
}

static void app_mcsync_audio_anc_passthru_callback(U16 length, U8 *p_data)
{
	APP_MCSYNC_DATA_ANC_T msg;
	FW_Memcpy(&msg, p_data, sizeof(APP_MCSYNC_DATA_ANC_T));
	//APP_MCSync_SyncAgentANCPassThruIndHandler(msg.userRegister, msg.enable, msg.flash_no, msg.type, msg.runtime_gain, msg.clockToStart);

	AudioDsp_ANCPassThruSlaveSyncHandler(msg.userRegister, msg.enable, msg.flash_no, msg.type, msg.runtime_gain, msg.clockToStart);

	UNUSED(length);
}

static void app_mcsync_audio_vprt_sync_callback(U16 length, U8 *p_data)
{
	UNUSED(length); UNUSED(p_data);
}

static void app_mcsync_audio_stop_vprt_callback(U16 length, U8 *p_data)
{
	UNUSED(length); UNUSED(p_data);
}

static void app_mcsync_audio_vprt_play_later_callback(U16 length, U8 *p_data)
{
	UNUSED(length); UNUSED(p_data);
}

static void app_mcsync_audio_peq_realtime_callback(U16 length, U8 *p_data)
{
	UNUSED(length); UNUSED(p_data);
}

static void app_mcsync_audio_vp_lang_index_callback(U16 length, U8 *p_data)
{
    APP_MCSYNC_DATA_VP_INDEX_T *ind = (APP_MCSYNC_DATA_VP_INDEX_T *)p_data;
	UNUSED(length);
    APP_VpSetLanguage(ind->index);
}

static void app_mcsync_audio_dsp_callback(U16 length, U8 *p_data)
{
	UNUSED(length); UNUSED(p_data);
}

static void app_mcsync_audio_speaker_mute_callback(U16 length, U8 *p_data)
{
	APP_MCSYNC_DATA_SPKER_MUTE_T *ind = (APP_MCSYNC_DATA_SPKER_MUTE_T *)p_data;
	UNUSED(length);

	DBG_LOG_APP_MCSYNC("[APP_MCSync][Partner] : Sync Spkr Mute, isMute(%d)", 1, ind->isMute);

	if(ind->isMute)
	{
		APP_VolSetting_SetSpeakerMuteOn(SPKR_MUTE_MASK_PARTNER_USE);
	}
	else
	{
		APP_VolSetting_SetSpeakerMuteOff(SPKR_MUTE_MASK_PARTNER_USE);
	}
}

#ifdef MTK_LEAKAGE_DETECTION_ENABLE
static void app_mcsync_audio_leakage_detect_info_callback(U16 length, U8 *p_data)
{
	UNUSED(length); UNUSED(p_data);
}
#endif

/**************************************************************************************************
* Static sys group(MCSync data init & callback)
**************************************************************************************************/

static void app_mcsync_sys_key_action_sync_callback(U16 length, U8 *p_data)
{
	AWSMCE_INTERNAL_KEY_CMD_T msg;
	FW_Memcpy(&msg, p_data, sizeof(AWSMCE_INTERNAL_KEY_CMD_T));
	APP_MCSync_SyncPartnerKeyIndHandler(msg.keyEventCode, msg.supportedState);
	UNUSED(length);
}

static void app_mcsync_sys_device_name_sync_callback(U16 length, U8 *p_data)
{
	UNUSED(length); UNUSED(p_data);
}

static void app_mcsync_sys_led_sync_callback(U16 length, U8 *p_data)
{
	APP_MCSYNC_LED_STRU msg;
	FW_Memcpy(&msg, p_data, sizeof(APP_MCSYNC_LED_STRU));
	APP_MCSync_SyncLEDIndHandler(msg.patternIndex, (U8*)&msg.patternType[0],
		msg.overwriteTime, msg.isOverwrite, msg.clockToStart, msg.action);

	UNUSED(length);
}

static void app_mcsync_sys_bdaddr_sync_callback(U16 length, U8 *p_data)
{
    APP_MCSYNC_DATA_PARTNER_BDADDR_T *ind = (APP_MCSYNC_DATA_PARTNER_BDADDR_T *)p_data;
    APP_AgentSavePartnerInfo(&ind->partnerBdAddr);
    BtAwsMce_AgentSavePartnerBdAddr(&ind->partnerBdAddr);
	UNUSED(length);
}

static void app_mcsync_sys_poweroff_sync_callback(U16 length, U8 *p_data)
{
    UNUSED(length);
    APP_MCSYNC_DATA_POWEROFF_T msg;
    FW_Memcpy(&msg, p_data, sizeof(APP_MCSYNC_DATA_POWEROFF_T));
    APP_MCSync_SyncPowerOffIndHandler(msg.reason, msg.isFollowerIFPKT, msg.isOnlyForFollower);
}

static void app_mcsync_sys_fcd_reset_sync_callback(U16 length, U8 *p_data)
{
	UNUSED(length);
	APP_MCSYNC_DATA_FCD_RESET_T *ind = (APP_MCSYNC_DATA_FCD_RESET_T *)p_data;

	if(!BtAwsMce_IsDefaultRoleAgent())
	{
		APP_FCD_ResetToFactory(ind->type);
	}
}

static void app_mcsync_sys_battery_sync_callback(U16 length, U8 *p_data)
{
	UNUSED(length); UNUSED(p_data);
}

static void app_mcsync_sys_clear_link_key_sync_callback(U16 length, U8 *p_data)
{
	if(BtAwsMce_IsDefaultRolePartner())
	{
		APP_LinkKey_ClearAllHistory();
	}
    UNUSED(length); UNUSED(p_data);
}

/**************************************************************************************************
* Static pph group(MCSync data init & callback)
**************************************************************************************************/
static void app_mcsync_pph_smt_chg_cse_sync_callback(U16 length, U8 *p_data)
{
    UNUSED(length);
    AWSMCE_INTERNAL_SMTCHGCSE_INFO_T msg;
    FW_Memcpy(&msg, p_data, sizeof(AWSMCE_INTERNAL_SMTCHGCSE_INFO_T));
    APP_MCSync_SyncSmtChgCseInfoIndHandler(msg.isTimerSet);
}

static void app_mcsync_pph_keycode_action_sync_callback(U16 length, U8 *p_data)
{
    UNUSED(length);
    APP_MCSYNC_DATA_KEYCODE_T msg;
    FW_Memcpy(&msg, p_data, sizeof(APP_MCSYNC_DATA_KEYCODE_T));
    APP_MCSync_SyncKeyCodeInfoIndHandler(msg.keyCode);
}

static void app_mcsync_pph_smart_charger_case_state_sync_callback(U16 length, U8 *p_data)
{
	APP_MCSYNC_DATA_SMART_CASE_STATE_T msg;
	FW_Memcpy(&msg, p_data, sizeof(APP_MCSYNC_DATA_SMART_CASE_STATE_T));
	APP_MCSync_SyncSmartChargeCaseStateIndHandler(msg.state);
	UNUSED(length);
}

static void app_mcsync_pph_in_ear_detection_status_sync_callback(U16 length, U8 *p_data)
{
    UNUSED(length);
    APP_MCSYNC_DATA_IN_EAR_DETECTION_T msg;
    FW_Memcpy(&msg, p_data, sizeof(APP_MCSYNC_DATA_IN_EAR_DETECTION_T));
    APP_MCSync_InEarDetectionStatusIndHandler(msg.status);
}


/**************************************************************************************************
*Static newton group(MCSync data init & callback)
**************************************************************************************************/
static void app_mcsync_newton_airokey_noise_sync_callback(U16 length, U8 *p_data)
{
	UNUSED(length); UNUSED(p_data);
}

static void app_mcsync_newton_airoha_key_sync_callback(U16 length, U8 *p_data)
{
	UNUSED(length); UNUSED(p_data);
}

static void app_mcsync_newton_airokey_setting_sync_callback(U16 length, U8 *p_data)
{
	UNUSED(length); UNUSED(p_data);
}

/**************************************************************************************************
* Prototype 	Static reserved group 1(MCSync data init & callback)
**************************************************************************************************/
static void app_mcsync_reserved1_group_ae_general_data_sync_callback(U16 length, U8 *p_data)
{
	UNUSED(length); UNUSED(p_data);
}
/**************************************************************************************************
*Static common (MCSync data init & callback)
**************************************************************************************************/


/**************************************************************************************************
*Public API
**************************************************************************************************/
void APP_MCSync_Data_Callback_Init(void)
{
	bt_mcsync_register_module((MODULE_TABLE_T*)&gAppMCSyncIFDataBtCbTable[0],
		MCSYNC_MODULE_BT_GROUP_NUM, MCSYNC_MODULE_GROUP_BT);

	bt_mcsync_register_module((MODULE_TABLE_T*)&gAppMCSyncIFDataGvaCbTable[0],
		MCSYNC_MODULE_GVA_GROUP_NUM, MCSYNC_MODULE_GROUP_GVA);

	bt_mcsync_register_module((MODULE_TABLE_T*)&gAppMCSyncIFDataAudioCbTable[0],
		MCSYNC_MODULE_AUDIO_GROUP_NUM, MCSYNC_MODULE_GROUP_AUDIO);

#ifdef PROFILE_AMA_ENABLE
	bt_mcsync_register_module((MODULE_TABLE_T*)&gAppMCSyncIFDataAmaCbTable[0],
		MCSYNC_MODULE_AMA_GROUP_NUM, MCSYNC_MODULE_GROUP_AMA);
#endif

	bt_mcsync_register_module((MODULE_TABLE_T*)&gAppMCSyncIFDataSysCbTable[0],
		MCSYNC_MODULE_SYS_GROUP_NUM, MCSYNC_MODULE_GROUP_SYS);

	bt_mcsync_register_module((MODULE_TABLE_T*)&gAppMCSyncIFDataPPHCbTable[0],
		MCSYNC_MODULE_PPH_GROUP_NUM, MCSYNC_MODULE_GROUP_PPH);

	bt_mcsync_register_module((MODULE_TABLE_T*)&gAppMCSyncIFDataNewtonCbTable[0],
		MCSYNC_MODULE_NEWTON_GROUP_NUM, MCSYNC_MODULE_GROUP_NEWTON);

	bt_mcsync_register_module((MODULE_TABLE_T*)&gAppMCSyncIFDataReserved1GroupCbTable[0],
		MCSYNC_MODULE_RESERVED1_GROUP_NUM, MCSYNC_MODULE_GROUP_RESERVED1);

<<<<<<< HEAD
=======
#ifdef AIR_TILE_ADV_ENABLE
	bt_mcsync_register_module((MODULE_TABLE_T*)&gAppMCSyncIFDataTileCbTable[0],
		MCSYNC_MODULE_TILE_GROUP_NUM, MCSYNC_MODULE_GROUP_RESERVED3);
#endif

>>>>>>> db20e11 (second commit)
	bt_mcsync_data_init();
}


