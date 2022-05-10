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

#ifndef _BT_STREAM_H_
#define _BT_STREAM_H_

typedef void (*STREAM_CALLBACK)(void *data);

/******************************************************************************
 * Enum
*******************************************************************************/
enum
{
	STREAM_PACKET_IN_IND,

	STREAM_MESSAGE_NUM
};

/******************************************************************************
 * Message Structure
*******************************************************************************/
typedef struct
{
    BD_ADDR_T bdAddr;
}STREAM_PACKET_IN_IND_T;

/******************************************************************************
 * Public functions
*******************************************************************************/
void BtStreamA2dp_SetDspPlayAddr(U8 *pBdAddr);
void BtStreamA2dp_ClearDspPlayAddr(void);
void BtStreamA2dp_SetTransfrom(TRANSFORM transform);
void BtStreamA2dp_ClearTransform(void);
void BtStreamA2dp_SetMediaPlay(U8 *pBdAddr, BOOL isMediaPlay);
void BtStreamA2dp_ClearLCList(void);
void BtStreamA2dp_SetA2dpHandler(Handler handler);
BOOL BtStreamA2dp_IsDspPlaying(void);
U16 BtStreamA2dp_GetBufferDepth(void);
U32 BtStreamA2dp_GetDelayReportValue(void);
BOOL BtStreamA2dp_SourceConfigureA2dp(SOURCE source, stream_config_type type, U32 value);
void BtStreamA2dp_RegisterAppCommonHandler(Handler handler);
void BtStreamA2dp_AdjustA2dpClock(void);
void BtStreamA2dp_EnableGameModeInitLatency(void);
void BtStreamA2dp_DisableGameModeInitLatency(void);
void BtStreamA2dp_UpdateMceRole(SOURCE source, U8 role);
U32 BtStreamA2dp_GetA2dpCurrentLatency(SOURCE source);

#endif// _BT_STREAM_H_

