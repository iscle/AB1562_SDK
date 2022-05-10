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

#ifndef __HAL_DUMP_PERIPHERALS_REGISTER_H__
#define __HAL_DUMP_PERIPHERALS_REGISTER_H__

#include "hal_define.h"
#include "ab1562.h"
#include "memory_map.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HAL_DUMP_MODULE_REGISTER_ENABLE
/*add your module here only dump RG that not read-update.
*  Example:  {"<module_name>", <dump_start_address>, <dump_stop_address>, 1},
*
*/

/*

        {"I2S_DMA",   (unsigned int *)(0xC9000000) , (unsigned int *)(0xC9000000 + 0x870), 1}, \

*/
#define HAL_DUMP_PERIPHERAL_REGISTER \
        {"uart0_0",  (unsigned int *)(UART0_BASE + 0x08),      (unsigned int *)(UART0_BASE + 0x0C), 1},  \
        {"uart0_1",  (unsigned int *)(UART0_BASE + 0x14),      (unsigned int *)(UART0_BASE + 0x24), 1},  \
        {"uart0_2",  (unsigned int *)(UART0_BASE + 0x30),      (unsigned int *)(UART0_BASE + 0x60), 1},  \
        {"uart1_0",  (unsigned int *)(UART1_BASE + 0x08),      (unsigned int *)(UART1_BASE + 0x0C), 1},  \
        {"uart1_1",  (unsigned int *)(UART1_BASE + 0x14),      (unsigned int *)(UART1_BASE + 0x24), 1},  \
        {"uart1_2",  (unsigned int *)(UART1_BASE + 0x30),      (unsigned int *)(UART1_BASE + 0x60), 1},  \
        {"DSP_DMA",   (unsigned int *)(0xA0010000) , (unsigned int *)(0xA0010000 + 0x12C), 1}, \
        {"SLOW0_DMA", (unsigned int *)(0xA0040000) , (unsigned int *)(0xA0040000 + 0x430), 1}, \
        {"SLOW1_DMA", (unsigned int *)(0xA30C0000) , (unsigned int *)(0xA30C0000 + 0x630), 1}, \
        {"gpio",     (unsigned int *)(GPIO_BASE + 0x00),       (unsigned int *)(GPIO_BASE + 0x100), 1},  \
        {"bt",       (unsigned int *)(BT_BASE),         (unsigned int *)(BT_BASE + 0x96F), 1},  \
        {"bt_modem", (unsigned int *)(BT_MODEM_BASE),   (unsigned int *)(BT_MODEM_BASE + 0xCE8), 1},  \
        {"bt_sleep", (unsigned int *)(BT_SLEEP_BASE),   (unsigned int *)(BT_SLEEP_BASE + 0x20C), 1},  \



#ifdef __cplusplus
}
#endif

#endif /* __HAL_PLATFORM_H__ */

