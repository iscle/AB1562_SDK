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
#include "App_Hfp.h"
#include "Hfp_Interface.h"
#include "APP_HfpScoHandler.h"
#include "BtHfp.h"
#include "hfp.h"
#include "App_Connection.h"
#include "App_ResourceManager.h"
#include "APP_HfpIncomingCallMediaHandler.h"
#include "app_hfpnvkey_nvstruc.h"
#include "App_Interface.h"
#include "APP_HfpState.h"
#include "App_Media.h"
#include "APP_AudioDriver.h"
#include "App_ReConnection.h"
#include "App_ChargeBattery.h"
#include "App_VolumeManage.h"
#include "App_VolumeProcess.h"
#include "App_State.h"
#include "Hfp_atCommand.h"
#include "PM_Interface.h"
#include "App_InEarDetection.h"
#include "App_HFPKey.h"
#include "App_RhoHandler.h"
#include "APP_HfpScoHandler.h"
#include "App_Customer_Func.h"

log_create_module(APP_HFP, PRINT_LEVEL_INFO);

//**************************************************************************************************
//* Prototype
//**************************************************************************************************
static U32 app_HfpMsgHandler(Handler handler, U16 id, void *msg, U32 handler_id);

//**************************************************************************************************
//* Variable
//**************************************************************************************************/
static const HandlerData gAppHfpMsgHandle = { app_HfpMsgHandler };

//**************************************************************************************************
//* Static Functions
//**************************************************************************************************

static void app_HfpStartServiceCfmHandler(HFP_START_SERVICE_CFM_T *msg)
{
	UNUSED(msg);
}

static void app_HfpConnectIndHandler(HFP_CONNECT_IND_T *ind)
{
	HFP_ConnectResponse(&ind->bdAddr, TRUE);
}

static void app_HfpDisconnectCfmHandler(HFP_DISCONNECT_CFM_T *cfm)
{
	U8 state;
	BD_ADDR_T *pBdAddr = &cfm->bdAddr;

	//app_HFP_AudioTransferDetachHandle(&cfm->bdAddr);

	APP_ImgMedia_AudioVoiceHandler(pBdAddr, IMG_AUDIO_VOICE_STOP);

	while((state = APP_State_GetTopState(&cfm->bdAddr)) >= APP_HFP_INCOMMING && state <= APP_HFP_CAMULTY && APP_IsSetAppLink(&cfm->bdAddr))
	{
		//if(state >= APP_HFP_CALLACTIVE )
			//APP_Media_PushMediaEvent(MEDIA_EVT_END_OF_CALL);

		DBG_LOG_APP_HFP( "[APP_HFP] In HFP State Disc Profile", 0);

		APP_State_RmvState(pBdAddr, state);
	}
	APP_ClearHfpLinkInfo(pBdAddr);
}

static void app_HfpDisconnectIndHandler(HFP_DISCONNECT_IND_T *msg)
{
	HFP_DisconnectRequest(&msg->bdAddr);
}

static void app_HfpConnectCfmHandler(HFP_CONNECT_CFM_T *msg)
{
	if(msg->status == BT_STATUS_SUCCESS)
	{
		APP_HFP_SendATCmdByLink(&msg->bdAddr, HFP_SET_INIT_VALUE);
		APP_HFP_SendEvtToStateHandle(&msg->bdAddr, HFP_CONNECT_CFM);
		if(APP_CheckRedialBdaddrisZero())
			APP_SetRedialBdAddr(&msg->bdAddr);
	}
	else
	{
		APP_ClearHfpLinkInfo(&msg->bdAddr);
	}

	if(msg->status !=BT_STATUS_BUSY)
    {
		if(APP_IsHFPConnecting(&msg->bdAddr) && (msg->profileBit & PROFILE_BIT_HFP) && APP_IsActiveLink(&msg->bdAddr))
		{
			PM_ConnectProfile(&msg->bdAddr, PROFILE_HEADSET);
			return;
		}
		APP_ReConn_DeleteProfileMask(&msg->bdAddr, PROFILE_HANDSFREE, msg->status);
    }
}

static void app_HfpAGErrorCodeIndHandler(HFP_AG_ERROR_CODE_IND_T *ind)
{
	APP_Media_PushMediaEvent(MEDIA_EVT_HFP_ERROR_RECEIVED);
	UNUSED(ind);
}

static void app_HfpAGExtErrorIndHandler(HFP_AG_EXT_ERROR_IND_T *ind)
{
	UNUSED(ind);
}

static void app_HfpCallerNumIndHandler(HFP_CALLER_NUM_IND_T *ind)
{
	APP_HFP_CTL_STRU *pHfpLinkInfo = APP_GetHfpLinkInfo(&ind->bdAddr);

	DBG_LOG_APP_HFP( "[APP_HFP] HFP_CALLER_NUM_IND", 0);

	if(pHfpLinkInfo)
	{
		pHfpLinkInfo->isCallerIdGet = TRUE;
		pHfpLinkInfo->incomingCall.length = (ind->length > CALLER_ID_DIGIT_NO) ? CALLER_ID_DIGIT_NO : ind->length;
		FW_Memcpy(pHfpLinkInfo->incomingCall.callNumId, ind->callNumId, pHfpLinkInfo->incomingCall.length);
        App_HfpImgCallMediaResetTimer();
	}
}

static void app_HfpInbandRingtoneIndHandler(HFP_INBAND_RINGTONE_IND_T *ind)
{
	UNUSED(ind);
}

static void app_HfpRingIndHandler(HFP_RING_IND_T *ind)
{
	DBG_LOG_APP_HFP( "[APP_HFP] HFP_RING_IND", 0);
	APP_HFP_SendEvtToStateHandle(&ind->bdAddr, HFP_RING_IND);
}

static void app_HfpSpeakerVolumeIndHandler(HFP_PLUS_VGS_IND_T *ind)
{
	BD_ADDR_T *pBdAddr = &ind->bdAddr;

	U8 newLevel = APP_VolManage_GetVgsByScoSoundLevel(ind->vgs);

	//BTA-5428
	if (HFP_IsSupportAGInbandRingTone(pBdAddr) && !APP_IsDSPInSCO(pBdAddr) && !newLevel)
		return;

	DBG_LOG_APP_HFP( "[APP_HFP] HFP_PLUS_VGS_IND - VGS : %d", 1, ind->vgs);

	APP_VolManage_SetCurrentVgs(pBdAddr, ind->vgs);

	if(!APP_IsDSPInSCO(pBdAddr))
	{
		goto VGS_SET_LEVEL;
	}

	if(APP_MUTE_CANCEL_WHEN_VOL_CHANGE_FEAT)
	{
		APP_VolManage_AutoMICMuteOff(pBdAddr);
	}

	if(!APP_VolProcess_IsScoMicMute(pBdAddr) || APP_ALLOW_VOL_ADJUST_WHEN_MUTED_FEAT)
	{
		if (APP_Hfp_IsFeatureOn(APP_PLAY_BEEP_ON_REMOTE_INDICATION_VGS_FEAT))
		{
			if(!PM_IsProfileConnected(pBdAddr, PROFILE_MCSYNC))
			{
				APP_VolProcess_Beep(newLevel, AUDIO_DEVICE_SCO, TRUE);
			}
			else
			{
				APP_Media_PushMediaEvent(MEDIA_EVT_PLAYING_BEEP_SYNC);
			}
		}
	}

	VGS_SET_LEVEL:
	APP_VolManage_SpkVolumeRemoteHandler(pBdAddr, newLevel, APP_HfpSco_IsSupportCodecWideBand(pBdAddr) ? COMPONENT_SCO_WB : COMPONENT_SCO_NB);

	//MMI_HCI_UartSendUnSolicitedIndication(MMI_UART_IND_PLUS_VGS, (U8 XDATA_PTR)&hfpParamPtr->vgsEvt.level, linkIndex);	TO DO
}

static void app_HfpMicVolumeIndHandler(HFP_PLUS_VGM_IND_T *ind)
{
	BD_ADDR_T *pBdAddr = &ind->bdAddr;
	U8 newLevel = APP_VolManage_GetScoMicSoundLevelByVgm(ind->vgm);

	DBG_LOG_APP_HFP( "[APP_HFP] HFP_PLUS_VGM_IND - VGM : %d", 1, ind->vgm);

	APP_VolManage_MicVolumeRemoteHandler(pBdAddr, newLevel, COMPONENT_MIC_SCO);

	//MMI_HCI_UartSendUnSolicitedIndication(MMI_UART_IND_PLUS_VGM, (U8 XDATA_PTR)&hfpParamPtr->vgmEvt.level, linkIndex);	TO DO
}

static void app_HfpServiceIndHandler(HFP_SERVICE_IND_T *ind)
{
	APP_HFP_CTL_STRU * pHfpLinkInfo;
	if((pHfpLinkInfo = APP_GetHfpLinkInfo(&ind->bdAddr)) == NULL)
	{
		DBG_LOG_APP_HFP( "[APP_HFP] SERVICE FAIL", 0);
		return;
	}
	//Network signal strength indicator
	pHfpLinkInfo->indicator.service = ind->value;
	APP_Media_PushMediaEvent((ind->value)?MEDIA_EVT_NETWORK_PRESENT:MEDIA_EVT_NETWORK_NOT_PRESENT);
	if(APP_HfpNvkeyGetTimerValue(TIMER_ID_NETWORK_SERVICE_INDICATION_REPEAT))
	{
		APP_HFP_SetTimer(&ind->bdAddr, TIMER_ID_NETWORK_SERVICE_INDICATION_REPEAT, APP_HfpNvkeyGetTimerValue(TIMER_ID_NETWORK_SERVICE_INDICATION_REPEAT) * ONE_SEC);
	}
}

static void app_HfpCallIndHandler(HFP_CALL_IND_T *ind)
{
	APP_HFP_CTL_STRU * pHfpLinkInfo;

	if((pHfpLinkInfo = APP_GetHfpLinkInfo(&ind->bdAddr)) == NULL)
	{
		DBG_LOG_APP_HFP( "[APP_HFP] CALL FAIL", 0);
		return;
	}
	pHfpLinkInfo->indicator.call = ind->value;
	DBG_LOG_APP_HFP( "[APP_HFP] Call Status %s, InEarDetection:%d", 2, APP_CallActiveLogString[pHfpLinkInfo->indicator.call], APP_InEarDetection_IsEnable());
	APP_HFP_SendEvtToStateHandle(&ind->bdAddr, HFP_CALL_IND);
	if(APP_InEarDetection_IsEnable() && (pHfpLinkInfo->indicator.call))
	{
		APP_InEarDetection_TriggerAction(APP_InEarDetection_GetStatus(), APP_InEarDetection_GetPartnerStatus());
	}
}

static void app_HfpCallSetupIndHandler(HFP_CALLSETUP_IND_T *ind)
{
	APP_HFP_CTL_STRU * pHfpLinkInfo;
	if((pHfpLinkInfo = APP_GetHfpLinkInfo(&ind->bdAddr)) == NULL)
	{
		DBG_LOG_APP_HFP( "[APP_HFP] CALL SETUP FAIL", 0);
		return;
	}
	pHfpLinkInfo->indicator.callsetup= ind->value;

	if(APP_Hfp_IsFeatureOn(APP_PLAY_DEFAULT_RING_INCOMING_FEAT) && pHfpLinkInfo->indicator.callsetup == 0 && APP_GetHfpIsRejectByKey(&ind->bdAddr))
	{
		APP_HFP_SetTimer(&ind->bdAddr, TIMER_ID_REJECT_CALL_MUTE_SCO, 2 * ONE_SEC);
	}
	DBG_LOG_APP_HFP( "[APP_HFP] Call Setup Status %s", 1, APP_CallSetupLogString[pHfpLinkInfo->indicator.callsetup]);
	APP_HFP_SendEvtToStateHandle(&ind->bdAddr, HFP_CALLSETUP_IND);
}

static void app_HfpCallHeldIndHandler(HFP_CALLHELD_IND_T *ind)
{
	APP_HFP_CTL_STRU * pHfpLinkInfo;
	if((pHfpLinkInfo = APP_GetHfpLinkInfo(&ind->bdAddr)) == NULL)
	{
		DBG_LOG_APP_HFP( "[APP_HFP] CALL HELD FAIL", 0);
		return;
	}
	pHfpLinkInfo->indicator.callheld= ind->value;
	DBG_LOG_APP_HFP( "[APP_HFP] Call Hold Status %s", 1, APP_CallHoldActiveLogString[pHfpLinkInfo->indicator.callheld]);
	APP_HFP_SendEvtToStateHandle(&ind->bdAddr, HFP_CALLHELD_IND);
}

static void app_HfpSignalIndHandler(HFP_SIGNAL_IND_T *ind)
{
	APP_HFP_CTL_STRU * pHfpLinkInfo;
	if((pHfpLinkInfo = APP_GetHfpLinkInfo(&ind->bdAddr)) == NULL)
	{
		DBG_LOG_APP_HFP( "[APP_HFP] SIGNAL FAIL", 0);
		return;
	}
	pHfpLinkInfo->indicator.signal = ind->value;
}

static void app_HfpRoamIndHandler(HFP_ROAM_IND_T *ind)
{
	APP_HFP_CTL_STRU * pHfpLinkInfo;
	if((pHfpLinkInfo = APP_GetHfpLinkInfo(&ind->bdAddr)) == NULL)
	{
		DBG_LOG_APP_HFP( "[APP_HFP] ROAM FAIL", 0);
		return;
	}
	pHfpLinkInfo->indicator.roam = ind->value;
}

static void app_HfpBatChgIndHandler(HFP_BATCHG_IND_T *ind)
{
	APP_HFP_CTL_STRU * pHfpLinkInfo;
	if((pHfpLinkInfo = APP_GetHfpLinkInfo(&ind->bdAddr)) == NULL)
	{
		DBG_LOG_APP_HFP( "[APP_HFP] BATCHG FAIL", 0);
		return;
	}
	pHfpLinkInfo->indicator.chgbat = ind->value;
}

static void app_HfpRedialCfmHandler(HFP_AT_REDIAL_CFM_T *cfm)
{
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(&cfm->bdAddr);

	if(cfm->status == HFP_CMD_CPL_STATUS_OK)
	{
		APP_Media_PushMediaEvent(MEDIA_EVT_REDIAL_SUCCESSFUL_EVT);
		APP_HFP_SetTimer(&cfm->bdAddr, TIMER_ID_AUDIO_DEV_ATCMD, 20 * ONE_SEC);
	}
	else
	{
		APP_AudioDeviceDeRegister(&cfm->bdAddr, AUDIO_DEVICE_KEY_AT_CMD);
		APP_ResumeOtherAudioDevice(&cfm->bdAddr, AUDIO_DEVICE_KEY_AT_CMD);
	}

	if(pLinkInfo)
		pLinkInfo->subState = APP_SBY_NONE_SUBSTATE;
}

static void app_HfpTerminateCallCfmHandler(HFP_AT_TERMINATE_CALL_CFM_T *cfm)
{
	APP_HFP_CTL_STRU * pHfpLinkInfo;
	BD_ADDR_T *pBdAddr = &cfm->bdAddr;

	if(cfm->status == HFP_CMD_CPL_STATUS_OK && (pHfpLinkInfo = APP_GetHfpLinkInfo(pBdAddr)) != NULL)
	{
		APP_HFP_ReleaseTimer(pBdAddr, TIMER_ID_EST_SCO_AFTER_OGGCALL);
		APP_HFP_SendEvtToStateHandle(pBdAddr, HFP_AT_TERMINATE_CALL_CFM);
	}
}

static void app_HfpVoiceDialCfmHandler(HFP_AT_VOICE_DIAL_CFM_T *cfm)
{
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(&cfm->bdAddr);

	if(pLinkInfo)
	{
		if(cfm->status == HFP_CMD_STATUS_REMOTE_INDICATION)
		{
			if(cfm->isEnable)
			{
				pLinkInfo->subState = APP_VDIAL_STARTED;
				if(pLinkInfo->hfpCtl.appleSiriStatus == HFP_APLSIRI_AVAILABLE_ENABLE)
					APP_Media_PushMediaEvent(MEDIA_EVT_SIRI_STARTING);
			}
			else
			{
				pLinkInfo->subState = APP_VDIAL_STOP;
				if(pLinkInfo->hfpCtl.appleSiriStatus == HFP_APLSIRI_AVAILABLE_ENABLE)
					APP_Media_PushMediaEvent(MEDIA_EVT_SIRI_END);
			}
		}
		else
		{
			if(cfm->isEnable)
			{
				if(cfm->status == HFP_CMD_CPL_STATUS_OK|| cfm->status == HFP_CMD_BVRA_TIMEOUT)
					pLinkInfo->subState = APP_VDIAL_STARTED;
				else
					pLinkInfo->subState = APP_VDIAL_STOP;
			}
			else
			{
				pLinkInfo->subState = APP_VDIAL_STOP;
			}
			APP_AudioDeviceDeRegister(&cfm->bdAddr, AUDIO_DEVICE_KEY_AT_CMD);
			APP_ResumeOtherAudioDevice(&cfm->bdAddr, AUDIO_DEVICE_KEY_AT_CMD);
		}
	}
}

static void app_HfpAppleSiriIndHandler(HFP_APPLE_SIRI_IND_T *ind)
{
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(&ind->bdAddr);

	DBG_LOG_APP_HFP( "[APP_HFP] HFP_APPLE_SIRI_IND", 0);
	pLinkInfo->hfpCtl.appleSiriStatus = ind->status;
}

static void app_HfpAcceptCallCfm(HFP_RESPONSE_HOLD_IND_T *cfm)
{
	switch(cfm->status)
	{
		case BTRH_AG_INCOMING_CALL_PUT_ON_HOLD:
			break;

		case BTRH_AG_INCOMING_CALL_ACCEPTED:
			APP_HFP_SendATCmdByLink(&cfm->bdAddr, HFP_ATCMD_ACCEPT_HELD_INCOMING_CALL);
			break;

		case BTRH_AG_INCOMING_CALL_REJECTED:
			APP_HFP_SendATCmdByLink(&cfm->bdAddr, HFP_ATCMD_REJECT_HELD_INCOMING_CALL);
			break;
	}
}

static void app_HfpVendorIndHandler(BD_ADDR_T * pBdAddr, U8 *data, U8 dataLen)
{
	UNUSED(pBdAddr);
	UNUSED(data);
	UNUSED(dataLen);
/*
	U8 *ptr = FW_GetMemory(dataLen);
	FW_Memcpy(ptr, data,dataLen);
	ptr[dataLen-1] = '\0';
	FW_FreeMemory(ptr);
*/
}

static void app_HfpVendorTimeoutIndHandler(BD_ADDR_T * pBdAddr, U8 cmdNo)
{
	UNUSED(pBdAddr);
	UNUSED(cmdNo);
	//printf("vendor cmd timeout : [%x]",cmdNo);
}

static void app_HfpVendorOKIndHandler(BD_ADDR_T * pBdAddr, U8 cmdNo)
{
	UNUSED(pBdAddr);
	UNUSED(cmdNo);
	//printf("vendor cmd ok : [%x]",cmdNo);
}

static void app_HfpVendorErrorIndHandler(BD_ADDR_T * pBdAddr, U8 cmdNo)
{
	UNUSED(pBdAddr);
	UNUSED(cmdNo);
	//printf("vendor cmd error : [%x]",cmdNo);
}


//**************************************************************************************************
//* Static (Message Handler)
//**************************************************************************************************

static U32 app_HfpMsgHandler(Handler handler, U16 id, void *msg, U32 handler_id)
{
	UNUSED(handler);
	UNUSED(handler_id);

	DBG_LOG_APP_HFP( "[APP_HFP] %s, id:0x%x", 2, APP_HFPMsg[ id - MSG_MESSAGE_BT_PROFILE_HSP ], id);

	switch(id)
	{
		//Connection
		case HFP_START_SERVICE_CFM:
			app_HfpStartServiceCfmHandler((HFP_START_SERVICE_CFM_T *)msg);
			break;

		case HFP_STOP_SERVICE_CFM:
			break;

		case HFP_CONNECT_IND:
			app_HfpConnectIndHandler((HFP_CONNECT_IND_T*)msg);
			break;

		case HFP_DISCONNECT_CFM:
			app_HfpDisconnectCfmHandler((HFP_DISCONNECT_CFM_T *)msg);
			break;

		case HFP_DISCONNECT_IND:
			app_HfpDisconnectIndHandler((HFP_DISCONNECT_IND_T *)msg);
			break;

		case HFP_CONNECT_CFM:
			app_HfpConnectCfmHandler((HFP_CONNECT_CFM_T *)msg);
			break;

		//Indication
		case HFP_AG_ERROR_CODE_IND:
			app_HfpAGErrorCodeIndHandler((HFP_AG_ERROR_CODE_IND_T *)msg);
			break;

		case HFP_AG_EXT_ERROR_IND:
			app_HfpAGExtErrorIndHandler((HFP_AG_EXT_ERROR_IND_T *)msg);
			break;

		case HFP_SUSCRIBER_NUM_IND:
			break;

		case HFP_CALLER_NUM_IND:
			app_HfpCallerNumIndHandler((HFP_CALLER_NUM_IND_T *)msg);
			break;

		case HFP_CURRENT_CALL_LIST_IND:
			break;

		case HFP_INBAND_RINGTONE_IND:
			app_HfpInbandRingtoneIndHandler((HFP_INBAND_RINGTONE_IND_T *)msg);
			break;

		case HFP_RESPONSE_HOLD_IND:
			break;

		case HFP_RING_IND:
			app_HfpRingIndHandler((HFP_RING_IND_T*)msg);
			break;

		case HFP_PLUS_VGS_IND:
			app_HfpSpeakerVolumeIndHandler((HFP_PLUS_VGS_IND_T *)msg);
			break;

		case HFP_PLUS_VGM_IND:
			app_HfpMicVolumeIndHandler((HFP_PLUS_VGM_IND_T *)msg);
			break;

		case HFP_SERVICE_IND:
			app_HfpServiceIndHandler((HFP_SERVICE_IND_T *)msg);
			break;

		case HFP_CALL_IND:
			app_HfpCallIndHandler((HFP_CALL_IND_T *)msg);
			break;

		case HFP_CALLSETUP_IND:
			app_HfpCallSetupIndHandler((HFP_CALLSETUP_IND_T *)msg);
			break;

		case HFP_CALLHELD_IND:
			app_HfpCallHeldIndHandler((HFP_CALLHELD_IND_T *)msg);
			break;

		case HFP_SIGNAL_IND:
			app_HfpSignalIndHandler((HFP_SIGNAL_IND_T *)msg);
			break;

		case HFP_ROAM_IND:
			app_HfpRoamIndHandler((HFP_ROAM_IND_T *)msg);
			break;

		case HFP_BATCHG_IND:
			app_HfpBatChgIndHandler((HFP_BATCHG_IND_T *)msg);
			break;

		case HFP_APPLE_SIRI_IND:
			app_HfpAppleSiriIndHandler((HFP_APPLE_SIRI_IND_T *)msg);
			break;

		case HFP_UNKNOWN_RESULT_CODE_IND:
			break;

		//Confirm
		case HFP_AT_ACCEPT_CALL_CFM:
			app_HfpAcceptCallCfm((HFP_RESPONSE_HOLD_IND_T *)msg);
			break;

		case HFP_AT_AG_ERROR_CODE_CFM:
			break;

		case HFP_AT_APL_EFM_CFM:
			break;

		case HSP_AT_BUTTON_PRESS_CFM:
			break;

		case HFP_AT_CALL_WAITING_CFM:
			break;

		case HFP_AT_CODEC_SELECT_CFM:
			break;

		case HFP_AT_INIT_SCO_CONN_CFM:
			break;

		case HFP_AT_NREC_CFM:
			break;

		case HFP_AT_REDIAL_CFM:
			app_HfpRedialCfmHandler((HFP_AT_REDIAL_CFM_T *)msg);
			break;

		case HFP_AT_TERMINATE_CALL_CFM:
			app_HfpTerminateCallCfmHandler((HFP_AT_TERMINATE_CALL_CFM_T *)msg);
			break;

		case HFP_AT_VGS_CFM:
			break;

		case HFP_AT_VGM_CFM:
			break;

		case HFP_AT_VOICE_DIAL_CFM:
			app_HfpVoiceDialCfmHandler((HFP_AT_VOICE_DIAL_CFM_T *)msg);
			break;

		case HFP_AT_VENDOR_IND:
			app_HfpVendorIndHandler(&((HFP_AT_VENDOR_IND_T *)msg)->bdAddr, ((HFP_AT_VENDOR_IND_T *)msg)->data, ((HFP_AT_VENDOR_IND_T *)msg)->dataLen);
			break;

		case HFP_AT_VENDOR_TIMEOUT_IND:
			app_HfpVendorTimeoutIndHandler(&((HFP_AT_VENDOR_TIMEOUT_IND_T *)msg)->bdAddr, ((HFP_AT_VENDOR_TIMEOUT_IND_T *)msg)->cmdNo);
			break;
		case HFP_AT_VENDOR_OK_IND:
			app_HfpVendorOKIndHandler(&((HFP_AT_VENDOR_OK_IND_T *)msg)->bdAddr, ((HFP_AT_VENDOR_OK_IND_T *)msg)->cmdNo);
			break;
		case HFP_AT_VENDOR_ERROR_IND:
			app_HfpVendorErrorIndHandler(&((HFP_AT_VENDOR_ERROR_IND_T *)msg)->bdAddr, ((HFP_AT_VENDOR_ERROR_IND_T *)msg)->cmdNo);
			break;
		default:
			break;
	}
	return 0;
}

static void app_HfpInitNvkey(void)
{
	App_HfpImgCallMediaInitNvkey(NVKEY_GetPayloadFlashAddress(NVKEYID_APP_IMG_CALL_MEDIA_CONFIG), NVKEY_GetPayloadLength(NVKEYID_APP_IMG_CALL_MEDIA_CONFIG));
	App_HfpSCOPktPara_InitNvKey();
	APP_HfpNvkeyTimerInit();
}

//**************************************************************************************************
//* APP Functions
//**************************************************************************************************
/*Public*/
void APP_Hfp_StartService(U8 profileBit)
{
	DBG_LOG_APP_HFP( "[APP_HFP] HFP_StartService", 0);

	APP_HfpSco_Register();

	if(profileBit & PROFILE_BIT_HFP)
	{
		app_HfpInitNvkey();
		//APP_Hfp_regVendorAT();
	}
	HFP_StartService((Handler)&gAppHfpMsgHandle, profileBit);
}

//**************************************************************************************************
//* Public Functions (Connection)
//**************************************************************************************************

//Vendor
//define by user, for example:


#define vendorATNum 5
#define vendorATMAXLen 16
#define vendorATResultNum vendorATNum
#define vendorATResultMaxLen 9
#define AT_CR	0x0D

const U8 appVendorAT[vendorATNum * vendorATMAXLen] =//Tx Cmd
{
	11, 'A','T','+','A','I','R','O','H','A','1',AT_CR,AT_CR,AT_CR,AT_CR,AT_CR,
	11, 'A','T','+','A','I','R','O','H','A','2',AT_CR,AT_CR,AT_CR,AT_CR,AT_CR,
	11, 'A','T','+','A','I','R','O','H','A','3',AT_CR,AT_CR,AT_CR,AT_CR,AT_CR,
	 9, 'A','T','+','T','E','S','T','0',AT_CR,AT_CR,AT_CR,AT_CR,AT_CR,AT_CR,AT_CR,
	 9, 'A','T','+','T','E','S','T','1',AT_CR,AT_CR,AT_CR,AT_CR,AT_CR,AT_CR,AT_CR
};

const U8 appVendResultAT[vendorATResultNum * vendorATResultMaxLen] =//Rx Rsp
{
	'+','A','I','R','O','H','A','1', 0,
	'+','A','I','R','O','H','A','2', 0,
	'+','A','I','R','O','H','A','3', 0,
	'+','T','E','S','T','0', 0, 0, 0,
	'+','T','E','S','T','1', 0, 0, 0
};


void APP_Hfp_regVendorAT()
{
	Hfp_RegVendorCmd((U8*)appVendorAT, (U8*)appVendResultAT, vendorATNum, vendorATMAXLen, vendorATResultNum, vendorATResultMaxLen);
}
void APP_Hfp_RegisterProfile()
{
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
	APP_Hfp_StartService(PROFILE_BIT_HFP | PROFILE_BIT_HSP);
#else
    APP_Hfp_StartService(PROFILE_BIT_HFP);
#endif
}

BOOL APP_Hfp_IsConnected(BD_ADDR_T *pBdAddr)
{
	return HFP_IsConnected(pBdAddr);
}

//**************************************************************************************************
//* Public Functions (battery send)
//**************************************************************************************************
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
BOOL APP_Hfp_IsHspConnected(BD_ADDR_T * pBdAddr)
{
	return PM_IsProfileConnected(pBdAddr, PROFILE_HEADSET);
}
#endif

BOOL APP_Hfp_IsFeatureOn(U32 feature)
{
	UNUSED(feature);
	return FALSE;
}

PUBLIC BD_ADDR_T *APP_Hfp_GetNextHfpLink(BD_ADDR_T * pBdAddr)
{
	#ifdef SUPPORT_MULTI_POINT
	U8 linkIndex;
	BD_ADDR_T  *pNextBdAddr;
	linkIndex = APP_GetLinkIndexByBdAddr(pBdAddr);

	for(linkIndex = 0; linkIndex <  FW_MAX_LINK_IDX; linkIndex++)
	{
		pNextBdAddr = APP_GetBdAddrByLinkIndex(linkIndex);
		if(pNextBdAddr != NULL && !FW_CmpBdAddr(pBdAddr, pNextBdAddr) && APP_Hfp_IsConnected(pNextBdAddr))
		{
			return pNextBdAddr;
		}
	}
#else
	UNUSED(pBdAddr);
#endif
	return APP_EOF;
}

//**************************************************************************************************
//* Public Functions (Send AT Cmd)
//**************************************************************************************************

BOOL APP_SendATCmdUnderConnection(BD_ADDR_T *pBdAddr, U8 opcode)
{
	if(HFP_IsConnected(pBdAddr))
	{
		APP_HFP_SendATCmdByLink(pBdAddr, opcode);
		return TRUE;
	}
	return FALSE;
}


void APP_HFP_SendATCmdByLink(BD_ADDR_T *pBdAddr, U8 opcode)
{

	APP_HFP_CTL_STRU * pHfpLinkInfo = APP_GetHfpLinkInfo(pBdAddr);
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	if(!pHfpLinkInfo || !pLinkInfo)
		return;

	HFP_SniffHandler(pBdAddr, opcode);
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE    
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE    
>>>>>>> db20e11 (second commit)
	if(APP_Hfp_IsHspConnected(pBdAddr))
	{
		switch(opcode)
		{
			case HFP_ATCMD_ADJUST_MIC_GAIN:
				HFP_Send_VGM(pBdAddr, APP_VolManage_GetCurrentScoVgm(pBdAddr));
				break;
			case HFP_ATCMD_ADJUST_VOLUME:
				HFP_Send_VGS(pBdAddr, APP_VolManage_GetVgsByScoSoundLevel(APP_VolManage_GetCurrentScoSoundLevel(pBdAddr)));
				break;
			case HFP_SET_INIT_VALUE:
				HFP_SetInitVolume(pBdAddr, APP_VolManage_GetVgsByScoSoundLevel(APP_VolManage_GetCurrentScoSoundLevel(pBdAddr)), APP_VolManage_GetCurrentScoVgm(pBdAddr));
				break;
			default:
				if(opcode > HFP_NO_COMMAND && opcode < HFP_TOTAL_ATCMD_COUNT)
					HFP_Send_CKPD200(pBdAddr);
				break;
		}
	}
	else
#endif        
	{
		switch(opcode)
		{
			case HFP_ATCMD_INIT_VOICE_DIAL: //gHfpAtBvra
			    if(APP_Customer_IsVDHandledByGattVoice(pBdAddr, 0)) {
			        break;
			    }
				//gMMI_ctl.mmiInfo[linkIndex].subState = MMI_VDIAL_STARTING;
				APP_Media_PushMediaEvent(MEDIA_EVT_VOICE_DIAL_ENABLE);
				pLinkInfo->subState = APP_VDIAL_STARTING;
				HFP_Send_BVRA(pBdAddr, TRUE);
				break;
			case HFP_ATCMD_CANCEL_VOICE_DIAL: //gHfpAtBvra
			    if(APP_Customer_IsVDHandledByGattVoice(pBdAddr, 1)) {
			        break;
			    }
				//gMMI_ctl.mmiInfo[linkIndex].subState = MMI_VDIAL_STOPPING;
				APP_Media_PushMediaEvent(MEDIA_EVT_VOICE_DIAL_ENABLE);
				pLinkInfo->subState = APP_VDIAL_STOPPING;
				HFP_Send_BVRA(pBdAddr, FALSE);
				break;
			case HFP_ATCMD_LAST_NUMBER_REDIAL: //gHfpAtBldn
				APP_Media_PushMediaEvent(MEDIA_EVT_REDIAL_ENABLE);
				pLinkInfo->subState = APP_RDIAL_STARTING;
				HFP_Send_BLDN(pBdAddr);
				break;
			case HFP_ATCMD_CANCEL_OUTGOING_CALL: //gHfpAtChup
			case HFP_ATCMD_REJECT_CALL:
				APP_ImgMedia_AudioVoiceHandler(pBdAddr, IMG_AUDIO_VOICE_STOP);
				APP_SetHfpIsRejectByKey(pBdAddr, TRUE);
                /* fallthrough */
			case HFP_ATCMD_END_CALL:
				HFP_Send_CHUP(pBdAddr);
				break;
			case HFP_ATCMD_ACCEPT_CALL: //gHfpAta
				APP_ImgMedia_AudioVoiceHandler(pBdAddr, IMG_AUDIO_VOICE_STOP);
				HFP_Send_ATA(pBdAddr);
				break;
			case HFP_ATCMD_ADJUST_VOLUME:
				HFP_Send_VGS(pBdAddr, APP_VolManage_GetVgsByScoSoundLevel(APP_VolManage_GetCurrentScoSoundLevel(pBdAddr)));
				break;
			case HFP_ATCMD_ADJUST_MIC_GAIN:
				HFP_Send_VGM(pBdAddr, APP_VolManage_GetCurrentScoVgm(pBdAddr));
				break;
			case HFP_SET_INIT_VALUE:
				HFP_SetInitVolume(pBdAddr, APP_VolManage_GetVgsByScoSoundLevel(APP_VolManage_GetCurrentScoSoundLevel(pBdAddr)), APP_VolManage_GetCurrentScoVgm(pBdAddr));
				break;
			case HFP_ATCMD_SET_NREC: //gHfpAtNrec
				HFP_Send_ECNR(pBdAddr,FALSE);
				break;
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE                
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE                
>>>>>>> db20e11 (second commit)
			case HFP_ATCMD_CKPD_200: //gHfpAtCkpd200
				HFP_Send_CKPD200(pBdAddr);
				break;
#endif
			case HFP_ATCMD_PLACE_CALL_WITH_PHONE_NUMBER: //HFP_at_atd_digit
				//OSMEM_memcpy_xdata_xdata((U8 XDATA_PTR)&msgPtr->msgBodyPtr.hfpCmd.DialNumCmd,
				//(U8 XDATA_PTR)&gMMI_HFP_ctl.HfpCtl[linkIndex].incomingCall,
				//sizeof(PhoneNumber));
				break;
			case HFP_ATCMD_MEMORY_DIALING: //gHfpAtdNumber
				//msgPtr->msgBodyPtr.hfpCmd.DialMemCmd.index = gMMI_HFP_ctl.HfpCtl[linkIndex].memoryDialIndex;
				#ifdef HFP_AT_ATD_NUMBER
				HFP_Send_ATDNUMBER(pBdAddr, pHfpLinkInfo->memoryDialIndex);
				#endif
				break;
/*
			//case HFP_ATCMD_HOLD_AND_MULTIPARTY_HANDLING: //HFP_at_chld => parsing chld code,1530 paras int HFP_Send_CHLD()
				//break;
*/
			case HFP_ATCMD_QUERY_OPERATOR_SELECTION: //HFP_at_cops
				#ifdef HFP_AT_COPS
				HFP_Send_COPS(pBdAddr);
				#endif
				break;
			case HFP_ATCMD_GET_SUBSCRIBER_NUMBER: //gHfpAtCnum
				#ifdef HFP_AT_CNUM
				HFP_Send_CNUM(pBdAddr);
				#endif
				break;
			case HFP_ATCMD_SELECT_CHAR_SET_UTF8: //HFP_at_cscs
				break;
			case HFP_ATCMD_ENABLE_CLI_NOTIFICATION: //HFP_at_clip
				HFP_Send_CLIP(pBdAddr,TRUE);
				break;
			case HFP_ATCMD_DISABLE_CLI_NOTIFICATION: //HFP_at_clip
				HFP_Send_CLIP(pBdAddr,FALSE);
				break;
			case HFP_ATCMD_SEND_DTMF: //HFP_at_vts
				//msgPtr->msgBodyPtr.hfpCmd.DTMFCmd.para = gMMI_HFP_ctl.HfpCtl[linkIndex].DTMFPara;//0 - 9 and * and #
				#ifdef HFP_AT_VTS
				HFP_Send_VTS(pBdAddr, pHfpLinkInfo->DTMFPara);
				#endif
				break;
			case HFP_ATCMD_ENABLE_CALL_WAITING_NOTIFICATION: //HFP_at_ccwa
				break;
			case HFP_ATCMD_DISABLE_CALL_WAITING_NOTIFICATION: //HFP_at_ccwa
				break;
			case HFP_ATCMD_ENABLE_COMMAND_ERROR: //HFP_at_cmee
				break;
			case HFP_ATCMD_DISABLE_COMMAND_ERROR: //HFP_at_cmee
				break;
			case HFP_ATCMD_QUERY_LIST_OF_CURRENT_CALLS: //gHfpAtClcc
				#ifdef HFP_AT_CLCC
				HFP_Send_CLCC(pBdAddr);
				#endif
				break;
			case HFP_ATCMD_ATTACH_LAST_VOICE_TAG_RECORDED: //HFP_at_binp
				#ifdef HFP_AT_BINP
				HFP_Send_BINP(pBdAddr);
				#endif
				break;
			case HFP_ATCMD_TEST_GET_SUBSCRIBER_NUMBER: //HFP_at_cnum_equal_ques
				break;
			case HFP_ATCMD_AVAILABLE_CODEC: //gHfpAtBac
				HFP_Send_BAC(pBdAddr);
				break;
			case HFP_ATCMD_SELECT_CODEC: //gHfpAtBcs
				//msgPtr->msgBodyPtr.hfpCmd.BCSCmd.scoCodec = gMMI_ctl.mmiInfo[linkIndex].scoCtl.scoCodec;
				HFP_Send_BCS(pBdAddr);
				break;
			case HFP_ATCMD_CODEC_CONNECTION: //HFP_at_BCC
				HFP_Send_BCC(pBdAddr);
				break;
			case HFP_ATCMD_ACTIVATE_INDICATORS: //HFP_at_BIA
				break;
			case HFP_ATCMD_PUT_INCOMING_CALL_ON_HOLD: //NULL
				break;
			case HFP_ATCMD_ACCEPT_HELD_INCOMING_CALL: //HFP_at_btrh
				#ifdef HFP_AT_BTRH
				HFP_Send_BTRH(pBdAddr,TRUE);
				#endif
				break;
			case HFP_ATCMD_REJECT_HELD_INCOMING_CALL: //HFP_at_btrh
				#ifdef HFP_AT_BTRH
				HFP_Send_BTRH(pBdAddr,FALSE);
				#endif
				break;
			case HFP_ATCMD_READ_CURRENT_RSP_AND_HOLD_STATUS: //HFP_at_btrh
				break;
			case HFP_ATCMD_APPLE_SIRI_STATUS: //HFP_at_aplsiri
				HFP_Send_APLSIRI(pBdAddr);
				break;
			case HFP_ATCMD_ENABLE_APPLE_EYES_FREE_MODE: //gHfpAtAplefm
				HFP_Send_APLEFM(pBdAddr, TRUE);
				break;
			case HFP_ATCMD_DISABLE_APPLE_EYES_FREE_MODE: //gHfpAtAplefm
				HFP_Send_APLEFM(pBdAddr, FALSE);
				break;
			case HFP_ATCMD_3WAY_RELNUDUB:
				//msgPtr->msgOpcode = HFP_ATCMD_HOLD_AND_MULTIPARTY_HANDLING;//HFP_at_chld
				//msgPtr->msgBodyPtr.hfpCmd.CHLDCmd.para = 0;
				if(pHfpLinkInfo->callState[0] > APP_CALL_EMPTY && pHfpLinkInfo->callState[1] > APP_CALL_EMPTY)
					pHfpLinkInfo->is3WayAdd = APP_CALL_NO_3WAY;
				HFP_Send_CHLD(pBdAddr,0);
				break;

			case HFP_ATCMD_3WAY_RELNACP:
				//msgPtr->msgOpcode = HFP_ATCMD_HOLD_AND_MULTIPARTY_HANDLING;//HFP_at_chld
				//msgPtr->msgBodyPtr.hfpCmd.CHLDCmd.para = 1;
				if(pHfpLinkInfo->callState[0] != APP_CALL_HELD || pHfpLinkInfo->callState[1] != APP_CALL_HELD)
					pHfpLinkInfo->is3WayAdd = APP_CALL_NO_3WAY;

				HFP_Send_CHLD(pBdAddr,1);
				break;

			case HFP_ATCMD_3WAY_RELNACP_X:
				//msgPtr->msgOpcode = HFP_ATCMD_HOLD_AND_MULTIPARTY_HANDLING;//HFP_at_chld
				//msgPtr->msgBodyPtr.hfpCmd.CHLDCmd.para = 12;
				if(pHfpLinkInfo->callState[0] != APP_CALL_HELD || pHfpLinkInfo->callState[1] != APP_CALL_HELD)
					pHfpLinkInfo->is3WayAdd = APP_CALL_NO_3WAY;
				HFP_Send_CHLD(pBdAddr,12);
				break;

			case HFP_ATCMD_3WAY_HOLDNACP:
				//msgPtr->msgOpcode = HFP_ATCMD_HOLD_AND_MULTIPARTY_HANDLING;//HFP_at_chld
				//msgPtr->msgBodyPtr.hfpCmd.CHLDCmd.para = 2;
				pHfpLinkInfo->is3WayAdd = APP_CALL_IN_3WAY;
				HFP_Send_CHLD(pBdAddr,2);
				break;
			case HFP_ATCMD_3WAY_HOLDNACP_X:
				//msgPtr->msgOpcode = HFP_ATCMD_HOLD_AND_MULTIPARTY_HANDLING;//HFP_at_chld
				//msgPtr->msgBodyPtr.hfpCmd.CHLDCmd.para = 22;
				pHfpLinkInfo->is3WayAdd = APP_CALL_IN_3WAY;
				HFP_Send_CHLD(pBdAddr,22);
				break;
			case HFP_ATCMD_3WAY_ADD:
				//msgPtr->msgOpcode = HFP_ATCMD_HOLD_AND_MULTIPARTY_HANDLING;//HFP_at_chld
				//msgPtr->msgBodyPtr.hfpCmd.CHLDCmd.para = 3;
				pHfpLinkInfo->is3WayAdd = APP_CALL_IN_3WAY;
				HFP_Send_CHLD(pBdAddr,3);
				break;

			case HFP_ATCMD_3WAY_CALLTRANSFER:
				//msgPtr->msgOpcode = HFP_ATCMD_HOLD_AND_MULTIPARTY_HANDLING;//HFP_at_chld
				//msgPtr->msgBodyPtr.hfpCmd.CHLDCmd.para = 4;
				pHfpLinkInfo->is3WayAdd = APP_CALL_NO_3WAY;
				HFP_Send_CHLD(pBdAddr,4);
				break;
			default:
				//FW_Assert(FALSE);
				break;
		}
	}
}

void APP_HFP_StopIncomingVPRing(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	APP_Media_StopMediaEvt(MEDIA_EVT_RINGTONE);
	APP_Media_StopMediaEvt(MEDIA_EVT_ENTER_INCOMING);
	APP_Media_StopMediaEvt(MEDIA_EVT_CALLER_ID);

	//pHfpLinkInfo->isCallerNameGet = FALSE;
	//#ifdef BLE_AIRApp
	//SendPhoneNumberToApp(linkIndex, gMMI_HFP_ctl.HfpCtl[linkIndex].incomingCall.length, 0);
	//#endif
}

void APP_HFP_GetCallSetupRemoteAlert(BD_ADDR_T *pBdAddr, BOOL isUnderCallActive)
{
	//For Smasung SGH E848:
	//For IOT Testing Issue 109
	if(!isUnderCallActive)
	{
		if (!APP_SCO_EXISTS(pBdAddr))
		{
			if(APP_HfpNvkeyGetTimerValue(TIMER_ID_EST_SCO_AFTER_OGGCALL))
			{
				APP_HFP_SetTimer(pBdAddr, TIMER_ID_EST_SCO_AFTER_OGGCALL, (U32)APP_HfpNvkeyGetTimerValue(TIMER_ID_EST_SCO_AFTER_OGGCALL) * 100L);
			}
		}
	}
}

void APP_HFP_ScoConnOKHandler(BD_ADDR_T *pBdAddr)
{
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
	if(APP_Hfp_IsHspConnected(pBdAddr))
	{
		APP_State_RmvState(pBdAddr, APP_HFP_INCOMMING);

		if(!APP_Hfp_IsFeatureOn(APP_NOT_CHG_STATE_ACP_HSP_RING_FEAT))
			APP_State_InsertState(pBdAddr, APP_HFP_CALLACTIVE, APP_CONNECTED);
	}
	else
#endif        
	{
		APP_State_ReplaceStateWithEntry(pBdAddr, APP_HFP_CALLACTIVE, APP_HFP_CALLACTIVE_WITHOUT_SCO, TRUE);
	}

	APP_SetHfpLinkAudioTransfer(pBdAddr, FALSE);
	APP_HFP_ReleaseTimer(pBdAddr, TIMER_ID_EST_SCO_AFTER_OGGCALL);
}

void APP_HFP_ScoConnFailHandler(BD_ADDR_T *pBdAddr)
{
	//sco retry on hv3
	if(APP_GetHfpLinkAudioTransfer(pBdAddr))
	{
		if (!APP_SCO_EXISTS(pBdAddr))
		{
			APP_HfpSco_ConnectRequest(pBdAddr, FALSE, LINK_TYPE_SCO_ONLY_HV3);
		}
		APP_SetHfpLinkAudioTransfer(pBdAddr, FALSE);
		//gMMI_HFP_ctl.HfpCtl[pBdAddr].relSCOCozExitOggState = FALSE;
	}
}

void APP_HFP_ScoDiscHandler(BD_ADDR_T *pBdAddr)
{
	if(APP_State_CheckNestStateByLink(pBdAddr, APP_HFP_CALLACTIVE))
	{
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE	
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE	
>>>>>>> db20e11 (second commit)
		if(APP_Hfp_IsHspConnected(pBdAddr))
		{
			APP_State_RmvState(pBdAddr, APP_HFP_CALLACTIVE);
		}
		else
#endif            
		{
			APP_State_ReplaceStateWithEntry(pBdAddr, APP_HFP_CALLACTIVE_WITHOUT_SCO, APP_HFP_CALLACTIVE, TRUE);
		}
	}
	APP_SetHfpLinkAudioTransfer(pBdAddr, FALSE);
	APP_HFP_ReleaseTimer(pBdAddr, TIMER_ID_EST_SCO_AFTER_OGGCALL);
	//if(gMMI_ctl.mmiInfo[pBdAddr].subState == MMI_VDIAL_STARTED || gMMI_ctl.mmiInfo[pBdAddr].subState == MMI_VDIAL_STARTING)
	//{
	//	gMMI_ctl.mmiInfo[pBdAddr].subState = MMI_VDIAL_STOP;
	//}

	//#ifdef BLE_AIRApp
	//Handle press vol down button when incomming call on iphone
	//if(MMI_CheckNestState(pBdAddr, APP_HFP_INCOMMING))
	//{
	//	if(AirApp_BleIsConnected() && (ATT_FindServerIndexByAddr((U8 CODE_PTR)&AppCtl.phoneNumber) != 0) && ((((ATT_GetCCCDByLink(ATT_FindServerIndexByAddr((U8 CODE_PTR)AppCtl.phnoeNumber_CCCD), 0)) >> 8) & 0x01) == 0x01))
	//	{
	//		APP_ImgMedia_AudioVoiceHandler(pBdAddr, IMG_AUDIO_VOICE_TRIGGER);
	//	}
	//}
	//#endif
}

void APP_HFP_CreateAudioConnection(BD_ADDR_T *pBdAddr, BOOL isViaKey)
{
	if (APP_SCO_EXISTS(pBdAddr))
		return;

	if(!isViaKey)
		goto DIRECTLY_EST_SCO;

	if(APP_Hfp_IsConnected(pBdAddr) && HFP_IsSupportCodecNegoFeature(pBdAddr))
	{
		APP_HFP_SendATCmdByLink(pBdAddr, HFP_ATCMD_CODEC_CONNECTION);
	}
	else
	{
		DIRECTLY_EST_SCO:
		//New feature - APP_FORCE_USE_ESCO_IN_AUDIO_TRANSFER_FEAT
		if (APP_FORCE_USE_ESCO_IN_AUDIO_TRANSFER_FEAT)
		{
			APP_HfpSco_ConnectRequest(pBdAddr,FALSE,LINK_TYPE_eSCO);
		}
		else
		{
			APP_HfpSco_ConnectRequest(pBdAddr,FALSE,LINK_TYPE_SCO);
		}
	}
}

extern const HandlerData gHfpStateCheckTimer;
void APP_HFP_SetTimer(BD_ADDR_T *pBdAddr, U16 id, U32 timeValue)
{
	APP_HFP_CTL_STRU * pLinkInfo;

	if(pBdAddr && (pLinkInfo = APP_GetHfpLinkInfo(pBdAddr)))
	{
		FW_SetTimer((Handler)&gHfpStateCheckTimer, id, NULL, (U32)pLinkInfo, timeValue);
	}
}

void APP_HFP_ReleaseTimer(BD_ADDR_T *pBdAddr, U16 id)
{
	APP_HFP_CTL_STRU * pLinkInfo;

	if(pBdAddr && (pLinkInfo = APP_GetHfpLinkInfo(pBdAddr)))
	{
		FW_ReleaseTimer((Handler)&gHfpStateCheckTimer, id, (U32)pLinkInfo);
	}
}

void APP_HFP_ReleaseAllTimer(BD_ADDR_T *pBdAddr)
{
	APP_HFP_CTL_STRU * pLinkInfo;
	if(pBdAddr && (pLinkInfo = APP_GetHfpLinkInfo(pBdAddr)))
	{
<<<<<<< HEAD
		MSG_MessageCancelEx2((Handler)&gHfpStateCheckTimer, (U32)pLinkInfo);
=======
		MSG_MessageCancelAllEx2((Handler)&gHfpStateCheckTimer, (U32)pLinkInfo);
>>>>>>> db20e11 (second commit)
	}
}

U16 APP_HFP_CheckTimer(BD_ADDR_T *pBdAddr, U16 id)
{
	APP_HFP_CTL_STRU * pLinkInfo;

	if(pBdAddr && (pLinkInfo = APP_GetHfpLinkInfo(pBdAddr)))
	{
		return FW_CheckTimer((Handler)&gHfpStateCheckTimer, id, (U32)pLinkInfo);
	}
	return 0;
}

U8 APP_Hfp_GetSupportCodec(BD_ADDR_T * pBdAddr)
{
	if(pBdAddr)
	{
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE	
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE	
>>>>>>> db20e11 (second commit)
		return (APP_Hfp_IsConnected(pBdAddr) || APP_Hfp_IsHspConnected(pBdAddr)) ? HFP_GetSupportCodec(pBdAddr): SCO_CODEC_CVSD;
#else
		return APP_Hfp_IsConnected(pBdAddr) ? HFP_GetSupportCodec(pBdAddr): SCO_CODEC_CVSD;
#endif
	}

	return SCO_CODEC_CVSD;
}

BOOL APP_Hfp_IsCodecWideBand(U8 codec)
{
	return (codec != SCO_CODEC_CVSD);
}

BOOL APP_Hfp_IsAGSiriSupported(void)
{
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(APP_GetServiceBdAddr());

	return (pLinkInfo && pLinkInfo->hfpCtl.appleSiriStatus != HFP_APLSIRI_NOT_AVAILABLE) ? TRUE : FALSE;
}

BOOL APP_Hfp_FillRhoData(void *pData)
{
	APP_RHO_HFP_STRU *pDataToFill = &((APP_RHO_DATA_STRU *)pData)->appHfp;

	if(!pDataToFill)
	{
		DBG_LOG_APP_HFP( "[APP_HFP] Fill RHO data error", 0);
		return FALSE;
	}

	if(!APP_HfpImgMedia_FillRhoData(pDataToFill))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL APP_Hfp_AssignRhoData(void *pData)
{
	APP_RHO_HFP_STRU *pDataToGet = &((APP_RHO_DATA_STRU *)pData)->appHfp;

	if(!pDataToGet)
	{
		DBG_LOG_APP_HFP( "[APP_HFP] Assign RHO data error", 0);
		return FALSE;
	}

	if(!APP_HfpImgMedia_AssignRhoData(pDataToGet))
	{
		return FALSE;
	}

	return TRUE;
}

U8 APP_Hfp_GetMsgNumInQueue(void)
{
	U8 totalMsgNum = 0, msgNum;

	if((msgNum = APP_HfpSco_GetMsgNumInQueue()))
	{
		totalMsgNum += msgNum;
		DBG_LOG_APP_HFP( "[APP_HFP] HfpSco RHO Not Allow Msg Number:%d", 1, msgNum);
	}

	if((msgNum = APP_HfpImgMedia_GetMsgNumInQueue()))
	{
		totalMsgNum += msgNum;
		DBG_LOG_APP_HFP( "[APP_HFP] HfpImgMedia RHO Not Allow Msg Number:%d", 1, msgNum);
	}

	return (totalMsgNum + MSG_MessageSearchAllHandler((Handler)&gAppHfpMsgHandle));
}
