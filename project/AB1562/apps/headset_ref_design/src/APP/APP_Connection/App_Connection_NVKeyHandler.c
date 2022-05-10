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


#define __APP_CONNECTION_NVKEY_HANDLER_C__
#include "bt.h"

#include "App_Connection_Nvkey.h"
#include "App_Connection_NVKeyHandler.h"
#include "App_ReConnection.h"

APP_RECONNECT_CONFIG_STRU CODE_PTR gpAppReconnectConfig;

void App_ConnNvkey_Init(void)
{
	gpAppReconnectConfig = (APP_RECONNECT_CONFIG_STRU CODE_PTR)NULL;
	if(NVKEY_GetPayloadLength(NVKEYID_APP_RECONNECT_CONFIG) == sizeof(APP_RECONNECT_CONFIG_STRU))
	{
		gpAppReconnectConfig =(APP_RECONNECT_CONFIG_STRU CODE_PTR)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_RECONNECT_CONFIG);
	}
}

BOOL App_ReConnNvkey_IsFeatureOn(U8 feature)
{
	if(gpAppReconnectConfig && (gpAppReconnectConfig->feature & feature))
		return TRUE;

	return FALSE;
}

U8 App_ReConnNvkey_GetReconnectLinkLossTimer(void)
{
	if(gpAppReconnectConfig)
		return gpAppReconnectConfig->reconnectdurationLinkLossTimer;

	return 0;
}

U8 App_ReConnNvkey_GetLinkLossReTryCnt(void)
{
	if(gpAppReconnectConfig)
		return gpAppReconnectConfig->linkLossRetryCnt;

	return 0x0A;
}

U8 App_ReConnNvkey_GetWatchDogReTryCnt(void)
{
	if(gpAppReconnectConfig)
		return gpAppReconnectConfig->watchDogRetryCnt;

	return 0x0A;
}

U8 App_ReConnNvkey_GetAutoReTryCnt(void)
{
	if(gpAppReconnectConfig)
		return gpAppReconnectConfig->autoRetryCnt;

	return 0x0A;
}

U8 App_ReConnNvkey_GetActionAfterPowerOn(void)
{
	if(gpAppReconnectConfig)
		return gpAppReconnectConfig->actionAfterPowerOn;

	return RECONNECT_AG_IND_0_1;
}

U8 App_ReConnNvkey_GetActionUserInitiated(void)
{
	if(gpAppReconnectConfig)
		return gpAppReconnectConfig->actionUserInitiated;

	return RECONNECT_AG_IND_0_1;
}

U8 App_ReConnNvkey_GetActionAfterLinkLoss(void)
{
	if(gpAppReconnectConfig)
		return gpAppReconnectConfig->actionAfterLinkLoss;

	return RECONNECT_LAST_DISC;
}

U8 App_ReConnNvKey_GetReconnectDelayTime(void)
{
	if(gpAppReconnectConfig)
		return gpAppReconnectConfig->reConnectDelayTimer;

	return 0x0A;
}

U8 App_ReConnNvKey_GetWatchDogReconnectTime(void)
{
	if(gpAppReconnectConfig)
		return gpAppReconnectConfig->watchDogReconnectTimer;

	return 0x0C;
}

