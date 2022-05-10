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
 
 
#ifndef _APP_CHARGE_BATTERY_H_
#define _APP_CHARGE_BATTERY_H_

#include "App_ResourceManager.h"
#include "app_chargerbattery_information_nvstruc.h"

/**************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
	APP_BAT_WELL,		//CHARGER_OUT
	APP_BAT_LOW,		//CHARGER_OUT
	APP_BAT_CHARGER_IN,
	APP_BAT_CHARGER_RECHARGE,
	APP_BAT_CHARGING_FULL,
};

enum
{
	IND_BATTERY_LOW  = 0x70,
	IND_BATTERY_INTERVAL_0,
	IND_BATTERY_INTERVAL_1,
	IND_BATTERY_INTERVAL_2,
	IND_BATTERY_INTERVAL_3,
};

/**************************************************************************************************
* Define
**************************************************************************************************/
#define APP_CHGBAT_SetKeyInvalidPwOnLowBat(a)		(gAppCtl.driverCtl.keyInvalidInPwrOnLowBat = (a))
//MMI_DRV_SetKeyInvalidPwOnLowBat
#define APP_CHGBAT_IsKeyInvalidPwOnLowBat()		(gAppCtl.driverCtl.keyInvalidInPwrOnLowBat)
//MMI_DRV_IsKeyInvalidPwOnLowBat

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
//#ifdef MMI_BATCHG_ENABLE
void APP_ChgBat_Init(void);//MMI_CHGBAT_Init
void APP_ChgBat_UpdateBatStatus(U8 batStatus);//MMI_UpdateBatStatus
void APP_ChgBat_CheckCurrentBatLevel(void);//MMI_CheckCurrentBatLevel
//#endif
U8 APP_ChgBat_GetLevelInQuarter(void);//MMI_BAT_GetLevelInQuater
U8 APP_ChgBat_IsChargerIn(void);//MMI_IsChargerIn
void APP_ChgBat_CheckBatStatus(void);//MMI_PollingBatStatus(void)
BOOL APP_ChgBat_IsBelowLowLevel(void);
U8 APP_ChgBat_GetBatStatus(void);
BOOL APP_Battery_IsBelowShutDownLevel(void);
BOOL APP_ChgBat_IsEventPowerOff(U16 opCode);
void APP_ChgBat_PowerOffHandler(void);
void APP_ChgBat_SetBatterMonitorTimer(void);
void APP_ChgBat_ResetTimeOutDectionTimer(void);
BOOL APP_ChgBat_IsTimerSet(U8 timerType);
U8 APP_ChgBat_GetPwrOnLowBatAction(void);
U16 APP_Battery_GetBatInThousandPercent(void);
void APP_ChgBat_ReportBatteryStatus(void);
BOOL APP_ChgBat_IsFeatureSupported(U16 feature);
BOOL APP_ChgBat_IsPowerOnNeeded(void);
void APP_ChgBat_ClearPowerOffState(void);
BOOL APP_Battery_IsBelowInitBat(void);
BOOL APP_ChgBat_IsChargerComplete(void);
void APP_ChgCase_SetChargerOffEn(void);
U8 APP_ChgCase_GetCaseType(void);
void APP_Battery_Init(void);

#endif
