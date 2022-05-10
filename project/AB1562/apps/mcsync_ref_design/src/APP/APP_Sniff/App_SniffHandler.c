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
#include "App_A2dp.h"
#include "App_Avrcp.h"
#include "App_ResourceManager.h"
#include "App_System.h"
#include "App_State.h"
#include "A2dp_Interface.h"
#include "PM_Interface.h"
#include "App_PowerOff.h"
#include "App_SniffHandler.h"
#ifdef SPECIAL_WHITE_LIST_HANDLE
#include "BtSdps.h"
#endif

#define DBG_LOG_APP_SNIFF(_message, arg_cnt, ...)  LOG_MSGID_I(APP_SNIFF, _message, arg_cnt, ##__VA_ARGS__)


log_create_module(APP_SNIFF, PRINT_LEVEL_INFO);

/****************************************************************************************************
* Static function
*****************************************************************************************************/
static BOOL app_IsSniffAllowed(APP_INFO_STRU *pAppLinkInfo);
static BOOL app_IsExitSniffAllowed(APP_INFO_STRU *pAppLinkInfo);

/****************************************************************************************************
* Variable
*****************************************************************************************************/
static U32 gAppSniffSystemMask;

/****************************************************************************************************
* Static function
*****************************************************************************************************/
static BOOL app_IsSniffAllowed(APP_INFO_STRU *pAppLinkInfo)
{
	if(pAppLinkInfo->sniffProfileMask || gAppSniffSystemMask)
		return FALSE;

	return TRUE;
}

static BOOL app_IsExitSniffAllowed(APP_INFO_STRU *pAppLinkInfo)
{
	if(!pAppLinkInfo->sniffProfileMask && !gAppSniffSystemMask)
		return FALSE;

	return TRUE;
}

static U8 app_GetSniffMaskType(U32 sniffReason)
{
	switch(sniffReason)
	{
		case APP_SNIFF_HFP:
		case APP_SNIFF_A2DP_MUSIC:
		case APP_SNIFF_ACL_READY_SLC_NOT_YET:
		case APP_SNIFF_SDP:
		case APP_SNIFF_DISC:
		case APP_SNIFF_SPP:
		case APP_SNIFF_MCSYNC:
        case APP_SNIFF_SCO:
			return APP_SNIFF_TYPE_LINK;

		case APP_FORCE_UNSNIFF:
		case APP_UNSNIFF_VPRT_SYNC:
		case APP_SNIFF_KEY:
        case APP_SNIFF_PARTNER_KEY:
			return APP_SNIFF_TYPE_SYSTEM;

		default:
			return APP_SNIFF_TYPE_UNKNOWN;
	}
}

/****************************************************************************************************
* Public function
*****************************************************************************************************/
void App_SniffDisable(BD_ADDR_T *pBdAddr, U32 sniffReason)
{
    U8 type = app_GetSniffMaskType(sniffReason);
	APP_INFO_STRU *pAppLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);
    
    DBG_LOG_APP_SNIFF( "[APP_SNIFF] APP Exit sniff: reason:%d, Mask:0x%x, 0x%x", 3, sniffReason, pAppLinkInfo->sniffProfileMask, gAppSniffSystemMask);

#ifdef SPECIAL_WHITE_LIST_HANDLE
        if(sniffReason == APP_SNIFF_SCO && bt_sdps_pnp_check_aqua(pBdAddr))
        {
            /* AQUA need to enter sniff */
            return;
        }
#endif

    if(APP_SNIFF_TYPE_SYSTEM == type)
    {
        gAppSniffSystemMask |= (U32_BIT_MASK(sniffReason));
    }
        
	if(pAppLinkInfo)
	{
		if(APP_SNIFF_TYPE_LINK == type)
			pAppLinkInfo->sniffProfileMask |= (U32_BIT_MASK(sniffReason));

		DBG_LOG_APP_SNIFF( "[APP_SNIFF] APP Exit sniff mode: BdAddr:0x%x%x, reason:%d, Mask:0x%x, 0x%x", 5,
			FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE),
			sniffReason, pAppLinkInfo->sniffProfileMask, gAppSniffSystemMask);

		if(app_IsExitSniffAllowed(pAppLinkInfo))
		{
			PM_Sniff_Disable(pBdAddr, PM_SNIFF_APP_LAYER);

			if(sniffReason == APP_SNIFF_MCSYNC)
				PM_Sniff_Disable(pBdAddr, PM_SNIFF_RHO);
		}
	}
}

void App_SniffEnable(BD_ADDR_T *pBdAddr, U32 sniffReason)
{
    U8 type = app_GetSniffMaskType(sniffReason);
	APP_INFO_STRU *pAppLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

    DBG_LOG_APP_SNIFF( "[APP_SNIFF] APP Enter sniff: reason:%d, Mask:0x%x, 0x%x", 3, sniffReason, pAppLinkInfo->sniffProfileMask, gAppSniffSystemMask);

    if(APP_SNIFF_TYPE_SYSTEM == type)
        gAppSniffSystemMask &= (~(U32_BIT_MASK(sniffReason)));
        
	if(pAppLinkInfo)
	{
		DBG_LOG_APP_SNIFF( "[APP_SNIFF] APP Enter sniff mode: BdAddr:0x%x%x, reason:%d, Mask:0x%x, 0x%x", 5,
			FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE),
			sniffReason, pAppLinkInfo->sniffProfileMask, gAppSniffSystemMask);

		if(APP_SNIFF_TYPE_LINK == type)
			pAppLinkInfo->sniffProfileMask &= (~(U32_BIT_MASK(sniffReason)));

		if(app_IsSniffAllowed(pAppLinkInfo))
		{
			PM_Sniff_Enable(pBdAddr, PM_SNIFF_APP_LAYER);
		}

		if(sniffReason == APP_SNIFF_MCSYNC)
			PM_Sniff_Enable(pBdAddr, PM_SNIFF_RHO);
	}
}

void App_SniffCheck(BD_ADDR_T *pBdAddr)
{
	APP_INFO_STRU *pAppLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	if(pAppLinkInfo)
	{
		DBG_LOG_APP_SNIFF( "[APP_SNIFF] APP Check sniff: BdAddr:0x%x%x, LinkMask:0x%x, systemMask:0x%x", 4,
			FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE),
			pAppLinkInfo->sniffProfileMask, gAppSniffSystemMask);

		if(pAppLinkInfo->sniffProfileMask || gAppSniffSystemMask)
			PM_Sniff_Disable(pBdAddr, PM_SNIFF_APP_LAYER);
		else
			PM_Sniff_Enable(pBdAddr, PM_SNIFF_APP_LAYER);
	}
}
