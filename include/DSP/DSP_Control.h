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
#ifndef _DSP_CONTROL_H_
#define _DSP_CONTROL_H_

#include "types.h"
#include "DSP_SDK.h"
#include "source_.h"
#include "sink_.h"
#include "message.h"


typedef enum AU_AUDIO_GAIN_COMPONENT_e
{
    AUDIO_GAIN_A2DP,
    AUDIO_GAIN_LINE,
    AUDIO_GAIN_SCO,
    AUDIO_GAIN_VC,
    AUDIO_GAIN_VP,
    AUDIO_GAIN_RT,
    AUDIO_GAIN_SCO_NB,
    AUDIO_GAIN_AT,
    AUDIO_GAIN_MAX_COMPONENT,

    // without gain table
    AUDIO_GAIN_ANC,
    AUDIO_GAIN_VAD,

} AUDIO_GAIN_COMPONENT_t;


typedef enum AU_AFE_OUT_GAIN_COMPONENT_e
{
    AU_AFE_OUT_COMPONENT_EAR_GAIN        = 0,
    AU_AFE_OUT_COMPONENT_DEPOP_GAIN      = 1,
    AU_AFE_OUT_COMPONENT_GAIN_INDEX      = 2,
    AU_AFE_OUT_COMPONENT_NO              = 3,
} AU_AFE_OUT_GAIN_COMPONENT_t;


typedef enum AU_AFE_IN_GAIN_COMPONENT_e
{
    AU_AFE_IN_COMPONENT_LINE_L           = 0,
    AU_AFE_IN_COMPONENT_LINE_R           = 1,
    AU_AFE_IN_COMPONENT_MIC_L            = 2,
    AU_AFE_IN_COMPONENT_MIC_R            = 3,
    AU_AFE_IN_COMPONENT_ANC_MIC_L        = 4,
    AU_AFE_IN_COMPONENT_ANC_MIC_R        = 5,
    AU_AFE_IN_COMPONENT_VAD_MIC          = 6,
    AU_AFE_IN_COMPONENT_NO               = 7,



} AU_AFE_IN_GAIN_COMPONENT_t;


typedef enum AU_DFE_IN_GAIN_COMPONENT_e
{
    AU_DFE_IN_COMPONENT_DIGITAL_GAIN0   = 0,
    AU_DFE_IN_COMPONENT_DIGITAL_GAIN1,
    AU_DFE_IN_COMPONENT_DIGITAL_GAIN2,
    AU_DFE_IN_COMPONENT_DIGITAL_GAIN3,
    AU_DFE_IN_COMPONENT_DIGITAL_GAIN_ECHO,
    AU_DFE_IN_COMPONENT_NO,
} AU_DFE_IN_GAIN_COMPONENT_t;

typedef enum DSP_AU_COMMAND_ANC_e
{
    AU_ANC_DISABLE,
    AU_ANC_FF,
    AU_ANC_FB,
    AU_ANC_HYBRID,
    AU_ANC_HYBRID_TWS,
} DSP_AU_COMMAND_ANC_t;

typedef enum DSP_AU_COMMAND_AT_e
{
    AU_AT_DISABLE,
    AU_AT_SIDETONE,
    AU_AT_mDSP,
    AU_AT_DSP,
    AU_AT_FEATURE_LIST,
    AU_AT_DATA_BYPASS_FROM_DSP,
} DSP_AU_COMMAND_AT_t;

typedef enum DSP_AU_COMMAND_VAD_e
{
    AU_VAD_DISABLE,
    AU_VAD_TRIGGER_MODE,
    AU_VAD_CONTINUOUS_MODE,
    AU_VAD_CONTINUOUS_MODE_AT_DRAM,
    AU_AMA_WITH_VAD_TRIGGER_MODE,
    AU_AMA_WITH_VAD_CONTINUOUS_MODE,
    AU_GVA_WITH_VAD_TRIGGER_MODE,
    AU_GVA_WITH_VAD_CONTINUOUS_MODE,
    AU_VAD_OPEN_FAIL                = 0xFFFFFFFF,
} DSP_AU_COMMAND_VAD_t;


typedef enum AU_AFE_SPK_VOLTAGE_e
{
    AU_AFE_SPK_VOL_3V               = 0,
    AU_AFE_SPK_VOL_1_8V             = 1,
    AU_AFE_SPK_VOL_1_6V             = 2,
} AU_AFE_SPK_VOLTAGE_t;


typedef enum MCE_PARA_SYNC_MODE_e
{
    MCE_NONE    = 0,
    MCE_VOICE   = 0x1000,
    MCE_VOICE_AECNR,

    MCE_AUDIO   = 0x2000,
    MCE_AUDIO_PEQ,
} MCE_PARA_SYNC_MODE_t;

typedef enum ADVANCED_AT_SAMPLE_RATE_e
{
    ADVANCED_AT_SAMPLE_RATE_16K_HZ  = 16,
    ADVANCED_AT_SAMPLE_RATE_32K_HZ  = 32,
    ADVANCED_AT_SAMPLE_RATE_48K_HZ  = 48,
    ADVANCED_AT_SAMPLE_RATE_TYPE    = 0xFFFFFFFF,
} ADVANCED_AT_SAMPLE_RATE_t;

typedef enum ADVANCED_AT_FRAME_SIZE_e
{
    ADVANCED_AT_FRAME_SIZE_8        = 8,
    ADVANCED_AT_FRAME_SIZE_16       = 16,
    ADVANCED_AT_FRAME_SIZE_32       = 32,
    ADVANCED_AT_FRAME_SIZE_64       = 64,
    ADVANCED_AT_FRAME_SIZE_TYPE     = 0xFFFFFFFF,
} ADVANCED_AT_FRAME_SIZE_t;




/** Align hal_audio_memory_sync_selection_t */
typedef enum {
    DSP_AU_SYNC_NONE                          = 0x0001, //0x0001, //HAL_AUDIO_MEMORY_SYNC_NONE,
    DSP_AU_SYNC_SW_TRIGGER                    = 0x0002, //0x0002, //HAL_AUDIO_MEMORY_SYNC_SW_TRIGGER,
    DSP_AU_SYNC_AUDIO_FORWARDER               = 0x0004, //0x0004, //HAL_AUDIO_MEMORY_SYNC_SW_TRIGGER,
    DSP_AU_SYNC_PLAY_EN                       = 0x0008, //0x0008, //HAL_AUDIO_MEMORY_SYNC_PLAY_EN,

    DSP_AU_SYNC_MULTIPLE_CHANNEl              = 0x0010, //0x0010, //HAL_AUDIO_MEMORY_SYNC_MULTIPLE_CHANNEl,
} DSP_AU_SYNC_CTRL_t;



typedef struct MDSP_MEMORY_IND_s
{
    U32 CONST *MDSP_PM_ADDR;
    U32 MDSP_PM_SIZE;
    U32 CONST *MDSP_DM_ADDR;
    U32 MDSP_DM_SIZE;
    U32 CONST *MDSP_CM_ADDR;
    U32 MDSP_CM_SIZE;
} MDSP_MEMORY_IND_t;

typedef enum MDSP_FEATURE_LIST_e
{
    MDSP_FEATURE_VAD                = 0,
    MDSP_FEATURE_ANC                = 1,
    MDSP_FEATURE_AT                 = 2,
} MDSP_FEATURE_LIST_t;

typedef enum DSP_EC_NR_MODE_e
{
   EC_NR_COMMON_MODE                = 0,
   EC_NR_IN_EAR_MODE                = 1,
   EC_NR_DUAL_MIC_IN_EAR_MODE       = 2,
   EC_NR_CUSTOMIZED_TX_NR           = 3,
} DSP_EC_NR_MODE_t;

typedef enum VAD_REPORT_STATUS_e
{
    VAD_STATUS_IDLE                 = 0,
    VAD_STATUS_BUSY                 = 1,
} VAD_REPORT_STATUS_t;

typedef enum VAD_THD_REPORT_e
{
    VAD_NOISE_LVL_UNKNOWN           = 0,
    VAD_NOISE_LVL_BELOW_THD         = 1,
    VAD_NOISE_LVL_OVER_THD          = 2,
} VAD_THD_REPORT_t;

typedef enum DSP_WWE_EVENT_e
{
    DSP_WWE_EVENT_NONE              = 0,
    DSP_WWE_EVENT_WWD               = 1,
    DSP_WWE_EVENT_ENV_IS_NOISY      = 2,
    DSP_WWE_EVENT_ENV_IS_SILENT     = 3,
} DSP_WWE_EVENT_t;

typedef enum WWE_MODE_e
{
   WWE_CON_MODE                     = 0,
   WWE_VAD_MODE                     = 1,
} WWE_MODE_t;

typedef enum DSP_HWGAIN_e
{
   DSP_HWGAIN_1                     = 1,
   DSP_HWGAIN_2                     = 2,
} DSP_HWGAIN_t;

typedef enum
{
    AUDIO_SOURCE_CHANGE_INPUT_LOOPBACK_DL1          = 1,
    AUDIO_SOURCE_CHANGE_INPUT_LOOPBACK_ANC_CH01     = 2,    //input is ANC path 0&1 for UL4
    AUDIO_SOURCE_CHANGE_INPUT_LOOPBACK_ANC_CH23     = 3,    //input is ANC path 2&3 for UL4
    AUDIO_SOURCE_CHANGE_INPUT_LOOPBACK_DL1_7BIT_SDM = 4,
} audio_source_change_input_t;

typedef struct MDSP_VAD_THD_REPORT_s
{
    VAD_THD_REPORT_t Status;
} MDSP_VAD_THD_REPORT_t;

typedef struct AU_VAD_FRAME_REPORT_s
{
    U16 WakeWordFrames;
    U16 PreRollFrames;
} AU_VAD_FRAME_REPORT_t;

typedef struct DSP_WWE_DATABASE_s
{
    U8* RoDatabaseAddr;
    U32 RoDatabaseSize;
    U8* RwDatabaseAddr;
    U32 RwDatabaseSize;
} DSP_WWE_DATABASE_t;

/* Digital Out Gain Control */
EXTERN S16 DSP_GC_GetDigitalOutLevel (AUDIO_GAIN_COMPONENT_t Component);
EXTERN VOID DSP_GC_SetDigitalOutLevel (AUDIO_GAIN_COMPONENT_t Component, S16 Gain);
EXTERN S16 DSP_GC_GetDigitalOutGain_AT (VOID);
EXTERN VOID DSP_GC_SetHwGain (DSP_HWGAIN_t HwGain, S32 GainVal);

/* Digital In Gain Control */
EXTERN S16 DSP_GC_GetDigitalInLevel (AU_DFE_IN_GAIN_COMPONENT_t gain_component);
EXTERN VOID DSP_GC_SetDigitalInLevel (AU_DFE_IN_GAIN_COMPONENT_t gain_component, S16 PercentageGain);
EXTERN VOID DSP_GC_SetDigitalInLevelByGainTable (AUDIO_GAIN_COMPONENT_t Component);
EXTERN VOID DSP_GC_LoadDigitalInLevelToDfe (VOID);

/* Analog Out Gain Control */
EXTERN S16 DSP_GC_GetAnalogOutLevel (VOID);
EXTERN VOID DSP_GC_SetAnalogOutLevel (S16 PercentageGain);
EXTERN VOID DSP_GC_SetAnalogOutScaleByGainTable (AUDIO_GAIN_COMPONENT_t Component);
EXTERN VOID DSP_GC_LoadAnalogOutLevelToAfe (VOID);

/* Analog In Gain Control */
EXTERN S16 DSP_GC_GetAnalogInLevel (AU_AFE_IN_GAIN_COMPONENT_t AfeComponent);
EXTERN VOID DSP_GC_SetAnalogInLevel (AU_AFE_IN_GAIN_COMPONENT_t AfeComponent, S16 Gain);
EXTERN VOID DSP_GC_SetAnalogInLevelByGainTable (AUDIO_GAIN_COMPONENT_t Component);
EXTERN VOID DSP_GC_LoadAnalogInLevelToAfe (AU_AFE_IN_GAIN_COMPONENT_t AfeComponent);
EXTERN VOID DSP_GC_LoadAnalogInLevelToAfeByScenario (AUDIO_GAIN_COMPONENT_t gain_component);
EXTERN VOID DSP_GC_LoadAnalogInLevelToAfeConcurrently (VOID);

/* Overall Gain Control */
EXTERN VOID DSP_GC_SetDefaultLevelByGainTable (AUDIO_GAIN_COMPONENT_t Component);
EXTERN VOID DSP_GC_LoadDefaultGainToAfe (AUDIO_GAIN_COMPONENT_t gain_component);
EXTERN VOID DSP_GC_UpdateAfeGains(AUDIO_GAIN_COMPONENT_t Component, S16 PercentageGain);
EXTERN VOID DSP_GC_SetAfeGainLevelByPercentage (S16 PercentageGain);

/* Mute Control */
EXTERN VOID DSP_GC_MuteAudioSource (BOOL Ctrl);
EXTERN VOID DSP_GC_MuteAudioSink (BOOL Ctrl);
EXTERN VOID DSP_GC_MuteSubAudioSink (BOOL Ctrl);

/* ANC, AT & VAD Control */
EXTERN DSP_AU_COMMAND_ANC_t DSP_AudioCommand_SetANC(DSP_AU_COMMAND_ANC_t command, VOID* ptr);
EXTERN DSP_AU_COMMAND_ANC_t DSP_AudioCommand_GetANCStatus(VOID);
EXTERN DSP_AU_COMMAND_AT_t DSP_AudioCommand_SetAT(DSP_AU_COMMAND_AT_t command, VOID* ptr);
EXTERN DSP_AU_COMMAND_AT_t DSP_AudioCommand_GetATStatus(VOID);
EXTERN DSP_AU_COMMAND_VAD_t DSP_AudioCommand_SetVAD(DSP_AU_COMMAND_VAD_t command, VOID* ptr, Handler handle);
EXTERN DSP_AU_COMMAND_VAD_t DSP_AudioCommand_GetVADStatus(VOID);
EXTERN VOID DSP_AudioCommand_ConfigAdvancedAT(ADVANCED_AT_SAMPLE_RATE_t Fs, ADVANCED_AT_FRAME_SIZE_t FrameSize);
EXTERN VAD_REPORT_STATUS_t DSP_VadStatusReport(VOID);
EXTERN VOID DSP_VadEventHandler(DSP_WWE_EVENT_t Evt, AU_VAD_FRAME_REPORT_t* FrameStatus);

/* EC NR Control */
EXTERN VOID DSP_AudioCommand_SetTxAecState(BOOL AvcDisable, BOOL TxNrDisable);

/* Ref Gain Control */
EXTERN VOID DSP_AudioCommand_GetRefGain(S16* GainAddr);

/* 2-mic Write Back Control */
EXTERN VOID DSP_AudioCommand_Get2micConvergeData(S16* Buf);

/* 2-mic Voice over cloud NR Parameter Check */
EXTERN BOOL DSP_AudioCommand_IsVaNr2micEnabled(VOID);

/* Audio 3-wire Control */
EXTERN VOID DSP_DRV_AFE_SetSpkVoltage (AU_AFE_SPK_VOLTAGE_t voltage);
EXTERN AU_AFE_SPK_VOLTAGE_t DSP_DRV_AFE_GetSpkVoltage (VOID);

/* Audio On/Off */
EXTERN VOID DSP_AudioCommand_AudioOff(VOID);
EXTERN VOID DSP_NotifyAudioDeinitialized(VOID);
EXTERN U32 DSP_CTRL_GetAfeDelayTimeMs(BOOL isPrompt);

/* Audio device rate*/
EXTERN FS_IN_OUT_RATE_enum_s AudioPeripheralInputSamplingRate_Get(SOURCE source);
EXTERN FS_IN_OUT_RATE_enum_s AudioPeripheralOutputSamplingRate_Get(SINK sink);
EXTERN  U8 DSP_UpDownRate2Value(AUDIO_STREAM_UPDOWN_RATE rate);
EXTERN  AUDIO_STREAM_UPDOWN_RATE DSP_UpValue2Rate(U8 value);
EXTERN  AUDIO_STREAM_UPDOWN_RATE DSP_DownValue2Rate(U8 value);

/* Audio Feature Control */
EXTERN VOID DSP_SetFeatureEntry(DSP_FEATURE_TYPE Feature, VOID* Init, VOID* Proc, VOID* Mem, VOID* Extra);
EXTERN VOID DSP_SetEcNrRefGainEntry(VOID* Entry, VOID* EntryInEar);
EXTERN VOID DSP_SetEcNrRevisionEntry(VOID* Entry);
EXTERN VOID DSP_SetMdspImageMemoryIndicator(MDSP_FEATURE_LIST_t Feature, MDSP_MEMORY_IND_t* Ind);
EXTERN VOID DSP_SetFeatureDatabase(DSP_FEATURE_TYPE Feature, DSP_WWE_DATABASE_t* DbCtrl);
EXTERN VOID DSP_SetEcNrMode(DSP_EC_NR_MODE_t Mode);
EXTERN DSP_EC_NR_MODE_t DSP_GetEcNrMode(VOID);

/* MCE related */
EXTERN void MCE_Init (void);
EXTERN void MCE_DeInit (void);
EXTERN VOID MCE_InitParaSync(MCE_PARA_SYNC_MODE_t sync_mode);
EXTERN VOID MCE_DisableParaSync(VOID);
EXTERN VOID MCE_TrigParaSync(VOID);
EXTERN U32  MCE_GetParaSyncStructSize(VOID);
EXTERN BOOL DSP_CommandSyncPara (U8* dataPtr);
EXTERN U32 SBC_GetParsedFrameSize (VOID);

/* MIC Bias Control */
EXTERN bool DSP_VAD_Bias_Enable (bool enable);
EXTERN VOID DSP_DRV_AFE_DisableAllBias (VOID);
EXTERN VOID DSP_DRV_AFE_SetInputPNFloat (VOID);

#endif /* _DSP_CONTROL_H_ */

