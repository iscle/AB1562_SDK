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
 
 
#ifndef _NVRAM_TYPE_APP_FEATURE_H_
#define _NVRAM_TYPE_APP_FEATURE_H_

/**************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
 	system,
	audio1,
	audio2,
	audio3,
	led,
	mode,
	HSP,
	HFP,
	connection,
	reConnection,
	lineInFeature,
	FMFeature,
	A2DP,
	AVRCP,
	battery,
	customerUART,
	keys,
	airFeature
};

/**************************************************************************************************
* Structure
**************************************************************************************************/
/**
 *
 * @KeyID 0xF21E
 * 
 */

/*NvkeyDefine NVKEYID_APP_NVRAM_APP_FEATURE*/
typedef struct
{
	U8 	system;
	U16 audio1;
	U8 	audio2;
	U16 audio3;
	U8 	led;
	U8 	mode;
	U8 	HSP;
	U16 reserved;
	U16	connection;
	U16	lineInFeature;
	U16	FMFeature;
	U16 A2DP;
	U8 	AVRCP;
	U8  battery;
	U8 	customerUART;
	U8 	keys;
	U8 	airFeature;
}PACKED APP_FEATURE_CTL_STRU;//MMIFeatureCtl

/*
APP_FEATURE_CTL_STRU App_Feature = 
{//MMI Feature
	0x31, //System
	#ifdef SUPPORT_VOICE_COMMAND
	0x2BE9, 0x84, 0x08F4,  // Audio
	#else
	0x2BE9, 0x84, 0x08F4,  // Audio
	#endif
	0x01, //LED
	0x00, //Mode
	0x70, //HSP
	0x001F
	#ifdef _HFP_CODEC_NEGO_
	| 0x0080
	#endif
	#ifdef _HFP_HF_INDICATORS_
	| 0x0100
	#endif
	, //HFP
#ifdef TRSPX_App
	0x0000,
	#else
	0x0030
	#ifdef HFP_Profile
	| 0x00CC
	#endif
	, // Connection
#endif
	0x1E ,  //ReConnection
	0x0000, //LineInFeature
	0x0000, //FMFeature
	#ifndef HFP_Profile
	0x0002 | //A2DP, //MMI_MUSIC_LAST_IN_FIRST_PLAYED_AND_PAUSE_OTHERS_FEAT
	#else
	0x0200 | //A2DP, //MMI_A2DP_MEMPUT_WHEN_OTHER_IN_MUSIC_FEAT
	#endif
	#ifdef OPERATION_IN_BB_FPGA_VERSION
	0x0060, //A2DP
	#else
	0x01C0, //A2DP
	#endif
	0x03, //AVRCP
	0x04, //Battery
	0x00 //customerUART
	#ifdef TRSPX_App
	| 0x04
	#endif
	,
	0x21, //Keys
	0x00, //AirFeature
};
*/

#endif
