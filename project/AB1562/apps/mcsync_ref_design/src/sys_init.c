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

#include <stdint.h>
#include <stdio.h>
#include <xtensa/hal.h>
#include <xtensa/xtruntime.h>
#include "hal.h"
#include "hal_cache.h"
#include "hal_gpio_config.h"
#include "syslog.h"
#include "serial_port_assignment.h"
#include "nvkey.h"
#include "nvkey_list.h"
#include "system.h"
#include "FreeRTOS.h"
#include "string.h"
#include "hal_pmu_charger_2562.h"
#include "hal_pmu_auxadc_2562.h"
#include "hal_core_status.h"
#include "task.h"
#include "drv_battery.h"
#include "drv_charger.h"
#include "drv_keypad.h"
#include "afe_ctl.h"
#include "pwr_mgr.h"
#include "dcxo_capid.h"
#include "exception_handler.h"
#ifdef MTK_BOOTREASON_CHECK_ENABLE
#include "bootreason_check.h"
#endif /* MTK_BOOTREASON_CHECK_ENABLE */
#ifdef MTK_SYSTEM_HANG_TRACER_ENABLE
#include "systemhang_tracer.h"
#endif /* MTK_SYSTEM_HANG_TRACER_ENABLE */
#include "ntc.h"

log_create_module(SYS_INIT, PRINT_LEVEL_INFO);

#define DBG_LOG_SYS_INIT(_message, arg_cnt, ...)      LOG_MSGID_I(SYS_INIT, _message, arg_cnt, ##__VA_ARGS__)

/**
* @brief       This function is to initialize cache controller.
* @param[in]   None.
* @return      None.
*/
static void cache_init(void)
{
    hal_cache_region_t region, region_number;

    /* Max region number is 8 */
    hal_cache_region_config_t region_cfg_tbl[] = {
        /* TODO: use Macro in feature */
        {0x00000000, 0x20000000},
        {0x20000000, 0x20000000}

    };

    region_number = (hal_cache_region_t)(sizeof(region_cfg_tbl) / sizeof(region_cfg_tbl[0]));

    if (HAL_CACHE_STATUS_OK != hal_cache_init()) {
        DBG_LOG_SYS_INIT("Cache Init Failed\r\n", 0);
        return;
    }

    if (HAL_CACHE_STATUS_OK != hal_cache_set_size(HAL_CACHE_SIZE_16KB)) {
        DBG_LOG_SYS_INIT("Cache Set Size Failed\r\n", 0);
        return;
    }

    for (region = HAL_CACHE_REGION_0; region < region_number; region++) {
        if (HAL_CACHE_STATUS_OK == hal_cache_region_config(region, &region_cfg_tbl[region])) {
            if (HAL_CACHE_STATUS_OK != hal_cache_region_enable(region)) {
                DBG_LOG_SYS_INIT("Cache Region%d Enable Failed\r\n", 1, region);
            }
        } else {
            DBG_LOG_SYS_INIT("Cache Region%d Config Failed\r\n", 1, region);
        }
    }

    if (HAL_CACHE_STATUS_OK != hal_cache_enable()) {
        DBG_LOG_SYS_INIT("Cache Enable Failed\r\n", 0);
        return;
    }
}

/**
* @brief       This function is to do system initialization, eg: system clock, hardware and logging port.
* @param[in]   None.
* @return      None.
*/

EXTERN VOID OS_DPC_init(VOID);
extern bool log_port_init(void);
extern void NVKEY_Init(void);
extern void DRV_PRBS_Init(uint32_t Seed);
EXTERN void HCI_XPORT_Init(hal_uart_port_t port, hal_uart_baudrate_t baudrate);
EXTERN const uint8_t * port_IntStack;
EXTERN void otp_calibration();
EXTERN void mpk_init(void);
EXTERN void hal_dcxo_init(void);
EXTERN void SYS_checkBootStatus(void);
extern exception_config_mode_t exception_dump_mode;
void kernelInit(void);

#define SYS_CONTROLLER_MODE            1
#define SYS_DEVICE_MODE                0

U8 GetSysMode(void)
{
    return  *((U8 *)NVKEY_GetPayloadFlashAddress(NVKEYID_BT_CON_FORCE_CONTROLLER));
}
void kernelInit(void);
VOID BT_Controller_Init(VOID);
VOID BT_Controller_Cfg(VOID);



void check_force_bt_controller_mode(void)
{
    EXTERN void log_global_turn_off(void);
    if(GetSysMode() != SYS_DEVICE_MODE)
    {
        log_global_turn_off();

        BT_Controller_Init();

        BT_Controller_Cfg();

        hal_core_status_write(HAL_CORE_DSP,HAL_CORE_ACTIVE);
<<<<<<< HEAD
        
=======

>>>>>>> db20e11 (second commit)
        hal_uart_disable_flowcontrol(HAL_UART_0);

        vTaskStartScheduler();
        /* If all is well, the scheduler will now be running, and the following line
        will never be reached.  If the following line does execute, then there was
        insufficient FreeRTOS heap memory available for the idle and/or timer tasks
        to be created.  See the memory management section on the FreeRTOS web site
        for more details. */
        for (;;);

    }
}

#define MTK_SYSTEM_CLOCK_208M
/* Set system clock to 26M Hz */
#ifdef MTK_SYSTEM_CLOCK_104M
static const uint32_t target_freq = 104000;
#elif defined (MTK_SYSTEM_CLOCK_208M)
static const uint32_t target_freq = 208000;
/* Set system clock to 208M Hz */
#elif defined (MTK_SYSTEM_CLOCK_83M)
static const uint32_t target_freq = 83000;
/* Set system clock to 83M Hz */
#elif defined (MTK_SYSTEM_CLOCK_26M)
static const uint32_t target_freq = 26000;
#endif

/**
* @brief       This function is to config system clock.
* @param[in]   None.
* @return      None.
*/
static void SystemClock_Config(void)
{
    hal_clock_init();
#ifdef HAL_DVFS_MODULE_ENABLED
    hal_dvfs_init();
    hal_dvfs_target_cpu_frequency(target_freq, HAL_DVFS_FREQ_RELATION_H);
#endif
}
extern void hal_rtc_register_dump();

void system_init(void)
{
    syslog_port_type_t syslog_type;
    uint32_t           syslog_idx;

    log_uart_init(CONFIG_SYSLOG_INIT_STAGE_PORT, CONFIG_SYSLOG_BAUDRATE);

    cache_init();
    hal_nvic_init();
    SystemClock_Config();
    hal_flash_init();
    NVKEY_Init();
    gpio_config_all();

    dcxo_load_capid();
    hal_dcxo_init();

    log_port_init();
    log_init(syslog_type, syslog_idx);
    log_set_filter();

    kernelInit();

    hal_dwt_init();
    hal_dwt_request_watchpoint(HAL_DWT_1, 0x0, 0x6, WDE_DATA_RW);

    hal_rtc_init();
    rtc_gpio_config_all();//rtc gpio config
    DBG_LOG_SYS_INIT("nvkey initialize rtc gpio done.\r\n", 0);
    hal_rtc_register_dump();

    exception_dump_config_init();

    OS_DPC_init();

    /* Prbs initialization */
    DRV_PRBS_Init(0x12345678);

    check_force_bt_controller_mode();

    memset(&port_IntStack, 0xa5, configISR_STACK_SIZE);

    hal_sleep_manager_init();

<<<<<<< HEAD
    pmu_get_power_on_reason();
    pmu_get_power_off_reason();

=======
>>>>>>> db20e11 (second commit)
    SYS_GET_CHG_CONFIG_from_NVKEY();
    SYS_Get_CHG_Case_Type_from_NVKEY();

    /* PMU */
    pmu_init();

    AFE_Init_from_NVKEY_CSR();
    AFE_Init_from_NVKEY_Reg_I2C();

    otp_calibration();
    NTC_StartStop_Init();
    mpk_init();

<<<<<<< HEAD
    DRV_BAT_Init();
=======
    pmu_bat_init();
>>>>>>> db20e11 (second commit)
    pmu_chg_init();
    pmu_auxadc_init();

    /* Init SYSRAM */
    //memset((VOID *)0x04200000,0,8*1024);

    /* init boot status */
    SYS_checkBootStatus();

    /* init power level */
    U8 PowerLv;
    U8 PwrLvstatus;

    gSysBootCtl.AdaptivePowerLv = SYS_POWER_MODE_SLEEP;
    PwrLvstatus = NVKEY_ReadFullKey(NVKEYID_PERIPHERAL_SYS_POWER_LEVEL,&PowerLv, 0x01);
    if (PwrLvstatus == 1)
        gSysBootCtl.ConstrainedPowerLv = PowerLv;
    DBG_LOG_SYS_INIT("Power Level = %d.\r\n", 1, PowerLv);

    DRV_CHARGER_ChkChgState();
    pmu_eint_clean_proc();

    AFE_Patch_from_NVKEY_CSR();
    AFE_Patch_from_NVKEY_Reg_I2C();

    hal_time_check_enable();

    if (exception_dump_mode.exception_mode_t.mask_irq_check_assert) {
        hal_time_check_enable();
    }
<<<<<<< HEAD
	
=======

>>>>>>> db20e11 (second commit)
    DBG_LOG_SYS_INIT("\r\n[DSP0]system initialize done.\r\n", 0);

#ifdef MTK_BOOTREASON_CHECK_ENABLE
    bootreason_init();
#endif /* MTK_BOOTREASON_CHECK_ENABLE */

#ifdef MTK_SYSTEM_HANG_TRACER_ENABLE
    /* system hang tracer service init */
<<<<<<< HEAD
    systemhang_tracer_init(60);
=======
    systemhang_tracer_init(30);
>>>>>>> db20e11 (second commit)
#endif /* MTK_SYSTEM_HANG_TRACER_ENABLE */
}
