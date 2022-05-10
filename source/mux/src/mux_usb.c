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

#ifdef MTK_USB_DEMO_ENABLED

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "hal_usb.h"
#include "mux.h"
#include "mux_port_device.h"
#include "hal_gpt.h"
#include "usb_custom_def.h"
#ifdef HAL_USB_MODULE_ENABLED

#include "hal_platform.h"
#include "hal_nvic.h"

#include "mux.h"
#include "mux_port_device.h"
#include "usbacm_drv.h"
#include "serial_port.h"
#include "assert.h"
#include "mux_port.h"
#ifdef MTK_CPU_NUMBER_0

LOG_CONTROL_BLOCK_DECLARE(MUX_PORT);


virtual_read_write_point_t g_mux_usb_r_w_point[2];
   
   
   mux_irq_handler_t g_mux_usb_callback;
   
#define USB_PORT_INDEX_TO_MUX_PORT(port_index) (port_index+MUX_USB_COM_1)
   
   serial_port_handle_t serial_port_usb_handle[2];


static void port_mux_usb_set_rx_hw_wptr_internal_use(uint8_t port_index, uint32_t move_bytes);
static void port_mux_usb_set_tx_hw_rptr_internal_use(uint8_t port_index, uint32_t move_bytes);

void usb_send_data(uint8_t port_index,uint32_t addr,uint32_t len,volatile uint32_t *sending_point)
{
    serial_port_status_t status;
    serial_port_write_data_t send_data;
    serial_port_get_write_avail_t usb_tx_avail_len;

    mux_driver_debug_for_check(&g_mux_usb_r_w_point[port_index]);
    if(len == 0)
        return;

    send_data.data = (uint8_t *)addr;
    send_data.size = len;
    status = serial_port_control(serial_port_usb_handle[port_index], SERIAL_PORT_CMD_GET_WRITE_AVAIL, (serial_port_ctrl_para_t *)&usb_tx_avail_len);
    if(SERIAL_PORT_STATUS_OK != status)
    {
        assert(0);
    }

    if(usb_tx_avail_len.ret_size < send_data.size)
        send_data.size = usb_tx_avail_len.ret_size;

    /*Must be  set the value of tx_sending_read_point firstly!!!!!must before call usb send!!!!!*/
    *sending_point = addr+send_data.size;
   
    status = serial_port_control(serial_port_usb_handle[port_index], SERIAL_PORT_CMD_WRITE_DATA, (serial_port_ctrl_para_t *)&send_data);

    if(SERIAL_PORT_STATUS_OK != status)
    {
        assert(0);
    }
    if(send_data.ret_size != send_data.size)
    {
            /*this is a risk of race condition, because we need restart transfer on IRQ handle,
                    if there have no time to update p->tx_sending_read_point, and the IRQ triggerred quickly, will be enter err status.
                    So we must query avali size firstly ,and makesure the all of data must be sent done on one tranfser!!!*/
            assert(0);
    }
}

static bool usb_mux_dump_data(uint8_t usb_port, void *start_address, uint32_t size)
{
    uint32_t Length = 0;
    bool is_unalign_data = false;
    uint8_t data[4];
    uint32_t i = 0;

    while (size > 0) {
        if (size > 64) {
            Length = 64;
        } else if (size & 0x03) {
            Length = size & (~0x03);
            is_unalign_data = true;
        } else {
            Length = size;
        }

        hal_usb_set_endpoint_tx_ready(usb_port + 1);
        while (!hal_usb_is_endpoint_tx_empty(usb_port + 1));
        hal_usb_write_endpoint_fifo(usb_port + 1, Length, start_address);
        hal_usb_set_endpoint_tx_ready(usb_port + 1);
        while (!hal_usb_is_endpoint_tx_empty(usb_port + 1));

        size = size - Length;
        start_address = start_address + Length;

        if (is_unalign_data) {
            for (i = 0; i < (size & 0x03); i++) {
                data[i] = *(uint8_t *)(start_address + i);
            }
            hal_usb_set_endpoint_tx_ready(usb_port + 1);
            while (!hal_usb_is_endpoint_tx_empty(usb_port + 1));
            hal_usb_write_endpoint_fifo(usb_port + 1, size & 0x03, data);
            hal_usb_set_endpoint_tx_ready(usb_port + 1);
            while (!hal_usb_is_endpoint_tx_empty(usb_port + 1));
            size = size - (size & 0x03);
            start_address = start_address + (size & 0x03);
        }
    }
    return true;
}


static void mux_usb_callback(serial_port_dev_t device, serial_port_callback_event_t event, void *parameter)
{
    uint8_t port_index = device - SERIAL_PORT_DEV_USB_COM1;
    virtual_read_write_point_t *p;
    uint32_t next_available_block_len,next_free_block_len,per_cpu_irq_mask;
    serial_port_read_data_t read_data;
    serial_port_status_t status;
    p=&g_mux_usb_r_w_point[port_index];

     mux_driver_debug_for_check(&g_mux_usb_r_w_point[port_index]);

     switch(event) {
         case SERIAL_PORT_EVENT_READY_TO_WRITE:
            if(p->tx_send_is_running != MUX_DEVICE_HW_RUNNING){
                //Liming add warining!!!
               return;
                //assert(0);
            }

            //transfer done , then update tx_buff_read_point
            port_mux_usb_set_tx_hw_rptr_internal_use(port_index,p->tx_sending_read_point-p->tx_buff_read_point);
            
            g_mux_usb_callback(USB_PORT_INDEX_TO_MUX_PORT(port_index),MUX_EVENT_READY_TO_WRITE);
            //Judge Tx buffer whether have data need to send
            
            port_mux_local_cpu_enter_critical(&per_cpu_irq_mask);
            next_available_block_len = mux_common_device_get_buf_next_available_block_len(p->tx_buff_start,p->tx_buff_read_point,p->tx_buff_write_point,p->tx_buff_end,p->tx_buff_available_len);
            if(next_available_block_len == 0)
            {
                p->tx_send_is_running = MUX_DEVICE_HW_IDLE;//change to idle
                port_mux_local_cpu_exit_critical(per_cpu_irq_mask);
                break;
            }
            else
            {
                p->tx_send_is_running = MUX_DEVICE_HW_RUNNING;//keep running
                //p->tx_sending_read_point = p->tx_buff_read_point + next_available_block_len;
                port_mux_local_cpu_exit_critical(per_cpu_irq_mask);
                usb_send_data(port_index,p->tx_buff_read_point,next_available_block_len,&p->tx_sending_read_point);
                break;
            }
         case SERIAL_PORT_EVENT_READY_TO_READ:
            // Rx buffer have some space to do receive.
            next_free_block_len = mux_common_device_get_buf_next_free_block_len(p->rx_buff_start,p->rx_buff_read_point,p->rx_buff_write_point,p->rx_buff_end,p->rx_buff_available_len);
            read_data.buffer = (uint8_t *)p->rx_buff_write_point;
            read_data.size = next_free_block_len;
            
            status = serial_port_control(serial_port_usb_handle[port_index], SERIAL_PORT_CMD_READ_DATA, (serial_port_ctrl_para_t *)&read_data);
            if(status != SERIAL_PORT_STATUS_OK)
                assert(0);
            LOG_MSGID_I(MUX_PORT,"Liming: next_free_block_len:%d read_data.ret_size:%d\r\n",2,(int)next_free_block_len,(int)read_data.ret_size);

            port_mux_usb_set_rx_hw_wptr_internal_use(port_index,read_data.ret_size);

            g_mux_usb_callback(USB_PORT_INDEX_TO_MUX_PORT(port_index),MUX_EVENT_READY_TO_READ);
             
            break;
         case SERIAL_PORT_EVENT_USB_CONNECTION:
            break;
         case SERIAL_PORT_EVENT_BT_DISCONNECTION:
            break; 
     }
            
}







mux_status_t port_mux_usb_normal_init(uint8_t port_index, mux_port_config_t *p_setting,mux_irq_handler_t irq_handler)
{
    serial_port_status_t serial_port_status;
    serial_port_open_para_t serial_port_usb_config;
    mux_common_device_r_w_point_init(&g_mux_usb_r_w_point[port_index],p_setting);

    g_mux_usb_callback = irq_handler;
    serial_port_usb_config.callback = mux_usb_callback;
#ifdef MTK_PORT_SERVICE_SLIM_ENABLE
    serial_port_usb_config.tx_buffer_size = 1024;
    serial_port_usb_config.rx_buffer_size = 1024;
#endif

    serial_port_status = serial_port_open(port_index+SERIAL_PORT_DEV_USB_COM1, &serial_port_usb_config, &serial_port_usb_handle[port_index]);
    if(serial_port_status != SERIAL_PORT_STATUS_OK)
        assert(0);
    mux_driver_debug_for_check(&g_mux_usb_r_w_point[port_index]);

    return MUX_STATUS_OK;
}

mux_status_t port_mux_usb_deinit(uint8_t port_index)
{
    serial_port_status_t status;

    mux_driver_debug_for_check(&g_mux_usb_r_w_point[port_index]);

   status = serial_port_close(serial_port_usb_handle[port_index]);
    if(status != SERIAL_PORT_STATUS_OK)
    {
        return MUX_STATUS_ERROR_DEINIT_FAIL;
    }
    return MUX_STATUS_OK;
}
    

void port_mux_usb_exception_init(uint8_t port_index)
{
    PORT_MUX_UNUSED(port_index);
}

void port_mux_usb_exception_send(uint8_t port_index, uint8_t *buffer, uint32_t size)
{
    usb_mux_dump_data(port_index, buffer, size);
}

#endif
bool port_mux_usb_buf_is_full(uint8_t port_index, bool is_rx)
{
    return mux_common_device_buf_is_full(&g_mux_usb_r_w_point[port_index],is_rx);
}

uint32_t port_mux_usb_get_hw_rptr(uint8_t port_index, bool is_rx)
{
    return mux_common_device_get_hw_rptr(&g_mux_usb_r_w_point[port_index],is_rx);
}

uint32_t port_mux_usb_get_hw_wptr(uint8_t port_index, bool is_rx)
{
    return mux_common_device_get_hw_wptr(&g_mux_usb_r_w_point[port_index],is_rx);
}

/*
    port_mux_usb_set_tx_hw_wptr: for Rx- MUX driver read data.
*/

void port_mux_usb_set_rx_hw_rptr(uint8_t port_index, uint32_t move_bytes)
{
    mux_common_device_set_rx_hw_rptr(&g_mux_usb_r_w_point[port_index],move_bytes);
}


static void port_mux_usb_set_rx_hw_wptr_internal_use(uint8_t port_index, uint32_t move_bytes)
{
    mux_common_device_set_rx_hw_wptr_internal_use(&g_mux_usb_r_w_point[port_index],move_bytes);
}

/*port_mux_usb_set_tx_hw_rptr_internal_use: for Tx-HW send data from Tx buff
    This function only need by USB / I2C slave / SPI slave.
    As UART with VFIFO, HW will move Rx write point. 
    But for  USB / I2C slave / SPI slave, mux_xxx driver should to do this.
*/
static void port_mux_usb_set_tx_hw_rptr_internal_use(uint8_t port_index, uint32_t move_bytes)
{
    mux_common_device_set_tx_hw_rptr_internal_use(&g_mux_usb_r_w_point[port_index],move_bytes);
}


/*
    port_mux_usb_set_tx_hw_wptr: for Tx- MUX driver send data.
*/
void port_mux_usb_set_tx_hw_wptr(uint8_t port_index, uint32_t move_bytes)
{
    virtual_read_write_point_t *p=&g_mux_usb_r_w_point[port_index];
    uint32_t per_cpu_irq_mask;
    uint32_t send_addr,send_len;
    mux_driver_debug_for_check(&g_mux_usb_r_w_point[port_index]);
    
    port_mux_local_cpu_enter_critical(&per_cpu_irq_mask);

    mux_common_device_set_tx_hw_wptr(&g_mux_usb_r_w_point[port_index],move_bytes);

    
    send_len = mux_common_device_get_buf_next_available_block_len(p->tx_buff_start,p->tx_buff_read_point,p->tx_buff_write_point,p->tx_buff_end,p->tx_buff_available_len);
    send_addr = p->tx_buff_read_point;


    if((p->tx_send_is_running == MUX_DEVICE_HW_RUNNING) || (send_len == 0) )//HW is running, no need to do send now, when hw transfer done, will restart on the IRQ handle
    {
        port_mux_local_cpu_exit_critical(per_cpu_irq_mask);
        return;
    }
    else
    {
        p->tx_send_is_running = MUX_DEVICE_HW_RUNNING;
        p->tx_sending_read_point = p->rx_buff_read_point+send_len;
    }
    
    port_mux_local_cpu_exit_critical(per_cpu_irq_mask);

    usb_send_data(port_index,send_addr,send_len,&p->tx_sending_read_point);
}


port_mux_device_ops_t g_port_mux_usb_ops = {
#ifdef MTK_CPU_NUMBER_0
    port_mux_usb_normal_init,
    port_mux_usb_deinit,
    port_mux_usb_exception_init,
    port_mux_usb_exception_send,
    port_mux_usb_buf_is_full,
#endif
//    port_mux_usb_hook,
    port_mux_usb_get_hw_rptr,
    port_mux_usb_set_rx_hw_rptr,
    port_mux_usb_get_hw_wptr,
    port_mux_usb_set_tx_hw_wptr,
};

#endif
#endif

