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

#ifndef BT_HH
#define BT_HH
#include <string.h>
#include "message.h"
#include <sink_.h>
#include "bt_common.h"
#include "bt_gap.h"
#include "bt_l2cap.h"
#include "bt_sdp.h"
#include "bt_rfcomm.h"
#include "bt_gatt.h"
#include "bt_avdtp.h"
#include "bt_race_command.h"
#include "FW_Api.h"
#include "log.h"
#include "feature_config.h"
#include "bt_stream.h"


/**
 * @addtogroup BT_LIB
   @{
       This section introduces the BT Library APIs including terms and acronyms,
       supported features, software architecture, details on how to use the Message, enums, structures, typedefs and functions.

       \image html bt_top_arch_pro.png "Figure : AB1530 bluetooth API architecure (Libraries)"

       @defgroup BT_LIB_COMMON COMMON
       @{
            @defgroup BT_LIB_COMMON_MACRO Macros
            @defgroup BT_LIB_COMMON_DS Data Structures
            @defgroup BT_LIB_COMMON_FN Functions
       @}

       @defgroup BT_LIB_GAP GAP
       @{
            @defgroup BT_LIB_GAP_MACRO Macros
            @defgroup BT_LIB_GAP_DS Data Structures
            @defgroup BT_LIB_GAP_FN Functions
       @}

       @defgroup BT_LIB_L2CAP L2CAP
       @{
            @defgroup BT_LIB_L2CAP_MACRO Macros
            @defgroup BT_LIB_L2CAP_DS Data Structures
            @defgroup BT_LIB_L2CAP_FN Functions
       @}

       @defgroup BT_LIB_SDP SDP
       @{
            @defgroup BT_LIB_SDP_MACRO Macros
            @defgroup BT_LIB_SDP_DS Data Structures
            @defgroup BT_LIB_SDP_FN Functions
       @}
       @defgroup BT_LIB_RFCOMM RFCOMM
       @{
            @defgroup BT_LIB_RFCOMM_MACRO Macros
            @defgroup BT_LIB_RFCOMM_DS Data Structures
            @defgroup BT_LIB_RFCOMM_FN Functions
       @}
       @defgroup BT_LIB_AVDTP AVDTP
       @{
            @defgroup BT_LIB_AVDTP_MACRO Macros
            @defgroup BT_LIB_AVDTP_DS Data Structures
            @defgroup BT_LIB_AVDTP_FN Functions
       @}
       @defgroup BT_LIB_AVCTP AVCTP
       @{
            @defgroup BT_LIB_AVCTP_MACRO Macros
            @defgroup BT_LIB_AVCTP_DS Data Structures
            @defgroup BT_LIB_AVCTP_FN Functions
       @}
       @defgroup BT_LIB_GATT GATT
       @{
            @defgroup BT_LIB_GATT_MACRO Macros
            @defgroup BT_LIB_GATT_DS Data Structures
            @defgroup BT_LIB_GATT_FN Functions
       @}

   @}
 */

/**************************************************************************************************
*Specification Defines
**************************************************************************************************/
#define STDINCLUDE	1

/**************************************************************************************************
* Constant Defines
**************************************************************************************************/


/**************************************************************************************************
* Type Defines
**************************************************************************************************/


#endif

