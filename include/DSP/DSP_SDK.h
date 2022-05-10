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
#ifndef _DSP_SDK_H_
#define _DSP_SDK_H_

#include "types.h"
#include "syslog.h"
#include "transform.h"
#include "message.h"
#include <stdbool.h>

/**
 * @addtogroup Middleware
 * @{
 * @addtogroup Stream
 * @{
 * This section introduces the DSP customization APIs including terms and acronyms,
 * details on how to use this APIs, feature table, enums, structures and functions.
 * The stream is data flow.
 * The DSP process is applied to stream by feature list.\n
 * The method of algorithm integration is shown in \ref STREAM_INTERFACE.
 *
 */



/******************************************************************************
 * CONSTANT DEFINITIONS
 ******************************************************************************/



/******************************************************************************
 * DSP Command Structure
 ******************************************************************************/
/**
 * @addtogroup Typedef
 * @{
 */

 /** @brief This structure defines the entry of the stream feature. */
typedef bool (*stream_feature_entry_t)   (void *para);
/** @brief This structure defines the entry of the stream feature codec. */
typedef stream_feature_entry_t stream_feature_codec_entry_t;
/** @brief This structure defines the entry of the stream feature function. */
typedef stream_feature_entry_t stream_feature_function_entry_t;

/**
 * @}
 */


/**
 * @addtogroup Struct
 * @{
 */

/** This structure defines the configuration of the stream feature table. */
typedef struct stream_feature_s {
    uint32_t   feature_type   : 8;              /**< The feature Type.  */
    uint32_t   memory_size    : 24;             /**< The instance memory(working buffer) size reserved for feature usage. */
    uint32_t   codec_output_size;               /**< The codec output size for streaming buffer allocate, Set 0 for functions. */
    stream_feature_entry_t initialize_entry;    /**< The Initializing entry for procedure. */
    stream_feature_entry_t process_entry;       /**< The Processing entry for procedure. */
}
/** @brief This structure defines the stream feature. */
stream_feature_t,
/** @brief This structure defines the stream feature pointer. */
*stream_feature_ptr_t,
/** @brief This structure defines the stream feature table. */
stream_feature_table_t,
/** @brief This structure defines the stream feature codec. */
stream_feature_codec_t,
/** @brief This structure defines the stream feature function. */
stream_feature_function_t,
DSP_FEATURE, DSP_FUNC, DSP_CODEC, *DSP_FEATURE_PTR, *DSP_FUNC_PTR, *DSP_CODEC_PTR;
/**
 * @}
 */

/**
 * @addtogroup Enum
 * @{
 */

/** This enum defines the types of audio features.*/
typedef enum
{
    DSP_DECODER_TYPE = 0,
    CODEC_NULL = DSP_DECODER_TYPE,
    CODEC_DECODER_VP,
    CODEC_DECODER_RT,
    CODEC_DECODER_SBC,
    CODEC_DECODER_AAC,
    CODEC_DECODER_MP3,
    CODEC_DECODER_MSBC,
    CODEC_DECODER_UART,
    CODEC_DECODER_UART16BIT,
    CODEC_DECODER_CELT_HD,
    CODEC_DECODER_CVSD,
    CODEC_DECODER_VP_MP3,
    CODEC_DECODER_INTERNAL_PATTERN,
    CODEC_DECODER_SAMPLE,


    DSP_ENCODER_TYPE = 0x10,
    CODEC_ENCODER_SBC = DSP_ENCODER_TYPE,
    CODEC_ENCODER_MSBC,
    CODEC_ENCODER_SB_WF,
    CODEC_ENCODER_CVSD,
    CODEC_ENCODER_OPUS,
    CODEC_ENCODER_GS_SBC,
    CODEC_PCM_COLLECT,

    DSP_FUNC_TYPE = 0x20,
    FUNC_END = DSP_FUNC_TYPE,
    FUNC_JOINT,
    FUNC_BRANCH,
    FUNC_MUTE,
    FUNC_AU_DRC,
    FUNC_PEQ_USER,
    FUNC_LPF,
    FUNC_CH_SEL,
    FUNC_SOUND_EFFECT,
    FUNC_PLC,
    FUNC_RX_NR,
    FUNC_TX_NR,
    FUNC_RX_WB_CPD,
    FUNC_RX_NB_CPD,
    FUNC_TX_WB_CPD,
    FUNC_TX_NB_CPD,
    FUNC_VC = 0x30,
    FUNC_VP_CPD,
    FUNC_DUMP_STREAM1,
    FUNC_DUMP_STREAM2,
<<<<<<< HEAD
=======
    FUNC_AU_DRC2,
>>>>>>> db20e11 (second commit)
    FUNC_PEQ_SPEAKER,
    FUNC_AUDIO_BIST,
    FUNC_PURE_DRC,
    FUNC_LINE_DRC,
    FUNC_VOICE_RX_AGC,
    FUNC_VOICE_TX_AGC,
    FUNC_FEEDBACK_UL,
    FUNC_VA,
    FUNC_MUTE_SMOOTHER,
    FUNC_INS,
    FUNC_AFC,
    FUNC_PEQ_AT,
<<<<<<< HEAD
    FUNC_AT_DRC = 0x40,
=======
    FUNC_AT_DRC = 0x41,
>>>>>>> db20e11 (second commit)
    FUNC_AT_NR,
    FUNC_AT_AGC,
    FUNC_CLK_SKEW_UL,
    FUNC_MIC_SW_GAIN,
    FUNC_WWE_PP_CON,
    FUNC_WWE_CON,
    FUNC_WWE_PP_HWVAD,
    FUNC_WWE_HWVAD,
    FUNC_LEAKAGE_DETECTION,
    FUNC_WWE_CON_AT_DRAM,
    FUNC_AMA,
    FUNC_GVA,
    FUNC_PCM_PREROLL,
    FUNC_TX_EQ,
    FUNC_RANDOM_NOISE,
    FUNC_POWER_DETECT,
<<<<<<< HEAD
=======
	FUNC_AUDIO_PLC,
>>>>>>> db20e11 (second commit)
    FUNC_SAMPLE,

    DSP_SRC = DSP_FUNC_TYPE-1,

<<<<<<< HEAD
    DSP_FEATURE_MAX_NUM = 0x51,
=======
    DSP_FEATURE_MAX_NUM = FUNC_SAMPLE+1,
>>>>>>> db20e11 (second commit)
    FUNC_CUSTOM_A,
    FUNC_CUSTOM_B,
    FUNC_CUSTOM_C,
    DSP_CUSTOM_FEATURE_MAX_NUM,
}
/** @brief This structure defines the stream feature type. */
stream_feature_type_t,
/** @brief This structure defines the stream feature type pointer. */
*stream_feature_type_ptr_t,
/** @brief This structure defines the stream feature list. */
stream_feature_list_t,
/** @brief This structure defines the stream feature list pointer. */
*stream_feature_list_ptr_t,
DSP_FEATURE_TYPE, DSP_FEATURE_TYPE_LIST, *DSP_FEATURE_TYPE_PTR, *DSP_FEATURE_TYPE_LIST_PTR;

#define DSP_CUSTOM_FEATURE_NO (DSP_CUSTOM_FEATURE_MAX_NUM - DSP_FEATURE_MAX_NUM - 1)

typedef enum {
    CONFIG_DECODER,    /**< To configure the decoder. */
    CONFIG_ENCODER,    /**< To configure the encoder. */
} stream_feature_configure_codec_t, CONFIG_FEATURE_CODEC;

/** @brief This enum defines the sampling rate.*/
typedef enum
{
    FS_RATE_8K          = 8,    /**< 8kHz sampling rate. */
    FS_RATE_11_025K     = 11,   /**< 11.025kHz sampling rate. */
    FS_RATE_12K         = 12,   /**< 12kHz sampling rate. */
    FS_RATE_16K         = 16,   /**< 16kHz sampling rate. */
    FS_RATE_22_05K      = 22,   /**< 22.05kHz sampling rate. */
    FS_RATE_24K         = 24,   /**< 24kHz sampling rate. */
    FS_RATE_32K         = 32,   /**< 32kHz sampling rate. */
    FS_RATE_44_1K       = 44,   /**< 44.1kHz sampling rate. */
    FS_RATE_48K         = 48,   /**< 48kHz sampling rate. */
    FS_RATE_64K         = 64,   /**< 64kHz sampling rate. */
    FS_RATE_88_2K       = 88,   /**< 88.2kHz sampling rate. */
    FS_RATE_96K         = 96,   /**< 96kHz sampling rate. */
    FS_RATE_176_4K      = 176,  /**< 176.4kHz sampling rate. */
    FS_RATE_192K        = 192,  /**< 193kHz sampling rate. */
} stream_samplerate_t, FS_IN_OUT_RATE_enum_s, AUDIO_FS_IN_OUT_RATE;

/** @brief This enum defines the bit resolution of audio samples.*/
typedef enum
{
    RESOLUTION_16BIT    = 0,    /**< 16 bits per sample. */
    RESOLUTION_32BIT    = 1,    /**< 32 bits per sample. */

    Res_16bit           = 0,
    Res_32bit           = 1,
} stream_resolution_t, AUDIO_RESOLUTION;

/** @brief This enum defines the audio stream sampling rate conversion ratio.*/
typedef enum
{
    UPSAMPLE_BY1        = 0x01,
    UPSAMPLE_BY2        = 0x02,
    UPSAMPLE_BY3        = 0x03,
    UPSAMPLE_BY4        = 0x04,
    UPSAMPLE_BY6        = 0x06,
    UPSAMPLE_BY12       = 0x0C,

    DOWNSAMPLE_BY1      = 0xFF,
    DOWNSAMPLE_BY2      = 0xFE,    /**< SRC ratio is 1/2x. */
    DOWNSAMPLE_BY3      = 0xFD,    /**< SRC ratio is 1/3x. */
    DOWNSAMPLE_BY4      = 0xFC,    /**< SRC ratio is 1/4x. */
    DOWNSAMPLE_BY6      = 0xFA,    /**< SRC ratio is 1/6x. */
    DOWNSAMPLE_BY12     = 0xF4,    /**< SRC ratio is 1/12x. */

} stream_feature_convert_samplerate_t, AUDIO_STREAM_UPDOWN_RATE;

typedef enum AUDIO_DSP_CH_SEL_e
{
    AU_DSP_CH_LR                    = 0,
    AU_DSP_CH_L                     = 1,
    AU_DSP_CH_R                     = 2,
    AU_DSP_CH_SWAP                  = 3,
    AU_DSP_CH_MIX                   = 4,
    AU_DSP_CH_MIX_SHIFT             = 5,
} stream_feature_audio_channel_selection_t, AUDIO_DSP_CH_SEL_t;


typedef enum
{
    DSP_WARNING_AU_OUT_ON_GUARD_TIME    = 1,
    DSP_WARNING_WAIT_OFF_SEQUENCE_START = 2,
    DSP_WARNING_WAIT_OFF_SEQUENCE_STOP  = 3,
} dsp_warning_message_t, DSP_WARNING_MESSAGE;


typedef enum
{
    FEATURE_MOUNT,
    FEATURE_UNMOUNT,
} stream_feature_mount_state_t, FEATURE_MOUNT_STATE;

typedef enum dsp_enc_bitrate_e
{
    ENCODER_BITRATE_16KBPS  = 16,
    ENCODER_BITRATE_32KBPS  = 32,
    ENCODER_BITRATE_40KBPS  = 40,
    ENCODER_BITRATE_56KBPS  = 56,
    ENCODER_BITRATE_64KBPS  = 64,
    ENCODER_BITRATE_MAX     = 0xFFFFFFFF,
} stream_feature_encoder_bit_rate_t, DSP_ENC_BITRATE_t;

typedef enum dsp_sbc_enc_bitpool_e
{
    SBC_ENCODER_BITPOOL_12  = 12,           //ENCODER_BITRATE_32KBPS
    SBC_ENCODER_BITPOOL_16  = 16,           //ENCODER_BITRATE_40KBPS
    SBC_ENCODER_BITPOOL_24  = 24,           //ENCODER_BITRATE_56KBPS
    SBC_ENCODER_BITPOOL_MAX = 0xFFFFFFFF,
} DSP_SBC_ENC_BITPOOL_t;

typedef enum
{
    AUDIO_COLLECT_PCM_CHANNEL_1 = (0x0001 << 0),
    AUDIO_COLLECT_PCM_CHANNEL_2 = (0x0001 << 1),
    AUDIO_COLLECT_PCM_CHANNEL_3 = (0x0001 << 2),
    AUDIO_COLLECT_PCM_CHANNEL_4 = (0x0001 << 3),
} PCM_COLLECTOR_CHANNEL_SEL;

typedef struct stru_dsp_pcm_col_interface_s* PCM_COL_MEM_PTR;

typedef struct stru_dsp_pcm_col_ctrl_u
{
    PCM_COL_MEM_PTR MemoryPtr;
    Handler Callback;
    U32 BufSize;
    U32 MsgThreshold;
    PCM_COLLECTOR_CHANNEL_SEL Channels;
} PCM_COLLECTOR_CTRL_t;

typedef struct stru_dsp_pcm_preroll_interface_s* PCM_PREROLL_MEM_PTR;

typedef struct stru_dsp_pcm_preroll_ctrl_u
{
    PCM_PREROLL_MEM_PTR MemoryPtr;
    U32 BufSize;
    U16 FrameSize;
    BOOL DataPassPermitted;
} PCM_PREROLL_CTRL_t;


/******************************************************************************
 * External Global Variables
 ******************************************************************************/






/******************************************************************************
 * External Functions
 ******************************************************************************/


/*!
  @brief Initialize customer's feature.
*/
void dsp_sdk_initialize(void);


/*!
  @brief Add feature to feature table.
  @param[in] feature_ptr is a pointer to corresponding feature pointer.
  @return TRUE when operation is success.
  @return FALSE when operation is failed.
*/
bool dsp_sdk_add_feature_table(stream_feature_ptr_t feature_ptr);


/*!
  @brief Configure Feature List Codec Type.
  @param[in] list is a pointer to corresponding feature type list.
  @param[in] codec_type is to identity corresponding codec type.
  @param[in] configure_codec is to identity corresponding codec configuration.
*/
void stream_feature_configure_type(stream_feature_list_ptr_t list, stream_feature_type_t codec_type, stream_feature_configure_codec_t configure_codec);


/*!
  @brief Configure Feature List Codec Output size.
  @param[in] list is a pointer to corresponding feature type list.
  @param[in] size is to identify the codec size.
  @param[in] configure_codec is to identify the codec's configuration.
*/
void stream_feature_configure_codec_output_size(stream_feature_list_ptr_t list, uint32_t size, stream_feature_configure_codec_t configure_codec);


/*!
  @brief Configure Feature List Codec Output Resolution.
  @param[in] list is a pointer to corresponding feature type list.
  @param[in] resolution is to identify the codec's resolution.
  @param[in] configure_codec is to identify the codec's configuration.
*/
void stream_feature_configure_resolution(stream_feature_list_ptr_t list, uint8_t resolution, stream_feature_configure_codec_t configure_codec);


/*!
  @brief Configure Feature List SRC
  @param[in] list is a pointer to corresponding feature type list.
  @param[in] in_res is the resolution of SRC's input.
  @param[in] out_res is the resolution of SRC's output.
  @param[in] src_out_sampling is sampling rate of SRC's output.
  @param[in] src_out_length is length of of SRC's output.
*/
void stream_feature_configure_src(stream_feature_list_ptr_t list, stream_resolution_t in_res, stream_resolution_t out_res, uint32_t src_out_sampling, uint32_t src_out_length);


/*!
  @brief reinitialize stream when an error occurs during the procedure.
  @param[in] para is a pointer to corresponding feature.
*/
void stream_feature_reinitialize(void *para);

/**
 * @defgroup STREAM_INTERFACE Interface
 * @{
 * This section introduces the APIs for algorithm integration.
 * - \b Introduction \n
 * The feature algorithm is separate to the codec and function. Each type uses different APIs. \n
 * The method of creating customer's codec is introduced in \ref STREAM_CODEC, and creating customer's function is introduced in \ref STREAM_FUNCTION.
 * - \b Stream \b interface \b architecture \n
 * The stream interface architecture is shown below:\n
 * @image html stream_interface_architecture.png
 */

/**
 * @defgroup STREAM_CODEC Codec
 * @{
 * This section introduces the DSP customization APIs for codecs.
 *
 *
 * @section STREAM_HOWTO_CREATE_CODEC How to create codec
 * The user must complete the following procedure to create a customized codec. For example:
 * - \b Step \b 1. Prepare entry of codec:\n
 *     1. Initialization\n
 *     2. Process\n
 *     .
 * The entry follows #stream_feature_entry_t.\n
 * The stream provides #stream_feature_reinitialize() to do initialization again when an error occurs, so there is no need to provide de-initialization api.\n
 * Note: The dynamic memory allocation is not allowed in procedure. The working buffer pointer get from #stream_codec_get_workingbuffer().
 * @code
 * typedef struct stream_codec_sample_instance_u
 * {
 *     uint8_t scratch_memory[128];
 *     uint8_t output[512];
 *     bool memory_check;
 *     bool reset_check;
 * } stream_codec_sample_instance_t, *stream_codec_sample_instance_ptr_t;
 *
 *
 * bool stream_codec_decoder_sample_initialize(void *para)
 * {
 *     stream_codec_sample_instance_ptr_t codec_sample_pointer;
 *
 *     //Get working buffer pointer
 *     codec_sample_pointer = (stream_codec_sample_instance_ptr_t)stream_codec_get_workingbuffer(para);
 *
 *     //Do initialize
 *     codec_sample_pointer->memory_check = true;
 *     codec_sample_pointer->reset_check = false;
 *     memset(codec_sample_pointer->scratch_memory, 0xFF, 128);
 *
 *     //return 0 when the initialization process is successful.
 *     return 0;
 * }
 *
 *
 * bool stream_codec_decoder_sample_process(void *para)
 * {
 *     stream_codec_sample_instance_ptr_t codec_sample_pointer;
 *     uint8_t *pattern_input_pointer;
 *     int16_t *output_l_pointer, *output_r_pointer;
 *     uint32_t input_length, output_length;
 *
 *     //Get working buffer pointer, stream buffer pointer, and length
 *     codec_sample_pointer = (stream_codec_sample_instance_ptr_t)stream_codec_get_workingbuffer(para);
 *     pattern_input_pointer = stream_codec_get_input_buffer(para, 1);
 *     output_l_pointer = stream_codec_get_output_buffer(para, 1);
 *     output_r_pointer = stream_codec_get_output_buffer(para, 2);
 *     input_length = stream_codec_get_input_size(para);
 *     output_length = stream_codec_get_output_size(para);
 *
 *
 *     //Call decoder
 *     //output sample rate : 16kHz
 *     //output resolution  : 16-bit
 *     output_length = codec_decoder_sample_api(pattern_input_pointer, output_l_pointer, output_r_pointer, input_length);
 *
 *     //Check decoder output
 *     if (output_length == 0) {
 *        //Do reinitialize when an error occurs.
 *        stream_feature_reinitialize(para);
 *     }
 *
 *     //Check expected resolution
 *     if (stream_codec_get_output_resolution(para) == RESOLUTION_32BIT) {
 *        dsp_converter_16bit_to_32bit((int32_t*)output_l_pointer, (int16_t*)output_l_pointer, output_length/sizeof(int16_t));
 *        dsp_converter_16bit_to_32bit((int32_t*)output_r_pointer, (int16_t*)output_r_pointer, output_length/sizeof(int16_t));
 *        output_length = output_length*2;
 *     }
 *
 *     //Modify stream buffering format
 *     stream_codec_modify_output_samplingrate(para, FS_RATE_16K);
 *     stream_codec_modify_output_size(para, output_length);
 *     stream_codec_modify_resolution(para, stream_codec_get_output_resolution(para));
 *
 *     //return 0 when the process is successful.
 *     return 0;
 * }
 * @endcode
 *
 * - \b Step \b 2. Add new feature type in #stream_feature_type_t.
 * @code
 * CODEC_DECODER_CELT_HD   = 0x09,             // decoder type: Reserved.
 * CODEC_DECODER_VENDOR    = 0x0A,             // decoder type: Vendor defined.
 * CODEC_DECODER_SAMPLE,                       // decoder type: example for the customer.
 * @endcode
 *
 * - \b Step \b 3. Define new feature codec, The structure folllows #stream_feature_t.\n
 * Note: The \ref stream_feature_t.memory_size is working buffer size.
 * @code
 * #define CODEC_SAMPLE_MEMORY_SIZE        sizeof(stream_codec_sample_instance_t)
 * #define CODEC_OUTPUT_SIZE               1024
 *
 * //                                                             feature_type,                memory_size, codec_output_size,                       initialize_entry,                       process_entry
 * stream_feature_codec_t stream_feature_sample_codec = { CODEC_DECODER_SAMPLE,   CODEC_SAMPLE_MEMORY_SIZE, CODEC_OUTPUT_SIZE, stream_codec_decoder_sample_initialize, stream_codec_decoder_sample_process};
 * @endcode
 *
 * - \b Step \b 4. Call #dsp_sdk_add_feature_table() to add to #stream_feature_table. \n
 * The sample code at #dsp_sdk_initialize().
 * @code
 * dsp_sdk_add_feature_table(&stream_feature_sample_codec);
 * @endcode
 *
 * - \b Step \b 5. Add feature type to feature list. The new codec is then available with the stream process.  \n
 * The sample code is replaced codec of prompt.
 * @code
 * stream_feature_list_t stream_feature_list_prompt[] =
 * {
 *     CODEC_DECODER_SAMPLE,       //New decoder
 *     FUNC_END,
 * };
 * @endcode
 */

/*!
  @brief Get corresponding feature type of this codec.
  @param[in] para a pointer to corresponding stream.
  @return codec's feature type.
*/
uint8_t stream_codec_get_type(void *para);

/*!
  @brief Get corresponding DSP task ID of this codec.
  @param[in] para a pointer to corresponding stream.
  @return DSP task ID.
*/
uint8_t stream_codec_get_task(void *para);

/*!
  @brief Get the channel number of the stream's input in this codec.
  @param[in] para a pointer to corresponding stream.
  @return channel number.
*/
uint8_t stream_codec_get_input_channel_number(void* para);

/*!
  @brief Get the sampling rate of the stream's input in this codec.
  @param[in] para a pointer to corresponding codec.
  @return sampling rate.
*/
uint8_t stream_codec_get_input_samplingrate(void* para);

/*!
  @brief Get corresponding size of the input stream for this codec.
  @param[in] para a pointer to corresponding codec.
  @return stream size.
*/
uint16_t stream_codec_get_input_size(void* para);

/*!
  @brief Get corresponding input stream size pointer for this codec.
  @param[in] para a pointer to corresponding codec.
  @return Sream size pointer.
*/
uint16_t* stream_codec_get_input_size_pointer(void *para);

/*!
  @brief Modify input stream size for this codec.
  @param[in] para a pointer to corresponding codec.
  @param[in] in_size input parameter for changing stream size.
  @return TRUE if the size is smaller or equal to allocated memory.
  @return FALSE if the size is lager than allocated memory.
*/
bool stream_codec_modify_input_size(void* para, uint16_t in_size);

/*!
  @brief Get the first input stream pointer for this codec.
  @param[in] para a pointer to corresponding codec.
  @return Sream pointer.
*/
void* stream_codec_get_1st_input_buffer(void* para);

/*!
  @brief Get the second input stream pointer for this codec.
  @param[in] para a pointer to corresponding codec.
  @return Sream pointer.
*/
void* stream_codec_get_2nd_input_buffer(void* para);

/*!
  @brief Get the third input stream pointer for this codec.
  @param[in] para a pointer to corresponding codec.
  @return Sream pointer.
*/
void* stream_codec_get_3rd_input_buffer(void* para);

/*!
  @brief Get the fourth input stream pointer for this codec.
  @param[in] para a pointer to corresponding codec.
  @return Sream pointer.
*/
void* stream_codec_get_4th_input_buffer(void* para);

/*!
  @brief Get specific number of stream pointer for this codec.
  @param[in] para a pointer to corresponding codec.
  @param[in] channel channel number.
  @return Sream pointer.
*/
void* stream_codec_get_input_buffer(void* para, uint32_t channel);

/*!
  @brief Get the channel number of the stream's output in this codec.
  @param[in] para a pointer to corresponding stream.
  @return channel number.
*/
uint8_t stream_codec_get_output_channel_number(void* para);

/*!
  @brief Get the sampling rate of the stream's output in this codec.
  @param[in] para a pointer to corresponding codec.
  @return sampling rate.
*/
uint8_t stream_codec_get_output_samplerate(void* para);

/*!
  @brief Get the pointer to the sampling rate of the stream's output in this codec.
  @param[in] para a pointer to corresponding codec.
  @return the pointer of sampling rate.
*/
uint8_t* stream_codec_get_output_samplerate_pointer(void *para);

/*!
  @brief Modify the sampling rate of output stream for this codec.
  @param[in] para a pointer to corresponding codec.
  @param[in] sampling_rate input parameter for changing sampling rate.
  @return TRUE
*/
bool stream_codec_modify_output_samplingrate(void* para, uint8_t sampling_rate);

/*!
  @brief Get corresponding size of the output stream for this codec.
  @param[in] para a pointer to corresponding codec.
  @return stream size.
*/
uint16_t stream_codec_get_output_size(void* para);

/*!
  @brief Get corresponding output stream's size pointer for this codec.
  @param[in] para a pointer to corresponding codec.
  @return Sream size pointer.
*/
uint16_t* stream_codec_get_output_size_pointer(void *para);

/*!
  @brief Modify output stream size for this codec.
  @param[in] para a pointer to corresponding codec.
  @param[in] out_size input parameter for changing stream size.
  @return TRUE if the size is smaller or equal to allocated memory
  @return FALSE if the size is lager than allocated memory.
*/
bool stream_codec_modify_output_size(void* para, uint16_t out_size);

/*!
  @brief Get 1st output stream pointer for this codec.
  @param[in] para a pointer to corresponding codec.
  @return Sream pointer.
*/
void* stream_codec_get_1st_output_buffer(void* para);

/*!
  @brief Get 2nd output stream pointer for this codec.
  @param[in] para a pointer to corresponding codec.
  @return Sream pointer.
*/
void* stream_codec_get_2nd_output_buffer(void* para);

/*!
  @brief Get 3rd output stream pointer for this codec.
  @param[in] para a pointer to corresponding codec.
  @return Sream pointer.
*/
void* stream_codec_get_3rd_output_buffer(void* para);

/*!
  @brief Get 4th output stream pointer for this codec.
  @param[in] para a pointer to corresponding codec.
  @return Sream pointer.
*/
void* stream_codec_get_4th_output_buffer(void* para);

/*!
  @brief Get the specified output stream pointer with channel number for this codec.
  @param[in] para a pointer to corresponding codec.
  @param[in] channel channel number.
  @return Sream pointer.
*/
void* stream_codec_get_output_buffer(void* para, uint32_t channel);

/*!
  @brief Get the pointer of the codec's memory instance.
  @param[in] para a pointer to corresponding codec.
  @return memory pointer.
*/
void* stream_codec_get_workingbuffer(void* para);

/*!
  @brief Get the resolution of the codec's input.
  @param[in] para a pointer to corresponding codec.
  @return resolution. (stream_resolution_t)
*/
uint8_t stream_codec_get_input_resolution(void* para);

/*!
  @brief Modify the resolution of codec process.
  @param[in] para a pointer to corresponding codec.
  @param[in] resolution resolution. (value is defined in stream_resolution_t)
  @return TRUE if it is successful.
  @return FALSE if it fails
*/
bool stream_codec_modify_resolution(void* para, uint8_t resolution);

/*!
  @brief Get the resolution of codec process.
  @param[in] para a pointer to corresponding codec.
  @return the resolution of codec process.
*/
uint8_t stream_codec_get_resolution(void* para);

/*!
  @brief Get the resolution of codec output.
  @param[in] para a pointer to corresponding codec.
  @return the resolution of codec output.
*/
uint8_t stream_codec_get_output_resolution(void* para);

/*!
  @brief Get the numbering of codec of feature list.
  @param[in] para a pointer to corresponding codec.
  @return the numbering.
*/
uint32_t stream_codec_get_numbering(void *para);


uint32_t stream_codec_get_encoder_bit_rate(void *para);

void *stream_codec_get_encoder_buffer_control_pointer(void *para);

void stream_codec_notify_output_empty(void *para, bool isEmpty);


/**
 * @}
 */



/**
 * @defgroup STREAM_FUNCTION Function
 * @{
 * This section introduces the DSP customization APIs for functions.
 *
 *
 * @section STREAM_HOWTO_CREATE_FUNCTION How to create function
 * The user must complete the following procedure to create a customized function. For example:
 * - \b Step \b 1. Prepare entry of function:\n
 *     1. Initialization\n
 *     2. Process\n
 *     .
 *     int16_t buffer_r[512];
 * } stream_function_sample_instance_t, *stream_function_sample_instance_ptr_t;
 *
 *
 * bool stream_function_sample_initialize(void *para)
 * {
 *     stream_function_sample_instance_ptr_t function_sample_pointer;
 *
 *     //Get working buffer pointer
 *     function_sample_pointer = (stream_function_sample_instance_ptr_t)stream_function_get_working_buffer(para);
 *
 *     //Do initialize
 *     memcpy(function_sample_pointer->filter, coefficient_table_16k, 13*sizeof(uint16_t));
 *     function_sample_pointer->coefficient_size = 13;
 *
 *     //return 0 when the initialization process is successful.
 *     return 0;
 * }
 *
 *
 * bool stream_function_sample_process(void *para)
 * {
 *     int16_t *l_pointer, *r_pointer;
 *     uint32_t frame_length;
 *     stream_function_sample_instance_ptr_t function_sample_pointer;
 *
 *     //Get working buffer pointer, stream buffer pointer, and length
 *     function_sample_pointer = (stream_function_sample_instance_ptr_t)stream_function_get_working_buffer(para);
 *     l_pointer = stream_function_get_inout_buffer(para, 1);
 *     r_pointer = stream_function_get_inout_buffer(para, 2);
 *     frame_length = stream_function_get_output_size(para);
 *
 *     //Call function API
 *     function_sample_api(function_sample_pointer,
 *                         l_pointer,
 *                         r_pointer,
 *                         function_sample_pointer->buffer_l,
 *                         function_sample_pointer->buffer_r,
 *                         frame_length);
 *
 *
 *     //return 0 when the process is successful.
 *     return 0;
 * }
 * @endcode
 *
 * - \b Step \b 2. Add new feature type in #stream_feature_type_t.
 * @code
 * FUNC_CH_SEL             = 0x33,             //< Basic FUNCTION type: CH SEL.
 * FUNC_MUTE_SMOOTHER      = 0x34,             //< Basic FUNCTION type: mute smoother.
 * FUNC_SAMPLE,                                //< FUNCTION type: example for the customer.
 * @endcode
 *
 * - \b Step \b 3. Define new feature function. The structure folllows #stream_feature_t.\n
 * Note: The \ref stream_feature_t.memory_size is working buffer size.
 * @code
 * #define FUNCTION_SAMPLE_MEMORY_SIZE     sizeof(stream_function_sample_instance_t)
 *
 * //                                                            feature_type,                 memory_size,   0(Must equal to zero),                  initialize_entry,                  process_entry
 * stream_feature_function_t  stream_feature_sample_function = {  FUNC_SAMPLE, FUNCTION_SAMPLE_MEMORY_SIZE,                       0, stream_function_sample_initialize, stream_function_sample_process};
 * @endcode
 *
 * - \b Step \b 4. Call #dsp_sdk_add_feature_table() to add to #stream_feature_table. \n
 * the sample code at #dsp_sdk_initialize()
 * @code
 * dsp_sdk_add_feature_table(&stream_feature_sample_function);
 * @endcode
 *
 * - \b Step \b 5. Add feature type to feature list. The effect of the new function is available to the stream process. \n
 * The sample code is added to HFP downlink.
 * @code
 * stream_feature_list_t stream_feature_list_hfp_downlink[] =
 * {
 *     CODEC_DECODER_MSBC,
 *     FUNC_PLC,
 *     FUNC_RX_NR,
 *     FUNC_RX_WB_DRC,
 *     FUNC_SAMPLE,                // Add New feature type
 *     FUNC_CLK_SKEW_HFP_DL,
 *     FUNC_END,
 * };
 * @endcode
 */


/*!
  @brief Get the Type of function.
  @param[in] para a pointer to corresponding function.
  @return the function's feature type.
*/
uint8_t stream_function_get_type(void *para);

/*!
  @brief Get the OS Task handle of function.
  @param[in] para a pointer to corresponding function.
  @return Task id.
*/
uint8_t stream_function_get_task(void *para);

/*!
  @brief Get the channel number of the stream in this function.
  @param[in] para a pointer to corresponding function.
  @return channel number.
*/
uint8_t stream_function_get_channel_number(void* para);

/*!
  @brief Get the sampling rate of the stream in this function.
  @param[in] para a pointer to corresponding function.
  @return sampling rate.
*/
uint8_t stream_function_get_samplingrate(void* para);

/*!
  @brief Get the size of the stream in this function.
  @param[in] para a pointer to corresponding function.
  @return the size of the stream.
*/
uint16_t stream_function_get_output_size(void* para);

/*!
  @brief Modify the sampling conversion ratio of the stream in this function.
  @param[in] para a pointer to corresponding function.
  @param[in] updown_rate identify updown ratio. (defined in #stream_feature_convert_samplerate_t)
  @return TRUE if it is successful.
  @return FALSE if it fails.
*/
bool stream_function_modify_samplingrateconversion_ratio(void* para, stream_feature_convert_samplerate_t updown_rate);

/*!
  @brief Modify the size of the stream in this function.
  @param[in] para a pointer to corresponding function.
  @param[in] size identify size for modification.
  @return TRUE if it is successful.
  @return FALSE if it fails.
*/
bool stream_function_modify_output_size(void* para, uint16_t size);

/*!
  @brief Get 1st stream pointer for this function.
  @param[in] para a pointer to corresponding function.
  @return Sream pointer.
*/
void* stream_function_get_1st_inout_buffer(void* para);

/*!
  @brief Get 2nd stream pointer for this function.
  @param[in] para a pointer to corresponding function.
  @return Sream pointer.
*/
void* stream_function_get_2nd_inout_buffer(void* para);

/*!
  @brief Get 3rd stream pointer for this function.
  @param[in] para a pointer to corresponding function.
  @return Sream pointer.
*/
void* stream_function_get_3rd_inout_buffer(void* para);

/*!
  @brief Get 4th stream pointer for this function.
  @param[in] para a pointer to corresponding function.
  @return Sream pointer.
*/
void* stream_function_get_4th_inout_buffer(void* para);

/*!
  @brief Get the specific stream pointer for this function.
  @param[in] para a pointer to corresponding function.
  @param[in] channel channel number.
  @return Sream pointer.
*/
void* stream_function_get_inout_buffer(void* para, uint32_t channel);

/*!
  @brief Get the pointer of the function's memory instance.
  @param[in] para a pointer to corresponding function.
  @return memory pointer.
*/
void* stream_function_get_working_buffer(void* para);

/*!
  @brief Modify the resolution of function process.
  @param[in] para a pointer to corresponding function.
  @param[in] resolution resolution. (defined in #stream_resolution_t)
  @return TRUE if it is successful.
  @return FALSE if it fails.
*/
bool stream_function_modify_output_resolution(void* para, uint8_t resolution);

/*!
  @brief Get the resolution of function process.
  @param[in] para a pointer to corresponding function.
  @return resolution. (defined in #stream_resolution_t)
*/
uint8_t stream_function_get_output_resolution(void* para);

/*!
  @brief Get the resolution of function's stream output.
  @param[in] para a pointer to corresponding function.
  @return resolution. (defined in #stream_resolution_t)
*/
uint8_t stream_function_get_sink_output_resolution(void* para);

/*!
  @brief Get the numbering of function process of feature list.
  @param[in] para a pointer to corresponding function.
  @return the numbering.
*/
uint32_t stream_function_get_numbering(void *para);


uint16_t stream_function_get_codec_input_size(void* para);

/**
 * @}
 */
<<<<<<< HEAD
/**
 * @}
 */
=======
uint8_t stream_codec_get_mute_flag(void *para);
/**
 * @}
 */
VOID stream_codec_set_mute_flag(void *para, uint8_t value);
>>>>>>> db20e11 (second commit)

/**
 * @}
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// FUNCTION WRAPPER MACRO///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define DSP_AddFeatureTable(feature_ptr)                                                    dsp_sdk_add_feature_table(feature_ptr);
#define DSP_ConfigFeatureListCodec(list, codec_type, conig)                                 stream_feature_configure_type(list, codec_type, conig)
#define DSP_ConfigFeatureListCodecSize(list, size, conig)                                   stream_feature_configure_codec_output_size(list, size, conig)
#define DSP_ConfigFeatureListCodecResolution(list, resolution, conig)                       stream_feature_configure_resolution(list, resolution, conig)
#define DSP_ConfigFeatureListSRC(list, in_res, out_res, src_out_sampling, src_out_length)   stream_feature_configure_src(list, in_res, out_res, src_out_sampling, src_out_length)
#define DSP_SetFeatureDeinit(para)                                                          stream_feature_reinitialize(para)


/* Codec APIs */
#define DSP_GetCodecType(para)                                      stream_codec_get_type(para)
#define DSP_GetCodecStreamTask(para)                                stream_codec_get_task(para)
#define DSP_GetCodecInStreamChannelNum(para)                        stream_codec_get_input_channel_number(para)
#define DSP_GetCodecInStreamSamplingRate(para)                      stream_codec_get_input_samplingrate(para)
#define DSP_GetCodecInStreamSize(para)                              stream_codec_get_input_size(para)
#define DSP_GetCodecInStreamSizePtr(para)                           stream_codec_get_input_size_pointer(para)
#define DSP_ModifyCodecInStreamSize(para, in_size)                  stream_codec_modify_input_size(para, in_size)
#define DSP_GetCodecInStream1Ptr(para)                              stream_codec_get_1st_input_buffer(para)
#define DSP_GetCodecInStream2Ptr(para)                              stream_codec_get_2nd_input_buffer(para)
#define DSP_GetCodecInStream3Ptr(para)                              stream_codec_get_3rd_input_buffer(para)
#define DSP_GetCodecInStream4Ptr(para)                              stream_codec_get_4th_input_buffer(para)
#define DSP_GetCodecInStreamPtr(para, channel)                      stream_codec_get_input_buffer(para, channel)
#define DSP_GetCodecOutStreamChannelNum(para)                       stream_codec_get_output_channel_number(para)
#define DSP_GetCodecOutStreamSamplingRate(para)                     stream_codec_get_output_samplerate(para)
#define DSP_GetCodecOutStreamSamplingRatePtr(para)                  stream_codec_get_output_samplerate_pointer(para)
#define DSP_ModifyCodecOutStreamSamplingRate(para, sampling_rate)   stream_codec_modify_output_samplingrate(para, sampling_rate)
#define DSP_GetCodecOutStreamSize(para)                             stream_codec_get_output_size(para)
#define DSP_GetCodecOutStreamSizePtr(para)                          stream_codec_get_output_size_pointer(para)
#define DSP_ModifyCodecOutStreamSize(para, out_size)                stream_codec_modify_output_size(para, out_size)
#define DSP_GetCodecOutStream1Ptr(para)                             stream_codec_get_1st_output_buffer(para)
#define DSP_GetCodecOutStream2Ptr(para)                             stream_codec_get_2nd_output_buffer(para)
#define DSP_GetCodecOutStream3Ptr(para)                             stream_codec_get_3rd_output_buffer(para)
#define DSP_GetCodecOutStream4Ptr(para)                             stream_codec_get_4th_output_buffer(para)
#define DSP_GetCodecOutStreamPtr(para, channel)                     stream_codec_get_output_buffer(para, channel)
#define DSP_GetCodecMemoryInstancePtr(para)                         stream_codec_get_workingbuffer(para)
#define DSP_GetCodecInResolution(para)                              stream_codec_get_input_resolution(para)
#define DSP_ModifyCodecProcessResolution(para, resolution)          stream_codec_modify_resolution(para, resolution)
#define DSP_GetCodecProcessResolution(para)                         stream_codec_get_resolution(para)
#define DSP_GetCodecOutResolution(para)                             stream_codec_get_output_resolution(para)
#define DSP_GetCodecNumbering(para)                                 stream_codec_get_numbering(para)
#define DSP_GetEncoderBitrate(para)                                 stream_codec_get_encoder_bit_rate(para)
#define DSP_GetEncoderBufCtrlPtr(para)                              stream_codec_get_encoder_buffer_control_pointer(para)



/* Function APIs */
#define DSP_GetFuncType(para)                                       stream_function_get_type(para)
#define DSP_GetFuncStreamTask(para)                                 stream_function_get_task(para)
#define DSP_GetFuncStreamChannelNum(para)                           stream_function_get_channel_number(para)
#define DSP_GetFuncStreamSamplingRate(para)                         stream_function_get_samplingrate(para)
#define DSP_GetFuncStreamSize(para)                                 stream_function_get_output_size(para)
#define DSP_ModifyFuncStreamSamplingUpDownRate(para, updown_rate)   stream_function_modify_samplingrateconversion_ratio(para, updown_rate)
#define DSP_ModifyFuncStreamSize(para, size)                        stream_function_modify_output_size(para, size)
#define DSP_GetFuncStream1Ptr(para)                                 stream_function_get_1st_inout_buffer(para)
#define DSP_GetFuncStream2Ptr(para)                                 stream_function_get_2nd_inout_buffer(para)
#define DSP_GetFuncStream3Ptr(para)                                 stream_function_get_3rd_inout_buffer(para)
#define DSP_GetFuncStream4Ptr(para)                                 stream_function_get_4th_inout_buffer(para)
#define DSP_GetFuncStreamPtr(para, channel)                         stream_function_get_inout_buffer (para, channel)
#define DSP_GetFuncMemoryInstancePtr(para)                          stream_function_get_working_buffer(para)
#define DSP_ModifyFuncProcessResolution(para, resolution)           stream_function_modify_output_resolution(para, resolution)
#define DSP_GetFuncProcessResolution(para)                          stream_function_get_output_resolution(para)
#define DSP_GetFuncStreamOutResolution(para)                        stream_function_get_sink_output_resolution(para)
#define DSP_GetFuncNumbering(para)                                  stream_function_get_numbering(para)
#define DSP_GetStreamInSize(para)                                   stream_function_get_codec_input_size(para)



EXTERN uint32_t DSP_GetFuncMemorySize(void* para);
EXTERN bool DSP_ForceStreamReenter(void* para);
EXTERN U32 DSP_GetEncodedStreamSize(TRANSFORM transform);
EXTERN BOOL DSP_GetEncodedStream (TRANSFORM transform, U8* Buf, U32 Length);
EXTERN BOOL DSP_FillMuteToEncodedStream (TRANSFORM transform); // Used after transform is established
EXTERN U16 DSP_GetStreamEncodedRatio (VOID* para);
EXTERN U32 DSP_GetEncodedStreamSamples (VOID* para);
EXTERN BOOL DSP_PauseEncodingStream (TRANSFORM transform, BOOL isPause);
EXTERN BOOL DSP_IsEncodingStreamPaused (VOID* para);

EXTERN AUDIO_DSP_CH_SEL_t DSP_GetSinkOutCh(VOID* para);
bool pcm_collector_configuration (PCM_COLLECTOR_CTRL_t *config);
EXTERN void audio_silence_self_check(void);

/* VAD Handler APIs*/
/**
 * DSP_HookVadTriggerHandler
 *
 * @Author : Yoyo <SYChiu@airoha.com.tw>
 */
EXTERN VOID DSP_HookVadTriggerHandler(VOID* fPtr);

EXTERN VOID VC_ModifyBitMask (S32 BitMask); // Temp for VC
EXTERN S16 VC_CollectCurrentResult (VOID); // Temp for VC
EXTERN VOID VC_UpdateDataBase (VOID* Db); // Temp for VC


typedef struct
{
    U16 elementID;
    U16 numOfParameter;
    U16 parameter[1];
} PEQ_ELEMENT_STRU;

typedef struct
{
    U16 numOfElement;
    U16 peqAlgorithmVer;
    PEQ_ELEMENT_STRU peqElement[1];
} PEQ_PARAMETER_STRU;

enum  //elementID
{
    PEQ_32K = 0x00,
    PEQ_44_1K,
    PEQ_48K,
    PEQ_96K,
};


EXTERN VOID PEQ_UpdateParaByPtr (stream_feature_type_t feature_type, PEQ_PARAMETER_STRU* update_ptr);
EXTERN VOID PEQ_ResetPara (stream_feature_type_t feature_type);

EXTERN VOID DSP_ChangeStreaming2Deinit (TRANSFORM transform);

EXTERN VOID Drift_ReportObservationLevel(U32 Target, U32 Current);

#if 1
#define LOG_PRINT_AUDIO(_message, arg_cnt, ...)           LOG_MSGID_I(audio, _message, arg_cnt, ##__VA_ARGS__)
#define LOG_ERROR_AUDIO(_message, arg_cnt, ...)           LOG_MSGID_E(audio, _message, arg_cnt, ##__VA_ARGS__)
#else
#define LOG_PRINT_AUDIO(_message, arg_cnt, ...)
#define LOG_ERROR_AUDIO(_message, arg_cnt, ...)
#endif


#endif /* _DSP_SDK_H_ */

