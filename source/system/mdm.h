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

#include "types.h"

typedef volatile unsigned int *      P_U32;

#define MDM_base (0x04201A00) // MDM
#define MDM_HEXFMT32B (MDM_base + 0x0048)
#define MDM_DECFMT32B (MDM_base + 0x004c)
#define MDM_BINFMT32B (MDM_base + 0x0050)
#define MDM_ENDFAIL (MDM_base + 0x0038)
#define MDM_ENDSUCC (MDM_base + 0x003C)
#define MDM_ENDSIM (MDM_base + 0x0040)
#define MDM_RUNTIME_USEC (MDM_base + 0x007C)
#define MDM_STR_CLEAR (MDM_base + 0x0098)
#define MDM_STR_DISPLAY (MDM_base + 0x009C)
#define MDM_STR0 (MDM_base + 0x00A0)

#define MDM_TM_HEXFMT32B(a) (*((P_U32)MDM_HEXFMT32B) = (U32)(a)) // display Hexa format 32b value
#define MDM_TM_DECFMT32B(a) (*((P_U32)MDM_DECFMT32B) = (U32)(a)) // display Dec format 32b value
#define MDM_TM_BINFMT32B(a) (*((P_U32)MDM_BINFMT32B) = (U32)(a)) // display Binary format 32b value
#define MDM_TM_RUNTIME_USEC(a) ((a) = *((P_U32)MDM_RUNTIME_USEC)) // get simulation runtime micro-second
#define MDM_TM_STR_CLEAR(n) (*((P_U32)MDM_STR_CLEAR) = (U32)(n)) // clear string N
#define MDM_TM_STR_DISPLAY(n) (*((P_U32)MDM_STR_DISPLAY) = (U32)(n)) // display string N
#define MAX_MDM_STR_LEN 255
    extern void MDM_TM_STR_N(U32 str_n, const char *str); // set string N
    extern void MDM_TM_STR(const char *str); // display string to string 0
    extern void MDM_TM_PRINTF(const char *fmt,...); // display format string to string 0
    extern void MDM_TM_PRINT_LINE_BY_LINE(const char *str, U32 len); // display string line by line to string 0
    extern void MDM_TM_HW_CHECK(void); // jump to 0x04
#define MDM_TM_ENDFAIL (*((P_U32)MDM_ENDFAIL) = 1) // Test bench complete with error
#define MDM_TM_ENDSIM (*((P_U32)MDM_ENDSIM) = 1) // Test bench complete with error
#define MDM_TM_ENDSUCC { MDM_TM_HW_CHECK(); } // Test bench complete successfully


