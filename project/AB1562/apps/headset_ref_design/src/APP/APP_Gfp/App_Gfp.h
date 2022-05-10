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

#ifndef __APP_GFP_H__
#define __APP_GFP_H__

<<<<<<< HEAD
=======

#define APP_GFP_LOG_MSGID_I(_message, arg_cnt, ...) LOG_MSGID_I(APP_GFP, "[APP_GFP]"_message, arg_cnt, ##__VA_ARGS__)


>>>>>>> db20e11 (second commit)
typedef enum
{
    GFP_GROUP_BLUETOOTH_EVENT           = 0x01,
    GFP_GROUP_COMPANION_APP_EVENT       = 0x02,
    GFP_GROUP_DEVICE_INFORMATION_EVENT  = 0x03,
    GFP_GROUP_DEVICE_ACTION_EVENT       = 0x04,
    GFP_GROUP_ACKNOWLEDGEMENT           = 0xFF,
}GFP_MESSAGE_GROUP;

<<<<<<< HEAD
=======

>>>>>>> db20e11 (second commit)
typedef enum
{
    GFP_CODE_ENABLE_SILENCE_MODE        = 0x01,
    GFP_CODE_DISABLE_SILENCE_MODE       = 0x02,
}GFP_BLUETOOTH_EVENT_CODE;

<<<<<<< HEAD
typedef enum
{
    GFP_CODE_LOG_BUFFER_FULL            = 0x01,
}GFP_COMPANION_APP_EVENT_CODE;
=======
>>>>>>> db20e11 (second commit)

typedef enum
{
    GFP_CODE_MODEL_ID                   = 0x01,
    GFP_CODE_BLE_ADDRESS_UPDATED        = 0x02,
    GFP_CODE_BATTERY_UPDATED            = 0x03,
    GFP_CODE_REMAINING_BATTERY_TIME     = 0x04,
    GFP_CODE_ACTIVE_COMPONENTS_REQ      = 0x05,
    GFP_CODE_ACTIVE_COMPONENTS_RESP     = 0x06,
    GFP_CODE_CAPABILITIES               = 0x07,
    GFP_CODE_PLATFORM_TYPE              = 0x08,
}GFP_DEVICE_INFORMATION_CODE;

<<<<<<< HEAD
=======

>>>>>>> db20e11 (second commit)
typedef enum
{
    GFP_CODE_RING                       = 0x01,
}GFP_DEVICE_ACTION_CODE;

<<<<<<< HEAD
=======

>>>>>>> db20e11 (second commit)
typedef enum
{
    GFP_CODE_ACK                        = 0x01,
    GFP_CODE_NAK                        = 0x02,
}GFP_ACKNOWLEDGEMENT_CODE;

<<<<<<< HEAD
=======

>>>>>>> db20e11 (second commit)
typedef struct
{
    U8 group;
    U8 code;
    U16 length;
}GFP_MESSAGE_STREAM_HEADER_T;

<<<<<<< HEAD
=======

>>>>>>> db20e11 (second commit)
typedef union
{
    U8 modelID[3];
    U8 bleAddr[6];
    U8 battery[3];
    U8 remainingTime;
    U8 activeComponentsResp;
    U8 capabilities;
    U8 platformType[2];
}GFP_DEVICE_INFORMATION_T;

<<<<<<< HEAD
=======

>>>>>>> db20e11 (second commit)
typedef struct
{
    U8 ringing[2];
}GFP_DEVICE_ACTION_T;

<<<<<<< HEAD
=======

>>>>>>> db20e11 (second commit)
typedef struct
{
    GFP_MESSAGE_STREAM_HEADER_T header;
    union
    {
        GFP_DEVICE_INFORMATION_T deviceInfo;
        GFP_DEVICE_ACTION_T deviceRing;
        U8 acknowledgement[3];
    }data;
}GFP_MESSAGE_STREAM_STRU;

<<<<<<< HEAD
=======

>>>>>>> db20e11 (second commit)
typedef enum
{
    GFP_ACKNOLEGEMENT_NAK_REASON_NOT_SUPPORTED = 0x00,  /**< FAST_PAIR message acknowledgement nak reason of not supported. */
    GFP_ACKNOLEGEMENT_NAK_REASON_DEVICE_BUSY   = 0x01,  /**< FAST_PAIR message acknowledgement nak reason of device busy. */
    GFP_ACKNOLEGEMENT_NAK_REASON_CURRENT_STATE = 0x02,  /**< FAST_PAIR message acknowledgement nak reason of not allowed due to current state. */
<<<<<<< HEAD
} GFP_ACK_REASON_T;
=======
}GFP_ACK_REASON_T;

>>>>>>> db20e11 (second commit)

typedef struct {
    U8 action;     /**< The acknowledge action ID. */
    U8 groupID;    /**< The message group ID of acknowledgements. */
    U8 codeID;     /**< The message code ID of acknowledgements. */
    U8 reason;     /**< The nak reason. It's only valid when action ID setted to #GFP_MESSAGE_CODE_ACKNOWLEDGEMENT_NAK. */
} GFP_ACK_T;

<<<<<<< HEAD
void APP_Gfp_Init(void);
void APP_Gfp_RegisterProfile(void);
void APP_Gfp_SendMessageStream(U8 group, U8 code);

#endif //__APP_GFP_H__
=======

typedef void (*gfp_spp_cb_t)(void *param);


void APP_Gfp_Init(void);
void APP_Gfp_RegisterProfile(void);
void APP_Gfp_SendMessageStream(U8 group, U8 code);
void APP_Gfp_ResetToFactory(void);
void APP_Gfp_AwsConnectedHandle(void);
void APP_Gfp_RhoDoneHandle(U8 newRole, BOOL status);
void APP_Gfp_McsyncDataHandle(U16 length, U8 *p_data);
BOOL APP_Gfp_SppIsAllowRho(void);

#endif //__APP_GFP_H__

>>>>>>> db20e11 (second commit)
