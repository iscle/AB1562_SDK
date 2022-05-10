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
 
#include "feature_config.h"

#if AIROHA_SHELL_EABLE


#ifndef __AB_UTIL_HH__
#define __AB_UTIL_HH__
#include <stdint.h>
#include <stdbool.h>

/*!
     @defgroup PAGE_API_LIBRARY_UTILITY ab_util.h
     @{
        @page PAGE_API_LIBRARY_UTILITY ab_util.h
        ab_util.h provides some useful function.

*/

/*!
    @brief Convert the hex string to number
    @param str Input the number as ASCII array
    @param num Out the result as unit32 type
    @return True if success
*/
bool AB_UTIL_Str2HexNum(char *str, uint32_t *num);
/*!
    @brief Convert the string to number
    @param str Input the number as ASCII array
    @param num Out the result as unit32 type
    @return True if success
*/
bool AB_UTIL_Str2Num(char *str, uint32_t *num);

/*!
    @brief Convert the string to unsigned 16 bits number
    @param str Input the number as ASCII array
    @param num Out the result as unit16_t type
    @return True if success
*/
bool AB_UTIL_Str2u16Num(char *str, uint16_t *num);

/*!
    @brief Convert the string to unsigned 8 bits number
    @param str Input the number as ASCII array
    @param num Out the result as unit8_t type
    @return True if success
*/
bool AB_UTIL_Str2u8Num(char *str, uint8_t *num);


/*!
    @brief Convert a string  to number array
    @param str The string
    @param nums Out the result as unit8 array
    @return Size of array
*/
int AB_UTIL_Str2u8HexNumArray(char *str, uint8_t nums[]);

/*!
    @brief Convert the hex string set to number array
    @param argc The number of ASCII array
    @param argv Input the number as ASCII array
    @param nums Out the result as unit8 array
    @return True if success
*/
bool AB_UTIL_StrArray2u8HexNumArray(int argc, char **argv, uint8_t nums[]);

/*!
    @brief Convert the string set to number array
    @param argc The number of ASCII array
    @param argv Input the number as ASCII array
    @param nums Out the result as unit8 array
    @return True if success
*/
bool AB_UTIL_StrArray2u8NumArray(int argc, char **argv, uint8_t nums[]);

/*!
    @brief Convert the string set to number array
    @param argc The number of ASCII array
    @param argv Input the number as ASCII array
    @param nums Out the result as unit8 array
    @return True if success
*/
bool AB_UTIL_StrArray2u32NumArray(int argc, char **argv, uint32_t nums[]);

/*! @} */

#endif

#endif
