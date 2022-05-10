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


#ifndef __NVKEY_HANDLE_H__
#define __NVKEY_HANDLE_H__
/**************************************************************************************************
* Extern Functions
**************************************************************************************************/
typedef enum
{
	NO_STATE_MASK,						//0
	//APP_CONNECT_STATE_MASK,
	NVKEY_HDL_STATE_MASK_APP_DSP_MAIN_PATH,
	NVKEY_HDL_STATE_MASK_APP_DSP_PROMPT,
	//APP_ANC_PASSTHRU_DSP_STATE_MASK,		//5
	//APP_A2DP_DSP_STATE_MASK,
	//APP_MP3_DSP_STATE_MASK,
	//APP_LINE_IN_DSP_STATE_MASK,
	NVKEY_HDL_STATE_MASK_APP_DSP_ASSISTANT,
	//APP_GVA_DSP_STATE_MASK,
	NVKEY_HDL_STATE_MASK_APP_SMART_CASE,
	//APP_XIAOAI_DSP_STATE_MASK,
	MAX_STATE_MASK	= 32,
}NVKEY_HDL_STATE_MASK;



BOOL nvkey_hdl_save_nvkey(U16 nvkey_id, U16 data_length, void *msg);
void nvkey_hdl_set_state(NVKEY_HDL_STATE_MASK state_type);
void nvkey_hdl_remove_state(NVKEY_HDL_STATE_MASK state_type);
U32  nvkey_hdl_ger_state(void);

#endif
