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
 
 
#ifndef _APP_A2DP_NVKEY_CONFIG1_H_
#define _APP_A2DP_NVKEY_CONFIG1_H_


/**************************************************************************************************
* Structure
**************************************************************************************************/

/**
 * 
 * @KeyID 0xF213
 * 
 *
*/
/*NvkeyDefine NVKEYID_APP_A2DP_CONFIG */
typedef struct APP_A2DP_CONFIG_STRU
{
	/*
		#define APP_A2DP_CYCLIC_VOLUME_ENABLED_FEAT							(0x0001)
		#define APP_MUSIC_LAST_IN_FIRST_PLAYED_AND_PAUSE_OTHERS_FEAT		(0x0002)
		#define APP_A2DP_VOLUME_SYNC_HFP_VOLUME_FEAT						(0x0008)
		#define APP_ADJUST_A2DP_RATHER_THAN_HFP_IN_IDLE_FEAT				(0x0010)
		#define APP_MUSIC_LAST_IN_FIRST_PLAYED_NO_PAUSE_OTHERS_FEAT			(0x0020)	
		#define APP_MUSIC_FORCE_USE_PEQ_FEAT								(0x0040)
		#define APP_A2DP_MEMPUT_WHEN_CALLACTIVE_FEAT						(0x0080)
		#define APP_REMEMBER_PEQ_CHANGE_FEAT								(0x0100)
		#define APP_A2DP_MEMPUT_WHEN_OTHER_IN_MUSIC_FEAT					(0x0200)
		#define APP_A2DP_RESERVED_FEAT										(0x0400)
		#define APP_DEFAULT_PEQ_ASSIGNMENT_FEAT								(0x0800)
		#define APP_A2DP_BYPASS_IPHONE_CHECK_FOR_AAC_FUNCTION_FEAT			(0x1000)
		#define APP_A2DP_DO_NOT_SEND_SUSPEND_AFTER_ACTIVE_START_FEAT		(0x2000)
		#define APP_A2DP_SEND_START_RESP_AFTER_DSP_IS_ENABLED_FEAT			(0x4000)
		APP_MUSIC_FORCE_USE_PEQ_FEAT | APP_A2DP_MEMPUT_WHEN_CALLACTIVE_FEAT | APP_REMEMBER_PEQ_CHANGE_FEAT | APP_A2DP_MEMPUT_WHEN_OTHER_IN_MUSIC_FEAT = 0x03C0
	*/ 
	U16 feature;				// Val:0x03E4, Range:0x0000~0xFFFF
}PACKED APP_A2DP_CONFIG_STRU;

#endif
