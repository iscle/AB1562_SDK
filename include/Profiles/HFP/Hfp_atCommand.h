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
 
 
#ifndef __HFP_ATCMD_HH_
#define __HFP_ATCMD_HH_

#define CHAR_TO_DIGIT(A)	((U8)((A) - 0x30))
#define DIGIT_TO_CHAR(A)	((U8)((A) + 0x30))

#define SCO_CODEC_NONE		0x00
#define SCO_CODEC_CVSD		0x01
#define SCO_CODEC_MSBC		0x02

enum
{
	HFP_CMD_IDLE,
	HFP_CMD_SEND,
	HFP_CMD_POP_INTERNAL_CMD,
	HFP_CMD_COUNT,
	
	HFP_CMD_INTERNAL_POP_CMD,
	HFP_CMD_INTERNAL_START,
	HFP_CMD_INTERNAL_EN_CALL_WAITING_NOTIFICAITON,		//0x06
	HFP_CMD_INTERNAL_SELECT_CHAR_SET_UTF8,				//0x07
	HFP_CMD_INTERNAL_ENABLE_CLI_NOTIFICATION,			//0x08
	HFP_CMD_INTERNAL_SET_AG_NREC_OFF,					//0x09
	HFP_CMD_INTERNAL_ACTIVATE_INDICATOR,				//0x0A
	HFP_CMD_INTERNAL_XAPL,								//0x0B
	HFP_CMD_INTERNAL_XEVENT_BATTERY_INQUIRY,			//0x0C
	HFP_CMD_INTERNAL_IPHONEACCEV,						//0x0D
	HFP_CMD_INTERNAL_XEVENT_BATTERY_SEND,				//0x0E
	HFP_CMD_INTERNAL_BIEV_SEND,							//0x0F
	HFP_CMD_INTERNAL_SLC_VGS_CMD,						//0x10
	HFP_CMD_INTERNAL_END,								//0x11
};


typedef struct
{
	//BD_ADDR_T bdAddr;
	U8 cmdState;
	U8 cmdSubstate;
	U8 length;
	U8 atCmdString;
}HFP_AT_GENERAL_CMD_T;


enum
{
	HFP_BIND_ENCHANCED_SAFTY = 1,
	HFP_BIND_BATTERY_LEVEL,
};

extern const HandlerData gHfpAtCmdHandler;

/**************************************************************************************************
* Public functions
**************************************************************************************************/
U16 hfp_AscIIToU16 (U8 * ptr1);
U8 *hfp_U16ToAscII(U8 *ptr1, U16 number);
U8 	hfp_GetATCmdLength (U8 * at_cmd);
const HandlerData *HFP_GetAtCmdHandler(void);

#endif//__HFP_ATCMD_HH_
