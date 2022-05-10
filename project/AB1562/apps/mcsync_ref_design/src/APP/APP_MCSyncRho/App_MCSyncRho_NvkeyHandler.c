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
#include "App_MCSyncRho_NvkeyHandler.h"
#include "App_RhoHandler.h"

/*************************************************************************************************
* Variable
**************************************************************************************************/
static APP_MCSYNC_RHO_CONFIG_STRU *gpAppMCSyncRhoConfig;

/*************************************************************************************************
* Public function
**************************************************************************************************/
U16 APP_MCSyncNvkey_GetRhoPercent(void)
{
	return gpAppMCSyncRhoConfig ? gpAppMCSyncRhoConfig->batCtl.rhoPercent : 200;
}

U16 APP_MCSyncNvkey_GetRhoGapPercent(void)
{
	return gpAppMCSyncRhoConfig ? gpAppMCSyncRhoConfig->batCtl.rhoGapPercent : 50;
}

S8 APP_MCSyncNvkey_GetRhoRssi(void)
{
	return gpAppMCSyncRhoConfig ? gpAppMCSyncRhoConfig->rssiCtl.rhoRssi : -85;
}

U8 APP_MCSyncNvkey_GetRhoGapRssi(void)
{
	return gpAppMCSyncRhoConfig ? gpAppMCSyncRhoConfig->rssiCtl.rhoGapRssi : 10;
}

BOOL APP_MCSyncNvkey_IsRhoFeatureOn(U16 feature)
{
	return (gpAppMCSyncRhoConfig && (gpAppMCSyncRhoConfig->triggerFeature & feature)) ? TRUE : FALSE;
}

U8 APP_MCSyncNvkey_GetRhoPriority(U8 rhoTrigger)
{
	return gpAppMCSyncRhoConfig ? gpAppMCSyncRhoConfig->priority[rhoTrigger] : 0;
}

U8 APP_MCSyncNvkey_GetRhoTimer(U8 timerType)
{
	switch(timerType)
	{
		case RHO_TIMER_ID_MONITOR_PARA:
			return gpAppMCSyncRhoConfig ? gpAppMCSyncRhoConfig->syncPartnerTimer : 15;
		default:
			break;
	}

	return 0;
}

void APP_MCSyncNvkey_RhoInit(void)
{
	if(NVKEY_GetPayloadLength(NVKEYID_APP_MCSYNC_RHO_CONFIG) == sizeof(APP_MCSYNC_RHO_CONFIG_STRU))
	{
		gpAppMCSyncRhoConfig = (APP_MCSYNC_RHO_CONFIG_STRU* )NVKEY_GetPayloadFlashAddress(NVKEYID_APP_MCSYNC_RHO_CONFIG);
	}

	if(!gpAppMCSyncRhoConfig)
	{
		DBG_LOG_APP_McsyncRho( "[APP_MCSync] nvkey init FAIL", 0);
	}
}

