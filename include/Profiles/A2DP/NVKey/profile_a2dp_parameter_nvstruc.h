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


#ifndef __A2DP_NVKEY_PARAMETER_H__
#define __A2DP_NVKEY_PARAMETER_H__

	/***************************************************************************************
	 *	A2DP_NVKEY STRUCT 0xF2D5 (NVKEYID_PROFILE_A2DP_PROFILE_PARAMETER)
	 ***************************************************************************************/

	/**
	 *@KeyID 0xF2D5
	*/
	/*NvkeyDefine NVKEYID_PROFILE_A2DP_PROFILE_PARAMETER*/
	typedef struct
	{
		U8 Content[2+4+8/*+2*/];	//Media Transport + Content Protection + Media Codec + Delay Reporting
		//U8 Content[2+4+8];	//Media Transport + Content Protection + Media Codec
	}SBCServiceCapabilities;

	typedef struct
	{
		U8 content1;
		U8 content2;
		U8 minBitPool;
		U8 maxBitPool;
	}SBCCodecSettings;

	typedef struct
	{
		U8 Content[2+4+10/*+2*/];	//Media Transport + Content Protection + Media Codec + Delay Reporting
	}AACServiceCapabilities;

	typedef struct
	{
		U8 objectType;
		U8 sampleRate1;
		U8 sampleRate2;
		U8 bitRate1;
		U8 bitRate2;
		U8 bitRate3;
	}AACCodecSettings;

	typedef struct
	{
		U8 content[2+4+17]; //Media Transport + Content Protection + Media Codec
	}CELTServiceCapabilities;

	typedef struct
	{
		U8 content[13];
	}CELTCodecSettings;

	typedef struct A2DP_NVKEY_PARAMETER
	{
		U16 						defaultDelayReport;
		U8							a2dp_codec_support;
		SBCServiceCapabilities		sbcServiceCapabilities;
		SBCCodecSettings			sbcDefaultSettings;
		AACServiceCapabilities		aacServiceCapabilities;
		AACCodecSettings			aacDefaultSettings;
		CELTServiceCapabilities 	celtServiceCapabilities;
		CELTCodecSettings			celtDefaultSettings;
	}PACKED A2DP_NVKEY_PARAMETER;

#endif //__A2DP_NVKEY_PARAMETER_H__

