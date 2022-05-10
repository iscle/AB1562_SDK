/* Copyright Statement:
 *
 * (C) 2019  Airoha Technology Corp. All rights reserved.
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

#include "DSP_SDK.h"

enum
{
    AUDIO_CH_STEREO,
    AUDIO_CH_MONO,
};


void drv_a2dp_init(void);
BOOL drv_a2dp_start(BD_ADDR_T *pBdAddr, U8 codec, U8 scms, U8 sampleRateIndex);
BOOL drv_a2dp_stop(BD_ADDR_T *pBdAddr);
BOOL drv_a2dp_is_playing(BD_ADDR_T *pBdAddr);
BD_ADDR_T * drv_a2dp_get_bdaddr(void);
void drv_a2dp_set_bdaddr(BD_ADDR_T *pBdAddr);

BOOL drv_a2dp_get_game_mode(void);
void drv_a2dp_set_game_mode(BOOL isOn);

void drv_a2dp_set_low_latency_mode(BD_ADDR_T *pBdAddr, BOOL isOn);
BOOL drv_a2dp_get_low_latency_mode(BD_ADDR_T *pBdAddr);

void drv_a2dp_set_partner_later_join(BOOL is_partner_later_join);
BOOL drv_a2dp_is_partner_later_join(void);

void drv_a2dp_update_audio_channel(U8 audio_ch);
U8 drv_a2dp_get_audio_channel(BOOL is_by_role);
void drv_a2dp_rho_done(void);

void drv_a2dp_mute_spk(void);
void drv_a2dp_set_share_mode_on(BOOL isOn, BOOL is_follower);

SOURCE drv_a2dp_get_source(void);

<<<<<<< HEAD
=======
BOOL drv_a2dp_get_ap_mode(void);
void drv_a2dp_set_ap_mode(BOOL mode);
>>>>>>> db20e11 (second commit)
