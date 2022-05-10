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
#include "Peq_NvkeyHandler.h"
#include "App_PeqProcess.h"
#include "A2dp_Interface.h"
#include "BtMCSync.h"
#include "App_ResourceManager.h"
#include "App_PeqManage.h"
#include "App_State.h"
#include "App_VolumeManage.h"
#include "App_Hfp.h"
#include "drv_a2dp.h"
#include "App_MCSync.h"
#include "App_MCSync_Data.h"
<<<<<<< HEAD
=======
#include "App_MCSync_Share.h"
>>>>>>> db20e11 (second commit)

/*************************************************************************************************
* Prototype
**************************************************************************************************/
<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
static void app_mcsync_SendSyncKeyInfo(U16 keyEventCode, U32 supportedState);
#endif
static void app_mcsync_SendSyncStopVpRtInfo(U16 eventIndex, U32 currentPicoClock);
static void app_mcsync_SendSyncVpRtPlayLater(U16 eventIndex, U32 clockToStart);
static void app_mcsync_SendSyncCallIdInfo(U8 *pCallerId, U8 length);
static void app_mcsync_SendPEQRealTimeInfo(U8 *pData, U16 dataLength, U32 clockToStart);
static void app_mcsync_SendSyncBdAddrInfo(void);
static void app_mcsync_SendVPLangIndex(U8 index);
static void app_mcsync_SendSyncRhoParaInfo(U16 batPercent, S8 rssi);
static void app_mcsync_SendSyncSmtChgCseInfo(BOOL isTimerSet);
static void app_mcsync_SendSyncInEarDetectionStatus(U8 status);
static void app_mcsync_SendSyncPowerOff(U8 reason, BOOL isFollowerIFPKT, BOOL isOnlyForFollower);
static void app_mcsync_SendSyncKeyCodeInfo(U16 keyCode);
static void app_mcsync_SendSyncSpeakerMute(BOOL isMute);
static void app_mcsync_SendReconnectMask(U8 notReconnMask);
static void app_mcsync_SendReconnectState(U8 type, BD_ADDR_T *pBdAddr);
static void app_mcsync_SendLinkList(U8 * pData, U32 length);
<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
static void app_mcsync_SendSyncShareModeInfo(U8 state, U8 shareMode);
static void app_mcsync_SendSyncFollowerSoundLevel(U8 soundLevel, U8 isFollowerIFPkt);
static void app_mcsync_SendOtherAgentBDA(BD_ADDR_T *pBdAddr);
#endif

/*************************************************************************************************
* Static function
**************************************************************************************************/
<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
static void app_mcsync_SendSyncKeyInfo(U16 keyEventCode, U32 supportedState)
{
	BtMCSync_SendSyncKeyInfo(keyEventCode, supportedState);
}
#endif
static void app_mcsync_SendSyncKeyCodeInfo(U16 keyCode)
{
    APP_MCSYNC_DATA_KEYCODE_T msg;
    msg.keyCode = keyCode;
    bt_mcsync_send_data_by_module_id(MCSYNC_MODULE_PPH_KEYCODE_ACTION, sizeof(APP_MCSYNC_DATA_KEYCODE_T), (U8*)&msg);
}

static void app_mcsync_SendSyncStopVpRtInfo(U16 eventIndex, U32 currentPicoClock)
{
	BtMCSync_SendSyncStopVpRtInfo(eventIndex, currentPicoClock);
}

static void app_mcsync_SendSyncVpRtPlayLater(U16 eventIndex, U32 clockToStart)
{
	BtMCSync_SendSyncVpRtPlayLater(eventIndex, clockToStart);
}

static void app_mcsync_SendSyncCallIdInfo(U8 *pCallerId, U8 length)
{
	BtMCSync_SendSyncCallIdInfo(pCallerId, length);
}

static void app_mcsync_SendPEQRealTimeInfo(U8 *pData, U16 dataLength, U32 clockToStart)
{
	BtMCSync_SendSyncPEQRealTimeInfo(pData, dataLength, clockToStart);
}

static void app_mcsync_SendSyncBdAddrInfo(void)
{
    APP_MCSYNC_DATA_PARTNER_BDADDR_T msg;
	FW_Memcpy(&msg.partnerBdAddr, BtAwsMce_ReadLocalBdAddr(), sizeof(BD_ADDR_T));
    bt_mcsync_send_data_by_module_id(MCSYNC_MODULE_SYS_BDADDR, sizeof(APP_MCSYNC_DATA_PARTNER_BDADDR_T), (U8*)&msg);
}

static void app_mcsync_SendVPLangIndex(U8 index)
{
    APP_MCSYNC_DATA_VP_INDEX_T msg;
    msg.index = index;
    bt_mcsync_send_data_by_module_id(MCSYNC_MODULE_AUDIO_VP_LANG_INDEX, sizeof(APP_MCSYNC_DATA_VP_INDEX_T), (U8*)&msg);
}

static void app_mcsync_SendReconnectMask(U8 notReconnMask)
{
    APP_MCSYNC_DATA_RECONNECT_MASK_T msg;
    msg.notReconnMask = notReconnMask;
    bt_mcsync_send_data_by_module_id(MCSYNC_MODULE_BT_RECONNECT_MASK, sizeof(APP_MCSYNC_DATA_RECONNECT_MASK_T), (U8*)&msg);
}

static void app_mcsync_SendReconnectState(U8 type, BD_ADDR_T *pBdAddr)
{
    APP_MCSYNC_DATA_RECONNECT_STATE_T msg;
    msg.type = type;
    if(pBdAddr)
        FW_Memcpy((U8 *)&msg.bdAddr, (U8 *)pBdAddr, sizeof(BD_ADDR_T));
    bt_mcsync_send_data_by_module_id(MCSYNC_MODULE_BT_RECONNECT_STATE, sizeof(APP_MCSYNC_DATA_RECONNECT_STATE_T), (U8*)&msg);
}

static void app_mcsync_SendSyncRhoParaInfo(U16 batPercent, S8 rssi)
{
    APP_MCSYNC_DATA_RHO_PARA_T msg;
    msg.batPercent = batPercent;
    msg.rssi = rssi;
    bt_mcsync_send_data_by_module_id(MCSYNC_MODULE_BT_RHO_PARA, sizeof(APP_MCSYNC_DATA_RHO_PARA_T), (U8*)&msg);
}

static void app_mcsync_SendSyncSmtChgCseInfo(BOOL isTimerSet)
{
    AWSMCE_INTERNAL_SMTCHGCSE_INFO_T msg;
    msg.isTimerSet = isTimerSet;
    bt_mcsync_send_data_by_module_id(MCSYNC_MODULE_PPH_SMT_CHG_CSE, sizeof(AWSMCE_INTERNAL_SMTCHGCSE_INFO_T), (U8*)&msg);
}

static void app_mcsync_SendSyncInEarDetectionStatus(U8 status)
{
    APP_MCSYNC_DATA_IN_EAR_DETECTION_T msg;
    msg.status = status;
    bt_mcsync_send_data_by_module_id(MCSYNC_MODULE_PPH_IN_EAR_DETECTION_STATUS, sizeof(APP_MCSYNC_DATA_IN_EAR_DETECTION_T), (U8*)&msg);

}

static void app_mcsync_SendSyncPowerOff(U8 reason, BOOL isFollowerIFPKT, BOOL isOnlyForFollower)
{
 	BtMCSync_SendSyncPowerOff(reason, isFollowerIFPKT, isOnlyForFollower);
}
<<<<<<< HEAD
static void app_mcsync_SendSyncGFPInfo(U8 *pData, U32 length)
{
    APP_MCSYNC_GFP_INFO_STRU msg;
    msg.length = length;
    FW_Memcpy((U8*)&msg.data, (U8*)pData, length);
    bt_mcsync_send_data_by_module_id(MCSYNC_MODULE_GVA_GFP, sizeof(APP_MCSYNC_GFP_INFO_STRU) + length - 1, (U8*)&msg);
}
=======

#ifdef AIR_TILE_ADV_ENABLE
static void app_mcsync_SendSyncTileInfo(BOOL isAgent)
{
    APP_MCSYNC_TILE_INFO_STRU msg;
	BOOL result;
	if (isAgent) {
    	msg.opcode = APP_MCSYNC_TILE_INFO_OPCODE_CMD;
		msg.streamState = 0;
	} else {
    	msg.opcode = APP_MCSYNC_TILE_INFO_OPCODE_RSP;
		msg.streamState = APP_Tile_Get_Partner_StreamState();
	}
    result = bt_mcsync_send_data_by_module_id(MCSYNC_MODULE_TILE_DATA, sizeof(APP_MCSYNC_TILE_INFO_STRU), (U8*)&msg);
	DBG_LOG_APP_MCSYNC( "[APP_MCSync] Send tile info, result is %d, opcode is %d, streamState is %d, lenth is %d", 4,
		result, msg.opcode, msg.streamState, sizeof(APP_MCSYNC_TILE_INFO_STRU));
}
#endif
>>>>>>> db20e11 (second commit)

static void app_mcsync_SendSyncFCDReset(U8 type)
{
/* Agent or Parnter condition before send Sync Data*/
    APP_MCSYNC_DATA_FCD_RESET_T msg;
    msg.type = type;
    bt_mcsync_send_data_by_module_id(MCSYNC_MODULE_SYS_FCD_RESET, sizeof(APP_MCSYNC_DATA_FCD_RESET_T), (U8*)&msg);
}

static void app_mcsync_SendSyncSpeakerMute(BOOL isMute)
{
/* Agent or Parnter condition before send Sync Data*/
    APP_MCSYNC_DATA_SPKER_MUTE_T msg;
    msg.isMute = isMute;
    bt_mcsync_send_data_by_module_id(MCSYNC_MODULE_AUDIO_SPEAKER_MUTE, sizeof(APP_MCSYNC_DATA_SPKER_MUTE_T), (U8*)&msg);
}

static void app_mcsync_SendSyncGeneralData(U8 *pData, U32 length)
{
	BtMCSync_SendSyncGeneralData(pData, length);
}

static void app_mcsync_SendLinkList(U8 * pData, U32 length)
{
    bt_mcsync_send_data_by_module_id(MCSYNC_MODULE_BT_LINK_LIST, length, (U8*)pData);
}

<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
static void app_mcsync_SendSyncShareModeInfo(U8 state, U8 shareMode)
{
	BtMCSync_SendSyncShareModeInfo(state, shareMode);
}

static void app_mcsync_SendSyncFollowerSoundLevel(U8 soundLevel, U8 isFollowerIFPkt)
{
	BtMCSync_SendSyncFollowerSoundLevel(soundLevel, isFollowerIFPkt);
}

static void app_mcsync_SendOtherAgentBDA(BD_ADDR_T *pBdAddr)
{
	BtMCSync_SendSyncOtherAgentBDA(pBdAddr);
}
#endif

/*************************************************************************************************
* Public function
**************************************************************************************************/
/* agent to client*/
void APP_MCSYNC_SendSyncA2dpInfo(BD_ADDR_T *pBdAddr, U8 specificRole)
{
	if(BtAwsMce_IsDefaultRoleAgent())
	{
        U8 gameMode = 0;
        if(drv_a2dp_get_game_mode())
            gameMode |= 0x01;
        #ifdef A2DP_LOW_LATENCY
        DBG_LOG_APP_MCSYNC("[APP_MCSync] APP_MCSYNC_SendSyncA2dpInfo 0x%x%x", 2, FW_bdaddr_to_2U32(pBdAddr,TRUE), FW_bdaddr_to_2U32(pBdAddr,FALSE));
        if(drv_a2dp_get_low_latency_mode(pBdAddr))
            gameMode |= 0x02;
        #endif
<<<<<<< HEAD
=======
        if(drv_a2dp_get_ap_mode())
        {
            gameMode |= 0x04;
        }
>>>>>>> db20e11 (second commit)

		if(PM_IsProfileConnected(pBdAddr, PROFILE_A2DP))
		{
			BtMCSync_SendSyncA2dpInfo(a2dp_get_codec(pBdAddr), APP_VolManage_GetCurrentAbsoluteVolume(pBdAddr), APP_PeqProc_GetPeqGroup(PEQ_A2DP),
				a2dp_get_scms(pBdAddr), gameMode, APP_GetAGNum(), specificRole);
		}
		else
		{
			BtMCSync_SendSyncA2dpInfo(a2dp_get_codec(pBdAddr), APP_VolManage_GetAbsoluteVolumeBySoundLevel(APP_VolManage_GetCurrentA2dpSoundLevel(pBdAddr)), APP_PeqProc_GetPeqGroup(PEQ_A2DP),
				a2dp_get_scms(pBdAddr), gameMode, APP_GetAGNum(), specificRole);
		}
	}
}

void APP_MCSYNC_SendSyncScoInfo(BD_ADDR_T *pBdAddr)
{
	if(BtAwsMce_IsDefaultRoleAgent())
	{
		APP_INFO_STRU * pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);
		BtMCSync_SendSyncScoInfo(APP_VolManage_GetCurrentScoSoundLevel(pBdAddr), APP_VolManage_GetCurrentScoMicSoundLevel(pBdAddr), APP_GetScoState(pLinkInfo), APP_State_GetTopState(pBdAddr));
	}
}

void APP_MCSYNC_SendSyncStateInfo(BD_ADDR_T *pBdAddr)
{
	APP_MCSYNC_DATA_STATE_T msg;
	if(BtAwsMce_IsDefaultRoleAgent())
	{
		msg.state =  APP_State_GetTopState(pBdAddr);
		bt_mcsync_send_data_by_module_id(MCSYNC_MODULE_BT_STATE,
		sizeof(APP_MCSYNC_DATA_STATE_T), (U8*)&msg);
	}
}

void APP_MCSYNC_SendSyncSmartChargeCaseStateInfo(U8 state)
{
	if(BtAwsMce_IsDefaultRolePartner())
	{
<<<<<<< HEAD
		#ifdef MCSYNC_SHARE_MODE
		if(BtMCSync_GetShareMode() == MCSYNC_SHARE_MODE_NORMAL_ENABLE)
=======
		#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
		if(BtMCSync_GetShareMode() == MCSYNC_SHARE_MODE_NORMAL_ENABLE &&
          (APP_MCSync_Share_GetState() == APP_SHARE_STATE_ENTERED_MODE ||
          APP_MCSync_Share_GetState() == APP_SHARE_STATE_PREPARING))
>>>>>>> db20e11 (second commit)
		{
			BtMCSync_SendSyncSmartChargeCaseStateInfo(state);
		}
		else
		#endif
		{
<<<<<<< HEAD
			DBG_LOG_APP_MCSYNC("[APP_MCSync] Partner send case state to Agent (%d)", state);
=======
			DBG_LOG_APP_MCSYNC("[APP_MCSync] Partner send case state to Agent (%d)", 1, state);
>>>>>>> db20e11 (second commit)
			APP_MCSYNC_DATA_SMART_CASE_STATE_T msg;
			msg.state = state;
			bt_mcsync_send_data_by_module_id(MCSYNC_MODULE_PPH_SMART_CHARGER_CASE_STATE,
			sizeof(APP_MCSYNC_DATA_SMART_CASE_STATE_T), (U8*)&msg);
		}
	}
}

void APP_MCSYNC_SendSyncRhoStateInfo(U8 state)
{
	if(BtAwsMce_IsDefaultRoleAgent())
	{
		APP_MCSYNC_DATA_RHO_STATE_T msg;
		msg.state = state;
		bt_mcsync_send_data_by_module_id(MCSYNC_MODULE_BT_RHO_STATE,
			sizeof(APP_MCSYNC_DATA_RHO_STATE_T), (U8*)&msg);
	}
}

void APP_MCSYNC_SendSyncVPInfo(BOOL isEnable)
{
	if(BtAwsMce_IsDefaultRoleAgent())
	{
		APP_MCSYNC_DATA_VP_IS_ENABLE_T msg;
		msg.isEnable = isEnable;
		bt_mcsync_send_data_by_module_id(MCSYNC_MODULE_AUDIO_VP_IS_ENABLE,
			sizeof(APP_MCSYNC_DATA_VP_IS_ENABLE_T), (U8*)&msg);
	}
}

void APP_MCSYNC_SendSyncANCPassThruInfo(U8 userRegister, U8 enable, U8 flash_no, U8 type, S16 runtime_gain, U32 clockToStart)
{
	if(BtAwsMce_IsDefaultRoleAgent())
	{
		APP_MCSYNC_DATA_ANC_T msg;
		msg.userRegister = userRegister;
		msg.enable 		 = enable;
		msg.flash_no 	 = flash_no;
		msg.type		 = type;
		msg.runtime_gain = runtime_gain;
		msg.clockToStart = clockToStart;
<<<<<<< HEAD
=======

		DBG_LOG_APP_MCSYNC("[APP_MCSync] SendSyncANCPassThruInfo userReg:%d, enable:%d, type:%d", 3,
			userRegister, enable, type);

>>>>>>> db20e11 (second commit)
		bt_mcsync_send_data_by_module_id(MCSYNC_MODULE_AUDIO_ANC_PASSTHRU,
			sizeof(APP_MCSYNC_DATA_ANC_T), (U8*)&msg);
	}
}

void APP_MCSYNC_SendSyncNumOfAGInfo(U8 numOfAG)
{
	if(BtAwsMce_IsDefaultRoleAgent())
    {
        APP_MCSYNC_DATA_NUM_AG_T msg;
        U8 a2dpState = APP_MCSYNC_A2DP_STATE_NONE;
        BD_ADDR_T *pBdAddr = BtMCSync_GetNormalLinkBdAddr();
<<<<<<< HEAD
                
        if(pBdAddr && !PM_IsProfileConnected(pBdAddr, PROFILE_MCSYNC) && numOfAG > 0)
            return;
        
=======

        if(pBdAddr && !PM_IsProfileConnected(pBdAddr, PROFILE_MCSYNC) && numOfAG > 0)
            return;

>>>>>>> db20e11 (second commit)
        if(pBdAddr)
        {
            if(a2dp_is_in_streaming(pBdAddr))
            {
                if(BtAvdtpGetIsMediaPlay(pBdAddr))
                    a2dpState = APP_MCSYNC_A2DP_STATE_PLAYING;
                else
                    a2dpState = APP_MCSYNC_A2DP_STATE_PLAYDROP;
            }
            else if(a2dp_is_open(pBdAddr))
            {
                a2dpState = APP_MCSYNC_A2DP_STATE_READY;
            }
        }
		msg.numOfAG = numOfAG;
        msg.a2dpState = a2dpState;
		bt_mcsync_send_data_by_module_id(MCSYNC_MODULE_BT_AGNUM, sizeof(APP_MCSYNC_DATA_NUM_AG_T), (U8*)&msg);
    }
}

void APP_MCSYNC_SendSyncVPLangIndex(U8 index)
{
	if(BtAwsMce_IsDefaultRoleAgent())
		app_mcsync_SendVPLangIndex(index);
}

void APP_MCSYNC_SendSyncCallIdInfo(U8 *pCallerId, U8 length)
{
	if(BtAwsMce_IsDefaultRoleAgent())
	{
		app_mcsync_SendSyncCallIdInfo(pCallerId, length);
	}
}

void APP_MCSYNC_SendPEQRealTimeInfo(U8 *pData, U16 dataLength, U32 clockToStart)
{
	if(BtAwsMce_IsDefaultRoleAgent())	//from client to agent
		app_mcsync_SendPEQRealTimeInfo(pData, dataLength, clockToStart);
}

void APP_MCSYNC_SendSyncPowerOff(U8 reason, BOOL isFollowerIFPKT, BOOL isOnlyForFollower)
{
	app_mcsync_SendSyncPowerOff(reason, isFollowerIFPKT, isOnlyForFollower);
}

<<<<<<< HEAD
void APP_MCSYNC_SendSyncGFPInfo(U8 *pData, U32 length)
{
	if(BtAwsMce_IsDefaultRoleAgent()) {
		app_mcsync_SendSyncGFPInfo(pData, length);
  }
}
=======
#ifdef AIR_TILE_ADV_ENABLE
void APP_MCSYNC_SendSyncTileInfo(BOOL isAgent)
{
	if (MCSYNC_LINK_CONNECTED != MCSYNC_GetMcsyncState()) {
		DBG_LOG_APP_MCSYNC( "[APP_MCSync] sync Tile info fail, AWS not connected", 0);
		return ;
	}
	if(isAgent && BtAwsMce_IsDefaultRoleAgent()) {
		DBG_LOG_APP_MCSYNC( "[APP_MCSync] sync Tile info to partner", 0);
		app_mcsync_SendSyncTileInfo(TRUE);
    } else if ((!isAgent) && BtAwsMce_IsDefaultRolePartner()) {
		DBG_LOG_APP_MCSYNC( "[APP_MCSync] sync Tile info to Agent", 0);
		app_mcsync_SendSyncTileInfo(FALSE);
	}
}
#endif
>>>>>>> db20e11 (second commit)

/* client to agent */
void APP_MCSYNC_SendSyncKeyInfo(U16 keyEventCode, U32 supportedState)
{
	if(BtAwsMce_IsDefaultRolePartner())
	{
<<<<<<< HEAD
		#ifdef MCSYNC_SHARE_MODE
		DBG_LOG_APP_MCSYNC( "[APP_MCSync] sync key info: share mode:%d", 1, BtMCSync_GetShareMode());

		if(BtMCSync_GetShareMode() == MCSYNC_SHARE_MODE_NORMAL_ENABLE)
=======
		#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
		DBG_LOG_APP_MCSYNC( "[APP_MCSync] sync key info: share mode:%d, state:%d", 2,
			BtMCSync_GetShareMode(), APP_MCSync_Share_GetState());

		if(BtMCSync_GetShareMode() == MCSYNC_SHARE_MODE_NORMAL_ENABLE &&
          (APP_MCSync_Share_GetState() == APP_SHARE_STATE_ENTERED_MODE || APP_MCSync_Share_GetState() == APP_SHARE_STATE_PREPARING))
>>>>>>> db20e11 (second commit)
		{
			app_mcsync_SendSyncKeyInfo(keyEventCode, supportedState);
		}
		else
		#endif
		{
			AWSMCE_INTERNAL_KEY_CMD_T msg;
			msg.keyEventCode = keyEventCode;
			msg.supportedState = supportedState;
			bt_mcsync_send_data_by_module_id(MCSYNC_MODULE_SYS_KEY_ACTION,
			sizeof(AWSMCE_INTERNAL_KEY_CMD_T), (U8*)&msg);
		}
	}
}

void APP_MCSYNC_SendSyncKeyCodeInfo(U16 keyCode)
{
    if(BtAwsMce_IsDefaultRolePartner())
    {
        app_mcsync_SendSyncKeyCodeInfo(keyCode);
    }
}

void APP_MCSYNC_SendSyncBdAddrInfo(void)
{
	if(BtAwsMce_IsDefaultRolePartner())
		app_mcsync_SendSyncBdAddrInfo();
}

void APP_MCSYNC_SendSyncRhoParaInfo(U16 batPercent, S8 rssi)
{
    if(BtAwsMce_IsDefaultRolePartner())
    {
        app_mcsync_SendSyncRhoParaInfo(batPercent, rssi);
    }
}

void APP_MCSYNC_SendSyncSmtChgCseInfo(BOOL isTimerSet)
{
    if(BtAwsMce_IsDefaultRolePartner())
    {
        app_mcsync_SendSyncSmtChgCseInfo(isTimerSet);
    }
}

void APP_MCSYNC_SendSyncPartnerInEarDetectionStatus(U8 status)
{
    if(BtAwsMce_IsDefaultRolePartner())
    {
    	app_mcsync_SendSyncInEarDetectionStatus(status);
    }
}


/* Both side */
void APP_MCSYNC_SendSyncVpRtInfo(U16 rtIndex, U16 vpIndex, U16 eventIndex, U32 clockToStart, U8 overwriteTime)
{
	BtMCSync_SendSyncVpRtInfo(rtIndex, vpIndex, eventIndex, clockToStart, overwriteTime);
}

void APP_MCSYNC_SendSyncStopVpRtInfo(U16 eventIndex, U32 currentPicoClock)
{
	app_mcsync_SendSyncStopVpRtInfo(eventIndex, currentPicoClock);
}

void APP_MCSYNC_SendSyncVpRtPlayLater(U16 eventIndex, U32 clockToStart)
{
	app_mcsync_SendSyncVpRtPlayLater(eventIndex, clockToStart);
}

void APP_MCSYNC_SendSyncLEDInfo(U8 patternIndex, U8 *patternType, U8 overwriteTime, BOOL isOverwrite, U32 clockToStart, U8 action)
{
	APP_MCSYNC_LED_STRU msg;

	msg.patternIndex 	= patternIndex;
	if(patternType)
	{
		FW_Memcpy((U8*)&msg.patternType[0], patternType, 6);
	}
	msg.overwriteTime 	= overwriteTime;
	msg.isOverwrite 	= isOverwrite;
	msg.clockToStart 	= clockToStart;
	msg.action			= action;

	//DBG_LOG_APP_MCSYNC( "[APP MCSync] Send Sync LED - patternIdx:%d, overWriteTime:%d, isOverWrite:%d, clockToStart:%d, action:%d", 5, patternIndex, overwriteTime, isOverwrite, clockToStart, action);
	//DBG_LOG_APP_MCSYNC( "[APP MCSync] Send Sync LED - ptnType0:%d, ptnType1:%d, ptnType2:%d", 3, patternType[0], patternType[1], patternType[2]);

	bt_mcsync_send_data_by_module_id(MCSYNC_MODULE_SYS_LED,
		sizeof(APP_MCSYNC_LED_STRU), (U8*)&msg);
}

void APP_MCSYNC_SendSyncFCDReset(U8 type)
{
	if(BtAwsMce_IsDefaultRoleAgent())
		app_mcsync_SendSyncFCDReset(type);
}

void APP_MCSYNC_SendSyncSpeakerMute(BOOL isMute)
{
	if(BtAwsMce_IsDefaultRoleAgent())
		app_mcsync_SendSyncSpeakerMute(isMute);
}

void APP_MCSYNC_SendSyncClearLinkKey(void)
{
	if(BtAwsMce_IsDefaultRoleAgent())
	{
        bt_mcsync_send_data_by_module_id(MCSYNC_MODULE_SYS_CLEAR_LINK_KEY, 0, (U8*)NULL);
	}
}

void APP_MCSYNC_SendSyncGeneralData(U8 *pData, U32 length)
{
	app_mcsync_SendSyncGeneralData(pData, length);
}

void APP_MCSYNC_SendReconnectMask(U8 notReconnMask)
{
    app_mcsync_SendReconnectMask(notReconnMask);
}

void APP_MCSYNC_SendReconnectState(U8 type, BD_ADDR_T *pBdAddr)
{
    app_mcsync_SendReconnectState(type, pBdAddr);
}

void APP_MCSYNC_SendLinkList(U8 * pData, U32 length)
{
    app_mcsync_SendLinkList(pData, length);
}

void APP_MCSYNC_SendSyncFollowerDisc(U8 isFollowerIFPkt, U8 isOnlyForFollower)
{
<<<<<<< HEAD
=======
    #ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
    APP_MCSYNC_DATA_FOLLOWER_DISC_T msg;

	msg.isFollowerIFPkt = isFollowerIFPkt;
	msg.isOnlyForFollower = isOnlyForFollower;
	DBG_LOG_APP_MCSYNC( "[APP_MCSync][Share] SendSyncFollowerDis:%d %d", 2, isFollowerIFPkt, isOnlyForFollower);

	bt_mcsync_send_data_by_module_id(MCSYNC_MODULE_BT_RETRANSMIT_FOLLOWER_DISC,
		sizeof(APP_MCSYNC_DATA_FOLLOWER_DISC_T), (U8*)&msg);
<<<<<<< HEAD
}

#ifdef MCSYNC_SHARE_MODE
=======
	#else
	UNUSED(isFollowerIFPkt); UNUSED(isOnlyForFollower);
	#endif
}

void APP_MCSYNC_SendNoticeMasterPartnerToShareMode(U8 enterShareMode)
{
	#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
	APP_MCSYNC_NOTICE_MASTER_PARTNER_TO_ENTER_SHARE_MODE_T msg;

 	msg.enterShareMode = enterShareMode;

	DBG_LOG_APP_MCSYNC( "[APP_MCSync][Share] SendNoticeMasterPartnerToShareMode:%d", 1,  enterShareMode);

	bt_mcsync_send_data_by_module_id(MCSYNC_MODULE_BT_NOTICE_MASTER_PARTNER_TO_SHARE_MODE,
		sizeof(APP_MCSYNC_NOTICE_MASTER_PARTNER_TO_ENTER_SHARE_MODE_T), (U8*)&msg);
	#else
	UNUSED(enterShareMode);
	#endif
}
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
void APP_MCSYNC_SendSyncShareModeInfo(U8 state, U8 shareMode)
{
	if(BtAwsMce_IsDefaultRoleAgent())
		app_mcsync_SendSyncShareModeInfo(state, shareMode);
}

void APP_MCSYNC_SendSyncFollowerSoundLevel(U8 soundLevel, U8 isFollowerIFPkt)
{
	if((BtMCSync_GetShareMode() == MCSYNC_SHARE_MODE_FOLLOWER_ENABLE) || BtAwsMce_IsDefaultRoleAgent())
	{
		app_mcsync_SendSyncFollowerSoundLevel(soundLevel, isFollowerIFPkt);
	}
}

void APP_MCSYNC_SendSyncOtherAgentBDA(BD_ADDR_T *pBdAddr)
{
	if(pBdAddr == NULL)
		return;

	if(BtAwsMce_IsDefaultRoleAgent())
	{
		app_mcsync_SendOtherAgentBDA(pBdAddr);
	}
}

void APP_MCSYNC_SendExitRequest(void)
{
    if( MCSYNC_SHARE_MODE_NORMAL_ENABLE == BtMCSync_GetShareMode())
    {
        BtMCSync_SendExitShareRequest();
    }
}
#endif
<<<<<<< HEAD
=======

#ifdef AIR_TAKE_OVER_LINK_ENABLE
void APP_MCSYNC_SendSyncTakeOverStateInfo(void)
{
	APP_MCSYNC_TAKE_OVER_STATE_T msg;
	if(BtAwsMce_IsDefaultRoleAgent())
	{
		msg.takeOverState =  BtMCSync_GetMcsyncTakeOverConnState();
		bt_mcsync_send_data_by_module_id(MCSYNC_MODULE_BT_TAKE_OVER_STATE,
		sizeof(APP_MCSYNC_TAKE_OVER_STATE_T), (U8*)&msg);
	}
}
#endif

>>>>>>> db20e11 (second commit)
