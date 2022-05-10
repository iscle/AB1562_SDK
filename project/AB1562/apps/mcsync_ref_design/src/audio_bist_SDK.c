/* Copyright Statement:
 *
 * (C) 2019  Airoha Technology Corp. All rights reserved.
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
#include "audio_bist_SDK.h" //It is at "include\system\"
#include "afe_ctl.h"

/**************************************************************************************************
* Public Function   20180323_Nick
**************************************************************************************************/
/*
WireVal = DRV_3WIRE_Read(0xC3);
DRV_3WIRE_Write(0xC3, WireVal | KS_EN_ENABLE);
*/
extern VOID DRV_3WIRE_Write(U8 Addr, U16 Data);
VOID AudioBIST_CP_L_Configuration (VOID)
{
	//Audio BIST ADDA Left channel configuration.
	DRV_3WIRE_Write(0x23, 0x0000);
	DRV_3WIRE_Write(0x24, 0x0388);
	DRV_3WIRE_Write(0x25, 0x0000);
	DRV_3WIRE_Write(0x26, 0x0003);
	DRV_3WIRE_Write(0x27, 0xE7C0);
	DRV_3WIRE_Write(0x28, 0x0004);
	DRV_3WIRE_Write(0x56, 0x0000);
	DRV_3WIRE_Write(0x57, 0x14B4);
	DRV_3WIRE_Write(0x58, 0x3A5B);
	#ifdef AB1531_1532_CP_TEST
	DRV_3WIRE_Write(0x59, 0x8A8B); //for AB1532_1
	#elif defined(AB1533_CP_TEST)
	DRV_3WIRE_Write(0x59, 0x8A87); //for AB1533
	#endif
	DRV_3WIRE_Write(0x5A, 0x3CD0);
	DRV_3WIRE_Write(0x5B, 0x8844);
	DRV_3WIRE_Write(0x5C, 0x3640);
	DRV_3WIRE_Write(0x5D, 0xAFFD);
	DRV_3WIRE_Write(0x5E, 0x0000);
	DRV_3WIRE_Write(0x5F, 0x0118);
	DRV_3WIRE_Write(0x60, 0x8F3F);
	DRV_3WIRE_Write(0x61, 0xAC03);
	DRV_3WIRE_Write(0x62, 0x0000);
	DRV_3WIRE_Write(0x63, 0x0148);
	DRV_3WIRE_Write(0x64, 0x2604);
	DRV_3WIRE_Write(0x65, 0x0F0F);
	DRV_3WIRE_Write(0x66, 0x8841);
	DRV_3WIRE_Write(0x67, 0x1070);
	DRV_3WIRE_Write(0x68, 0x03FF);
	DRV_3WIRE_Write(0x73, 0x0000);
	DRV_3WIRE_Write(0x74, 0x0000);
	DRV_3WIRE_Write(0x75, 0x2001);
	DRV_3WIRE_Write(0x76, 0x2435);
	#ifdef AB1531_1532_CP_TEST
	DRV_3WIRE_Write(0x77, 0x03FF); //for AB1532_1
	DRV_3WIRE_Write(0x78, 0x0000); //for AB1532_1
	#elif defined(AB1533_CP_TEST)
	DRV_3WIRE_Write(0x77, 0xFFFF); //for AB1533
	DRV_3WIRE_Write(0x78, 0x0001); //for AB1533
	#endif
	DRV_3WIRE_Write(0x79, 0x0000);
	DRV_3WIRE_Write(0x7A, 0x0000);
	DRV_3WIRE_Write(0x8D, 0x8CFF);
	DRV_3WIRE_Write(0xA3, 0x00F0);
	DRV_3WIRE_Write(0xED, 0x4E23);
}

VOID AudioBIST_CP_R_Configuration (VOID)
{
	//Audio BIST ADDA Right channel configuration.


	DRV_3WIRE_Write(0x23, 0x0000);
	DRV_3WIRE_Write(0x24, 0x0388);
	DRV_3WIRE_Write(0x25, 0x0000);
	DRV_3WIRE_Write(0x26, 0x0003);
	DRV_3WIRE_Write(0x27, 0xE7C0);
	DRV_3WIRE_Write(0x28, 0x0004);
	DRV_3WIRE_Write(0x56, 0x0000);
	DRV_3WIRE_Write(0x57, 0x14B4);
	DRV_3WIRE_Write(0x58, 0x3A5B);
	#ifdef AB1531_1532_CP_TEST
	DRV_3WIRE_Write(0x59, 0x85C7);//for AB1532_1
	#elif defined(AB1533_CP_TEST)
	DRV_3WIRE_Write(0x59, 0x85CB); //for AB1533
	#endif
	DRV_3WIRE_Write(0x5A, 0x14D0);
	DRV_3WIRE_Write(0x5B, 0x8844);
	DRV_3WIRE_Write(0x5C, 0x3640);
	DRV_3WIRE_Write(0x5D, 0xAFFD);
	DRV_3WIRE_Write(0x5E, 0x0000);
	DRV_3WIRE_Write(0x5F, 0x0118);
	DRV_3WIRE_Write(0x60, 0x8F3F);
	DRV_3WIRE_Write(0x61, 0xAC03);
	DRV_3WIRE_Write(0x62, 0x0000);
	DRV_3WIRE_Write(0x63, 0x0148);
	DRV_3WIRE_Write(0x64, 0x2504);
	DRV_3WIRE_Write(0x65, 0x0F0F);
	DRV_3WIRE_Write(0x66, 0x8841);
	DRV_3WIRE_Write(0x67, 0x1070);
	DRV_3WIRE_Write(0x68, 0x03FF);
	DRV_3WIRE_Write(0x73, 0x0000);
	DRV_3WIRE_Write(0x74, 0x0000);
	DRV_3WIRE_Write(0x75, 0x2001);
	DRV_3WIRE_Write(0x76, 0x2435);
	#ifdef AB1531_1532_CP_TEST
	DRV_3WIRE_Write(0x77, 0x03FF); //for AB1532_1
	DRV_3WIRE_Write(0x78, 0x0000); //for AB1532_1
	#elif defined(AB1533_CP_TEST)
	DRV_3WIRE_Write(0x77, 0xFFFF); //for AB1533
	DRV_3WIRE_Write(0x78, 0x0001); //for AB1533
	#endif
	DRV_3WIRE_Write(0x79, 0x0000);
	DRV_3WIRE_Write(0x7A, 0x0000);
	DRV_3WIRE_Write(0x8D, 0x8CFF);
	DRV_3WIRE_Write(0xA3, 0x0F00);
	DRV_3WIRE_Write(0xED, 0x4E23);
}
