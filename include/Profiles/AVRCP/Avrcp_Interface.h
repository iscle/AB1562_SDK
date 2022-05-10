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


#ifndef _AVRCP_INTERFACE_H_
#define _AVRCP_INTERFACE_H_

#include "bt.h"


/**************************************************************************************************
* Typedef
**************************************************************************************************/
#define AVRCP_GET_APP_SETTINGS_NONE	0
#define AVRCP_GET_APP_SETTINGS_KEY	1
#define AVRCP_GET_APP_SETTINGS_UART	2
#define AVRCP_GET_APP_SETTINGS_SDK	3

//Pass Through Indication
#define AVRCP_IND_GET_STOP_REJECTED 0x85

#define AVRCP_VOLUME_SYNC_DISABLED_FEAT		0x01

/**************************************************************************************************
* Enumeration
**************************************************************************************************/
/*Internal Message*/
typedef enum {
    //Initialization, Connection Confirm
    AVRCP_START_SERVICE_CFM,
    AVRCP_STOP_SERVICE_CFM,
    AVRCP_CONNECT_CFM,
    AVRCP_DISCONNECT_CFM,

    //Initialization, Connection Indication
    AVRCP_CONNECT_IND,

    //Received Command Indication -- to MMI
    AVRCP_GET_CMD_UNITINFO_IND, //5
    AVRCP_GET_CMD_SUBUNITINFO_IND,
    AVRCP_GET_CMD_GETCAPS_IND,
    AVRCP_GET_CMD_GETELEMENTATTRIBUTES_IND,
    AVRCP_GET_CMD_REGISTERNOTIFICATION_IND,
    //AVRCP_GET_CMD_REGISTERNOTIFICATION_VOL_IND,
    AVRCP_GET_CMD_SETABSOLUTEVOL_IND, //10
    AVRCP_GET_CMD_UNKNOWNVENDORDEPENDENT_IND,
    AVRCP_GET_CMD_PASSTHROUGH_IND,
    AVRCP_GET_CMD_PASSTHROUGH_VENDORUNIQUE_IND,

    //Response Information Indication
    AVRCP_GET_APPLICATION_SETTING_IND,
    AVRCP_GET_PLAY_STATUS_IND, //15
    AVRCP_GET_REGISTER_NOTIFY_IND,
    AVRCP_GET_PASS_THROUGH_IND,
    AVRCP_GET_ELEMENT_ATTRIBUTE_IND,
    AVRCP_GET_CABILITY_IND,
    AVRCP_PHONE_SUPPORT_VOLUME_CHANGE_IND,
    
    //Get Response Confirm -- to MMI

    //Send Command
    AVRCP_INTERNAL_SEND_CMD_UNITINFO,
    AVRCP_INTERNAL_SEND_CMD_SUBUNITINFO,
    AVRCP_INTERNAL_SEND_CMD_GETCAPS,
    AVRCP_INTERNAL_SEND_CMD_GETELEMENTATTRIBUTES,
    AVRCP_INTERNAL_SEND_CMD_REGISTERNOTIFICATION,
    AVRCP_INTERNAL_SEND_CMD_SETABSOLUTEVOL,
    AVRCP_INTERNAL_SEND_CMD_PASSTHROUGH,
    AVRCP_INTERNAL_SEND_CMD_CONTNUERESPONSE,
    AVRCP_INTERNAL_SEND_CMD_VENDORUNIQUE,
    AVRCP_INTERNAL_SEND_CMD_GETPLAYSTATUS,
    AVRCP_INTERNAL_SEND_CMD_LISTAPPATTRIBUTE,
    AVRCP_INTERNAL_SEND_CMD_LISTAPPVALUE,
    AVRCP_INTERNAL_SEND_CMD_GETAPPVALUE,
    AVRCP_INTERNAL_SEND_CMD_SETAPPVALUE,
    AVRCP_INTERNAL_SEND_CMD_GETAPPATTRIBUTETEXT,
    AVRCP_INTERNAL_SEND_CMD_GETAPPVALUETEXT,

    //Send Response
    AVRCP_INTERNAL_SEND_RSP_UNITINFO,
    AVRCP_INTERNAL_SEND_RSP_SUBUNITINFO,
    AVRCP_INTERNAL_SEND_RSP_GETCAPS,
    AVRCP_INTERNAL_SEND_RSP_GETELEMENTATTRIBUTES,
    AVRCP_INTERNAL_SEND_RSP_REGISTERNOTIFICATION_VOL,
    AVRCP_INTERNAL_SEND_RSP_SETABSOLUTEVOL,
    AVRCP_INTERNAL_SEND_RSP_VENDORDEPENDENTREJECT,
    AVRCP_INTERNAL_SEND_RSP_PASSTHROUGH,
    AVRCP_INTERNAL_SEND_RSP_GETPLAYSTATUS,
    AVRCP_INTERNAL_SEND_RSP_LISTAPPATTRIBUTE,
    AVRCP_INTERNAL_SEND_RSP_LISTAPPVALUE,
    AVRCP_INTERNAL_SEND_RSP_GETCURRENTAPPVALUE,
    AVRCP_INTERNAL_SEND_RSP_SETAPPVALUE,
    AVRCP_INTERNAL_SEND_RSP_GETAPPATTRIBUTETEXT,
    AVRCP_INTERNAL_SEND_RSP_GETAPPVALUETEXT,

    //Changed Response
    AVRCP_INTERNAL_SEND_RSP_PLAYBACKSTATUSCHANGED,
    AVRCP_INTERNAL_SEND_RSP_TRACKCHANGED,
    AVRCP_INTERNAL_SEND_RSP_TRACKREACHEDEND,
    AVRCP_INTERNAL_SEND_RSP_TRACKREACHEDSTART,
    AVRCP_INTERNAL_SEND_RSP_PLAYBACKPOSCHANGED,
    AVRCP_INTERNAL_SEND_RSP_SYSTEMSTATUSCHANGED,
    AVRCP_INTERNAL_SEND_RSP_PLAYSTATUSCHANGED,
    AVRCP_INTERNAL_SEND_RSP_PLAYERAPPSETTINGCHANGED,
    AVRCP_INTERNAL_SEND_RSP_VOLCHANGED,
} AVRCP_MSG_E;

/* Register Notification: Event ID */
enum {
    AVRCP_EVENT_ID_PLAYBACK_STATUS_CHANGED = 0x01,
    AVRCP_EVENT_ID_TRACK_CHANGED = 0x02,
    AVRCP_EVENT_ID_TRACK_REACHED_END = 0x03,
    AVRCP_EVENT_ID_TRACK_REACHED_START = 0x04,
    AVRCP_EVENT_ID_PLAYBACK_POS_CHANGED = 0x05,
    AVRCP_EVENT_ID_BATT_STATUS_CHANGED = 0x06,
    AVRCP_EVENT_ID_SYSTEM_STATUS_CHANGED = 0x07,
    AVRCP_EVENT_ID_PLAYER_APPLICATION_SETTING_CHANGED = 0x08,
    AVRCP_EVENT_ID_NOW_PLAYING_CONTENT_CHANGED = 0x09,
    AVRCP_EVENT_ID_AVAILABLE_PLAYERS_CHANGED = 0x0a,
    AVRCP_EVENT_ID_ADDRESSED_PLAYER_CHANGED = 0x0b,
    AVRCP_EVENT_ID_UIDS_CHANGED = 0x0c,
    AVRCP_EVENT_ID_VOLUME_CHANGED = 0x0d,
}; //Get Capability Event ID

enum {
    PUSHED,
    RELEASED
}; //Pass Through Button State

/* Error Code */
enum {
    AVC_ERROR_CODE_INVALID_COMMAND = 0x00,
    AVC_ERROR_CODE_INVALID_PARAMETER,
    AVC_ERROR_CODE_PARAMETER_ERROR,
    AVC_ERROR_CODE_INTERNAL_ERROR,
    AVC_ERROR_CODE_NO_ERROR,
    AVC_ERROR_CODE_UIDS_CHANGED,
    AVC_ERROR_CODE_RESERVED,
    AVC_ERROR_CODE_INVALID_DIRECTION,
    AVC_ERROR_CODE_NOT_A_DIRECTORY,
    AVC_ERROR_CODE_DOES_NOT_EXIST,
    AVC_ERROR_CODE_INVALID_SCOPE,
    AVC_ERROR_CODE_RANGE_OUT_OF_BOUNDS,
    AVC_ERROR_CODE_FOLDER_NOT_PLAYABLE,
    AVC_ERROR_CODE_MEDIA_IN_USE,
    AVC_ERROR_CODE_NOW_PLAYING_LIST_FULL,
    AVC_ERROR_CODE_SEARCH_NOT_SUPPORTED,
    AVC_ERROR_CODE_SEARCH_IN_PROGRESS,
    AVC_ERROR_CODE_INVALID_PLAYER_ID,
    AVC_ERROR_CODE_PLAYER_NOT_BROWSABLE,
    AVC_ERROR_CODE_PLAYER_NOT_ADDRESSED,
    AVC_ERROR_CODE_NO_VALID_SEARCH_RESULTS,
    AVC_ERROR_CODE_NO_AVAILABLE_PLAYERS,
    AVC_ERROR_CODE_ADDRESSED_PLAYER_CHANGED,
};

/* AVC Operation ID (Pass Through) */
enum {
    AVC_OPERATION_POWER = 0x40,
    AVC_OPERATION_VOL_UP = 0x41,
    AVC_OPERATION_VOL_DOWN = 0x42,
    AVC_OPERATION_MUTE = 0x43, //not used
    AVC_OPERATION_PLAY = 0x44,
    AVC_OPERATION_STOP = 0x45,
    AVC_OPERATION_PAUSE = 0x46,
    AVC_OPERATION_RECORD = 0x47, //not used
    AVC_OPERATION_REWIND = 0x48,
    AVC_OPERATION_FAST_FORWARD = 0x49,
    AVC_OPERATION_EJECT = 0x4a, //not used
    AVC_OPERATION_FORWARD = 0x4b,
    AVC_OPERATION_BACKWARD = 0x4c,
    AVC_OPERATION_VENDOR_UNIQUE = 0x7e, //1394 Trade Association , AV/C Panel Subunit

    //Airoha
    AVC_OPERATION_INTERNAL_REWIND_RELEASE = 0x4d,
    AVC_OPERATION_INTERNAL_FAST_FORWARD_RELEASE = 0x4e,
#ifdef PEQ_ENABLE
    AVC_OPERATION_PEQOFF = 0x01,
    AVC_OPERATION_PEQON = 0x02,
#endif
    AVC_OPERATION_KEY_PLAY = 0x03,
    AVC_OPERATION_KEY_PAUSE = 0x04,
    AVC_OPERATION_FORCE_PLAY = 0x05,
    AVC_OPERATION_FORCE_PAUSE = 0x06, // CODEC_SWITCH_FEATURE
    AVC_OPERATION_GET_REPEAT_MODE = 0x10,
    AVC_OPERATION_GET_SHUFFLE_MODE = 0x11,
    AVC_OPERATION_GET_PLAY_STATUS = 0x12,
    AVC_OPERATION_SET_REPEAT_MODE = 0x13,
    AVC_OPERATION_SET_SHUFFLE_MODE = 0x14,
};

enum {
    AVRCP_REPEAT_OFF = 0x01,
    AVRCP_REPEAT_SINGLE_TRACK = 0x02,
    AVRCP_REPEAT_ALL_TRACK = 0x03,
    //AVRCP_REPEAT_GROUP			= 0x04, // ignore coz this may not be supported in mobile[iphone]
    AVRCP_REPEAT_MODE_NO = AVRCP_REPEAT_ALL_TRACK,

    AVRCP_REPEAT_CMD_REJECT = 0xEE,
};

enum {
    AVRCP_SHUFFLE_OFF = 0x01,
    AVRCP_SHUFFLE_ALL_TRACK = 0x02,
    //AVRCP_SHUFFLE_GROUP			= 0x03,// ignore coz this may not be supported in mobile[iphone]
    AVRCP_SHUFFLE_MODE_NO = AVRCP_SHUFFLE_ALL_TRACK,

    AVRCP_SHUFFLE_CMD_REJECT = 0xEE,
};

enum {
    AVRCP_PLAYBACK_STATUS_STOPPED,
    AVRCP_PLAYBACK_STATUS_PLAYING,
    AVRCP_PLAYBACK_STATUS_PAUSED,
    AVRCP_PLAYBACK_STATUS_FWD_SEEK,
    AVRCP_PLAYBACK_STATUS_REV_SEEK,
    AVRCP_PLAYBACK_STATUS_REJECT = 0x80,
    AVRCP_PLAYBACK_STATUS_NONE = 0xF0,
    AVRCP_PLAYBACK_STATUS_ERROR = 0xFF,
}; //Play Status

/**************************************************************************************************
* Structures
**************************************************************************************************/
#ifdef WIN32
#pragma pack(push,1)
#endif

/* Connection Relative */
typedef struct {
    U8 status;
} PACKED AVRCP_START_SERVICE_CFM_T;

typedef struct {
    U8 status;
} PACKED AVRCP_STOP_SERVICE_CFM_T;

typedef struct {
    U8 status;
    BD_ADDR_T bdAddr;
} PACKED AVRCP_CONNECT_CFM_T;

typedef struct {
    U8 status;
    BD_ADDR_T bdAddr;
} PACKED AVRCP_DISCONNECT_CFM_T;

typedef struct {
    BD_ADDR_T bdAddr;
    U8 identifier;
    U16 channelId;
} PACKED AVRCP_CONNECT_IND_T;

/* Indication To App */
typedef struct {
    BD_ADDR_T bdAddr;
    U8 transactionLabel;
    U8 resultCode;
    U8 subUnitID;
    U8 subUnitType;
} PACKED AVRCP_GET_CMD_UNITINFO_IND_T;

typedef struct {
    BD_ADDR_T bdAddr;
    U8 transactionLabel;
    U8 resultCode;
    U8 subUnitID;
    U8 subUnitType;
} PACKED AVRCP_GET_CMD_SUBUNITINFO_IND_T;

typedef struct {
    BD_ADDR_T bdAddr;
    U8 transactionLabel;
    U8 resultCode;
    U32 CompanyID;
    U8 CapabilityId;
} PACKED AVRCP_GET_CMD_GETCAPS_IND_T;

typedef struct {
    BD_ADDR_T bdAddr;
    U8 transactionLabel;
    U8 resultCode;
    U32 CompanyID;
} PACKED AVRCP_GET_CMD_GETELEMENTATTRIBUTES_IND_T;

typedef struct {
    BD_ADDR_T bdAddr;
    U8 transactionLabel;
    U8 resultCode;
    U32 CompanyID;
    U8 pduID;
    U8 eventID;
} PACKED AVRCP_GET_CMD_REGISTERNOTIFICATION_IND_T;

/*typedef struct
{
	BD_ADDR_T bdAddr;
	U8 transactionLabel;
	U8 resultCode;
	U32 CompanyID;
}AVRCP_GET_CMD_REGISTERNOTIFICATION_VOL_IND_T;*/

typedef struct {
    BD_ADDR_T bdAddr;
    U8 transactionLabel;
    U8 resultCode;
    U32 CompanyID;
    U8 absVol;
} PACKED AVRCP_GET_CMD_SETABSOLUTEVOL_IND_T;

typedef struct {
    BD_ADDR_T bdAddr;
    U8 transactionLabel;
    U8 resultCode;
    U32 CompanyID;
    U8 pduID;
} PACKED AVRCP_GET_CMD_UNKNOWNVENDORDEPENDENT_IND_T;

typedef struct {
    BD_ADDR_T bdAddr;
    U8 transactionLabel;
    U8 resultCode;
    U8 operandID;
    U8 buttonReleased;
    U8 operandLength;
} PACKED AVRCP_GET_CMD_PASSTHROUGH_IND_T;

typedef struct {
    BD_ADDR_T bdAddr;
    U8 transactionLabel;
    U8 resultCode;
    U8 operandID;
    U8 buttonReleased;
    U8 operandLength;
    U8 operantionId;
    U8 operantionParam;
} PACKED AVRCP_GET_CMD_PASSTHROUGH_VENDORUNIQUE_IND_T;

//Response Information Indication
typedef struct {
    BD_ADDR_T bdAddr;
    U8 attributeId;
    U8 attributeValue;
} PACKED AVRCP_GET_APPLICATION_SETTING_IND_T;

typedef struct {
    BD_ADDR_T bdAddr;
    BOOL isSendPlayPause;
    U8 playStatus;
} PACKED AVRCP_GET_PLAY_STATUS_IND_T;

typedef struct {
    U8 attributeID;
    U8 attributeValue;
} PACKED PLAY_APP_ELEMENT_STRU;

typedef struct {
    BD_ADDR_T bdAddr;
    U8 eventId;
    union {
        U8 playStatus;
        U8 UID[8];
        U32 playbackPos;
        U8 absVolume;
        U8 ctypeOrResp;
        struct {
            U8 attributeIDNum;
            PLAY_APP_ELEMENT_STRU attribute[2];
        } PACKED playAppSettings;
    } PACKED params;
} PACKED AVRCP_GET_REGISTER_NOTIFY_IND_T;

typedef struct {
    BD_ADDR_T bdAddr;
    U8 operationId;
} PACKED AVRCP_GET_PASS_THROUGH_IND_T;

typedef struct {
    BD_ADDR_T bdAddr;
<<<<<<< HEAD
    U32 AttributeID;
    U16 CharacterSetID;
    U16 AttributeValueLength;
    U8 AttributeValue[1];
} PACKED AVRCP_GET_ELEMENT_IND_T;

=======
    U16 len;// len of getEleAttPara;
    U8 attNum;
    union {
        struct {
          U32 AttributeID;
          U16 CharacterSetID;
          U16 AttributeValueLength;
          U8 AttributeValue[0];
        } PACKED singleAttrData;
        struct {
          U8 multData[1];//muti data
        } PACKED multiAttrData;
    } PACKED getEleAttPara; 
} PACKED AVRCP_GET_ELEMENT_IND_T;
>>>>>>> db20e11 (second commit)
typedef struct {
    BD_ADDR_T bdAddr;
    U8 CapabilityCount;
    U8 CapabilityValue[1];
} PACKED AVRCP_GET_CABILITY_IND_T;

typedef struct {
    BD_ADDR_T bdAddr;
} PACKED AVRCP_PHONE_SUPPORT_VOLUME_CHANGE_IND_T;

BOOL AVRCP_IsConnected(BD_ADDR_T *pBdAddr);
BOOL AVRCP_IsDisconnected(BD_ADDR_T *pBdAddr);

#ifdef WIN32
#pragma pack(pop)
#endif

/**************************************************************************************************
* API
**************************************************************************************************/
#include "BtAvrcp.h"


#endif //_AVRCP_INTERFACE_H_
