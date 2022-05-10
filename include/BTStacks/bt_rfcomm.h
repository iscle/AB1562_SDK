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

#ifndef __BT_RFCOMM_HH
#define __BT_RFCOMM_HH

/*!
    @addtogroup BT_LIB_RFCOMM
    @{
         \image html rfcomm_msc.png "Figure 1: RFCOMM message sequence chart."

        ##RFCOMM

        RFCOMM APIs provide profiles or applications way to exchange data through RFCOMM channels with remote devices.

        ###Create Channel
        If application/profile knows the remote channel number(by SDP), it could use \ref BtRfcommConnectRequest to establish
        channel connection with specific remote devices. After complete, \ref BT_RFCOMM_CLIENT_CONNECT_CFM will be sent to corresponding
        handler. As to server side, if connection is incoming, application/profile handler will received \ref BT_RFCOMM_CONNECT_IND
        message and then send the \ref BtRfcommConnectResponse. Figure 1 shows the sequence chart.

        ###Data Exchange
        On connection complete, the \ref BT_RFCOMM_SERVER_CONNECT_CFM_T(or \ref BT_RFCOMM_CLIENT_CONNECT_CFM_T) will provide the sink instance. Application could use
        this instance to send or received data manually or automatically(\ref StreamConnect). Figure 1 shows the sequence chart.

        ###Destroy Channel
        Once channel was established, application/profile could use \ref BtRfcommDisconnectRequest to disconnect. After complete,
        BT_RFCOMM_DISCONNECT_CFM will be sent to corresponding handler. As to server, if channel disconnection was requesting by
        remote, it will receive \ref BT_RFCOMM_DISCONNECT_IND, then use \ref BtRfcommDisconnectResponse to finish the disconnection. Figure 1
        shows the sequence chart.

    @}
*/
/**************************************************************************************************/
/*                            Macros                                                              */
/**************************************************************************************************/
/*!
    @addtogroup BT_LIB_RFCOMM_MACRO
    @{
*/
/*!
    @}
*/
/**************************************************************************************************/
/*                            Data Structures                                                     */
/**************************************************************************************************/
/*!
    @addtogroup BT_LIB_RFCOMM_DS
    @{
*/
/*!
    @brief Message payload of \ref BT_RFCOMM_REGISTER_CFM.
*/
typedef struct
{
    BT_GENERIC_STATUS_E status; /*!< Status. */
    U8 channel;                 /*!< Listening RFCOMM channel. Range is 1 to 30.*/
} BT_RFCOMM_REGISTER_CFM_T;

/*!
    @brief Message payload of \ref BT_RFCOMM_UNREGISTER_CFM.
*/
typedef struct
{
    BT_GENERIC_STATUS_E status; /*!< Status. */
	U8 channel;
} BT_RFCOMM_UNREGISTER_CFM_T;

/*!
    @brief Message payload of \ref BT_RFCOMM_CLIENT_CONNECT_CFM.
*/
typedef struct
{
	BD_ADDR_T bdAddr;
    BT_GENERIC_STATUS_E status; /*!< Status.*/
    U8 client_channel;          /*!< RFCOMM channel.*/
    SINK sink;                  /*!< Sink object.*/
    U32 serial_id;              /*!< Internal used field.*/
} BT_RFCOMM_CLIENT_CONNECT_CFM_T;

/*!
    @brief Message payload of \ref BT_RFCOMM_SERVER_CONNECT_CFM.
*/
typedef struct
{
	BD_ADDR_T bdAddr;
    BT_GENERIC_STATUS_E status; /*!< Status*/
    U8 server_channel;          /*!< RFCOMM channel.*/
    SINK sink;                  /*!< Sink object*/
} BT_RFCOMM_SERVER_CONNECT_CFM_T;

/*!
    @brief Enumerations for line_status parameter of \ref BtRfcommRemoteLineStatueRequest.
*/
typedef enum {
    RFCOMM_LINE_STATUS_NO_ERROR = 0x0,      /*!< No error.*/
    RFCOMM_LINE_STATUS_FRAMING_ERROR = 0x3, /*!< A character did not terminate with a stop bit.*/
    RFCOMM_LINE_STATUS_PARITY_ERROR = 0x5,  /*!< Received character?s parity was incorrect.*/
    RFCOMM_LINE_STATUS_OVERRUN_ERROR = 0x9, /*!< Received character overwrote an unread character.*/
} RFCOMM_LINE_STATUS_E;

/*!
    @brief Message payload of \ref BT_RFCOMM_DISCONNECT_CFM.
*/
typedef struct
{
    BT_GENERIC_STATUS_E status;/*!< Status.*/
    SINK sink;                 /*!< Sink object.*/
} BT_RFCOMM_DISCONNECT_CFM_T;

/*!
    @brief Message payload of \ref BT_RFCOMM_DISCONNECT_IND.
*/
typedef struct
{
    BT_GENERIC_STATUS_E status;/*!< Status.*/
    SINK sink;                 /*!< Sink object.*/
} BT_RFCOMM_DISCONNECT_IND_T;

/*!
    @brief Message payload of \ref BT_RFCOMM_CONNECT_IND.
*/
typedef struct
{
    U8 bd_addr[6];      /*!< Remote BT address.*/
    U8 server_channel;  /*!< Local RFCOMM channel*/
} BT_RFCOMM_CONNECT_IND_T;

/*!
    @brief Data structure for config parameter of \ref BtRfcommConnectRequest and \ref BtRfcommConnectResponse.

    Used to change the preference of RFCOMM channel.
*/
typedef struct
{
    U32 msc_timeout_ms; /*!< Timeout value in milliseconds of modem status command.*/
    U32 max_frame_size; /*!< Maximal RFCOMM frame size in bytes*/
} BT_RFCOMM_CONFIG_T;

/*!
    @}
*/
/**************************************************************************************************/
/*                            Functions                                                           */
/**************************************************************************************************/
/*!
    @addtogroup BT_LIB_RFCOMM_FN
    @{
*/
/*!
    @brief Register a channel to RFCOMM server.

    @param handler The server handler. Message \ref BT_RFCOMM_REGISTER_CFM will be sent to it, after operation completed.
    @param channel The local channel which allowed to be established further.
    @param max_frame_size RFCOMM maximal frame size. The final maximal frame size is the minimal of this value and
    config->max_frame_size of @ref BtRfcommConnectResponse.
*/
void BtRfcommRegisterChannel(Handler handler, U8 channel, U16 max_frame_size);

/*!
    @brief Unregister a channel of RFCOMM server

    @param handler The server handler. Message BT_RFCOMM_UNREGISTER_CFM will send to it, after operation completed.
    @param channel The local channel which was registered before.
*/
void BtRfcommUnregisterChannel(Handler handler, U8 channel);

/*!

    @brief Request to connect remote RFCOMM server.


    @param handler The client handler. Message BT_RFCOMM_CLIENT_CONNECT_CFM will send to it, after operation completed.
    @param bdaddr Remote's BT MAC address.
    @param max_frame_size RFCOMM maximal frame size.
    @param channel Channel number which want to connect to.
    @param config User preference setting. If NULL, then the default value will be applied:
    - msc_timeout_ms (1000) Milliseconds
    - max_frame_size (600) Bytes
    @return Return the serial id. This value was passed to BT_RFCOMM_CLIENT_CONNECT_CFM message. User could use it
    to identify request.

*/
U32 BtRfcommConnectRequest(Handler handler, U8 bdaddr[6], U8 channel, BT_RFCOMM_CONFIG_T *config);

/*!
    @brief Response the incoming connection requesting.
    This function is used to answer the BT_RFCOMM_CONNECT_IND message.

    @param handler The server handler. Message BT_RFCOMM_SERVER_CONNECT_CFM will send to it, after operation completed.
    @param bdaddr Remote's BT MAC address.
    @param response Accept or deny the incoming request.
    @param channel Channel number passed by BT_RFCOMM_CONNECT_IND message.
*/
void BtRfcommConnectResponse(Handler handler, U8 bdaddr[6], BOOL response, U8 channel, BT_RFCOMM_CONFIG_T *config);

/*!
    @brief Request to send remote line status(RLS)
    @param handler The handler. Message BT_RFCOMM_LINE_STATUS_CFM will send to it, after operation completed.
    @param sink The existing channel which to be requested.
    @param line_status The line status.
*/
void BtRfcommRemoteLineStatueRequest(Handler handler, SINK sink, RFCOMM_LINE_STATUS_E line_status);

/*!

    @brief Request to disconnect channel.
    \image html rfcomm_disconnect.png

    @param handler The client handler. The BT_RFCOMM_DISCONNECT_CFM message will be returned to this handler.
    @param sink The existing channel which to be disconnected.

*/

void BtRfcommDisconnectRequest(Handler handler, SINK sink);

/*!
    @brief Response the incoming disconnect requesting

    This function is used to answer the @ref BT_RFCOMM_DISCONNECT_IND message

    \image html rfcomm_disconnect.png

    @param sink The existing channel which passed by @ref BT_RFCOMM_DISCONNECT_IND message.
*/
void BtRfcommDisconnectResponse(Handler handler, SINK sink);
/*!
    @}
*/

/* RHO related */
BOOL BtRfcommIsRhoAllow(void);
U16  BtRfcommGetRhoDataLength(void);
BOOL BtRfcommGetRhoData(U8 *pBuffer);
BOOL BtRfcommApplyAgentRhoData(BD_ADDR_T *pBdAddr, U8 *pRfcommRhoData, U16 length);
BOOL BtRfcommApplyPartnerRhoData(BD_ADDR_T *pBdAddr, U8 *pRfcommRhoData, U16 length);
U8 BtRfcommGetRhoSinkId(SINK sink);
SINK BtRfcommGetRhoSinkById(U8 idx);

#endif
