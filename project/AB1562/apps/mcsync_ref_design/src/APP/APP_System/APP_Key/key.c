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
#include "types.h"
#include "message.h"
#include "key_config_nvstruc.h"
#include "key.h"
#include "drv_keypad.h"
#include "hal_gpio_config.h"

/**************************************************************************************************
* Enum
**************************************************************************************************/
#define MESSAGE_KEY_MANAGE_TIMER_OFFSET			0x0001
#define MESSAGE_KEY_MANAGE_REPEAT_OFFSET		(MESSAGE_KEY_MANAGE_TIMER_OFFSET + TOTAL_KEY_NUM)
#define	MESSAGE_KEY_MANAGE_LINE_IN_OFFSET		(MESSAGE_KEY_MANAGE_REPEAT_OFFSET + TOTAL_KEY_NUM)


/**************************************************************************************************
* Define
**************************************************************************************************/
#define KEY1_PRESS 			0x80000000
#define KEY1_RELEASE 		0x00008000

log_create_module(MID_KEY, PRINT_LEVEL_INFO);

#define DBG_LOG_MID_KEY(_message, arg_cnt, ...)   LOG_MSGID_I(MID_KEY, _message, arg_cnt, ##__VA_ARGS__)
enum
{
	KEY_TYPE_NORMAL = 0x00,
	KEY_TYPE_SWITCH,
	KEY_TYPE_ROTARY,
	KEY_TYPE_TOGGLE,
	KEY_TYPE_CAPTOUCH,
};

#define KEY_GPIO_MASK		0x1F

#define GET_CURRENT_PRESSED_KEY_COUNT 	(gCurrentKeyStatus & 0x0000FFFF)
#define GET_CURRENT_PRESSED_KEY_MAP 	((gCurrentKeyStatus & 0xFFFF0000) >> 24)

#define Feature_MODE			0x00
#define NO_COMBO_KEY_MODE_FEAT 	Feature_MODE & 0x01

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static void key_cancel_timer(U8 keyIndex, U8 type);
static void key_set_timer(U8 keyIndex, U8 type, U8 timerType);
static U32 key_get_key_var_name(U8 keyIndex);
static U32 key_get_key_index(U8 keyVarName);
static bool key_is_regen_key(U8 keyIndex);
static U8 key_get_regen_key_index(void);
static U32 key_check_keypad_event(bool isRegenKey, U32 oldKeyIndexMap, U32 currentKeyIndexMap);
static BOOL key_is_key_pressed(U8 keyIndex);
static BOOL key_is_switch_key(U8 keyIndex);
static BOOL key_is_normal_key(U8 keyIndex);
static void key_press_handler(U8 keyIndex, U32 checkKeyEvent);
static void key_release_handler(U8 keyIndex);
static void key_normal_handler(bool isRegenKey, U32 keyIndexMap);
static void key_normal_timer_handler(U8 keyIndex);
static void key_repeat_timer_handler(U8 keyIndex);
static U32 key_timer_handler(Handler handler, U16 id, void *msg, U32 ext_id);
static U32 key_gpio_timer(Handler handler, U16 id, void *msg, U32 id_ext);
/*		Function Reserved*/
static bool key_is_timer_set(U8 keyIndex, U8 timerType);


/**************************************************************************************************
* Variable
**************************************************************************************************/
static U32 gPreviousKeyIndexMap;
static U8 gPreviousRegenIndexMap;
static U32 gDrvVarKeyMap = 0;
static U32 gDrvVarRegenMap = 0;
static U16 gKeyTimerType[16] = { TIMER_KEY_NONE };
static U32 gCurrentKeyStatus; 			// MSB 16bits for recording pressed key index (Bit31 for Key1, Bit32 for Key2...), and LSB 16bits for number of key pressed.
static U8  gCurrentPressedKeyIndex; 	// Record the 1st pressed key of combine keys.
static Handler gKey = 0;
static KEY_CONFIG_STRU gKeyConfig[TOTAL_KEY_NUM];

static const HandlerData gKeyMapGpioTimerHandle = { key_timer_handler };
static const HandlerData gKeyMapGpioHandle = { key_gpio_timer };
static const U32 gKeyNvkeyID[TOTAL_KEY_NUM] =
{
    NVKEYID_APP_KEY1_SETTING,
    NVKEYID_APP_KEY2_SETTING,
    NVKEYID_APP_KEY3_SETTING,
    NVKEYID_APP_KEY4_SETTING,
    NVKEYID_APP_KEY5_SETTING,
    NVKEYID_APP_KEY6_SETTING,
    NVKEYID_APP_KEY7_SETTING,
    NVKEYID_APP_KEY8_SETTING,
    NVKEYID_APP_KEY9_SETTING,
    NVKEYID_APP_KEY10_SETTING
};

/**************************************************************************************************
* Static Function
**************************************************************************************************/
static void key_init_nvkey(void)
{
	U8* pAddr;
    U8 keyIndex;

    for(keyIndex = KEY1 ; keyIndex < TOTAL_KEY_NUM ; keyIndex++)
    {
        if((pAddr = (U8*)NVKEY_GetPayloadFlashAddress(gKeyNvkeyID[keyIndex])) != NULL)
        {
            FW_Memcpy((U8*)&gKeyConfig[keyIndex], pAddr, sizeof(KEY_CONFIG_STRU));
        }
        else
        {
            FW_Assert(FALSE);
        }
    }
}

static KEY_CONFIG_STRU * key_get_key_config(U8 keyIndex)
{
    KEY_CONFIG_STRU *pKeyConfig = (KEY_CONFIG_STRU *)NULL;

    if(keyIndex < TOTAL_KEY_NUM)
    {
        pKeyConfig = &gKeyConfig[keyIndex];
    }
    else
    {
        FW_Assert(FALSE);
    }
    return pKeyConfig;
}

static U8 key_get_key_time(U8 keyIndex, U8 timerType)
{
    KEY_TIME_STRU *pKeyTimeConfig;
    KEY_CONFIG_STRU *pKeyConfig = key_get_key_config(keyIndex);

    if(!pKeyConfig)
    {
        return 0;
    }

    pKeyTimeConfig = &pKeyConfig->keyTime;

    switch(timerType)
    {
        case TIMER_KEY_TAP:
        case TIMER_KEY_DOUBLE:
        case TIMER_KEY_TRIPPLE:
        case TIMER_KEY_MULTI:
            return pKeyTimeConfig->keyMultiTime;
        case TIMER_KEY_PRESS:
            return pKeyTimeConfig->keyPressTime;
        case TIMER_KEY_LONG:
            return pKeyTimeConfig->keyLongPressTime;
        case TIMER_KEY_LONGLONG:
            return pKeyTimeConfig->keyLongLongPressTime;
        case TIMER_KEY_EXTREMELY_LONG:
            return pKeyTimeConfig->keyExtremelyLongPressTime;
        case TIMER_KEY_REPEAT:
            return pKeyTimeConfig->keyContPressTime;
        case TIMER_KEY_INTERNAL_TOGGLE_LONG:
            return pKeyTimeConfig->keyLongPressTime;
        case TIMER_KEY_INTERNAL_TOGGLE_NONE:
            return pKeyTimeConfig->keyLongPressTime;
        case TIMER_KEY_EXTREMELY_LONG_EXCEED:
            return 2*pKeyTimeConfig->keyExtremelyLongPressTime;
        default:
            FW_Assert(FALSE);
            break;
    }
    return 0;
}

static void key_set_timer(U8 keyIndex, U8 type, U8 timerType)
{
    if(keyIndex >= TOTAL_KEY_NUM)
    {
        FW_Assert(FALSE);
        return;
    }

    if(type == MESSAGE_KEY_MANAGE_TIMER_OFFSET)
    {
        gKeyTimerType[keyIndex] = timerType;
    }

    FW_SetTimer((Handler)&gKeyMapGpioTimerHandle, keyIndex, NULL, type, key_get_key_time(keyIndex, timerType)*MILLI_SEC_100);
}

static U32 key_get_key_var_name(U8 keyIndex)
{
    KEY_CONFIG_STRU *pKeyConfig = key_get_key_config(keyIndex);

    if(!pKeyConfig)
    {
        return 0;
    }

    return pKeyConfig->keyVarName;
}

static U32 key_get_key_index(U8 keyVarName)
{
	U8 i;

	for(i = 0; i < TOTAL_KEY_NUM; i++)
	{
		if(keyVarName == key_get_key_var_name(i))
			return i;
	}

	DBG_LOG_MID_KEY("[Key]GetKeyIndex Error. keyVarName:0x%x", 1, keyVarName);
	return KEY_NOT_USED;
}

static U32 key_get_key_type(U8 keyIndex)
{
    KEY_CONFIG_STRU *pKeyConfig = key_get_key_config(keyIndex);

    if(!pKeyConfig)
    {
        return 0;
    }

    return pKeyConfig->keyType;
}


static bool key_is_regen_key(U8 keyIndex)
{
    if(keyIndex < TOTAL_KEY_NUM)
        return (HAL_POWER_KEY_PIN == key_get_key_var_name(keyIndex));
    else
        return FALSE;
}

static U8 key_get_regen_key_index(void)
{
	U8 i;

	for(i = 0; i < TOTAL_KEY_NUM; i++)
	{
		if(key_is_regen_key(i))
			return i;
	}

	DBG_LOG_MID_KEY("[Key]GetRegenKeyIndex No regen key", 0);
	return KEY_NOT_USED;
}

static U32 key_check_keypad_event(bool isRegenKey, U32 oldKeyIndexMap, U32 currentKeyIndexMap)
{
	U8 keyIndex;
	U32 pressReleaseKeyIndexMap = 0;
	U32 keyIndexMap;

	UNUSED(isRegenKey);

	for(keyIndex = 0; keyIndex < TOTAL_KEY_NUM; keyIndex++)
	{
		keyIndexMap = 1L << keyIndex;

		if((oldKeyIndexMap & keyIndexMap) && !(currentKeyIndexMap & keyIndexMap))
		{ // pressed to released
			pressReleaseKeyIndexMap = pressReleaseKeyIndexMap | (KEY1_RELEASE >> keyIndex);
		}
		else if(!(oldKeyIndexMap & keyIndexMap) && (currentKeyIndexMap & keyIndexMap))
		{ // released to pressed
			pressReleaseKeyIndexMap = pressReleaseKeyIndexMap | (KEY1_PRESS >> keyIndex);
		}
	}

	return pressReleaseKeyIndexMap;
}

static BOOL key_is_key_pressed(U8 keyIndex)
{
	if(key_is_regen_key(keyIndex) ?
	   (gPreviousRegenIndexMap & (1L << keyIndex)) :
	   (gPreviousKeyIndexMap & (1L << keyIndex)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

static BOOL key_is_switch_key(U8 keyIndex)
{
	return ((key_get_key_type(keyIndex)) == KEY_TYPE_SWITCH) ? TRUE : FALSE;
}

static BOOL key_is_normal_key(U8 keyIndex)
{
	return ((key_get_key_type(keyIndex)) == KEY_TYPE_NORMAL) ? TRUE : FALSE;
}

/*
static BOOL key_is_rotary_key(U8 keyIndex)
{
	return (key_get_key_var_name(keyIndex) & KEY_TYPE_MASK) == KEY_TYPE_ROTARY;
}
*/

static BOOL key_is_toggle_key(U8 keyIndex)
{
	return ((key_get_key_type(keyIndex)) == KEY_TYPE_TOGGLE) ? TRUE : FALSE;
}


static void key_press_handler(U8 keyIndex, U32 checkKeyEvent)
{
/*
	if(keyIndex == KEY1 && !key_is_switch_key(keyIndex))   // power key
	{
		DRIVER_SetBusyMask(TRUE, DRIVER_BUSY_POWER_KEY);
	}

	if(key_is_rotary_key(keyIndex))
	{
		if(!gDriver_Key_Ctl.rotaryEncoderKeyStatus)
			MSG_MessageSendEx(gKey, KEY_DOWN, NULL, (U32)keyIndex);

		gDriver_Key_Ctl.rotaryEncoderKeyStatus |=  (0x0001<<keyIndex);
	}
    */
	if(key_is_toggle_key(keyIndex))
	{
        key_cancel_timer(keyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET);
		gKeyTimerType[keyIndex] = TIMER_KEY_INTERNAL_TOGGLE_LONG;
		if(!key_is_timer_set(keyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET))
		{
			key_set_timer(keyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET, TIMER_KEY_INTERNAL_TOGGLE_LONG);
		}
	}

	if(key_is_switch_key(keyIndex))
	{
		if(gKeyTimerType[keyIndex] == TIMER_KEY_NONE)
		{
			key_set_timer(keyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET, TIMER_KEY_LONG);
			MSG_MessageSendEx(gKey, EVENT_KEY_DOWN, NULL, (U32)keyIndex);
		}
	}
	else if(key_is_normal_key(keyIndex))
	{
		if(keyIndex < KEY5 && !NO_COMBO_KEY_MODE_FEAT)	//KEY is normal with combo effect
		{
			// Send EVENT_KEY_DOWN event as soon as any key is pressed
			MSG_MessageSendEx(gKey, EVENT_KEY_DOWN, NULL, (U32)keyIndex);

			// Keep current key status
			gCurrentKeyStatus |= checkKeyEvent;
			gCurrentKeyStatus ++;

			// First key
			if(GET_CURRENT_PRESSED_KEY_COUNT == 1)
			{
				if(gCurrentPressedKeyIndex != 0xFF)
				{
					key_cancel_timer(gCurrentPressedKeyIndex, MESSAGE_KEY_MANAGE_REPEAT_OFFSET);
				}

				gCurrentPressedKeyIndex = keyIndex;

				if(gKeyTimerType[keyIndex] == TIMER_KEY_NONE)
				{
					key_set_timer(keyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET, TIMER_KEY_MULTI);
					key_set_timer(keyIndex, MESSAGE_KEY_MANAGE_REPEAT_OFFSET, TIMER_KEY_REPEAT);
				}
			}
			// combo
			else if(gCurrentPressedKeyIndex != 0xFF)
			{
				key_cancel_timer(gCurrentPressedKeyIndex, MESSAGE_KEY_MANAGE_REPEAT_OFFSET);
				if(gKeyTimerType[gCurrentPressedKeyIndex] == TIMER_KEY_MULTI)
				{
					key_set_timer(gCurrentPressedKeyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET, TIMER_KEY_MULTI);
				}
				else // invalid combo
				{
					key_cancel_timer(gCurrentPressedKeyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET);
					gCurrentPressedKeyIndex = 0xFF;
				}
			}
		}
		else  // KEY is normal.
		{
			// Send EVENT_KEY_DOWN event as soon as any key is pressed
			MSG_MessageSendEx(gKey, EVENT_KEY_DOWN, NULL, (U32)keyIndex);

			// First key
			if(!gCurrentKeyStatus)
			{
				gCurrentPressedKeyIndex = keyIndex;
				key_set_timer(keyIndex, MESSAGE_KEY_MANAGE_REPEAT_OFFSET, TIMER_KEY_REPEAT);
			}

			// Keep current key status
			gCurrentKeyStatus |= checkKeyEvent;

			if(gKeyTimerType[keyIndex] == TIMER_KEY_NONE)
			{
				key_set_timer(keyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET, TIMER_KEY_MULTI);
			}
		}
	}
}

static void key_release_handler(U8 keyIndex)
{
	/*
	if(keyIndex == KEY1 && !key_is_switch_key(keyIndex))   // power key
	{
		DRIVER_SetBusyMask(FALSE, DRIVER_BUSY_POWER_KEY);
	}

	if(key_is_rotary_key(keyIndex))
	{
		gDriver_Key_Ctl.rotaryEncoderKeyStatus &= (~(0x0001<<keyIndex));

		if(gDriver_Key_Ctl.rotaryEncoderKeyStatus)
		{
			MSG_MessageSendEx(gKey, EVENT_KEY_UP, NULL, (U32)(keyIndex));
		}
	}
    */
	if(key_is_toggle_key(keyIndex))
	{
        key_cancel_timer(keyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET);
		gKeyTimerType[keyIndex]= TIMER_KEY_INTERNAL_TOGGLE_NONE;
		if(!key_is_timer_set(keyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET))
		{
			key_set_timer(keyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET, TIMER_KEY_INTERNAL_TOGGLE_NONE);
		}
	}
    else
        MSG_MessageSendEx(gKey, EVENT_KEY_UP, NULL, (U32)(keyIndex));
	if(key_is_switch_key(keyIndex))
	{
		switch(gKeyTimerType[keyIndex])
		{
			case TIMER_KEY_LONG:
				MSG_MessageSendEx(gKey, EVENT_KEY_LONGPRESS_RELEASE, NULL, (U32)(keyIndex));
				break;
			case TIMER_KEY_LONGLONG:
				MSG_MessageSendEx(gKey, EVENT_KEY_LONGLONGPRESS_RELEASE, NULL, (U32)(keyIndex));
				break;

			case TIMER_KEY_NONE:
			case TIMER_KEY_EXTREMELY_LONG:
				MSG_MessageSendEx(gKey, EVENT_KEY_EXTREMELY_LONGPRESS_RELEASE, NULL, (U32)(keyIndex));
				break;
		}
		key_cancel_timer(keyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET);
	}
	else if(key_is_normal_key(keyIndex))
	{
		if(keyIndex < KEY5 && !NO_COMBO_KEY_MODE_FEAT)
		{
			if(GET_CURRENT_PRESSED_KEY_COUNT == 1)
			{
				gCurrentKeyStatus &= ~(KEY1_PRESS >> keyIndex);
				gCurrentKeyStatus--;

				if(gCurrentPressedKeyIndex == keyIndex)
				{
					goto NORMAL_SINGLE_KEY_RELEASE;
				}
			}
			else
			{
				if(gCurrentPressedKeyIndex != 0xFF)
				{
					switch(gKeyTimerType[gCurrentPressedKeyIndex])
					{
						case TIMER_KEY_MULTI:
						case TIMER_KEY_PRESS:
							MSG_MessageSendEx(gKey, EVENT_KEY_TAP, NULL, (U32)(GET_CURRENT_PRESSED_KEY_MAP & 0XF0));
							key_cancel_timer(gCurrentPressedKeyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET);
							break;

						case TIMER_KEY_LONG:
							MSG_MessageSendEx(gKey, EVENT_KEY_PRESS_RELEASE, NULL, (U32)(GET_CURRENT_PRESSED_KEY_MAP & 0XF0));
							key_cancel_timer(gCurrentPressedKeyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET);
							break;

						case TIMER_KEY_LONGLONG:
							MSG_MessageSendEx(gKey, EVENT_KEY_LONGPRESS_RELEASE, NULL, (U32)(GET_CURRENT_PRESSED_KEY_MAP & 0XF0));
							key_cancel_timer(gCurrentPressedKeyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET);
							break;

						case TIMER_KEY_EXTREMELY_LONG:
							MSG_MessageSendEx(gKey, EVENT_KEY_LONGLONGPRESS_RELEASE, NULL, (U32)(GET_CURRENT_PRESSED_KEY_MAP & 0XF0));
							key_cancel_timer(gCurrentPressedKeyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET);
							break;

						case TIMER_KEY_VERYLONG:
							MSG_MessageSendEx(gKey, EVENT_KEY_EXTREMELY_LONGPRESS_RELEASE, NULL, (U32)(GET_CURRENT_PRESSED_KEY_MAP & 0XF0));
							key_cancel_timer(gCurrentPressedKeyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET);
							break;

						default:
							break;
					}
				}
				gCurrentPressedKeyIndex = 0xFF;
				gCurrentKeyStatus --;
				gCurrentKeyStatus &= ~(KEY1_PRESS >> keyIndex);
			}
		}
		else
		{
			NORMAL_SINGLE_KEY_RELEASE:
			if(gCurrentPressedKeyIndex == keyIndex)
			{
				gCurrentPressedKeyIndex = 0xFF;
				key_cancel_timer(keyIndex, MESSAGE_KEY_MANAGE_REPEAT_OFFSET);
			}

			gCurrentKeyStatus &= ~(KEY1_PRESS >> keyIndex);

			switch(gKeyTimerType[keyIndex])
			{
				case TIMER_KEY_PRESS:
					MSG_MessageSendEx(gKey, EVENT_KEY_TAP, NULL, (U32)(keyIndex));
					key_cancel_timer(keyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET);
					break;

				case TIMER_KEY_LONG:
					MSG_MessageSendEx(gKey, EVENT_KEY_PRESS_RELEASE, NULL, (U32)(keyIndex));
					key_cancel_timer(keyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET);
					break;

				case TIMER_KEY_LONGLONG:
					MSG_MessageSendEx(gKey, EVENT_KEY_LONGPRESS_RELEASE, NULL, (U32)(keyIndex));
					key_cancel_timer(keyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET);
					break;

				case TIMER_KEY_EXTREMELY_LONG:
					MSG_MessageSendEx(gKey, EVENT_KEY_LONGLONGPRESS_RELEASE, NULL, (U32)(keyIndex));
					key_cancel_timer(keyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET);
					break;
				case TIMER_KEY_VERYLONG:
					MSG_MessageSendEx(gKey, EVENT_KEY_EXTREMELY_LONGPRESS_RELEASE, NULL, (U32)(keyIndex));
					key_cancel_timer(keyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET);
					break;

				case TIMER_KEY_MULTI:
					gKeyTimerType[keyIndex] = TIMER_KEY_TAP;
					key_set_timer(keyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET, TIMER_KEY_TAP);
					break;

				case TIMER_KEY_TAP:
					gKeyTimerType[keyIndex] = TIMER_KEY_DOUBLE;
					key_set_timer(keyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET, TIMER_KEY_DOUBLE);
					break;

				case TIMER_KEY_DOUBLE:
					gKeyTimerType[keyIndex] = TIMER_KEY_TRIPPLE;
					key_set_timer(keyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET, TIMER_KEY_TRIPPLE);
					break;

				default:
					// Keep for tap double and tripple
					break;
			}
		}

        if(!gCurrentKeyStatus)
        {
			MSG_MessageSendEx(gKey, EVENT_KEY_UP, NULL, (U32)(KEYALL));
        }
	}
}

static void key_normal_handler(bool isRegenKey, U32 keyIndexMap)
{
	U32 changedKeyEvent;
	U16 scanIndex;

	if(isRegenKey ? (keyIndexMap == gPreviousRegenIndexMap) : (keyIndexMap == gPreviousKeyIndexMap))
	{
		return;
	}

	// key bitmap change detected and process the corresponding keys (bit from left to right: press key1 ~ key16, release key1 ~ key16)
	changedKeyEvent = key_check_keypad_event(isRegenKey, isRegenKey ? gPreviousRegenIndexMap : gPreviousKeyIndexMap, keyIndexMap);

	for(scanIndex = 0; scanIndex < TOTAL_KEY_NUM; scanIndex++)
	{
		if(changedKeyEvent & (KEY1_PRESS >> scanIndex))
		{
			key_press_handler((U8)scanIndex, changedKeyEvent & (KEY1_PRESS >> scanIndex));
		}

		if(changedKeyEvent & (KEY1_RELEASE >> scanIndex))
		{
			key_release_handler((U8)scanIndex);
		}
	}

	// Update the key's bitmap.
	isRegenKey ? (gPreviousRegenIndexMap = keyIndexMap) : (gPreviousKeyIndexMap = keyIndexMap);

	return;
}

static void key_normal_timer_handler(U8 keyIndex)
{
	U8 pressType;

	pressType = gKeyTimerType[keyIndex];
	gKeyTimerType[keyIndex] = TIMER_KEY_NONE;

	switch(pressType)
	{
		case TIMER_KEY_MULTI:
			if(key_is_key_pressed(keyIndex))
			{
				key_set_timer(keyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET, TIMER_KEY_PRESS);
			}
			else
			{
				MSG_MessageSendEx(gKey, EVENT_KEY_TAP, NULL, (U32)(keyIndex));
			}
			break;
		case TIMER_KEY_TAP:
			if(!key_is_key_pressed(keyIndex))
			{
				MSG_MessageSendEx(gKey, EVENT_KEY_TAP, NULL, (U32)(keyIndex));
			}
			break;
		case TIMER_KEY_DOUBLE:
			if(!key_is_key_pressed(keyIndex))
			{
				MSG_MessageSendEx(gKey, EVENT_KEY_DOUBLE, NULL, (U32)(keyIndex));
			}
			break;
		case TIMER_KEY_TRIPPLE:
			if(!key_is_key_pressed(keyIndex))
			{
				MSG_MessageSendEx(gKey, EVENT_KEY_TRIPLE, NULL, (U32)(keyIndex));
			}
			break;
		case TIMER_KEY_PRESS:
			if(key_is_key_pressed(keyIndex))
			{
				MSG_MessageSendEx(gKey, EVENT_KEY_PRESS, NULL, (U32)((GET_CURRENT_PRESSED_KEY_COUNT > 1) ? (GET_CURRENT_PRESSED_KEY_MAP & 0XF0) : keyIndex));
				key_set_timer(keyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET, TIMER_KEY_LONG);
			}
			else
			{
				MSG_MessageSendEx(gKey, EVENT_KEY_TAP, NULL, (U32)(keyIndex));
			}
			break;
		case TIMER_KEY_LONG:
			if(key_is_switch_key(keyIndex))
			{
				key_set_timer(keyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET, TIMER_KEY_LONGLONG);
				MSG_MessageSendEx(gKey, EVENT_KEY_LONGPRESS, NULL, (U32)(keyIndex));
			}
			else if(key_is_normal_key(keyIndex))
			{
				if (key_is_key_pressed(keyIndex))
				{
					MSG_MessageSendEx(gKey, EVENT_KEY_LONGPRESS, NULL, (U32)((GET_CURRENT_PRESSED_KEY_COUNT > 1) ? (GET_CURRENT_PRESSED_KEY_MAP & 0XF0) : keyIndex));
					key_set_timer(keyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET, TIMER_KEY_LONGLONG);
				}
				else
				{
					MSG_MessageSendEx(gKey, EVENT_KEY_PRESS_RELEASE, NULL, (U32)(keyIndex));
				}
			}
			break;
		case TIMER_KEY_LONGLONG:
			if(key_is_switch_key(keyIndex))
			{
				key_set_timer(keyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET, TIMER_KEY_EXTREMELY_LONG);
				MSG_MessageSendEx(gKey, EVENT_KEY_LONGLONGPRESS, NULL, (U32)(keyIndex));
			}
			else if(key_is_normal_key(keyIndex))
			{
				if (key_is_key_pressed(keyIndex))
				{
					MSG_MessageSendEx(gKey, EVENT_KEY_LONGLONGPRESS, NULL, (U32)((GET_CURRENT_PRESSED_KEY_COUNT > 1) ? (GET_CURRENT_PRESSED_KEY_MAP & 0XF0) : keyIndex));
					key_set_timer(keyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET, TIMER_KEY_EXTREMELY_LONG);
				}
				else
				{
					MSG_MessageSendEx(gKey, EVENT_KEY_LONGPRESS_RELEASE, NULL, (U32)(keyIndex));
				}
			}
			break;
		case TIMER_KEY_EXTREMELY_LONG:
			if(key_is_switch_key(keyIndex))
			{
				gKeyTimerType[keyIndex] = TIMER_KEY_NONE;
				MSG_MessageSendEx(gKey, EVENT_KEY_EXTREMELY_LONGPRESS, NULL, (U32)(keyIndex));
			}
			else if(key_is_normal_key(keyIndex))
			{
				if (key_is_key_pressed(keyIndex))
				{
					MSG_MessageSendEx(gKey, EVENT_KEY_EXTREMELY_LONGPRESS, NULL, (U32)((GET_CURRENT_PRESSED_KEY_COUNT > 1) ? (GET_CURRENT_PRESSED_KEY_MAP & 0XF0) : keyIndex));
					gKeyTimerType[keyIndex] = TIMER_KEY_VERYLONG;
				}
				else
				{
					MSG_MessageSendEx(gKey, EVENT_KEY_LONGLONGPRESS_RELEASE, NULL, (U32)(keyIndex));
				}
			}
            else if(key_is_toggle_key(keyIndex))
            {
                if(key_is_key_pressed(keyIndex))
                {
                    MSG_MessageSendEx(gKey, EVENT_KEY_EXTREMELY_LONGPRESS, NULL, (U32)(keyIndex));
                }
            }
			break;
        case TIMER_KEY_EXTREMELY_LONG_EXCEED:
            if(key_is_toggle_key(keyIndex))
            {
                MSG_MessageSendEx(gKey, EVENT_KEY_EXTREMELY_LONGPRESS, NULL, (U32)(keyIndex));
            }
            break;
        case TIMER_KEY_INTERNAL_TOGGLE_LONG:
            if(key_is_toggle_key(keyIndex))
            {
                //key_set_timer(keyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET, TIMER_KEY_EXTREMELY_LONG_EXCEED);
                MSG_MessageSendEx(gKey, EVENT_KEY_LONGPRESS, NULL, (U32)((GET_CURRENT_PRESSED_KEY_COUNT > 1) ? (GET_CURRENT_PRESSED_KEY_MAP & 0XF0) : keyIndex));
            }
            break;
        case TIMER_KEY_INTERNAL_TOGGLE_NONE:
            if(key_is_toggle_key(keyIndex))
            {
                MSG_MessageSendEx(gKey, EVENT_KEY_LONGPRESS_RELEASE, NULL, (U32)(keyIndex));
            }
            break;
		default:
			break;
	}
}

static void key_repeat_timer_handler(U8 keyIndex)
{
	MSG_MessageSendEx(gKey, EVENT_KEY_REPEAT, NULL, (U32)(keyIndex));
	key_set_timer(keyIndex, MESSAGE_KEY_MANAGE_REPEAT_OFFSET, TIMER_KEY_REPEAT);
}

static U32 key_timer_handler(Handler handler, U16 id, void *msg, U32 ext_id)
{
	UNUSED(handler);
	UNUSED(msg);

	switch(ext_id)
	{
		case MESSAGE_KEY_MANAGE_TIMER_OFFSET:
			key_normal_timer_handler(id);
			break;

		case MESSAGE_KEY_MANAGE_REPEAT_OFFSET:
			key_repeat_timer_handler(id);
			break;

		default:
			FW_Assert(FALSE);
			break;
	}

	return 0;
}

static void key_cancel_timer(U8 keyIndex, U8 type)
{
	if(type == MESSAGE_KEY_MANAGE_TIMER_OFFSET)
	{
		gKeyTimerType[keyIndex] = TIMER_KEY_NONE;
	}

	FW_ReleaseTimer((Handler)&gKeyMapGpioTimerHandle, keyIndex, type);
}

static U32 key_gpio_timer(Handler handler, U16 id, void *msg, U32 id_ext)
{
	U8 keyIndex;

	UNUSED(msg);
	UNUSED(handler);

	DBG_LOG_MID_KEY("[Key]Rx VarName status. varName:0x%x, isPressed:%d", 2, id, id_ext);

	switch(id)
	{
		case MMI_KEY1_PIN:
		case MMI_KEY2_PIN:
		case MMI_KEY3_PIN:
		case MMI_KEY4_PIN:
		case MMI_KEY5_PIN:
		case MMI_KEY6_PIN:
		case MMI_KEY7_PIN:
		case MMI_KEY8_PIN:
		case MMI_KEY9_PIN:
		case MMI_KEY10_PIN:
			if(KEY_NOT_USED != (keyIndex = key_get_key_index(id)))
			{
				(id_ext) ? (gDrvVarKeyMap |= (1L << keyIndex)) : (gDrvVarKeyMap &= ~(1L << keyIndex));
				key_normal_handler(FALSE, gDrvVarKeyMap);
			}
			break;

		case HAL_POWER_KEY_PIN:
			if(KEY_NOT_USED != (keyIndex = key_get_regen_key_index()))
			{
				(id_ext) ? (gDrvVarRegenMap |= (1L << keyIndex)) : (gDrvVarRegenMap &= ~(1L << keyIndex));
				key_normal_handler(TRUE, gDrvVarRegenMap);
			}
			break;
		default:
			break;
	}

	return 0;
}

/*		Function Reserved*/
static bool key_is_timer_set(U8 keyIndex, U8 timerType)
{
	return FW_CheckTimer((Handler)&gKeyMapGpioTimerHandle, keyIndex, timerType);
}

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void key_map_gpio_init(Handler app)
{
    key_init_nvkey();
	gPreviousKeyIndexMap = 0;
	gPreviousRegenIndexMap = 0;
	gCurrentKeyStatus = 0;
    gKey = app;

	drv_pwrkey_init();
	DRV_KeyPad_Init();
	//DRV_KeyPad_Setting(0, 3, 0);
	DRV_KeyPad_Mapping();

    //DRV_SYS_POWER_LEV_Pin_Setting();
    DRV_KeyPad_INTR();

	// Register key pad message handler
	MSG_MessageKeyPadHandler((Handler)&gKeyMapGpioHandle);
}

BOOL key_map_gpio_key_pressed(void)
{
	return (MSG_MessageSearchAllHandler((Handler)&gKeyMapGpioTimerHandle))? TRUE:FALSE;
}

BOOL key_map_gpio_powerkey_pressed(void)
{
	return (gPreviousRegenIndexMap)? TRUE:FALSE;
}

BOOL key_map_gpio_regenkey_switched(void)
{
	U8 keyIndex;
	for(keyIndex = 0; keyIndex < TOTAL_KEY_NUM; keyIndex++)
	{
		if(key_is_regen_key(keyIndex))
		{
			if(key_is_switch_key(keyIndex))
				return TRUE;
		}
	}
	return FALSE;

}

BOOL key_map_gpio_switch_on_needed(void)
{
	U8 keyIndex;
	U32 status;

	for(keyIndex = 0; keyIndex < TOTAL_KEY_NUM; keyIndex++)
	{
		if(key_is_regen_key(keyIndex) && key_is_switch_key(keyIndex))
		{
			status = DRV_RegenKey_Getstatus();
			DBG_LOG_MID_KEY("[Key] regent key switch:%d, status:%d", 2, gKeyTimerType[keyIndex], status);
			return (status == 1 && gKeyTimerType[keyIndex] < TIMER_KEY_LONG) ? TRUE : FALSE;
		}
	}

	return FALSE;
}

U32 key_map_gpio_get_keytime(U8 keyIndex, U8 timerType)
{
    if(keyIndex < TOTAL_KEY_NUM)
        return key_get_key_time(keyIndex, timerType)*MILLI_SEC_100;
    else
    {
        FW_Assert(FALSE);
    }
    return 0;
}

BOOL key_map_gpio_keycaptouch(U8 keyIndex)
{
	return ((key_get_key_type(keyIndex)) == KEY_TYPE_CAPTOUCH) ? TRUE : FALSE;
}

void key_reset_toggle_timer(void)
{
    U8 keyIndex;
    for(keyIndex = KEY1; keyIndex < TOTAL_KEY_NUM; keyIndex++)
    {
        if(key_is_key_pressed(keyIndex) && key_is_toggle_key(keyIndex))
        {
            DBG_LOG_MID_KEY("[Key] KEY%d is toggle key and pressed", 1, keyIndex+1);
            if(!key_is_timer_set(keyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET))
            {
                key_set_timer(keyIndex, MESSAGE_KEY_MANAGE_TIMER_OFFSET, TIMER_KEY_INTERNAL_TOGGLE_LONG);
            }
        }
    }
}
