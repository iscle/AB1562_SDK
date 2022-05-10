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
#include "App_Nvram_Type.h"
#include "NvramType_Nvkey.h"

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static void app_NvramType_ReadAppFeatureConfig(void);

/**************************************************************************************************
* Variable
**************************************************************************************************/
APP_NVRAM_TYPE_STRU gAppNvramType;//gMMI_nvram

/********************************************************************************************************
* Static function
********************************************************************************************************/
static void app_NvramType_ReadAppFeatureConfig(void)
{
	U8* pAddr;
	U8 dataSize;
		
	pAddr = (U8*)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_NVRAM_APP_FEATURE);
	dataSize = NVKEY_GetPayloadLength(NVKEYID_APP_NVRAM_APP_FEATURE);
	
	if(pAddr == NULL || dataSize <= 0)
	{
		FW_Assert(FALSE);
	}
	else
	{
		gAppNvramType.pAppFeatureCtl = (APP_FEATURE_CTL_STRU *)pAddr;
	}

	return ;
}

/**************************************************************************************************
* Public Function
**************************************************************************************************/
void APP_NvramType_Init(void)
{
	app_NvramType_ReadAppFeatureConfig();
}

U16 APP_NvramType_FeturCtl_GetAppFeatureControl(U8 feature)
{
	switch(feature)
	{
		case system:
			return gAppNvramType.pAppFeatureCtl->system;
		case audio1:
			return gAppNvramType.pAppFeatureCtl->audio1;
		case audio2:
			return gAppNvramType.pAppFeatureCtl->audio2;
		case audio3:
			return gAppNvramType.pAppFeatureCtl->audio3;
		case led:
			return gAppNvramType.pAppFeatureCtl->led;
		case mode:
			return gAppNvramType.pAppFeatureCtl->mode;
		case HSP:
			return gAppNvramType.pAppFeatureCtl->HSP;
		case connection:
			return gAppNvramType.pAppFeatureCtl->connection;
		case lineInFeature:
			return gAppNvramType.pAppFeatureCtl->lineInFeature;
		case FMFeature:
			return gAppNvramType.pAppFeatureCtl->FMFeature;
		case A2DP:
			return gAppNvramType.pAppFeatureCtl->A2DP;
		case AVRCP:
			return gAppNvramType.pAppFeatureCtl->AVRCP;
		case battery:
			return gAppNvramType.pAppFeatureCtl->battery;
		case customerUART:
			return gAppNvramType.pAppFeatureCtl->customerUART;
		case keys:
			return gAppNvramType.pAppFeatureCtl->keys;
		case airFeature:
			return gAppNvramType.pAppFeatureCtl->airFeature;
		default:
			FW_Assert(FALSE);
			break;
	}
	return 0;
}
