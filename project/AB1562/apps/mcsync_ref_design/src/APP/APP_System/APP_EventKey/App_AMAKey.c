/* Copyright Statement:
 *
 * (C) 2019  Airoha Technology Corp. All rights reserved.
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

#ifdef PROFILE_AMA_ENABLE
#include "App_EventOpcode.h"
#include "AudioDSP_StreamManager.h"
#include "App_AMAKey.h"
#include "BtAma.h"
#include "App_ResourceManager.h"
#include "App_Ama.h"
/**************************************************************************************************
* Functions prototype
**************************************************************************************************/
static BOOL app_AMA_SpeechStop(BD_ADDR_T *pBdAddr);
static BOOL app_AMA_SpeechStart(BD_ADDR_T *pBdAddr);


/**************************************************************************************************
* Variables
**************************************************************************************************/
static APP_KEY_CHECK_STRU const gAppAMAKeyCheckValidHandler[AMA_KEY_EVENT_NO] =
{
	{NULL},	//AMA_KEY_EVENT_KEY_SPEECH_TOGGLE
	{NULL}, //AMA_KEY_EVENT_KEY_SPEECH_START
	{NULL}, //AMA_KEY_EVENT_KEY_SPEECH_STOP
};


/**************************************************************************************************
* Static Functions
**************************************************************************************************/


static BOOL app_AMA_SpeechStart(BD_ADDR_T *pBdAddr)
{
	AMA_START_SPEECH_T startSpeechParam =
	{
	    .audioProfile = AMA_AUDIO_PROFILE_NEAR_FIELD,
	    .audioFormat = AMA_AUDIO_FORMAT_OPUS_16KHZ_32KBPS_CBR_0_20MS,
	    .audioSource = AMA_AUDIO_SOURCE_STREAM,
	    .type = AMA_SPEECH_INITIATOR_TYPE_TAP,
	    .suppressEarcon = 0,
	};

	if(AMA_STATUS_OK == AMA_Target_StartSpeech(pBdAddr, &startSpeechParam))
	{
		return TRUE;
	}

	return FALSE;
}

static BOOL app_AMA_SpeechStop(BD_ADDR_T *pBdAddr)
{
	AUDIO_DSP_STOP_PARA_STUR audioDspPara;
	FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_STOP_PARA_STUR));
	FW_Memcpy(&audioDspPara.bdAddr, pBdAddr, sizeof(BD_ADDR_T));
	audioDspPara.audioType = AUDIO_AMA;
	APP_AudioDeviceDeRegister(pBdAddr, AUDIO_DEVICE_AMA);
    APP_AudioDspStop(&audioDspPara);
	if(AMA_STATUS_OK == AMA_Target_StopSpeech(pBdAddr))
	{

		return TRUE;
	}

	return FALSE;
}


/**************************************************************************************************
* Public Functions
**************************************************************************************************/

U16 APP_AMA_CheckValidKey(U8 evtIndex, U16 keyCode)
{
	if(gAppAMAKeyCheckValidHandler[evtIndex].fKeyCheckValid)
	{
		keyCode = gAppAMAKeyCheckValidHandler[evtIndex].fKeyCheckValid(keyCode);
	}
	return keyCode;
}

void APP_AMA_ProcessKeyEvent(BD_ADDR_T *pBdAddr, U16 keyIndex)
{
	switch(keyIndex)
	{
		case AMA_KEY_EVENT_KEY_SPEECH_TOGGLE:
			APP_AMA_Key_SpeechToggle(pBdAddr);
			break;
		case AMA_KEY_EVENT_KEY_SPEECH_START:
			APP_AMA_Key_SpeechStart(pBdAddr);
			break;
		case AMA_KEY_EVENT_KEY_SPEECH_STOP:
			APP_AMA_Key_SpeechStop(pBdAddr);
			break;
		default:
			break;
	}
}

void APP_AMA_Key_SpeechToggle(BD_ADDR_T *pBdAddr)
{
    UNUSED(pBdAddr);
}

void APP_AMA_Key_SpeechStart(BD_ADDR_T *pBdAddr)
{
	APP_AMA_CTL_STRU *pAmaLinkInfo = APP_Ama_GetLinkInfo();
	UNUSED(pBdAddr);

	if(pAmaLinkInfo)
	{
		printf("[APP AMA] speech start");
		app_AMA_SpeechStart(&pAmaLinkInfo->bdAddr);
	}
	else
	{
		printf("[APP AMA] app ama key speech start fail");
	}
}

void APP_AMA_Key_SpeechStop(BD_ADDR_T *pBdAddr)
{
	APP_AMA_CTL_STRU *pAmaLinkInfo = APP_Ama_GetLinkInfo();
	UNUSED(pBdAddr);

	if(pAmaLinkInfo)
	{
		app_AMA_SpeechStop(&pAmaLinkInfo->bdAddr);
	}
}

#endif//PROFILE_AMA_ENABLE
