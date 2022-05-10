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

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "mux.h"
#include "mux_port.h"
#include "mux_port_device.h"
#include "syslog_debug.h"
#include "FreeRTOS.h"
#include "exception_handler.h"
//#include "nvdm.h"



extern port_mux_device_ops_t *g_mux_device_ops_array[];


/* create mux debug module */
log_create_module(MUX_PORT, PRINT_LEVEL_INFO);

/* define mux init flow */
#define MUX_INIT_NONE      0
#define MUX_INIT_NORMAL    1
#define MUX_INIT_EXCEPTION 2

/* define one port max support user number */
#define MAX_MUX_USER_ON_ONE_PORT    10

/* define the max cached rx data package */
#define MAX_MUX_USER_RX_DATA_CNT 3

/* define mux handle magic symble */
#define HANDLE_MAGIC_NUMBER 0x55AA
#define handle_to_port(handle) (handle & 0xFF)
#define handle_to_user_id(handle) ((handle >> 8) & 0xFF)
#define user_id_to_handle(user_id, port) ((HANDLE_MAGIC_NUMBER << 16) | (user_id << 8) | port)

typedef struct {
    uint32_t wptr;
    uint32_t rptr;
    uint32_t is_empty;
    uint8_t *p_data[MAX_MUX_USER_RX_DATA_CNT];
    uint32_t data_size[MAX_MUX_USER_RX_DATA_CNT];
    uint32_t drop_count;
    mux_callback_t callback;
    void *user_data;
    uint32_t need_tx_callback;
    uint32_t need_rx_callback;
    const char *user_name;
    mux_port_t port;
} mux_user_config_t;

#ifdef MTK_SINGLE_CPU_ENV
ATTR_ZIDATA_IN_TCM static volatile mux_port_config_t g_mux_port_configs[MUX_PORT_END];
ATTR_ZIDATA_IN_TCM static volatile mux_user_config_t g_mux_user_configs[MAX_MUX_USER_COUNT];
#else
#include "hal_resource_assignment.h"
static volatile mux_port_config_t *g_mux_port_configs = (volatile mux_port_config_t *)HW_SYSRAM_PRIVATE_MEMORY_MUX_VAR_PORT_START; //56 * N
static volatile mux_user_config_t *g_mux_user_configs = (volatile mux_user_config_t *)HW_SYSRAM_PRIVATE_MEMORY_MUX_VAR_USER_START; //64 * N
#endif/* MTK_SINGLE_CPU_ENV */

#define HANDLE_MAGIC_NUMBER 0x55AA
#define handle_to_port(handle) (handle & 0xFF)
#define handle_to_user_id(handle) ((handle >> 8) & 0xFF)
#define user_id_to_handle(user_id, port) ((HANDLE_MAGIC_NUMBER << 16) | (user_id << 8) | port)

static bool handle_is_valid(mux_handle_t handle)
{
    if (((handle >> 16) & 0xFFFF) != HANDLE_MAGIC_NUMBER) {
        return false;
    }
    return true;
}

static uint32_t parse_port_type(mux_port_t port, mux_port_type_t *port_type)
{
    if (port <= MUX_UART_END) {
        *port_type = MUX_PORT_TYPE_UART;
        return port;
    } else if (port <= MUX_USB_END) {
        *port_type = MUX_PORT_TYPE_USB;
        return port - MUX_USB_BEGIN;
    } else if (port <= MUX_AIRAPP_BEGIN) {
        *port_type = MUX_PORT_TYPE_AIRAPP;
        return port - MUX_AIRAPP_END;
    }
    return 0xdeadbeef;
}

#ifdef MTK_CPU_NUMBER_0
static uint32_t buffer_check_avail_size(mux_port_t port)
{
    bool buf_full;
    mux_port_type_t port_type;
    uint32_t port_index, avail_size, hw_rptr, hw_wptr;

    port_index = parse_port_type(port, (mux_port_type_t*)&port_type);

    hw_rptr = port_mux_device_get_hw_rptr(port_type, port_index, true);
    hw_wptr = port_mux_device_get_hw_wptr(port_type, port_index, true);

    if (hw_wptr < hw_rptr) {
        avail_size = g_mux_port_configs[port].rx_buf_size - (hw_rptr - hw_wptr);
    } else if (hw_wptr > hw_rptr) {
        avail_size = hw_wptr - hw_rptr;
    } else {
        buf_full = port_mux_device_buf_is_full(port_type, port_index, true);
        if (buf_full == true) {
            avail_size = g_mux_port_configs[port].rx_buf_size;
        } else {
            avail_size = 0;
        }
    }

    return avail_size;
}

uint8_t *buffer_copy_data_in(mux_port_t port, uint8_t *src_buf, uint8_t *dst_buf, uint32_t log_size)
{
    uint32_t i, rx_buf_end;
    uint8_t *p_buf;

    rx_buf_end = g_mux_port_configs[port].rx_buf_addr + g_mux_port_configs[port].rx_buf_size;

    if ((rx_buf_end - (uint32_t)src_buf) <= log_size) {
        memcpy(dst_buf, src_buf, rx_buf_end - (uint32_t)src_buf);
        i = rx_buf_end - (uint32_t)src_buf;
        memcpy(&dst_buf[i], (uint8_t *)(g_mux_port_configs[port].rx_buf_addr), log_size - i);
        p_buf = (uint8_t *)((g_mux_port_configs[port].rx_buf_addr) + log_size - i);
    } else {
        memcpy(dst_buf, src_buf, log_size);
        p_buf = src_buf + log_size;
    }

    return p_buf;
}

bool mux_pop_one_package_data_from_rx_buff_fifo(uint32_t user_id,uint8_t *read_buf,uint32_t read_len,uint32_t *real_read_len)
{
    uint8_t *src_addr;
    uint32_t src_len;
    volatile mux_user_config_t *p_user_config = &g_mux_user_configs[user_id];

    src_addr = p_user_config->p_data[p_user_config->rptr];
    src_len = p_user_config->data_size[p_user_config->rptr];
    if(read_len<src_len)
    return false;

    memcpy(read_buf,src_addr,src_len);
    *real_read_len = src_len;
    port_mux_free(p_user_config->p_data[p_user_config->rptr]);

    p_user_config->rptr++;
    p_user_config->rptr%=MAX_MUX_USER_RX_DATA_CNT;
    if (p_user_config->wptr == p_user_config->rptr){
        p_user_config->is_empty = true;
    }

    //MUX_PORT_MSGID_I("pop FIFO:*real_read_len:%d,p_user_config->rptr:%d\r\n",*real_read_len,p_user_config->rptr);
    return true;

}

bool mux_push_one_package_data_to_rx_buff_fifo(uint32_t user_id,uint32_t hw_rx_read_ptr,uint32_t new_pkg_len,uint32_t rx_buf_addr,uint32_t rx_buf_size)
{
    uint32_t first_cpy_len;
    volatile mux_user_config_t *p_user_config;
    uint8_t *p_malloc_buff_addr;
    p_user_config = &g_mux_user_configs[user_id];
    if ((p_user_config->wptr == p_user_config->rptr) && (p_user_config->is_empty == false)){
        //push fail
        p_user_config->drop_count ++;
        MUX_PORT_MSGID_E("user_id:%d, Rx, push buffer fail, drop_count:%d,p_user_config->wptr:%d",3,user_id,p_user_config->drop_count,p_user_config->wptr);
        return false;
    }else{
        p_malloc_buff_addr = port_mux_malloc(new_pkg_len);
        if (p_malloc_buff_addr == NULL) {
            assert(0);
        }
        p_user_config->p_data[p_user_config->wptr] = p_malloc_buff_addr;
        p_user_config->data_size[p_user_config->wptr] = new_pkg_len;
        p_user_config->wptr++;
        p_user_config->wptr%=MAX_MUX_USER_RX_DATA_CNT;
        p_user_config->is_empty = false;

        if(hw_rx_read_ptr+new_pkg_len <= rx_buf_size){
            memcpy((void*)p_malloc_buff_addr,(void*)(rx_buf_addr+hw_rx_read_ptr),new_pkg_len);
        }else{
            first_cpy_len = rx_buf_size-hw_rx_read_ptr;
            memcpy((void*)p_malloc_buff_addr,(void*)(rx_buf_addr+hw_rx_read_ptr),first_cpy_len);
            memcpy((void*)(p_malloc_buff_addr+first_cpy_len),(void*)rx_buf_addr,new_pkg_len-first_cpy_len);
        }
        //MUX_PORT_MSGID_I("push FIFO:new_pkg_len:%d,p_user_config->wptr:%d\r\n",new_pkg_len,p_user_config->wptr);
        return true;
    }
}


/*Liming: There have risk of multi IRQ thread will call this handle!!!!  for example UART and DMA*/
static void irq_mux_handler(mux_port_t port, mux_event_t type)
{
    uint32_t i, user_id, port_index, total_rx_left_data_size;
    uint32_t per_cpu_irq_mask, hw_rx_read_ptr, consume_len, new_pkg_len;
    mux_port_type_t port_type;
    mux_handle_t handle;
    static volatile mux_user_config_t *user_config;
    mux_buffer_t buffers[2];
    volatile mux_port_config_t *p_rx = &g_mux_port_configs[port];

    port_index = parse_port_type(port, (mux_port_type_t*)&port_type);

    if (type == MUX_EVENT_READY_TO_READ)
    {
        total_rx_left_data_size = buffer_check_avail_size(port);
        while (total_rx_left_data_size>0)
        {
            hw_rx_read_ptr = port_mux_device_get_hw_rptr(port_type, port_index, true);
            if ((hw_rx_read_ptr + total_rx_left_data_size) > p_rx->rx_buf_size){
                buffers[0].p_buf = (uint8_t *)(p_rx->rx_buf_addr + hw_rx_read_ptr);
                buffers[0].buf_size = p_rx->rx_buf_size - hw_rx_read_ptr;
                buffers[1].p_buf = (uint8_t *)(p_rx->rx_buf_addr);
                buffers[1].buf_size = total_rx_left_data_size - buffers[0].buf_size;
                consume_len = 0;
                new_pkg_len = 0;
                p_rx->protocol.rx_protocol_callback(&handle, buffers, 2,&consume_len, &new_pkg_len,p_rx->protocol.user_data);
                MUX_PORT_MSGID_I("there is two receive buffer !!!######hw_rx_read_ptr:%d consume_len:%d new_pkg_len:%d",3,(int)hw_rx_read_ptr,consume_len,new_pkg_len);
            }else{
                buffers[0].p_buf = (uint8_t *)(p_rx->rx_buf_addr + hw_rx_read_ptr);
                buffers[0].buf_size = total_rx_left_data_size;
                buffers[1].p_buf = NULL;
                buffers[1].buf_size = 0;
                consume_len = 0;
                new_pkg_len = 0;
                p_rx->protocol.rx_protocol_callback(&handle, buffers, 1,&consume_len, &new_pkg_len,p_rx->protocol.user_data);
            }
            if((handle_to_port(handle) != port) && (new_pkg_len != 0)){
                MUX_PORT_MSGID_E("Mux error info: port dismatch with handle-- port: 0x%x, handle:0x%08x",2,(int)port,(int)handle);
                assert(0);
            }
            if((consume_len+new_pkg_len) > total_rx_left_data_size){
                MUX_PORT_MSGID_E("Mux error info: Rx unpackage callback Error:consume_len:%d,new_pkg_len:%d,total_rx_left_data_size:%d",3,
                        (int)consume_len,(int)new_pkg_len,(int)total_rx_left_data_size);
                assert(0);
            }

             /* Move the read pointer for corrupt data and protocol header */
            port_mux_device_set_rx_hw_rptr(port_type, port_index, consume_len);
            total_rx_left_data_size -= consume_len;

            /* new package available? */
            if (new_pkg_len == 0)
            {
                break;
            }
            user_id = handle_to_user_id(handle);
            port_mux_local_cpu_enter_critical(&per_cpu_irq_mask);

            if ((hw_rx_read_ptr + consume_len) >= p_rx->rx_buf_size) {
                hw_rx_read_ptr = (hw_rx_read_ptr + consume_len) - p_rx->rx_buf_size;
            } else {
                hw_rx_read_ptr += consume_len;
            }
            mux_push_one_package_data_to_rx_buff_fifo(user_id,hw_rx_read_ptr,new_pkg_len,p_rx->rx_buf_addr,p_rx->rx_buf_size);
            port_mux_device_set_rx_hw_rptr(port_type, port_index, new_pkg_len);

            port_mux_local_cpu_exit_critical(per_cpu_irq_mask);

            user_config = &g_mux_user_configs[user_id];

            if ((user_config->user_name != NULL) && (user_config->callback != NULL)
                            && (user_config->is_empty != true))
            {
                //set need_rx_callback as flase every time before call ready to read callback
                user_config->callback(handle, MUX_EVENT_READY_TO_READ,new_pkg_len,user_config->user_data);
            }

            total_rx_left_data_size -= new_pkg_len;

            //MUX_PORT_MSGID_I("unpackage one time consume_len:%d,new_pkg_len:%d!!!  hw_rx_read_ptr:%d",3,consume_len,new_pkg_len,hw_rx_read_ptr);
        }
        //MUX_PORT_MSGID_I("unpackage done,!!!",0);
    }  else if (type == MUX_EVENT_WAKEUP_FROM_SLEEP) {
        //directly lock sleep or use lock it?
        for (i=0; i<MAX_MUX_USER_COUNT; i++) {
            user_config = &g_mux_user_configs[i];
            if ((user_config->port == port) && (user_config->user_name != NULL) && (user_config->callback != NULL)) {
                //user_config->need_tx_callback = false;
                handle = user_id_to_handle(i, user_config->port);
                user_config->callback(handle, MUX_EVENT_WAKEUP_FROM_SLEEP,0,user_config->user_data);
            }
        }
    } else {
        for (i=0; i<MAX_MUX_USER_COUNT; i++) {
            user_config = &g_mux_user_configs[i];
            if ((user_config->port == port) && (user_config->user_name != NULL) && (user_config->need_tx_callback == true) && (user_config->callback != NULL)) {
                user_config->need_tx_callback = false;
                handle = user_id_to_handle(i, user_config->port);
                user_config->callback(handle, MUX_EVENT_READY_TO_WRITE,0,user_config->user_data);
            }
        }
    }
}

mux_status_t mux_init(mux_port_t port, mux_port_setting_t *setting, mux_protocol_t *protocol_callback)
{
    uint32_t port_type, port_index;
    //exception_config_type exception_config;

    if ((port >= MUX_PORT_END) || (setting == NULL)) {
        return MUX_STATUS_ERROR_PARAMETER;
    }

    if (g_mux_port_configs[port].init_status == MUX_INIT_NORMAL) {
        return MUX_STATUS_ERROR_INITIATED;
    }

    if (g_mux_port_configs[port].init_status != MUX_INIT_NONE) {
        return MUX_STATUS_ERROR_INIT;
    }

    port_index = parse_port_type(port, (mux_port_type_t*)&port_type);
    if(port_index == 0xdeadbeef){
        return MUX_STATUS_ERROR_PARAMETER;
    }

    /* get the TX/RX buffer info */
    memcpy((void*)&g_mux_port_configs[port].user_setting,(void*)setting,sizeof(mux_port_setting_t));

    g_mux_port_configs[port].tx_buf_size = setting->tx_buffer_size;
    g_mux_port_configs[port].tx_buf_addr = (uint32_t)pvPortMalloc(g_mux_port_configs[port].tx_buf_size);
    g_mux_port_configs[port].rx_buf_size = setting->rx_buffer_size;
    g_mux_port_configs[port].rx_buf_addr = (uint32_t)pvPortMalloc(g_mux_port_configs[port].rx_buf_size);

    if(((uint32_t*)g_mux_port_configs[port].tx_buf_addr == NULL) || ((uint32_t*)g_mux_port_configs[port].rx_buf_addr == NULL)){
        vPortFree((uint32_t*)g_mux_port_configs[port].tx_buf_addr);
        vPortFree((uint32_t*)g_mux_port_configs[port].rx_buf_addr);
        return MUX_STATUS_ERROR;
    }

    if(protocol_callback == NULL){
        g_mux_port_configs[port].protocol.tx_protocol_callback = NULL;
        g_mux_port_configs[port].protocol.rx_protocol_callback = NULL;
        g_mux_port_configs[port].protocol.user_data = NULL;
    }else{
        g_mux_port_configs[port].protocol.tx_protocol_callback = protocol_callback->tx_protocol_callback;
        g_mux_port_configs[port].protocol.rx_protocol_callback = protocol_callback->rx_protocol_callback;
        g_mux_port_configs[port].protocol.user_data = protocol_callback->user_data;
    }

    if(MUX_STATUS_OK != port_mux_device_normal_init(port_type, port_index, (mux_port_config_t*)&g_mux_port_configs[port],irq_mux_handler))
    {
        g_mux_port_configs[port].tx_buf_size = 0;
        vPortFree((uint32_t*)g_mux_port_configs[port].tx_buf_addr);
        g_mux_port_configs[port].rx_buf_size = 0;
        vPortFree((uint32_t*)g_mux_port_configs[port].rx_buf_addr);
        return MUX_STATUS_ERROR_INIT_FAIL;
    }

    g_mux_port_configs[port].init_status = MUX_INIT_NORMAL;

    return MUX_STATUS_OK;
}

mux_status_t mux_deinit(mux_port_t port)
{
    uint8_t port_type, port_index,user_id;

    if (port >= MUX_PORT_END) {
        return MUX_STATUS_ERROR_PARAMETER;
    }

    if (g_mux_port_configs[port].init_status != MUX_INIT_NORMAL) {
        return MUX_STATUS_ERROR_NOT_INIT;
    }

    for (user_id = 0; user_id < MAX_MUX_USER_COUNT; user_id++) {
        if (g_mux_user_configs[user_id].user_name != NULL) {
            if(g_mux_user_configs[user_id].port == port)
                return MUX_STATUS_ERROR_SOME_USER_STILL_OPEN;
        }
    }
    port_index = parse_port_type(port, (mux_port_type_t*)&port_type);

    if(MUX_STATUS_OK != port_mux_device_deinit(port_type, port_index)){
        return MUX_STATUS_ERROR_DEINIT_FAIL;
    }

    //free memory
    if (g_mux_port_configs[port].tx_buf_addr) {
        vPortFree((uint32_t*)g_mux_port_configs[port].tx_buf_addr);
        g_mux_port_configs[port].tx_buf_addr = 0;
    }

    if (g_mux_port_configs[port].rx_buf_addr) {
        vPortFree((uint32_t*)g_mux_port_configs[port].rx_buf_addr);
        g_mux_port_configs[port].rx_buf_addr = 0;
    }

    g_mux_port_configs[port].protocol.tx_protocol_callback = NULL;
    g_mux_port_configs[port].protocol.rx_protocol_callback = NULL;
    g_mux_port_configs[port].protocol.user_data = NULL;

    g_mux_port_configs[port].init_status = MUX_INIT_NONE;
    return MUX_STATUS_OK;
}

mux_status_t mux_exception_init(mux_handle_t handle)
{
    mux_port_t port;
    uint32_t port_index;
    mux_port_type_t port_type;

    if (handle_is_valid(handle) == false) {
        return MUX_STATUS_ERROR_PARAMETER;
    }

    port = handle_to_port(handle);
    port_index = parse_port_type(port, (mux_port_type_t*)&port_type);
    port_mux_device_exception_init(port_type, port_index);
    g_mux_port_configs[port].init_status = MUX_INIT_EXCEPTION;

    return MUX_STATUS_OK;
}

mux_status_t mux_exception_send(mux_handle_t handle, const mux_buffer_t *buffers,uint32_t buffers_counter)
{
    mux_port_t port;
    uint32_t i, port_index;
    mux_port_type_t port_type;

    if (handle_is_valid(handle) == false) {
        return MUX_STATUS_ERROR_PARAMETER;
    }
    if (buffers == NULL) {
        return MUX_STATUS_ERROR;
    }

    port = handle_to_port(handle);
    port_index = parse_port_type(port, (mux_port_type_t*)&port_type);
    for (i = 0; i<buffers_counter; i++) {
        port_mux_device_exception_send(port_type, port_index, buffers[i].p_buf, buffers[i].buf_size);
    }
    return MUX_STATUS_OK;
}

mux_status_t mux_open(mux_port_t port, const char *user_name, mux_handle_t *p_handle,mux_callback_t callback,void*user_data)
{
    //mux_handle_t handle;
    uint32_t user_id, per_cpu_irq_mask;
    volatile mux_user_config_t *p_user_config;
    *p_handle = 0xdeadbeef;

    if ((port >= MUX_PORT_END) || (user_name == NULL)) {
        return MUX_STATUS_ERROR_PARAMETER;
    }
    if (g_mux_port_configs[port].init_status != MUX_INIT_NORMAL) {
        return MUX_STATUS_ERROR_NOT_INIT;
    }

    port_mux_local_cpu_enter_critical(&per_cpu_irq_mask);

    for (user_id = 0; user_id < MAX_MUX_USER_COUNT; user_id++) {
        if (g_mux_user_configs[user_id].user_name == NULL) {
            break;
        }
    }
    if (user_id >= MAX_MUX_USER_COUNT) {
        port_mux_local_cpu_exit_critical(per_cpu_irq_mask);
        return MUX_STATUS_ERROR_TOO_MANY_USERS;
    }
    p_user_config = &g_mux_user_configs[user_id];
    memset((void*)p_user_config, 0, sizeof(mux_user_config_t));
    p_user_config->is_empty = true;
    p_user_config->callback = callback;
    p_user_config->user_data = user_data;
    p_user_config->user_name = user_name;
    p_user_config->port = port;

    port_mux_local_cpu_exit_critical(per_cpu_irq_mask);
    *p_handle = user_id_to_handle(user_id, port);

    return MUX_STATUS_OK;
}

mux_status_t mux_close(mux_handle_t handle)
{
    uint32_t user_id, per_cpu_irq_mask;
    volatile mux_user_config_t *user_config;
    mux_port_t port;
    //mux_user_config_t *user_config;

    if (handle_is_valid(handle) == false) {
        return MUX_STATUS_ERROR_PARAMETER;
    }
    port = handle_to_port(handle);
    if (g_mux_port_configs[port].init_status != MUX_INIT_NORMAL) {
        return MUX_STATUS_ERROR_NOT_INIT;
    }

    user_id = handle_to_user_id(handle);
    user_config = &g_mux_user_configs[user_id];
    if(user_config->user_name == NULL)
    {
        return MUX_STATUS_USER_NOT_OPEN;
    }
    //[Qinghua] how will be if call mux_rx after do mux_close
    port_mux_local_cpu_enter_critical(&per_cpu_irq_mask);
    memset((void*)user_config, 0, sizeof(mux_user_config_t));
    //g_mux_user_configs[user_id].user_name = NULL;
    port_mux_local_cpu_exit_critical(per_cpu_irq_mask);

    return MUX_STATUS_OK;
}

mux_status_t mux_rx(mux_handle_t handle, mux_buffer_t *buffer,uint32_t *receive_done_data_len)
{
    uint32_t user_id;
    volatile mux_user_config_t *user_config;
    mux_port_t port;
    *receive_done_data_len = 0;

    if ((handle_is_valid(handle) == false) ||
        (buffer == NULL) ||
        (buffer->p_buf == NULL) || (buffer->buf_size == 0)) {
        return MUX_STATUS_ERROR_PARAMETER;
    }

    port = handle_to_port(handle);
    user_id = handle_to_user_id(handle);

    if (g_mux_port_configs[port].init_status != MUX_INIT_NORMAL) {
        return MUX_STATUS_ERROR;
    }

    user_config = &g_mux_user_configs[user_id];
    if(user_config->user_name == NULL)
    {
        return MUX_STATUS_USER_NOT_OPEN;
    }
    //always set to ture
    user_config->need_rx_callback = true;

    if (user_config->is_empty == true) {
        *receive_done_data_len = 0;
        return MUX_STATUS_OK;
    }
    if(mux_pop_one_package_data_from_rx_buff_fifo(user_id,buffer->p_buf,buffer->buf_size,receive_done_data_len)){
        return MUX_STATUS_OK;
    }else{
        return  MUX_STATUS_USER_RX_BUF_SIZE_NOT_ENOUGH;
    }
}

mux_status_t mux_query_user_port_setting(const char *user_name, mux_port_config_t *setting)
{
    uint32_t i;
    mux_port_t port;

    if(user_name == NULL || setting == NULL){
        return MUX_STATUS_ERROR_PARAMETER;
    }

    for(i=0 ; i<(sizeof(g_mux_user_configs)/(sizeof(mux_user_config_t))) ; i++) {
        if(strcmp(g_mux_user_configs[i].user_name,user_name) == 0){
            port = g_mux_user_configs[i].port;
            break;
        }
    }

    if(i >= (sizeof(g_mux_user_configs)/(sizeof(mux_user_config_t)))){
        return MUX_STATUS_ERROR;
    }

    setting->tx_buf_addr = g_mux_port_configs[port].tx_buf_addr;
    setting->tx_buf_size = g_mux_port_configs[port].tx_buf_size;
    setting->sw_wptr     = g_mux_port_configs[port].sw_wptr;

    return MUX_STATUS_OK;
}

mux_status_t mux_query_user_name(mux_handle_t handle, const char **user_name)
{
    uint8_t user_id;
    user_id = handle_to_user_id(handle);
    if(handle_is_valid(handle) == false){
        return MUX_STATUS_ERROR_PARAMETER;
    }
    if(g_mux_user_configs[user_id].user_name == NULL){
        user_name = NULL;
        return MUX_STATUS_ERROR_PARAMETER;
    }
    *user_name = g_mux_user_configs[user_id].user_name;
    return MUX_STATUS_OK;
}

mux_status_t mux_query_user_handle(mux_port_t port, const char *user_name, mux_handle_t *p_handle)
{
    uint32_t i;
    *p_handle = 0xdeadbeef;

    if (g_mux_port_configs[port].init_status != MUX_INIT_NORMAL) {
        return MUX_STATUS_ERROR;
    }
    
    for(i=0 ; i<(sizeof(g_mux_user_configs)/(sizeof(mux_user_config_t))) ; i++) {
        if(g_mux_user_configs[i].port == port){
            if(strcmp(g_mux_user_configs[i].user_name,user_name) == 0){
                *p_handle = user_id_to_handle(i, port);
                return MUX_STATUS_OK;
            }
        }
    }

    return MUX_STATUS_ERROR;
}

/* For port and user management */
/* syslog setting read/write to non-volatile memory */
#ifdef MTK_NVDM_ENABLE
#include <stdio.h>
#include <string.h>
#include "nvdm.h"


//#define NVDM_USER_CONFIG_LENGTH 128
//#define MAX_USER_NAME   16

typedef struct {
    mux_port_t port;
    uint8_t initialized;
    uint8_t user_count;
    uint8_t reserved;
    char user_name[MAX_MUX_USER_ON_ONE_PORT][MAX_USER_NAME];
    mux_port_setting_t port_setting;
} nvdm_mux_user_config_t;

//static nvdm_mux_user_config_t nvdm_mux_config[MUX_PORT_END];

mux_status_t mux_nvdm_init(mux_port_t port)
{
    (void)port;
    #if 0
    uint32_t i;
    mux_port_t port_indx;
    for(port_indx=MUX_UART_BEGIN; port_indx <MUX_PORT_END; port_indx++) {
        if (nvdm_mux_config[port_indx].initialized == 1) {
            return MUX_STATUS_ERROR_NVDM_INIT_FAIL;
        }
        for (i=0; i < MAX_MUX_USER_ON_ONE_PORT; i++) {
            nvdm_mux_config[port_indx].initialized = 0;
            nvdm_mux_config[port_indx].port = port;
            nvdm_mux_config[port_indx].user_count = 0;
            strcpy(nvdm_mux_config[port_indx].user_name[i],"NULL");
        }
    }
    #endif
    return MUX_STATUS_OK;
}

mux_status_t mux_open_save_to_nvdm(mux_port_t port, const char *user_name)
{
    char group_name[24];
    nvdm_status_t status;
    uint32_t size;

    mux_port_t p_setting = MUX_PORT_END;

    if (port >= MUX_PORT_END) {
        return MUX_STATUS_ERROR_PARAMETER;
    }

    snprintf(group_name, 24, "%s%d%s","mux_port_", port, "_user");
    size = 1;
    status = nvdm_read_data_item(group_name, user_name, &p_setting, &size);
    if (status == NVDM_STATUS_ITEM_NOT_FOUND) {
        //save it to NVDM
    } else if (status != NVDM_STATUS_OK) {
        return MUX_STATUS_ERROR;
    }
    if ((p_setting == port) && (size == 1)) {
        //had saved
        return MUX_STATUS_OK;
    }

    size = 1;
    p_setting = port;
    status = nvdm_write_data_item(group_name, user_name, NVDM_DATA_ITEM_TYPE_RAW_DATA, &p_setting, size);
    if (status != NVDM_STATUS_OK) {
        return MUX_STATUS_ERROR;
    }

    return MUX_STATUS_OK;
}

#if 0
mux_status_t mux_close_save_to_nvdm(mux_port_t port, const char *user_name)
{
    return mux_open_save_to_nvdm(port, user_name);
}
#endif

static mux_status_t mux_close_delete_port_user_nvdm(mux_port_t port, const char *user_name)
{
    char group_name[24];
    nvdm_status_t status;

    if (port >= MUX_PORT_END) {
        return MUX_STATUS_ERROR_PARAMETER;
    }
    snprintf(group_name,24,"%s%d%s","mux_port_", port, "_user");

    status = nvdm_delete_data_item(group_name, user_name);
    if (status != NVDM_STATUS_OK) {
        return MUX_STATUS_ERROR;
    }

    return MUX_STATUS_OK;
}

mux_status_t mux_close_delete_from_nvdm(mux_port_t port, const char *user_name)
{
    return mux_close_delete_port_user_nvdm(port, user_name);
}


mux_status_t mux_query_port_setting_from_nvdm(mux_port_t port, mux_port_setting_t *setting)
{
    char group_name[6];
    char user_name[16];
    mux_port_setting_t nvdm_set;
    nvdm_status_t status;
    uint32_t size;

    if (port >= MUX_PORT_END || setting == NULL) {
        return MUX_STATUS_ERROR_PARAMETER;
    }

    strcpy(group_name, "mux");
    snprintf(user_name, 16, "%s%d%s","_port_", port, "_cfg");
    size = sizeof(mux_port_setting_t);
    status = nvdm_read_data_item(group_name,user_name, &nvdm_set, &size);

    if(status == NVDM_STATUS_ITEM_NOT_FOUND) {
        return MUX_STATUS_ERROR_NVDM_NOT_FOUND;
    } else if(status != NVDM_STATUS_OK) {
        return MUX_STATUS_ERROR;
    }

    if (size != sizeof(mux_port_setting_t)) {
        //if read back size is not same as code, it means the bin is changed
        //delete it from NVDM
        nvdm_delete_data_item(group_name,user_name);
        return MUX_STATUS_ERROR_PORT_SETTING_CHANGED;
    }

    memcpy(setting, &nvdm_set, sizeof(mux_port_setting_t));

    return MUX_STATUS_OK;
}

//olny call when port setting is changed
mux_status_t mux_save_port_setting_to_nvdm(mux_port_t port, mux_port_setting_t *setting)
{
    char group_name[6];
    char user_name[16];
    nvdm_status_t status;

    if (port >= MUX_PORT_END) {
        return MUX_STATUS_ERROR_PARAMETER;
    }

    strcpy(group_name,"mux");
    snprintf(user_name, 16, "%s%d%s","_port_", port, "_cfg");

    status = nvdm_write_data_item(group_name, user_name, NVDM_DATA_ITEM_TYPE_RAW_DATA, setting, sizeof(mux_port_setting_t));
    if (status != NVDM_STATUS_OK) {
        return MUX_STATUS_ERROR;
    }

    return MUX_STATUS_OK;
}

#if 0
//mux_status_t mux_config_query_port_users(mux_port_t port, uint32_t *user_count)
mux_status_t mux_query_port_user_number_form_nvdm(mux_port_t port, uint32_t *user_count)
{
    uint8_t port_user_count = 0;
    uint32_t per_cpu_irq_mask;

    if ((port >= MUX_PORT_END) || (user_count == NULL)) {
        return MUX_STATUS_ERROR_PARAMETER;
    }

    port_mux_local_cpu_enter_critical(&per_cpu_irq_mask);
    if (g_mux_port_configs[port].init_status == MUX_INIT_NORMAL) {
        port_user_count = nvdm_mux_config[port].user_count;
    } else {
        *user_count = port_user_count;
        port_mux_local_cpu_exit_critical(per_cpu_irq_mask);
        return MUX_STATUS_ERROR;
    }

    port_mux_local_cpu_exit_critical(per_cpu_irq_mask);
    *user_count = port_user_count;
    return MUX_STATUS_OK;
}

mux_status_t mux_query_port_user_name_from_nvdm(mux_port_t port, mux_port_buffer_t *user_name)
{
    uint8_t port_user_count = 0;
    uint32_t per_cpu_irq_mask;
    uint32_t user_id, count;
    mux_port_buffer_t *port_conf = user_name;

    count = 0;
    if ((port >= MUX_PORT_END) || (user_name == NULL)) {
        return MUX_STATUS_ERROR_PARAMETER;
    }

    port_mux_local_cpu_enter_critical(&per_cpu_irq_mask);
    if (g_mux_port_configs[port].init_status == MUX_INIT_NORMAL) {
        uint32_t i;
        for (i=0; i < MUX_PORT_END; i++) {
            if ((strcmp(nvdm_mux_config[port].user_name[i],"NULL") != 0)) {
                if (count > port_conf->count) {
                    assert(0);
                }
                strcpy(user_name->buf[count], nvdm_mux_config[port].user_name[user_id]);
                count++;
            }
        }
    } else {
        port_mux_local_cpu_exit_critical(per_cpu_irq_mask);
        return MUX_STATUS_ERROR;
    }

    port_mux_local_cpu_exit_critical(per_cpu_irq_mask);
    //*user_count = port_user_count;
    return MUX_STATUS_OK;
}

//mux_status_t mux_config_query_port_users(mux_port_t port, uint32_t *user_count)
mux_status_t mux_query_port_user_number(mux_port_t port, uint32_t *user_count)
{
    uint8_t port_user_count = 0;
    uint32_t per_cpu_irq_mask;

    if ((port >= MUX_PORT_END) || (user_count == NULL)) {
        return MUX_STATUS_ERROR_PARAMETER;
    }

    port_mux_local_cpu_enter_critical(&per_cpu_irq_mask);
    if (g_mux_port_configs[port].init_status == MUX_INIT_NORMAL) {
        port_user_count = nvdm_mux_config[port].user_count;
    } else {
        *user_count = port_user_count;
        port_mux_local_cpu_exit_critical(per_cpu_irq_mask);
        return MUX_STATUS_ERROR;
    }

    port_mux_local_cpu_exit_critical(per_cpu_irq_mask);
    *user_count = port_user_count;
    return MUX_STATUS_OK;
}

mux_status_t mux_query_port_user_name(mux_port_t port, mux_port_buffer_t *user_name)
{
    uint8_t port_user_count = 0;
    uint32_t per_cpu_irq_mask;
    uint32_t user_id, count;
    mux_port_buffer_t *port_conf = user_name;

    count = 0;
    if ((port >= MUX_PORT_END) || (user_name == NULL)) {
        return MUX_STATUS_ERROR_PARAMETER;
    }

    port_mux_local_cpu_enter_critical(&per_cpu_irq_mask);
    if (g_mux_port_configs[port].init_status == MUX_INIT_NORMAL) {
        uint32_t i;
        for (i=0; i < MUX_PORT_END; i++) {
            if ((strcmp(nvdm_mux_config[port].user_name[i],"NULL") != 0)) {
                if (count > port_conf->count) {
                    assert(0);
                }
                strcpy(user_name->buf[count], nvdm_mux_config[port].user_name[user_id]);
                count++;
            }
        }
    } else {
        port_mux_local_cpu_exit_critical(per_cpu_irq_mask);
        return MUX_STATUS_ERROR;
    }

    port_mux_local_cpu_exit_critical(per_cpu_irq_mask);
    //*user_count = port_user_count;
    return MUX_STATUS_OK;
}

#endif

//how many ports the users is using
//param:  in  user_name
//        out port_count   the user used ports number
mux_status_t mux_query_user_port_numbers_from_nvdm(const char *user_name, uint32_t *port_count)
{
    mux_port_t port_user_count = 0;
    uint32_t port_index;
    nvdm_status_t status;

    if (port_count == NULL) {
        return MUX_STATUS_ERROR_PARAMETER;
    }

    for (port_index = MUX_UART_BEGIN; port_index < MUX_PORT_END; port_index++) {
        char group_name[24];
        mux_port_t port;
        uint32_t size = 1;
        snprintf(group_name, 24, "%s%d%s","mux_port_", port_index, "_user");
        status = nvdm_read_data_item(group_name, user_name, &port, &size);
        if (status == NVDM_STATUS_OK) {
            port_user_count++;
            if (port != port_index || size != 1) {
                //return MUX_STATUS_ERROR;
                assert(0);
            }
        }
    }

    *port_count = port_user_count;
    return MUX_STATUS_OK;
}

mux_status_t mux_query_port_numbers_from_nvdm(const char *user_name, mux_port_buffer_t *port_conf)
{
    uint32_t port_index;

    if ((user_name == NULL) || (port_conf == NULL)) {
        return MUX_STATUS_ERROR_PARAMETER;
    }

    memset(&port_conf->buf[0], 0xff, MUX_PORT_END);
    port_conf->count = 0;

    for (port_index = MUX_UART_BEGIN; port_index < MUX_PORT_END; port_index++) {
        char group_name[24];
        mux_port_t port;
        nvdm_status_t status;

        uint32_t size = 1;
        snprintf(group_name, 24, "%s%d%s","mux_port_", port_index, "_user");
        status = nvdm_read_data_item(group_name, user_name, &port, &size);
        if (status == NVDM_STATUS_OK) {
            if (port_conf->count > MUX_PORT_END) {
                assert(0);
            }
            if (port != port_index || size != 1){
                //for debug
                assert(0);
            }
            port_conf->buf[port_conf->count] = port;
            port_conf->count++;
        }
    }

    return MUX_STATUS_OK;
}

#else
mux_status_t mux_open_save_to_nvdm(mux_port_t port, const char *user_name)
{
    PORT_MUX_UNUSED(port);
    PORT_MUX_UNUSED(user_name);
    return MUX_STATUS_OK;
}

mux_status_t mux_close_save_to_nvdm(mux_port_t port, const char *user_name)
{
    PORT_MUX_UNUSED(port);
    PORT_MUX_UNUSED(user_name);
    return MUX_STATUS_OK;
}


mux_status_t mux_query_port_setting_from_nvdm(mux_port_t port, mux_port_setting_t *setting)
{
    PORT_MUX_UNUSED(port);
    PORT_MUX_UNUSED(setting);
    return MUX_STATUS_OK;
}

mux_status_t mux_save_port_setting_to_nvdm(mux_port_t port, mux_port_setting_t *setting)
{
    PORT_MUX_UNUSED(port);
    PORT_MUX_UNUSED(setting);
    return MUX_STATUS_OK;
}

mux_status_t mux_query_user_port_numbers_from_nvdm(const char *user_name, uint32_t *port_count)
{
    PORT_MUX_UNUSED(user_name);
    PORT_MUX_UNUSED(port_count);
    *port_count = 0;
    return MUX_STATUS_OK;
}


mux_status_t mux_query_port_numbers_from_nvdm(const char *user_name, mux_port_buffer_t *port)
{
    PORT_MUX_UNUSED(user_name);
    PORT_MUX_UNUSED(port);
    port->count = 0;
    return MUX_STATUS_OK;
}

#if 0
mux_status_t mux_query_port_user_number_form_nvdm(mux_port_t port, uint32_t *user_count)
{
    PORT_MUX_UNUSED(port);
    PORT_MUX_UNUSED(user_count);
    return MUX_STATUS_OK;
}

mux_status_t mux_query_port_user_name_from_nvdm(mux_port_t port, mux_port_buffer_t *user_name)
{
    PORT_MUX_UNUSED(port);
    PORT_MUX_UNUSED(user_name);
    return MUX_STATUS_OK;
}
#endif

#endif

#endif

void mux_restore_callback(void)
{
    uint32_t i;

    for (i = 0; i < MUX_UART_END; i++) {
        g_mux_port_configs[i].sw_wptr = 0;
    }
}

void mux_restore_callback_port(mux_port_t mux_port)
{
    g_mux_port_configs[mux_port].sw_wptr = 0;
}

static uint32_t buffer_check_free_space(mux_port_t port)
{
    uint32_t port_index, free_space, hw_rptr, hw_wptr;
    mux_port_type_t port_type;

    port_index = parse_port_type(port, (mux_port_type_t*)&port_type);

    hw_rptr = port_mux_device_get_hw_rptr(port_type, port_index, false);
    hw_wptr = port_mux_device_get_hw_wptr(port_type, port_index, false);

    if (hw_wptr >= hw_rptr) {
        if (g_mux_port_configs[port].sw_wptr >= hw_wptr) {
            free_space = (g_mux_port_configs[port].tx_buf_size - g_mux_port_configs[port].sw_wptr) + hw_rptr;
        } else {
            free_space = hw_rptr - g_mux_port_configs[port].sw_wptr;
        }
    } else {
        free_space = hw_rptr - g_mux_port_configs[port].sw_wptr;
    }

    /* In order to simplify the code flow and overhead of logging alloc and update,
        * we always keep 4 bytes reserved space to avoid the full case of logging buffer.
        * As it's difficult to distinguish the different case of full case.
        */
    if (free_space < 4) {
        free_space = 0;
    } else {
        free_space -= 4;
    }

    return free_space;
}

static uint8_t *buffer_copy_data_out(mux_port_t port, uint8_t *src_buf, uint8_t *dst_buf, uint32_t log_size)
{
    uint32_t i, tx_buf_end;
    uint8_t *p_buf;

    tx_buf_end = g_mux_port_configs[port].tx_buf_addr + g_mux_port_configs[port].tx_buf_size;

    if ((tx_buf_end - (uint32_t)dst_buf) <= log_size) {
        memcpy(dst_buf, src_buf, tx_buf_end - (uint32_t)dst_buf);
        i = tx_buf_end - (uint32_t)dst_buf;
        memcpy((uint8_t *)(g_mux_port_configs[port].tx_buf_addr), &src_buf[i], log_size - i);
        p_buf = (uint8_t *)((g_mux_port_configs[port].tx_buf_addr) + log_size - i);
    } else {
        memcpy(dst_buf, src_buf, log_size);
        p_buf = dst_buf + log_size;
    }

    return p_buf;
}

static uint8_t *internal_buffer_pre_alloc(mux_port_t port, uint32_t total_size)
{
    uint8_t *log_buf_ptr;
    uint32_t free_space, p_log_fill;

    port_mux_cross_cpu_enter_critical();

    free_space = buffer_check_free_space(port);
    if (free_space < total_size) {
        port_mux_cross_cpu_exit_critical();
        return NULL;
    }

    p_log_fill = g_mux_port_configs[port].sw_wptr;
    if ((g_mux_port_configs[port].sw_wptr + total_size) < g_mux_port_configs[port].tx_buf_size) {
        g_mux_port_configs[port].sw_wptr += total_size;
    } else {
        g_mux_port_configs[port].sw_wptr = total_size - (g_mux_port_configs[port].tx_buf_size - g_mux_port_configs[port].sw_wptr);
    }
    g_mux_port_configs[port].nest_count++;

    log_buf_ptr = (uint8_t *)(g_mux_port_configs[port].tx_buf_addr + p_log_fill);

    port_mux_cross_cpu_exit_critical();

    return log_buf_ptr;
}

void internal_buffer_wptr_update(mux_port_t port)
{
    uint32_t p_log_hw_rptr, p_log_hw_wptr;
    uint32_t port_index, move_bytes;
    mux_port_type_t port_type;

    port_mux_cross_cpu_enter_critical();

    port_index = parse_port_type(port, (mux_port_type_t*)&port_type);

    g_mux_port_configs[port].nest_count--;
    if (g_mux_port_configs[port].nest_count == 0) {
        /* update WPTR */
        p_log_hw_rptr = port_mux_device_get_hw_rptr(port_type, port_index, false);
        p_log_hw_wptr = port_mux_device_get_hw_wptr(port_type, port_index, false);
        if (p_log_hw_wptr >= p_log_hw_rptr) {
            if (g_mux_port_configs[port].sw_wptr >= p_log_hw_wptr) {
                move_bytes = g_mux_port_configs[port].sw_wptr - p_log_hw_wptr;
            } else {
                move_bytes = g_mux_port_configs[port].sw_wptr + (g_mux_port_configs[port].tx_buf_size - p_log_hw_wptr);
            }
        } else {
            move_bytes = g_mux_port_configs[port].sw_wptr - p_log_hw_wptr;
        }
        port_mux_device_set_tx_hw_wptr(port_type, port_index, move_bytes);
    }

    port_mux_cross_cpu_exit_critical();
}


mux_status_t mux_tx(mux_handle_t handle, const mux_buffer_t buffers[], uint32_t buffers_counter,uint32_t *send_done_data_len)
{
    uint8_t *p_log_fill;
    uint32_t i, payload_size, total_size;
    ////head_buf and tail_buf need more 4B for memory crash check!!!
    uint32_t head_buf[(TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN+4)/4], tail_buf[(TX_PROTOCOL_CB_TAIL_BUFFER_MAX_LEN+4)/4];
    uint32_t user_id, per_cpu_irq_mask;
    //uint32_t port_index;
    volatile mux_protocol_t *mux_protocol;
    mux_buffer_t head_buf_info, tail_buf_info;
    mux_port_t port;
    //mux_port_type_t port_type;
    volatile mux_user_config_t *user_config;

    if ((handle_is_valid(handle) == false) || (buffers == NULL)) {
        return MUX_STATUS_ERROR_PARAMETER;
    }

    port = handle_to_port(handle);
    if (g_mux_port_configs[port].init_status != MUX_INIT_NORMAL) {
        return MUX_STATUS_ERROR;
    }

    user_id = handle_to_user_id(handle);
    user_config = &g_mux_user_configs[user_id];
    if(user_config->user_name == NULL)
    {
        return MUX_STATUS_USER_NOT_OPEN;
    }
    //mask by Liming
    //port_mux_device_tx_hook(port_type, port_index);

    /* Calculate total size of payload */
    payload_size = 0;
    //for (i = 0; buffers[i]->p_buf != NULL; i++) {
    for (i = 0; i < buffers_counter; i++) {
        payload_size += buffers[i].buf_size;
    }


    /* Generate the protocol header */
    head_buf_info.p_buf = NULL;
    tail_buf_info.p_buf = NULL;
    mux_protocol = &g_mux_port_configs[port].protocol;
    if (mux_protocol->tx_protocol_callback != NULL) {
        head_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN/4] = 0xdeadbeef;//for memory crash check
        head_buf_info.p_buf = (uint8_t *)head_buf;
        head_buf_info.buf_size = TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN;
        tail_buf[TX_PROTOCOL_CB_TAIL_BUFFER_MAX_LEN/4] = 0xdeadbeef;//for memory crash check
        tail_buf_info.p_buf = (uint8_t *)tail_buf;
        tail_buf_info.buf_size = TX_PROTOCOL_CB_TAIL_BUFFER_MAX_LEN;
        mux_protocol->tx_protocol_callback(handle, buffers, buffers_counter,&head_buf_info, &tail_buf_info,mux_protocol->user_data);
        if((head_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN/4] != 0xdeadbeef) ||
                    (tail_buf[TX_PROTOCOL_CB_TAIL_BUFFER_MAX_LEN/4] != 0xdeadbeef)){
            //memory crash check!!! Rx and Tx buffer max len is TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN and TX_PROTOCOL_CB_TAIL_BUFFER_MAX_LEN
            //But the callback write too many memory
            //assert(0);
            return MUX_STATUS_USER_ERROR_OF_RX_TX_PROTOCOL;
        }
        if((head_buf_info.buf_size>TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN) ||
                    (tail_buf_info.buf_size>TX_PROTOCOL_CB_TAIL_BUFFER_MAX_LEN)){
            //memory crash check!!! Rx and Tx buffer max len is TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN and TX_PROTOCOL_CB_TAIL_BUFFER_MAX_LEN
            //But the callback write too many memory
            //assert(0);
            return MUX_STATUS_USER_ERROR_OF_RX_TX_PROTOCOL;
        }
        total_size = head_buf_info.buf_size + payload_size + tail_buf_info.buf_size;
    } else {
        total_size = payload_size;
    }

#ifdef MTK_CPU_NUMBER_0
    /* Directly send out using polling mode if exception happen */
    if (g_mux_port_configs[port].init_status == MUX_INIT_EXCEPTION) {
        uint32_t port_index;
        mux_port_type_t port_type;
        port_index = parse_port_type(port, (mux_port_type_t*)&port_type);
        if (head_buf_info.p_buf != NULL) {
            port_mux_device_exception_send(port_type, port_index,head_buf_info.p_buf, head_buf_info.buf_size);
        }
        for (i = 0; i < buffers_counter; i++) {
            port_mux_device_exception_send(port_type, port_index,buffers[i].p_buf, buffers[i].buf_size);
        }
        if (tail_buf_info.p_buf != NULL) {
            port_mux_device_exception_send(port_type, port_index,tail_buf_info.p_buf, tail_buf_info.buf_size);
        }
        return payload_size;
    }
#endif

    port_mux_local_cpu_enter_critical(&per_cpu_irq_mask);
    port_mux_cross_cpu_enter_critical();

    /* Alloc the buffer space for the package */
    p_log_fill = internal_buffer_pre_alloc(port, total_size);
    if (p_log_fill == NULL) {
        user_config->need_tx_callback = true;
        port_mux_cross_cpu_exit_critical();
        port_mux_local_cpu_exit_critical(per_cpu_irq_mask);
        *send_done_data_len = 0;
        return MUX_STATUS_USER_TX_BUF_SIZE_NOT_ENOUGH; // the Tx buffer size not enough!!!
    }

    port_mux_cross_cpu_exit_critical();
    port_mux_local_cpu_exit_critical(per_cpu_irq_mask);

    port_mux_local_cpu_enter_critical(&per_cpu_irq_mask);
    /* Copy header to VFIFO buffer */
    if(head_buf_info.p_buf != NULL)
        p_log_fill = buffer_copy_data_out(port, head_buf_info.p_buf, p_log_fill, head_buf_info.buf_size);

    /* Copy payload to VFIFO buffer */
    //for (i = 0; buffers[i]->p_buf != NULL; i++) {
    for (i = 0; i<buffers_counter; i++) {
        p_log_fill = buffer_copy_data_out(port, buffers[i].p_buf, p_log_fill, buffers[i].buf_size);
    }

    /* Copy tail to VFIFO buffer */
    if(tail_buf_info.p_buf != NULL)
        buffer_copy_data_out(port, tail_buf_info.p_buf, p_log_fill, tail_buf_info.buf_size);

    port_mux_local_cpu_exit_critical(per_cpu_irq_mask);

    port_mux_local_cpu_enter_critical(&per_cpu_irq_mask);
    port_mux_cross_cpu_enter_critical();

    /* Update HW WPTR if it's possible. */
    internal_buffer_wptr_update(port);

    port_mux_cross_cpu_exit_critical();
    port_mux_local_cpu_exit_critical(per_cpu_irq_mask);
    *send_done_data_len = payload_size;

    return MUX_STATUS_OK;
}

mux_status_t mux_control(mux_port_t port, mux_ctrl_cmd_t command, mux_ctrl_para_t *para)
{
    uint32_t port_index;
    mux_port_type_t port_type;

    if (g_mux_port_configs[port].init_status != MUX_INIT_NORMAL) {
        return MUX_STATUS_ERROR_NOT_INIT;
    }

    switch (command) {
        case MUX_CMD_GET_TX_AVAIL: {
            mux_get_tx_avail_t *p_mux_tx_avail = (mux_get_tx_avail_t *)para;
            p_mux_tx_avail->ret_size = buffer_check_free_space(port);
        }break;

        case MUX_CMD_GET_RX_AVAIL: {
            mux_get_rx_avail_t *p_mux_rx_avail = (mux_get_rx_avail_t *)para;
            p_mux_rx_avail->ret_size = buffer_check_avail_size(port);
        }break;

        case MUX_CMD_GET_TX_BUFFER_STATUS: {
        }break;

        case MUX_CMD_GET_RX_BUFFER_STATUS: {
        }break;

        case MUX_CMD_CLEAN: {
        }break;

        default:{
            port_index = parse_port_type(port, &port_type);
            if(MUX_STATUS_ERROR == port_mux_device_normal_control(port_type, port_index, command, para)){
                return MUX_STATUS_ERROR;
            }
        }break;
    }

    return MUX_STATUS_OK;
}


