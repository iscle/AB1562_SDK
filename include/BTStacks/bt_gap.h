<<<<<<< HEAD
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

#ifndef __BT_GAP_HH
#define __BT_GAP_HH

/**************************************************************************************************/
/*                            Macros                                                              */
/**************************************************************************************************/
/*!
    @addtogroup BT_LIB_GAP_MACRO
    @{
*/
/*!
@name GAP AD Data Type [Defines]
GAP Advertisement Type
@{
*/
#define EIR_TYPE_FLAGS 0x01                              ///< Flags for discover ability.
#define EIR_TYPE_16BIT_SERVICE_UUID_MORE_AVAILABLE 0x02  ///< Partial list of 16 bit service UUIDs.
#define EIR_TYPE_16BIT_SERVICE_UUID_COMPLETE 0x03        ///< Complete list of 16 bit service UUIDs.
#define EIR_TYPE_32BIT_SERVICE_UUID_MORE_AVAILABLE 0x04  ///< Partial list of 32 bit service UUIDs.
#define EIR_TYPE_32BIT_SERVICE_UUID_COMPLETE 0x05        ///< Complete list of 32 bit service UUIDs.
#define EIR_TYPE_128BIT_SERVICE_UUID_MORE_AVAILABLE 0x06 ///< Partial list of 128 bit service UUIDs.
#define EIR_TYPE_128BIT_SERVICE_UUID_COMPLETE 0x07       ///< Complete list of 128 bit service UUIDs.
#define EIR_TYPE_SHORT_LOCAL_NAME 0x08                   ///< Short local device name.
#define EIR_TYPE_COMPLETE_LOCAL_NAME 0x09                ///< Complete local device name.
#define EIR_TYPE_TX_POWER_LEVEL 0x0A                     ///< Transmit power level.
#define EIR_TYPE_CLASS_OF_DEVICE 0x0D                    ///< Class of device.
#define EIR_TYPE_SIMPLE_PAIRING_HASH_C 0x0E              ///< Simple Pairing Hash C.
#define EIR_TYPE_SIMPLE_PAIRING_RANDOMIZER_R 0x0F        ///< Simple Pairing Randomizer R.
#define EIR_TYPE_SECURITY_MANAGER_TK_VALUE 0x10          ///< Security Manager TK Value.
#define EIR_TYPE_SECURITY_MANAGER_OOB_FLAGS 0x11         ///< Security Manager Out Of Band Flags.
#define EIR_TYPE_SLAVE_CONNECTION_INTERVAL_RANGE 0x12    ///< Slave Connection Interval Range.
#define EIR_TYPE_SOLICITED_SERVICE_UUIDS_16BIT 0x14      ///< List of 16-bit Service Solicitation UUIDs.
#define EIR_TYPE_SOLICITED_SERVICE_UUIDS_128BIT 0x15     ///< List of 128-bit Service Solicitation UUIDs.
#define EIR_TYPE_SERVICE_DATA 0x16                       ///< Service Data - 16-bit UUID.
#define EIR_TYPE_PUBLIC_TARGET_ADDRESS 0x17              ///< Public Target Address.
#define EIR_TYPE_RANDOM_TARGET_ADDRESS 0x18              ///< Random Target Address.
#define EIR_TYPE_APPEARANCE 0x19                         ///< Appearance.
#define EIR_TYPE_ADVERTISING_INTERVAL 0x1A               ///< Advertising Interval.
#define EIR_TYPE_LE_BLUETOOTH_DEVICE_ADDRESS 0x1B        ///< LE Bluetooth Device Address.
#define EIR_TYPE_LE_ROLE 0x1C                            ///< LE Role.
#define EIR_TYPE_SIMPLE_PAIRING_HASH_C256 0x1D           ///< Simple Pairing Hash C-256.
#define EIR_TYPE_SIMPLE_PAIRING_RANDOMIZER_R256 0x1E     ///< Simple Pairing Randomizer R-256.
#define EIR_TYPE_SERVICE_DATA_32BIT_UUID 0x20            ///< Service Data - 32-bit UUID.
#define EIR_TYPE_SERVICE_DATA_128BIT_UUID 0x21           ///< Service Data - 128-bit UUID.
#define EIR_TYPE_UNIFORM_RESOUCE_IDENTIFIER 0x24         ///< Uniform Resource Identifier
#define EIR_TYPE_3D_INFORMATION_DATA 0x3D                ///< 3D Information Data.
#define EIR_TYPE_MANUFACTURER_SPECIFIC_DATA 0xFF         ///< Manufacturer Specific Data.
/*!
@}
*/
#define BT_GAP_LE_AD_FLAG_LIMITED_DISCOVERABLE     (0x01 << 0) /**< LE limited discoverable mode. */
#define BT_GAP_LE_AD_FLAG_GENERAL_DISCOVERABLE     (0x01 << 1) /**< LE general discoverable mode. */
#define BT_GAP_LE_AD_FLAG_BR_EDR_NOT_SUPPORTED     (0x01 << 2) /**< BR/EDR not supported. */
/*!
    @}
*/
/**************************************************************************************************/
/*                            Data Structures                                                     */
/**************************************************************************************************/
/*!
    @addtogroup BT_LIB_GAP_DS
    @{
*/
/*!
	@brief HCI error code.
*/

#ifdef WIN32
#pragma pack(push, 1)
#endif

/*!
    @brief Leagcy BT mac address
*/
typedef struct BD_ADDR_T
{
    U8 addr[6];
}BD_ADDR_T;

/*!
    @brief BT mac address with type
*/
typedef struct BDADDR_T
{
    U8 type;
    U8 addr[6];
}BDADDR_T;

#ifdef WIN32
#pragma pack(pop)
#endif

typedef enum PAGE_SCAN_REP_MODE_T {
    PAGE_SCAN_REP_MODE_R0,
    PAGE_SCAN_REP_MODE_R1,
    PAGE_SCAN_REP_MODE_R2,
    PAGE_SCAN_REP_MODE_UNKNOWN
} PAGE_SCAN_REP_MODE_E;

typedef enum SCAN_TYPE_T {
    SCAN_TYPE_STANDARD,
    SCAN_TYPE_INTERLANCE
} SCAN_TYPE_T;

typedef enum SCAN_ENABLE_T {
    SCAN_ENABLE_OFF,
    SCAN_ENABLE_INQ,
    SCAN_ENABLE_PAGE,
    SCAN_ENABLE_INQ_AND_PAGE
} SCAN_ENABLE_T;

typedef enum INQUARY_MODE_E {
    INQUARY_MODE_STARNDAR,
    INQUARY_MODE_RSSI,
    INQUARY_MODE_RSSI_EIR,
} INQUARY_MODE_E;

typedef enum LINK_KEY_TYPE_E {
    LINK_KEY_COMBINATION = 0,
    LINK_KEY_LOCAL_UNIT,
    LINK_KEY_REMOTE_UNIT,
    LINK_KEY_DEBUG_COMBINATION,
    LINK_KEY_UNAUTHENTICATED,
    LINK_KEY_AUTHENTICATED,
    LINK_KEY_CHANGED_COMBINATION,
    LINK_KEY_UNAUTHENTICATED_P256,
    LINK_KEY_AUTHENTICATED_P256,
} LINK_KEY_TYPE_E;

typedef enum LINK_MODE_E {
    LINK_MODE_ACTIVE,
    LINK_MODE_HOLD,
    LINK_MODE_SNIFF,
    LINK_MODE_PARK,
} LINK_MODE_E;

typedef enum ROLE_E {
    ROLE_MASTER,
    ROLE_SLAVE,
} ROLE_E;


typedef enum CLOCK_E {
    LOCAL_CLOCK,
    PICONET_CLOCK,
} CLOCK_E;


typedef enum BT_SM_SECURITY_EVENT_E {
    BT_SM_ENCRYPTION_ON,
    BT_SM_LINK_KEY_MISSING,
} BT_SM_SECURITY_EVENT_E;

typedef enum
{
    BT_DISCONNECT_REASON_AUTHENTICATION_FAILURE = 0x05,
    BT_DISCONNECT_REASON_REMOTE_USER_TERMINATED_CONNECTION = 0x13,
    BT_DISCONNECT_REASON_REMOTE_TERMINATED_CONNECTION_DUE_TO_LOW_RESOURCES = 0x14,
    BT_DISCONNECT_REASON_REMOTE_TERMINATED_CONNECTION_DUE_TO_POWER_OFF = 0x15,
    BT_DISCONNECT_REASON_UNSUPPORTED_REMOTE_FEATURE = 0x1A,
    BT_DISCONNECT_REASON_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED = 0x29,
    BT_DISCONNECT_REASON_UNACCEPTABLE_CONNECTION_PARAMETERS = 0x3B,
} BT_DISCONNECT_REASON_E;

typedef struct LINK_MODE_HOLD_PARAM_T
{
    U16 beacon_max_interval;
    U16 beacon_min_interval;
} LINK_MODE_HOLD_PARAM_T, LINK_MODE_PARK_PARAM_T;


typedef struct LINK_MODE_SNIFF_PARAM_T
{
    U16 sniff_max_interval;
    U16 sniff_min_interval;
    U16 sniff_attempt;
    U16 sniff_timeout;
} LINK_MODE_SNIFF_PARAM_T;

typedef struct LINK_MODE_PARAM_T
{
    union {
        LINK_MODE_HOLD_PARAM_T hold_parm;
        LINK_MODE_SNIFF_PARAM_T sniff_parm;
        LINK_MODE_PARK_PARAM_T park_parm;
    };
} LINK_MODE_PARAM_T;


typedef enum BT_GAP_SYSTEM_STATUS_E {
    BT_GAP_SYSTEM_STATUS_NONE,
    BT_GAP_SYSTEM_STATUS_READY_TO_POWER_OFF,
} BT_GAP_SYSTEM_STATUS_E;

typedef enum BT_GAP_GET_ROLE_STATUS_E {
    BT_GAP_LINK_NOT_EXIST,
    BT_GAP_LINK_ROLE_SLAVE,
    BT_GAP_LINK_ROLE_MASTER,
} BT_GAP_GET_ROLE_STATUS_E;

typedef struct BT_SYNC_CONFIG_PARAM_T
{
    U32 tx_bandwidth;
    U32 rx_bandwidth;
    U16 max_latency;
    U16 voice_settings;
    U8 retx_effort;
    U16 packet_type;
} BT_SYNC_CONFIG_PARAM_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
	U8 bdaddr[6];
} BT_INIT_CFM_T, BT_DEINIT_CFM_T, BT_SYNC_REGISTER_CFM_T, BT_SYNC_UNREGISTER_CFM_T, BT_SET_MODE_CFM_T, BT_RFCOMM_LINE_STATUS_CFM_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
    HCI_STATUS_E hci_status;
} BT_INQUIRY_CANCEL_CFM_T, BT_INQUIRY_REQ_CFM_T;

typedef struct
{
    HCI_STATUS_E status;
} BT_SET_SUPERVISION_CFM_T, BT_SET_SNIFF_SUBRATING_CFM_T, BT_INQUIRY_STOP_IND_T,BT_SET_LINK_POLICY_CFM_T;



typedef struct BT_GET_ROLE_CFM_T
{
    HCI_STATUS_E status;
    ROLE_E role;
	U8 bdaddr[6];
} BT_GET_ROLE_CFM_T, BT_SET_ROLE_CFM_T;

typedef struct BT_QOS_SETUP_CFM_T
{
    HCI_STATUS_E status;
    U8 bdaddr[6];

    U8 flags;
    U8 service_type;
    U32 token_rate;
    U32 peak_bandwidth;
    U32 latency;
    U32 delay_variation;
} BT_QOS_SETUP_CFM_T;



typedef struct BT_ROLE_CHANGE_IND_T
{
    HCI_STATUS_E status;
    U8 bdaddr[6];
    ROLE_E role;
} BT_ROLE_CHANGE_IND_T;

typedef struct BT_MODE_CHANGE_IND_T
{
    HCI_STATUS_E status;
    U8 bdaddr[6];
    LINK_MODE_E mode;
    U16 interval;
} BT_MODE_CHANGE_IND_T;

typedef struct BT_SUPERVISION_TIMEOUT_CHANGE_IND_T
{
    U8 bdaddr[6];
    U16 timeout;
} BT_SUPERVISION_TIMEOUT_CHANGE_IND_T;

typedef struct BT_SNIFF_SUB_RATING_IND_T
{
    HCI_STATUS_E status;
    U8 bdaddr[6];
    U16 max_rx_latency;
    U16 max_tx_latency;
    U16 min_remote_timeout;
    U16 min_local_timeout;
} BT_SNIFF_SUB_RATING_IND_T;

typedef struct BT_SM_AUTHENTICATE_CFM_T
{
    BT_GENERIC_STATUS_E status;
    U8 bdaddr[6];
} BT_SM_AUTHENTICATE_CFM_T;

typedef struct BT_GENERIC_CFM_T
{
    HCI_STATUS_E hci_status;
} BT_WRITE_INQUIRY_ACCESS_CODE_CFM_T, BT_WRITE_INQUIRY_SCAN_TYPE_CFM_T,
    BT_WRITE_PAGE_SCAN_TYPE_CFM_T, BT_WRITE_SCAN_ENABLE_CFM_T,
    BT_WRITE_INQSCAN_ACTIVITY_CFM_T, BT_WRITE_PAGESCAN_ACTIVITY_CFM_T,
    BT_WRITE_CLASS_OF_DEVICE_CFM_T, BT_WRITE_PAGE_TIMEOUT_CFM_T, BT_WRITE_FLUSH_TIMEOUT_CFM_T,
    BT_WRITE_LOCAL_NAME_CFM_T, BT_SM_SET_DEVICE_CONFIG_CFM_T, BT_WRITE_EIR_DATA_CFM_T,
    BT_WRITE_INQUIRY_TX_CFM_T, BT_WRITE_INQUIRY_MODE_CFM_T,
    BT_READ_REMOTE_NAME_CANCEL_CFM_T, BT_READ_REMOTE_NAME_CFM_T,BT_WRITE_LOCAL_BD_ADDR_CFM_T,
    BT_SYNC_SET_RETRANSMISSION_ENABLE_CFM_T,BT_AWS_MCE_SHARE_MODE_ENABLE_CFM_T,BT_SET_LOW_LATENCY_MODE_ENABLE_CFM_T;

typedef struct BT_SYNC_CONNECT_CFM_T
{
    HCI_STATUS_E hci_status;
    U8 link_type;
    U8 bd_addr[6];
    SINK sink;
    U16 rx_bandwidth;
    U16 tx_bandwidth;
    U16 sco_handle;
    U8 airmode;
} BT_SYNC_CONNECT_CFM_T;

typedef struct BT_SYNC_CONNECT_IND_T
{
    U8 link_type;
    U8 bd_addr[6];
} BT_SYNC_CONNECT_IND_T;

typedef struct BT_SYNC_DISCONNECT_IND_T
{
    HCI_STATUS_E hci_status;
    SINK sink;
    U8 reason;
} BT_SYNC_DISCONNECT_IND_T;

typedef struct BT_SYNC_DISCONNECT_CFM_T
{
    HCI_STATUS_E hci_status;
    SINK sink;
} BT_SYNC_DISCONNECT_CFM_T;




typedef struct BT_GENERIC_CFM_BOOL_T
{
    BT_GENERIC_STATUS_E status;
} BT_SM_SET_SERVICE_CONFIG_CFM_T, BT_SM_UNSET_DEVICE_CONFIG_CFM_T, BT_SM_UNSET_SERVICE_CONFIG_CFM_T;

typedef struct BT_READ_INQUIRY_MODE_CFM_T
{
    U8 hci_status;
    U8 mode;
} BT_READ_INQUIRY_MODE_CFM_T;

typedef struct BT_READ_LINK_QUALITY_CFM_T
{
    HCI_STATUS_E hci_status;
    U8 link_quality;
    SINK sink;
} BT_READ_LINK_QUALITY_CFM_T;

typedef struct BT_READ_RSSI_CFM_T
{
    HCI_STATUS_E hci_status;
    S8 rssi;
    U8 bd_addr[6];
} BT_READ_RSSI_CFM_T;


typedef struct BT_READ_AFH_CHANNEL_MAP_CFM_T
{
    HCI_STATUS_E hci_status;
    U8 bd_addr[6];
    U8 afh_mode_enable;
    U8 channel_map[10];
} BT_READ_AFH_CHANNEL_MAP_CFM_T;


typedef struct BT_READ_CLOCK_CFM_T
{
    HCI_STATUS_E hci_status;
    U32 clock;
    U16 accuracy;
    U8 bd_addr[6];
} BT_READ_CLOCK_CFM_T;


typedef struct BT_READ_REMOTE_VERSION_CFM_T
{
    HCI_STATUS_E hci_status;
    U8 lmpVersion;
    U16 manufacturerName;
    U16 lmpSubVersion;
    U16 handle;
    SINK sink;
} BT_READ_REMOTE_VERSION_CFM_T;

typedef struct BT_READ_LOCAL_VERSION_CFM_T
{
    HCI_STATUS_E hci_status;
    U8 hciVersion;
    U16 hciRevision;
    U8 lmpVersion;
    U16 manufacturerName;
    U16 lmpSubVersion;
} BT_READ_LOCAL_VERSION_CFM_T;

typedef struct BT_READ_CLOCK_OFFSET_CFM_T
{
    HCI_STATUS_E hci_status;
    SINK sink;
    U16 clock_offset;
} BT_READ_CLOCK_OFFSET_CFM_T;


typedef struct BT_SET_PACKET_TYPE_CFM_T
{
    HCI_STATUS_E hci_status;
    U16 connHandle;
    U16 pktType;
} BT_SET_PACKET_TYPE_CFM_T;



typedef struct BT_READ_LOCAL_BD_ADDR_CFM_T
{
    U8 hci_status;
    U8 bd_addr[6];
} BT_READ_LOCAL_BD_ADDR_CFM_T;

typedef struct BT_REMOTE_FEATURES_CFM_T
{
    U8 hci_status;
    U8 features[8];
    SINK sink;
} BT_REMOTE_FEATURES_CFM_T;

typedef struct BT_READ_SCAN_ENABLE_CFM_T
{
    U8 hci_status;
    U8 scan_enable;
} BT_READ_SCAN_ENABLE_CFM_T;

typedef struct BT_READ_LOCAL_NAME_CFM_T
{
    U8 hci_status;
    U8 name[248];
} BT_READ_LOCAL_NAME_CFM_T;

typedef struct BT_INQUIRY_RESULT_IND_T
{
    U8 bd_addr[6];
    PAGE_SCAN_REP_MODE_E rep_mode;
    U32 COD;
    U16 clock_offset;
    S8 rssi;
    U8 eirlen;
    U8 eir[1];
} BT_INQUIRY_RESULT_IND_T;

typedef struct BT_READ_REMOTE_NAME_COMPLETE_IND_T
{
    U8 hci_status;
    U8 bd_addr[6];
    U8 length;
    U8 remote_name[248];
} BT_READ_REMOTE_NAME_COMPLETE_IND_T;

typedef struct BT_EDR_LINK_INFO_T
{
    U8 bd_addr[6];
    U8 remote_name[30];
} PACKED BT_EDR_LINK_INFO_T;

typedef struct BT_GET_EDR_LINK_STATUS_IND_T
{
    U8 status;
    U8 actived_link_num;
    U8 link[1];
} BT_GET_EDR_LINK_STATUS_IND_T;

typedef struct BT_READ_INQUIRY_TX_CFM_T
{
    U8 hci_status;
    S8 tx_power;
} BT_READ_INQUIRY_TX_CFM_T;

typedef struct BT_ACL_OPENED_IND_T
{
    /*! HCI status*/
    U8 status;
    /*! remote BT address*/
    U8 addrType;
    U8 bdaddr[6];
    BOOL isBle;
} BT_ACL_OPENED_IND_T;

typedef struct BT_SM_ENCRYPT_IND_T
{
    U8 bdAddr[6]; /*! The Bluetooth address.*/
    U8 isEncrypted;
    U8 isKeyRefresh;
} BT_SM_ENCRYPT_IND_T;

typedef struct BT_ACL_CLOSED_IND_T
{
    /*! HCI status*/
    U8 status;
    /*! reason */
    U8 reason;
    /*! remote BT address*/
    U8 addrType;
    U8 bdaddr[6];
    BOOL isBle;
} BT_ACL_CLOSED_IND_T;


typedef struct BT_ACL_DETACH_CFM_T
{
    BT_GENERIC_STATUS_E status;
    U8 bdaddr[6];
} BT_ACL_DETACH_CFM_T;


typedef struct BT_SM_ADD_AUTH_DEVICE_CFM_T
{
    BT_GENERIC_STATUS_E status;
} BT_SM_ADD_AUTH_DEVICE_CFM_T;

typedef struct BT_SM_KEY_ID_T
{
    U16 oldKeyId;
    U16 newKeyId;
} BT_SM_KEY_ID_T;

typedef struct BT_SM_ADD_AUTH_DEVICE_LIST_CFM_T
{
    BT_GENERIC_STATUS_E status;
    U8 numOfList;
    BT_SM_KEY_ID_T keyIdList[1];
} BT_SM_ADD_AUTH_DEVICE_LIST_CFM_T;

typedef struct
{
    BDADDR_T bdaddr;
    U8 bonded : 1;
    U8 isBle : 1;
    U8 isEdr : 1;
    U8 reserved : 5;
    U8 link_key_size;
    U8 key_type;
    U8 link_key[16];
}BT_LINK_INFO_T;

typedef struct BT_SM_GET_AUTH_DEVICE_CFM_T
{
    BT_GENERIC_STATUS_E status;
    BT_LINK_INFO_T linkInfo;
} BT_SM_GET_AUTH_DEVICE_CFM_T;

typedef enum SEARCH_LINK_TYPE_E {
    SEARCH_TYPE_EDR = 0,
    SEARCH_TYPE_BLE,
    SEARCH_TYPE_ALL,
} SEARCH_LINK_TYPE_E;

typedef struct BT_SM_GET_AUTH_DEVICE_LIST_CFM_T
{
    BT_GENERIC_STATUS_E status;
    U8 num_of_devices;
    BDADDR_T bdaddr_list[1];
} BT_SM_GET_AUTH_DEVICE_LIST_CFM_T;

typedef struct BT_SM_GET_AUTH_DEVICE_LIST_EX_CFM_T
{
    BT_GENERIC_STATUS_E status;
    SEARCH_LINK_TYPE_E searchType;
    U8 numOfList;
    U16 length;
    U8 list[1];
} BT_SM_GET_AUTH_DEVICE_LIST_EX_CFM_T;

typedef struct BT_SM_DELETE_AUTH_DEVICE_CFM_T
{
    BT_GENERIC_STATUS_E status;
} BT_SM_DELETE_AUTH_DEVICE_CFM_T;

typedef struct BT_SM_DELETE_ALL_AUTH_DEVICE_CFM_T
{
    BT_GENERIC_STATUS_E status;
} BT_SM_DELETE_ALL_AUTH_DEVICE_CFM_T;

typedef struct BT_SM_PIN_CODE_IND_T
{
    U8 bd_addr[6];
} BT_SM_PIN_CODE_IND_T, BT_SM_USER_PASSKEY_REQ_IND_T;

typedef struct BT_SM_IO_CAPABILITY_REQ_IND_T
{
    U8 bd_addr[6];
    BOOL isBle;
} BT_SM_IO_CAPABILITY_REQ_IND_T;

typedef struct BT_SM_REMOTE_IO_CAPABILITY_IND_T
{
    U8 bd_addr[6];
    U8 iocap;
    U8 oob_data_present;
    U8 mitm;
    U8 bonding;
} BT_SM_REMOTE_IO_CAPABILITY_IND_T;



typedef struct BT_SM_USER_CONFIRMATION_REQ_IND_T
{
    U8 bd_addr[6];
    U32 passkey;
} BT_SM_USER_CONFIRMATION_REQ_IND_T, BT_SM_USER_PASSKEY_NOTIFICATION_IND_T;

typedef enum BT_SM_SECURITY_REQ_CFM_STATUS_E {
    BT_SECURITY_REQ_SUCCESS,
    BT_SECURITY_REQ_FAIL,
    BT_SECURITY_REQ_TIMEOUT,
    BT_PAIRING_IN_PROGRESS,
    BT_LINK_KEY_MISSING,
} BT_SM_SECURITY_REQ_CFM_STATUS_E;

typedef struct BT_SM_SECURITY_REQ_CFM_T
{
    U8 bd_addr[6];
    U8 status;
} BT_SM_SECURITY_REQ_CFM_T;

typedef struct BT_SM_AUTHENTICATION_IND_T
{
    U8 bd_addr[6];
    U8 status;
} BT_SM_AUTHENTICATION_IND_T;

typedef struct BT_SM_OOB_DATA_REQ_IND_T
{
    U8 bd_addr[6];
    U8 oobDataPresent;
    U8 r[16];
    U8 c[16];
} BT_SM_OOB_DATA_REQ_IND_T;

typedef struct BT_BLE_GEN_RESOLVABLE_PRIVATE_ADDR_RSP_T
{
    U8 status;
    U8 bd_addr[6];
} BT_BLE_GEN_RESOLVABLE_PRIVATE_ADDR_RSP_T;

typedef struct BT_SM_SECURITY_EVENT_IND_T
{
    BT_SM_SECURITY_EVENT_E event;
    U8 bd_addr[6];
} BT_SM_SECURITY_EVENT_IND_T;



typedef enum BT_PROTOCOL_E {
    BT_PROTOCOL_L2CAP,
    BT_PROTOCOL_RFCOMM,

} BT_PROTOCOL_E;

typedef struct BT_PROTOCOL_T
{
    BT_PROTOCOL_E protocol;
    union {
        U16 channel; /*! RFCOMM channel*/
        U16 psm;     /*! l2cap psm*/
    };
} BT_PROTOCOL_T;

typedef struct BT_READ_EIR_DATA_CFM_T
{
    U8 status;
    U8 fec_required;
    U8 eir_data[240];
} BT_READ_EIR_DATA_CFM_T;

typedef struct BT_REC_CLEAR_RECLIST_BDA_T
{
    U8 bdaddr[6];
} BT_REC_CLEAR_RECLIST_BDA_T;

typedef struct
{
    BOOL isDataValid;
    BOOL isLinkEncrypted;
    U8 bdaddr[6];
}BT_ACL_LINK_STATUS_T;


/**************************************************************************************************/
/*SM structures*/

#define BLE_SM_IO_CAP_DISPLAY_ONLY                      0x00
#define BLE_SM_IO_CAP_DISPLAY_YES_NO                    0x01
#define BLE_SM_IO_CAP_KEYBOARD_ONLY                     0x02
#define BLE_SM_IO_CAP_NO_INPUT_NO_OUTPUT                0x03
#define BLE_SM_IO_CAP_KEYBOARD_DISPLAY                  0x04
typedef U8 BLE_SM_IO_CAPABILITY_T;

#define BLE_SM_OOB_DATA_NOT_PRESENTED                   0x00
#define BLE_SM_OOB_DATA_PRESENTED                       0x01
typedef U8 BLE_SM_OOB_DATA_FLAG_T;

#define BLE_SM_AUTH_REQ_NO_BONDING                      0x00
#define BLE_SM_AUTH_REQ_BONDING                         (0x01 << 0)
#define BLE_SM_AUTH_REQ_MITM                            (0x01 << 2)
#define BLE_SM_AUTH_REQ_SECURE_CONNECTION               (0x01 << 3)
typedef U8 BLE_SM_AUTH_REQ_T;

#define BLE_SM_KEY_DISTRIBUTION_ENC_KEY                 (0x01 << 0)
#define BLE_SM_KEY_DISTRIBUTION_ID_KEY                  (0x01 << 1)
#define BLE_SM_KEY_DISTRIBUTION_SIGN_KEY                (0x01 << 2)
#define BLE_SM_KEY_DISTRIBUTION_LINK_KEY                (0x01 << 3)
typedef U8 BLE_SM_KEY_DISTRIBUTION_T;


#define ROLE_MASTER 0x00
#define ROLE_SLAVE  0x01
typedef U8 ROLE_T;

typedef enum BT_IO_CAPABILITY_E {
    IO_CAP_DISPLAY_ONLY = 0,
    IO_CAP_DISPLAY_YES_NO,
    IO_CAP_KEYBOARD_ONLY,
    IO_CAP_NOINPUT_OUTPUT,
    IO_CAP_KEYBOARD_DISPLAY,
} BT_IO_CAPABILITY_E;

typedef enum BT_KEY_PRESS_TYPE_E {
    KEY_START,
    KEY_ENTER,
    KEY_ERASE,
    KEY_CLEAR,
    KEY_COMPLETE
} BT_KEY_PRESS_TYPE_E;

typedef enum BT_OOB_DATA_TYPE_E {
    OOB_DATA_NOT_PRESERNT = 0,
    OOB_DATA_P192,
    OOB_DATA_P256,
    OOB_DATA_P192_P256,
} BT_OOB_DATA_TYPE_E;

typedef enum BT_SECURITY_MODE_E {
    SEC_MODE0_OFF,
    SEC_MODE1_NON_SECURE,
    SEC_MODE2_SERVICE,
    SEC_MODE3_LINK,
    SEC_MODE4_SSP,
} BT_SECURITY_MODE_E;

/*!
	@brief SSP security level
	Table 5.8: Security Level mapping to link key requirements
*/
typedef enum BT_SECURITY_MODE4_LEVEL_E {
    MODE4_LEVEL0, /*!< No security.*/
    MODE4_LEVEL1, /*!< No MITM pairing, encryption depends on peer's type.(No encryption for pre-v2.1 devices).*/
    MODE4_LEVEL2, /*!< No MITM pairing, encryption desired.*/
    MODE4_LEVEL3, /*!< MITM pairing, encryption desired. */
    MODE4_LEVEL4, /*!< MITM pairing(P256 based SSP), encryption desired.*/
} BT_SECURITY_MODE4_LEVEL_E;

/** \brief  Legacy mode 2 attributes */
typedef struct BT_SECURITY_MODE2_ATTRIBUTE_T
{
    U8 MODE2_AHTHORIZATION;  /*!< Authorization required*/
    U8 MODE2_AHTHENTICATION; /*!< Authentication required*/
    U8 MODE2_ENCRYPTION;     /*!< Encryption required*/
} BT_SECURITY_MODE2_ATTRIBUTE_T;

/** \brief  Legacy mode 3 encrypted setting */
typedef U8 BT_SECURITY_MODE3_ENCRYPTION_T;

typedef struct BT_SECURITY_CONFIG_T
{
    BT_SECURITY_MODE_E mode;
    BT_SECURITY_MODE3_ENCRYPTION_T mode3_setting;
} BT_SECURITY_CONFIG_T;

typedef struct BT_SECURITY_SERVICE_CONFIG_T
{
    BT_PROTOCOL_T protocol;
    BT_SECURITY_MODE2_ATTRIBUTE_T mode2_attr;
    BT_SECURITY_MODE4_LEVEL_E mode4_level;
    BOOL IsIncomming; /*! Incoming or outgoing*/
} BT_SECURITY_SERVICE_CONFIG_T;

/** \brief Confirmation message of \ref BtWriteClassOfDevice*/
typedef struct BT_READ_CLASS_OF_DEVICE_CFM_T
{
    /** The HCI status. */
    U8 hci_status;
    /** The cod. */
    U32 COD;
} BT_READ_CLASS_OF_DEVICE_CFM_T;

typedef struct BT_SM_KEYPRESS_NOTIFICATION_IND_T
{
    U8 bd_addr[6];
    BT_KEY_PRESS_TYPE_E type;
} BT_SM_KEYPRESS_NOTIFICATION_IND_T;

typedef enum BLE_SECURITY_MODE_E {
    SEC_MODE1,
    SEC_MODE2,
} BLE_SECURITY_MODE_E;

typedef enum BLE_SECURITY_MODE1_LEVEL_E {
    SEC_MODE1_LEVEL1, /*!< No security (No authentication and no encryption).*/
    SEC_MODE1_LEVEL2, /*!< Unauthenticated pairing with encryption.*/
    SEC_MODE1_LEVEL3, /*!< Authenticated pairing with encryption. */
    SEC_MODE1_LEVEL4, /*!< Authenticated LE Secure Connections pairing with encryption using a 128-bit strength encryption key.*/
} BLE_SECURITY_MODE1_LEVEL_E;

typedef enum BLE_SECURITY_MODE2_LEVEL_E {
    SEC_MODE2_NONE,   /*!< None.*/
    SEC_MODE2_LEVEL1, /*!< Unauthenticated pairing with data signing.*/
    SEC_MODE2_LEVEL2, /*!< Authenticated pairing with data signing.*/
} BLE_SECURITY_MODE2_LEVEL_E;

typedef struct BLE_SECURITY_SERVICE_CONFIG_T
{
    BLE_SECURITY_MODE_E mode;
    U8 level;
} BLE_SECURITY_SERVICE_CONFIG_T;

typedef struct BLE_SM_IO_CAPABILITY_REQUEST_RSP_T
{
    BLE_SM_IO_CAPABILITY_T io_capability;
    BLE_SM_OOB_DATA_FLAG_T oob_data;
    BLE_SM_AUTH_REQ_T auth_req;
    BLE_SM_KEY_DISTRIBUTION_T key_distribution;
} BLE_SM_IO_CAPABILITY_REQUEST_RSP_T;

/**************************************************************************************************/
/*Link Policy*/
#define DISABLE_ALL_LM_MODES	0x0000
#define ENABLE_ROLE_SWITCH		0x0001
#define ENABLE_HOLD_MODE		0x0002
#define ENABLE_SNIFF_MODE		0x0004
#define ENABLE_PARK_STATE		0x0008


/**************************************************************************************************/
/*BLE structures*/
#define BLE_MAX_DATA_SIZE 31

typedef enum {
    BLE_ADV_IND = 0,
    BLE_ADV_DIRECT_IND = 1,
    BLE_ADV_SCAN_IND = 2,
    BLE_ADV_NON_CONN_IND =3,
} BLE_ADV_TYPE_E;

typedef enum {
    PUBLIC_DEVICE_ADDR,
    RANDOM_DEVICE_ADDR,
    RESOLVABLE_PRIVATE_ADDR_OR_PUBLIC_ADDR,
    RESOLVABLE_PRIVATE_ADDR_OR_RANDOM_ADDR,
} BLE_OWN_ADDR_TYPE_E;

typedef enum {
    BLE_FILTER_NONE,         /*! Allow scan and connect request from any */
    BLE_FILTER_SCAN_ONLY,    /*! Allow scan request from white list only, allow any connect */
    BLE_FILTER_CONNECT_ONLY, /*! Allow any scan, Allow connect from white list only */
    BLE_FILTER_BOTH,         /*! Allow scan and connect request from white list only */
} BLE_ADV_FILTER_POLICY_E;

typedef enum {
    BLE_ADDR_WRITE_STATIC,
    BLE_ADDR_GENERATE_STATIC,
    BLE_ADDR_GENERATE_NON_RESOLVABLE,
    BLE_ADDR_GENERATE_RESOLVABLE,
} BLE_RANDOM_ADDR_TYPE_E;

typedef U8 BLE_ADDR_TYPE;

enum BLE_ADDR_TYPE_E{
    BLE_ADDR_TYPE_PUBLIC_DEVICE_ADDR = (BLE_ADDR_TYPE)0x00,
    BLE_ADDR_TYPE_RANDOM_DEVICE_ADDR = (BLE_ADDR_TYPE)0x01,
};

typedef enum {
    BLE_PASSIVE_SCANNING, /*! No SCAN_REQ packets shall be sent.(default) */
    BLE_ACTIVE_SCANNING,  /*! SCAN_REQ packets may be sent */
} BLE_SCAN_TYPE_E;

typedef enum {
    BLE_SCAN_FILTER_POLICY_0, /*! accept all
                                                             advertisement packets except directed advertising packets notaddressed to this device (default). */
    BLE_SCAN_FILTER_POLICY_1, /*! accept only
                                                             advertisement packets from devices where the advertiser!|s address is in the White list.
                                                             Directed advertising packets which are not addressed for this device shall be ignored. */
    BLE_SCAN_FILTER_POLICY_2, /*! accept all 
                                                             undirected advertisement packets, and
                                                             directed advertising packets where the initiator address is a resolvable private address, and
                                                             directed advertising packets addressed to this device. */
    BLE_SCAN_FILTER_POLICY_3, /*! accept all
                                                             advertisement packets from devices where the advertiser!|s address is in the White list, and
                                                             directed advertising packets where the initiator address is a resolvable private address, and
                                                             directed advertising packets addressed to this device */
} BLE_SCAN_FILTER_POLICY_E;

typedef enum {
    BLE_REPORT_TYPE_IND,         /*! Connectable undirected advertising (ADV_IND). */
    BLE_REPORT_TYPE_DIRECT_IND,  /*! Connectable directed advertising (ADV_DIRECT_IND) */
    BLE_REPORT_TYPE_SCAN_IND,    /*! Scannable undirected advertising (ADV_SCAN_IND) */
    BLE_REPORT_TYPE_NONCONN_IND, /*! Non connectable undirected advertising (ADV_NONCONN_IND) */
    BLE_REPORT_TYPE_SCAN_RSP,    /*! Scan Response (SCAN_RSP) */
} BLE_REPORT_TYPE_E;

typedef enum {
    BLE_ADV_PRIMARY,
    BLE_ADV_GFP,
    BLE_ADV_AMA,
	BLE_ADV_XIAOAI,
	BLE_ADV_TILE,
    BLE_ADV_SWIFT,
    BLE_ADV_HANDLE_NUM, /* Need to check NUM_OF_SUPP_LE_ADV_SETS in Config.h with the owner of controller */
} BLE_ADV_HANDLE_E;

typedef struct
{
    BLE_SCAN_TYPE_E scanType;
    U16 scanInterval;
    U16 scanWindow;
    BLE_OWN_ADDR_TYPE_E ownAddrType;
    BLE_SCAN_FILTER_POLICY_E scanFilterPolicy;
} BLE_SCAN_PARAMS_T;

typedef struct
{
    U32 advIntervalMin;
    U32 advIntervalMax;
    BLE_ADV_FILTER_POLICY_E filterPolicy;
} BLE_UNDIRECTED_ADV_PARAMS_T;

typedef struct
{
    BOOL randomDirectAddr;
    BD_ADDR_T directBdAddr;
} BLE_DIRECTED_ADV_PARAMS_T;

typedef union BLE_ADV_PARAMS_T {
    BLE_UNDIRECTED_ADV_PARAMS_T undirectedAdvParams;
    BLE_DIRECTED_ADV_PARAMS_T directedAdvParams;
} BLE_ADV_PARAMS_T;

typedef enum {
    INITIATOR_FILTER_POLICY_WHITE_LIST_NOT_USED, // Peer_Address_Type and Peer_Address shall be used
    INITIATOR_FILTER_POLICY_WHITE_LIST_USED,     // Peer_Address_Type and Peer_Address shall be ignored
} BLE_INITIATOR_FILTER_POLICY_E;

typedef enum {
    PEER_PUBLIC_DEVICE_ADDR,
    PEER_RANDOM_DEVICE_ADDR,
    PEER_PUBLIC_IDENTITY_ADDR,
    PEER_RANDOM_IDENTITY_ADDR,
} BLE_PEER_ADDR_TYPE_E;

typedef struct
{
    BLE_ADDR_TYPE addrType;
    BD_ADDR_T bdAddr;
    U16 connIntervalMin;    /* unit:1.25 ms, 7.5 ms - 4000 ms*/
    U16 connIntervalMax;    /* unit:1.25ms, 7.5 ms - 4000 ms*/
    U16 connLatency;
    U16 supervisonTimeout;  /* unit:10 ms, 100 ms - 32000 ms*/
} BLE_CONN_PARAMS_UPDATE_T;

typedef struct
{
    BLE_ADDR_TYPE addrType;
    BD_ADDR_T bdAddr;
    U8 id;
    U16 connIntervalMin;
    U16 connIntervalMax;
    U16 connLatency;
    U16 supervisonTimeout;
} BLE_CONN_PARAMS_UPDATE_IND_T;

typedef struct
{
    U16 scanInterval;
    U16 scanWindow;
    BLE_INITIATOR_FILTER_POLICY_E policy;
    BLE_PEER_ADDR_TYPE_E peerAddrType;
    BD_ADDR_T peerBdAddr;
    BLE_OWN_ADDR_TYPE_E ownAddrType;
    U16 connIntervalMin;
    U16 connIntervalMax;
    U16 connLatency;
    U16 supervisionTimeout;
    U16 minCeLen;
    U16 maxCeLen;
} BLE_CREATE_CONN_PARAMS_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
} BT_BLE_SET_SCAN_ENABLE_CFM_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
} BT_BLE_SET_SCAN_PARAMS_CFM_T;

typedef struct
{
    BLE_REPORT_TYPE_E reportType;
    BLE_OWN_ADDR_TYPE_E addrType;
    BD_ADDR_T bdAddr;
    U8 lenData;
    U8 data[BLE_MAX_DATA_SIZE];
    U8 rssi;
} BT_BLE_ADV_REPORT_IND_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
} BT_BLE_SET_SCAN_RESPONSE_DATA_CFM_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
} BT_BLE_SET_ADV_DATA_CFM_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
} BT_BLE_SET_ADV_ENABLE_CFM_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
} BT_BLE_SET_ADV_PARAMS_CFM_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
} BT_BLE_SET_ADV_PARAMS_EX_CFM_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
} BT_BLE_SET_LOCAL_RANDOM_ADDR_CFM_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
    U8 size;
} BT_BLE_READ_WHITE_LIST_SIZE_CFM_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
} BT_BLE_CLEAR_WHITE_LIST_CFM_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
} BT_BLE_ADD_DEVICE_TO_WHITE_LIST_CFM_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
} BT_BLE_REMOVE_DEVICE_FROM_WHITE_LIST_CFM_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
    BLE_ADDR_TYPE addrType;
    BD_ADDR_T bdAddr;
    U16 connInterval;       /* unit:1.25ms */
    U16 connLatency;
    U16 supervisionTimeout; /* 10ms */
} BT_BLE_CONNECTION_PARAMS_UPDATE_CFM_T;

typedef struct
{
    BLE_CONN_PARAMS_UPDATE_IND_T params;

} BT_BLE_CONNECTION_PARAMS_UPDATE_IND_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
    U8 index;
} BT_BLE_CREATE_CONNECTION_CFM_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
} BT_BLE_CREATE_CONNECTION_CANCEL_CFM_T;

typedef struct
{
    U8 bdAddr[6]; /*! The Bluetooth address.*/
    U8 reason;
    U8 status;
} BT_BLE_DISCONNECT_REQ_RSP_T;

typedef struct BT_BLE_SM_SET_SERVICE_CONFIG_CFM_T
{
    BT_GENERIC_STATUS_E status;
} BT_BLE_SM_SET_SERVICE_CONFIG_CFM_T;

typedef struct BT_BLE_LINK_INFO_T
{
    U8 bd_addr[6];
    U8 bd_addrType;
} PACKED BT_BLE_LINK_INFO_T;

typedef struct BT_BLE_GET_LINK_STATUS_IND_T
{
    U8 status;
    U8 actived_link_num;
    U8 link[0];
} BT_BLE_GET_LINK_STATUS_IND_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
} BT_BLE_SET_ADV_ENABLE_ALL_CFM_T;

typedef struct
{
    U8 disableAdvMask;
    BT_GENERIC_STATUS_E status;
} BT_BLE_SET_ADV_DISABLE_ALL_CFM_T;

typedef struct BT_GAP_DUTMODE_CFM_T
{
    U8 status;

} BT_GAP_DUTMODE_CFM_T, BT_GAP_CANCEL_CONN_CFM_T;


typedef struct BLE_EXT_ADV_DATA_T
{
    U8 *advData;
    U8 sizeAdvData;
} BLE_EXT_ADV_DATA_T;

typedef struct BLE_EXT_SCAN_RSP_DATA_T
{
    U8 *scanRspData;
    U8 sizeRspData;
} BLE_EXT_SCAN_RSP_DATA_T;



typedef struct BLE_EXT_ADV_PARAM_T
{
    BLE_ADV_TYPE_E advType;
    BLE_OWN_ADDR_TYPE_E ownAddrType;
    BLE_ADV_PARAMS_T AdvParms;
    U8 channelMap;
} BLE_EXT_ADV_PARAM_T;

typedef struct BT_BLE_ADV_DATA_T
{
    BLE_RANDOM_ADDR_TYPE_E randomAddrType;
    BD_ADDR_T RandomAddr;
} BLE_EXT_RANDOM_ADDR_T;

typedef struct BLE_EXT_ADV_PARAM_EX_T
{
    U8 setAdvParam:1;
    U8 setAdvData:1;
    U8 setScanRspData:1;
    U8 setRandomAddr:1;

    /* ADV param */
    BLE_EXT_ADV_PARAM_T extAdvParam;
    /* ADV data */
    U8 *advData;
    U8 sizeAdvData;
    /* Scan rsp data */
    U8 *scanRspData;
    U8 sizeRspData;
    /* Random addr */
    BLE_EXT_RANDOM_ADDR_T extRadomAddr;
} BLE_EXT_ADV_PARAM_EX_T;


/*!
    @}
*/
/**************************************************************************************************/
/*                            Functions                                                           */
/**************************************************************************************************/
/*!
    @addtogroup BT_LIB_GAP_FN
    @{
*/
/*inquiry*/
void BtWriteEirData(Handler handler, U8 fec_enabled, U8 eir_len, U8 *eir);
void BtReadEirData(Handler handler);
void BtInquiry(Handler handler, U32 lap, U8 timeout, U8 max_num);
void BtInquiryPeriodic(Handler theAppHandler, U16 min_period, U16 max_period, U32 lap, U8 timeout, U8 max_num);
void BtInquiryCancel(Handler handler);
void BtWriteInquiryTxPower(Handler handler, S8 tx_power);
void BtReadInquiryTxPower(Handler handler);
void BtReadInquiryMode(Handler handler);
void BtWriteInquiryMode(Handler handler, INQUARY_MODE_E mode);
void BtReadRemoteName(Handler handler, U8 bdaddr[6]);
void BtReadRemoteNameCancel(Handler handler, U8 bdaddr[6]);
void BtGetEDRLinkStatus(Handler handler);

/*baseband*/
void BtWriteInquiryScanType(Handler handler, SCAN_TYPE_T type);
void BtWritePageScanType(Handler handler, SCAN_TYPE_T type);
void BtReadScanEnable(Handler app);
void BtWriteScanEnable(Handler handler, SCAN_ENABLE_T mode);
void BtWriteInquiryAccessCode(Handler handler, const U32 *iac, U8 num_iac);
void BtReadClassOfDevice(Handler handler);
void BtWriteLocalBdaddr(Handler handler, U8 bdaddr[6]);

/*!
	@brief Write class of device
	@param handler The message handler to received complete message BT_WRITE_CLASS_OF_DEVICE_CFM.
	@param COD Class of device.
*/

void BtWriteClassOfDevice(Handler handler, U32 COD);
void BtWriteInquiryScanActivity(Handler handler, U16 interval, U16 window);
void BtWritePagescanActivity(Handler handler, U16 interval, U16 window);
void BtWriteFlushTimeout(Handler handler, SINK sink, U16 flush_timeout);
void BtWriteLocalName(Handler handler, U16 size_local_name, U8 *local_name);
void BtReadLocalName(Handler handler);
void BtWritePageTimeout(Handler handler, U16 timeout);

/*security*/
void BtSmAuthenticateRequest(Handler handler, U8 bdaddr[6]);
void BtSmSetDeviceSecurityConfig(Handler handler, BT_SECURITY_CONFIG_T sec);
void BtSmSetServiceSecurityConfig(Handler handler, BT_SECURITY_SERVICE_CONFIG_T sec);
void BtSmUnsetServiceSecurityConfig(Handler handler, BT_SECURITY_SERVICE_CONFIG_T sec);
void BtSmPinCodeResponse(U8 bd_addr[6], U8 len, U8 *pin_code);
void BtSmGetAuthDeviceList(Handler handler, U16 max_num, SEARCH_LINK_TYPE_E type);
void BtSmGetAuthDevice(Handler handler, BDADDR_T bdaddr);
void BtSmAddAuthDevice(Handler handler, BDADDR_T addr, U8 link_key_size, U8 key_type, U8 *link_key);
void BtSmDeleteAuthDevice(Handler handler, BDADDR_T bdadddr);
void BtSmDeleteAllAuthDevice(Handler handler);
void BtSmIoCapabilityResponse(U8 bdaddr[6], BT_IO_CAPABILITY_E io_capability, BOOL mitm, BOOL bonding, BT_OOB_DATA_TYPE_E oob_data_present, U8 keyDistribution);
void BtSmUserConfirmationResponse(U8 bdaddr[6], BOOL confirmed);
void BtSmPasskeyResponse(U8 bdaddr[6], BOOL isOk, U32 number);
void BtSmPasskeyNotificationResponse(U8 bdaddr[6], U32 number);
void BtSmSendKeyPressNotify(U8 bdaddr[6], BT_KEY_PRESS_TYPE_E type);
void BtSmSaveLinkKeyRecord(BOOL enable);
void BtSmAddAuthDeviceList(Handler app, U16 length, void *link_key_list);
void BtSmGetAuthDeviceListEx(Handler app, SEARCH_LINK_TYPE_E searchType);

/*information*/
void BtReadRemoteVersion(Handler handler, SINK sink);
void BtReadRemoteSuppFeature(Handler handler, SINK sink);
void BtReadLocalAddr(Handler handler);
void BtReadLinkQuality(Handler handler, SINK sink);
void BtReadRssi(Handler app, U8 (*bdAddr)[6]);
void BtReadClockOffset(Handler handler, SINK sink);
void BtReadLocalVersion(Handler handler);
void BtGetCurrentClock(Handler app, U8 (*bdAddr)[6], CLOCK_E clock_type);
void BtReadAfhChannelMap(Handler app, U8 (*bdAddr)[6]);


/*policy*/
void BtSwitchRole(Handler handler, U8 (*bdAddr)[6], ROLE_T role);
void BtGetRole(Handler handler, U8 (*bdAddr)[6]);
void BtSetSupperTimeout(Handler handler, U8 (*bdAddr)[6], U16 timeout);
void BtSetLinkMode(Handler handler,U8 (*bdAddr)[6], LINK_MODE_E mode, LINK_MODE_PARAM_T *parm);
void BtSetSniffSubrating(Handler handler, U8 (*bdAddr)[6], U16 latency, U16 remote_timeout, U16 local_timeout);
void BtQosSetup(Handler handler,U8 (*bdAddr)[6],U8 flags,U8 ServiceType ,U32 TokenRate,U32 PeakBandwidth,U32 Latency,U32 DelayVariation);
void BtSetLinkPolicy(Handler handler, U8 (*bdAddr)[6], U16 settings);
void BtSyncSetLowLatencyModeEnable(Handler handler, U8 (*bdAddr)[6], BOOL enable);


/*BLE*/
void BleSetScanEnableReq(Handler handler, BOOL enable);
void BleSetScanParamsReq(Handler handler, BLE_SCAN_PARAMS_T *pParams);
void BleSetScanResponseDataReq(Handler handler, U8 sizeSrData, const U8 *srData);
void BleSetAdvDataReq(Handler handler, U8 sizeAdvData, const U8 *advData);
void BleSetAdvEnableReq(Handler handler, BOOL enable);
void BleSetAdvParamsReq(Handler handler, BLE_ADV_TYPE_E advType, BLE_OWN_ADDR_TYPE_E ownAddrType, U8 channelMap, const BLE_ADV_PARAMS_T *pAdvParms);
void BleSetLocalRandomAddrReq(Handler handler, BLE_RANDOM_ADDR_TYPE_E randomAddrType, BD_ADDR_T *pRandomddr);
void BleReadWhiteListSizeReq(VOID);
void BleClearWhiteListReq(VOID);
void BleAddDeviceToWhiteListReq(BLE_ADDR_TYPE addrType, BD_ADDR_T *pAddr);
void BleRemoveDeviceFromWhiteListReq(BLE_ADDR_TYPE addrType, BD_ADDR_T *pAddr);
void BleConnectionParamsUpdateReq(Handler handler, BLE_CONN_PARAMS_UPDATE_T *pParams);
void BleConnectionParamsUpdateRsp(BOOL rsp, BLE_CONN_PARAMS_UPDATE_IND_T *pParams);
void BleCreateConnectionReq(Handler handler, BLE_CREATE_CONN_PARAMS_T *pParams);
void BleDisconnecReq(Handler handler, U8 bdAddr[6], U8 reason);
void BleGetLinkStatus(Handler handler);
void BleSetLinkInfoNotifyHandler(Handler handler);
void BleSetExtendedLocalRandomAddrReq(Handler handler,BLE_ADV_HANDLE_E handle, BLE_EXT_RANDOM_ADDR_T* randomAddr);
void BleSetExtendedAdvParamsReq(Handler handler,BLE_ADV_HANDLE_E handle, BLE_EXT_ADV_PARAM_T* advParams);
void BleSetExtendedAdvParamsExReq(Handler handler,BLE_ADV_HANDLE_E handle, BLE_EXT_ADV_PARAM_EX_T* advParams);
void BleSetExtendedAdvDataReq(Handler handler,BLE_ADV_HANDLE_E handle, BLE_EXT_ADV_DATA_T* advData);
void BleSetExtendedAdvEnableReq(Handler handler,BLE_ADV_HANDLE_E handle, BOOL enable);
void BleSetExtendedScanResponseDataReq(Handler handler,BLE_ADV_HANDLE_E handle , BLE_EXT_SCAN_RSP_DATA_T* scanRspData);
void BleEnableAdvAllReq(Handler handler, U8 mask);
void BleDisableAdvAllReq(Handler handler);
U32 BleGetEnableAdvMask(void);
void BleGenResolvablePrivateAddrReq(Handler handler);



/* SM */
void BtSmSecurityRequest(U8 bdAddr[6], BOOL mitm, BOOL bonding, BOOL keyPress);
void BtSmOobDataReqResponse(U8 bdAddr[6], BOOL oobDataPresent, U8 *r, U8 *c);
void BleSmSetServiceSecurityConfig(Handler handler, BLE_SECURITY_SERVICE_CONFIG_T sec);
void BleSmIoCapabilityResponse(U8 bdaddr[6], BLE_SM_IO_CAPABILITY_REQUEST_RSP_T *rsp);

/*Synchronous*/
void BtSyncRegister(Handler app);
void BtSyncUnregister(Handler app);
void BtSyncConnectRequest(Handler app, SINK sink, BT_SYNC_CONFIG_PARAM_T *config_params);
void BtSyncConnectResponse(Handler app, U8 bdaddr[6], BOOL accept, U8 link_type, U8 reason, BT_SYNC_CONFIG_PARAM_T *config_params);
void BtSyncDisconnectRequest(Handler app, SINK sink, U8 reason);
void BtSyncSetRetransmitssionEnable(Handler handler, U8 (*bdAddr)[6], BOOL enable);


/* Test mode */
void BtEnableDutMode(Handler app);



/*Common Operation*/

//VOID BtBDaddrCpy(BD_ADDR_T* pDst, BD_ADDR_T* pSrc);
//U8 BtIsSameBDaddr(BD_ADDR_T* pDst, BD_ADDR_T* pSrc);

/* Link control*/
void BtSetMaximumEDRLinkNum(U8 num);
void BtCancelCreateConnection(Handler app, U8 bdaddr[6]);
/* Set acl detach timer when there is no l2cap channel connected. (unit: ms)*/
void BtSetAclDetachTimeout(U8 bdaddr[6], U16 timeout);

/* Detach ACL link, return BT_ACL_DETACH_CFM */
void BtDetchAclLink(Handler app, U8 bdaddr[6]);


void BtGetEdrAclLinkStatus(BT_ACL_LINK_STATUS_T* link_status);


void BtSetGapSystemStatus(BT_GAP_SYSTEM_STATUS_E status);
BOOL BtCheckDetachTimerExist(U8 Bdaddr[6]);

/* others */

BT_GAP_GET_ROLE_STATUS_E BtGetLinkRole(U8 bdaddr[6]);

/*!
    @}
*/


BOOL BtGapRhoLock(void);
BOOL BtGapIsRhoAllow(void);
U16 BtGapGetRhoDataLength(void);
BOOL BtGapGetRhoData(U8 *pBuffer);
BOOL BtGapApplyAgentRhoData(BD_ADDR_T *pBdAddr, U8 *pGapRhoData, U16 length);
BOOL BtGapApplyPartnerRhoData(BD_ADDR_T *pBdAddr, U8 *pGapRhoData, U16 length);




#endif
=======
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

#ifndef __BT_GAP_HH
#define __BT_GAP_HH

#include "bt_common.h"
#include <sink_.h>

/**************************************************************************************************/
/*                            Macros                                                              */
/**************************************************************************************************/
/*!
    @addtogroup BT_LIB_GAP_MACRO
    @{
*/
/*!
@name GAP AD Data Type [Defines]
GAP Advertisement Type
@{
*/
#define EIR_TYPE_FLAGS 0x01                              ///< Flags for discover ability.
#define EIR_TYPE_16BIT_SERVICE_UUID_MORE_AVAILABLE 0x02  ///< Partial list of 16 bit service UUIDs.
#define EIR_TYPE_16BIT_SERVICE_UUID_COMPLETE 0x03        ///< Complete list of 16 bit service UUIDs.
#define EIR_TYPE_32BIT_SERVICE_UUID_MORE_AVAILABLE 0x04  ///< Partial list of 32 bit service UUIDs.
#define EIR_TYPE_32BIT_SERVICE_UUID_COMPLETE 0x05        ///< Complete list of 32 bit service UUIDs.
#define EIR_TYPE_128BIT_SERVICE_UUID_MORE_AVAILABLE 0x06 ///< Partial list of 128 bit service UUIDs.
#define EIR_TYPE_128BIT_SERVICE_UUID_COMPLETE 0x07       ///< Complete list of 128 bit service UUIDs.
#define EIR_TYPE_SHORT_LOCAL_NAME 0x08                   ///< Short local device name.
#define EIR_TYPE_COMPLETE_LOCAL_NAME 0x09                ///< Complete local device name.
#define EIR_TYPE_TX_POWER_LEVEL 0x0A                     ///< Transmit power level.
#define EIR_TYPE_CLASS_OF_DEVICE 0x0D                    ///< Class of device.
#define EIR_TYPE_SIMPLE_PAIRING_HASH_C 0x0E              ///< Simple Pairing Hash C.
#define EIR_TYPE_SIMPLE_PAIRING_RANDOMIZER_R 0x0F        ///< Simple Pairing Randomizer R.
#define EIR_TYPE_SECURITY_MANAGER_TK_VALUE 0x10          ///< Security Manager TK Value.
#define EIR_TYPE_SECURITY_MANAGER_OOB_FLAGS 0x11         ///< Security Manager Out Of Band Flags.
#define EIR_TYPE_SLAVE_CONNECTION_INTERVAL_RANGE 0x12    ///< Slave Connection Interval Range.
#define EIR_TYPE_SOLICITED_SERVICE_UUIDS_16BIT 0x14      ///< List of 16-bit Service Solicitation UUIDs.
#define EIR_TYPE_SOLICITED_SERVICE_UUIDS_128BIT 0x15     ///< List of 128-bit Service Solicitation UUIDs.
#define EIR_TYPE_SERVICE_DATA 0x16                       ///< Service Data - 16-bit UUID.
#define EIR_TYPE_PUBLIC_TARGET_ADDRESS 0x17              ///< Public Target Address.
#define EIR_TYPE_RANDOM_TARGET_ADDRESS 0x18              ///< Random Target Address.
#define EIR_TYPE_APPEARANCE 0x19                         ///< Appearance.
#define EIR_TYPE_ADVERTISING_INTERVAL 0x1A               ///< Advertising Interval.
#define EIR_TYPE_LE_BLUETOOTH_DEVICE_ADDRESS 0x1B        ///< LE Bluetooth Device Address.
#define EIR_TYPE_LE_ROLE 0x1C                            ///< LE Role.
#define EIR_TYPE_SIMPLE_PAIRING_HASH_C256 0x1D           ///< Simple Pairing Hash C-256.
#define EIR_TYPE_SIMPLE_PAIRING_RANDOMIZER_R256 0x1E     ///< Simple Pairing Randomizer R-256.
#define EIR_TYPE_SERVICE_DATA_32BIT_UUID 0x20            ///< Service Data - 32-bit UUID.
#define EIR_TYPE_SERVICE_DATA_128BIT_UUID 0x21           ///< Service Data - 128-bit UUID.
#define EIR_TYPE_UNIFORM_RESOUCE_IDENTIFIER 0x24         ///< Uniform Resource Identifier
#define EIR_TYPE_3D_INFORMATION_DATA 0x3D                ///< 3D Information Data.
#define EIR_TYPE_MANUFACTURER_SPECIFIC_DATA 0xFF         ///< Manufacturer Specific Data.
/*!
@}
*/
#define BT_GAP_LE_AD_FLAG_LIMITED_DISCOVERABLE     (0x01 << 0) /**< LE limited discoverable mode. */
#define BT_GAP_LE_AD_FLAG_GENERAL_DISCOVERABLE     (0x01 << 1) /**< LE general discoverable mode. */
#define BT_GAP_LE_AD_FLAG_BR_EDR_NOT_SUPPORTED     (0x01 << 2) /**< BR/EDR not supported. */
/*!
    @}
*/
/**************************************************************************************************/
/*                            Data Structures                                                     */
/**************************************************************************************************/
/*!
    @addtogroup BT_LIB_GAP_DS
    @{
*/
/*!
	@brief HCI error code.
*/

#ifdef WIN32
#pragma pack(push, 1)
#endif

/*!
    @brief Leagcy BT mac address
*/
typedef struct BD_ADDR_T
{
    U8 addr[6];
}BD_ADDR_T;

/*!
    @brief BT mac address with type
*/
typedef struct BDADDR_T
{
    U8 type;
    U8 addr[6];
}BDADDR_T;

#ifdef WIN32
#pragma pack(pop)
#endif

typedef enum PAGE_SCAN_REP_MODE_T {
    PAGE_SCAN_REP_MODE_R0,
    PAGE_SCAN_REP_MODE_R1,
    PAGE_SCAN_REP_MODE_R2,
    PAGE_SCAN_REP_MODE_UNKNOWN
} PAGE_SCAN_REP_MODE_E;

typedef enum SCAN_TYPE_T {
    SCAN_TYPE_STANDARD,
    SCAN_TYPE_INTERLANCE
} SCAN_TYPE_T;

typedef enum SCAN_ENABLE_T {
    SCAN_ENABLE_OFF,
    SCAN_ENABLE_INQ,
    SCAN_ENABLE_PAGE,
    SCAN_ENABLE_INQ_AND_PAGE
} SCAN_ENABLE_T;

typedef enum INQUARY_MODE_E {
    INQUARY_MODE_STARNDAR,
    INQUARY_MODE_RSSI,
    INQUARY_MODE_RSSI_EIR,
} INQUARY_MODE_E;

typedef enum LINK_KEY_TYPE_E {
    LINK_KEY_COMBINATION = 0,
    LINK_KEY_LOCAL_UNIT,
    LINK_KEY_REMOTE_UNIT,
    LINK_KEY_DEBUG_COMBINATION,
    LINK_KEY_UNAUTHENTICATED,
    LINK_KEY_AUTHENTICATED,
    LINK_KEY_CHANGED_COMBINATION,
    LINK_KEY_UNAUTHENTICATED_P256,
    LINK_KEY_AUTHENTICATED_P256,
} LINK_KEY_TYPE_E;

typedef enum LINK_MODE_E {
    LINK_MODE_ACTIVE,
    LINK_MODE_HOLD,
    LINK_MODE_SNIFF,
    LINK_MODE_PARK,
} LINK_MODE_E;

typedef enum ROLE_E {
    ROLE_MASTER,
    ROLE_SLAVE,
} ROLE_E;


typedef enum CLOCK_E {
    LOCAL_CLOCK,
    PICONET_CLOCK,
} CLOCK_E;


typedef enum BT_SM_SECURITY_EVENT_E {
    BT_SM_ENCRYPTION_ON,
    BT_SM_LINK_KEY_MISSING,
} BT_SM_SECURITY_EVENT_E;

typedef enum
{
    BT_DISCONNECT_REASON_AUTHENTICATION_FAILURE = 0x05,
    BT_DISCONNECT_REASON_REMOTE_USER_TERMINATED_CONNECTION = 0x13,
    BT_DISCONNECT_REASON_REMOTE_TERMINATED_CONNECTION_DUE_TO_LOW_RESOURCES = 0x14,
    BT_DISCONNECT_REASON_REMOTE_TERMINATED_CONNECTION_DUE_TO_POWER_OFF = 0x15,
    BT_DISCONNECT_REASON_UNSUPPORTED_REMOTE_FEATURE = 0x1A,
    BT_DISCONNECT_REASON_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED = 0x29,
    BT_DISCONNECT_REASON_UNACCEPTABLE_CONNECTION_PARAMETERS = 0x3B,
} BT_DISCONNECT_REASON_E;

typedef struct LINK_MODE_HOLD_PARAM_T
{
    U16 beacon_max_interval;
    U16 beacon_min_interval;
} LINK_MODE_HOLD_PARAM_T, LINK_MODE_PARK_PARAM_T;


typedef struct LINK_MODE_SNIFF_PARAM_T
{
    U16 sniff_max_interval;
    U16 sniff_min_interval;
    U16 sniff_attempt;
    U16 sniff_timeout;
} LINK_MODE_SNIFF_PARAM_T;

typedef struct LINK_MODE_PARAM_T
{
    union {
        LINK_MODE_HOLD_PARAM_T hold_parm;
        LINK_MODE_SNIFF_PARAM_T sniff_parm;
        LINK_MODE_PARK_PARAM_T park_parm;
    };
} LINK_MODE_PARAM_T;


typedef enum BT_GAP_SYSTEM_STATUS_E {
    BT_GAP_SYSTEM_STATUS_NONE,
    BT_GAP_SYSTEM_STATUS_READY_TO_POWER_OFF,
} BT_GAP_SYSTEM_STATUS_E;

typedef enum BT_GAP_GET_ROLE_STATUS_E {
    BT_GAP_LINK_NOT_EXIST,
    BT_GAP_LINK_ROLE_SLAVE,
    BT_GAP_LINK_ROLE_MASTER,
} BT_GAP_GET_ROLE_STATUS_E;

typedef enum {
    BT_GAP_LC_MODE_OTA = 2,
} BT_GAP_LC_MODE_E;

typedef struct BT_SYNC_CONFIG_PARAM_T
{
    U32 tx_bandwidth;
    U32 rx_bandwidth;
    U16 max_latency;
    U16 voice_settings;
    U8 retx_effort;
    U16 packet_type;
} BT_SYNC_CONFIG_PARAM_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
	U8 bdaddr[6];
} BT_INIT_CFM_T, BT_DEINIT_CFM_T, BT_SYNC_REGISTER_CFM_T, BT_SYNC_UNREGISTER_CFM_T, BT_SET_MODE_CFM_T, BT_RFCOMM_LINE_STATUS_CFM_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
    HCI_STATUS_E hci_status;
} BT_INQUIRY_CANCEL_CFM_T, BT_INQUIRY_REQ_CFM_T;

typedef struct
{
    HCI_STATUS_E status;
} BT_SET_SUPERVISION_CFM_T, BT_SET_SNIFF_SUBRATING_CFM_T, BT_INQUIRY_STOP_IND_T,BT_SET_LINK_POLICY_CFM_T;



typedef struct BT_GET_ROLE_CFM_T
{
    HCI_STATUS_E status;
    ROLE_E role;
	U8 bdaddr[6];
} BT_GET_ROLE_CFM_T, BT_SET_ROLE_CFM_T;

typedef struct BT_QOS_SETUP_CFM_T
{
    HCI_STATUS_E status;
    U8 bdaddr[6];


    U8 flags;
    U8 service_type;
    U32 token_rate;
    U32 peak_bandwidth;
    U32 latency;
    U32 delay_variation;
} BT_QOS_SETUP_CFM_T;



typedef struct BT_ROLE_CHANGE_IND_T
{
    HCI_STATUS_E status;
    U8 bdaddr[6];
    ROLE_E role;
} BT_ROLE_CHANGE_IND_T;

typedef struct BT_MODE_CHANGE_IND_T
{
    HCI_STATUS_E status;
    U8 bdaddr[6];
    LINK_MODE_E mode;
    U16 interval;
} BT_MODE_CHANGE_IND_T;

typedef struct BT_SUPERVISION_TIMEOUT_CHANGE_IND_T
{
    U8 bdaddr[6];
    U16 timeout;
} BT_SUPERVISION_TIMEOUT_CHANGE_IND_T;

typedef struct BT_SNIFF_SUB_RATING_IND_T
{
    HCI_STATUS_E status;
    U8 bdaddr[6];
    U16 max_rx_latency;
    U16 max_tx_latency;
    U16 min_remote_timeout;
    U16 min_local_timeout;
} BT_SNIFF_SUB_RATING_IND_T;

typedef struct BT_SM_AUTHENTICATE_CFM_T
{
    BT_GENERIC_STATUS_E status;
    U8 bdaddr[6];
} BT_SM_AUTHENTICATE_CFM_T;

typedef struct BT_GENERIC_CFM_T
{
    HCI_STATUS_E hci_status;
} BT_WRITE_INQUIRY_ACCESS_CODE_CFM_T, BT_WRITE_INQUIRY_SCAN_TYPE_CFM_T,
    BT_WRITE_PAGE_SCAN_TYPE_CFM_T, BT_WRITE_SCAN_ENABLE_CFM_T,
    BT_WRITE_INQSCAN_ACTIVITY_CFM_T, BT_WRITE_PAGESCAN_ACTIVITY_CFM_T,
    BT_WRITE_CLASS_OF_DEVICE_CFM_T, BT_WRITE_PAGE_TIMEOUT_CFM_T, BT_WRITE_FLUSH_TIMEOUT_CFM_T,
    BT_WRITE_LOCAL_NAME_CFM_T, BT_SM_SET_DEVICE_CONFIG_CFM_T, BT_WRITE_EIR_DATA_CFM_T,
    BT_WRITE_INQUIRY_TX_CFM_T, BT_WRITE_INQUIRY_MODE_CFM_T,
    BT_READ_REMOTE_NAME_CANCEL_CFM_T, BT_READ_REMOTE_NAME_CFM_T,BT_WRITE_LOCAL_BD_ADDR_CFM_T,
    BT_SYNC_SET_RETRANSMISSION_ENABLE_CFM_T,BT_AWS_MCE_SHARE_MODE_ENABLE_CFM_T,BT_SET_LOW_LATENCY_MODE_ENABLE_CFM_T,
    BT_SET_LC_CONTROL_MODE_ENABLE_CFM_T;

typedef struct BT_SYNC_CONNECT_CFM_T
{
    HCI_STATUS_E hci_status;
    U8 link_type;
    U8 bd_addr[6];
    SINK sink;
    U16 rx_bandwidth;
    U16 tx_bandwidth;
    U16 sco_handle;
    U8 airmode;
} BT_SYNC_CONNECT_CFM_T;

typedef struct BT_SYNC_CONNECT_IND_T
{
    U8 link_type;
    U8 bd_addr[6];
} BT_SYNC_CONNECT_IND_T;

typedef struct BT_SYNC_DISCONNECT_IND_T
{
    HCI_STATUS_E hci_status;
    SINK sink;
    U8 reason;
} BT_SYNC_DISCONNECT_IND_T;

typedef struct BT_SYNC_DISCONNECT_CFM_T
{
    HCI_STATUS_E hci_status;
    SINK sink;
} BT_SYNC_DISCONNECT_CFM_T;




typedef struct BT_GENERIC_CFM_BOOL_T
{
    BT_GENERIC_STATUS_E status;
} BT_SM_SET_SERVICE_CONFIG_CFM_T, BT_SM_UNSET_DEVICE_CONFIG_CFM_T, BT_SM_UNSET_SERVICE_CONFIG_CFM_T;

typedef struct BT_READ_INQUIRY_MODE_CFM_T
{
    U8 hci_status;
    U8 mode;
} BT_READ_INQUIRY_MODE_CFM_T;

typedef struct BT_READ_LINK_QUALITY_CFM_T
{
    HCI_STATUS_E hci_status;
    U8 link_quality;
    SINK sink;
} BT_READ_LINK_QUALITY_CFM_T;

typedef struct BT_READ_RSSI_CFM_T
{
    HCI_STATUS_E hci_status;
    S8 rssi;
    U8 bd_addr[6];
} BT_READ_RSSI_CFM_T;


typedef struct BT_READ_AFH_CHANNEL_MAP_CFM_T
{
    HCI_STATUS_E hci_status;
    U8 bd_addr[6];
    U8 afh_mode_enable;
    U8 channel_map[10];
} BT_READ_AFH_CHANNEL_MAP_CFM_T;


typedef struct BT_READ_CLOCK_CFM_T
{
    HCI_STATUS_E hci_status;
    U32 clock;
    U16 accuracy;
    U8 bd_addr[6];
} BT_READ_CLOCK_CFM_T;


typedef struct BT_READ_REMOTE_VERSION_CFM_T
{
    HCI_STATUS_E hci_status;
    U8 lmpVersion;
    U16 manufacturerName;
    U16 lmpSubVersion;
    U16 handle;
    SINK sink;
} BT_READ_REMOTE_VERSION_CFM_T;

typedef struct BT_READ_LOCAL_VERSION_CFM_T
{
    HCI_STATUS_E hci_status;
    U8 hciVersion;
    U16 hciRevision;
    U8 lmpVersion;
    U16 manufacturerName;
    U16 lmpSubVersion;
} BT_READ_LOCAL_VERSION_CFM_T;

typedef struct BT_READ_CLOCK_OFFSET_CFM_T
{
    HCI_STATUS_E hci_status;
    SINK sink;
    U16 clock_offset;
} BT_READ_CLOCK_OFFSET_CFM_T;


typedef struct BT_SET_PACKET_TYPE_CFM_T
{
    HCI_STATUS_E hci_status;
    U16 connHandle;
    U16 pktType;
} BT_SET_PACKET_TYPE_CFM_T;



typedef struct BT_READ_LOCAL_BD_ADDR_CFM_T
{
    U8 hci_status;
    U8 bd_addr[6];
} BT_READ_LOCAL_BD_ADDR_CFM_T;

typedef struct BT_REMOTE_FEATURES_CFM_T
{
    U8 hci_status;
    U8 features[8];
    SINK sink;
} BT_REMOTE_FEATURES_CFM_T;

typedef struct BT_READ_SCAN_ENABLE_CFM_T
{
    U8 hci_status;
    U8 scan_enable;
} BT_READ_SCAN_ENABLE_CFM_T;

typedef struct BT_READ_LOCAL_NAME_CFM_T
{
    U8 hci_status;
    U8 name[248];
} BT_READ_LOCAL_NAME_CFM_T;

typedef struct BT_INQUIRY_RESULT_IND_T
{
    U8 bd_addr[6];
    PAGE_SCAN_REP_MODE_E rep_mode;
    U32 COD;
    U16 clock_offset;
    S8 rssi;
    U8 eirlen;
    U8 eir[1];
} BT_INQUIRY_RESULT_IND_T;

typedef struct BT_READ_REMOTE_NAME_COMPLETE_IND_T
{
    U8 hci_status;
    U8 bd_addr[6];
    U8 length;
    U8 remote_name[248];
} BT_READ_REMOTE_NAME_COMPLETE_IND_T;

typedef struct BT_EDR_LINK_INFO_T
{
    U8 bd_addr[6];
    U8 remote_name[30];
} PACKED BT_EDR_LINK_INFO_T;

typedef struct BT_GET_EDR_LINK_STATUS_IND_T
{
    U8 status;
    U8 actived_link_num;
    U8 link[1];
} BT_GET_EDR_LINK_STATUS_IND_T;

typedef struct BT_READ_INQUIRY_TX_CFM_T
{
    U8 hci_status;
    S8 tx_power;
} BT_READ_INQUIRY_TX_CFM_T;

typedef struct BT_ACL_OPENED_IND_T
{
    /*! HCI status*/
    U8 status;
    /*! remote BT address*/
    U8 addrType;
    U8 bdaddr[6];
    BOOL isBle;
} BT_ACL_OPENED_IND_T;

typedef struct BT_SM_ENCRYPT_IND_T
{
    U8 bdAddr[6]; /*! The Bluetooth address.*/
    U8 isEncrypted;
    U8 isKeyRefresh;
} BT_SM_ENCRYPT_IND_T;

typedef struct BT_ACL_CLOSED_IND_T
{
    /*! HCI status*/
    U8 status;
    /*! reason */
    U8 reason;
    /*! remote BT address*/
    U8 addrType;
    U8 bdaddr[6];
    BOOL isBle;
} BT_ACL_CLOSED_IND_T;


typedef struct BT_ACL_DETACH_CFM_T
{
    BT_GENERIC_STATUS_E status;
    U8 bdaddr[6];
} BT_ACL_DETACH_CFM_T;


typedef struct BT_SM_ADD_AUTH_DEVICE_CFM_T
{
    BT_GENERIC_STATUS_E status;
} BT_SM_ADD_AUTH_DEVICE_CFM_T;

typedef struct BT_SM_KEY_ID_T
{
    U16 oldKeyId;
    U16 newKeyId;
} BT_SM_KEY_ID_T;

typedef struct BT_SM_ADD_AUTH_DEVICE_LIST_CFM_T
{
    BT_GENERIC_STATUS_E status;
    U8 numOfList;
    BT_SM_KEY_ID_T keyIdList[1];
} BT_SM_ADD_AUTH_DEVICE_LIST_CFM_T;

typedef struct
{
    BDADDR_T bdaddr;
    U8 bonded : 1;
    U8 isBle : 1;
    U8 isEdr : 1;
    U8 reserved : 5;
    U8 link_key_size;
    U8 key_type;
    U8 link_key[16];
}BT_LINK_INFO_T;

typedef struct BT_SM_GET_AUTH_DEVICE_CFM_T
{
    BT_GENERIC_STATUS_E status;
    BT_LINK_INFO_T linkInfo;
} BT_SM_GET_AUTH_DEVICE_CFM_T;

typedef enum SEARCH_LINK_TYPE_E {
    SEARCH_TYPE_EDR = 0,
    SEARCH_TYPE_BLE,
    SEARCH_TYPE_ALL,
} SEARCH_LINK_TYPE_E;

typedef struct BT_SM_GET_AUTH_DEVICE_LIST_CFM_T
{
    BT_GENERIC_STATUS_E status;
    U8 num_of_devices;
    BDADDR_T bdaddr_list[1];
} BT_SM_GET_AUTH_DEVICE_LIST_CFM_T;

typedef struct BT_SM_GET_AUTH_DEVICE_LIST_EX_CFM_T
{
    BT_GENERIC_STATUS_E status;
    SEARCH_LINK_TYPE_E searchType;
    U8 numOfList;
    U16 length;
    U8 list[1];
} BT_SM_GET_AUTH_DEVICE_LIST_EX_CFM_T;

typedef struct BT_SM_DELETE_AUTH_DEVICE_CFM_T
{
    BT_GENERIC_STATUS_E status;
} BT_SM_DELETE_AUTH_DEVICE_CFM_T;

typedef struct BT_SM_DELETE_ALL_AUTH_DEVICE_CFM_T
{
    BT_GENERIC_STATUS_E status;
} BT_SM_DELETE_ALL_AUTH_DEVICE_CFM_T;

typedef struct BT_SM_PIN_CODE_IND_T
{
    U8 bd_addr[6];
} BT_SM_PIN_CODE_IND_T, BT_SM_USER_PASSKEY_REQ_IND_T;

typedef struct BT_SM_IO_CAPABILITY_REQ_IND_T
{
    U8 bd_addr[6];
    BOOL isBle;
} BT_SM_IO_CAPABILITY_REQ_IND_T;

typedef struct BT_SM_REMOTE_IO_CAPABILITY_IND_T
{
    U8 bd_addr[6];
    U8 iocap;
    U8 oob_data_present;
    U8 mitm;
    U8 bonding;
} BT_SM_REMOTE_IO_CAPABILITY_IND_T;



typedef struct BT_SM_USER_CONFIRMATION_REQ_IND_T
{
    U8 bd_addr[6];
    U32 passkey;
} BT_SM_USER_CONFIRMATION_REQ_IND_T, BT_SM_USER_PASSKEY_NOTIFICATION_IND_T;

typedef enum BT_SM_SECURITY_REQ_CFM_STATUS_E {
    BT_SECURITY_REQ_SUCCESS,
    BT_SECURITY_REQ_FAIL,
    BT_SECURITY_REQ_TIMEOUT,
    BT_PAIRING_IN_PROGRESS,
    BT_LINK_KEY_MISSING,
} BT_SM_SECURITY_REQ_CFM_STATUS_E;

typedef struct BT_SM_SECURITY_REQ_CFM_T
{
    U8 bd_addr[6];
    U8 status;
} BT_SM_SECURITY_REQ_CFM_T;

typedef struct BT_SM_AUTHENTICATION_IND_T
{
    U8 bd_addr[6];
    U8 status;
} BT_SM_AUTHENTICATION_IND_T;

typedef struct BT_SM_OOB_DATA_REQ_IND_T
{
    U8 bd_addr[6];
    U8 oobDataPresent;
    U8 r[16];
    U8 c[16];
} BT_SM_OOB_DATA_REQ_IND_T;

typedef struct BT_BLE_GEN_RESOLVABLE_PRIVATE_ADDR_RSP_T
{
    U8 status;
    U8 bd_addr[6];
} BT_BLE_GEN_RESOLVABLE_PRIVATE_ADDR_RSP_T;

typedef struct BT_SM_SECURITY_EVENT_IND_T
{
    BT_SM_SECURITY_EVENT_E event;
    U8 bd_addr[6];
} BT_SM_SECURITY_EVENT_IND_T;



typedef enum BT_PROTOCOL_E {
    BT_PROTOCOL_L2CAP,
    BT_PROTOCOL_RFCOMM,

} BT_PROTOCOL_E;

typedef struct BT_PROTOCOL_T
{
    BT_PROTOCOL_E protocol;
    union {
        U16 channel; /*! RFCOMM channel*/
        U16 psm;     /*! l2cap psm*/
    };
} BT_PROTOCOL_T;

typedef struct BT_READ_EIR_DATA_CFM_T
{
    U8 status;
    U8 fec_required;
    U8 eir_data[240];
} BT_READ_EIR_DATA_CFM_T;

typedef struct BT_REC_CLEAR_RECLIST_BDA_T
{
    U8 bdaddr[6];
} BT_REC_CLEAR_RECLIST_BDA_T;

typedef struct
{
    BOOL isDataValid;
    BOOL isLinkEncrypted;
    U8 bdaddr[6];
}BT_ACL_LINK_STATUS_T;

typedef struct
{
    U8 bd_addr[6];
    U8 cod[3];
} BT_CONNECT_REQUEST_IND_T;

/**************************************************************************************************/
/*SM structures*/

#define BLE_SM_IO_CAP_DISPLAY_ONLY                      0x00
#define BLE_SM_IO_CAP_DISPLAY_YES_NO                    0x01
#define BLE_SM_IO_CAP_KEYBOARD_ONLY                     0x02
#define BLE_SM_IO_CAP_NO_INPUT_NO_OUTPUT                0x03
#define BLE_SM_IO_CAP_KEYBOARD_DISPLAY                  0x04
typedef U8 BLE_SM_IO_CAPABILITY_T;

#define BLE_SM_OOB_DATA_NOT_PRESENTED                   0x00
#define BLE_SM_OOB_DATA_PRESENTED                       0x01
typedef U8 BLE_SM_OOB_DATA_FLAG_T;

#define BLE_SM_AUTH_REQ_NO_BONDING                      0x00
#define BLE_SM_AUTH_REQ_BONDING                         (0x01 << 0)
#define BLE_SM_AUTH_REQ_MITM                            (0x01 << 2)
#define BLE_SM_AUTH_REQ_SECURE_CONNECTION               (0x01 << 3)
typedef U8 BLE_SM_AUTH_REQ_T;

#define BLE_SM_KEY_DISTRIBUTION_ENC_KEY                 (0x01 << 0)
#define BLE_SM_KEY_DISTRIBUTION_ID_KEY                  (0x01 << 1)
#define BLE_SM_KEY_DISTRIBUTION_SIGN_KEY                (0x01 << 2)
#define BLE_SM_KEY_DISTRIBUTION_LINK_KEY                (0x01 << 3)
typedef U8 BLE_SM_KEY_DISTRIBUTION_T;


#define ROLE_MASTER 0x00
#define ROLE_SLAVE  0x01
typedef U8 ROLE_T;

typedef enum BT_IO_CAPABILITY_E {
    IO_CAP_DISPLAY_ONLY = 0,
    IO_CAP_DISPLAY_YES_NO,
    IO_CAP_KEYBOARD_ONLY,
    IO_CAP_NOINPUT_OUTPUT,
    IO_CAP_KEYBOARD_DISPLAY,
} BT_IO_CAPABILITY_E;

typedef enum BT_KEY_PRESS_TYPE_E {
    KEY_START,
    KEY_ENTER,
    KEY_ERASE,
    KEY_CLEAR,
    KEY_COMPLETE
} BT_KEY_PRESS_TYPE_E;

typedef enum BT_OOB_DATA_TYPE_E {
    OOB_DATA_NOT_PRESERNT = 0,
    OOB_DATA_P192,
    OOB_DATA_P256,
    OOB_DATA_P192_P256,
} BT_OOB_DATA_TYPE_E;

typedef enum BT_SECURITY_MODE_E {
    SEC_MODE0_OFF,
    SEC_MODE1_NON_SECURE,
    SEC_MODE2_SERVICE,
    SEC_MODE3_LINK,
    SEC_MODE4_SSP,
} BT_SECURITY_MODE_E;

/*!
	@brief SSP security level
	Table 5.8: Security Level mapping to link key requirements
*/
typedef enum BT_SECURITY_MODE4_LEVEL_E {
    MODE4_LEVEL0, /*!< No security.*/
    MODE4_LEVEL1, /*!< No MITM pairing, encryption depends on peer's type.(No encryption for pre-v2.1 devices).*/
    MODE4_LEVEL2, /*!< No MITM pairing, encryption desired.*/
    MODE4_LEVEL3, /*!< MITM pairing, encryption desired. */
    MODE4_LEVEL4, /*!< MITM pairing(P256 based SSP), encryption desired.*/
} BT_SECURITY_MODE4_LEVEL_E;

/** \brief  Legacy mode 2 attributes */
typedef struct BT_SECURITY_MODE2_ATTRIBUTE_T
{
    U8 MODE2_AHTHORIZATION;  /*!< Authorization required*/
    U8 MODE2_AHTHENTICATION; /*!< Authentication required*/
    U8 MODE2_ENCRYPTION;     /*!< Encryption required*/
} BT_SECURITY_MODE2_ATTRIBUTE_T;

/** \brief  Legacy mode 3 encrypted setting */
typedef U8 BT_SECURITY_MODE3_ENCRYPTION_T;

typedef struct BT_SECURITY_CONFIG_T
{
    BT_SECURITY_MODE_E mode;
    BT_SECURITY_MODE3_ENCRYPTION_T mode3_setting;
} BT_SECURITY_CONFIG_T;

typedef struct BT_SECURITY_SERVICE_CONFIG_T
{
    BT_PROTOCOL_T protocol;
    BT_SECURITY_MODE2_ATTRIBUTE_T mode2_attr;
    BT_SECURITY_MODE4_LEVEL_E mode4_level;
    BOOL IsIncomming; /*! Incoming or outgoing*/
} BT_SECURITY_SERVICE_CONFIG_T;

/** \brief Confirmation message of \ref BtWriteClassOfDevice*/
typedef struct BT_READ_CLASS_OF_DEVICE_CFM_T
{
    /** The HCI status. */
    U8 hci_status;
    /** The cod. */
    U32 COD;
} BT_READ_CLASS_OF_DEVICE_CFM_T;

typedef struct BT_SM_KEYPRESS_NOTIFICATION_IND_T
{
    U8 bd_addr[6];
    BT_KEY_PRESS_TYPE_E type;
} BT_SM_KEYPRESS_NOTIFICATION_IND_T;

typedef enum BLE_SECURITY_MODE_E {
    SEC_MODE1,
    SEC_MODE2,
} BLE_SECURITY_MODE_E;

typedef enum BLE_SECURITY_MODE1_LEVEL_E {
    SEC_MODE1_LEVEL1, /*!< No security (No authentication and no encryption).*/
    SEC_MODE1_LEVEL2, /*!< Unauthenticated pairing with encryption.*/
    SEC_MODE1_LEVEL3, /*!< Authenticated pairing with encryption. */
    SEC_MODE1_LEVEL4, /*!< Authenticated LE Secure Connections pairing with encryption using a 128-bit strength encryption key.*/
} BLE_SECURITY_MODE1_LEVEL_E;

typedef enum BLE_SECURITY_MODE2_LEVEL_E {
    SEC_MODE2_NONE,   /*!< None.*/
    SEC_MODE2_LEVEL1, /*!< Unauthenticated pairing with data signing.*/
    SEC_MODE2_LEVEL2, /*!< Authenticated pairing with data signing.*/
} BLE_SECURITY_MODE2_LEVEL_E;

typedef struct BLE_SECURITY_SERVICE_CONFIG_T
{
    BLE_SECURITY_MODE_E mode;
    U8 level;
} BLE_SECURITY_SERVICE_CONFIG_T;

typedef struct BLE_SM_IO_CAPABILITY_REQUEST_RSP_T
{
    BLE_SM_IO_CAPABILITY_T io_capability;
    BLE_SM_OOB_DATA_FLAG_T oob_data;
    BLE_SM_AUTH_REQ_T auth_req;
    BLE_SM_KEY_DISTRIBUTION_T key_distribution;
} BLE_SM_IO_CAPABILITY_REQUEST_RSP_T;

/**************************************************************************************************/
/*Link Policy*/
#define DISABLE_ALL_LM_MODES	0x0000
#define ENABLE_ROLE_SWITCH		0x0001
#define ENABLE_HOLD_MODE		0x0002
#define ENABLE_SNIFF_MODE		0x0004
#define ENABLE_PARK_STATE		0x0008


/**************************************************************************************************/
/*BLE structures*/
#define BLE_MAX_DATA_SIZE 31

typedef enum {
    BLE_ADV_IND = 0,
    BLE_ADV_DIRECT_IND = 1,
    BLE_ADV_SCAN_IND = 2,
    BLE_ADV_NON_CONN_IND =3,
} BLE_ADV_TYPE_E;

typedef enum {
    PUBLIC_DEVICE_ADDR,
    RANDOM_DEVICE_ADDR,
    RESOLVABLE_PRIVATE_ADDR_OR_PUBLIC_ADDR,
    RESOLVABLE_PRIVATE_ADDR_OR_RANDOM_ADDR,
} BLE_OWN_ADDR_TYPE_E;

typedef enum {
    BLE_FILTER_NONE,         /*! Allow scan and connect request from any */
    BLE_FILTER_SCAN_ONLY,    /*! Allow scan request from white list only, allow any connect */
    BLE_FILTER_CONNECT_ONLY, /*! Allow any scan, Allow connect from white list only */
    BLE_FILTER_BOTH,         /*! Allow scan and connect request from white list only */
} BLE_ADV_FILTER_POLICY_E;

typedef enum {
    BLE_ADDR_WRITE_STATIC,
    BLE_ADDR_GENERATE_STATIC,
    BLE_ADDR_GENERATE_NON_RESOLVABLE,
    BLE_ADDR_GENERATE_RESOLVABLE,
} BLE_RANDOM_ADDR_TYPE_E;

typedef U8 BLE_ADDR_TYPE;

enum BLE_ADDR_TYPE_E{
    BLE_ADDR_TYPE_PUBLIC_DEVICE_ADDR = (BLE_ADDR_TYPE)0x00,
    BLE_ADDR_TYPE_RANDOM_DEVICE_ADDR = (BLE_ADDR_TYPE)0x01,
};

typedef enum {
    BLE_PASSIVE_SCANNING, /*! No SCAN_REQ packets shall be sent.(default) */
    BLE_ACTIVE_SCANNING,  /*! SCAN_REQ packets may be sent */
} BLE_SCAN_TYPE_E;

typedef enum {
    BLE_SCAN_FILTER_POLICY_0, /*! accept all
                                                             advertisement packets except directed advertising packets notaddressed to this device (default). */
    BLE_SCAN_FILTER_POLICY_1, /*! accept only
                                                             advertisement packets from devices where the advertiser!|s address is in the White list.
                                                             Directed advertising packets which are not addressed for this device shall be ignored. */
    BLE_SCAN_FILTER_POLICY_2, /*! accept all 
                                                             undirected advertisement packets, and
                                                             directed advertising packets where the initiator address is a resolvable private address, and
                                                             directed advertising packets addressed to this device. */
    BLE_SCAN_FILTER_POLICY_3, /*! accept all
                                                             advertisement packets from devices where the advertiser!|s address is in the White list, and
                                                             directed advertising packets where the initiator address is a resolvable private address, and
                                                             directed advertising packets addressed to this device */
} BLE_SCAN_FILTER_POLICY_E;

typedef enum {
    BLE_REPORT_TYPE_IND,         /*! Connectable undirected advertising (ADV_IND). */
    BLE_REPORT_TYPE_DIRECT_IND,  /*! Connectable directed advertising (ADV_DIRECT_IND) */
    BLE_REPORT_TYPE_SCAN_IND,    /*! Scannable undirected advertising (ADV_SCAN_IND) */
    BLE_REPORT_TYPE_NONCONN_IND, /*! Non connectable undirected advertising (ADV_NONCONN_IND) */
    BLE_REPORT_TYPE_SCAN_RSP,    /*! Scan Response (SCAN_RSP) */
} BLE_REPORT_TYPE_E;

typedef enum {
    BLE_ADV_PRIMARY,
    BLE_ADV_GFP,
    BLE_ADV_AMA,
	BLE_ADV_XIAOAI,
	BLE_ADV_TILE,
    BLE_ADV_SWIFT,
    BLE_ADV_HANDLE_NUM, /* Need to check NUM_OF_SUPP_LE_ADV_SETS in Config.h with the owner of controller */
} BLE_ADV_HANDLE_E;

typedef struct
{
    BLE_SCAN_TYPE_E scanType;
    U16 scanInterval;
    U16 scanWindow;
    BLE_OWN_ADDR_TYPE_E ownAddrType;
    BLE_SCAN_FILTER_POLICY_E scanFilterPolicy;
} BLE_SCAN_PARAMS_T;

typedef struct
{
    U32 advIntervalMin;
    U32 advIntervalMax;
    BLE_ADV_FILTER_POLICY_E filterPolicy;
} BLE_UNDIRECTED_ADV_PARAMS_T;

typedef struct
{
    BOOL randomDirectAddr;
    BD_ADDR_T directBdAddr;
} BLE_DIRECTED_ADV_PARAMS_T;

typedef union BLE_ADV_PARAMS_T {
    BLE_UNDIRECTED_ADV_PARAMS_T undirectedAdvParams;
    BLE_DIRECTED_ADV_PARAMS_T directedAdvParams;
} BLE_ADV_PARAMS_T;

typedef enum {
    INITIATOR_FILTER_POLICY_WHITE_LIST_NOT_USED, // Peer_Address_Type and Peer_Address shall be used
    INITIATOR_FILTER_POLICY_WHITE_LIST_USED,     // Peer_Address_Type and Peer_Address shall be ignored
} BLE_INITIATOR_FILTER_POLICY_E;

typedef enum {
    PEER_PUBLIC_DEVICE_ADDR,
    PEER_RANDOM_DEVICE_ADDR,
    PEER_PUBLIC_IDENTITY_ADDR,
    PEER_RANDOM_IDENTITY_ADDR,
} BLE_PEER_ADDR_TYPE_E;

typedef struct
{
    BLE_ADDR_TYPE addrType;
    BD_ADDR_T bdAddr;
    U16 connIntervalMin;    /* unit:1.25 ms, 7.5 ms - 4000 ms*/
    U16 connIntervalMax;    /* unit:1.25ms, 7.5 ms - 4000 ms*/
    U16 connLatency;
    U16 supervisonTimeout;  /* unit:10 ms, 100 ms - 32000 ms*/
} BLE_CONN_PARAMS_UPDATE_T;

typedef struct
{
    BLE_ADDR_TYPE addrType;
    BD_ADDR_T bdAddr;
    U8 id;
    U16 connIntervalMin;
    U16 connIntervalMax;
    U16 connLatency;
    U16 supervisonTimeout;
} BLE_CONN_PARAMS_UPDATE_IND_T;

typedef struct
{
    U16 scanInterval;
    U16 scanWindow;
    BLE_INITIATOR_FILTER_POLICY_E policy;
    BLE_PEER_ADDR_TYPE_E peerAddrType;
    BD_ADDR_T peerBdAddr;
    BLE_OWN_ADDR_TYPE_E ownAddrType;
    U16 connIntervalMin;
    U16 connIntervalMax;
    U16 connLatency;
    U16 supervisionTimeout;
    U16 minCeLen;
    U16 maxCeLen;
} BLE_CREATE_CONN_PARAMS_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
} BT_BLE_SET_SCAN_ENABLE_CFM_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
} BT_BLE_SET_SCAN_PARAMS_CFM_T;

typedef struct
{
    BLE_REPORT_TYPE_E reportType;
    BLE_OWN_ADDR_TYPE_E addrType;
    BD_ADDR_T bdAddr;
    U8 lenData;
    U8 data[BLE_MAX_DATA_SIZE];
    U8 rssi;
} BT_BLE_ADV_REPORT_IND_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
} BT_BLE_SET_SCAN_RESPONSE_DATA_CFM_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
} BT_BLE_SET_ADV_DATA_CFM_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
} BT_BLE_SET_ADV_ENABLE_CFM_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
} BT_BLE_SET_ADV_PARAMS_CFM_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
} BT_BLE_SET_ADV_PARAMS_EX_CFM_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
} BT_BLE_SET_LOCAL_RANDOM_ADDR_CFM_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
    U8 size;
} BT_BLE_READ_WHITE_LIST_SIZE_CFM_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
} BT_BLE_CLEAR_WHITE_LIST_CFM_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
} BT_BLE_ADD_DEVICE_TO_WHITE_LIST_CFM_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
} BT_BLE_REMOVE_DEVICE_FROM_WHITE_LIST_CFM_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
    BLE_ADDR_TYPE addrType;
    BD_ADDR_T bdAddr;
    U16 connInterval;       /* unit:1.25ms */
    U16 connLatency;
    U16 supervisionTimeout; /* 10ms */
} BT_BLE_CONNECTION_PARAMS_UPDATE_CFM_T;

typedef struct
{
    BLE_CONN_PARAMS_UPDATE_IND_T params;

} BT_BLE_CONNECTION_PARAMS_UPDATE_IND_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
    U8 index;
} BT_BLE_CREATE_CONNECTION_CFM_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
} BT_BLE_CREATE_CONNECTION_CANCEL_CFM_T;

typedef struct
{
    U8 bdAddr[6]; /*! The Bluetooth address.*/
    U8 reason;
    U8 status;
} BT_BLE_DISCONNECT_REQ_RSP_T;

typedef struct BT_BLE_SM_SET_SERVICE_CONFIG_CFM_T
{
    BT_GENERIC_STATUS_E status;
} BT_BLE_SM_SET_SERVICE_CONFIG_CFM_T;

typedef struct BT_BLE_LINK_INFO_T
{
    U8 bd_addr[6];
    U8 bd_addrType;
} PACKED BT_BLE_LINK_INFO_T;

typedef struct BT_BLE_GET_LINK_STATUS_IND_T
{
    U8 status;
    U8 actived_link_num;
    U8 link[0];
} BT_BLE_GET_LINK_STATUS_IND_T;

typedef struct
{
    BT_GENERIC_STATUS_E status;
} BT_BLE_SET_ADV_ENABLE_ALL_CFM_T;

typedef struct
{
    U8 disableAdvMask;
    BT_GENERIC_STATUS_E status;
} BT_BLE_SET_ADV_DISABLE_ALL_CFM_T;

typedef struct BT_GAP_DUTMODE_CFM_T
{
    U8 status;

} BT_GAP_DUTMODE_CFM_T, BT_GAP_CANCEL_CONN_CFM_T;


typedef struct BLE_EXT_ADV_DATA_T
{
    U8 *advData;
    U8 sizeAdvData;
} BLE_EXT_ADV_DATA_T;

typedef struct BLE_EXT_SCAN_RSP_DATA_T
{
    U8 *scanRspData;
    U8 sizeRspData;
} BLE_EXT_SCAN_RSP_DATA_T;



typedef struct BLE_EXT_ADV_PARAM_T
{
    BLE_ADV_TYPE_E advType;
    BLE_OWN_ADDR_TYPE_E ownAddrType;
    BLE_ADV_PARAMS_T AdvParms;
    U8 channelMap;
} BLE_EXT_ADV_PARAM_T;

typedef struct BT_BLE_ADV_DATA_T
{
    BLE_RANDOM_ADDR_TYPE_E randomAddrType;
    BD_ADDR_T RandomAddr;
} BLE_EXT_RANDOM_ADDR_T;

typedef struct BLE_EXT_ADV_PARAM_EX_T
{
    U8 setAdvParam:1;
    U8 setAdvData:1;
    U8 setScanRspData:1;
    U8 setRandomAddr:1;

    /* ADV param */
    BLE_EXT_ADV_PARAM_T extAdvParam;
    /* ADV data */
    U8 *advData;
    U8 sizeAdvData;
    /* Scan rsp data */
    U8 *scanRspData;
    U8 sizeRspData;
    /* Random addr */
    BLE_EXT_RANDOM_ADDR_T extRadomAddr;
} BLE_EXT_ADV_PARAM_EX_T;


/*!
    @}
*/
/**************************************************************************************************/
/*                            Functions                                                           */
/**************************************************************************************************/
/*!
    @addtogroup BT_LIB_GAP_FN
    @{
*/
/*inquiry*/
void BtWriteEirData(Handler handler, U8 fec_enabled, U8 eir_len, U8 *eir);
void BtReadEirData(Handler handler);
void BtInquiry(Handler handler, U32 lap, U8 timeout, U8 max_num);
void BtInquiryPeriodic(Handler theAppHandler, U16 min_period, U16 max_period, U32 lap, U8 timeout, U8 max_num);
void BtInquiryCancel(Handler handler);
void BtWriteInquiryTxPower(Handler handler, S8 tx_power);
void BtReadInquiryTxPower(Handler handler);
void BtReadInquiryMode(Handler handler);
void BtWriteInquiryMode(Handler handler, INQUARY_MODE_E mode);
void BtReadRemoteName(Handler handler, U8 bdaddr[6]);
void BtReadRemoteNameCancel(Handler handler, U8 bdaddr[6]);
void BtGetEDRLinkStatus(Handler handler);

/*baseband*/
void BtWriteInquiryScanType(Handler handler, SCAN_TYPE_T type);
void BtWritePageScanType(Handler handler, SCAN_TYPE_T type);
void BtReadScanEnable(Handler app);
void BtWriteScanEnable(Handler handler, SCAN_ENABLE_T mode);
void BtWriteInquiryAccessCode(Handler handler, const U32 *iac, U8 num_iac);
void BtReadClassOfDevice(Handler handler);
void BtWriteLocalBdaddr(Handler handler, U8 bdaddr[6]);

/*!
	@brief Write class of device
	@param handler The message handler to received complete message BT_WRITE_CLASS_OF_DEVICE_CFM.
	@param COD Class of device.
*/

void BtWriteClassOfDevice(Handler handler, U32 COD);
void BtWriteInquiryScanActivity(Handler handler, U16 interval, U16 window);
void BtWritePagescanActivity(Handler handler, U16 interval, U16 window);
void BtWriteFlushTimeout(Handler handler, SINK sink, U16 flush_timeout);
void BtWriteLocalName(Handler handler, U16 size_local_name, U8 *local_name);
void BtReadLocalName(Handler handler);
void BtWritePageTimeout(Handler handler, U16 timeout);

/*security*/
void BtSmAuthenticateRequest(Handler handler, U8 bdaddr[6]);
void BtSmSetDeviceSecurityConfig(Handler handler, BT_SECURITY_CONFIG_T sec);
void BtSmSetServiceSecurityConfig(Handler handler, BT_SECURITY_SERVICE_CONFIG_T sec);
void BtSmUnsetServiceSecurityConfig(Handler handler, BT_SECURITY_SERVICE_CONFIG_T sec);
void BtSmPinCodeResponse(U8 bd_addr[6], U8 len, U8 *pin_code);
void BtSmGetAuthDeviceList(Handler handler, U16 max_num, SEARCH_LINK_TYPE_E type);
void BtSmGetAuthDevice(Handler handler, BDADDR_T bdaddr);
void BtSmAddAuthDevice(Handler handler, BDADDR_T addr, U8 link_key_size, U8 key_type, U8 *link_key);
void BtSmDeleteAuthDevice(Handler handler, BDADDR_T bdadddr);
void BtSmDeleteAllAuthDevice(Handler handler);
void BtSmIoCapabilityResponse(U8 bdaddr[6], BT_IO_CAPABILITY_E io_capability, BOOL mitm, BOOL bonding, BT_OOB_DATA_TYPE_E oob_data_present, U8 keyDistribution);
void BtSmUserConfirmationResponse(U8 bdaddr[6], BOOL confirmed);
void BtSmPasskeyResponse(U8 bdaddr[6], BOOL isOk, U32 number);
void BtSmPasskeyNotificationResponse(U8 bdaddr[6], U32 number);
void BtSmSendKeyPressNotify(U8 bdaddr[6], BT_KEY_PRESS_TYPE_E type);
void BtSmSaveLinkKeyRecord(BOOL enable);
void BtSmAddAuthDeviceList(Handler app, U16 length, void *link_key_list);
void BtSmGetAuthDeviceListEx(Handler app, SEARCH_LINK_TYPE_E searchType);

/*information*/
void BtReadRemoteVersion(Handler handler, SINK sink);
void BtReadRemoteSuppFeature(Handler handler, SINK sink);
void BtReadLocalAddr(Handler handler);
void BtReadLinkQuality(Handler handler, SINK sink);
void BtReadRssi(Handler app, U8 (*bdAddr)[6]);
void BtReadClockOffset(Handler handler, SINK sink);
void BtReadLocalVersion(Handler handler);
void BtGetCurrentClock(Handler app, U8 (*bdAddr)[6], CLOCK_E clock_type);
void BtReadAfhChannelMap(Handler app, U8 (*bdAddr)[6]);


/*policy*/
void BtSwitchRole(Handler handler, U8 (*bdAddr)[6], ROLE_T role);
void BtGetRole(Handler handler, U8 (*bdAddr)[6]);
void BtSetSupperTimeout(Handler handler, U8 (*bdAddr)[6], U16 timeout);
void BtSetLinkMode(Handler handler,U8 (*bdAddr)[6], LINK_MODE_E mode, LINK_MODE_PARAM_T *parm);
void BtSetSniffSubrating(Handler handler, U8 (*bdAddr)[6], U16 latency, U16 remote_timeout, U16 local_timeout);
void BtQosSetup(Handler handler,U8 (*bdAddr)[6],U8 flags,U8 ServiceType ,U32 TokenRate,U32 PeakBandwidth,U32 Latency,U32 DelayVariation);
void BtSetLinkPolicy(Handler handler, U8 (*bdAddr)[6], U16 settings);
void BtSyncSetLowLatencyModeEnable(Handler handler, U8 (*bdAddr)[6], BOOL enable);
void BtSetLcControlModeEnable(Handler handler, U8 (*bdAddr)[6], BOOL enable, U8 mode);

/*BLE*/
void BleSetScanEnableReq(Handler handler, BOOL enable);
void BleSetScanParamsReq(Handler handler, BLE_SCAN_PARAMS_T *pParams);
void BleSetScanResponseDataReq(Handler handler, U8 sizeSrData, const U8 *srData);
void BleSetAdvDataReq(Handler handler, U8 sizeAdvData, const U8 *advData);
void BleSetAdvEnableReq(Handler handler, BOOL enable);
void BleSetAdvParamsReq(Handler handler, BLE_ADV_TYPE_E advType, BLE_OWN_ADDR_TYPE_E ownAddrType, U8 channelMap, const BLE_ADV_PARAMS_T *pAdvParms);
void BleSetLocalRandomAddrReq(Handler handler, BLE_RANDOM_ADDR_TYPE_E randomAddrType, BD_ADDR_T *pRandomddr);
void BleReadWhiteListSizeReq(VOID);
void BleClearWhiteListReq(VOID);
void BleAddDeviceToWhiteListReq(BLE_ADDR_TYPE addrType, BD_ADDR_T *pAddr);
void BleRemoveDeviceFromWhiteListReq(BLE_ADDR_TYPE addrType, BD_ADDR_T *pAddr);
void BleConnectionParamsUpdateReq(Handler handler, BLE_CONN_PARAMS_UPDATE_T *pParams);
void BleConnectionParamsUpdateRsp(BOOL rsp, BLE_CONN_PARAMS_UPDATE_IND_T *pParams);
void BleCreateConnectionReq(Handler handler, BLE_CREATE_CONN_PARAMS_T *pParams);
void BleDisconnecReq(Handler handler, U8 bdAddr[6], U8 reason);
void BleGetLinkStatus(Handler handler);
void BleSetLinkInfoNotifyHandler(Handler handler);
void BleSetExtendedLocalRandomAddrReq(Handler handler,BLE_ADV_HANDLE_E handle, BLE_EXT_RANDOM_ADDR_T* randomAddr);
void BleSetExtendedAdvParamsReq(Handler handler,BLE_ADV_HANDLE_E handle, BLE_EXT_ADV_PARAM_T* advParams);
void BleSetExtendedAdvParamsExReq(Handler handler,BLE_ADV_HANDLE_E handle, BLE_EXT_ADV_PARAM_EX_T* advParams);
void BleSetExtendedAdvDataReq(Handler handler,BLE_ADV_HANDLE_E handle, BLE_EXT_ADV_DATA_T* advData);
void BleSetExtendedAdvEnableReq(Handler handler,BLE_ADV_HANDLE_E handle, BOOL enable);
void BleSetExtendedScanResponseDataReq(Handler handler,BLE_ADV_HANDLE_E handle , BLE_EXT_SCAN_RSP_DATA_T* scanRspData);
void BleEnableAdvAllReq(Handler handler, U8 mask);
void BleDisableAdvAllReq(Handler handler);
U32 BleGetEnableAdvMask(void);
void BleGenResolvablePrivateAddrReq(Handler handler);



/* SM */
void BtSmSecurityRequest(U8 bdAddr[6], BOOL mitm, BOOL bonding, BOOL keyPress);
void BtSmOobDataReqResponse(U8 bdAddr[6], BOOL oobDataPresent, U8 *r, U8 *c);
void BleSmSetServiceSecurityConfig(Handler handler, BLE_SECURITY_SERVICE_CONFIG_T sec);
void BleSmIoCapabilityResponse(U8 bdaddr[6], BLE_SM_IO_CAPABILITY_REQUEST_RSP_T *rsp);

/*Synchronous*/
void BtSyncRegister(Handler app);
void BtSyncUnregister(Handler app);
void BtSyncConnectRequest(Handler app, SINK sink, BT_SYNC_CONFIG_PARAM_T *config_params);
void BtSyncConnectResponse(Handler app, U8 bdaddr[6], BOOL accept, U8 link_type, U8 reason, BT_SYNC_CONFIG_PARAM_T *config_params);
void BtSyncDisconnectRequest(Handler app, SINK sink, U8 reason);
void BtSyncSetRetransmitssionEnable(Handler handler, U8 (*bdAddr)[6], BOOL enable);


/* Test mode */
void BtEnableDutMode(Handler app);



/*Common Operation*/

//VOID BtBDaddrCpy(BD_ADDR_T* pDst, BD_ADDR_T* pSrc);
//U8 BtIsSameBDaddr(BD_ADDR_T* pDst, BD_ADDR_T* pSrc);

/* Link control*/
void BtSetMaximumEDRLinkNum(U8 num);
void BtCancelCreateConnection(Handler app, U8 bdaddr[6]);
/* Set acl detach timer when there is no l2cap channel connected. (unit: ms)*/
void BtSetAclDetachTimeout(U8 bdaddr[6], U16 timeout);

void BtSetConnectResponeHandler(Handler handler);
void BtConnectResponse(Handler app, U8 bdaddr[6], BOOL accept, U8 cod[3]);
U16 BtGetAclDetachTimeout(U8 bdaddr[6]);


/* Detach ACL link, return BT_ACL_DETACH_CFM */
void BtDetchAclLink(Handler app, U8 bdaddr[6]);


void BtGetEdrAclLinkStatus(BT_ACL_LINK_STATUS_T* link_status);


void BtSetGapSystemStatus(BT_GAP_SYSTEM_STATUS_E status);
BOOL BtCheckDetachTimerExist(U8 Bdaddr[6]);

/* others */

BT_GAP_GET_ROLE_STATUS_E BtGetLinkRole(U8 bdaddr[6]);

/*!
    @}
*/


BOOL BtGapRhoLock(void);
BOOL BtGapIsRhoAllow(void);
U16 BtGapGetRhoDataLength(void);
BOOL BtGapGetRhoData(U8 *pBuffer);
BOOL BtGapApplyAgentRhoData(BD_ADDR_T *pBdAddr, U8 *pGapRhoData, U16 length);
BOOL BtGapApplyPartnerRhoData(BD_ADDR_T *pBdAddr, U8 *pGapRhoData, U16 length);




#endif
>>>>>>> db20e11 (second commit)
