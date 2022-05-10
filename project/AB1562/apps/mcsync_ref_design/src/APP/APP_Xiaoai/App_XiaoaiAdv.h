/* Copyright Statement:
 *
 * (C) 2020  Airoha Technology Corp. All rights reserved.
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


#ifndef _APP_XIAOAI_ADV_HH_
#define _APP_XIAOAI_ADV_HH_

#define ADV_MAX_LENGHT (31)
#define XIAOI_ADV_UPDATE_TIME 1000

/**************************************************************************************************
* Structure
**************************************************************************************************/

//xiaoai adv 1.6.8
//
#define LR_STATUS_BYTE 10
#define EARBUDS_STATUS_BTYE 11
#define L_BATTERY_BYTE 12
#define R_BATTERY_BYTE 13
#define CASE_BATTERY_BYTE 14

#define LAST_AG_LAP_LSB_BYTE 15
#define LAST_AG_LAP_02_BYTE 16
#define LAST_AG_LAP_MSB_BYTE 17

typedef struct
{
	U8 isThisDevL:1;
	U8 minorID:2;
	U8 rsv:1;
	U8 isLDiscoverable:1;
	U8 isRDiscoverable:1;
	U8 isLConnectable:1;
	U8 isRConnectable:1;
}XIAOAI_LR_STATUS_STRU;

typedef struct
{
	U8 rsv:1;
	U8 isCaseLidOpen:1;
	U8 isEDRConnected:1;
	U8 isEDRPaired:1;
	U8 isMACEncrypted:1;//0
	U8 isLROutOfCase:1;
	U8 isTWSConnected:1;
	U8 isTWSConnecting:1;
}XIAOAI_EARBUDS_STATUS_STRU;

typedef struct
{
	U8 batInPercent:7;
	U8 isCharging:1;
}
XIAOAI_BATTERY_STATUS_STRU;

#define RIGHT_CONNECTABLE 0x80  //offset: 10
#define LEFT_CONNECTABLE 0x40
#define RIGHT_DISCOVERABLE 0x20
#define LEFT_DISCOVERABLE 0x10
#define Subld_mac_device 0x02
#define LEFT_IS_ADVERTISER 0x01

typedef enum {
    SMART_CASE_OPENED = 0x02,
    BT_ALREADY_CONNECTED = 0x04,
    BT_ALREADY_PAIRED = 0x08,
    PRIVATE_MAC_ADDR = 0x10,
    EARBUDS_OUT_OF_BOX = 0x20,
    APP_TWS_CONNECTED = 0x40,
    APP_TWS_CONNECTING = 0x80,
}HW_STATUS_TYPE;

typedef enum{
    XIAOAI_ADV_UPDATE_TIMEOUT_IND = 0x001c,
}xiaoai_adv_ind_e;

BOOL APP_XiaoaiEnableAdv(void);
BOOL APP_XiaoaiDisableAdv(void);
void APP_XiaoaiInitAdv(void);
BOOL APP_Xiaoai_IsAdvEnable(void);
void APP_XiaoaiResetAdv(void);


#endif
