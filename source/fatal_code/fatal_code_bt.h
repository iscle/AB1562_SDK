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
#ifndef __FATAL_CODE_BT_H__
#define __FATAL_CODE_BT_H__

#include "fatal_code.h"

/**
 * @addtogroup Exception_Code Exception
 * @{
 * @addtogroup Fatal_Code Fatal Error Code Enums
 * @{
 * @addtogroup BT_Fatal_Err BT
 * @{
 */
/** @brief
 * This enum defines the BT fatal error code
 */
typedef enum{
    FATAL_ERROR_BT_DISCONNECTION = FATAL_ERROR_BT_START, /**< 0x201: BT disconnection error code. Please refer to #bt_disconnection_sub_err_code_type_t.*/
    FATAL_ERROR_BT_XXXXXXXXXXXXX,                        /**< 0x202: xxxxxxxxxxxxxx. Please refer to #xxxxxxxx if w/ sub-err code..*/
    FATAL_ERROR_BT_YYYYYYYYYYYYY,                        /**< 0x203: xxxxxxxxxxxxxx. Please refer to #xxxxxxxx if w/ sub-err code..*/
    FATAL_ERROR_BT_ZZZZZZZZZZZZZ,                        /**< 0x204: xxxxxxxxxxxxxx. Please refer to #xxxxxxxx if w/ sub-err code..*/
    FATAL_ERROR_BT_MAX           = FATAL_ERROR_BT_END,   /**< 0x400: BT error code end. */
}fatal_code_bt_type_t;

/** @brief
 * This enum defines the sub-error code for FATAL_ERROR_BT_DISCONNECTION
 */
typedef enum{
    BT_DISCONNECTION_LONG_DISTANCE = 0x0, /**< 0x0: BT disconnection because of long-distance. */
    BT_DISCONNECTION_IN_SHIELDING,        /**< 0x1: BT disconnection because of in shielding. */
    BT_DISCONNECTION_LOW_BATTERY,         /**< 0x2: BT disconnection because of low battery. */
    BT_DISCONNECTION_IN_BOX,              /**< 0x3: BT disconnection because of in-box. */
}bt_disconnection_sub_err_code_type_t;

/** @brief
 * This enum defines the sub-error code for FATAL_ERROR_BT_XXXXXXXXXXXXX
 */
typedef enum{
    BT_XXXXXXXXXXXXX_CASE_A = 0x0, /**< 0x0: xxxxxxxxxxxxxxxxxx. */
    BT_XXXXXXXXXXXXX_CASE_B,       /**< 0x1: xxxxxxxxxxxxxxxxxx. */
    BT_XXXXXXXXXXXXX_CASE_C,       /**< 0x2: xxxxxxxxxxxxxxxxxx. */
    BT_XXXXXXXXXXXXX_CASE_D,       /**< 0x3: xxxxxxxxxxxxxxxxxx. */
}bt_xxxxxxxxxxxxx_sub_err_code_type_t;

/**
  * @}
  * @}
  * @}
  */
#endif /*__FATAL_CODE_BT_H__*/