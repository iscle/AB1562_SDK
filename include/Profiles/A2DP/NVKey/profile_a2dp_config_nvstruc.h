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
 
 
#ifndef __A2DP_NVKEY_CONFIG_H__
#define __A2DP_NVKEY_CONFIG_H__

/***************************************************************************************
 *	A2DP_NVKEY STRUCT 0xF014 (NVKEY_ID_USER_0 + NVKEY_ID_PROFILE_A2DP_CONFIG1)
 ***************************************************************************************/

/**
 *@KeyID 0xF214
*/
/*NvkeyDefine NVKEYID_PROFILE_A2DP_FEATURE_CONTROL*/
typedef struct A2DP_NVKEY_CONFIG
{
	/*
	#define A2DP_SEND_START_AFTER_ACTIVE_MEDIA_CHANNEL_CONN_FEAT	0x0001
	#define A2DP_DELAY_REPORT_FEAT									0x0002
	#define A2DP_DELAY_REPORT_REPEAT_FEAT							0x0004
	#define A2DP_BYPASS_IPHONE_CHECK_FOR_AAC_FUNCTION_FEAT			0x0008
	#define A2DP_DO_NOT_SEND_SUSPEND_AFTER_ACTIVE_START_FEAT		0x0010
	#define A2DP_SEND_START_RESP_AFTER_DSP_IS_ENABLED_FEAT			0x0020
	#define ADJUST_A2DP_RATHER_THAN_HFP_IN_IDLE_FEAT				0x0040

	*/
	U16 A2DPFeature;					/**< @Value 0x0008		@Desc A2DPFeature  @Range: 0x0000~0xFFFF*/

	U32 checkMediaChannelEstTimePassive; //unit,ms val:2600	/**< @Value 0x00000A28	@Desc checkMediaChannelEstTimePassive */
	U32 checkMediaChannelEstTimeActive;  //unit,ms val:2000	/**< @Value 0x000007D0	@Desc checkMediaChannelEstTimeActive */
	U32 gapTimer100; 					 //unit:s, by spec GAVDP 1.3 page 30 section 8. Tgavdp is proposed between 0.5~3 seconds
	U32 gapTimerInterval; 				 //unit:s, by spec GAVDP 1.3 page 30 section 8. Tgavdp is proposed between 0.5~3 seconds
	U32 suspendDelayTime;				 //unit:s

}PACKED A2DP_NVKEY_CONFIG;

#endif //__A2DP_NVKEY_CONFIG_H__
