#ifndef _STREAM_CONFIG_H_
#define _STREAM_CONFIG_H_
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



#include "types.h"

/**
 * @brief stream config type
 */
typedef enum
{
    /*!< mute enable : 1(TRUE) enable ,0(FALSE) disble  */
    AUDIO_SOURCE_MUTE_ENABLE =      0x0200,
    AUDIO_SOURCE_DOWNSAMP_RATE =    0x0201,
    AUDIO_SOURCE_FRAME_SIZE =       0x0202,
    AUDIO_SOURCE_FRAME_NUMBER =     0x0203,
    AUDIO_SOURCE_CIC_FILTER =       0x0204,
    AUDIO_SOURCE_SRC_ENABLE =       0x0205,
    AUDIO_SOURCE_SRC_DISABLE =      0x0206,
    AUDIO_SOURCE_CH_SELECT =        0x0207,
    AUDIO_SOURCE_RESOLUTION =       0x0208,
    AUDIO_SOURCE_SET_HANDLE =       0x0209,
    AUDIO_SOURCE_LR_SWITCH_ENABLE = 0x020A,
    AUDIO_SOURCE_LR_SWITCH_DISABLE= 0x020B,
    AUDIO_SOURCE_FORCE_START =      0x020C,
    AUDIO_SOURCE_SAMPLE_RATE =      0x020D,
    AUDIO_SOURCE_SCENARIO =         0x020E,
    AUDIO_SOURCE_CHANGE_INPUT_SEL = 0x020F,

    AUDIO_SINK_MUTE_ENABLE =        0x0280,
    AUDIO_SINK_UPSAMPLE_RATE =      0x0281,
    AUDIO_SINK_FRAME_SIZE =         0x0282,
    AUDIO_SINK_FRAME_NUMBER =       0x0283,
    AUDIO_VP_SINK_FRAME_SIZE =      0x0284,
    AUDIO_VP_SINK_FRAME_NUMBER =    0x0285,
    AUDIO_SINK_CIC_FILTER =         0x0286,
    AUDIO_SINK_SIDE_TONE_ENABLE =   0x0287,
    AUDIO_SINK_SIDE_TONE_DISABLE =  0x0288,
    AUDIO_SINK_LR_SWITCH_ENABLE =   0x0289,
    AUDIO_SINK_SRC_ENABLE =         0x028A,
    AUDIO_SINK_SRC_DISABLE =        0x028B,
    AUDIO_SINK_CH_SELECT =          0x028C,
    AUDIO_SINK_FORCE_START =        0x028D,
    AUDIO_SINK_RESOLUTION   =       0x028E,
    AUDIO_SINK_SRCIN_SAMPLE_RATE =  0X028F,
    AUDIO_SINK_SRCIN_RESOLUTION =   0x0290,
    AUDIO_SINK_PAUSE =              0x0291,
    AUDIO_SINK_RESUME =             0x0292,
    AUDIO_SINK_SET_HANDLE =         0x0293,

    VIRTUAL_SINK_SET_HANDLE =       0X0350,
    VIRTUAL_SINK_BUF_SIZE =         0X0351,


    SCO_SINK_MUTE_ENABLE =          0x0380,
    SCO_SINK_PKT_SIZE =             0x0381,

    SCO_SINK_PKT_NUMBER =           0x0382,


    SCO_SOURCE_PKT_INFO_LENGTH =    0x0300,
    SCO_SOURCE_PKT_SIZE =           0x0301,
    SCO_SOURCE_PKT_NUMBER =         0x0302,
    SCO_SOURCE_WAIT_FOR_SYNC_START =0x0303,
    SCO_SOURCE_MCE_ROLE =           0x0304,

    MEMORY_SOURCE_MAX_DATA_READ =   0x0400,
    MEMORY_SOURCE_SET_HANDLE =      0x0401,
    MEMORY_SOURCE_UPDATE_MEM_ADDR = 0x0402, // Source Memory length will be set to zero to prevent Unexpected data read.
    MEMORY_SOURCE_UPDATE_MEM_LEN =  0x0403,


    FILE_SET_FILE_HANDLE_POINTER, // Set the address of the opened file's handle.
    FILE_CLEAR_BUFFER, // Clear internal memory buffer for preloading data from SD/eMMC.
    FILE_FINISHED_READ_HANDLER, // Specify the message handler which will receive read finished message.
    FILE_PLAYED_POSITION, // Read the file position of playing.
    FILE_EOF, // Check whether the file is read complete or not.

    A2DP_SOURCE_MCE_ROLE =          0x0450,
    A2DP_SOURCE_FS =                0x0451,
    A2DP_SOURCE_ALC_ENABLE =        0x0452,
    A2DP_SOURCE_INITIAL_WAIT =      0x0453,
    A2DP_SOURCE_SET_GAMING_MODE =   0x0454,
    A2DP_SOURCE_GET_GAMING_MODE =   0x0455,
<<<<<<< HEAD
=======
    A2DP_SOURCE_SET_DEVICE_TYPE =   0x0456,//for ap mode
>>>>>>> db20e11 (second commit)
} stream_config_type;


#endif  /* __APP_STREAM_IF_H__ */
