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


#ifndef _AVRCP_DATAHANDLER_H_
#define _AVRCP_DATAHANDLER_H_

#include "bt_avctp.h"


/**************************************************************************************************
* Typedef
**************************************************************************************************/
#define AVRCP_INVALID_TRANSACTION_LABEL 0xFF

#define AVRCP_PDU_ID_GET_CAPABILITIES			0x10
#define AVRCP_PDU_ID_GET_PALYER_APPLICATION		0x13
#define AVRCP_PDU_ID_SET_PALYER_APPLICATION     0x14   //dean
#define AVRCP_PDU_ID_GET_ELEMENT_ATTRIBUTES		0x20
#define AVRCP_PDU_ID_GET_PLAY_STATUS			0x30
#define AVRCP_PDU_ID_REGISTER_NOTIFICATION		0x31
#define AVRCP_PDU_ID_REQUEST_CONTINUE_RESPONSE	0x40
#define AVRCP_PDU_ID_ABORT_CONTINUE_RESPONSE	0x41
#define AVRCP_PDU_ID_SET_ABSOLUTE_VOLUME		0x50
#define AVRCP_PDU_ID_SET_ADDRESSED_PLAYER		0x60
#define AVRCP_PDU_ID_AVCTP_TS_FRA_BV_03_C		0xFC
#define AVRCP_PDU_ID_AVCTP_TS_FRA_BV_04_C		0xFD

#define AVRCP_FIRST_NOTIFY_LABEL	8
#define AVRCP_MAX_CMD_LABEL_COUNT	(16 - AVRCP_FIRST_NOTIFY_LABEL)

/* MMI AVRCP Settings */
//Following defines AVRCP notify event mask
#define APP_AVRCP_PLAYBACK_STATUS_CHANGED				0x01
#define APP_AVRCP_TRACK_CHANGED							0x02
#define APP_AVRCP_TRACK_REACHED_END						0x04
#define APP_AVRCP_TRACK_REACHED_START					0x08
#define APP_AVRCP_PLAYBACK_POS_CHANGED					0x10
#define APP_AVRCP_VOLUME_CHANGED						0x20
#define APP_AVRCP_PLAYER_APPLICATION_SETTING_CHANGED	0x40

//Following defines AVRCP media event mask
#define APP_AVRCP_MEDIA_TITLE				0x01
#define APP_AVRCP_MEDIA_ARTIST_NAME			0x02
#define APP_AVRCP_MEDIA_ALBUM_NAME			0x04
#define APP_AVRCP_MEDIA_TRACK_NUMBER		0x08
#define APP_AVRCP_MEDIA_TOTAL_TRACKS		0x10
#define APP_AVRCP_MEDIA_GENRE				0x20
#define APP_AVRCP_MEDIA_PLAYING_TIME		0x40

#define AVRCP_REPORT_VOLUME_CHANGE						0x01
#define AVRCP_NOT_SUPPORT_PAUSE_CMD_IN_REMOTE			0x02
#define AVRCP_SEND_PLAY_PAUSE_AFTER_GET_PLAY_STATUS		0x04
#define AVRCP_MISC_REWIND_OR_FAST_FORWARD_SENT			0x08
#define AVRCP_MISC_FIRST_INTERIM_TRACK_CHANGED			0x10
#define AVRCP_SEND_VOLUME_CHANGE_SUPPORT_IND            0x20

/**************************************************************************************************
* Enumeration
**************************************************************************************************/
/* Command / Response Group */
enum {
    AVC_OPCODE_VENDOR_DEPENDENT = 0x00,
    AVC_OPCODE_UNIT_INFO = 0x30,
    AVC_OPCODE_SUBUNIT_INFO = 0x31,
    AVC_OPCODE_PASS_THROUGH = 0x7C,
}; //OpCode

/* Get Capabilities */
enum {
    AVRCP_GET_CAPABILITIES_COMPANY_ID = 0x02,
    AVRCP_GET_CAPABILITIES_EVENTS_SUPPORTED = 0x03
}; //Capability ID

enum {
    AVRCP_MEDIA_TITLE = 0x01,
    AVRCP_MEDIA_ARTIST_NAME = 0x02,
    AVRCP_MEDIA_ALBUM_NAME = 0x03,
    AVRCP_MEDIA_TRACK_NUMBER = 0x04,
    AVRCP_MEDIA_TOTAL_TRACKS = 0x05,
    AVRCP_MEDIA_GENRE = 0x06,
    AVRCP_MEDIA_PLAYING_TIME = 0x07,
}; //Element Attribute ID

enum {
    SUBUNIT_MONITOR = 0x00,
    SUBUNIT_AUDIO = 0x01,
    SUBUNIT_PRINTER = 0x02,
    SUBUNIT_DISC = 0x03,
    SUBUNIT_TAPE_RECORDER_PLAYER = 0x04,
    SUBUNIT_TUNER = 0x05,
    SUBUNIT_CA = 0x06,
    SUBUNIT_CAMERA = 0x07,
    SUBUNIT_PANEL = 0x09,
    SUBUNIT_BULLETIN_BOARD = 0x0A,
    SUBUNIT_CAMERA_STORAGE = 0x0B,
    SUBUNIT_EXTENDED_TO_NEXT_BYTE = 0x1E,
    SUBUNIT_UNIT = 0x1F,
}; //Subunit Type

enum {
    AVRCP_LABEL_PLAYBACK_STATUS_CHANGED = AVRCP_FIRST_NOTIFY_LABEL,
    AVRCP_LABEL_TRACK_CHANGED,
    AVRCP_LABEL_TRACK_REACHED_END,
    AVRCP_LABEL_TRACK_REACHED_START,
    AVRCP_LABEL_PLAYBACK_POS_CHANGED,
    AVRCP_LABEL_VOLUME_CHANGED,
    AVRCP_LABEL_PLAYER_APPLICATION_SETTING_CHANGED,
}; //Specific Transaction Label of Register Notification

/* AVRCP Packet Type */
enum {
    AVRCP_PKT_TYPE_SINGLE = 0,
    AVRCP_PKT_TYPE_START,
    AVRCP_PKT_TYPE_CONTINUE,
    AVRCP_PKT_TYPE_END
};

enum {
    //Command Type
    AVC_CMD_CONTROL = 0x00,				// Used to instruct a target to perform an operation.
    AVC_CMD_STATUS = 0x01,				// Used to check a device's current status.
    AVC_CMD_SPECIFIC_INQUIRY = 0x02,	// Used to check whether a target supports a particular CONTROL command. All operands are included.
    AVC_CMD_NOTIFY = 0x03,				// Used for receiving notification of a change in a device's state.
    AVC_CMD_GENERAL_INQUIRY = 0x04,		// Used to check whether a target supports a particular CONTROL command. Operands are not included.

    //Response Code
    AVC_RES_NOT_IMPLEMENTED = 0x08,		// The target does not implement the command specified by the opcode and operand marked
    // in the ck column, or doesn't implement the specified subunit.
    AVC_RES_ACCEPTED = 0x09,			// The target executed or is executing the command.
    AVC_RES_REJECTED = 0x0A,			// The target implements the command specified by the opcode and operands marked in the ck
    // column, but cannot respond because the current state of the target doesn't allow it.
    // Note that some commands may return a REJECTED response as a result of invalid operands not marked in the ck column.
    AVC_RES_IN_TRANSITION = 0x0B,		// The target implements the STATUS command, but it is in a state of transition. The STATUS command may be retried at a future time.
    AVC_RES_STABLE = 0x0C,				// For SPECIFIC INQUIRY or GENERAL INQUIRY commands, the target implements the command.
    //For STATUS commands, the target returns STABLE and includes the status results.
    AVC_RES_CHANGED = 0x0D,				// The response frame contains a notification that the target device!|s state has changed.
    AVC_RES_INTERIM = 0x0F,				// For CONTROL commands, the target has accepted the request but cannot return information within 100 milliseconds.
    // For NOTIFY commands, the target has accepted the command and will notify the controller of a change of target state at a future time.
};

///////////////////////////////////////////////////////
///////////// Command and Response Table///////////////
///////////////////////////////////////////////////////
//Com\Res	|NotImplement		| Accept	| Reject| In transition	| ImplementStable	| Change	| Interim
//------------------------------------------------------------------------------------------------------------
//Control   |    O              |    O		|    O	|        X		|          X		|      X    |   O
//------------------------------------------------------------------------------------------------------------
//Status    |    O				|    X		|    O	|        O      |    O (Stable)		|      X    |   X
//------------------------------------------------------------------------------------------------------------
//SpecificI |    O              |    X		|    X	|        X      |    O (Implement)	|      X    |   X
//------------------------------------------------------------------------------------------------------------
//Notify    |    O              |    X		|    O	|        X      |          X		|      O    |   O
//------------------------------------------------------------------------------------------------------------
//GeneralI  |    O              |    X		|    X	|        X      |    O (Implement)	|      X    |   X
//------------------------------------------------------------------------------------------------------------

/*
https://www.bluetooth.com/specifications/assigned-numbers/service-discovery
Audio/Video Remote Control Profile (AVRCP)
NOTE: Used as both Service Class Identifier and Profile Identifier.
*/

/* None Used */
enum {
    AVRCP_PLAYER_SETTINGS_ILLEGAL = 0x00,
    AVRCP_PEQ_ON_OFF_STATUS = 0x01,
    AVRCP_REPEAT_MODE_STATUS = 0x02,
    AVRCP_SHUFFLE_ON_OFF_STATUS = 0x03,
    AVRCP_SCAN_ON_OFF_STATUS = 0x04,
}; //AVRCP Appendix F



/**************************************************************************************************
* Structure
**************************************************************************************************/
#ifdef WIN32
#pragma pack(push,1)
#endif

typedef struct {
    U8 operandID;
    U8 operandLength;
} PACKED AVRCP_PASSTHROUGH_DBG;

typedef struct {
    U8 operandID : 7;
    U8 buttonReleased : 1;
    U8 operandLength;
} PACKED AVRCP_PASSTHROUGH_PDU;

typedef struct {
    U8 operandID : 7;
    U8 buttonReleased : 1;
    U8 operandLength;
    U8 companyID[3];
    union {
        U16 bluetooth_operation_id;
        struct {
            U8 operationId;
            U8 operationParam;
        } PACKED airoha;
    } PACKED vendor_unique;
} PACKED AVRCP_VENDOR_UNIQUE_PDU;

typedef struct {
    U8 companyID[3];
    U8 pduID;
    U8 packetType : 2;
    U8 reserved : 6;
    U16 ParameterLength;
} PACKED AVRCP_VENDOR_DEPENDENT_PDU;

typedef struct {
    U8 companyID[3];
    U8 pduID;
    U8 packetType : 2;
    U8 reserved : 6;
    U16 ParameterLength;
    U8 errorCode;
} PACKED AVRCP_REJECT_RSP;

typedef struct {
    U8 companyID[3];
    U8 pduID;
    U8 packetType : 2;
    U8 reserved : 6;
    U16 ParameterLength;
    U8 CapabilityId;
} PACKED AVRCP_GET_CAPABILITIES_CMD;

typedef struct {
    U8 companyID[3];
    U8 pduID;
    U8 packetType : 2;
    U8 reserved : 6;
    U16 ParameterLength;
    U8 CapabilityId;
    U8 CapabilityCount;
} PACKED AVRCP_GET_CAPABILITIES_RSP;

typedef struct {
    U8 companyID[3];
    U8 pduID;
    U8 packetType : 2;
    U8 reserved : 6;
    U16 ParameterLength;
    U8 UID[8];
    U8 NumAttributes;
    U32 AttributeID[1];
} PACKED AVRCP_GET_ELEMENT_ATTRIBUTES_CMD;

typedef struct {
<<<<<<< HEAD
    U32 AttributeID;
    U16 CharacterSetID;
    U16 AttributeValueLength;
=======
    //U32 AttributeID;
    //U16 CharacterSetID;
    //U16 AttributeValueLength;
    U8 AttNum;
>>>>>>> db20e11 (second commit)
    U8 AttributeValue[1];
} PACKED AVRCP_ELEMENT_ATTRIBUTE;

typedef struct {
    U8 companyID[3];
    U8 pduID;
    U8 packetType : 2;
    U8 reserved : 6;
    U16 ParameterLength;
<<<<<<< HEAD
    U8 NumAttributes;
    AVRCP_ELEMENT_ATTRIBUTE	Attribute[1];
=======
    //U8 NumAttributes;
    U8	Attribute[1];
>>>>>>> db20e11 (second commit)
} PACKED AVRCP_GET_ELEMENT_ATTRIBUTES_RSP;

typedef struct {
    U8 companyID[3];
    U8 pduID;
    U8 packetType : 2;
    U8 reserved : 6;
    U16 ParameterLength;
} PACKED AVRCP_GET_PLAY_STATUS_CMD;

typedef struct {
    U8 companyID[3];
    U8 pduID;
    U8 packetType : 2;
    U8 reserved : 6;
    U16 ParameterLength;
    U32 SongLength;		//milliseconds
    U32 SongPosition;	//milliseconds
    U8 PlayStatus;
} PACKED AVRCP_GET_PLAY_STATUS_RSP;

typedef struct {
    U8 companyID[3];
    U8 pduID;
    U8 packetType : 2;
    U8 reserved : 6;
    U16 ParameterLength;
    U8 absoluteVolume : 7;
    U8 RFD : 1;
} PACKED AVRCP_ABSOLUTE_VOLUME_PDU;

typedef struct {
    U8 companyID[3];
    U8 pduID;
    U8 packetType : 2;
    U8 reserved : 6;
    U16 ParameterLength;
    U8 eventID;
    union {
        U8 PlayStatus;		//playback status changed
        U8 UID[8];			//track changed
        U32 PlaybackPos;	//playback pos changed
        U8 absVolume;
        struct {
            U8 attributeIDNum;
            U8 attributeStart[1];
        } PACKED playAppSettings;
    } PACKED eventParams;
} PACKED AVRCP_NOTIFY_PDU;

typedef struct {
    U8 companyID[3];
    U8 pduID;
    U8 packetType : 2;
    U8 reserved : 6;
    U16 ParameterLength;
    U8 eventID;
    U8 unusedParam[4];
} PACKED AVRCP_REGISTER_PDU;

typedef struct {
    U8 companyID[3];
    U8 pduID;
    U8 packetType : 2;
    U8 reserved : 6;
    U16 ParameterLength;
    U8 numOfSettings;
    U8 settingAttributeID;
    U8 settingValue;
} PACKED AVRCP_GET_CURRENT_PLAYER_APP_VALUE_RESP;

typedef union {
    AVRCP_PASSTHROUGH_DBG passThroughDbg;
    AVRCP_PASSTHROUGH_PDU passThroughPdu;
    AVRCP_VENDOR_UNIQUE_PDU vendorUniquePdu;
    AVRCP_VENDOR_DEPENDENT_PDU vendorDependentPdu;
    AVRCP_REJECT_RSP rejectRsp;
    AVRCP_NOTIFY_PDU notifyPdu;
    AVRCP_REGISTER_PDU registerPdu;
    AVRCP_ABSOLUTE_VOLUME_PDU absoluteVolumePdu;
    AVRCP_GET_CURRENT_PLAYER_APP_VALUE_RESP playerSettingsValueResp;
    AVRCP_GET_CAPABILITIES_CMD getCapabilitiesCmd;
    AVRCP_GET_CAPABILITIES_RSP getCapabilitiesRsp;
    AVRCP_GET_ELEMENT_ATTRIBUTES_CMD getElementAttributesCmd;
    AVRCP_GET_ELEMENT_ATTRIBUTES_RSP getElementAttributesRsp;
    AVRCP_GET_PLAY_STATUS_CMD getPlayStatusCmd;
    AVRCP_GET_PLAY_STATUS_RSP getPlayStatusRsp;
} PACKED AVRCP_MSG_BODY_T;

typedef struct {
    U8 ctypeOrResp : 4;
    U8 reserved : 4;
    U8 subUnitID : 3;
    U8 subUnitType : 5;
    U8 opCode;
    AVRCP_MSG_BODY_T msgBodyPtr;
} PACKED AVRCP_PKT_T;

#ifdef WIN32
#pragma pack(pop)
#endif

/**************************************************************************************************
* Internal Public Functions
**************************************************************************************************/
U8 Avrcp_OpCode2StrIndex(U8 opcode);
U8 Avrcp_PDUID2StrIndex(U8 pduID);
U8 Avrcp_OperandID2StrIndex(U8 opID);

BOOL Avrcp_SendData(BD_ADDR_T *pBdAddr, U8 *dataPtr, U8 dataLength, U8 msgType, U8 RspTransactionLabel);

/**************************************************************************************************
* Internal Public Functions (Data Handler)
**************************************************************************************************/
void Avrcp_DataHandeler(BT_AVCTP_MORE_DATA_T *msg);


#endif //_AVRCP_DATAHANDLER_H_
