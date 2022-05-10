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
#include "Peq_NvkeyHandler.h"
#include "Peq_Nvkey.h"
#include "App_PeqProcess.h"
#include "App_PeqMount.h"
#include "App_PeqSync.h"
#include "App_A2dp_NVKeyHandler.h"
#include "BtMCSync.h"
#include "App_ResourceManager.h"
#include "App_System.h"
#include "AudioDSP_Registration.h"
#include "PM_Connection.h"
#include "App_MCSync_Message.h"
#include "AudioDSP_StreamManager.h"

/**************************************************************************************************
* Structure
**************************************************************************************************/

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static U32 app_Peq_TimerHandler(Handler handler, U16 id, void *msg, U32 handler_id);


/**************************************************************************************************
* Variables
**************************************************************************************************/
APP_PEQ_REALTIME_SYNC_STRU gAppPeqRealTimeSyncCtl;
HandlerData gAppPeqTimerHandle = {app_Peq_TimerHandler};


/**************************************************************************************************
* Static functions
**************************************************************************************************/
static U32 app_Peq_TimerHandler(Handler handler, U16 id, void *msg, U32 handler_id)
{
	BD_ADDR_T *pAgentBdAddr = BtMCSync_ReadAgentBdAddr();
	BD_ADDR_T *pCurrentAudiableBdAddr = APP_GetCurrentAudioBdAddr();
	UNUSED(handler);UNUSED(msg);UNUSED(handler_id);
	switch(id)
	{
		case AWSMCE_PEQ_SYNC_PLAYING_TIMER:
		DBG_LOG_APP_PEQ( "[APP_PEQ]PEQ Playing Timer", 0);

		if(BtAwsMce_IsDefaultRolePartner())
		{
			DBG_LOG_APP_PEQ( "[APP_PEQ]Partner End", 0);
			if(PM_IsProfileConnected(pAgentBdAddr, PROFILE_MCSYNC) && BtAwsMce_IsDefaultRolePartner()
				&& APP_AudioDspIsStreaming(pAgentBdAddr, AUDIO_A2DP))
			{
				APP_PeqProc_RealtimeUpdate(*gAppPeqRealTimeSyncCtl.pRealTime, (gAppPeqRealTimeSyncCtl.pRealTime + 1), gAppPeqRealTimeSyncCtl.length-1);
			}
		}
		else if(BtAwsMce_IsDefaultRoleAgent())//Agent
		{
			DBG_LOG_APP_PEQ( "[APP_PEQ]Agent End", 0);
			if(pCurrentAudiableBdAddr && BtAwsMce_IsDefaultRoleAgent()&& APP_AudioDspIsStreaming(pCurrentAudiableBdAddr, AUDIO_A2DP)) // agent role
			{
				APP_PeqProc_RealtimeUpdate(*gAppPeqRealTimeSyncCtl.pRealTime, (gAppPeqRealTimeSyncCtl.pRealTime + 1), gAppPeqRealTimeSyncCtl.length-1);
			}
		}
		APP_Peq_ClearSyncInfo();
		break;
		default:
			break;
	}
	return 0;
}


/**************************************************************************************************
* Public functions
**************************************************************************************************/

BOOL APP_Peq_SyncHandler(void)
{
	BD_ADDR_T *pServiceBdAddr = (BD_ADDR_T *)APP_GetServiceBdAddr();

	if(pServiceBdAddr)
	{
		//printf("==== PEQ pSevice bd addr: %x %x %x %x %x %x", pServiceBdAddr->addr[0], pServiceBdAddr->addr[1],
		//	pServiceBdAddr->addr[2], pServiceBdAddr->addr[3], pServiceBdAddr->addr[4], pServiceBdAddr->addr[5]);

		BtAwsMce_GetCurrentPiconetBtClock(pServiceBdAddr, PICO_EVENT_PEQ);
		return TRUE;
	}
	return FALSE;
}

void APP_Peq_SetSyncTimer(U32 currentPicoClock)
{
	U32 timeToWait;
	U32 clockToStart;

	if(BtAwsMce_IsDefaultRolePartner())
	{
		timeToWait =  ((gAppPeqRealTimeSyncCtl.clockToStart - currentPicoClock) * HALF_SLOT) / 1000; //ms

		DBG_LOG_APP_PEQ( "[APP_PEQ]PEQ current Pico:%d, get current Pico:%d", 2, gAppPeqRealTimeSyncCtl.clockToStart, currentPicoClock);
		DBG_LOG_APP_PEQ( "[APP_PEQ]partner sync value:%d", 1, timeToWait);

		if(timeToWait > PEQ_SYNC_TIME)
		{
			timeToWait = 0;
		}
	}
	else if(BtAwsMce_IsDefaultRoleAgent())//Agent
	{
		BD_ADDR_T *pBdAddr = APP_GetCurrentAudioBdAddr();

		if(!pBdAddr)
			return;

		timeToWait = PEQ_SYNC_TIME;

		clockToStart = currentPicoClock + (timeToWait * 1000) / HALF_SLOT;

		DBG_LOG_APP_PEQ( "[APP_PEQ]PEQ SYNC TIME: %d, clockToStart: %d", 2, timeToWait, clockToStart);

		APP_MCSYNC_SendPEQRealTimeInfo(gAppPeqRealTimeSyncCtl.pRealTime,
			gAppPeqRealTimeSyncCtl.length, clockToStart);

	}
    else
        return;

	FW_SetTimer(&gAppPeqTimerHandle, AWSMCE_PEQ_SYNC_PLAYING_TIMER, NULL, 0, timeToWait);
}

void APP_Peq_ClearSyncInfo(void)
{
	FW_FreeMemory(gAppPeqRealTimeSyncCtl.pRealTime);
	gAppPeqRealTimeSyncCtl.pRealTime = NULL;
	gAppPeqRealTimeSyncCtl.length = 0;
	gAppPeqRealTimeSyncCtl.clockToStart = 0;
}

void APP_Peq_SyncAgentParameter(AWSMCE_SYNC_PEQREALTIME_IND_T* ind)
{
	gAppPeqRealTimeSyncCtl.pRealTime = FW_GetMemory(ind->dataLength);
	FW_Memcpy(gAppPeqRealTimeSyncCtl.pRealTime, (U8*)&ind->data[0], ind->dataLength);
	gAppPeqRealTimeSyncCtl.length = ind->dataLength;
	gAppPeqRealTimeSyncCtl.clockToStart = ind->clockToStart;
}

void APP_Peq_SetAgentParameter(U8 *pPeqPara, U16 length)
{
	gAppPeqRealTimeSyncCtl.pRealTime = FW_GetMemory(length);
	FW_Memcpy(gAppPeqRealTimeSyncCtl.pRealTime, pPeqPara, length);
	gAppPeqRealTimeSyncCtl.length = length;
	gAppPeqRealTimeSyncCtl.clockToStart = 0;
}

