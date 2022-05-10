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
 
 
#ifndef _APP_LED_CONTROL_H_
#define _APP_LED_CONTROL_H_

#include "message.h"
#include "drv_led.h"
#include "Led_NVKey.h"

#define LED_INVALID			0xFF
#define DBG_LOG_APP_LED(_message, arg_cnt, ...)   	LOG_MSGID_I(APP_LED, _message, arg_cnt, ##__VA_ARGS__)

/*
LED's on/off scenario setting.
*/
enum
{
	LED_OFF,
	LED_ON,
	LED_ALT_FIRST,
	LED_ALT_SECOND,
	LED_ALT_THIRD,
	LED_ALT_FOURTH,
	LED_ALT_FIFTH,
	LED_ALT_SIXTH,
	LED_DONT_CARE,
};

enum
{
	LED_NUM_DATA,
	LED_BG_DATA,
	LED_FG_DATA,
	LED_FILTER_DATA,
	LED_TYPE_NUM,
};

enum
{
	LED_UNIT_TIME_1MS = 0x01,
	LED_UNIT_TIME_2MS = 0x02,
	LED_UNIT_TIME_4MS = 0x04,
	LED_UNIT_TIME_8MS = 0x08,
	LED_UNIT_TIME_16MS = 0x16,
	LED_UNIT_TIME_32MS = 0x32,
	LED_UNIT_TIME_64MS = 0x64,
};

enum
{
	LED_HDL_ID_NONE,
	LED_HDL_ID_REPEAT_TIMES,
	LED_HDL_ID_ALT_SCENARIO,
	LED_HDL_ID_FG_TIMER,
	LED_HDL_ID_ADD_SET_TIMER,
	LED_HDL_ID_ADD_RESUME_TIMER,
};

//LED_HDL_ID_REPEAT_TIMES
enum
{
	LED_HDL_IDEXT_REPEAT_LED0,
	LED_HDL_IDEXT_REPEAT_LED1,
	LED_HDL_IDEXT_REPEAT_LED2,
	LED_HDL_IDEXT_REPEAT_LED3,
	LED_HDL_IDEXT_REPEAT_LED4,
	LED_HDL_IDEXT_REPEAT_LED5,
};

//LED_HDL_ID_ALT_SCENARIO
enum
{
	LED_HDL_IDEXT_ALT_LED0,
	LED_HDL_IDEXT_ALT_LED1,
	LED_HDL_IDEXT_ALT_LED2,
	LED_HDL_IDEXT_ALT_LED3,
	LED_HDL_IDEXT_ALT_LED4,
	LED_HDL_IDEXT_ALT_LED5,
};

//LED_HDL_ID_FG_TIMER
enum
{
	LED_HDL_IDEXT_FG_TIMER_LED0,
	LED_HDL_IDEXT_FG_TIMER_LED1,
	LED_HDL_IDEXT_FG_TIMER_LED2,
	LED_HDL_IDEXT_FG_TIMER_LED3,
	LED_HDL_IDEXT_FG_TIMER_LED4,
	LED_HDL_IDEXT_FG_TIMER_LED5,
};

//LED_HDL_ID_ADD_TIMER
enum
{
	LED_HDL_IDEXT_ADD_SET_LED0,
	LED_HDL_IDEXT_ADD_SET_LED1,
	LED_HDL_IDEXT_ADD_SET_LED2,
	LED_HDL_IDEXT_ADD_SET_LED3,
	LED_HDL_IDEXT_ADD_SET_LED4,
	LED_HDL_IDEXT_ADD_SET_LED5,
};

//LED_HDL_ID_ADD_RESUME_TIMER
enum
{
	LED_HDL_IDEXT_ADD_RESUME_LED0,
	LED_HDL_IDEXT_ADD_RESUME_LED1,
	LED_HDL_IDEXT_ADD_RESUME_LED2,
	LED_HDL_IDEXT_ADD_RESUME_LED3,
	LED_HDL_IDEXT_ADD_RESUME_LED4,
	LED_HDL_IDEXT_ADD_RESUME_LED5,
};

enum
{
	FILTER_USER_DEF = 1,
	FILTER_MULTI_DIVIDE,
	FILTER_SWAP_CHANGE,
	FILTER_ADD,
};

typedef struct
{
	U8 *pattern;
	U8 ptnType;
	U8 loopTimes;
	U8 *resumeBGPtn;
}LED_MSG_CTL_STRU;
/*
The data structure passed to the function "LED_SetPatternToHW".
An array contains each LED's settings.
*/

typedef struct
{
	LED_NUM_STRU 		pLedNum;
	U8 *				pLedBGHeader;
	U8 *				pLedFGHeader;
	BOOL				isDataOk;
	LED_MSG_CTL_STRU	ledMsgCtl[LED_NUM];
	U8 *				pFilterHeader;
	U8 *				pFilterPtn;
    U8                  currentType;
}LED_CTRL_STRU;


void APP_LED_SetBgPattern(U8 ptnIndex);
void APP_LED_SetFgPattern(U8 ptnIndex, U8 overwriteTime, BOOL isOverWrite);
void APP_LED_SetFilterPattern(U8 ptnIndex);
BOOL APP_LED_IsFGLedOn(void);
void APP_LED_Init(void);
void App_LED_SetPatternToHW(U8 ledIndex ,LED_SETTING_STRU *pLedPtr);
void APP_LED_CancelFilter(U8 ptnIndex);

U8 APP_LED_GetDefaultIndex(U8 state);
U8 APP_LED_GetMaxLinkIndex(void);

#endif
