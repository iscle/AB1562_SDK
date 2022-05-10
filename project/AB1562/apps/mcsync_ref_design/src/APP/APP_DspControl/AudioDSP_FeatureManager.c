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

#include "bt.h"
#include "transform.h"
#include "DSP_Control.h"

/******************************************************************************
* Type Definition
*******************************************************************************/
typedef VOID (*DSP_GENERAL_ENTRY) (VOID);

/******************************************************************************
 * External Functions prototype
*******************************************************************************/

/* OPUS encoder */
EXTERN int opus_encoder_init(unsigned char *);
EXTERN int opus_encoder_process(unsigned char *, unsigned char *,  short *,int );
EXTERN int opus_encoder_get_size();

/* EC NR*/
EXTERN void Voice_WB_TX_Init(int, short*, void*, void*, void*);
EXTERN void Voice_WB_TX_Inear_Init(int, short*, void*, void*, void*, void*, void*);
EXTERN void Voice_WB_TX_Inear_Init_V2(int, short *, void*, void*, void*, void*, void*);
EXTERN void Voice_EC_Init(int , void *, void *, S16 *);

EXTERN void Voice_WB_TX_Prcs(short* , short* , short* , short* , void* );
EXTERN void Voice_WB_TX_Inear_Prcs(short*, short*, short*, short*, void*, short*, short*);
EXTERN void Voice_WB_TX_Inear_Prcs_V2(short*, short*, short*, short*, short*, void*);
EXTERN void Voice_EC_Prcs(short* ,short* ,short* ,short* , short*);
EXTERN void Voice_PostEC_Prcs(short*, short*, void*);

EXTERN int  get_aec_nr_memsize (void);

EXTERN void EC_REF_GAIN_READBACK (short*);
EXTERN void IE_EC_REF_GAIN_READBACK (short*);

EXTERN void AEC_NR_AirDump (void);
EXTERN void AEC_NR_InEar_AirDump (void);
EXTERN void AEC_NR_DualMic_InEar_AirDump (void);

EXTERN int get_ECNR_SVN(void);

/* TX FIR EQ */
EXTERN int  get_fir_wbeq_memsize(void);
EXTERN void Voice_TxEQ_init(void *, void *);
EXTERN void Voice_TxEQ_Prcs(short *, void *);

/* advanced AT AFC */
EXTERN void AFC_Init (void*, void*);
EXTERN void AFC_prcs (short*, short*, short*, void*);
EXTERN int  get_at_memsize (void);

/* advanced AT NR */
EXTERN void AT_NR_init (void*, void*, void*);
EXTERN void AT_NR_Prcs (short*);
EXTERN int get_at_nr_memsize (int);

/* advanced AT AGC */
EXTERN void at_agc_init(void* , void* , short);
EXTERN short at_agc_proc(int frame[], void* , short);
EXTERN int get_at_agc_memsize(void);
EXTERN void AGC_gain_change(void* , short);

/* Airoha WWE */
EXTERN BOOL WWE_PP_Init (void*, WWE_MODE_t);
EXTERN BOOL WWE_PreProcess (void*, WWE_MODE_t);
EXTERN BOOL WWE_P_Init (void*, WWE_MODE_t, bool);
EXTERN BOOL WWE_Process (void*, WWE_MODE_t);

/* AMA */
EXTERN bool AMA_Interface_Init (void*);
EXTERN bool AMA_Interface_Process (void*);

/* GVA */
EXTERN bool GVA_Interface_Init (void*);
EXTERN bool GVA_Interface_Process (void*);

/* Leakage detection */
EXTERN bool Leakage_detection_Interface_Init (void *para);
EXTERN bool Leakage_detection_Interface_Process (void *para);



/******************************************************************************
* Structure
*******************************************************************************/

/******************************************************************************
 * Enumeration
*******************************************************************************/

/******************************************************************************
 * Variable
*******************************************************************************/
#ifdef AMAZON_AMA_ENABLE
ALIGN(4) const char ama_database[] =
{
#include "ama_db.txt"
};
#endif

#ifdef GOOGLE_GVA_ENABLE
ALIGN(16) const char gva_database_ro[] =
{
    #include "gva.txt"
};

ALIGN(16) const char gva_database_rw[] =
{
    #include "gva_rw.txt"
};
#endif

/******************************************************************************
 * Static Functions prototype
*******************************************************************************/

/******************************************************************************
 * Static Functions
*******************************************************************************/


/******************************************************************************
 * Public Functions
*******************************************************************************/
<<<<<<< HEAD
#if defined(MTK_3RD_PARTY_NR)
=======
#if defined(AIR_TXNR_3RD_PARTY_ENABLE) || defined(AIR_TXNR_INTELLIGO_1MIC_ENABLE)
>>>>>>> db20e11 (second commit)
int TWO_MIC_WB_Write_FLASH(S16* bufou)
{
    UNUSED(bufou);

    return FALSE;
}
#endif


/******************************************************************************
 * Public Functions (Stream)
*******************************************************************************/
VOID APP_AudioDsp_FeatureEntry_Init (VOID)
{
    #ifdef MTK_OPUS_ENCODER_ENABLE
    DSP_SetFeatureEntry(CODEC_ENCODER_OPUS, opus_encoder_init, opus_encoder_process, opus_encoder_get_size, NULL);
    #endif

<<<<<<< HEAD
    #if defined(MTK_INEAR_ENHANCEMENT_ENABLE)
=======
    #if defined(AIR_TXNR_1_MIC_INEAR_ENABLE)
>>>>>>> db20e11 (second commit)
    DSP_SetEcNrMode(EC_NR_IN_EAR_MODE);
    DSP_SetFeatureEntry(FUNC_TX_NR, Voice_WB_TX_Inear_Init, Voice_WB_TX_Inear_Prcs, get_aec_nr_memsize, AEC_NR_InEar_AirDump);
    DSP_SetEcNrRefGainEntry(EC_REF_GAIN_READBACK, IE_EC_REF_GAIN_READBACK);
    DSP_SetEcNrRevisionEntry(get_ECNR_SVN);
<<<<<<< HEAD
    #elif defined(MTK_DUALMIC_INEAR_ENABLE)
=======
    #elif defined(AIR_TXNR_2_MIC_INEAR_ENABLE)
>>>>>>> db20e11 (second commit)
    DSP_SetEcNrMode(EC_NR_DUAL_MIC_IN_EAR_MODE);
    DSP_SetFeatureEntry(FUNC_TX_NR, Voice_WB_TX_Inear_Init_V2, Voice_WB_TX_Inear_Prcs_V2, get_aec_nr_memsize, AEC_NR_DualMic_InEar_AirDump);
    DSP_SetEcNrRefGainEntry(EC_REF_GAIN_READBACK, IE_EC_REF_GAIN_READBACK);
    DSP_SetEcNrRevisionEntry(get_ECNR_SVN);
<<<<<<< HEAD
    #elif defined(MTK_3RD_PARTY_NR)
=======
    #elif defined(AIR_TXNR_3RD_PARTY_ENABLE) || defined(AIR_TXNR_INTELLIGO_1MIC_ENABLE)
>>>>>>> db20e11 (second commit)
    DSP_SetEcNrMode(EC_NR_CUSTOMIZED_TX_NR);
    DSP_SetFeatureEntry(FUNC_TX_NR, Voice_EC_Init, Voice_EC_Prcs, get_aec_nr_memsize, Voice_PostEC_Prcs);
    DSP_SetFeatureEntry(FUNC_TX_EQ, Voice_TxEQ_init, Voice_TxEQ_Prcs, get_fir_wbeq_memsize, NULL);
    DSP_SetEcNrRefGainEntry(EC_REF_GAIN_READBACK, NULL);
    #else
    DSP_SetEcNrMode(EC_NR_COMMON_MODE);
    DSP_SetFeatureEntry(FUNC_TX_NR, Voice_WB_TX_Init, Voice_WB_TX_Prcs, get_aec_nr_memsize, AEC_NR_AirDump);
    DSP_SetEcNrRefGainEntry(EC_REF_GAIN_READBACK, NULL);
    #endif

    #ifdef AIROHA_WWE_ENABLE
    DSP_SetFeatureEntry(FUNC_WWE_PP_CON, WWE_PP_Init, WWE_PreProcess, NULL, NULL);
    DSP_SetFeatureEntry(FUNC_WWE_CON, WWE_P_Init, WWE_Process, NULL, NULL);
    #endif

    #ifdef AMAZON_AMA_ENABLE
    DSP_WWE_DATABASE_t AmaDbCtrl;
    AmaDbCtrl.RoDatabaseAddr = (U8*)ama_database;
    AmaDbCtrl.RoDatabaseSize = sizeof(ama_database);
    DSP_SetFeatureDatabase(FUNC_AMA, &AmaDbCtrl);
    DSP_SetFeatureEntry(FUNC_AMA, AMA_Interface_Init, AMA_Interface_Process, NULL, NULL);
    #endif

    #ifdef GOOGLE_GVA_ENABLE
    DSP_WWE_DATABASE_t GvaDbCtrl;
    GvaDbCtrl.RoDatabaseAddr = (U8*)gva_database_ro;
    GvaDbCtrl.RoDatabaseSize = sizeof(gva_database_ro);
    GvaDbCtrl.RwDatabaseAddr = (U8*)gva_database_rw;
    GvaDbCtrl.RwDatabaseSize = sizeof(gva_database_rw);
    DSP_SetFeatureDatabase(FUNC_GVA, &GvaDbCtrl);
    DSP_SetFeatureEntry(FUNC_GVA, GVA_Interface_Init, GVA_Interface_Process, NULL, NULL);
    #endif

    #ifdef MTK_LEAKAGE_DETECTION_ENABLE
    DSP_SetFeatureEntry(FUNC_LEAKAGE_DETECTION, Leakage_detection_Interface_Init, Leakage_detection_Interface_Process, NULL, NULL);
    #endif

    #if (AB1536)
    DSP_SetFeatureEntry(FUNC_AFC, AFC_Init, AFC_prcs, get_at_memsize, NULL);
    //DSP_SetFeatureEntry(FUNC_AT_NR, AT_NR_init, AT_NR_Prcs, get_at_nr_memsize, NULL);
    DSP_SetFeatureEntry(FUNC_AT_AGC, at_agc_init, at_agc_proc, get_at_agc_memsize, AGC_gain_change);
    #endif
}
