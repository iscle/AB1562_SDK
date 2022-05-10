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
 
 
#ifndef	__APP_AUDIO_DRIVER_H__
#define	__APP_AUDIO_DRIVER_H__

#define	DRIVER_SUBSINK_Q_MAX		10
#define DBG_LOG_APP_AUDIO_DRIVER(_message, arg_cnt, ...)   	LOG_MSGID_I(APP_AUDIO_DRIVER, _message, arg_cnt, ##__VA_ARGS__)

enum
{
	DRIVER_SUBSINK_STATE_END,
	DRIVER_SUBSINK_STATE_START,
};

typedef struct
{
	U8 cmdType;
	U16 srcIndex;
	U16 id;
}APP_DRIVER_SUBSINK_Q_STRU;

typedef struct
{
	Handler							callBackHdl;
	U16								callBackMsgID;
	U8 								subSinkState;
	U8								qCmdNum;
	APP_DRIVER_SUBSINK_Q_STRU		qCmd[DRIVER_SUBSINK_Q_MAX];
}APP_DRIVER_SUBSINK_HDL_STRU;

BOOL APP_AudioDriver_SendSubSinkCmd(U8 cmdType, U16 srcIndex, U16 id);
void APP_AudioDriver_SubSinkClear(void);
U8 APP_AudioDriver_GetSubSinkQNum(void);
void APP_AudioDriver_Init(Handler msgHdl, U16 msgID);

void APP_AudioDriver_ClearSubSinkCmd(U16 id);
void APP_AudioDriver_SpecificEventVPStopCmd(U16 id); //MMI_DRV_SpecificEventVPStopCmd
void APP_AudioDriver_SpecificEventRTStopCmd(U16 id); //MMI_DRV_SpecificEventRingtoneStopCmd
void APP_AudioDriver_SendVoicepromptStopCmd(void);
void APP_AudioDriver_SendRingtoneStopCmd(void);

void APP_AudioDriver_SendRingtoneCmd(U16 id, U16 srcIndex, BOOL isPlayNow);
U16 APP_AudioDriver_GetPlayingID(void);

#endif

