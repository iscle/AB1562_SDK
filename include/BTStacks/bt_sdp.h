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


#ifndef _BT_SDP_HH__
#define _BT_SDP_HH__

/*!
    @addtogroup BT_LIB_SDP
    @{
        \image html sdp_msc.png "Figure 1: SDP message sequence chart."

        ## SDP
        SDP APIs provides profiles or applications to register services or discover services and attributes of remote devices.

    @}
*/

#include "bt_common.h"


/**************************************************************************************************/
/*                            Macros                                                              */
/**************************************************************************************************/
/*!
    @addtogroup BT_LIB_SDP_MACRO
    @{
        @brief SDP Service record and attribute setup definition.
*/

//////////////////////////////////////////////////////////////////
//SDP Service and Attribute constant value
//////////////////////////////////////////////////////////////////

/*!
    @name SDP Protocol Identifiers [Defines]
    SDP Protocol UUID

    @{
*/
#define PROTOCOL_UUID_RFCOMM                0x0003  ///< RFCOMM.
<<<<<<< HEAD
=======
#define PROTOCOL_UUID_ATT                   0x0007  /* ATT */
>>>>>>> db20e11 (second commit)
#define PROTOCOL_UUID_OBEX                  0x0008  ///< OBEX.
#define PROTOCOL_UUID_HID                   0x0011  ///< HIDP.
#define PROTOCOL_UUID_AVCTP                 0x0017  ///< AVCTP.
#define PROTOCOL_UUID_AVDTP                 0x0019  ///< AVDTP.
#define PROTOCOL_UUID_L2CAP                 0x0100  ///< L2CAP.
/*!
    @}
*/


/*!
    @name SDP Service Class Identifiers [Defines]
    SDP Service Class UUID

    @{
*/
#define SERVICE_UUID_SERVICE_DISCOVERY_SVR  0x1000  ///< Service Discovery Server Service.
#define SERVICE_UUID_BROWSE_GROUP_DESCRIP   0x1001  ///< Browse Group Descriptor Service.
#define SERVICE_UUID_PUBLIC_BROWSE_ROOT     0x1002  ///< Public Browse Root.
#define SERVICE_UUID_SERIAL_PORT            0x1101  ///< Serial Port.
#define SERVICE_UUID_OBEX_PUSH              0x1105  ///< OBEX Object Push.
#define SERVICE_UUID_HEADSET                0x1108  ///< Headset.
#define SERVICE_UUID_AUDIO_SOURCE           0x110A  ///< Audio Source.
#define SERVICE_UUID_AUDIO_SINK             0x110B  ///< Audio Sink.
#define SERVICE_UUID_REMOTE_CONTROL_TRG     0x110C  ///< A/V Remote Control Target.
#define SERVICE_UUID_REMOTE_CONTROL         0x110E  ///< A/V Remote Control.
#define SERVICE_UUID_REMOTE_CONTROL_CTL     0x110F  ///< A/V Remote Control Controller.
#define SERVICE_UUID_HEADSET_AG             0x1112  ///< Headset - Audio Gateway (AG).
#define SERVICE_UUID_HANDSFREE              0x111E  ///< Handsfree.
#define SERVICE_UUID_HANDSFREE_AG           0x111F  ///< Handsfree Audio Gateway.
#define SERVICE_UUID_HUMAN_INTERFACE_DEVICE 0x1124  ///< Human Interface Device Service.
#define SERVICE_UUID_SIM_ACCESS             0x112D  ///< SIM Access.
#define SERVICE_UUID_PB_ACCESS_CLIENT       0x112E  ///< Phonebook Access - PCE.
#define SERVICE_UUID_PB_ACCESS_SERVER       0x112F  ///< Phonebook Access - PSE.
#define SERVICE_UUID_HEADSET_HS             0x1131  ///< Headset - HS.
#define SERVICE_UUID_MESSAGE_ACCESS         0x1132  ///< Message Access Server.
#define SERVICE_UUID_MESSAGE_NOTIFICATION   0x1133  ///< Message Notification Service.
#define SERVICE_UUID_PNP_INFORMATION        0x1200  ///< PnP Information.
#define SERVICE_UUID_GENERIC_AUDIO          0x1203  ///< Generic Audio.
<<<<<<< HEAD
=======
#define SERVICE_UUID_GENERIC_ATTRIBUTE      0x1801  /* Generic Attribute */
>>>>>>> db20e11 (second commit)
/*!
    @}
*/


/*!
    @name SDP Profile Identifiers [Defines]
    SDP Profile UUID

    @{
*/
#define PROFILE_UUID_SPP                    0x1101  ///< Serial Port Profile (SPP).
#define PROFILE_UUID_OPP                    0x1105  ///< Object Push Profile (OPP).
#define PROFILE_UUID_FTP                    0x1106  ///< File Transfer Profile (FTP).
#define PROFILE_UUID_HSP                    0x1108  ///< Headset Profile (HSP).
#define PROFILE_UUID_A2DP                   0x110D  ///< Advanced Audio Distribution Profile (A2DP).
#define PROFILE_UUID_AVRCP                  0x110E  ///< Audio/Video Remote Control Profile (AVRCP).
#define PROFILE_UUID_HFP                    0x111E  ///< Hands-Free Profile (HFP).
#define PROFILE_UUID_HID                    0x1124  ///< Human Interface Device (HID).
#define PROFILE_UUID_SAP                    0x112D  ///< SIM Access Profile (SAP).
#define PROFILE_UUID_PBAP                   0x1130  ///< Phonebook Access Profile (PBAP).
#define PROFILE_UUID_MAP                    0x1134  ///< Message Access Profile (MAP).
#define PROFILE_UUID_PNP                    0x1200  ///< Device Identification Profile (DID).
/*!
    @}
*/


/*!
    @name SDP Attribute Identifiers [Defines]
    Attribute ID for all services

    @{
*/
#define ATTRID_ServiceRecordHandle          0x0000  ///< Service Record Handle.
#define ATTRID_ServiceClassIdList           0x0001  ///< Service Class ID List.
#define ATTRID_ServiceRecordState           0x0002  ///< Service Record State.
#define ATTRID_ServiceId                    0x0003  ///< Service ID.
#define ATTRID_ProtocolDescriptorList       0x0004  ///< Protocol Descriptor List.
#define ATTRID_BrowseGroupList              0x0005  ///< Browse Group List.
#define ATTRID_LangBaseAttributeIdList      0x0006  ///< Language Base Attribute ID List.
#define ATTRID_ServiceInfoTimeToLive        0x0007  ///< Service Info Time To Live.
#define ATTRID_ServiceAvailability          0x0008  ///< Service Availability.
#define ATTRID_ProfileDescriptorList        0x0009  ///< Bluetooth Profile Descriptor List.
#define ATTRID_DocumentationURL             0x000A  ///< Documentation URL.
#define ATTRID_ClientExecutableURL          0x000B  ///< Client Executable URL.
#define ATTRID_IconURL                      0x000C  ///< Icon URL.
#define ATTRID_AddProtocolDescriptorLists   0x000D  ///< Additional Protocol Descriptor Lists.
#define ATTRID_ServiceName                  0x0100  ///< Service Name.
#define ATTRID_ServiceDescription           0x0101  ///< Service Description.
#define ATTRID_ProviderName                 0x0102  ///< Provider Name.
#define ATTRID_SupportedFormatsList         0x0303  ///< Supported Formats List.
#define ATTRID_SupportedFeatures            0x0311  ///< Supported Features.
#define ATTRID_SupportedRepositories        0x0314  ///< Supported Repositories.
/*!
    @}
*/


/*!
    @name Service Discovery Service Attribute Identifiers [Defines]
    Attribute ID for Service Discovery Service

    @{
*/
#define ATTRID_VersionNumberList            0x0200  ///< Version Number List.
#define ATTRID_ServiceDatabaseState         0x0201  ///< Service Database State.
/*!
    @}
*/


/*!
    @name Browse Group Descriptor Service Attribute Identifiers [Defines]
    Attribute ID for Browse Group Descriptor Service

    @{
*/
#define ATTRID_GroupID                      0x0200  ///< Group ID.
/*!
    @}
*/


/*!
    @name Headset Profile Attribute Identifiers [Defines]
    Attribute ID for Headset Profile

    @{
*/
#define ATTRID_RemoteAudioVolControl        0x0302  ///< Remote Audio Volume Control.
/*!
    @}
*/


/*!
    @name Human Interface Device Profile Attribute Identifiers [Defines]
    Attribute ID for HID Profile

    @{
*/
#define ATTRID_DeviceReleaseNumber          0x0200  ///< HID Device Release Number
#define ATTRID_ParserVersion                0x0201  ///< HID Parser Version.
#define ATTRID_DeviceSubclass               0x0202  ///< HID Device Subclass.
#define ATTRID_CountryCode                  0x0203  ///< HID Country Code.
#define ATTRID_VirtualCable                 0x0204  ///< HID Virtual Cable.
#define ATTRID_ReconnectInitiate            0x0205  ///< HID Reconnect Initiate.
#define ATTRID_DescriptorList               0x0206  ///< HID Descriptor List.
#define ATTRID_LANGIDBaseList               0x0207  ///< HID LANGID Base List.
#define ATTRID_SDPDisable                   0x0208  ///< HID SDP Disable.
#define ATTRID_BatteryPower                 0x0209  ///< HID Battery Power.
#define ATTRID_RemoteWake                   0x020A  ///< HID Remote Wake.
#define ATTRID_ProfileVersion               0x020B  ///< HID Profile Version.
#define ATTRID_SupervisionTimeout           0x020C  ///< HID Supervision Timeout.
#define ATTRID_NormallyConnectable          0x020D  ///< HID Normally Connectable.
#define ATTRID_BootDevice                   0x020E  ///< HID Boot Device.
#define ATTRID_SSRHostMaxLatency            0x020F  ///< HID SSR Host Max Latency.
#define ATTRID_SSRHostMinTimeout            0x0210  ///< HID SSR Host Min Timeout.
/*!
    @}
*/


/*!
    @name Device Identification Profile Attribute Identifiers [Defines]
    Attribute ID for PNP Information

    @{
*/
#define ATTRID_SpecifcationID               0x0200  ///< Specification ID.
#define ATTRID_VendorID                     0x0201  ///< Vendor ID.
#define ATTRID_ProductID                    0x0202  ///< Product ID.
#define ATTRID_ProductVersion               0x0203  ///< Version.
#define ATTRID_PrimaryRecord                0x0204  ///< Primary Record.
#define ATTRID_VendorIDSource               0x0205  ///< Vendor ID Source.
/*!
    @}
*/


/*!
    @}
*/


/*!
    @addtogroup BT_LIB_SDP_DS
    @{
*/
//////////////////////////////////////////////////////////////////
//Attribute Data Element constant value
//////////////////////////////////////////////////////////////////

/*!
    @brief Data element types.
*/
enum
{
    DELEM_NIL,                              /*!< Nil, the null type. */
    DELEM_UINT,                             /*!< Unsigned Integer. */
    DELEM_SINT,                             /*!< Signed twos-complement integer. */
    DELEM_UUID,                             /*!< UUID, a universally unique identifier. */
    DELEM_TEXT,                             /*!< Text string. */
    DELEM_BOOLEAN,                          /*!< Boolean. */
    DELEM_SEQUENCE,                         /*!< Data element sequence. */
    DELEM_ALTERNATIVE,                      /*!< Data element alternative. */
    DELEM_URL                               /*!< URL, a uniform resource locator. */
};


/*!
    @brief Data element size.
*/
enum
{
    DELEM_SIZE_1,                           /*!< 1 byte. */
    DELEM_SIZE_2,                           /*!< 2 bytes. */
    DELEM_SIZE_4,                           /*!< 4 bytes. */
    DELEM_SIZE_8,                           /*!< 8 bytes. */
    DELEM_SIZE_16,                          /*!< 16 bytes. */
    DELEM_EXTRA_U8,                         /*!< Data size contain in additional 8 bits. */
    DELEM_EXTRA_U16,                        /*!< Data size contain in additional 16 bits. */
    DELEM_EXTRA_U32                         /*!< Data size contain in additional 32 bits. */
};
/*!
    @}
*/


/*!
     @addtogroup BT_LIB_SDP_MACRO
     @{

*/

/*!
    @name Data Element Header
    Header of data element, composed of two parts: a type descriptor and a size descriptor.

    @{
*/
#define DELEM_TYPE(x)           ((U8)(x) >> 3)                                  ///< Get data type from data element header.
#define DELEM_SIZE(x)           ((U8)(x) & 7)                                   ///< Get data Size from data element header.

#define DELEM_U8                ((DELEM_UINT << 3) | DELEM_SIZE_1)              ///< Type: Unsigned Integer, Size: 1 byte.
#define DELEM_U16               ((DELEM_UINT << 3) | DELEM_SIZE_2)              ///< Type: Unsigned Integer, Size: 2 bytes.
#define DELEM_U32               ((DELEM_UINT << 3) | DELEM_SIZE_4)              ///< Type: Unsigned Integer, Size: 4 bytes.
#define DELEM_U64               ((DELEM_UINT << 3) | DELEM_SIZE_8)              ///< Type: Unsigned Integer, Size: 8 bytes.
#define DELEM_U128              ((DELEM_UINT << 3) | DELEM_SIZE_16)              ///< Type: Unsigned Integer, Size: 16 bytes.

#define DELEM_UUID16            ((DELEM_UUID << 3) | DELEM_SIZE_2)              ///< Type: UUID, Size: 2 bytes.
#define DELEM_UUID32            ((DELEM_UUID << 3) | DELEM_SIZE_4)              ///< Type: UUID, Size: 4 bytes.
#define DELEM_UUID128           ((DELEM_UUID << 3) | DELEM_SIZE_16)             ///< Type: UUID, Size: 16 bytes.

#define DELEM_TEXT8             ((DELEM_TEXT << 3) | DELEM_EXTRA_U8)            ///< Type: Text, Size: 1 byte
#define DELEM_TEXT16            ((DELEM_TEXT << 3) | DELEM_EXTRA_U16)           ///< Type: Text, Size: 2 bytes.
#define DELEM_TEXT32            ((DELEM_TEXT << 3) | DELEM_EXTRA_U32)           ///< Type: Text, Size: 4 bytes.

#define DELEM_BOOL              ((DELEM_BOOLEAN << 3) | DELEM_SIZE_1)           ///< Type: Boolean, Size: 1 byte.

#define DELEM_SEQ8              ((DELEM_SEQUENCE << 3) | DELEM_EXTRA_U8)        ///< Type: Sequence, Size: 1 byte.
#define DELEM_SEQ16             ((DELEM_SEQUENCE << 3) | DELEM_EXTRA_U16)       ///< Type: Sequence, Size: 2 bytes.
#define DELEM_SEQ32             ((DELEM_SEQUENCE << 3) | DELEM_EXTRA_U32)       ///< Type: Sequence, Size: 4 bytes.

#define DELEM_URL8              ((DELEM_URL << 3) | DELEM_EXTRA_U8)             ///< Type: URL, Size: 1 byte.
#define DELEM_URL16             ((DELEM_URL << 3) | DELEM_EXTRA_U16)            ///< Type: URL, Size: 2 bytes.
/*!
    @}
*/



//////////////////////////////////////////////////////////////////
//Service Record
//////////////////////////////////////////////////////////////////

/*!
    @name Data Element
    Data representation, consists of two fields: a header field and a data field.

    @{
*/
#define GET_B0(value)           (U8)((value) & 0xFF)                            ///< Get Byte0 of U32.
#define GET_B1(value)           (U8)(((value) >> 8) & 0xFF)                     ///< Get Byte1 of U32.
#define GET_B2(value)           (U8)(((value) >> 16) & 0xFF)                    ///< Get Byte2 of U32.
#define GET_B3(value)           (U8)(((value) >> 24) & 0xFF)                    ///< Get Byte3 of U32.

#define SDP_U8(x)               GET_B0(x)                                       ///< SDP U8 data.
#define SDP_U16(x)              GET_B1(x), GET_B0(x)                            ///< SDP U16 data.
#define SDP_U32(x)              GET_B3(x), GET_B2(x), GET_B1(x), GET_B0(x)      ///< SDP U32 data.

#define DEDEF_U8(x)             DELEM_U8, SDP_U8(x)                             ///< Data element: U8.
#define DEDEF_U16(x)            DELEM_U16, SDP_U16(x)                           ///< Data element: U16.
#define DEDEF_U32(x)            DELEM_U32, SDP_U32(x)                           ///< Data element: U32.

#define DEDEF_UUID16(x)         DELEM_UUID16, SDP_U16(x)                        ///< Data element: UUID, 2 Bytes.

#define DEDEF_TEXT8(x)          DELEM_TEXT8, SDP_U8(x)                          ///< Data element: TEXT, 1 Byte.
#define DEDEF_TEXT16(x)         DELEM_TEXT16, SDP_U16(x)                        ///< Data element: TEXT, 2 Bytes.

#define DEDEF_BOOL(x)           DELEM_BOOL, SDP_U8(x)                           ///< Data element: Boolean, 1 Byte.

#define DEDEF_SEQ8(x)           DELEM_SEQ8, SDP_U8(x)                           ///< Data element: Sequence, 1 byte.
#define DEDEF_SEQ16(x)          DELEM_SEQ16, SDP_U16(x)                         ///< Data element: Sequence, 2 bytes.

#define DEDEF_URL8(x)           DELEM_URL8, SDP_U8(x)                           ///< Data element: URL, 1 byte.
#define DEDEF_URL16(x)          DELEM_URL16, SDP_U16(x)                         ///< Data element: URL, 2 byte.

#define SERVICE_HANDLE32(x)     DELEM_U32, SDP_U16(0x0001), SDP_U16(x)          ///< Data element: Service handle, 4 bytes.
#define SERVICE_UUID16(x)       DEDEF_UUID16(x)                                 ///< Data element: Service UUID, 2 bytes.
#define PROFILE_UUID16(x)       DEDEF_UUID16(x)                                 ///< Data element: Profile UUID, 2 bytes.
#define PROTOCOL_UUID16(x)      DEDEF_UUID16(x)                                 ///< Data element: Protocol UUID, 2 bytes.
#define PSM_U16(x)              DEDEF_U16(x)                                    ///< Data element: PSM, 2 bytes.
#define VERSION_U16(x)          DEDEF_U16(x)                                    ///< Data element: Version, 2 bytes.
/*!
    @}
*/

/*!
    @}
*/



/**************************************************************************************************/
/*                            Data Structures                                                     */
/**************************************************************************************************/
/*!
    @addtogroup BT_LIB_SDP_DS
    @{
        @brief SDP service record data element and SDP Messages data structures.
*/

/*!
    @brief SDP service attribute type.
*/
typedef struct
{
    U16 attr_id;                            /*!< Attribute ID. */
    U8 attr_len;                            /*!< Attribute length. */
    U8 *attr_val;                           /*!< Attribute data. */
} BT_SDP_ATTRIBUTE_T;

/*!
    @brief SDP server status.
*/
typedef enum
{
    SDS_STATUS_SUCCESS,                     /*!< Success. */
    SDS_STATUS_PENDING,                     /*!< Pending. */
    SDS_STATUS_UNSPECIFIED_ERROR,           /*!< Unspecified error. */
    SDS_STATUS_INVALID_SR_DATA_ERROR,       /*!< Invalid service record data error. */
    SDS_STATUS_INVALID_SR_HANDLE_ERROR,     /*!< Invalid service record handle error. */
    SDS_STATUS_INSUFFICIENT_RESOURCES,      /*!< Insufficient resources. */
    SDS_STATUS_UNKNOWN,                     /*!< Unknown status. */
} BT_SDS_STATUS_E;

/*!
    @brief Message payload of \ref BT_SDP_REGISTER_CFM.
*/
typedef struct
{
    BT_SDS_STATUS_E status;                 /*!< SDP service register status.*/
    U32 service_handle;                     /*!< Service handle.*/
} BT_SDP_REGISTER_CFM_T;

/*!
    @brief Message payload of \ref BT_SDP_UNREGISTER_CFM.
*/
typedef struct
{
    BT_SDS_STATUS_E status;                 /*!< SDP service unregister status.*/
    U32 service_handle;                     /*!< Service handle.*/
} BT_SDP_UNREGISTER_CFM_T;

/*!
    @brief SDP open search status.
*/
typedef enum
{
    SDP_OPEN_SEARCH_OK,                     /*!< Open search attempt was successful. */
    SDP_OPEN_SEARCH_FAILED,                 /*!< Open search attempt was failed. */
    SDP_OPEN_SEARCH_DISCONNECT,             /*!< Connection was closed by remote. */
    SDP_OPEN_SEARCH_OPEN,                   /*!< Search is already open. */
    SDP_OPEN_SEARCH_NO_RESOURCE,            /*!< No resource. */
    SDP_OPEN_SEARCH_ACL_LINK_ALREADY_EXIST,
    SDP_OPEN_SEARCH_PAGE_TIMEOUT,
	SDP_OPEN_SEARCH_CONNECTION_ACCEPT_TIMEOUT,
    SDP_OPEN_SEARCH_CONNECTION_DISALLOW,
<<<<<<< HEAD
=======
	SDP_OPEN_SEARCH_CONNECTION_LIMIT_RESOURCE,
>>>>>>> db20e11 (second commit)
    SDP_OPEN_SEARCH_STATUS_MAX,             /*!< Open search status max. */
} BT_SDP_OPEN_STATUS_E;

/*!
    @brief Message payload of \ref BT_SDP_OPEN_SEARCH_CFM.
*/
typedef struct
{
    BT_SDP_OPEN_STATUS_E status;            /*!< SDP  open search status.*/
    U8 bdAddr[6];                           /*!< Remote BT address.*/
} BT_SDP_OPEN_SEARCH_CFM_T;

/*!
    @brief SDP close search status.
*/
typedef enum
{
    SDP_CLOSE_SEARCH_OK,                    /*!< Close search attempt was successful. */
    SDP_CLOSE_SEARCH_FAILED,                /*!< Close search attempt was failed. */
    SDP_CLOSE_SEARCH_DISCONNECT,            /*!< Connection was closed by remote. */
    SDP_CLOSE_SEARCH_NOT_OPEN,              /*!< Search is not open. */
    SDP_CLOSE_SEARCH_BUSY,                  /*!< SDP Client is busy. */
    SDP_CLOSE_SEARCH_STATUS_MAX,            /*!< Close search status max. */
} BT_SDP_CLOSE_STATUS_E;

/*!
    @brief Message payload of \ref BT_SDP_CLOSE_SEARCH_CFM.
*/
typedef struct
{
    BT_SDP_CLOSE_STATUS_E status;           /*!< SDP close search status.*/
    U8 bdAddr[6];                           /*!< Remote BT address.*/
} BT_SDP_CLOSE_SEARCH_CFM_T;

/*!
    @brief SDP search status.
*/
typedef enum
{
    SDP_RESPONSE_SUCCESS,                   /*!< Search was successful .*/
    SDP_ERROR_RESPONSE_PDU,                 /*!< Error response PDU. */
    SDP_NO_RESPONSE_DATA,                   /*!< No response data. */
    SDP_CON_DISCONNECTED,                   /*!< Connection disconnected. */
    SDP_CONNECTION_ERROR,                   /*!< Connection error. */
    SDP_CONFIGURE_ERROR,                    /*!< Configuration error. */
    SDP_SEARCH_DATA_ERROR,                  /*!< Search data error. */
    SDP_DATA_CFM_ERROR,                     /*!< Data confirmation error. */
    SDP_SEARCH_BUSY,                        /*!< Search busy. */
    SDP_RESPONSE_PDU_HEADER_ERROR,          /*!< Invalid PDU header. */
    SDP_RESPONSE_PDU_SIZE_ERROR,            /*!< Invalid PDU size. */
    SDP_RESPONSE_TIMEOUT_ERROR,             /*!< Search timeout error. */
    SDP_SEARCH_SIZE_TOO_BIG,                /*!< Size too big error. */
    SDP_RESPONSE_OUT_OF_MEMORY,             /*!< Out of memory error. */
    SDP_RESPONSE_TERMINATED,                /*!< Response terminated. */
    SDP_SEARCH_STATUS_MAX,                  /*!< Search status max. */
} BT_SDP_SEARCH_STATUS_E;

/*!
    @brief Message payload of \ref BT_SDP_SERVICE_SEARCH_CFM.
*/
typedef struct
{
    BT_SDP_SEARCH_STATUS_E status;          /*!< SDP search status. */
    U8 bdAddr[6];                           /*!< Remote BT address.*/
    U16 error_code;                         /*!< Response error code. */
    U16 num_records;                        /*!< Number of service record. */
    U8 records[1];                          /*!< Service record handle list. */
} BT_SDP_SERVICE_SEARCH_CFM_T;

/*!
    @brief Message payload of \ref BT_SDP_ATTRIBUTE_SEARCH_CFM.
*/
typedef struct
{
    BT_SDP_SEARCH_STATUS_E status;          /*!< SDP search status.*/
    U8 bdAddr[6];                           /*!< Remote BT address.*/
    U16 error_code;                         /*!< Response error code.*/
    U16 size_attributes;                    /*!< Size of the attribute list.*/
    U8 more_data;                           /*!< More data to come.*/
    U8 attributes[1];                       /*!< Attribute list.*/
} BT_SDP_ATTRIBUTE_SEARCH_CFM_T;

/*!
    @brief Message payload of \ref BT_SDP_SERVICE_SEARCH_ATTRIBUTE_CFM.
*/
typedef struct
{
    BT_SDP_SEARCH_STATUS_E status;          /*!< SDP search status.*/
    U8 bdAddr[6];                           /*!< Remote BT address.*/
    U16 error_code;                         /*!< Response error code.*/
    U16 size_attributes;                    /*!< Size of the attribute list.*/
    U8 more_data;                           /*!< More data to come.*/
    U8 attributes[1];                       /*!< Attribute list.*/
} BT_SDP_SERVICE_SEARCH_ATTRIBUTE_CFM_T;


/*!
    @}
*/

typedef struct
{
    U8 bdAddr[6];
    U8 status;
}BT_SDP_SERVER_CONNECT_IND_T,BT_SDP_SERVER_DISCONNECT_IND_T;


typedef enum
{
    SDP_SERVER_DISCON_SINGLE,
    SDP_SERVER_DISCON_ALL,
} BT_SDP_SERVER_DISCON_OPTION;


/**************************************************************************************************/
/*                                          Functions                                             */
/**************************************************************************************************/

/*!
    @addtogroup BT_LIB_SDP_FN
    @{
        @brief SDP functions for server or client.
*/


/*!
    @brief Register service record to SDP server.

    @param app The server handler. Message \ref BT_SDP_REGISTER_CFM will be sent to it, after operation completed.
    @param service_record_size The size of the service record. Number of attributes in the service record.
    @param service_record The service record to register.
*/
void BtSdpRegisterServiceRecord(Handler app, U16 service_record_size, const BT_SDP_ATTRIBUTE_T *service_record);


/*!
    @brief Unregister service record from SDP server.

    @param app The server handler. Message \ref BT_SDP_UNREGISTER_CFM will be sent to it, after operation completed.
    @param service_record_hdl The handle of the service record to unregister.
*/
void BtSdpUnregisterServiceRecord(Handler app, U32 service_record_hdl);


/*!
    @brief Configure SDP server MTU size.

    @param mtu The MTU size to use in SDP server.
*/
void BtSdpConfigServerMtu(U16 mtu);


/*!
    @brief Configure SDP client MTU size.

    @param mtu The MTU size to use in SDP client.
*/
void BtSdpConfigClientMtu(U16 mtu);


/*!
    @brief Open a search to the remote device.

    @param app The client handler. Message \ref BT_SDP_OPEN_SEARCH_CFM will be sent to it, after operation completed.
    @param bdaddr Remote's BT MAC address.
*/
void BtSdpOpenSearchRequest(Handler app, U8 bdAddr[6]);


/*!
    @brief Close the existing search.

    @param app The client handler. Message \ref BT_SDP_CLOSE_SEARCH_CFM will be sent to it, after operation completed.
    @param bdaddr Remote's BT MAC address.
*/
void BtSdpCloseSearchRequest(Handler app, U8 bdAddr[6]);


/*!
    @brief Search service records match the search pattern.

    @param app The client handler. Message \ref BT_SDP_SERVICE_SEARCH_CFM will be sent to it, after operation completed.
    @param bdaddr Remote's BT MAC address.
    @param uuid The service search pattern uuid.
*/
void BtSdpServiceSearchRequest(Handler app, U8 bdAddr[6], BT_UUID_T uuid);

/*!
    @brief Retrieve specified attribute values from a specific service record.

    @param app The client handler. Message \ref BT_SDP_ATTRIBUTE_SEARCH_CFM will be sent to it, after operation completed.
    @param bdaddr Remote's BT MAC address.
    @param service_record_hdl The handle of service record from which attribute values to retrieve.
    @param start_att_id The attribute ID to start search.
    @param end_att_id The attribute ID to end search. Set equal to start_att_id if use only an attribute ID.
*/
void BtSdpAttributeSearchRequest(Handler app, U8 bdAddr[6], U32 service_record_hdl, U16 start_att_id, U16 end_att_id);

/*!
    @brief Retrieved service records that match the service search pattern.

    @param app The client handler. Message \ref BT_SDP_SERVICE_SEARCH_ATTRIBUTE_CFM will be sent to it, after operation completed.
    @param bdaddr Remote's BT MAC address.
    @param uuid The service search pattern uuid.
    @param start_att_id The attribute ID to start search.
    @param end_att_id The attribute ID to end search. Set equal to start_att_id if use only an attribute ID.
*/
void BtSdpServiceSearchAttributeRequest(Handler app, U8 bdAddr[6], BT_UUID_T uuid, U16 start_att_id, U16 end_att_id);

/*!
    @}
*/


void BtSdpServerRegisterNotifyHandler(Handler app);

void BtSdpServerDisconnectReq(U8 bdAddr[6],BT_SDP_SERVER_DISCON_OPTION option);

U8 BtSdpGetDataElementSize(U8 dataElementHeader);

<<<<<<< HEAD
=======
BOOL BtSdpGetRfcommChannel(U8 *ptr, U8 *pChannel);

>>>>>>> db20e11 (second commit)
/* for aws RHO*/
BOOL BtSdpIsRhoAllow(void);
U16 BtSdpGetRhoDataLength(void);
BOOL BtSdpGetRhoData(U8 *pBuffer);
BOOL BtSdpApplyAgentRhoData(BD_ADDR_T *pBdAddr, U8 *pSdpRhoData, U16 length);
BOOL BtSdpApplyPartnerRhoData(BD_ADDR_T *pBdAddr, U8 *pSdpRhoData, U16 length);


#endif  //_BT_SDP_HH__
