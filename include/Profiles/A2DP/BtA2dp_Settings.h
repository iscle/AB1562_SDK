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


#ifndef _BT_A2DP_SETTING_HH_
#define _BT_A2DP_SETTING_HH_

#include "feature_config.h"


#define A2DP_SIGNAL_MTU_SIZE		335
#define A2DP_DEFAULT_MEDIA_MTU_SIZE	672
#define A2DP_AAC_MEDIA_MTU_SIZE		1020
#define UNUSED_SAMPLE_RATE_INDEX	0xFF

/************************************************************************************************
ENUM
**************************************************************************************************/
enum
{
	A2DP_AAC_SAMPLE_RATE_INDEX_8000 = 0,
	A2DP_AAC_SAMPLE_RATE_INDEX_11025,
	A2DP_AAC_SAMPLE_RATE_INDEX_12000,
	A2DP_AAC_SAMPLE_RATE_INDEX_16000,
	A2DP_AAC_SAMPLE_RATE_INDEX_22050,
	A2DP_AAC_SAMPLE_RATE_INDEX_24000,
	A2DP_AAC_SAMPLE_RATE_INDEX_32000,
	A2DP_AAC_SAMPLE_RATE_INDEX_44100,
	A2DP_AAC_SAMPLE_RATE_INDEX_48000,
	A2DP_AAC_SAMPLE_RATE_INDEX_64000,
	A2DP_AAC_SAMPLE_RATE_INDEX_88200,
	A2DP_AAC_SAMPLE_RATE_INDEX_96000,
};

enum
{
	A2DP_SBC_SAMPLE_RATE_INDEX_16000 = 0,
 	A2DP_SBC_SAMPLE_RATE_INDEX_32000,
	A2DP_SBC_SAMPLE_RATE_INDEX_44100,
	A2DP_SBC_SAMPLE_RATE_INDEX_48000,
};


enum
{
	DUMMY_SEID, /* to ensure real SEID start from 1 */
	SBC_SNK_SEID,
	AAC_SNK_SEID,
	SBC_SRC_SEID,
	CELT_SNK_SEID,
	AUX_ENCODER_SEID = 0x3E,
};

#endif //_BT_A2DP_SETTING_HH_
