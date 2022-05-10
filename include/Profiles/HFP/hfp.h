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

#ifndef __HSP_HH_
#define __HSP_HH_

/**************************************************************************************************
* Define
**************************************************************************************************/

#if 0/*LOG_PRINT_156X*/
#define DBG_LOG_PROFILE_HFP(_message, arg_cnt, ...)   printf(_message, ##__VA_ARGS__)
#else
#define DBG_LOG_PROFILE_HFP(_message, arg_cnt, ...)   LOG_MSGID_I(LIB_HFP, _message, arg_cnt, ##__VA_ARGS__)
#define STRING_LOG_PROFILE_HFP(_message, arg...)      LOG_I(LIB_HFP, _message, ##arg)
#endif

#define EOF	0xFF

#undef CIEV_IND_DEF
#define CIEV_IND_DEF(NAME, MASK)	HFP_CIEVIND_##NAME,
enum
{
	#include "Hfp_CievIndicator.h"
	HFP_CIEVIND_NO,
};

#undef CIEV_IND_DEF
#define CIEV_IND_DEF(NAME, MASK)	HFP_CINDIND_##MASK##_MASK = 0x01L << (HFP_CIEVIND_##NAME),
typedef enum
{
	#include "Hfp_CievIndicator.h"
}hfpCINDMask;

//CINDMask
//#define HFP_CINDIND_SHOWALL_MASK				0x02	//accept all indicators
//#define HFP_CINDIND_SHOWEXT_MASK				0x04	//accept unknown indicators
//#define HFP_CINDIND_CUSTOM_CODE_MASK			0x08	//make copy to custom code

#define MAX_CIEVIND_NO	20 ////The Hands Free Profile specification limits the number of indicators returned by the AG to a maximum of 20.

#define HFP_NVRAM_FEAT_DISABLE_AG_ECNR			0x0001
#define HFP_NVRAM_FEAT_IPHONE_BATTERY_DISPLAY	0x0002
#define HFP_NVRAM_FEAT_IPHONE_SIRI				0x0004
#define HFP_NVRAM_FEAT_IPHONE_DOCK				0x0008
#define HFP_NVRAM_FEAT_XEVENT_BATTERY_DISPLAY	0x0010

#define HF_BRSF_FEAT_ECNR									0x0001
#define HF_BRSF_FEAT_3WAYCALLING							0x0002
#define HF_BRSF_FEAT_CLI_PRESENTATION						0x0004
#define HF_BRSF_FEAT_VOICE_RECOGNITION						0x0008
#define HF_BRSF_FEAT_REMOTE_VOLUME_CONTROL					0x0010
#define HF_BRSF_FEAT_ENHANCED_CALL_STATUS					0x0020
#define HF_BRSF_FEAT_ENHANCED_CALL_CONTROL					0x0040
#define HF_BRSF_FEAT_CODEC_NEGOTIATION						0x0080
#define HF_BRSF_FEAT_HF_INDICATORS							0x0100
#define HF_BRSF_FEAT_SCO_S4									0x0200


#define AG_BRSF_FEAT_3WAYCALLING 							0X0001
#define AG_BRSF_FEAT_ECNR 									0X0002
#define AG_BRSF_FEAT_VOICE_RECOGNITION 						0X0004
#define AG_BRSF_FEAT_INBAND_RINGTONE 						0X0008
#define AG_BRSF_FEAT_VOICE_TAG								0X0010
#define AG_BRSF_FEAT_REJECT_CALL							0X0020
#define AG_BRSF_FEAT_ENHCALL_STATUS							0X0040
#define AG_BRSF_FEAT_ENHCALL_CTRL							0X0080
#define AG_BRSF_FEAT_EXT_ERR_RESULT_CODE					0X0100
#define AG_BRSF_FEAT_CODEC_NEGOTIATION						0X0200
#define AG_BRSF_FEAT_HF_INDICATORS							0X0400
#define AG_BRSF_FEAT_SCO_S4									0x0200

/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct
{
	U8 ind;
	U8 val;
}HFP_IND_VALUE_STRU;

#undef CIEV_IND_DEF
#define CIEV_IND_DEF(NAME, MASK)	HFP_IND_VALUE_STRU NAME##Ctl;

typedef struct
{
	#include "Hfp_CievIndicator.h"
}HFP_IND_CTL_STRU;

typedef union
{
	HFP_IND_CTL_STRU 	totalInd;	//place holder, actual size is HFP_CIEVIND_NO
	HFP_IND_VALUE_STRU	standardInd[MAX_CIEVIND_NO];
}HFP_IND_TABLE_STRU;

/**************************************************************************************************
* Public functions
**************************************************************************************************/
BOOL hfp_IsSupportVendorDefATcmd(void);
U8 hfp_GetVendorATNum(void);

void Hfp_RegVendorCmd(U8 *HfpAtVendor, U8 *HfpVendorResultCode, U8 VendorATnum, U8 VendorATMAXLen, U8 VendorATResultNum, U8 VendorATResultMaxLen);
void Hfp_UnRegVendorCmd(void);

U8 *hfp_GetVendorAT(void);
U8 *hfp_GetVendorATResult(void);
U8 hfp_GetVendorATNum(void);
U8 hfp_GetVendorATMAXLen(void);
U8 hfp_GetVendorATResultNum(void);
U8 hfp_GetVendorATResultMAXLen(void);



Handler hfp_GetAppHandler(void);
void 	hfp_SetHandle(Handler app);

void 	hfp_SetHspSdpServiceHandle(U32 serviceHandle);
U32 	hfp_GetHspSdpServiceHandle(void);
void	hfp_SetHspServiceState(U8 serviceState);
U8 		hfp_GetHspServiceState(void);
void 	hfp_ClearHspServiceCtl(void);

void 	hfp_SetHfpSdpServiceHandle(U32 serviceHandle);
U32 	hfp_GetHfpSdpServiceHandle(void);
void 	hfp_SetHfpServiceState(U8 serviceState);
U8 		hfp_GetHfpServiceState(void);
void 	hfp_ClearHfpServiceCtl(void);

void	hfp_SetSupportFeatureToService();
void 	hfp_SetSupportFeature(U16 supportFeature);
U16 	hfp_GetSupportFeature(void);
BOOL 	hfp_IsHFFeatureSupport(U16 feature);

void 	hfp_SetMSBCSupport(BOOL isSupport);
BOOL 	hfp_IsMSBCSupported(void);

void 	hfp_SetCINDMask(U32 mask);
BOOL 	hfp_IsShowAllIndicators(void);
BOOL	hfp_IsShowUnknownIndicator(void);
BOOL 	hfp_IsShowIndicator(U8 localInd);

void hfp_SetSLCCmdTimeOut(U32 time);
void hfp_SetAtCmdTimeOut(U32 time);
void hfp_SetBatCmdTimeOut(U32 time);
U32 hfp_GetSLCCmdTimeOut(void);
U32 hfp_GetAtCmdTimeOut(void);
U32 hfp_GetBatCmdTimeOut(void);



BOOL 	hfp_IsNvramFeatureOn(U16 feature);
void 	HFP_Init(void);


#endif //__HSP_HH_
