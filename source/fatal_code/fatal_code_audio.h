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
#ifndef __FATAL_CODE_AUDIO_H__
#define __FATAL_CODE_AUDIO_H__

#include "fatal_code.h"
/**
 * @addtogroup Exception_Code Exception
 * @{
 * @addtogroup Fatal_Code Fatal Error Code Enums
 * @{
 * @addtogroup Audio_Fatal_Err Audio
 * @{
 */
/** @brief
 * This enum defines the Audio fatal error code
 */
typedef enum{
    FATAL_ERROR_AUDIO_SILENCE      = FATAL_ERROR_AUDIO_START,   /**< 0x401: audio silence error code . Please refer to #audio_silence_sub_err_code_type_t.*/
    FATAL_ERROR_AUDIO_DECODE_FAIL,                              /**< 0x402: audio decode fail error code . Please refer to #audio_decode_fail_err_code_type_t.*/
    FATAL_ERROR_AUDIO_ABNORMAL_CONTROL,                         /**< 0x403: audio control is abnormal. Please refer to #audio_abnormal_control_err_code_type_t.*/
    FATAL_ERROR_AUDIO_ABNORMAL_PARAMETER,                       /**< 0x404: audio parameter in algorithm is abnormal. Please refer to #audio_abnormal_parameter_err_code_type_t.*/
    FATAL_ERROR_AUDIO_MAX           = FATAL_ERROR_AUDIO_END,    /**< 0x600: audio error code end. */
}fatal_code_audio_type_t;

/** @brief
 * This enum defines the sub-error code for FATAL_ERROR_AUDIO_SILENCE
 */
typedef enum{
    AUDIO_SILENCE_AFE_BUFFER_EMPTY = 0x0,      /**< 0x0: audio afe meets buffer empty. */
    AUDIO_SILENCE_CODEC_MISMATCH,              /**< 0x1: audio silence due to codec mismatch. */
    AUDIO_SILENCE_TRANSFORM_NONE,              /**< 0x2: audio silence due to transform is not established. */
    AUDIO_SILENCE_VOLUME_IS_ZERO,              /**< 0x3: audio silence due to volume is zero. */
    AUDIO_SILENCE_SINK_IS_MUTE,                /**< 0x4: audio silence due to sink is muted by application layer. */
    AUDIO_SILENCE_SOURCE_L_IS_MUTE,            /**< 0x5: audio silence due to source L channel from audio gateway is mute pattern. */
    AUDIO_SILENCE_SOURCE_R_IS_MUTE,            /**< 0x6: audio silence due to source R channel from audio gateway is mute pattern. */
    AUDIO_SILENCE_MUTE_PATTERN,                /**< 0x7: audio silence due to mute pattern is inserted due to lack of packets. */
    AUDIO_SILENCE_UNKNOWN_PAYLOAD,             /**< 0x8: audio silence may due to unknown payload. */
<<<<<<< HEAD
    AUDIO_SILENCE_MAX,                         /**< 0x9: audio silence sub error code end. */
=======
    AUDIO_SILENCE_DROP_PKT_BY_CONTROLLER,      /**< 0x9: audio silence may due to heap size not enough controller drop bt pkt. */
    AUDIO_SILENCE_SOURCE_IS_MUTE,              /**< 0xA: audio silence due to source is muted by application layer. */
    AUDIO_SILENCE_MAX,                         /**< 0xB: audio silence sub error code end. */
>>>>>>> db20e11 (second commit)
}audio_silence_sub_err_code_type_t;

/** @brief
 * This enum defines the sub-error code for FATAL_ERROR_AUDIO_DECODE_FAIL
 */
typedef enum{
    AUDIO_DECODE_FAIL_SBC = 0x0,               /**< 0x0: sbc module decoded an error pattern. */
    AUDIO_DECODE_FAIL_AAC,                     /**< 0x1: aac module decoded an error pattern. */
}audio_decode_fail_err_code_type_t;

/** @brief
 * This enum defines the sub-error code for FATAL_ERROR_AUDIO_ABNORMAL_CONTROL
 */
typedef enum{
    AUDIO_CONTROL_DUPLICATED_SOURCE = 0x0,     /**< 0x0: duplicated audio source is created. */
    AUDIO_CONTROL_DUPLICATED_SINK,             /**< 0x1: duplicated audio sink is created. */
}audio_abnormal_control_err_code_type_t;


/** @brief
 * This enum defines the sub-error code for FATAL_ERROR_AUDIO_ABNORMAL_PARAMETER
 */
typedef enum{
    AUDIO_PARAMETER_PEQ_UPDATE_ERROR = 0x0,    /**< 0x0: updated parameter to peq submodule is abnormal. */
    AUDIO_PARAMETER_MP3_PATTERN_ERROR,         /**< 0x1: mp3 pattern is not supported. */
}audio_abnormal_parameter_err_code_type_t;

/**
  * @}
  * @}
  * @}
  */
#endif /*__FATAL_CODE_AUDIO_H__*/
