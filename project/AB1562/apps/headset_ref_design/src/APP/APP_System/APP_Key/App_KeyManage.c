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


#include "stdlib.h"
#include "bt.h"
#include "types.h"
#include "nvkey.h"
#include "App_EventOpcode.h"
#include "key_nvkey.h"
#include "key.h"
#include "App_KeyManage.h"
#include "App_System.h"
#include "System_Nvkey.h"
#include "App_Media.h"
#include "App_ResourceManager.h"
#include "App_EventKeyHandler.h"


#define APP_BUTTON_SEQ_BEGIN_INDEX  0x00

/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct
{
	U8 *pKeyButtonSequenceRecord;
	U8 maxSeqButtonNum;
}APP_BTN_SEQ_INFO_STRU;
/**************************************************************************************************
* Prototype
**************************************************************************************************/
static U32 app_KeyManage_Handler(Handler handler, U16 id, void *payload, U32 id_ext);
static void app_KeyManage_ReadConfiguration(void);

/**************************************************************************************************
* Variable
**************************************************************************************************/
static HandlerData *gpSystemHandlerData;
static KEY_BTN_SEQ_CONFIG_STRU *pKeyButtonSequenceInfo;
static APP_BTN_SEQ_INFO_STRU gKeyBtnSeqInfo;
static const HandlerData gKeyManageHandle = { app_KeyManage_Handler };

/**************************************************************************************************
* Static Function
**************************************************************************************************/
/** Timer **/
static void app_KeyManage_CancelAllTimer(U8 type)
{
	MSG_MessageCancelAll((Handler)&gKeyManageHandle, type);
}

static void app_KeyManage_ButtonSequenceSetTimer(U8 type, U16 time)
{
	FW_SetTimer((Handler)&gKeyManageHandle, type, 0, 0, time);
}

static U32 app_KeyManage_Handler(Handler handler, U16 id, void *payload, U32 id_ext)
{
	UNUSED(handler);
	UNUSED(payload);

	if(id < EVENT_KEY_ACTION_NUM)
	{
		MSG_MessageSendEx((Handler)gpSystemHandlerData, id, NULL, id_ext);
	}
	else if(id == EVENT_KEY_BUTTON_SEQUENCE1_TIMEOUT)
	{
		FW_Memset(gKeyBtnSeqInfo.pKeyButtonSequenceRecord, 0, sizeof(U8) * gKeyBtnSeqInfo.maxSeqButtonNum);
	}

	return 0;
}

static void app_KeyManage_ReadConfiguration(void)
{
	U8* pAddr;
	U8 dataSize;

	pAddr = (U8*)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_KEY_BTN_SEQ_PATTERN);
	dataSize = NVKEY_GetPayloadLength(NVKEYID_APP_KEY_BTN_SEQ_PATTERN);

	if(pAddr != NULL && dataSize > 0)
	{
		gKeyBtnSeqInfo.maxSeqButtonNum = dataSize / sizeof(KEY_BTN_SEQ_CONFIG_STRU);

		gKeyBtnSeqInfo.pKeyButtonSequenceRecord = (U8 *)malloc(sizeof(U8) * gKeyBtnSeqInfo.maxSeqButtonNum);
		FW_Memset(gKeyBtnSeqInfo.pKeyButtonSequenceRecord, 0, sizeof(U8) * gKeyBtnSeqInfo.maxSeqButtonNum);

		pKeyButtonSequenceInfo = (KEY_BTN_SEQ_CONFIG_STRU *)malloc(dataSize);
		FW_Memcpy(pKeyButtonSequenceInfo, pAddr, dataSize);
	}
	else
	{
		FW_Assert(FALSE);
	}
}

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
BOOL APP_KeyManage_CheckButtonSequence(U16 keyIndex, U16 keyEvent)
{
	U8 i, unmappedButtonSequenceNumber;
	BOOL matchButtonSequence;

	matchButtonSequence = FALSE;

	if (keyEvent == EVENT_KEY_TAP || keyEvent == EVENT_KEY_PRESS_RELEASE || keyEvent == EVENT_KEY_LONGPRESS_RELEASE ||
		keyEvent == EVENT_KEY_DOUBLE || keyEvent == EVENT_KEY_TRIPLE || keyEvent == EVENT_KEY_LONGLONGPRESS_RELEASE ||
		keyEvent == EVENT_KEY_EXTREMELY_LONGPRESS_RELEASE)
	{
		for(i = 0, unmappedButtonSequenceNumber = 0; i < gKeyBtnSeqInfo.maxSeqButtonNum; i++)
		{
			if(keyIndex == pKeyButtonSequenceInfo[i].keyAction[gKeyBtnSeqInfo.pKeyButtonSequenceRecord[i]].keyIndex &&
			   keyEvent == pKeyButtonSequenceInfo[i].keyAction[gKeyBtnSeqInfo.pKeyButtonSequenceRecord[i]].keyEvent)
			{
				gKeyBtnSeqInfo.pKeyButtonSequenceRecord[i]++;

				if(gKeyBtnSeqInfo.pKeyButtonSequenceRecord[i] == MAX_KEY_MATCH_SEQ_NUM || pKeyButtonSequenceInfo[i].keyAction[gKeyBtnSeqInfo.pKeyButtonSequenceRecord[i] + 1].keyIndex == KEYNONE)
				{
					gKeyBtnSeqInfo.pKeyButtonSequenceRecord[i] = APP_BUTTON_SEQ_BEGIN_INDEX;
					APP_Media_PushMediaEvent(pKeyButtonSequenceInfo[i].keyCode);
					APP_EvtKey_KeyEventHandler(APP_GetServiceBdAddr(), pKeyButtonSequenceInfo[i].keyCode);
					matchButtonSequence = TRUE;
					app_KeyManage_ButtonSequenceSetTimer(EVENT_KEY_BUTTON_SEQUENCE1_TIMEOUT, 10);// T.O. to clear the ctl data
				}
				else
				{
					app_KeyManage_ButtonSequenceSetTimer(EVENT_KEY_BUTTON_SEQUENCE1_TIMEOUT, System_ReadNvkeyBtnSequenceTimer() * ONE_SEC);
				}
			}
			else
			{
				gKeyBtnSeqInfo.pKeyButtonSequenceRecord[i] = APP_BUTTON_SEQ_BEGIN_INDEX;
				unmappedButtonSequenceNumber++;
			}
		}

		if(unmappedButtonSequenceNumber == gKeyBtnSeqInfo.maxSeqButtonNum)
		{
			app_KeyManage_CancelAllTimer(EVENT_KEY_BUTTON_SEQUENCE1_TIMEOUT);
		}
	}

	return matchButtonSequence;
}

void APP_KeyManage_Init(Handler handler)
{
	gpSystemHandlerData = (HandlerData*)handler;
	app_KeyManage_ReadConfiguration();
	key_map_gpio_init((Handler)&gKeyManageHandle);
}
