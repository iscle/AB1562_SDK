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


#ifndef _BT_ATT_H_
#define _BT_ATT_H_

/**************************************************************************************************/
/*                            Macros                                                              */
/**************************************************************************************************/
/*!
    @addtogroup
    @{
*/
/*!
	 @brief ATT error code
 */
#define BT_ATT_NOERR 0x00
#define BT_ATT_ERR_INVALID_HANDLE 0x01
#define BT_ATT_ERR_READ_NOT_PERMITTED 0x02
#define BT_ATT_ERR_WRITE_NOT_PERMITTED 0x03
#define BT_ATT_ERR_INVALID_PDU 0x04
#define BT_ATT_ERR_INSUFFICIENT_AUTHEN 0x05
#define BT_ATT_ERR_UNSUPPORTED_REQ 0x06
#define BT_ATT_ERR_INVALID_OFFSET 0x07
#define BT_ATT_ERR_INSUFFICIENT_AUTHOR 0x08
#define BT_ATT_ERR_PREPARE_QUEUE_FULL 0x09
#define BT_ATT_ERR_ATTR_NOT_FOUND 0x0A
#define BT_ATT_ERR_ATTR_NOT_LONG 0x0B
#define BT_ATT_ERR_INSUFFICIENT_KEY_SIZE 0x0C
#define BT_ATT_ERR_INVALID_VALUE_LENGTH 0x0D
#define BT_ATT_ERR_UNLIKELY 0x0E
#define BT_ATT_ERR_INSUFFICIENT_ENCRYPT 0x0F
#define BT_ATT_ERR_UNSUPPORTED_GRP_TYPE 0x10
#define BT_ATT_ERR_INSUFFICIENT_RESOURCES 0x11

#define BT_ATT_RP_WP (ATT_R_PERMIT | ATT_W_PERMIT)
#define BT_ATT_RP_WN (ATT_R_PERMIT | ATT_W_AUTHEN)
#define BT_ATT_RP_WO (ATT_R_PERMIT | ATT_W_AUTHOR)
#define BT_ATT_RP_WE (ATT_R_PERMIT | ATT_W_ENCRYPT)
#define BT_ATT_RP_WB (ATT_R_PERMIT | ATT_W_BANNED)

#define BT_ATT_RN_WP (ATT_R_AUTHEN | ATT_W_PERMIT)
#define BT_ATT_RN_WN (ATT_R_AUTHEN | ATT_W_AUTHEN)
#define BT_ATT_RN_WO (ATT_R_AUTHEN | ATT_W_AUTHOR)
#define BT_ATT_RN_WE (ATT_R_AUTHEN | ATT_W_ENCRYPT)
#define BT_ATT_RN_WB (ATT_R_AUTHEN | ATT_W_BANNED)

#define BT_ATT_RO_WP (ATT_R_AUTHOR | ATT_W_PERMIT)
#define BT_ATT_RO_WN (ATT_R_AUTHOR | ATT_W_AUTHEN)
#define BT_ATT_RO_WO (ATT_R_AUTHOR | ATT_W_AUTHOR)
#define BT_ATT_RO_WE (ATT_R_AUTHOR | ATT_W_ENCRYPT)
#define BT_ATT_RO_WB (ATT_R_AUTHOR | ATT_W_BANNED)

#define BT_ATT_RE_WP (ATT_R_ENCRYPT | ATT_W_PERMIT)
#define BT_ATT_RE_WN (ATT_R_ENCRYPT | ATT_W_AUTHEN)
#define BT_ATT_RE_WO (ATT_R_ENCRYPT | ATT_W_AUTHOR)
#define BT_ATT_RE_WE (ATT_R_ENCRYPT | ATT_W_ENCRYPT)
#define BT_ATT_RE_WB (ATT_R_ENCRYPT | ATT_W_BANNED)


#define BT_ATT_RB_WP (ATT_R_BANNED | ATT_W_PERMIT)
#define BT_ATT_RB_WN (ATT_R_BANNED | ATT_W_AUTHEN)
#define BT_ATT_RB_WO (ATT_R_BANNED | ATT_W_AUTHOR)
#define BT_ATT_RB_WE (ATT_R_BANNED | ATT_W_ENCRYPT)
#define BT_ATT_RB_WB (ATT_R_BANNED | ATT_W_BANNED)


/*!
    @}
*/
/**************************************************************************************************/
/*                            Data Structures                                                     */
/**************************************************************************************************/

/*!

     @addtogroup BT_LIB_GATT_DS
     @{

*/

/*!
     @brief attribute handle.
 */
typedef U16 BT_ATT_HANDLE;


/*!
    @brief UUID Type in bytes.
*/
typedef U8 BT_UUID_TYPE;

enum UUID_TYPE_E
{
    UUID_TYPE_16            = (BT_UUID_TYPE)2,          /**< 2 bytes*/
    UUID_TYPE_32            = (BT_UUID_TYPE)4,          /**< 4 bytes*/
    UUID_TYPE_128           = (BT_UUID_TYPE)16,         /**<16 bytes*/
};


/*!
    @brief GATT service type.
*/
typedef U16 BT_SERVICE_TYPE;

enum SERVICE_TYPE_E
{
    PRIMARY_SERVICE         = (BT_SERVICE_TYPE)0x2800,  /**<0x2800*/
    SECONDARY_SERVICE       = (BT_SERVICE_TYPE)0x2801,  /**<0x2801*/
    INCLUDE_SERVICE         = (BT_SERVICE_TYPE)0x2802,  /**<0x2802*/
};

/*!
    @brief attribute permissions.
*/
typedef U8 BT_ATT_PERMIT;

enum {
    ATT_R_PERMIT            = (BT_ATT_PERMIT)0x00,      /* Always permitted, no restrictions*/
    ATT_R_AUTHEN            = (BT_ATT_PERMIT)0x01,      /* Authentication required */
    ATT_R_AUTHOR            = (BT_ATT_PERMIT)0x02,      /* Authorization required */
    ATT_R_ENCRYPT           = (BT_ATT_PERMIT)0x04,      /* Can only be accessed in encrypted link*/
    ATT_R_BANNED            = (BT_ATT_PERMIT)0x08,      /* Operation not permitted */

    ATT_W_PERMIT            = (BT_ATT_PERMIT)0x00,      /* Always permitted, no restrictions*/
    ATT_W_AUTHEN            = (BT_ATT_PERMIT)0x10,      /* Authentication required */
    ATT_W_AUTHOR            = (BT_ATT_PERMIT)0x20,      /* Authorization required */
    ATT_W_ENCRYPT           = (BT_ATT_PERMIT)0x40,      /* Can only be accessed in encrypted link*/
    ATT_W_BANNED            = (BT_ATT_PERMIT)0x80,      /* Operation not permitted */
};

/*!
     @brief characteristic Property.
 */
typedef U8 BT_CHAR_PROPERTY;

enum CHAR_PROPERTY_E
{
    C_BCAST                 = (BT_CHAR_PROPERTY)0x01,
    C_READ                  = (BT_CHAR_PROPERTY)0x02,
    C_WRITE_NO_RSP          = (BT_CHAR_PROPERTY)0x04,
    C_WRITE                 = (BT_CHAR_PROPERTY)0x08,
    C_NOTIFY                = (BT_CHAR_PROPERTY)0x10,
    C_INDICATE              = (BT_CHAR_PROPERTY)0x20,
    C_AUTHEN                = (BT_CHAR_PROPERTY)0x40,
    C_EXTENDED              = (BT_CHAR_PROPERTY)0x80,
};

typedef U8 BT_ATT_ACCESS_TYPE;

enum
{
    ATT_ACCESS_READ         = (BT_ATT_ACCESS_TYPE)0,
    ATT_ACCESS_WRITE        = (BT_ATT_ACCESS_TYPE)1,
    ATT_ACCESS_WRITE_CMD    = (BT_ATT_ACCESS_TYPE)2,    /* without response */
};

typedef enum {
    ATT_LINK_CLOSED,
    ATT_LINK_CONNECTED,
} BT_ATT_LINK_STATUS_E;


typedef struct BT_ATT_LINK_STATUS_IND_T
{
    U8 type;
    U8 bdAddr[6]; /*! The Bluetooth address.*/
    BOOL isBle;
    BT_ATT_LINK_STATUS_E status;
} BT_ATT_LINK_STATUS_IND_T;

typedef struct BT_ATT_ACCESS_IND_T
{
    U8 bdAddr[6]; /*! The Bluetooth address.*/
    BT_ATT_ACCESS_TYPE flags;
    U16 handle;
    U16 offset;
    U16 valueLen;
    U8 value[1];
} BT_ATT_ACCESS_IND_T;

typedef struct BT_ATT_NOTIFICATION_CFM_T
{
    U8 bdAddr[6]; /*! The Bluetooth address.*/
    U16 handle;
    U8 status;
} BT_ATT_NOTIFICATION_CFM_T, BT_ATT_INDICATION_CFM_T;

typedef struct BT_ATT_TX_AVAILABLE_IND_T
{
    U8 bdAddr[6]; /*! The Bluetooth address.*/
} BT_ATT_TX_AVAILABLE_IND_T;

typedef struct BT_GATT_EXCHANGE_MTU_IND_T
{
    U8 bdAddr[6]; /*! The Bluetooth address.*/
    U16 peerMtu;
} BT_GATT_EXCHANGE_MTU_IND_T;

typedef struct BT_GATT_TIMEOUT_IND_T
{
    U8 bdAddr[6]; /*! The Bluetooth address.*/
} BT_GATT_TIMEOUT_IND_T;

typedef struct BT_GATT_START_SERVER_CFM_T
{
    BT_GENERIC_STATUS_E status;
} BT_GATT_START_SERVER_CFM_T, BT_GATT_STOP_SERVER_CFM_T;

/*!
     @brief API parameter for creating a GATT service.
 */
typedef struct
{
    BT_SERVICE_TYPE     serviceType;                /**< service type*/
    BT_UUID_TYPE        uuidType;                   /**< service UUID Type*/
    U8                  uuid[16];                   /**< service UUID*/
    U8                  registerLinkStatusEvt:1;    /**< service register connection state event*/
    U8                  Rsvd:7;
} BT_GATT_ADD_SERVICE_T;

/*!
     @brief API parameter for including a GATT service in another.
 */
typedef struct
{
    BT_ATT_HANDLE       service_handle;         /**< service declaration attribute handle*/
} BT_GATT_ADD_INCLUDE_T;

/*!
     @brief API parameter for creating a GATT characteristic.
 */
typedef struct
{
    BT_UUID_TYPE        uuidType;               /**< characteristic UUID Type*/
    U8                  uuid[16];               /**< characteristic UUID*/
    BT_CHAR_PROPERTY    property;               /**< characteristic property @ref CHAR_PROPERTY */
    BT_ATT_PERMIT       permission;             /**< characteristic value attribute access permission */
    U16                 data_initial_length;    /**< characteristic value attribute default length */
    U16                 data_maximum_length;    /**< characteristic value attribute maximum length, 0 indicates its a fixed length attribute */
    U8                  *char_data;             /**< characteristic value data pointer */
} BT_GATT_ADD_CHARACTERISTIC_T;

/*!
     @brief API parameter for creating a GATT characteristic descriptor.
 */
typedef struct
{
    BT_UUID_TYPE        uuidType;               /**< descriptor UUID Type*/
    U8                  uuid[16];               /**< descriptor UUID*/
    BT_ATT_PERMIT       permission;             /**< descriptor attribute access permission */
    U16                 data_initial_length;    /**< descriptor attribute default length */
    U16                 data_maximum_length;    /**< descriptor attribute maximum length, 0 indicates its a fixed length attribute */
    U8                  *desc_data;             /**< descriptor data pointer */
} BT_GATT_ADD_DESCRIPTOR_T;

/*!
    @}
*/
/**************************************************************************************************/
/*                            Functions                                                           */
/**************************************************************************************************/
/*!
    @addtogroup BT_LIB_GATT_FN
    @{
*/

/*!
     @brief API for adding a GATT Service declaration into the attribute server database.

     @param para @ref BT_GATT_ADD_SERVICE_T
     @return The attribute handle of service declaration within the attribute server database.

     @attention Service declaration should always be the first attribute in the database.
 */
BT_ATT_HANDLE BtGattAddService(Handler handler, const BT_GATT_ADD_SERVICE_T *para);

/*!
     @brief API for including another GATT Service in last added service.

     @param para @ref BT_GATT_ADD_INCLUDE_T
     @return The attribute handle of include declaration within the attribute server database.

     @attention The included service should have smaller attribute handle to avoid circular including.
 */
BT_ATT_HANDLE BtGattAddInclude(const BT_GATT_ADD_INCLUDE_T *para);

/*!
     @brief API for declaring a GATT Characteristic in last added service.

     @param para @ref BT_GATT_ADD_CHARACTERISTIC_T
     @return The attribute handle of characteristic value within the attribute server database.
 */
BT_ATT_HANDLE BtGattAddCharacteristic(const BT_GATT_ADD_CHARACTERISTIC_T *para);

/*!
     @brief API for declaring a GATT descriptor in last added characteristic.

     @param para @ref BT_GATT_ADD_DESCRIPTOR_T
     @return The attribute handle of descriptor within the attribute server database.
 */
BT_ATT_HANDLE BtGattAddDescriptor(const BT_GATT_ADD_DESCRIPTOR_T *para);

U16 BtGattGetAttributeLength(BT_ATT_HANDLE attHandle);

BOOL BtGattUpdateAttributeLength(BT_ATT_HANDLE attHandle, U16 length);

/* ATT */
void BtAttAccessResponse(U8 bdAddr[6], U8 result, U16 handle, U16 valueLen, U8 *valuePtr);
void BtAttNotificationRequest(U8 bdAddr[6], U16 handle, U16 valueLen, U8 *valuePtr);
void BtAttIndicationRequest(U8 bdAddr[6], U16 handle, U16 valueLen, U8 *valuePtr);
<<<<<<< HEAD
=======
extern void ATT_Enable_EDRBrearer(void);
>>>>>>> db20e11 (second commit)

/*GATT*/
void BtGattStartServer(Handler app);
void BtGattStopServer(Handler app);
void BtGattExchangeMtuResponse(U8 bdAddr[6], U16 mtu);

/*!
    @}
*/

#endif
