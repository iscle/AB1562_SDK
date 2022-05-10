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
#include "FW_Api.h"
#include "DSP_SDK.h"
#include "feature_config.h"
#include <stdio.h>


////////////////////////////////////////////////////////////////////////////////
typedef struct stream_function_sample_instance_u
{
    uint32_t coefficient_size;
    uint8_t filter[32];
    int16_t buffer_l[512];
    int16_t buffer_r[512];
} stream_function_sample_instance_t, *stream_function_sample_instance_ptr_t;


////////////////////////////////////////////////////////////////////////////////
// Constant Definitions ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define FUNCTION_SAMPLE_MEMORY_SIZE     sizeof(stream_function_sample_instance_t)
<<<<<<< HEAD
#define CUSTOMIZED_TX_NR_MEMORY_SIZE    10000 /* Modify 3rd party customized TX NR scratch size here*/
=======
#if defined(AIR_TXNR_3RD_PARTY_ENABLE) || defined(AIR_TXNR_INTELLIGO_1MIC_ENABLE)
#define CUSTOMIZED_TX_NR_MEMORY_SIZE    80*1024 /* Modify 3rd party customized TX NR scratch size here*/
#endif
>>>>>>> db20e11 (second commit)
#define TX_NR_FRAME_SIZE                480   /* In unit of bytes */

////////////////////////////////////////////////////////////////////////////////
// External Functions //////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
extern void DSP_SetTxNrMemSize (uint32_t Size);

////////////////////////////////////////////////////////////////////////////////
// Function Prototypes /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef FEATURE_DSP_SDK
bool stream_function_sample_initialize(void *para);
bool stream_function_sample_process(void *para);
uint32_t function_sample_api(stream_function_sample_instance_ptr_t instance_ptr, int16_t *input_l_pointer, int16_t *input_r_pointer, int16_t *output_l_pointer, int16_t *output_r_pointer, uint32_t length);
#endif
void dsp_sys_initialize(void);

bool dsp_customized_func_a_init(void *para);
bool dsp_customized_func_b_init(void *para);
bool dsp_customized_func_c_init(void *para);

bool dsp_customized_func_a_proc(void *para);
bool dsp_customized_func_b_proc(void *para);
bool dsp_customized_func_c_proc(void *para);

////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DSP_FEATURE dsp_customized_feature_table[DSP_CUSTOM_FEATURE_NO] = {
  /*{  FeatureType,  MemSize, CodecOutSize,             *InitialEntry,                *ProcessEntry}*/
    {FUNC_CUSTOM_A,        0,            0, dsp_customized_func_a_init,  dsp_customized_func_a_proc},
    {FUNC_CUSTOM_B,        0,            0, dsp_customized_func_b_init,  dsp_customized_func_b_proc},
    {FUNC_CUSTOM_C,        0,            0, dsp_customized_func_c_init,  dsp_customized_func_c_proc},
};


/* A2DP Feature List */
stream_feature_list_t stream_feature_list_a2dp[10] =
{
    CODEC_NULL,
    FUNC_MUTE_SMOOTHER,
    FUNC_CH_SEL,
    FUNC_PEQ_USER,
    FUNC_AU_DRC,
<<<<<<< HEAD
=======
#ifdef AIR_PEQ_SPEAKER_ENBALE
    FUNC_PEQ_SPEAKER,
    FUNC_AU_DRC2,
#endif
>>>>>>> db20e11 (second commit)
    FUNC_END,
};

const stream_feature_list_t stream_feature_list_a2dp_with_peq[6] =
{
    CODEC_NULL,
    FUNC_MUTE_SMOOTHER,
    FUNC_CH_SEL,
    FUNC_PEQ_USER,
    FUNC_AU_DRC,
    FUNC_END,
};

const stream_feature_list_t stream_feature_list_a2dp_without_peq[6] =
{
    CODEC_NULL,
    FUNC_MUTE_SMOOTHER,
    FUNC_CH_SEL,
    FUNC_AU_DRC,
    FUNC_END,
};

const stream_feature_list_t stream_feature_list_a2dp_share_with_peq[6] =
{
    CODEC_NULL,
    FUNC_CH_SEL,
    FUNC_PEQ_USER,
    FUNC_AU_DRC,
    FUNC_END,
};

const stream_feature_list_t stream_feature_list_a2dp_share_without_peq[6] =
{
    CODEC_NULL,
    FUNC_CH_SEL,
    FUNC_AU_DRC,
    FUNC_END,
};

/* eSCO Feature List (EC/NR on) */
<<<<<<< HEAD
const stream_feature_list_t stream_feature_list_hfp_nb_uplink_ecnr_on[] =
=======
stream_feature_list_t stream_feature_list_hfp_nb_uplink_ecnr_on[] =
>>>>>>> db20e11 (second commit)
{
    CODEC_NULL,
    FUNC_CLK_SKEW_UL,
    FUNC_MIC_SW_GAIN,
    FUNC_TX_NR,
    FUNC_VOICE_TX_AGC,
    FUNC_TX_WB_CPD,
    FUNC_END,
};

const stream_feature_list_t stream_feature_list_hfp_nb_downlink_ecnr_on[] =
{
    CODEC_NULL,
    FUNC_PLC,
    FUNC_RX_NR,
    FUNC_VOICE_RX_AGC,
    FUNC_RX_NB_CPD,
    FUNC_END,
};

<<<<<<< HEAD
const stream_feature_list_t stream_feature_list_hfp_wb_uplink_ecnr_on[] =
=======
stream_feature_list_t stream_feature_list_hfp_wb_uplink_ecnr_on[] =
>>>>>>> db20e11 (second commit)
{
    CODEC_NULL,
    FUNC_CLK_SKEW_UL,
    FUNC_MIC_SW_GAIN,
    FUNC_TX_NR,
    FUNC_VOICE_TX_AGC,
    FUNC_TX_WB_CPD,
    FUNC_END,
};

const stream_feature_list_t stream_feature_list_hfp_wb_downlink_ecnr_on[] =
{
    CODEC_NULL,
    FUNC_PLC,
    FUNC_RX_NR,
    FUNC_VOICE_RX_AGC,
    FUNC_RX_WB_CPD,
    FUNC_END,
};

/* eSCO Feature List (EC/NR off) */
<<<<<<< HEAD
const stream_feature_list_t stream_feature_list_hfp_nb_uplink_ecnr_off[] =
=======
stream_feature_list_t stream_feature_list_hfp_nb_uplink_ecnr_off[] =
>>>>>>> db20e11 (second commit)
{
    CODEC_NULL,
    FUNC_CLK_SKEW_UL,
    FUNC_MIC_SW_GAIN,
    FUNC_TX_WB_CPD,
    FUNC_END,
};

const stream_feature_list_t stream_feature_list_hfp_nb_downlink_ecnr_off[] =
{
    CODEC_NULL,
    FUNC_PLC,
    FUNC_RX_NB_CPD,
    FUNC_END,
};

<<<<<<< HEAD
const stream_feature_list_t stream_feature_list_hfp_wb_uplink_ecnr_off[] =
=======
stream_feature_list_t stream_feature_list_hfp_wb_uplink_ecnr_off[] =
>>>>>>> db20e11 (second commit)
{
    CODEC_NULL,
    FUNC_CLK_SKEW_UL,
    FUNC_MIC_SW_GAIN,
    FUNC_TX_WB_CPD,
    FUNC_END,
};

const stream_feature_list_t stream_feature_list_hfp_wb_downlink_ecnr_off[] =
{
    CODEC_NULL,
    FUNC_PLC,
    FUNC_RX_WB_CPD,
    FUNC_END,
};

#ifdef FEATURE_DSP_SDK
/**
 * stream_codec_decoder_sample_process
 *
 *
 */
bool stream_codec_decoder_sample_initialize(void *para)
{
    stream_codec_sample_instance_ptr_t codec_sample_pointer;

    //Get working buffer pointer
    codec_sample_pointer = (stream_codec_sample_instance_ptr_t)stream_codec_get_workingbuffer(para);

    //Do initialize
    codec_sample_pointer->memory_check = true;
    codec_sample_pointer->reset_check = false;
    memset(codec_sample_pointer->scratch_memory, 0xFF, 128);

    //return 0 when successfully initialize
    return 0;
}


/**
 * stream_function_sample_initialize
 *
 *
 */
bool stream_function_sample_initialize(void *para)
{
    stream_function_sample_instance_ptr_t function_sample_pointer;

    //Get working buffer pointer
    function_sample_pointer = (stream_function_sample_instance_ptr_t)stream_function_get_working_buffer(para);

    //Do initialize
    memcpy(function_sample_pointer->filter, coefficient_table_16k, 13*sizeof(uint16_t));
    function_sample_pointer->coefficient_size = 13;

    //return 0 when successfully initialize
    return 0;
}

/**
 * stream_function_sample_process
 *
 *
 */
bool stream_function_sample_process(void *para)
{
    int16_t *l_pointer, *r_pointer;
    uint32_t frame_length;
    stream_function_sample_instance_ptr_t function_sample_pointer;

    //Get working buffer pointer, stream buffer pointer, and length
    function_sample_pointer = (stream_function_sample_instance_ptr_t)stream_function_get_working_buffer(para);
    l_pointer = stream_function_get_inout_buffer(para, 1);
    r_pointer = stream_function_get_inout_buffer(para, 2);
    frame_length = stream_function_get_output_size(para);

    //Call function API
    function_sample_api(function_sample_pointer,
                        l_pointer,
                        r_pointer,
                        function_sample_pointer->buffer_l,
                        function_sample_pointer->buffer_r,
                        frame_length);



    //return 0 when successfully process
    return 0;
}

uint32_t function_sample_api(stream_function_sample_instance_ptr_t instance_ptr, int16_t *input_l_pointer, int16_t *input_r_pointer, int16_t *output_l_pointer, int16_t *output_r_pointer, uint32_t length)
{
    UNUSED(instance_ptr);
    UNUSED(input_l_pointer);
    UNUSED(input_r_pointer);
    UNUSED(output_l_pointer);
    UNUSED(output_r_pointer);
    UNUSED(length);
    return 0;
}

#endif

/* One time initialization at system boot */
void dsp_sdk_initialize (void)
{
<<<<<<< HEAD
    #if defined(MTK_3RD_PARTY_NR)
=======
    #if defined(AIR_TXNR_3RD_PARTY_ENABLE) || defined(AIR_TXNR_INTELLIGO_1MIC_ENABLE)
>>>>>>> db20e11 (second commit)
    DSP_SetTxNrMemSize(CUSTOMIZED_TX_NR_MEMORY_SIZE);
    #endif
}

bool dsp_customized_func_a_init(void *para)
{
    UNUSED(para);
    //printf("[dsp_sdk] func a init");
    return 0;
}

bool dsp_customized_func_b_init(void *para)
{
    UNUSED(para);
    //printf("[dsp_sdk] func b init");
    return 0;
}

bool dsp_customized_func_c_init(void *para)
{
    UNUSED(para);
    //printf("[dsp_sdk] func c init");
    return 0;
}



bool dsp_customized_func_a_proc(void *para)
{
    UNUSED(para);
    //printf("[dsp_sdk] func a proc");
    return 0;
}


bool dsp_customized_func_b_proc(void *para)
{
    UNUSED(para);
    //printf("[dsp_sdk] func b proc");
    return 0;
}


bool dsp_customized_func_c_proc(void *para)
{
    UNUSED(para);
    //printf("[dsp_sdk] func c proc");
    return 0;
}


/* Customized TX NR
 *
 * Do not remove symbol name & modify prototype, otherwise compile may result in error
 *
 */
<<<<<<< HEAD
void Voice_TX_NR_Init(void* tx_nr_ptr, 
                    int32_t* nr_para)
{
    UNUSED(tx_nr_ptr);
    UNUSED(nr_para);

    //printf("[dsp_sdk] nr_para[0]:0x%x", (uint32_t)nr_para[0]);
    //printf("[dsp_sdk] nr_para[9]:0x%x", (uint32_t)nr_para[9]);
}

void Voice_TX_NR_Prcs(int16_t* ec_out1,
                      int16_t* ec_out2,
                      int16_t* nr_out,
                      int16_t nr_gain,
                      void* tx_nr_ptr)
{
    UNUSED(tx_nr_ptr);
    UNUSED(nr_gain);
    UNUSED(ec_out2);
    FW_Memcpy(nr_out, ec_out1, TX_NR_FRAME_SIZE);
=======
#ifdef AIR_TXNR_INTELLIGO_1MIC_ENABLE
extern void IGO_NR_Init(void*, uint8_t*);
extern void IGO_NR_Prcs(int16_t* in, int16_t* out, void* buf_ptr);
#endif

void Voice_TX_NR_Init(void *tx_nr_ptr, int32_t *nr_para, int32_t *non_fota_nr_para)
{
#if defined(AIR_TXNR_INTELLIGO_1MIC_ENABLE)
    UNUSED(nr_para);
    extern void igo_table;
    IGO_NR_Init(tx_nr_ptr, (uint8_t *)non_fota_nr_para);
#else
    UNUSED(nr_para);
    UNUSED(tx_nr_ptr);
    UNUSED(non_fota_nr_para);
#endif
}

void Voice_TX_NR_Prcs(int16_t *ec_out1,
                      int16_t *ec_out2,
                      int16_t *nr_out,
                      int16_t nr_gain,
                      void *tx_nr_ptr)
{
#if defined(AIR_TXNR_INTELLIGO_1MIC_ENABLE)
    UNUSED(ec_out2);
    UNUSED(nr_gain);
    IGO_NR_Prcs(ec_out1, nr_out, tx_nr_ptr);
#else
    UNUSED(nr_gain);
    UNUSED(ec_out2);
    UNUSED(ec_out1);
    UNUSED(nr_out);
    UNUSED(tx_nr_ptr);
#endif
>>>>>>> db20e11 (second commit)
}

