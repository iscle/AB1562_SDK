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
 
 
#ifndef _AUDIO_DSP_NVKEY_PRIORITY_H_
#define _AUDIO_DSP_NVKEY_PRIORITY_H_


/******************************************************************************
 * Sturcture
*******************************************************************************/
/**
 * @brief Use brief, otherwise the index won't have a brief explanation.
 * @KeyID 0xF206
 */
/*NvkeyDefine NVKEYID_APP_AUDIO_DEVICE_PRIORITY*/
typedef struct
{
	U8 priority0;	/**< @Value 0x0A	@Desc AUDIO_DEVICE_FM */
	U8 priority1;	/**< @Value 0x03	@Desc desc AUDIO_DEVICE_SCO_IDLE */
	U8 priority2;	/**< @Value 0x0D	@Desc desc AUDIO_DEVICE_MP3 */
	U8 priority3;	/**< @Value 0x04	@Desc desc AUDIO_DEVICE_MUSIC */
	U8 priority4;	/**< @Value 0x07	@Desc desc AUDIO_DEVICE_AVRCP_PLAY */
	U8 priority8;	/**< @Value 0x09	@Desc desc AUDIO_DEVICE_LINE_IN */
	U8 priority5;	/**< @Value 0x08	@Desc desc AUDIO_DEVICE_KEY_AT_CMD */
	U8 priority6;	/**< @Value 0x06	@Desc desc AUDIO_DEVICE_CALL_STATE */
	U8 priority7;	/**< @Value 0x05	@Desc desc AUDIO_DEVICE_MUSIC_CALL_NOTIFY */
	U8 priority9;	/**< @Value 0x02	@Desc desc AUDIO_DEVICE_SCO_HSP */
	U8 priority10;	/**< @Value 0x01	@Desc desc AUDIO_DEVICE_SCO */
	U8 priority11;	/**< @Value 0x0B	@Desc desc AUDIO_DEVICE_DEMO_SOUND */
	U8 priority12;	/**< @Value 0	@Desc desc reserved */
	U8 priority13;	/**< @Value 0	@Desc desc reserved */
	U8 priority14;	/**< @Value 0	@Desc desc reserved */
	U8 priority15;	/**< @Value 0	@Desc desc reserved */
} PACKED AUDIO_PRIORITY_STRU;


#endif //_AUDIO_DSP_NVKEY_PRIORITY_H_
