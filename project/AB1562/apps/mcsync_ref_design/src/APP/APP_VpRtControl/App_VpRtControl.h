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
 
 
#ifndef _APP_VPRT_CONTROL_H_
#define _APP_VPRT_CONTROL_H_

#include "message.h"
#include "BtMCSync.h"


/**************************************************************************************************
* Define
**************************************************************************************************/
#define APP_VP_RT_UNIFF_WAITING_TIME 1500

#define APP_VP_RT_DIRECTLY_PLAY_TIME	80

#if 0 /* Unused LOG_PRINT_156X */
#define DBG_LOG_APP_VpRtControl(_message, arg_cnt, ...)   printf(_message, ##__VA_ARGS__)
#else
#define DBG_LOG_APP_VpRtControl(_message, arg_cnt, ...)   LOG_MSGID_I(APP_VPRT_CTRL, _message, arg_cnt, ##__VA_ARGS__)
#endif

/**************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
	AWSMCE_VPRT_PLAYING_TIMER,
	AWSMCE_VPRT_BEEP_TIMER,
	AWSMCE_VPRT_STOP_TIMER,
	AWSMCE_VPRT_CHECK_SNIFF_STATE_TIMER,
	AWSMCE_VPRT_SET_SYNC_TIMER_MASTER,
	AWSMCE_VPRT_SET_SYNC_TIMER_SLAVE,
};

enum
{
	VPRT_PLAYING_SYNC,
	VPRT_STOP_SYNC,
	VPRT_BEEP_SYNC,
};

/**************************************************************************************************
* Structure 
**************************************************************************************************/

typedef struct
{
	U16 rtIndex;
	U16 vpIndex;
	U16 eventIndex;
	U8 overwriteTime;
	U32 currentPicoClock;
	U8 isVpRtSyncEvent:1;
	U8 isWaitUnsniff:1;
<<<<<<< HEAD
	U8 reserved:6;
=======
	U8 isVpRtSecondDevice:1;
	U8 reserved:5;
>>>>>>> db20e11 (second commit)
}APP_VPRT_INFO_STRU;

/**************************************************************************************************
* Public Functions 
**************************************************************************************************/
void APP_VpRt_SetSyncTimerMaster(U16 rtIndex, U16 vpIndex, U16 eventIndex, U8 overwriteTime);
void APP_VpRt_SetSyncTimerSlave(U16 rtIndex, U16 vpIndex, U16 eventIndex, U8 overwriteTime, U32 masterPicoClock);
void APP_VpRt_SyncTimerSlaveSetting(U16 rtIndex, U16 vpIndex, U16 eventIndex, U8 overwriteTime, U32 masterPicoClock);
void APP_VpRt_SetSyncStopTimerSlave(U32 currentPicoClock, U32 targetPicoClock, U16 eventIndex);
BOOL APP_VpRt_SyncStopHandler(U16 eventIndex);
void APP_VpRt_SyncPlayLaterHandler(U16 eventIndex, U32 clockToStart);
void APP_VpRt_SyncAgentParameter(U16 rtIndex, U16 vpIndex, U16 eventIndex, U32 currentPicoClock, 
										U8 overwriteTime);
void APP_VpRt_SetVpRtPlayInfo(U16 rtIndex, U16 vpIndex, U16 eventIndex, U8 overwriteTime);
void APP_VpRt_OSDPCTimer(U16 id, U32 delayMs);
void APP_VpRt_OSDPCStopTimer(U16 id, U32 delayMs, U16 eventIndex);
void APP_VpRt_SetMediaEventInfo(U16 eventIndex, U8 vpIndex, U8 rtIndex, U8 overwriteTime);
void APP_VpRt_UnSniffSendIFPacket(void);
void APP_VpRt_ModeChangeIndHandler(void);
void APP_VpRt_StopSystemVpUnderHFPState(U16 eventIndex);
void APP_VpRt_StopHFPVpUnderHFPState(U16 eventIndex);

void APP_VpRt_ReleaseTimer(void);
void APP_VpRt_Init(void);
<<<<<<< HEAD
=======
void APP_VpRt_T0Timer(U16 id, U32 clockToStart, U32 delayMs);
>>>>>>> db20e11 (second commit)

#endif
