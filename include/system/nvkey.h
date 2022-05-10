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

#ifndef _NVKEY_H_
#define _NVKEY_H_

// For user NVKEYs, start from the following base number, and ranges from 0xFE00~0xFFFF.
#define NVKEY_ID_USER_0			0xFE00
#define NVKEY_ID_USER_END		0xFFFF

 /**
 * @addtogroup NVKey
 * @{
 */

/*!
  @brief This function can be used to write NVKEY's in the whole numbering space (from 0x0001 to 0xFFFF).

  @param Key The specified NVKEY's ID (from 0x0001~0xFFFF).
  @param ptr The memory space will be filled with the NVKEY's value.
  @param length Bytes of the NVKEY's value.
  
  @return 0: successful.
	1: specified NVKEY is successfully deleted.
	2: no data has been written since the length is not between 1~4087.
	4: flash space not enough for this writing operation.
	5: no data has been written since the key id is equal to 0. The valid range should be in 1~65535.
	6: memory allocation fail.
	7: NVKEY subsystem not initialized.
*/
U16 NVKEY_WriteFullKey(U16 Key, void *ptr, U16 length);

/*!
  @brief Read the specified NVKEY in the whole NVKEY numbering space (0x0001~0xFFFF).
  
  @param Key The specified NVKEY's ID.
  @param ptr The memory space will be filled with the NVKEY's value.
  @param length The length of the memory space specified in the 2nd parameter, and this will limit the number of bytes can be read out.
  
  @return Return bytes read; return 0 if the specifed NVKEY not exists.
*/
U16 NVKEY_ReadFullKey(U16 Key, void *ptr, U16 length);

/*!
  @brief Get the starting flash address of the specified NVKEY's payload.
  
  @param Key The specified NVKEY's ID.
  
  @return Return 0 if the specified NVKEY not found or NVKEY module not initialized. Return the specified NVKEY's starting flash address if successes.
*/
U32 NVKEY_GetPayloadFlashAddress(U16 Key);

/*!
  @brief Get the length of the specified NVKEY's payload.
  
  @param Key The specified NVKEY's ID.
  
  @return Return 0 if the specified NVKEY not found, else return the length (in bytes) of the specified NVKEY's payload.
*/
U16 NVKEY_GetPayloadLength(U16 Key);

/*!
  @brief Try to reclaim invalid flash space if the specified minimum space is not met in the current NVKEY flash storage. This API can be called when the whole system is not busy, and
	avoid the performance impact when a recycling procedure triggered during a heavy loading operation (eg. audio streaming).
  
  @param minimum_space The minimum flash space required during operation.
  
  @return Return 0 if the minimum space requirement can't be met (the NVKEY flash storage is almost full). Return 1 if the space is alreay enough and no recycling procedure triggered.
	Return 2 if the minimum space requirement is met after a recycling procedure triggered.
*/
U16 NVKEY_ReclaimCheck(U16 minimum_space);

/*!
  @brief Disable flash writing and buffer NVKEY writing request into RAM. For example, if the system is streaming music, we should set a bit to disable flash writing.
  
  @param SpecifiedBit Specify which bit (0-based) will be set to 1. We have 32 bits to set in total.
  
  @return Return modified 32 bits value.
*/
U32 NVKEY_Flash_Access_Disable(U8 SpecifiedBit);

/*!
  @brief Enable flash writing and when all bits are cleared to 0, all NVKEY writing request buffered in RAM will be flushed to flash.
  
  @param SpecifiedBit Specify which bit will be clear to 0. We have 32 bits to clear in total.
  
  @return Return modified 32 bits value.
*/
U32 NVKEY_Flash_Access_Enable(U8 SpecifiedBit);

/*!
  @brief Get flash writing disable bitmap. Bits set to 1 will block flash writing.
  
  @param None.
  
  @return Return flash writing disable bitmap.
*/
U32 NVKEY_Flash_Access_GetBitmap(void);

/*!
  @brief Set decryption key for NVKEYs between 0xF410~0xF45F.
  
  @param Key An array of 32 bytes to specify the key used for decryption.
  
  @return Always return 0.
*/
U32 NVKEY_SetDecryptionKey(U8 Key[32]);

/*!
  @brief Call this function before using flash address returned by NVKEY_GetPayloadFlashAddress().
  
  @param
  
  @return Return a 32 bits value for debugging.
*/
U32 NVKEY_GetPayloadFlashAddress_Enter(void);

/*!
  @brief Call this function after finishing using flash address returned by NVKEY_GetPayloadFlashAddress().
  
  @param
  
  @return Return a 32 bits value for debugging.
*/
U32 NVKEY_GetPayloadFlashAddress_Leave(void);

/**
* @}
*/

/*!
  @brief This function can be used to create a new NVKEY in non-FOTA region.

  @param Key The specified NVKEY's ID (from 0x0001~0xFFFF).
  @param ptr The memory space will be filled with the NVKEY's value.
  @param length Bytes of the NVKEY's value.
  
  @return 0: successful.
	1: specified NVKEY is successfully deleted.
	2: no data has been written since the length is not between 1~4087.
	4: flash space not enough for this writing operation.
	5: no data has been written since the key id is equal to 0. The valid range should be in 1~65535.
	6: memory allocation fail.
	7: NVKEY subsystem not initialized.
    8: wrong parameters.
    9: specified NVKEY already exists.
*/
U16 NVKEY_CreateNewKey_In_NonFOTA(U16 Key, void *ptr, U16 length);

#endif
