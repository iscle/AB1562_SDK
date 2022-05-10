/* Copyright Statement:
 *
 * (C) 2018  Airoha Technology Corp. All rights reserved.
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

#include "FreeRTOS.h"
#include "timers.h"

#include "log.h"
#include "hal.h"
#include "hal_uart_internal.h"
#include "hal_eint.h"
#include "hal_gpio.h"
#include "hal_sleep_manager_platform.h"
#include "memory_attribute.h"
#include "drv_smartcharger.h"
#include "hal_pmu_auxadc_2562.h"
#include "drv_charger.h"
#include "drv_battery.h"
#include "nvkey_list.h"
#include "nvkey.h"
#include "BtMCSync.h"
#include "DSP_SDK.h"
#include "hal_gpio_config.h"
#include "stream.h"

#define BIT_FIELD_CLEAR16(r,p,l)    (((U16)(r)) & ~((((U16)1 << (l)) - 1) << (p)))
#define BIT_FIELD_CLEAR32(r,p,l)    (((U32)(r)) & ~((((U32)1 << (l)) - 1) << (p)))
#define BIT_FIELD_EXTRACT8(r,p,l)   (((U8)(r)  >> (p)) & (((U8)1  << (l)) - 1))
#define BIT_FIELD_EXTRACT16(r,p,l)  (((U16)(r) >> (p)) & (((U16)1 << (l)) - 1))
#define BIT_FIELD_EXTRACT32(r,p,l)  (((U32)(r) >> (p)) & (((U32)1 << (l)) - 1))
#define BIT_FIELD_INSERT8(r,p,l,v)  (BIT_FIELD_CLEAR8((r),(p),(l))  | ((U8)(v)  << (p)))
#define BIT_FIELD_INSERT16(r,p,l,v) (BIT_FIELD_CLEAR16((r),(p),(l)) | ((U16)(v) << (p)))
#define BIT_FIELD_INSERT32(r,p,l,v) (BIT_FIELD_CLEAR32((r),(p),(l)) | ((U32)(v) << (p)))

extern bool kernel_task_send_1wire_race_msg(U32 enable);

////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define SMART_CHARGER_1_WIRE_UART_BUFFER_SIZE 32
static uint8_t oneWire_tx[SMART_CHARGER_1_WIRE_UART_BUFFER_SIZE];
static uint8_t oneWire_rx[SMART_CHARGER_1_WIRE_UART_BUFFER_SIZE];

#define CHARGER_COMM_MODE_GPIO HAL_GPIO_1
//#define CHARGER_COMM_MODE_GPIO HAL_GPIO_2

extern TimerHandle_t pSmartChargerTimer;
extern TimerHandle_t pSmartChargerOutTimer;
extern U8       smartChargerIntr;
extern Handler  gMSG_SmartChargerHandler;
extern U8       chargerInitState;

TimerHandle_t pSmartCharger1Wire;

uint8_t LOCK_DVFS_SLEEP;
uint8_t isCommMode;
uint8_t oneWireChannel;
uint8_t isRaceMode;
uint8_t is1WireLoggingMode;
uint8_t isLogging[5];

static uint8_t raceEvt[]= {
    0x05,      //race header
    0x5B,
    0x05,      //lengthL
    0x00,      //lengthH
    0x00,      //cmd type
    0x20,      //cmd tpye
    0x00,      //event
    0x00,      //R or L
    0x00,      //parameter
};

#define RACE_TYPE       1
#define LENGTH_L_INDEX  2
#define LENGTH_H_INDEX  3
#define EVENT_INDEX     6
#define EARBUD_INDEX    7
#define PARAMETER_INDEX 8
#define KEY_INDEX       8

#define CASE_LID_OPEN      2
#define CASE_LID_CLOSE     3
#define CASE_CHARGER_OFF   4
#define CASE_CHARGER_KEY   5
#define CASE_BATTER_LEVEL  6
#define CASE_CHARGER_STATE 7
#define CASE_USER1         8
#define CASE_BD_ADDRESS    9
#define CASE_RACE_ENABLE   0xF

////////////////////////////////////////////////////////////////////////////////
//  Declarations ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
static void DRV_SmartChargerCase_1wire_Uart_Cb(hal_uart_callback_event_t status, void *user_data)
{
    UNUSED(user_data);

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (status == HAL_UART_EVENT_READY_TO_READ)
    {
        if( xTimerStartFromISR(pSmartChargerTimer, &xHigherPriorityTaskWoken ) != pdPASS )
        {
            LOG_COMMON_MSGID_INFO("[SmartCharger][1Wire]timer start from isr fail", 0);
        }
    }

    if (status == HAL_UART_EVENT_WAKEUP_SLEEP)
    {
    }

    if (xHigherPriorityTaskWoken != pdFALSE) {
        // Actual macro used here is port specific.
        portYIELD_FROM_ISR(pdTRUE);
    }
}

static const uint8_t bootevet[]= {
    0x05,      //race header
    0x5D,
    0x05,      //lengthL
    0x00,      //lengthH
    0x00,      //cmd type
    0x20,      //cmd tpye
    0x00,      //event
    0x00,      //R or L
    0x00,      //parameter
};

U8 bootFlag;
<<<<<<< HEAD
=======
extern bool one_wire_disable_rx_irq_flag;
>>>>>>> db20e11 (second commit)

static void DRV_SmartCharger_1wire_Uart_Init( void )
{
    U16 rc;
    U8 UartConfigBaudrate[2];

    rc = NVKEY_ReadFullKey(NVKEYID_PERIPHERAL_DRV_UART_PARA, UartConfigBaudrate, 2);

    /* init 1 wire uart */
    hal_uart_deinit(HAL_UART_1);

    hal_uart_config_t uart_config;
    hal_uart_status_t status;
    hal_uart_dma_config_t dma_config;
    if(UartConfigBaudrate[1] > HAL_UART_BAUDRATE_115200)
    {
        uart_config.baudrate = HAL_UART_BAUDRATE_115200;
    }
    else
    {
        uart_config.baudrate = UartConfigBaudrate[1];
    }
    LOG_COMMON_MSGID_INFO("[SmartCharger][1Wire]uart init UART baudrate[%d]", 1,uart_config.baudrate);
    uart_config.parity = HAL_UART_PARITY_NONE;
    uart_config.stop_bit = HAL_UART_STOP_BIT_1;
    uart_config.word_length = HAL_UART_WORD_LENGTH_8;
    status = hal_uart_init(HAL_UART_1, &uart_config);

    if(bootFlag == 0)
    {
        hal_gpio_set_driving_current(HAL_GPIO_7 , HAL_GPIO_DRIVING_CURRENT_12MA);
        hal_pinmux_set_function(HAL_GPIO_7, HAL_GPIO_7_UART1_TXD);

        uart_send_polling(HAL_UART_1 , bootevet, sizeof(bootevet));
        hal_gpt_delay_us(500);
        hal_gpio_set_driving_current(HAL_GPIO_7 , HAL_GPIO_DRIVING_CURRENT_4MA);
        hal_pinmux_set_function(HAL_GPIO_7, HAL_GPIO_7_GPIO7);

        bootFlag = 1;
    }

    if(status != HAL_UART_STATUS_OK)
    {
        LOG_COMMON_MSGID_INFO("[SmartCharger][1Wire]uart init fail status[%d]", 1,status);
    }

    dma_config.receive_vfifo_alert_size = 8;
    dma_config.receive_vfifo_buffer = oneWire_rx;
    dma_config.receive_vfifo_buffer_size = 32;
    dma_config.receive_vfifo_threshold_size = 5;
    dma_config.send_vfifo_buffer = oneWire_tx;
    dma_config.send_vfifo_buffer_size = 32;
    dma_config.send_vfifo_threshold_size = 5;
    status = hal_uart_set_dma(HAL_UART_1, (const hal_uart_dma_config_t *)&dma_config);
<<<<<<< HEAD

=======
    //one_wire_disable_rx_irq_flag = true;
>>>>>>> db20e11 (second commit)
    if(status != HAL_UART_STATUS_OK)
    {
        LOG_COMMON_MSGID_INFO("[SmartCharger][1Wire]uart set dma fail status[%d]", 1,status);
    }

    hal_uart_register_callback(HAL_UART_1, DRV_SmartChargerCase_1wire_Uart_Cb, NULL);
}


static void DRV_SmartChargerCase_Enter_Race_Mode(hal_uart_baudrate_t baudrate)
{
    LOG_COMMON_MSGID_INFO("[SmartCharger][1Wire] Enable Race", 0);
    xTimerChangePeriod(pSmartChargerOutTimer, pdMS_TO_TICKS(1000), 0);

    hal_uart_deinit(HAL_UART_1);

    /* init 1 wire uart */
    hal_uart_config_t uart_config;
    hal_uart_status_t status;

    uart_config.baudrate = baudrate;
    uart_config.parity = HAL_UART_PARITY_NONE;
    uart_config.stop_bit = HAL_UART_STOP_BIT_1;
    uart_config.word_length = HAL_UART_WORD_LENGTH_8;
    status = hal_uart_init(HAL_UART_1, &uart_config);

    LOG_COMMON_MSGID_INFO("[SmartCharger][1Wire]uart init UART status[%d]", 1,status);
    kernel_task_send_1wire_race_msg(1);
}

static void DRV_SmartChargerCase_Leave_Race_Mode(void)
{
    LOG_COMMON_MSGID_INFO("[SmartCharger][1Wire] leave race mode", 0);

    xTimerChangePeriod(pSmartChargerOutTimer, pdMS_TO_TICKS(150), 0);

    /* re init uart */
    DRV_SmartCharger_1wire_Uart_Init();
    kernel_task_send_1wire_race_msg(0);
}

static void DRV_SmartChargerCase_Chg_Mode(void)
{
    LOG_COMMON_MSGID_INFO("[SmartCharger][1Wire]charger mode\r\n",0);

    if(xTimerStop(pSmartChargerOutTimer, 0) == pdFAIL)
    {
        LOG_COMMON_MSGID_INFO("[SmartCharger][1Wire]cancel charger timeout fail\r\n",0);
    }

    hal_pinmux_set_function(HAL_GPIO_7 , HAL_GPIO_7_GPIO7);

    if(LOCK_DVFS_SLEEP == true)
    {
#ifdef HAL_SLEEP_MANAGER_ENABLED
        hal_sleep_manager_unlock_sleep(SLEEP_LOCK_CHARGER_CASE);
#endif
        LOCK_DVFS_SLEEP = false;
    }
}

static void DRV_SmartChargerCase_Comm_Mode(void)
{
    isCommMode = TRUE;

    LOG_COMMON_MSGID_INFO("[SmartCharger][1Wire]comm mode\r\n",0);

    hal_pinmux_set_function(HAL_GPIO_7 , HAL_GPIO_7_UART1_RXD);

    if(LOCK_DVFS_SLEEP != true)
    {
#ifdef HAL_SLEEP_MANAGER_ENABLED
        hal_sleep_manager_lock_sleep(SLEEP_LOCK_CHARGER_CASE);
#endif
        LOCK_DVFS_SLEEP = true;
    }
}


const uint8_t patternTable[] = {0,
                                0,
                                DRV_SMARTCHARGER_EVENT_BT_ON_PATTERN,
                                DRV_SMARTCHARGER_EVENT_BT_OFF_PATTERN,
                                DRV_SMARTCHARGER_EVENT_CHARGER_COMPLETE_PATTERN,
                                DRV_SMARTCHARGER_EVENT_CHARGER_KEY_PATTERN,
                                0,
                                0,
                                DRV_SMARTCHARGER_EVENT_USER1_PATTERN
                                };


static void DRV_SmartCharger_1wire_CallBack( TimerHandle_t pxExpiredTimer )
{
    UNUSED(pxExpiredTimer);

    uint32_t rxSize;
    uint8_t  raceCmd[SMART_CHARGER_1_WIRE_UART_BUFFER_SIZE];
    uint8_t  batterLevel;

    uint32_t* parameter;
    uint32_t  length;

    rxSize = hal_uart_get_available_receive_bytes(HAL_UART_1);

    LOG_COMMON_MSGID_INFO("[SmartCharger][1Wire] Rx Size[%d]", 1,rxSize);

    if(xTimerReset(pSmartChargerOutTimer, 0) == pdFAIL)
    {
        LOG_COMMON_MSGID_INFO("[SmartCharger][1Wire]cancel charger timeout fail\r\n",0);
    }

    if(rxSize > 7)
    {
        hal_uart_receive_dma(HAL_UART_1, raceCmd, rxSize);

        LOG_COMMON_MSGID_INFO("[SmartCharger][1Wire] race rx tyte[0x%x] earbud[%d] event[%d] parameter[%d]", 4, raceCmd[RACE_TYPE]
                                                                                                        , raceCmd[EARBUD_INDEX]
                                                                                                        , raceCmd[EVENT_INDEX]
                                                                                                        , raceCmd[PARAMETER_INDEX]);

        uint32_t channel = oneWireChannel;

        raceEvt[EARBUD_INDEX] = channel;

        LOG_COMMON_MSGID_INFO("[SmartCharger][1Wire] earbud direction[%d]", 1, channel);

        if(raceCmd[0] == 0x5)
        {
            /* check cmd or event adn which earbud */
            if(raceCmd[RACE_TYPE] == 0x5A)
            {
                if(raceCmd[EVENT_INDEX])
                {
                    if(raceCmd[PARAMETER_INDEX] == 100)
                    {
                        batterLevel = 31;
                    }
                    else if(raceCmd[PARAMETER_INDEX] == 0)
                    {
                        batterLevel = 0;
                    }
                    else
                    {
                        batterLevel = 31 - 31*(100 - raceCmd[PARAMETER_INDEX])/100;
                    }


                    if(raceCmd[EVENT_INDEX] < CASE_BD_ADDRESS)
                    {
                        /* send charger complete to app only if channel is the same*/
                        if((raceCmd[EVENT_INDEX] != CASE_CHARGER_OFF && raceCmd[EVENT_INDEX] != CASE_CHARGER_KEY)
                          ||raceCmd[EARBUD_INDEX] == channel)
                        {
                            MSG_MessageSendEx(gMSG_SmartChargerHandler, patternTable[raceCmd[EVENT_INDEX]], NULL, batterLevel);
                        }
<<<<<<< HEAD
                        U16 tmp = DRV_BAT_GetThousandPercent()/10;
=======
                        U16 tmp = pmu_bat_get_perc();
>>>>>>> db20e11 (second commit)
                        parameter = (uint32_t*)&tmp;
                        length    = 1;
                    }
                    else
                    {
                        if(raceCmd[EVENT_INDEX] == CASE_BD_ADDRESS)
                        {
                             BD_ADDR_T* bdAdress = BtMCSync_ReadAgentBdAddr();
                             parameter = (uint32_t*)bdAdress;
                             length    = sizeof(BD_ADDR_T);
                        }
                        else
                        {
                            parameter = NULL;
                            length    = 0;
                        }
                    }

                    if(raceCmd[EARBUD_INDEX] == channel)
                    {
                        /* return event */
                        raceEvt[LENGTH_L_INDEX]  = 4 + length;
                        raceEvt[EVENT_INDEX]     = raceCmd[EVENT_INDEX];

                        hal_pinmux_set_function(HAL_GPIO_7, HAL_GPIO_7_UART1_TXD);

                        LOG_COMMON_MSGID_INFO("[SmartCharger][1Wire] race tx tyte[0x%x] earbud[%d] event[%d]", 3, raceEvt[RACE_TYPE]
                                                                                                                , raceEvt[EARBUD_INDEX]
                                                                                                                , raceEvt[EVENT_INDEX]);


                        uart_send_polling(HAL_UART_1 , (const uint8_t *)raceEvt, 8);
                        uart_send_polling(HAL_UART_1 , (const uint8_t *)parameter, length);
                        while(uart_query_tx_empty(HAL_UART_1) == 0)
                        {
                        }

                        if(raceCmd[EVENT_INDEX] == CASE_CHARGER_OFF)
                        {
                            hal_pinmux_set_function(HAL_GPIO_7 , HAL_GPIO_7_GPIO7);
                        }

                        hal_pinmux_set_function(HAL_GPIO_7 , HAL_GPIO_7_UART1_RXD);

                        if(raceCmd[EVENT_INDEX] == CASE_RACE_ENABLE)
                        {
                            isRaceMode = TRUE;
                            DRV_SmartChargerCase_Enter_Race_Mode(raceCmd[PARAMETER_INDEX]);
                        }
                    }
                }
            }
        }
        else
        {
            /* just drop if not race */
        }
    }
    else
    {
        /* just drop */
        hal_uart_receive_dma(HAL_UART_1, raceCmd, rxSize);
    }
}


static void DRV_SmartCharger_1wire_CallBack_Out( TimerHandle_t pxExpiredTimer )
{
    UNUSED(pxExpiredTimer);

    uint32_t data;
    uint32_t pmuData;
    uint32_t rxLength;
    uint32_t isTxBusy;
    uint32_t isRxBusy;

    rxLength = hal_uart_get_available_receive_bytes(HAL_UART_1);
    isTxBusy = uart_query_rx_empty(HAL_UART_1);
    isRxBusy = uart_query_tx_empty(HAL_UART_1);

    if(rxLength && !isTxBusy && !isRxBusy)
    {
        LOG_COMMON_MSGID_INFO("[SmartCharger][1Wire]rxLength[%d] isTxBusy[%d] isRxBusy[%d]\r\n"
                                ,3, data, rxLength, isTxBusy, isRxBusy);
        return;
    }

    data = pmu_auxadc_get_channel_value(PMU_AUX_VCHG);

    rxLength = hal_uart_get_available_receive_bytes(HAL_UART_1);
    isTxBusy = uart_query_rx_empty(HAL_UART_1);
    isRxBusy = uart_query_tx_empty(HAL_UART_1);

    if(rxLength && !isTxBusy && !isRxBusy)
    {
        LOG_COMMON_MSGID_INFO("[SmartCharger][1Wire]polling adc[%d] rxLength[%d] isTxBusy[%d] isRxBusy[%d]\r\n"
                                ,4, data, rxLength, isTxBusy, isRxBusy);
        return;
    }

    LOG_COMMON_MSGID_INFO("[SmartCharger][1Wire]polling adc[%d]\r\n",1, data);

    if(data < 140 && !rxLength)
    {
        if(isRaceMode)
        {
            DRV_SmartChargerCase_Leave_Race_Mode();
            isRaceMode = FALSE;
        }

        /* real charger out */
        DRV_SmartChargerCase_Chg_Mode();
        hal_gpio_set_direction(HAL_GPIO_7 , HAL_GPIO_DIRECTION_INPUT);
        hal_gpio_disable_pull(HAL_GPIO_7);

        /* Dsiable VIO18 pullup */
<<<<<<< HEAD
        pmuData  = pmu_get_register_value_2byte(0x2, 0xFFFF, 0);
        pmuData  = BIT_FIELD_INSERT32(pmuData, 3, 1, 1);
        pmu_force_set_register_value_2byte(0x2, pmuData);
=======
        pmuData  = pmu_get_register_value(0x2, 0xFFFF, 0);
        pmuData  = BIT_FIELD_INSERT32(pmuData, 3, 1, 1);
        pmu_force_set_register_value(0x2, pmuData);
>>>>>>> db20e11 (second commit)

        hal_pinmux_set_function(HAL_GPIO_7 , HAL_GPIO_7_GPIO7);
        hal_gpio_set_direction(HAL_GPIO_7 , HAL_GPIO_DIRECTION_INPUT);
        hal_gpio_disable_pull(HAL_GPIO_7);

        /* VBUS discharge on */
<<<<<<< HEAD
        pmuData  = pmu_get_register_value_2byte(0x32A, 0xFFFF, 0);
        pmuData = BIT_FIELD_INSERT16(pmuData, 11, 1, 1);
        pmu_force_set_register_value_2byte(0x32A, pmuData);

        /* EOC */
        pmuData  = pmu_get_register_value_2byte(0x1E, 0xFFFF, 0);
        pmuData = BIT_FIELD_INSERT16(pmuData, 5, 3, 1);
        pmu_force_set_register_value_2byte(0x1E, pmuData);

#if 0
        pmuData  = pmu_get_register_value_2byte(0x320, 0xFFFF, 0);
        pmuData = BIT_FIELD_INSERT16(pmuData, 12, 1, 1);
        pmuData &= 0x1FAA;
        pmu_force_set_register_value_2byte(0x320, pmuData);
#else
        pmu_eoc_ctrl(1, 1);
#endif

        /* PSW_MAIN_CL = ON PSW_MAIN = OFF */
        pmuData  = pmu_get_register_value_2byte(0x2, 0xFFFF, 0);
        pmuData  = BIT_FIELD_INSERT32(pmuData, 2, 1, 1);
        pmuData  = BIT_FIELD_INSERT32(pmuData, 0, 1, 1);
        pmu_force_set_register_value_2byte(0x2, pmuData);

        /* VBUS discharge off */
        pmuData  = pmu_get_register_value_2byte(0x32A, 0xFFFF, 0);
        pmuData = BIT_FIELD_INSERT16(pmuData, 11, 1, 0);
        pmu_force_set_register_value_2byte(0x32A, pmuData);
=======
        pmuData  = pmu_get_register_value(0x32A, 0xFFFF, 0);
        pmuData = BIT_FIELD_INSERT16(pmuData, 11, 1, 1);
        pmu_force_set_register_value(0x32A, pmuData);

        /* EOC */
        pmuData  = pmu_get_register_value(0x1E, 0xFFFF, 0);
        pmuData = BIT_FIELD_INSERT16(pmuData, 5, 3, 1);
        pmu_force_set_register_value(0x1E, pmuData);

#if 0
        pmuData  = pmu_get_register_value(0x320, 0xFFFF, 0);
        pmuData = BIT_FIELD_INSERT16(pmuData, 12, 1, 1);
        pmuData &= 0x1FAA;
        pmu_force_set_register_value(0x320, pmuData);
#else
        uint32_t bat_volt = pmu_bat_adc_to_volt(pmu_bat_avg_adc());
        if (bat_volt > PMU_1WIRE_BAT_THRD)
            pmu_eoc_ctrl(1, 1);
        else
            PMU_MSGID_E("pmu_eoc_ctrl, bypass bat_volt[%d]", 1, bat_volt);
#endif

        /* PSW_MAIN_CL = ON PSW_MAIN = OFF */
        pmuData  = pmu_get_register_value(0x2, 0xFFFF, 0);
        pmuData  = BIT_FIELD_INSERT32(pmuData, 2, 1, 1);
        if (bat_volt > PMU_1WIRE_BAT_THRD)
        {
            pmuData  = BIT_FIELD_INSERT32(pmuData, 0, 1, 1);
            LOG_COMMON_MSGID_INFO("[SmartCharger] DRV_SmartCharger_1wire_CallBack_Out, UART power switch, bat_volt[%d]", 1, bat_volt);
        }
        pmu_force_set_register_value(0x2, pmuData);

        /* VBUS discharge off */
        pmuData  = pmu_get_register_value(0x32A, 0xFFFF, 0);
        pmuData = BIT_FIELD_INSERT16(pmuData, 11, 1, 0);
        pmu_force_set_register_value(0x32A, pmuData);
>>>>>>> db20e11 (second commit)

        isCommMode  = FALSE;
        /* real charger out */
        /* send charger out to app */
        MSG_MessageSend(gMSG_SmartChargerHandler, DRV_SMARTCHARGER_EVENT_CHARGER_OUT_PATTERN, NULL);
    }
}

void battery_management_callback_for_smart_1wire( TimerHandle_t pxExpiredTimer )
{
    UNUSED(pxExpiredTimer);

    uint32_t pmuData;
    uint32_t mask;
    uint32_t type;

    LOG_COMMON_MSGID_INFO("[SmartCharger]battery_management_callback_for_smart_1wire\r\n",0);

    hal_nvic_save_and_set_interrupt_mask(&mask);
    type = smartChargerIntr;
    smartChargerIntr = 0;
    hal_nvic_restore_interrupt_mask(mask);

    if(is1WireLoggingMode)
    {
        if(isLogging[4] != 0)
        {
            LOG_COMMON_MSGID_INFO("[1Wire]1Wire logging times[%d]", 1, isLogging[4]);

            isLogging[4]--;
            NVKEY_WriteFullKey(NVKEYID_PERIPHERAL_1WIRE_LOGGING_SETTING, isLogging, 5);
            return;
        }
        else
        {
            hal_wdt_software_reset();
        }
    }

    if(type & CHARGER_UNPLUG_INT)
    {
        /* Dsiable VIO18 pullup */
<<<<<<< HEAD
        pmuData  = pmu_get_register_value_2byte(0x2, 0xFFFF, 0);
        pmuData  = BIT_FIELD_INSERT32(pmuData, 3, 1, 1);
        pmu_force_set_register_value_2byte(0x2, pmuData);
=======
        pmuData  = pmu_get_register_value(0x2, 0xFFFF, 0);
        pmuData  = BIT_FIELD_INSERT32(pmuData, 3, 1, 1);
        pmu_force_set_register_value(0x2, pmuData);
>>>>>>> db20e11 (second commit)

        hal_pinmux_set_function(HAL_GPIO_7 , HAL_GPIO_7_GPIO7);
        hal_gpio_set_direction(HAL_GPIO_7 , HAL_GPIO_DIRECTION_INPUT);
        hal_gpio_disable_pull(HAL_GPIO_7);

        /* VBUS discharge on */
<<<<<<< HEAD
        pmuData  = pmu_get_register_value_2byte(0x32A, 0xFFFF, 0);
        pmuData = BIT_FIELD_INSERT16(pmuData, 11, 1, 1);
        pmu_force_set_register_value_2byte(0x32A, pmuData);

        /* EOC */
        pmuData  = pmu_get_register_value_2byte(0x1E, 0xFFFF, 0);
        pmuData = BIT_FIELD_INSERT16(pmuData, 5, 3, 1);
        pmu_force_set_register_value_2byte(0x1E, pmuData);

#if 0
        pmuData  = pmu_get_register_value_2byte(0x320, 0xFFFF, 0);
        pmuData = BIT_FIELD_INSERT16(pmuData, 12, 1, 1);
        pmuData &= 0x1FAA;
        pmu_force_set_register_value_2byte(0x320, pmuData);
#else
        pmu_eoc_ctrl(1, 0);
=======
        pmuData  = pmu_get_register_value(0x32A, 0xFFFF, 0);
        pmuData = BIT_FIELD_INSERT16(pmuData, 11, 1, 1);
        pmu_force_set_register_value(0x32A, pmuData);

        /* EOC */
        pmuData  = pmu_get_register_value(0x1E, 0xFFFF, 0);
        pmuData = BIT_FIELD_INSERT16(pmuData, 5, 3, 1);
        pmu_force_set_register_value(0x1E, pmuData);

#if 0
        pmuData  = pmu_get_register_value(0x320, 0xFFFF, 0);
        pmuData = BIT_FIELD_INSERT16(pmuData, 12, 1, 1);
        pmuData &= 0x1FAA;
        pmu_force_set_register_value(0x320, pmuData);
#else
        uint32_t bat_volt = pmu_bat_adc_to_volt(pmu_bat_avg_adc());
        if (bat_volt > PMU_1WIRE_BAT_THRD)
            pmu_eoc_ctrl(1, 0);
        else
            PMU_MSGID_E("pmu_eoc_ctrl, bypass bat_volt[%d]", 1, bat_volt);
>>>>>>> db20e11 (second commit)
#endif

#if 0
        hal_gpt_delay_ms(1);

        uint32_t data = pmu_auxadc_get_channel_value(PMU_AUX_VCHG);
        LOG_COMMON_MSGID_INFO("[SmartCharger][1Wire]vchg adc = %d\r\n",1, data);

        data = pmu_auxadc_get_channel_value(PMU_AUX_VCHG);
        LOG_COMMON_MSGID_INFO("[SmartCharger][1Wire]vchg adc = %d\r\n",1, data);
#endif
        /* always comm mode */
        /* PSW_MAIN = OFF , ENABLE PULL UP */
<<<<<<< HEAD
        pmuData  = pmu_get_register_value_2byte(0x2, 0xFFFF, 0);
        pmuData  = BIT_FIELD_INSERT32(pmuData, 0, 1, 1);
        pmuData  = BIT_FIELD_INSERT32(pmuData, 3, 1, 0);
        pmu_force_set_register_value_2byte(0x2, pmuData);

        /* VBUS discharge off */
        pmuData  = pmu_get_register_value_2byte(0x32A, 0xFFFF, 0);
        pmuData = BIT_FIELD_INSERT16(pmuData, 11, 1, 0);
        pmu_force_set_register_value_2byte(0x32A, pmuData);
=======
        pmuData  = pmu_get_register_value(0x2, 0xFFFF, 0);
        if (bat_volt > PMU_1WIRE_BAT_THRD)
        {
            pmuData  = BIT_FIELD_INSERT32(pmuData, 0, 1, 1);
            LOG_COMMON_MSGID_INFO("[SmartCharger] battery_management_callback_for_smart_1wire, UART power switch, bat_volt[%d]", 1, bat_volt);
        }
        else
        {
            pmuData  = BIT_FIELD_INSERT32(pmuData, 2, 1, 1);
        }
        pmuData  = BIT_FIELD_INSERT32(pmuData, 3, 1, 0);
        pmu_force_set_register_value(0x2, pmuData);

        /* VBUS discharge off */
        pmuData  = pmu_get_register_value(0x32A, 0xFFFF, 0);
        pmuData = BIT_FIELD_INSERT16(pmuData, 11, 1, 0);
        pmu_force_set_register_value(0x32A, pmuData);
>>>>>>> db20e11 (second commit)

        DRV_SmartChargerCase_Comm_Mode();
        xTimerStart(pSmartChargerOutTimer, 0);
    }
    else if (type & CHARGER_PLUG_INT)
    {
        if(isRaceMode)
        {
            DRV_SmartChargerCase_Leave_Race_Mode();
            isRaceMode = FALSE;
        }

        /* gpio7 output high*/
        hal_pinmux_set_function(HAL_GPIO_7 , HAL_GPIO_7_GPIO7);
        hal_gpio_set_direction(HAL_GPIO_7, HAL_GPIO_DIRECTION_OUTPUT);
        hal_gpio_set_output(HAL_GPIO_7, HAL_GPIO_DATA_HIGH);

<<<<<<< HEAD
        pmuData  = pmu_get_register_value_2byte(0x2, 0xFFFF, 0);
=======
        pmuData  = pmu_get_register_value(0x2, 0xFFFF, 0);
>>>>>>> db20e11 (second commit)
        if (isCommMode == FALSE)
        {
            /* Enable VIO18 pullup */
            pmuData  = BIT_FIELD_INSERT32(pmuData, 3, 1, 0);
        }
        else
        {
            /* PSW_MAON_CL ON */
            pmuData  = BIT_FIELD_INSERT32(pmuData, 2, 1, 1);
        }

        /* PSW_MAIN ON */
        pmuData  = BIT_FIELD_INSERT32(pmuData, 0, 1, 0);
<<<<<<< HEAD
        pmu_force_set_register_value_2byte(0x2, pmuData);
=======
        pmu_force_set_register_value(0x2, pmuData);
>>>>>>> db20e11 (second commit)
        hal_gpt_delay_us(100);

        /* PSW_MAIN_CL OFF */
        pmuData  = BIT_FIELD_INSERT32(pmuData, 2, 1, 0);
<<<<<<< HEAD
        pmu_force_set_register_value_2byte(0x2, pmuData);

        /* EOC = CHG*/
        pmuData  = pmu_get_register_value_2byte(0x1E, 0xFFFF, 0);
        pmuData = BIT_FIELD_INSERT16(pmuData, 5, 3, 0);
        pmu_force_set_register_value_2byte(0x1E, pmuData);

#if 0
        pmuData  = pmu_get_register_value_2byte(0x320, 0xFFFF, 0);
=======
        pmu_force_set_register_value(0x2, pmuData);

        /* EOC = CHG*/
        pmuData  = pmu_get_register_value(0x1E, 0xFFFF, 0);
        pmuData = BIT_FIELD_INSERT16(pmuData, 5, 3, 0);
        pmu_force_set_register_value(0x1E, pmuData);

#if 0
        pmuData  = pmu_get_register_value(0x320, 0xFFFF, 0);
>>>>>>> db20e11 (second commit)
        pmuData = BIT_FIELD_INSERT16(pmuData, 12, 1, 0);
        pmuData &= 0x0FAA;
        LOG_COMMON_MSGID_INFO("[SmartCharger][1Wire]dummy start\r\n",0);

        ignore1 = 1;
        ignore2 = 1;

<<<<<<< HEAD
        pmu_force_set_register_value_2byte(0x320, pmuData);
=======
        pmu_force_set_register_value(0x320, pmuData);
>>>>>>> db20e11 (second commit)
#else
        pmu_eoc_ctrl(0, 1);
#endif

        /* do not set if plug in from comm mode */
        /* VBUS discharge on */
        U8 tmpdata;
        if (isCommMode == FALSE)
        {
            tmpdata = 1;
        }
        else
        {
            tmpdata = 0;
        }

<<<<<<< HEAD
        pmuData  = pmu_get_register_value_2byte(0x32A, 0xFFFF, 0);
        pmuData = BIT_FIELD_INSERT16(pmuData, 11, 1, tmpdata);
        pmu_force_set_register_value_2byte(0x32A, pmuData);
=======
        pmuData  = pmu_get_register_value(0x32A, 0xFFFF, 0);
        pmuData = BIT_FIELD_INSERT16(pmuData, 11, 1, tmpdata);
        pmu_force_set_register_value(0x32A, pmuData);
>>>>>>> db20e11 (second commit)

        /*charger mode first when charger in */
        DRV_SmartChargerCase_Chg_Mode();
        MSG_MessageSend(gMSG_SmartChargerHandler, DRV_SMARTCHARGER_EVENT_CHARGER_IN_INTERRUPT, NULL);

        isCommMode = FALSE;
    }
    else if (type & CHARGER_COMPLETE_INT)
    {
        /* recharger/complete */
        MSG_MessageSend(gMSG_SmartChargerHandler, DRV_SMARTCHARGER_EVENT_CHARGER_COMPLETE_INTERRUPT, NULL);
    }
    else if (type & CHARGER_RECHARGE)
    {
        MSG_MessageSend(gMSG_SmartChargerHandler, DRV_SMARTCHARGER_EVENT_CHARGER_RECHARGE_INTERRUPT, NULL);
    }
}

void DRV_SmartChargerCase_1wire_Init(void)
{
    uint32_t pmuData;
    uint16_t rc;

<<<<<<< HEAD
=======
    LOG_COMMON_MSGID_INFO("[SmartCharger][1Wire]DRV_SmartChargerCase_1wire_Init", 0);

>>>>>>> db20e11 (second commit)
    isCommMode = FALSE;

    pSmartCharger1Wire      = xTimerCreate( "Smart Charger", pdMS_TO_TICKS(1),   pdFALSE, NULL, battery_management_callback_for_smart_1wire );
    pSmartChargerTimer      = xTimerCreate( "Smart Charger", pdMS_TO_TICKS(2),   pdFALSE, NULL, DRV_SmartCharger_1wire_CallBack );
    pSmartChargerOutTimer   = xTimerCreate( "Smart Charger", pdMS_TO_TICKS(150), pdTRUE , NULL, DRV_SmartCharger_1wire_CallBack_Out );

    if(!pSmartChargerOutTimer || !pSmartCharger1Wire || !pSmartChargerTimer)
    {
        LOG_COMMON_MSGID_INFO("[1Wire]Smart Charger Timer Out Create fail", 0);
    }

    /* 0xF023 get special syslog configuartion */
    rc = NVKEY_ReadFullKey(NVKEYID_PERIPHERAL_1WIRE_LOGGING_SETTING, isLogging, 5);
    if (rc != 0)
    {
        if(isLogging[4] != 0)
        {
            LOG_COMMON_MSGID_INFO("[1Wire]1Wire logging times[%d]", 1, isLogging[4]);
            isLogging[4]--;
            NVKEY_WriteFullKey(NVKEYID_PERIPHERAL_1WIRE_LOGGING_SETTING, isLogging, 5);
            is1WireLoggingMode = TRUE;
            return;
        }
    }

#if 1
    U8* channelConig = (U8*)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_AUDIO_CHANNEL_SELECTION);
    U8 setting = 0;

    switch(channelConig[0])
    {
        case 0xFF:  //default sw detect
            /* fallthrough */
        case 0:     //sw detect
            setting = channelConig[1];
            break;
        case 1:     //hw detect
            {
                U8 gpioIndex0 = hal_gpio_get_pin_num_from_gpio_var_name(MMI_ADUIO_CHANNEL_DETECT_PIN);
                hal_gpio_data_t index0Status;

                if(gpioIndex0 != 0xFF)
                {
                    if(hal_gpio_get_input(gpioIndex0, &index0Status) == 0)
                    {
                        if(index0Status)
                        {
                            setting = channelConig[2];
                        }
                        else
                        {
                            setting = channelConig[3];
                        }
                    }
                    else
                    {
                        configASSERT(FALSE);
                    }
                }
                else
                {
                    configASSERT(FALSE);
                }
            }
            break;
        default:
            configASSERT(FALSE);
    }


    if(setting == AU_DSP_CH_L)
    {
        oneWireChannel = 0;
    }
    else if(setting == AU_DSP_CH_R)
    {
        oneWireChannel = 1;
    }
    else
    {
        configASSERT(FALSE);
    }
#else
    oneWireChannel = 0;
#endif

    if(!chargerInitState)
    {
        /* PSW_MAIN_CL = ON , PSW_MAIN = OFF , DISABLE VIO PULL UP */
<<<<<<< HEAD
        pmuData  = pmu_get_register_value_2byte(0x2, 0xFFFF, 0);
        pmuData  = BIT_FIELD_INSERT32(pmuData, 2, 1, 1);
        pmuData  = BIT_FIELD_INSERT32(pmuData, 0, 1, 1);
        pmuData  = BIT_FIELD_INSERT32(pmuData, 3, 1, 1);
        pmu_force_set_register_value_2byte(0x2, pmuData);

        /* CHG_STATUS = EOC */
        pmuData  = pmu_get_register_value_2byte(0x1E, 0xFFFF, 0);
        pmuData = BIT_FIELD_INSERT16(pmuData, 5, 3, 1);
        pmu_force_set_register_value_2byte(0x1E, pmuData);

#if 0
        pmuData  = pmu_get_register_value_2byte(0x320, 0xFFFF, 0);
        pmuData = BIT_FIELD_INSERT16(pmuData, 12, 1, 1);
        pmuData &= 0x1FAA;
        pmu_force_set_register_value_2byte(0x320, pmuData);
#else
        pmu_eoc_ctrl(1, 0);
=======
        pmuData  = pmu_get_register_value(0x2, 0xFFFF, 0);
        pmuData  = BIT_FIELD_INSERT32(pmuData, 2, 1, 1);
        uint32_t bat_volt = pmu_bat_adc_to_volt(pmu_bat_avg_adc());
        if (bat_volt > PMU_1WIRE_BAT_THRD)
        {
            pmuData  = BIT_FIELD_INSERT32(pmuData, 0, 1, 1);
            LOG_COMMON_MSGID_INFO("[SmartCharger] DRV_SmartChargerCase_1wire_Init, UART power switch, bat_volt[%d]", 1, bat_volt);
        }
        pmuData  = BIT_FIELD_INSERT32(pmuData, 3, 1, 1);
        pmu_force_set_register_value(0x2, pmuData);

        /* CHG_STATUS = EOC */
        pmuData  = pmu_get_register_value(0x1E, 0xFFFF, 0);
        pmuData = BIT_FIELD_INSERT16(pmuData, 5, 3, 1);
        pmu_force_set_register_value(0x1E, pmuData);

#if 0
        pmuData  = pmu_get_register_value(0x320, 0xFFFF, 0);
        pmuData = BIT_FIELD_INSERT16(pmuData, 12, 1, 1);
        pmuData &= 0x1FAA;
        pmu_force_set_register_value(0x320, pmuData);
#else
        if (bat_volt > PMU_1WIRE_BAT_THRD)
            pmu_eoc_ctrl(1, 0);
        else
            PMU_MSGID_E("pmu_eoc_ctrl, bypass bat_volt[%d]", 1, bat_volt);
>>>>>>> db20e11 (second commit)
#endif

        /* init gpio 7*/
        hal_pinmux_set_function(HAL_GPIO_7, HAL_GPIO_7_GPIO7);
        hal_gpio_set_direction(HAL_GPIO_7, HAL_GPIO_DIRECTION_INPUT);
        hal_gpio_disable_pull(HAL_GPIO_7);

        /* VBUS discharge off */
<<<<<<< HEAD
        pmuData  = pmu_get_register_value_2byte(0x32A, 0xFFFF, 0);
        pmuData = BIT_FIELD_INSERT16(pmuData, 11, 1, 0);
        pmu_force_set_register_value_2byte(0x32A, pmuData);
=======
        pmuData  = pmu_get_register_value(0x32A, 0xFFFF, 0);
        pmuData = BIT_FIELD_INSERT16(pmuData, 11, 1, 0);
        pmu_force_set_register_value(0x32A, pmuData);
>>>>>>> db20e11 (second commit)
    }
    else
    {
        LOG_COMMON_MSGID_INFO("[SmartCharger][1Wire]no need init pmu", 0);
    }
    DRV_SmartCharger_1wire_Uart_Init();

    LOG_COMMON_MSGID_INFO("[SmartCharger][1Wire]init done", 0);
}


