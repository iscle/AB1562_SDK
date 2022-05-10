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

#ifndef _APP_BLE_H_
#define _APP_BLE_H_

#define BLE_ADV_PRIMARY_MASK    (1<<BLE_ADV_PRIMARY)
#define BLE_ADV_GFP_MASK        (1<<BLE_ADV_GFP)
#define BLE_ADV_AMA_MASK        (1<<BLE_ADV_AMA)
#define BLE_ADV_XIAOAI_MASK     (1<<BLE_ADV_XIAOAI)
#define BLE_ADV_TILE_MASK       (1<<BLE_ADV_TILE)
#define BLE_ADV_SWIFT_MASK      (1<<BLE_ADV_SWIFT)
#define BLE_ADV_ALL             (0xFF)
typedef struct stAppBleAdvDesc APP_BLE_ADV_HANDLE_DESC;
typedef struct stAppBleAdvNode APP_BLE_ADV_HANDLE_NODE;

typedef BOOL (*APP_BLE_GATT_SERVER_READY)(void);
typedef BOOL (*APP_BLE_ADV_ENABLE)(void);
typedef BOOL (*APP_BLE_ADV_DISABLE)(void);

struct stAppBleAdvDesc
{
	U8 advHandleMask;
    APP_BLE_GATT_SERVER_READY appBleGattServerReady;
	APP_BLE_ADV_ENABLE  appBleAdvEnable;
	APP_BLE_ADV_DISABLE	appBleAdvDisable;
};

struct stAppBleAdvNode
{
	APP_BLE_ADV_HANDLE_DESC const *appBleAdvDesc;
	APP_BLE_ADV_HANDLE_NODE *nextNode;
};

void APP_Ble_Init(void);
void APP_Ble_StartGattService(void);
BOOL APP_Ble_IsGattServerReady(void);
BOOL APP_Ble_Disconnect(BD_ADDR_T *pBdAddr);
BOOL APP_Ble_EnableAdv(U8 advHandleMask);
BOOL APP_Ble_DisableAdv(U8 advHandleMask);
void APP_Ble_RegisterAdvHandle(APP_BLE_ADV_HANDLE_NODE *pAdvHandleNode);
U8 APP_Ble_GetAdvMask(void);

#endif

