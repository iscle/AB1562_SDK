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

#ifndef __BT_L2CAP_HH
#define __BT_L2CAP_HH

/**************************************************************************************************/
/*                            Macros                                                              */
/**************************************************************************************************/
/*!
    @addtogroup BT_LIB_L2CAP_MACRO
    @{
*/
/*!
    @}
*/
/**************************************************************************************************/
/*                            Data Structures                                                     */
/**************************************************************************************************/
 /*!
    @addtogroup BT_LIB_L2CAP_DS
    @{
*/
typedef enum {
    L2CAP_INFO_TYPE_NONE,
    L2CAP_INFO_TYPE_CONNECTIONLESS_MTU,
    L2CAP_INFO_TYPE_EXTENDED_FEATURES_SUPPORTED,
    L2CAP_INFO_TYPE_FIXED_CHANNELS_SUPPORTED,
    L2CAP_INFO_TYPE_RESERVED,
} BT_L2CAP_INFO_TYPE_E;

typedef struct
{
    U16 mtu;
} BT_L2CAP_CONNECT_CONFIG_T;

/** \brief L2CAP register confirmation. */
typedef struct BT_L2CAP_REGISTER_CFM_T
{
	U16 psm;
    U8 status;
} BT_L2CAP_REGISTER_CFM_T;

typedef struct BT_L2CAP_UNREGISTER_CFM_T
{
	U16 psm;
    U8 status;
} BT_L2CAP_UNREGISTER_CFM_T;

typedef struct
{
    SINK sink;
    BD_ADDR_T bdAddr;
	U16 psm;
	U16 channelId;
    BT_L2CAP_CONNECT_CONFIG_T remoteConfig;
	U8 status;
}  BT_L2CAP_CONNECT_CFM_T;

typedef struct
{
    BD_ADDR_T bdAddr;
    U16 psm;
    U16 channelId;
	U8 identifier;
} BT_L2CAP_CONNECT_IND_T;

typedef struct
{
    SINK sink;
	U8 identifier;
    U8 status;
} BT_L2CAP_DISCONNECT_IND_T;

typedef struct
{
    Handler app;
    SINK sink;
} BT_L2CAP_DISCONNECT_REQ_T;

typedef struct
{
    SINK sink;
	U8 status;
}BT_L2CAP_DISCONNECT_CFM_T;

typedef struct
{
    BD_ADDR_T bdAddr;
    U16 infoType;
	U8 *dataPtr;
    U16 result;
    U8 len;
	U8 status;
}  BT_L2CAP_INFO_RSP_T;

typedef struct
{
    BD_ADDR_T bdAddr;
    U16 len;
    U8 *dataPtr;
	U8 status;
}  BT_L2CAP_ECHO_RSP_T;

typedef struct
{
    BD_ADDR_T bdAddr;
    U16 psm;
	U8 *dataPtr;
    U16 len;
}  BT_L2CAP_CONNECTIONLESS_DATA_IND_T;

/*!
    @}
*/
/**************************************************************************************************/
/*                            Functions                                                           */
/**************************************************************************************************/
/*!
    @addtogroup BT_LIB_L2CAP_FN
    @{
*/

/*L2CAP*/
/*!
	@brief Register an L2CAP PSM with the L2CAP.
	@param app The client handler. The BT_L2CAP_REGISTER_CFM message is return to the handler. This is also the handler associated with the registered PSM.
	All incoming connection indications for this PSM will be forwarded to this handler.
	@param psm The PSM being registered with the L2CAP.
*/
void BtL2capRegisterRequest(Handler app, U16 psm);
/*!
	@brief Unregister an L2CAP PSM from the L2CAP.
	@param app The client handler. The BT_L2CAP_UNREGISTER_CFM message is return to the handler. Only the handler that registered the PSM with the L2CAP can unregister it.
	@param psm The PSM being unregistered with the L2CAP.
*/
void BtL2capUnregisterRequest(Handler app, U16 psm);
/*!
	@brief initiate an L2CAP connection to a particular device.
	@param app The client handler. The BT_L2CAP_CONNECTION_CFM message is return to the handler.
	@param bd_addr The address of the particular device being connected to.
	@param psm The PSM of remote device being connected to.
	@param config The configuration of the L2CAP connection.
*/
void BtL2capConnectRequest(Handler app, const BD_ADDR_T *bd_addr, U16 psm, BT_L2CAP_CONNECT_CONFIG_T config);
/*!
	@brief Response to BT_L2CAP_CONNECT_IND message.
	@param app The client handler. The BT_L2CAP_CONNECTION_CFM message is return to the handler.
	@param response set to TRUE to accept the incoming connection or FALSE to reject it.
	@param psm The PSM of remote device being connected to.
	@param channelId The channelId of connect request from BT_L2CAP_CONNECT_IND message.
	@param identifier The identifier of connect request from BT_L2CAP_CONNECT_IND message.
	@param config The configuration of the L2CAP connection.
*/
void BtL2capConnectResponse(Handler app, BOOL response, U16 psm, U16 channelId, U8 identifier,BD_ADDR_T *bd_addr, BT_L2CAP_CONNECT_CONFIG_T config);
/*!
	@brief Request to disconnect an L2CAP connection.
	@param app The client handler. The BT_L2CAP_DISCONNECT_CFM message is return to the handler.
	@param sink The sink of the L2CAP connection.
*/
void BtL2capDisconnectRequest(Handler app, SINK sink);
/*!
	@brief response to a BT_L2CAP_DISCONNECT_IND message.
	@param identifier The identifier of disconnect request from BT_L2CAP_DISCONNECT_IND message.
	@param sink The sink of the L2CAP connection.
*/
void BtL2capDisconnectResponse(U8 identifier, SINK sink);
/*
	@brief initiate an L2CAP echo request to a particular device.
	@param app The client handler. The BT_L2CAP_ECHO_RSP message is return to the handler.
	@param bd_addr The address of the particular device being connected to.
	@param len The length of data.
	@param dataPtr The point of data.
*/
void BtL2capEchoReq(Handler app, const BD_ADDR_T *bd_addr, U16 len, U8 *dataPtr);
/*
	@brief initiate an L2CAP echo request to a particular device.
	@param app The client handler. The BT_L2CAP_INFO_RSP message is return to the handler.
	@param bd_addr The address of the particular device being connected to.
	@param infoType The type of implementation specific information being requested.
*/
void BtL2capInfoReq(Handler app, const BD_ADDR_T *bd_addr, U16 infoType);
/*!
    @}
*/

U16 BtL2capGetDcidByCid(U16 cid);

/* Audio Path*/
void BtSetAudioPathInfo(BD_ADDR_T *bd_addr, U16 cid, U8 codec, U8 scms);
void BtClearAudioInfo(BD_ADDR_T *bd_addr);


/* RHO */
BOOL bt_l2cap_is_rho_allow(void);
U16  bt_l2cap_get_rho_data_length(void);
BOOL bt_l2cap_get_rho_data(U8 *p_buffer);
BOOL bt_l2cap_apply_agent_rho_data(BD_ADDR_T *p_bdaddr, U8 *p_l2cap_rho_data, U16 length);
BOOL bt_l2cap_apply_partner_rho_data(BD_ADDR_T *p_bdaddr, U8 *p_l2cap_rho_data, U16 length);
void bt_l2cap_set_handler_by_cid(U16 cid, Handler app);

#endif
