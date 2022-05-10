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

#ifndef _AMA_INTERFACE_H_
#define _AMA_INTERFACE_H_


#define AMA_CENTRAL_NAME_MAX_LEN    32

/* Cellular Calling */
#define AMA_AT_COMMAND_MAX_LEN      34
#define AMA_CALLER_NUMBER_MAX_LEN   34
#define AMA_WAKEWORD_METADATA_MAX_LEN   256
#define AMA_LOCAL_NAME_MAX_LEN   17
/**************************************************************************************************
* Enumeration
**************************************************************************************************/
/* Message */
typedef enum {
    AMA_START_SERVICE_CFM,              /*0x00*/
    AMA_STOP_SERVICE_CFM,               /*0x01*/
    AMA_ENABLE_ADV_CFM,                 /*0x02*/
    AMA_DISABLE_ADV_CFM,                /*0x03*/
    AMA_CONNECT_CFM,                    /*0x04*/
    AMA_DISCONNECT_CFM,                 /*0x05*/
    AMA_DISCONNECT_IND,                 /*0x06*/

    /* SPEECH */
    AMA_START_SPEECH_CFM,               /*0x07*/
    AMA_PROVIDE_SPEECH_IND,             /*0x08*/
    AMA_STOP_SPEECH_IND,                /*0x09*/
    AMA_STOP_SPEECH_CFM,                /*0x0a*/
    AMA_ENDPOINT_SPEECH_IND,            /*0x0b*/
    AMA_ENDPOINT_SPEECH_CFM,            /*0x0c*/
    AMA_SPEECH_STATE_IND,               /*0x0d*/
    /* STATE */
    AMA_GET_STATE_CFM,                  /*0x0e*/
    AMA_GET_STATE_IND,                  /*0x0f*/
    AMA_SET_STATE_CFM,                  /*0x10*/
    AMA_SET_STATE_IND,                  /*0x11*/
    AMA_SYNCHRONIZE_STATE_CFM,          /*0x12*/
    AMA_SYNCHRONIZE_STATE_IND,          /*0x13*/

    /* MEDIA */
    AMA_ISSUE_MEDIA_CONTROL_CFM,        /*0x14*/
    AMA_ISSUE_MEDIA_CONTROL_IND,        /*0x15*/

    /* Cellular Calling */
    AMA_FORWARD_AT_COMMAND_IND,         /*0x16*/
    AMA_INCOMING_CALL_CFM,              /*0x17*/

    /* MISC */
    AMA_SWITCH_TRANSPORT_IND,           /*0x18*/
    AMA_UPDATE_DEVICE_INFORMATION_IND,  /*0x19*/
    AMA_OVERRIDE_ASSISTANT_CFM,         /*0x1a*/
    AMA_OVERRIDE_ASSISTANT_IND,         /*0x1b*/
    AMA_GET_CENTRAL_INFORMATION_CFM,    /*0x1c*/
    AMA_RESET_CONNECTION_IND,           /*0x1d*/
    AMA_REMOVE_DEVICE_IND,              /*0x1e*/
    AMA_FACTORY_RESET_CFM,              /*0x1f*/
    AMA_DATA_CHANGED_IND,               /*0x20*/
    AMA_ADD_DATA_APPLY_AGENT_CFM,       /*0x21*/
    AMA_ADD_DATA_APPLY_PARTNER_CFM,     /*0x22*/
    AMA_GET_DATA_CFM,                   /*0x23*/
    AMA_NOTIFY_DEVICE_CONFIG_CFM,       /*0x24*/
    AMA_COMPLETE_SETUP_IND,             /*0x25*/
    AMA_START_SETUP_IND,                /*0x26*/
    AMA_GET_LOCALES_IND,                /*0x27*/
    AMA_SET_LOCALE_IND,                 /*0x28*/
    AMA_MSG_MAX,
} AMA_MSG_E;

/* ERROR CODE */
typedef enum {
    AMA_ERROR_CODE_SUCCESS = 0,
    AMA_ERROR_CODE_UNKNOWN = 1,
    AMA_ERROR_CODE_INTERNAL = 2,
    AMA_ERROR_CODE_UNSUPPORTED = 3,
    AMA_ERROR_CODE_USER_CANCELLED = 4,
    AMA_ERROR_CODE_NOT_FOUND = 5,
    AMA_ERROR_CODE_INVALID = 6,
    AMA_ERROR_CODE_BUSY = 7
} AMA_ERROR_CODE_E;

/* VALUE TYPE */
typedef enum {
    AMA_VALUE_TYPE_BOOLEAN = 2,
    AMA_VALUE_TYPE_INTEGER = 3,
} AMA_VALUE_TYPE_E;

/* SPEECH releated */
typedef enum {
    AMA_AUDIO_PROFILE_CLOSE_TALK = 0,
    AMA_AUDIO_PROFILE_NEAR_FIELD = 1,
    AMA_AUDIO_PROFILE_FAR_FIELD = 2
} AMA_AUDIO_PROFILE_E;

typedef enum {
    AMA_AUDIO_FORMAT_PCM_L16_16KHZ_MONO = 0,
    AMA_AUDIO_FORMAT_OPUS_16KHZ_32KBPS_CBR_0_20MS = 1,
    AMA_AUDIO_FORMAT_OPUS_16KHZ_16KBPS_CBR_0_20MS = 2,
    AMA_AUDIO_FORMAT_MSBC = 3
} AMA_AUDIO_FORMAT_E;

typedef enum {
    AMA_AUDIO_SOURCE_STREAM = 0,
    AMA_AUDIO_SOURCE_BLUETOOTH_SCO = 1
} AMA_AUDIO_SOURCE_E;

typedef enum {
    AMA_SPEECH_INITIATOR_TYPE_NONE = 0,
    AMA_SPEECH_INITIATOR_TYPE_PRESS_AND_HOLD = 1,
    AMA_SPEECH_INITIATOR_TYPE_TAP = 3,
    AMA_SPEECH_INITIATOR_TYPE_WAKEWORD = 4
} AMA_SPEECH_INITIATOR_TYPE_E;

/* SPEECH STATE */
typedef enum {
    AMA_SPEECH_STATE_IDLE = 0,
    AMA_SPEECH_STATE_LISTENING = 1,
    AMA_SPEECH_STATE_PROCESSING = 2,
    AMA_SPEECH_STATE_SPEAKING = 3
} AMA_SPEECH_STATE_E;

/* STATE FEATURE  */
typedef enum {
    AMA_STATE_FEATURE_AUX_CONNECTED = 0x100,
    AMA_STATE_FEATURE_A2DP_ENABLED = 0x130,
    AMA_STATE_FEATURE_HFP_ENABLED = 0x131,
    AMA_STATE_FEATURE_A2DP_CONNECTED = 0x132,
    AMA_STATE_FEATURE_HFP_CONNECTED = 0x133,
    AMA_STATE_FEATURE_BT_DISCOVERABLE = 0x134,
    AMA_STATE_FEATURE_A2DP_ACTIVE = 0x135,
    AMA_STATE_FEATURE_HFP_ACTIVE = 0x136,
    AMA_STATE_FEATURE_CALIBRATION_REQUIRED = 0x200,
    AMA_STATE_FEATURE_DEVICE_THEME = 0x201,
    AMA_STATE_FEATURE_DEVICE_DND_ENABLED = 0x202,
    AMA_STATE_FEATURE_DEVICE_NETWORD_STATUS = 0x203,
    AMA_STATE_FEATURE_PRIVACY_MODE_ENABLED = 0x204,
    AMA_STATE_FEATURE_ACTIVE_NC_LEVEL = 0x205,
    AMA_STATE_FEATURE_PASSTHROUGH_LEVEL = 0x206,
    AMA_STATE_FEATURE_SETUP_MODE_ENABLED = 0x207,
    AMA_STATE_FEATURE_EXT_MIC_ENABLED = 0x208,
    AMA_STATE_FEATURE_MESSAGE_NOTI = 0x300,
    AMA_STATE_FEATURE_CALL_NOTI = 0x301,
    AMA_STATE_FEATURE_REMOTE_NOTI = 0x302,
    AMA_STATE_FEATURE_SO_SPEECH_EARCON_ENABLED = 0x350,
    AMA_STATE_FEATURE_EO_SPEECH_EARCON_ENABLED = 0x351,
    AMA_STATE_FEATURE_WAKE_WORD_DECTION_ENABLED = 0x352,
    AMA_STATE_FEATURE_ALEXA_FOLLOWUP_MODE_ENABLED = 0x353,
    AMA_STATE_FEATURE_AVRCP_OVERRIDE = 0x400,
    AMA_STATE_FEATURE_VOLUME_CHANGED_NOTI = 0x401,
} AMA_STATE_FEATURE_E;

/* MEDIA CONTROL  */
typedef enum {
    AMA_MEDIA_CONTROL_PLAY = 0,
    AMA_MEDIA_CONTROL_PAUSE = 1,
    AMA_MEDIA_CONTROL_NEXT = 2,
    AMA_MEDIA_CONTROL_PREVIOUS = 3,
    AMA_MEDIA_CONTROL_PLAY_PAUSE = 4,
} AMA_MEDIA_CONTROL_E;

typedef enum {
    AMA_PLATFORM_UNDEFINED,
    AMA_PLATFORM_IOS,
    AMA_PLATFORM_ANDROID,
} AMA_PLATFORM_E;

/* Status */
typedef enum {
    AMA_STATUS_OK = 0,
    AMA_STATUS_ERROR,
    AMA_STATUS_OUT_OF_MEMORY,
} AMAStatus;

/**************************************************************************************************
* Structures
**************************************************************************************************/
typedef struct {
    U8 status;
} AMA_START_SERVICE_CFM_T;

typedef struct {
    U8 status;
} AMA_STOP_SERVICE_CFM_T;

typedef struct {
    U8 status;
} AMA_ENABLE_ADV_CFM_T;

typedef struct {
    U8 status;
} AMA_DISABLE_ADV_CFM_T;

typedef struct {
    U8 status;
    BD_ADDR_T bdAddr;
} AMA_CONNECT_CFM_T;

typedef struct {
    U8 status;
    BD_ADDR_T bdAddr;
} AMA_DISCONNECT_CFM_T;

typedef struct {
    U8 status;
    BD_ADDR_T bdAddr;
} AMA_DISCONNECT_IND_T;

/* SPEECH */
typedef struct {
    U32 startIndexInSamples;
    U32 endIndexInSamples;
    U16 metadataLength;
    bool nearMiss;
    U8 metadata[1]; /* Max length: AMA_WAKEWORD_METADATA_MAX_LEN */
} AMA_SPEECH_INITIATOR_WAKEWORD;

typedef struct {
    AMA_AUDIO_PROFILE_E audioProfile;
    AMA_AUDIO_FORMAT_E audioFormat;
    AMA_AUDIO_SOURCE_E audioSource;
    AMA_SPEECH_INITIATOR_TYPE_E type;
    BOOL suppressEarcon;
    BOOL suppressEndpointEarcon;
    BOOL suppressStartEarcon;
    AMA_SPEECH_INITIATOR_WAKEWORD wake_word;
} AMA_START_SPEECH_T;

typedef struct AMA_START_SPEECH_CFM_T {
    BD_ADDR_T bdAddr;
    AMA_ERROR_CODE_E errorCode;
} AMA_START_SPEECH_CFM_T;   /* AMA_START_SPEECH_CFM */

typedef struct {
    AMA_AUDIO_PROFILE_E audioProfile;
    AMA_AUDIO_FORMAT_E audioFormat;
    AMA_AUDIO_SOURCE_E audioSource;
} AMA_PROVIDE_SPEECH_T;

typedef struct AMA_PROVIDE_SPEECH_IND_T {
    BD_ADDR_T bdAddr;
} AMA_PROVIDE_SPEECH_IND_T; /* AMA_PROVIDE_SPEECH_IND */

typedef struct AMA_STOP_SPEECH_IND_T {
    BD_ADDR_T bdAddr;
    AMA_ERROR_CODE_E errorCode;
} AMA_STOP_SPEECH_IND_T;    /* AMA_STOP_SPEECH_IND */

typedef struct AMA_STOP_SPEECH_CFM_T {
    BD_ADDR_T bdAddr;
    AMA_ERROR_CODE_E errorCode;
} AMA_STOP_SPEECH_CFM_T;    /* AMA_STOP_SPEECH_CFM */

typedef struct AMA_ENDPOINT_SPEECH_IND_T {
    BD_ADDR_T bdAddr;
} AMA_ENDPOINT_SPEECH_IND_T;    /*AMA_ENDPOINT_SPEECH_IND */

typedef struct AMA_ENDPOINT_SPEECH_CFM_T {
    BD_ADDR_T bdAddr;
    AMA_ERROR_CODE_E errorCode;
} AMA_ENDPOINT_SPEECH_CFM_T;    /* AMA_ENDPOINT_SPEECH_CFM */

typedef struct AMA_SPEECH_STATE_IND_T {
    BD_ADDR_T bdAddr;
    AMA_SPEECH_STATE_E state;
} AMA_SPEECH_STATE_IND_T;   /* AMA_SPEECH_STATE_IND */

/* STATE */
typedef struct {
    AMA_STATE_FEATURE_E feature;
    AMA_VALUE_TYPE_E valueType;
    union {
        BOOL boolean;
        U32 integer;
    } value;
} AMA_STATE_T;

typedef struct AMA_GET_STATE_CFM_T {
    BD_ADDR_T bdAddr;
    AMA_ERROR_CODE_E errorCode;
    AMA_STATE_T state;
} AMA_GET_STATE_CFM_T;  /* AMA_GET_STATE_CFM */

typedef struct AMA_GET_STATE_IND_T {
    BD_ADDR_T bdAddr;
    AMA_STATE_FEATURE_E feature;
} AMA_GET_STATE_IND_T;  /* AMA_GET_STATE_IND */

typedef struct AMA_SET_STATE_CFM_T {
    BD_ADDR_T bdAddr;
    AMA_ERROR_CODE_E errorCode;
} AMA_SET_STATE_CFM_T;  /* AMA_SET_STATE_CFM */

typedef struct AMA_SET_STATE_IND_T {
    BD_ADDR_T bdAddr;
    AMA_STATE_T state;
} AMA_SET_STATE_IND_T;  /* AMA_SET_STATE_IND */

typedef struct AMA_SYNCHRONIZE_STATE_CFM_T {
    BD_ADDR_T bdAddr;
    AMA_ERROR_CODE_E errorCode;
} AMA_SYNCHRONIZE_STATE_CFM_T;  /* AMA_SYNCHRONIZE_STATE_CFM */

typedef struct AMA_SYNCHRONIZE_STATE_IND_T {
    BD_ADDR_T bdAddr;
    AMA_STATE_T state;
} AMA_SYNCHRONIZE_STATE_IND_T;  /* AMA_SYNCHRONIZE_STATE_IND */

/* MEDIA */
typedef struct AMA_ISSUE_MEDIA_CONTROL_CFM_T {
    BD_ADDR_T bdAddr;
    AMA_ERROR_CODE_E errorCode;
} AMA_ISSUE_MEDIA_CONTROL_CFM_T;  /* AMA_ISSUE_MEDIA_CONTROL_CFM */

typedef struct AMA_ISSUE_MEDIA_CONTROL_IND_T {
    BD_ADDR_T bdAddr;
    AMA_MEDIA_CONTROL_E control;
} AMA_ISSUE_MEDIA_CONTROL_IND_T;  /* AMA_ISSUE_MEDIA_CONTROL_IND */

/* Cellular Calling */
typedef struct AMA_FORWARD_AT_COMMAND_IND_T {
    BD_ADDR_T bdAddr;
    U8 command[AMA_AT_COMMAND_MAX_LEN];
} AMA_FORWARD_AT_COMMAND_IND_T; /* AMA_FORWARD_AT_COMMAND_IND */

typedef struct AMA_INCOMING_CALL_CFM_T {
    BD_ADDR_T bdAddr;
    AMA_ERROR_CODE_E errorCode;
} AMA_INCOMING_CALL_CFM_T;      /* AMA_INCOMING_CALL_CFM */

/* MISC */
typedef struct {
    BD_ADDR_T bdAddr;
    BD_ADDR_T newBdAddr;
} AMA_SWITCH_TRANSPORT_IND_T;

typedef struct {
    BD_ADDR_T bdAddr;
    AMA_ERROR_CODE_E errorCode;
} AMA_OVERRIDE_ASSISTANT_CFM_T;

typedef struct {
    BD_ADDR_T bdAddr;
    AMA_ERROR_CODE_E errorCode;
} AMA_OVERRIDE_ASSISTANT_IND_T;

typedef struct {
    BD_ADDR_T bdAddr;
    U32 timeout;
    U8 isForceDisconnect;
} AMA_RESET_CONNECTION_IND_T;

typedef struct {
    BD_ADDR_T bdAddr;
} AMA_REMOVE_DEVICE_IND_T;

typedef struct {
    BD_ADDR_T bdAddr;
    U8 name[AMA_LOCAL_NAME_MAX_LEN];
} AMA_UPDATE_DEVICE_INFORMATION_IND_T;

typedef struct {
    BD_ADDR_T bdAddr;
    AMA_ERROR_CODE_E errorCode;
    U8 name[AMA_CENTRAL_NAME_MAX_LEN];
    U8 platform;
} AMA_GET_CENTRAL_INFORMATION_CFM_T;

typedef struct {
    U8 status;
} AMA_FACTORY_RESET_CFM_T;

typedef struct {
    U8 status;
} AMA_ADD_DATA_APPLY_AGENT_CFM_T;

typedef struct {
    U8 status;
} AMA_ADD_DATA_APPLY_PARTNER_CFM_T;

typedef struct {
    U8 status;
    U16 length;
    U8 pData[1];
} AMA_GET_DATA_CFM_T;

typedef struct {
    U8 status;
} AMA_DATA_CHANGED_IND_T;

typedef struct {
    U8 errorcode;
    BD_ADDR_T bdAddr;
}AMA_NOTIFY_DEVICE_CONFIG_CFM_T;

typedef struct {
    BD_ADDR_T bdAddr;
} AMA_COMPLETE_SETUP_IND_T;

typedef struct {
    BD_ADDR_T bdAddr;
} AMA_START_SETUP_IND_T;

typedef struct {
    BD_ADDR_T bdAddr;
} AMA_GET_LOCALES_IND_T;

typedef enum {
    AMA_LOCALE_ID_DE_DE             = 0x00,
    AMA_LOCALE_ID_EN_AU             = 0x01,
    AMA_LOCALE_ID_EN_GB             = 0x02,
    AMA_LOCALE_ID_EN_IN             = 0x03,
    AMA_LOCALE_ID_EN_US             = 0x04,
    AMA_LOCALE_ID_ES_ES             = 0x05,
    AMA_LOCALE_ID_ES_MX             = 0x06,
    AMA_LOCALE_ID_FR_CA             = 0x07,
    AMA_LOCALE_ID_FR_FR             = 0x08,
    AMA_LOCALE_ID_IT_IT             = 0x09,
    AMA_LOCALE_ID_JA_JP             = 0x0A,
    AMA_LOCALE_ID_PT_BR             = 0x0B,

    AMA_LOCALE_ID_MAX               = 0xFF,
} AMA_LOCALE_ID_T;

typedef struct {
    BD_ADDR_T bdAddr;
    AMA_LOCALE_ID_T locale_id;
} AMA_SET_LOCALE_IND_T;

#endif /* _AMA_INTERFACE_H_ */

