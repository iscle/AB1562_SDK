/* Copyright Statement:
 *
 * (C) 2020  Airoha Technology Corp. All rights reserved.
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
#ifndef __FATAL_CODE_H__
#define __FATAL_CODE_H__

/** @addtogroup Exception_Code Exception
  * @{
  * @addtogroup Fatal_Code Fatal Error Code Enums
  * @{
  */
/** @brief
 * This enum defines the fatal error code.
 */
typedef enum{
    FATAL_ERROR_SYSTEM_SERVICE_START = 0x0,   /**< 0x0: system service error code start. */
    FATAL_ERROR_SYSTEM_SERVICE_END   = 0x200, /**< 0x200: system service error code end. */
    FATAL_ERROR_BT_START             = 0x201, /**< 0x201: BT fatal error code start. */
    FATAL_ERROR_BT_END               = 0x400, /**< 0x400: BT fatal error code end. */
    FATAL_ERROR_AUDIO_START          = 0x401, /**< 0x401: Audio fatal error code start. */
    FATAL_ERROR_AUDIO_END            = 0x600, /**< 0x600: Audio fatal error code end. */
    FATAL_ERROR_UI_START             = 0x601, /**< 0x601: UI fatal error code start. */
    FATAL_ERROR_UI_END               = 0x800, /**< 0x800: UI fatal error code end. */
<<<<<<< HEAD
=======
    FATAL_ERROR_ESCO_START           = 0x801, /**< 0x801: ESCO fatal error code start. */
    FATAL_ERROR_ESCO_END             = 0xA00, /**< 0xA00: ESCO fatal error code end. */
>>>>>>> db20e11 (second commit)
}fatal_code_type_t;

/**
  * @}
  * @}
  */
#endif /*__FATAL_CODE_H__*/
