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


#ifndef _APP_MCSYNC_DATA_H_
#define _APP_MCSYNC_DATA_H_

/**************************************************************************************************
* Structure
**************************************************************************************************/


/**************************************************************************************************
* Structure bt group
**************************************************************************************************/
typedef struct APP_MCSYNC_DATA_STATE_T
{
	U8 state;
}APP_MCSYNC_DATA_STATE_T;

enum
{
    APP_MCSYNC_A2DP_STATE_NONE,
    APP_MCSYNC_A2DP_STATE_READY,
    APP_MCSYNC_A2DP_STATE_PLAYING,
    APP_MCSYNC_A2DP_STATE_PLAYDROP,
};
typedef struct APP_MCSYNC_DATA_NUM_AG_T
{
	U8 numOfAG;
    U8 a2dpState;
}PACKED APP_MCSYNC_DATA_NUM_AG_T;

typedef struct APP_MCSYNC_DATA_RHO_STATE_T
{
	U8 state;
}APP_MCSYNC_DATA_RHO_STATE_T;

typedef struct APP_MCSYNC_DATA_RECONNECT_MASK_T
{
	U8 notReconnMask;
}APP_MCSYNC_DATA_RECONNECT_MASK_T;

typedef struct APP_MCSYNC_DATA_RECONNECT_STATE_T
{
	U8 type;
	BD_ADDR_T bdAddr;
}APP_MCSYNC_DATA_RECONNECT_STATE_T;

typedef struct APP_MCSYNC_DATA_RHO_PARA_T
{
	U16 batPercent;
	S8 rssi;
}APP_MCSYNC_DATA_RHO_PARA_T;

typedef struct APP_MCSYNC_DATA_FOLLOWER_DISC_T
{
	U8 isFollowerIFPkt;
	U8 isOnlyForFollower;
}APP_MCSYNC_DATA_FOLLOWER_DISC_T;

<<<<<<< HEAD
=======
typedef struct APP_MCSYNC_TAKE_OVER_STATE_T
{
	U8 takeOverState;
}APP_MCSYNC_TAKE_OVER_STATE_T;

typedef struct APP_MCSYNC_NOTICE_MASTER_PARTNER_TO_ENTER_SHARE_MODE_T
{
	U8 enterShareMode;
}APP_MCSYNC_NOTICE_MASTER_PARTNER_TO_ENTER_SHARE_MODE_T;
>>>>>>> db20e11 (second commit)

/**************************************************************************************************
* Structure gva group
**************************************************************************************************/
<<<<<<< HEAD
typedef struct APP_MCSYNC_GFP_INFO_STRU
{
	U32 length;
	U8 data[1];
}APP_MCSYNC_GFP_INFO_STRU;
=======

>>>>>>> db20e11 (second commit)

/**************************************************************************************************
* Structure ama group
**************************************************************************************************/

/**************************************************************************************************
*Structure  audio group
**************************************************************************************************/
typedef struct APP_MCSYNC_DATA_VP_IS_ENABLE_T
{
	U8 isEnable;
}APP_MCSYNC_DATA_VP_IS_ENABLE_T;

typedef struct APP_MCSYNC_DATA_ANC_T
{
	U8 userRegister;
	U8 enable;
	U8 flash_no;
	U8 type;
	S16 runtime_gain;
	U32 clockToStart;
}PACKED APP_MCSYNC_DATA_ANC_T;

typedef struct APP_MCSYNC_DATA_SYNC_VPRT_T
{
	U8 overwriteTime;
	U8 reserved;
	U16 rtIndex;
	U16 vpIndex;
	U16 eventIndex;
	U32 currentPicoClock;
}PACKED APP_MCSYNC_DATA_SYNC_VPRT_T;

typedef struct
{
	U8 index;
}APP_MCSYNC_DATA_VP_INDEX_T;

/**************************************************************************************************
*Structure sys group
**************************************************************************************************/
typedef struct
{
	U8 type;
}APP_MCSYNC_DATA_FCD_RESET_T;

typedef struct APP_MCSYNC_DATA_POWEROFF_T
{
	U8 reason;
	U8 isFollowerIFPKT:1;
	U8 isOnlyForFollower:1;
	U8 reserved:6;
}APP_MCSYNC_DATA_POWEROFF_T;

typedef struct
{
	BOOL isMute;
}APP_MCSYNC_DATA_SPKER_MUTE_T;

typedef struct APP_MCSYNC_DATA_PARTNER_BDADDR_T
{
	BD_ADDR_T partnerBdAddr;
}PACKED APP_MCSYNC_DATA_PARTNER_BDADDR_T;

/**************************************************************************************************
* Structure pph group
**************************************************************************************************/
typedef struct APP_MCSYNC_DATA_SMART_CASE_STATE_T
{
	U8 state;
}APP_MCSYNC_DATA_SMART_CASE_STATE_T;

typedef struct APP_MCSYNC_DATA_IN_EAR_DETECTION_T
{
	U8 status;
}APP_MCSYNC_DATA_IN_EAR_DETECTION_T;

typedef struct APP_MCSYNC_DATA_KEYCODE_T
{
	U16 keyCode;
}APP_MCSYNC_DATA_KEYCODE_T;
/**************************************************************************************************
* Structure newton group
**************************************************************************************************/

<<<<<<< HEAD
=======
/**************************************************************************************************
* Structure Tile group
**************************************************************************************************/
typedef enum
{
	APP_MCSYNC_TILE_INFO_OPCODE_CMD,
	APP_MCSYNC_TILE_INFO_OPCODE_RSP
} APP_MCSYNC_TILE_INFO_OPCODE_E;


typedef struct APP_MCSYNC_TILE_INFO_STRU
{
	APP_MCSYNC_TILE_INFO_OPCODE_E opcode;
	U8 streamState;//HFP and A2DP Streaming State
}APP_MCSYNC_TILE_INFO_STRU;

>>>>>>> db20e11 (second commit)

/**************************************************************************************************
*Public API
**************************************************************************************************/
void APP_MCSync_Data_Callback_Init(void);
#endif // _APP_MCSYNC_DATA_H_
