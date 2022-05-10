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
#include "Power_NvkeyHandler.h"
#include "Power_Nvkey.h"

/**************************************************************************************************
* Variable
**************************************************************************************************/
static POWER_INFOR_STRU *pPowerInfo;

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static void app_PowerNvkey_ReadInfoConfig(void);

/**************************************************************************************************
* Static functions
**************************************************************************************************/
static void app_PowerNvkey_ReadInfoConfig(void)
{
	U8* pAddr;
	U8 dataSize;
		
	pAddr = (U8*)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_POWER_INFORMATION);
	dataSize = NVKEY_GetPayloadLength(NVKEYID_APP_POWER_INFORMATION);
	
	if(pAddr == NULL || dataSize <= 0)
	{
		FW_Assert(FALSE);
	}
	else
	{
		pPowerInfo = (POWER_INFOR_STRU *)pAddr;
	}

	return ;
}


/**************************************************************************************************
* Public functions
**************************************************************************************************/
void APP_PowerNvkey_Init(void)
{
	app_PowerNvkey_ReadInfoConfig();
}

U16 APP_PowerNvkey_GetTimer(U16 timerType)
{
	U16 timerValue;
	
	switch(timerType)
	{
		case TIMER_DISPOWEROFF_AFTER_POWERON_TIMEOUT:
			timerValue = pPowerInfo->timer.disPowerOffAfterPowerOn;
			break;
		case TIMER_AUTO_SWITCH_OFF_TIMEOUT:
			timerValue = pPowerInfo->timer.autoSwitchOff;
			break;
		case TIMER_SET_HW_POWER_OFF_TIMEOUT:
			timerValue = pPowerInfo->timer.setHWPowerOff;
			break;
		case TIMER_SET_HW_RESET_TIMEOUT:
			timerValue = pPowerInfo->timer.setHWReset;
			break;
		default:
			FW_Assert(FALSE);
			break;
	}
	
	return timerValue;
}
