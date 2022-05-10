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
#include "App_Connection.h"
#include "App_Interface.h"
#include "App_Avrcp.h"
#include "App_AvrcpKey.h"
#include "Avrcp_Interface.h"
#include "Avrcp_DataHandler.h" //for test code
#include "App_A2dp.h"
#include "A2dp_Interface.h"
#include "App_Avrcp_NVKeyHandler.h"
#include "App_VolumeManage.h"
#include "App_Nvram_Type.h"
#include "App_ReConnection.h"
#include "App_A2dp_NVKeyHandler.h"
#include "App_LinkKey.h"
#include "App_VolumeA2dpProcess.h"
#include "PM_Interface.h"
#ifdef PROFILE_AMA_ENABLE
#include "BtAma.h"
#include "App_Ama.h"
#endif


#ifdef MODULE_LOG_FEATURE
#include "BtModule_log.h"
#endif

#include "BtSdps.h"

#define DBG_LOG_APP_AVRCP(_message, arg_cnt, ...)  LOG_MSGID_I(APP_AVRCP, _message, arg_cnt, ##__VA_ARGS__)


log_create_module(APP_AVRCP, PRINT_LEVEL_INFO);

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static U32 app_AvrcpIndHandler(Handler handler, U16 id, void *msg, U32 handler_id);
static U32 app_AvrcpTimerHandler(Handler handler, U16 id, void *msg, U32 handler_id);

/**************************************************************************************************
* Variable
**************************************************************************************************/
static const HandlerData gAppAvrcpHandle = { app_AvrcpIndHandler };
static const HandlerData gAppAvrcpTimerHandle = { app_AvrcpTimerHandler };

/**************************************************************************************************
* Static Functions
**************************************************************************************************/
void App_Avrcp_KeyVolUpDownCmdHandler(BD_ADDR_T *pBdAddr, BOOL isVolUp)
{
	U8 currentSoundLevel = APP_VolManage_GetCurrentA2dpSoundLevel(pBdAddr);

	if (isVolUp && APP_VolManage_GetCurrentA2dpSoundLevel(pBdAddr) < APP_MAX_A2DP_SOUND_LEVEL)
	{
		currentSoundLevel += 1;
<<<<<<< HEAD
		#ifdef A2DP_Profile
=======
		#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
		APP_VolManage_SetCurrentA2dpSoundLevel(pBdAddr, (U8)(currentSoundLevel));
		#endif
		APP_VolManage_SetCurrentAbsoluteVolume(pBdAddr, APP_VolManage_GetAbsoluteVolumeBySoundLevel(currentSoundLevel));
		AVRCP_VolumeChangedNotifyResponse(pBdAddr, APP_VolManage_GetCurrentAbsoluteVolume(pBdAddr));

		APP_VolA2dp_RemoteHandler(pBdAddr, currentSoundLevel);
	}
	else if (!isVolUp && APP_VolManage_GetCurrentA2dpSoundLevel(pBdAddr) > 0)
	{
		currentSoundLevel -= 1;
<<<<<<< HEAD
		#ifdef A2DP_Profile
=======
		#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
		APP_VolManage_SetCurrentA2dpSoundLevel(pBdAddr, (U8)(currentSoundLevel));
		#endif
		APP_VolManage_SetCurrentAbsoluteVolume(pBdAddr, APP_VolManage_GetAbsoluteVolumeBySoundLevel(currentSoundLevel));
		AVRCP_VolumeChangedNotifyResponse(pBdAddr, APP_VolManage_GetCurrentAbsoluteVolume(pBdAddr));

		APP_VolA2dp_RemoteHandler(pBdAddr, currentSoundLevel);
	}
}

static void app_AvrcpTgGetVolChanged(BD_ADDR_T *pBdAddr, U8 absVol)
{
	AVRCP_SetNotifyMask(pBdAddr, APP_AVRCP_VOLUME_CHANGED);
	UNUSED(absVol);
}

static void app_AvrcpSetPlayStatus(BD_ADDR_T *pBdAddr, U8 playStatus)
{
	APP_AVRCP_CTL_STRU *pLinkInfo = APP_GetAvrcpLinkInfo(pBdAddr);

	if(pLinkInfo)
	{
		pLinkInfo->playStatus = playStatus;
	}
}

static void app_AvrcpGetRemotePlayStatus(BD_ADDR_T *pBdAddr, U8 playStatus)
{
	APP_AVRCP_CTL_STRU *pLinkInfo = APP_GetAvrcpLinkInfo(pBdAddr);

	if(pLinkInfo)
	{
		//For LG phone send pause-->play-->pause event notification after sending AVRCP pause
<<<<<<< HEAD
		#ifdef A2DP_Profile
=======
		#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
		//if (A2DP_IsMusicSuspending(pBdAddr)) //Now suspend timer is cancelled
		//{
		//	if (pLinkInfo->playStatus != playStatus && playStatus == AVRCP_PLAYBACK_STATUS_PLAYING)
		//	{
		//		pLinkInfo->playStatus = playStatus;
		//		return;
		//	}
		//}
		#endif

		DBG_LOG_APP_AVRCP( "[APP_AVRCP]CurrentPlayStatus:%d remotePlayStatus:%d", 2, pLinkInfo->playStatus, playStatus);

		if (pLinkInfo->playStatus != AVRCP_PLAYBACK_STATUS_NONE)		//issue #6396
		{
			if (pLinkInfo->playStatus != playStatus)
			{
				if (playStatus == AVRCP_PLAYBACK_STATUS_PLAYING)
				{
					APP_AvrcpSetMusicPlay(pBdAddr, TRUE);
				}
				else if (playStatus == AVRCP_PLAYBACK_STATUS_STOPPED || playStatus == AVRCP_PLAYBACK_STATUS_PAUSED)
				{
					APP_AvrcpSetMusicPlay(pBdAddr, FALSE);
				}
<<<<<<< HEAD
				#if defined(A2DP_Profile) && defined(AVRCP_Profile)
=======
				#if defined(AIR_A2DP_PROFILE_ENABLE) && defined(AIR_AVRCP_PROFILE_ENABLE)
>>>>>>> db20e11 (second commit)
				APP_A2dp_PlayStatusChanged(pBdAddr, playStatus);
				#endif
				pLinkInfo->isPlayStatusWorking = TRUE;
				APP_AvrcpResetKeyPlayToggle(pBdAddr);
			}
			else
			{
<<<<<<< HEAD
				#if defined(A2DP_Profile) && defined(AVRCP_Profile)
=======
				#if defined(AIR_A2DP_PROFILE_ENABLE) && defined(AIR_AVRCP_PROFILE_ENABLE)
>>>>>>> db20e11 (second commit)
				APP_A2dp_PlayStatusTheSame(pBdAddr, playStatus);
				#endif
			}
		}
		else
		{
<<<<<<< HEAD
			#if defined(A2DP_Profile) && defined(AVRCP_Profile)
=======
			#if defined(AIR_A2DP_PROFILE_ENABLE) && defined(AIR_AVRCP_PROFILE_ENABLE)
>>>>>>> db20e11 (second commit)
			APP_A2dp_CheckPlayStatusConsistency(pBdAddr, playStatus, TRUE);
			#endif
		}
		pLinkInfo->playStatus = playStatus;
	}
}

/**************************************************************************************************
* Internal Public Functions
**************************************************************************************************/
void APP_Avrcp_RegisterProfile()
{
	AVRCP_StartService((Handler)&gAppAvrcpHandle);
}

void APP_AvrcpDecideOperation(BD_ADDR_T *pBdAddr, U8 opID)
{
	#ifdef PROFILE_AMA_ENABLE
	APP_AMA_CTL_STRU *pAmaLinkInfo = APP_Ama_GetLinkInfo();
	#endif

	APP_AVRCP_CTL_STRU *pLinkInfo = APP_GetAvrcpLinkInfo(pBdAddr);
	U8 playStatus = APP_Avrcp_GetPlayStatus(pBdAddr);

	if(!pLinkInfo)
		return;

	DBG_LOG_APP_AVRCP( "[APP_AVRCP] Decide Operation: %s", 1, AVRCP_OPID_STR[Avrcp_OperandID2StrIndex(opID)]);

	#ifdef MODULE_LOG_FEATURE
	BtModuleLog_SendLogHandler(MODULE_LOG_MODULE_APP_AVRCP, 
					MODULE_LOG_MODULE_APP_AVRCP_SUB_MODULE_DECIDE_OPERATION, opID);
	#endif

	switch (opID)
	{
		case AVC_OPERATION_KEY_PAUSE:
			opID = AVC_OPERATION_PAUSE;
			if (APP_MUSIC_PLAY_PAUSE_ONEKEY_FEAT)
			{
				if (pLinkInfo->lastTxOpIDForPlayPause != 0)
				{
					if (pLinkInfo->lastTxOpIDForPlayPause == AVC_OPERATION_PAUSE)
						opID = AVC_OPERATION_PLAY;
					else
						opID = AVC_OPERATION_PAUSE;
				}
				else if (pLinkInfo->isPlayStatusWorking)
				{
					if (playStatus == AVRCP_PLAYBACK_STATUS_PLAYING)
						opID = AVC_OPERATION_PAUSE;
					else if (playStatus == AVRCP_PLAYBACK_STATUS_STOPPED || playStatus == AVRCP_PLAYBACK_STATUS_PAUSED)
						opID = AVC_OPERATION_PLAY;
					else
						return;
				}
				else
				{
					if (!APP_KEY_PLAY_PAUSE_DONT_GET_PLAY_STATUS && pLinkInfo->isGetPlayStatusSupported)
						opID = AVC_OPERATION_KEY_PAUSE;
					else
					{
						if (!pLinkInfo->isMusicPlay
<<<<<<< HEAD
							#ifdef A2DP_Profile
=======
							#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
							|| !APP_a2dp_is_in_streaming(pBdAddr)
							#endif
							)
						{
							opID = AVC_OPERATION_PLAY;
						}
					}
				}
				pLinkInfo->lastTxOpIDForPlayPause = opID;
			}
			break;

        case AVC_OPERATION_FORCE_PLAY:
            opID = AVC_OPERATION_PLAY;
            pLinkInfo->lastTxOpIDForPlayPause = opID;
            break;

		case AVC_OPERATION_KEY_PLAY:
			opID = AVC_OPERATION_PLAY;
			if (APP_MUSIC_PLAY_PAUSE_ONEKEY_FEAT)
			{
				if (pLinkInfo->lastTxOpIDForPlayPause != 0)
				{
					if (pLinkInfo->lastTxOpIDForPlayPause == AVC_OPERATION_PAUSE)
						opID = AVC_OPERATION_PLAY;
					else
						opID = AVC_OPERATION_PAUSE;
				}
				else if (pLinkInfo->isPlayStatusWorking)
				{
					if (playStatus == AVRCP_PLAYBACK_STATUS_PLAYING)
						opID = AVC_OPERATION_PAUSE;
					else if (playStatus == AVRCP_PLAYBACK_STATUS_STOPPED || playStatus == AVRCP_PLAYBACK_STATUS_PAUSED)
						opID = AVC_OPERATION_PLAY;
					else
						return;
				}
				else
				{
					if (!APP_KEY_PLAY_PAUSE_DONT_GET_PLAY_STATUS && pLinkInfo->isGetPlayStatusSupported)
						opID = AVC_OPERATION_KEY_PLAY;
					else
					{
						if (pLinkInfo->isMusicPlay
<<<<<<< HEAD
							#ifdef A2DP_Profile
=======
							#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
							&& APP_a2dp_is_in_streaming(pBdAddr)
							#endif
							)
						{
							opID = AVC_OPERATION_PAUSE;
						}
					}
				}
				pLinkInfo->lastTxOpIDForPlayPause = opID;
			}
			break;

		case AVC_OPERATION_PLAY:
			if (!pLinkInfo->isGetPlayStatusSupported && pLinkInfo->isMusicPlay
<<<<<<< HEAD
				#ifdef A2DP_Profile
=======
				#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
				&& a2dp_is_in_streaming(pBdAddr)
				#endif
				)
			{
				opID = AVC_OPERATION_PAUSE;
			}
			break;

		case AVC_OPERATION_PAUSE:
			if ((!pLinkInfo->isGetPlayStatusSupported && !pLinkInfo->isMusicPlay)
<<<<<<< HEAD
				#ifdef A2DP_Profile
=======
				#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
				|| !a2dp_is_in_streaming(pBdAddr)
				#endif
				)
			{
				opID = AVC_OPERATION_PLAY;
			}
			break;

		case AVC_OPERATION_REWIND:
		case AVC_OPERATION_FAST_FORWARD:
		case AVC_OPERATION_STOP:
		case AVC_OPERATION_FORWARD:
		case AVC_OPERATION_BACKWARD:
		case AVC_OPERATION_INTERNAL_REWIND_RELEASE:
		case AVC_OPERATION_INTERNAL_FAST_FORWARD_RELEASE:
<<<<<<< HEAD
			#ifdef A2DP_Profile
=======
			#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
			if (!a2dp_is_connected(pBdAddr))
				return;
			#endif
			break;

		case AVC_OPERATION_GET_PLAY_STATUS:
			if (pLinkInfo->isSendingGetPlayStatus == AVRCP_PLAY_STATUS_SENT)
				return;

			pLinkInfo->isSendingGetPlayStatus = AVRCP_PLAY_STATUS_SENT;
			break;

		case AVC_OPERATION_VOL_UP:
		case AVC_OPERATION_VOL_DOWN:
			break;
			
		case AVC_OPERATION_FORCE_PAUSE:
<<<<<<< HEAD
			#ifdef CODEC_SWITCH_WORKAROUND_SONY_XPERIA
=======
			#ifdef AIR_CODEC_SWITCH_WORKAROUND_SONY_XPERIA_ENABLE
>>>>>>> db20e11 (second commit)
			if(bt_sdps_pnp_is_sony_xperia(pBdAddr))
			{
				DBG_LOG_APP_AVRCP( "[APP_AVRCP] [Codec Work-Around] Force Pause", 0);
				opID = AVC_OPERATION_PAUSE;
	            pLinkInfo->lastTxOpIDForPlayPause = opID;
			}
			#endif
			break;
	}

    #ifdef PROFILE_AMA_ENABLE
	if(AMA_Target_IsEnable())
	{
		if(APP_Ama_GetLinkAVRCPOverride())
		{
			AMA_MEDIA_CONTROL_E control = AMA_MEDIA_CONTROL_PLAY;
			switch(opID)
			{
		        case AVC_OPERATION_PLAY:
		            control = AMA_MEDIA_CONTROL_PLAY;
		            break;

		        case AVC_OPERATION_PAUSE:
		            control = AMA_MEDIA_CONTROL_PAUSE;
		            break;

		        case AVC_OPERATION_FORWARD:
		            control = AMA_MEDIA_CONTROL_NEXT;
		            break;

		        case AVC_OPERATION_BACKWARD:
		            control = AMA_MEDIA_CONTROL_PREVIOUS;
		            break;

		        default:
		            control = AMA_MEDIA_CONTROL_PAUSE;
		            break;

			}
			if(pAmaLinkInfo)
			{
				DBG_LOG_APP_AVRCP( "[APP_AVRCP] AMA control id:%d", 1, control);				
    			AMA_Target_IssueMediaControl(&pAmaLinkInfo->bdAddr, control);

				if(control == AMA_MEDIA_CONTROL_PLAY)
				{
					APP_Avrcp_SetTimer(pBdAddr, APP_AVRCP_TIMER_AMA_MEDIA_CONTROL_PLAY, 1 * ONE_SEC, 0);
				}
			}
			//return;
		}
	}
    #endif

	DBG_LOG_APP_AVRCP( "[APP_AVRCP] Send Operation: %s", 1, AVRCP_OPID_STR[Avrcp_OperandID2StrIndex(opID)]);

	#ifdef MODULE_LOG_FEATURE
	BtModuleLog_SendLogHandler(MODULE_LOG_MODULE_APP_AVRCP, 
					MODULE_LOG_MODULE_APP_AVRCP_SUB_MODULE_SEND_OPERATION, opID);
	#endif

	AVRCP_SendOperation(pBdAddr, opID, 0);
}

void APP_Avrcp_SetTimer(BD_ADDR_T *pBdAddr, U16 id, U32 time, U32 para)
{
	APP_MSG_TYPE_STRU *pMsg;
	APP_AVRCP_CTL_STRU * pLinkInfo = APP_GetAvrcpLinkInfo(pBdAddr);

	if (!pLinkInfo || FW_CheckTimer((Handler)&gAppAvrcpTimerHandle, id, (U32)pLinkInfo))
		return;

	pMsg = (APP_MSG_TYPE_STRU *)NULL;
	if(para || id == APP_AVRCP_TIMER_BACKUP_AND_PUSH_PLAY_STATUS || id == APP_AVRCP_TIMER_CHECK_STREAMING_STATE_LATER)
	{
		if((pMsg = APP_GetAppCmdMsgMemory(id)) != NULL)
		{
			pMsg->pMsgBody.avrcpTaskCmd.fcallback = (VOID *)NULL;
			if (id == APP_AVRCP_TIMER_BACKUP_AND_PUSH_PLAY_STATUS || id == APP_AVRCP_TIMER_CHECK_STREAMING_STATE_LATER)
			{
				pMsg->pMsgBody.avrcpTaskCmd.playStatus = (U8)para;
			}
			else
			{
				pMsg->pMsgBody.avrcpTaskCmd.fcallback = (VOID *)para;
			}
		}
		else
		{
			FW_Assert(FALSE);
		}
	}
	FW_SetTimer((Handler)&gAppAvrcpTimerHandle, id, pMsg, (U32)pLinkInfo, time);
}

U16 APP_Avrcp_ReleaseTimer(BD_ADDR_T *pBdAddr, U16 id)
{
	APP_AVRCP_CTL_STRU * pLinkInfo = APP_GetAvrcpLinkInfo(pBdAddr);

	if (pLinkInfo)
	{

		FW_ReleaseTimer((Handler)&gAppAvrcpTimerHandle, id, (U32)pLinkInfo);
		return 1;
	}
	return 0;
}

U16 APP_Avrcp_CheckTimer(BD_ADDR_T *pBdAddr, U16 id)
{
	APP_AVRCP_CTL_STRU * pLinkInfo = APP_GetAvrcpLinkInfo(pBdAddr);

	if (pLinkInfo)
	{
		if(FW_CheckTimer((Handler)&gAppAvrcpTimerHandle, id, (U32)pLinkInfo))
		{
			FW_ReleaseTimer((Handler)&gAppAvrcpTimerHandle, id, (U32)pLinkInfo);
			return 1;
		}
	}
	return 0;
}

void APP_Avrcp_ReleaseAllTimer(BD_ADDR_T *pBdAddr)
{
	APP_AVRCP_CTL_STRU * pLinkInfo;
	if(pBdAddr && (pLinkInfo = APP_GetAvrcpLinkInfo(pBdAddr)))
	{
		MSG_MessageCancelEx2((Handler)&gAppAvrcpTimerHandle, (U32)pLinkInfo);
	}
}


void APP_AvrcpCheckStreamingStateLater(BD_ADDR_T *pBdAddr, U8 playStatus, U8 isNeedDelay)
{
	APP_Avrcp_ReleaseTimer(pBdAddr, APP_AVRCP_TIMER_CHECK_STREAMING_STATE_LATER);

	APP_Avrcp_SetTimer(pBdAddr, APP_AVRCP_TIMER_CHECK_STREAMING_STATE_LATER, (isNeedDelay)? (2*ONE_SEC):HALF_SEC, (U32)playStatus);
}

void APP_AvrcpSetMusicPlay(BD_ADDR_T *pBdAddr, BOOL isPlay)
{
	APP_AVRCP_CTL_STRU *pLinkInfo = APP_GetAvrcpLinkInfo(pBdAddr);

	if(pLinkInfo)
		pLinkInfo->isMusicPlay = isPlay;
}

BOOL APP_AvrcpIsMusicPlaying(BD_ADDR_T *pBdAddr)
{
	APP_AVRCP_CTL_STRU *pLinkInfo = APP_GetAvrcpLinkInfo(pBdAddr);

	if(pLinkInfo)
		return pLinkInfo->isMusicPlay;

	return FALSE;
}

void APP_AvrcpResetKeyPlayToggle(BD_ADDR_T *pBdAddr)
{
	APP_AVRCP_CTL_STRU *pLinkInfo = APP_GetAvrcpLinkInfo(pBdAddr);

	if (pLinkInfo)
		pLinkInfo->lastTxOpIDForPlayPause = 0; //reset for music play updating
}

BOOL APP_AvrcpIsGetAppSettingsIdle(BD_ADDR_T *pBdAddr)
{
	APP_AVRCP_CTL_STRU *pLinkInfo = APP_GetAvrcpLinkInfo(pBdAddr);

	return (pLinkInfo->getAppSettingsOwner == AVRCP_GET_APP_SETTINGS_NONE) ? TRUE : FALSE;
}

void APP_AvrcpSdkSetGetAppSettingsOwner(BD_ADDR_T *pBdAddr)
{
	APP_AVRCP_CTL_STRU *pLinkInfo = APP_GetAvrcpLinkInfo(pBdAddr);

	pLinkInfo->getAppSettingsOwner = AVRCP_GET_APP_SETTINGS_SDK;
}

U8 APP_Avrcp_GetPlayStatus(BD_ADDR_T *pBdAddr)
{
	APP_AVRCP_CTL_STRU *pLinkInfo = APP_GetAvrcpLinkInfo(pBdAddr);

	if(pLinkInfo)
	{
		if (pLinkInfo->isDisablePlayStatusMonitor)
		{
			return pLinkInfo->playStatus;
		}
		else
		{
			return AVRCP_PLAYBACK_STATUS_NONE;
		}
	}
	return AVRCP_PLAYBACK_STATUS_NONE;
}

/**************************************************************************************************
* Static Functions (Indication Handler)
**************************************************************************************************/
static void app_AvrcpTgGetSetAbsVolIndHandler(BD_ADDR_T *pBdAddr, U8 absVol)
{

	DBG_LOG_APP_AVRCP( "[APP_AVRCP] AVRCP_GetSetAbsVol - BdAddr:0x%x%x, absVol:%d", 3, FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE) ,absVol);

<<<<<<< HEAD
	#ifdef A2DP_Profile
=======
	#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
	APP_VolManage_SpkVolumeRemoteHandler(pBdAddr, APP_VolManage_GetSoundLevelByAbsoluteVolume(absVol), COMPONENT_A2DP);
	#endif
}

static void app_AvrcpPassThroughIndHandler(AVRCP_GET_CMD_PASSTHROUGH_IND_T *msg)
{
	if (msg->resultCode == AVC_ERROR_CODE_NO_ERROR && msg->buttonReleased == PUSHED)
	{
		if (AVC_OPERATION_POWER <= msg->operandID && msg->operandID <= AVC_OPERATION_BACKWARD)
		{
			DBG_LOG_APP_AVRCP( "[APP_AVRCP] PassThrough Ind, OPID:%s", 1, AVRCP_OPID_STR[Avrcp_OperandID2StrIndex(msg->operandID)]);

			switch (msg->operandID)
			{
				case AVC_OPERATION_VOL_UP:
					App_Avrcp_KeyVolUpDownCmdHandler(&msg->bdAddr, TRUE);
					break;

				case AVC_OPERATION_VOL_DOWN:
					App_Avrcp_KeyVolUpDownCmdHandler(&msg->bdAddr, FALSE);
					break;

				#ifdef A2DP_SRC_Profile
				case AVC_OPERATION_PLAY:
					bt_a2dp_start_request(&msg->bdAddr);
					break;

				case AVC_OPERATION_STOP:
				case AVC_OPERATION_PAUSE:
					bt_a2dp_suspend_request(&msg->bdAddr);
					break;

				case AVC_OPERATION_FORWARD:
					break;

				case AVC_OPERATION_BACKWARD:
					break;
				#endif
			}
		}
	}
}

static void app_AvrcpPassThroughVendorUniqueIndHandler(AVRCP_GET_CMD_PASSTHROUGH_VENDORUNIQUE_IND_T *msg)
{
	if (msg->resultCode == AVC_ERROR_CODE_NO_ERROR && msg->buttonReleased == PUSHED)
	{
		if (msg->operandID == AVC_OPERATION_VENDOR_UNIQUE)
		{

		}
	}
}

static void app_AvrcpAppSettingRspIndHandler(BD_ADDR_T *pBdAddr, U8 attributeId, U8 attributeValue)
{
	APP_AVRCP_CTL_STRU *pLinkInfo = APP_GetAvrcpLinkInfo(pBdAddr);

	if (attributeValue == AVRCP_REPEAT_CMD_REJECT)
	{
		switch (pLinkInfo->getAppSettingsOwner)
		{
			case AVRCP_GET_APP_SETTINGS_UART:
				//MMI_AVRCP_SendPlayAppSettingsUartInd(linkIndex, AVRCP_REPEAT_CMD_REJECT, 0);
				break;

			case AVRCP_GET_APP_SETTINGS_SDK:
				break;
		}

		pLinkInfo->getAppSettingsOwner = AVRCP_GET_APP_SETTINGS_NONE;
		return;
	}

	if (attributeId == AVRCP_SHUFFLE_ON_OFF_STATUS)
	{
		switch (pLinkInfo->getAppSettingsOwner)
		{
			case AVRCP_GET_APP_SETTINGS_KEY:
				AVRCP_SendOperation(pBdAddr, AVC_OPERATION_SET_SHUFFLE_MODE, (U8)((attributeValue % AVRCP_SHUFFLE_MODE_NO) + 1));
				break;

			case AVRCP_GET_APP_SETTINGS_UART:
				//MMI_AVRCP_SendPlayAppSettingsUartInd(linkIndex, AVRCP_SHUFFLE_ON_OFF_STATUS, paramPtr->GetPlayAppSettings.attributeValue);
				break;

			case AVRCP_GET_APP_SETTINGS_SDK:
				break;
		}
	}
	else if (attributeId == AVRCP_REPEAT_MODE_STATUS)
	{
		switch (pLinkInfo->getAppSettingsOwner)
		{
			case AVRCP_GET_APP_SETTINGS_KEY:
				AVRCP_SetAppValueRequest(NULL, pBdAddr, AVC_OPERATION_SET_REPEAT_MODE, (U8)((attributeValue % AVRCP_REPEAT_MODE_NO) + 1));
				break;

			case AVRCP_GET_APP_SETTINGS_UART:
				//MMI_AVRCP_SendPlayAppSettingsUartInd(linkIndex, AVRCP_REPEAT_MODE_STATUS, paramPtr->GetPlayAppSettings.attributeValue);
				break;

			case AVRCP_GET_APP_SETTINGS_SDK:
				break;
		}
	}
	pLinkInfo->getAppSettingsOwner = AVRCP_GET_APP_SETTINGS_NONE;
}

static void app_AvrcpGetPlayStatusRspIndHandler(BD_ADDR_T *pBdAddr, BOOL isSendPlayPause, U8 playStatus)
{
	APP_AVRCP_CTL_STRU *pLinkInfo = APP_GetAvrcpLinkInfo(pBdAddr);

	DBG_LOG_APP_AVRCP( "[APP_AVRCP] Get Play Status Rsp Ind - isSendPlayPause:%d, Play Status:%d", 2, isSendPlayPause, playStatus);

	if (isSendPlayPause)
	{
		switch (playStatus)
		{
			case AVRCP_PLAYBACK_STATUS_PLAYING:
			case AVRCP_PLAYBACK_STATUS_FWD_SEEK:
			case AVRCP_PLAYBACK_STATUS_REV_SEEK:
				APP_AvrcpDecideOperation(pBdAddr, AVC_OPERATION_PAUSE);
				break;

			case AVRCP_PLAYBACK_STATUS_STOPPED:
			case AVRCP_PLAYBACK_STATUS_PAUSED:
				APP_AvrcpDecideOperation(pBdAddr, AVC_OPERATION_PLAY);
				break;

			default:
				if (pLinkInfo->isMusicPlay
<<<<<<< HEAD
					#ifdef A2DP_Profile
=======
					#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
					&& APP_a2dp_is_in_streaming(pBdAddr)
					#endif
					)
					APP_AvrcpDecideOperation(pBdAddr, AVC_OPERATION_PAUSE);
				else
					APP_AvrcpDecideOperation(pBdAddr, AVC_OPERATION_PLAY);
				break;
		}
	}
	else
	{
		if (playStatus != AVRCP_PLAYBACK_STATUS_REJECT)
		{
			APP_Avrcp_ReleaseTimer(pBdAddr, APP_AVRCP_TIMER_BACKUP_AND_PUSH_PLAY_STATUS);
		}

		if(pLinkInfo)
		{
			pLinkInfo->isGetPlayStatusSupported = (playStatus != AVRCP_PLAYBACK_STATUS_REJECT) ? TRUE : FALSE;
			pLinkInfo->isSendingGetPlayStatus = AVRCP_PLAY_STATUS_DONE;
		}
		app_AvrcpGetRemotePlayStatus(pBdAddr, playStatus);
	}
}

static void app_AvrcpRegisterNotifyRspIndHandler(AVRCP_GET_REGISTER_NOTIFY_IND_T *msg)
{
	U8 i;
	PLAY_APP_ELEMENT_STRU *pAttribute;

	DBG_LOG_APP_AVRCP( "[APP_AVRCP] RegisterNotify Rsp Ind - %s", 1, APP_AvrcpRegNotifyRspEvtID[msg->eventId]);

	switch (msg->eventId)
	{
		case AVRCP_EVENT_ID_PLAYBACK_STATUS_CHANGED:
		{
			APP_AVRCP_CTL_STRU *pLinkInfo = APP_GetAvrcpLinkInfo(&msg->bdAddr);

			APP_Avrcp_ReleaseTimer(&msg->bdAddr, APP_AVRCP_TIMER_BACKUP_AND_PUSH_PLAY_STATUS);
			APP_Avrcp_SetTimer(&msg->bdAddr, APP_AVRCP_TIMER_BACKUP_AND_PUSH_PLAY_STATUS, 50, (U32)msg->params.playStatus);
			if(pLinkInfo)
				pLinkInfo->isDisablePlayStatusMonitor = TRUE;
			else
				DBG_LOG_APP_AVRCP( "[APP_AVRCP] Warning!!! Can't get Link Info!!!", 0);
		}
		break;

		case AVRCP_EVENT_ID_TRACK_CHANGED:
		case AVRCP_EVENT_ID_TRACK_REACHED_END:
		case AVRCP_EVENT_ID_TRACK_REACHED_START:
			//MMI_AVRCP_SendTrackChangeUartEvt(linkIndex, paramPtr->Notify.eventID);
			break;

		case AVRCP_EVENT_ID_PLAYBACK_POS_CHANGED:
			//MMI_AVRCP_SendPlaybackPosUartEvt(linkIndex, paramPtr->Notify.eventParams.PlaybackPos);
			break;

		case AVRCP_EVENT_ID_VOLUME_CHANGED:
			app_AvrcpTgGetVolChanged(&msg->bdAddr, msg->params.absVolume);
			#ifdef FAST_CONNECT_DONGLE
			MMI_FCD_ReportVolume(HCI_VEVT_OCF_FCD_VOL_A2DP, paramPtr->Notify.eventParams.AbsoluteVolume);
			#else
			//MMI_HCI_UartSendUnSolicitedIndication(MMI_UART_IND_SET_ABSOLUTE_VOLUME, (U8 *)&paramPtr->Notify.eventParams.AbsoluteVolume, linkIndex);
			#endif
			break;

		case AVRCP_EVENT_ID_PLAYER_APPLICATION_SETTING_CHANGED:
			for (i = 0; i < msg->params.playAppSettings.attributeIDNum; i++)
			{
				pAttribute = &msg->params.playAppSettings.attribute[i];

				if (pAttribute->attributeID == AVRCP_REPEAT_MODE_STATUS || pAttribute->attributeID == AVRCP_SHUFFLE_ON_OFF_STATUS)
				{
					//MMI_AVRCP_SendPlayAppSettingsUartInd(linkIndex, pAttribute->attributeID, pAttribute->attributeValue);
				}
			}
			break;
	}
}

void app_AvrcpPassThroughRspIndHandler(BD_ADDR_T *pBdAddr, U8 operationId)
{

	DBG_LOG_APP_AVRCP( "[APP_AVRCP] PassThrough Rsp Ind: %d", 1, operationId);

	switch (operationId)
	{
		case AVRCP_IND_GET_STOP_REJECTED:
<<<<<<< HEAD
			#ifdef A2DP_Profile
=======
			#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
			APP_A2dp_ClearResumeState(pBdAddr);
			#endif
			break;

		case AVC_OPERATION_PLAY:
			APP_AvrcpSetMusicPlay(pBdAddr, TRUE);
			break;

		case AVC_OPERATION_PAUSE:
		case AVC_OPERATION_STOP:
			APP_AvrcpSetMusicPlay(pBdAddr, FALSE);
			break;
	}
}

/**************************************************************************************************
* Static (Message Handler)
**************************************************************************************************/
static U32 app_AvrcpIndHandler(Handler handler, U16 id, void *msg, U32 handler_id)
{
	BD_ADDR_T *pBdAddr;
	UNUSED(handler);
	UNUSED(handler_id);

	DBG_LOG_APP_AVRCP( "[APP_AVRCP] app_AvrcpIndHandler id - %s,%d", 2, APP_AvrcpIndID[id], id);

	switch(id)
	{
		case AVRCP_START_SERVICE_CFM:
			break;

		case AVRCP_STOP_SERVICE_CFM:
			break;

		case AVRCP_CONNECT_CFM:
			pBdAddr = &((AVRCP_CONNECT_CFM_T *)msg)->bdAddr;
			if(((AVRCP_CONNECT_CFM_T *)msg)->status == BT_STATUS_SUCCESS)
			{
				app_AvrcpSetPlayStatus(pBdAddr, AVRCP_PLAYBACK_STATUS_NONE);
				AVRCP_GetCapsRequest(NULL, pBdAddr, AVRCP_GET_CAPABILITIES_EVENTS_SUPPORTED);
				AVRCP_GetPlayStatusRequest(NULL, pBdAddr);

				if (gAppCtl.isEsdPowerOn)
				{
                    gAppCtl.isEsdPowerOn = FALSE;
                    if (gAppEsd.isAudioA2dpOn && FW_CmpBdAddr(&gAppEsd.audioA2dpBdAddr, pBdAddr))
                    {
					    APP_AvrcpDecideOperation(pBdAddr, AVC_OPERATION_FORCE_PLAY);
                    }
				}
			}
			else
			{
				APP_ClearAvrcpLinkInfo(pBdAddr);

				if(APP_IsActiveLink(pBdAddr) && ((AVRCP_CONNECT_CFM_T *)msg)->status == BT_STATUS_REJECT_BY_REMOTE)
				{
					PM_SetReconnectProfile(pBdAddr, PROFILE_AVRCP, 2300);// trigger again
				}
			}

			if(((AVRCP_CONNECT_CFM_T *)msg)->status != BT_STATUS_BUSY)
			{
				APP_ReConn_DeleteProfileMask(&((AVRCP_CONNECT_CFM_T *)msg)->bdAddr, PROFILE_AVRCP, ((AVRCP_CONNECT_CFM_T *)msg)->status);
			}
			break;
		case AVRCP_DISCONNECT_CFM:
			APP_ClearAvrcpLinkInfo(&((AVRCP_DISCONNECT_CFM_T *)msg)->bdAddr);
			break;

		case AVRCP_CONNECT_IND:
			AVRCP_ConnectResponse(&((AVRCP_CONNECT_IND_T *)msg)->bdAddr, TRUE, ((AVRCP_CONNECT_IND_T *)msg)->channelId, ((AVRCP_CONNECT_IND_T *)msg)->identifier);
			break;

		/*Command Indication*/
		case AVRCP_GET_CMD_UNITINFO_IND:
			AVRCP_UnitInfoResponse(&((AVRCP_GET_CMD_UNITINFO_IND_T *)msg)->bdAddr, ((AVRCP_GET_CMD_UNITINFO_IND_T *)msg)->transactionLabel, ((AVRCP_GET_CMD_UNITINFO_IND_T *)msg)->resultCode, ((AVRCP_GET_CMD_UNITINFO_IND_T *)msg)->subUnitID, ((AVRCP_GET_CMD_UNITINFO_IND_T *)msg)->subUnitType);
			break;

		case AVRCP_GET_CMD_SUBUNITINFO_IND:
			AVRCP_SubunitInfoResponse(&((AVRCP_GET_CMD_SUBUNITINFO_IND_T *)msg)->bdAddr, ((AVRCP_GET_CMD_SUBUNITINFO_IND_T *)msg)->transactionLabel, ((AVRCP_GET_CMD_SUBUNITINFO_IND_T *)msg)->resultCode, ((AVRCP_GET_CMD_SUBUNITINFO_IND_T *)msg)->subUnitID, ((AVRCP_GET_CMD_SUBUNITINFO_IND_T *)msg)->subUnitType);
			break;

		case AVRCP_GET_CMD_GETCAPS_IND:
			AVRCP_GetCapsResponse(&((AVRCP_GET_CMD_GETCAPS_IND_T *)msg)->bdAddr, ((AVRCP_GET_CMD_GETCAPS_IND_T *)msg)->transactionLabel, ((AVRCP_GET_CMD_GETCAPS_IND_T *)msg)->resultCode,
				((AVRCP_GET_CMD_GETCAPS_IND_T *)msg)->CompanyID, ((AVRCP_GET_CMD_GETCAPS_IND_T *)msg)->CapabilityId);
			break;

		case AVRCP_GET_CMD_GETELEMENTATTRIBUTES_IND:
			AVRCP_GetElementAttributesResponse(&((AVRCP_GET_CMD_GETELEMENTATTRIBUTES_IND_T *)msg)->bdAddr, ((AVRCP_GET_CMD_GETELEMENTATTRIBUTES_IND_T *)msg)->transactionLabel, ((AVRCP_GET_CMD_GETELEMENTATTRIBUTES_IND_T *)msg)->resultCode,
				((AVRCP_GET_CMD_GETELEMENTATTRIBUTES_IND_T *)msg)->CompanyID);
			break;

		case AVRCP_GET_CMD_REGISTERNOTIFICATION_IND:
			switch (((AVRCP_GET_CMD_REGISTERNOTIFICATION_IND_T *)msg)->eventID)
			{
				case AVRCP_EVENT_ID_VOLUME_CHANGED:
					AVRCP_SendRegisterNotificationVolInterim(&((AVRCP_GET_CMD_REGISTERNOTIFICATION_IND_T *)msg)->bdAddr, ((AVRCP_GET_CMD_REGISTERNOTIFICATION_IND_T *)msg)->transactionLabel, ((AVRCP_GET_CMD_REGISTERNOTIFICATION_IND_T *)msg)->resultCode,
						((AVRCP_GET_CMD_REGISTERNOTIFICATION_IND_T *)msg)->CompanyID, APP_VolManage_GetCurrentAbsoluteVolume(&((AVRCP_GET_CMD_REGISTERNOTIFICATION_IND_T *)msg)->bdAddr));
					break;

				default:
					AVRCP_VendorDependentRejectResponse(&((AVRCP_GET_CMD_REGISTERNOTIFICATION_IND_T *)msg)->bdAddr, ((AVRCP_GET_CMD_REGISTERNOTIFICATION_IND_T *)msg)->transactionLabel, ((AVRCP_GET_CMD_REGISTERNOTIFICATION_IND_T *)msg)->resultCode,
						((AVRCP_GET_CMD_REGISTERNOTIFICATION_IND_T *)msg)->CompanyID, ((AVRCP_GET_CMD_REGISTERNOTIFICATION_IND_T *)msg)->pduID);
					break;
			}
			break;

		case AVRCP_GET_CMD_SETABSOLUTEVOL_IND:
			app_AvrcpTgGetSetAbsVolIndHandler(&((AVRCP_GET_CMD_SETABSOLUTEVOL_IND_T *)msg)->bdAddr, ((AVRCP_GET_CMD_SETABSOLUTEVOL_IND_T *)msg)->absVol);
			AVRCP_SetAbsoluteVolumeResponse(&((AVRCP_GET_CMD_SETABSOLUTEVOL_IND_T *)msg)->bdAddr, ((AVRCP_GET_CMD_SETABSOLUTEVOL_IND_T *)msg)->transactionLabel, ((AVRCP_GET_CMD_SETABSOLUTEVOL_IND_T *)msg)->resultCode,
				((AVRCP_GET_CMD_SETABSOLUTEVOL_IND_T *)msg)->CompanyID, ((AVRCP_GET_CMD_SETABSOLUTEVOL_IND_T *)msg)->absVol);
			break;

		case AVRCP_GET_CMD_UNKNOWNVENDORDEPENDENT_IND:
			AVRCP_VendorDependentRejectResponse(&((AVRCP_GET_CMD_UNKNOWNVENDORDEPENDENT_IND_T *)msg)->bdAddr, ((AVRCP_GET_CMD_UNKNOWNVENDORDEPENDENT_IND_T *)msg)->transactionLabel, ((AVRCP_GET_CMD_UNKNOWNVENDORDEPENDENT_IND_T *)msg)->resultCode,
				((AVRCP_GET_CMD_UNKNOWNVENDORDEPENDENT_IND_T *)msg)->CompanyID, ((AVRCP_GET_CMD_UNKNOWNVENDORDEPENDENT_IND_T *)msg)->pduID);
			break;

		case AVRCP_GET_CMD_PASSTHROUGH_IND:
			app_AvrcpPassThroughIndHandler(msg);
			AVRCP_PassthroughResponse(&((AVRCP_GET_CMD_PASSTHROUGH_IND_T *)msg)->bdAddr, ((AVRCP_GET_CMD_PASSTHROUGH_IND_T *)msg)->transactionLabel, ((AVRCP_GET_CMD_PASSTHROUGH_IND_T *)msg)->resultCode,
				((AVRCP_GET_CMD_PASSTHROUGH_IND_T *)msg)->operandID, ((AVRCP_GET_CMD_PASSTHROUGH_IND_T *)msg)->buttonReleased, ((AVRCP_GET_CMD_PASSTHROUGH_IND_T *)msg)->operandLength);
			break;

		case AVRCP_GET_CMD_PASSTHROUGH_VENDORUNIQUE_IND:
			app_AvrcpPassThroughVendorUniqueIndHandler(msg);
			AVRCP_PassthroughResponse(&((AVRCP_GET_CMD_PASSTHROUGH_VENDORUNIQUE_IND_T *)msg)->bdAddr, ((AVRCP_GET_CMD_PASSTHROUGH_VENDORUNIQUE_IND_T *)msg)->transactionLabel, ((AVRCP_GET_CMD_PASSTHROUGH_VENDORUNIQUE_IND_T *)msg)->resultCode,
				((AVRCP_GET_CMD_PASSTHROUGH_VENDORUNIQUE_IND_T *)msg)->operandID, ((AVRCP_GET_CMD_PASSTHROUGH_VENDORUNIQUE_IND_T *)msg)->buttonReleased, ((AVRCP_GET_CMD_PASSTHROUGH_VENDORUNIQUE_IND_T *)msg)->operandLength);
			break;

			/*Response Information Indication*/
		case AVRCP_GET_APPLICATION_SETTING_IND:
			app_AvrcpAppSettingRspIndHandler(&((AVRCP_GET_APPLICATION_SETTING_IND_T *)msg)->bdAddr, ((AVRCP_GET_APPLICATION_SETTING_IND_T *)msg)->attributeId, ((AVRCP_GET_APPLICATION_SETTING_IND_T *)msg)->attributeValue);
			break;

		case AVRCP_GET_PLAY_STATUS_IND:
			app_AvrcpGetPlayStatusRspIndHandler(&((AVRCP_GET_PLAY_STATUS_IND_T *)msg)->bdAddr, ((AVRCP_GET_PLAY_STATUS_IND_T *)msg)->isSendPlayPause, ((AVRCP_GET_PLAY_STATUS_IND_T *)msg)->playStatus);
			break;

		case AVRCP_GET_REGISTER_NOTIFY_IND:
			app_AvrcpRegisterNotifyRspIndHandler((AVRCP_GET_REGISTER_NOTIFY_IND_T *)msg);
			break;

		case AVRCP_GET_PASS_THROUGH_IND:
			app_AvrcpPassThroughRspIndHandler(&((AVRCP_GET_PASS_THROUGH_IND_T *)msg)->bdAddr, ((AVRCP_GET_PASS_THROUGH_IND_T *)msg)->operationId);
			break;
        case AVRCP_GET_ELEMENT_ATTRIBUTE_IND:
            break;
        case AVRCP_GET_CABILITY_IND:
            break;
        case AVRCP_PHONE_SUPPORT_VOLUME_CHANGE_IND:
            break;
        default:
            break;
	}

	return 0;
}

/**************************************************************************************************
* Static (Timer Handler)
**************************************************************************************************/
static U32 app_AvrcpTimerHandler(Handler handler, U16 id, void *msg, U32 handler_id)
{
	APP_AVRCP_CTL_STRU * pLinkInfo;
	BD_ADDR_T * pBdAddr;

	#ifdef PROFILE_AMA_ENABLE
	APP_AMA_CTL_STRU *pAmaLinkInfo = APP_Ama_GetLinkInfo();
	#endif

	if(!handler_id)
		return 0;

	pLinkInfo = (APP_AVRCP_CTL_STRU *)handler_id;

	pBdAddr = APP_GetBdAddrByAvrcpLinkInfo(pLinkInfo);

	if(!pBdAddr)
		return 0;

	DBG_LOG_APP_AVRCP( "[APP_AVRCP] Time Out, id : %s, %d", 2, APP_AvrcpTimerID[id], id);

	switch (id)
	{
		case APP_AVRCP_TIMER_SEND_GET_PLAY_STATUS_TO_PAUSE_MUSIC:
		case APP_AVRCP_TIMER_MONITOR_PLAY_STATUS:
			APP_AvrcpDecideOperation(pBdAddr, AVC_OPERATION_GET_PLAY_STATUS);
			if(msg && ((APP_MSG_TYPE_STRU *)msg)->pMsgBody.avrcpTaskCmd.fcallback)
			{
				((APP_MSG_TYPE_STRU *)msg)->pMsgBody.avrcpTaskCmd.fcallback(pBdAddr, TRUE);
			}
			break;

		case APP_AVRCP_TIMER_SEND_PAUSE_MUSIC:
			if(msg && ((APP_MSG_TYPE_STRU *)msg)->pMsgBody.avrcpTaskCmd.fcallback)
			{
				((APP_MSG_TYPE_STRU *)msg)->pMsgBody.avrcpTaskCmd.fcallback(pBdAddr, FALSE);
			}
			break;

		case APP_AVRCP_TIMER_QUERY_CURRENT_PLAY_STATUS:
<<<<<<< HEAD
			#ifdef A2DP_Profile
=======
			#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
			APP_A2dp_PlayStatusTheSame(pBdAddr, APP_Avrcp_GetPlayStatus(pBdAddr));
			#endif
			break;

		case APP_AVRCP_TIMER_BACKUP_AND_PUSH_PLAY_STATUS:
			if(msg)
			{
				app_AvrcpGetRemotePlayStatus(pBdAddr, ((APP_MSG_TYPE_STRU *)msg)->pMsgBody.avrcpTaskCmd.playStatus);
			}
			break;

		case APP_AVRCP_TIMER_CHECK_STREAMING_STATE_LATER:
			if(msg && ((APP_MSG_TYPE_STRU *)msg)->pMsgBody.avrcpTaskCmd.playStatus == APP_Avrcp_GetPlayStatus(pBdAddr))
			{
				if (((APP_MSG_TYPE_STRU *)msg)->pMsgBody.avrcpTaskCmd.playStatus == AVRCP_PLAYBACK_STATUS_PLAYING && a2dp_is_open(pBdAddr) )
				{
					if (!APP_IsActiveAudioDevInAUX() && !APP_IsActiveAudioDevInCall()
						&& !(APP_A2dp_NVKeyIsFeatureOn(APP_A2DP_MEMPUT_WHEN_OTHER_IN_MUSIC_FEAT) && APP_IsActiveAudioDevInMusic()))
					{
<<<<<<< HEAD
						#ifdef CODEC_SWITCH_WORKAROUND_SONY_XPERIA
=======
						#ifdef AIR_CODEC_SWITCH_WORKAROUND_SONY_XPERIA_ENABLE
>>>>>>> db20e11 (second commit)
						if(!bt_a2dp_get_reconfigure_status(pBdAddr))
						#endif
						{
						bt_a2dp_start_request(pBdAddr);
					}
				}
			}
			}
			break;

<<<<<<< HEAD
	#ifdef CODEC_SWITCH_WORKAROUND_SONY_XPERIA
=======
	#ifdef AIR_CODEC_SWITCH_WORKAROUND_SONY_XPERIA_ENABLE
>>>>>>> db20e11 (second commit)
		case APP_AVRCP_TIMER_CHECK_STREAMING_WORK_AROUND1:
			if(bt_sdps_pnp_is_sony_xperia(pBdAddr))
			{
				DBG_LOG_APP_AVRCP( "[APP_AVRCP] [Codec Work-Around] CHECK_STREAMING_WORK_AROUND1", 0);
				
				APP_AvrcpDecideOperation(pBdAddr, AVC_OPERATION_FORCE_PAUSE);
				APP_Avrcp_SetTimer(pBdAddr, APP_AVRCP_TIMER_CHECK_SET_PLAY, (4*ONE_SEC), 0);
			}
			break;
		case APP_AVRCP_TIMER_CHECK_SET_PLAY:
			if(bt_sdps_pnp_is_sony_xperia(pBdAddr))
			{
				DBG_LOG_APP_AVRCP( "[APP_AVRCP] [Codec Work-Around] CHECK_SET_PLAY", 0);
				
				APP_AVRCP_KeyPlay(pBdAddr);
				bt_a2dp_set_reconfigure_status(pBdAddr, 0);
			}
			break;
		#endif

		#ifdef PROFILE_AMA_ENABLE
		case APP_AVRCP_TIMER_AMA_MEDIA_CONTROL_PLAY:
			if(pAmaLinkInfo)
			{
				DBG_LOG_APP_AVRCP( "[APP_AVRCP] Send AMA control play", 0);
				AMA_Target_IssueMediaControl(&pAmaLinkInfo->bdAddr, 
					AMA_MEDIA_CONTROL_PLAY);
			}
			break;
		#endif
	
	}
	UNUSED(handler);
	return 0;
}

BOOL APP_Avrcp_isDisablePlayStatusMonitor(BD_ADDR_T *pBdAddr)
{
	APP_AVRCP_CTL_STRU *pLinkInfo = APP_GetAvrcpLinkInfo(pBdAddr);

	if(pLinkInfo)
	{
		return (pLinkInfo->isDisablePlayStatusMonitor)?TRUE:FALSE;
	}
	return FALSE;
}

void APP_Avrcp_SetPlayStatus(BD_ADDR_T *pBdAddr, U8 status)
{
	APP_AVRCP_CTL_STRU *pLinkInfo = APP_GetAvrcpLinkInfo(pBdAddr);

	if(pLinkInfo)
	{
		pLinkInfo->playStatus = status;
	}
}

void APP_Avrcp_SendVolumeChangeNotification(BD_ADDR_T *pBdAddr, U8 soundLevel)
{
	AVRCP_VolumeChangedNotifyResponse(pBdAddr, APP_VolManage_GetAbsoluteVolumeBySoundLevel(soundLevel));
}

void APP_Avrcp_Init(void)
{
	APP_Avrcp_InitNvkeyAppAvrcpConfig();
}

