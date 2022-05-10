
/* Copyright Statement:
 *
 * (C) 2018  Airoha Technology Corp. All rights reserved.
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


#ifndef __BT_AWS_MCE_H_
#define __BT_AWS_MCE_H_


#include "bt.h"


/**************************************************************************************************
* Definition
**************************************************************************************************/
#define BT_AWS_MCE_INFORMATION_A2DP              (0x81)
#define BT_AWS_MCE_INFORMATION_SCO               (0x82)
#define BT_AWS_MCE_INFORMATION_REPORT_APP        (0x85)
#define BT_AWS_MCE_INFORMATION_REPORT_RESET      (0x86)
#define BT_AWS_MCE_INFORMATION_ROLE_HANDOVER     (0x87)
#define BT_AWS_MCE_INFORMATION_FOLLOWER_PKT		 (0x88)
#define BT_AWS_MCE_INFORMATION_PARTNER_SELECTION (0x8C)
#define BT_AWS_MCE_INFORMATION_MIC_SELECTION     (0x8E)
#define BT_AWS_MCE_INFORMATION_RACE              (0x8F)

#define BT_AWS_MCE_IF_CID 			0x0008
#define BT_AWS_MCE_IF_URGENT_CID 	0x0009

/**************************************************************************************************
* Enumeration
**************************************************************************************************/
typedef enum AWS_MCE_ROLE_E {
	ROLE_NONE = 0x00,
    ROLE_PARTNER = 0x30,
    ROLE_AGENT = 0x40,
    ROLE_SLAVE_AGENT = 0x41,
    ROLE_SLAVE_CONNECTABLE_AGENT = 0x51,
    ROLE_SLAVE_AGENT_WITH_PARTNER = 0x61,
    ROLE_FOLLOWER_1 = 0x38,
    ROLE_FOLLOWER_2 = 0x34,
} AWS_MCE_ROLE_E;


typedef enum AWS_MCE_CONNECT_TARGET_E {

    AWS_MCE_CONNECT_LOCAL,
    AWS_MCE_CONNECT_AGENT,
    AWS_MCE_CONNECT_AGENT_F1,
    AWS_MCE_CONNECT_AGENT_F2,
} AWS_MCE_CONNECT_TARGET_E;


typedef enum AWS_MCE_RHO_DATA_MODULE_E {

    AWS_MCE_RHO_MODULE_APP,
    AWS_MCE_RHO_MODULE_HFP,
	AWS_MCE_RHO_MODULE_A2DP,
	AWS_MCE_RHO_MODULE_AVRCP,
	AWS_MCE_RHO_MODULE_AVCTP,
	AWS_MCE_RHO_MODULE_RFCOMM, //5
	AWS_MCE_RHO_MODULE_SDP,
	AWS_MCE_RHO_MODULE_L2CAP,
	AWS_MCE_RHO_MODULE_GAP,
	AWS_MCE_RHO_MODULE_AVDTP,
	AWS_MCE_RHO_MODULE_AWSMCE, //10
	AWS_MCE_RHO_MODULE_PM,
	AWS_MCE_RHO_MODULE_AMA,
	AWS_MCE_RHO_MODULE_IAP2,
    AWS_MCE_RHO_MODULE_XIAOAI,
<<<<<<< HEAD
    //AWS_MCE_RHO_MODULE_GFP,
=======
	AWS_MCE_RHO_MODULE_ATT,
    AWS_MCE_RHO_MODULE_GFP,
>>>>>>> db20e11 (second commit)
	AWS_MCE_RHO_MODULE_NUM,
} AWS_MCE_RHO_DATA_MODULE_E;


typedef enum AWS_MCE_RHO_PREPARE_STATUS_E {

    AWS_MCE_RHO_PREPARE_SUCCESS,
    AWS_MCE_RHO_PREPARE_FAIL,
} AWS_MCE_RHO_PREPARE_STATUS_E;

typedef enum AWS_MCE_RHO_SET_FINISH_STATUS_E {

    AWS_MCE_RHO_SET_FINISH_SUCCESS,
    AWS_MCE_RHO_SET_FINISH_FAIL,
} AWS_MCE_RHO_SET_FINISH_STATUS_E;

typedef enum AWS_MCE_RHO_CANCEL_STATUS_E {

    AWS_MCE_RHO_CANCEL_SUCCESS,
    AWS_MCE_RHO_CANCEL_FAIL,
} AWS_MCE_RHO_CANCEL_STATUS_E;


typedef enum AWS_MCE_DISCONNECT_STATUS_E {

    AWS_MCE_DISCONNECT_SUCCESS,
    AWS_MCE_DISCONNECT_FAIL_LINK_NOT_EXIST,
} AWS_MCE_DISCONNECT_STATUS_E;


/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct BT_AWS_MCE_REGISTER_CFM_T
{
    U8 status;
} BT_AWS_MCE_REGISTER_CFM_T;


typedef struct
{
    SINK l2cap_sink;
    SINK l2cap_urgent_sink;
	U8 status;
    U8 bdaddr[6];
}BT_AWS_MCE_CONNECT_CFM_T;


typedef BT_READ_CLOCK_CFM_T BT_AWS_MCE_READ_CLOCK_CFM_T;
typedef BT_GAP_CANCEL_CONN_CFM_T BT_AWS_MCE_CANCEL_CONN_CFM_T;

typedef struct
{
	U8 status;
    U8 reason;
    U8 bdaddr[6];
}BT_AWS_MCE_DISCONNECT_IND_T;


typedef struct
{
    HCI_STATUS_E status;
	AWS_MCE_ROLE_E role;
    U8 bdaddr[6];
}BT_AWS_MCE_STATUS_CHANGE_IND_T,BT_AWS_MCE_RHO_ROLE_CHANGE_IND_T;


typedef struct
{
    AWS_MCE_RHO_PREPARE_STATUS_E status;
}BT_AWS_MCE_PREPARE_ROLE_HANDOVER_CFM_T;



typedef struct
{
    AWS_MCE_RHO_SET_FINISH_STATUS_E status;
}BT_AWS_MCE_SET_ROLE_HANDOVER_FINISH_CFM_T;


typedef struct
{
    AWS_MCE_RHO_CANCEL_STATUS_E status;
}BT_AWS_MCE_CANCEL_ROLE_HANDOVER_CFM_T;

typedef struct
{
    AWS_MCE_DISCONNECT_STATUS_E status;
    U8 bdaddr[6];
}BT_AWS_MCE_DISCONNECT_CFM_T;


typedef struct
{
    U8 type;
    U8 rsvd;
    U16 length;
}PACKED BT_AWS_MCE_IF_PACKET_HEADER;


typedef struct
{
    BT_AWS_MCE_IF_PACKET_HEADER header;
    U16 l2cap_channel;
    U8 state;
    U8 volume;
    U8 code_type;
    U8 seq_type;
    U8 delay_report;
    U8 codec_infolen;
    U16 bt_clk_intra;
    U32 bt_clk;
    U32 timestamp;
    U8 sec_type;
    U8 codec_info[6];
}PACKED BT_AWS_MCE_IF_PACKET_A2DP;


typedef struct
{
	U8 moduleId;
	U16 length;
	U8 pData[1];
}PACKED BT_AWS_MCE_RHO_MODULE_STUR;


/**************************************************************************************************
* Public function
**************************************************************************************************/
void BtAwsMceRegisterHandler(Handler AwsMceHandler);

void BtAwsMceSetStatus(U8 (*bdAddr)[6], AWS_MCE_ROLE_E role);

void BtAwsMceConnect(U8 (*bdAddr)[6], AWS_MCE_CONNECT_TARGET_E target);

void BtAwsMceConnectionCancel(Handler app,U8 (*bdAddr)[6]);

void BtAwsMceDisconnect(Handler app, U8 (*bdAddr)[6], AWS_MCE_CONNECT_TARGET_E target);

void BtAwsMceReadCurrentClock(Handler app, U8 (*bdAddr)[6]);

U32 BtAwsMceReadClock(void);

void BtAwsMcePrepareRoleHandoverReq(Handler app, U8 (*aws_bdAddr)[6], U8 (*partner_bdAddr)[6]);

void BtAwsMceCancelRoleHandoverReq(Handler app, U8 (*bdAddr)[6]);

void BtAwsMceSetRoleHandoverFinish(Handler app, U8 (*bdAddr)[6]);
void BtAwsMceShareModeEnable(Handler app, U8 (*bdAddr)[6], BOOL enable ,U8 (*shareModeBDA)[6]);

U8* BtAwsMceClaimRHOPacket(U16 length);

void BtAwsMceSendRHOPacket(U8* packet);


#endif
