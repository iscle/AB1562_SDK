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
 

#ifndef _AIRAPP_H
#define _AIRAPP_H


#include "bt.h"

/**************************************************************************************************
* Macro
**************************************************************************************************/
#define MSG_MESSAGE_BT_PROFILE_AIRAPP MSG_MESSAGE_BT_PROFILE_BASE+0x0200  //possible the same value with other profile


/**************************************************************************************************
* Type define
**************************************************************************************************/
typedef enum
{
    AIRAPP_INIT_CFM = MSG_MESSAGE_BT_PROFILE_AIRAPP,
    AIRAPP_CONNECT_IND,
    AIRAPP_SPP_CONN_CFM,
    AIRAPP_DISCONNECT_IND,
    AIRAPP_DISCONNECT_CFM,

    // For Google Fast Pair Service 2.0
    GFPSV2_BLE_DISABLE_ADV_IND,
    GFPSV2_BLE_SET_RANDOM_ADDR_IND,
    GFPSV2_BLE_UPDATE_IND,
} AIRAPP_MSG_E;


typedef enum
{
    AIRAPP_SUCCESS,
    AIRAPP_SPP_FAIL,
    AIRAPP_BLE_FAIL,
    AIRAPP_ALREADY_INITED,
}AIRAPP_INIT_STATUS;


typedef enum
{
    AIRAPP_INIT_SPP_BLE,
    AIRAPP_INIT_SPP,
    AIRAPP_INIT_BLE
}AIRAPP_INIT_OPTION;


typedef enum
{
    AIRAPP_SPP,
    AIRAPP_BLE
}AIRAPP_BEARER;


typedef enum
{
    AIRAPP_DISCON_SUCCESS,
    AIRAPP_DISCON_ALREADY_DISCONN
}AIRAPP_DISCONN_CFM_STATUS;


typedef enum
{
    AIRAPP_CONNECT_SUCCESS,
    AIRAPP_CONNECT_ALREADY_CONNECT,
    AIRAPP_CONNECT_FAIL
}AIRAPP_CONNECT_CFM_STATUS;


typedef struct
{
    AIRAPP_INIT_STATUS status;
}AIRAPP_INIT_CFM_T;


typedef struct
{
    AIRAPP_BEARER bearer;
    BD_ADDR_T bdAddr;
}AIRAPP_CONNECT_IND_T;


typedef struct
{
    AIRAPP_BEARER bearer;
    BD_ADDR_T bdAddr;
}AIRAPP_DISCONNECT_IND_T;


typedef struct
{
    AIRAPP_DISCONN_CFM_STATUS status;
    AIRAPP_BEARER bearer;
    BD_ADDR_T bdAddr;
}AIRAPP_DISCONNECT_CFM_T;


typedef struct
{
    AIRAPP_CONNECT_CFM_STATUS status;
    BD_ADDR_T bdAddr;
}AIRAPP_SPP_CONN_CFM_T;


typedef struct
{
    U8 battery:1;
    U8 rsvd:7;
}AIRAPP_NOTIFICATION_TYPE_T;


/**************************************************************************************************
* API
**************************************************************************************************/
VOID AirApp_Init(Handler app, AIRAPP_INIT_OPTION option);
VOID AirApp_Spp_Conn(BD_ADDR_T* pBdaddr);
VOID AirApp_Spp_Disconn(BD_ADDR_T* pBdaddr);
VOID AirApp_RegisterNotification(AIRAPP_NOTIFICATION_TYPE_T type);
BOOL AirApp_IsFOTASuccess(void);

#endif

