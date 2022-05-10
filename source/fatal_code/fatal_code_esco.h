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

#ifndef __FATAL_CODE_ESCO_H__
#define __FATAL_CODE_ESCO_H__

#include "fatal_code.h"

/**
 * @addtogroup Exception_Code Exception
 * @{
 * @addtogroup Fatal_Code Fatal Error Code Enums
 * @{
 * @addtogroup ESCO_Fatal_Err ESCO
 * @{
 */


/** @brief
 * This enum defines the ESCO fatal error code
 */
typedef enum {
    FATAL_ERROR_ESCO_SILENCE = FATAL_ERROR_ESCO_START,  /**< 0x801: ESCO silence error code. Please refer to #esco_silence_sub_err_code_type_t. */
    FATAL_ERROR_ESCO_NOISE,                             /**< 0x802: ESCO noise error code. Please refer to #esco_noise_sub_err_code_type_t. */
    FATAL_ERROR_ESCO_ABNORMAL_CONTROL,                  /**< 0x803: ESCO control abnormal. Please refer to #esco_control_sub_err_code_type_t. */
    FATAL_ERROR_ESCO_MAX  = FATAL_ERROR_ESCO_END,       /**< 0xA00: ESCO error code end. */
} fatal_code_esco_type_t;

/** @brief
 * This enum defines the sub-error code for FATAL_ERROR_ESCO_SILENCE
 */
typedef enum {
    ESCO_SILENCE_DL_TRANSFORM_NONE = 0x0,               /**< 0x0: ESCO silence due to Rx transform is not established. */
    ESCO_SILENCE_DL_VOLUME_IS_ZERO,                     /**< 0x1: ESCO silence due to afe DL volume is zero. */
    ESCO_SILENCE_UL_TRANSFORM_NONE,                     /**< 0x2: ESCO silence due to Tx transform is not established. */
    ESCO_SILENCE_UL_VOLUME_IS_ZERO,                     /**< 0x3: ESCO silence due to afe UL volume is zero. */
    ESCO_SILENCE_MAX,                                   /**< 0xA: ESCO silence sub error code end. */
} esco_silence_sub_err_code_type_t;

/** @brief
 * This enum defines the sub-error code for FATAL_ERROR_ESCO_NOISE
 */
typedef enum {
    ESCO_NOISE_DL_CODEC_FAIL = 0x0,                     /**< 0x0: ESCO noise due to codec error. */
    ESCO_NOISE_DL_PKT_LOSS_TOO_MANY,                    /**< 0x1: ESCO noise due to package drop many. */
    ESCO_NOISE_DL_AFE_BUFFER_EMPTY,                     /**< 0x2: ESCO noise due to AFE buffer become empty. */
    ESCO_NOISE_DL_PATTERN_SHIFT,                        /**< 0x3: ESCO noise due to sync header shift happen. */
    ESCO_NOISE_UL_AFE_BUFFER_FULL,                      /**< 0x4: ESCO noise due to uplink AFE buffer become full. */
    ESCO_NOISE_UL_FORWARDER_BUFFER_EMPTY,               /**< 0x5: ESCO noise due to uplink forwarder buffer become empty. */
    ESCO_NOISE_CALLBACK_BUSY,                           /**< 0x6: ESCO noise due to process task run too long */
    ESCO_NOISE_MAX,                                     /**< 0xA: ESCO noise sub error code end. */
} esco_noise_sub_err_code_type_t;

/** @brief
 * This enum defines the sub-error code for FATAL_ERROR_ESCO_ABNORMAL_CONTROL
 */
typedef enum {
    ESCO_CONTROL_DUPLICATED_ESCO_LINK = 0x0,            /**< 0x0: ESCO abnormal control due to eSCO link has ready set up before. */
    ESCO_CONTROL_DUPLICATED_SOURCE_SINK,                /**< 0x1: ESCO abnormal control due to duplicated source or sink. */
    ESCO_CONTROL_ONLY_LIGHT_MONO_MODE,                  /**< 0x2: ESCO abnormal control due to light mono mode is used. */
    ESCO_CONTROL_ESCO_CODEC_MISMATCH,                   /**< 0x3: ESCO abnormal control due to codec type is not correct. */
    ESCO_CONTROL_MAX,                                   /**< 0xA: ESCO abnormal control sub error code end. */
} esco_control_sub_err_code_type_t;


/**
  * @}
  * @}
  * @}
  */

#endif /*__FATAL_CODE_ESCO_H__*/
