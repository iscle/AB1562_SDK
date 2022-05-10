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


#ifndef _APP_KEY_MAP_GPIO_H_
#define _APP_KEY_MAP_GPIO_H_

/**************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
	TIMER_KEY_NONE, 					// 0
	TIMER_KEY_MULTI,
	TIMER_KEY_TAP,
	TIMER_KEY_DOUBLE,
	TIMER_KEY_TRIPPLE,
	TIMER_KEY_PRESS,					// 5
	TIMER_KEY_LONG,
	TIMER_KEY_LONGLONG,
	TIMER_KEY_EXTREMELY_LONG,
	TIMER_KEY_EXTREMELY_LONG_EXCEED,
	TIMER_KEY_VERYLONG, 				// 10
	TIMER_KEY_INTERNAL_TOGGLE_LONG,
	TIMER_KEY_INTERNAL_TOGGLE_NONE,
	TIMER_KEY_REPEAT,
};

enum
{
	EVENT_KEY_TAP,								// 0
	EVENT_KEY_PRESS,
	EVENT_KEY_PRESS_RELEASE,
	EVENT_KEY_LONGPRESS,
	EVENT_KEY_LONGPRESS_RELEASE,
	EVENT_KEY_DOUBLE, 							// 5
	EVENT_KEY_TRIPLE,
	EVENT_KEY_LONGLONGPRESS,
	EVENT_KEY_LONGLONGPRESS_RELEASE,
	EVENT_KEY_EXTREMELY_LONGPRESS,
	EVENT_KEY_EXTREMELY_LONGPRESS_RELEASE, 		// 10
	EVENT_KEY_REPEAT, 					 		// 11
	EVENT_KEY_DOWN, 						 	// 12
	EVENT_KEY_UP, 						 		// 13
    EVENT_KEY_ACTION_NUM,
	EVENT_KEY_BUTTON_SEQUENCE1_TIMEOUT,
};

enum
{
	KEY1,
	KEY2,
	KEY3,
	KEY4,
	KEY5,
	KEY6,
	KEY7,
	KEY8,
	KEY9,
	KEY10,
	TOTAL_KEY_NUM,
	KEYALL,
	KEYNONE,
	KEY_PWR = KEY1, // power key gpio 0
	KEY_COMB_34 = (0x80 >> KEY3) | (0x80 >> KEY4),					//0x30
	KEY_COMB_23 = (0x80 >> KEY2) | (0x80 >> KEY3),					//0x60
	KEY_COMB_13 = (0x80 >> KEY1) | (0x80 >> KEY3),					//0xA0
	KEY_COMB_24 = (0x80 >> KEY2) | (0x80 >> KEY4),					//0x50
	KEY_COMB_14 = (0x80 >> KEY1) | (0x80 >> KEY4),					//0x90
	KEY_COMB_12 = (0x80 >> KEY1) | (0x80 >> KEY2),					//0xC0
	KEY_COMB_123 = (0x80 >> KEY1) | (0x80 >> KEY2) | (0x80 >> KEY3),//0xE0
	KEY_COMB_124 = (0x80 >> KEY1) | (0x80 >> KEY2) | (0x80 >> KEY4),//0xD0
	KEY_COMB_134 = (0x80 >> KEY1) | (0x80 >> KEY3) | (0x80 >> KEY4),//0xB0
	KEY_COMB_234 = (0x80 >> KEY2) | (0x80 >> KEY3) | (0x80 >> KEY4),//0x70
	KEY_COMB_1234 = 0xF0,

    KEY_NOT_USED = 0xFF,
};


/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void key_map_gpio_init(Handler app);
BOOL key_map_gpio_key_pressed(void);
BOOL key_map_gpio_powerkey_pressed(void);
BOOL key_map_gpio_regenkey_switched(void);
BOOL key_map_gpio_switch_on_needed(void);
U32 key_map_gpio_get_keytime(U8 keyIndex, U8 timerType);
BOOL key_map_gpio_keycaptouch(U8 keyIndex);
void key_reset_toggle_timer(void);
#endif
