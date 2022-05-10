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


#ifndef _AUDIO_DSP_STREAM_MANAGER_H_
#define _AUDIO_DSP_STREAM_MANAGER_H_

#if 0 /* Unused LOG_PRINT_156X */
#define DBG_LOG_APP_DSP_CONTROL(_message, arg_cnt, ...) 
#else
#define DBG_LOG_APP_DSP_CONTROL(_message, arg_cnt, ...)      LOG_MSGID_I(APP_DSP_CONTROL, _message, arg_cnt, ##__VA_ARGS__)
#endif

/******************************************************************************
* Structure
*******************************************************************************/
typedef struct 
{
	U8 scms;
	U8 sampleRateIndex;
}AUDIO_A2DP_PARA_STRU;

typedef struct
{
	U8 mode;
}AUDIO_RECORD_PARA_STRU;

typedef struct
{
	BD_ADDR_T bdAddr;
	U8 audioType;
	U8 codec;
	union
	{
		AUDIO_A2DP_PARA_STRU a2dpPara;
		AUDIO_RECORD_PARA_STRU recordPara;
		U32 bitRate;
	};
}AUDIO_DSP_START_PARA_STUR;

typedef struct
{
	BD_ADDR_T bdAddr;
	U8 audioType;
}AUDIO_DSP_STOP_PARA_STUR;

/******************************************************************************
 * Enumeration
*******************************************************************************/
enum
{
	AUDIO_NONE,
	AUDIO_SCO,
	AUDIO_A2DP,
	AUDIO_ADDA, //Side Tone
	AUDIO_VP,
	AUDIO_RT,
	AUDIO_MP3,
	AUDIO_ANC,
	AUDIO_LINEIN,
	AUDIO_AMA,
    AUDIO_VAD,
    AUDIO_ENCODER,
	AUDIO_STREAM_RECORD,
	AUDIO_STREAM_NO,
	AUDIO_SUSPEND,
	AUDIO_CLOSE,
    AUDIO_FAKE_EVT,
    AUDIO_ANCPASSTHRU_SUSPEND,
    AUDIO_SCO_DISC,


	AUDIO_INVALID = 0xFF,
}; //Audio Type

/******************************************************************************
 * Public Functions
*******************************************************************************/
void APP_AudioDspInit();

/******************************************************************************
 * Public Functions (Stream)
*******************************************************************************/
BOOL APP_AudioDspPlay(AUDIO_DSP_START_PARA_STUR *pParameter);
BOOL APP_AudioDspStop(AUDIO_DSP_STOP_PARA_STUR *pParameter);
BOOL APP_AudioDspIsStreaming(BD_ADDR_T *pBdAddr, U8 audioType);
BOOL APP_AudioDspInMainPathStreaming(void);
U8 APP_AudioDspGetMainPath(void);
void APP_AudioDspClose(void);
void APP_AudioDSPRealtimeSuspend(void);
void APP_AudioDSPRealtimeResume(void);
BOOL APP_IsAbleToLoadVpRtGain(void);
void APP_ReleaseDspSuspend(void);
BOOL APP_AudioIsDspSuspend(void);
BOOL APP_AudioIsPermittedToPlay(U8 audio_type);

#endif //_AUDIO_DSP_STREAM_MANAGER_H_

