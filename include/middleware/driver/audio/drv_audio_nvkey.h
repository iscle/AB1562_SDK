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

 
#ifndef _DRV_AUDIO_NVKEY_H_
#define _DRV_AUDIO_NVKEY_H_



/******************************************************************************
 * Define
*******************************************************************************/
#define AUDIO_DSP_VAD_ALWAYS_ON_FEATURE			0x0001
#define AUDIO_DSP_VAD_ANDROID_ON_FEATURE		0x0002
#define AUDIO_DSP_VAD_PHASE1_ONLY               0x0004


#define PRODUCT_HEADSET     0
#define PRODUCT_MCSYNC      1


/******************************************************************************
* Public Functions (Product)
*******************************************************************************/
BOOL drv_audio_nvkey_is_mcsync_product(void);

/******************************************************************************
* Public Functions (Audio Path)
*******************************************************************************/
BOOL drv_audio_nvkey_init(U8 product);

U16 drv_audio_nvkey_get_output_path();
U16 drv_audio_nvkey_get_audio_input_path();
U16 drv_audio_nvkey_get_voice_input_path();
U16 drv_audio_nvkey_get_voice_input_path2(void);
U16 drv_audio_nvkey_get_audio_channel();

BOOL drv_audio_nvkey_is_feature_on(U16 feature);

/******************************************************************************
* Public Functions (Side Tone Config)
*******************************************************************************/
BOOL drv_audio_nvkey_is_sidetone_sco_on(void);
BOOL drv_audio_nvkey_is_sidetone_a2dp_on(void);

/******************************************************************************
* Public Functions (A2DP Channel Selection)
*******************************************************************************/
enum
{
    AUDIO_CHANNEL_SW_MODE,
    AUDIO_CHANNEL_HW_MODE,
    DEFAULT_AUDIO_CHANNEL_SW_MODE = 0xFF,
};


U8 drv_audio_nvkey_a2dp_get_sw_audio_channel_setting(void);
U8 drv_audio_nvkey_a2dp_get_audio_mode_setting(void);
U8 drv_audio_nvkey_a2dp_get_audio_channel_gpio_h(void);
U8 drv_audio_nvkey_a2dp_get_audio_channel_gpio_l(void);
BOOL drv_audio_nvkey_peq_is_peq_feature_on(void);

#endif
