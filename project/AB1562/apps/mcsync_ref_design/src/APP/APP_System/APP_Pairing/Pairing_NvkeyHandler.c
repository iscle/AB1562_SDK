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
#include "Pairing_NvkeyHandler.h"
#include "Pairing_Nvkey.h"

/**************************************************************************************************
* Variable
**************************************************************************************************/
static PAIRING_INFOR_STRU *pPairingInfo;

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static void app_PairNvkey_ReadInfoConfig(void);

/**************************************************************************************************
* Static functions
**************************************************************************************************/
static void app_PairNvkey_ReadInfoConfig(void)
{
	U8* pAddr;
	U8 dataSize;
		
	pAddr = (U8*)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_PAIRING_INFORMATION);
	dataSize = NVKEY_GetPayloadLength(NVKEYID_APP_PAIRING_INFORMATION);
	
	if(pAddr == NULL || dataSize <= 0)
	{
		FW_Assert(FALSE);
	}
	else
	{
		pPairingInfo = (PAIRING_INFOR_STRU *)pAddr;
	}

	return ;
}


/**************************************************************************************************
* Public functions
**************************************************************************************************/
void APP_PairNvkey_Init(void)
{
	app_PairNvkey_ReadInfoConfig();
}

U16 APP_PairNvkey_GetTimer(U16 timerType)
{
	U16 timerValue = 0;
	
	switch(timerType)
	{
		case TIMER_PAIRING_MODE_TIMEOUT:
			timerValue = pPairingInfo->timer.PairModeTimeout;
			break;
		case TIMER_PAIRING_MODE_PDL_EMPTY_TIMEOUT:
			timerValue = pPairingInfo->timer.PairModeTimeoutIfPDLIsEmpty;
			break;
		case TIMER_PAIRING_CONNECTABLE_TIMEOUT:
			timerValue = pPairingInfo->timer.ConnectableTimeout;
			break;
		default:
			FW_Assert(FALSE);
			break;
	}
	
	return timerValue;
}

bool APP_PairNvkey_IsFeatureOn(U16 feature)
{
	#if FPGA
	if(feature == APP_DISCOVER_MODE_AFTER_RECOVERY_FEAT)
		return TRUE;
	#endif
	return (pPairingInfo->feature & feature);
}

U8 APP_PairNvkey_GetDiscoverableIfPdlLessThan(void)
{
	return pPairingInfo->discoverableIfPdlLessThan;
}

U8 APP_PairNvkey_GetActionAfterPairingModeTimeout(void)
{
	return pPairingInfo->actionAfterPairingModeTimeout;
}

U8 APP_PairNvkey_GetMaxPageDevice(void)
{
	return pPairingInfo->maxPageDevice;
}
