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


#ifndef _SYSTEM_NVKEY_KEY1_CONFIG_H_
#define _SYSTEM_NVKEY_KEY1_CONFIG_H_

#include "key_keyconfigstru.h"

/**************************************************************************************************
* Structure
**************************************************************************************************/
/**
 *
 * @KeyID 0xF208
 *
 */

/*NvkeyDefine NVKEYID_APP_KEY1_SETTING*/
/*NvkeyDefine NVKEYID_APP_KEY2_SETTING*/
/*NvkeyDefine NVKEYID_APP_KEY3_SETTING*/
/*NvkeyDefine NVKEYID_APP_KEY4_SETTING*/
/*NvkeyDefine NVKEYID_APP_KEY5_SETTING*/
/*NvkeyDefine NVKEYID_APP_KEY6_SETTING*/
/*NvkeyDefine NVKEYID_APP_KEY7_SETTING*/
/*NvkeyDefine NVKEYID_APP_KEY8_SETTING*/
/*NvkeyDefine NVKEYID_APP_KEY9_SETTING*/
/*NvkeyDefine NVKEYID_APP_KEY10_SETTING*/

typedef struct
{
	KEY_TIME_STRU keyTime;	/* Units:100ms, Range:0x00-0xFF
							   keyMultiTime 				Val:0x07
							   keyPressTime					Val:0x0F
							   keyLongPressTime				Val:0x0A
							   keyLongLongPressTime			Val:0x1E
							   keyExtremelyLongPressTime	Val:0x1E
							   keyContPressTime				Val:0x05
							*/
	U8 keyVarName;				// Val:0x00, Range:0-31, 255(for not use)
	U8 reserved:1;	// Val:0x01, Range:0->Not Regen Key, 1->Regen Key
	U8 keyType:7;
							// Note: if isWithRegenKey = 0x01, keyGpio must = 0x00
}KEY_CONFIG_STRU;

#endif
