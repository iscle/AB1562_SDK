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

#ifndef _BTIAP2_H_
#define _BTIAP2_H_

/**************************************************************************************************
*Enum
**************************************************************************************************/
/**
 *  @brief Define for IAP2 event to callback which is registered in BtIAP2StartService.
 */
typedef enum
{
    BT_IAP2_START_SERVICE_CFM,          // this will be sent after BtIAP2StartService is done.  
	BT_IAP2_STOP_SERVICE_CFM,           // this will be sent after BtIAP2StopService is done.  
    BT_IAP2_CONNECT_CFM,                // this will be sent when rfcomm session is connected.
    BT_IAP2_DISCONNECT_CFM,             // this will be sent when rfcomm session is disconnected actively.
    BT_IAP2_DISCONNECT_IND,             // this will be sent when device gets rfcomm session disconnection request.
    BT_IAP2_RX_DATA_READY_IND,          // this will be sent when receiving data.
	BT_IAP2_SESSION_OPEN_IND,           // this will be sent when iap2 session is open.
	BT_IAP2_SESSION_CLOSE_IND,          // this will be sent when iap2 session is closed.
	BT_IAP2_NOTIFY_IAP2_CONNECT_IND,    // this will be sent when iap2 control session is open.
}BT_IAP2_EVENT;	

/**
 *  @brief Define for IAP2 status.
 */
typedef enum
{
     BT_IAP2_STATUS_OK = 0,             // ststus ok
     BT_IAP2_STATUS_ERROR,              // ststus error
     BT_IAP2_STATUS_OUT_OF_MEMORY,      // ststus out of memory
}BT_IAP2_STATUS;


/**
 *  @brief Define for IAP2 Session type.
 */
#define IAP2_SESSION_TYPE_CONTROL                (0)       /**< Control session. */
#define IAP2_SESSION_TYPE_FILE_TRANSFER          (1)       /**< Open state. */
#define IAP2_SESSION_TYPE_EXTERNAL_ACCESSORY     (2)       /**< Streaming state. */

/**************************************************************************************************
*Structure
**************************************************************************************************/

typedef struct
{
    U8 status;              /**<BT_IAP2_STATUS  */
} BT_IAP2_START_SERVICE_CFM_T;

typedef struct
{
    U8 status;              /**<BT_IAP2_STATUS  */
}BT_IAP2_STOP_SERVICE_CFM_T;

typedef struct
{
	BD_ADDR_T bdAddr;
    U8 status;              /**<BT_IAP2_STATUS  */
}BT_IAP2_CONNECT_CFM_T;

typedef struct
{
	BD_ADDR_T bdAddr;
    U8 status;              /**<BT_IAP2_STATUS  */
}BT_IAP2_DISCONNECT_CFM_T;

typedef struct
{
	BD_ADDR_T bdAddr;
    U8 status;              /**<BT_IAP2_STATUS  */
}BT_IAP2_DISCONNECT_IND_T;

typedef struct
{
	BD_ADDR_T bdAddr;
    U16 length;             /**< The length of the received packet.*/
    U8  session_t;          /**<  IAP2_SESSION_TYPE_CONTROL or IAP2_SESSION_TYPE_EXTERNAL_ACCESSORY  */
    U16 session_id;         /**<  it is only for EA Session, session_t == IAP2_SESSION_TYPE_CONTROL */
    U16 message_id;         /**<  it is only for Control Session, session_t == IAP2_SESSION_TYPE_EXTERNAL_ACCESSORY */
	U8  data[1];            /**< The packet is received from a remote device.*/
}BT_IAP2_RX_DATA_READY_IND_T;

typedef struct
{
	BD_ADDR_T bdAddr;
	U16 session_id;         /**< session id in iap2 .*/
	U16 protocol_id;        /**< protocol id in iap2 .*/
	U16 max_packet_length;  /**< max packet length in iap2 .*/
}BT_IAP2_SESSION_OPEN_IND_T;

typedef struct
{
	BD_ADDR_T bdAddr;
	U16 session_id;         /**< session id in iap2 .*/
	U16 protocol_id;        /**< protocol id in iap2 .*/
}BT_IAP2_SESSION_CLOSE_IND_T;

typedef struct
{
    BD_ADDR_T bdAddr;
	U8 status;              /**<BT_IAP2_STATUS  */
}BT_IAP2_NOTIFY_IAP2_CONNECT_IND_T;

/**************************************************************************************************
* API (for RHO)
**************************************************************************************************/
BOOL BtIAP2Lock();
BOOL BtIAP2Unlock();
BOOL BtIAP2IsAllow();
U16  BtIAP2GetRhoDataLength();
BOOL BtIAP2GetRhoData(U8 *pBuffer);
BOOL BtIAP2ApplyAgentRhoData(BD_ADDR_T *pBdAddr, U8 *pAwsMceData, U16 length);
BOOL BtIAP2ApplyPartnerRhoData(BD_ADDR_T *pBdAddr, U8 *pAwsMceData, U16 length);


/**************************************************************************************************
* API (for Service)
**************************************************************************************************/
/**
 *  @brief This function starts the service in iap2.
 *  <input>
 *  Handler app : callback for BT_IAP2_EVENT
 *  U8 *pName   : string for iap2 to open session.
 *  U8 length   : length of string [pName]
 *  U8 isMatch  : "App Match" feature. please see "App Match" section in iap2 specification document
 *  <output>
 *  U8          : return BT_IAP2_STATUS; 
 */
U8 	 BtIAP2StartService(Handler app, U8 *pName, U8 length, U8 isMatch);

/**
 *  @brief This function stops the service in iap2.
 *  <input>
 *  Handler app : callback for BT_IAP2_EVENT
 */
void BtIAP2StopService(Handler app);

/**
 *  @brief This function transmit data over iap2.
 *  <input>
 *  BD_ADDR_T *pBdAddr  : EDR bluetooth address of device.
 *  U16 session_id      : session_id which is return in BT_IAP2_SESSION_OPEN_IND_T.
 *  U8 *data            : transmitted data address.
 *  U16 length          : transmitted data length.
 */
U8   BtIAP2SendData(BD_ADDR_T *pBdAddr, U16 session_id, U8 *data, U16 length);
void BtIAP2NVKeyInit(void);
U8 *BtIAP2GetSerialNumber(void);
void BtIAP2GetSerialNumData(U8 *ptr, U16 serialNumLength);
U16 BtIAP2GetSerialNumLength(void);
U8 BtIAP2SendAppLaunchRequest(BD_ADDR_T *pBdAddr, uint8_t *app_id, bool is_alert);



#endif	/* _BTIAP2_H_ */

