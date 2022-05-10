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

#ifndef __BT_COMMON_HH
#define __BT_COMMON_HH

<<<<<<< HEAD
=======
#include "types.h"
#include "message.h"
>>>>>>> db20e11 (second commit)

/**************************************************************************************************/
/*                            Macros                                                              */
/**************************************************************************************************/
/*!
    @addtogroup BT_LIB_COMMON_MACRO
    @{
*/
/*!
        @name Utilities macros for message and handler
        @{
*/
#define MSG_HANDLER(msg, ind) msg##_FN(msg##_T *ind, Handler handler, U32 handler_id)
#define MSG_ENTRY_PAIR(id) {id, (MSG_HANDLE_PROC)id##_FN}
#define MSG_ENTRY_PAIR_END {0xFFFF, NULL}
#define MSG_HANDLER_EX(prefix, id, ind) prefix##id##_FN(id##_T *ind, Handler handler, U32 handler_id)
#define MSG_ENTRY_PAIR_EX(prefix, id) {id, (U32(*)(void*, Handler, U32))prefix##id##_FN}
/*!
        @}
*/

#define FBLUE 0x1
#define FGREEN 0x2
#define FRED 0x4
#define FLIGHT 0x8


#define SUPPORT_MULTI_POINT

/*!
    @}
*/
/**************************************************************************************************/
/*                            Data Structures                                                     */
/**************************************************************************************************/
/*!
    @addtogroup BT_LIB_COMMON_DS
    @{
*/

typedef enum BT_CONTROLER_ID_E {
    CONTROLER_TI_2560 = 0,
    CONTROLER_AIROHA_1530
} BT_CONTROLER_ID_E;

/*!
    @brief Message of BT libraries
*/
typedef enum {
    /* Initialization */
    BT_INIT_CFM = MSG_MESSAGE_BT_BASE, //1541
    BT_DEINIT_CFM,

    BT_SET_ROLE_CFM,
    BT_GET_ROLE_CFM,
    BT_SET_SUPERVISION_CFM,
    BT_SET_MODE_CFM,
    BT_SET_SNIFF_SUBRATING_CFM,
    BT_QOS_SETUP_CFM,
    BT_SET_LINK_POLICY_CFM,
    BT_ROLE_CHANGE_IND,
    BT_MODE_CHANGE_IND, //1551
    BT_SUPERVISION_TIMEOUT_CHANGE_IND,
    BT_SNIFF_SUB_RATING_IND,

    BT_INQUIRY_CANCEL_CFM,
    BT_INQUIRY_REQ_CFM,
    BT_INQUIRY_STOP_IND,
    BT_INQUIRY_RESULT_IND,
    BT_READ_REMOTE_NAME_CFM,
    BT_READ_REMOTE_NAME_COMPLETE_IND,
    BT_READ_REMOTE_NAME_CANCEL_CFM,
    BT_READ_LOCAL_NAME_CFM, //1561
    BT_WRITE_INQUIRY_TX_CFM,
    BT_READ_INQUIRY_TX_CFM,
    BT_READ_CLASS_OF_DEVICE_CFM,
    BT_WRITE_CLASS_OF_DEVICE_CFM,
    BT_WRITE_PAGE_TIMEOUT_CFM,

    BT_SYNC_REGISTER_CFM,
    BT_SYNC_UNREGISTER_CFM,
    BT_SYNC_CONNECT_CFM,
    BT_SYNC_CONNECT_IND,
    BT_SYNC_DISCONNECT_CFM, /* 1571*/
    BT_SYNC_DISCONNECT_IND,
    BT_SYNC_RENEGOTIATE_IND,
    BT_SYNC_SET_RETRANSMISSION_ENABLE_CFM,

    BT_READ_LOCAL_BD_ADDR_CFM,
    BT_WRITE_LOCAL_BD_ADDR_CFM,
    BT_READ_LINK_QUALITY_CFM,
    BT_READ_RSSI_CFM, //629a
    BT_READ_AFH_CHANNEL_MAP_CFM,
    BT_REMOTE_FEATURES_CFM,
    BT_READ_LOCAL_VERSION_CFM, /* 1581 */
    BT_READ_REMOTE_VERSION_CFM,
    BT_READ_CLOCK_OFFSET_CFM,
    BT_SET_PACKET_TYPE_CFM,
    BT_GET_EDR_LINK_STATUS_IND,
    BT_READ_CLOCK_CFM, /* 50 */

    BT_ACL_OPENED_IND,
    BT_ACL_CLOSED_IND,
    BT_ACL_DETACH_CFM,
    BT_SM_READ_LOCAL_OOB_DATA_CFM,
    BT_SM_AUTHENTICATE_CFM, /* 1591 */
    BT_SM_SET_DEVICE_CONFIG_CFM,
    BT_SM_SET_SERVICE_CONFIG_CFM,
    BT_SM_UNSET_SERVICE_CONFIG_CFM,
    BT_SM_PIN_CODE_IND,
    BT_SM_IO_CAPABILITY_REQ_IND,
    BT_SM_REMOTE_IO_CAPABILITY_IND,
    BT_SM_USER_CONFIRMATION_REQ_IND,
    BT_SM_USER_PASSKEY_REQ_IND,
    BT_SM_USER_PASSKEY_NOTIFICATION_IND,
    BT_SM_KEYPRESS_NOTIFICATION_IND, /* 1601 */
    BT_SM_AUTHORISE_IND,
    BT_SM_ENCRYPT_CFM,
    BT_SM_ENCRYPT_IND,
    BT_SM_ADD_AUTH_DEVICE_CFM,
    BT_SM_GET_AUTH_DEVICE_CFM,
    BT_SM_GET_AUTH_DEVICE_LIST_CFM,
    BT_SM_DELETE_AUTH_DEVICE_CFM,
    BT_SM_DELETE_ALL_AUTH_DEVICE_CFM,
    BT_SM_SECURITY_REQ_CFM,
    BT_SM_AUTHENTICATION_IND, /* 1611 */
    BT_SM_OOB_DATA_REQ_IND,
    BT_SM_SECURITY_EVENT_IND,
    BT_SM_ADD_AUTH_DEVICE_LIST_CFM,
    BT_SM_GET_AUTH_DEVICE_LIST_EX_CFM,

    /*SDP */
    BT_SDP_REGISTER_CFM,                    /*!< Confirmation of \ref BtSdpRegisterServiceRecord.*/
    BT_SDP_UNREGISTER_CFM,                  /*!< Confirmation of \ref BtSdpUnregisterServiceRecord.*/
    BT_SDP_OPEN_SEARCH_CFM,                 /*!< Confirmation of \ref BtSdpOpenSearchRequest.*/
    BT_SDP_CLOSE_SEARCH_CFM,                /*!< Confirmation of \ref BtSdpCloseSearchRequest.*/
    BT_SDP_SERVICE_SEARCH_CFM,              /*!< Confirmation of \ref BtSdpServiceSearchRequest.*/
    BT_SDP_ATTRIBUTE_SEARCH_CFM,            /*!< Confirmation of \ref BtSdpAttributeSearchRequest.*/ /* 1621 */
    BT_SDP_SERVICE_SEARCH_ATTRIBUTE_CFM,    /*!< Confirmation of \ref BtSdpServiceSearchAttributeRequest.*/
    BT_SDP_SERVER_CONNECT_IND,
    BT_SDP_SERVER_DISCONNECT_IND,

    BT_L2CAP_REGISTER_CFM,
    BT_L2CAP_UNREGISTER_CFM,
    BT_L2CAP_CONNECT_CFM,
    BT_L2CAP_CONNECT_IND,
    BT_L2CAP_CONNECTIONLESS_DATA_IND,
    BT_L2CAP_MAP_CONNECTIONLESS_CFM,
    BT_L2CAP_MAP_CONNECTIONLESS_IND, /* 1631 */
    BT_L2CAP_UNMAP_CONNECTIONLESS_IND,
    BT_L2CAP_DISCONNECT_IND,
    BT_L2CAP_DISCONNECT_CFM,
    BT_L2CAP_TIMEOUT_IND,
    BT_L2CAP_INFO_RSP,                   /* 100 */
    BT_L2CAP_ECHO_RSP,

    BT_RFCOMM_REGISTER_CFM,       /*!< Confirmation of \ref BtRfcommRegisterChannel.*/
    BT_RFCOMM_UNREGISTER_CFM,     /*!< Confirmation of \ref BtRfcommUnregisterChannel.*/
    BT_RFCOMM_CLIENT_CONNECT_CFM, /*!< Confirmation of \ref BtRfcommConnectRequest.*/
    BT_RFCOMM_SERVER_CONNECT_CFM, /*!< Confirmation of \ref BtRfcommConnectResponse.*/ /* 1641*/
    BT_RFCOMM_CONNECT_IND,        /*!< Indication of remote connect requesting.*/
    BT_RFCOMM_LINE_STATUS_CFM,    /*!< Confirmation of \ref BtRfcommRemoteLineStatueRequest.*/
    BT_RFCOMM_DISCONNECT_IND,     /*!< Indication of remote disconnect requesting.*/
    BT_RFCOMM_DISCONNECT_CFM,     /*!< Confirmation of \ref BtRfcommDisconnectRequest.*/

    /*AVCTP*/
    BT_AVCTP_REGISTER_CFM,
    BT_AVCTP_UNREGISTER_CFM,
    BT_AVCTP_CONNECT_CFM,
    BT_AVCTP_CONNECT_IND,
    BT_AVCTP_DISCONNECT_CFM,
    BT_AVCTP_MORE_DATA,

    /*BLE*/
    BT_BLE_ADV_REPORT_IND,
    BT_BLE_SET_SCAN_ENABLE_CFM,
    BT_BLE_SET_SCAN_PARAMS_CFM,
    BT_BLE_SET_SCAN_RESPONSE_DATA_CFM,
    BT_BLE_SET_ADV_DATA_CFM,
    BT_BLE_SET_ADV_ENABLE_CFM,
    BT_BLE_SET_ADV_PARAMS_CFM,
    BT_BLE_SET_LOCAL_RANDOM_ADDR_CFM,
    BT_BLE_READ_WHITE_LIST_SIZE_CFM,
    BT_BLE_CLEAR_WHITE_LIST_CFM,
    BT_BLE_ADD_DEVICE_TO_WHITE_LIST_CFM,
    BT_BLE_REMOVE_DEVICE_FROM_WHITE_LIST_CFM,
    BT_BLE_CONNECTION_PARAMS_UPDATE_CFM,
    BT_BLE_CONNECTION_PARAMS_UPDATE_IND,
    BT_BLE_DISCONNECT_REQ_RSP,
    BT_BLE_SM_SET_SERVICE_CONFIG_CFM,
    BT_BLE_GET_LINK_STATUS_IND,
    BT_BLE_SET_ADV_ENABLE_ALL_CFM,
    BT_BLE_SET_ADV_DISABLE_ALL_CFM,
    BT_BLE_GEN_RESOLVABLE_PRIVATE_ADDR_RSP,

    /* ATT */
    BT_ATT_LINK_STATUS_IND,
    BT_ATT_ACCESS_IND,
    BT_ATT_TX_AVAILABLE_IND,
    BT_ATT_NOTIFICATION_CFM,
    BT_ATT_INDICATION_CFM,

    /* GATT */
    BT_GATT_EXCHANGE_MTU_IND,
    BT_GATT_TIMEOUT_IND,
    BT_GATT_START_SERVER_CFM,
    BT_GATT_STOP_SERVER_CFM,

    BT_GAP_DUTMODE_CFM,
    BT_GAP_CANCEL_CONN_CFM,
    BT_WRITE_INQUIRY_ACCESS_CODE_CFM,
    BT_WRITE_INQUIRY_MODE_CFM,
    BT_WRITE_INQUIRY_SCAN_TYPE_CFM,
    BT_READ_SCAN_ENABLE_CFM,
    BT_WRITE_SCAN_ENABLE_CFM,                        /* 150 */
    BT_WRITE_PAGE_SCAN_TYPE_CFM,
    BT_WRITE_INQSCAN_ACTIVITY_CFM,
    BT_WRITE_PAGESCAN_ACTIVITY_CFM,
    BT_WRITE_FLUSH_TIMEOUT_CFM,
    BT_WRITE_LOCAL_NAME_CFM,
    BT_READ_INQUIRY_MODE_CFM,
    BT_READ_EIR_DATA_CFM,
    BT_WRITE_EIR_DATA_CFM,

    BT_AVDTP_REGISTER_CFM,
    BT_AVDTP_UNREGISTER_CFM,
    BT_AVDTP_SIGNAL_CONNECT_CFM,
    BT_AVDTP_SIGNAL_DISCONNECT_CFM,
    BT_AVDTP_SIGNAL_CONNECT_IND,
    BT_AVDTP_MEDIA_CONNECT_CFM,
    BT_AVDTP_MEDIA_DISCONNECT_CFM,
    BT_AVDTP_MEDIA_CONNECT_IND,

    BT_AVDTP_SIGNAL_DISCOVER_CMD_IND,
    BT_AVDTP_SIGNAL_GET_CAPABILITIES_CMD_IND,
    BT_AVDTP_SIGNAL_SET_CONFIGURATION_CMD_IND,
    BT_AVDTP_SIGNAL_GET_CONFIGURATION_CMD_IND,
    BT_AVDTP_SIGNAL_RECONFIGURE_CMD_IND,
    BT_AVDTP_SIGNAL_OPEN_CMD_IND,
    BT_AVDTP_SIGNAL_START_CMD_IND,
    BT_AVDTP_SIGNAL_CLOSE_CMD_IND,
    BT_AVDTP_SIGNAL_SUSPEND_CMD_IND,
    BT_AVDTP_SIGNAL_ABORT_CMD_IND,
    BT_AVDTP_SIGNAL_SECURITY_CONTROL_CMD_IND,
    BT_AVDTP_SIGNAL_DELAY_REPORT_CMD_IND,
    BT_AVDTP_SIGNAL_GET_ALL_CAPABILITIES_CMD_IND,

    BT_AVDTP_SIGNAL_DISCOVER_RSP_IND,
    BT_AVDTP_SIGNAL_GET_CAPABILITIES_RSP_IND,
    BT_AVDTP_SIGNAL_SET_CONFIGURATION_RSP_IND,
    BT_AVDTP_SIGNAL_OPEN_RSP_IND,
    BT_AVDTP_SIGNAL_START_RSP_IND,
    BT_AVDTP_SIGNAL_SUSPEND_RSP_IND,
    BT_AVDTP_SIGNAL_GET_ALL_CAPABILITIES_RSP_IND,
    BT_AVDTP_SIGNAL_RECONFIGURE_RSP_IND,
    BT_AVDTP_SIGNAL_GET_CONFIGURATION_RSP_IND,
    BT_AVDTP_SIGNAL_CLOSE_RSP_IND,
    BT_AVDTP_SIGNAL_ABORT_RSP_IND,
    BT_AVDTP_SIGNAL_SECURITY_CONTROL_RSP_IND,
    BT_AVDTP_SIGNAL_DELAY_REPORT_RSP_IND,

	BT_APP_DSP_REALTIME_SUSPEND_DSP,
	BT_APP_DSP_REALTIME_RESUME_DSP,
	BT_APP_DSP_REALTIME_ANC_ADAPTIVE_CHECK,
	BT_APP_DSP_SOUND_EFFECT_UPDATE,
	BT_APP_DSP_PEQ_UPDATE,
	BT_APP_DSP_PEQ_SAVE_STATUS,

    BT_AWS_MCE_REGISTER_CFM,
    BT_APP_HCI_CFM,                                        /* 200 */
    BT_AWS_MCE_CONNECT_CFM,
    BT_AWS_MCE_STATUS_CHANGE_IND,
    BT_AWS_MCE_DISCONNECT_IND,
    BT_AWS_MCE_READ_CLOCK_CFM,
    BT_AWS_MCE_PREPARE_ROLE_HANDOVER_CFM,
    BT_AWS_MCE_CANCEL_ROLE_HANDOVER_CFM,
    BT_AWS_MCE_SET_ROLE_HANDOVER_FINISH_CFM,
    BT_AWS_MCE_RHO_ROLE_CHANGE_IND,
    BT_AWS_MCE_DISCONNECT_CFM,
    BT_AWS_MCE_SHARE_MODE_ENABLE_CFM,

	BT_REC_CLEAR_RECLIST_BDA,

	BT_AUDIO_DSP_A2DP_OUT_OF_SYNC_IND,

    /* BLE */
    BT_BLE_SET_ADV_PARAMS_EX_CFM,

    BT_SET_LOW_LATENCY_MODE_ENABLE_CFM,

<<<<<<< HEAD
=======
    BT_CONNECT_REQUEST_IND,

    BT_SET_LC_CONTROL_MODE_ENABLE_CFM,

>>>>>>> db20e11 (second commit)
    BT_MSG_E_MAX,
} BT_MSG_E;

/*!
    @brief 128 bit integer
*/
typedef struct
{
    U8 dat[16];
} U128;

/*!
    @brief UUID
*/
typedef enum {
    BT_UUID_UNSPEC = 0,
    BT_UUID_16 = 2,
    BT_UUID_32 = 4,
    BT_UUID_128 = 16,
} BT_UUID_TYPE_E;

typedef struct
{
    BT_UUID_TYPE_E type;
    union {
        U16 u16;
        U32 u32;
        U128 u128;
    } value;
} BT_UUID_T;

typedef enum
{
    PSM_SDP = 0x0001,
    PSM_RFCOMM = 0x0003,
    PSM_HID_CTL = 0x0011,
    PSM_HID_INT = 0x0013,
    PSM_AVCTP = 0x0017,
    PSM_AVDTP = 0x0019,
<<<<<<< HEAD
=======
    PSM_ATT   = 0x001F,
>>>>>>> db20e11 (second commit)
} PSM_ENUM_E;

/*!
    @brief Status code of BT libraries
*/
typedef enum BT_GENERIC_STATUS_E {
    BT_STATUS_SUCCESS = 0,
    BT_STATUS_GENERIC_FAIL,
    BT_STATUS_NOT_EXISTED,
    BT_STATUS_ALREADY_EXISTED,
    BT_STATUS_TIMEOUT,
    BT_STATUS_NOT_ALLOWED, //5
    BT_STATUS_HCI_CMD_FAIL,
    BT_STATUS_LINK_NOT_EXISTED,
    BT_STATUS_ABORT,
    BT_STATUS_AUTH_FAIL,
    BT_STATUS_ENC_FAIL, //10
    BT_STATUS_NO_RESOURCE,
    BT_STATUS_INVALID_PARAM,
    BT_STATUS_LOCAL_NOT_REGISTERED,
    BT_STATUS_UNDER_DEVELOPMENT,
    BT_STATUS_PRIORI_CONDITION_NEEDED, //15
    BT_STATUS_NOT_SUPPORTED,
    BT_STATUS_REJECT_BY_REMOTE,
    BT_STATUS_OTHER_OPERATION_ONGOING,
    BT_STATUS_CONNECTION_FAIL,
    BT_STATUS_BUSY, //20
    BT_STATUS_PARAMETERS_REJECTED,
    BT_STATUS_SUCCESS_SEND,
    BT_STATUS_SDP_ERROR,
	BT_STATUS_REMOTE_ACCEPT_TIMEOUT,
	BT_STATUS_SECURITY_BLOCKED, //25
    BT_STATUS_MAX
} BT_GENERIC_STATUS_E;

typedef enum {
    HCI_STATUS_SUCCESS,
    HCI_STATUS_UNKNOWN_HCI_COMMAND,
    HCI_STATUS_UNKNOWN_CONNECTION_IDENTIFIER,
    HCI_STATUS_HARDWARE_FAILURE,
    HCI_STATUS_PAGE_TIMEOUT,
    HCI_STATUS_AUTHENTICATION_FAILURE, //5
    HCI_STATUS_PIN_OR_KEY_MISSING,
    HCI_STATUS_MEMORY_CAPACITY_EXCEEDED,
    HCI_STATUS_CONNECTION_TIMEOUT,
    HCI_STATUS_CONNECTION_LIMIT_EXCEEDED,
    HCI_STATUS_SYNCHRONOUS_CONNECTION_LIMIT_TO_A_DEVICE_EXCEEDED, //10
    HCI_STATUS_ACL_CONNECTION_ALREADY_EXISTS,
    HCI_STATUS_COMMAND_DISALLOWED,
    HCI_STATUS_CONNECTION_REJECTED_DUE_TO_LIMITED_RESOURCES,
    HCI_STATUS_CONNECTION_REJECTED_DUE_TO_SECURITY_REASONS,
    HCI_STATUS_CONNECTION_REJECTED_DUE_TO_UNACCEPTABLE_BD_ADDR, //15
    HCI_STATUS_CONNECTION_ACCEPT_TIMEOUT_EXCEEDED,
    HCI_STATUS_UNSUPPORTED_FEATURE_OR_PARAMETER_VALUE,
    HCI_STATUS_INVALID_HCI_COMMAND_PARAMETERS,
    HCI_STATUS_REMOTE_USER_TERMINATED_CONNECTION,
    HCI_STATUS_REMOTE_DEVICE_TERMINATED_CONNECTION_DUE_TO_LOW_RESOURCES, //20
    HCI_STATUS_REMOTE_DEVICE_TERMINATED_CONNECTION_DUE_TO_POWER_OFF,
    HCI_STATUS_CONNECTION_TERMINATED_BY_LOCAL_HOST,
    HCI_STATUS_REPEATED_ATTEMPTS,
    HCI_STATUS_PAIRING_NOT_ALLOWED,
    HCI_STATUS_UNKNOWN_LMP_PDU, //25
    HCI_STATUS_UNSUPPORTED_REMOTE_FEATURE_UNSUPPORTED_LMP_FEATURE,
    HCI_STATUS_SCO_OFFSET_REJECTED,
    HCI_STATUS_SCO_INTERVAL_REJECTED,
    HCI_STATUS_SCO_AIR_MODE_REJECTED,
    HCI_STATUS_INVALID_LMP_PARAMETERS, //30
    HCI_STATUS_UNSPECIFIED_ERROR,
    HCI_STATUS_UNSUPPORTED_LMP_PARAMETER_VALUE,
    HCI_STATUS_ROLE_CHANGE_NOT_ALLOWED,
    HCI_STATUS_LMP_RESPONSE_TIMEOUT, //34
    HCI_STATUS_LMP_ERROR_TRANSACTION_COLLISION,
    HCI_STATUS_LMP_PDU_NOT_ALLOWED,
    HCI_STATUS_ENCRYPTION_MODE_NOT_ACCEPTABLE,
    HCI_STATUS_LINK_KEY_CAN_NOT_BE_CHANGED,
    HCI_STATUS_REQUESTED_QOS_NOT_SUPPORTED,
    HCI_STATUS_INSTANT_PASSED,
    HCI_STATUS_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED,
    HCI_STATUS_DIFFERENT_TRANSACTION_COLLISION,
    HCI_STATUS_0x2B,
    HCI_STATUS_QOS_UNACCEPTABLE_PARAMETER,
    HCI_STATUS_QOS_REJECTED,
    HCI_STATUS_CHANNEL_CLASSIFICATION_NOT_SUPPORTED,
    HCI_STATUS_INSUFFICIENT_SECURITY,
    HCI_STATUS_PARAMETER_OUT_OF_MANDATORY_RANGE,
    HCI_STATUS_0x31,
    HCI_STATUS_ROLE_SWITCH_PENDING,
    HCI_STATUS_0x33,
    HCI_STATUS_RESERVED_SLOT_VIOLATION,
    HCI_STATUS_ROLE_SWITCH_FAILED,
    HCI_STATUS_EXTENDED_INQUIRY_RESPONSE_TOOL_LARGE,
    HCI_STATUS_SIMPLE_PAIRING_NOT_SUPPORTED_BY_HOST,
    HCI_STATUS_HOST_BUSY_PAIRING,
    HCI_STATUS_CONNECTION_REJECT_DUE_TO_NO_SUITABLE_CHANNEL_FOUND,
    HCI_STATUS_CONTROLLER_BUSY,
    HCI_STATUS_UNACCEPT_CONNECTION_PARAMETER,
    HCI_STATUS_DIRECTED_ADVERTISING_TIMEOUT,
    HCI_STATUS_CONN_TERMINATED_DUE_TO_MIC_FAILURE,
    HCI_STATUS_CONNECTION_FAILED_TO_BE_ESTABLISHED,
    HCI_STATUS_MAC_CONNECTION_FAIL,
    HCI_STATUS_COARSE_CLOCK_ADJUST_REJECT,
    HCI_STATUS_MAX
} HCI_STATUS_E;
/*!
    @brief Message handle procedure
    It was used for searching id by table lookup way.
*/
typedef U32 (*MSG_HANDLE_PROC)(void *, Handler, U32);

/*!
    @brief Id and handler paired structure
    It was used for searching id by table lookup way.
*/
typedef struct
{
    U16 id;
    MSG_HANDLE_PROC fn;
} MSG_HANDLER_ENTRY_T;


typedef struct
{
  U8 status; ///**< HCI error code*/
  U16 op;
  void *saved_info;
  U8 len;
  U8* resp;
} PACKED BT_APP_HCI_CFM_T;

typedef struct
{
	U8 addr[6];
}BT_AUDIO_DSP_A2DP_OUT_OF_SYNC_IND_T;

/*!
    @}
*/
/**************************************************************************************************/
/*                            Functions                                                           */
/**************************************************************************************************/
/*!
    @addtogroup BT_LIB_COMMON_FN
    @{
*/
/*
    @brief Initialize Airoha bluetooth core stacks
    @param app The client handler. All the unsolicited message(usually _IND suffix) would be send here.
    @param id Controller id to be initialized.
*/
void BtInit(Handler app, BT_CONTROLER_ID_E id);

/*
    @brief Convert BT message enumerations to string
    @retval String of corresponding message.
*/
const char *BtMsgString(BT_MSG_E msg);

/*
    @brief Covert BT status enumerations to string.
    @return String of corresponding status.
*/
const char *BtStatusString(BT_GENERIC_STATUS_E status);

/*
    @brief Covert HCI error code to string.
    @return String of corresponding status.
*/
const char *BtHciStatusString(HCI_STATUS_E hci_status);

/*!
    @brief De-initialize bluetooth stacks.
*/
void BtDeinit(Handler app);



void BtHciCmdSend(Handler app,U16 op,U8 len,U8* para);



#endif

/**************************************************************************************************/
/*                            Macros                                                              */
/**************************************************************************************************/
/*!
    @addtogroup
    @{
*/
/*!
    @}
*/
/**************************************************************************************************/
/*                            Data Structures                                                     */
/**************************************************************************************************/
/*!
    @addtogroup
    @{
*/
/*!
    @}
*/
/**************************************************************************************************/
/*                            Functions                                                           */
/**************************************************************************************************/
/*!
    @addtogroup
    @{
*/
/*!
    @}
*/
