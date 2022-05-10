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
 
#include "bt.h"
#include "App_MCSync_NvkeyHandler.h"

/*************************************************************************************************
* Function prototype
**************************************************************************************************/
static BOOL app_mcsync_NvkeyConfigInit(void);
static BOOL app_mcsync_NvkeyIACInit(void);

/*************************************************************************************************
* Variable
**************************************************************************************************/
static APP_AWSMCE_CONFIG_STRU 	 			*gAppMCSyncNvKeyConfig;
static APP_AWSMCE_IAC_VALUE_STRU 			*gAppMCSyncNvKeyIAC;
/*************************************************************************************************
* Static function
**************************************************************************************************/
static BOOL app_mcsync_NvkeyConfigInit(void)
{
	return (NVKEY_GetPayloadLength(NVKEYID_APP_MCSYNC_CONFIG) == sizeof(APP_AWSMCE_CONFIG_STRU)) ? TRUE : FALSE;
}

static BOOL app_mcsync_NvkeyIACInit(void)
{
	return (NVKEY_GetPayloadLength(NVKEYID_APP_IAC_VALUE) == sizeof(APP_AWSMCE_IAC_VALUE_STRU)) ? TRUE : FALSE;
}

/*************************************************************************************************
* Public function
**************************************************************************************************/
U16 APP_MCSync_Nvkey_GetSharePairingTime(void)
{
	return (gAppMCSyncNvKeyConfig)? gAppMCSyncNvKeyConfig->earbudsPairingTimer : 0;
}

U16 APP_MCSync_Nvkey_GetAirPairingTime(void)
{
	return (gAppMCSyncNvKeyConfig)? gAppMCSyncNvKeyConfig->awsStateTimer:0;
}

S8 APP_MCSync_Nvkey_RSSIThreshold(void)
{
	return (gAppMCSyncNvKeyConfig)? gAppMCSyncNvKeyConfig->awsRSSIThreshold : 0x7F;
}

U8 APP_MCSync_Nvkey_InquiryTime(void)
{
	return (gAppMCSyncNvKeyConfig)? gAppMCSyncNvKeyConfig->awsInquiryTime:0;
}

U8 APP_MCSync_Nvkey_InquiryDevNum(void)
{
	return  (gAppMCSyncNvKeyConfig)? gAppMCSyncNvKeyConfig->awsInquiryDevNum:0;
}

U32 APP_MCSync_Nvkey_GetLIAC(void)
{
	return (gAppMCSyncNvKeyIAC) ? gAppMCSyncNvKeyIAC->liacValue : 0x9E8B3F;
}

U32 APP_MCSync_Nvkey_GetSpecialLIAC(void)
{
	return (gAppMCSyncNvKeyIAC) ? gAppMCSyncNvKeyIAC->specialLIACValue : 0x9E8B3F;
}

U32 APP_MCSync_Nvkey_GetInquiryLIAC(void)
{
	return (gAppMCSyncNvKeyIAC) ? gAppMCSyncNvKeyIAC->inquiryLIACValue : 0x9E8B3F;
}

U32 APP_MCSync_Nvkey_GetSharePairingLIAC(void)
{
	return (gAppMCSyncNvKeyIAC) ? gAppMCSyncNvKeyIAC->inquiryEarbudsPlusLIACValue : 0x9E8B3F;
}

U8   APP_MCSync_Nvkey_SharePairingInquiryTime(void)
{
	return (gAppMCSyncNvKeyConfig)? gAppMCSyncNvKeyConfig->earbudsPairingInquiryTime:0;
}

U8   APP_MCSync_Nvkey_SharePairingInquiryDevNum(void)
{
	return (gAppMCSyncNvKeyConfig)? gAppMCSyncNvKeyConfig->earbudsPairingInquiryDevNum:0;
}

S8 APP_MCSync_Nvkey_SharePairingRssiThreshold(void)
{
	return (gAppMCSyncNvKeyConfig)? gAppMCSyncNvKeyConfig->earbudsPairingRSSIThreshold:0;
}

BOOL APP_MCSync_Nvkey_Init(void)
{
	if(app_mcsync_NvkeyConfigInit() && app_mcsync_NvkeyIACInit())
	{
		gAppMCSyncNvKeyConfig 			= (APP_AWSMCE_CONFIG_STRU*)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_MCSYNC_CONFIG);
		gAppMCSyncNvKeyIAC 		= (APP_AWSMCE_IAC_VALUE_STRU*)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_IAC_VALUE);
		//gpAppAwsmcePreferLIACValue 	= (APP_AWSMCE_PREFER_LIAC_VALUE_STRU*)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_PREFFER_LIAC_VALUE);

		if(gAppMCSyncNvKeyConfig && gAppMCSyncNvKeyIAC)
		{
			return TRUE;
		}
	}
	return FALSE; 
}
