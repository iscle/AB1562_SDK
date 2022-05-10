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

#include "hal_uart.h"
#include "hal_uart_internal.h"
#include "hal_nvic.h"
#include "hal_nvic_internal.h"

#ifdef HAL_UART_MODULE_ENABLED
//#define LOGING_CANNOT_USE
#include "memory_attribute.h"
#include "hal_clock.h"
#include "hal_gpt.h"
#ifdef HAL_SLEEP_MANAGER_ENABLED
#include "hal_sleep_manager.h"
#include "hal_sleep_manager_internal.h"
#include "mux.h"
#endif

<<<<<<< HEAD
=======
#ifndef log_hal_error
#define log_hal_error(fmt, ...)
#endif

>>>>>>> db20e11 (second commit)
#define UNUSED(p) ((void)(p))

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAL_SLEEP_MANAGER_ENABLED

#define UART_WAKE_UP_ENABLED        /*Enable rx pin wakeup feature*/
static bool g_uart_frist_send_complete_interrupt[HAL_UART_MAX];
static bool g_uart_send_lock_status[HAL_UART_MAX];
static sleep_management_lock_request_t uart_sleep_handle[HAL_UART_MAX] = {SLEEP_LOCK_UART0, SLEEP_LOCK_UART1};
static mux_port_t g_uart_mux_port[HAL_UART_MAX] = {MUX_UART_0, MUX_UART_1};
static hal_uart_port_t g_uart_eint_port[HAL_UART_MAX];
extern void mux_restore_callback_port(mux_port_t mux_port);
#endif

static uart_flowcontrol_t g_uart_flowcontrol_status[HAL_UART_MAX] = {UART_FLOWCONTROL_NONE, UART_FLOWCONTROL_NONE};
static uart_sw_flowcontrol_config_t g_uart_sw_flowcontrol_config[HAL_UART_MAX];
static hal_uart_config_t g_uart_config[HAL_UART_MAX];
static bool g_uart_not_wait_empty_status = false;
static bool g_uart_global_data_initialized = false;
static hal_uart_baudrate_t g_baudrate[HAL_UART_MAX];
static hal_uart_port_t g_uart_port_for_logging = HAL_UART_0;
static volatile uart_hwstatus_t g_uart_hwstatus[HAL_UART_MAX];
static const uint32_t g_uart_baudrate_map[] = {110, 300, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600, 2000000, 3000000};
static uart_callback_t g_uart_callback[HAL_UART_MAX];
static uart_dma_callback_data_t g_uart_dma_callback_data[HAL_UART_MAX * 2];
static hal_uart_dma_config_t g_uart_dma_config[HAL_UART_MAX];
<<<<<<< HEAD

=======
bool one_wire_disable_rx_irq_flag = false;
>>>>>>> db20e11 (second commit)

VDMA_REGISTER_T *const g_vdma_regbase[HAL_UART_MAX][2] = {{VDMA_RG_UART0_TX_BASE, VDMA_RG_UART0_RX_BASE},{VDMA_RG_UART1_TX_BASE, VDMA_RG_UART1_RX_BASE}};
const hal_clock_cg_id g_uart_port_to_pdn[] = {HAL_CLOCK_CG_UART0, HAL_CLOCK_CG_UART1};
static const hal_nvic_irq_t g_uart_port_to_irq_num[] = {UART0_IRQn, UART1_IRQn};

extern UART_REGISTER_T *const g_uart_regbase[];

static bool uart_port_is_valid(hal_uart_port_t uart_port)
{
    return (uart_port < HAL_UART_MAX);
}

static bool uart_baudrate_is_valid(hal_uart_baudrate_t baudrate)
{
    return (baudrate < HAL_UART_BAUDRATE_MAX);
}

static bool uart_config_is_valid(const hal_uart_config_t *config)
{
    return ((config->baudrate < HAL_UART_BAUDRATE_MAX) &&
            (config->word_length <= HAL_UART_WORD_LENGTH_8) &&
            (config->stop_bit <= HAL_UART_STOP_BIT_2) &&
            (config->parity <= HAL_UART_PARITY_EVEN));
}



/* triggered by vfifo dma rx thershold interrupt or UART receive timeout interrupt.
 * 1. When vfifo dma rx thershold interrupt happen,
 * this function is called with is_timeout=false.
 * then call suer's callback to notice that data can be fetched from receive buffer.
 * 2. When UART receive timeout interrupt happen,
 * this function is called with is_timeout=true.
 * then call suer's callback to notice that data can be fetched from receive buffer.
 */
<<<<<<< HEAD
=======
static uint32_t rx_irq_count   =0;
>>>>>>> db20e11 (second commit)

void uart_receive_handler(hal_uart_port_t uart_port, bool is_timeout)
{
    vdma_channel_t channel;
    uint32_t avail_bytes;
    hal_uart_callback_t callback;
    void *arg;
    UART_REGISTER_T *uartx;
    vdma_status_t status;
    uint8_t vdma_irq_is_enabled;

    if (g_uart_hwstatus[uart_port] != UART_HWSTATUS_DMA_INITIALIZED) {
        UART_ASSERT();
        return;
    }

<<<<<<< HEAD
=======
    if(uart_port == HAL_UART_0){
            rx_irq_count++;
            if ((rx_irq_count % 500) == 0){
                printf("uart rx irq, timeout=%d,uart_port= %d\n", is_timeout,uart_port);
            }
    }

>>>>>>> db20e11 (second commit)
    uartx = g_uart_regbase[uart_port];
    channel = uart_port_to_dma_channel(uart_port, 1);

    if (is_timeout == true) {
        uart_clear_timeout_interrupt(uartx);
    }

    status = vdma_get_available_receive_bytes(channel, &avail_bytes);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return;
    }
    if (avail_bytes == 0) {
        return;
    }

    callback = g_uart_callback[uart_port].func;
    arg = g_uart_callback[uart_port].arg;
    if (callback == NULL) {
        UART_ASSERT();
        return;
    }
    /* for callback re-entry issue */
    /* Avoid user didn't get data in the callback, then user will receive one dummy irq*/
    vdma_get_interrupt_status(channel, &vdma_irq_is_enabled);
    if (vdma_irq_is_enabled == true) {
        vdma_disable_interrupt(channel);
    }
    callback(HAL_UART_EVENT_READY_TO_READ, arg);
    if (vdma_irq_is_enabled == true) {
        vdma_enable_interrupt(channel);
    }

    status = vdma_get_available_receive_bytes(channel, &avail_bytes);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return;
    }

    if (avail_bytes >= g_uart_dma_config[uart_port].receive_vfifo_threshold_size) {
        status = vdma_disable_interrupt(channel);
        if (status != VDMA_OK) {
            UART_ASSERT();
            return;
        }
    }

}

/* triggered by vfifo dma tx thershold interrupt or uart transmit complete interrupt.
 * 1. When vfifo dma tx thershold interrupt happen,
 * this function is called with is_send_complete_trigger=false.
 * then call suer's callback to notice that data can be put in send buffer again.
 * 2. When UART transmit complete interrupt happen,
 * this function is called with is_send_complete_trigger=true.
 * Now all user data has been sent out, so we call hal_sleep_manager_unlock_sleep()
 * to unlock sleep.
 */
void uart_send_handler(hal_uart_port_t uart_port, bool is_send_complete_trigger)
{
    vdma_channel_t channel;
    vdma_status_t status;
    uint32_t compare_space, avail_space;
    hal_uart_callback_t callback;
    void *arg;
#ifdef HAL_SLEEP_MANAGER_ENABLED
    uint32_t irq_status;
#endif

    if (g_uart_hwstatus[uart_port] != UART_HWSTATUS_DMA_INITIALIZED) {
        UART_ASSERT();
        return;
    }

    channel = uart_port_to_dma_channel(uart_port, 0);
    if (is_send_complete_trigger == true) {
#ifdef HAL_SLEEP_MANAGER_ENABLED
        if (g_uart_frist_send_complete_interrupt[uart_port] == false) {
            hal_nvic_save_and_set_interrupt_mask(&irq_status);
            status = vdma_get_available_send_space(channel, &avail_space);
            if (status != VDMA_OK) {
                hal_nvic_restore_interrupt_mask(irq_status);
                UART_ASSERT();
                return;
            }

            /*when driver discern [is_send_complete_trigger] is true,it means HW fifo empty IRQ triggered
              if driver discerb DMA vfifo empty at the moment, uart tx unlock sleep.
             */
            if (g_uart_dma_config[uart_port].send_vfifo_buffer_size - avail_space == 0) {
                if (g_uart_send_lock_status[uart_port] == true) {
                    if( hal_sleep_manager_is_sleep_handle_alive(uart_sleep_handle[uart_port]) == true) {
                        hal_sleep_manager_unlock_sleep(uart_sleep_handle[uart_port]);
                    }
                    g_uart_send_lock_status[uart_port] = false;
                }
            }
            hal_nvic_restore_interrupt_mask(irq_status);
        } else {
            g_uart_frist_send_complete_interrupt[uart_port] = false;
        }
        return;
#endif
    } else {
        callback = g_uart_callback[uart_port].func;
        arg = g_uart_callback[uart_port].arg;
        if (callback == NULL) {
            UART_ASSERT();
            return;
        }
        /* for callback re-entry issue */
        vdma_disable_interrupt(channel);
        callback(HAL_UART_EVENT_READY_TO_WRITE, arg);
        vdma_enable_interrupt(channel);

        status = vdma_get_available_send_space(channel, &avail_space);
        if (status != VDMA_OK) {
            UART_ASSERT();
            return;
        }
        compare_space = g_uart_dma_config[uart_port].send_vfifo_buffer_size
                        - g_uart_dma_config[uart_port].send_vfifo_threshold_size;
        if (avail_space >= compare_space) {
            status = vdma_disable_interrupt(channel);
            if (status != VDMA_OK) {
                UART_ASSERT();
                return;
            }
        }
    }
}

/* Only triggered by UART error interrupt */
void uart_error_handler(hal_uart_port_t uart_port)
{
    UART_REGISTER_T *uartx;
    hal_uart_callback_t callback;
    void *arg;
<<<<<<< HEAD
=======
    log_hal_error("uart_error_handler port = %d\r\n",uart_port);
>>>>>>> db20e11 (second commit)

    uartx = g_uart_regbase[uart_port];

    if (!uart_verify_error(uartx)) {
        uart_purge_fifo(uartx, 1);
        uart_purge_fifo(uartx, 0);
        callback = g_uart_callback[uart_port].func;
        arg = g_uart_callback[uart_port].arg;
        if (callback == NULL) {
            UART_ASSERT();
            return;
        }
        callback(HAL_UART_EVENT_FRAMING_ERROR, arg);
    }
}

static void uart_dma_callback_handler(vdma_event_t event, void  *user_data)
{
    /* fix warning */
    vdma_event_t local_event = event;
    local_event = local_event;
    uart_dma_callback_data_t *callback_data = (uart_dma_callback_data_t *)user_data;

    if (callback_data->is_rx == true) {
        uart_receive_handler(callback_data->uart_port, false);
    } else {
        uart_send_handler(callback_data->uart_port, false);
    }
}

#ifdef HAL_SLEEP_MANAGER_ENABLED
static const hal_eint_number_t g_uart_eint_num[HAL_UART_MAX] = {HAL_EINT_UART_0_RX, HAL_EINT_UART_1_RX};
extern uint32_t eint_get_status(void);

#ifdef UART_WAKE_UP_ENABLED
#if 0
static  void    uart_eint_event_process(hal_uart_port_t uart_port)
{
    if ((eint_get_status() >> g_uart_eint_num[uart_port]) & 0x01) {
        hal_uart_callback_t callback;
        void *arg;

        hal_eint_unmask(g_uart_eint_num[uart_port]);
        /* call ATCI callback */
        callback = g_uart_callback[uart_port].func;
        arg = g_uart_callback[uart_port].arg;
        if (callback == NULL) {
            UART_ASSERT();
        }
        callback(HAL_UART_EVENT_WAKEUP_SLEEP, arg);
    }
}
#endif
static void eint_uart_handler(void *parameter)
{
    hal_uart_port_t uart_port = *((hal_uart_port_t *)parameter);
    hal_uart_callback_t callback;
    void *arg;
    /*
        uart configures all existing ports in eint mode in backup.
        eint isr is executed after the restore function is completed,
        but only the port triggered by eint is executed and deinit eint.
    */

    /* call user callback .send wakeup event to user */
    callback = g_uart_callback[uart_port].func;
    arg = g_uart_callback[uart_port].arg;
    if (callback != NULL) {
        callback(HAL_UART_EVENT_WAKEUP_SLEEP, arg);
    }

    hal_eint_deinit(g_uart_eint_num[uart_port]);

}
#endif/*UART_WAKE_UP_ENABLED     */

void uart_backup_all_registers(void)
{
    hal_uart_port_t uart_port;
    uart_hwstatus_t uart_hwstatus;
    uart_flowcontrol_t uart_flowcontrol;
    hal_uart_callback_t uart_callback;
    void *uart_callback_arg;
    //UART_REGISTER_T *uartx;

    for (uart_port = HAL_UART_0; uart_port < HAL_UART_MAX; uart_port++) {
            /*
            if( (g_uart_flowcontrol_status[uart_port] == UART_FLOWCONTROL_SOFTWARE) ) {
                uartx = g_uart_regbase[uart_port];
                uart_put_char_block(uartx, g_uart_sw_flowcontrol_config[uart_port].xon);
            }*/
            uart_hwstatus = g_uart_hwstatus[uart_port];
            uart_flowcontrol = g_uart_flowcontrol_status[uart_port];
            uart_callback = g_uart_callback[uart_port].func;
            uart_callback_arg = g_uart_callback[uart_port].arg;
            hal_uart_deinit(uart_port);
            g_uart_hwstatus[uart_port] = uart_hwstatus;
            g_uart_flowcontrol_status[uart_port] = uart_flowcontrol;
            g_uart_callback[uart_port].func = uart_callback;
            g_uart_callback[uart_port].arg = uart_callback_arg;

#ifdef UART_WAKE_UP_ENABLED
            hal_eint_config_t config_eint;
            g_uart_eint_port[uart_port] = uart_port;
            config_eint.trigger_mode = HAL_EINT_EDGE_FALLING;
            config_eint.debounce_time = 0;
            hal_eint_init(g_uart_eint_num[uart_port], &config_eint);
            hal_eint_register_callback(g_uart_eint_num[uart_port], eint_uart_handler, (void *) &g_uart_eint_port[uart_port]);
            hal_eint_unmask(g_uart_eint_num[uart_port]);
#endif
    }
}

void uart_restore_all_registers(void)
{
    uint32_t delay;
    hal_uart_port_t uart_port;
    UART_REGISTER_T *uartx;
    uart_hwstatus_t uart_hwstatus;

    for (uart_port = HAL_UART_0; uart_port < HAL_UART_MAX; uart_port++) {
            /*  Because, DMA's HW read & write pointer's value are same as MUX's software pointer.
                But, DMA's HW read & write pointer will reset after UART's operation of deinit&init.
                So, uart need reset reset software pointer in MUX layer,manually.
            */
            mux_restore_callback_port(g_uart_mux_port[uart_port]);
            /*it's useful to use global value (g_uart_hwstatus) to assign value to uart_hwstatus as a judgment condition.
              because hal_uart_init() will change g_uart_hwstatus to POLL_INITIALIZED that cause DMA mode initialization failure.
            */
            uart_hwstatus = g_uart_hwstatus[uart_port];
            if (uart_hwstatus != UART_HWSTATUS_UNINITIALIZED) {
                /*reset Hardware status as uninitialized to reinit*/
                g_uart_hwstatus[uart_port] = UART_HWSTATUS_UNINITIALIZED;
                hal_uart_init(uart_port, &g_uart_config[uart_port]);
            }
            /* Because of hardware limitation, we have to send XON manually
            * when software flow control is turn on for that port.
            */
            if (g_uart_flowcontrol_status[uart_port] == UART_FLOWCONTROL_SOFTWARE) {
                uartx = g_uart_regbase[uart_port];
                uart_put_char_block(uartx, g_uart_sw_flowcontrol_config[uart_port].xon);
                /* must delay until xon character is sent out, to avoid xon error with escape */
                delay = ((10000000) / g_uart_baudrate_map[g_uart_config[uart_port].baudrate]) + 2;
                hal_gpt_delay_us(delay);
                hal_uart_set_software_flowcontrol(uart_port,
                                                  g_uart_sw_flowcontrol_config[uart_port].xon,
                                                  g_uart_sw_flowcontrol_config[uart_port].xoff,
                                                  g_uart_sw_flowcontrol_config[uart_port].escape_character);
            } else if (g_uart_flowcontrol_status[uart_port] == UART_FLOWCONTROL_HARDWARE) {
                hal_uart_set_hardware_flowcontrol(uart_port);
            }

            if (uart_hwstatus == UART_HWSTATUS_DMA_INITIALIZED) {
                hal_uart_set_dma(uart_port, &g_uart_dma_config[uart_port]);
                hal_uart_register_callback(uart_port, g_uart_callback[uart_port].func, g_uart_callback[uart_port].arg);
            }
    }
    if( g_uart_not_wait_empty_status == true) {
        printf("warning:uart received xoff\r\n");
        g_uart_not_wait_empty_status = false;
    }

}

#endif

hal_uart_status_t hal_uart_set_baudrate(hal_uart_port_t uart_port, hal_uart_baudrate_t baudrate)
{
    uint32_t actual_baudrate;
    uint32_t  irq_status;
    UART_REGISTER_T *uartx;

    if ((!uart_port_is_valid(uart_port)) ||
        (!uart_baudrate_is_valid(baudrate))) {
        return HAL_UART_STATUS_ERROR_PARAMETER;
    }

    if (g_uart_hwstatus[uart_port] == UART_HWSTATUS_UNINITIALIZED) {
        return HAL_UART_STATUS_ERROR_UNINITIALIZED;
    }

    hal_nvic_save_and_set_interrupt_mask(&irq_status);
#ifdef HAL_SLEEP_MANAGER_ENABLED
    g_uart_config[uart_port].baudrate = baudrate;
#endif
    g_baudrate[uart_port] = baudrate;
    hal_nvic_restore_interrupt_mask(irq_status);

    uartx = g_uart_regbase[uart_port];
    actual_baudrate = g_uart_baudrate_map[baudrate];

    uart_set_baudrate(uartx, actual_baudrate);

    return HAL_UART_STATUS_OK;
}

hal_uart_status_t hal_uart_set_format(hal_uart_port_t uart_port,
                                      const hal_uart_config_t *config)
{
    uint32_t irq_status;
    UART_REGISTER_T *uartx;

    if ((!uart_port_is_valid(uart_port)) ||
        (!uart_config_is_valid(config))) {
        return HAL_UART_STATUS_ERROR_PARAMETER;
    }

    if (g_uart_hwstatus[uart_port] == UART_HWSTATUS_UNINITIALIZED) {
        return HAL_UART_STATUS_ERROR_UNINITIALIZED;
    }

    hal_nvic_save_and_set_interrupt_mask(&irq_status);
#ifdef HAL_SLEEP_MANAGER_ENABLED
    g_uart_config[uart_port].baudrate = config->baudrate;
    g_uart_config[uart_port].word_length = config->word_length;
    g_uart_config[uart_port].stop_bit = config->stop_bit;
    g_uart_config[uart_port].parity = config->parity;
#endif
    hal_nvic_restore_interrupt_mask(irq_status);

    uartx = g_uart_regbase[uart_port];

    hal_uart_set_baudrate(uart_port, config->baudrate);
    uart_set_format(uartx, config->word_length, config->stop_bit, config->parity);

    return HAL_UART_STATUS_OK;
}

hal_uart_status_t hal_uart_init(hal_uart_port_t uart_port, hal_uart_config_t *uart_config)
{
    UART_REGISTER_T *uartx;
    uint32_t i, actual_baudrate;
    uint32_t irq_status;
    hal_clock_status_t clock_status;

    if ((!uart_port_is_valid(uart_port)) ||
        (!uart_config_is_valid(uart_config))) {
        return HAL_UART_STATUS_ERROR_PARAMETER;
    }

    hal_nvic_save_and_set_interrupt_mask(&irq_status);
    if (g_uart_hwstatus[uart_port] != UART_HWSTATUS_UNINITIALIZED) {
        hal_nvic_restore_interrupt_mask(irq_status);
        return HAL_UART_STATUS_ERROR_BUSY;
    }

    if (g_uart_global_data_initialized == false) {
        for (i = 0; i < HAL_UART_MAX; i++) {
            g_uart_hwstatus[i] = UART_HWSTATUS_UNINITIALIZED;
#ifdef HAL_SLEEP_MANAGER_ENABLED
            g_uart_flowcontrol_status[i] = UART_FLOWCONTROL_NONE;
            g_uart_frist_send_complete_interrupt[i] = false;
            g_uart_send_lock_status[i] = false;
#endif
            g_uart_callback[i].arg = NULL;
            g_uart_callback[i].func = NULL;
        }
        g_uart_global_data_initialized = true;
    }
#ifdef HAL_SLEEP_MANAGER_ENABLED
    g_uart_config[uart_port].baudrate = uart_config->baudrate;
    g_uart_config[uart_port].word_length = uart_config->word_length;
    g_uart_config[uart_port].stop_bit = uart_config->stop_bit;
    g_uart_config[uart_port].parity = uart_config->parity;
#endif
    g_uart_hwstatus[uart_port] = UART_HWSTATUS_POLL_INITIALIZED;
    hal_nvic_restore_interrupt_mask(irq_status);

    clock_status = hal_clock_enable(g_uart_port_to_pdn[uart_port]);
    if (clock_status != HAL_CLOCK_STATUS_OK) {
        UART_ASSERT();
        return HAL_UART_STATUS_ERROR;
    }

    uartx = g_uart_regbase[uart_port];

    uart_reset_default_value(uartx);
    actual_baudrate = g_uart_baudrate_map[uart_config->baudrate];
    g_baudrate[uart_port] = uart_config->baudrate;
    uart_set_baudrate(uartx, actual_baudrate);
    uart_set_format(uartx, uart_config->word_length, uart_config->stop_bit, uart_config->parity);
    uart_set_fifo(uartx);
#ifdef HAL_SLEEP_MANAGER_ENABLED
    //uart_set_sleep_mode(uartx);
    uart_set_sleep_idle_fc_mode(uartx);
    sleep_management_register_suspend_callback(SLEEP_BACKUP_RESTORE_UART, (sleep_management_suspend_callback_t)uart_backup_all_registers, NULL);
    sleep_management_register_resume_callback(SLEEP_BACKUP_RESTORE_UART, (sleep_management_resume_callback_t)uart_restore_all_registers, NULL);
#endif
    return HAL_UART_STATUS_OK;
}

hal_uart_status_t hal_uart_deinit(hal_uart_port_t uart_port)
{
    UART_REGISTER_T *uartx;
    VDMA_REGISTER_T *dmax;
    vdma_channel_t tx_dma_channel, rx_dma_channel;
    uint32_t irq_status;
    vdma_status_t status;
    hal_clock_status_t clock_status;

    if (!uart_port_is_valid(uart_port)) {
        return HAL_UART_STATUS_ERROR_PARAMETER;
    }


    if (g_uart_hwstatus[uart_port] == UART_HWSTATUS_UNINITIALIZED) {
        return HAL_UART_STATUS_ERROR_UNINITIALIZED;
    }

    uartx = g_uart_regbase[uart_port];

    /* wait all left data sent out before deinit. */
    uart_wait_empty(uart_port);
    /* unregister vdma module */
    if (g_uart_hwstatus[uart_port] == UART_HWSTATUS_DMA_INITIALIZED) {
        tx_dma_channel = uart_port_to_dma_channel(uart_port, 0);
        rx_dma_channel = uart_port_to_dma_channel(uart_port, 1);

        status = vdma_disable_interrupt(tx_dma_channel);
        if (status != VDMA_OK) {
            UART_ASSERT();
            return HAL_UART_STATUS_ERROR;
        }
        status = vdma_disable_interrupt(rx_dma_channel);
        if (status != VDMA_OK) {
            UART_ASSERT();
            return HAL_UART_STATUS_ERROR;
        }

        status = vdma_stop(tx_dma_channel);
        if (status != VDMA_OK) {
            UART_ASSERT();
            return HAL_UART_STATUS_ERROR;
        }
        status = vdma_stop(rx_dma_channel);
        if (status != VDMA_OK) {
            UART_ASSERT();
            return HAL_UART_STATUS_ERROR;
        }

        //ack pending vdma irq
        dmax = (VDMA_REGISTER_T*)g_vdma_regbase[uart_port][0];
        dmax->VDMA_ACKINT = 0x8000;

        status = vdma_deinit(tx_dma_channel);
        if (status != VDMA_OK) {
            UART_ASSERT();
            return HAL_UART_STATUS_ERROR;
        }
        status = vdma_deinit(rx_dma_channel);
        if (status != VDMA_OK) {
            UART_ASSERT();
            return HAL_UART_STATUS_ERROR;
        }

        hal_nvic_disable_irq(g_uart_port_to_irq_num[uart_port]);

    }

    uart_reset_default_value(uartx);
    //clr pending uart irq
    uart_query_interrupt_type(uartx);


    clock_status = hal_clock_disable(g_uart_port_to_pdn[uart_port]);
    if (clock_status != HAL_CLOCK_STATUS_OK) {
        UART_ASSERT();
        return HAL_UART_STATUS_ERROR;
    }

    g_uart_callback[uart_port].func = NULL;
    g_uart_callback[uart_port].arg = NULL;

    hal_nvic_save_and_set_interrupt_mask(&irq_status);
#ifdef HAL_SLEEP_MANAGER_ENABLED
    g_uart_frist_send_complete_interrupt[uart_port] = false;
    g_uart_flowcontrol_status[uart_port] = UART_FLOWCONTROL_NONE;
#endif
    g_uart_hwstatus[uart_port] = UART_HWSTATUS_UNINITIALIZED;

    hal_nvic_restore_interrupt_mask(irq_status);

    return HAL_UART_STATUS_OK;
}

void hal_uart_put_char(hal_uart_port_t uart_port, char byte)
{
    UART_REGISTER_T *uartx;

    if (!uart_port_is_valid(uart_port)) {
        return;
    }

    if (g_uart_hwstatus[uart_port] != UART_HWSTATUS_POLL_INITIALIZED) {
        return;
    }

    uartx = g_uart_regbase[uart_port];
    uart_put_char_block(uartx, byte);
}

uint32_t hal_uart_send_polling(hal_uart_port_t uart_port, const uint8_t *data, uint32_t size)
{
    uint32_t i = 0;

    if ((!uart_port_is_valid(uart_port)) || (data == NULL)) {
        return 0;
    }

    if (g_uart_hwstatus[uart_port] != UART_HWSTATUS_POLL_INITIALIZED) {
        return 0;
    }

    for (i = 0; i < size; i++) {
        hal_uart_put_char(uart_port, *data);
        data++;
    }

    return size;
}


char hal_uart_get_char(hal_uart_port_t uart_port)
{
    char data;
    UART_REGISTER_T *uartx;

    if (!uart_port_is_valid(uart_port)) {
        return 0;
    }

    if (g_uart_hwstatus[uart_port] != UART_HWSTATUS_POLL_INITIALIZED) {
        return 0;
    }

    uartx = g_uart_regbase[uart_port];

    data = uart_get_char_block(uartx);

    return data;
}

uint32_t hal_uart_get_char_unblocking(hal_uart_port_t uart_port)
{
    uint32_t data;
    UART_REGISTER_T *uartx;

    if (!uart_port_is_valid(uart_port)) {
        return 0;
    }

    if (g_uart_hwstatus[uart_port] != UART_HWSTATUS_POLL_INITIALIZED) {
        return 0;
    }

    uartx = g_uart_regbase[uart_port];

    data = uart_get_char_unblocking(uartx);

    return data;
}

uint32_t hal_uart_receive_polling(hal_uart_port_t uart_port, uint8_t *buffer, uint32_t size)
{
    uint32_t i;
    uint8_t *pbuf = buffer;

    if ((!uart_port_is_valid(uart_port)) ||
        (buffer == NULL)) {
        return 0;
    }

    if (g_uart_hwstatus[uart_port] != UART_HWSTATUS_POLL_INITIALIZED) {
        return 0;
    }

    for (i = 0; i < size; i++) {
        pbuf[i] = hal_uart_get_char(uart_port);
    }

    return size;
}

uint32_t hal_uart_send_dma(hal_uart_port_t uart_port, const uint8_t *data, uint32_t size)
{
    vdma_channel_t channel;
    uint32_t real_count, avail_space;
    vdma_status_t status;
#ifdef HAL_SLEEP_MANAGER_ENABLED
    uint32_t irq_status;
#endif

    if ((!uart_port_is_valid(uart_port)) || (data == NULL) || (size == 0)) {
        return 0;
    }

    if (g_uart_hwstatus[uart_port] != UART_HWSTATUS_DMA_INITIALIZED) {
        return 0;
    }

#ifdef HAL_SLEEP_MANAGER_ENABLED
    /*UART port except logging port need lock sleep before staring one data transmition
      UART need unlock sleep in uart_send_handler() when uart hardware fifo empty which means one data tansmit done.
    */
    hal_nvic_save_and_set_interrupt_mask(&irq_status);
    if (g_uart_send_lock_status[uart_port] == false) {
        hal_sleep_manager_lock_sleep(uart_sleep_handle[uart_port]);
        g_uart_send_lock_status[uart_port] = true;
    }
    hal_nvic_restore_interrupt_mask(irq_status);
#endif

    channel = uart_port_to_dma_channel(uart_port, 0);

    status = vdma_get_available_send_space(channel, &avail_space);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return 0;
    }

    if (avail_space >= size) {
        real_count = size;
    } else {
        real_count = avail_space;
    }

    /* Update to multiple byte push operation */
    status = vdma_push_data_multi_bytes(channel, (uint8_t *)data, real_count);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return 0;
    }

    /* If avail space is not enough, turn on TX IRQ
       * so that UART driver can notice user when user's data has been sent out.
       */
    if (real_count == avail_space) {
        status = vdma_enable_interrupt(channel);
        if (status != VDMA_OK) {
            UART_ASSERT();
            return 0;
        }
    }

    return real_count;
}

uint32_t hal_uart_receive_dma(hal_uart_port_t uart_port, uint8_t *buffer, uint32_t size)
{
    vdma_channel_t channel;
    uint32_t receive_count, avail_count;
    vdma_status_t status;
    //uint32_t irq_status;

    if ((!uart_port_is_valid(uart_port)) ||
        (buffer == NULL) ||
        (size == 0)) {
        return 0;
    }

    if (g_uart_hwstatus[uart_port] != UART_HWSTATUS_DMA_INITIALIZED) {
        return 0;
    }

    channel = uart_port_to_dma_channel(uart_port, 1);

    status = vdma_get_available_receive_bytes(channel, &avail_count);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return 0;
    }

    if (avail_count < size) {
        receive_count = avail_count;
    } else {
        receive_count = size;
    }

    status = vdma_pop_data_multi_bytes(channel, buffer, receive_count);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return 0;
    }

    /* If avail bytes is not enough, turn on RX IRQ
       * so that UART driver can notice user when new user's data has been received.
       */
    if (receive_count == avail_count) {
        status = vdma_enable_interrupt(channel);
        if (status != VDMA_OK) {
            UART_ASSERT();
            return 0;
        }
    }

    return receive_count;
}

static void uart_start_dma_transmission(hal_uart_port_t uart_port)
{
    UART_REGISTER_T *uartx;
    vdma_channel_t tx_dma_channel, rx_dma_channel;
    vdma_status_t status;

    uartx = g_uart_regbase[uart_port];
    tx_dma_channel = uart_port_to_dma_channel(uart_port, 0);
    rx_dma_channel = uart_port_to_dma_channel(uart_port, 1);

    uart_mask_send_interrupt(uartx);
    uart_unmask_receive_interrupt(uartx);
<<<<<<< HEAD
=======

#if 0
    /*for 1-wire uart
    *  disable UART RX timeout/full/greaterthan threadshold  interrupt.
    * *So 1-wire uart only use DMA RX irq to inform user read-to-read.
    * */
    if(one_wire_disable_rx_irq_flag == true){
        *(volatile uint32_t*)0xA307000C = 0x200;
    }
#endif
>>>>>>> db20e11 (second commit)
#ifdef HAL_SLEEP_MANAGER_ENABLED
    g_uart_frist_send_complete_interrupt[uart_port] = true;
#endif
    hal_nvic_enable_irq(g_uart_port_to_irq_num[uart_port]);  /*open uart interrupt after changing global variable "g_uart_frist_send_complete_interrupt" */

    status = vdma_enable_interrupt(tx_dma_channel);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return;
    }

    status = vdma_enable_interrupt(rx_dma_channel);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return;
    }
    status = vdma_start(tx_dma_channel);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return;
    }
    status = vdma_start(rx_dma_channel);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return;
    }
    uart_enable_dma(uartx);
    uart_set_fifo(uartx);
}


hal_uart_status_t hal_uart_set_dma(hal_uart_port_t uart_port, const hal_uart_dma_config_t *dma_config)
{
    uint32_t irq_status;
    vdma_config_t internal_dma_config;
    vdma_channel_t tx_dma_channel, rx_dma_channel;
    vdma_status_t status;

    if (!uart_port_is_valid(uart_port)) {
        return HAL_UART_STATUS_ERROR_PARAMETER;
    }
    if ((dma_config->send_vfifo_buffer == NULL) ||
        (dma_config->receive_vfifo_buffer == NULL)) {
        return HAL_UART_STATUS_ERROR_PARAMETER;
    }
    if ((dma_config->send_vfifo_buffer_size >= UART_DMA_MAX_SETTING_VALUE) ||
        (dma_config->send_vfifo_threshold_size >= UART_DMA_MAX_SETTING_VALUE) ||
        (dma_config->send_vfifo_threshold_size > dma_config->send_vfifo_buffer_size)) {
        return HAL_UART_STATUS_ERROR_PARAMETER;
    }
    if ((dma_config->receive_vfifo_buffer_size >= UART_DMA_MAX_SETTING_VALUE) ||
        (dma_config->receive_vfifo_threshold_size >= UART_DMA_MAX_SETTING_VALUE) ||
        (dma_config->receive_vfifo_alert_size >= UART_DMA_MAX_SETTING_VALUE) ||
        (dma_config->receive_vfifo_threshold_size > dma_config->receive_vfifo_buffer_size) ||
        (dma_config->receive_vfifo_alert_size > dma_config->receive_vfifo_buffer_size)) {
        return HAL_UART_STATUS_ERROR_PARAMETER;
    }

    if (g_uart_hwstatus[uart_port] != UART_HWSTATUS_POLL_INITIALIZED) {
        return HAL_UART_STATUS_ERROR_UNINITIALIZED;
    }

    tx_dma_channel = uart_port_to_dma_channel(uart_port, 0);
    rx_dma_channel = uart_port_to_dma_channel(uart_port, 1);

    status = vdma_init(tx_dma_channel);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return HAL_UART_STATUS_ERROR;
    }
    internal_dma_config.base_address = (uint32_t)dma_config->send_vfifo_buffer;
    internal_dma_config.size = dma_config->send_vfifo_buffer_size;
    status = vdma_configure(tx_dma_channel, &internal_dma_config);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return HAL_UART_STATUS_ERROR;
    }
    status = vdma_set_threshold(tx_dma_channel, dma_config->send_vfifo_threshold_size);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return HAL_UART_STATUS_ERROR;
    }

    status = vdma_init(rx_dma_channel);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return HAL_UART_STATUS_ERROR;
    }
    internal_dma_config.base_address = (uint32_t)dma_config->receive_vfifo_buffer;
    internal_dma_config.size = dma_config->receive_vfifo_buffer_size;
    status = vdma_configure(rx_dma_channel, &internal_dma_config);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return HAL_UART_STATUS_ERROR;
    }
    status = vdma_set_threshold(rx_dma_channel, dma_config->receive_vfifo_threshold_size);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return HAL_UART_STATUS_ERROR;
    }
    status = vdma_set_alert_length(rx_dma_channel, dma_config->receive_vfifo_alert_size);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return HAL_UART_STATUS_ERROR;
    }

    hal_nvic_save_and_set_interrupt_mask(&irq_status);
    g_uart_dma_config[uart_port].send_vfifo_buffer = dma_config->send_vfifo_buffer;
    g_uart_dma_config[uart_port].send_vfifo_buffer_size = dma_config->send_vfifo_buffer_size;
    g_uart_dma_config[uart_port].send_vfifo_threshold_size = dma_config->send_vfifo_threshold_size;
    g_uart_dma_config[uart_port].receive_vfifo_alert_size = dma_config->receive_vfifo_alert_size;
    g_uart_dma_config[uart_port].receive_vfifo_buffer = dma_config->receive_vfifo_buffer;
    g_uart_dma_config[uart_port].receive_vfifo_buffer_size = dma_config->receive_vfifo_buffer_size;
    g_uart_dma_config[uart_port].receive_vfifo_threshold_size = dma_config->receive_vfifo_threshold_size;
    hal_nvic_restore_interrupt_mask(irq_status);

    return HAL_UART_STATUS_OK;
}


hal_uart_status_t hal_uart_register_callback(hal_uart_port_t uart_port,
        hal_uart_callback_t user_callback,
        void *user_data)
{
    vdma_channel_t tx_dma_channel, rx_dma_channel;
    uint32_t irq_status;
    vdma_status_t status;
    hal_nvic_status_t nvic_status;

    if ((!uart_port_is_valid(uart_port)) ||
        (user_callback == NULL)) {
        return HAL_UART_STATUS_ERROR_PARAMETER;
    }

    if (g_uart_hwstatus[uart_port] != UART_HWSTATUS_POLL_INITIALIZED) {
        return HAL_UART_STATUS_ERROR_UNINITIALIZED;
    }

    tx_dma_channel = uart_port_to_dma_channel(uart_port, 0);
    rx_dma_channel = uart_port_to_dma_channel(uart_port, 1);

    g_uart_callback[uart_port].func = user_callback;
    g_uart_callback[uart_port].arg = user_data;

    uart_dma_channel_to_callback_data(tx_dma_channel, &g_uart_dma_callback_data[uart_port * 2]);
    status = vdma_register_callback(tx_dma_channel, uart_dma_callback_handler, &g_uart_dma_callback_data[uart_port * 2]);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return HAL_UART_STATUS_ERROR;
    }
    uart_dma_channel_to_callback_data(rx_dma_channel, &g_uart_dma_callback_data[(uart_port * 2) + 1]);
    status = vdma_register_callback(rx_dma_channel, uart_dma_callback_handler, &g_uart_dma_callback_data[(uart_port * 2) + 1]);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return HAL_UART_STATUS_ERROR;
    }

    hal_nvic_save_and_set_interrupt_mask(&irq_status);
    nvic_status = hal_nvic_register_isr_handler(g_uart_port_to_irq_num[uart_port], uart_interrupt_handler);
    if (nvic_status != HAL_NVIC_STATUS_OK) {
        UART_ASSERT();
        return HAL_UART_STATUS_ERROR;
    }
    g_uart_hwstatus[uart_port] = UART_HWSTATUS_DMA_INITIALIZED;
    hal_nvic_restore_interrupt_mask(irq_status);
    uart_start_dma_transmission(uart_port);
    return HAL_UART_STATUS_OK;
}

uint32_t hal_uart_get_available_send_space(hal_uart_port_t uart_port)
{
    vdma_channel_t channel;
    uint32_t roomleft;
    vdma_status_t status;

    if (!uart_port_is_valid(uart_port)) {
        return 0;
    }

    if (g_uart_hwstatus[uart_port] != UART_HWSTATUS_DMA_INITIALIZED) {
        return 0;
    }

    channel = uart_port_to_dma_channel(uart_port, 0);
    status = vdma_get_available_send_space(channel, &roomleft);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return 0;
    }

    return roomleft;
}

uint32_t hal_uart_get_available_receive_bytes(hal_uart_port_t uart_port)
{
    vdma_channel_t channel;
    uint32_t avail;
    vdma_status_t status;

    if (!uart_port_is_valid(uart_port)) {
        return 0;
    }

    if (g_uart_hwstatus[uart_port] != UART_HWSTATUS_DMA_INITIALIZED) {
        return 0;
    }

    channel = uart_port_to_dma_channel(uart_port, 1);
    status = vdma_get_available_receive_bytes(channel, &avail);
    if (status != VDMA_OK) {
        UART_ASSERT();
        return 0;
    }

    return avail;
}

hal_uart_status_t hal_uart_set_dma_timeout(hal_uart_port_t uart_port, uint32_t timeout)
{
    UART_REGISTER_T *uartx;

    if (!uart_port_is_valid(uart_port)) {
        return HAL_UART_STATUS_ERROR_PARAMETER;
    }
    if (timeout > HAL_UART_TIMEOUT_VALUE_MAX) {
        return HAL_UART_STATUS_ERROR_PARAMETER;
    }

    uartx = g_uart_regbase[uart_port];
    uart_set_timeout_value(uartx, timeout);

    return HAL_UART_STATUS_OK;
}

hal_uart_status_t hal_uart_set_hardware_flowcontrol(hal_uart_port_t uart_port)
{
    UART_REGISTER_T *uartx;

    if (!uart_port_is_valid(uart_port)) {
        return HAL_UART_STATUS_ERROR_PARAMETER;
    }

    if (g_uart_hwstatus[uart_port] == UART_HWSTATUS_UNINITIALIZED) {
        return HAL_UART_STATUS_ERROR_UNINITIALIZED;
    }

    uartx = g_uart_regbase[uart_port];

    uart_set_hardware_flowcontrol(uartx);

#ifdef HAL_SLEEP_MANAGER_ENABLED
    g_uart_flowcontrol_status[uart_port] = UART_FLOWCONTROL_HARDWARE;
#endif

    return HAL_UART_STATUS_OK;
}

hal_uart_status_t hal_uart_set_software_flowcontrol(hal_uart_port_t uart_port,
        uint8_t xon,
        uint8_t xoff,
        uint8_t escape_character)
{
    UART_REGISTER_T *uartx;

    if (!uart_port_is_valid(uart_port)) {
        return HAL_UART_STATUS_ERROR_PARAMETER;
    }

    if (g_uart_hwstatus[uart_port] == UART_HWSTATUS_UNINITIALIZED) {
        return HAL_UART_STATUS_ERROR_UNINITIALIZED;
    }

    uartx = g_uart_regbase[uart_port];

    uart_set_software_flowcontrol(uartx, xon, xoff, escape_character);

#ifdef HAL_SLEEP_MANAGER_ENABLED
    g_uart_flowcontrol_status[uart_port] = UART_FLOWCONTROL_SOFTWARE;
    g_uart_sw_flowcontrol_config[uart_port].xon = xon;
    g_uart_sw_flowcontrol_config[uart_port].xoff = xoff;
    g_uart_sw_flowcontrol_config[uart_port].escape_character = escape_character;
#endif

    return HAL_UART_STATUS_OK;
}

hal_uart_status_t hal_uart_disable_flowcontrol(hal_uart_port_t uart_port)
{
    UART_REGISTER_T *uartx;

    if (!uart_port_is_valid(uart_port)) {
        return HAL_UART_STATUS_ERROR_PARAMETER;
    }

    if (g_uart_hwstatus[uart_port] == UART_HWSTATUS_UNINITIALIZED) {
        return HAL_UART_STATUS_ERROR_UNINITIALIZED;
    }

    uartx = g_uart_regbase[uart_port];

    uart_disable_flowcontrol(uartx);

#ifdef HAL_SLEEP_MANAGER_ENABLED
    g_uart_flowcontrol_status[uart_port] = UART_FLOWCONTROL_NONE;
#endif

    return HAL_UART_STATUS_OK;
}

hal_uart_status_t hal_uart_set_auto_baudrate(hal_uart_port_t uart_port, bool is_enable)
{
    UART_REGISTER_T *uartx;
    uint32_t current_baudrate;

    if (!uart_port_is_valid(uart_port)) {
        return HAL_UART_STATUS_ERROR_PARAMETER;
    }

    uartx = g_uart_regbase[uart_port];
    current_baudrate = g_uart_baudrate_map[g_baudrate[uart_port]];

    uart_set_auto_baudrate(uartx, is_enable, current_baudrate);
    return HAL_UART_STATUS_OK;
}

/* For logging
    1562 did not use this API to init logging instead using API in mux_uart.c
*/
hal_uart_status_t uart_init_for_logging(hal_uart_port_t uart_port,
                                        hal_uart_config_t *uart_config,
                                        const hal_uart_dma_config_t *dma_config,
                                        hal_uart_callback_t user_callback,
                                        void *user_data)
{
    hal_uart_init(uart_port, uart_config);
    hal_uart_set_dma(uart_port, dma_config);
    hal_uart_register_callback(uart_port, user_callback, user_data);
    g_uart_port_for_logging = uart_port;
    return HAL_UART_STATUS_OK;

}


volatile uint32_t wait_time_s,wait_time_e,wait_time_d;
volatile uint32_t uart_dma_tx_rptr = 0;

volatile uint32_t uart_log_rcv_xoff_flag = 0;  //for software flow control
volatile uint32_t uart_log_rcv_rts_flag = 0;   //for hardware flow control

bool uart_agree_logging_enter_sleep(void)
{
    vdma_channel_t  dma_channel;
    VDMA_REGISTER_T *dma_reg;
    uint32_t avail_space = 0;
    UART_REGISTER_T *uartx;
    uartx = g_uart_regbase[g_uart_port_for_logging];


    dma_channel = uart_port_to_dma_channel(g_uart_port_for_logging, 0);
    /* how many bytes are still empty in vfifo.*/
    if(VDMA_OK != vdma_get_available_send_space(dma_channel, &avail_space)) {
        UART_ASSERT();
        return 0;
    }

    dma_reg = (VDMA_REGISTER_T*)g_vdma_regbase[g_uart_port_for_logging][0];

    /*record dma read pointer for comparison*/
    uint32_t temp_record_rptr = dma_reg->VDMA_RDPTR;

    /* Conditional branch for hardware and software flow control  */
    /*software flow control*/
    if ( g_uart_flowcontrol_status[g_uart_port_for_logging] == UART_FLOWCONTROL_SOFTWARE ) {

            /*normal case: uart did not receive xoff char,but vfifo is not empty.To avoid losing log to request mcu can not enter sleep.
              result: mcu can not enter sleep,if vfifo is not empty.
                      mcu can enter sleep,if vfifo is empty.
            */
        if( ((uartx->MCR_UNION.MCR & UART_MCR_XOFF_STATUS_MASK)==0) && (g_uart_dma_config[g_uart_port_for_logging].send_vfifo_buffer_size - avail_space != 0)) {
            uart_log_rcv_xoff_flag = 0;
            return false;

            /*special case: uart loggoing port received xoff, actually here is the first or second time enter this function.
              Record the values of empty vfifo size and gpt count to facilitate subsequent comparisons
              result:  can not enter sleep.
            */
        } else if((uartx->MCR_UNION.MCR & UART_MCR_XOFF_STATUS_MASK) && (uart_log_rcv_xoff_flag == 0)) {
            uart_log_rcv_xoff_flag = 1;
            uart_dma_tx_rptr = temp_record_rptr; // record this size
            hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, (uint32_t*) &wait_time_s); //record "wait_time_s" gpt count
            return false;

            /*special case:Within 10ms, if the available size of vfifo is unchanged because of receiving xoff, it will not sleep temporarily.
                           If the peer sends xoff when disconnected due to special reasons, in order to avoid that the system can not sleep all the time,
                           it can be forced to sleep after more than 10 seconds, and hal_uart_deinit to reset hw setting.
              result:  MCU can not enter sleep.
            */
        } else if((uartx->MCR_UNION.MCR & UART_MCR_XOFF_STATUS_MASK) && (uart_log_rcv_xoff_flag == 1)) {
            hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, (uint32_t*) &wait_time_e);
            hal_gpt_get_duration_count(wait_time_s, wait_time_e, (uint32_t*) &wait_time_d);
            /*
            Only if the condition is satisfied: the query time is greater than 10ms, the read pointer has not changed.
                                                The MCU thinks that a special situation has occurred, and the peer is disconnected,
                                                so it discards the log and goes to sleep.
            Analysis of other conditions that cannot sleep:
                                        1.  (wait_time_d < 10000) && (uart_dma_tx_rptr == temp_record_rptr)
                                            reason:   uart received xoff and the read pointer did not change within 10ms.
                                                      The disadvantage is that SPM may need to wait more time every time it wants to enter Deep sleep
                                        2.  (wait_time_d < 10000) && (uart_dma_tx_rptr != temp_record_rptr)
                                            reason:   The next time SPM query whether MCU can enter deep sleep, but uart receives xoff again within 10ms.
                                                      At this time, the read pointer changes, indicating that uart is transmitting the log normally.
                                        3.  (wait_time_d >= 10000) && ( uart_dma_tx_rptr != temp_record_rptr )
                                            reason:   The next time SPM query whether MCU can enter deep sleep, but uart receives xoff again over 10ms.
                                                      At this time, the read pointer changes, indicating that uart is transmitting the log normally.
            */
            if(!((wait_time_d >= 10000) && ( uart_dma_tx_rptr == temp_record_rptr )))
                return false;
        }

        //clear flag before enter sleep.
        uart_log_rcv_xoff_flag = 0;
        //clear Comparison value.
        uart_dma_tx_rptr = 0;

    /*hardware flow control*/
    } else if (g_uart_flowcontrol_status[g_uart_port_for_logging] == UART_FLOWCONTROL_HARDWARE ) {

        if( ((uartx->MCR_UNION.MCR & UART_MCR_RTS_MASK)==0) && (g_uart_dma_config[g_uart_port_for_logging].send_vfifo_buffer_size - avail_space != 0)) {
            uart_log_rcv_rts_flag = 0;
            return false;
        } else if((uartx->MCR_UNION.MCR & UART_MCR_RTS_MASK) && (uart_log_rcv_rts_flag == 0)) {
            uart_log_rcv_rts_flag = 1;
            uart_dma_tx_rptr = temp_record_rptr;
            hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M,(uint32_t*) &wait_time_s);
            return false;
        } else if((uartx->MCR_UNION.MCR & UART_MCR_RTS_MASK) && (uart_log_rcv_rts_flag == 1)) {
            hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M,(uint32_t*) &wait_time_e);
            hal_gpt_get_duration_count(wait_time_s,wait_time_e,(uint32_t*) &wait_time_d);
            if(!((wait_time_d >= 10000) && ( uart_dma_tx_rptr == temp_record_rptr )))
                return false;
        }
        uart_log_rcv_rts_flag = 0;
        uart_dma_tx_rptr = 0;
    }

    /*uart can enter sleep*/
    return true;
}

uint32_t uart_get_hw_rptr(hal_uart_port_t uart_port, bool is_rx)
{
    vdma_channel_t dma_channel;
    uint32_t read_pointer;

    dma_channel = uart_port_to_dma_channel(uart_port, is_rx);
    vdma_get_hw_read_point(dma_channel, &read_pointer);

    if(is_rx)
    {
        return (read_pointer - (uint32_t)(g_uart_dma_config[uart_port].receive_vfifo_buffer));
    }
    else
    {
        return (read_pointer - (uint32_t)(g_uart_dma_config[uart_port].send_vfifo_buffer));
    }
}

hal_uart_status_t uart_set_sw_move_byte(hal_uart_port_t uart_port, bool is_rx, uint16_t sw_move_byte)
{
    vdma_status_t status;
    vdma_channel_t dma_channel;

    if (g_uart_hwstatus[uart_port] == UART_HWSTATUS_UNINITIALIZED) {
        return HAL_UART_STATUS_ERROR_UNINITIALIZED;
    }
    dma_channel = uart_port_to_dma_channel(uart_port, is_rx);
    status = vdma_set_sw_move_byte(dma_channel, sw_move_byte);
    if (status != VDMA_OK) {
        return HAL_UART_STATUS_ERROR_PARAMETER;
    }

    return HAL_UART_STATUS_OK;
}

uint32_t uart_get_hw_wptr(hal_uart_port_t uart_port, bool is_rx)
{
    vdma_channel_t dma_channel;
    uint32_t write_pointer;

    dma_channel = uart_port_to_dma_channel(uart_port, is_rx);
    vdma_get_hw_write_point(dma_channel, &write_pointer);
    if(is_rx) //if rx change buffer pointer
    {
        return (write_pointer - (uint32_t)(g_uart_dma_config[uart_port].receive_vfifo_buffer));
    }
    else
    {
        return (write_pointer - (uint32_t)(g_uart_dma_config[uart_port].send_vfifo_buffer));
    }
}
#if 0
bool uart_get_buf_full_status(hal_uart_port_t uart_port, bool is_rx)
{
    bool            ret = false;
    uint32_t        receive_bytes = 0, available_space = 0;
    vdma_channel_t  dma_channel;
    VDMA_REGISTER_T *dma_reg;

    dma_channel = uart_port_to_dma_channel(uart_port, is_rx);
    if (is_rx) {
        dma_reg = (VDMA_REGISTER_T * )g_vdma_regbase[((int) dma_channel/2)][is_rx];
        vdma_get_available_receive_bytes(dma_channel, &receive_bytes);
        if (receive_bytes == dma_reg->VDMA_FFCNT) {
            ret = true;
        } else {
            ret = false;
        }
    } else {
        vdma_get_available_send_space(dma_channel, &available_space);
        if (available_space == 0) {
            ret = true;
        } else {
            ret = false;
        }
    }
    return ret;
}
#else
bool uart_get_buf_full_status(hal_uart_port_t uart_port, bool is_rx)
{

    bool            ret = false;
    uint32_t        receive_bytes = 0, available_space = 0;
    vdma_channel_t  dma_channel;

    dma_channel = uart_port_to_dma_channel(uart_port, is_rx);
    if (is_rx) {
        vdma_get_available_receive_bytes(dma_channel, &receive_bytes);
        /* For RX, The amount of data present in vfifo is equal to the size of vfifo, indicating that the buffer is full*/
        if (receive_bytes == g_uart_dma_config[uart_port].receive_vfifo_buffer_size) {
            ret = true;
        } else {
            ret = false;
        }
    } else {
        vdma_get_available_send_space(dma_channel, &available_space);
        /* For TX, The space available for storing data to be transmitted is zero, indicating that the buffuer is full*/
        if (available_space == 0) {
            ret = true;
        } else {
            ret = false;
        }
    }

    return ret;
}
#endif
uint32_t uart_send_polling(hal_uart_port_t uart_port, const uint8_t *data, uint32_t size)
{
    uint32_t i;
    UART_REGISTER_T *uartx;

    uartx = g_uart_regbase[uart_port];

    for (i = 0; i < size; i++) {
        uart_put_char_block(uartx, *data);
        data++;
    }

    return size;
}

uint32_t uart_exception_send_polling(hal_uart_port_t uart_port, const uint8_t *data, uint32_t size)
{
    uint32_t i, j;
    uint32_t loop_count, remainder, curr_word;
    uint8_t *p_word;
    uint32_t *p_data;
    UART_REGISTER_T *uartx;

    uartx = g_uart_regbase[uart_port];

    loop_count = size / 4;
    remainder = size % 4;

    /* DSP IRAM must be fetched with WORD unit. */
    p_data = (uint32_t *)data;
    for (i=0; i<loop_count; i++) {
        curr_word = p_data[i];
        p_word = (uint8_t *)&curr_word;
        for (j = 0; j < 4; j++) {
            uart_put_char_block(uartx, p_word[j]);
        }
    }
    if (remainder) {
        curr_word = p_data[i];
        p_word = (uint8_t *)&curr_word;
        for (j = 0; j < remainder; j++) {
            uart_put_char_block(uartx, p_word[j]);
        }
    }

    return size;
}

void uart_wait_empty(hal_uart_port_t uart_port)
{
    UART_REGISTER_T *uartx;

    uartx = g_uart_regbase[uart_port];
    /* wait all left data sent out before deinit. */
    while (!((uartx->LSR) & UART_LSR_TEMT_MASK))
    {
        /*
            There is a risk that uart receive xoff when MCU is waitting UART fifo empty.
            Add break condition to avoid system hang.(such as disconnection of logging tool/other device ).
            But it is possible to bring about data lost when uart receive flow control signal under normal mode.
        */
        if ( g_uart_flowcontrol_status[uart_port] == UART_FLOWCONTROL_SOFTWARE ) {
            if( uartx->MCR_UNION.MCR & UART_MCR_XOFF_STATUS_MASK) {
                g_uart_not_wait_empty_status = true;
                break;
            }
        } else if (g_uart_flowcontrol_status[uart_port] == UART_FLOWCONTROL_HARDWARE ) {
            if( uartx->MCR_UNION.MCR & UART_MCR_RTS_MASK) {
                g_uart_not_wait_empty_status = true;
                break;
            }
        }
    }
}

hal_uart_status_t hal_uart_ext_get_uart_config(hal_uart_port_t uart_port, hal_uart_config_t  *uart_config, hal_uart_dma_config_t *dma_config, uint32_t *callback)
{
    if (!uart_port_is_valid(uart_port)) {
        return HAL_UART_STATUS_ERROR_PARAMETER;
    }

    if(g_uart_hwstatus[uart_port] == UART_HWSTATUS_UNINITIALIZED ) {
        return HAL_UART_STATUS_ERROR_UNINITIALIZED;
    }

#ifdef HAL_SLEEP_MANAGER_ENABLED
    *uart_config = g_uart_config[uart_port];
    *dma_config = g_uart_dma_config[uart_port];
    *callback = (uint32_t)g_uart_callback[uart_port].func;
#else
    UNUSED(uart_config);
    UNUSED(dma_config);
    UNUSED(callback);
#endif

    return HAL_UART_STATUS_OK;
}

bool uart_query_rx_empty(hal_uart_port_t uart_port)
{
    UART_REGISTER_T *uartx;
    uartx = g_uart_regbase[uart_port];
    if ((uartx->LSR) & UART_LSR_DR_MASK) {
        return false;
    } else {
        return true;
    }
}
bool uart_query_tx_empty(hal_uart_port_t uart_port)
{
    UART_REGISTER_T *uartx;
    uartx = g_uart_regbase[uart_port];
    if ((uartx->LSR) & UART_LSR_TEMT_MASK) {
        return true;
    } else {
        return false;
    }
}

<<<<<<< HEAD
#if 0 /* debug dump RG */
void uart_debug_dump_rg(hal_uart_port_t uart_port)
{
        /* dump uart register*/

        UART_REGISTER_T    *uartx = g_uart_regbase[uart_port];
        #ifndef LOGING_CANNOT_USE
        printf("uart%d [00]>:  xxxxxxxx  xxxxxxxx  %08x  xxxxxxxx\r\n",uart_port,uartx->DLM_DLL);
        printf("uart%d [10]>:  xxxxxxxx  %08x  %08x  %08x\r\n",uart_port,uartx->FCR_UNION.FCR, uartx->EFR_UNION.EFR, uartx->LCR_UNION.LCR);
        printf("uart%d [20]>:  %08x  %08x  %08x  %08x\r\n",uart_port,uartx->MCR_UNION.MCR, uartx->XON_XOFF_UNION.XON_XOFF,uartx->LSR,uartx->SCR);
        printf("uart%d [30]>:  %08x  %08x  %08x  %08x\r\n",uart_port,uartx->AUTOBAUD_CON_UNION.AUTOBAUD_CON, uartx->HIGHSPEED, uartx->SAMPLE_REG_UNION.SAMPLE_REG, uartx->AUTOBAUD_REG_UNION.AUTOBAUD_REG);
        printf("uart%d [40]>:  %08x  %08x  %08x  %08x\r\n",uart_port,uartx->RATEFIX_UNION.RATEFIX, uartx->GUARD, uartx->ESCAPE_REG_UNION.ESCAPE_REG,uartx->SLEEP_REG);
        printf("uart%d [50]>:  %08x  %08x  %08x  %08x\r\n",uart_port,uartx->DMA_CON_UNION.DMA_CON, uartx->RXTRIG, uartx->FRACDIV,uartx->RX_TO_CON_UNION.RX_TO_CON);
        printf("uart%d [60]>:  %08x  \r\n",uart_port,uartx->RX_TOC_DEST);
        #else
        debug_printf("uart%d [00]>:  xxxxxxxx  xxxxxxxx  %08x  xxxxxxxx\r\n",uart_port,uartx->DLM_DLL);
        debug_printf("uart%d [10]>:  xxxxxxxx  %08x  %08x  %08x\r\n",uart_port,uartx->FCR_UNION.FCR, uartx->EFR_UNION.EFR, uartx->LCR_UNION.LCR);
        debug_printf("uart%d [20]>:  %08x  %08x  %08x  %08x\r\n",uart_port,uartx->MCR_UNION.MCR, uartx->XON_XOFF_UNION.XON_XOFF,uartx->LSR,uartx->SCR);
        debug_printf("uart%d [30]>:  %08x  %08x  %08x  %08x\r\n",uart_port,uartx->AUTOBAUD_CON_UNION.AUTOBAUD_CON, uartx->HIGHSPEED, uartx->SAMPLE_REG_UNION.SAMPLE_REG, uartx->AUTOBAUD_REG_UNION.AUTOBAUD_REG);
        debug_printf("uart%d [40]>:  %08x  %08x  %08x  %08x\r\n",uart_port,uartx->RATEFIX_UNION.RATEFIX, uartx->GUARD, uartx->ESCAPE_REG_UNION.ESCAPE_REG,uartx->SLEEP_REG);
        debug_printf("uart%d [50]>:  %08x  %08x  %08x  %08x\r\n",uart_port,uartx->DMA_CON_UNION.DMA_CON, uartx->RXTRIG, uartx->FRACDIV,uartx->RX_TO_CON_UNION.RX_TO_CON);
        debug_printf("uart%d [60]>:  %08x  \r\n",uart_port,uartx->RX_TOC_DEST);
        #endif
        /*dump DMA register*/
        /*DMA mode enable*/
        uint32_t i;
        if( uartx->DMA_CON_UNION.DMA_CON & 0xFFFFFFFF ) {

            for ( i = 0; i < 2; i++) {
                VDMA_REGISTER_T *dmax = g_vdma_regbase[uart_port][i];
                #ifndef LOGING_CANNOT_USE
                printf("vdma%d [10]>:  %08x  %08x  %08x  %08x\r\n",i,dmax->VDMA_COUNT,  dmax->VDMA_CON_UNION.VDMA_CON, dmax->VDMA_START, dmax->VDMA_INTSTA);
                printf("vdma%d [20]>:  %08x  %08x  %08x  %08x\r\n",i,dmax->VDMA_ACKINT, dmax->DUMMY1_OFFSET, dmax->VDMA_LIMITER, dmax->VDMA_PGMADDR);
                printf("vdma%d [30]>:  %08x  %08x  %08x  %08x\r\n",i,dmax->VDMA_WRPTR,dmax->VDMA_RDPTR, dmax->VDMA_FFCNT, dmax->VDMA_FFSTA);
                printf("vdma%d [40]>:  %08x  %08x  xxxxxxxx  xxxxxxxx\r\n",i,dmax->VDMA_ALTLEN, dmax->VDMA_FFSIZE);
                printf("vdma%d [60]>:  %08x  %08x  %08x  %08x\r\n",i,dmax->VDMA_SW_MV_BYTE, dmax->VDMA_BNDRY_ADDR, dmax->VDMA_BYTE_TO_BNDRY, dmax->VDMA_BYTE_AVAIL);
                #else
                debug_printf("vdma%d [10]>:  %08x  %08x  %08x  %08x\r\n",i,dmax->VDMA_COUNT,  dmax->VDMA_CON_UNION.VDMA_CON, dmax->VDMA_START, dmax->VDMA_INTSTA);
                debug_printf("vdma%d [20]>:  %08x  %08x  %08x  %08x\r\n",i,dmax->VDMA_ACKINT, dmax->DUMMY1_OFFSET, dmax->VDMA_LIMITER, dmax->VDMA_PGMADDR);
                debug_printf("vdma%d [30]>:  %08x  %08x  %08x  %08x\r\n",i,dmax->VDMA_WRPTR,dmax->VDMA_RDPTR, dmax->VDMA_FFCNT, dmax->VDMA_FFSTA);
                debug_printf("vdma%d [40]>:  %08x  %08x  xxxxxxxx  xxxxxxxx\r\n",i,dmax->VDMA_ALTLEN, dmax->VDMA_FFSIZE);
                debug_printf("vdma%d [60]>:  %08x  %08x  %08x  %08x\r\n",i,dmax->VDMA_SW_MV_BYTE, dmax->VDMA_BNDRY_ADDR, dmax->VDMA_BYTE_TO_BNDRY, dmax->VDMA_BYTE_AVAIL);
                #endif
            }

        } else{

            #ifndef LOGING_CANNOT_USE
            printf("UART %d is not DMA mode\r\n",uart_port);
            #else
            debug_printf("UART %d is not DMA mode\r\n",uart_port);
            #endif
        }

        if( uart_port == g_uart_port_for_logging){
        /*print share buffer*/

            #ifndef LOGING_CANNOT_USE
            printf("========logging port is uart %d=======\r\n",uart_port);
            #else
            debug_printf("========logging port is uart %d=======\r\n",uart_port);
            #endif
        }

}
#endif /* debug dump RG */

=======
>>>>>>> db20e11 (second commit)
#ifdef __cplusplus
}
#endif

#endif

