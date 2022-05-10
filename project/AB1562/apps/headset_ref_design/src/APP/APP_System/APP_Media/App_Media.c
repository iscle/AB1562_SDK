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
#include "App_Media.h"
#include "App_ResourceManager.h"
#include "APP_AudioDriver.h"
#include "AudioDSP_StreamManager.h"
#include "AudioDSP_Vp.h"
#include "App_LedControl.h"
#include "App_EventOpcode.h"
#include "App_State.h"
#include "App_ChargeBattery.h"
#include "FW_Api.h"
#include "App_Nvram_Type.h"
#include "App_System.h"

static U32 app_Media_MsgHandler(Handler handler, U16 id, void *payload, U32 id_ext);

/****************************************************************************************************
Variables
*****************************************************************************************************/
HandlerData gMediaHandler = { app_Media_MsgHandler };
MEDIA_EVT_CTL_STRU gMediaCtl;

/****************************************************************************************************
Define
*****************************************************************************************************/

#define	MEDIA_EVT_DATA_NUM		gMediaCtl.mediaEvtNum
#define MEDIA_EVT_LOCK_NUM		gMediaCtl.mediaEvtLockNum
#define MEDIA_EVT_FILTER_NUM	gMediaCtl.mediaEvtFilterNum

#define	MEDIA_EVT_DATA(X)		((MEDIA_EVT_DATA_STRU *)gMediaCtl.pMediaData + X)
#define	MEDIA_EVT_LOCK(X)		((MEDIA_EVT_LOCK_STRU *)gMediaCtl.pMediaLockTbl + X)
#define MEDIA_EVT_FILTER(X)		((MEDIA_EVT_FILTER_STRU *)gMediaCtl.pMediaFilter+ X)

/****************************************************************************************************
Static function
*****************************************************************************************************/
static void app_Media_ReadMediaDataNum(void)
{
	U16 number;

	gMediaCtl.mediaEvtNum = 0;
	if((number = NVKEY_ReadFullKey(NVKEYID_APP_MEDIA_EVENT_DATA, NULL, 0)) > 0)
	{
		if((number % sizeof(MEDIA_EVT_DATA_STRU)) == 0)
		{
			gMediaCtl.mediaEvtNum = (number / sizeof(MEDIA_EVT_DATA_STRU));
		}
	}
}

static void app_Media_ReadMediaLockNum(void)
{
	U16 number;

	gMediaCtl.mediaEvtLockNum = 0;
	if((number = NVKEY_ReadFullKey(NVKEYID_APP_MEDIA_EVENT_LOCK, NULL, 0)) > 0)
	{
		if((number % sizeof(MEDIA_EVT_LOCK_STRU)) == 0)
		{
			gMediaCtl.mediaEvtLockNum = (number / sizeof(MEDIA_EVT_LOCK_STRU));
		}
	}
}

static void app_Media_ReadMediaFilterNum(void)
{
	U16 number;

	gMediaCtl.mediaEvtFilterNum = 0;
	if((number = NVKEY_ReadFullKey(NVKEYID_APP_MEDIA_EVENT_FILTER, NULL, 0)) > 0)
	{
		if((number % sizeof(MEDIA_EVT_FILTER_STRU)) == 0)
		{
			gMediaCtl.mediaEvtFilterNum = (number / sizeof(MEDIA_EVT_FILTER_STRU));
		}
	}
}

static BOOL app_Meida_ReadMediaData(void)
{
	U8 *pNvkeyPtr;

	app_Media_ReadMediaDataNum();
	if(MEDIA_EVT_DATA_NUM == 0)
	{
		return FALSE;
	}

	if(NVKEY_ReadFullKey(NVKEYID_APP_MEDIA_EVENT_DATA, NULL, 0) != (sizeof(MEDIA_EVT_DATA_STRU) * MEDIA_EVT_DATA_NUM))
	{
		return FALSE;
	}

	pNvkeyPtr = (U8 *)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_MEDIA_EVENT_DATA);

	if(pNvkeyPtr == NULL)
	{
		return FALSE;
	}

	gMediaCtl.pMediaData = pNvkeyPtr;

	return TRUE;
}

static BOOL app_Meida_ReadMediaLockTbl(void)
{
	U8 *pNvkeyPtr;

	app_Media_ReadMediaLockNum();

	if(MEDIA_EVT_LOCK_NUM == 0)
	{
		return TRUE;
	}

	if(NVKEY_ReadFullKey(NVKEYID_APP_MEDIA_EVENT_LOCK, NULL, 0) != (sizeof(MEDIA_EVT_LOCK_STRU) * MEDIA_EVT_LOCK_NUM))
	{
		return FALSE;
	}
	pNvkeyPtr = (U8 *)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_MEDIA_EVENT_LOCK);

	if(pNvkeyPtr == NULL)
	{
		return FALSE;
	}

	gMediaCtl.pMediaLockTbl = pNvkeyPtr;

	return TRUE;
}

static BOOL app_Media_ReadMediaFilterData(void)
{
	U8 *pNvkeyPtr;

	app_Media_ReadMediaFilterNum();

	if(MEDIA_EVT_FILTER_NUM == 0)
	{
		return TRUE;
	}

	if(NVKEY_ReadFullKey(NVKEYID_APP_MEDIA_EVENT_FILTER, NULL, 0) != (sizeof(MEDIA_EVT_FILTER_STRU) * MEDIA_EVT_FILTER_NUM))
	{
		return FALSE;
	}
	pNvkeyPtr = (U8 *)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_MEDIA_EVENT_FILTER);

	if(pNvkeyPtr == NULL)
	{
		return FALSE;
	}

	gMediaCtl.pMediaFilter = pNvkeyPtr;
	return TRUE;
}

static void app_Media_RemoveQBuf(U16 msgOpCode)
{
	U8 evtCount;

	if(gMediaCtl.mediaQNum > 0 && gMediaCtl.mediaQ[0] == msgOpCode)
	{
		gMediaCtl.mediaQNum--;
		for(evtCount = 0 ; evtCount < gMediaCtl.mediaQNum ; evtCount++)
		{
			gMediaCtl.mediaQ[evtCount] = gMediaCtl.mediaQ[evtCount + 1];
		}
		gMediaCtl.mediaQ[evtCount] = 0;
	}
}

//#if !BRINGUP_1562
//#if !FPGA
static BOOL app_Media_CheckValidPowerOffMediaEvt(U16 evtOpCode)
{
	switch(evtOpCode)
	{
		case MEDIA_EVT_KEY_POWER_OFF:
		case MEDIA_EVT_POWER_OFF:
		case MEDIA_EVT_POWER_OFF_IN_SHUTDOWN_LOW_BAT:
		case MEDIA_EVT_OFF_STATE:
		case MEDIA_EVT_POWER_OFF_UNDER_CHECKPOINT1:
		case MEDIA_EVT_POWER_OFF_UNDER_CHECKPOINT2:
		case MEDIA_EVT_POWER_OFF_UNDER_CHECKPOINT3:
		case MEDIA_EVT_KEY_RESET_PAIRED_DEVICES:
		case MEDIA_EVT_KEY_RESET_TO_FACTORY:
			return TRUE;
			break;
		default:
			return FALSE;
			break;
	}
}
//#endif
//#endif

static U16 app_Media_FindMediaEvtCount(U16 evtOpCode)
{
	U16 evtCount;

	for(evtCount = 0 ; evtCount < MEDIA_EVT_DATA_NUM ; evtCount++)
	{
		if(MEDIA_EVT_DATA(evtCount)->evtOpCode != NO_MEDIA && evtOpCode == MEDIA_EVT_DATA(evtCount)->evtOpCode)
		{
			return evtCount;
		}
	}
	return MEDIA_EVT_DATA_NUM;
}

static void app_Media_MediaFilterEvtHandle(U16 evtOpCode)
{
	U16 evtCount;

	for(evtCount = 0 ; evtCount < MEDIA_EVT_FILTER_NUM ; evtCount++)
	{
		if(MEDIA_EVT_FILTER(evtCount)->evtOpCode != NO_MEDIA && evtOpCode == MEDIA_EVT_FILTER(evtCount)->evtOpCode)
		{
			if(MEDIA_EVT_FILTER(evtCount)->isCancleFilter)
			{
				APP_LED_CancelFilter(MEDIA_EVT_FILTER(evtCount)->filterIndex);
			}
			else
			{
				APP_LED_SetFilterPattern(MEDIA_EVT_FILTER(evtCount)->filterIndex);
			}
		}
	}
}

static void app_Media_SendMediaCmd(U16 evtCount)
{
//#if !BRINGUP_1562
//#if !FPGA
	U8 mmiState = APP_State_GetTopState(APP_GetServiceBdAddr());
//#endif
//#endif
	DBG_LOG_APP_SYSTEM( "[Media]: mediaQ0:%x, LED: %d, VP: %s, RT: %s", 4, gMediaCtl.mediaQ[0], (MEDIA_EVT_DATA(evtCount)->ledFGIndex & EVENT_LED_FG_TIMEOUT_MASK), APP_VPLogString[MEDIA_EVT_DATA(evtCount)->vpIndex], APP_RTLogString[MEDIA_EVT_DATA(evtCount)->rtIndex]);
//#if !BRINGUP_1562
//#if !FPGA
	if((mmiState != APP_OFF && mmiState != APP_DETACHING_LINK) || app_Media_CheckValidPowerOffMediaEvt(MEDIA_EVT_DATA(evtCount)->evtOpCode))
	{
		if(MEDIA_EVT_DATA(evtCount)->ledFGIndex != LED_INVALID)
			APP_LED_SetFgPattern((MEDIA_EVT_DATA(evtCount)->ledFGIndex & EVENT_LED_FG_TIMEOUT_MASK), MEDIA_EVT_DATA(evtCount)->overwriteTime & EVENT_LED_FG_TIMEOUT_MASK, (MEDIA_EVT_DATA(evtCount)->ledFGIndex & VOICE_PROMPT_BEFORE_RINGTONE_BIT) ? TRUE : FALSE);

		if((MEDIA_EVT_DATA(evtCount)->overwriteTime & VOICE_PROMPT_BEFORE_RINGTONE_BIT))
		{
			APP_AudioDriver_SendSubSinkCmd(AUDIO_VP, MEDIA_EVT_DATA(evtCount)->vpIndex, MEDIA_EVT_DATA(evtCount)->evtOpCode);
			APP_AudioDriver_SendSubSinkCmd(AUDIO_RT, MEDIA_EVT_DATA(evtCount)->rtIndex, MEDIA_EVT_DATA(evtCount)->evtOpCode);
		}
		else
		{
			APP_AudioDriver_SendSubSinkCmd(AUDIO_RT, MEDIA_EVT_DATA(evtCount)->rtIndex, MEDIA_EVT_DATA(evtCount)->evtOpCode);
			APP_AudioDriver_SendSubSinkCmd(AUDIO_VP, MEDIA_EVT_DATA(evtCount)->vpIndex, MEDIA_EVT_DATA(evtCount)->evtOpCode);
		}
	}
//#endif
//#endif
	APP_Media_SendFakeMediaCmd(MEDIA_EVT_DATA(evtCount)->evtOpCode);
}

static void app_Media_SendEvtToMsgHdl(U16 msgOpCode)
{
	APP_State_SendMsgToHandler(NULL, msgOpCode, APP_FAKE_MEDIA_EVT, TRUE);
}

static void app_Media_ClearMediaEvtQ(U16 EvtIndex)
{
	U8 qCount, tarCount;

	for(qCount = 1 ; qCount < gMediaCtl.mediaQNum ; qCount++)
	{
		if(gMediaCtl.mediaQ[qCount] == EvtIndex)
		{
			gMediaCtl.mediaQNum--;
			for(tarCount = qCount ; tarCount < gMediaCtl.mediaQNum - 1; tarCount++)
			{
				gMediaCtl.mediaQ[tarCount] = gMediaCtl.mediaQ[tarCount + 1];
			}
			gMediaCtl.mediaQ[tarCount] = 0;
		}
	}
}

<<<<<<< HEAD
=======
static void app_Media_ClearAllMediaEvtQ(void)
{
	U8 qCount, tarCount;

	for(qCount = 1 ; qCount < gMediaCtl.mediaQNum ; qCount++)
	{
		gMediaCtl.mediaQ[qCount] = 0;
	}

	gMediaCtl.mediaQNum = 0;
	gMediaCtl.mediaState = MEDIA_STATE_END;
}

>>>>>>> db20e11 (second commit)
static void app_Media_CantFindNvKeyLogPrint(U16 id)
{
	if(id <= MEDIA_EVT_KEY_INTERNAL_TOTAL_NO)//Media evtKey
	{
		DBG_LOG_APP_SYSTEM( "[Media] Cant Find NVKey:%s", 1, APP_MediaEvent1LogString[id]);
	}
	else if((MEDIA_EVT_BAT_LOW <= id) && (id <= MEDIA_EVT_CALLER_ID))
	{
		DBG_LOG_APP_SYSTEM( "[Media] Cant Find NVKey:%s", 1, APP_MediaEvent2LogString[id-MEDIA_EVT_BAT_LOW]);
	}
	else if((MEDIA_EVT_SLC_CONNECTED <= id) && (id < MEDIA_EVT_UART_CMD_RESERVE))
	{
		DBG_LOG_APP_SYSTEM( "[Media] Cant Find NVKey:%s", 1, APP_MediaEvent3LogString[id-MEDIA_EVT_SLC_CONNECTED]);
	}
	else
	{
		DBG_LOG_APP_SYSTEM( "[Media] Cant Find NVKey:%s", 1, id);
	}
}

static U32 app_Media_MsgHandler(Handler handler, U16 id, void *payload, U32 id_ext)
{
	U16 evtCount;
	UNUSED(handler);
	UNUSED(payload);
	UNUSED(id_ext);

	switch(id)
	{
		case MEDIA_STATE_END:
			app_Media_SendEvtToMsgHdl(id_ext);
			if(gMediaCtl.mediaQ[0] != id_ext)
				break;
			app_Media_RemoveQBuf(id_ext);
			gMediaCtl.mediaState = MEDIA_STATE_END;
		case MEDIA_STATE_START:
			if(gMediaCtl.mediaState == MEDIA_STATE_END && gMediaCtl.mediaQNum > 0)
			{
				app_Media_MediaFilterEvtHandle(gMediaCtl.mediaQ[0]);

				if(((evtCount = app_Media_FindMediaEvtCount(gMediaCtl.mediaQ[0])) >=  MEDIA_EVT_DATA_NUM)
					#ifdef LINEIN_ENABLE
					|| (APP_LINE_IN_CYCLIC_VOLUME_ENABLED_FEAT && APP_AudioDspIsStreaming(NULL,AUDIO_LINEIN))
					|| (APP_NO_CONNECTED_MEDIA_EVT_IN_LINE_IN_FEAT && (evtCount >= MEDIA_EVT_SLC_CONNECTED || evtCount <= MEDIA_EVT_4_SLC_CONNECTED))
					#endif
					)
				{
					app_Media_CantFindNvKeyLogPrint(gMediaCtl.mediaQ[0]);
					MSG_MessageSendEx(&gMediaHandler, MEDIA_STATE_END, NULL, gMediaCtl.mediaQ[0]);
					break;
				}

				if(APP_AudioDriver_GetSubSinkQNum() < (DRIVER_SUBSINK_Q_MAX ))
				{
					gMediaCtl.mediaState = MEDIA_STATE_START;
					app_Media_SendMediaCmd(evtCount);
				}
				else
				{
					FW_SetTimer(&gMediaHandler, MEDIA_STATE_START, NULL, 0, MEDIA_EVT_DELAY_SEND_MS);
				}
			}
			break;
	}

	return 0;
}


static void app_Media_PushMediaEventLogPrint(U16 id)
{
	if(id <= MEDIA_EVT_KEY_INTERNAL_TOTAL_NO)//Media evtKey
	{
		DBG_LOG_APP_SYSTEM( "[Media]: PushMediaEvent %s, Qnum: %d", 2, APP_MediaEvent1LogString[id], gMediaCtl.mediaQNum);
	}
	else if((MEDIA_EVT_BAT_LOW <= id) && (id <= MEDIA_EVT_CALLER_ID))
	{
		DBG_LOG_APP_SYSTEM( "[Media]: PushMediaEvent %s, Qnum: %d", 2, APP_MediaEvent2LogString[id-MEDIA_EVT_BAT_LOW], gMediaCtl.mediaQNum);
	}
	else if((MEDIA_EVT_SLC_CONNECTED <= id) && (id < MEDIA_EVT_UART_CMD_RESERVE))
	{
		DBG_LOG_APP_SYSTEM( "[Media]: PushMediaEvent %s, Qnum: %d", 2, APP_MediaEvent3LogString[id-MEDIA_EVT_SLC_CONNECTED], gMediaCtl.mediaQNum);
	}
	else
	{
		DBG_LOG_APP_SYSTEM( "[Media]: PushMediaEvent %s, Qnum: %d", 2, id, gMediaCtl.mediaQNum);
	}
}

<<<<<<< HEAD
=======
static BOOL app_Media_IsAllowPushMediaEvent(U16 mediaEventId)
{
	if(APP_State_IsAnyLinkInSpecifiedState(APP_HFP_INCOMMING))
	{
		switch(mediaEventId)
		{
			case MEDIA_EVT_CALLER_ID:
			case MEDIA_EVT_ENTER_INCOMING:
			case MEDIA_EVT_EXIT_INCOMING:
			case MEDIA_EVT_INCOMING_CALL_ENDED:
			case KEY_REJCALL:
			case KEY_ACPCALL:
			case KEY_POWER_OFF:
			case MEDIA_EVT_POWER_OFF:
			case MEDIA_EVT_POWER_OFF_UNDER_CHECKPOINT1:
			case MEDIA_EVT_POWER_OFF_IN_SHUTDOWN_LOW_BAT:
			case MEDIA_EVT_POWER_OFF_UNDER_CHECKPOINT2:
			case MEDIA_EVT_POWER_OFF_UNDER_CHECKPOINT3:
				return TRUE;
			
			default:
				return FALSE;
		}
	}

	return TRUE;
}

>>>>>>> db20e11 (second commit)
BOOL APP_Media_PushMediaEvent(U16 mediaEventId)
{
	if(!gMediaCtl.isDataOk)
	{
		return TRUE;
	}
<<<<<<< HEAD
=======

	if(!app_Media_IsAllowPushMediaEvent(mediaEventId))
	{
		DBG_LOG_APP_SYSTEM( "[Media] PushMediaEvent not allow evt:0x%x", 1, mediaEventId);
		return FALSE;
	}

>>>>>>> db20e11 (second commit)
	if(gMediaCtl.mediaQNum < MEDIA_EVT_Q_NUM)
	{
		app_Media_PushMediaEventLogPrint(mediaEventId);
		gMediaCtl.mediaQ[gMediaCtl.mediaQNum++] = mediaEventId;

		if(gMediaCtl.mediaQNum == 1)
		{
			FW_MessageSend(&gMediaHandler, MEDIA_STATE_START, NULL);
		}
		return TRUE;
	}
<<<<<<< HEAD
=======

>>>>>>> db20e11 (second commit)
	return FALSE;
}


BOOL APP_Media_SendFakeMediaCmd(U16 mediaEventId)
{
	APP_AudioDriver_SendSubSinkCmd(AUDIO_FAKE_EVT, 0, mediaEventId);
	return FALSE;
}

BOOL APP_Media_IsSubSinkQBufFull(void)
{
	if(APP_AudioDriver_GetSubSinkQNum() >= DRIVER_SUBSINK_Q_MAX || gMediaCtl.mediaQNum >= MEDIA_EVT_Q_NUM)
		return TRUE;

	return FALSE;
}

BOOL APP_Media_IsMediaEvtLock(U16 keyEvt)
{
	U16 evtCount;

	for(evtCount = 0 ; evtCount < MEDIA_EVT_LOCK_NUM ; evtCount++)
	{
		if(MEDIA_EVT_LOCK(evtCount)->evtOpcode == gMediaCtl.mediaQ[0] && (MEDIA_EVT_LOCK(evtCount)->lockKeyEvt == keyEvt))
			return TRUE;
	}

	return FALSE;
}

void APP_Media_StopMediaEvt(U16 EvtIndex)
{
	APP_AudioDriver_ClearSubSinkCmd(EvtIndex);
	app_Media_ClearMediaEvtQ(EvtIndex);
}

<<<<<<< HEAD
=======
void APP_Media_ClearAllMediaEvt(void)
{
	APP_AudioDriver_ClearAllSubSinkCmd();
	app_Media_ClearAllMediaEvtQ();
}

>>>>>>> db20e11 (second commit)
void APP_Media_Init(void)
{
	if(app_Meida_ReadMediaData() && app_Meida_ReadMediaLockTbl() && app_Media_ReadMediaFilterData())
	{
		DBG_LOG_APP_SYSTEM( "[Media] Media Init Sucess", 0);
		gMediaCtl.isDataOk = TRUE;
	}

	if(gMediaCtl.isDataOk)
	{
		APP_AudioDriver_Init(&gMediaHandler, MEDIA_STATE_END);

		gMediaCtl.mediaQNum = 0;
		gMediaCtl.mediaState = MEDIA_STATE_END;
		FW_Memset(&gMediaCtl.mediaQ, 0, sizeof(U16) * MEDIA_EVT_Q_NUM);
	}
}

void APP_Media_PowerOffHandle(BOOL isReset)
{
	APP_AudioDriver_SendVoicepromptStopCmd();
	APP_AudioDriver_SendRingtoneStopCmd();

	if(!isReset)
	{
		if(APP_Battery_IsBelowShutDownLevel())
		{
			APP_Media_PushMediaEvent(MEDIA_EVT_POWER_OFF_IN_SHUTDOWN_LOW_BAT);
		}
		else
		{
			switch(APP_ChgBat_GetLevelInQuarter())
			{
				case IND_BATTERY_LOW:
				case IND_BATTERY_INTERVAL_0:
					APP_Media_PushMediaEvent(MEDIA_EVT_POWER_OFF_UNDER_CHECKPOINT1);
					break;
				case IND_BATTERY_INTERVAL_1:
					APP_Media_PushMediaEvent(MEDIA_EVT_POWER_OFF_UNDER_CHECKPOINT2);
					break;
				case IND_BATTERY_INTERVAL_2:
					APP_Media_PushMediaEvent(MEDIA_EVT_POWER_OFF_UNDER_CHECKPOINT3);
					break;
				case IND_BATTERY_INTERVAL_3:
					APP_Media_PushMediaEvent(MEDIA_EVT_POWER_OFF);
					break;
				default:
					FW_Assert(FALSE);
					break;
			}
		}
	}
}
