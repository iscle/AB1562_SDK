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
#ifndef __APP_CUSTOMER_H__
#define __APP_CUSTOMER_H__
#include "BtMCSync.h"

enum{
	CUSTOMER_EVENT_CAPTOUCH_EVENT_HANDLER,
};

enum{
	CUSTOMER_TIMER_KEY_DISCOVERABLE_MONITOR,
	CUSTOMER_TIMER_BATTERY_LEVEL_VP,
	CUSTOMER_TIMER_ENTER_DEEP_SLEEP,
	CUSTOMER_TIMER_KEY_EVENT_MANAGE,
	CUSTOMER_TIMER_RESUME_KEY_EVENT,
	CUSTOMER_TIMER_AUTO_ENTER_RTC,
	CUSTOMER_TIMER_CLEAN_LINKKEY_MONITOR,
	CUSTOMER_TIMER_CANCEL_CONNECTED_FILTER,
};

enum{
	Customer_KeyRelease,
	Customer_KeyPress,
};

void APP_Customer_SetActionPowerOff(BOOL isNeed);
U8 APP_Customer_GetActionPowerOff(void);
void APP_Customer_KeyActionPowerOff(void);
void APP_Customer_PowerOnLoaderHandler(void);
BOOL APP_Cutomer_isValidActionEnterDiscoverable(void);
U16 APP_Customer_ParseCustKeyEvent(void *ptr,U8 KeyAction);
void APP_Customer_SetSmartChargeStateWhenPowerOn(void);
void APP_Customer_BatteryLevelVP(void);
void APP_Customer_ClearPairListLedPattern(U16 fgEvent);
void APP_Customer_SendOtherKeyStatus(U8 keyIndex,U8 keyStatus);
void APP_Customer_SyncHandler(AWSMCE_SYNC_GENERAL_DATA_IND_T* ind);
void APP_Customer_McsynDisconnect(void);
void APP_Customer_CheckKeyStatusAfterRho(void);
void APP_Customer_CaptouchPressHandler(U8 keyIndex);
void APP_Customer_CaptouchReleaseHandler(U8 keyIndex);
void APP_Customer_Captouch_MCSYNCDisconnect(void);
void APP_Customer_GameModeToggle(U8 isGameMode);
void APP_Customer_SyncPartnerKeyEventHandler(U16 keyEventCode);
void APP_Customer_PEQVpToggle(U8 PeqIndex);
void APP_Customer_ResetPassThruPara(void);
BOOL APP_Customer_isNeedSaveANCPassThruState(BOOL isNeedSave);
void APP_Customer_AncPassThruVp(U8 type,U8 isOn);
void APP_Customer_setKeyResetToFactoryStatus(BOOL isReset);
void APP_Customer_AfterCleanLinkkeyAutoEnterDeepSlee(void);
void APP_Customer_MusicPlayingLed_Set(void);
void APP_Customer_MusicPlayingLed_Resume(void);
void APP_Customer_CancelConnectedFilter(void);
U16 APP_Customer_IsSpecialEventCode(U16 eventCode);
#endif
