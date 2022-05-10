#ifndef __AUDIO_CONFIG_H__
#define __AUDIO_CONFIG_H__
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



/*!
  @brief audio_hardware used in StreamAudioSource()/StreamAudioSink() traps.
*/

typedef enum {
    AUDIO_HARDWARE_DISABLE          = 0x00,
    AUDIO_HARDWARE_PCM              = 0x01, /*!< The audio PCM hardware with high audio quality. "instance" specifies the physical interface (AUDIO_INSTANCE_0 is the first PCM interface, AUDIO_INSTANCE_1 is the second, on chips which have two).
                                                 Audio hardware PCM, I2S and SPDIF are mutually exclusive for the same audio_instance.
                                                 "channel" specifies the PCM slot; only AUDIO_CHANNEL_SLOT_x are valid for this hardware type.*/
    AUDIO_HARDWARE_I2S_M            = 0x02, /*!< The audio I2S hardware. "instance" specifies the physical interface (AUDIO_INSTANCE_0 is the first I2S interface, AUDIO_INSTANCE_1 is the second, on chips which have two).
                                                 Audio hardware PCM, I2S and SPDIF are mutually exclusive for the same audio_instance.
                                                 "channel" specifies the I2S slot; only AUDIO_CHANNEL_SLOT_x are valid for this hardware type.*/
    AUDIO_HARDWARE_I2S_S            = 0x04,
    AUDIO_HARDWARE_SPDIF            = 0x08, /*!< The audio SPDIF hardware. "instance" specifies the physical interface (AUDIO_INSTANCE_0 is the first SPDIF interface, AUDIO_INSTANCE_1 is the second, on chips which have two).
                                                 Audio hardware PCM, I2S and SPDIF are mutually exclusive for the same audio_instance.
                                                 "channel" specifies the SPDIF slot; only AUDIO_CHANNEL_SLOT_x are valid for this hardware type.*/
    AUDIO_HARDWARE_I2S_MULTICHANNEL = 0x10, /*!< The audio I2S Master hardware with multichannel. Need to configurate I2S_Master_Output_Ch*/

    AUDIO_HARDWARE_DIGITAL_MIC      = 0x20, /*!< The audio digital MIC hardware. "instance" specifies the physical interface (AUDIO_INSTANCE_0 is the first digital MIC interface, AUDIO_INSTANCE_1 is the second,
                                                 AUDIO_INSTANCE_2 is the third, on chips which have three).
                                                 "channel" specifies the digital MIC channel; only AUDIO_CHANNEL_A or AUDIO_CHANNEL_B are valid for this hardware type.
                                                 An "instance" consists of a pair of channels (stereo).*/
    AUDIO_HARDWARE_ANALOG_MIC       = 0x40,
    AUDIO_HARDWARE_MULTIPLE_MIC     = 0x60, /*!< The audio multiple MIC(digital mic and analog mic)*/
    AUDIO_HARDWARE_MINI_DSP         = 0x80,

} audio_hardware;

typedef enum {
    INSTANCE_A, //"instance A" specifies the physical interface A for given audio hardware
    INSTANCE_B, //"instance B" specifies the physical interface B for given audio hardware
    INSTANCE_C, //"instance C" specifies the physical interface C for given audio hardware
    INSTANCE_D, //"instance D" specifies the physical interface D for given audio hardware
} audio_instance;


/*!
  @brief audio_channel used in StreamAudioSource(), StreamAudioSink() and CodecSetIirFilter() traps.
*/

typedef enum {
    AUDIO_CHANNEL_A = 1, /*!< The audio channel A*/
    AUDIO_CHANNEL_B, /*!< The audio channel B*/
    AUDIO_CHANNEL_A_AND_B, /*!< The audio channel A and B (obtaining the stereo CODEC in mono-mode)*/
    AUDIO_CHANNEL_3CH,
    AUDIO_CHANNEL_4CH,
    AUDIO_CHANNEL_5CH,
    AUDIO_CHANNEL_6CH,
    AUDIO_CHANNEL_E = 0x80,
    AUDIO_CHANNEL_A_AND_E, /*!< The audio channel A and Echo path, only avalable on audio source*/
    AUDIO_CHANNEL_B_AND_E, /*!< The audio channel B and Echo path, only avalable on audio source*/
    AUDIO_CHANNEL_A_AND_B_AND_E, /*!< The audio channel A and B and Echo path, only avalable on audio sink (obtaining the stereo CODEC in mono-mode)*/
    AUDIO_CHANNEL_3CH_AND_E,
    AUDIO_CHANNEL_4CH_AND_E,
    AUDIO_CHANNEL_5CH_AND_E,
    AUDIO_CHANNEL_6CH_AND_E,
    AUDIO_CHANNEL_VP,

    AUDIO_CHANNEL_SLOT_0 = 0, /*!< The audio digital slot 0*/
    AUDIO_CHANNEL_SLOT_1, /*!< The audio digital slot 1*/
    AUDIO_CHANNEL_SLOT_2, /*!< The audio digital slot 2*/
    AUDIO_CHANNEL_SLOT_3, /*!< The audio digital slot 3*/

    SPDIF_CHANNEL_A = 0, /*!< The 1st SPDIF subframe*/
    SPDIF_CHANNEL_B, /*!< The 2nd SPDIF subframe*/
    SPDIF_CHANNEL_USER, /*!< The User data in SPDIF subframes*/
    SPDIF_CHANNEL_A_B_INTERLEAVED, /*!< SPDIF channels to be interleaved*/
    AUDIO_BRANCH_A_AND_B,
    AUDIO_JOINT_A_AND_B,
} audio_channel;

typedef enum {
    RT_mode,
    VP_amr59,
    VP_amr122,
    VP_WB,
    VP_ADPCM,
    VP_mSBC,
} PATTERN_CODEC;

typedef enum {
    CVSD = 0,
    mSBC = 1,
    Transparent = 2,
} SCO_CODEC;

typedef enum {
    SCO  = 0,
    eSCO = 1,
} SCO_type;

typedef enum {
    TxOut = 1,
    RxIn  = 0,
} SCO_INOUT;

typedef enum {
    Disable = 0,
    Active  = 1,
} SCO_status;

typedef enum {
    EV3  = 0,
    EV3_2 = 1,
    EV4  = 2,
    EV5  = 3,
} SCO_PACKET_type;

#endif  /* __APP_AUDIO_IF_H__ */
