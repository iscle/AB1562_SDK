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
#include "App_EventOpcode.h"
#include "App_Nvram_Type.h"
#include "BtHfp.h"
#include "App_EventKeyHandler.h"
#include "App_ResourceManager.h"
#include "APP_HfpScoHandler.h"
#include "App_Hfp.h"
#include "App_State.h"
#include "APP_HfpState.h"
#include "App_HFPKey.h"
#include "App_PowerOff.h"
#include "App_System.h"

/**************************************************************************************************
* Prototype
**************************************************************************************************/


/**************************************************************************************************
* Variables
**************************************************************************************************/
/*
static APP_KEY_EVT_STRU CODE APP_HFP_KeyEvtHandler [HFP_KEY_EVENT_NO] =
{
	{APP_HFP_KeyAudioTransfer}, 			//HFP_KEY_EVENT_KEY_AUDIO_TRANSFER
	{NULL}, 								//HFP_KEY_EVENT_KEY_SET_VOICE_RECOGNITION 					//Wait until voice prompt is over
	{NULL}, 								//HFP_KEY_EVENT_KEY_RDIAL									//Wait until voice prompt is over
	{APP_HFP_KeyDialMemory}, 				//HFP_KEY_EVENT_KEY_DIALMEM
	{APP_HFP_KeyDialNumber}, 				//HFP_KEY_EVENT_KEY_DIALNUM,
	{APP_HFP_KeyCancelOutGoingCall},		//HFP_KEY_EVENT_KEY_CNLOUTGOING,
	{APP_HFP_KeyRejectCall},				//HFP_KEY_EVENT_KEY_REJCALL,
	{APP_HFP_KeyRejectCallSecondPhone}, 	//HFP_KEY_EVENT_KEY_REJCALL_SECOND_PHONE,
	{APP_HFP_KeyOnHoldCall}, 				//HFP_KEY_EVENT_KEY_ONHOLD_CALL,							//0x36
	{APP_HFP_KeyAcceptCall}, 				//HFP_KEY_EVENT_KEY_ACPCALL,
	{APP_HFP_KeyEndCall}, 					//HFP_KEY_EVENT_KEY_ENDCALL,
	{APP_HFP_Key3WayRelNUDB}, 				//HFP_KEY_EVENT_KEY_3WAYRELNUDUB,
	{APP_HFP_Key3WayRelNAcp}, 				//HFP_KEY_EVENT_KEY_3WAYRELNACP,
	{APP_HFP_Key3WayRelNAcp_X}, 			//HFP_KEY_EVENT_KEY_3WAYRELNACP_X,							//0x3B
	{APP_HFP_Key3WayHoldNAcp}, 				//HFP_KEY_EVENT_KEY_3WAYHOLDNACP,
	{APP_HFP_Key3WayHoldNAcp_X}, 			//HFP_KEY_EVENT_KEY_3WAYHOLDNACP_X,
	{APP_HFP_Key3WayAdd}, 					//HFP_KEY_EVENT_KEY_3WAYADD,
	{APP_HFP_Key3WayCallTransfer}, 			//HFP_KEY_EVENT_KEY_3WAYCALLTRANSFER,
	{APP_HFP_KeyHSPButtonPressed}, 			//HFP_KEY_EVENT_KEY_HSP_BUTTON_PRESSED,
	{APP_HFP_KeySetNREC}, 					//HFP_KEY_EVENT_KEY_DISABLE_NREC,							//0x46
	{APP_HFP_KeyGetCallNum}, 				//HFP_KEY_EVENT_KEY_GETCALLNUM,
	{APP_HFP_KeyQueryCallList},				//HFP_KEY_EVENT_KEY_QUERY_CALL_LIST,
	{APP_HFP_KeySendDTMF},					//HFP_KEY_EVENT_KEY_SEND_DTMF,
	{APP_HFP_KeyAttachLastVoiceTagRecord},	//HFP_KEY_EVENT_KEY_ATTACH_LAST_VOICE_TAG_RECORDED,
	{APP_HFP_KeySendCLIP}, 					//HFP_KEY_EVENT_KEY_SEND_CLIP,								//0x4B
	{NULL}, 								//HFP_KEY_EVENT_KEY_SET_VOICE_RECOGNITION_SECONDARY 		//Wait until voice prompt is over
	{NULL}, 								//HFP_KEY_EVENT_KEY_RDIAL_SECONDARY,
	{NULL}, 								//HFP_KEY_EVENT_KEY_SET_VOICE_RECOGNITION_CANCEL,
	{NULL}, 								//HFP_KEY_EVENT_KEY_SET_VOICE_RECOGNITION_SECONDARY_CANCEL,
	{NULL}, 								//HFP_KEY_EVENT_KEY_SIRI_START_OR_CONTINUE,					//Wait until voice prompt is over
	{NULL}, 								//HFP_KEY_EVENT_KEY_SIRI_END,								//Wait until voice prompt is over
	{NULL}, 								//HFP_KEY_EVENT_KEY_SIRI_START_OR_CONTINUE_SECONDARY,		//Wait until voice prompt is over
	{NULL}, 								//HFP_KEY_EVENT_KEY_SIRI_END_SECONDARY,						//Wait until voice prompt is over
};
*/

static void APP_HFP_AudioTransferKeyHandler(BD_ADDR_T *pBdAddr, BOOL isViaKey)
{
	if(APP_GetHfpLinkAudioTransfer(pBdAddr))
		return;

	if (APP_IsActiveLink(pBdAddr))
	{
		if (!APP_SCO_EXISTS(pBdAddr))
		{
			DBG_LOG_APP_SYSTEM( "[APP_HFP_KEY] Audio Transfer From AG change to DUT", 0);
			APP_HFP_CreateAudioConnection(pBdAddr, isViaKey);
		}
		else
		{
			DBG_LOG_APP_SYSTEM( "[APP_HFP_KEY] Audio Transfer From DUT change to AG", 0);
			APP_HfpSco_DisconnectRequest(pBdAddr);
/*
			U8 anotherLink;
			APP_HfpSco_DisconnectRequest(pBdAddr);
			APP_HFP_SetTimer(pBdAddr, TIMER_ID_AUDIO_TRANSFER_DETACH, APP_HfpNvkeyGetTimerValue(TIMER_ID_AUDIO_TRANSFER_DETACH) * 500L);
			anotherLink = MMI_GetNextActiveLink(linkIndex);
			if (anotherLink < MAX_MULTI_POINT_NO && (gMMI_HFP_ctl.HfpCtl[anotherLink].callCtrlTbl[0].callStatus > MMI_CALL_EMPTY || gMMI_HFP_ctl.HfpCtl[anotherLink].callCtrlTbl[1].callStatus > MMI_CALL_EMPTY))
			{
				if(!APP_SCO_EXISTS(anotherLink))
				{
					linkIndex = anotherLink;
					goto AUDIO_TRANSFER_ESTABLISH_SCO;
				}
			}
*/
		}
		APP_SetHfpLinkAudioTransfer(pBdAddr, TRUE);
	}
	else //if (/*gMMI_ctl.miscMask2 & MMI_CREATE_CONN_AFTER_AUDIO_TRANSFER*/)
	{
		DBG_LOG_APP_SYSTEM( "[APP_HFP_KEY] !APP_IsActiveLink", 0);
/*
		//Active Conn, if it exists.
		MMI_Reconnect(RECONNECT_LAST_ONCE_AUDIOTRANSFER);
		gMMI_ctl.miscMask2 &= ( ~MMI_CREATE_CONN_AFTER_AUDIO_TRANSFER);
*/
	}

}

void APP_HFP_KeyAudioTransfer(BD_ADDR_T *pBdAddr)
{
	APP_HFP_AudioTransferKeyHandler(pBdAddr, TRUE);
}

void APP_HFP_KeyRedial(BD_ADDR_T *pBdAddr)
{
	APP_INFO_STRU *pLinkInfo;
	pBdAddr = APP_GetRedialBdaddr();

	if(pBdAddr != NULL)
	{
		pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

		if(pLinkInfo)
		{
			if(pLinkInfo->subState != APP_RDIAL_STARTING)
			{
				APP_AudioDeviceRegister(pBdAddr, AUDIO_DEVICE_KEY_AT_CMD);
				if( pLinkInfo->subState == APP_VDIAL_STARTED || pLinkInfo->subState == APP_VDIAL_STARTING )
				{
					if(APP_CANCEL_VR_WHEN_RDIAL_FEAT)
						APP_HFP_SendATCmdByLink(pBdAddr, HFP_ATCMD_CANCEL_VOICE_DIAL);
				}
				APP_HFP_SendATCmdByLink(pBdAddr, HFP_ATCMD_LAST_NUMBER_REDIAL);
			}
		}
	}
}

void APP_HFP_KeyVoiceRecognition(U16 opcode)
{
	APP_INFO_STRU *pLinkInfo;
	BD_ADDR_T	*pBdAddr = APP_GetRedialBdaddr();

	//1. Decide link and Reconnect or VDial
   	if(pBdAddr != NULL)
	{
		pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);
		if(pLinkInfo)
		{
			if(HFP_IsSupportVoiceRecognition(pBdAddr))
			{
				//2. Pause Music in another link first[should be done after link is decided]
				if(APP_VOICE_DIAL_START_AND_END_THE_SAME_KEY_FEAT)
				{
					if(pLinkInfo->subState == APP_VDIAL_STARTED || pLinkInfo->subState == APP_VDIAL_STARTING)
					{
						APP_HFP_SendATCmdByLink(pBdAddr, HFP_ATCMD_CANCEL_VOICE_DIAL);
					}
					else if(pLinkInfo->subState == APP_VDIAL_STOP || pLinkInfo->subState == APP_VDIAL_STOPPING)
					{
						APP_AudioDeviceRegister(pBdAddr, AUDIO_DEVICE_KEY_AT_CMD);
						APP_HFP_SendATCmdByLink(pBdAddr, HFP_ATCMD_INIT_VOICE_DIAL);
					}
				}
				else
				{
					if(pLinkInfo->subState == APP_VDIAL_STARTED || pLinkInfo->subState == APP_VDIAL_STARTING)
					{
						if(opcode == KEY_SET_VOICE_RECOGNITION_CANCEL)
							APP_HFP_SendATCmdByLink(pBdAddr, HFP_ATCMD_CANCEL_VOICE_DIAL);
					}
					else if(pLinkInfo->subState == APP_VDIAL_STOP || pLinkInfo->subState == APP_VDIAL_STOPPING)
					{
						if(opcode == KEY_SET_VOICE_RECOGNITION)
						{
							APP_AudioDeviceRegister(pBdAddr, AUDIO_DEVICE_KEY_AT_CMD);
							APP_HFP_SendATCmdByLink(pBdAddr, HFP_ATCMD_INIT_VOICE_DIAL);
						}
					}
				}
			}
		}
	}
}

void APP_HFP_KeyAppleSiri(U16 opcode)
{
	BD_ADDR_T *pBdAddr;
	APP_INFO_STRU *pLinkInfo;

	//1. Decide link and Reconnect or VDial

	if((pBdAddr = APP_GetRedialBdaddr()) == APP_EOF)
		return;

	pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	if(pLinkInfo == (APP_INFO_STRU *)NULL || pLinkInfo->hfpCtl.appleSiriStatus != HFP_APLSIRI_AVAILABLE_ENABLE)
	{
		return;
	}

	if( pLinkInfo->subState == APP_VDIAL_STOP ||  pLinkInfo->subState == APP_VDIAL_STOPPING)
	{
		if(opcode == KEY_SIRI_START_OR_CONTINUE)
		{
			APP_HFP_SendATCmdByLink(pBdAddr, HFP_ATCMD_INIT_VOICE_DIAL);
			APP_Media_PushMediaEvent(MEDIA_EVT_SIRI_STARTING);
		}
	}
	else if(pLinkInfo->subState == APP_VDIAL_STARTED || pLinkInfo->subState == APP_VDIAL_STARTING)
	{
		APP_HFP_SendATCmdByLink(pBdAddr, (opcode == KEY_SIRI_START_OR_CONTINUE )? HFP_ATCMD_INIT_VOICE_DIAL: HFP_ATCMD_CANCEL_VOICE_DIAL);
		APP_Media_PushMediaEvent((opcode == KEY_SIRI_START_OR_CONTINUE )? MEDIA_EVT_SIRI_CONTINUE : MEDIA_EVT_SIRI_END);
	}
}

#ifdef HFP_AT_ATD_NUMBER
void APP_HFP_KeyDialMemory(BD_ADDR_T *pBdAddr)
{
	APP_SendATCmdUnderConnection(pBdAddr, HFP_ATCMD_MEMORY_DIALING);
}
#endif

#ifdef HFP_AT_ATD_DIGIT
void APP_HFP_KeyDialNumber(BD_ADDR_T *pBdAddr)
{
		UNUSED(pBdAddr);
		//if(gMMI_HFP_ctl.incomingLinkInd == APP_EOF)
			//return;
		//MMI_SendATCmdUnderConnection(gMMI_HFP_ctl.incomingLinkInd, HFP_ATCMD_PLACE_CALL_WITH_PHONE_NUMBER);
}
#endif

void APP_HFP_KeyCancelOutGoingCall(BD_ADDR_T *pBdAddr)
{
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	APP_SendATCmdUnderConnection(pBdAddr, HFP_ATCMD_CANCEL_OUTGOING_CALL);
	pLinkInfo->stateEntryExit |= APP_EXIT_OUTGOING_STATE;
}

void APP_HFP_KeyRejectCall(BD_ADDR_T *pBdAddr)
{
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	APP_SendATCmdUnderConnection(pBdAddr, HFP_ATCMD_REJECT_CALL);

	//if(APP_Hfp_IsFeatureOn(APP_PLAY_DEFAULT_RING_INCOMING_FEAT))
		//MMI_DRV_RejectCallMuteSCOCmd(TRUE);

	pLinkInfo->stateEntryExit |= APP_EXIT_IMGCALL_STATE;
}

void APP_HFP_KeyRejectCallSecondPhone(BD_ADDR_T *pBdAddr)
{
	BD_ADDR_T *nextBdAddr;

	if(APP_Hfp_IsFeatureOn(APP_SUPPORT_MULTI_HFP_FEAT))
	{
		nextBdAddr = APP_GetNextNormalLink(pBdAddr);
		if(nextBdAddr != APP_EOF && APP_State_CheckLinkInIncomingState(nextBdAddr))
		{
			APP_HFP_SendATCmdByLink(nextBdAddr, HFP_ATCMD_REJECT_CALL);
		}
	}
}

void APP_HFP_KeyOnHoldCall(BD_ADDR_T *pBdAddr)
{
	APP_SendATCmdUnderConnection(pBdAddr, HFP_ATCMD_3WAY_HOLDNACP);
	if(APP_HFP_CountCallsInCallStatus(pBdAddr,APP_CALL_HELD) == 0 && APP_HFP_CountCallsInCallStatus(pBdAddr, APP_CALL_ACTIVE) == 1)//will receive callheld = 2 (call on hold, no active call)
	{
		APP_HFP_CallheldIndicationHandler_ForNoActiveCalls_HF(pBdAddr);
	}
	else if(APP_HFP_CountCallsInCallStatus(pBdAddr, APP_CALL_HELD) == 1 && APP_HFP_CountCallsInCallStatus(pBdAddr, APP_CALL_ACTIVE) == 0)//will receive callheld = 0 (no calls held)
	{
		APP_HFP_CallheldIndicationHandler_ForNoCallsHeld_HF(pBdAddr);
	}
}

void APP_HFP_KeyAcceptCall(BD_ADDR_T *pBdAddr)
{
	APP_SendATCmdUnderConnection(pBdAddr, HFP_ATCMD_ACCEPT_CALL);
}

void APP_HFP_KeyEndCall(BD_ADDR_T *pBdAddr)
{
	APP_HFP_CTL_STRU * pHfpLinkInfo = APP_GetHfpLinkInfo(pBdAddr);

	if(pHfpLinkInfo->indicator.callheld == CALLHELD_ON_HOLD_NOACTIVE)
		APP_SendATCmdUnderConnection(pBdAddr, HFP_ATCMD_3WAY_RELNUDUB);
	else
		APP_SendATCmdUnderConnection(pBdAddr, HFP_ATCMD_END_CALL);
}

void APP_HFP_Key3WayRelNUDB(BD_ADDR_T *pBdAddr)
{
	APP_SendATCmdUnderConnection(pBdAddr, HFP_ATCMD_3WAY_RELNUDUB);
}

void APP_HFP_Key3WayRelNAcp(BD_ADDR_T *pBdAddr)
{
	APP_SendATCmdUnderConnection(pBdAddr, (HFP_IsSupport3WayCalling(pBdAddr)) ? HFP_ATCMD_3WAY_RELNACP : HFP_ATCMD_END_CALL);
}

void APP_HFP_Key3WayRelNAcp_X(BD_ADDR_T *pBdAddr)
{
	APP_SendATCmdUnderConnection(pBdAddr, HFP_ATCMD_3WAY_RELNACP_X);
}

void APP_HFP_Key3WayHoldNAcp(BD_ADDR_T *pBdAddr)
{
	APP_SendATCmdUnderConnection(pBdAddr, HFP_ATCMD_3WAY_HOLDNACP);
}

void APP_HFP_Key3WayHoldNAcp_X(BD_ADDR_T *pBdAddr)
{
	APP_SendATCmdUnderConnection(pBdAddr, HFP_ATCMD_3WAY_HOLDNACP_X);
}

void APP_HFP_Key3WayAdd(BD_ADDR_T *pBdAddr)
{
	APP_SendATCmdUnderConnection(pBdAddr, HFP_ATCMD_3WAY_ADD);
}

void APP_HFP_Key3WayCallTransfer(BD_ADDR_T *pBdAddr)
{
	APP_SendATCmdUnderConnection(pBdAddr, HFP_ATCMD_3WAY_CALLTRANSFER);
}

<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
void APP_HFP_KeyHSPButtonPressed(BD_ADDR_T *pBdAddr)
{
	APP_SendATCmdUnderConnection(pBdAddr, HFP_ATCMD_CKPD_200);
}
#endif

void APP_HFP_KeySetNREC(BD_ADDR_T *pBdAddr)
{
	APP_SendATCmdUnderConnection(pBdAddr, HFP_ATCMD_SET_NREC);
}

#ifdef HFP_AT_CNUM
void APP_HFP_KeyGetCallNum(BD_ADDR_T *pBdAddr)
{
	APP_SendATCmdUnderConnection(pBdAddr, HFP_ATCMD_GET_SUBSCRIBER_NUMBER);
}
#endif

#ifdef HFP_AT_CLCC
void APP_HFP_KeyQueryCallList(BD_ADDR_T *pBdAddr)
{
	APP_SendATCmdUnderConnection(pBdAddr, HFP_ATCMD_QUERY_LIST_OF_CURRENT_CALLS);
}
#endif

#ifdef HFP_AT_VTS
void APP_HFP_KeySendDTMF(BD_ADDR_T *pBdAddr)
{
	APP_SendATCmdUnderConnection(pBdAddr, HFP_ATCMD_SEND_DTMF);
}
#endif

#ifdef HFP_AT_BINP
void APP_HFP_KeyAttachLastVoiceTagRecord(BD_ADDR_T *pBdAddr)
{
	APP_SendATCmdUnderConnection(pBdAddr, HFP_ATCMD_ATTACH_LAST_VOICE_TAG_RECORDED);
}
#endif

void APP_HFP_KeySendCLIP(BD_ADDR_T *pBdAddr)
{
	APP_SendATCmdUnderConnection(pBdAddr, HFP_ATCMD_ENABLE_CLI_NOTIFICATION);
}


void APP_HFP_ProcessKeyEvent(BD_ADDR_T *pBdAddr, U16 keyCode)
{
	switch(keyCode)
	{
		case HFP_KEY_EVENT_KEY_AUDIO_TRANSFER:								//KEY_AUDIO_TRANSFER,
			APP_HFP_KeyAudioTransfer(pBdAddr);
			break;

		case HFP_KEY_EVENT_KEY_SET_VOICE_RECOGNITION:						//KEY_SET_VOICE_RECOGNITION,
			//APP_HFP_KeyVoiceRecognition(opcode);							//wait fake media event over
			//void APP_HFP_KeyAppleSiri(opcode);							//wait fake media event over
			//NULL
			break;

		case HFP_KEY_EVENT_KEY_RDIAL:										//KEY_RDIAL,
			//APP_HFP_KeyRedial(pBdAddr);									//wait fake media event over
			//NULL
			break;

		case HFP_KEY_EVENT_KEY_DIALMEM:										//KEY_DIALMEM,
			#ifdef HFP_AT_ATD_NUMBER
			APP_HFP_KeyDialMemory(pBdAddr);
			#endif
			break;

		case HFP_KEY_EVENT_KEY_DIALNUM:										//KEY_DIALNUM,
			#ifdef HFP_AT_ATD_DIGIT
			APP_HFP_KeyDialNumber(pBdAddr);
			#endif
			break;

		case HFP_KEY_EVENT_KEY_CNLOUTGOING:									//KEY_CNLOUTGOING,
			APP_HFP_KeyCancelOutGoingCall(pBdAddr);
			break;

		case HFP_KEY_EVENT_KEY_REJCALL:										//KEY_REJCALL,
			APP_HFP_KeyRejectCall(pBdAddr);
			break;

		case HFP_KEY_EVENT_KEY_REJCALL_SECOND_PHONE:						//KEY_REJCALL_SECOND_PHONE,
			APP_HFP_KeyRejectCallSecondPhone(pBdAddr);
			break;

		case HFP_KEY_EVENT_KEY_ONHOLD_CALL:									//KEY_ONHOLD_CALL,
			APP_HFP_KeyOnHoldCall(pBdAddr);
			break;

		case HFP_KEY_EVENT_KEY_ACPCALL:										//KEY_ACPCALL,
			APP_HFP_KeyAcceptCall(pBdAddr);
			break;

		case HFP_KEY_EVENT_KEY_ENDCALL:										//KEY_ENDCALL,
			APP_HFP_KeyEndCall(pBdAddr);
			break;

		case HFP_KEY_EVENT_KEY_3WAYRELNUDUB:								//KEY_3WAYRELNUDUB,
			APP_HFP_Key3WayRelNUDB(pBdAddr);
			break;

		case HFP_KEY_EVENT_KEY_3WAYRELNACP:									//KEY_3WAYRELNACP,
			APP_HFP_Key3WayRelNAcp(pBdAddr);
			break;

		case HFP_KEY_EVENT_KEY_3WAYRELNACP_X:								//KEY_3WAYRELNACP_X,
			APP_HFP_Key3WayRelNAcp_X(pBdAddr);
			break;

		case HFP_KEY_EVENT_KEY_3WAYHOLDNACP:								//KEY_3WAYHOLDNACP,
			APP_HFP_Key3WayHoldNAcp(pBdAddr);
			break;

		case HFP_KEY_EVENT_KEY_3WAYHOLDNACP_X:							//KEY_3WAYHOLDNACP_X,
			APP_HFP_Key3WayHoldNAcp_X(pBdAddr);
			break;

		case HFP_KEY_EVENT_KEY_3WAYADD:										//KEY_3WAYADD,
			APP_HFP_Key3WayAdd(pBdAddr);
			break;

		case HFP_KEY_EVENT_KEY_3WAYCALLTRANSFER:							//KEY_3WAYCALLTRANSFER,
			APP_HFP_Key3WayCallTransfer(pBdAddr);
			break;

<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
		case HFP_KEY_EVENT_KEY_HSP_BUTTON_PRESSED:							//KEY_HSP_BUTTON_PRESSED,
			APP_HFP_KeyHSPButtonPressed(pBdAddr);
			break;
#endif

		case HFP_KEY_EVENT_KEY_DISABLE_NREC:								//KEY_DISABLE_NREC,
			APP_HFP_KeySetNREC(pBdAddr);
			break;

		case HFP_KEY_EVENT_KEY_GETCALLNUM:									//KEY_GETCALLNUM,
			#ifdef HFP_AT_CNUM
			APP_HFP_KeyGetCallNum(pBdAddr);
			#endif
			break;

		case HFP_KEY_EVENT_KEY_QUERY_CALL_LIST:							//KEY_QUERY_CALL_LIST,
			#ifdef HFP_AT_CLCC
			APP_HFP_KeyQueryCallList(pBdAddr);
			#endif
			break;

		case HFP_KEY_EVENT_KEY_SEND_DTMF:									//KEY_SEND_DTMF,
			#ifdef HFP_AT_VTS
			APP_HFP_KeySendDTMF(pBdAddr);
			#endif
			break;

		case HFP_KEY_EVENT_KEY_ATTACH_LAST_VOICE_TAG_RECORDED:				//KEY_ATTACH_LAST_VOICE_TAG_RECORDED,
			#ifdef HFP_AT_BINP
			APP_HFP_KeyAttachLastVoiceTagRecord(pBdAddr);
			#endif
			break;

		case HFP_KEY_EVENT_KEY_SEND_CLIP:									//KEY_SEND_CLIP,
			APP_HFP_KeySendCLIP(pBdAddr);
			break;

		case HFP_KEY_EVENT_KEY_RESERVED1:									//KEY_RESERVED1,
			//NULL
			break;

		case HFP_KEY_EVENT_KEY_RESERVED2:									//KEY_RESERVED2,
			//NULL
			break;

		case HFP_KEY_EVENT_KEY_SET_VOICE_RECOGNITION_CANCEL:				//KEY_SET_VOICE_RECOGNITION_CANCEL,
			//NULL
			break;

		case HFP_KEY_EVENT_KEY_RESERVED3:									//KEY_RESERVED3,
			//NULL
			break;

		case HFP_KEY_EVENT_KEY_SIRI_START_OR_CONTINUE:						//KEY_SIRI_START_OR_CONTINUE,
			//NULL
			break;

		case HFP_KEY_EVENT_KEY_SIRI_END:									//KEY_SIRI_END,
			//NULL
			break;

		case HFP_KEY_EVENT_KEY_RESERVED4:									//KEY_RESERVED4,
			//NULL
			break;

		case HFP_KEY_EVENT_KEY_RESERVED5:									//KEY_RESERVED5,
			//NULL
			break;

		default:
			FW_Assert(FALSE);
			break;

	}
}

/*static*/ U8 APP_HFP_CheckKeyIsValid(U16 keyCode, BD_ADDR_T *plinkBdAddr, BD_ADDR_T *pTargetBdAddr)
{
	if(pTargetBdAddr == APP_EOF)
	{
		plinkBdAddr = APP_EOF;
		//if(!(gMMI_driver_variation_nvram.link_history_info[0].miscMask & NVRAM_LINK_KEY_MISC_MASK_HFP) )  //last disc link in history
		{
			keyCode = KEY_INVALID;
		}
	}
	else
	{
		plinkBdAddr = pTargetBdAddr;
	}

	/*if(plinkBdAddr != APP_EOF && gMMI_ctl.sdapCtl[*linkIndexPtr].remoteApplicationID != PRODUCT_AIR_NONE)
	{
		keyCode = KEY_INVALID;
	}*/

	return keyCode;
}


static U16 APP_HFP_CheckKeySetVoiceRecognition(U16 keyCode)
{
	BD_ADDR_T *pBdAddr;
	APP_INFO_STRU *pLinkInfo;

	if(!APP_PowerOff_IsPowerOn() || (pBdAddr = APP_GetRedialBdaddr()) == NULL || (pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr)) == (APP_INFO_STRU *)NULL)
		return KEY_INVALID;

	if(	pLinkInfo->subState != APP_VDIAL_STOP && pLinkInfo->subState != APP_VDIAL_STARTED &&
		pLinkInfo->subState != APP_VDIAL_STOPPING && pLinkInfo->subState != APP_VDIAL_STARTING)
	{
		return KEY_INVALID;
	}

<<<<<<< HEAD
=======
	DBG_LOG_APP_SYSTEM( "[APP_HFP_KEY] debug siri status:%d, feat:%d", 2, 
		pLinkInfo->hfpCtl.appleSiriStatus, APP_VOICE_DIAL_START_AND_END_THE_SAME_KEY_FEAT);

>>>>>>> db20e11 (second commit)
	if(pLinkInfo->hfpCtl.appleSiriStatus == HFP_APLSIRI_AVAILABLE_ENABLE) // ios device
	{
		if(APP_VOICE_DIAL_START_AND_END_THE_SAME_KEY_FEAT &&
		(pLinkInfo->subState == APP_VDIAL_STARTED || pLinkInfo->subState == APP_VDIAL_STARTING))
		{
			keyCode = KEY_SIRI_END;
		}
		else
		{
			keyCode = KEY_SIRI_START_OR_CONTINUE;
		}
	}
	else
	{
		if(!APP_VOICE_DIAL_START_AND_END_THE_SAME_KEY_FEAT && (pLinkInfo->subState == APP_VDIAL_STARTED || pLinkInfo->subState == APP_VDIAL_STARTING))
		{
			keyCode = KEY_INVALID;
		}
	}

	return keyCode;
}

static U16 APP_HFP_CheckKeyRedial(U16 keyCode)
{
	BD_ADDR_T *redialBdAddr;
	APP_INFO_STRU *pLinkInfo;

	if((redialBdAddr = APP_GetRedialBdaddr()) != NULL && (pLinkInfo = APP_GetAppLinkByBdAddr(redialBdAddr)) != NULL)
	{
		if(pLinkInfo->subState == APP_VDIAL_STOP)
			return keyCode;
	}
	return KEY_INVALID;
}

static U16 APP_HFP_CheckKeyDialNumber(U16 keyCode)
{
	BD_ADDR_T *pBdAddr;
	APP_HFP_CTL_STRU *pHfpLinkInfo;

	keyCode = APP_HFP_CheckKeyIsValid(keyCode, pBdAddr, APP_GetImgCallBdaddr());

	if(pBdAddr != NULL && (pHfpLinkInfo = APP_GetHfpLinkInfo(pBdAddr)) != (APP_HFP_CTL_STRU *)NULL && !pHfpLinkInfo->incomingCall.length)
	{
		keyCode = KEY_INVALID;
	}

	return keyCode;
}

static U16 APP_HFP_CheckKeyIncomingCallDial(U16 keyCode)
{
	BD_ADDR_T *pBdAddr;
	APP_HFP_CTL_STRU *pHfpLinkInfo;

	keyCode = APP_HFP_CheckKeyIsValid(keyCode, pBdAddr, APP_GetServiceBdAddr());
	if(pBdAddr != NULL && (pHfpLinkInfo = APP_GetHfpLinkInfo(pBdAddr)) != (APP_HFP_CTL_STRU *)NULL && !pHfpLinkInfo->incomingCall.length)
	{
		keyCode = KEY_INVALID;
	}

	return keyCode;
}

static U16 APP_HFP_CheckKeyCancelVoiceRecognition(U16 keyCode)
{

	BD_ADDR_T *pBdAddr;
	APP_INFO_STRU *pLinkInfo;

	pBdAddr = APP_GetRedialBdaddr();

	// Normal voice recognition
	if((pBdAddr = APP_GetRedialBdaddr()) == NULL || (pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr)) == NULL ||
	(pLinkInfo->subState != APP_VDIAL_STARTED && pLinkInfo->subState != APP_VDIAL_STARTING))
	{
		return KEY_INVALID;
	}

	if(pLinkInfo->hfpCtl.appleSiriStatus == HFP_APLSIRI_AVAILABLE_ENABLE) // ios device
	{
		keyCode = KEY_SIRI_END;
	}

	return keyCode;
}


static APP_KEY_CHECK_STRU CODE APP_HFP_KeyCheckValidHandler[HFP_KEY_EVENT_NO] =
{
	{NULL}, 					//HFP_KEY_EVENT_KEY_AUDIO_TRANSFER}
	{APP_HFP_CheckKeySetVoiceRecognition}, //HFP_KEY_EVENT_KEY_SET_VOICE_RECOGNITION //Wait until voice prompt is over
	{APP_HFP_CheckKeyRedial}, 	//HFP_KEY_EVENT_KEY_RDIAL //Wait until voice prompt is over
	{NULL}, //HFP_KEY_EVENT_KEY_DIALMEM
	{APP_HFP_CheckKeyDialNumber}, //HFP_KEY_EVENT_KEY_DIALNUM,
	{NULL}, //HFP_KEY_EVENT_KEY_CNLOUTGOING,
	{NULL}, //HFP_KEY_EVENT_KEY_REJCALL,
	{NULL}, //HFP_KEY_EVENT_KEY_REJCALL_SECOND_PHONE,
	{NULL}, //HFP_KEY_EVENT_KEY_ONHOLD_CALL,		//0x36
	{NULL}, //HFP_KEY_EVENT_KEY_ACPCALL,
	{NULL}, //HFP_KEY_EVENT_KEY_ENDCALL,
	{NULL}, //HFP_KEY_EVENT_KEY_3WAYRELNUDUB,
	{NULL}, //HFP_KEY_EVENT_KEY_3WAYRELNACP,
	{NULL}, //HFP_KEY_EVENT_KEY_3WAYRELNACP_X,	//0x3B
	{NULL}, //HFP_KEY_EVENT_KEY_3WAYHOLDNACP,
	{NULL}, //HFP_KEY_EVENT_KEY_3WAYHOLDNACP_X,
	{NULL}, //HFP_KEY_EVENT_KEY_3WAYADD,
	{NULL}, //HFP_KEY_EVENT_KEY_3WAYCALLTRANSFER,
	{APP_HFP_CheckKeyIncomingCallDial}, //HFP_KEY_EVENT_KEY_HSP_BUTTON_PRESSED,
	{NULL}, //HFP_KEY_EVENT_KEY_DISABLE_NREC,		//0x46
	{APP_HFP_CheckKeyIncomingCallDial}, //HFP_KEY_EVENT_KEY_GETCALLNUM,
	{APP_HFP_CheckKeyIncomingCallDial}, //HFP_KEY_EVENT_KEY_QUERY_CALL_LIST,
	{APP_HFP_CheckKeyIncomingCallDial}, //HFP_KEY_EVENT_KEY_SEND_DTMF,
	{APP_HFP_CheckKeyIncomingCallDial}, //HFP_KEY_EVENT_KEY_ATTACH_LAST_VOICE_TAG_RECORDED,
	{APP_HFP_CheckKeyIncomingCallDial}, //HFP_KEY_EVENT_KEY_SEND_CLIP,			//0x4B
	{NULL}, //HFP_KEY_EVENT_KEY_RESERVED1 //Wait until voice prompt is over
	{NULL}, //HFP_KEY_EVENT_KEY_RESERVED2,
	{APP_HFP_CheckKeyCancelVoiceRecognition}, //HFP_KEY_EVENT_KEY_SET_VOICE_RECOGNITION_CANCEL,
	{NULL}, //HFP_KEY_EVENT_KEY_RESERVED3,
	{NULL}, //HFP_KEY_EVENT_KEY_SIRI_START_OR_CONTINUE,		//Wait until voice prompt is over
	{NULL}, //HFP_KEY_EVENT_KEY_SIRI_END,			//Wait until voice prompt is over
	{NULL}, //HFP_KEY_EVENT_KEY_RESERVED4, //Wait until voice prompt is over
	{NULL}, //HFP_KEY_EVENT_KEY_RESERVED5, //Wait until voice prompt is over
};



U16 APP_HFP_CheckValidKey(U8 index, U16 keyCode)
{
	if(APP_HFP_KeyCheckValidHandler[index].fKeyCheckValid)
	{
		keyCode = APP_HFP_KeyCheckValidHandler[index].fKeyCheckValid(keyCode);
	}
	return keyCode;
}

