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
#include "App_EventOpcode.h"
#include "key_nvkey.h"
#include "key.h"
#include "app_key_eventmap_nvstruc.h"
#include "App_Nvkey_KeymapHandler.h"
#include "App_ResourceManager.h"
#include "hal_gpio_internal.h"
#include "log.h"
#include "nvkey_hdl.h"
#include "App_System.h"
#include "App_Customer.h"

/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct
{
	AppKeyMapType *pActionAddr;
	U16 len;
}PACKED KEY_ACTION_KEYMAP_ADDR_STRU;

typedef struct USER_KEY_CTL_STRU
{
	UserKey* userKey;
	U16 userKeyLen;
}PACKED USER_KEY_CTL_STRU;

/**************************************************************************************************
* Variables
**************************************************************************************************/
static USER_KEY_CTL_STRU gUserKeyCtl;
KEY_ACTION_KEYMAP_ADDR_STRU gActionKeyMap[EVENT_KEY_ACTION_NUM];

/**************************************************************************************************
* Public Function
**************************************************************************************************/
void APP_NVKEY_KeyParseEventPrintLog(U16 keyIndex, U8 keyAction, U8 state)
{
	if( keyIndex >= KEY_COMB_34 )
	{
		switch(keyIndex)
		{
			case KEY_COMB_34:
				keyIndex = 14;//Mapping log string table in log.c
				break;
			case KEY_COMB_23:
				keyIndex = 15;//Mapping log string table in log.c
				break;
			case KEY_COMB_13:
				keyIndex = 16;//Mapping log string table in log.c
				break;
			case KEY_COMB_24:
				keyIndex = 17;//Mapping log string table in log.c
				break;
			case KEY_COMB_14:
				keyIndex = 18;//Mapping log string table in log.c
				break;
			case KEY_COMB_12:
				keyIndex = 19;//Mapping log string table in log.c
				break;
			case KEY_COMB_123:
				keyIndex = 20;//Mapping log string table in log.c
				break;
			case KEY_COMB_124:
				keyIndex = 21;//Mapping log string table in log.c
				break;
			case KEY_COMB_134:
				keyIndex = 22;//Mapping log string table in log.c
				break;
			case KEY_COMB_234:
				keyIndex = 23;//Mapping log string table in log.c
				break;
			case KEY_COMB_1234:
				keyIndex = 24;//Mapping log string table in log.c
				break;
		}
	}

    if(keyIndex <= 24)
        DBG_LOG_APP_SYSTEM( "[Key]: index: %s, action: %s, stateMask: 0x%x", 3, APP_KeyIndexLogString[keyIndex], APP_ActionLogString[keyAction], (U32)(1L << state));
    else
        DBG_LOG_APP_SYSTEM( "[Key]: index: %d, action: %d, stateMask: 0x%x", 3, keyIndex, keyAction, (U32)(1L << state));
}

U16 APP_NVKEY_KeyParseEvent(U16 keyIndex, U8 keyAction, U8 state)
{
	U8 dataNum = 0;
	U16 len = 0;
	AppKeyMapType *ptr;

	len = gActionKeyMap[keyAction].len;
	ptr = gActionKeyMap[keyAction].pActionAddr;
	U8	deleteKeyIndex = 0xFF;


	if(gUserKeyCtl.userKey)
	{
		U8 userSettingNum = (gUserKeyCtl.userKeyLen / sizeof(UserKey));

		UserKey *uPtr = gUserKeyCtl.userKey;

		while(userSettingNum)
		{
			if((uPtr->action == keyAction) && ((uPtr->KeyMapType.keyCode == KEYALL) || (uPtr->KeyMapType.keyCode == keyIndex)) && ((1L << state) & uPtr->KeyMapType.keySupportedState))
			{
				if(uPtr->KeyMapType.appKeyEvent)
				{
					return uPtr->KeyMapType.appKeyEvent;
				}
				else//delte key
				{
				    
					deleteKeyIndex = uPtr->KeyMapType.keyCode;
					//deleteState = uPtr->KeyMapType.keySupportedState;
                    DBG_LOG_APP_SYSTEM( "[Key]: deleteKeyIndex 0x%x", 1, uPtr->KeyMapType.keyCode);
				}
			}
			uPtr++;
			userSettingNum--;
		}
	}


	if(!len || !ptr)
	{
		return KEY_INVALID;
	}

	dataNum = len / sizeof(AppKeyMapType);

	while(dataNum)
	{
		if(((ptr->keyCode == keyIndex) || (ptr->keyCode == KEYALL)) && ((1L << state) & ptr->keySupportedState))
		{
			if((ptr->keyCode != deleteKeyIndex))
			{
			     //DBG_LOG_APP_SYSTEM( "[Key]: (0x%x 0x%x) (0x%x)", 3,  ptr->keyCode, deleteKeyIndex, ptr->keySupportedState);   
                return APP_Customer_ParseCustKeyEvent(ptr,keyAction);
                //return ptr->appKeyEvent; marked for APP_Customer_ParseCustKeyEvent
			}
		}

		ptr++;
		dataNum--;
	}

	return KEY_INVALID;
}

U32 APP_NVKEY_GetSupportedStateByEventCode(U16 eventCode)
{
	U16 i;
	AppKeyMapType *ptr = NULL;
	U16 len = 0;
	U8 dataNum = 0;

	if(gUserKeyCtl.userKey)
	{
		UserKey *uPtr = gUserKeyCtl.userKey;
		U8 userSettingNum = (gUserKeyCtl.userKeyLen / sizeof(UserKey));
		while(userSettingNum)
		{
			if((uPtr->KeyMapType.appKeyEvent == eventCode) && (uPtr->KeyMapType.keySupportedState))
			{
				return uPtr->KeyMapType.keySupportedState;
			}
			uPtr++;
			userSettingNum--;
		}
	}

	for(i = EVENT_KEY_TAP; i< EVENT_KEY_ACTION_NUM; i++)
	{
		len = gActionKeyMap[i].len;
		ptr = gActionKeyMap[i].pActionAddr;


		if(!len || !ptr)
		{
			return 0;
		}

		dataNum = len / sizeof(AppKeyMapType);

		while(dataNum)
		{
			if((ptr->appKeyEvent == eventCode) && (ptr->keySupportedState))
			{
				return ptr->keySupportedState;
			}

			ptr++;
			dataNum--;
		}

	}

	return 0;

}

void APP_NVKEY_InitKeyMapAddr(void)
{
	U8 i;
	U16 nvkeyid = 0;

	for(i = EVENT_KEY_TAP; i< EVENT_KEY_ACTION_NUM; i++)
	{
		switch(i)
		{
			case EVENT_KEY_TAP:
			case EVENT_KEY_PRESS:
			case EVENT_KEY_PRESS_RELEASE:
			case EVENT_KEY_LONGPRESS:
			case EVENT_KEY_LONGPRESS_RELEASE:
			case EVENT_KEY_DOUBLE:
			case EVENT_KEY_TRIPLE:
			case EVENT_KEY_LONGLONGPRESS:
			case EVENT_KEY_LONGLONGPRESS_RELEASE:
			case EVENT_KEY_EXTREMELY_LONGPRESS:
			case EVENT_KEY_EXTREMELY_LONGPRESS_RELEASE:
			case EVENT_KEY_REPEAT:
				nvkeyid = NVKEYID_APP_KEY_ACTION_TAP + (i-EVENT_KEY_TAP);
				break;
			case EVENT_KEY_DOWN:
			case EVENT_KEY_UP:
				nvkeyid = NVKEYID_APP_KEY_ACTION_DOWN + (i-EVENT_KEY_DOWN);
				break;
			default:
				gActionKeyMap[i].len = 0;
				gActionKeyMap[i].pActionAddr = NULL;
				break;
		}
		gActionKeyMap[i].len = NVKEY_GetPayloadLength(nvkeyid);
		gActionKeyMap[i].pActionAddr = (AppKeyMapType*)NVKEY_GetPayloadFlashAddress(nvkeyid);
	}
}

void APP_CustomizeKey_RaceNotify(U8 channelId)
{
	RACE_Nvkey_Customer_Key_Notification(channelId, gUserKeyCtl.userKeyLen, gUserKeyCtl.userKey);
}

void APP_CustomizedKey_Init(void *msg)
{
	U16 len;
	UserKey* ptr;

	if(msg)//load from para
	{
		len = ((MSG_ID_RACE_TO_APP_SET_CUSTOMER_KEY_T *)msg)->length;
	}
	else//load from nvkey
	{
		len = NVKEY_GetPayloadLength(NVKEYID_CUSTOMER_KEY_CONFIG);
	}

	if((len < sizeof(UserKey)) || ((len % sizeof(UserKey)) != 0))
		return;

	if(gUserKeyCtl.userKey)
	{
		FW_FreeMemory(gUserKeyCtl.userKey);
		gUserKeyCtl.userKeyLen = 0;
	}

	gUserKeyCtl.userKey = FW_GetMemory(len);

	if(msg)
	{
		DBG_LOG_APP_SYSTEM("==[Key] app key load from para!!!", 0);
		ptr =(UserKey *)((MSG_ID_RACE_TO_APP_SET_CUSTOMER_KEY_T *)msg)->data;
	}
	else
	{
		DBG_LOG_APP_SYSTEM("==[Key] app key load from nvkey!!!", 0);
		ptr = (UserKey *)NVKEY_GetPayloadFlashAddress(NVKEYID_CUSTOMER_KEY_CONFIG);

	}

	FW_Memcpy(gUserKeyCtl.userKey, ptr, len);
	gUserKeyCtl.userKeyLen = len;

	DBG_LOG_APP_SYSTEM("[Key]: CustomizedKey Init, KeyNum(%d)", 1, (len/sizeof(UserKey)));

	nvkey_hdl_save_nvkey(NVKEYID_CUSTOMER_KEY_CONFIG, gUserKeyCtl.userKeyLen ,gUserKeyCtl.userKey);

	if(gUserKeyCtl.userKey)
	{
		U8 userSettingNum = (gUserKeyCtl.userKeyLen / sizeof(UserKey));

		UserKey *uPtr = gUserKeyCtl.userKey;

		while(userSettingNum)
		{
			DBG_LOG_APP_SYSTEM("==[Key] app key init(%d), action[%d]", 2, userSettingNum, uPtr->action);
			DBG_LOG_APP_SYSTEM("==[Key] app key init(%d), key index[%d]", 2, userSettingNum, uPtr->KeyMapType.keyCode);
			DBG_LOG_APP_SYSTEM("==[Key] app key init(%d), key event[%d]", 2, userSettingNum, uPtr->KeyMapType.appKeyEvent);
			DBG_LOG_APP_SYSTEM("==[Key] app key init(%d), key supportedState[0x%x]", 2, userSettingNum, uPtr->KeyMapType.keySupportedState);
			uPtr++;
			userSettingNum--;
		}
	}

}

