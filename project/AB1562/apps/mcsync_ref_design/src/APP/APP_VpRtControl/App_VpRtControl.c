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
#include "types.h"
#include "stdio.h"
#include "message.h"
#include "drv_vp.h"
#include "drv_rt.h"
#include "App_ResourceManager.h"
#include "App_System.h"
#include "App_VpRtControl.h"
#include "App_MCSync_Message.h"
#include "AudioDSP_StreamManager.h"
#include "App_Media.h"
#include "APP_AudioDriver.h"
#include "App_Hfp.h"
#include "App_VolumeA2dpProcess.h"
#include "App_VolumeProcess.h"
#include "BtMCSync.h"
#include "App_SniffHandler.h"
#include "bt_gap.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "App_VolumeManage.h"
#include "DSP_Control.h"
#include "syslog.h"

log_create_module(APP_VPRT_CTRL, PRINT_LEVEL_INFO);


/**************************************************************************************************
* Prototype
**************************************************************************************************/
static void app_VpRt_PlayingVpRtDPCCallback(void *Para);
static void app_VpRt_PlayingBeepCallback(void *Para);
static void app_VpRt_SetSyncStopTimerMaster(U16 eventIndex, U32 currentPicoClock);
static U8 app_VpRt_checkVpSyncType(U16 eventCode);
static U32 app_VpRt_StopVpRtHandler(Handler handler, U16 id, void *msg, U32 handler_id);
static U32 app_VpRt_TimerHandler(Handler handler, U16 id, void *msg, U32 handler_id);
static void app_VpRt_PreparePlay(void);
static void app_VpRt_SyncHandler(void);
static U32 app_ClockToStart_Get(void);
static void app_ClockToStart_Set(U32 num);
static U32 app_CurrentBtClock_Get(void);

EXTERN BOOL Sink_SubAudio_Start (VOID);

/**************************************************************************************************
* Variables
**************************************************************************************************/
APP_VPRT_INFO_STRU gVpRtInfo;

static const HandlerData gAppStopVpHandle = {app_VpRt_StopVpRtHandler};
static const HandlerData gAppVpRtTimerHandle = {app_VpRt_TimerHandler};

extern MEDIA_EVT_CTL_STRU gMediaCtl;

static U32 vprt_semaphore;
void *pVPRT_semaphore = &vprt_semaphore;
extern unsigned int port_OsStart;

static FW_OS_DPC_BLOCK_STRU gAppVpRtPlayDpcBlock = {NULL, 0};
static FW_OS_DPC_BLOCK_STRU gAppVpRtStopDpcBlock = {NULL, 0};
static FW_OS_DPC_BLOCK_STRU gAppBeepPlayDpcBlock = {NULL, 0};

static U32 gClkToStart;

/**************************************************************************************************
* Static Functions
**************************************************************************************************/
static U8 app_VpRt_checkVpSyncType(U16 eventCode)
{
	U8 syncType = 0;

	switch(eventCode)
	{
		case MEDIA_EVT_KEY_REJCALL:
		case MEDIA_EVT_EXIT_INCOMING:
		case MEDIA_EVT_INCOMING_CALL_ENDED:
			syncType = VPRT_STOP_SYNC;
			break;

		case MEDIA_EVT_PLAYING_BEEP_SYNC:
			syncType = VPRT_BEEP_SYNC;
			break;

		default:
			syncType = VPRT_PLAYING_SYNC;
			break;
	}

	return syncType;
}

static void app_VpRt_PlayingVpRtDPCCallback(void *Para)
{
	UNUSED(Para);


	DBG_LOG_APP_VpRtControl( "[VPRT] app_VpRt_PlayingVpRtDPCCallback. CurrClk:%d, eventIndex:0x%x", 2, BtAwsMce_GetCurrentBtClock(), gVpRtInfo.eventIndex);

	if(port_OsStart)
	{
		xSemaphoreTakeRecursive(pVPRT_semaphore, portMAX_DELAY);
	}

    gAppVpRtPlayDpcBlock.dpcHandle = NULL;

	if(!gVpRtInfo.isVpRtSyncEvent)
	{
		if(port_OsStart)
		{
			xSemaphoreGiveRecursive(pVPRT_semaphore);
		}
		return;
	}

	if(port_OsStart)
	{
		xSemaphoreGiveRecursive(pVPRT_semaphore);
	}

	if(app_ClockToStart_Get() != 0 && 
		(app_ClockToStart_Get() - app_CurrentBtClock_Get() <= 10 ))
	{
		while(app_CurrentBtClock_Get() < app_ClockToStart_Get())
		{
		};

		 app_ClockToStart_Set(0);
	}

	Sink_SubAudio_Start();
	if((BtAwsMce_IsDefaultRoleAgent() || BtAwsMce_IsDefaultRolePartner()) &&
		app_VpRt_checkVpSyncType(gVpRtInfo.eventIndex) == VPRT_STOP_SYNC)
	{
		APP_HFP_StopIncomingVPRing((BD_ADDR_T*)NULL);
	}

	gVpRtInfo.isVpRtSyncEvent = FALSE;
    drv_vp_set_sync(FALSE);
    drv_rt_set_sync(FALSE);

	#if LAB_TUNNING_APP
	if(APP_AudioDspIsStreaming(NULL, AUDIO_STREAM_RECORD) && MEDIA_EVT_ANC_CAL_SZ == APP_AudioDriver_GetPlayingID())
	{	
		drv_recorder_resume_delay(150);
	}
	#endif
}

static U32 app_VpRt_StopVpRtHandler(Handler handler, U16 id, void *msg, U32 handler_id)
{
	U16 eventIndex = handler_id;

	UNUSED(handler);
	UNUSED(msg);

	switch(id)
	{
		case 0:
			APP_Media_StopMediaEvtInternal(eventIndex);
			break;
	}

	return 0;
}

static void app_VpRt_StopVpRtDPCCallback(void *Para)
{
	U16 eventIndex = *((U16 *)Para);
    gAppVpRtStopDpcBlock.dpcHandle = NULL;
	DBG_LOG_APP_VpRtControl( "[VPRT] app_VpRt_StopVpRtDPCCallback. CurrClk:%d, eventIndex:0x%x", 2, BtAwsMce_GetCurrentBtClock(), eventIndex);

	FW_MessageSendEx((Handler)&gAppStopVpHandle, 0, NULL, eventIndex); // exception happened if directly call APP_Media_StopMediaEvtInternal(eventIndex)
}

static void app_VpRt_PlayingBeepCallback(void *Para)
{
	UNUSED(Para);
	DBG_LOG_APP_VpRtControl( "[VPRT] app_VpRt_PlayingBeepCallback. CurrClk:%d, eventIndex:0x%x", 2, BtAwsMce_GetCurrentBtClock(), gVpRtInfo.eventIndex);

	if(port_OsStart)
	{
	    xSemaphoreTakeRecursive(pVPRT_semaphore, portMAX_DELAY);
	}

    gAppBeepPlayDpcBlock.dpcHandle = NULL;

	if(!gVpRtInfo.isVpRtSyncEvent)
	{
		if(port_OsStart)
		{
			xSemaphoreGiveRecursive(pVPRT_semaphore);
		}

		APP_Media_SendFakeMediaCmd(gVpRtInfo.eventIndex);
		return;
	}

	if(port_OsStart)
	{
		xSemaphoreGiveRecursive(pVPRT_semaphore);
	}

	DBG_LOG_APP_VpRtControl( "[VPRT] VpRt Beep Timer: IsAgent:%d, IsPartner:%d", 2, BtAwsMce_IsDefaultRoleAgent(), BtAwsMce_IsDefaultRolePartner());
	if(BtAwsMce_IsDefaultRoleAgent() || BtAwsMce_IsDefaultRolePartner())
	{
		if(app_VpRt_checkVpSyncType(gVpRtInfo.eventIndex) == VPRT_BEEP_SYNC)
		{
			Sink_SubAudio_Start();
		}
	}

	APP_Media_SendFakeMediaCmd(gVpRtInfo.eventIndex);
	gVpRtInfo.isVpRtSyncEvent = FALSE;
    drv_vp_set_sync(FALSE);
    drv_rt_set_sync(FALSE);
}

static void app_VpRt_SendSyncStopInfo(U16 eventIndex, U32 timeToWait, U32 currentPicoClock)
{
	U32 clockToStart;

	clockToStart = currentPicoClock + (timeToWait * 1000) / HALF_SLOT;

	APP_MCSYNC_SendSyncStopVpRtInfo(eventIndex, clockToStart);
}

static void app_VpRt_SetSyncStopTimerMaster(U16 eventIndex, U32 currentPicoClock)
{
	if(app_VpRt_checkVpSyncType(eventIndex) == VPRT_BEEP_SYNC)
	{
		DBG_LOG_APP_VpRtControl( "[VPRT] Master NOT Sync beep stop", 0);
	}
	else
	{
		APP_VpRt_OSDPCStopTimer(AWSMCE_VPRT_STOP_TIMER, VPRT_STOP_SYNC_TIME, eventIndex);
		app_VpRt_SendSyncStopInfo(eventIndex, VPRT_STOP_SYNC_TIME, currentPicoClock);
	}
}

static U32 app_VpRt_TimerHandler(Handler handler, U16 id, void *msg, U32 handler_id)
{
	UNUSED(handler);
	UNUSED(msg);
	UNUSED(handler_id);

	switch(id)
	{
		case AWSMCE_VPRT_CHECK_SNIFF_STATE_TIMER:
			app_VpRt_SyncHandler();
			App_SniffEnable(APP_GetServiceBdAddr(), APP_UNSNIFF_VPRT_SYNC);
			break;

		case AWSMCE_VPRT_SET_SYNC_TIMER_MASTER:
			APP_VpRt_SetSyncTimerMaster(gVpRtInfo.rtIndex, gVpRtInfo.vpIndex, gVpRtInfo.eventIndex, gVpRtInfo.overwriteTime);
			break;

		case AWSMCE_VPRT_SET_SYNC_TIMER_SLAVE:
			APP_VpRt_SetSyncTimerSlave(gVpRtInfo.rtIndex, gVpRtInfo.vpIndex, gVpRtInfo.eventIndex, gVpRtInfo.overwriteTime, handler_id);
			//"handler_id" is masterPicoClock.
			break;

		default:
			break;
	}

	return 0;
}

static void app_VpRt_PreparePlay(void)
{
	if(gVpRtInfo.eventIndex != MEDIA_EVT_PLAYING_BEEP_SYNC)
	{
		if((gVpRtInfo.overwriteTime & VOICE_PROMPT_BEFORE_RINGTONE_BIT))
		{
			APP_AudioDriver_SendSubSinkCmd(AUDIO_VP, gVpRtInfo.vpIndex, gVpRtInfo.eventIndex);
			APP_AudioDriver_SendSubSinkCmd(AUDIO_RT, gVpRtInfo.rtIndex, gVpRtInfo.eventIndex);
		}
		else
		{
			APP_AudioDriver_SendSubSinkCmd(AUDIO_RT, gVpRtInfo.rtIndex, gVpRtInfo.eventIndex);
			APP_AudioDriver_SendSubSinkCmd(AUDIO_VP, gVpRtInfo.vpIndex,gVpRtInfo.eventIndex);
		}

		APP_Media_SendFakeMediaCmd(gVpRtInfo.eventIndex);
	}
	else
	{
		APP_VolManage_SyncBeep();
<<<<<<< HEAD
=======
		APP_Media_SendFakeMediaCmd(gVpRtInfo.eventIndex);
>>>>>>> db20e11 (second commit)
	}
}

static void app_VpRt_SyncHandler(void)
{
	BD_ADDR_T *pBdServiceAddr = APP_GetServiceBdAddr();
<<<<<<< HEAD

	app_VpRt_PreparePlay();

=======
	app_VpRt_PreparePlay();

	DBG_LOG_APP_VpRtControl( "[VPRT] sync handler, media event:0x%x, service BDA:0x%x%x, link connected:%d ", 4,
		gVpRtInfo.eventIndex, FW_bdaddr_to_2U32(pBdServiceAddr, TRUE), FW_bdaddr_to_2U32(pBdServiceAddr, FALSE),
		PM_IsProfileConnected(pBdServiceAddr, PROFILE_MCSYNC));

>>>>>>> db20e11 (second commit)
	if(PM_IsProfileConnected(pBdServiceAddr, PROFILE_MCSYNC) && APP_Media_IsMediaEventAllowSync(gVpRtInfo.eventIndex))
	{
		FW_MessageSendEx((Handler)&gAppVpRtTimerHandle, AWSMCE_VPRT_SET_SYNC_TIMER_MASTER, NULL, 0);
	}
<<<<<<< HEAD
=======
	else if(BtMCSync_ReadAgentBdAddr() != NULL && APP_Media_CheckIsConnEvent(gVpRtInfo.eventIndex))
	{
		DBG_LOG_APP_VpRtControl( "[VPRT]sync handler, special connected:%d", 1, PM_IsProfileConnected(BtMCSync_ReadAgentBdAddr(), PROFILE_MCSYNC));
		if(PM_IsProfileConnected(BtMCSync_ReadAgentBdAddr(), PROFILE_MCSYNC))
		{
			FW_MessageSendEx((Handler)&gAppVpRtTimerHandle, AWSMCE_VPRT_SET_SYNC_TIMER_MASTER, NULL, 0);
		}
		else
		{
			APP_VpRt_OSDPCTimer((gVpRtInfo.eventIndex == MEDIA_EVT_PLAYING_BEEP_SYNC) ? AWSMCE_VPRT_BEEP_TIMER : AWSMCE_VPRT_PLAYING_TIMER, APP_VP_RT_DIRECTLY_PLAY_TIME);
		}
	}
>>>>>>> db20e11 (second commit)
	else
	{
		APP_VpRt_OSDPCTimer((gVpRtInfo.eventIndex == MEDIA_EVT_PLAYING_BEEP_SYNC) ? AWSMCE_VPRT_BEEP_TIMER : AWSMCE_VPRT_PLAYING_TIMER, APP_VP_RT_DIRECTLY_PLAY_TIME);
		//If delay 0 ms, the callback function will be processed before the following code.
		//Because the priority of DPC timer is higher than BT host.
	}

	gVpRtInfo.isWaitUnsniff = FALSE;
}

static U32 app_ClockToStart_Get(void)
{
	return gClkToStart;
}

static void app_ClockToStart_Set(U32 num)
{
	gClkToStart = num;
}

static U32 app_CurrentBtClock_Get(void)
{
	return BtAwsMce_GetCurrentBtClock();
}


/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void APP_VpRt_SetSyncTimerMaster(U16 rtIndex, U16 vpIndex, U16 eventIndex, U8 overwriteTime)
{
	U32 currentPicoClock = BtAwsMce_GetCurrentBtClock();
	U32 timeToWait = (app_VpRt_checkVpSyncType(eventIndex) == VPRT_BEEP_SYNC) ? BEEP_PLAY_SYNC_TIME : VPRT_PLAY_SYNC_TIME;
	U32 timeToOpenHw = DSP_CTRL_GetAfeDelayTimeMs(TRUE); //get the rest of time before HW opened
	U32 clockToStart;
<<<<<<< HEAD
=======
	BD_ADDR_T *pBdServiceAddr = APP_GetServiceBdAddr();
>>>>>>> db20e11 (second commit)

	timeToWait = (timeToWait < timeToOpenHw) ? timeToOpenHw : timeToWait;
	clockToStart = currentPicoClock + (timeToWait * 1000) / HALF_SLOT;

	DBG_LOG_APP_VpRtControl( "[VPRT] Set sync timer Master. CurrClk:%d, WaitTime:%d, StartClk:%d, EvtIdx:0x%x, VP:%s, RT:%s", 6,
		currentPicoClock, timeToWait, clockToStart, eventIndex, APP_VPLogString[vpIndex], APP_RTLogString[rtIndex]);

	if(rtIndex != 0xFF || vpIndex != 0xFF || app_VpRt_checkVpSyncType(eventIndex) == VPRT_BEEP_SYNC)
<<<<<<< HEAD
		APP_VpRt_OSDPCTimer((app_VpRt_checkVpSyncType(eventIndex) == VPRT_BEEP_SYNC) ? AWSMCE_VPRT_BEEP_TIMER : AWSMCE_VPRT_PLAYING_TIMER, timeToWait);

=======
	{
		if((pBdServiceAddr && PM_IsProfileConnected(pBdServiceAddr, PROFILE_MCSYNC)) ||
			(BtMCSync_ReadAgentBdAddr() && PM_IsProfileConnected(BtMCSync_ReadAgentBdAddr(), PROFILE_MCSYNC)))
		{
			APP_VpRt_T0Timer((app_VpRt_checkVpSyncType(eventIndex) == VPRT_BEEP_SYNC) ? AWSMCE_VPRT_BEEP_TIMER : AWSMCE_VPRT_PLAYING_TIMER, clockToStart, timeToWait);
		}
		else
		{
			APP_VpRt_OSDPCTimer((app_VpRt_checkVpSyncType(eventIndex) == VPRT_BEEP_SYNC) ? AWSMCE_VPRT_BEEP_TIMER : AWSMCE_VPRT_PLAYING_TIMER, timeToWait);
		}
	}
>>>>>>> db20e11 (second commit)
	APP_MCSYNC_SendSyncVpRtInfo(rtIndex, vpIndex, eventIndex, clockToStart, overwriteTime);

	app_ClockToStart_Set(clockToStart);
}

void APP_VpRt_SetSyncTimerSlave(U16 rtIndex, U16 vpIndex, U16 eventIndex, U8 overwriteTime, U32 masterPicoClock/*ie. Clock to start*/)
{
	U32 timeToWait, timeToOpenHw, clockToStart;
	U32 currentPicoClock = BtAwsMce_GetCurrentBtClock();
	U32 timeTH = (eventIndex == MEDIA_EVT_PLAYING_BEEP_SYNC) ? BEEP_PLAY_SYNC_TIME : VPRT_PLAY_SYNC_TIME;
<<<<<<< HEAD

=======
	BD_ADDR_T *pBdServiceAddr = APP_GetServiceBdAddr();
	
>>>>>>> db20e11 (second commit)
	UNUSED(overwriteTime);

	app_ClockToStart_Set(masterPicoClock);
	timeToWait = ((masterPicoClock - currentPicoClock) * HALF_SLOT) / 1000; //ms

	DBG_LOG_APP_VpRtControl( "[VPRT] Set sync timer Slave. EvtIdx:0x%x, VP:%s, RT:%s", 3, eventIndex, APP_VPLogString[vpIndex], APP_RTLogString[rtIndex]);
	DBG_LOG_APP_VpRtControl( "[VPRT] Set sync timer Slave. CurrClk:%d, WaitTime:%d, StartClk:%d, timeTH:%d", 4, currentPicoClock, timeToWait, masterPicoClock, timeTH);

	if(timeToWait > timeTH || (rtIndex == 0xFF && vpIndex == 0xFF && app_VpRt_checkVpSyncType(eventIndex) != VPRT_BEEP_SYNC)) //overtime, masterPicoClock(clock to start).
	{
<<<<<<< HEAD
=======
		if(APP_Media_CheckIsConnEvent(eventIndex))
		{
			Sink_SubAudio_Start();
		}

>>>>>>> db20e11 (second commit)
		APP_AudioDriver_ClearSubSinkCmd(eventIndex);
		APP_Media_SendFakeMediaCmd(eventIndex);
		return;
	}

	timeToOpenHw = DSP_CTRL_GetAfeDelayTimeMs(TRUE); //get the rest of time before HW opened
	if(timeToWait < timeToOpenHw)
	{
		timeToWait = (timeToOpenHw < VPRT_PLAY_MIN_DELAY_TIME) ? VPRT_PLAY_MIN_DELAY_TIME : timeToOpenHw;
		clockToStart = currentPicoClock + (timeToWait * 1000) / HALF_SLOT;
		DBG_LOG_APP_VpRtControl( "[APP_MCSync] : Sync VPRT Play Later. eventIndex(%d), waitTime(%d), clockToStart(%d)", 3, eventIndex, timeToWait, clockToStart);
		APP_MCSYNC_SendSyncVpRtPlayLater(eventIndex, clockToStart);
	}

<<<<<<< HEAD
	APP_VpRt_OSDPCTimer( (app_VpRt_checkVpSyncType(eventIndex) == VPRT_BEEP_SYNC) ? AWSMCE_VPRT_BEEP_TIMER : AWSMCE_VPRT_PLAYING_TIMER, timeToWait);
=======
	if((pBdServiceAddr && PM_IsProfileConnected(pBdServiceAddr, PROFILE_MCSYNC)) ||
		(BtMCSync_ReadAgentBdAddr() && PM_IsProfileConnected(BtMCSync_ReadAgentBdAddr(), PROFILE_MCSYNC)))
	{
		APP_VpRt_T0Timer( (app_VpRt_checkVpSyncType(eventIndex) == VPRT_BEEP_SYNC) ? AWSMCE_VPRT_BEEP_TIMER : AWSMCE_VPRT_PLAYING_TIMER, masterPicoClock, timeToWait);
	}
	else
	{
		APP_VpRt_OSDPCTimer((app_VpRt_checkVpSyncType(eventIndex) == VPRT_BEEP_SYNC) ? AWSMCE_VPRT_BEEP_TIMER : AWSMCE_VPRT_PLAYING_TIMER, timeToWait);
	}
>>>>>>> db20e11 (second commit)
}

void APP_VpRt_SyncTimerSlaveSetting(U16 rtIndex, U16 vpIndex, U16 eventIndex, U8 overwriteTime, U32 masterPicoClock)
{
	APP_VpRt_SetMediaEventInfo(eventIndex, vpIndex, rtIndex, overwriteTime);
	gMediaCtl.mediaState = MEDIA_STATE_SYNC_PLAY;
	app_VpRt_PreparePlay();
	FW_MessageSendEx((Handler)&gAppVpRtTimerHandle, AWSMCE_VPRT_SET_SYNC_TIMER_SLAVE, NULL, masterPicoClock);
}

void APP_VpRt_SetMediaEventInfo(U16 eventIndex, U8 vpIndex, U8 rtIndex, U8 overwriteTime)
{
	gVpRtInfo.eventIndex = eventIndex;
	gVpRtInfo.vpIndex = vpIndex;
	gVpRtInfo.rtIndex = rtIndex;
	gVpRtInfo.overwriteTime = overwriteTime;
	gVpRtInfo.isVpRtSyncEvent = TRUE;
    drv_vp_set_sync(TRUE);
    drv_rt_set_sync(TRUE);
}

void APP_VpRt_SetSyncStopTimerSlave(U32 currentPicoClock, U32 targetPicoClock, U16 eventIndex)
{
	U32 timeToWait;

	timeToWait = ((targetPicoClock - currentPicoClock) * HALF_SLOT) / 1000; //ms

	if(eventIndex == MEDIA_EVT_PLAYING_BEEP_SYNC)
	{
		return;
	}

	if(timeToWait > VPRT_STOP_SYNC_TIME)
	{
		timeToWait = 0;
	}

	APP_VpRt_OSDPCStopTimer(AWSMCE_VPRT_STOP_TIMER, timeToWait, eventIndex);
}

BOOL APP_VpRt_SyncStopHandler(U16 eventIndex)
{
	BD_ADDR_T *pServiceBdAddr = (BD_ADDR_T *)APP_GetServiceBdAddr();

	DBG_LOG_APP_VpRtControl( "[APP_MCSync] VPRT Stop handler, eventIndex:0x%x, 0x%x", gVpRtInfo.eventIndex, eventIndex); 

	if(pServiceBdAddr)
	{
		if(gVpRtInfo.eventIndex == eventIndex)
		{
			app_VpRt_SetSyncStopTimerMaster(eventIndex, BtAwsMce_GetCurrentBtClock());
			return TRUE;
		}
	}
	return FALSE;
}

void APP_VpRt_SyncPlayLaterHandler(U16 eventIndex, U32 clockToStart)
{
	U32 currentPicoClock = BtAwsMce_GetCurrentBtClock();
	U32 timeToWait;

	if(gVpRtInfo.isVpRtSyncEvent && gVpRtInfo.eventIndex == eventIndex
		&& currentPicoClock < clockToStart)
	{
		timeToWait = ((clockToStart - currentPicoClock) * HALF_SLOT) / 1000; //ms

		if(app_VpRt_checkVpSyncType(eventIndex) == VPRT_BEEP_SYNC)
		{
			FW_ReleaseDPCTimer(&gAppBeepPlayDpcBlock);
			APP_VpRt_OSDPCTimer(AWSMCE_VPRT_BEEP_TIMER, timeToWait);
		}
		else
		{
			FW_ReleaseDPCTimer(&gAppVpRtPlayDpcBlock);
			APP_VpRt_OSDPCTimer(AWSMCE_VPRT_PLAYING_TIMER, timeToWait);
		}
	}
}

<<<<<<< HEAD
void APP_VpRt_OSDPCTimer(U16 id, U32 delayMs)
{
	DBG_LOG_APP_VpRtControl( "[VPRT] Set OS DPC Timer, id:%d, CurrClk:%d, delay(ms):%d", 3, id, BtAwsMce_GetCurrentBtClock(), delayMs);
=======
void APP_VpRt_T0Timer(U16 id, U32 clockToStart, U32 delayMs)
{
	BD_ADDR_T *pBdAddr = NULL;
	BD_ADDR_T *pBdServiceAddr = APP_GetServiceBdAddr();
	BD_ADDR_T *pAgentAddr = BtMCSync_ReadAgentBdAddr();
	
	DBG_LOG_APP_VpRtControl( "[VPRT] Set LC t0 Timer, id:%d, CurrClk:%d", 2, id, BtAwsMce_GetCurrentBtClock());

	if(pBdServiceAddr && PM_IsProfileConnected(pBdServiceAddr, PROFILE_MCSYNC))
	{
		pBdAddr = pBdServiceAddr;
		DBG_LOG_APP_VpRtControl( "[VPRT] LC t0 timer, service link exist", 0);
	}

	if(pAgentAddr && PM_IsProfileConnected(pAgentAddr, PROFILE_MCSYNC))
	{
		pBdAddr = pAgentAddr;
		DBG_LOG_APP_VpRtControl( "[VPRT] LC t0 timer, agent link exist", 0);
	}
	
	if(pBdAddr)
	{
		DBG_LOG_APP_VpRtControl( "[VPRT] start LC t0 Timer, clock:%d, BDA:0x%x%x", 3,
			clockToStart, FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE));

		drv_vp_set_LC_SetVPSyncClk(clockToStart, pBdAddr);

		if((BtAwsMce_IsDefaultRoleAgent() || BtAwsMce_IsDefaultRolePartner()) &&
			app_VpRt_checkVpSyncType(gVpRtInfo.eventIndex) == VPRT_STOP_SYNC)
		{
			APP_HFP_StopIncomingVPRing((BD_ADDR_T*)NULL);
		}

		gVpRtInfo.isVpRtSyncEvent = FALSE;
    	drv_vp_set_sync(FALSE);
    	drv_rt_set_sync(FALSE);
	}
	else
	{
		if(id == AWSMCE_VPRT_PLAYING_TIMER)
			FW_SetDPCTimer(&gAppVpRtPlayDpcBlock, app_VpRt_PlayingVpRtDPCCallback, delayMs);
		else if(id == AWSMCE_VPRT_BEEP_TIMER)
			FW_SetDPCTimer(&gAppBeepPlayDpcBlock, app_VpRt_PlayingBeepCallback, delayMs);
	}

}


void APP_VpRt_OSDPCTimer(U16 id, U32 delayMs)
{
	DBG_LOG_APP_VpRtControl( "[VPRT] Set DPC Timer, id:%d, CurrClk:%d, delay(ms):%d", 3, id, BtAwsMce_GetCurrentBtClock(), delayMs);
>>>>>>> db20e11 (second commit)

	if(id == AWSMCE_VPRT_PLAYING_TIMER)
		FW_SetDPCTimer(&gAppVpRtPlayDpcBlock, app_VpRt_PlayingVpRtDPCCallback, delayMs);
	else if(id == AWSMCE_VPRT_BEEP_TIMER)
		FW_SetDPCTimer(&gAppBeepPlayDpcBlock, app_VpRt_PlayingBeepCallback, delayMs);
}

void APP_VpRt_OSDPCStopTimer(U16 id, U32 delayMs, U16 eventIndex)
{
	DBG_LOG_APP_VpRtControl( "[VPRT] Set OS DPC stop Timer, id:%d, CurrClk:%d, delay(ms):%d, eventIndex:%d", 4, id, BtAwsMce_GetCurrentBtClock(), delayMs, eventIndex);

	if(id == AWSMCE_VPRT_STOP_TIMER)
		FW_SetDPCTimerWithData(&gAppVpRtStopDpcBlock, app_VpRt_StopVpRtDPCCallback, delayMs, &eventIndex, sizeof(U16));
}

void APP_VpRt_UnSniffSendIFPacket(void)
{
	BD_ADDR_T *pBdAddr = APP_GetServiceBdAddr();

	App_SniffDisable(pBdAddr, APP_UNSNIFF_VPRT_SYNC);
<<<<<<< HEAD
=======

	DBG_LOG_APP_VpRtControl( "[VPRT] UnSniffSendIFPacket, BDA:0x%x%x, active link:%d", 3,
		FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE), PM_IsInActiveOrEmptyLink(pBdAddr));

>>>>>>> db20e11 (second commit)
	if(PM_IsInActiveOrEmptyLink(pBdAddr))
	{
		app_VpRt_SyncHandler();
		App_SniffEnable(pBdAddr, APP_UNSNIFF_VPRT_SYNC);
	}
	else
	{
		gVpRtInfo.isWaitUnsniff = TRUE;
		FW_SetTimer((Handler)&gAppVpRtTimerHandle, AWSMCE_VPRT_CHECK_SNIFF_STATE_TIMER, NULL, 0, APP_VP_RT_UNIFF_WAITING_TIME);
	}
}

void APP_VpRt_ModeChangeIndHandler(void)
{
	BD_ADDR_T *pBdAddr = APP_GetServiceBdAddr();

	if(gVpRtInfo.isWaitUnsniff && PM_IsInActiveOrEmptyLink(pBdAddr))
	{
		app_VpRt_SyncHandler();
		App_SniffEnable(pBdAddr, APP_UNSNIFF_VPRT_SYNC);
		FW_ReleaseTimer((Handler)&gAppVpRtTimerHandle, AWSMCE_VPRT_CHECK_SNIFF_STATE_TIMER, 0);
	}
}

void APP_VpRt_StopSystemVpUnderHFPState(U16 eventIndex)
{
	if(eventIndex == MEDIA_EVT_INCOMING_CALL_ENDED || eventIndex == MEDIA_EVT_ENTER_INCOMING ||
		eventIndex == MEDIA_EVT_EXIT_INCOMING || eventIndex == MEDIA_EVT_CALLER_ID)
	{
		APP_Media_StopMediaEvt(MEDIA_EVT_BAT_LOW);
		APP_Media_StopMediaEvt(MEDIA_EVT_BAT_FULL);
		APP_Media_StopMediaEvt(MEDIA_EVT_BAT_CHGCPL);
		APP_Media_StopMediaEvt(MEDIA_EVT_BAT_RECHG);
		APP_Media_StopMediaEvt(MEDIA_EVT_BAT_CHGROUT);
		APP_Media_StopMediaEvt(MEDIA_EVT_BAT_CHGRIN);
		APP_Media_StopMediaEvt(MEDIA_EVT_BAT_CHGTO);
		APP_Media_StopMediaEvt(MEDIA_EVT_BAT_HW_CHGTO);
		APP_Media_StopMediaEvt(MEDIA_EVT_BATTERY_INTERVAL_0);
		APP_Media_StopMediaEvt(MEDIA_EVT_BATTERY_INTERVAL_1);
		APP_Media_StopMediaEvt(MEDIA_EVT_BATTERY_INTERVAL_2);
		APP_Media_StopMediaEvt(MEDIA_EVT_BATTERY_INTERVAL_3);
		APP_Media_StopMediaEvt(MEDIA_EVT_BAT_LOW_LED);
		APP_Media_StopMediaEvt(MEDIA_EVT_BAT_LOW_RING);
		APP_Media_StopMediaEvt(MEDIA_EVT_BAT_SECURE_TEMP);
		APP_Media_StopMediaEvt(MEDIA_EVT_BAT_RISKY_TEMP);
		APP_Media_StopMediaEvt(MEDIA_EVT_BAT_OK);
		APP_Media_StopMediaEvt(MEDIA_EVT_BAT_DISCOUNT_TEMP);
	}
}

void APP_VpRt_StopHFPVpUnderHFPState(U16 eventIndex)
{
	if(eventIndex == MEDIA_EVT_EXIT_INCOMING)
	{
		APP_Media_StopMediaEvt(MEDIA_EVT_CALLER_ID);
	}
}

void APP_VpRt_Init(void)
{
	pVPRT_semaphore = xSemaphoreCreateRecursiveMutex();
}

void APP_VpRt_ReleaseTimer(void)
{
	FW_ReleaseDPCTimer(&gAppVpRtPlayDpcBlock);
	FW_ReleaseDPCTimer(&gAppBeepPlayDpcBlock);
	FW_ReleaseDPCTimer(&gAppVpRtStopDpcBlock);
	FW_ReleaseTimer((Handler)&gAppVpRtTimerHandle, AWSMCE_VPRT_BEEP_TIMER, 0);
	FW_ReleaseTimer((Handler)&gAppVpRtTimerHandle, AWSMCE_VPRT_PLAYING_TIMER, 0);
}

