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
 
 
#ifndef APP_MCSYNC_RHO_NVKEYCONFIG_H
#define APP_MCSYNC_RHO_NVKEYCONFIG_H

#include "nvkey.h"
#include "nvkey_list.h"

/*************************************************************************************************
* Define
**************************************************************************************************/
#define APP_RHO_TRIGGER_FEATURE_BAT			0x0001
#define APP_RHO_TRIGGER_FEATURE_RSSI		0x0002
#define APP_RHO_TRIGGER_FEATURE_POWEROFF	0x0004
#define APP_RHO_TRIGGER_FEATURE_CHARGECASE	0x0008	

/*************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
	APP_RHO_TRIGGER_BAT,	//Battery
	APP_RHO_TRIGGER_RSSI,	//RSSI
	APP_RHO_TRIGGER_NUM,
	
	RESERVED0 = APP_RHO_TRIGGER_NUM,
	RESERVED1,
	RESERVED2,
	RESERVED3,
	RESERVED4,
	RESERVED5,
	RESERVED6,
	RESERVED7,
	
	APP_RHO_TRIGGER_TOTAL_NUM
};

enum
{
	RHO_TIMER_ID_MONITOR_PARA,
};

/*************************************************************************************************
* Structure
**************************************************************************************************/
/**
 *@KeyID 0xF2B1
*/

typedef struct
{
	U16	rhoPercent; 	//range: 10~1000
	U16	rhoGapPercent;	//range: 10~1000
}PACKED APP_MCSYNC_RHO_BAT_STRU;

typedef struct
{
	S8	rhoRssi;		//range: -90~0
	U8	rhoGapRssi;		//range: 0~90
}PACKED APP_MCSYNC_RHO_RSSI_STRU;

/*NvkeyDefine NVKEYID_APP_MCSYNC_RHO_CONFIG*/
typedef struct
{
	U16 triggerFeature;
	U8 	syncPartnerTimer;						//unit: second
	U8	priority[APP_RHO_TRIGGER_TOTAL_NUM];	//the bigger, the higher, start from 1, all different
	APP_MCSYNC_RHO_BAT_STRU batCtl;
	APP_MCSYNC_RHO_RSSI_STRU rssiCtl;
}PACKED APP_MCSYNC_RHO_CONFIG_STRU;

#endif // APP_MCSYNC_RHO_NVKEYCONFIG_H
