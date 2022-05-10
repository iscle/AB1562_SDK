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

#ifndef __OFFLINE_DUMP_H__
#define __OFFLINE_DUMP_H__

#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "hal_flash.h"

#ifdef HAL_RTC_MODULE_ENABLED
#include "hal_rtc.h"
#endif

#define OFFLINE_BUILD_TIME_COUNT         1
#define OFFLINE_BUILD_INFO_SIZE          48
#define OFFLINE_BUILD_INFO_TOTAL_SIZE    OFFLINE_BUILD_TIME_COUNT * OFFLINE_BUILD_INFO_SIZE

typedef struct {
    uint8_t status;
    uint8_t reserved[3];
    uint16_t header_checksum;
    uint16_t header_size;
    uint8_t dump_version;
    uint8_t payload_type;
    uint8_t sdk_ver_cnt;
    uint8_t build_time_cnt;
    uint8_t magic_name[32];
    uint32_t payload_size;
    uint32_t payload_real_size;
    uint32_t seq_number;
    uint8_t rtc_time[8];
    uint8_t sdk_version[OFFLINE_BUILD_INFO_SIZE];
    uint8_t build_time[OFFLINE_BUILD_INFO_TOTAL_SIZE];
} offline_dump_header_t;

typedef struct {
    uint32_t BinaryId;
    uint32_t PartitionId;
    uint32_t LoadAddressHigh;
    uint32_t LoadAddressLow;
    uint32_t BinaryLengthHigh;
    uint32_t BinaryLengthLow;
    uint32_t ExecutionAddress;
    uint32_t CellSize;
    uint32_t CellCount;
    uint32_t ReservedItem0;
    uint32_t ReservedItem1;
    uint32_t ReservedItem2;
} OfflineDumpPartitionTable_T;

/* offline dump region partition map */
extern uint8_t _offline_dump_partition_start_[];
#define OFFLINE_DUMP_ROM_BASE       ((uint32_t)_offline_dump_partition_start_ & 0x0FFFFFFF)
#define PARTITION_TABLE             ((OfflineDumpPartitionTable_T *)OFFLINE_DUMP_ROM_BASE)

/* offilne dump region configure */
#define SERIAL_FLASH_BLOCK_SIZE                     4096

/* Offline dump version */
#define OFFLINE_REGION_VERSION                      0x01

/* serial flash information */
#ifdef HAL_FLASH_MODULE_ENABLED
#define PORT_FLASH_BLOCK_SIZE                       HAL_FLASH_BLOCK_4K
#define PORT_FLASH_READ(address, buffer, length)    hal_flash_read(address, buffer, length)
#define PORT_FLASH_WRITE(address, data, length)     hal_flash_write(address, data, length)
#define PORT_FLASH_ERASE(address, block_size)       hal_flash_erase(address, block_size)
#else
#define PORT_FLASH_BLOCK_SIZE                       0
#define PORT_FLASH_READ(address, buffer, length)
#define PORT_FLASH_WRITE(address, data, length)
#define PORT_FLASH_ERASE(address, block_size)
#endif

typedef void (*offline_callback_t)(void);

typedef enum {
    OFFLINE_REGION_EXCEPTION_LOG = 0,
    OFFLINE_REGION_MINI_DUMP,
    OFFLINE_REGION_OFFLINE_LOG,
    OFFLINE_REGION_MAX,
} offline_dump_region_type_t;

bool offline_dump_region_init(void);

bool offline_dump_region_alloc(offline_dump_region_type_t region_type, uint32_t *p_start_addr);

bool offline_dump_region_write(offline_dump_region_type_t region_type, uint32_t curr_addr, uint8_t *data, uint32_t length);

bool offline_dump_region_write_end(offline_dump_region_type_t region_type, uint32_t total_length);

bool offline_dump_region_read(offline_dump_region_type_t region_type, uint32_t curr_addr, uint8_t *buf, uint32_t length);

bool offline_dump_region_query_seq_range(offline_dump_region_type_t region_type, uint32_t *p_min_seq, uint32_t *p_max_seq);

bool offline_dump_region_query_by_seq(offline_dump_region_type_t region_type, uint32_t seq, uint32_t *p_start_addr, uint32_t *p_total_length);

bool offline_dump_region_query_cell_size(offline_dump_region_type_t region_type, uint32_t *p_cell_size);

bool offline_dump_region_query_cell_valid_size(offline_dump_region_type_t region_type, uint32_t *p_valid_size);

bool offline_dump_register_head_callback(offline_dump_region_type_t region_type, offline_callback_t user_callback);
<<<<<<< HEAD
=======
bool offline_dump_register_tail_callback(offline_dump_region_type_t region_type, offline_callback_t user_callback);

void offline_dump_callback_head_handle(offline_dump_region_type_t region_type);
void offline_dump_callback_tail_handle(offline_dump_region_type_t region_type);
>>>>>>> db20e11 (second commit)

#endif

