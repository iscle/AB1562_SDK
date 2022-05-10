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
 

#include "feature_config.h"

#ifndef __SPP_HH_
#define __SPP_HH_
#include "bt.h"
#include "stream.h"


/*!
    @addtogroup BT_PRO_SPP
    @{

        \ref spp.h "These APIs" provide applications a easy way to exchange data with remote by standard Serial Port Profile.\br
        This simple SPP was implemented by \ref BT_LIB_RFCOMM and \ref BT_LIB_SDP to support an one server and one client. \br
        The message sequence chart were shown below:

        \image html spp_client.png

        \image html spp_server.png
    @}
*/

/**************************************************************************************************
* Macro
**************************************************************************************************/
/*!
@addtogroup BT_PRO_SPP_MACRO
@{
    @brief SPP setup definition
*/


/*!
	@brief SPP message offset. \ref MSG_MESSAGE_BT_PROFILE_BASE
*/
#define MSG_MESSAGE_BT_PROFILE_SPP MSG_MESSAGE_BT_PROFILE_BASE+0x0000

/*!
@}
*/
/**************************************************************************************************
* Type define
**************************************************************************************************/
/*!
@addtogroup BT_PRO_SPP_DS
@{
    @brief Messages and their data structures
*/

/*!
    @name Enum (SPP Message)
    @{
	@brief SPP Messages
*/
typedef enum {
    SPP_START_SERVICE_CFM = MSG_MESSAGE_BT_PROFILE_SPP, /*!< Confirmation of \ref SPP_StartService*/
    SPP_STOP_SERVICE_CFM,                               /*!< Confirmation of \ref Spp_StopService*/
    SPP_CONNECT_CFM,                             		/*!< Indication of remote connection.*/
    SPP_DISCONNECT_CFM,                                 /*!< Confirmation of \ref SppDisconnectRequest*/
	SPP_RX_DATA_IND,
	SPP_TX_AVAILABLE_IND,
	SPP_INTERNAL_TX_DATA_IND,
} SPP_MSG_E;

/*!
    @}
*/

/*!
    @name Data Structure
    @{
*/
/*!
    @typedef SPP_START_SERVICE_CFM_T
	@brief Data structure of \ref SPP_START_SERVICE_CFM message
*/
typedef struct
{
    BT_GENERIC_STATUS_E status; /*!< Execute status */
}SPP_START_SERVICE_CFM_T;

/*!
    @typedef SPP_STOP_SERVICE_CFM_T
    @brief Data structure of \ref SPP_STOP_SERVICE_CFM message
*/
typedef struct
{
   BT_GENERIC_STATUS_E status; /*!< Execute statue */
}SPP_STOP_SERVICE_CFM_T;

/*!
	@brief Data structure of \ref SPP_CONNECT_CFM message
*/
typedef struct SPP_CONNECT_CFM_T
{
	BT_GENERIC_STATUS_E status;
	BD_ADDR_T bdAddr;
}SPP_CONNECT_CFM_T;

/*!
	@brief Data structure of \ref SPP_DISCONNECT_CFM message
*/
typedef struct SPP_DISCONNECT_CFM_T
{
	BT_GENERIC_STATUS_E status;
	BD_ADDR_T bdAddr;
}SPP_DISCONNECT_CFM_T;

typedef struct SPP_INTERNAL_TX_DATA_IND_T
{
	U32 pLinkInfo;
	U16 length;
	U8 data[1];
}SPP_INTERNAL_TX_DATA_IND_T;

typedef struct SPP_RX_DATA_IND_T
{
	BD_ADDR_T bdAddr;
	U16 length;
	U8 data[1];
}SPP_RX_DATA_IND_T;

typedef struct SPP_TX_AVAILABLE_IND_T
{
	BD_ADDR_T bdAddr;
}SPP_TX_AVAILABLE_IND_T;


/*!
    @}
*/

/*!
@}
*/

/**************************************************************************************************
* API
**************************************************************************************************/
/*!
@addtogroup BT_PRO_SPP_FN
@{
     @brief 4 Functions
*/

/*!
    @brief Start SPP service
    After done, SPP_START_SERVICE_CFM message will be sent to the app handler.
    @param app The application handler
*/
void SPP_StartService(Handler app, U8 sppChannel);

/*!
    @brief Stop SPP service
    After done, SPP_STOP_SERVICE_CFM message will be sent to the app handler.
    @param app The application handler
*/
void Spp_StopService(Handler app);
/*!
    @brief Disconnect exist SPP link.
    After done, SPP_DISCONNECT_CFM message will be sent to the app handler.
    @param app The application handler.
*/
BOOL SPP_DisconnectRequest(BD_ADDR_T *pBdAddr);

/*!
    @brief Create a new SPP link.
    After done, SPP_CLIENT_CONNECT_CFM message will be sent to the app handler.
    @param bdaddr Remote BT address.
*/
BOOL SPP_ConnectRequest(BD_ADDR_T *pBdAddr);

/*!
    @brief transmit data via SPP link.
    After done, MESSAGE_MORE_SPACE message will be sent to the app handler.
    @param bdaddr Remote BT address.
    @param length The length of data.
	@param pData The memory location of data.
*/
BOOL SPP_TxData(BD_ADDR_T *pBdAddr, U16 length, U8 * pData);

U16 SPP_GetMtu(BD_ADDR_T *pBdAddr);

BOOL SPP_RxDone(BD_ADDR_T *pBdAddr, U16 length);

BOOL SPP_SetRxDropByUpperLayer(BD_ADDR_T *pBdAddr, BOOL enable);
#endif
