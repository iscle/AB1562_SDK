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
#include "App_State.h"
#include "App_Media.h"
#include "App_Hfp.h"
#include "App_Connection.h"
#include "Hfp_Interface.h"
#include "APP_HfpIncomingCallMediaHandler.h"
#include "App_Nvram_Type.h"
#include "AudioDSP_StreamManager.h"
#include "drv_vp.h"
#include "App_MCSync_Message.h"
#include "App_RhoHandler.h"


/******************************************************************************
 * Enumeration
*******************************************************************************/
enum
{
	APP_HFP_IMG_MEDIA_NONE,
	APP_HFP_IMG_MEDIA_VP,
	APP_HFP_IMG_MEDIA_CALLER_NAME,
	APP_HFP_IMG_MEDIA_CALLER_ID,
	APP_HFP_IMG_MEDIA_RINGTONE,
	APP_HFP_IMG_MEDIA_NUM = APP_HFP_IMG_MEDIA_RINGTONE,
};

/******************************************************************************
 * Define
*******************************************************************************/
#define IMG_AUDIO_VOICE_LOAD_OK				0x00
#define IMG_AUDIO_VOICE_LOAD_STATE_ERROR	0x01
#define IMG_AUDIO_VOICE_LOAD_PEDNING		0x02
#define IMG_AUDIO_VOICE_LOAD_INVALID		0xFF

/******************************************************************************
 * Structure
*******************************************************************************/
<<<<<<< HEAD
typedef struct
{
	U8 imgAudioVoiceState;
	BD_ADDR_T bdAddr;
}APP_HFP_IMG_MEDIA_STRU;
=======
>>>>>>> db20e11 (second commit)

typedef struct
{
	U32 callerIDRepeatTime;
	U8 	ImgCallMediaPriority[APP_HFP_IMG_MEDIA_NUM];
}NVKEY_CIMG_CALL_MEDIA_STRU;

/******************************************************************************
 * Variable
*******************************************************************************/
static U32 app_ImgCallMediaTimerHandler(Handler handler, U16 id, void* msg, U32 handler_id);
<<<<<<< HEAD
static const HandlerData gAppImgCallMediaTimerHandler = {app_ImgCallMediaTimerHandler};
=======
HandlerData gAppImgCallMediaTimerHandler = {app_ImgCallMediaTimerHandler};
>>>>>>> db20e11 (second commit)

APP_HFP_IMG_MEDIA_STRU	gAppHfpImgMediaCtl;

NVKEY_CIMG_CALL_MEDIA_STRU const * gNvkeyImgCallMediaAddr;

/******************************************************************************
 * Static Function
*******************************************************************************/
static U8 app_ImgCallGetMediaPriority(U8 index)
{
	if(gNvkeyImgCallMediaAddr)
		return gNvkeyImgCallMediaAddr->ImgCallMediaPriority[index];
	else
	{
		switch(index)
		{
			case 0:
				return APP_HFP_IMG_MEDIA_VP;
			case 1:
				return APP_HFP_IMG_MEDIA_CALLER_ID;
			case 2:
				return APP_HFP_IMG_MEDIA_CALLER_NAME;
			case 3:
				return APP_HFP_IMG_MEDIA_RINGTONE;
			default:
				return APP_HFP_IMG_MEDIA_NONE;
		}
	}
}

static U32 app_ImgCallGetMediaPeriodTime(void)
{
	if(gNvkeyImgCallMediaAddr)
		return gNvkeyImgCallMediaAddr->callerIDRepeatTime;
	else
		return (3 * HALF_SEC);
}

static U8 app_ImgMedia_IncomingVPHandler(BD_ADDR_T *pBdAddr)
{
	APP_HFP_CTL_STRU * pHfpLinkInfo = APP_GetHfpLinkInfo(pBdAddr);

	if(!APP_State_CheckLinkInImgAndCaImgState(pBdAddr) || !pHfpLinkInfo)
		return IMG_AUDIO_VOICE_LOAD_STATE_ERROR;

	#ifdef BLE_AIRApp
	//if(MMI_HFP_IsBleAppCallerNameOn(linkIndex))
	//{
	//	if(pHfpLinkInfo->isImgVPTriggered)
	//		return IMG_AUDIO_VOICE_LOAD_INVALID;
	//}
	#endif

	APP_Media_PushMediaEvent(MEDIA_EVT_ENTER_INCOMING);
	pHfpLinkInfo->isImgVPTriggered = TRUE;

	return IMG_AUDIO_VOICE_LOAD_OK;
}

static U8 app_ImgMedia_CallerNameHandler(BD_ADDR_T *pBdAddr)
{
	APP_HFP_CTL_STRU * pHfpLinkInfo = APP_GetHfpLinkInfo(pBdAddr);

	if (!APP_State_CheckLinkInIncomingState(pBdAddr) || !pHfpLinkInfo)
		return IMG_AUDIO_VOICE_LOAD_STATE_ERROR;

	if(HFP_IsConnected(pBdAddr))
		return IMG_AUDIO_VOICE_LOAD_INVALID;

	//if(DRIVER_Query_IsDSPSuspending())
		//return IMG_AUDIO_VOICE_LOAD_PEDNING;

	if(!APP_IsServiceBdAddr(pBdAddr) && APP_Hfp_IsFeatureOn(APP_IGNORE_CALLER_VOICE_IF_OTHER_LINK_IN_CALL_FEAT)) // for 1 to 2 incomming both --> caller name is valid in only service link
		return IMG_AUDIO_VOICE_LOAD_INVALID;

	if(PM_IsProfileConnected(pBdAddr, PROFILE_AIR_APP))
	{
		if(pHfpLinkInfo->isCallerNameGet)
		{
	//		LightDbgPrint("Call Name!!!!");
	//		MMI_HFP_SendCallerNameVPCmd();
	//		REMEMBER PUSH MEDIA EVENT!!!!!!
	//		return IMG_AUDIO_VOICE_LOAD_OK;
		}
	}
	#ifdef BLE_AIRApp
	//else if(MMI_HFP_IsBleAppCallerNameOn(linkIndex))
	//{
	//	LightDbgPrint("Call Name_BLE:%d!!!!", (U8)(app_ImgCallGetMediaPeriodTime()/1000));
	//	return IMG_AUDIO_VOICE_LOAD_OK;
	//}
	#endif

	return IMG_AUDIO_VOICE_LOAD_INVALID;
}

static U8 app_ImgMedia_CallerIDHandler(BD_ADDR_T *pBdAddr)
{
	APP_HFP_CTL_STRU * pHfpLinkInfo = APP_GetHfpLinkInfo(pBdAddr);

	if(!APP_State_CheckLinkInIncomingState(pBdAddr) || !pHfpLinkInfo)
		return IMG_AUDIO_VOICE_LOAD_STATE_ERROR;
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
	if(APP_Hfp_IsHspConnected(pBdAddr))
		return IMG_AUDIO_VOICE_LOAD_INVALID;
#endif
	//if(DRIVER_Query_IsDSPSuspending())
	//	return IMG_AUDIO_VOICE_LOAD_PEDNING;

	if(!APP_IsServiceBdAddr(pBdAddr) && APP_IGNORE_CALLER_VOICE_IF_OTHER_LINK_IN_CALL_FEAT) // for 1 to 2 incomming both --> caller name is valid in only service link
		return IMG_AUDIO_VOICE_LOAD_INVALID;

	#ifdef BLE_AIRApp
	//if(MMI_HFP_IsBleAppCallerNameOn(linkIndex))
	//	return IMG_AUDIO_VOICE_LOAD_INVALID;
	#endif

	if(PM_IsProfileConnected(pBdAddr, PROFILE_AIR_APP) && pHfpLinkInfo->isCallerNameGet)
		return IMG_AUDIO_VOICE_LOAD_INVALID;

	if(pHfpLinkInfo->isCallerIdGet /*&& APP_Hfp_IsFeatureOn(APP_READ_OUT_CALLER_ID_BY_VP_FEAT)*/)
	{
		U8 i;

		if(pHfpLinkInfo->incomingCall.length == 0)
			return IMG_AUDIO_VOICE_LOAD_INVALID;

		for(i = 0; i < pHfpLinkInfo->incomingCall.length; i++)
		{
			if(pHfpLinkInfo->incomingCall.callNumId[i] >= '0' && pHfpLinkInfo->incomingCall.callNumId[i] <= '9')
			{
			}
			else
				return IMG_AUDIO_VOICE_LOAD_INVALID;
		}

		if(drv_vp_set_caller_id(&pHfpLinkInfo->incomingCall.callNumId[0], pHfpLinkInfo->incomingCall.length))
		{
			APP_MCSYNC_SendSyncCallIdInfo(&pHfpLinkInfo->incomingCall.callNumId[0], pHfpLinkInfo->incomingCall.length);
			APP_Media_PushMediaEvent(MEDIA_EVT_CALLER_ID);
		}
		else
		{
			DBG_LOG_APP_HFP( "[APP_HFP] IncomingCall SetNum FAIL", 0);
			return IMG_AUDIO_VOICE_LOAD_INVALID;
		}

		return IMG_AUDIO_VOICE_LOAD_OK;
	}
	return IMG_AUDIO_VOICE_LOAD_INVALID;
}

static U8 app_ImgMedia_RingHandler(BD_ADDR_T *pBdAddr)
{
	BOOL isInCall = FALSE;
	if(!APP_State_CheckLinkInIncomingState(pBdAddr))
		return IMG_AUDIO_VOICE_LOAD_STATE_ERROR;
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
	if(APP_Hfp_IsFeatureOn(APP_PLAY_DEFAULT_RING_INCOMING_FEAT) || APP_Hfp_IsHspConnected(pBdAddr))
#else
    if(APP_Hfp_IsFeatureOn(APP_PLAY_DEFAULT_RING_INCOMING_FEAT))
#endif        
		goto DECIDE_RING_MEDIA;

	#ifdef BLE_AIRApp
	//if(MMI_HFP_IsBleAppCallerNameOn(linkIndex) && APP_IsServiceBdAddr(pBdAddr))
	//	return IMG_AUDIO_VOICE_LOAD_INVALID;
	#endif

	if(APP_State_CheckOtherLinksInCallRelatedState(pBdAddr))
	{
		isInCall = TRUE;
		goto RING_START;
	}

	//BRSF sent by Nokia 2630 indicates that it supports in-band ringing.
	//However, in its MUTE mode, 2630 won't establish SCO conection with us.
	//So we can't hear in-band ringtone as well as local alerting ringtone.
	if (!HFP_IsSupportAGInbandRingTone(pBdAddr) ||
		(HFP_IsSupportAGInbandRingTone(pBdAddr) && !APP_SCO_EXISTS(pBdAddr)))
	{
		DECIDE_RING_MEDIA:

		if(APP_State_AllLinksInCallActiveState() || APP_State_CheckOtherLinksInOutGoingState(pBdAddr))
			isInCall = TRUE;
		else
			isInCall = FALSE;

		RING_START:
		//LightDbgPrint("app_ImgMedia_RingHandler!!!,%d",(U8)isInCall);
		//if(SYS_IsBDAddrIsTheSame(&gMMI_ctl.mmiInfo[linkIndex].bdAddr[0], &gMMI_driver_variation_nvram.misc_para.agInd[0].member[0]))
		{
			APP_Media_PushMediaEvent((isInCall) ? MEDIA_EVT_RINGTONE_IN_CALL : MEDIA_EVT_RINGTONE);
		}
		//else
		//{
		//	APP_Media_PushMediaEvent((isInCall) ? MEDIA_EVT_RINGTONE2_IN_CALL : MEDIA_EVT_RINGTONE_2);
		//}
		return IMG_AUDIO_VOICE_LOAD_OK;
	}

	return IMG_AUDIO_VOICE_LOAD_INVALID;
}

static void app_ImgMedia_ImgLoadAudioVoice(BD_ADDR_T *pBdAddr)
{
	U8 result = IMG_AUDIO_VOICE_LOAD_STATE_ERROR;

	switch(gAppHfpImgMediaCtl.imgAudioVoiceState)
	{
		case APP_HFP_IMG_MEDIA_VP:
			result = app_ImgMedia_IncomingVPHandler(pBdAddr);
			break;
		case APP_HFP_IMG_MEDIA_CALLER_NAME:
			result = app_ImgMedia_CallerNameHandler(pBdAddr);
			break;
		case APP_HFP_IMG_MEDIA_CALLER_ID:
			result = app_ImgMedia_CallerIDHandler(pBdAddr);
			break;
		case APP_HFP_IMG_MEDIA_RINGTONE:
			result = app_ImgMedia_RingHandler(pBdAddr);
			break;
	}
	if(result == IMG_AUDIO_VOICE_LOAD_STATE_ERROR)
	{
		// cancel this link
		gAppHfpImgMediaCtl.imgAudioVoiceState = APP_HFP_IMG_MEDIA_NONE;
		APP_Media_SendFakeMediaCmd(MEDIA_EVT_IMGCALL_MEDIA_NOT_FOUND);
	}
	else if(result == IMG_AUDIO_VOICE_LOAD_PEDNING)
	{
		FW_SetTimer((Handler)&gAppImgCallMediaTimerHandler, 0, NULL, 0, MILLI_SEC_100);
	}
	else if(result == IMG_AUDIO_VOICE_LOAD_OK)
	{
	}
	else
	{
		//decide audio voice
		APP_Media_SendFakeMediaCmd(MEDIA_EVT_IMGCALL_MEDIA_NOT_FOUND);
	}
}

static U32 app_ImgCallMediaTimerHandler(Handler handler, U16 id, void* msg, U32 handler_id)
{
	UNUSED(handler);
	UNUSED(id);
	UNUSED(msg);
	UNUSED(handler_id);
	APP_ImgMedia_AudioVoiceHandler(&gAppHfpImgMediaCtl.bdAddr, IMG_AUDIO_VOICE_TIMER_TO);
	return 0;
}

/******************************************************************************
 * Public Function
*******************************************************************************/
void APP_ImgMedia_AudioVoiceHandler(BD_ADDR_T *pBdAddr, U8 action)
{

	if(app_ImgCallGetMediaPriority(0) == APP_HFP_IMG_MEDIA_NONE)
		return;

	if(action == IMG_AUDIO_VOICE_TRIGGER) //step 1: initialize.
	{
		if(gAppHfpImgMediaCtl.imgAudioVoiceState == APP_HFP_IMG_MEDIA_NONE) //initialize state and start to play sound.
		{

			//#ifdef BLE_AIRApp
			//if(gMMI_HFP_ctl.HfpCtl[linkIndex].incomingCall.length && gMMI_HFP_ctl.HfpCtl[linkIndex].isCallerIdGet && SDAP_IsIOS(linkIndex))
			//	SendPhoneNumberToApp(linkIndex, gMMI_HFP_ctl.HfpCtl[linkIndex].incomingCall.length, (U8)(app_ImgCallGetMediaPeriodTime()/10));
			//#endif

			FW_Memcpy(&gAppHfpImgMediaCtl.bdAddr, pBdAddr, sizeof(BD_ADDR_T));
			gAppHfpImgMediaCtl.imgAudioVoiceState = app_ImgCallGetMediaPriority(0);
			FW_SetTimer((Handler)&gAppImgCallMediaTimerHandler, 0, NULL, 0, 3 * ONE_SEC); // wait BSIR and CLIP and SCO --> 3 seconds
		}
	}
	else if(action == IMG_AUDIO_VOICE_FAKE_EVT) //step 2: select a state.
	{
		U8 i;
		APP_HFP_CTL_STRU *pHfpLinkInfo;

		if(!FW_CmpBdAddr(&gAppHfpImgMediaCtl.bdAddr, pBdAddr) || FW_CheckTimer((Handler)&gAppImgCallMediaTimerHandler, 0, 0))
			return;

		if(	(pHfpLinkInfo = APP_GetHfpLinkInfo(pBdAddr)) == (APP_HFP_CTL_STRU *)NULL ||
			gAppHfpImgMediaCtl.imgAudioVoiceState == APP_HFP_IMG_MEDIA_NONE) //current link is none or nothing to do, check the other link.
		{
			BD_ADDR_T *pOtherLinkBdAddr;
			if((pOtherLinkBdAddr = APP_State_CheckOtherLinksInIncomingState(pBdAddr)) != (BD_ADDR_T *)NULL)
			{
				FW_Memcpy(&gAppHfpImgMediaCtl.bdAddr, pOtherLinkBdAddr, sizeof(BD_ADDR_T));
				gAppHfpImgMediaCtl.imgAudioVoiceState = app_ImgCallGetMediaPriority(0);
				FW_SetTimer((Handler)&gAppImgCallMediaTimerHandler, 0, NULL, 0, app_ImgCallGetMediaPeriodTime() / 2);
			}
			else if(APP_State_CheckLinkInImgAndCaImgState(pBdAddr))
			{
				gAppHfpImgMediaCtl.imgAudioVoiceState = app_ImgCallGetMediaPriority(0);
				FW_SetTimer((Handler)&gAppImgCallMediaTimerHandler, 0, NULL, 0, app_ImgCallGetMediaPeriodTime() * 2);
			}
		}
		else //current link is something to do.
		{
			i = 0;
			while(i < APP_HFP_IMG_MEDIA_NUM)
			{
				if(app_ImgCallGetMediaPriority(i) == gAppHfpImgMediaCtl.imgAudioVoiceState) //find the state priority.
				{
					if(i == (APP_HFP_IMG_MEDIA_NUM -1) || app_ImgCallGetMediaPriority(i+1) == APP_HFP_IMG_MEDIA_NONE) //the state is the last.
					{
						BD_ADDR_T *pOtherLinkBdAddr;

						if((pOtherLinkBdAddr = APP_State_CheckOtherLinksInIncomingState(pBdAddr)) != (BD_ADDR_T *)NULL) //check the other link.
						{
							FW_Memcpy(&gAppHfpImgMediaCtl.bdAddr, pOtherLinkBdAddr, sizeof(BD_ADDR_T));
							gAppHfpImgMediaCtl.imgAudioVoiceState = app_ImgCallGetMediaPriority(0);
							FW_SetTimer((Handler)&gAppImgCallMediaTimerHandler, 0, NULL, 0, app_ImgCallGetMediaPeriodTime() / 2);
						}
						#ifdef BLE_AIRApp
						//else if(MMI_HFP_IsBleAppCallerNameOn(linkIndex))
						//{
						//	gAppHfpImgMediaCtl.imgAudioVoiceState = APP_HFP_IMG_MEDIA_NONE;
						//	app_ImgCallMediaReleaseTimer();
						//	pHfpLinkInfo->isImgVPTriggered = FALSE;
						//}
						#endif
						else if(APP_State_CheckLinkInImgAndCaImgState(pBdAddr)) //if there is a incoming call, do the first.
						{
							gAppHfpImgMediaCtl.imgAudioVoiceState = app_ImgCallGetMediaPriority(0);
							FW_SetTimer((Handler)&gAppImgCallMediaTimerHandler, 0, NULL, 0, app_ImgCallGetMediaPeriodTime());
						}
						else //no any incoming call, terminate.
						{
							gAppHfpImgMediaCtl.imgAudioVoiceState = APP_HFP_IMG_MEDIA_NONE;
							FW_ReleaseTimer((Handler)&gAppImgCallMediaTimerHandler, 0, 0);
							pHfpLinkInfo->isImgVPTriggered = FALSE;
						}
					}
					else //there is something to do after the action.
					{
						gAppHfpImgMediaCtl.imgAudioVoiceState = app_ImgCallGetMediaPriority(i+1);
						FW_SetTimer((Handler)&gAppImgCallMediaTimerHandler, 0, NULL, 0, MILLI_SEC_100);
					}
					break;
				}
				i++;
			}
		}
	}
	else if(action == IMG_AUDIO_VOICE_TIMER_TO) //step 3: start to play sound.
	{
		app_ImgMedia_ImgLoadAudioVoice(pBdAddr);
	}
	else if(action == IMG_AUDIO_VOICE_STOP)
	{
		if(FW_CmpBdAddr(&gAppHfpImgMediaCtl.bdAddr, pBdAddr))
		{
			BD_ADDR_T *otherBdAddr;
			gAppHfpImgMediaCtl.imgAudioVoiceState = APP_HFP_IMG_MEDIA_NONE;
			FW_ReleaseTimer((Handler)&gAppImgCallMediaTimerHandler, 0, 0);
			APP_HFP_StopIncomingVPRing(pBdAddr);
			if((otherBdAddr = APP_GetNextNormalLink(pBdAddr)) != (BD_ADDR_T *)NULL && APP_State_CheckLinkInImgAndCaImgState(otherBdAddr))
			{
				DBG_LOG_APP_HFP( "[APP_HFP] ImgAudioVoice:STOP New", 0);
				FW_Memcpy(&gAppHfpImgMediaCtl.bdAddr, otherBdAddr, sizeof(BD_ADDR_T));
				gAppHfpImgMediaCtl.imgAudioVoiceState = app_ImgCallGetMediaPriority(0);
				FW_SetTimer((Handler)&gAppImgCallMediaTimerHandler, 0, NULL, 0, 3 * ONE_SEC);
			}
		}
	}
}

void APP_ImgMedia_FakeEvtHandler(BD_ADDR_T *pBdAddr)
{
	if(pBdAddr)
		APP_ImgMedia_AudioVoiceHandler(pBdAddr, IMG_AUDIO_VOICE_FAKE_EVT);
}

void App_HfpImgCallMediaInitNvkey(U32 pNvkey, U16 length)
{
	if(pNvkey && length == sizeof(NVKEY_CIMG_CALL_MEDIA_STRU))
		gNvkeyImgCallMediaAddr = (NVKEY_CIMG_CALL_MEDIA_STRU const *)pNvkey;
}

void App_HfpImgCallMediaResetTimer(void)
{
    if(FW_CheckTimer((Handler)&gAppImgCallMediaTimerHandler, 0, 0))
    {
        FW_ReleaseTimer((Handler)&gAppImgCallMediaTimerHandler, 0, 0);
        FW_SetTimer((Handler)&gAppImgCallMediaTimerHandler, 0, NULL, 0, 10);
    }    
}


BOOL APP_HfpImgMedia_FillRhoData(void *pData)
{
	APP_RHO_HFP_IMG_MEDIA_STRU *pDataToFill = &((APP_RHO_HFP_STRU *)pData)->appHfpImgMedia;

	if(!pDataToFill)
	{
		DBG_LOG_APP_HFP( "[RHO_APP_HfpImg] Old Agent fill data error", 0);
		return FALSE;
	}

	FW_Memcpy(pDataToFill, &gAppHfpImgMediaCtl, sizeof(APP_HFP_IMG_MEDIA_STRU));

	pDataToFill->queueNo = MSG_MessageSearchAllHandler((Handler)&gAppImgCallMediaTimerHandler);
	FW_ReleaseTimer((Handler)&gAppImgCallMediaTimerHandler, 0, 0);
	DBG_LOG_APP_HFP( "[RHO_APP_HfpImg] Old Agent Queue number:%d", 1, pDataToFill->queueNo);
	
	return TRUE;
}

BOOL APP_HfpImgMedia_AssignRhoData(void *pData)
{
	APP_RHO_HFP_IMG_MEDIA_STRU *pDataToGet = &((APP_RHO_HFP_STRU *)pData)->appHfpImgMedia;

	if(!pDataToGet)
	{
		DBG_LOG_APP_HFP( "[RHO_APP_HfpImg] New Agent Assign data error", 0);
		return FALSE;
	}

	FW_Memcpy(&gAppHfpImgMediaCtl, pDataToGet, sizeof(APP_HFP_IMG_MEDIA_STRU));

	DBG_LOG_APP_HFP( "[RHO_APP_HfpImg] New Agent Assign Queue number:%d, state:%d", 2, pDataToGet->queueNo, gAppHfpImgMediaCtl.imgAudioVoiceState);
	if(pDataToGet->queueNo)
	{
		if(gAppHfpImgMediaCtl.imgAudioVoiceState == app_ImgCallGetMediaPriority(0))
			FW_SetTimer((Handler)&gAppImgCallMediaTimerHandler, 0, NULL, 0,  app_ImgCallGetMediaPeriodTime());
		else
			FW_SetTimer((Handler)&gAppImgCallMediaTimerHandler, 0, NULL, 0,  MILLI_SEC_100);
	}
	
	return TRUE;
}

U8 APP_HfpImgMedia_GetMsgNumInQueue(void)
{
	//return MSG_MessageSearchAllHandler((Handler)&gAppImgCallMediaTimerHandler);
	return 0;
}

