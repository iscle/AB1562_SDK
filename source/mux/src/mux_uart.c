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

#include "mux.h"
#include "mux_port_device.h"
#include "hal_uart.h"
#include "hal_gpt.h"

#ifdef MTK_CPU_NUMBER_0

#include "hal_uart_internal.h"
#include "serial_port_assignment.h"

static mux_irq_handler_t g_mux_irq_handler;
#define UART_PORT_INDEX_TO_MUX_PORT(port_index) (port_index+MUX_UART_0)

<<<<<<< HEAD
=======
extern hal_uart_baudrate_t query_syslog_baudrate(void);

>>>>>>> db20e11 (second commit)
static void mux_uart_callback(hal_uart_callback_event_t event, void *user_data)
{
    uint8_t port_index = (uint32_t)user_data;

    PORT_MUX_UNUSED(user_data);

    if (event == HAL_UART_EVENT_READY_TO_WRITE) {
        g_mux_irq_handler(UART_PORT_INDEX_TO_MUX_PORT(port_index),MUX_EVENT_READY_TO_WRITE);
    } else if (event == HAL_UART_EVENT_READY_TO_READ) {
        g_mux_irq_handler(UART_PORT_INDEX_TO_MUX_PORT(port_index),MUX_EVENT_READY_TO_READ);
    }else if (event == HAL_UART_EVENT_WAKEUP_SLEEP) {
        g_mux_irq_handler(UART_PORT_INDEX_TO_MUX_PORT(port_index),MUX_EVENT_WAKEUP_FROM_SLEEP);
    }
}


mux_status_t port_mux_uart_normal_init(uint8_t port_index, mux_port_config_t *p_setting,mux_irq_handler_t irq_handler)
{
    hal_uart_config_t uart_config;
    hal_uart_dma_config_t dma_config;

    g_mux_irq_handler = irq_handler;

    uart_config.baudrate = p_setting->user_setting.dev_setting.uart.uart_config.baudrate;  //CONFIG_SYSLOG_BAUDRATE;
    uart_config.parity = p_setting->user_setting.dev_setting.uart.uart_config.parity;//HAL_UART_PARITY_NONE;
    uart_config.stop_bit = p_setting->user_setting.dev_setting.uart.uart_config.stop_bit;//HAL_UART_STOP_BIT_1;
    uart_config.word_length = p_setting->user_setting.dev_setting.uart.uart_config.word_length;//HAL_UART_WORD_LENGTH_8;

    dma_config.send_vfifo_buffer      = (uint8_t*)(uint32_t*)p_setting->tx_buf_addr;
    dma_config.send_vfifo_buffer_size = p_setting->tx_buf_size;
    dma_config.receive_vfifo_buffer   = (uint8_t*)(uint32_t*)p_setting->rx_buf_addr;
    dma_config.receive_vfifo_buffer_size = p_setting->rx_buf_size;
    dma_config.send_vfifo_threshold_size = dma_config.send_vfifo_buffer_size / 8;
    dma_config.receive_vfifo_threshold_size   = dma_config.receive_vfifo_buffer_size / 2;
    dma_config.receive_vfifo_alert_size       = 12;
    if(HAL_UART_STATUS_OK != hal_uart_init(port_index, &uart_config))
    {
        return MUX_STATUS_ERROR_INIT_FAIL;
    }
    if(HAL_UART_STATUS_OK != hal_uart_set_dma(port_index, &dma_config))
    {
        return MUX_STATUS_ERROR_INIT_FAIL;
    }

    if(p_setting->user_setting.dev_setting.uart.flowcontrol_type == MUX_UART_NONE_FLOWCONTROL)
    {
       hal_uart_disable_flowcontrol(port_index);
    }
    else if(p_setting->user_setting.dev_setting.uart.flowcontrol_type == MUX_UART_SW_FLOWCONTROL)
    {
       hal_uart_set_software_flowcontrol(port_index,0x11,0x13,0x77);
    }
    else if(p_setting->user_setting.dev_setting.uart.flowcontrol_type == MUX_UART_HW_FLOWCONTROL)
    {
        hal_uart_set_hardware_flowcontrol(port_index);
    }
    else
    {
        assert(0);
    }

    if(HAL_UART_STATUS_OK != hal_uart_register_callback(port_index, mux_uart_callback, (void*)(intptr_t)port_index))
    {
        return MUX_STATUS_ERROR_INIT_FAIL;
    }

    return MUX_STATUS_OK;

}

mux_status_t port_mux_uart_deinit(uint8_t port_index)
{
    if(HAL_UART_STATUS_OK != hal_uart_deinit(port_index))
        return MUX_STATUS_ERROR_DEINIT_FAIL;
    else
        return MUX_STATUS_OK;
}

void port_mux_uart_exception_init(uint8_t port_index)
{
    hal_uart_config_t uart_config;

    hal_uart_deinit(port_index);
<<<<<<< HEAD
    //TODO: finally all project need use CONFIG_RACE_BAUDRATE.
#ifdef MTK_DEBUG_PLAIN_LOG_ENABLE
    uart_config.baudrate = CONFIG_SYSLOG_BAUDRATE;
#else
    uart_config.baudrate = CONFIG_RACE_BAUDRATE;
=======

#ifdef MTK_DEBUG_PLAIN_LOG_ENABLE
    uart_config.baudrate = CONFIG_SYSLOG_BAUDRATE;
#else
    uart_config.baudrate = query_syslog_baudrate();
>>>>>>> db20e11 (second commit)
#endif
    uart_config.parity = HAL_UART_PARITY_NONE;
    uart_config.stop_bit = HAL_UART_STOP_BIT_1;
    uart_config.word_length = HAL_UART_WORD_LENGTH_8;
    hal_uart_init(port_index, &uart_config);
#ifndef MTK_DEBUG_PLAIN_LOG_ENABLE
    //TODO: need open finally
    if(port_index == 0){
        hal_uart_set_software_flowcontrol(port_index, 0x11, 0x13, 0x77);
    }
#endif
}

void port_mux_uart_exception_send(uint8_t port_index, uint8_t *buffer, uint32_t size)
{
    uint32_t i;

    for(i=0; i<size ;i++){
        hal_uart_send_polling(port_index, buffer++, 1);
        hal_gpt_delay_us(2);
    }

    //hal_uart_send_polling(port_index, buffer, size);
}
#endif
bool port_mux_uart_buf_is_full(uint8_t port_index, bool is_rx)
{
    return uart_get_buf_full_status(port_index, is_rx);
}

uint32_t port_mux_uart_get_hw_rptr(uint8_t port_index, bool is_rx)
{
    return uart_get_hw_rptr(port_index, is_rx);
}

uint32_t port_mux_uart_get_hw_wptr(uint8_t port_index, bool is_rx)
{
    return uart_get_hw_wptr(port_index, is_rx);
}

/**When Receive data, MUX will call this API to move Rx Ring buffer Read point.
    for MUX, firstly ,should copy data out. Then call this API to move Rx Ring buffer Read point. **/
void port_mux_uart_set_rx_hw_rptr(uint8_t port_index, uint32_t move_bytes)
{
    uart_set_sw_move_byte(port_index, 1, move_bytes);
}

/**When Send data, MUX will call this API to move Tx Ring buffer Write point.
    for MUX, firstly ,should copy data in. Then call this API to move Rx Ring buffer Write point. **/
void port_mux_uart_set_tx_hw_wptr(uint8_t port_index, uint32_t move_bytes)
{
    //printf("port_index:%d, port_mux_uart_set_hw_wptr:%d #######",port_index,move_bytes);
    uart_set_sw_move_byte(port_index, 0, move_bytes);
}

mux_status_t port_mux_uart_control(uint8_t port_index, mux_ctrl_cmd_t command, mux_ctrl_para_t *para)
{
    PORT_MUX_UNUSED(port_index);
    PORT_MUX_UNUSED(command);
    PORT_MUX_UNUSED(para);
    LOG_MSGID_E(MUX_PORT,"ERROR!!! MUX uart not support port_mux_uart_control!!!",0);
    return MUX_STATUS_OK;
}

port_mux_device_ops_t g_port_mux_uart_ops = {
#ifdef MTK_CPU_NUMBER_0
    port_mux_uart_normal_init,
    port_mux_uart_deinit,
    port_mux_uart_exception_init,
    port_mux_uart_exception_send,
#endif
    port_mux_uart_buf_is_full,
    port_mux_uart_get_hw_rptr,
    port_mux_uart_set_rx_hw_rptr,
    port_mux_uart_get_hw_wptr,
    port_mux_uart_set_tx_hw_wptr,
    port_mux_uart_control,
};

