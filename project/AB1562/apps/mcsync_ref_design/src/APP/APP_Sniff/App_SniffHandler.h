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
 
 
#ifndef __APP_SNIFF_HANDLER_H__
#define __APP_SNIFF_HANDLER_H__

#include "bt.h"
#include "App_ResourceManager.h"


/****************************************************************************************************
* Definition
*****************************************************************************************************/
#define APP_SNIFF_HFP						0
#define APP_SNIFF_A2DP_MUSIC				3
#define APP_SNIFF_ACL_READY_SLC_NOT_YET		4
#define APP_SNIFF_SDP						5
#define APP_SNIFF_DISC						6
#define APP_SNIFF_SPP						7
#define APP_SNIFF_MCSYNC					8
#define APP_FORCE_UNSNIFF					9
#define APP_UNSNIFF_VPRT_SYNC				10
#define APP_SNIFF_KEY						11
#define APP_SNIFF_SCO						12
#define APP_SNIFF_PARTNER_KEY               13
#define APP_SNIFF_ANC_IF_PKT               	14

/****************************************************************************************************
* Enumeration
*****************************************************************************************************/
enum
{
	APP_SNIFF_TYPE_LINK,
	APP_SNIFF_TYPE_SYSTEM,
	APP_SNIFF_TYPE_UNKNOWN,
};

/****************************************************************************************************
* Public functions
*****************************************************************************************************/
void App_SniffEnable(BD_ADDR_T *pBdAddr, U32 sniffReason);
void App_SniffDisable(BD_ADDR_T *pBdAddr, U32 sniffReason);
void App_SniffCheck(BD_ADDR_T *pBdAddr);

#endif //__APP_SNIFF_HANDLER_H__
