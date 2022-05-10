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

#ifndef _RACE_CMD_H_
#define _RACE_CMD_H_

#include "bt.h"
#include "types.h"
#include "message.h"
#include "stream.h"
#include "log.h"
/**
 * @addtogroup race Race Command
 * @{
 * Race command is used to communicate with airoha chip. It could be transmitted over different transport media
 * like UART, USB CDC or Bluetooth, ...etc. By default, airoha was defined a set of race commands, user could use
 * tools which was provided by airoha to do some controlling , configuration, quering, ... operations. This module
 * was provide a way to extend the race command in application layer. When internal parser didn't recognize the incomming
 * race command, it will pass to application, and application could use those APIs to send response to tools. Here introduce
 * the race packet format and tranport media.
 *
 *  ### Packet Format
 *  Race packet was formed by 4 parts: <b>Type</b>, <b>Length</b>, <b>ID</b> and <b>Payload</b>.
 *  - <b>Type</b> : 4 types defined
 *   + Command(0x5A) : Do some defined operations on chip. (receive)
 *   + Response(0x5B) : Response the command which issued by command(0x5A) type. (transmit)
 *   + Command without response(0x5C) : Do some defined operations on chip, no response(0x5B) returned furthur. (receive)
 *   + Notification(0x5D): Used by chip to provide information or status to devices. (transmit)
 *  - <b>Length</b> : Bytes count from ID to last byte of payload.
 *  - <b>ID</b> : A pre-assigned 16 bits number, each represent an operation.
 *  - <b>Payload</b> : Operation's parameter.
 * ![Figure 1 : race packet format.](@ref race_packet.png)
 *
 *
 * ### Transport Media
 * In USB CDC or UART transport, race packets were transmitted by H4 or H5 protocols, and the revered HCI packet type 0x05 was used.
 * ![Figure 2 : H4 protocol.](@ref race_h4.png)
 * ![Figure 3 : H5 protocol.](@ref race_h5.png)
 * In Bluetooth transport, the race packets were formed without any extra header.
 *
 * This module was designed by the concept of  avoiding application to handing the complicated packet format and tranport media by
 * \ref RACE_ClaimPacket, and \ref RACE_FlushPacket APIs.
 *
 * ### Register Race Command Handler\br
 * Register the handler. Once the internal unrecognized race command was received, \ref MESSAGE_RACE_CMD will be sent to it.
 * ~~~~~~~~~~~~~~~{.c}
 * static const HandlerData handler_data = {race_handler};
 * ...
 * RACE_RegisterHandler((Handler)&handler_data);
 * ...
 * ~~~~~~~~~~~~~~~
 * ### Implement Race Command Handler\br
 * ~~~~~~~~~~~~~~~{.c}
 * The race command packet were copied into \ref MESSAGE_RACE_CMD_T structure, thus id, length and payload were provided.
 * Then, using \ref RACE_ClaimPacket to allocate reponse packet and \ref RACE_FlushPacket to send back to tool.
 * static U32 race_handler(Handler t, U16 id, void*msg, U32 id_ext)
 * {
 *    if (id == MESSAGE_RACE_CMD)
 *    {
 *        MESSAGE_RACE_CMD_T *race_msg = (MESSAGE_RACE_CMD_T*)msg;
 *        U16 param_length = race_msg->race_cmd.header.hdr.length - sizeof(U16); //RACE (Length field)
 *        U16 id = id = race_msg->race_cmd.header.id; //<--- RACE (ID field)
 *        U8 *payload = race_msg->race_cmd.param; //<--- RACE (Payload field)
 *        switch(id)
 *        {
 *            case MY_RACE_ECHO:
 *                 U8 *ptr = (U8*)RACE_ClaimPacket(RACE_TYPE_RESPONSE, MY_RACE_ECHO, param_length, race_msg->channel_id);
 *                 memcpy(ptr, payload, param_length);
 *                 RACE_FlushPacket(ptr, param_length);
 *            break;
 *                ...
 *        }
 *
 *    }
 *    return 0;
 * }
 * ~~~~~~~~~~~~~~~
 * ### Unsolicited Notification by Race\br
 * By some situations, application need to send some information to tool, RACE_TYPE_NOTIFICATION
 * ~~~~~~~~~~~~~~~{.c}
 *   U8 *ptr = (U8*)RACE_ClaimPacket(RACE_TYPE_NOTIFICATION, MY_ALERT, ..., ...);
 *    ...
 *   RACE_FlushPacket(ptr, ...);
 * ~~~~~~~~~~~~~~~
 *
 *
 */

////////////////////////////////////////////////////////////////////////////////
// CONSTANT DEFINITIONS ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/*RACE global error code*/
typedef U8 RACE_ERRCODE;
#define RACE_ERRCODE_SUCCESS (RACE_ERRCODE)(0)
#define RACE_ERRCODE_FAIL (RACE_ERRCODE)(1)
#define RACE_ERRCODE_NOT_SUPPORT (RACE_ERRCODE)(2)
#define RACE_PARAMETER_ERROR (RACE_ERRCODE)(12)

/*RACE type*/
#define RACE_TYPE_COMMAND 0x5A
#define RACE_TYPE_RESPONSE 0x5B
#define RACE_TYPE_COMMAND_WITHOUT_RSP 0x5C
#define RACE_TYPE_NOTIFICATION 0x5D

/*RACE done*/
#define RACE_DONE_SUCCESS 0xFF
#define RACE_DONE_UNKNOWN 0xFE

/* RACE channel Type */
#define RACE_CHANNEL_TYPE_UART      0
#define RACE_CHANNEL_TYPE_USB       1
#define RACE_CHANNEL_TYPE_AIRAPP    2
#define RACE_CHANNEL_TYPE_SOFTWARE  3
#define RACE_CHANNEL_TYPE_AIRUPDATE 4
#define RACE_CHANNEL_TYPE_AWSPEER   5
#define RACE_CHANNEL_TYPE_FCD       6


#define RACE_MSGID_I(fmt,cnt,arg...)        LOG_MSGID_I(RACE_CMD,fmt,cnt,##arg)
#define RACE_MSGID_W(fmt,cnt,arg...)        LOG_MSGID_W(RACE_CMD,fmt,cnt,##arg)
#define RACE_MSGID_E(fmt,cnt,arg...)        LOG_MSGID_E(RACE_CMD,fmt,cnt,##arg)

////////////////////////////////////////////////////////////////////////////////
// TYPE DEFINITIONS ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#pragma pack(push, 1)
#endif

/*!
    @brief Race packet without payload and id.
*/
typedef struct
{
    U8 pktId;       /*!< Packet type, usually is 0x5*/
    U8 type;        /*!< Type field*/
    U16 length;     /*!< Length field*/
} PACKED RACE_COMMON_HDR_STRU, *PTR_RACE_COMMON_HDR_STRU;

/*!
    @brief Race packet without payload.
*/
typedef struct
{
    RACE_COMMON_HDR_STRU hdr;   /*!<Race packet without payload and id*/
    U16 id;                     /*!< ID field*/
} PACKED RACE_GLOBAL_HDR_STRU, *PTR_RACE_GLOBAL_HDR_STRU;

/*!
    @brief A specific race response packet.
    This packet was formed by 1 byte payload(status).
*/
typedef struct
{
    RACE_GLOBAL_HDR_STRU hdr; /*!< Race packet without payload*/
    U8 status;                /*!< 1 byte status payload*/
} PACKED RACE_EVT_HDR_STRU, *PTR_RACE_EVT_HDR_STRU;

/*!
    @brief A generic race packet.
*/
typedef struct
{
    RACE_GLOBAL_HDR_STRU header; /*!< Race packet without payload*/
    U8 param[0];                 /*!< Payload field*/
} PACKED RACE_PAYLOAD_STRU, *PTR_RACE_PAYLOAD_STRU;

#ifdef _WIN32
#pragma pack(pop)
#endif

/*!
    @brief Data structure of \ref MESSAGE_RACE_CMD message.
*/
typedef struct
{
    U8 channel_id; /**< Transport media identifier*/
    RACE_PAYLOAD_STRU race_cmd; /**< Race packet*/
} MESSAGE_RACE_CMD_T;

typedef enum
{
    RACE_DBG_NONE,
    RACE_DBG_CMD,
    RACE_DBG_EVT,
    RACE_DBG_EVT_APP,
    RACE_DBG_IF_RELAY,
    RACE_DBG_FLUSH,
    RACE_DBG_MAX,
}RACE_DBG_E;

////////////////////////////////////////////////////////////////////////////////
// FUNCTION DECLARATIONS /////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*!
    @brief Allocate Race packet with specific type and id.
    @param race_type Race type. Only \ref RACE_TYPE_COMMAND, \ref RACE_TYPE_RESPONSE, \ref RACE_TYPE_COMMAND_WITHOUT_RSP and \ref RACE_TYPE_NOTIFICATION allowed.
    @param race_id Race ID.
    @param channel_id Transport media id, provided by  channel_id of \ref MESSAGE_RACE_CMD_T structure.
    @return The payload field data pointer.
*/
VOID *RACE_ClaimPacket(U8 race_type, U16 race_id, U16 dat_len, U8 channel_id);

/*!
    @brief Send race packet which was allocated by \ref RACE_ClaimPacket.
    @param ptr Payload field data pointer, which was provide by \ref RACE_ClaimPacket.
    @param dat_len Payload length to be sent. No more than the length parameter of \ref RACE_ClaimPacket.
*/
VOID RACE_FlushPacket(VOID *ptr, U16 dat_len);

/*!
    @brief Shrink race packet.
    @param ptr Payload field data pointer, which was provide by \ref RACE_ClaimPacket.
    @param dat_len Payload length to be sent. No more than the length parameter of \ref RACE_ClaimPacket.
    @note This function was used internally.
*/
VOID RACE_ShrinkPacket(VOID *ptr, U16 dat_len);

/*!
    @breif Register handler for customized race command.
*/


VOID RACE_DestroyPacket(VOID *ptr);



VOID RACE_RegisterHandler(Handler app);

//EXTERN VOID RaceCmdUnknownCmdHdlr(IN PTR_RACE_GLOBAL_HDR_STRU pCmdMsg, U8 channel_id);


U8 RACE_SourceSinkPlug(SOURCE source,SINK sink,U8 type);
U8 RACE_HandlerPlug(Handler handler, U8 type);

VOID RACE_ExcuteCmd(U8 channel_id);
VOID RACE_Unplug(U8 channel_id);

U32 RACE_GetChannelIdByType(U8 *ptr, U32 maxnum, U32 target_type);

VOID RACE_DUMP(RACE_DBG_E type, VOID *ptr);

/*!
@}
*/
#endif /* _RACE_CMD_H_ */
