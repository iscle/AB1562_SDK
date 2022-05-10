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


#ifdef BLE_AIRApp
#include "..\SDAP\SDAP_Interface.h"
#endif

#include "bt.h"
#include "App_ResourceManager.h"
#include "App_Interface.h"
#include "App_State.h"
#include "App_Media.h"
#include "App_Hfp.h"
#include "APP_HfpState.h"
#include "App_LinkKey.h"
#include "APP_HfpIncomingCallMediaHandler.h"
#include "App_Nvram_Type.h"
#include "App_A2dp.h"
#include "hfp.h"
#include "Hfp_atCommand.h"
#include "a2dp.h"
#include "AudioDSP_StreamManager.h"
#include "AudioDSP_Registration.h"
#include "App_VolumeManage.h"
#include "APP_HfpScoHandler.h"
#include "App_EventOpcode.h"
#include "APP_AudioDriver.h"
#include "App_PowerOff.h"
#include "App_EventKeyHandler.h"
#include "app_hfpnvkey_nvstruc.h"
#include "App_HFPKey.h"
#include "App_A2dp_NVKeyHandler.h"

static U32 app_HFP_CheckTimer(Handler handler, U16 id, void *payload, U32 id_ext);
const HandlerData gHfpStateCheckTimer = {app_HFP_CheckTimer};

PUBLIC U8 APP_HFP_SearchCallStatusInd(BD_ADDR_T *pBdAddr, U8 callStatus)
{
	U8 callInd;
	APP_HFP_CTL_STRU * pHfpLinkInfo = APP_GetHfpLinkInfo(pBdAddr);

	for (callInd = 0; callInd < APP_MAX_ONGOING_CALL_NO; callInd++)
	{
		if (callStatus == APP_CALL_STATE_NUM)
		{
			if (pHfpLinkInfo->callState[callInd] != APP_CALL_EMPTY)
				break;
		}
		else
		{
			if (pHfpLinkInfo->callState[callInd] == callStatus)
				break;
		}
	}

	return callInd;
}


PUBLIC BOOL APP_HFP_TransitCallStatus(BD_ADDR_T *pBdAddr, U8 beforeStatus, U8 afterStatus)
{
	U8 callInd = APP_HFP_SearchCallStatusInd(pBdAddr, beforeStatus);
	APP_HFP_CTL_STRU * pHfpLinkInfo = APP_GetHfpLinkInfo(pBdAddr);

	if(callInd < APP_MAX_ONGOING_CALL_NO)
	{
		pHfpLinkInfo->callState[callInd] = afterStatus;
		if((afterStatus == APP_CALL_ACTIVE && beforeStatus != APP_CALL_HELD) || (afterStatus == APP_CALL_HELD && beforeStatus == APP_CALL_EMPTY))
		{
			pHfpLinkInfo->ongoingCallNo++;
		}
		else if((beforeStatus == APP_CALL_ACTIVE|| beforeStatus == APP_CALL_HELD) && afterStatus == APP_CALL_EMPTY)
		{
			pHfpLinkInfo->ongoingCallNo--;
		}
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

U8 APP_HFP_CountCallsInCallStatus(BD_ADDR_T *pBdAddr, U8 callStatus)
{
	U8 callInd, count = 0;
	APP_HFP_CTL_STRU * pHfpLinkInfo = APP_GetHfpLinkInfo(pBdAddr);

	if(pHfpLinkInfo == (APP_HFP_CTL_STRU *)NULL)
		return 0;

	for (callInd = 0; callInd < APP_MAX_ONGOING_CALL_NO; callInd++)
	{
		if (callStatus == APP_CALL_STATE_NUM)
		{
			if (pHfpLinkInfo->callState[callInd] != APP_CALL_EMPTY)
				count++;
		}
		else
		{
			if (pHfpLinkInfo->callState[callInd] == callStatus)
				count++;
		}
	}

	return count;
}

///////////////////////////////////////////////////////////////
//	MMI_Connected Related
///////////////////////////////////////////////////////////////
static void app_HFP_ConnectedHandler(BD_ADDR_T *pBdAddr, APP_MSG_TYPE_STRU *pMsg)//(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr)
{
	U8 state;
	APP_HFP_CTL_STRU *pHfpLinkInfo = APP_GetHfpLinkInfo(pBdAddr);

	gAppCtl.msgHandled = PROCD_RELEASE;
	state = APP_State_GetTopState(pBdAddr);
	switch(pMsg->eventCode)
	{
		case HFP_RING_IND:
			//HFP SPEC. 4.13.1 & 4.13.2 & 4.13.3
			if(state == APP_CONNECTED || (APP_State_IsInSystemAudioState() && APP_State_CheckNestStateByLink(pBdAddr, APP_CONNECTED) && !APP_State_CheckNestStateByLink(pBdAddr, APP_HFP_INCOMMING)))
			{
				APP_State_AddTopState(pBdAddr, APP_HFP_INCOMMING);
				if(APP_Hfp_IsConnected(pBdAddr))
				{
					APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_EMPTY, APP_CALL_INCOMMING);
				}
			}
			break;

		case HFP_CALLSETUP_IND:
			switch(pHfpLinkInfo->indicator.callsetup)
			{
				case CALLSETUP_INCOMMING:
					APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_EMPTY, APP_CALL_INCOMMING);
					//HFP SPEC. 4.13.1 & 4.13.2 & 4.13.3
					APP_State_AddTopState(pBdAddr, APP_HFP_INCOMMING);
					break;

				case CALLSETUP_NO_CALSETUP:
					APP_ImgMedia_AudioVoiceHandler(pBdAddr, IMG_AUDIO_VOICE_STOP);
					break;

				case CALLSETUP_REMOTE_ALERTED:
					APP_HFP_GetCallSetupRemoteAlert(pBdAddr, FALSE);
				case CALLSETUP_OUTGOING:
					APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_EMPTY, APP_CALL_OUTGOING);
					APP_State_AddTopState(pBdAddr, APP_HFP_OUTGOING);
					break;
			}
			break;

		case HFP_CALL_IND:
			//call indicator changes from 0 to 1
			if (pHfpLinkInfo->indicator.call == CALL_ACTIVE && !APP_State_CheckLinkInCallActiveState(pBdAddr))
			{
				APP_ImgMedia_AudioVoiceHandler(pBdAddr, IMG_AUDIO_VOICE_STOP);
				APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_EMPTY, APP_CALL_ACTIVE);

				APP_State_AddTopState(pBdAddr, (APP_SCO_EXISTS(pBdAddr))? APP_HFP_CALLACTIVE : APP_HFP_CALLACTIVE_WITHOUT_SCO);
				APP_Media_PushMediaEvent(MEDIA_EVT_CALL_CONNECTED);
			}
			break;

		case HFP_CONNECT_CFM:
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE            
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE            
>>>>>>> db20e11 (second commit)
			if(!APP_Hfp_IsHspConnected(pBdAddr))
#else
            if(1)
#endif                
			{
				/* redial link: first connected, first assigned */

				//!!! should check the call callsetup status to make state jump decision
				// coze in HSP, only incoming and call active, no difference in logic, so we put this here
				if (pHfpLinkInfo->indicator.call == CALL_NO_ACTIVE)
				{
					switch(pHfpLinkInfo->indicator.callsetup)
					{
						case CALLSETUP_INCOMMING:
							APP_State_AddTopState(pBdAddr, APP_HFP_INCOMMING);
							break;

						case CALLSETUP_REMOTE_ALERTED:
							APP_HFP_GetCallSetupRemoteAlert(pBdAddr, FALSE);
						case CALLSETUP_OUTGOING:
							APP_State_AddTopState(pBdAddr, APP_HFP_OUTGOING);
							break;
					}
				}
				else if(!APP_State_CheckLinkInCallRelatedState(pBdAddr))// pHfpLinkInfo->indicator.call == CALL_ACTIVE
				{
					if (APP_SCO_EXISTS(pBdAddr))
					{
						APP_State_AddTopState(pBdAddr, APP_HFP_CALLACTIVE);
					}
					else
					{
						APP_State_AddTopState(pBdAddr, APP_HFP_CALLACTIVE_WITHOUT_SCO);
						// Add this coze maybe AG doesnt init esting sco right after connection.
						APP_HFP_GetCallSetupRemoteAlert(pBdAddr, FALSE);
					}
					APP_Media_PushMediaEvent(MEDIA_EVT_CALL_CONNECTED);

					if (pHfpLinkInfo->indicator.callheld == CALLHELD_NO_CALLS_HELD)
					{
						switch(pHfpLinkInfo->indicator.callsetup)
						{
							case CALLSETUP_INCOMMING:
								APP_State_AddTopState(pBdAddr, APP_HFP_CAIMG);
								break;
							case CALLSETUP_OUTGOING:
								APP_State_AddTopState(pBdAddr, APP_HFP_CAOGG);
								break;
							default:
								break;
						}
					}
					else if(pHfpLinkInfo->indicator.callheld == CALLHELD_CALLS_HELD_ACTIVE)
					{
						switch(pHfpLinkInfo->indicator.callsetup)
						{
							case CALLSETUP_NO_CALSETUP:
								pHfpLinkInfo->callState[0] = APP_CALL_ACTIVE;
								pHfpLinkInfo->callState[1] = APP_CALL_HELD;
								pHfpLinkInfo->ongoingCallNo = 2;
								APP_State_AddTopState(pBdAddr, APP_HFP_CAMULTY);
								break;
							case CALLSETUP_INCOMMING:
								pHfpLinkInfo->ongoingCallNo = 1;
								APP_State_AddTopState(pBdAddr, APP_HFP_CAIMG);
								break;
							case CALLSETUP_OUTGOING:
								pHfpLinkInfo->ongoingCallNo = 1;
								APP_State_AddTopState(pBdAddr, APP_HFP_CAOGG);
								break;
						}
					}
				}
			}
			break;

		case APP_FAKE_MEDIA_EVT:
			if(state == APP_CONNECTED || (APP_State_IsInSystemAudioState() && APP_State_CheckNestStateByLink(pBdAddr, APP_CONNECTED)))
			{
				if(!FW_CmpBdAddr(pBdAddr, APP_GetRedialBdaddr()) || !APP_PowerOff_IsPowerOn())
					break;

				switch(pMsg->msgOpcode)
				{
					case MEDIA_EVT_KEY_RDIAL:
						APP_HFP_KeyRedial(pBdAddr);
						break;
					case MEDIA_EVT_KEY_SET_VOICE_RECOGNITION:
					case MEDIA_EVT_KEY_SET_VOICE_RECOGNITION_CANCEL:
						APP_HFP_KeyVoiceRecognition(pMsg->msgOpcode);
						break;
					case MEDIA_EVT_KEY_SIRI_START_OR_CONTINUE:
					case MEDIA_EVT_KEY_SIRI_END:
						APP_HFP_KeyAppleSiri(pMsg->msgOpcode);
						break;
				}
			}
			break;

		default:
			gAppCtl.msgHandled = NOTPROCD;
			break;
	}
}

///////////////////////////////////////////////////////////////
//	MMI_HFPImg Related
///////////////////////////////////////////////////////////////
static void app_HFP_ImgEntryFromCaImg(BD_ADDR_T *pBdAddr)
{
	APP_HFP_CTL_STRU * pHfpLinkInfo = APP_GetHfpLinkInfo(pBdAddr);

	pHfpLinkInfo->isImgVPTriggered = FALSE;

	if(!APP_PowerOff_IsPowerOn())
		return;

	//#ifdef AIRAPP_Profile
	//AirApp_EnterIncomingState(linkIndex);
	//#endif

	APP_HfpSco_EnableLightMonoMode(pBdAddr);

	if(!APP_SCO_EXISTS(pBdAddr))
	{
<<<<<<< HEAD
		#ifdef A2DP_Profile
=======
		#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
		if(APP_a2dp_is_in_streaming(pBdAddr))
		{
			if(APP_AudioDeviceRegister(pBdAddr, AUDIO_DEVICE_MUSIC_CALL_NOTIFY))
			{
				if(APP_A2dp_NVKeyIsFeatureOn(APP_A2DP_MEMPUT_WHEN_CALLACTIVE_FEAT))
				{
					APP_A2dpDspStop(pBdAddr);
				}
				else
				{
					APP_A2dp_DspPlay(pBdAddr);
				}
			}
		}
		#endif
	}
	else
	{
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
		if(APP_AudioDeviceRegister(pBdAddr, APP_Hfp_IsHspConnected(pBdAddr)? AUDIO_DEVICE_SCO_HSP : AUDIO_DEVICE_SCO))
#else
        if(APP_AudioDeviceRegister(pBdAddr, AUDIO_DEVICE_SCO))
#endif            
		{
			APP_HfpSco_SendDSPCmd(pBdAddr);
		}
	}
	APP_AudioDeviceRegister(pBdAddr, AUDIO_DEVICE_CALL_STATE);
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
	if(APP_State_CheckOtherLinksInCallRelatedState(pBdAddr) || APP_Hfp_IsFeatureOn(APP_PLAY_DEFAULT_RING_INCOMING_FEAT) || APP_Hfp_IsHspConnected(pBdAddr))
#else
    if(APP_State_CheckOtherLinksInCallRelatedState(pBdAddr) || APP_Hfp_IsFeatureOn(APP_PLAY_DEFAULT_RING_INCOMING_FEAT))
#endif
	{
		APP_VolManage_SetSpkMuteByLink(pBdAddr, SET_MUTE_ON);
	}

	APP_ImgMedia_AudioVoiceHandler(pBdAddr, IMG_AUDIO_VOICE_TRIGGER);

	if(APP_HfpNvkeyGetTimerValue(TIMER_ID_AUTO_ANSWER))
	{
		APP_HFP_SetTimer(pBdAddr, TIMER_ID_AUTO_ANSWER, (U32)APP_HfpNvkeyGetTimerValue(TIMER_ID_AUTO_ANSWER) * 100L);
	}

	APP_SetImgCallBdAddr(pBdAddr);
}

static void app_HFP_ImgEntry(BD_ADDR_T *pBdAddr)//(U8 linkIndex)
{
	APP_HFP_CTL_STRU * pHfpLinkInfo = APP_GetHfpLinkInfo(pBdAddr);

	pHfpLinkInfo->isCallerIdGet = FALSE;
	pHfpLinkInfo->isImgVPTriggered = FALSE;
	if(!APP_PowerOff_IsPowerOn())
		return;

	#ifdef AIRAPP_Profile
	//AirApp_EnterIncomingState(linkIndex);
	#endif

	APP_HfpSco_EnableLightMonoMode(pBdAddr);

	if(!APP_State_CheckOtherLinksInIncomingState(pBdAddr))
	{
		if(!APP_SCO_EXISTS(pBdAddr))
		{
<<<<<<< HEAD
			#ifdef A2DP_Profile
=======
			#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
			if(APP_a2dp_is_in_streaming(pBdAddr))
			{
				if(APP_AudioDeviceRegister(pBdAddr, AUDIO_DEVICE_MUSIC_CALL_NOTIFY))
				{
					if(APP_A2dp_NVKeyIsFeatureOn(APP_A2DP_MEMPUT_WHEN_CALLACTIVE_FEAT))
					{
						APP_A2dpDspStop(pBdAddr);
					}
					else
					{
						APP_A2dp_DspPlay(pBdAddr);
					}
				}
			}
			#endif
		}
		else
		{
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE		
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE		
>>>>>>> db20e11 (second commit)
			if(APP_AudioDeviceRegister(pBdAddr, APP_Hfp_IsHspConnected(pBdAddr) ? AUDIO_DEVICE_SCO_HSP : AUDIO_DEVICE_SCO))
#else
            if(APP_AudioDeviceRegister(pBdAddr, AUDIO_DEVICE_SCO)
#endif
			{
				APP_HfpSco_SendDSPCmd(pBdAddr);
			}
		}
	}
	APP_AudioDeviceRegister(pBdAddr, AUDIO_DEVICE_CALL_STATE);
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
	if(APP_State_CheckOtherLinksInCallRelatedState(pBdAddr) || APP_Hfp_IsFeatureOn(APP_PLAY_DEFAULT_RING_INCOMING_FEAT) || APP_Hfp_IsHspConnected(pBdAddr))
#else
    if(APP_State_CheckOtherLinksInCallRelatedState(pBdAddr) || APP_Hfp_IsFeatureOn(APP_PLAY_DEFAULT_RING_INCOMING_FEAT))
#endif        
	{
		APP_VolManage_SetSpkMuteByLink(pBdAddr, SET_MUTE_ON);
	}

	APP_ImgMedia_AudioVoiceHandler(pBdAddr, IMG_AUDIO_VOICE_TRIGGER);

	if(APP_HfpNvkeyGetTimerValue(TIMER_ID_AUTO_ANSWER))
	{
		APP_HFP_SetTimer(pBdAddr, TIMER_ID_AUTO_ANSWER, (U32)APP_HfpNvkeyGetTimerValue(TIMER_ID_AUTO_ANSWER) * 100L);
	}

	APP_SetImgCallBdAddr(pBdAddr);
}

static void app_HFP_ImgHandler(BD_ADDR_T *pBdAddr, APP_MSG_TYPE_STRU *pMsg)//(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr)
{
	APP_HFP_CTL_STRU *pHfpLinkInfo = APP_GetHfpLinkInfo(pBdAddr);

	gAppCtl.msgHandled = PROCD_RELEASE;

	switch(pMsg->eventCode)
	{
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
		case HFP_ATCMD_CKPD_200:
			APP_State_RmvState(pBdAddr, APP_HFP_INCOMMING);
			//Enter CallActive state after CKPD=200 has been sent.
			//There are 2 reasons to explain why it should enter CallActive state but not
			//back to Connected. One is [KEY1, TAP] shall be the key action that could disconnect
			//with the HSP link, and this action might not be HSP related command in Connected state.
			//Another one is Audio Transfer could be activated in that way.
			
			if (!APP_Hfp_IsFeatureOn(APP_NOT_CHG_STATE_ACP_HSP_RING_FEAT))
			{
				if (APP_SCO_EXISTS(pBdAddr))
				{
				 	APP_State_AddTopState(pBdAddr, APP_HFP_CALLACTIVE);
				}
			}         
			break;
#endif

		case HFP_CALL_IND:
			if (pHfpLinkInfo->indicator.call == CALL_ACTIVE)
			{
				//ISSUE: Bandit Handset may miss Tx callsetup = 0.-----------
				goto IMG_CALL_ACTIVE;
			}
			break;

		case HFP_CALLSETUP_IND:
			if (pHfpLinkInfo->indicator.callsetup == CALLSETUP_NO_CALSETUP)
			{
				if (pHfpLinkInfo->indicator.call == CALL_ACTIVE)
				{
					IMG_CALL_ACTIVE:
					//call indicator: 0 to 1, callsetup: 1 to 0
					APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_INCOMMING, APP_CALL_ACTIVE);

					//HFP SPEC. 4.13.1 & 4.13.2 & 4.13.3
					// jump to active state if we get both call = 1 & callsetup = 0
					APP_State_ReplaceStateWithEntry(pBdAddr, (APP_SCO_EXISTS(pBdAddr))? APP_HFP_CALLACTIVE : APP_HFP_CALLACTIVE_WITHOUT_SCO, APP_HFP_INCOMMING, TRUE);
					if(!APP_GetHfpIsRejectByKey(pBdAddr))
					{
						APP_Media_PushMediaEvent(MEDIA_EVT_CALL_CONNECTED);
					}
				}
				else
				{
					//call indicaotr doesnt change, callsetup from 1 to 0
					APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_INCOMMING, APP_CALL_EMPTY);
					//Mantis 9761 related
					if(!APP_GetHfpIsRejectByKey(pBdAddr))
					{
						APP_Media_PushMediaEvent(MEDIA_EVT_INCOMING_CALL_ENDED);
					}
					//HFP SPEC. 4.14.1 & 4.14.2
					APP_State_RmvState(pBdAddr, APP_HFP_INCOMMING);
				}
			}
			break;

		case APP_FAKE_MEDIA_EVT:
			switch(pMsg->msgOpcode)
			{
				case MEDIA_EVT_ENTER_INCOMING:
				case MEDIA_EVT_RINGTONE:
				case MEDIA_EVT_RINGTONE_IN_CALL:
				case MEDIA_EVT_RINGTONE2_IN_CALL:
				case MEDIA_EVT_RINGTONE_2:
				case MEDIA_EVT_CALLER_ID:
				case MEDIA_EVT_IMGCALL_MEDIA_NOT_FOUND:
					APP_ImgMedia_FakeEvtHandler(pBdAddr);
					break;
			}
			break;

		default:
			gAppCtl.msgHandled = NOTPROCD;
			break;
	}
}

static void app_HFP_ImgExit(BD_ADDR_T *pBdAddr)//(U8 linkIndex)
{
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	APP_ImgMedia_AudioVoiceHandler(pBdAddr, IMG_AUDIO_VOICE_STOP);

	APP_Media_PushMediaEvent(MEDIA_EVT_EXIT_INCOMING);

	APP_HfpSco_DisableLightMonoMode(pBdAddr);

	APP_HFP_ReleaseTimer(pBdAddr, TIMER_ID_AUTO_ANSWER);

	pLinkInfo->stateEntryExit |= APP_EXIT_IMGCALL_STATE;

	APP_VolManage_SetSpkMuteByLink(pBdAddr, SET_MUTE_OFF);

<<<<<<< HEAD
	#ifdef A2DP_Profile
=======
	#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
	if(APP_a2dp_is_in_streaming(pBdAddr))
	{
		APP_AudioDeviceDeRegister(pBdAddr, AUDIO_DEVICE_MUSIC_CALL_NOTIFY);
		APP_ResumeOtherAudioDevice(pBdAddr, AUDIO_DEVICE_MUSIC_CALL_NOTIFY);
	}
	#endif

	#ifdef AIRAPP_Profile
	//AirApp_ExitIncomingState(linkIndex);
	#endif
}

///////////////////////////////////////////////////////////////
//	MMI_HFPOgg Related
///////////////////////////////////////////////////////////////
static void app_HFP_OggEntry(BD_ADDR_T *pBdAddr)//(U8 linkIndex)
{
	APP_VolManage_SetSpkMuteByLink(pBdAddr, SET_MUTE_OFF);

	if(APP_HFP_CheckTimer(pBdAddr, TIMER_ID_AUDIO_DEV_ATCMD))
	{
		APP_AudioDeviceDeRegister(pBdAddr, AUDIO_DEVICE_KEY_AT_CMD);
		APP_ResumeOtherAudioDevice(pBdAddr, AUDIO_DEVICE_KEY_AT_CMD);
		APP_HFP_ReleaseTimer(pBdAddr, TIMER_ID_AUDIO_DEV_ATCMD);
	}

	if(APP_AudioDeviceRegister(pBdAddr, AUDIO_DEVICE_SCO)) //APP_Hfp_IsConnected(pBdAddr) ? AUDIO_DEVICE_SCO_HSP : AUDIO_DEVICE_SCO))
	{
		APP_HfpSco_SendDSPCmd(pBdAddr);
	}

	APP_AudioDeviceRegister(pBdAddr, AUDIO_DEVICE_CALL_STATE);

	APP_SetRedialBdAddr(pBdAddr);
}

static void app_HFP_OggHandler(BD_ADDR_T *pBdAddr, APP_MSG_TYPE_STRU *pMsg)//(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr)
{
	APP_HFP_CTL_STRU * pHfpLinkInfo = APP_GetHfpLinkInfo(pBdAddr);
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	gAppCtl.msgHandled = PROCD_RELEASE;
	switch(pMsg->eventCode)
	{
		case HFP_CALL_IND:
			if (pHfpLinkInfo->indicator.call == CALL_ACTIVE)
			{
				//ISSUE: Bandit Handset may miss Tx callsetup = 0.-----------
				goto OGG_CALL_ACTIVE;
			}
			break;

		//indicator "call" should be sent before callsetup. we might never go into this case
		//however, if "call" is not sent if remote rejects the call. just step into STANDBY state
		case HFP_CALLSETUP_IND:
			if (pHfpLinkInfo->indicator.callsetup == CALLSETUP_NO_CALSETUP)
			{
				if(pHfpLinkInfo->indicator.call == CALL_ACTIVE)
				{
					OGG_CALL_ACTIVE:
					//call indicator: 1 to 0, callsetup: 2 to 0
					APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_OUTGOING, APP_CALL_ACTIVE);

					//HFP SPEC. 4.18 & 4.19 & 4.20
					APP_State_ReplaceStateWithEntry(pBdAddr, (APP_SCO_EXISTS(pBdAddr)) ? APP_HFP_CALLACTIVE : APP_HFP_CALLACTIVE_WITHOUT_SCO, APP_HFP_OUTGOING, TRUE);
					if(!APP_GetHfpIsRejectByKey(pBdAddr))
					{
						APP_Media_PushMediaEvent(MEDIA_EVT_CALL_CONNECTED);
					}
				}
				else
				{
					//call indicator doesnt change, callsetup: 2 to 0
					APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_OUTGOING, APP_CALL_EMPTY);

					pLinkInfo->stateEntryExit |= APP_EXIT_OUTGOING_STATE;

					APP_State_RmvState(pBdAddr, APP_HFP_OUTGOING);
				}
			}
			else if(pHfpLinkInfo->indicator.callsetup == CALLSETUP_REMOTE_ALERTED)
			{
				APP_HFP_GetCallSetupRemoteAlert(pBdAddr, FALSE);
			}
			else if(pHfpLinkInfo->indicator.callsetup == CALLSETUP_INCOMMING) //BTA-3630 MI IOT issue
			{
				APP_State_ReplaceStateWithEntry(pBdAddr, APP_HFP_INCOMMING, APP_HFP_OUTGOING, TRUE);
			}
			else
			{
				gAppCtl.msgHandled = NOTPROCD;
			}
			break;

		default:
			gAppCtl.msgHandled = NOTPROCD;
			break;
	}
}

///////////////////////////////////////////////////////////////
//	MMI_HFPCa Related
///////////////////////////////////////////////////////////////
static void app_HFP_CaEntry(BD_ADDR_T *pBdAddr)//(U8 linkIndex)
{
	APP_ImgMedia_AudioVoiceHandler(pBdAddr, IMG_AUDIO_VOICE_STOP);

	if(APP_AudioDeviceRegister(pBdAddr, AUDIO_DEVICE_SCO))
	{
		APP_HfpSco_SendDSPCmd(pBdAddr);
	}

	if (APP_AUTO_UNMUTE_ENTER_OR_EXIT_CALL_FEAT)
	{
		if(APP_IsDSPInSCO(pBdAddr))
		{
			APP_VolManage_AutoMICMuteOff(pBdAddr);
		}
		APP_VolManage_MicMuteHandler(pBdAddr, SET_MUTE_OFF);
	}

	APP_VolManage_MicVolumeRemoteHandler(pBdAddr, APP_VolManage_GetCurrentScoVgm(pBdAddr), COMPONENT_MIC_SCO);

	/*if(HFP_GetLinkInfo(linkIndex)->currentCmd == HFP_ATCMD_INIT_VOICE_DIAL)		TO DO
	{	//Mantis Issue #6065
		SYS_SetTimer(&HFP_GetLinkInfo(linkIndex)->ost_command_ptr, HALF_SEC);
	}*/
	APP_AudioDeviceRegister(pBdAddr, AUDIO_DEVICE_CALL_STATE);
}

static void app_HFP_ClearCallStatus(BD_ADDR_T *pBdAddr)
{
	U8 callInd;
	APP_HFP_CTL_STRU * pHfpLinkInfo = APP_GetHfpLinkInfo(pBdAddr);

	//no active call, clear the call control table info
	for (callInd = 0; callInd < APP_MAX_ONGOING_CALL_NO; callInd++)
	{
		pHfpLinkInfo->callState[callInd] = APP_CALL_EMPTY;
		pHfpLinkInfo->ongoingCallNo = 0;
	}
}

static void app_HFP_CaStateCallHeldOnHeldNoActive(BD_ADDR_T *pBdAddr)
{
	BD_ADDR_T *activateLink;
	BD_ADDR_T *nextBdAddr;

	if((nextBdAddr = APP_Hfp_GetNextHfpLink(pBdAddr)) != APP_EOF)
	{
		activateLink = pBdAddr;
		if(APP_State_CheckLinkInCallNotifyState(nextBdAddr))
		{
			if(APP_SUPPORT_MULTI_HFP_FEAT)
			{
				APP_HFP_SendATCmdByLink(nextBdAddr, HFP_ATCMD_ACCEPT_CALL);
				APP_AudioDriver_SendVoicepromptStopCmd();
			}
			activateLink = nextBdAddr;
		}
		else if(APP_State_CheckLinkInCallActiveState(nextBdAddr))
		{
			if(APP_MAX_ONGOING_CALL_NO == APP_HFP_SearchCallStatusInd(nextBdAddr, APP_CALL_HELD))
			{
				activateLink = nextBdAddr ;
			}
			else
			{
				if(APP_SUPPORT_MULTI_HFP_FEAT)
				{
					APP_HFP_SendATCmdByLink(nextBdAddr, HFP_ATCMD_3WAY_HOLDNACP);
					#ifdef _HFP_CALLHELD_PREDICT_
					APP_HFP_TransitCallStatus(nextBdAddr, APP_CALL_HELD, APP_CALL_ACTIVE);
					#endif
					activateLink = pBdAddr;
				}
			}
		}

		if(!FW_CmpBdAddr(activateLink, pBdAddr))
		{
			if(APP_SCO_EXISTS(activateLink))
				APP_ResumeOtherAudioDevice(pBdAddr, AUDIO_DEVICE_SCO);
			else
				APP_ResumeOtherAudioDevice(pBdAddr, AUDIO_DEVICE_CALL_STATE);
		}
		else if(APP_GetCurrentAudioDevice() == AUDIO_DEVICE_SCO && !APP_IsDSPActive(pBdAddr))
		{
			APP_ResumeOtherAudioDevice(APP_GetCurrentAudioBdAddr(), AUDIO_DEVICE_SCO);
		}
	}
}

static void app_HFP_CaStateCallHeldNoCallsHeld(BD_ADDR_T *pBdAddr)
{
	BD_ADDR_T *nextBdAddr;
	BD_ADDR_T *activateLink;

	if((nextBdAddr = APP_Hfp_GetNextHfpLink(pBdAddr)) != APP_EOF)
	{
		activateLink = pBdAddr;
		if(APP_State_CheckLinkInOutGoingState(nextBdAddr))
		{
			activateLink = nextBdAddr;
		}
		else if(APP_State_CheckLinkInIncomingState(nextBdAddr))
		{
			activateLink = pBdAddr;
		}
		else if(APP_State_CheckLinkInCallActiveState(nextBdAddr))
		{
			if(APP_MAX_ONGOING_CALL_NO == APP_HFP_SearchCallStatusInd(nextBdAddr, APP_CALL_HELD))
				activateLink = nextBdAddr ;
			else
				activateLink = pBdAddr;
		}

		if(!FW_CmpBdAddr(activateLink, pBdAddr))
		{
			APP_ResumeOtherAudioDevice(pBdAddr, AUDIO_DEVICE_SCO);
		}
		else if(APP_GetCurrentAudioDevice() == AUDIO_DEVICE_SCO && !APP_IsDSPActive(pBdAddr))
		{
			APP_ResumeOtherAudioDevice(APP_GetCurrentAudioBdAddr(), AUDIO_DEVICE_SCO);
		}
	}
}

static void app_HFP_CaHandler(BD_ADDR_T *pBdAddr, APP_MSG_TYPE_STRU *pMsg)//(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr)
{
	APP_HFP_CTL_STRU * pHfpLinkInfo = APP_GetHfpLinkInfo(pBdAddr);

	gAppCtl.msgHandled = PROCD_RELEASE;

	switch(pMsg->eventCode)
	{
		case HFP_CALL_IND:
			if (pHfpLinkInfo->indicator.call == CALL_NO_ACTIVE)
			{
				app_HFP_ClearCallStatus(pBdAddr);

				if(!APP_GetHfpIsRejectByKey(pBdAddr))
				{
					APP_Media_PushMediaEvent(MEDIA_EVT_END_OF_CALL);
				}
				//HFP SPEC. 4.15.1 & 4.15.2
				if (APP_State_GetTopState(pBdAddr) == APP_HFP_CALLACTIVE)
				{
					APP_State_RmvState(pBdAddr, APP_HFP_CALLACTIVE);
				}
				else if (APP_State_GetTopState(pBdAddr) == APP_HFP_CALLACTIVE_WITHOUT_SCO)
				{
					APP_State_RmvState(pBdAddr, APP_HFP_CALLACTIVE_WITHOUT_SCO);
				}
				APP_AudioDeviceDeRegister(pBdAddr, AUDIO_DEVICE_CALL_STATE);
			}
			break;

		case HFP_CALLSETUP_IND:
			if (HFP_IsSupport3WayCalling(pBdAddr))
			{
				switch(pHfpLinkInfo->indicator.callsetup)
				{
					case CALLSETUP_INCOMMING:
						APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_EMPTY, APP_CALL_INCOMMING);
						//HFP SPEC. 4.22.1
						APP_State_AddTopState(pBdAddr, APP_HFP_CAIMG);
						break;

					case CALLSETUP_REMOTE_ALERTED:
						APP_HFP_GetCallSetupRemoteAlert(pBdAddr, TRUE);
					case CALLSETUP_OUTGOING:
						APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_EMPTY, APP_CALL_OUTGOING);
						//HFP SPEC. 4.22.2
						APP_State_AddTopState(pBdAddr, APP_HFP_CAOGG);
						break;

					case CALLSETUP_NO_CALSETUP:
						//no need to process this case
						gAppCtl.msgHandled = NOTPROCD;
						break;
				}
			}
			break;

		case HFP_CALLHELD_IND:
			switch(pHfpLinkInfo->indicator.callheld)
			{
				case CALLHELD_ON_HOLD_NOACTIVE:
					APP_HFP_CallheldIndicationHandler_ForNoActiveCalls_HF(pBdAddr);
					break;
				case CALLHELD_CALLS_HELD_ACTIVE:
					/*
					 * added to fix issue #90
					 * in call active state, an outgoing call is accepted by the remote. however callheld = 1 is sent after callsetup = 0
					 * to fix - callheld =0 will be received in call active state, add a new active call
					 */
					if (APP_HFP_SearchCallStatusInd(pBdAddr, APP_CALL_ACTIVE) == 0)
					{
						APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_ACTIVE, APP_CALL_HELD);
					}
					 APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_EMPTY, APP_CALL_ACTIVE);
					//HFP SPEC. 4.18 & 4.19 & 4.20
					APP_State_AddTopState(pBdAddr, APP_HFP_CAMULTY);
					break;
				case CALLHELD_NO_CALLS_HELD:
					APP_HFP_CallheldIndicationHandler_ForNoCallsHeld_HF(pBdAddr);
					break;
			}
			break;

		case HFP_AT_TERMINATE_CALL_CFM:
			if(pHfpLinkInfo->ongoingCallNo)
			{
				if(APP_HFP_SearchCallStatusInd(pBdAddr,APP_CALL_ACTIVE) < APP_MAX_ONGOING_CALL_NO)
					APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_ACTIVE, APP_CALL_EMPTY);
				else
					APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_HELD, APP_CALL_EMPTY);
			}
			break;

		default:
			gAppCtl.msgHandled = NOTPROCD;
			break;
	}
}

static void app_HFP_CaExit(BD_ADDR_T *pBdAddr)//(U8 linkIndex)
{
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);
	if (APP_AUTO_UNMUTE_ENTER_OR_EXIT_CALL_FEAT)
	{
		if(APP_IsDSPInSCO(pBdAddr))
		{
			APP_VolManage_AutoMICMuteOff(pBdAddr);
		}
		APP_VolManage_MicMuteHandler(pBdAddr, SET_MUTE_OFF);
	}

	if(pLinkInfo->stateEntryExit & APP_EXIT_IMGCALL_STATE && !APP_GetHfpIsRejectByKey(pBdAddr))
		pLinkInfo->stateEntryExit &= ~APP_EXIT_IMGCALL_STATE;

	if(!APP_GetHfpIsRejectByKey(pBdAddr))
		pLinkInfo->stateEntryExit |= APP_EXIT_CALLACTIVE_STATE;
}

///////////////////////////////////////////////////////////////
//	MMI_HFPCaImg Related
///////////////////////////////////////////////////////////////
static void app_HFP_CaImgEntry(BD_ADDR_T *pBdAddr)//(U8 linkIndex)
{
	APP_HFP_CTL_STRU * pHfpLinkInfo = APP_GetHfpLinkInfo(pBdAddr);

	pHfpLinkInfo->isImgVPTriggered = FALSE;

	if(!APP_PowerOff_IsPowerOn())
		return;

	APP_ImgMedia_AudioVoiceHandler(pBdAddr, IMG_AUDIO_VOICE_TRIGGER);

	APP_SetImgCallBdAddr(pBdAddr);

	APP_AudioDeviceRegister(pBdAddr, AUDIO_DEVICE_CALL_STATE);
}

static void app_HFP_CaImgHandler(BD_ADDR_T *pBdAddr, APP_MSG_TYPE_STRU *pMsg)//(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr)
{
	APP_HFP_CTL_STRU * pHfpLinkInfo = APP_GetHfpLinkInfo(pBdAddr);

	gAppCtl.msgHandled = PROCD_RELEASE;

	switch(pMsg->eventCode)
	{
		case HFP_CALL_IND:
			//call = 0 then call = 1 might be sent if call waiting happens and chld = 1 is sent by AG
			//no need to process this message but it is not allowed to pass throught. it will not influence MMI state
			if (pHfpLinkInfo->indicator.call == CALL_ACTIVE)
			{
				APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_INCOMMING, APP_CALL_ACTIVE);
			}
			else
			{
				APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_ACTIVE, APP_CALL_EMPTY);
				APP_State_RmvState(pBdAddr, APP_HFP_CAIMG);
				//mantis 8867: Ca+Img --> Call drop --> no ring in Img: to cover call = 0 and then call = 1 are sent in CaImgState
				APP_State_ReplaceStateWithEntry(pBdAddr, APP_HFP_INCOMMING, APP_State_GetTopState(pBdAddr), FALSE);
				//APP_Media_PushMediaEvent(MEDIA_EVT_END_OF_CALL); // mantis 8831
				//mantis 8867: Ca+Img --> Call drop --> no ring in Img: to cover call = 0 and then call = 1 are sent in CaImgState

				APP_HFP_SetTimer(pBdAddr, TIMER_ID_WAIT_IMG_RMV_CALL_TRANSIT, HALF_SEC);

				gAppCtl.msgHandled = NOTPROCD;
			}
			break;

		case HFP_CALLHELD_IND:
			APP_State_RmvState(pBdAddr, APP_HFP_CAIMG);
			switch(pHfpLinkInfo->indicator.callheld)
			{
				case CALLHELD_CALLS_HELD_ACTIVE:
					APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_ACTIVE, APP_CALL_HELD);
					APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_INCOMMING, APP_CALL_ACTIVE);
					APP_State_AddTopState(pBdAddr, APP_HFP_CAMULTY);
					APP_Media_PushMediaEvent(MEDIA_EVT_CALL_CONNECTED);
					break;

				case CALLHELD_NO_CALLS_HELD:
					APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_HELD, APP_CALL_ACTIVE);
					APP_State_AddTopState(pBdAddr, (APP_SCO_EXISTS(pBdAddr)) ? APP_HFP_CALLACTIVE : APP_HFP_CALLACTIVE_WITHOUT_SCO);
					break;
				case CALLHELD_ON_HOLD_NOACTIVE:
					APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_ACTIVE, APP_CALL_HELD);
					APP_State_AddTopState(pBdAddr, (APP_SCO_EXISTS(pBdAddr)) ? APP_HFP_CALLACTIVE : APP_HFP_CALLACTIVE_WITHOUT_SCO);
					break;
			}
			break;

		case HFP_CALLSETUP_IND:
			if (pHfpLinkInfo->indicator.callsetup == CALLSETUP_NO_CALSETUP)
			{
				APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_INCOMMING, APP_CALL_EMPTY);
				APP_State_RmvState(pBdAddr, APP_HFP_CAIMG);
			}
			break;

		case APP_FAKE_MEDIA_EVT:
			switch(pMsg->msgOpcode)
			{
				case MEDIA_EVT_ENTER_INCOMING:
				case MEDIA_EVT_RINGTONE:
				case MEDIA_EVT_RINGTONE_IN_CALL:
				case MEDIA_EVT_RINGTONE2_IN_CALL:
				case MEDIA_EVT_RINGTONE_2:
				case MEDIA_EVT_CALLER_ID:
				case MEDIA_EVT_IMGCALL_MEDIA_NOT_FOUND:
					APP_ImgMedia_FakeEvtHandler(pBdAddr);
					break;
			}
			break;

		default:
			gAppCtl.msgHandled = NOTPROCD;
			break;
	}
}

///////////////////////////////////////////////////////////////
//	MMI_HFPCaOgg Related
///////////////////////////////////////////////////////////////
static void app_HFP_CaOggEntry(BD_ADDR_T *pBdAddr)//(U8 linkIndex)
{
	if(!APP_PowerOff_IsPowerOn())
		return;
	APP_AudioDeviceRegister(pBdAddr, AUDIO_DEVICE_CALL_STATE);
}

static void app_HFP_CaOggHandler(BD_ADDR_T *pBdAddr, APP_MSG_TYPE_STRU *pMsg)//(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr)
{
	APP_HFP_CTL_STRU * pHfpLinkInfo = APP_GetHfpLinkInfo(pBdAddr);

	gAppCtl.msgHandled = PROCD_RELEASE;

	switch(pMsg->eventCode)
	{
		case HFP_CALL_IND:
			if(pHfpLinkInfo->indicator.call == CALL_ACTIVE)
			{
				//Some AGs send Call = 1 then Callsetup = 0 while remote party answer
				//the phone call. However, all AGs send Callsetup = 0 to indicate the end of
				//this outgoing call, that means adding state 'MultiActiveCall' when we receive
				//Callsetup = 0 is not going to work. To get into state MultiActiveCall, we should
				//regard Call = 1 as a signal for devices such as Samsung C3050c & MTK (Golden).
				APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_OUTGOING, APP_CALL_ACTIVE);
			}
			else
			{
				APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_ACTIVE, APP_CALL_EMPTY);
				APP_State_RmvState(pBdAddr, APP_HFP_CAOGG);
				APP_State_ReplaceStateWithEntry(pBdAddr, APP_HFP_OUTGOING, APP_State_GetTopState(pBdAddr), FALSE);
				//APP_Media_PushMediaEvent(MEDIA_EVT_END_OF_CALL); // mantis 8831

				gAppCtl.msgHandled = NOTPROCD;
			}
			break;

		case HFP_CALLHELD_IND:
			APP_State_RmvState(pBdAddr, APP_HFP_CAOGG);
			switch(pHfpLinkInfo->indicator.callheld)
			{
				case CALLHELD_CALLS_HELD_ACTIVE:
					APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_ACTIVE, APP_CALL_HELD);
					APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_OUTGOING, APP_CALL_ACTIVE);
					APP_State_AddTopState(pBdAddr, APP_HFP_CAMULTY);
					break;

				case CALLHELD_NO_CALLS_HELD:
					APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_HELD, APP_CALL_ACTIVE);
					APP_State_AddTopState(pBdAddr, (APP_SCO_EXISTS(pBdAddr)) ? APP_HFP_CALLACTIVE : APP_HFP_CALLACTIVE_WITHOUT_SCO);
					break;
				case CALLHELD_ON_HOLD_NOACTIVE:
					APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_ACTIVE, APP_CALL_HELD);
					APP_State_AddTopState(pBdAddr, (APP_SCO_EXISTS(pBdAddr)) ? APP_HFP_CALLACTIVE : APP_HFP_CALLACTIVE_WITHOUT_SCO);
					break;
			}
			break;



		case HFP_CALLSETUP_IND:
			if (pHfpLinkInfo->indicator.callsetup == CALLSETUP_NO_CALSETUP)
			{
				APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_OUTGOING, APP_CALL_EMPTY);
				APP_State_RmvState(pBdAddr, APP_HFP_CAOGG);
			}
			break;

		case HFP_AT_TERMINATE_CALL_CFM:
			APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_OUTGOING, APP_CALL_EMPTY);
			APP_State_RmvState(pBdAddr, APP_HFP_CAOGG);
			break;

		default:
			gAppCtl.msgHandled = NOTPROCD;
			break;
	}
}

///////////////////////////////////////////////////////////////
//	MMI_HFPCaMulty Related
///////////////////////////////////////////////////////////////
static void app_HFP_CaMultyEntry(BD_ADDR_T *pBdAddr)//(U8 linkIndex)
{
	if(!APP_PowerOff_IsPowerOn())
		return;

	APP_VolManage_MicVolumeRemoteHandler(pBdAddr, APP_VolManage_GetCurrentScoVgm(pBdAddr), COMPONENT_MIC_SCO);

	APP_AudioDeviceRegister(pBdAddr, AUDIO_DEVICE_CALL_STATE);
}

static void app_HFP_CaMultyHandler(BD_ADDR_T *pBdAddr, APP_MSG_TYPE_STRU *pMsg)//(U8 linkIndex, MMIMsgType XDATA_PTR msgPtr)
{
	APP_HFP_CTL_STRU * pHfpLinkInfo = APP_GetHfpLinkInfo(pBdAddr);

	gAppCtl.msgHandled = PROCD_RELEASE;
	switch(pMsg->eventCode)
	{
		case HFP_CALL_IND:
			if (pHfpLinkInfo->indicator.call == CALL_NO_ACTIVE)
			{
				APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_ACTIVE, APP_CALL_EMPTY);
				APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_ACTIVE, APP_CALL_EMPTY);

				//HFP SPEC. 4.15.1 & 4.15.2
				APP_State_RmvState(pBdAddr, APP_HFP_CAMULTY);
				APP_State_RmvState(pBdAddr, APP_HFP_CALLACTIVE);	//temporarily modified for issue 6380
				APP_State_RmvState(pBdAddr, APP_HFP_CALLACTIVE_WITHOUT_SCO);	//modified for issue 9995

				if(!APP_GetHfpIsRejectByKey(pBdAddr))
				{
					APP_Media_PushMediaEvent(MEDIA_EVT_END_OF_CALL);
				}

				APP_AudioDeviceDeRegister(pBdAddr, AUDIO_DEVICE_CALL_STATE);
				//pass down to call active
				gAppCtl.msgHandled = NOTPROCD;
			}
			break;
		case HFP_CALLHELD_IND:

			switch(pHfpLinkInfo->indicator.callheld)
			{
				case CALLHELD_NO_CALLS_HELD:
					APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_HELD, APP_CALL_EMPTY);
					//HFP SPEC. 4.15.1 & 4.15.2
					//check the active call no
					//go back to CALLACTIVE if call no is 1
					APP_State_RmvState(pBdAddr, APP_HFP_CAMULTY);
					break;
				case CALLHELD_CALLS_HELD_ACTIVE:
					if (APP_HFP_SearchCallStatusInd(pBdAddr, APP_CALL_ACTIVE) == 0)
					{
						APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_HELD, APP_CALL_ACTIVE);
						APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_ACTIVE, APP_CALL_HELD);
					}
					else
					{
						APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_ACTIVE, APP_CALL_HELD);
						APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_HELD, APP_CALL_ACTIVE);
					}
					break;
				case CALLHELD_ON_HOLD_NOACTIVE:
					APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_ACTIVE, APP_CALL_EMPTY);
					APP_State_RmvState(pBdAddr, APP_HFP_CAMULTY);
					break;
			}
			break;
		case HFP_AT_TERMINATE_CALL_CFM:
			APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_ACTIVE, APP_CALL_EMPTY);
			break;
		case HFP_CALLSETUP_IND:
			//For special case: Some AGs may not send any result code, if AG answers the second call.
			//Finally, we should receive HFP_CIEVIND_CALLSETUP=CALLSETUP_NO_CALSETUP in ca+incomming state.
			//But this solution will have a side effect(ISSUE:0000192), we need to check
			// incomming & outgoing events in multy-call state.
			/*
			switch(gMMI_HFP_ctl.HfpCtl[linkIndex].indicator.callsetup)
			{
				case CALLSETUP_INCOMMING:
					//HFP SPEC. 4.22.1
					MMI_RmvState(linkIndex, MMI_HFP_CAMULTY);
					MMI_AddTopState(linkIndex, MMI_HFP_CAIMG);



					gMMI_HFP_ctl.HfpCtl[linkIndex].callCtrlTbl[APP_HFP_SearchCallStatusInd(linkIndex,MMI_CALL_EMPTY)].callStatus = MMI_CALL_INCOMMING;
					gMMI_HFP_ctl.HfpCtl[linkIndex].ongoingCallNo--;

					break;

				case CALLSETUP_OUTGOING:
					//HFP SPEC. 4.22.2
					//!!! maybe we need to check the indicator "callheld"
					MMI_RmvState(linkIndex, MMI_HFP_CAMULTY);
					MMI_AddTopState(linkIndex, MMI_HFP_CAOGG);

					gMMI_HFP_ctl.HfpCtl[linkIndex].callCtrlTbl[APP_HFP_SearchCallStatusInd(linkIndex,MMI_CALL_EMPTY)].callStatus = MMI_CALL_OUTGOING;
					gMMI_HFP_ctl.HfpCtl[linkIndex].ongoingCallNo--;

					break;

				case CALLSETUP_REMOTE_ALERTED:
					MMI_HFP_GetCallSetupRemoteAlert(linkIndex, TRUE);
				case CALLSETUP_NO_CALSETUP:
					//no need to process this case
					gMMI_ctl.msgHandled = NOTPROCD;
					break;
			} */
			break;
		default:
			gAppCtl.msgHandled = NOTPROCD;
			break;
	}
}

///////////////////////////////////////////////////////////////
//	MMI Handler Tables
///////////////////////////////////////////////////////////////
APP_STATE_HANDLER_SET_TYPE_STRU const gAppStateHFPConnectedHandler =
{
	APP_State_ConnectedEntry, app_HFP_ConnectedHandler, APP_State_ConnectedExit, //MMI_CONNECTED
};

APP_STATE_HANDLER_SET_TYPE_STRU const gAppStateHFPImgHandler =
{
	app_HFP_ImgEntry, app_HFP_ImgHandler, app_HFP_ImgExit, //MMI_HFP_INCOMMING
};

APP_STATE_HANDLER_SET_TYPE_STRU const gAppStateHFPOggHandler =
{
	app_HFP_OggEntry, app_HFP_OggHandler, NULL, //MMI_HFP_OUTGOING
};

APP_STATE_HANDLER_SET_TYPE_STRU const gAppStateHFPCaHandler =
{
	app_HFP_CaEntry, app_HFP_CaHandler, app_HFP_CaExit, //MMI_HFP_CALLACTIVE
};

APP_STATE_HANDLER_SET_TYPE_STRU const gAppStateHFPCaImgHandler =
{
	app_HFP_CaImgEntry, app_HFP_CaImgHandler, app_HFP_ImgExit, //MMI_HFP_CAIMG
};

APP_STATE_HANDLER_SET_TYPE_STRU const gAppStateHFPCaOggHandler =
{
	app_HFP_CaOggEntry, app_HFP_CaOggHandler, NULL, //MMI_HFP_CAOGG
};

APP_STATE_HANDLER_SET_TYPE_STRU const gAppStateHFPCaMultyHandler =
{
	app_HFP_CaMultyEntry, app_HFP_CaMultyHandler, NULL, //MMI_HFP_CAMULTY
};


#ifdef BLE_AIRApp
static BOOL app_HFP_IsBleAppCallerNameOn(U8 linkIndex)
{
	if(AirApp_BleIsConnected() && (ATT_FindServerIndexByAddr((U8 CODE_PTR)&AppCtl.phoneNumber) != 0) && ((((ATT_GetCCCDByLink(ATT_FindServerIndexByAddr((U8 CODE_PTR)AppCtl.phnoeNumber_CCCD), 0)) >> 8) & 0x01) == 0x01) && APP_SCO_EXISTS(linkIndex))
	{
		return TRUE;
	}
	return FALSE;
}
#endif

BD_ADDR_T *APP_HFP_SwitchLinkToNextHFPLink(BD_ADDR_T *pBdAddr)
{
	BD_ADDR_T *nextActiveBdAddr;
	if((nextActiveBdAddr = APP_GetNextNormalLink(pBdAddr)) != APP_EOF)
	{
		if(HFP_IsConnected(nextActiveBdAddr))
			return nextActiveBdAddr;
	}
	return APP_EOF;
}

void APP_HFP_SendEvtToStateHandle(BD_ADDR_T *pBdAddr, U16 evtCode)
{
	APP_State_SendMsgToHandler(pBdAddr, evtCode, evtCode, TRUE);
}

PUBLIC void APP_HFP_CallheldIndicationHandler_ForNoActiveCalls_HF(BD_ADDR_T *pBdAddr)
{
	if (!APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_ACTIVE, APP_CALL_HELD))
	{
		app_HFP_CaStateCallHeldOnHeldNoActive(pBdAddr);
	}
}

PUBLIC void APP_HFP_CallheldIndicationHandler_ForNoCallsHeld_HF(BD_ADDR_T *pBdAddr)
{
	if(APP_HFP_SearchCallStatusInd(pBdAddr, APP_CALL_HELD) < APP_MAX_ONGOING_CALL_NO)
	{
		if (!APP_HFP_TransitCallStatus(pBdAddr, APP_CALL_HELD, APP_CALL_ACTIVE))
		{
			app_HFP_CaStateCallHeldNoCallsHeld(pBdAddr);
		}
	}
}

static U32 app_HFP_CheckTimer(Handler handler, U16 id, void *payload, U32 id_ext)
{
	APP_HFP_CTL_STRU * pHfpLinkInfo;
	BD_ADDR_T *pBdAddr;
	UNUSED(handler);
	UNUSED(payload);

	if(!id_ext)
		return 0;

	pHfpLinkInfo = (APP_HFP_CTL_STRU *)id_ext;
	pBdAddr = APP_GetBdAddrByHfpLinkInfo(pHfpLinkInfo);

	if(!pBdAddr)
		return 0;

	switch(id)
	{
		case TIMER_ID_EST_SCO_AFTER_OGGCALL:
			if(APP_State_CheckLinkInCallRelatedState(pBdAddr))
			{
				APP_HFP_CreateAudioConnection(pBdAddr, TRUE);
			}
			break;
		case TIMER_ID_AUTO_ANSWER:
			if(!APP_State_CheckOtherLinksInCallActiveState(pBdAddr) && !APP_State_CheckOtherLinksInOutGoingState(pBdAddr))
			{
				APP_EvtKey_KeyEventHandler(pBdAddr, KEY_ACPCALL);
			}
			break;
		case TIMER_ID_AUDIO_TRANSFER_DETACH:
			break;
		case TIMER_ID_NETWORK_SERVICE_INDICATION_REPEAT:
			if(pHfpLinkInfo->indicator.service)
			{
				APP_Media_PushMediaEvent(MEDIA_EVT_NETWORK_PRESENT);
			}
			else
			{
				APP_Media_PushMediaEvent(MEDIA_EVT_NETWORK_NOT_PRESENT);
			}
			APP_HFP_SetTimer(pBdAddr, TIMER_ID_NETWORK_SERVICE_INDICATION_REPEAT, APP_HfpNvkeyGetTimerValue(TIMER_ID_NETWORK_SERVICE_INDICATION_REPEAT) * ONE_SEC);
			break;

		case TIMER_ID_REJECT_CALL_MUTE_SCO:
			//if(APP_Hfp_IsFeatureOn(APP_PLAY_DEFAULT_RING_INCOMING_FEAT))
			//{
			//	MMI_DRV_RejectCallMuteSCOCmd(FALSE);
			//}
			break;
		case TIMER_ID_WAIT_IMG_RMV_CALL_TRANSIT:
			if(APP_HFP_INCOMMING == APP_State_GetTopState(pBdAddr))
				app_HFP_ImgEntryFromCaImg(pBdAddr);
			break;
		case TIMER_ID_AUDIO_DEV_ATCMD:
			APP_AudioDeviceDeRegister(pBdAddr, AUDIO_DEVICE_KEY_AT_CMD);
			APP_ResumeOtherAudioDevice(pBdAddr, AUDIO_DEVICE_KEY_AT_CMD);
			break;
	}
	return 0;
}

//static void app_HFP_AudioTransferDetachHandle(BD_ADDR_T *pBdAddr)
//{
//	UNUSED(pBdAddr);
//	if(APP_PowerOff_IsPowerOn())
//	{
//		// NOTE!!!!This shloud be prior than MMI_HFP_ReleaseConnectionTimer		TO DO
//		if (APP_HFP_CheckTimer(pBdAddr, TIMER_ID_AUDIO_TRANSFER_DETACH))
//		{
//			//gMMI_ctl.miscMask2 |= MMI_CREATE_CONN_AFTER_AUDIO_TRANSFER;
//		}
//		else
//		{
//			//gMMI_ctl.miscMask2 &= ~MMI_CREATE_CONN_AFTER_AUDIO_TRANSFER;
//		}
//	}
//	APP_HFP_ReleaseTimer(pBdAddr, TIMER_ID_AUDIO_TRANSFER_DETACH);
//}

PUBLIC void APP_HFP_Init_HF(void)
{
	U8 linkIndex;

	for(linkIndex = 0; linkIndex < MAX_MULTI_POINT_NO; linkIndex++)
	{
		#ifdef _EXTENSION_ENTITIES_
		gMMI_HFP_ctl.HfpCtl[linkIndex].DTMFPara = '0';
		#endif
		#ifdef _MEMORY_DIALING_
		gMMI_HFP_ctl.HfpCtl[linkIndex].memoryDialIndex = 1; //[0-65535]
		#endif
	}
}

