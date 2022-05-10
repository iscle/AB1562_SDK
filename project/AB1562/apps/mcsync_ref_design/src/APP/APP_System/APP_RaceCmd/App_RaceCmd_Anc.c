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
#include "AudioDSP_AncPassThru.h"
#include "drv_anc.h"
#include "DSP_ANC_Control.h"
#include "App_System.h"

#include "drv_calibration.h"


/******************************************************************************
 * Structure
*******************************************************************************/
typedef struct
{
    U8 isInit;
    U8 id;
    U8 syncMode;
    U8 channelId;
}ANC_PASSTHRU_REALTIME_CTL;

typedef struct
{
	U8 status;
	U8 ancId;
}PACKED APP_RACE_ANC_PASSTHRU_HEADER;

typedef struct
{
	APP_RACE_ANC_PASSTHRU_HEADER header;
	U8 flash_no;
	U8 ancType;
    U8 syncMode;
}PACKED APP_RACE_CMD_ANC_PASSTHRU_ON;

typedef struct
{
	APP_RACE_ANC_PASSTHRU_HEADER header;
	U8 flash_no;
	U8 ancType;
    U8 syncMode;
    U8 reserved;
}PACKED APP_RACE_RSP_ANC_PASSTHRU_ON;

typedef struct
{
	APP_RACE_ANC_PASSTHRU_HEADER header;
    U8 syncMode;
}PACKED APP_RACE_CMD_ANC_PASSTHRU_OFF;

typedef struct
{
	APP_RACE_ANC_PASSTHRU_HEADER header;
    U8 syncMode;
    U8 reserved[3];
}PACKED APP_RACE_RSP_ANC_PASSTHRU_OFF;

typedef struct
{
	APP_RACE_ANC_PASSTHRU_HEADER header;
    S16 gainFFl;
    S16 gainFBl;
    S16 gainFFr;
    S16 gainFBr;
    S16 gainSPKl;
    S16 gainSPKr;
}PACKED APP_RACE_CMD_ANC_PASSTHRU_SET_VOL;

typedef struct
{
	APP_RACE_ANC_PASSTHRU_HEADER header;
    S16 gainFFl;
    S16 gainFBl;
    S16 gainFFr;
    S16 gainFBr;
    S16 gainSPKl;
    S16 gainSPKr;
}PACKED APP_RACE_RSP_ANC_PASSTHRU_SET_VOL;

typedef struct
{
	APP_RACE_ANC_PASSTHRU_HEADER header;
}PACKED APP_RACE_CMD_ANC_PASSTHRU_READ_NVDM;

typedef struct
{
	APP_RACE_ANC_PASSTHRU_HEADER header;
    S16 gainFFl;
    S16 gainFBl;
    S16 gainFFr;
    S16 gainFBr;
    S16 gainSPKl;
    S16 gainSPKr;
}PACKED APP_RACE_RSP_ANC_PASSTHRU_READ_NVDM;

typedef struct
{
	APP_RACE_ANC_PASSTHRU_HEADER header;
    S16 gainFFl;
    S16 gainFBl;
    S16 gainFFr;
    S16 gainFBr;
    S16 gainSPKl;
    S16 gainSPKr;
}PACKED APP_RACE_CMD_ANC_PASSTHRU_WRITE_NVDM;

typedef struct
{
	APP_RACE_ANC_PASSTHRU_HEADER header;
    S16 gainFFl;
    S16 gainFBl;
    S16 gainFFr;
    S16 gainFBr;
    S16 gainSPKl;
    S16 gainSPKr;
}PACKED APP_RACE_RSP_ANC_PASSTHRU_WRITE_NVDM;

typedef struct
{
	APP_RACE_ANC_PASSTHRU_HEADER header;
    S16 gain;
    U8 syncMode;
}PACKED APP_RACE_CMD_ANC_PASSTHRU_SET_RUNTIME_VOL;

typedef struct
{
	APP_RACE_ANC_PASSTHRU_HEADER header;
    S16 gain;
    U8 syncMode;
    U8 reserved;
}PACKED APP_RACE_RSP_ANC_PASSTHRU_SET_RUNTIME_VOL;

typedef struct
{
	APP_RACE_ANC_PASSTHRU_HEADER header;
    U8 hybridEnable;
}PACKED APP_RACE_RSP_ANC_PASSTHRU_GET_HYBRID_CAP;

typedef struct
{
	APP_RACE_ANC_PASSTHRU_HEADER header;
    U8 ch;
    U8 data[1];
}PACKED APP_RACE_CMD_ANC_PASSTHRU_SET_COEF;

typedef struct
{
	APP_RACE_ANC_PASSTHRU_HEADER header;
}PACKED APP_RACE_RSP_ANC_PASSTHRU_SET_COEF;

typedef struct
{
	APP_RACE_ANC_PASSTHRU_HEADER header;
}PACKED APP_RACE_CMD_ANC_PASSTHRU_ENTER_MP_MODE;

typedef struct
{
	APP_RACE_ANC_PASSTHRU_HEADER header;
}PACKED APP_RACE_CMD_ANC_PASSTHRU_LEAVE_MP_MODE;

typedef struct
{
    APP_RACE_ANC_PASSTHRU_HEADER header;
    BOOL enable;
<<<<<<< HEAD
=======
    U8 low_pass_filter;
>>>>>>> db20e11 (second commit)
}PACKED APP_RACE_CMD_ANC_PASSTHRU_ENABLE_CALIBRATION;

typedef struct
{
    APP_RACE_ANC_PASSTHRU_HEADER header;
    BOOL enable;
<<<<<<< HEAD
=======
    U8 low_pass_filter;
>>>>>>> db20e11 (second commit)
}PACKED APP_RACE_RSP_ANC_PASSTHRU_ENABLE_CALIBRATION;

typedef struct
{
    APP_RACE_ANC_PASSTHRU_HEADER header;
    U8 mode;
}PACKED APP_RACE_CMD_ANC_PASSTHRU_SET_CALIBRATION;

typedef struct
{
    APP_RACE_ANC_PASSTHRU_HEADER header;
    U8 mode;
}PACKED APP_RACE_RSP_ANC_PASSTHRU_SET_CALIBRATION;

typedef struct
{
    APP_RACE_ANC_PASSTHRU_HEADER header;
<<<<<<< HEAD
=======
    U32 output_frequency;
    S16 output_volume;
}PACKED APP_RACE_CMD_ANC_PASSTHRU_SET_TONE;

typedef struct
{
    APP_RACE_ANC_PASSTHRU_HEADER header;
    U32 output_frequency;
    S16 output_volume;
}PACKED APP_RACE_RSP_ANC_PASSTHRU_SET_TONE;

typedef struct
{
    APP_RACE_ANC_PASSTHRU_HEADER header;
>>>>>>> db20e11 (second commit)
    U8 mode;
    U16 signal_power;
    U16 thd;
}PACKED APP_RACE_CMD_ANC_PASSTHRU_GET_CALIBRATION_RESULT;

typedef struct
{
    APP_RACE_ANC_PASSTHRU_HEADER header;
    U8 mode;
    S16 signal_power;
    S16 thd;
}PACKED APP_RACE_RSP_ANC_PASSTHRU_GET_CALIBRATION_RESULT;

typedef union APP_RACE_ANC_PASSTHRU
{
	APP_RACE_ANC_PASSTHRU_HEADER header;

	APP_RACE_CMD_ANC_PASSTHRU_ON onCmd;
    APP_RACE_CMD_ANC_PASSTHRU_OFF offCmd;
    APP_RACE_CMD_ANC_PASSTHRU_SET_VOL gainCmd;
    APP_RACE_CMD_ANC_PASSTHRU_READ_NVDM readNVDMCmd;
    APP_RACE_CMD_ANC_PASSTHRU_WRITE_NVDM writeNVDMCmd;
    APP_RACE_CMD_ANC_PASSTHRU_SET_RUNTIME_VOL runtimeGainCmd;
    APP_RACE_CMD_ANC_PASSTHRU_SET_COEF setCoefCmd;
	APP_RACE_CMD_ANC_PASSTHRU_ENTER_MP_MODE enterMpCmd;
	APP_RACE_CMD_ANC_PASSTHRU_LEAVE_MP_MODE leaveMpCmd;
    APP_RACE_CMD_ANC_PASSTHRU_ENABLE_CALIBRATION enableCalCmd;
    APP_RACE_CMD_ANC_PASSTHRU_SET_CALIBRATION setCalCmd;
    APP_RACE_CMD_ANC_PASSTHRU_GET_CALIBRATION_RESULT getCalResCmd;
<<<<<<< HEAD
=======
    APP_RACE_CMD_ANC_PASSTHRU_SET_TONE  setCalToneCmd;
>>>>>>> db20e11 (second commit)

    APP_RACE_RSP_ANC_PASSTHRU_ON onRsp;
    APP_RACE_RSP_ANC_PASSTHRU_OFF offRsp;
    APP_RACE_RSP_ANC_PASSTHRU_SET_VOL gainRsp;
    APP_RACE_RSP_ANC_PASSTHRU_READ_NVDM readNVDMRsp;
    APP_RACE_RSP_ANC_PASSTHRU_WRITE_NVDM writeNVDMRsp;
    APP_RACE_RSP_ANC_PASSTHRU_SET_RUNTIME_VOL runtimeGainRsp;
    APP_RACE_RSP_ANC_PASSTHRU_GET_HYBRID_CAP getHybridRsp;
    APP_RACE_RSP_ANC_PASSTHRU_SET_COEF setCoefRsp;
	APP_RACE_CMD_ANC_PASSTHRU_ENTER_MP_MODE enterMpRsp;
	APP_RACE_CMD_ANC_PASSTHRU_LEAVE_MP_MODE leaveMpRsp;
    APP_RACE_RSP_ANC_PASSTHRU_ENABLE_CALIBRATION enableCalRsp;
    APP_RACE_RSP_ANC_PASSTHRU_SET_CALIBRATION setCalRsp;
    APP_RACE_RSP_ANC_PASSTHRU_GET_CALIBRATION_RESULT getCalResRsp;
<<<<<<< HEAD
=======
    APP_RACE_RSP_ANC_PASSTHRU_SET_TONE setCalToneRsp;
>>>>>>> db20e11 (second commit)
}PACKED APP_RACE_ANC_PASSTHRU;


/******************************************************************************
 * Emun
*******************************************************************************/
enum
{
	REALTIME_ANCPASSTHRU_ON = 10,
	REALTIME_ANCPASSTHRU_OFF,
	REALTIME_ANCPASSTHRU_SET_VOL,
	REALTIME_ANCPASSTHRU_READ_NVDM,
	REALTIME_ANCPASSTHRU_WRITE_NVDM,
	REALTIME_ANCPASSTHRU_SET_RUNTIME_VOL, //0x0F
	REALTIME_ANCPASSTHRU_ENTER_MP_MODE, //0x10
	REALTIME_ANCPASSTHRU_LEAVE_MP_MODE, //0x11
    REALTIME_ANCPASSTHRU_GET_HYBRID_CAP = 22,
    REALTIME_ANCPASSTHRU_UPDATE_COEF = 23,
    REALTIME_ANCPASSTHRU_ENABLE_CALIBRATION = 24,
    REALTIME_ANCPASSTHRU_SET_CALIBRATION_CONTROL = 25,
    REALTIME_ANCPASSTHRU_GET_CALIBRATION_RESULT = 26,
<<<<<<< HEAD
=======
    REALTIME_ANCPASSTHRU_SET_OUTPUT_TONE = 27,
>>>>>>> db20e11 (second commit)
};


/******************************************************************************
 * Variable
*******************************************************************************/
ANC_PASSTHRU_REALTIME_CTL gANCPassThruRealTimeCtl;


void app_RaceCmd_Anc_OnCallback(anc_control_event_t event_id, anc_control_result_t result)
{
    APP_RACE_ANC_PASSTHRU rsp;
    if(event_id == ANC_CONTROL_EVENT_ON && gANCPassThruRealTimeCtl.id == REALTIME_ANCPASSTHRU_ON)
    {
        rsp.onRsp.header.status = (result >= ANC_CONTROL_EXECUTION_SUCCESS)  ? 0 : result;
        rsp.onRsp.header.ancId = REALTIME_ANCPASSTHRU_ON;
        rsp.onRsp.flash_no = drv_anc_get_flash_no();
        rsp.onRsp.ancType = drv_anc_get_type();
        rsp.onRsp.syncMode =  gANCPassThruRealTimeCtl.syncMode;
        RACE_Cmd_RealTime_ANC_Resp(gANCPassThruRealTimeCtl.channelId, (U8 *)&rsp, sizeof(APP_RACE_RSP_ANC_PASSTHRU_ON));        
    }
    else if(event_id == ANC_CONTROL_EVENT_OFF && gANCPassThruRealTimeCtl.id == REALTIME_ANCPASSTHRU_OFF)
    {
        rsp.offRsp.header.status = (result >= ANC_CONTROL_EXECUTION_SUCCESS)  ? 0 : result;
        rsp.offRsp.header.ancId = REALTIME_ANCPASSTHRU_OFF;
        rsp.offRsp.syncMode =  gANCPassThruRealTimeCtl.syncMode;
        RACE_Cmd_RealTime_ANC_Resp(gANCPassThruRealTimeCtl.channelId, (U8 *)&rsp, sizeof(APP_RACE_RSP_ANC_PASSTHRU_OFF));        
    }
}


static void app_RaceCmd_Anc_OnHandler(APP_RACE_ANC_PASSTHRU *pData)
{
    APP_ANCPASSTHRU_PARA_STRU para;
    U8 ancType;

<<<<<<< HEAD
=======
    memset(&para, 0, sizeof(APP_ANCPASSTHRU_PARA_STRU));
>>>>>>> db20e11 (second commit)
    if(!gANCPassThruRealTimeCtl.isInit)
    {
        audio_anc_passthru_register_callback(app_RaceCmd_Anc_OnCallback, ANC_CONTROL_EVENT_ON|ANC_CONTROL_EVENT_OFF, CB_LEVEL_ALL);
        gANCPassThruRealTimeCtl.isInit = TRUE;
    }

    gANCPassThruRealTimeCtl.id = REALTIME_ANCPASSTHRU_ON;
    gANCPassThruRealTimeCtl.syncMode = pData->onCmd.syncMode;

    ancType = pData->onCmd.ancType;

    para.action = ACTION_ON;
    para.sync = pData->onCmd.syncMode;
    para.flash_no = pData->onCmd.flash_no;
    para.type = ancType;

    if(ancType == HAL_AUDIO_PASSTHRU_TYPE_FF)
    {
        para.userRegister = USER_PASSTHRU_OPEN;
        audio_passthru_get_runtime_gain(&para.runtime_gain);
    }
    else if(ancType < HAL_AUDIO_PASSTHRU_TYPE_FF)
    {

        para.userRegister = USER_ANC_OPEN;
        audio_anc_get_runtime_gain(&para.runtime_gain);
    }

    AudioDsp_ANCPassThruHandler(para);
}

static void app_RaceCmd_Anc_OffHandler(APP_RACE_ANC_PASSTHRU *pData)
{
    APP_ANCPASSTHRU_PARA_STRU para;
<<<<<<< HEAD
    
=======
    memset(&para, 0, sizeof(APP_ANCPASSTHRU_PARA_STRU));
>>>>>>> db20e11 (second commit)
    if(!gANCPassThruRealTimeCtl.isInit)
    {
        audio_anc_passthru_register_callback(app_RaceCmd_Anc_OnCallback, ANC_CONTROL_EVENT_ON|ANC_CONTROL_EVENT_OFF, CB_LEVEL_ALL);
        gANCPassThruRealTimeCtl.isInit = TRUE;
    }

    if(drv_anc_is_playing())
    {
        gANCPassThruRealTimeCtl.id = REALTIME_ANCPASSTHRU_OFF;
        gANCPassThruRealTimeCtl.syncMode = pData->offCmd.syncMode;

        para.action = ACTION_OFF;
        para.sync = pData->offCmd.syncMode;
        AudioDsp_ANCPassThruHandler(para);
    }
    else
    {
        APP_RACE_ANC_PASSTHRU rsp;

        rsp.offRsp.header.status = 0;
        rsp.offRsp.header.ancId = REALTIME_ANCPASSTHRU_OFF;
        rsp.offRsp.syncMode =  pData->offCmd.syncMode;
        RACE_Cmd_RealTime_ANC_Resp(gANCPassThruRealTimeCtl.channelId, (U8 *)&rsp, sizeof(APP_RACE_RSP_ANC_PASSTHRU_OFF));
    }
}

<<<<<<< HEAD
static void app_RaceCmd_Anc_SetVolHandler(APP_RACE_ANC_PASSTHRU *pData)
{
    APP_RACE_ANC_PASSTHRU rsp;

    rsp.gainRsp.header.status = audio_anc_passthru_set_dg_all_gain(pData->gainCmd.gainFFl, pData->gainCmd.gainFBl, pData->gainCmd.gainSPKl, pData->gainCmd.gainSPKr);
=======
static void app_RaceCmd_Anc_SetVolHandler(APP_RACE_ANC_PASSTHRU *pData, U16 length)
{
    APP_RACE_ANC_PASSTHRU rsp;
    if (length == sizeof(APP_RACE_RSP_ANC_PASSTHRU_SET_VOL)) {
        rsp.gainRsp.gainSPKl = pData->gainCmd.gainSPKl;
        rsp.gainRsp.gainSPKr = pData->gainCmd.gainSPKr;
    } else {
        rsp.gainRsp.gainSPKl = 0;
        rsp.gainRsp.gainSPKr = 0;
    }
    rsp.gainRsp.header.status = audio_anc_passthru_set_dg_all_gain(pData->gainCmd.gainFFl, pData->gainCmd.gainFBl, rsp.gainRsp.gainSPKl, rsp.gainRsp.gainSPKr);
>>>>>>> db20e11 (second commit)
    rsp.gainRsp.header.ancId = REALTIME_ANCPASSTHRU_SET_VOL;
    rsp.gainRsp.gainFFl = pData->gainCmd.gainFFl;
    rsp.gainRsp.gainFBl = pData->gainCmd.gainFBl;
    rsp.gainRsp.gainFFr = pData->gainCmd.gainFFr;
    rsp.gainRsp.gainFBr = pData->gainCmd.gainFBr;
<<<<<<< HEAD
    rsp.gainRsp.gainSPKl = pData->gainCmd.gainSPKl;
    rsp.gainRsp.gainSPKr = pData->gainCmd.gainSPKr;
    RACE_Cmd_RealTime_ANC_Resp(gANCPassThruRealTimeCtl.channelId, (U8 *)&rsp, sizeof(APP_RACE_RSP_ANC_PASSTHRU_SET_VOL));
}

static void app_RaceCmd_Anc_ReadNVDMHandler(APP_RACE_ANC_PASSTHRU *pData)
=======
    RACE_Cmd_RealTime_ANC_Resp(gANCPassThruRealTimeCtl.channelId, (U8 *)&rsp, length);
}

static void app_RaceCmd_Anc_ReadNVDMHandler(APP_RACE_ANC_PASSTHRU *pData, U16 length)
>>>>>>> db20e11 (second commit)
{
    APP_RACE_ANC_PASSTHRU rsp;
    memset(&rsp, 0, sizeof(APP_RACE_ANC_PASSTHRU));
    UNUSED(pData);
<<<<<<< HEAD
=======
    UNUSED(length);
>>>>>>> db20e11 (second commit)
    rsp.readNVDMRsp.header.status = audio_anc_passthru_read_dg_gain(&rsp.readNVDMRsp.gainFFl, &rsp.readNVDMRsp.gainFBl);
    rsp.readNVDMRsp.header.status |= audio_anc_passthru_read_dg_speaker_gain(&rsp.readNVDMRsp.gainSPKl, &rsp.readNVDMRsp.gainSPKr);
    rsp.readNVDMRsp.header.ancId = REALTIME_ANCPASSTHRU_READ_NVDM;
    RACE_Cmd_RealTime_ANC_Resp(gANCPassThruRealTimeCtl.channelId, (U8 *)&rsp, sizeof(APP_RACE_RSP_ANC_PASSTHRU_READ_NVDM));
}

<<<<<<< HEAD
static void app_RaceCmd_Anc_WriteNVDMHandler(APP_RACE_ANC_PASSTHRU *pData)
{
    APP_RACE_ANC_PASSTHRU rsp;

    rsp.writeNVDMRsp.header.status = audio_anc_passthru_write_dg_gain(pData->writeNVDMCmd.gainFFl, pData->writeNVDMCmd.gainFBl);
    rsp.writeNVDMRsp.header.status |= audio_anc_passthru_write_dg_speaker_gain(pData->writeNVDMCmd.gainSPKl, pData->writeNVDMCmd.gainSPKr);
=======
static void app_RaceCmd_Anc_WriteNVDMHandler(APP_RACE_ANC_PASSTHRU *pData, U16 length)
{
    APP_RACE_ANC_PASSTHRU rsp;
    if (length == sizeof(APP_RACE_RSP_ANC_PASSTHRU_WRITE_NVDM)) {
        rsp.writeNVDMRsp.gainSPKl = pData->writeNVDMCmd.gainSPKl;
        rsp.writeNVDMRsp.gainSPKr = pData->writeNVDMCmd.gainSPKr;
    } else {
        rsp.writeNVDMRsp.gainSPKl = 0;
        rsp.writeNVDMRsp.gainSPKr = 0;
    }
    rsp.writeNVDMRsp.header.status = audio_anc_passthru_write_dg_gain(pData->writeNVDMCmd.gainFFl, pData->writeNVDMCmd.gainFBl);
    rsp.writeNVDMRsp.header.status |= audio_anc_passthru_write_dg_speaker_gain(rsp.writeNVDMRsp.gainSPKl, rsp.writeNVDMRsp.gainSPKr);
>>>>>>> db20e11 (second commit)
    rsp.writeNVDMRsp.header.ancId = REALTIME_ANCPASSTHRU_WRITE_NVDM;
    rsp.writeNVDMRsp.gainFFl = pData->writeNVDMCmd.gainFFl;
    rsp.writeNVDMRsp.gainFBl = pData->writeNVDMCmd.gainFBl;
    rsp.writeNVDMRsp.gainFFr = pData->writeNVDMCmd.gainFFr;
    rsp.writeNVDMRsp.gainFBr = pData->writeNVDMCmd.gainFBr;
<<<<<<< HEAD
    rsp.writeNVDMRsp.gainSPKl = pData->writeNVDMCmd.gainSPKl;
    rsp.writeNVDMRsp.gainSPKr = pData->writeNVDMCmd.gainSPKr;

    RACE_Cmd_RealTime_ANC_Resp(gANCPassThruRealTimeCtl.channelId, (U8 *)&rsp, sizeof(APP_RACE_RSP_ANC_PASSTHRU_WRITE_NVDM));
=======

    RACE_Cmd_RealTime_ANC_Resp(gANCPassThruRealTimeCtl.channelId, (U8 *)&rsp, length);
>>>>>>> db20e11 (second commit)
}

static void app_RaceCmd_Anc_SetRealtimeVolHandler(APP_RACE_ANC_PASSTHRU *pData)
{
    APP_RACE_ANC_PASSTHRU rsp;
    APP_ANCPASSTHRU_PARA_STRU para;
    S16 runtime_gain = pData->runtimeGainCmd.gain;
<<<<<<< HEAD
    
=======

    memset(&para, 0, sizeof(APP_ANCPASSTHRU_PARA_STRU));
>>>>>>> db20e11 (second commit)
    para.action = ACTION_SET_VOL;
    para.sync = pData->runtimeGainCmd.syncMode;
    para.runtime_gain = runtime_gain;
    rsp.runtimeGainRsp.header.status = AudioDsp_ANCPassThruHandler(para);
    rsp.runtimeGainRsp.header.ancId = REALTIME_ANCPASSTHRU_SET_RUNTIME_VOL;
    rsp.runtimeGainRsp.gain = runtime_gain;
    rsp.runtimeGainRsp.syncMode = pData->runtimeGainCmd.syncMode;
    
    RACE_Cmd_RealTime_ANC_Resp(gANCPassThruRealTimeCtl.channelId, (U8 *)&rsp, sizeof(APP_RACE_RSP_ANC_PASSTHRU_SET_RUNTIME_VOL));
}

static void app_RaceCmd_Anc_GetHybridCapHandler(APP_RACE_ANC_PASSTHRU *pData)
{
    APP_RACE_ANC_PASSTHRU rsp;
    rsp.getHybridRsp.header.status = 0;
    rsp.getHybridRsp.header.ancId = REALTIME_ANCPASSTHRU_GET_HYBRID_CAP;
    rsp.getHybridRsp.hybridEnable = audio_anc_passthru_get_hybrid_cap();
    RACE_Cmd_RealTime_ANC_Resp(gANCPassThruRealTimeCtl.channelId, (U8 *)&rsp, sizeof(APP_RACE_RSP_ANC_PASSTHRU_GET_HYBRID_CAP));
    UNUSED(pData);
}

static void app_RaceCmd_Anc_UpdateCoefHandler(APP_RACE_ANC_PASSTHRU *pData, U16 length)
{
    APP_RACE_ANC_PASSTHRU rsp;
    length -= 3;
    U8 * ptr = FW_GetMemory(length);
    if(ptr)
    {
        FW_Memcpy(ptr, &pData->setCoefCmd.data[0], length);
        rsp.setCoefRsp.header.status = audio_anc_passthru_realtime_update_coefficient(pData->setCoefCmd.ch, ptr /*&pData->setCoefCmd.data[0]*/, length);
        rsp.setCoefRsp.header.ancId = REALTIME_ANCPASSTHRU_UPDATE_COEF;
        FW_FreeMemory(ptr);
        RACE_Cmd_RealTime_ANC_Resp(gANCPassThruRealTimeCtl.channelId, (U8 *)&rsp, sizeof(APP_RACE_RSP_ANC_PASSTHRU_SET_COEF));
    }
}

static void app_RaceCmd_Anc_EnterMpModeHandler(APP_RACE_ANC_PASSTHRU *pData)
{
<<<<<<< HEAD
=======
	DSP_GC_MuteAudioSink(FALSE);

>>>>>>> db20e11 (second commit)
    APP_RACE_ANC_PASSTHRU rsp;
    APP_ANCPassThru_SetMpMode(TRUE);
    rsp.enterMpRsp.header.status = 0;
    rsp.enterMpRsp.header.ancId = REALTIME_ANCPASSTHRU_ENTER_MP_MODE;
    RACE_Cmd_RealTime_ANC_Resp(gANCPassThruRealTimeCtl.channelId, (U8 *)&rsp, sizeof(APP_RACE_CMD_ANC_PASSTHRU_ENTER_MP_MODE));
	UNUSED(pData);	
}

static void app_RaceCmd_Anc_LeaveMpModeHandler(APP_RACE_ANC_PASSTHRU *pData)
{
    APP_RACE_ANC_PASSTHRU rsp;
	APP_ANCPassThru_SetMpMode(FALSE);
    rsp.leaveMpRsp.header.status = 0;
    rsp.leaveMpRsp.header.ancId = REALTIME_ANCPASSTHRU_LEAVE_MP_MODE;
	RACE_Cmd_RealTime_ANC_Resp(gANCPassThruRealTimeCtl.channelId, (U8 *)&rsp, sizeof(APP_RACE_CMD_ANC_PASSTHRU_LEAVE_MP_MODE));
	UNUSED(pData);
}

static void app_RaceCmd_Anc_EnableCalibration(APP_RACE_ANC_PASSTHRU *pData)
{
    APP_RACE_ANC_PASSTHRU rsp;
    BOOL control_status;
#ifdef AUDIO_CALIBRATION_ENABLE
    if(pData->enableCalCmd.enable) {
<<<<<<< HEAD
=======
        drv_calibration_set_low_pass_filter(pData->enableCalCmd.low_pass_filter);
>>>>>>> db20e11 (second commit)
        control_status = drv_calibration_start();
    } else {
        control_status = drv_calibration_stop(false);
    }
#else
    control_status = false;
#endif
    rsp.enableCalRsp.header.status = (control_status) ? false : true;
    rsp.enableCalRsp.header.ancId = REALTIME_ANCPASSTHRU_ENABLE_CALIBRATION;
    rsp.enableCalRsp.enable = pData->enableCalCmd.enable;
<<<<<<< HEAD
=======
    rsp.enableCalRsp.low_pass_filter = pData->enableCalCmd.low_pass_filter;
>>>>>>> db20e11 (second commit)
    RACE_Cmd_RealTime_ANC_Resp(gANCPassThruRealTimeCtl.channelId, (U8 *)&rsp, sizeof(APP_RACE_RSP_ANC_PASSTHRU_ENABLE_CALIBRATION));
}

static void app_RaceCmd_Anc_SetCalibrationMode(APP_RACE_ANC_PASSTHRU *pData)
{
    APP_RACE_ANC_PASSTHRU rsp;
    BOOL control_status = TRUE;
#ifdef AUDIO_CALIBRATION_ENABLE
    control_status = drv_calibration_set_mode(pData->setCalCmd.mode);
#endif
    rsp.setCalRsp.header.status = control_status;
    rsp.setCalRsp.header.ancId = REALTIME_ANCPASSTHRU_SET_CALIBRATION_CONTROL;
    rsp.setCalRsp.mode = pData->setCalCmd.mode;
    RACE_Cmd_RealTime_ANC_Resp(gANCPassThruRealTimeCtl.channelId, (U8 *)&rsp, sizeof(APP_RACE_CMD_ANC_PASSTHRU_SET_CALIBRATION));
}

static void app_RaceCmd_Anc_GetCalibrationResult(APP_RACE_ANC_PASSTHRU *pData)
{
    APP_RACE_ANC_PASSTHRU rsp;
    S32 result = 0;
    BOOL status = TRUE;
#ifdef AUDIO_CALIBRATION_ENABLE
    result = drv_calibration_get_result();
    status = false;
#endif
    UNUSED(pData);
    rsp.getCalResRsp.header.status = status;
    rsp.getCalResRsp.header.ancId = REALTIME_ANCPASSTHRU_GET_CALIBRATION_RESULT;
    rsp.getCalResRsp.signal_power = (S16)result;
    rsp.getCalResRsp.thd = 0;
    RACE_Cmd_RealTime_ANC_Resp(gANCPassThruRealTimeCtl.channelId, (U8 *)&rsp, sizeof(APP_RACE_RSP_ANC_PASSTHRU_GET_CALIBRATION_RESULT));
<<<<<<< HEAD
}

=======
    DBG_LOG_APP_SYSTEM("[APP RaceCmd ANC] app_RaceCmd_Anc_GetCalibrationResult %d", 1, result);
}

static void app_RaceCmd_Anc_SetCalibrationOutputTone(APP_RACE_ANC_PASSTHRU *pData)
{
    APP_RACE_ANC_PASSTHRU rsp;
    BOOL control_status = TRUE;
#ifdef AUDIO_CALIBRATION_ENABLE
    control_status = drv_calibration_set_output_frequency(pData->setCalToneCmd.output_frequency, pData->setCalToneCmd.output_volume);
#endif
    rsp.setCalToneRsp.header.status = control_status;
    rsp.setCalToneRsp.header.ancId = REALTIME_ANCPASSTHRU_SET_OUTPUT_TONE;
    rsp.setCalToneRsp.output_frequency = pData->setCalToneCmd.output_frequency;
    rsp.setCalToneRsp.output_volume = pData->setCalToneCmd.output_volume;
    RACE_Cmd_RealTime_ANC_Resp(gANCPassThruRealTimeCtl.channelId, (U8 *)&rsp, sizeof(APP_RACE_RSP_ANC_PASSTHRU_SET_TONE));
}
>>>>>>> db20e11 (second commit)

static void app_RaceCmd_Anc_ErrorHandler(APP_RACE_ANC_PASSTHRU *pData)
{
    APP_RACE_ANC_PASSTHRU rsp;
    rsp.header.status = 1;
    rsp.header.ancId = pData->header.ancId;
    RACE_Cmd_RealTime_ANC_Resp(gANCPassThruRealTimeCtl.channelId, (U8 *)&rsp, sizeof(APP_RACE_ANC_PASSTHRU_HEADER));
}


static void app_RaceCmd_Anc_Handler(U8 channelId, U16 length, U8* pMsg)
{
	APP_RACE_ANC_PASSTHRU * pData = (APP_RACE_ANC_PASSTHRU *)pMsg;

	DBG_LOG_APP_SYSTEM("[APP RaceCmd ANC] app_RaceCmd_Anc_Handler data:%x %x %x %x %x %x", 6, *(pMsg+0),*(pMsg+1),*(pMsg+2),*(pMsg+3),*(pMsg+4),*(pMsg+5));
    
    gANCPassThruRealTimeCtl.id = pData->header.ancId;
    gANCPassThruRealTimeCtl.channelId = channelId;

	DBG_LOG_APP_SYSTEM("[APP RaceCmd ANC] app_RaceCmd_Anc_Handler anc id:%d, channel id:%d", 2, pData->header.ancId, channelId);
  
	switch(pData->header.ancId)
	{
		case REALTIME_ANCPASSTHRU_ON:
            app_RaceCmd_Anc_OnHandler(pData);
			break;

		case REALTIME_ANCPASSTHRU_OFF:
            app_RaceCmd_Anc_OffHandler(pData);
			break;

		case REALTIME_ANCPASSTHRU_SET_VOL:
<<<<<<< HEAD
            app_RaceCmd_Anc_SetVolHandler(pData);
			break;

		case REALTIME_ANCPASSTHRU_READ_NVDM:
            app_RaceCmd_Anc_ReadNVDMHandler(pData);
			break;

		case REALTIME_ANCPASSTHRU_WRITE_NVDM:
            app_RaceCmd_Anc_WriteNVDMHandler(pData);
=======
            app_RaceCmd_Anc_SetVolHandler(pData, length);
			break;

		case REALTIME_ANCPASSTHRU_READ_NVDM:
            app_RaceCmd_Anc_ReadNVDMHandler(pData, length);
			break;

		case REALTIME_ANCPASSTHRU_WRITE_NVDM:
            app_RaceCmd_Anc_WriteNVDMHandler(pData, length);
>>>>>>> db20e11 (second commit)
			break;

		case REALTIME_ANCPASSTHRU_SET_RUNTIME_VOL:
            app_RaceCmd_Anc_SetRealtimeVolHandler(pData);
			break;

        case REALTIME_ANCPASSTHRU_GET_HYBRID_CAP:
            app_RaceCmd_Anc_GetHybridCapHandler(pData);
            break;
            
        case REALTIME_ANCPASSTHRU_UPDATE_COEF:
            app_RaceCmd_Anc_UpdateCoefHandler(pData, length);
            break;

		case REALTIME_ANCPASSTHRU_ENTER_MP_MODE:
			app_RaceCmd_Anc_EnterMpModeHandler(pData);
			break;

		case REALTIME_ANCPASSTHRU_LEAVE_MP_MODE:
			app_RaceCmd_Anc_LeaveMpModeHandler(pData);
			break;
        case REALTIME_ANCPASSTHRU_ENABLE_CALIBRATION:
            app_RaceCmd_Anc_EnableCalibration(pData);
            break;
        case REALTIME_ANCPASSTHRU_SET_CALIBRATION_CONTROL:
            app_RaceCmd_Anc_SetCalibrationMode(pData);
            break;
        case REALTIME_ANCPASSTHRU_GET_CALIBRATION_RESULT:
            app_RaceCmd_Anc_GetCalibrationResult(pData);
            break;
<<<<<<< HEAD
=======
        case REALTIME_ANCPASSTHRU_SET_OUTPUT_TONE:
            app_RaceCmd_Anc_SetCalibrationOutputTone(pData);
            break;
>>>>>>> db20e11 (second commit)
		default:
            app_RaceCmd_Anc_ErrorHandler(pData);
            break;
	}
}

void APP_RaceCmd_Anc_Init(void)
{
	RACE_RegisterANCPassThruCb(app_RaceCmd_Anc_Handler);
}
