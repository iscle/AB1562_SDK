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
#pragma once 

/**
 * @ingroup PAGE_USB_AUDIO
 * @{
 * This section introduces the USB Audio Class APIs 
 *
 */

/*!
  @ingroup PAGE_USB_AUDIO_DS
  @{  
	@brief Audio class buffer size control
*/

// ping-pong buffer settings. frame base. NUM > 2 needed
#define USB_AUDIO_BUFFER_NUM        2
#define USB_AUDIO_BUFFER_SIZE       288

/*!
   @}
*/


/*!
  @ingroup PAGE_USB_AUDIO_FN\
  @{  
	@brief Audio class api
*/

/*!
  @brief Init USB_AUDIO memory & flags
  
  @return void:
*/
EXTERN VOID DRV_USB_Audio_Init();

/*!
  @brief Release USB_AUDIO memory & flags
  
  @return void:
*/
EXTERN VOID DRV_USB_Audio_Release();

/*!
  @brief Get 1st data length on the queue
  
  @return 0: no data to get
        none-zero  data length be read.
*/
EXTERN U32 DRV_USB_Audio_GetDataLen();

/*!
  @brief Get 1st data on the queue
  
  @param pBuffer Buffer pointer 
  @param DataLen Data length to send
  
  @return 0: no data to get
        none-zero  data length be read.
*/
EXTERN U32 DRV_USB_Audio_Send(U8 *pBuffer, U32 DataLen);

/*!
  @brief Send data out 
  
  @param pBuffer Buffer pointer 
  @param DataLen Data length to send
  
  @return 0: Hardware busy. need resend. 
        none-zero  data length be send
*/
EXTERN U32 DRV_USB_Audio_Get(U8 *pBuffer, U32 RequestLen);
/**
* @}
*/
