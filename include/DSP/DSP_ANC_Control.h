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
#ifndef _DSP_ANC_CONTROL_H_
#define _DSP_ANC_CONTROL_H_
//#include "config.h"
//#include "os.h"
//#include "bt_types.h"
//#include "types.h"


////////////////////////////////////////////////////////////////////////////////
// Constant Definitions ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// Type Defintions /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef enum {
    FILTER_ANC_1 = 1,
    FILTER_ANC_2,
    FILTER_ANC_3,
    FILTER_ANC_4,
    FILTER_PASS_THRU_BIGIN = 9,
    FILTER_PASS_THRU_1 = FILTER_PASS_THRU_BIGIN,
    FILTER_PASS_THRU_2,
    FILTER_PASS_THRU_3,
} anc_filter_type_t;

typedef enum {
    HAL_AUDIO_ANC_TYPE_HYBRID = 0,
    HAL_AUDIO_ANC_TYPE_FF = 1,
    HAL_AUDIO_ANC_TYPE_FB = 2,
    HAL_AUDIO_ANC_TYPE_USER_DEFINE = 3, //temp: not support for 1562
    HAL_AUDIO_PASSTHRU_TYPE_FF = 4,
    HAL_AUDIO_ANC_TYPE_DUMMY = 0xFFFFFFFF,
} hal_audio_anc_type_t;

/** @brief ANC&Pass-through control return values. */
typedef enum {
    ANC_CONTROL_EXECUTION_NONE    = -2,
    ANC_CONTROL_EXECUTION_FAIL    = -1,
    ANC_CONTROL_EXECUTION_SUCCESS =  0,
    ANC_CONTROL_EXECUTION_CANCELLED = 1,
} anc_control_result_t;

#define ANC_CONTROL_EVENT_ON    (1 << 0)
#define ANC_CONTROL_EVENT_OFF   (1 << 1)
typedef uint8_t anc_control_event_t;

#define CB_LEVEL_ALL             (0)
#define CB_LEVEL_SUCCESS_ONLY    (1)
typedef uint8_t anc_callback_level_t;

#define ANC_REALTIME_UPDATE_CH_FF_L  (0)
#define ANC_REALTIME_UPDATE_CH_FF_R  (1)
#define ANC_REALTIME_UPDATE_CH_FB_L  (2)
<<<<<<< HEAD
#define ANC_REALTIME_UPDATE_CH_FBC_L (4)
=======
#define ANC_REALTIME_UPDATE_CH_FB_R  (3)
#define ANC_REALTIME_UPDATE_CH_FBC_L (4)
#define ANC_REALTIME_UPDATE_CH_FBC_R (5)

>>>>>>> db20e11 (second commit)
typedef uint8_t anc_realtime_update_ch_t;

typedef void (*anc_control_callback_t)(anc_control_event_t event_id, anc_control_result_t result);

/** @brief Hal audio anc filter mask. */
typedef enum {
    AUDIO_ANC_FILTER_MASK_NONE = 0,

    AUDIO_ANC_FILTER_MASK_EARBUDS_HYBRID = (1 << 0) | (1 << 1) | (1 << 4),
    AUDIO_ANC_FILTER_MASK_EARBUDS_FF = (1 << 0),
    AUDIO_ANC_FILTER_MASK_EARBUDS_FB = (1 << 0) | (1 << 4),

    AUDIO_ANC_FILTER_MASK_HEADSET_FF_L = (1 << 0),
    AUDIO_ANC_FILTER_MASK_HEADSET_FF_R = (1 << 1),

    AUDIO_ANC_FILTER_MASK_FF_FB_ALL = (1 << 0) | (1 << 1),
    AUDIO_ANC_FILTER_MASK_DEQ_ALL = (1 << 4) | (1 << 5),
    AUDIO_ANC_FILTER_MASK_ALL = (1 << 0) | (1 << 1) | (1 << 4) | (1 << 5),
} audio_anc_filter_mask_t;


////////////////////////////////////////////////////////////////////////////////
// Macro ///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// Function Prototypes /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN anc_control_result_t audio_anc_enable (anc_filter_type_t flash_no, hal_audio_anc_type_t type, int16_t runtime_gain);
EXTERN anc_control_result_t audio_passthru_enable (anc_filter_type_t flash_no, hal_audio_anc_type_t type, int16_t runtime_gain);
EXTERN anc_control_result_t audio_anc_passthru_disable (uint8_t speed_mode);
EXTERN anc_control_result_t audio_anc_passthru_get_current_status (uint8_t *enable, anc_filter_type_t *flash_no, hal_audio_anc_type_t *type, audio_anc_filter_mask_t *filter_mask_l, audio_anc_filter_mask_t *filter_mask_r, uint32_t *sam_bank_mux);
EXTERN anc_control_result_t audio_anc_passthru_register_callback(anc_control_callback_t callback, anc_control_event_t event_mask, anc_callback_level_t level);
EXTERN anc_control_result_t audio_anc_passthru_deregister_callback(anc_control_callback_t callback);
EXTERN anc_control_result_t audio_anc_passthru_get_status_from_nvkey(uint8_t *enable, anc_filter_type_t *flash_no, hal_audio_anc_type_t *type, int16_t *runtime_gain);
EXTERN anc_control_result_t audio_anc_passthru_save_status_into_nvkey(void);
EXTERN anc_control_result_t audio_anc_passthru_set_runtime_gain(int16_t runtime_gain);
EXTERN anc_control_result_t audio_anc_get_runtime_gain(int16_t *runtime_gain);
EXTERN anc_control_result_t audio_passthru_get_runtime_gain(int16_t *runtime_gain);
EXTERN anc_control_result_t audio_anc_passthru_set_dg_gain(int16_t gain_0, int16_t gain_1);
EXTERN anc_control_result_t audio_anc_passthru_set_dg_all_gain(int16_t gain_0, int16_t gain_1, int16_t gain_2, int16_t gain_3);
EXTERN anc_control_result_t audio_anc_passthru_read_dg_gain(int16_t *gain_0, int16_t *gain_1);
EXTERN anc_control_result_t audio_anc_passthru_read_dg_speaker_gain(int16_t *gain_0, int16_t *gain_1);
EXTERN anc_control_result_t audio_anc_passthru_write_dg_gain(int16_t gain_0, int16_t gain_1);
EXTERN anc_control_result_t audio_anc_passthru_write_dg_speaker_gain(int16_t gain_0, int16_t gain_1);
EXTERN uint8_t audio_anc_passthru_get_hybrid_cap(void);
EXTERN anc_control_result_t audio_anc_passthru_realtime_update_coefficient(anc_realtime_update_ch_t ch, void *coef, uint32_t length);
EXTERN anc_control_result_t audio_anc_passthru_set_input_for_fbc_calibration(bool enable);


#endif /* _DSP_ANC_CONTROL_H_ */

