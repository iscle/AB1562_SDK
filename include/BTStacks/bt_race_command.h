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

#ifndef _BT_RACE_COMMAND_H_
#define _BT_RACE_COMMAND_H_

#include "message.h"
#include "types.h"
#include "dsp_para_anc_nvstruc.h"


/**************************************************************************************************
* Enumeration
**************************************************************************************************/
/*Race Command to APP ID*/
enum
{
    MSG_ID_RACE_TO_APP_KEYCODE_WITH_BDADDR,
    MSG_ID_RACE_TO_APP_KEYCODE,
    MSG_ID_RACE_TO_APP_ANC_ON,
    MSG_ID_RACE_TO_APP_ANC_OFF,
    MSG_ID_RACE_TO_APP_ANC_GET_STATUS,
    MSG_ID_RACE_TO_APP_ANC_SET_GAIN,
    MSG_ID_RACE_TO_APP_ANC_READ_PARA,
    MSG_ID_RACE_TO_APP_ANC_WRITE_PARA,
    MSG_ID_RACE_TO_APP_CONNECTION_DISCONNECT_BT30,
    MSG_ID_RACE_TO_APP_CONNECTION_ENABLE_PTS_TEST,
    MSG_ID_RACE_TO_APP_AIRDUMP_CMD,
    MSG_ID_RACE_TO_APP_FCD_CMD,
    MSG_ID_RACE_TO_APP_FOTA_START,
    MSG_ID_RACE_TO_APP_FOTA_STOP,
    MSG_ID_RACE_TO_APP_RELOAD_NVKEY,
    MSG_ID_RACE_TO_APP_SET_CUSTOMER_KEY,
    MSG_ID_RACE_TO_APP_GET_CUSTOMER_KEY,
    MSG_ID_RACE_TO_APP_GET_EAR_STATUS_CMD,
    MSG_ID_RACE_TO_APP_SET_EARDETECT_ON_OFF_CMD,
    MSG_ID_RACE_TO_APP_GET_EARDETECT_ON_OFF_CMD
};

/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef void (*RACE_ANCPassThru_Cb)(U8 channelId, U16 length, U8* pMsg);

typedef struct BT_APP_DSP_REALTIME_SUSPEND_DSP_T
{
	U8 test;
}BT_APP_DSP_REALTIME_SUSPEND_DSP_T;

typedef struct BT_APP_DSP_REALTIME_RESUME_DSP_T
{
	U8 test;
}BT_APP_DSP_REALTIME_RESUME_DSP_T;

typedef struct BT_APP_DSP_SOUND_EFFECT_UPDATE_T
{
	U16 length;
	U8 data[1];
}BT_APP_DSP_SOUND_EFFECT_UPDATE_T;

typedef struct BT_APP_DSP_PEQ_UPDATE_T
{
	U16 length;
	U8 data[1];
}BT_APP_DSP_PEQ_UPDATE_T;

typedef struct BT_APP_DSP_PEQ_SAVE_STATUS_T
{
	U8 test;
}BT_APP_DSP_PEQ_SAVE_STATUS_T;

typedef struct
{
	U8 channelId;
	U16 keyCode;
	BD_ADDR_T bdAddr;
}MSG_ID_RACE_TO_APP_KEYCODE_WITH_BDADDR_T;

typedef struct
{
	U8 channelId;
	U16 keyCode;
}MSG_ID_RACE_TO_APP_KEYCODE_T;

typedef struct
{
	U8 channelId;
	U8 type;
}MSG_ID_RACE_TO_APP_ANC_ON_T;

typedef struct
{
	U8 channelId;
}MSG_ID_RACE_TO_APP_ANC_OFF_T;

typedef struct
{
	U8 channelId;
}MSG_ID_RACE_TO_APP_ANC_GET_STATUS_T;

typedef struct
{
	U8 channelId;
	U8 component;
	S8 gain;
}MSG_ID_RACE_TO_APP_ANC_SET_GAIN_T;

typedef struct
{
	U8 channelId;
}MSG_ID_RACE_TO_APP_ANC_READ_PARA_T;

typedef struct
{
	U8 channelId;
	S8 gainL;
	S8 gainR;
}MSG_ID_RACE_TO_APP_ANC_WRITE_PARA_T;

typedef struct
{
	U8 channelId;
	BD_ADDR_T bdAddr;
}MSG_ID_RACE_TO_APP_CONNECTION_DISCONNECT_BT30_T;

typedef struct
{
    U8 is_enable;
}MSG_ID_RACE_TO_APP_CONNECTION_ENABLE_PTS_TEST_T;

typedef struct
{
	U8 channelId;
	U8 status;
}MSG_ID_RACE_TO_APP_AIRDUMP_CMD_T;

typedef struct
{
	U8 channelId;
	U16 keyCode;
	U8 type;
}MSG_ID_RACE_TO_APP_FCD_CMD_T;

typedef struct MSG_ID_RACE_TO_APP_SET_CUSTOMER_KEY_T
{
	U16 length;
	U8 data[1];
}MSG_ID_RACE_TO_APP_SET_CUSTOMER_KEY_T;

typedef struct MSG_ID_RACE_TO_APP_GET_CUSTOMER_KEY_T
{
	U8 channelId;
}MSG_ID_RACE_TO_APP_GET_CUSTOMER_KEY_T;

typedef struct
{
	U8 channelId;
	U8 mode;
	U8 is_enable;
}BT_APP_DSP_REALTIME_ANC_ADAPTIVE_CHECK_T;

typedef struct
{
    U8 channelId;
    U8 role;
    U16 keyCode;
}MSG_ID_RACE_TO_APP_GET_EAR_STATUS_CMD_T;

typedef struct
{
    U8 channelId;
    U8 isOn;
    U16 keycode;
}MSG_ID_RACE_TO_APP_SET_EARDETECT_ON_OFF_CMD_T;

typedef struct
{
    U8 channelId;
    U16 keycode;
}MSG_ID_RACE_TO_APP_GET_EARDETECT_ON_OFF_CMD_T;

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void BtRaceCommand_RealTime_RegisterHandler(Handler app);
void RACE_App_KeyCode_RegisterHandler(Handler handler);
void RACE_App_Anc_RegisterHandler(Handler handler);
void RACE_App_Connection_RegisterHandler(Handler handler);
void RACE_App_Fota_RegisterHandler(Handler handler);

void RACE_Anc_Status_Notification(U8 channelId, U8 status);
void RACE_Anc_Para_Notification(U8 channelId, DSP_PARA_ANC_STRU para);

void RACE_AudioCmd_Get_RefGain_Notification(U8 channelId);
void RACE_AudioCmd_Get_TwoMICPara_Notification(U8 channelId);
void RACE_Cmd_RealTime_ANC_Resp(U8 channelId, U8 * pData, U16 length);
void race_cmd_audio_bist_send_test_result_notification(U8 channelId, S32 SNP, S32 THD_N, U32 freq);
void race_cmd_audio_bist_send_test_result_notification_new_version(U8 channelId, S32 THD_N, S32 SNP, S32 TOTAL_POW);

void RACE_AirDump_RegisterAppHandler(Handler handler);
void RACE_AirDump_Send(U8 channelId,U8 RaceType, U8 *ptr, U16 len);

void RACE_FCD_RegisterAppHandler(Handler handler);
void RACE_RegisterANCPassThruCb(RACE_ANCPassThru_Cb fptr);

void RACE_APP_RegisterMobileAppHandler(Handler handler);

#if FAST_CONNECTION_DONGLE
void RACE_AirDongle_SendDUTRsp(U8 *ptr, U32 length);
#endif
void RACE_App_System_RegisterHandler(Handler handler);
void RACE_Nvkey_Customer_Key_Notification(U8 channelId, U16 len, void *data);


void RACE_ANCAaptiveCheck_Send(U8 channelId,U8 RaceType, U8 *ptr, U16 len);

/**************************************************************************************************
* Host Audio
**************************************************************************************************/
typedef U8 VP_STATUS_TYPE;
enum
{
    VP_OFF  = (VP_STATUS_TYPE)0x0,
    VP_ON   = (VP_STATUS_TYPE)0x1,
};

typedef U8 ANC_STATUS_TYPE;
enum
{
    ANC_OFF     = (ANC_STATUS_TYPE)0x0,
    ANC_FILTER1 = (ANC_STATUS_TYPE)0x1,
    ANC_FILTER2 = (ANC_STATUS_TYPE)0x2,
    ANC_FILTER3 = (ANC_STATUS_TYPE)0x3,
    ANC_FILTER4 = (ANC_STATUS_TYPE)0x4,
    PASSTHROUGH = (ANC_STATUS_TYPE)0x5,
};

typedef U8 RACE_AUDIO_PATH_TYPE;
enum
{
    RACE_AUDIO_PATH_A2DP     = (RACE_AUDIO_PATH_TYPE)0x0,
    RACE_AUDIO_PATH_AUX      = (RACE_AUDIO_PATH_TYPE)0x1,
};

enum
{
	MODULE_PEQ_GROUP_INDEX = 0,
	MODULE_VP_ON_OFF,
	MODULE_VP_LANGUAGE,
	MODULE_VP_GET,
	MODULE_VP_SET,
	MODULE_ANC_STATUS,
	MODULE_GAME_MODE,
	MODULE_PASSTHRU_GAIN,
    MODULE_MIC_SWAP,
    MODULE_ECNR,
    MODULE_AUDIO_PATH,
    MODULE_LINE_IN_PEQ_GROUP_INDEX,
    MODULE_RHO_DONE = 20,
};

typedef struct
{
    U8 peq_group_index;
}MODULE_PEQ_GROUP_INDEX_T;

typedef struct
{
    VP_STATUS_TYPE vp_status;
}MODULE_VP_ON_OFF_T;

typedef struct
{
    U8 lang_id;
}MODULE_VP_LANG_T;

typedef union
{
    MODULE_PEQ_GROUP_INDEX_T    peq;
    MODULE_VP_ON_OFF_T          vp_on_off;
    MODULE_VP_LANG_T            vp_lang;
    U8                          is_ecnr_on;
    U8                          is_main_mic;
}HOST_AUDIO_MODULE_T;

void race_hostaudio_register_handler(Handler handler);
void race_hostaudio_a2dp_peq_changed(U8 group_index);
void race_hostaudio_line_in_peq_changed(U8 group_index);
void race_hostaudio_set_is_vp_on(BOOL is_vp_on);
void race_hostaudio_set_vp_lang(U8 lang_cnt, U8 lang_id);
void race_hostaudio_set_anc_status(U8 status, S16 gain);
void race_hostaudio_set_is_game_mode(BOOL is_game_mode_on);
void race_hostaudio_set_ecnr(BOOL is_ecnr_on);
void race_hostaudio_set_audio_path(RACE_AUDIO_PATH_TYPE audio_path);

#endif

