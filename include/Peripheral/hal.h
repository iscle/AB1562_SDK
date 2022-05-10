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

#ifndef __HAL_H__
#define __HAL_H__

/**
 * @addtogroup HAL
 * @{
 * This section introduces the HAL driver APIs including terms and acronyms, supported features, software architecture,
 * details on how to use this driver, HAL function groups, enums, structures and functions.
 * @section HAL_Overview_1_Chapter    Terms and acronyms
 *
 * |Acronyms                         |Definition                                                           |
 * |------------------------------|------------------------------------------------------------------------|
 * |\b ADC                  | Analog-to-digital converter
 * |\b EINT                 | External interrupt
 * |\b GPIO                 | General purpose Input Output
 * |\b I2C                  | Inter-integrated circuit
 * |\b SPI                  | Serial Peripheral Interface
 * |\b PWM                  | Pulse Width Modulation
 * |\b UART                 | Universal asynchronous receiver/transmitter
 *
 * @section HAL_Overview_2_Chapter    Overview of HAL drivers
 *
 * - \b Introduction \n
 *   The HAL driver includes:\n
 *   - Drivers for the peripheral modules, such as UART and I2C.\n
 *   - Drivers for the system modules, such as CACHE, MPU and FLASH.\n
 *   - APIs of the modules.\n
 *
 *   The HAL driver has both standard and advanced features and is easy to use. It makes the upper hardware abstraction layer portable.\n
 *   The HAL layer provides a generic set of APIs to interact with the upper layers, such as application, middleware and the OS.\n
 *   The HAL driver covers rich modules, including all peripheral modules, such as UART, I2C and SPI. It also includes number of system modules, such as CACHE, MPU, FLASH and DMA.\n
 *   The HAL complies with the general naming convention and common coding style. Each module of the HAL has its distinctive folder structure along with supporting examples.
 * - \b HAL \b driver \b architecture \n
 * The HAL driver architecture is shown below:\n
 * @image html hal_overview_driver_architecture.png
 *
 * - \b File \b inclusion \b model \n
 * @image html hal_overview_file_inclusion_model.png
 *
 * - \b HAL \b configuration \n
 *  - \b hal_feature_config.h \n
 *      Every project should include hal_feature_config.h for HAL configuration.\n
 *      If certain HAL modules are not used, they can be removed from the feature options by undefining the macro, such as undefine the option of HAL_ADC_MODULE_ENABLED.\n
 * \n
 * \n
 * @section HAL_Overview_3_Chapter    HAL Driver Model
 * Some of the drivers can operate in two modes: polling and interrupt.
 * The UART HAL driver, for example, can operate in polling and interrupt modes during data communication. The polling mode is suitable for read and write operations of small amount of data in low frequency. The interrupt mode is suitable for read and write operations of small amount of data in high frequency. In UART DMA mode, for example, an interrupt is triggered once the DMA is complete.
 * - \b Polling \b mode \b architecture \n
 *   Most of the driver modules support the polling mode, which is the basic feature in HAL.\n
 *   The polling mode driver architecture is shown below:
 *   @image html hal_overview_driver_polling_architecture.png
 *
 * - \b Interrupt \b mode \b architecture \n
 *   To improve the portability, the HAL driver hides the OS dependence by preventing the upper layer to call the OS level services directly. The HAL drivers uses an interrupt handler to provide the upper layer with a callback mechanism.
 *
 *   To use the interrupt handler of a driver module, a callback should be registered with the interrupt handler. When the interrupt occurs, the callback is invoked.
 *   Call the #hal_uart_send_dma() API to send data in the UART interrupt mode (UART DMA mode). The UART HAL driver will copy the data to DMA buffer and start the DMA operation while the DMA sends data to the UART hardware. When the DMA operation completes, the DMA interrupt is issued. The UART HAL driver will invoke the user callback in interrupt handler to notify that the data has been sent out by the UART.
 *   The interrupt mode driver architecture is shown below:
 *   @image html hal_overview_interrupt_mode_architecture.png
 *
 *   All HAL APIs are thread-safe and available for ISR. Developers may call HAL APIs in any FreeRTOS task or in an ISR.\n
 *   However, some hardware have limited resources, and the corresponding APIs return "ERROR_BUSY" status when there is a resource conflict due to the re-entrance. Developers should always check the return values from HAL APIs.\n
 *   For example, I2C master supports only one user at a time, so #hal_i2c_master_init() returns #HAL_I2C_STATUS_ERROR_BUSY when another user is using I2C master.\n
 *
 * @}
 */


#include "hal_feature_config.h"

/*****************************************************************************
* module header file include
*****************************************************************************/
#ifdef HAL_ACCDET_MODULE_ENABLED
#include "hal_accdet.h"
#endif

#ifdef HAL_ADC_MODULE_ENABLED
#include "hal_adc.h"
#endif

#ifdef HAL_AES_MODULE_ENABLED
#include "hal_aes.h"
#endif

#ifdef HAL_AESOTF_MODULE_ENABLED
#include "hal_aesotf.h"
#endif

#ifdef HAL_DAC_MODULE_ENABLED
#include "hal_dac.h"
#endif

#ifdef HAL_DES_MODULE_ENABLED
#include "hal_des.h"
#endif

<<<<<<< HEAD
#ifdef HAL_AUDIO_MODULE_ENABLED
#include "hal_audio.h"
#endif

=======
>>>>>>> db20e11 (second commit)
#ifdef HAL_CACHE_MODULE_ENABLED
#include "hal_cache.h"
#endif

#ifdef HAL_CLOCK_MODULE_ENABLED
#include "hal_clock.h"
#endif

#ifdef HAL_DVFS_MODULE_ENABLED
#include "hal_dvfs.h"
#endif

#ifdef HAL_DES_MODULE_ENABLED
#include "hal_des.h"
#endif

#ifdef HAL_EINT_MODULE_ENABLED
#include "hal_eint.h"
#endif

#ifdef HAL_FLASH_MODULE_ENABLED
#include "hal_flash.h"
#endif

#ifdef HAL_SW_DMA_MODULE_ENABLED
#include "hal_sw_dma.h"
#endif

#ifdef HAL_GDMA_MODULE_ENABLED
#include "hal_gdma.h"
#endif

#ifdef HAL_GPC_MODULE_ENABLED
#include "hal_gpc.h"
#endif

#ifdef HAL_GPIO_MODULE_ENABLED
#include "hal_gpio.h"
#endif

#ifdef HAL_GPT_MODULE_ENABLED
#include "hal_gpt.h"
#endif

#ifdef HAL_I2C_MASTER_MODULE_ENABLED
#include "hal_i2c_master.h"
#endif

#ifdef HAL_I2S_MODULE_ENABLED
#include "hal_i2s.h"
#endif

#ifdef HAL_IRRX_MODULE_ENABLED
#include "hal_irrx.h"
#endif

#ifdef HAL_IRTX_MODULE_ENABLED
#include "hal_irtx.h"
#endif

#ifdef HAL_ISINK_MODULE_ENABLED
#include "hal_isink.h"
#endif

#ifdef HAL_KEYPAD_MODULE_ENABLED
#include "hal_keypad.h"
#endif

#ifdef HAL_MD5_MODULE_ENABLED
#include "hal_md5.h"
#endif

#ifdef HAL_MPU_MODULE_ENABLED
#include "hal_mpu.h"
#endif

#ifdef HAL_NVIC_MODULE_ENABLED
#include "hal_nvic.h"
#endif

#ifdef HAL_PWM_MODULE_ENABLED
#include "hal_pwm.h"
#endif

#ifdef HAL_RTC_MODULE_ENABLED
#include "hal_rtc.h"
#endif

#ifdef HAL_SD_MODULE_ENABLED
#include "hal_sd.h"
#endif

#ifdef HAL_SDIO_MODULE_ENABLED
#include "hal_sdio.h"
#endif

#ifdef HAL_SDIO_SLAVE_MODULE_ENABLED
#include "hal_sdio_slave.h"
#endif

#ifdef HAL_SHA_MODULE_ENABLED
#include "hal_sha.h"
#endif

#ifdef HAL_SPI_MASTER_MODULE_ENABLED
#include "hal_spi_master.h"
#endif

#ifdef HAL_SPI_SLAVE_MODULE_ENABLED
#include "hal_spi_slave.h"
#endif

#ifdef HAL_TRNG_MODULE_ENABLED
#include "hal_trng.h"
#endif

#ifdef HAL_UART_MODULE_ENABLED
#include "hal_uart.h"
#endif

#ifdef HAL_USB_MODULE_ENABLED
#include "hal_usb.h"
#endif

#ifdef HAL_WDT_MODULE_ENABLED
#include "hal_wdt.h"
#endif

#ifdef HAL_SLEEP_MANAGER_ENABLED
#include "hal_sleep_manager.h"
#endif

#ifdef HAL_G2D_MODULE_ENABLED
#include "hal_g2d.h"
#endif

#ifdef HAL_PMU_MODULE_ENABLED
#include "hal_pmu.h"
#include "hal_pmu_platform.h"
#endif

#ifdef HAL_TIME_CHECK_ENABLED
#include "hal_time_check.h"
#endif

#ifdef HAL_CCNI_MODULE_ENABLED
#include "hal_ccni.h"
#endif

#ifdef HAL_HW_SEMAPHORE_MODULE_ENABLED
#include "hal_hw_semaphore.h"
#endif

#ifdef HAL_DWT_MODULE_ENABLED
#include "hal_dwt.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif /* __HAL_H__ */

