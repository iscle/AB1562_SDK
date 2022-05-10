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
#include "App_AEKey.h"
#include "App_EventOpcode.h"

/**************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
	APP_AE_FUN1,
	APP_AE_FUN2,
	APP_AE_FUN3,
	APP_AE_FUN4,
	APP_AE_FUN5,
	APP_AE_FUN6,
	APP_AE_FUN7,
	APP_AE_FUN8,
	APP_AE_FUN9,
	APP_AE_FUN10,
	
	APP_AE_FUN_TOTAL_NO
};

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static void app_AE_Fun1(BD_ADDR_T *pBdAddr);
static void app_AE_Fun2(BD_ADDR_T *pBdAddr);
static void app_AE_Fun3(BD_ADDR_T *pBdAddr);
static void app_AE_Fun4(BD_ADDR_T *pBdAddr);
static void app_AE_Fun5(BD_ADDR_T *pBdAddr);
static void app_AE_Fun6(BD_ADDR_T *pBdAddr);
static void app_AE_Fun7(BD_ADDR_T *pBdAddr);
static void app_AE_Fun8(BD_ADDR_T *pBdAddr);
static void app_AE_Fun9(BD_ADDR_T *pBdAddr);
static void app_AE_Fun10(BD_ADDR_T *pBdAddr);

/**************************************************************************************************
* Variables
**************************************************************************************************/
static APP_KEY_EVT_STRU const APP_AE_KeyEvtHandler[APP_AE_FUN_TOTAL_NO] = 
{
	{app_AE_Fun1},
	{app_AE_Fun2},
	{app_AE_Fun3}, 	   				
	{app_AE_Fun4},			  
	{app_AE_Fun5},
	{app_AE_Fun6},
	{app_AE_Fun7},
	{app_AE_Fun8}, 	   				
	{app_AE_Fun9},			  
	{app_AE_Fun10},
};

/**************************************************************************************************
* Static Functions
**************************************************************************************************/
static void app_AE_Fun1(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	
	return;
}

static void app_AE_Fun2(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	
	return;
}

static void app_AE_Fun3(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	
	return;
}

static void app_AE_Fun4(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	
	return;
}

static void app_AE_Fun5(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	
	return;
}

static void app_AE_Fun6(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	
	return;
}

static void app_AE_Fun7(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	
	return;
}

static void app_AE_Fun8(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	
	return;
}

static void app_AE_Fun9(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	
	return;
}

static void app_AE_Fun10(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	
	return;
}

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void APP_AE_ProcessKeyEvent(BD_ADDR_T *pBdAddr, U8 index)
{
	if(APP_AE_KeyEvtHandler[index].fKeyEvt)
	{
		APP_AE_KeyEvtHandler[index].fKeyEvt(pBdAddr);
	}
}
