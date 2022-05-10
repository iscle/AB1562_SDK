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
#include "App_VolumeProcess.h"
#include "App_ResourceManager.h"
#include "AudioDSP_Registration.h"
#include "App_A2dp_NVKeyHandler.h"
#include "App_Media.h"
#include "APP_AudioDriver.h"
#include "Volume_NvkeyHandler.h"

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
bool APP_VolProcess_IsSpeakerMuteByLink(BD_ADDR_T *pBdAddr)
{
	APP_INFO_STRU * pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	return APP_IsSetAppLink(pBdAddr) ? (pLinkInfo->linkPara.isSpkMuteForMicMute || pLinkInfo->linkPara.isSpkMuteForSCO) : FALSE;
}

bool APP_VolProcess_IsScoMicMute(BD_ADDR_T *pBdAddr)
{
	return APP_IsSetAppLink(pBdAddr) ? APP_GetAppLinkByBdAddr(pBdAddr)->linkPara.isMicMute : FALSE;
}

void APP_VolProcess_Beep(U8 soundLevel, U8 device, BOOL isPlayNow)
{
	//Modified by Eric Chou, 2010.7.23
	//If beepTone is 0xFF, do not play.
	U8 beepTone;

	if(device == AUDIO_DEVICE_MUSIC)
	{
		beepTone = APP_VolumeNvkey_GetBeepToneBySoundLevel(COMPONENT_A2DP, soundLevel);
	}
	#ifdef LINEIN_ENABLE
	else if(device == AUDIO_DEVICE_LINE_IN)
	{
		beepTone = APP_VolumeNvkey_GetBeepToneBySoundLevel(COMPONENT_LINEIN, soundLevel);
	}
	#endif
	else
	{
		beepTone = APP_VolumeNvkey_GetBeepToneBySoundLevel(COMPONENT_SCO_WB, soundLevel);
	}

	if (beepTone != INVALID_BEEP_PATTERN)
	{
	 	APP_AudioDriver_SendRingtoneCmd(MEDIA_EVT_PLAYING_BEEP_SYNC , beepTone, isPlayNow);
	}
}

