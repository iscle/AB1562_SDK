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

#if defined(MTK_OFFLINELOG_ENABLE) || defined(MTK_MINIDUMP_ENABLE)
#include <string.h>
#include "offline_dump.h"
#include "syslog_debug.h"
#include "mux.h"
#include "mux_port_device.h"

#define ABANDON_LEN 256

/* offline log variable */
extern bool g_offline_log_is_enable;
extern bool g_offline_log_is_busy;

typedef struct {
    uint32_t dump_buffer_addr;      /* Start address of dump buffer, view with CPU 0, other CPU should do remap */
    uint32_t dump_buffer_size;      /* Size of dump buffer */
    uint32_t dump_buffer_pos;       /* Current write position in dump buffer, view with relative address */
} offline_dump_share_variable_t;

static offline_dump_share_variable_t offline_dump_share_variable;
static volatile offline_dump_share_variable_t *g_offline_dump_share_variable = &offline_dump_share_variable;

bool syslog_ram_flash_init(offline_dump_region_type_t region_type)
{
    uint32_t cell_size;
    mux_port_config_t log_port_config;

    memset((uint8_t *)g_offline_dump_share_variable, 0, sizeof(offline_dump_share_variable_t));
    memset((uint8_t *)&log_port_config, 0, sizeof(mux_port_config_t));

    if (region_type >= OFFLINE_REGION_MAX) {
        return false; /* region error */
    }

    if(mux_query_user_port_setting("SYSLOG", (mux_port_config_t *)&log_port_config) != MUX_STATUS_OK) {
        return false; /* query setting error */
    }

    if((uint32_t *)&log_port_config.tx_buf_addr == NULL || (log_port_config.tx_buf_size == 0)) {
        return false; /* illegal parameter */
    }

    if (offline_dump_region_query_cell_size(region_type, &cell_size) == false) {
        return false;
    }

    if ((log_port_config.tx_buf_size > cell_size) || (cell_size == 0)) {
        return false; /* buffer size is too large */
    }

    /* SYSRAM sharing buffer use for dumo memory */
    g_offline_dump_share_variable->dump_buffer_addr = log_port_config.tx_buf_addr;
    g_offline_dump_share_variable->dump_buffer_size = log_port_config.tx_buf_size;
    g_offline_dump_share_variable->dump_buffer_pos  = log_port_config.sw_wptr;

    /* update sw_wrpt */
    if((g_offline_dump_share_variable->dump_buffer_pos + ABANDON_LEN) >= g_offline_dump_share_variable->dump_buffer_size){
        g_offline_dump_share_variable->dump_buffer_pos = ABANDON_LEN -(g_offline_dump_share_variable->dump_buffer_size - g_offline_dump_share_variable->dump_buffer_pos);
    }else{
        g_offline_dump_share_variable->dump_buffer_pos += ABANDON_LEN;
    }

    /* Reserve for the header */
    g_offline_dump_share_variable->dump_buffer_size = g_offline_dump_share_variable->dump_buffer_size - ABANDON_LEN;

    return true;
}

void internal_offline_dump_region_write(offline_dump_region_type_t region_type)
{
    uint32_t left_dump_size, curr_dump_size, curr_flash_addr, curr_dump_pos;
    uint32_t cell_valid_size;
    /* total dump size */
    left_dump_size = g_offline_dump_share_variable->dump_buffer_size;
    /* query dump sw_wptr */
    curr_dump_pos = g_offline_dump_share_variable->dump_buffer_pos;
    /* query offline dump cell_valid_size = 8k - header_size */
    if (offline_dump_region_query_cell_valid_size(region_type, &cell_valid_size) == false) {
        return ;
    }
    while (left_dump_size) {
        if (left_dump_size > cell_valid_size) {
            /* dump ram size > flash cell size */
            curr_dump_size = cell_valid_size;
        } else {
            /* dump ram size <= flash cell size */
            curr_dump_size = left_dump_size; 
        }
        left_dump_size -= curr_dump_size;
        /* get flash write pointer ==> alloc 8k - header region */
        offline_dump_region_alloc(region_type, &curr_flash_addr);

        if (curr_dump_pos < ABANDON_LEN) {
            offline_dump_region_write(region_type, curr_flash_addr,
                                        (uint8_t *)(g_offline_dump_share_variable->dump_buffer_addr + curr_dump_pos),
                                        g_offline_dump_share_variable->dump_buffer_size);
        } else {
            offline_dump_region_write(region_type, curr_flash_addr,
                                        (uint8_t *)(g_offline_dump_share_variable->dump_buffer_addr + curr_dump_pos),
                                        g_offline_dump_share_variable->dump_buffer_size + ABANDON_LEN - curr_dump_pos);
            /* update write pointer */
            curr_flash_addr += (g_offline_dump_share_variable->dump_buffer_size + ABANDON_LEN - curr_dump_pos);

            offline_dump_region_write(region_type, curr_flash_addr,
                                        (uint8_t *)(g_offline_dump_share_variable->dump_buffer_addr),
                                        curr_dump_pos - ABANDON_LEN);
        }

        offline_dump_region_write_end(region_type, curr_dump_size);
    }
}

void exception_syslog_offline_dump_callback(void)
{
    bool init_status = false;

    init_status = syslog_ram_flash_init(OFFLINE_REGION_EXCEPTION_LOG);

    if(init_status == true) {
        internal_offline_dump_region_write(OFFLINE_REGION_EXCEPTION_LOG);
    }
}

void offline_log_dump_handler(void)
{
    bool init_status = false;

    if(g_offline_log_is_enable == false) {
        return ;
    }

    init_status = syslog_ram_flash_init(OFFLINE_REGION_OFFLINE_LOG);

    if(init_status == true) {
        internal_offline_dump_region_write(OFFLINE_REGION_OFFLINE_LOG);
    }
}

#else

void exception_syslog_offline_dump_callback(void)
{
    return ;
}

void offline_log_dump_handler(void)
{
    return ;
}

#endif /* defined(MTK_OFFLINELOG_ENABLE) || defined(MTK_MINIDUMP_ENABLE) */







