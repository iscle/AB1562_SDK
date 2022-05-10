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


#ifndef _APP_POWER_OFF_H_
#define _APP_POWER_OFF_H_


/**************************************************************************************************
* Define
**************************************************************************************************/
#define AUTO_SWITCH_OFF_BIT_CONDISCABLE		(1 << 0)
#define AUTO_SWITCH_OFF_BIT_OFF				(1 << 1)
#define AUTO_SWITCH_OFF_BIT_TESTMODE		(1 << 2)
#define AUTO_SWITCH_OFF_BIT_FAKEOFF			(1 << 3)
#define AUTO_SWITCH_OFF_BIT_FM				(1 << 4)
#define AUTO_SWITCH_OFF_BIT_MP3				(1 << 5)
#define AUTO_SWITCH_OFF_BIT_LINEIN			(1 << 6)
#define AUTO_SWITCH_OFF_BIT_ANC				(1 << 7)
#define AUTO_SWITCH_OFF_BIT_CONNECTED		(1 << 8)
#define AUTO_SWITCH_OFF_BIT_ANCPASSTHRU		(1 << 9)

/**************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
	POWEROFF_REASON_KEY = 1,
	POWEROFF_REASON_SHUT_DOWN_LOW_BAT,
	POWEROFF_REASON_AUTO_SWITCH_OFF_TIMER,
	POWEROFF_REASON_PAIRING_MODE_TO,
	POWEROFF_REASON_TEMP_PROTECT,
	POWEROFF_REASON_AWS_IDLE,
	POWEROFF_REASON_AWS_RHO_FINISHED,
	POWEROFF_REASON_CHARGING_CASE,
};

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void APP_PowerOff_Init(void);
void APP_PowerOff_Loader(U8 reason);//MMI_PowerOffHandler
void APP_PowerOff_Proce(void);//MMI_PowerOffProce
void APP_PowerOff_SetTimer(U8 timerType);
void APP_PowerOff_CancelTimer(U8 timerType);
bool APP_PowerOff_IsTimerSet(U8 timerType);
void APP_SetPowerOnStatus(BOOL isOn); //APP_SET_POWER_ON_STATUS(A)
U8 APP_PowerOff_IsPowerOn(void);//MMI_IsPowerOn  // (MMI_KEY_STATE_POWER_OFF != MMI_GetKeyState())
void APP_PowerOff_SystemOff(void);//MMI_SystemOff
void APP_PowerOff_SetAutoSwitchOffBit(U16 bitMask, bool isBitAdded, BD_ADDR_T *pBdAddr);
void APP_Reset(void);//MMI_Reset
void APP_Reset_Loader(void);

#endif
