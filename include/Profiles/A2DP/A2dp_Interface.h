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


#ifndef __A2DP_INTERFACE_HH_
#define __A2DP_INTERFACE_HH_

	#include "PM_Message.h"

	typedef enum A2DP_MSG_E
	{
		A2DP_START_SERVICE_CFM = MSG_MESSAGE_BT_PROFILE_A2DP,	//0x0000
		A2DP_STOP_SERVICE_CFM,									//0x0001
		A2DP_SIGNAL_CONNECT_IND,
		A2DP_SIGNAL_CONNECT_CFM,
		A2DP_SIGNAL_DISCONNECT_CFM,
		A2DP_MEDIA_CONNECT_CFM,
		A2DP_MEDIA_DISCONNECT_CFM,
		A2DP_OPEN_CFM,
		A2DP_START_IND,
		A2DP_START_CFM,
		A2DP_SUSPEND_IND,
		A2DP_SUSPEND_CFM,
		A2DP_CLOSE_CFM,
        A2DP_RECONFIG_CFM,
	}A2DP_MSG_E;

	typedef struct A2DP_START_SERVICE_CFM_T
	{
		BT_GENERIC_STATUS_E status;
	}A2DP_START_SERVICE_CFM_T;

	typedef struct A2DP_STOP_SERVICE_CFM_T
	{
		BT_GENERIC_STATUS_E status;
	}A2DP_STOP_SERVICE_CFM_T;

	typedef struct A2DP_SIGNAL_CONNECT_IND_T
	{
		BD_ADDR_T bdAddr;
		U8 identifier;
		U16 channelId;
	}A2DP_SIGNAL_CONNECT_IND_T;

	typedef struct A2DP_SIGNAL_CONNECT_CFM_T
	{
		SINK sink;
		U8 status;
		BD_ADDR_T bdAddr;
	}A2DP_SIGNAL_CONNECT_CFM_T;

	typedef struct A2DP_SIGNAL_DISCONNECT_CFM_T
	{
		U8 status;
		BD_ADDR_T bdAddr;
	}A2DP_SIGNAL_DISCONNECT_CFM_T;

	typedef struct BT_A2DP_MEDIA_CONNECT_CFM
	{
		U8 status;
		BD_ADDR_T bdAddr;
	}A2DP_MEDIA_CONNECT_CFM_T;

	typedef struct A2DP_MEDIA_DISCONNECT_CFM_T
	{
		U8 status;
		BD_ADDR_T bdAddr;
	}A2DP_MEDIA_DISCONNECT_CFM_T;

	typedef struct A2DP_START_IND_T
	{
		BD_ADDR_T bdAddr;
	}A2DP_START_IND_T;

	typedef struct A2DP_START_CFM_T
	{
		BD_ADDR_T bdAddr;
		U8 isInt;
	}A2DP_START_CFM_T;

	typedef struct A2DP_SUSPEND_IND_T
	{
		BD_ADDR_T bdAddr;
	}A2DP_SUSPEND_IND_T;

	typedef struct A2DP_SUSPEND_CFM_T
	{
		BD_ADDR_T bdAddr;
	}A2DP_SUSPEND_CFM_T;

	typedef struct A2DP_CLOSE_CFM_T
	{
		BD_ADDR_T bdAddr;
	}A2DP_CLOSE_CFM_T;
    
	typedef struct A2DP_RECONFIG_CFM_T
	{
		BD_ADDR_T bdAddr;
	}A2DP_RECONFIG_CFM_T;

	typedef void (*A2DP_STREAM_CALLBACK)(void *data);

    void a2dp_start_service(Handler app);
    void a2dp_stop_service(Handler app);
    BOOL bt_a2dp_signal_disconnect_request(BD_ADDR_T *pBdAddr);
    BOOL bt_a2dp_signal_connect_request(BD_ADDR_T *pBdAddr);
    void a2dp_connect_response(BD_ADDR_T *pBdAddr, BOOL response, U16 channelId, U8 identifier);

    BOOL a2dp_is_in_streaming(BD_ADDR_T *pBdAddr);
    BOOL a2dp_is_open(BD_ADDR_T *pBdAddr);
    BOOL a2dp_is_connected(BD_ADDR_T *pBdAddr);
    BOOL a2dp_is_disconnected(BD_ADDR_T *pBdAddr);
    U8 a2dp_get_codec(BD_ADDR_T *pBdAddr);
    U8 a2dp_get_scms(BD_ADDR_T *pBdAddr);
    U16 a2dp_get_AAC_codec_sample_rate(BD_ADDR_T *pBdAddr);
    U8 a2dp_get_SBC_codec_sample_rate(BD_ADDR_T *pBdAddr);
    void bt_a2dp_close_request(BD_ADDR_T * pBdAddr);
    void a2dp_disconnect_signal_connection(BD_ADDR_T * pBdAddr);

    void bt_a2dp_start_cmd_response(BD_ADDR_T *pBdAddr, BOOL isAccept, BOOL isDropPkt);
    void bt_a2dp_suspend_cmd_response(BD_ADDR_T *pBdAddr, BOOL isAccept);
    void bt_a2dp_set_meida_play(BD_ADDR_T *pBdAddr, BOOL isPlay);

    void bt_a2dp_start_request(BD_ADDR_T *pBdAddr);
    void bt_a2dp_suspend_request(BD_ADDR_T *pBdAddr);
    void bt_a2dp_get_configuration(BD_ADDR_T *pBdAddr);

    U8  bt_a2dp_get_sample_rate_index(BD_ADDR_T *pBdAddr, U8 codec);
    void bt_a2dp_update_client_codec_info(U8 codecType, U16 codecInfo);
    void bt_a2dp_drop_pkt_setting(BD_ADDR_T *pBdAddr, BOOL isDropPkt);
    void bt_a2dp_set_reconfigure_status(BD_ADDR_T *pBdAddr, U8 status);
    U8 bt_a2dp_get_reconfigure_status(BD_ADDR_T *pBdAddr);
    U8 bt_a2dp_get_sbc_max_bitpool(BD_ADDR_T *pBdAddr);
    U8 bt_a2dp_get_sbc_min_bitpool(BD_ADDR_T *pBdAddr);

    void bt_a2dp_control_pkt(BD_ADDR_T *pBdAddr, BOOL value);
/**************************************************************************************************
* API (RHO)
**************************************************************************************************/
BOOL bt_a2dp_lock();
BOOL bt_a2dp_unlock();
BOOL bt_a2dp_is_allow();
U16 bt_a2dp_get_rho_data_length();
BOOL bt_a2dp_get_rho_data(U8 *pBuffer);
BOOL bt_a2dp_apply_agent_rho_data(BD_ADDR_T *pBdAddr, U8 *pA2dpData, U16 length);
BOOL bt_a2dp_apply_partner_rho_data(BD_ADDR_T *pBdAddr, U8 *pA2dpData, U16 length);
void bt_a2dp_set_directly_use_bit_rate_mode(BOOL isOn);




#endif //__A2DP_INTERFACE_HH_
