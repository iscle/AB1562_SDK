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

#ifndef _DRV_CALIBRATION_
#define _DRV_CALIBRATION_

#define AUDIO_CALIBRATION_ENABLE

typedef enum {
    DRV_CALIBRATION_MODE_FF_MIC_L   = 1,
    DRV_CALIBRATION_MODE_FF_MIC_R   = 2,
    DRV_CALIBRATION_MODE_FB_MIC_L   = 3,
    DRV_CALIBRATION_MODE_FB_MIC_R   = 4,
    DRV_CALIBRATION_MODE_SPEAKER_L  = 5,
    DRV_CALIBRATION_MODE_SPEAKER_R  = 6,
} drv_calibration_mode_t;


BOOL drv_calibration_start(void);
BOOL drv_calibration_stop(BOOL is_suspend);
BOOL drv_calibration_is_playing(void);
BOOL drv_calibration_set_detect_channel(U32 set_chaneel);
BOOL drv_calibration_set_output_mute(BOOL is_mute);
<<<<<<< HEAD
BOOL drv_calibration_set_mode(U32 set_chaneel);
S32 drv_calibration_get_result(void);
=======
BOOL drv_calibration_set_output_frequency(U32 output_frequency, S16 output_volume);
BOOL drv_calibration_set_mode(U32 set_chaneel);
S32 drv_calibration_get_result(void);
S16 drv_calibration_set_low_pass_filter(S16 lpf_frequency);
>>>>>>> db20e11 (second commit)

#endif //_DRV_CALIBRATION_
