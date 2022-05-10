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

#ifndef __MUX_H__
#define __MUX_H__


#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include "hal.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup kernel_service
 * @{
 * @addtogroup MUX
 * @{
 *
 * @brief This section introduces the MUX APIs including terms and acronyms, supported features, details on how to use MUX, function groups, enums, structures, and functions.
 *
 * MUX is a multiplexer for some communication devices, like: USB/UART/SPI slave/I2C slave etc.
 *
 * @section MUX_Terms_Chapter Terms and acronyms
 *
 * |        Terms         |           Details                              |
 * |----------------------|------------------------------------------------|
 * |\b      MUX          |        Multiplexer            |
 * |\b      Virtual \b Register          |        Interface for SPI and I2C slave            |
 * |\b      NVDM          |        Non-volatile Data Management            |
 * |\b      UART          |  Universal Asynchronous Receiver/Transmitter   |
 * |\b      USB           |           Universal Serial Bus                 |
 * @section MUX_Architecture Architecture
 * @image html mux_sw_architecture_overview.png
 * @section MUX_Features_Chapter Supported features
 * - \b Ports \b management
 *   - The MUX module manages a lot of device ports to do port assignment.
 * - \b Data \b communication
 *   - The MUX module use various device ports to send and receive data.
 * - \b Support \b multiple \b users \b on \b one \b MUX \b channel.
 *   - Some user with same data package format can MUX together to use one MUX port.
 * - \b High \b performance
 *   - Just one time memory copy when MUX send data.
 * - \b Less \b resources \b requirement
 *   - No need task to do MUX send and receive data.
 *   - Use read point and write point to manage the ring buffer.
 *     @image html mux_rx_tx_ring_buffer.png
 * @section MUX_HOWTO How to use MUX Module
 * - \b How \b to \b use \b MUX \b module
 *    - UART as sample:
 *      @code
 *           mux_protocol_t mux_uart1_op;
 *           mux_handle_t uart1_user1_handle,uart1_user2_handle;
 *           mux_port_setting_t setting;
 *           setting.tx_buffer_size = 1024;
 *           setting.rx_buffer_size = 1024;
 *           setting.dev_setting.uart.uart_config.baudrate = HAL_UART_BAUDRATE_921600;
 *           setting.dev_setting.uart.uart_config.word_length = HAL_UART_WORD_LENGTH_8;
 *           setting.dev_setting.uart.uart_config.stop_bit = HAL_UART_STOP_BIT_1;
 *           setting.dev_setting.uart.uart_config.parity = HAL_UART_PARITY_NONE;
 *           setting.dev_setting.uart.flowcontrol_type = MUX_UART_SW_FLOWCONTROL;
 *            mux_uart1_op.tx_protocol_callback = mux_uart1_package_protocol_cb_for_tx;
 *            mux_uart1_op.rx_protocol_callback = mux_uart1_unpackage_protocol_cb_for_rx;
 *
 *            status = mux_init(MUX_UART_1,&setting,&mux_uart1_op);
 *            if(MUX_STATUS_OK != status)
 *                assert(0);
 *            status = mux_open(MUX_UART_1,"uart1_user1",&uart1_user1_handle,uart1_mux_user1_cb,NULL);
 *            if(MUX_STATUS_OK != status)
 *                assert(0);
 *            printf("uart1 user1 open done~~~handle:0x%x\r\n",uart1_user1_handle);
 *
 *            status = mux_open(MUX_UART_1,"uart1_user2",&uart1_user2_handle,uart1_mux_user2_cb,NULL);
 *            if(MUX_STATUS_OK != status)
 *                assert(0);
 *            printf("uart1 user2 open done~~~handle:0x%x\r\n",uart1_user2_handle);
 *      @endcode
 *    - USB as sample:
 *      @code
 *            mux_protocol_t mux_usb2_op;
 *            mux_handle_t usb2_user1_handle,usb2_user2_handle;
 *            mux_port_setting_t setting;
 *            setting.tx_buffer_size = 1024;
 *            setting.rx_buffer_size = 1024;
 *            mux_usb2_op.tx_protocol_callback = NULL;
 *            mux_usb2_op.rx_protocol_callback = mux_usb2_unpackage;
 *
 *            status = mux_init(MUX_USB_COM_2,&setting,&mux_usb2_op);
 *            if(MUX_STATUS_OK != status)
 *             assert(0);
 *
 *            status = mux_open(MUX_USB_COM_2,"usb2_user1",&usb2_user1_handle,usb2_mux_user1_cb,NULL);
 *            if(MUX_STATUS_OK != status)
 *             assert(0);
 *            printf("USB2 user1 open done~~~handle:0x%x\r\n",usb2_user1_handle);
 *
 *            status = mux_open(MUX_USB_COM_2,"usb2_user2",&usb2_user2_handle,usb2_mux_user2_cb,NULL);
 *            if(MUX_STATUS_OK != status)
 *             assert(0);
 *            printf("USB2 user2 open done~~~handle:0x%x\r\n",usb2_user2_handle);
 *      @endcode
 *    - SPI slave as sample:
 *      @code
 *            mux_protocol_t mux_spi_slave_op;
 *            mux_handle_t spi_slave_user1_handle,spi_slave_user2_handle;
 *            mux_port_setting_t setting;
 *            setting.tx_buffer_size = 1024;
 *            setting.rx_buffer_size = 1024;
 *            setting.dev_setting.spi_slave.slave_config.bit_order = HAL_SPI_SLAVE_LSB_FIRST;
 *            setting.dev_setting.spi_slave.slave_config.phase = HAL_SPI_SLAVE_CLOCK_PHASE0;
 *            setting.dev_setting.spi_slave.slave_config.polarity = HAL_SPI_SLAVE_CLOCK_POLARITY0;
 *            setting.dev_setting.spi_slave.slave_config.timeout_threshold = 0xffffffff;
 *            //setting.dev_setting.spi_slave.early_miso = HAL_SPI_SLAVE_EARLY_MISO_DISABLE;
 *
 *            mux_spi_slave_op.tx_protocol_callback = NULL;
 *            mux_spi_slave_op.rx_protocol_callback = mux_spi_slave_unpackage;
 *
 *            status = mux_init(MUX_SPI_SLAVE_0,&setting,&mux_spi_slave_op);
 *            if(MUX_STATUS_OK != status)
 *                assert(0);
 *
 *            status = mux_open(MUX_SPI_SLAVE_0,"spi_slave_user1",&spi_slave_user1_handle,spi_slave_mux_user1_cb,NULL);
 *            if(MUX_STATUS_OK != status)
 *                assert(0);
 *            printf("spi_slave user1 open done~~~handle:0x%x\r\n",spi_slave_user1_handle);
 *
 *            status = mux_open(MUX_SPI_SLAVE_0,"spi_slave_user2",&spi_slave_user2_handle,spi_slave_mux_user2_cb,NULL);
 *            if(MUX_STATUS_OK != status)
 *                assert(0);
 *            printf("spi_slave user2 open done~~~handle:0x%x\r\n",spi_slave_user2_handle);
 *      @endcode
 *    - I2C slave as sample:
 *      @code
 *             #define DEMO_I2C_SLAVE_ADDRESS_CR_OR_CW 0x50
 *             #define DEMO_I2C_SLAVE_ADDRESS_R        0x54
 *             #define DEMO_I2C_SLAVE_ADDRESS_W        0x58
 *             mux_protocol_t mux_i2c_slave_op;
 *             mux_handle_t i2c_slave_user1_handle,i2c_slave_user2_handle;
 *             mux_port_setting_t setting;
 *             setting.tx_buffer_size = 1024;
 *             setting.rx_buffer_size = 1024;
 *             setting.dev_setting.i2c_slave.cr_or_cw_slave_address = DEMO_I2C_SLAVE_ADDRESS_CR_OR_CW;
 *             setting.dev_setting.i2c_slave.r_slave_address = DEMO_I2C_SLAVE_ADDRESS_R;
 *             setting.dev_setting.i2c_slave.w_slave_address = DEMO_I2C_SLAVE_ADDRESS_W;
 *             mux_i2c_slave_op.tx_protocol_callback = NULL;
 *             mux_i2c_slave_op.rx_protocol_callback = mux_i2c_slave_unpackage;
 *
 *             status = mux_init(MUX_I2C_SLAVE_0,&setting,&mux_i2c_slave_op);
 *             if(MUX_STATUS_OK != status)
 *                 assert(0);
 *
 *             status = mux_open(MUX_I2C_SLAVE_0,"i2c_slave_user1",&i2c_slave_user1_handle,i2c_slave_mux_user1_cb,NULL);
 *             if(MUX_STATUS_OK != status)
 *                 assert(0);
 *             printf("i2c_slave user1 open done~~~handle:0x%x\r\n",i2c_slave_user1_handle);
 *
 *             status = mux_open(MUX_I2C_SLAVE_0,"i2c_slave_user2",&i2c_slave_user2_handle,i2c_slave_mux_user2_cb,NULL);
 *             if(MUX_STATUS_OK != status)
 *                 assert(0);
 *             printf("i2c_slave user2 open done~~~handle:0x%x\r\n",i2c_slave_user2_handle);
 *      @endcode
 * - \b How \b to \b implement \b Rx \b and \b Tx \b protocol_callback
 *     - \b About \b tx_protocol_callback \n
 *          When the user owns a mux channel, means maybe no need add package header,
 *                   then the parameter of tx_protocol_callback should be NULL.\n
 *          When multiple users want to share a mux channel, means need add package header,
 *                   then the parameter of tx_protocol_callback should implement by user.\n
 *          For example:
 *      @code
 *                                       void mux_uart1_package_protocol_cb_for_tx(mux_handle_t handle, const mux_buffer_t payload[],uint32_t buffers_counter, mux_buffer_t *head, mux_buffer_t *tail,void *user_data)
 *                                       {
 *                                           //step1:Analysis the handle to identify which one user.
 *                                                  user_id = analysis_handle(handle);
 *                                          //step2: Calculate the payload size.
 *                                                  size = calculate_size(payload,buffers_counter);
 *                                          //step3:prepare the header buffer if need.
 *                                                  //prepare header buffer
 *                                                  //User must pay attention to the buffer copy length and make sure that it does not exceed TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN
 *                                                  head->p_buf[0] = 0x11;
 *                                                  head->p_buf[1] = 0x22;
 *                                                  //....
 *                                                  head->p_buf[N] = 0xnn;
 *                                                  //User must pay attention to N must be less than TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN
 *                                                  head->buf_size = N;
 *                                         //step4:prepare the tail buffer if need.
 *                                                  //User must pay attention to the buffer copy length and make sure that it does not exceed TX_PROTOCOL_CB_TAIL_BUFFER_MAX_LEN
 *                                                  tail->p_buf[0] = 0x11;
 *                                                  tail->p_buf[1] = 0x22;
 *                                                  //....
 *                                                  tail->p_buf[M] = 0xnn;
 *                                                  //User must pay attention to M must be less than TX_PROTOCOL_CB_TAIL_BUFFER_MAX_LEN
 *                                                  tail->buf_size = M;
 *                                      }
 *      @endcode
 *     - \b About \b rx_protocol_callback \n
 *          When the user owns a mux channel, means maybe no need add package tail,
 *                   then the parameter of rx_protocol_callback should be NULL.\n
 *          When multiple users want to share a mux channel, user decide whether need tail in the package or not.
 *                   if need, the parameter of tx_protocol_callback should implement by user.\n
 *
 *          For example:
 *      @code
 *                                       void mux_uart1_unpackage_protocol_cb_for_rx(mux_handle_t *handle, mux_buffer_t buffers[],uint32_t buffers_counter,
 *                                                      uint32_t *consume_len, uint32_t *package_len,void *user_data)
 *                                       {
 *                                          //step1:Analysis the handle to identify which one user.
 *                                                 user_id = analysis_handle(handle);
 *                                          //Step2:analysis data to find the abandon data in front, user should pay attention that the consume_len data will be abandoned.
 *                                              *consume_len = analysis_data_to_find_consume_len(buffers,buffers_counter);
 *                                          //Step3:analysis data to find first full package in front, if not find, please set *package_len to 0x0, means the data not enough, mux driver will call the callback again when receive more data.
 *                                              *package_len = analysis_data_to_find_consume_len(buffers,buffers_counter);
 *                                       }
 *      @endcode
 * - \b How \b to \b send \b data
 *     - Send one buffer one time:
 *      @code
 *           uint32_t return_value;
 *           mux_status_t status;
 *           mux_buffer_t temp_buff;
 *           temp_buff.p_buf  = "This is UART1 user1 Tx~~~\r\n";
 *           temp_buff.buf_size = strlen(temp_buff.p_buf);
 *           mux_tx(uart1_user1_handle,&temp_buff,1,&return_value);
 *      @endcode
 *     - Send multi buffers one time:
 *      @code
 *           uint32_t return_value;
 *           mux_status_t status;
 *           mux_buffer_t temp_buff[3];
 *           temp_buff[0].p_buf  = "This is UART1 user1 Tx~~~ one\r\n";
 *           temp_buff[0].buf_size = strlen(temp_buff[0].p_buf);
 *           temp_buff[1].p_buf  = "This is UART1 user1 Tx~~~ two\r\n";
 *           temp_buff[1].buf_size = strlen(temp_buff[1].p_buf);
 *           temp_buff[2].p_buf  = "This is UART1 user1 Tx~~~ three\r\n";
 *           temp_buff[2].buf_size = strlen(temp_buff[2].p_buf);
 *           mux_tx(uart1_user1_handle,&temp_buff,3,&return_value);
 *      @endcode
 * - \b How \b to \b receive \b data
 *    - UART as sample:
 *      @code
 *            void uart1_mux_user1_cb(mux_handle_t handle, mux_event_t event,uint32_t data_len,void*user_data)
 *            {
 *                if(MUX_EVENT_READY_TO_READ == event)
 *                {
 *                    printf("this is uart1 user1 MUX_EVENT_READY_TO_READ");
 *                    // User also can just send a message to task on here, and call mux_rx() to get data on task thread.
 *                    {
 *                        char *temp_buff[100];
 *                        mux_buffer_t buffer;
 *                        buffer.buf_size = 100;
 *                        buffer.p_buf = temp_buff;
 *                        uint32_t len;
 *                        mux_rx(uart1_user1_handle, &buffer,&len);
 *                        printf("vMuxTest_UART1_user1_Rx, get @@@@%d data\r\n",len);
 *                   }
 *
 *                }
 *                else if(MUX_EVENT_READY_TO_WRITE == event)
 *                    printf("this is uart1 user1 MUX_EVENT_READY_TO_WRITE");
 *            }
 *      @endcode
 * - \b How \b to \b get \b and \n save \b MUX \b port \b setting \b from \b NVDM
 *    - Take UART as sample:
 *      @code
 *            mux_protocol_t mux_uart1_op;
 *            mux_handle_t uart1_user1_handle,uart1_user2_handle;
 *            mux_port_setting_t setting;
 *            setting.tx_buffer_size = 1024;
 *            setting.rx_buffer_size = 1024;
 *            setting.dev_setting.uart.uart_config.baudrate = HAL_UART_BAUDRATE_921600;
 *            setting.dev_setting.uart.uart_config.word_length = HAL_UART_WORD_LENGTH_8;
 *            setting.dev_setting.uart.uart_config.stop_bit = HAL_UART_STOP_BIT_1;
 *            setting.dev_setting.uart.uart_config.parity = HAL_UART_PARITY_NONE;
 *            setting.dev_setting.uart.flowcontrol_type = MUX_UART_SW_FLOWCONTROL;
 *            mux_uart1_op.tx_protocol_callback = mux_uart1_package_protocol_cb_for_tx;
 *            mux_uart1_op.rx_protocol_callback = mux_uart1_unpackage_protocol_cb_for_rx;
 *
 *            status = mux_init(MUX_UART_1,&setting,&mux_uart1_op);
 *            if(MUX_STATUS_OK != status)
 *                assert(0);
 *
 *            status = mux_save_port_setting_to_nvdm(MUX_UART_1, &setting);
 *            if(MUX_STATUS_OK != status)
 *                assert(0);
 *
 *            status = mux_open(MUX_UART_1,"uart1_user1",&uart1_user1_handle,uart1_mux_user1_cb,NULL);
 *            if(MUX_STATUS_OK != status)
 *                assert(0);
 *
 *            status = mux_open_save_to_nvdm(MUX_UART_1, "uart1_user1");
 *            if(MUX_STATUS_OK != status)
 *                assert(0);
 *
 *            //the user had opened port
 *            mux_port_buffer_t buffer;
 *            //query which port that uart1_user1 had opened
 *            status = mux_query_port_numbers_from_nvdm("uart1_user1", &buffer);
 *            if (status != MUX_STATUS_OK)
 *                assert(0);
 *            //can get the porting setting when got uart1_user1 used port
 *            status = mux_query_port_setting_from_nvdm(buffer.buf[0], &setting);
 *            if (status != MUX_STATUS_OK)
 *                assert(0);
 *
 *            //delete user from NVDM
 *            status = mux_close_delete_from_nvdm(MUX_UART_1, "uart1_user1");
 *            if(MUX_STATUS_OK != status)
 *                return status;
 *      @endcode
 * @section MUX_MASTER_HOWTO Mux support multiple devices
 * - \b MUX \b support \b UART \b and \b USB \n
 *      UART and USB are basic modules for data communication. Those serial devices are easy to use by mux_rx and mux_rx to do data communication.\n
 * - \b MUX \b support \b SPI \b slave \n
 *      The architecture of MUX SPI slave as below:
 *      @image html mux_spi_slave_architecture.png
 *     The SPI master demo code as below:
 *      @code
 *            #ifdef ENABLE_ERROR_RETRY
 *                #define MAX_ERROR_NUMBER 1000
 *                 uint32_t g_spi_master_send_error_counter = 0;
 *                 uint32_t g_spi_master_receive_error_counter = 0;
 *            #endif
 *
 *            #define SPIS_CFG_RD_CMD         0x0a
 *            #define SPIS_RD_CMD             0x81
 *            #define SPIS_CFG_WR_CMD         0x0c
 *            #define SPIS_WR_CMD             0x0e
 *            #define SPIS_RS_CMD             0x06
 *            #define SPIS_WS_CMD             0x08
 *            #define SPIS_PWON_CMD           0x04
 *            #define SPIS_PWOFF_CMD          0x02
 *            #define SPIS_CT_CMD             0x10
 *
 *            #define VG_SPI_SLAVE_CMD_LEN 9 // The first word[31:16]: for cmd. The first word[15:0]::for reg index  The second word: for register length.
 *            //matser<--> slave CR or CW data format:
 *            //         Word 0:
 *            //             [31:16]- VG_SPI_SLAVE_CR_CMD or VG_SPI_SLAVE_CW_CMD
 *            //             [15:0]-   VG_SPI_SLAVE_xxx_xxx_REG_OFFSET
 *            //         Word:
 *            //             [31:0] Register len.
 *
 *            #define VG_SPI_SLAVE_TX_LEN_REG_OFFSET 0x08
 *            #define VG_SPI_SLAVE_TX_BUF_REG_OFFSET 0x2000
 *
 *            #define VG_SPI_SLAVE_RX_LEN_REG_OFFSET 0x04
 *            #define VG_SPI_SLAVE_RX_BUF_REG_OFFSET 0x1000
 *
 *             ATTR_RWDATA_IN_NONCACHED_SYSRAM_4BYTE_ALIGN uint8_t send_data_buf[50+1]; //the data that used to send
 *             ATTR_RWDATA_IN_NONCACHED_SYSRAM_4BYTE_ALIGN uint8_t receive_data_buf[50+1] = {0};
 *
 *             volatile int32_t send_irq_err_flag = 0;
 *             volatile int32_t receive_irq_err_flag = 0;
 *
 *             static SemaphoreHandle_t g_spi_master_mutex = NULL;
 *             static SemaphoreHandle_t wait_send_done_xSemaphore = NULL;
 *             static SemaphoreHandle_t wait_receive_done_xSemaphore = NULL;
 *
 *             void spi_master_mutex_take(void)
 *             {
 *                 printf("spi_master_mutex_take\r\n");
 *                 if (xSemaphoreTake(g_spi_master_mutex, portMAX_DELAY) == pdFALSE) {
 *                     printf("spi_master_mutex_take error\r\n", 0);
 *                     return;
 *                 }
 *             }
 *             void spi_master_mutex_give(void)
 *             {
 *                 printf("spi_master_mutex_give\r\n");
 *                 if (xSemaphoreGive(g_spi_master_mutex) == pdFALSE) {
 *                     LOG_MSGID_E(common, "spi_master_mutex_give error\r\n", 0);
 *                     return;
 *                 }
 *             }
 *             static void spi_query_slave_status(uint8_t *status, hal_spi_master_port_t master_port)
 *             {
 *                 uint8_t status_cmd = SPIS_RS_CMD;
 *                 uint8_t status_receive[2];
 *                 hal_spi_master_send_and_receive_config_t spi_send_and_receive_config;
 *
 *            //      Note:
 *            //      * The value of receive_length is the valid number of bytes received plus the number of bytes to send.
 *            //      * For example, here the valid number of bytes received is 1 byte,
 *            //      * and the number of bytes to send also is 1 byte, so the receive_length is 2.
 *
 *                 status_receive[1] = 0;
 *                 spi_send_and_receive_config.receive_length = 2;
 *                 spi_send_and_receive_config.send_length = 1;
 *                 spi_send_and_receive_config.send_data = &status_cmd;
 *                 spi_send_and_receive_config.receive_buffer = status_receive;
 *                 if (HAL_SPI_MASTER_STATUS_OK != hal_spi_master_send_and_receive_polling(master_port, &spi_send_and_receive_config)) {
 *                     printf("SPI master query status of slave failed\r\n");
 *                     return;
 *                 }
 *
 *                 printf("  Status receive: 0x%x\r\n", status_receive[1]);
 *                 *status = status_receive[1];
 *             }
 *
 *
 *             void mux_spi_master_demo_init()
 *             {
 *                 demo_uart_echo_init();
 *
 *                 g_spi_master_mutex = xSemaphoreCreateMutex();
 *                 if (g_spi_master_mutex == NULL) {
 *                     printf( "g_spi_master_mutex create error\r\n");
 *                     return;
 *                 }
 *                 wait_send_done_xSemaphore = xSemaphoreCreateBinary();
 *                 if (wait_send_done_xSemaphore == NULL) {
 *                     printf( "wait_send_done_xSemaphore create error\r\n");
 *                     return;
 *                 }
 *                 wait_receive_done_xSemaphore = xSemaphoreCreateBinary();
 *                 if (wait_receive_done_xSemaphore == NULL) {
 *                     printf( "wait_receive_done_xSemaphore create error\r\n");
 *                     return;
 *                 }
 *
 *                 hal_gpio_init(HAL_GPIO_12); //init the gpio pin
 *                 hal_gpio_init(HAL_GPIO_13); //init the gpio pin
 *                 hal_gpio_init(HAL_GPIO_14); //init the gpio pin
 *                 hal_gpio_init(HAL_GPIO_15); //init the gpio pin
 *                 hal_gpio_init(HAL_GPIO_16); //init the gpio pin
 *                 hal_gpio_init(HAL_GPIO_17); //init the gpio pin
 *
 *                 hal_pinmux_set_function(HAL_GPIO_12, HAL_GPIO_12_SPI_MST0_MOSI);//Set the pinmux
 *                 hal_pinmux_set_function(HAL_GPIO_13, HAL_GPIO_13_SPI_MST0_MISO);//Set the pinmux
 *                 hal_pinmux_set_function(HAL_GPIO_14, HAL_GPIO_14_SPI_MST0_SIO2);//Set the pinmux
 *                 hal_pinmux_set_function(HAL_GPIO_15, HAL_GPIO_15_SPI_MST0_SIO3);//Set the pinmux
 *                 hal_pinmux_set_function(HAL_GPIO_16, HAL_GPIO_16_SPI_MST0_SCK);//Set the pinmux
 *                 hal_pinmux_set_function(HAL_GPIO_17, HAL_GPIO_17_SPI_MST0_CS0);//Set the pinmux
 *
 *                 hal_gpio_set_driving_current(HAL_GPIO_12, HAL_GPIO_DRIVING_CURRENT_4MA);
 *                 hal_gpio_set_driving_current(HAL_GPIO_13, HAL_GPIO_DRIVING_CURRENT_4MA);
 *                 hal_gpio_set_driving_current(HAL_GPIO_14, HAL_GPIO_DRIVING_CURRENT_4MA);
 *                 hal_gpio_set_driving_current(HAL_GPIO_15, HAL_GPIO_DRIVING_CURRENT_4MA);
 *                 hal_gpio_set_driving_current(HAL_GPIO_16, HAL_GPIO_DRIVING_CURRENT_4MA);
 *                 hal_gpio_set_driving_current(HAL_GPIO_17, HAL_GPIO_DRIVING_CURRENT_4MA);
 *
 *                   {
 *                         hal_spi_master_config_t spi_master_config;
 *                         spi_master_config.bit_order = HAL_SPI_MASTER_LSB_FIRST;
 *                         spi_master_config.clock_frequency = 1000000;
 *                         spi_master_config.phase = HAL_SPI_MASTER_CLOCK_PHASE0;
 *                         spi_master_config.polarity = HAL_SPI_MASTER_CLOCK_POLARITY0;
 *                         spi_master_config.slave_port = HAL_SPI_MASTER_SLAVE_0;
 *                         if (HAL_SPI_MASTER_STATUS_OK != hal_spi_master_init(HAL_SPI_MASTER_0, &spi_master_config)) {
 *                             printf( "mux_spi_master_demo_init hal_spi_master_init error!!!\r\n");
 *                                 return ;
 *                        }
 *                    {
 *                         uint8_t poweron_cmd;
 *                         poweron_cmd = SPIS_PWON_CMD;
 *                         if (HAL_SPI_MASTER_STATUS_OK != hal_spi_master_send_polling(HAL_SPI_MASTER_0, &poweron_cmd, 1)) {
 *                         printf("mux_spi_master_demo_init ERROR. hal_spi_master_send_polling fail!!!\r\n");
 *                         return ;
 *                         }
 *                         while(1){
 *                         uint8_t status_recv, check_status;
 *                         spi_query_slave_status(&status_recv,HAL_SPI_MASTER_0);
 *                         if((SPIS_STA_SLV_ON_MASK & status_recv)==SPIS_STA_SLV_ON_MASK)
 *                         break;
 *                         }
 *                     }
 *                 }
 *             }
 *
 *             void spi_user_send_callback(hal_spi_master_callback_event_t event, void *user_data)
 *             {
 *                 BaseType_t xHigherPriorityTaskWoken = pdFALSE;
 *                 uint32_t send_done_len = (uint32_t)user_data;
 *                 if(event == HAL_SPI_MASTER_EVENT_SEND_FINISHED){
 *                     send_irq_err_flag= 1;
 *                     printf("Send done, size: %d\r\n",strlen(send_data_buf));
 *                 }
 *                 else{
 *                     send_irq_err_flag = -1;
 *                     printf("spi_user_send_callback ERROR, err event:%d\r\n",event);
 *                     assert(0);
 *                 }
 *                 xSemaphoreGiveFromISR( wait_send_done_xSemaphore, &xHigherPriorityTaskWoken );
 *             }
 *
 *             void spi_user_receive_callback(hal_spi_master_callback_event_t event, void *user_data)
 *             {
 *                 BaseType_t xHigherPriorityTaskWoken = pdFALSE;
 *                 uint32_t receive_done_len = (uint32_t)user_data;
 *                 if(event == HAL_SPI_MASTER_EVENT_RECEIVE_FINISHED){
 *                     receive_irq_err_flag = 1;
 *                  }
 *                 else{
 *                     send_irq_err_flag = -1;
 *                     printf("spi_user_receive_callback ERROR, err event:%d\r\n",event);
 *                     assert(0);
 *                 }
 *                 xSemaphoreGiveFromISR( wait_receive_done_xSemaphore, &xHigherPriorityTaskWoken );
 *             }
 *
 *
 *             void wait_for_send_done()
 *             {
 *                if(xSemaphoreTake( wait_send_done_xSemaphore, portMAX_DELAY) == pdTRUE){
 *                }
 *                else{
 *                    assert(0);
 *                }
 *             }
 *             void wait_for_receive_done()
 *             {
 *                if(xSemaphoreTake( wait_receive_done_xSemaphore, portMAX_DELAY) == pdTRUE){
 *                }
 *                else{
 *                    assert(0);
 *                }
 *             }
 *             ATTR_RWDATA_IN_NONCACHED_SYSRAM_4BYTE_ALIGN uint8_t g_send_buf[1];
 *             ATTR_RWDATA_IN_NONCACHED_SYSRAM_4BYTE_ALIGN uint8_t g_receive_buf[4+1];
 *            //
 *            //     This is Master read data from slave:
 *            //     Step0: Init SPI master
 *            //     Step1: Master read Slave the register of <VG_SPI_SLAVE_TX_LEN_REG_OFFSET> with a length of 4 bytes.
 *            //          Step1_a: Master sends the Config Read (CR) cmd to slave:
 *            //                     Master sends to slave the 9-byte command: [0]:SPIS_CFG_RD_CMD, [1:4]VG_SPI_SLAVE_TX_LEN_REG_OFFSET,[5:8] len-1
 *            //          Step1_b: Wait for SPI slave CR to be ready.
 *            //          Step1_c: Master receives the value of <VG_SPI_SLAVE_TX_LEN_REG_OFFSET>
 *            //               Master receives 4 bytes, it's the data len which Slave prepared.
 *            //     Step2: Master reads Slave the register of <VG_SPI_SLAVE_TX_BUF_REG_OFFSET> with the len of data_buffer_len.
 *            //             Step2_a: Master sends the Config Read (CR) cmd to slave:
 *            //                Master sends to slave the 9-byte command: [0]:SPIS_CFG_RD_CMD, [1:4]VG_SPI_SLAVE_TX_BUF_REG_OFFSET,[5:8] len-1
 *            //             Step2_b: Wait for SPI slave CR to be ready.
 *            //             Step2_c: Master receives <data_buffer_len> data.
 *            //                     Master receives <data_buffer_len> bytes, it's the data which Slave prepared
 *            //     Step3: Deinit SPI master
 *
 *             uint32_t mux_spi_master_demo_receive(uint8_t *buf,uint32_t len)
 *             {
 *                    hal_spi_master_config_t spi_master_init_config;
 *                    hal_spi_master_send_and_receive_config_t spi_send_and_receive_config;
 *                    hal_spi_master_status_t status;
 *                    uint8_t request_cmd[9];
 *                    uint32_t receive_reg_value;
 *                    uint32_t length,offset;
 *            #ifdef ENABLE_ERROR_RETRY
 *             mux_spi_master_demo_receive_restart:
 *            #endif
 *                    spi_master_mutex_take();
 *            //     Step1: Master reads Slave the register of <VG_SPI_SLAVE_TX_LEN_REG_OFFSET> with a length of 4 bytes.
 *            //         Step1_a: Master sends the Config Read (CR) cmd to slave:
 *             //                    Master sends to slave the 9-byte command: [0]:SPIS_CFG_RD_CMD, [1:4]VG_SPI_SLAVE_TX_LEN_REG_OFFSET,[5:8] len-1
 *            #ifdef ENABLE_ERROR_RETRY
 *                 g_spi_master_receive_error_counter = 0;
 *            #endif
 *                     length = 4;
 *                     offset = VG_SPI_SLAVE_TX_LEN_REG_OFFSET;
 *                     request_cmd[0] = SPIS_CFG_RD_CMD;
 *                     request_cmd[1] = offset & 0xff;
 *                     request_cmd[2] = (offset >> 8) & 0xff;
 *                     request_cmd[3] = (offset >> 16) & 0xff;
 *                     request_cmd[4] = (offset >> 24) & 0xff;
 *                     request_cmd[5] = (length - 1) & 0xff;
 *                     request_cmd[6] = ((length - 1) >> 8) & 0xff;
 *                     request_cmd[7] = ((length - 1) >> 16) & 0xff;
 *                     request_cmd[8] = ((length - 1) >> 24) & 0xff;
 *                   while(1){
 *                     printf("mux_spi_master_demo_receive-Step1_a: Send VG_SPI_SLAVE_CRD_CMD.\r\n");
 *                     printf("mux_spi_master_demo_receive-Step1_a: Master wants to send 9B\r\n");
 *                     status = hal_spi_master_send_polling(HAL_SPI_MASTER_0,request_cmd,VG_SPI_SLAVE_CMD_LEN);
 *                     if( HAL_SPI_MASTER_STATUS_OK == status){
 *                         printf("mux_spi_master_demo_receive-Step1_a: Send VG_SPI_SLAVE_CRD_CMD success.\r\n");
 *                         break;
 *                     }
 *                      printf("mux_spi_master_demo_receive-Step1_a:: SPI master send error, status:%d \r\n",status);
 *                      assert(0);
 *            #ifdef ENABLE_ERROR_RETRY
 *                         g_spi_master_receive_error_counter++;
 *                         if(g_spi_master_receive_error_counter>MAX_ERROR_NUMBER){
 *                             printf("mux_spi_master_demo_receive-Step1_a: #### too many ERROR counter %d, now go to restart!!!!!\r\n",g_spi_master_receive_error_counter);
 *                             spi_master_mutex_give();
 *                            goto mux_spi_master_demo_receive_restart;
 *                         }
 *            #endif
 *                   }
 *             //       Step1_b: Wait for SPI slave CR to be ready.
 *            #ifdef ENABLE_ERROR_RETRY
 *                 g_spi_master_receive_error_counter = 0;
 *            #endif
 *                     printf("mux_spi_master_demo_receive-Step1_b: wait slave CR done...\r\n");
 *                     while(1){
 *                         uint8_t status_recv, check_status;
 *                         spi_query_slave_status(&status_recv,HAL_SPI_MASTER_0);
 *                         if((SPIS_STA_TXRX_FIFO_RDY_MASK & status_recv)==SPIS_STA_TXRX_FIFO_RDY_MASK)
 *                             break;
 *            #ifdef ENABLE_ERROR_RETRY
 *                         g_spi_master_receive_error_counter++;
 *                         if(g_spi_master_receive_error_counter>MAX_ERROR_NUMBER){
 *                             printf("mux_spi_master_demo_receive-Step1_b: #### too many ERROR counter %d, now go to restart!!!!!\r\n",g_spi_master_receive_error_counter);
 *                             spi_master_mutex_give();
 *                            goto mux_spi_master_demo_receive_restart;
 *                         }
 *            #endif
 *                     }
 *            //         Step1_c: Master receives the value of <VG_SPI_SLAVE_TX_LEN_REG_OFFSET>
 *            //                Master receives 4 bytes. It is the data length which Slave prepared.
 *            #ifdef ENABLE_ERROR_RETRY
 *                    g_spi_master_receive_error_counter = 0;
 *            #endif
 *                    while(1){
 *                         printf("mux_spi_master_demo_receive-Step1_c: Receive SPI slave Tx_len Reg value. \r\n");
 *                         printf("mux_spi_master_demo_receive-Step1_c: Master want to receive 4B.\r\n");
 *                         {
 *                             g_send_buf[0] = SPIS_RD_CMD;
 *                             spi_send_and_receive_config.receive_length = 4+1;
 *                             spi_send_and_receive_config.send_length = 1;
 *                             spi_send_and_receive_config.send_data = g_send_buf;
 *                             spi_send_and_receive_config.receive_buffer = g_receive_buf;
 *                             status = hal_spi_master_send_and_receive_polling(HAL_SPI_MASTER_0,&spi_send_and_receive_config);
 *                             if( HAL_SPI_MASTER_STATUS_OK == status){
 *                                 receive_reg_value = 0;
 *                                 receive_reg_value = g_receive_buf[1] | (g_receive_buf[2]<<8) | (g_receive_buf[3]<<16)|(g_receive_buf[3]<<24);
 *                                 printf("mux_spi_master_demo_receive-Step1_c: Receive SPI slave Tx_len Reg value:0x%x. success!!!\r\n",receive_reg_value);
 *                                 break;
 *                             }
 *                             printf("mux_spi_master_demo_receive-Step1_c : SPI master receive err,status:%d\r\n",status);
 *                             assert(0);
 *            #ifdef ENABLE_ERROR_RETRY
 *                         g_spi_master_receive_error_counter++;
 *                         if(g_spi_master_receive_error_counter>MAX_ERROR_NUMBER){
 *                             printf("mux_spi_master_demo_receive-Step1_c: #### too many ERROR counter %d, now go to restart!!!!!\r\n",g_spi_master_receive_error_counter);
 *                             spi_master_mutex_give();
 *                            goto mux_spi_master_demo_receive_restart;
 *                         }
 *            #endif
 *                         }
 *                    }
 *                    if(receive_reg_value > len){
 *                         receive_reg_value = len;
 *                         printf("mux_spi_master_demo_receive-Step1_c: slave data len too big, request %d B firstly;\r\n",len);
 *                    }
 *                    if(receive_reg_value == 0){//The received length should not be 0.
 *                         spi_master_mutex_give();
 *                         return 0;
 *                    }
 *                    spi_master_mutex_give();
 *                 spi_master_mutex_take();
 *            //     Step2: Master read Slave the register of <VG_SPI_SLAVE_TX_BUF_REG_OFFSET> with the len of data_buffer_len.
 *            //             Step2_a: Master sends the Config Read (CR) cmd to slave:
 *            //                Master sends to slave the 9-byte command: [0]:SPIS_CFG_RD_CMD, [1:4]VG_SPI_SLAVE_TX_BUF_REG_OFFSET,[5:8] len-1
 *            #ifdef ENABLE_ERROR_RETRY
 *                    g_spi_master_receive_error_counter = 0;
 *            #endif
 *                     length = receive_reg_value;
 *                     offset = VG_SPI_SLAVE_TX_BUF_REG_OFFSET;
 *                     request_cmd[0] = SPIS_CFG_RD_CMD;
 *                     request_cmd[1] = offset & 0xff;
 *                     request_cmd[2] = (offset >> 8) & 0xff;
 *                     request_cmd[3] = (offset >> 16) & 0xff;
 *                     request_cmd[4] = (offset >> 24) & 0xff;
 *                     request_cmd[5] = (length - 1) & 0xff;
 *                     request_cmd[6] = ((length - 1) >> 8) & 0xff;
 *                     request_cmd[7] = ((length - 1) >> 16) & 0xff;
 *                     request_cmd[8] = ((length - 1) >> 24) & 0xff;
 *                    while(1){
 *                        printf("mux_spi_master_demo_receive-Step2_a: send VG_SPI_SLAVE_RD_CMD.\r\n");
 *                        printf("mux_spi_master_demo_receive-Step2_a: Master want to send 9B cmd.\r\n");
 *                        status = hal_spi_master_send_polling(HAL_SPI_MASTER_0,request_cmd,VG_SPI_SLAVE_CMD_LEN);
 *                        if( HAL_SPI_MASTER_STATUS_OK == status){
 *                          printf("mux_spi_master_demo_receive-Step2_a: Send VG_SPI_SLAVE_RD_CMD. success!!!\r\n");
 *                          break;
 *                        }
 *                        printf("mux_spi_master_demo_receive-Step2_a: SPI master send err, try again...status:%d \r\n",status);
 *                        while(1);
 *                        assert(0);
 *            #ifdef ENABLE_ERROR_RETRY
 *                         g_spi_master_receive_error_counter++;
 *                         if(g_spi_master_receive_error_counter>MAX_ERROR_NUMBER){
 *                             printf("mux_spi_master_demo_receive-Step2_a: #### too many ERROR counter %d, now go to restart!!!!!\r\n",g_spi_master_receive_error_counter);
 *                             spi_master_mutex_give();
 *                            goto mux_spi_master_demo_receive_restart;
 *                         }
 *            #endif
 *                    }
 *            //       Step2_b: Wait for SPI slave CR to be ready
 *            #ifdef ENABLE_ERROR_RETRY
 *                    g_spi_master_receive_error_counter = 0;
 *            #endif
 *                     printf("mux_spi_master_demo_receive-Step2_b: wait slave CR done...\r\n");
 *                     while(1){
 *                         uint8_t status_recv, check_status;
 *                         spi_query_slave_status(&status_recv,HAL_SPI_MASTER_0);
 *                         if((SPIS_STA_TXRX_FIFO_RDY_MASK & status_recv)==SPIS_STA_TXRX_FIFO_RDY_MASK)
 *                             break;
 *            #ifdef ENABLE_ERROR_RETRY
 *                         g_spi_master_receive_error_counter++;
 *                         if(g_spi_master_receive_error_counter>MAX_ERROR_NUMBER){
 *                             printf("mux_spi_master_demo_receive-Step2_b: #### too many ERROR counter %d, now go to restart!!!!!\r\n",g_spi_master_receive_error_counter);
 *                             spi_master_mutex_give();
 *                            goto mux_spi_master_demo_receive_restart;
 *                         }
 *            #endif
 *                     }
 *            //         Step2_c: Master receives <data_buffer_len> data.
 *            //                     Master receives <data_buffer_len> bytes. It is the data which Slave prepared.
 *            #ifdef ENABLE_ERROR_RETRY
 *                    g_spi_master_receive_error_counter = 0;
 *            #endif
 *                    hal_spi_master_register_callback(HAL_SPI_MASTER_0, spi_user_receive_callback, receive_reg_value);//Register a user callback.
 *                     g_send_buf[0] = SPIS_RD_CMD;
 *                     spi_send_and_receive_config.receive_length = receive_reg_value+1;
 *                     spi_send_and_receive_config.send_length = 1;
 *                     spi_send_and_receive_config.send_data = g_send_buf;
 *                     spi_send_and_receive_config.receive_buffer = buf;
 *                     printf("mux_spi_master_demo_receive-Step2_c: Master want to receive:%d\r\n",receive_reg_value);
 *                    while(1){
 *                        receive_irq_err_flag = 0;
 *                        printf("mux_spi_master_demo_receive-Step2_c: receive Tx_buff data.  \r\n");
 *                        status = hal_spi_master_send_and_receive_dma(HAL_SPI_MASTER_0, &spi_send_and_receive_config);
 *                        if (HAL_SPI_MASTER_STATUS_OK == status) {
 *                             wait_for_receive_done();
 *                             if(receive_irq_err_flag == 1){
 *                                 printf("mux_spi_master_demo_receive-Step2_c: Receive Tx_buff data. success\r\n");
 *                                 break;
 *                             }
 *                        }
 *                        else{
 *                          printf("mux_spi_master_demo_receive-Step2_c: hal_spi_master_receive_dma_ex return ERROR!!! status:%d\r\n",status);
 *                             while(1);
 *                            assert(0);
 *                        }
 *                        printf("mux_spi_master_demo_receive-Step2_c: SPI master receive err, \r\n");
 *                        while(1);
 *                        assert(0);
 *            #ifdef ENABLE_ERROR_RETRY
 *                         g_spi_master_receive_error_counter++;
 *                         if(g_spi_master_receive_error_counter>MAX_ERROR_NUMBER){
 *                             printf("mux_spi_master_demo_receive-Step2_c: #### too many ERROR counter %d, now go to restart!!!!!\r\n",g_spi_master_receive_error_counter);
 *                             spi_master_mutex_give();
 *                            goto mux_spi_master_demo_receive_restart;
 *                         }
 *            #endif
 *                  }
 *                  spi_master_mutex_give();
 *                  return receive_reg_value;
 *             }
 *             extern void vPortFreeNC( void *pv );
 *             extern void *pvPortMallocNC( size_t xWantedSize );
 *            //     This is the data Master writes to slave:
 *            //     Step0: Init SPI master
 *            //     Step1: Master reads Slave the register of <VG_SPI_SLAVE_RX_LEN_REG_OFFSET> with a length of 4 bytes.
 *            //             Step1_a: Master sends the Config Read (CR) cmd to slave:
 *            //                 Master sends to slave the 9-byte command: [0]:SPIS_CFG_RD_CMD, [1:4]VG_SPI_SLAVE_RX_LEN_REG_OFFSET,[5:8] len-1
 *            //             Step1_b: Wait for SPI slave CR to be ready
 *            //             Step1_c: Master receives the value of <VG_SPI_SLAVE_RX_LEN_REG_OFFSET>
 *            //                      Master receives 4 bytes. It is the data length which Slave prepared.
 *            //
 *            //     Step2: Master read Slave the register of <VG_SPI_SLAVE_RX_BUF_REG_OFFSET> with the len of data_buffer_len which master wants to write.
 *            //             Step2_a: Master sends the Config Write (CW) cmd to slave:
 *            //                 Master sends to slave the 9-byte command: [0]:SPIS_CFG_RD_CMD, [1:4]VG_SPI_SLAVE_TX_BUF_REG_OFFSET,[5:8] len-1
 *            //             Step2_b: Wait for SPI slave CR to be ready
 *            //             Step2_c: Master sends the <data_buffer_len> data.
 *            //                 Slave address is DEMO_SPI_SLAVE_ADDRESS_W
 *            //                 Master sends <data_buffer_len> bytes. It is the data that master wants to write
 *            //     Step3: Deinit SPI master
 *
 *             uint32_t mux_spi_master_demo_send(uint8_t *buf,uint32_t len)
 *             {
 *                    hal_spi_master_config_t spi_master_init_config;
 *                    hal_spi_master_send_and_receive_config_t spi_send_and_receive_config;
 *                    hal_spi_master_status_t status;
 *                    uint8_t request_cmd[9];
 *                    uint32_t receive_reg_value;
 *                    uint32_t length,offset;
 *                    uint8_t* p_temp_malloc_buff;
 *
 *            #ifdef ENABLE_ERROR_RETRY
 *                    mux_spi_master_demo_send_restart:
 *            #endif
 *                 spi_master_mutex_take();
 *            //     Step1:Master read Slave the register of <VG_SPI_SLAVE_RX_LEN_REG_OFFSET> with a length of 4 bytes.
 *            //             Step1_a: Master sends the Config Read (CR) cmd to slave:
 *            //                 Master sends the 9-byte command to slave: [0]:SPIS_CFG_RD_CMD, [1:4]VG_SPI_SLAVE_RX_LEN_REG_OFFSET,[5:8] len-1
 *            #ifdef ENABLE_ERROR_RETRY
 *                     g_spi_master_send_error_counter = 0;
 *            #endif
 *                     length = 4;
 *                     offset = VG_SPI_SLAVE_RX_LEN_REG_OFFSET;
 *                     request_cmd[0] = SPIS_CFG_RD_CMD;
 *                     request_cmd[1] = offset & 0xff;
 *                     request_cmd[2] = (offset >> 8) & 0xff;
 *                     request_cmd[3] = (offset >> 16) & 0xff;
 *                     request_cmd[4] = (offset >> 24) & 0xff;
 *                     request_cmd[5] = (length - 1) & 0xff;
 *                     request_cmd[6] = ((length - 1) >> 8) & 0xff;
 *                     request_cmd[7] = ((length - 1) >> 16) & 0xff;
 *                     request_cmd[8] = ((length - 1) >> 24) & 0xff;
 *                     while(1){
 *                         printf("mux_spi_master_demo_send-Step1_a: Send VG_SPI_SLAVE_CRD_CMD.\r\n");
 *                         printf("mux_spi_master_demo_send-Step1_a: Master want to send 9B cmd.\r\n");
 *                         status = hal_spi_master_send_polling(HAL_SPI_MASTER_0,request_cmd,VG_SPI_SLAVE_CMD_LEN);
 *                     if( HAL_SPI_MASTER_STATUS_OK == status){
 *                         printf("mux_spi_master_demo_send-Step1_a: Send VG_SPI_SLAVE_CRD_CMD success!!!\r\n");
 *                         break;
 *                     }
 *                         printf("mux_spi_master_demo_send-Step1_a: SPI master send err, try again...status:%d \r\n",status);
 *                         assert(0);
 *            #ifdef ENABLE_ERROR_RETRY
 *                       g_spi_master_send_error_counter++;
 *                       if(g_spi_master_send_error_counter>MAX_ERROR_NUMBER){
 *                           printf("mux_spi_master_demo_send-Step1_a: #### too many ERROR counter %d, now go to restart!!!!!\r\n",g_spi_master_send_error_counter);
 *                           spi_master_mutex_give();
 *                          goto mux_spi_master_demo_send_restart;
 *                       }
 *            #endif
 *                     }
 *             //     Step1_b: Wait for SPI slave CR to be ready
 *            #ifdef ENABLE_ERROR_RETRY
 *                     g_spi_master_send_error_counter = 0;
 *            #endif
 *                         printf("mux_spi_master_demo_send-Step1_b: wait slave CR done...\r\n");
 *                         while(1){
 *                             uint8_t status_recv, check_status;
 *                             spi_query_slave_status(&status_recv,HAL_SPI_MASTER_0);
 *                             if((SPIS_STA_TXRX_FIFO_RDY_MASK & status_recv)==SPIS_STA_TXRX_FIFO_RDY_MASK)
 *                                 break;
 *            #ifdef ENABLE_ERROR_RETRY
 *                       g_spi_master_send_error_counter++;
 *                       if(g_spi_master_send_error_counter>MAX_ERROR_NUMBER){
 *                           printf("mux_spi_master_demo_send-Step1_b: #### too many ERROR counter %d, now go to restart!!!!!\r\n",g_spi_master_send_error_counter);
 *                           spi_master_mutex_give();
 *                          goto mux_spi_master_demo_send_restart;
 *                       }
 *            #endif
 *                         }
 *
 *            //    Step1_c: Master receives the value of <VG_SPI_SLAVE_RX_LEN_REG_OFFSET>
 *            //                      Master receives 4 bytes. It is the data length which Slave prepared.
 *            #ifdef ENABLE_ERROR_RETRY
 *                             g_spi_master_send_error_counter = 0;
 *            #endif
 *                    while(1){
 *                         printf("mux_spi_master_demo_send-Step1_c: receive SPI slave Tx_len Reg value.\r\n");
 *                         printf("mux_spi_master_demo_send-Step1_c: Master want to receive 4B.\r\n");
 *                         g_send_buf[0] = SPIS_RD_CMD;
 *                         spi_send_and_receive_config.receive_length = 4+1;
 *                         spi_send_and_receive_config.send_length = 1;
 *                         spi_send_and_receive_config.send_data = g_send_buf;
 *                         spi_send_and_receive_config.receive_buffer = g_receive_buf;
 *                         status = hal_spi_master_send_and_receive_polling(HAL_SPI_MASTER_0,&spi_send_and_receive_config);
 *                         if( HAL_SPI_MASTER_STATUS_OK == status){
 *                             receive_reg_value = 0;
 *                             receive_reg_value = g_receive_buf[1] | (g_receive_buf[2]<<8) | (g_receive_buf[3]<<16)|(g_receive_buf[3]<<24);
 *                             printf("mux_spi_master_demo_send-Step1_c: Receive SPI slave Rx_len Reg value:0x%x. success!!!\r\n",receive_reg_value);
 *                             break;
 *                         }
 *                         printf("mux_spi_master_demo_send-Step1_c: SPI master receive err, try again...status:%d\r\n",status);
 *                         assert(0);
 *            #ifdef ENABLE_ERROR_RETRY
 *                       g_spi_master_send_error_counter++;
 *                       if(g_spi_master_send_error_counter>MAX_ERROR_NUMBER){
 *                           printf("mux_spi_master_demo_send-Step1_c: #### too many ERROR counter %d, now go to restart!!!!!\r\n",g_spi_master_send_error_counter);
 *                           spi_master_mutex_give();
 *                          goto mux_spi_master_demo_send_restart;
 *                       }
 *            #endif
 *                    }
 *                    if(receive_reg_value > len){
 *                         printf("mux_spi_master_demo_send-Step1_c: Receive SPI slave Rx_len Reg value:%d, but master just want to send:%d\r\n",receive_reg_value,len);
 *                         receive_reg_value = len;
 *                     }
 *                    if(receive_reg_value == 0){
 *                         spi_master_mutex_give();
 *                         return 0;
 *                    }
 *                    spi_master_mutex_give();
 *
 *                    spi_master_mutex_take();
 *            //     Step2: Master reads Slave the register of <VG_SPI_SLAVE_RX_BUF_REG_OFFSET> with the len of data_buffer_len which master wants to write.
 *            //             Step2_a: Master sends Config Write (CW) cmd to slave:
 *            //                 Master sends the 9-byte command to slave: [0]:SPIS_CFG_RD_CMD, [1:4]VG_SPI_SLAVE_TX_BUF_REG_OFFSET,[5:8] len-1
 *
 *            #ifdef ENABLE_ERROR_RETRY
 *                    g_spi_master_send_error_counter = 0;
 *            #endif
 *                    length = receive_reg_value;
 *                     offset = VG_SPI_SLAVE_RX_BUF_REG_OFFSET;
 *                     request_cmd[0] = SPIS_CFG_WR_CMD;
 *                     request_cmd[1] = offset & 0xff;
 *                     request_cmd[2] = (offset >> 8) & 0xff;
 *                     request_cmd[3] = (offset >> 16) & 0xff;
 *                     request_cmd[4] = (offset >> 24) & 0xff;
 *                     request_cmd[5] = (length - 1) & 0xff;
 *                     request_cmd[6] = ((length - 1) >> 8) & 0xff;
 *                     request_cmd[7] = ((length - 1) >> 16) & 0xff;
 *                     request_cmd[8] = ((length - 1) >> 24) & 0xff;
 *                    while(1){
 *                        printf("mux_spi_master_demo_send-Step2_a: send VG_SPI_SLAVE_WD_CMD.\r\n");
 *                        printf("mux_spi_master_demo_send-Step2_a:Master want to send 8B \r\n");
 *                        status = hal_spi_master_send_polling(HAL_SPI_MASTER_0,request_cmd,VG_SPI_SLAVE_CMD_LEN);
 *                        if( HAL_SPI_MASTER_STATUS_OK == status)
 *                        {
 *                          printf("mux_spi_master_demo_send-Step2_a: Send VG_SPI_SLAVE_RD_CMD. success!!!\r\n");
 *                          break;
 *                        }
 *                        printf("mux_spi_master_demo_send-Step2_a: SPI master send err, try again...status:%d \r\n",status);
 *                        assert(0);
 *            #ifdef ENABLE_ERROR_RETRY
 *                       g_spi_master_send_error_counter++;
 *                       if(g_spi_master_send_error_counter>MAX_ERROR_NUMBER){
 *                           printf("mux_spi_master_demo_send-Step2_a: #### too many ERROR counter %d, now go to restart!!!!!\r\n",g_spi_master_send_error_counter);
 *                           spi_master_mutex_give();
 *                          goto mux_spi_master_demo_send_restart;
 *                       }
 *            #endif
 *                    }
 *             //         Step2_b: Wait for SPI slave CR to be ready
 *            #ifdef ENABLE_ERROR_RETRY
 *                    g_spi_master_send_error_counter = 0;
 *            #endif
 *                     printf("mux_spi_master_demo_send-Step2_b: wait slave CR done...\r\n");
 *                     while(1){
 *                         uint8_t status_recv, check_status;
 *                         spi_query_slave_status(&status_recv,HAL_SPI_MASTER_0);
 *                         if((SPIS_STA_TXRX_FIFO_RDY_MASK & status_recv)==SPIS_STA_TXRX_FIFO_RDY_MASK)
 *                             break;
 *            #ifdef ENABLE_ERROR_RETRY
 *                       g_spi_master_send_error_counter++;
 *                       if(g_spi_master_send_error_counter>MAX_ERROR_NUMBER){
 *                           printf("mux_spi_master_demo_send-Step2_b: #### too many ERROR counter %d, now go to restart!!!!!\r\n",g_spi_master_send_error_counter);
 *                           spi_master_mutex_give();
 *                          goto mux_spi_master_demo_send_restart;
 *                       }
 *            #endif
 *                     }
 *            //  Step2_c: Master sends <data_buffer_len> data.
 *            //             Slave address is DEMO_SPI_SLAVE_ADDRESS_W
 *            //             Master sends <data_buffer_len> bytes. It is the data which master wants to write.
 *            #ifdef ENABLE_ERROR_RETRY
 *                    g_spi_master_send_error_counter = 0;
 *            #endif
 *            //        Just for the SPI master design behavior. The buf address must be aligned with 4B.
 *                        p_temp_malloc_buff = NULL;
 *                        if((uint32_t)buf % 4 != 0){
 *                            p_temp_malloc_buff= pvPortMallocNC(receive_reg_value+1);
 *                            memcpy(p_temp_malloc_buff+1,buf,receive_reg_value);
 *                            buf = p_temp_malloc_buff;
 *                        }
 *                    hal_spi_master_register_callback(HAL_SPI_MASTER_0, spi_user_send_callback, receive_reg_value);//Register a user callback.
 *                    printf("mux_spi_master_demo_send-Step2_c:Master want to send:%d\r\n",receive_reg_value);
 *                    while(1){
 *                        send_irq_err_flag = 0;
 *                        printf("mux_spi_master_demo_send-Step2_c: master send data. buf address:0x%x\r\n",buf);
 *                        buf[0] = SPIS_WR_CMD;
 *                        status = hal_spi_master_send_dma(HAL_SPI_MASTER_0, buf, receive_reg_value+1);
 *                        if (HAL_SPI_MASTER_STATUS_OK == status) {
 *                             wait_for_send_done();
 *                             if(send_irq_err_flag == 1){
 *                                 printf("mux_spi_master_demo_send-Step2_c: receive Tx_buff data. success\r\n");
 *                                 break;
 *                             }
 *                        }
 *                        else{
 *                             printf("mux_spi_master_demo_send-Step2_c: send fail!!!status:%d\r\n",status);
 *                            assert(0);
 *                        }
 *                        printf("mux_spi_master_demo_send-Step2_c: SPI master receive err, try again...\r\n");
 *                        while(1);
 *                        assert(0);
 *            #ifdef ENABLE_ERROR_RETRY
 *                       g_spi_master_send_error_counter++;
 *                       if(g_spi_master_send_error_counter>MAX_ERROR_NUMBER){
 *                           printf("mux_spi_master_demo_send-Step2_c: #### too many ERROR counter %d, now go to restart!!!!!\r\n",g_spi_master_send_error_counter);
 *                           if(p_temp_malloc_buff != NULL)
 *                             vPortFreeNC(p_temp_malloc_buff);
 *                           spi_master_mutex_give();
 *                          goto mux_spi_master_demo_send_restart;
 *                       }
 *            #endif
 *                  }
 *                  if(p_temp_malloc_buff != NULL)
 *                     vPortFreeNC(p_temp_malloc_buff);
 *                  spi_master_mutex_give();
 *                  return receive_reg_value;
 *             }
 *      @endcode
 * - \b MUX \b support \b I2C \b slave \n
 *      The architecture of MUX I2C slave as below:
 *      @image html mux_i2c_slave_architecture.png
 *     The I2C master demo code as below:
 *      @code
 *            #ifdef ENABLE_ERROR_RETRY
 *                #define MAX_ERROR_NUMBER 1000
 *                 uint32_t g_i2c_master_send_error_counter = 0;
 *                 uint32_t g_i2c_master_receive_error_counter = 0;
 *            #endif
 *
 *            #define VG_I2C_SLAVE_CR_CMD 0xaa51
 *            #define VG_I2C_SLAVE_CW_CMD 0xaa53
 *            #define VG_I2C_SLAVE_CMD_LEN 8 // The first word[31:16]: for cmd. The first word[15:0]::for reg index  The second word: for register length.
 *             //matser<--> slave CR or CW data format:
 *             //        Word 0:
 *             //            [31:16]- VG_I2C_SLAVE_CR_CMD or VG_I2C_SLAVE_CW_CMD
 *             //            [15:0]-   VG_I2C_SLAVE_xxx_xxx_REG_OFFSET
 *             //        Word:
 *             //            [31:0] Register len.
 *
 *            #define VG_I2C_SLAVE_TX_LEN_REG_OFFSET 0x08
 *            #define VG_I2C_SLAVE_TX_BUF_REG_OFFSET 0x2000
 *
 *            #define VG_I2C_SLAVE_RX_LEN_REG_OFFSET 0x04
 *            #define VG_I2C_SLAVE_RX_BUF_REG_OFFSET 0x1000
 *
 *            #define DEMO_I2C_SLAVE_ADDRESS_CR_OR_CW 0x50//(0x30)//
 *            #define DEMO_I2C_SLAVE_ADDRESS_R        0x54//(0xa1)//
 *            #define DEMO_I2C_SLAVE_ADDRESS_W        0x58//(0xa2)//
 *
 *             ATTR_RWDATA_IN_NONCACHED_SYSRAM_4BYTE_ALIGN uint8_t send_data_buf[50]; //the data that used to send
 *             ATTR_RWDATA_IN_NONCACHED_SYSRAM_4BYTE_ALIGN uint8_t receive_data_buf[50] = {0};
 *
 *             volatile int32_t send_irq_err_flag = 0;
 *             volatile int32_t receive_irq_err_flag = 0;
 *
 *             static SemaphoreHandle_t g_i2c_master_mutex = NULL;
 *             static SemaphoreHandle_t wait_send_done_xSemaphore = NULL;
 *             static SemaphoreHandle_t wait_receive_done_xSemaphore = NULL;
 *
 *             void i2c_master_mutex_take(void)
 *             {
 *                 printf("i2c_master_mutex_take\r\n");
 *                 if (xSemaphoreTake(g_i2c_master_mutex, portMAX_DELAY) == pdFALSE) {
 *                     printf("i2c_master_mutex_take error\r\n", 0);
 *                     return;
 *                 }
 *             }
 *             void i2c_master_mutex_give(void)
 *             {
 *                 printf("i2c_master_mutex_give\r\n");
 *                 if (xSemaphoreGive(g_i2c_master_mutex) == pdFALSE) {
 *                     LOG_MSGID_E(common, "i2c_master_mutex_give error\r\n", 0);
 *                     return;
 *                 }
 *             }
 *
 *             void mux_i2c_master_demo_init()
 *             {
 *                 demo_uart_echo_init();
 *                 g_i2c_master_mutex = xSemaphoreCreateMutex();
 *                 if (g_i2c_master_mutex == NULL) {
 *                     printf( "g_i2c_master_mutex create error\r\n");
 *                     return;
 *                 }
 *                 wait_send_done_xSemaphore = xSemaphoreCreateBinary();
 *                 if (wait_send_done_xSemaphore == NULL) {
 *                     printf( "wait_send_done_xSemaphore create error\r\n");
 *                     return;
 *                 }
 *                 wait_receive_done_xSemaphore = xSemaphoreCreateBinary();
 *                 if (wait_receive_done_xSemaphore == NULL) {
 *                     printf( "wait_receive_done_xSemaphore create error\r\n");
 *                     return;
 *                 }
 *                 hal_gpio_init(HAL_GPIO_9); //init the gpio pin
 *                 hal_pinmux_set_function(HAL_GPIO_9, HAL_GPIO_9_I2C_MST1_SCL);//Set the pinmux
 *                 hal_gpio_init(HAL_GPIO_10); //init the gpio pin
 *                 hal_pinmux_set_function(HAL_GPIO_10, HAL_GPIO_10_I2C_MST1_SDA);//Set the pinmux
 *
 *                 {
 *                     hal_i2c_config_t i2c_config;
 *                     i2c_config.frequency = HAL_I2C_FREQUENCY_50K;
 *                    if(HAL_I2C_STATUS_OK == hal_i2c_master_init(HAL_I2C_MASTER_1, &i2c_config)){
 *                         hal_i2c_master_set_io_config(HAL_I2C_MASTER_1,HAL_I2C_IO_PUSH_PULL);
 *                    }
 *                    else{
 *                     printf("mux_i2c_master_demo_receive: open fail, try next time...\r\n");
 *                     return;
 *                   }
 *                 }
 *
 *             }
 *
 *             void i2c_user_send_callback(uint8_t slave_address, hal_i2c_callback_event_t event, void *user_data)
 *             {
 *                 BaseType_t xHigherPriorityTaskWoken = pdFALSE;
 *                 uint32_t send_done_len = (uint32_t)user_data;
 *                 if(event == HAL_I2C_EVENT_SUCCESS){
 *                     send_irq_err_flag= 1;
 *                     printf("Send done, size: %d\r\n",strlen(send_data_buf));
 *                 }
 *                 else{
 *                     send_irq_err_flag = -1;
 *                     printf("i2c_user_send_callback ERROR, err event:%d\r\n",event);
 *                 }
 *                 xSemaphoreGiveFromISR( wait_send_done_xSemaphore, &xHigherPriorityTaskWoken );
 *             }
 *
 *
 *
 *             void i2c_user_receive_callback(uint8_t slave_address, hal_i2c_callback_event_t event, void *user_data)
 *             {
 *                 BaseType_t xHigherPriorityTaskWoken = pdFALSE;
 *                 uint32_t receive_done_len = (uint32_t)user_data;
 *                 if(event == HAL_I2C_EVENT_SUCCESS){
 *                     receive_irq_err_flag = 1;
 *                  }
 *                 else{
 *                     receive_irq_err_flag = -1;
 *                     printf("i2c_user_receive_callback ERROR, err event:%d\r\n",event);
 *                 }
 *                 xSemaphoreGiveFromISR( wait_receive_done_xSemaphore, &xHigherPriorityTaskWoken );
 *             }
 *
 *             void wait_for_send_done()
 *             {
 *                if(xSemaphoreTake( wait_send_done_xSemaphore, portMAX_DELAY) == pdTRUE){
 *                }
 *                else{
 *                    assert(0);
 *                }
 *             }
 *             void wait_for_receive_done()
 *             {
 *                if(xSemaphoreTake( wait_receive_done_xSemaphore, portMAX_DELAY) == pdTRUE){
 *                }
 *                else{
 *                    assert(0);
 *                }
 *             }
 *            //     This is data Master read from slave:
 *            //     Step0: Init I2C master
 *            //     Step1: Master reads Slave the register of <VG_I2C_SLAVE_TX_LEN_REG_OFFSET> with a length of 4 bytes.
 *            //             Step1_a: Master sends the Config Read (CR) cmd to slave:
 *            //                Slave address is DEMO_I2C_SLAVE_ADDRESS_CR_OR_CW.
 *            //                 Master send two word to slave:{(VG_I2C_SLAVE_CR_CMD<<16)|VG_I2C_SLAVE_TX_LEN_REG_OFFSET,4}
 *            //             Step1_b: Master receives the value of <VG_I2C_SLAVE_TX_LEN_REG_OFFSET>
 *            //                Slave address is DEMO_I2C_SLAVE_ADDRESS_R
 *            //                Master receives 4 bytes, it's the data len which Slave prepared.
 *            //     Step2: Master reads Slave the register of <VG_I2C_SLAVE_TX_BUF_REG_OFFSET> with the length of data_buffer_len.
 *            //             Step2_a: Master sends the Config Read (CR) cmd to slave:
 *            //                 Slave address is DEMO_I2C_SLAVE_ADDRESS_CR_OR_CW.
 *            //                 Master send two word to slave:{(VG_I2C_SLAVE_CR_CMD<<16)|VG_I2C_SLAVE_TX_BUF_REG_OFFSET,data_buffer_len}
 *            //             Step2_b: Master receives <data_buffer_len> data.
 *            //                 Slave address is DEMO_I2C_SLAVE_ADDRESS_R
 *            //                 Master receive <data_buffer_len> bytes, it's the data which Slave prepared.
 *            //     Step3: Deinit I2C master
 *             uint32_t mux_i2c_master_demo_receive(uint8_t *buf,uint32_t len)
 *             {
 *                    hal_i2c_config_t i2c_config;
 *                    hal_i2c_receive_config_t receive_config;
 *                    hal_i2c_send_config_t send_config;
 *                    hal_i2c_status_t status;
 *                    uint32_t request_cmd[2];
 *                    uint32_t receive_reg_value = 0;
 *            #ifdef ENABLE_ERROR_RETRY
 *             mux_i2c_master_demo_receive_restart:
 *            #endif
 *                    i2c_master_mutex_take();
 *            //     Step1: Master reads Slave the register of <VG_I2C_SLAVE_TX_LEN_REG_OFFSET> with a length of 4 bytes.
 *            //          Step1_a: Master sends the Config Read (CR) cmd to slave:
 *            //                     Slave address is DEMO_I2C_SLAVE_ADDRESS_CR_OR_CW.
 *            //                     Master sends two words to slave::{(VG_I2C_SLAVE_CR_CMD<<16)|VG_I2C_SLAVE_TX_LEN_REG_OFFSET,4}
 *            #ifdef ENABLE_ERROR_RETRY
 *                 g_i2c_master_receive_error_counter = 0;
 *            #endif
 *                   request_cmd[0] = (VG_I2C_SLAVE_CR_CMD<<16) | VG_I2C_SLAVE_TX_LEN_REG_OFFSET;
 *                   request_cmd[1] = 4;
 *                   while(1){
 *                     printf("mux_i2c_master_demo_receive-Step1_a: Send VG_I2C_SLAVE_CRD_CMD.\r\n");
 *                     printf("mux_i2c_master_demo_receive-Step1_a: Switch I2C Slave address to :0x%x. Master want to send 8B\r\n",DEMO_I2C_SLAVE_ADDRESS_CR_OR_CW);
 *                     status = hal_i2c_master_send_polling(HAL_I2C_MASTER_1, DEMO_I2C_SLAVE_ADDRESS_CR_OR_CW,request_cmd,VG_I2C_SLAVE_CMD_LEN);
 *                     if( HAL_I2C_STATUS_OK == status){
 *                         printf("mux_i2c_master_demo_receive-Step1_a: Send VG_I2C_SLAVE_CRD_CMD success!!!\r\n");
 *                         break;
 *                     }
 *                      printf("mux_i2c_master_demo_receive-Step1_a: I2C master send err, try again...status:%d \r\n",status);
 *            #ifdef ENABLE_ERROR_RETRY
 *                         g_i2c_master_receive_error_counter++;
 *                         if(g_i2c_master_receive_error_counter>MAX_ERROR_NUMBER){
 *                             printf("mux_i2c_master_demo_receive-Step1_a: #### too many ERROR counter %d, now go to restart!!!!!\r\n",g_i2c_master_receive_error_counter);
 *                             i2c_master_mutex_give();
 *                            goto mux_i2c_master_demo_receive_restart;
 *                         }
 *            #endif
 *                   }
 *            //         Step1_b: Master receives the value of <VG_I2C_SLAVE_TX_LEN_REG_OFFSET>
 *            //                Slave address is DEMO_I2C_SLAVE_ADDRESS_R
 *            //                Master receives 4 bytes. It is the data length which Slave prepared.
 *            #ifdef ENABLE_ERROR_RETRY
 *                    g_i2c_master_receive_error_counter = 0;
 *            #endif
 *                    while(1){
 *                         printf("mux_i2c_master_demo_receive-Step1_b: Receive I2C slave Tx_len Reg value. \r\n");
 *                         printf("mux_i2c_master_demo_receive-Step1_b: Switch I2C Slave address to :0x%x.Master want to receive 4B.\r\n",
 *                                 DEMO_I2C_SLAVE_ADDRESS_R);
 *                         status = hal_i2c_master_receive_polling(HAL_I2C_MASTER_1, DEMO_I2C_SLAVE_ADDRESS_R, &receive_reg_value, 4);
 *                         if( HAL_I2C_STATUS_OK == status){
 *                             printf("mux_i2c_master_demo_receive-Step1_b: Receive I2C slave Tx_len Reg value:0x%x. success!!!\r\n",receive_reg_value);
 *                             break;
 *                         }
 *                         printf("mux_i2c_master_demo_receive-Step1_b : I2C master receive err, try again...status:%d\r\n",status);
 *            #ifdef ENABLE_ERROR_RETRY
 *                                     g_i2c_master_receive_error_counter++;
 *                                     if(g_i2c_master_receive_error_counter>MAX_ERROR_NUMBER){
 *                                         printf("mux_i2c_master_demo_receive-Step1_b: #### too many ERROR counter %d, now go to restart!!!!!\r\n",g_i2c_master_receive_error_counter);
 *                                         i2c_master_mutex_give();
 *                                        goto mux_i2c_master_demo_receive_restart;
 *                                     }
 *            #endif
 *                    }
 *                    if(receive_reg_value > len){
 *                         receive_reg_value = len;
 *                         printf("mux_i2c_master_demo_receive-Step1_b: slave data len too big, request %d B firstly;\r\n",len);
 *                    }
 *                    if(receive_reg_value == 0){//The received length should not be 0.
 *                         i2c_master_mutex_give();
 *                         return 0;
 *                    }
 *                    i2c_master_mutex_give();
 *                 i2c_master_mutex_take();
 *            //     Step2: Master reads Slave the register of <VG_I2C_SLAVE_TX_BUF_REG_OFFSET> with the length of data_buffer_len.
 *            //             Step2_a: Master sends the Config Read (CR) cmd to slave:
 *            //                 Slave address is DEMO_I2C_SLAVE_ADDRESS_CR_OR_CW.
 *            //                 Master send two word to slave:{(VG_I2C_SLAVE_CR_CMD<<16)|VG_I2C_SLAVE_TX_BUF_REG_OFFSET,data_buffer_len}
 *            #ifdef ENABLE_ERROR_RETRY
 *                        g_i2c_master_receive_error_counter = 0;
 *            #endif
 *                    request_cmd[0] = (VG_I2C_SLAVE_CR_CMD<<16) | VG_I2C_SLAVE_TX_BUF_REG_OFFSET;
 *                    request_cmd[1] = receive_reg_value;
 *                    while(1){
 *                        printf("mux_i2c_master_demo_receive-Step2_a: send VG_I2C_SLAVE_RD_CMD.\r\n");
 *                        printf("mux_i2c_master_demo_receive-Step2_a: Switch I2C Slave address to :0x%x.Master want to send 8B cmd.\r\n",DEMO_I2C_SLAVE_ADDRESS_CR_OR_CW);
 *                        status = hal_i2c_master_send_polling(HAL_I2C_MASTER_1, DEMO_I2C_SLAVE_ADDRESS_CR_OR_CW,request_cmd,VG_I2C_SLAVE_CMD_LEN);
 *                        if( HAL_I2C_STATUS_OK == status){
 *                          printf("mux_i2c_master_demo_receive-Step2_a: Send VG_I2C_SLAVE_RD_CMD. success!!!\r\n");
 *                          break;
 *                        }
 *                        printf("mux_i2c_master_demo_receive-Step2_a: I2C master send err, try again...status:%d \r\n",status);
 *            #ifdef ENABLE_ERROR_RETRY
 *                        g_i2c_master_receive_error_counter++;
 *                        if(g_i2c_master_receive_error_counter>MAX_ERROR_NUMBER){
 *                            printf("mux_i2c_master_demo_receive-Step2_a: #### too many ERROR counter %d, now go to restart!!!!!\r\n",g_i2c_master_receive_error_counter);
 *                            i2c_master_mutex_give();
 *                           goto mux_i2c_master_demo_receive_restart;
 *                        }
 *            #endif
 *                    }
 *            //         Step2_b: Master receives <data_buffer_len> data.
 *            //                     Slave address is DEMO_I2C_SLAVE_ADDRESS_R
 *            //                     Master receives <data_buffer_len> bytes. It is the data which Slave prepared.
 *            #ifdef ENABLE_ERROR_RETRY
 *                    g_i2c_master_receive_error_counter = 0;
 *            #endif
 *                    hal_i2c_master_register_callback(HAL_I2C_MASTER_1, i2c_user_receive_callback, receive_reg_value);//Register a user callback.
 *                    receive_config.receive_bytes_in_one_packet = receive_reg_value;
 *                    receive_config.receive_buffer = buf;
 *                    receive_config.receive_packet_length = 1;
 *                    receive_config.slave_address = DEMO_I2C_SLAVE_ADDRESS_R;
 *                    printf("mux_i2c_master_demo_receive-Step2_b: Switch I2C Slave address to :0x%x,Master want to receive:%d\r\n",
 *                     DEMO_I2C_SLAVE_ADDRESS_R,receive_reg_value);
 *                    while(1){
 *                        receive_irq_err_flag = 0;
 *                        printf("mux_i2c_master_demo_receive-Step2_b: receive Tx_buff data.  \r\n");
 *                        status = hal_i2c_master_receive_dma_ex(HAL_I2C_MASTER_1, &receive_config);
 *                        if (HAL_I2C_STATUS_OK == status) {
 *                             wait_for_receive_done();
 *                             if(receive_irq_err_flag == 1){
 *                                 printf("mux_i2c_master_demo_receive-Step2_b: Receive Tx_buff data. success\r\n");
 *                                 break;
 *                             }
 *                        }
 *                        else{
 *                          printf("mux_i2c_master_demo_receive-Step2_b: hal_i2c_master_receive_dma_ex return ERROR!!! status:%d\r\n",status);
 *                             while(1);
 *                            assert(0);
 *                        }
 *                        printf("mux_i2c_master_demo_receive-Step2_b: I2C master receive err, try again...\r\n");
 *            #ifdef ENABLE_ERROR_RETRY
 *                       g_i2c_master_receive_error_counter++;
 *                       if(g_i2c_master_receive_error_counter>MAX_ERROR_NUMBER){
 *                           printf("mux_i2c_master_demo_receive-Step2_b: #### too many ERROR counter %d, now go to restart!!!!!\r\n",g_i2c_master_receive_error_counter);
 *                           i2c_master_mutex_give();
 *                          goto mux_i2c_master_demo_receive_restart;
 *                       }
 *            #endif
 *                  }
 *                  i2c_master_mutex_give();
 *                  return receive_reg_value;
 *             }
 *            //     This is the data Master writes to slave:
 *            //     Step0: Init I2C master
 *            //     Step1: Master reads Slave the register of <VG_I2C_SLAVE_RX_LEN_REG_OFFSET> with with a length of 4 bytes.
 *            //             Step1_a: Master sends the Config Read (CR) cmd to slave:
 *            //                 Slave address is DEMO_I2C_SLAVE_ADDRESS_CR_OR_CW.
 *            //                 Master send two word to slave:{(VG_I2C_SLAVE_CR_CMD<<16)|VG_I2C_SLAVE_RX_LEN_REG_OFFSET,4}
 *            //             Step1_b: Master receives the value of <VG_I2C_SLAVE_RX_LEN_REG_OFFSET>
 *            //                Slave address is DEMO_I2C_SLAVE_ADDRESS_R
 *            //                Master receives 4 bytes. It is the data length of the Slave Rx buffer free size.
 *            //     Step2: Master read Slave the register of <VG_I2C_SLAVE_RX_BUF_REG_OFFSET> with the length of data_buffer_len which master want to write.
 *            //             Step2_a: Master sends the Config Write (CW) cmd to slave:
 *            //                 Slave address is DEMO_I2C_SLAVE_ADDRESS_CR_OR_CW.
 *            //                 Master send two word to slave:{(VG_I2C_SLAVE_CW_CMD<<16)|VG_I2C_SLAVE_RX_BUF_REG_OFFSET,data_buffer_len}
 *            //             Step2_b: Master sends <data_buffer_len> data.
 *            //                 Slave address is DEMO_I2C_SLAVE_ADDRESS_W
 *            //                 Master sends <data_buffer_len> bytes. It is the data which Slave prepared.
 *            //     Step3: Deinit I2C master
 *             uint32_t mux_i2c_master_demo_send(uint8_t *buf,uint32_t len)
 *             {
 *                    hal_i2c_config_t i2c_config;
 *                    hal_i2c_send_config_t send_config;
 *                    hal_i2c_status_t status;
 *                    uint32_t request_cmd[2];
 *                    uint32_t receive_reg_value;
 *            #ifdef ENABLE_ERROR_RETRY
 *                    mux_i2c_master_demo_send_restart:
 *            #endif
 *                 i2c_master_mutex_take();
 *                 //Step1: Master reads Slave the register of <VG_I2C_SLAVE_RX_LEN_REG_OFFSET> with a length of 4 bytes.
 *                 //    Step1_a: Master sends the Config Read (CR) cmd to slave:
 *                 //        Slave address is DEMO_I2C_SLAVE_ADDRESS_CR_OR_CW.
 *                 //        Master send two word to slave:{(VG_I2C_SLAVE_CR_CMD<<16)|VG_I2C_SLAVE_RX_LEN_REG_OFFSET,4}
 *            #ifdef ENABLE_ERROR_RETRY
 *                     g_i2c_master_send_error_counter = 0;
 *            #endif
 *                     request_cmd[0] = (VG_I2C_SLAVE_CR_CMD<<16) | VG_I2C_SLAVE_RX_LEN_REG_OFFSET;
 *                     request_cmd[1] = 4;
 *                     while(1){
 *                         printf("mux_i2c_master_demo_send-Step1_a: Send VG_I2C_SLAVE_CRD_CMD.\r\n");
 *                         printf("mux_i2c_master_demo_send-Step1_a: Switch I2C Slave address to :0x%x. Master want to send 8B cmd.\r\n",DEMO_I2C_SLAVE_ADDRESS_CR_OR_CW);
 *                         status = hal_i2c_master_send_polling(HAL_I2C_MASTER_1, DEMO_I2C_SLAVE_ADDRESS_CR_OR_CW,request_cmd,VG_I2C_SLAVE_CMD_LEN);
 *                     if( HAL_I2C_STATUS_OK == status){
 *                         printf("mux_i2c_master_demo_send-Step1_a: Send VG_I2C_SLAVE_CRD_CMD success!!!\r\n");
 *                         break;
 *                     }
 *                         printf("mux_i2c_master_demo_send-Step1_a: I2C master send err, try again...status:%d \r\n",status);
 *            #ifdef ENABLE_ERROR_RETRY
 *                       g_i2c_master_receive_error_counter++;
 *                       if(g_i2c_master_receive_error_counter>MAX_ERROR_NUMBER){
 *                           printf("mux_i2c_master_demo_send-Step1_a: #### too many ERROR counter %d, now go to restart!!!!!\r\n",g_i2c_master_receive_error_counter);
 *                           i2c_master_mutex_give();
 *                          goto mux_i2c_master_demo_send_restart;
 *                       }
 *            #endif
 *                     }
 *                 //Step1_b: Master receives the value of <VG_I2C_SLAVE_RX_LEN_REG_OFFSET>
 *                 //           Slave address is DEMO_I2C_SLAVE_ADDRESS_R
 *                 //           Master receives 4 bytes. It is the data length which Slave Rx buffer free size.
 *            #ifdef ENABLE_ERROR_RETRY
 *                             g_i2c_master_send_error_counter = 0;
 *            #endif
 *                    while(1){
 *                         printf("mux_i2c_master_demo_send-Step1_b: receive I2C slave Tx_len Reg value.\r\n");
 *                         printf("mux_i2c_master_demo_send-Step1_b: Switch I2C Slave address to :0x%x.Master want to receive 4B.\r\n",DEMO_I2C_SLAVE_ADDRESS_R);
 *                         status = hal_i2c_master_receive_polling(HAL_I2C_MASTER_1, DEMO_I2C_SLAVE_ADDRESS_R, &receive_reg_value, 4);
 *                         if( HAL_I2C_STATUS_OK == status){
 *                             printf("mux_i2c_master_demo_send-Step1_b: Receive I2C slave Rx_len Reg value:0x%x. success!!!\r\n",receive_reg_value);
 *                             break;
 *                         }
 *                         printf("mux_i2c_master_demo_send-Step1_b: I2C master receive err, try again...status:%d\r\n",status);
 *            #ifdef ENABLE_ERROR_RETRY
 *                       g_i2c_master_receive_error_counter++;
 *                       if(g_i2c_master_receive_error_counter>MAX_ERROR_NUMBER){
 *                           printf("mux_i2c_master_demo_send-Step1_b: #### too many ERROR counter %d, now go to restart!!!!!\r\n",g_i2c_master_receive_error_counter);
 *                           i2c_master_mutex_give();
 *                          goto mux_i2c_master_demo_send_restart;
 *                       }
 *            #endif
 *                    }
 *                    if(receive_reg_value > len){
 *                         printf("mux_i2c_master_demo_send-Step1_b: Receive I2C slave Rx_len Reg value:%d, but master just want to send:%d\r\n",receive_reg_value,len);
 *                         receive_reg_value = len;
 *                     }
 *                    if(receive_reg_value == 0){
 *                         i2c_master_mutex_give();
 *                         return 0;
 *                    }
 *                    i2c_master_mutex_give();
 *
 *                    i2c_master_mutex_take();
 *                 //Step2: Master reads Slave the register of <VG_I2C_SLAVE_RX_BUF_REG_OFFSET> with the length of data_buffer_len which master want to write.
 *                     //Step2_a: Master send Config Write (CW) cmd to slave:
 *                         //Slave address is DEMO_I2C_SLAVE_ADDRESS_CR_OR_CW.
 *                         //Master send two word to slave:{(VG_I2C_SLAVE_CW_CMD<<16)|VG_I2C_SLAVE_RX_BUF_REG_OFFSET,data_buffer_len}
 *            #ifdef ENABLE_ERROR_RETRY
 *                    g_i2c_master_send_error_counter = 0;
 *            #endif
 *                    request_cmd[0] = (VG_I2C_SLAVE_CW_CMD<<16) | VG_I2C_SLAVE_RX_BUF_REG_OFFSET;
 *                    request_cmd[1] = receive_reg_value;
 *                    while(1){
 *                        printf("mux_i2c_master_demo_send-Step2_a: send VG_I2C_SLAVE_WD_CMD.\r\n");
 *                        printf("mux_i2c_master_demo_send-Step2_a: Switch I2C Slave address to :0x%x.Master want to send 8B \r\n",DEMO_I2C_SLAVE_ADDRESS_CR_OR_CW);
 *                        status = hal_i2c_master_send_polling(HAL_I2C_MASTER_1, DEMO_I2C_SLAVE_ADDRESS_CR_OR_CW,request_cmd,VG_I2C_SLAVE_CMD_LEN);
 *                        if( HAL_I2C_STATUS_OK == status)
 *                        {
 *                          printf("mux_i2c_master_demo_send-Step2_a: Send VG_I2C_SLAVE_RD_CMD. success!!!\r\n");
 *                          break;
 *                        }
 *                        printf("mux_i2c_master_demo_send-Step2_a: I2C master send err, try again...status:%d \r\n",status);
 *            #ifdef ENABLE_ERROR_RETRY
 *                      g_i2c_master_receive_error_counter++;
 *                      if(g_i2c_master_receive_error_counter>MAX_ERROR_NUMBER){
 *                          printf("mux_i2c_master_demo_send-Step2_a: #### too many ERROR counter %d, now go to restart!!!!!\r\n",g_i2c_master_receive_error_counter);
 *                          i2c_master_mutex_give();
 *                         goto mux_i2c_master_demo_send_restart;
 *                      }
 *            #endif
 *                    }
 *                 //Step2_b: Master sends <data_buffer_len> data.
 *                 //        Slave address is DEMO_I2C_SLAVE_ADDRESS_W
 *                 //        Master sends <data_buffer_len> bytes. It is the data which master wants to write.
 *            #ifdef ENABLE_ERROR_RETRY
 *                    g_i2c_master_send_error_counter = 0;
 *            #endif
 *                    hal_i2c_master_register_callback(HAL_I2C_MASTER_1, i2c_user_send_callback, receive_reg_value);//Register a user callback.
 *                    send_config.send_bytes_in_one_packet = receive_reg_value;
 *                    send_config.send_data = buf;
 *                    send_config.send_packet_length = 1;
 *                    send_config.slave_address = DEMO_I2C_SLAVE_ADDRESS_W;
 *                    printf("mux_i2c_master_demo_send-Step2_b: Switch I2C Slave address to :0x%x. Master want to send:%d\r\n",
 *                                                 DEMO_I2C_SLAVE_ADDRESS_W,receive_reg_value);
 *                    while(1){
 *                        send_irq_err_flag = 0;
 *                        printf("mux_i2c_master_demo_send-Step2_b: master send data. \r\n");
 *                        status = hal_i2c_master_send_dma_ex(HAL_I2C_MASTER_1, &send_config);
 *                        if (HAL_I2C_STATUS_OK == status) {
 *                             wait_for_send_done();
 *                             if(send_irq_err_flag == 1){
 *                                 printf("mux_i2c_master_demo_send-Step2_b: receive Tx_buff data. success\r\n");
 *                                 break;
 *                             }
 *                        }
 *                        else{
 *                             printf("mux_i2c_master_demo_send-Step2_b: send fail!!!status:%d\r\n",status);
 *                             while(1);
 *                            assert(0);
 *                        }
 *                        printf("mux_i2c_master_demo_send-Step2_b: I2C master receive err, try again...\r\n");
 *            #ifdef ENABLE_ERROR_RETRY
 *                     g_i2c_master_receive_error_counter++;
 *                     if(g_i2c_master_receive_error_counter>MAX_ERROR_NUMBER){
 *                         printf("mux_i2c_master_demo_send-Step2_a: #### too many ERROR counter %d, now go to restart!!!!!\r\n",g_i2c_master_receive_error_counter);
 *                         i2c_master_mutex_give();
 *                        goto mux_i2c_master_demo_send_restart;
 *                     }
 *            #endif
 *                  }
 *                  i2c_master_mutex_give();
 *                  return receive_reg_value;
 *             }
 *      @endcode
 * @}
 * @}
 */



/**
* @addtogroup kernel_service
* @{
* @addtogroup MUX
* @{
*/


/** @defgroup mux_macro Macro
 *  @{
 */

/** @brief This macro defines the max MUX user number. */
#define MAX_MUX_USER_COUNT 8

/** @brief This macro defines the max length MUX user user name. */
#define MAX_USER_NAME    16
/**
 * @}
 */



/** @defgroup mux_enum Enum
 *  @{
 */
/** @brief MUX port type. */
typedef enum {
    MUX_UART_BEGIN,             /**<  port of  MUX_UART_BEGIN*/
    MUX_UART_0 = MUX_UART_BEGIN,/**<  port of  MUX_UART_0*/
    MUX_UART_1,                 /**<  port of  MUX_UART_1*/
    MUX_UART_END = MUX_UART_1,  /**<  port of  MUX_UART_END*/

    MUX_USB_BEGIN,                  /**<  port of  MUX_USB_BEGIN*/
    MUX_USB_COM_1 = MUX_USB_BEGIN,  /**<  port of  MUX_USB_COM_1*/
    MUX_USB_END = MUX_USB_COM_1,    /**<  port of  MUX_USB_END*/

    MUX_AIRAPP_BEGIN,                 /**<  port of  MUX_AIRAPP_BEGIN*/
    MUX_AIRAPP_0 = MUX_AIRAPP_BEGIN,  /**<  port of  MUX_AIRAPP_0*/
    MUX_AIRAPP_END = MUX_AIRAPP_0,    /**<  port of  MUX_AIRAPP_END*/

    MUX_PORT_END,   /**<  port of  MUX_PORT_END*/
} mux_port_t;
/** @brief MUX event type. */
typedef enum {
    MUX_EVENT_READY_TO_READ = 1,    /**<  event of  ready to read*/
    MUX_EVENT_READY_TO_WRITE,       /**<  event of  ready to write*/
    MUX_EVENT_CONNECTION,           /**<  event of  connection, special for usb*/
    MUX_EVENT_DISCONNECTION,        /**<  event of  disconnection, special for usb*/
    MUX_EVENT_WAKEUP_FROM_SLEEP,     /**<  event of  wakeup*/
} mux_event_t;

/** @brief MUX status type. */
typedef enum {
    MUX_STATUS_OK,                          /**<  status ok*/
    MUX_STATUS_ERROR,                       /**<  status error*/
    MUX_STATUS_ERROR_PARAMETER,             /**<  status error parameter*/
    MUX_STATUS_ERROR_INIT,                  /**<  status initialization*/
    MUX_STATUS_ERROR_NOT_INIT,              /**<  status uninitialized*/
    MUX_STATUS_ERROR_INITIATED,             /**<  status of have initialized*/
    MUX_STATUS_ERROR_NOT_EXIST,             /**<  status error of not exist*/
    MUX_STATUS_ERROR_INIT_FAIL,             /**<  status of initialize fail*/
    MUX_STATUS_ERROR_SOME_USER_STILL_OPEN,  /**<  status of still have some user opening this mux port, can not to close*/
    MUX_STATUS_ERROR_DEINIT_FAIL,           /**<  status of uninitialized fail*/
    MUX_STATUS_ERROR_TOO_MANY_USERS,        /**<  status of  too many users*/
    MUX_STATUS_USER_RX_QUEUE_EMPTY,         /**<  status of  port user rx package queue empty*/
    MUX_STATUS_USER_RX_BUF_SIZE_NOT_ENOUGH, /**<  status of  Rx buffer not enough*/
    MUX_STATUS_USER_TX_BUF_SIZE_NOT_ENOUGH, /**<  status of  Tx buffer not enough*/
    MUX_STATUS_USER_NOT_OPEN,               /**<  status of not open*/
    MUX_STATUS_USER_ERROR_OF_RX_TX_PROTOCOL,/**<  status of error, user Tx and Rx protocol callback error*/
    MUX_STATUS_ERROR_NVDM_INIT_FAIL,        /**<  status of NVDM initialized fail*/
    MUX_STATUS_ERROR_PORT_SETTING_CHANGED,  /**<  status of port setting changed*/
    MUX_STATUS_ERROR_NVDM_NOT_FOUND,        /**<  status of NVDM not found*/
} mux_status_t;

/** @brief MUX uart flow control type. */
typedef enum {
    MUX_UART_NONE_FLOWCONTROL,  /**<  MUX uart none flow control*/
    MUX_UART_SW_FLOWCONTROL,    /**<  MUX uart SW flow control*/
    MUX_UART_HW_FLOWCONTROL,    /**<  MUX uart HW flow control*/
} mux_uart_fc_t;
/**
 * @}
 */

/** @defgroup mux_struct struct
 *  @{
 */

/** @brief MUX handle type.\n
    bit[16:31]:magic number.\n
    bit[8:15]:user_id.\n
    bit[0:7]:mux port.\n
    */
typedef uint32_t mux_handle_t;

/** @brief MUX buffer type. */
typedef struct {
    uint8_t *p_buf;     /**<  MUX buffer start address*/
    uint32_t buf_size;  /**<  MUX buffer lenght*/
} mux_buffer_t;

/** @brief MUX port setting type. */
typedef struct {
    uint32_t tx_buffer_size;    /**<  MUX tx buffer size*/
    uint32_t rx_buffer_size;    /**<  MUX rx buffer size*/
    union {
        struct {
            hal_uart_config_t uart_config;      /**<  MUX uart config*/
            mux_uart_fc_t flowcontrol_type;     /**<  MUX uart flow control setting*/
        } uart;
        struct {
        } usb;
        struct { /**<  For I2C slave data accuracy, support difference slave address.*/
           uint8_t cr_or_cw_slave_address;    /**<  MUX I2C slave config read or config write slave address, should use slave address when master wants to config read or write .*/
           uint8_t r_slave_address;           /**<  MUX I2C slave read slave address, should use slave address when master wants to read.*/
           uint8_t w_slave_address;           /**<  MUX I2C slave write address, should use slave address when master wants to write.*/
        } i2c_slave;
        struct {
            #if 0
            hal_spi_slave_config_t slave_config;    /**<  MUX SPI slave config*/
            #endif
        } spi_slave;
        struct {
            /* no airapp setting */
        } airapp;
    } dev_setting;
} mux_port_setting_t;

/** @brief MUX port buffer type from NVDM */
typedef struct {
    uint8_t count;     /**<  query the count of how many ports opened by a designated user by the function of mux_query_user_port_numbers_from_nvdm() */
    uint8_t buf[MUX_PORT_END];  /**<  all of the ports information of the query*/
} mux_port_buffer_t;



/** @brief MUX callback function type
    When event == MUX_EVENT_READY_TO_READ, data_len means: there have one package data, len is data_len.\n
    When event == MUX_EVENT_READY_TO_WRITE,data_len means: will be always as 0x0.
*/
typedef void (*mux_callback_t)(mux_handle_t handle, mux_event_t event,uint32_t data_len,void*user_data);

/** @brief MUX protocol Tx callback head buffer max len, user should pay attention to void memory crash when implement the function of tx_protocol_callback() to prepare header buffer .*/
#define TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN 64
/** @brief MUX protocol Tx callback tail buffer max len, user should pay attention to void memory crash when implement the function of tx_protocol_callback() to prepare tail buffer.*/
#define TX_PROTOCOL_CB_TAIL_BUFFER_MAX_LEN 64

/** @brief MUX protocol callback type */
typedef struct {
    void (*tx_protocol_callback)(mux_handle_t handle, const mux_buffer_t payload[],uint32_t buffers_counter,
                mux_buffer_t *head, mux_buffer_t *tail,void *user_data); /**<  To do package, *head and *tail are the parameters out.*/
    void (*rx_protocol_callback)(mux_handle_t *handle, mux_buffer_t buffers[],uint32_t buffers_counter,
                uint32_t *consume_len, uint32_t *package_len,void *user_data); /**<  To do unpackage, *consume_len and *package_len are the parameters out.*/
    void *user_data;    /**<  A user data.*/
} mux_protocol_t;

/** @brief
 * This structure indicates current available data received when
 * using #MUX_CMD_GET_RX_AVAIL.
 */
typedef struct {
    uint32_t ret_size;               /**< The size of the received data. */
} mux_get_rx_avail_t;

/** @brief
 * This structure indicates current available space to put the transmit data in when
 * using #MUX_CMD_GET_TX_AVAIL.
 */
typedef struct {
    uint32_t ret_size;               /**< The current available space to put the transmit data in. */
} mux_get_tx_avail_t;


/** @brief
 * This enum defines the commands for each parameter in #mux_ctrl_para_t.
 */
typedef enum {
    MUX_CMD_GET_TX_AVAIL,               /**< UART or USB or BT special command to get the current available space to store the transmit data. */
    MUX_CMD_GET_RX_AVAIL,               /**< UART or USB or BT special command to get the current available size for the received data. */
    MUX_CMD_GET_TX_BUFFER_STATUS,       /**< UART or USB or BT special command to get the current available space to store the transmit data. */
    MUX_CMD_GET_RX_BUFFER_STATUS,       /**< UART or USB or BT special command to get the current available size for the received data. */
    MUX_CMD_CONNECT,                    /**< special command to do connect. */
    MUX_CMD_DISCONNECT,                 /**< special command to do disconnect. */
    MUX_CMD_GET_CONNECTION_PARAM,       /**< iAP2 special command to get connection parameters. */
    MUX_CMD_SET_RX_PARAM ,              /**< iAP2 special command to set rx paramete. */
    MUX_CMD_CLEAN,                      /**< special command to clean tx/rx buffer data. */
    MUX_CMD_GET_VIRTUAL_TX_AVAIL_LEN,   /**< special command to get tx buffer avail data length. */
    MUX_CMD_GET_VIRTUAL_RX_AVAIL_LEN,   /**< special command to get rx buffer avail data length. */
    MUX_CMD_CLEAN_TX_VIRUTUAL,          /**< special command to clean tx buffer data. */
    MUX_CMD_CLEAN_RX_VIRUTUAL,          /**< special command to clean rx buffer data. */
    MUX_CMD_TX_BUFFER,                /**< special command to send tx  buffer data. */
    MUX_CMD_MAX,                        /**< The maximum number of commands. */
} mux_ctrl_cmd_t;

/** @brief
 * This structure defines the data structure for each command in
 * #mux_ctrl_cmd_t.
 */
typedef union {
    mux_get_tx_avail_t mux_get_tx_avail;      /**< Parameters for port tx buffer free space.  */
    mux_get_rx_avail_t mux_get_rx_avail;      /**< Parameters for port rx buffer free space.  */
} mux_ctrl_para_t;


/**
 * @}
 */

#include "mux_port_device.h"

/** @brief This function is mux port initialize.
 * @param[in] port is the mux port.
 * @param[in] setting is mux port setting.
 * @param[in] protocol_callback is protocol callback function when mux to do send and receive data.
 * @return
 *         MUX_STATUS_OK, MUX port initialize success.
 * @code
 * @endcode
*/
mux_status_t mux_init(mux_port_t port, mux_port_setting_t *setting, mux_protocol_t *protocol_callback);


/** @brief This function is mux port deinitialize.
 * @param[in] port is the mux port.
 * @return
 *         MUX_STATUS_OK, MUX port deinitialize success.
 * @code
 * @endcode
*/
mux_status_t mux_deinit(mux_port_t port);


/** @brief This function is logging port exception initialize, special for mux port as logging port.
 * @param[in] handle is the mux logging user handle.
 * @return
 *         MUX_STATUS_OK, MUX logging port exception initialize success.
 * @code
 * @endcode
*/
mux_status_t mux_exception_init(mux_handle_t handle);


/** @brief This function is logging port send exception data, special for mux port as logging port.
 * @param[in] handle is the mux logging user handle.
 * @param[in] buffers is buffer list want to send.
  * @param[in] buffers_counter is buffer count.
 * @return
 *         MUX_STATUS_OK, MUX exception send data success.
 * @code
 * @endcode
*/
mux_status_t mux_exception_send(mux_handle_t handle, const mux_buffer_t *buffers,uint32_t buffers_counter);


/** @brief This function is mux port open by user.
 * @param[in] port is the mux port.
 * @param[in] user_name is the user name.
 * @param[out] p_handle is handle, user can get the handle by mux_open.
 * @param[in] callback is the callback function for ready to read and read to write.
 * @param[in] user_data the user data, as a parameter of callback.
 * @return
 *         MUX_STATUS_OK, MUX port open success.
 * @code
 * @endcode
*/
mux_status_t mux_open(mux_port_t port, const char *user_name, mux_handle_t *p_handle,mux_callback_t callback,void*user_data);


/** @brief This function is mux port close by user.
 * @param[in] handle is the user handle.
 * @return
 *         MUX_STATUS_OK, MUX port close success.
 * @code
 * @endcode
*/
mux_status_t mux_close(mux_handle_t handle);


/** @brief This function is mux receive data.
 * @param[in] handle is the mux user handle.
 * @param[in] buffer is receive buffer.
 * @param[out] receive_done_data_len is how many data received.
 * @return
 *         MUX_STATUS_OK, MUX receive data success.
 * @code
 * @endcode
*/
mux_status_t mux_rx(mux_handle_t handle, mux_buffer_t *buffer, uint32_t *receive_done_data_len);


/** @brief This function is query the user's port setting.
 * @param[in] user_name is the user open name.
 * @param[out] setting is the user's port setting.
 * @return
 *         MUX_STATUS_OK, Query user name success.
 * @code
 * @endcode
*/
mux_status_t mux_query_user_port_setting(const char *user_name,mux_port_config_t *setting);

/** @brief This function is query the user's name.
 * @param[in] handle is the mux user handle.
 * @param[out] user_name is the user's name.
 * @return
 *         MUX_STATUS_OK, Query user name success.
 * @code
 * @endcode
*/
mux_status_t mux_query_user_name(mux_handle_t handle, const char **user_name);


/** @brief This function is query the user's handle.
 * @param[in] port is the mux port.
 * @param[in] user_name is the user's user_name.
 * @param[out] p_handle is the user's handle.
 * @return
 *         MUX_STATUS_OK, Query user handle success.
 * @code
 * @endcode
*/
mux_status_t mux_query_user_handle(mux_port_t port, const char *user_name, mux_handle_t *p_handle);

/** @brief This function is mux send data.
 * @param[in] handle is the mux user handle.
 * @param[in] buffers[] is send buffer list.
 * @param[in] buffers_counter is the count of buffer list which want to send.
 * @param[out] send_done_data_len is how many data sent.
 * @return
 *         MUX_STATUS_OK, MUX send success.
 * @code
 * @endcode
*/
mux_status_t mux_tx(mux_handle_t handle, const mux_buffer_t buffers[],uint32_t buffers_counter,uint32_t *send_done_data_len);


/** @brief This function is mux control operate.
 * @param[in] port is the mux port.
 * @param[in] command is the command want to do.
 * @param[out] para is the parameter which want to return from mux_control.
 * @return
 *         MUX_STATUS_OK, MUX operate success.
 * @code
 * @endcode
*/
mux_status_t mux_control(mux_port_t port, mux_ctrl_cmd_t command, mux_ctrl_para_t *para);


/** @brief This function saves the user's port information to NVDM.
 * @param[in] port is the mux user port.
 * @param[in] user_name is the user's name.
 * @return
 *         MUX_STATUS_OK, MUX save user port success.
 * @code
 * @endcode
*/
mux_status_t mux_open_save_to_nvdm(mux_port_t port, const char *user_name);


/** @brief This function deletes the user's port information from NVDM.
 * @param[in] port is the mux user port.
 * @param[in] user_name is the user's name.
 * @return
 *         MUX_STATUS_OK, MUX delete user port success.
 * @code
 * @endcode
*/
mux_status_t mux_close_delete_from_nvdm(mux_port_t port, const char *user_name);


/** @brief This function reads the port setting from NVDM.
 * @param[in] port is the mux user port.
 * @param[out] setting is the port's configuration.
 * @return
 *         MUX_STATUS_OK, MUX query port setting success.
 * @code
 * @endcode
*/
mux_status_t mux_query_port_setting_from_nvdm(mux_port_t port, mux_port_setting_t *setting);


/** @brief This function save the port setting to NVDM.
 * @param[in] port is the mux user port.
 * @param[in] setting is the port's configuration.
 * @return
 *         MUX_STATUS_OK, MUX save port setting success.
 * @code
 * @endcode
*/
mux_status_t mux_save_port_setting_to_nvdm(mux_port_t port, mux_port_setting_t *setting);


/** @brief This function queries how many ports the user had opened from NVDM.
 * @param[in] user_name is the user's name.
 * @param[out] port_count is the used port quantity.
 * @return
 *         MUX_STATUS_OK, MUX query parameter success.
 * @code
 * @endcode
*/
mux_status_t mux_query_user_port_numbers_from_nvdm(const char *user_name, uint32_t *port_count);


/** @brief This function query the used port index by the user from NVDM.
 * @param[in] user_name is the user's name.
 * @param[out] port is a point, it returns the used ports index.
 * @return
 *         MUX_STATUS_OK, MUX query parameter success.
 * @code
 * @endcode
*/
mux_status_t mux_query_port_numbers_from_nvdm(const char *user_name, mux_port_buffer_t *port);

/**
* @}
* @}
*/


#ifdef __cplusplus
}
#endif


#endif//__MUX_H__

