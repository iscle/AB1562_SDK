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
 
 
#ifndef _MMI_AVRCP_H_
#define _MMI_AVRCP_H_

#include "bt.h"


/**************************************************************************************************
* Typedef
**************************************************************************************************/
#define AVRCP_PID PROFILE_UUID_AVRCP

#define AVRCP_ABSOLUTE_VOLUME_MAX				127
#define AVRCP_ABSOLUTE_VOLUME_FULL_RANGE		128

//isSendingGetPlayStatus
#define AVRCP_PLAY_STATUS_DONE FALSE
#define AVRCP_PLAY_STATUS_SENT TRUE

/**************************************************************************************************
* Enumeration
**************************************************************************************************/
enum
{
	APP_AVRCP_TIMER_SEND_GET_PLAY_STATUS_TO_PAUSE_MUSIC = 1,
	APP_AVRCP_TIMER_SEND_PAUSE_MUSIC,
	APP_AVRCP_TIMER_QUERY_CURRENT_PLAY_STATUS,
	APP_AVRCP_TIMER_MONITOR_PLAY_STATUS,
	APP_AVRCP_TIMER_BACKUP_AND_PUSH_PLAY_STATUS,
<<<<<<< HEAD
	APP_AVRCP_TIMER_CHECK_STREAMING_STATE_LATER,//CODEC_SWITCH_WORKAROUND_SONY_XPERIA
	APP_AVRCP_TIMER_CHECK_STREAMING_WORK_AROUND1,//CODEC_SWITCH_WORKAROUND_SONY_XPERIA
=======
	APP_AVRCP_TIMER_CHECK_STREAMING_STATE_LATER,//AIR_CODEC_SWITCH_WORKAROUND_SONY_XPERIA_ENABLE
	APP_AVRCP_TIMER_CHECK_STREAMING_WORK_AROUND1,//AIR_CODEC_SWITCH_WORKAROUND_SONY_XPERIA_ENABLE
>>>>>>> db20e11 (second commit)
	APP_AVRCP_TIMER_CHECK_SET_PLAY,
	APP_AVRCP_TIMER_AMA_MEDIA_CONTROL_PLAY,
	APP_AVRCP_TIMER_AMA_MEDIA_CONTROL_PAUSE,
}; //AVRCP Mission for Timer (handler_id)

/**************************************************************************************************
* Structure
**************************************************************************************************/


/**************************************************************************************************
* Internal Public Functions
**************************************************************************************************/
void APP_Avrcp_RegisterProfile();
void APP_AvrcpDecideOperation(BD_ADDR_T *pBdAddr, U8 opID);

/**************************************************************************************************
* Public Functions (Link)
**************************************************************************************************/
void APP_AvrcpSetMusicPlay(BD_ADDR_T *pBdAddr, BOOL isPlay);
BOOL APP_AvrcpIsMusicPlaying(BD_ADDR_T *pBdAddr);
void APP_AvrcpResetKeyPlayToggle(BD_ADDR_T *pBdAddr);
BOOL APP_AvrcpIsGetAppSettingsIdle(BD_ADDR_T *pBdAddr);
void APP_AvrcpSdkSetGetAppSettingsOwner(BD_ADDR_T *pBdAddr);
void APP_AvrcpCheckStreamingStateLater(BD_ADDR_T *pBdAddr, U8 playStatus, U8 isNeedDelay);
U16 	APP_Avrcp_ReleaseTimer(BD_ADDR_T *pBdAddr, U16 id);
U16 	APP_Avrcp_CheckTimer(BD_ADDR_T *pBdAddr, U16 id);
void 	APP_Avrcp_ReleaseAllTimer(BD_ADDR_T *pBdAddr);
void 	APP_Avrcp_SetTimer(BD_ADDR_T *pBdAddr, U16 id, U32 time, U32 para);
void 	APP_Avrcp_SetPlayStatus(BD_ADDR_T *pBdAddr, U8 status);
U8 		APP_Avrcp_GetPlayStatus(BD_ADDR_T *pBdAddr);
BOOL 	APP_Avrcp_isDisablePlayStatusMonitor(BD_ADDR_T *pBdAddr);
void 	APP_Avrcp_SendVolumeChangeNotification(BD_ADDR_T *pBdAddr, U8 soundLevel);
void 	App_Avrcp_KeyVolUpDownCmdHandler(BD_ADDR_T *pBdAddr, BOOL isVolUp);
void 	APP_Avrcp_Init(void);




#endif //_MMI_AVRCP_H_
