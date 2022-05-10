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

#include "hal_spm.h"

#ifdef HAL_SLEEP_MANAGER_ENABLED

#include <stdio.h>
#include <string.h>
#include "syslog.h"

log_create_module(SPM, PRINT_LEVEL_INFO);
#define SPM_MCE_MSGID_I(fmt,cnt,arg...)        LOG_MSGID_I(SPM,fmt,cnt,##arg)
#define SPM_MCE_MSGID_W(fmt,cnt,arg...)        LOG_MSGID_W(SPM,fmt,cnt,##arg)
#define SPM_MCE_MSGID_E(fmt,cnt,arg...)        LOG_MSGID_E(SPM,fmt,cnt,##arg)

#if 0
#include "hal_log.h"
#include "memory_attribute.h"
#include "hal_nvic.h"
#include "hal_nvic_internal.h"
#include "assert.h"
#include "hal_gpt.h"
#include "hal_platform.h"
#include "hal_wdt_internal.h"
#endif

/* AB1562 SPM Code Rev : pcm_suspend_20200922.spm */
const unsigned int pcm_event_vector_parameter[8] = {0x31,0x7f0033,0x100035,0x540030,0x260034,0x0,0x0,0x0};

#define PCM_IM_MAX_LENGTH 1423

static const unsigned int spm_im[PCM_IM_MAX_LENGTH] = {
0x18c0001f,0xa2110810,0xe0e00020,0x18c0001f,0xa21103a0,0xe0e00000,0x1940001f,0xa2110184,0xe1600000,
0xe8208000,0xa2110318,0x0b16ff08,0x1b00001f,0x00090001,0xf0000000,0x17c07c1f,0x18c0001f,0xa2110810,
0xe0e0002c,0x1b00001f,0x00800001,0xf0000000,0x17c07c1f,0x18c0001f,0xa2111028,0xe0e00001,0x18c0001f,
0xa2110c84,0xe0e00100,0x1900001f,0xa21103a0,0xe1200000,0x1b00001f,0x00200001,0x1940001f,0x00000001,
0xf0000000,0x17c07c1f,0x18c0001f,0xa2110810,0xe0e00010,0x1910001f,0xa21103b0,0x81081001,0xc8c00a04,
0x17c07c1f,0xc8e00644,0x17c07c1f,0xf0000000,0x17c07c1f,0x18c0001f,0xa2110810,0xe0e00014,0x18c0001f,
0xa2110204,0x1940001f,0x00000001,0x1910001f,0xa21103ac,0x81041001,0xc8e01b04,0x17c07c1f,0x18c0001f,
0xa211100c,0x1940001f,0x00000000,0xc0c01b00,0x17c07c1f,0x1940001f,0xa2110cb4,0xe1600001,0x1910001f,
0xa2110b00,0x91201001,0xa0121000,0xa0108400,0x1b00001f,0x00090001,0xf0000000,0x17c07c1f,0x1b00001f,
0x00200001,0xf0000000,0x17c07c1f,0x18c0001f,0xa2110810,0xe0e00018,0x1940001f,0xa2110184,0xe1600001,
0x1990001f,0xa2110368,0x81841801,0x1910001f,0xa21103b0,0x810c1001,0xa1001804,0xc8c00ce4,0x17c07c1f,
0xc8e00e24,0x17c07c1f,0xf0000000,0x17c07c1f,0x18c0001f,0xa2110810,0xe0e0001c,0x1940001f,0xa2110184,
0xe1600000,0x1b00001f,0x00090001,0xf0000000,0x17c07c1f,0xa0120400,0x1b00001f,0x00020001,0x18c0001f,
0xa21103a0,0xe0e00001,0xe8208000,0xa2110318,0x0b16ff0c,0xe8208000,0xa2110318,0x0b16ff08,0xf0000000,
0x17c07c1f,0x18c0001f,0xa2110810,0xe0e00024,0x80308400,0x1b80001f,0x200005a7,0x80320400,0x18c0001f,
0xa2110810,0xe0e00028,0x18c0001f,0xa2110204,0x1940001f,0x00000001,0x1910001f,0xa2110808,0x81009001,
0xc8e01384,0x17c07c1f,0x1940001f,0x00000000,0x18c0001f,0xa211100c,0xc0c01380,0x17c07c1f,0x1b00001f,
0x00300001,0xf0000000,0x17c07c1f,0xe0e00016,0x1b80001f,0x20000003,0xe0e0001e,0x1b80001f,0x20000003,
0x1910001f,0xa21101e4,0x17c07c1f,0x17c07c1f,0x81001001,0xd8001764,0x17c07c1f,0x81801401,0xd8001626,
0x17c07c1f,0x81809401,0xd80019a6,0x17c07c1f,0xd00019e0,0x17c07c1f,0x1900001f,0xa211027c,0x1980001f,
0x00000000,0xe1000006,0x1980001f,0x01000000,0xe1000006,0xd00019e0,0x17c07c1f,0x1900001f,0xa211027c,
0x1980001f,0x00000000,0xe1000006,0x1980001f,0x01000000,0xe1000006,0x1900001f,0xa2111094,0x1980001f,
0x00000000,0xe1000006,0x1980001f,0x07000000,0xe1000006,0xd00019e0,0x17c07c1f,0xd00019e0,0x17c07c1f,
0xe0e0000e,0xe0e0000c,0xe0e0001c,0x1b80001f,0x20000004,0xe0e0001d,0xe0e0000d,0xf0000000,0x17c07c1f,
0xe0e0001d,0xe0e0001f,0x1910001f,0xa21101e4,0x17c07c1f,0x17c07c1f,0x81001001,0xd8001e64,0x17c07c1f,
0x81801401,0xd8001d26,0x17c07c1f,0x81809401,0xd80020a6,0x17c07c1f,0xd00020e0,0x17c07c1f,0x1900001f,
0xa211027c,0x1980001f,0x00000000,0xe1000006,0x1980001f,0x0000000f,0xe1000006,0xd00020e0,0x17c07c1f,
0x1900001f,0xa211027c,0x1980001f,0x00000000,0xe1000006,0x1980001f,0x0000000f,0xe1000006,0x1900001f,
0xa2111094,0x1980001f,0x00000000,0xe1000006,0x1980001f,0x00000007,0xe1000006,0xd00020e0,0x17c07c1f,
0xd00020e0,0x17c07c1f,0xe0e0001e,0xe0e0001a,0xe0e00012,0xf0000000,0x17c07c1f,0x18c0001f,0xa2110810,
0xe0e00000,0x1b80001f,0x2000000e,0x81000801,0xc94027c4,0x17c07c1f,0xc9602864,0x17c07c1f,0x81008801,
0xc94027c4,0x17c07c1f,0xc9602864,0x17c07c1f,0x81010801,0xc94027c4,0x17c07c1f,0xc9602864,0x17c07c1f,
0x81018801,0xc94027c4,0x17c07c1f,0xc9602864,0x17c07c1f,0x81020801,0xc94027c4,0x17c07c1f,0xc9602864,
0x17c07c1f,0x81028801,0xc94027c4,0x17c07c1f,0xc9602864,0x17c07c1f,0x81030801,0xc94027c4,0x17c07c1f,
0xc9602864,0x17c07c1f,0x81038801,0xc94027c4,0x17c07c1f,0xc9602864,0x17c07c1f,0xe0e000ff,0x1b80001f,
0x2000000e,0xf0000000,0x17c07c1f,0xe0e000ff,0x1b80001f,0x2000000a,0xf0000000,0x17c07c1f,0xe0e00000,
0x1b80001f,0x2000000e,0xf0000000,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,
0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x17c07c1f,0x1840001f,0x00000001,
0x18c0001f,0xa2110810,0xe0e00004,0x81f08407,0xa1d00407,0x18c0001f,0xa2110380,0xe0f07fff,0xe8208000,
0xa2110318,0x0b16ff03,0x18c0001f,0xa2110810,0xe0e0000c,0x1b00001f,0x07400001,0x18d0001f,0xa2110930,
0x80cc8c01,0xd8000b03,0x17c07c1f,0x1b00001f,0x07400001,0x18d0001f,0xa2110930,0x80cb0c01,0x1910001f,
0xa21103b0,0x81001001,0x80e01003,0xd82001a3,0x17c07c1f,0x1b00001f,0x00300001,0x1b80001f,0xd0100001,
0x1b80001f,0x90100001,0x18d0001f,0xa2110808,0x81008c01,0x1950001f,0xa2110830,0x814a9401,0x81801404,
0xd8200486,0x17c07c1f,0xd00001a0,0x17c07c1f,0x18c0001f,0xa2110810,0xe0e0003c,0xf0000000,0x18c0001f,
0xa2110810,0xe0e00030,0x18c0001f,0xa2110200,0x1940001f,0x00000002,0x1910001f,0xa2110808,0x81001001,
0xd8200904,0x17c07c1f,0x81f10407,0xc8c01b04,0x17c07c1f,0xd0000320,0x17c07c1f,0x1a00001f,0xa2110b90,
0xe2200001,0xc8e01384,0x17c07c1f,0xa1d10407,0xd0000320,0x17c07c1f,0x1a00001f,0xa2110b90,0xe2200001,
0x1a00001f,0xa2110c80,0xe2200100,0xd0000320,0x17c07c1f,0x18c0001f,0xa2110810,0xe0e00008,0x18c0001f,
0xa2110214,0x1940001f,0x00000004,0x1910001f,0xa2110808,0x81029001,0xd8201b84,0x17c07c1f,0x1980001f,
0xa2200044,0xe1a00001,0x1910001f,0xa2200044,0x81041001,0xd8200ce4,0x17c07c1f,0x1940001f,0xa21f0018,
0xe1600001,0xe0e0001d,0xe0e0001f,0x1900001f,0xa2110150,0xe1200001,0x1a00001f,0x00000000,0x1a40001f,
0x000000ff,0x1910001f,0xa21101e4,0x81001001,0xd8201004,0x17c07c1f,0x1900001f,0xa2111084,0xe1200000,
0x1900001f,0xa2110310,0xe1200001,0x1900001f,0xa2110424,0xe1000008,0xe1200001,0x1900001f,0xa2110424,
0xe1200011,0x1900001f,0xa2110430,0xe1000008,0xe1200001,0x1900001f,0xa2110430,0xe1200011,0x1900001f,
0xa211043c,0xe1000008,0xe1200001,0x1900001f,0xa211043c,0xe1200011,0x1900001f,0xa2110448,0xe1000008,
0xe1200001,0x1900001f,0xa2110448,0xe1200011,0x1900001f,0xa2110454,0xe1000008,0xe1000009,0x1900001f,
0xa2110460,0xe1000008,0xe1000009,0x1900001f,0xa211046c,0xe1000008,0xe1000009,0x1900001f,0xa2110478,
0xe1000008,0xe1200001,0x1900001f,0xa2110478,0xe1000008,0xe1200003,0x1900001f,0xa2110478,0xe1000008,
0xe1200013,0x1900001f,0xa2110478,0xe1000008,0xe1200033,0x1900001f,0xa2110480,0xe1000008,0xe1200001,
0x1900001f,0xa2110480,0xe1200003,0x1900001f,0xa2110480,0xe1200007,0x1900001f,0xa2110480,0xe120000f,
0x1900001f,0xa2110480,0xe120001f,0x1900001f,0xa2110480,0xe120003f,0x1900001f,0xa2110480,0xe120007f,
0x1900001f,0xa2110480,0xe12000ff,0xe0e0001e,0xe0e0001a,0xe0e00012,0x1900001f,0xa2110cbc,0xe1200001,
0xd00002e0,0x17c07c1f,0x1a00001f,0xa2111028,0xe2200001,0xe0e00016,0x1b80001f,0x20000003,0xe0e0001e,
0x1b80001f,0x20000003,0x1900001f,0xa2110150,0xe1200000,0x1900001f,0xa2110310,0xe1200000,0x1910001f,
0xa21101e4,0x81001001,0xd8201e64,0x17c07c1f,0x1900001f,0xa2111084,0xe1200001,0x1900001f,0xa2110424,
0xe1200010,0x1900001f,0xa2110424,0xe1200000,0x1940001f,0xa2110430,0xe1600010,0x1940001f,0xa2110430,
0xe1600000,0x1980001f,0xa211043c,0xe1a00010,0x1980001f,0xa211043c,0xe1a00000,0x1900001f,0xa2110448,
0xe1200010,0x1900001f,0xa2110448,0xe1200000,0x1940001f,0xa2110454,0xe1600000,0x1980001f,0xa2110460,
0xe1a00000,0x1900001f,0xa211046c,0xe1200000,0x1940001f,0xa2110478,0xe1600032,0x1940001f,0xa2110478,
0xe1600030,0x1940001f,0xa2110478,0xe1600020,0x1940001f,0xa2110478,0xe1600000,0x1980001f,0xa2110480,
0xe1a000fe,0x1980001f,0xa2110480,0xe1a000fc,0x1980001f,0xa2110480,0xe1a000f8,0x1980001f,0xa2110480,
0xe1a000f0,0x1980001f,0xa2110480,0xe1a000e0,0x1980001f,0xa2110480,0xe1a000c0,0x1980001f,0xa2110480,
0xe1a00080,0x1980001f,0xa2110480,0xe1a00000,0xe0e0000e,0xe0e0001e,0xe0e0001c,0x1b80001f,0x2000001a,
0xe0e0001d,0x1a40001f,0x01000000,0xe1000009,0xe1400009,0xe1800009,0x1900001f,0xa2110424,0x1940001f,
0xa2110430,0x1980001f,0xa211043c,0xe1000009,0xe1400009,0xe1800009,0x1900001f,0xa2110448,0x1940001f,
0xa2110454,0x1980001f,0xa2110460,0xe1000009,0xe1400009,0xe1800009,0xe0e0000d,0x1940001f,0xa2200044,
0xe1600000,0x1910001f,0xa2200044,0x81041001,0xd8002b24,0x17c07c1f,0x1940001f,0xa21f0018,0xe1600000,
0xd00002e0,0x17c07c1f,0x1a00001f,0xa2111028,0xe2200001,0x1a00001f,0xa2110c84,0xe2200001,0xd00002e0,
0x17c07c1f,0x18d0001f,0xa21101d4,0x81040c01,0x80c00c01,0xd8003143,0x17c07c1f,0x18c0001f,0xa2110810,
0xe0e00038,0x80328400,0x18c0001f,0xa0020800,0xe0e000ab,0x18c0001f,0xa0020010,0xe0e00001,0x18c0001f,
0xa0020014,0xe0e00000,0x18c0001f,0xa0020000,0xe0e00008,0xe0e0000c,0x18d0001f,0xa0020000,0x80c00c01,
0xd8003043,0x17c07c1f,0x18c0001f,0xa0020000,0xe0e00000,0x18c0001f,0xa21101d0,0xe0e00101,0xd00001a0,
0x17c07c1f};


static struct {
    uint32_t conn_top_on;
    uint32_t conn_top_off;
    uint32_t audio;
} mtcmos_resource;

uint32_t manual_spm_read_im(uint32_t addr);
void manual_spm_write_im(uint32_t addr, uint32_t data);

void manual_spm_write_im(uint32_t addr, uint32_t data)
{
    *SPM_PCM_IM_HOST_RW_PTR = 0x01010000 | addr;
    *SPM_PCM_IM_HOST_RW_DAT = data;
    return;
}

uint32_t manual_spm_read_im(uint32_t addr)
{
    uint32_t data;
    *SPM_PCM_IM_HOST_RW_PTR = 0x01000000 | addr;
    data = *SPM_PCM_IM_HOST_RW_DAT;
    *SPM_PCM_IM_HOST_RW_PTR = 0;
    return (data);
}

int spm_init(void)
{
    int result = 0;
    uint32_t i, status;
    extern uint32_t read_chip_version(void);

    if((read_chip_version()&0xFF00) == 0xCB00) {
        /* AB1562U */
        *((volatile uint32_t *)(0xA21101E4)) = 1;
    }else {
        *((volatile uint32_t *)(0xA21101E4)) = 0;
    }

    /* [4]: Resets PCM */
    *SPM_PCM_CON0 = 0x0B160000 | 0x10;
    hal_gpt_delay_us(10);
    *SPM_PCM_CON0 = 0x0B160000;
    hal_gpt_delay_us(10);

    /* Load PCM_EVENT_VECTOR[0-7] */
    *SPM_PCM_EVENT_VECTOR0 = pcm_event_vector_parameter[0];
    *SPM_PCM_EVENT_VECTOR1 = pcm_event_vector_parameter[1];
    *SPM_PCM_EVENT_VECTOR2 = pcm_event_vector_parameter[2];
    *SPM_PCM_EVENT_VECTOR3 = pcm_event_vector_parameter[3];
    *SPM_PCM_EVENT_VECTOR4 = pcm_event_vector_parameter[4];
    *SPM_PCM_EVENT_VECTOR5 = pcm_event_vector_parameter[5];
    *SPM_PCM_EVENT_VECTOR6 = pcm_event_vector_parameter[6];
    *SPM_PCM_EVENT_VECTOR7 = pcm_event_vector_parameter[7];

    /* PCM kick*/
    *SPM_POWER_ON_VAL0 = 0;             /* POWER_ON_VAL0 */
    *SPM_POWER_ON_VAL1 = 0x84;          /* POWER_ON_VAL1 */
    *SPM_PCM_REG_DATA_INI = *SPM_POWER_ON_VAL0 | 0x1; /* [0]: sc_xo_cg_en default =1 */
    *SPM_PCM_PWR_IO_EN = 0x00010000;    /* for R0 */
    *SPM_PCM_PWR_IO_EN = 0x00000000;
    *SPM_PCM_REG_DATA_INI = *SPM_POWER_ON_VAL1;
    *SPM_PCM_PWR_IO_EN = 0x00800000;    /* for R7 */
    *SPM_PCM_PWR_IO_EN = 0x00000000;
    *SPM_CLK_SETTLE = 0x3;           /* CLK_SETTLE */
    *SPM_PCM_PWR_IO_EN = 0x81;
    //*((volatile uint32_t*)(SPM_BASE + 0x0380)) = 0x0;           /* mask all AP wakeup event */
    *SPM_SLEEP_WAKEUP_EVENT_MASK = 0x0;           /* mask all AP wakeup event */

    /* Init IM Length and pointer */
    *SPM_PCM_IM_LEN = PCM_IM_MAX_LENGTH;
    *SPM_PCM_IM_PTR = (uint32_t)spm_im;
    /* manual load spm code */
    for (i = 0; i < PCM_IM_MAX_LENGTH; i++) {
        manual_spm_write_im(i, spm_im[i]);
    }
    *SPM_PCM_CON1 = 0x0B169C49;
    /* Kick IM process */
    *SPM_PCM_CON0 = 0x0B160002;

    /* Wait ready state */
    do {
        status = (*SPM_PCM_FSM_STA >> 9) & 0x1;
    } while (status != 0x01);

    *SPM_PCM_PWR_IO_EN = 0x0081;        // enable R0 & R7 output
    *SPM_SEQUENCER_26M_REG_1 = 0x1B;

#if 0
    system_info = *((volatile uint32_t *)(0xA2010040)); /* SYSTEM_INFOD */
    if (((system_info >> 8) & 0x1) == 0) {
        /* bond_psram_sip == 0 */
        *SPM_SKIP_SFC_EMI_TASK |= (1 << 0);    /* SKIP PSRAM */
    }

    if (((system_info >> 9) & 0x1) == 0) {
        /* bond_sf_sip == 0 */
        *SPM_SKIP_SFC_EMI_TASK |= (1 << 8);    /* SKIP SFC */
    }
#endif

    for (i = 0; i < PCM_IM_MAX_LENGTH; i++) {
        /* Check SPM Code */
        if (manual_spm_read_im(i) != spm_im[i]) {
            SPM_MCE_MSGID_E("SPM Code loading Fail\r\n",0);
            return (result++);
        }
    }

    *SPM_SEQUENCER_32K_REG_4 = 0x1D;
    *((volatile uint32_t *)(0xA2060010)) |= 0x1000000;  /* xo ready enable */

    /* [4]: Resets PCM */
    *SPM_PCM_CON0 = 0x0B160000 | 0x10;
    hal_gpt_delay_us(10);
    *SPM_PCM_CON0 = 0x0B160000;
    hal_gpt_delay_us(10);

    //SPM_CLEAR_LOCK_INFRA;
    //SPM_INFRA_OFF_FLAG = 0;

    *SPM_SEQUENCER_32K_REG_1 = 0x0;
    *SPM_SEQUENCER_32K_REG_2 = 0x0;
    *SPM_SEQUENCER_32K_REG_3 = 0x40;
    *SPM_SEQUENCER_32K_REG_4 = 0x85;

    /* Wakeup Source mask setting */
    *SPM_DSP_WAKEUP_SOURCE_MASK = 0x0;

    /* Disable DCXO in sleep mode */
    *SPM_CONN_AUDIO_ABB_XO_SIDEBAND = 0;

    /* SPM Kick Start */
    *SPM_PCM_CON0 = 0x0B160103;

    return (result);
}

void spm_control_mtcmos_internal(spm_mtcmos_type_t mtcmos, spm_mtcmos_ctrl_t ctrl)
{
    bool audio_clock_status;
    if (mtcmos == SPM_MTCMOS_CONN_TOP_ON) {
        if (ctrl == SPM_MTCMOS_PWR_DISABLE) {
            /* CONN TOP ON MTCMOS OFF */
            *SPM_CONN_TOP_ON_PWR_CON = 0x1D; // [4]: clk_dis = 1
            *SPM_CONN_TOP_ON_PWR_CON = 0x1F; // [1]: iso = 1
            *SPM_CONN_TOP_ON_PWR_CON = 0x1E;   // [0]: rstb = 0
            *SPM_CONN_TOP_ON_PWR_CON = 0x1A;  // [2]: pwr_on = 0
            *SPM_CONN_TOP_ON_PWR_CON = 0x12;  // [3]: pwr_on_2nd = 0
            hal_gpt_delay_us(1);
        } else {
            /* CONN TOP ON MTCMOS ON */
            *SPM_CONN_TOP_ON_PWR_CON = 0x16; // [2]: pwr_on = 1
            *SPM_CONN_TOP_ON_PWR_CON = 0x1E; // [3]: pwr_on_2nd = 1
            *SPM_CONN_TOP_ON_PWR_CON = 0xE;   // [4]: clk_dis = 0
            *SPM_CONN_TOP_ON_PWR_CON = 0x1E;  // [4]: clk_dis = 1
            *SPM_CONN_TOP_ON_PWR_CON = 0x1C;  // [4]: clk_dis = 1, iso = 0
            *SPM_CONN_TOP_ON_PWR_CON = 0x1D;  // [0]: rstb = 1
            *SPM_CONN_TOP_ON_PWR_CON = 0xD;   // [4]: clk_dis = 0
            hal_gpt_delay_us(1);
        }
        return;
    }

    if (mtcmos == SPM_MTCMOS_CONN_TOP_OFF) {
        if (ctrl == SPM_MTCMOS_PWR_DISABLE) {
            /* CONN TOP OFF MTCMOS OFF */
            *SPM_CONN_TOP_OFF_PWR_CON = 0x1D; // [4]: clk_dis = 1
            *SPM_CONN_TOP_OFF_PWR_CON = 0x1F; // [1]: iso = 1
            *SPM_CONN_TOP_OFF_PWR_CON = 0x1E;   // [0]: rstb = 0
            *SPM_CONN_TOP_OFF_PWR_CON = 0x1A;  // [2]: pwr_on = 0
            *SPM_CONN_TOP_OFF_PWR_CON = 0x12;  // [3]: pwr_on_2nd = 0
            hal_gpt_delay_us(1);
        } else {
            /* CONN TOP OFF MTCMOS ON */
            *SPM_CONN_TOP_OFF_PWR_CON = 0x16; // [2]: pwr_on = 1
            *SPM_CONN_TOP_OFF_PWR_CON = 0x1E; // [3]: pwr_on_2nd = 1
            *SPM_CONN_TOP_OFF_PWR_CON = 0xE;   // [4]: clk_dis = 0
            *SPM_CONN_TOP_OFF_PWR_CON = 0x1E;  // [4]: clk_dis = 1
            *SPM_CONN_TOP_OFF_PWR_CON = 0x1C;  // [4]: clk_dis = 1, iso = 0
            *SPM_CONN_TOP_OFF_PWR_CON = 0x1D;  // [0]: rstb = 1
            *SPM_CONN_TOP_OFF_PWR_CON = 0xD;   // [4]: clk_dis = 0
            hal_gpt_delay_us(1);
        }
        return;
    }

    if (mtcmos == SPM_MTCMOS_AUDIO) {
        audio_clock_status = hal_clock_is_enabled(HAL_CLOCK_CG_AUD_INTBUS);
        if (audio_clock_status == false) {
            hal_clock_enable(HAL_CLOCK_CG_AUD_INTBUS);
        }
        if (ctrl == SPM_MTCMOS_PWR_DISABLE) {
            /* AUDIO MTCMOS OFF */
            *SPM_AUDIO_PWR_CON = 0x1D; // [4]: clk_dis = 1
            *SPM_AUDIO_PWR_CON = 0x1F; // [1]: iso = 1

            // --- AUDIO SRAM begin ------
            *SPM_AUDIO_AFE_MEM_IF_CONTROL_1   = 0x00030303;//[24]:MEM_ISO_EN_B [17:16]:RET2N [9:8]:RET1N [1:0]:PGEN
            *SPM_AUDIO_ASRC1_CONTROL_1        = 0x00030303;//[24]:MEM_ISO_EN_B [17:16]:RET2N [9:8]:RET1N [1:0]:PGEN
            *SPM_AUDIO_ASRC2_CONTROL_1        = 0x00030303;//[24]:MEM_ISO_EN_B [17:16]:RET2N [9:8]:RET1N [1:0]:PGEN

            *SPM_AUDIO_G_CONTROL            = 0x00010101;//[24]:MEM_ISO_EN_B [16]:RET2N [8]:RET1N [0]:PGEN
            *SPM_AUDIO_DFX_CH01_CONTROL     = 0x00030303;//[24]:MEM_ISO_EN_B [17:16]:RET2N [9:8]:RET1N [1:0]:PGEN
            *SPM_AUDIO_DFX_CH23_CONTROL     = 0x00030303;//[24]:MEM_ISO_EN_B [17:16]:RET2N [9:8]:RET1N [1:0]:PGEN
            *SPM_AUDIO_DFX_CH45_CONTROL     = 0x00030303;//[24]:MEM_ISO_EN_B [17:16]:RET2N [9:8]:RET1N [1:0]:PGEN
            *SPM_AUDIO_UL1_CONTROL          = 0x00010101;//[24]:MEM_ISO_EN_B [16]:RET2N [8]:RET1N [0]:PGEN
            *SPM_AUDIO_UL2_CONTROL          = 0x00010101;//[24]:MEM_ISO_EN_B [16]:RET2N [8]:RET1N [0]:PGEN
            *SPM_AUDIO_UL3_CONTROL          = 0x00010101;//[24]:MEM_ISO_EN_B [16]:RET2N [8]:RET1N [0]:PGEN
            *SPM_AUDIO_UL4_CONTROL          = 0x00010101;//[24]:MEM_ISO_EN_B [16]:RET2N [8]:RET1N [0]:PGEN
            *SPM_AUDIO_DL_CONTROL           = 0x00010101;//[24]:MEM_ISO_EN_B [16]:RET2N [8]:RET1N [0]:PGEN

            // --- AUDIO SRAM end ------
            *SPM_AUDIO_PWR_CON = 0x1E;   // [0]: rstb = 0
            *SPM_AUDIO_PWR_CON = 0x1A;  // [2]: pwr_on = 0
            *SPM_AUDIO_PWR_CON = 0x12;  // [3]: pwr_on_2nd = 0

            //turn AUDIO protect_en=1
            *INFRA_CFG_MTCMOS3 = 0x1;
            //wait AUDIO protect ready=1
            while (*INFRA_CFG_MTCMOS3 != 0x101);
        } else {
            /* AUDIO MTCMOS ON */
            *SPM_AUDIO_PWR_CON = 0x16; // [2]: pwr_on = 1
            *SPM_AUDIO_PWR_CON = 0x1E; // [3]: pwr_on_2nd = 1

            // --- AUDIO SRAM begin ------
            *SPM_AUDIO_AFE_MEM_IF_CONTROL_1 = 0x00000300;//[24]:MEM_ISO_EN_B [17:16]:RET2N [9:8]:RET1N [1:0]:PGEN
            *SPM_AUDIO_ASRC1_CONTROL_1      = 0x00000300;//[24]:MEM_ISO_EN_B [17:16]:RET2N [9:8]:RET1N [1:0]:PGEN
            *SPM_AUDIO_ASRC2_CONTROL_1      = 0x00000300;//[24]:MEM_ISO_EN_B [17:16]:RET2N [9:8]:RET1N [1:0]:PGEN

            *SPM_AUDIO_G_CONTROL            = 0x00000100;//[24]:MEM_ISO_EN_B [16]:RET2N [8]:RET1N [0]:PGEN
            *SPM_AUDIO_DFX_CH01_CONTROL     = 0x00000300;//[24]:MEM_ISO_EN_B [17:16]:RET2N [9:8]:RET1N [1:0]:PGEN
            *SPM_AUDIO_DFX_CH23_CONTROL     = 0x00000300;//[24]:MEM_ISO_EN_B [17:16]:RET2N [9:8]:RET1N [1:0]:PGEN
            *SPM_AUDIO_DFX_CH45_CONTROL     = 0x00000300;//[24]:MEM_ISO_EN_B [17:16]:RET2N [9:8]:RET1N [1:0]:PGEN
            *SPM_AUDIO_UL1_CONTROL          = 0x00000100;//[24]:MEM_ISO_EN_B [16]:RET2N [8]:RET1N [0]:PGEN
            *SPM_AUDIO_UL2_CONTROL          = 0x00000100;//[24]:MEM_ISO_EN_B [16]:RET2N [8]:RET1N [0]:PGEN
            *SPM_AUDIO_UL3_CONTROL          = 0x00000100;//[24]:MEM_ISO_EN_B [16]:RET2N [8]:RET1N [0]:PGEN
            *SPM_AUDIO_UL4_CONTROL          = 0x00000100;//[24]:MEM_ISO_EN_B [16]:RET2N [8]:RET1N [0]:PGEN
            *SPM_AUDIO_DL_CONTROL           = 0x00000100;//[24]:MEM_ISO_EN_B [16]:RET2N [8]:RET1N [0]:PGEN

            // --- AUDIO SRAM end ------
            *SPM_AUDIO_PWR_CON = 0xE;   // [4]: clk_dis = 0
            *SPM_AUDIO_PWR_CON = 0x1E;  // [4]: clk_dis = 1
            *SPM_AUDIO_PWR_CON = 0x1C;  // [4]: clk_dis = 1, iso = 0
            *SPM_AUDIO_PWR_CON = 0x1D;  // [0]: rstb = 1

            // --- AUDIO SRAM begin ------
            *SPM_AUDIO_AFE_MEM_IF_CONTROL_1 = 0x01000300;//[24]:MEM_ISO_EN_B [17:16]:RET2N [9:8]:RET1N [1:0]:PGEN
            *SPM_AUDIO_ASRC1_CONTROL_1      = 0x01000300;//[24]:MEM_ISO_EN_B [17:16]:RET2N [9:8]:RET1N [1:0]:PGEN
            *SPM_AUDIO_ASRC2_CONTROL_1      = 0x01000300;//[24]:MEM_ISO_EN_B [17:16]:RET2N [9:8]:RET1N [1:0]:PGEN

            *SPM_AUDIO_G_CONTROL            = 0x01000100;//[24]:MEM_ISO_EN_B [16]:RET2N [8]:RET1N [0]:PGEN
            *SPM_AUDIO_DFX_CH01_CONTROL     = 0x01000300;//[24]:MEM_ISO_EN_B [17:16]:RET2N [9:8]:RET1N [1:0]:PGEN
            *SPM_AUDIO_DFX_CH23_CONTROL     = 0x01000300;//[24]:MEM_ISO_EN_B [17:16]:RET2N [9:8]:RET1N [1:0]:PGEN
            *SPM_AUDIO_DFX_CH45_CONTROL     = 0x01000300;//[24]:MEM_ISO_EN_B [17:16]:RET2N [9:8]:RET1N [1:0]:PGEN
            *SPM_AUDIO_UL1_CONTROL          = 0x01000100;//[24]:MEM_ISO_EN_B [16]:RET2N [8]:RET1N [0]:PGEN
            *SPM_AUDIO_UL2_CONTROL          = 0x01000100;//[24]:MEM_ISO_EN_B [16]:RET2N [8]:RET1N [0]:PGEN
            *SPM_AUDIO_UL3_CONTROL          = 0x01000100;//[24]:MEM_ISO_EN_B [16]:RET2N [8]:RET1N [0]:PGEN
            *SPM_AUDIO_UL4_CONTROL          = 0x01000100;//[24]:MEM_ISO_EN_B [16]:RET2N [8]:RET1N [0]:PGEN
            *SPM_AUDIO_DL_CONTROL           = 0x01000100;//[24]:MEM_ISO_EN_B [16]:RET2N [8]:RET1N [0]:PGEN

            // --- AUDIO SRAM end ------
            *SPM_AUDIO_PWR_CON = 0xD;   // [4]: clk_dis = 0

            //turn AUDIO protect_en=0
            *INFRA_CFG_MTCMOS3 = 0x0;
            //wait AUDIO protect ready=0
            while (*INFRA_CFG_MTCMOS3 != 0x0);
        }
        if (audio_clock_status == false) {
            hal_clock_disable(HAL_CLOCK_CG_AUD_INTBUS);
        }
        return;
    }
}

void spm_mask_wakeup_source(uint32_t wakeup_source)
{
    if (wakeup_source == HAL_SLEEP_MANAGER_WAKEUP_SOURCE_ALL) {
        *SPM_DSP_WAKEUP_SOURCE_MASK = 0xFFFFFFFF;
    } else {
        *SPM_DSP_WAKEUP_SOURCE_MASK |= (1 << wakeup_source);
    }
}

void spm_unmask_wakeup_source(uint32_t wakeup_source)
{
    if (wakeup_source == HAL_SLEEP_MANAGER_WAKEUP_SOURCE_ALL) {
        *SPM_DSP_WAKEUP_SOURCE_MASK = 0x0;
    } else {
        *SPM_DSP_WAKEUP_SOURCE_MASK &= ~(1 << wakeup_source);
    }
}

uint32_t spm_control_mtcmos(spm_mtcmos_type_t mtcmos, spm_mtcmos_ctrl_t ctrl)
{
    if (mtcmos == SPM_MTCMOS_CONN_TOP_ON) {
        if (ctrl == SPM_MTCMOS_PWR_DISABLE) {
            if (mtcmos_resource.conn_top_on == 0) {
                spm_control_mtcmos_internal(SPM_MTCMOS_CONN_TOP_ON, SPM_MTCMOS_PWR_DISABLE);
                return 0;
            }
            mtcmos_resource.conn_top_on--;
            if (mtcmos_resource.conn_top_on == 0) {
                spm_control_mtcmos_internal(SPM_MTCMOS_CONN_TOP_ON, SPM_MTCMOS_PWR_DISABLE);
            }
        } else {
            mtcmos_resource.conn_top_on++;
            if (mtcmos_resource.conn_top_on == 1) {
                spm_control_mtcmos_internal(SPM_MTCMOS_CONN_TOP_ON, SPM_MTCMOS_PWR_ENABLE);
            }
        }
        return (mtcmos_resource.conn_top_on);
    }

    if (mtcmos == SPM_MTCMOS_CONN_TOP_OFF) {
        if (ctrl == SPM_MTCMOS_PWR_DISABLE) {
            if (mtcmos_resource.conn_top_off == 0) {
                spm_control_mtcmos_internal(SPM_MTCMOS_CONN_TOP_OFF, SPM_MTCMOS_PWR_DISABLE);
                return 0;
            }
            mtcmos_resource.conn_top_off--;
            if (mtcmos_resource.conn_top_off == 0) {
                spm_control_mtcmos_internal(SPM_MTCMOS_CONN_TOP_OFF, SPM_MTCMOS_PWR_DISABLE);
            }
        } else {
            mtcmos_resource.conn_top_off++;
            if (mtcmos_resource.conn_top_off == 1) {
                spm_control_mtcmos_internal(SPM_MTCMOS_CONN_TOP_OFF, SPM_MTCMOS_PWR_ENABLE);
            }
        }
        return (mtcmos_resource.conn_top_off);
    }

    if (mtcmos == SPM_MTCMOS_AUDIO) {
        if (ctrl == SPM_MTCMOS_PWR_DISABLE) {
            if (mtcmos_resource.audio == 0) {
                spm_control_mtcmos_internal(SPM_MTCMOS_AUDIO, SPM_MTCMOS_PWR_DISABLE);
                return 0;
            }
            mtcmos_resource.audio--;
            if (mtcmos_resource.audio == 0) {
                spm_control_mtcmos_internal(SPM_MTCMOS_AUDIO, SPM_MTCMOS_PWR_DISABLE);
            }
        } else {
            mtcmos_resource.audio++;
            if (mtcmos_resource.audio == 1) {
                spm_control_mtcmos_internal(SPM_MTCMOS_AUDIO, SPM_MTCMOS_PWR_ENABLE);
            }
        }
        return (mtcmos_resource.audio);
    }
    return 0;
}

void spm_debug_io(unsigned int debug_bus)
{
    //Set GPIO
#if 0
    hal_pinmux_set_function(HAL_GPIO_2, 11);
    hal_pinmux_set_function(HAL_GPIO_3, 11);
    hal_pinmux_set_function(HAL_GPIO_4, 11);
    hal_pinmux_set_function(HAL_GPIO_5, 11);

    hal_pinmux_set_function(HAL_GPIO_6, 11);
    hal_pinmux_set_function(HAL_GPIO_7, 11);
#endif


    //*TOP_DEBUG(0xA2010008) = 0x14;
    *(volatile uint32_t *)(0xA2010008) = 0x14;
    //*INFRA_CFG_DBGMON__AO_DBGMON_SEL(0xA2200030)  = 0x0
    *(volatile uint32_t *)(0xA2200030) = 0x0;
    //*SPM_DEBUG_CON(0xA21103D4)=0x1;
    *(volatile uint32_t *)(0xA21103D4) = 0x1;
    //0xA21103D0
    *SPM_SPM_DEBUG_SELECT = debug_bus;

    SPM_MCE_MSGID_I("SPM_SPM_DEBUG_SELECT=0x%x\r\n\r\n", 1, *SPM_SPM_DEBUG_SELECT);

}

void spm_check_debug_bus()
{
    spm_debug_io(0x43424342);

    int i, delay = 0;;

    while (1) {
        *SPM_STATE_STATUS = i;
        for (delay = 0; delay < 512; delay++);
        if (i++ >= 512) {
            i = 0;
        }
    }
}

void spm_audio_lowpower_setting(spm_sleep_state_t sleep_state, spm_ctrl_t enable)
{
    if (sleep_state == SPM_STATE1) {
        if (enable == SPM_ENABLE) {
            *SPM_SPM_STATE_CONTROL_0 |= 0x1;        /* Lock S1 */
        } else {
            *SPM_SPM_STATE_CONTROL_0 &= 0xFFFFFFFE; /* Unlock S1 */
        }
        return;
    }

    if (sleep_state == SPM_STATE4) {
        if (enable == SPM_ENABLE) {
            //*((volatile uint32_t *)(0xA2120B00)) |= 0x1;              /* SYS CLK Force on */
            spm_force_sleep_state4(SPM_AUDIO_REQUEST, SPM_ENABLE);      /* Lock S4 */
        } else {
            //*((volatile uint32_t *)(0xA2120B00)) &= 0xFFFFFFFE;        /* SYS CLK Force on */
            spm_force_sleep_state4(SPM_AUDIO_REQUEST, SPM_DISABLE);     /* Unlock S4 */
        }
        return;
    }
}

void spm_force_sleep_state4(spm_request_t user,spm_ctrl_t enable)
{
    static uint8_t resource_control = 0;

    if (enable == SPM_ENABLE) {
        resource_control |= (1 << user);
    } else {
        resource_control &= ~(1 << user);
    }

    if (resource_control == 0) {
        *SPM_SPM_STATE_CONTROL_0 &= 0xFEFFFFFF;     /* Unlock S4 */
    } else {
        *SPM_SPM_STATE_CONTROL_0 |= 0x01000000;     /* Lock S4 */
    }
}

void spm_force_sleep_state3(spm_request_t user,spm_ctrl_t enable)
{
    static uint8_t resource_control = 0;

    if (enable == SPM_ENABLE) {
        resource_control |= (1 << user);
    } else {
        resource_control &= ~(1 << user);
    }

    if (resource_control == 0) {
        *SPM_SPM_STATE_CONTROL_0 &= 0xFFFEFFFF;     /* Unlock S3 */
    } else {
        *SPM_SPM_STATE_CONTROL_0 |= 0x00010000;     /* Lock S3 */
    }
}
#endif
