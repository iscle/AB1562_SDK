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

#ifndef __HAL_PLATFORM_H__
#define __HAL_PLATFORM_H__


#include "hal_define.h"
#include "hal_feature_config.h"

#ifdef __cplusplus
extern "C" {
#endif
#define HAL_CLOCK_MODULE_ENABLED
#define HAL_DVFS_MODULE_ENABLED
#define HAL_RTC_MODULE_ENABLED
/*****************************************************************************
* Defines for module subfeatures.
* All the subfeatures described below are mandatory for the driver operation. No change is recommended.
*****************************************************************************/
#ifdef HAL_CACHE_MODULE_ENABLED
#define HAL_CACHE_WITH_REMAP_FEATURE     /* Enable CACHE setting with remap feature. */
#endif

#ifdef HAL_I2C_MASTER_MODULE_ENABLED
#define HAL_I2C_MASTER_FEATURE_HIGH_SPEED                        /* Enable I2C high speed 2M&3.25M. */
#define HAL_I2C_MASTER_FEATURE_SEND_TO_RECEIVE  /*Enable I2C master send to receive feature. */
#define HAL_I2C_MASTER_FEATURE_EXTENDED_DMA     /* Enable I2C master extend DMA feature.*/
#define HAL_I2C_MASTER_FEATURE_CONFIG_IO        /* Enable I2C master config IO mode feature.*/
#define HAL_I2C_MASTER_FRATURE_SW_FIFO
#define HAL_I2C_MASTER_FRATURE_NONE_BLOCKING    /* Enable I2C master none-blocking feature.*/
#endif

#ifdef HAL_WDT_MODULE_ENABLED
#define HAL_WDT_FEATURE_SECOND_CHANNEL          /* Supports the second WDT */
#endif

#ifdef HAL_SPI_MASTER_MODULE_ENABLED
#define HAL_SPI_MASTER_FEATURE_ADVANCED_CONFIG       /* Enable SPI master advanced configuration feature. For more details, please refer to hal_spi_master.h.*/
#define HAL_SPI_MASTER_FEATURE_DEASSERT_CONFIG       /* Enable SPI master deassert configuration feature to deassert the chip select signal after each byte data transfer is complete. For more details, please refer to hal_spi_master.h.*/
#define HAL_SPI_MASTER_FEATURE_CHIP_SELECT_TIMING    /* Enable SPI master chip select timing configuration feature to set timing value for chip select signal. For more details, please refer to hal_spi_master.h.*/
#define HAL_SPI_MASTER_FEATURE_DMA_MODE              /* Enable SPI master DMA mode feature to do data transfer. For more details, please refer to hal_spi_master.h.*/
#define HAL_SPI_MASTER_FEATURE_DUAL_QUAD_MODE        /* Enable SPI master to use dual mode and quad mode. For more details, please refer to hal_spi_master.h. */
#endif

#ifdef HAL_GPIO_MODULE_ENABLED
#define HAL_GPIO_FEATURE_INVERSE              /* Input status of the pin can be set to the reverse state. For more details, please refer to hal_gpio.h. */
//#define HAL_GPIO_FEATURE_PUPD               /* Pull state of the pin can be configured with different resistors through different combinations of GPIO_PUPD_x,GPIO_RESEN0_x and GPIO_RESEN1_x. For more details, please refer to hal_gpio.h. */
<<<<<<< HEAD
//#define HAL_GPIO_FEATURE_CLOCKOUT           /* The pin can be configured as an output clock. For more details, please refer to hal_gpio.h. */
=======
#define HAL_GPIO_FEATURE_CLOCKOUT           /* The pin can be configured as an output clock. For more details, please refer to hal_gpio.h. */
>>>>>>> db20e11 (second commit)
#define HAL_GPIO_FEATURE_HIGH_Z             /* The pin can be configured to provide high impedance state to prevent possible electric leakage. For more details, please refer to hal_gpio.h. */
#define HAL_GPIO_FEATURE_SET_DRIVING        /* The pin can be configured to enhance driving. For more details, please refer to hal_gpio.h. */
#define HAL_GPIO_FEATURE_SET_SCHMITT        /* The pin can be configured to enhance schmitt trigger hysteresis. */
#define HAL_GPIO_FEATURE_SET_SLEW_RATE      /* The pin can be configured to enhance slew rate. */
//#define HAL_GPIO_FEATURE_CONFLICT_CHECK        /* The pin can be conflict check if the pin is used other function */
#endif

#ifdef HAL_EINT_MODULE_ENABLED
#define HAL_EINT_FEATURE_MASK                /* Supports EINT mask interrupt. */
//#define HAL_EINT_FEATURE_SW_TRIGGER_EINT     /* Supports software triggered EINT interrupt. */
//#define HAL_EINT_FEATURE_MUX_MAPPING         /* Supports EINT number mux to different EINT GPIO pin. */
#endif

#ifdef HAL_GPC_MODULE_ENABLED
#define HAL_GPC_FEATURE_ADVANCED_CONFIG     /*Supports the interrupt mode.*/
#endif

#ifdef HAL_GPT_MODULE_ENABLED
#define HAL_GPT_FEATURE_US_TIMER               /* Supports a microsecond timer. */
#define HAL_GPT_SW_GPT_FEATURE                 /* Supports software GPT timer. */
#define HAL_GPT_PORT_ALLOCATE                  /* Allocates GPT communication port. */
#define HAL_GPT_SW_GPT_US_FEATURE              /* Supports software GPT us timer. */
#endif

#ifdef HAL_PWM_MODULE_ENABLED
//#define HAL_PWM_FEATURE_ADVANCED_CONFIG        /* Supports PWM advanced configuration. */
#endif

#ifdef HAL_RTC_MODULE_ENABLED
#define HAL_RTC_FEATURE_TIME_CALLBACK           /* Supports time change notification callback. */
#define HAL_RTC_FEATURE_RTC_MODE                /* Supports enter RTC mode. */
#define HAL_RTC_FEATURE_GPIO                    /* Supports RTC GPIO configuration. */
#define HAL_RTC_FEATURE_GPIO_EINT               /* Supports RTC GPIO and EINT configuration. */
#define HAL_RTC_FEATURE_CAPTOUCH                /* Supports CAPTOUCH configuration. */
#define HAL_RTC_FEATURE_EINT                    /* Supports EINT configuration. */
<<<<<<< HEAD
=======

typedef enum {
    HAL_RTC_OSC32K_EOSC_MODE = 0,
    HAL_RTC_OSC32K_DCXO_MODE = 1,
    HAL_RTC_OSC32K_XOSC_MODE = 2,
}hal_rtc_osc32k_mode_t;


>>>>>>> db20e11 (second commit)
#endif

#ifdef HAL_PWM_MODULE_ENABLED
#define HAL_PWM_FEATURE_ADVANCED_CONFIG        /* Supports PWM advanced configuration. */
#define HAL_PWM_CLOCK_32K_SUPPORTED            /* Supports 32K PWM source clock. */
#define HAL_PWM_CLOCK_26M_SUPPORTED            /* Supports 26M PWM source clock. */
#define HAL_PWM_CLOCK_48M_SUPPORTED            /* Supports 48M PWM source clock. */
#endif


#ifdef HAL_SPI_SLAVE_MODULE_ENABLED
#define HAL_SPI_SLAVE_FEATURE_SW_CONTROL        /* Supports SPI slave to communicate with SPI master using software control. */
#endif

#ifdef HAL_I2S_MODULE_ENABLED
#define HAL_I2S_EXTENDED                        /* Supports advanced settings. */
#define HAL_I2S_SUPPORT_VFIFO                   /* Supports virtual FIFO. */
#define HAL_I2S_FEATURE_MULTI_I2S               /* Supports multiple I2S connections. */
#define HAL_I2S_FEATURE_TDM                     /* Supports TDM mode. */
#endif

#ifdef HAL_UART_MODULE_ENABLED
//#define HAL_UART_FEATURE_VFIFO_DMA_TIMEOUT        /* Supports configurable timeout value setting */
#endif

#ifdef HAL_AES_MODULE_ENABLED
#define HAL_AES_USE_PHYSICAL_MEMORY_ADDRESS        /* Notify caller must use physical memory */
#endif

#ifdef HAL_DES_MODULE_ENABLED
#define HAL_DES_USE_PHYSICAL_MEMORY_ADDRESS        /* Notify caller must use physical memory */
#endif

#ifdef HAL_MD5_MODULE_ENABLED
#define HAL_MD5_USE_PHYSICAL_MEMORY_ADDRESS        /* Notify caller must use physical memory */
#endif

#ifdef HAL_SHA_MODULE_ENABLED
#define HAL_SHA_USE_PHYSICAL_MEMORY_ADDRESS        /* Notify caller must use physical memory */
#endif

#ifdef HAL_SLEEP_MANAGER_ENABLED
/**
 * @addtogroup HAL
 * @{
 * @addtogroup SLEEP_MANAGER
 * @{
 * @addtogroup hal_sleep_manager_enum Enum
 * @{
 */
/*****************************************************************************
 * Enum
 *****************************************************************************/
/** @brief Sleep modes */
typedef enum {
    HAL_SLEEP_MODE_NONE = 0,        /**< No sleep. */
    HAL_SLEEP_MODE_IDLE,            /**< Idle state. */
    HAL_SLEEP_MODE_SLEEP,           /**< Sleep state. */
    HAL_SLEEP_MODE_NUMBER           /**< To support range detection. */
} hal_sleep_mode_t;
/** @brief sleep_manager wake up source */
typedef enum {
    HAL_SLEEP_MANAGER_WAKEUP_SOURCE_GPT = 0,            /**< General purpose timer. */
    HAL_SLEEP_MANAGER_WAKEUP_SOURCE_EINT = 1,           /**< External interrupt. */
    HAL_SLEEP_MANAGER_WAKEUP_SOURCE_IRQGEN = 2,         /**< IRQGEN. */
    HAL_SLEEP_MANAGER_WAKEUP_SOURCE_OST = 3,            /**< OST. */
    HAL_SLEEP_MANAGER_WAKEUP_SOURCE_AUDIO = 4,          /**< AUDIO.  */
    HAL_SLEEP_MANAGER_WAKEUP_SOURCE_I2S = 5,            /**< I2S. */
    HAL_SLEEP_MANAGER_WAKEUP_SOURCE_I2S_DMA = 6,        /**< I2S_DMA. */
    HAL_SLEEP_MANAGER_WAKEUP_SOURCE_CAP_TOUCH = 7,      /**< CAP_TOUCH. */
    HAL_SLEEP_MANAGER_WAKEUP_SOURCE_ANC = 8,            /**< ANC. */
    HAL_SLEEP_MANAGER_WAKEUP_SOURCE_SPI_SLAVE = 9,      /**< SPI_SLAVE. */
    HAL_SLEEP_MANAGER_WAKEUP_SOURCE_RGU = 10,           /**< RGU. */
    HAL_SLEEP_MANAGER_WAKEUP_SOURCE_DSP_DMA = 12,       /**< DSP_DMA. */
    HAL_SLEEP_MANAGER_WAKEUP_SOURCE_ALL = 13            /**< All wakeup source. */
} hal_sleep_manager_wakeup_source_t;
/**
 * @}
 * @}
 * @}
 */
#endif

#ifdef HAL_UART_MODULE_ENABLED
/**
 * @addtogroup HAL
 * @{
 * @addtogroup UART
 * @{
 * @addtogroup hal_uart_enum Enum
 * @{
 */
/*****************************************************************************
* UART
*****************************************************************************/
/** @brief UART port index
 * There are total of four UART ports. Only UART0 and UART1 support hardware flow control.
 * | UART port | Hardware Flow Control |
 * |-----------|-----------------------|
 * |  UART0    |           V           |
 * |  UART1    |           V           |
 * |  UART2    |           X           |
 * |  UART3    |           X           |
 */
typedef enum {
    HAL_UART_0 = 0,                            /**< UART port 0. */
    HAL_UART_1 = 1,                            /**< UART port 1. */
    HAL_UART_MAX                               /**< The total number of UART ports (invalid UART port number). */
} hal_uart_port_t;

/**
  * @}
  */

/**@addtogroup hal_uart_define Define
 * @{
  */

/** @brief  The maximum timeout value for UART timeout interrupt, unit is ms.
  */
#define HAL_UART_TIMEOUT_VALUE_MAX  (2500)

/**
  * @}
  */

/**
 * @}
 * @}
 */
#endif


#ifdef HAL_I2C_MASTER_MODULE_ENABLED
#ifdef HAL_I2C_MASTER_FEATURE_EXTENDED_DMA
/**
 * @addtogroup HAL
 * @{
 * @addtogroup I2C_MASTER
 * @{
 * @section HAL_I2C_Transaction_Pattern_Chapter API transaction length and packets
 *  The Transaction packet is a packet sent by the I2C master using SCL and SDA.
 *  Different APIs have different transaction packets, as shown below.
 * - \b Transaction \b length \b supported \b by \b the \b APIs \n
 *  The total transaction length is determined by 4 parameters:\n
 *  send_packet_length(Ns), which indicates the number of sent packet.\n
 *  send_bytes_in_one_packet(Ms).\n
 *  receive_packet_length(Nr).\n
 *  receive_bytes_in_one_packet(Mr).\n
 *  Next, the relationship between transaction packet and these 4 parameters is introduced.
 *  - Total transaction length = Ns * Ms + Nr * Mr.
 *   - Ns is the packet length to be sent by the I2C master.
 *   - Ms is the total number of bytes in a sent packet.
 *   - Nr is the packet length to be received by the I2C master.
 *   - Mr is the total number of bytes in a received packet.
 *  - NA means the related parameter should be ignored.
 *  - 1~8 specifies the parameter range is from 1 to 8. 1~15 specifies the parameter range is from 1 to 15. 1~255 specifies the parameter range from 1 to 255.
 *  - 1 means the parameter value can only be 1.
 *  - Note1: functions with the suffix "_ex" have these 4 parameters. Other functions only have the "size" parameter and the driver splits the "size" into these 4 parameters.
 *  - Note2: The maximum of total transaction length is 256K.\n
 *    #hal_i2c_master_send_polling() for example, the "size" will be divided like this: send_packet_length = 1, send_bytes_in_one_packet = size.
 *          As a result, the total size should be: send_packet_length * send_bytes_in_one_packet = 1 * size = size. The range of "size" should be from 1 to 8.
 * |API                                         |send_packet_length(Ns) | send_bytes_in_one_packet(Ms) | receive_packet_length(Nr) | receive_bytes_in_one_packet(Mr) |
 * |--------------------------------------------|-----------------------|------------------------------|---------------------------|---------------------------------|
 * |hal_i2c_master_send_polling                 |          1            |            1~8               |            NA             |                NA               |
 * |hal_i2c_master_receive_polling              |          NA           |            NA                |            1              |                1~8              |
 * |hal_i2c_master_send_to_receive_polling      |          1            |            1~8               |            1              |                1~8              |
 * |hal_i2c_master_send_dma                     |          1            |            1~65535           |            NA             |                NA               |
 * |hal_i2c_master_receive_dma                  |          NA           |            NA                |            1~65535        |                1                |
 * |hal_i2c_master_send_to_receive_dma          |          1            |            1~65535           |            1~65534        |                1                |
 * |hal_i2c_master_send_dma_ex                  |          1~65535      |            1~65535           |            NA             |                NA               |
 * |hal_i2c_master_receive_dma_ex               |          NA           |            NA                |            1~65535        |                1~65535          |
 * |hal_i2c_master_send_to_receive_dma_ex       |          1            |            1~65535           |            1~65534        |                1~65535          |
 *
 * - \b Waveform \b pattern \b supported \b by \b the \b APIs \n
 *  The 4 parameters (send_packet_length(Ns), send_bytes_in_one_packet(Ms), receive_packet_length(Nr), receive_bytes_in_one_packet(Mr) will also affect the transaction packet.
 *  The relationship between transaction packet and these 4 parameters is shown below.
 *  - Ns is the send_packet_length.
 *  - Ms is the send_bytes_in_one_packet.
 *  - Nr is the receive_packet_length.
 *  - Mr is the receive_bytes_in_one_packet.
 * |API                                          |transaction packet format                                 |
 * |---------------------------------------------|----------------------------------------------------------|
 * | hal_i2c_master_send_polling                 |  @image html hal_i2c_send_poling_waveform.png            |
 * | hal_i2c_master_receive_polling              |  @image html hal_i2c_receive_poling_waveform.png         |
 * | hal_i2c_master_send_to_receive_polling      |  @image html hal_i2c_send_to_receive_poling_waveform.png |
 * | hal_i2c_master_send_dma                     |  @image html hal_i2c_send_dma_waveform.png            |
 * | hal_i2c_master_receive_dma                  |  @image html hal_i2c_receive_dma_waveform.png         |
 * | hal_i2c_master_send_to_receive_dma          |  @image html hal_i2c_send_to_receive_dma_waveform.png |
 * | hal_i2c_master_send_dma_ex                  |  @image html hal_i2c_send_dma_ex_waveform.png            |
 * | hal_i2c_master_receive_dma_ex               |  @image html hal_i2c_receive_dma_ex_waveform.png         |
 * | hal_i2c_master_send_to_receive_dma_ex       |  @image html hal_i2c_send_to_receive_dma_ex_waveform.png |
 *
 *
 *
 *
 */
#else
/**
 * @addtogroup HAL
 * @{
 * @addtogroup I2C_MASTER
 * @{
 */
#endif

/** @defgroup hal_i2c_master_define Define
 * @{
  */

/** @brief  The maximum polling mode transaction size.
  */
#define HAL_I2C_MAXIMUM_POLLING_TRANSACTION_SIZE  8

/** @brief  The maximum DMA mode transaction size.
  */
#define HAL_I2C_MAXIMUM_DMA_TRANSACTION_SIZE   15

/**
  * @}
  */

/** @addtogroup hal_i2c_master_enum Enum
  * @{
  */

/*****************************************************************************
* I2C master
*****************************************************************************/
/** @brief This enum defines the I2C port.
 *  The platform supports 4 I2C masters. Three of them support polling mode and DMA mode,
 *  while the other only supports polling mode. For more information about the polling mode,
 *  DMA mode, queue mode, please refer to @ref HAL_I2C_Features_Chapter. The details
 *  are shown below:
 *  - Supported features of I2C masters \n
 *    V : supported;  X : not supported.
 * |I2C Master   | Polling mode | DMA mode | Extended DMA mode |
 * |-------------|--------------|----------|-------------------|
 * |I2C0         |      V       |    V     |         V         |
 * |I2C1         |      V       |    V     |         V         |
 * |I2C2         |      V       |    V     |         V         |
 * |I2CAO        |      X       |    X     |         X         |
 *
 *
*/
typedef enum {
    HAL_I2C_MASTER_0 = 0,                /**< I2C master 0. */
    HAL_I2C_MASTER_1,                /**< I2C master 1. */
    HAL_I2C_MASTER_AO,                /**< I2C master AO. */
    HAL_I2C_MASTER_MAX                  /**< The total number of I2C masters (invalid I2C master number). */
} hal_i2c_port_t;

/**
  * @}
  */

/**
 * @}
 * @}
 */
#endif


#ifdef HAL_GPIO_MODULE_ENABLED
/**
* @addtogroup HAL
* @{
* @addtogroup GPIO
* @{
*
* @addtogroup hal_gpio_enum Enum
* @{
*/

/*****************************************************************************
* GPIO
*****************************************************************************/
/** @brief This enum defines the GPIO port.
 *  The platform supports a total of 18 GPIO pins with various functionality.
 *
*/

typedef enum {
    HAL_GPIO_0  = 0,    /**< GPIO pin0. */
    HAL_GPIO_1  = 1,    /**< GPIO pin1. */
    HAL_GPIO_2  = 2,    /**< GPIO pin2. */
    HAL_GPIO_3  = 3,    /**< GPIO pin3. */
    HAL_GPIO_4  = 4,    /**< GPIO pin4. */
    HAL_GPIO_5  = 5,    /**< GPIO pin5. */
    HAL_GPIO_6  = 6,    /**< GPIO pin6. */
    HAL_GPIO_7  = 7,    /**< GPIO pin7. */
    HAL_GPIO_8  = 8,    /**< GPIO pin8. */
    HAL_GPIO_9  = 9,    /**< GPIO pin9. */
    HAL_GPIO_10 = 10,   /**< GPIO pin10. */
    HAL_GPIO_11 = 11,   /**< GPIO pin11. */
    HAL_GPIO_12 = 12,   /**< GPIO pin12. */
    HAL_GPIO_13 = 13,   /**< GPIO pin13. */
    HAL_GPIO_14 = 14,   /**< GPIO pin14. */
    HAL_GPIO_15 = 15,   /**< GPIO pin15. */
    HAL_GPIO_16 = 16,   /**< GPIO pin16. */
    HAL_GPIO_17 = 17,   /**< GPIO pin17. */
    HAL_GPIO_MAX                               /**< The total number of GPIO pins (invalid GPIO pin number). */
} hal_gpio_pin_t;

/**
  * @}
  */

/**
 * @}
 * @}
 */
#endif

#ifdef HAL_GPIO_FEATURE_CLOCKOUT
/**
 * @addtogroup HAL
 * @{
 * @addtogroup GPIO
 * @{
 * @addtogroup hal_gpio_enum Enum
 * @{
 */
/*****************************************************************************
* CLKOUT
*****************************************************************************/
/** @brief  This enum defines output clock number of GPIO */
typedef enum {
    HAL_GPIO_CLOCK_0   = 0,              /**< define GPIO output clock 0 */
    HAL_GPIO_CLOCK_1   = 1,              /**< define GPIO output clock 1 */
    HAL_GPIO_CLOCK_2   = 2,              /**< define GPIO output clock 2 */
<<<<<<< HEAD
    HAL_GPIO_CLOCK_3   = 3,              /**< define GPIO output clock 3 */
    HAL_GPIO_CLOCK_4   = 4,              /**< define GPIO output clock 4 */
=======
>>>>>>> db20e11 (second commit)
    HAL_GPIO_CLOCK_MAX                   /**< define GPIO output clock max number(invalid) */
} hal_gpio_clock_t;


/** @brief This enum defines output clock mode of GPIO */
typedef enum {
<<<<<<< HEAD
    HAL_GPIO_CLOCK_MODE_32K = 0,        /**< define GPIO output clock mode as 32KHz */
    HAL_GPIO_CLOCK_MODE_24M = 2,        /**< define GPIO output clock mode as 24MHz */
    HAL_GPIO_CLOCK_MODE_48M = 3,        /**< define GPIO output clock mode as 48MHz */
    HAL_GPIO_CLOCK_MODE_40K = 5,        /**< define GPIO output clock mode as 40MHz */
=======
    HAL_GPIO_CLOCK_MODE_32K = 1,        /**< define GPIO output clock mode as 32KHz */
    HAL_GPIO_CLOCK_MODE_26M = 5,        /**< define GPIO output clock mode as 26MHz */
>>>>>>> db20e11 (second commit)
    HAL_GPIO_CLOCK_MODE_MAX             /**< define GPIO output clock mode of max number(invalid) */
} hal_gpio_clock_mode_t;
/**
  * @}
  */

/**
 * @}
 * @}
 */
#endif

#ifdef HAL_ADC_MODULE_ENABLED
#define HAL_ADC_FEATURE_NO_NEED_PINMUX             /*please not to modify*/
#define HAL_ADC_CALIBRATION_ENABLE                 /*Enable ADC calibration */

/**
* @addtogroup HAL
* @{
* @addtogroup ADC
* @{
*
* @addtogroup hal_adc_enum Enum
* @{
*/

/*****************************************************************************
* ADC
*****************************************************************************/
/** @brief adc channel */
typedef enum {
    HAL_ADC_CHANNEL_0 = 0,                        /**< ADC channel 0. */
    HAL_ADC_CHANNEL_1 = 1,                        /**< ADC channel 1. */
    HAL_ADC_CHANNEL_2 = 2,                        /**< ADC channel 2. */
    HAL_ADC_CHANNEL_3 = 3,                        /**< ADC channel 3. */
    HAL_ADC_CHANNEL_MAX                           /**< The total number of ADC channels (invalid ADC channel).*/
} hal_adc_channel_t;

/**
  * @}
  */


/**
 * @}
 * @}
 */
#endif



#ifdef HAL_I2S_MODULE_ENABLED
/**
* @addtogroup HAL
* @{
* @addtogroup I2S
* @{
*
* @addtogroup hal_i2s_enum Enum
* @{
*/


/*****************************************************************************
* I2S
*****************************************************************************/
#ifdef HAL_I2S_FEATURE_MULTI_I2S
/** @brief This enum defines the I2S port.
 *
 *  The platform supports 2 I2S HW interface. Two of them support master mode and slave mode.
 *  User can drive 2 I2S HW simultaneously by the extended APIs. The basic APIs are only for I2S0.
 *  User should not use basic APIs and extension APIs simultaneously. The details are shown below:
 *
 *  - I2S supported feature table \n
 *    V : means support;  X : means not support.
 *
 * |I2S PORT   |SAMPLE WIDTH    | FS | I2S TDM  |
 * |--------- |--------------|-------------|-------------------|
 * |I2S0         | 16 bits                 |11.025 / 16 / 22.05 / 24 / 44.1 /48 /96 / 192 KHZ     | V        |
 * |I2S1         | 16 / 24 bits         |11.025 / 16 / 22.05 / 24 / 44.1 /48 /96 / 192 KHZ     | X        |
 *
*/
typedef enum {
    HAL_I2S_0  = 0,   /**< I2S interfeace 0. */
    HAL_I2S_1  = 1,    /**< I2S interfeace 1. */
    HAL_I2S_MAX
} hal_i2s_port_t;
#endif


#ifdef HAL_I2S_FEATURE_TDM
/** @brief Channels per frame sync. Number of channels in each frame sync.*/
typedef enum {
    HAL_I2S_TDM_2_CHANNEL  = 0,   /**< 2 channels. */
    HAL_I2S_TDM_4_CHANNEL  = 1    /**< 4 channels. */
} hal_i2s_tdm_channel_t;


/** @brief Polarity of BCLK.*/
typedef enum {
    HAL_I2S_BCLK_INVERSE_DISABLE  = 0, /**< Normal mode. (Invalid)*/
    HAL_I2S_BCLK_INVERSE_EABLE  = 1    /**< Invert BCLK. (Invalid)*/
} hal_i2s_bclk_inverse_t;
#endif

#ifdef HAL_I2S_EXTENDED
/** @brief I2S sample widths.  */
typedef enum {
    HAL_I2S_SAMPLE_WIDTH_8BIT  = 0,   /**< I2S sample width is 8bit. (Invalid)*/
    HAL_I2S_SAMPLE_WIDTH_16BIT = 1,   /**< I2S sample width is 16bit. (HAL_I2S_0)*/
    HAL_I2S_SAMPLE_WIDTH_24BIT = 2    /**< I2S sample width is 24bit. (HAL_I2S_0/HAL_I2S_1)*/
} hal_i2s_sample_width_t;


/** @brief Number of bits per frame sync(FS). This parameter determines the bits of a complete sample of both left and right channels.*/
typedef enum {
    HAL_I2S_FRAME_SYNC_WIDTH_32  = 0,   /**< 32 bits per frame. */
    HAL_I2S_FRAME_SYNC_WIDTH_64  = 1,   /**< 64 bits per frame. */
    HAL_I2S_FRAME_SYNC_WIDTH_128  = 2   /**< 128 bits per frame. */
} hal_i2s_frame_sync_width_t;


/** @brief Enable or disable right channel of the I2S TX to send the same data as on the left channel of the I2S TX.\n
        This function only works when the sample width of the I2S is 16 bits.*/
typedef enum {
    HAL_I2S_TX_MONO_DUPLICATE_DISABLE = 0,  /**< Keep data to its channel. */
    HAL_I2S_TX_MONO_DUPLICATE_ENABLE  = 1   /**< Assume input is mono data for left channel, the data is duplicated to right channel.*/
} hal_i2s_tx_mode_t;


/** @brief Enable or disable twice the downsampling rate mode in the I2S RX.
                 In this mode the sampling rate of the I2S TX is 48kHz while the sampling rate of the I2S RX is 24kHz. The I2S RX automatically drops 1 sample in each 2 input samples received. */
typedef enum {
    HAL_I2S_RX_DOWN_RATE_DISABLE = 0,  /**< Actual sampling rate of the I2S RX = sampling rate. (Default)*/
    HAL_I2S_RX_DOWN_RATE_ENABLE  = 1   /**< Actual sampling rate of the I2S RX is half of the original sampling rate. (Invalid)*/
} hal_i2s_rx_down_rate_t;
#endif //  #ifdef HAL_I2S_EXTENDED


/** @brief Enable or disable data swapping between right and left channels of the I2S link.\n
        This function only works when the sample width of the I2S is 16 bits.*/
typedef enum {
    HAL_I2S_LR_SWAP_DISABLE = 0,  /**< Disable the data swapping. */
    HAL_I2S_LR_SWAP_ENABLE  = 1   /**< Enable the data swapping.  */
} hal_i2s_lr_swap_t;


/** @brief Enable or disable word select clock inverting of the I2S link. */
typedef enum {
    HAL_I2S_WORD_SELECT_INVERSE_DISABLE = 0,  /**< Disable the word select clock inverting. */
    HAL_I2S_WORD_SELECT_INVERSE_ENABLE  = 1   /**< Enable the word select clock inverting.  */
} hal_i2s_word_select_inverse_t;

/** @brief This enum defines initial type of the I2S.
 */

typedef enum {
    HAL_I2S_TYPE_EXTERNAL_MODE          = 0,        /**< External mode. I2S mode with external codec.*/
    HAL_I2S_TYPE_EXTERNAL_TDM_MODE      = 1,        /**< External TDM mode. I2S TDM mode with external codec*/
    HAL_I2S_TYPE_INTERNAL_MODE          = 2,        /**< Internal mode. I2S mode with internal codec. (Invalid)*/
    HAL_I2S_TYPE_INTERNAL_LOOPBACK_MODE = 3,        /**< I2S internal loopback mode. Data loopback mode.*/
    HAL_I2S_TYPE_INTERNAL_TDM_LOOPBACK_MODE = 4,    /**< TDM internal loopback mode. Data loopback mode.*/
    HAL_I2S_TYPE_MAX = 5
} hal_i2s_initial_type_t;


/** @brief I2S event */
typedef enum {
    HAL_I2S_EVENT_ERROR               = -1, /**<  An error occurred during the function call. */
    HAL_I2S_EVENT_NONE                =  0, /**<  No error occurred during the function call. */
    HAL_I2S_EVENT_OVERFLOW            =  1, /**<  RX data overflow. */
    HAL_I2S_EVENT_UNDERFLOW           =  2, /**<  TX data underflow. */
    HAL_I2S_EVENT_DATA_REQUEST        =  3, /**<  Request for user-defined data. */
    HAL_I2S_EVENT_DATA_NOTIFICATION   =  4  /**<  Notify user the RX data is ready. */
} hal_i2s_event_t;


/** @brief I2S sampling rates */
typedef enum {
    HAL_I2S_SAMPLE_RATE_8K        = 0,  /**<  8000Hz  */
    HAL_I2S_SAMPLE_RATE_11_025K   = 1,  /**<  11025Hz */
    HAL_I2S_SAMPLE_RATE_12K       = 2,  /**<  12000Hz */
    HAL_I2S_SAMPLE_RATE_16K       = 3,  /**<  16000Hz */
    HAL_I2S_SAMPLE_RATE_22_05K    = 4,  /**<  22050Hz */
    HAL_I2S_SAMPLE_RATE_24K       = 5,  /**<  24000Hz */
    HAL_I2S_SAMPLE_RATE_32K       = 6,  /**<  32000Hz */
    HAL_I2S_SAMPLE_RATE_44_1K     = 7,  /**<  44100Hz */
    HAL_I2S_SAMPLE_RATE_48K       = 8,  /**<  48000Hz */
    HAL_I2S_SAMPLE_RATE_88_2K     = 9,  /**<  88200Hz */
    HAL_I2S_SAMPLE_RATE_96K       = 10, /**<  96000Hz */
    HAL_I2S_SAMPLE_RATE_176_4K    = 11, /**<  176400Hz */
    HAL_I2S_SAMPLE_RATE_192K      = 12  /**<  192000Hz */
} hal_i2s_sample_rate_t;

/**
  * @}
  */

/**
 * @}
 * @}
 */
#endif


#ifdef HAL_SPI_MASTER_MODULE_ENABLED
/**
 * @addtogroup HAL
 * @{
 * @addtogroup SPI_MASTER
 * @{
 * @defgroup hal_spi_master_define Define
 * @{
 */

/** @brief  The maximum polling mode transaction size in bytes.
 */
#define HAL_SPI_MAXIMUM_POLLING_TRANSACTION_SIZE  32

/** @brief  The maximum transaction size in bytes when configuration is not single mode.
 */
#define HAL_SPI_MAXIMUM_NON_SINGLE_MODE_TRANSACTION_SIZE  15

/** @brief  The minimum clock frequency.
 */
#define  HAL_SPI_MASTER_CLOCK_MIN_FREQUENCY  30000

/** @brief  The maximum clock frequency.
 */
#define  HAL_SPI_MASTER_CLOCK_MAX_FREQUENCY  52000000

/**
 * @}
 */

/**
 * @addtogroup hal_spi_master_enum Enum
 * @{
 */

/*****************************************************************************
* SPI master
*****************************************************************************/
/** @brief This enum defines the SPI master port.
 *  The chip supports total of 4 SPI master ports, each of them supports polling mode
 *  and DMA mode. For more details about polling mode and DMA mode, please refer to @ref
 *  HAL_SPI_MASTER_Features_Chapter.
 */
typedef enum {
    HAL_SPI_MASTER_0 = 0,                              /**< SPI master port 0. */
    HAL_SPI_MASTER_MAX                                 /**< The total number of SPI master ports (invalid SPI master port). */
} hal_spi_master_port_t;

/** @brief This enum defines the options to connect the SPI slave device to the SPI master's CS pins. */
typedef enum {
    HAL_SPI_MASTER_SLAVE_0 = 0,                       /**< The SPI slave device is connected to the SPI master's CS0 pin. */
    HAL_SPI_MASTER_SLAVE_MAX                          /**< The total number of SPI master CS pins (invalid SPI master CS pin). */
} hal_spi_master_slave_port_t;

/** @brief SPI master transaction bit order definition. */
typedef enum {
    HAL_SPI_MASTER_LSB_FIRST = 0,                       /**< Both send and receive data transfer LSB first. */
    HAL_SPI_MASTER_MSB_FIRST = 1                        /**< Both send and receive data transfer MSB first. */
} hal_spi_master_bit_order_t;

/** @brief SPI master clock polarity definition. */
typedef enum {
    HAL_SPI_MASTER_CLOCK_POLARITY0 = 0,                     /**< Active high. In idle state, SCLK low. */
    HAL_SPI_MASTER_CLOCK_POLARITY1 = 1                      /**< Active low. In idle state, SCLK high. */
} hal_spi_master_clock_polarity_t;

/** @brief SPI master clock format definition. */
typedef enum {
    HAL_SPI_MASTER_CLOCK_PHASE0 = 0,                         /**< Data sampling occurs at ODD edge. */
    HAL_SPI_MASTER_CLOCK_PHASE1 = 1                          /**< Data sampling occurs at EVEN edge. */
} hal_spi_master_clock_phase_t;

/** @brief This enum defines the mode of the SPI master. */
typedef enum {
    HAL_SPI_MASTER_SINGLE_MODE = 0,                      /**< Single mode. */
    HAL_SPI_MASTER_3_WIRE_MODE = 1,                      /**< Normal mode. */
    HAL_SPI_MASTER_DUAL_MODE = 2,                        /**< Dual mode. */
    HAL_SPI_MASTER_QUAD_MODE = 3,                        /**< Quad mode. */
} hal_spi_master_mode_t;

/**
 * @}
 */

/**
 * @}
 * @}
 */
#endif

#ifdef HAL_SPI_SLAVE_MODULE_ENABLED
/**
 * @addtogroup HAL
 * @{
 * @addtogroup SPI_SLAVE
 * @{
 * @addtogroup hal_spi_slave_enum Enum
 * @{
 */

/*****************************************************************************
* SPI slave
*****************************************************************************/
/** @brief This enum defines the SPI slave port. This chip supports only one
 *  SPI slave port.
 */
typedef enum {
    HAL_SPI_SLAVE_0 = 0,                             /**< SPI slave port 0. */
    HAL_SPI_SLAVE_MAX                                /**< The total number of SPI slave ports (invalid SPI slave port number). */
} hal_spi_slave_port_t;

/** @brief SPI slave transaction bit order definition. */
typedef enum {
    HAL_SPI_SLAVE_LSB_FIRST = 0,                       /**< Both send and receive data transfer is the LSB first. */
    HAL_SPI_SLAVE_MSB_FIRST = 1                        /**< Both send and receive data transfer is the MSB first. */
} hal_spi_slave_bit_order_t;

/** @brief SPI slave clock polarity definition. */
typedef enum {
    HAL_SPI_SLAVE_CLOCK_POLARITY0 = 0,                 /**< Clock polarity is 0. */
    HAL_SPI_SLAVE_CLOCK_POLARITY1 = 1                  /**< Clock polarity is 1. */
} hal_spi_slave_clock_polarity_t;

/** @brief SPI slave clock format definition. */
typedef enum {
    HAL_SPI_SLAVE_CLOCK_PHASE0 = 0,                    /**< Clock format is 0. */
    HAL_SPI_SLAVE_CLOCK_PHASE1 = 1                     /**< Clock format is 1. */
} hal_spi_slave_clock_phase_t;

/** @brief This enum defines the SPI slave event when an interrupt occurs. */
typedef enum {
    HAL_SPI_SLAVE_EVENT_POWER_ON = SPIS_INT_POWER_ON_MASK,         /**< Power on command is received. */
    HAL_SPI_SLAVE_EVENT_POWER_OFF = SPIS_INT_POWER_OFF_MASK,       /**< Power off command is received. */
    HAL_SPI_SLAVE_EVENT_CRD_FINISH = SPIS_INT_RD_CFG_FINISH_MASK,  /**< Configure read command is received. */
    HAL_SPI_SLAVE_EVENT_RD_FINISH = SPIS_INT_RD_TRANS_FINISH_MASK, /**< Read command is received. */
    HAL_SPI_SLAVE_EVENT_CWR_FINISH = SPIS_INT_WR_CFG_FINISH_MASK,  /**< Configure write command is received. */
    HAL_SPI_SLAVE_EVENT_WR_FINISH = SPIS_INT_WR_TRANS_FINISH_MASK, /**< Write command is received. */
    HAL_SPI_SLAVE_EVENT_RD_ERR = SPIS_INT_RD_DATA_ERR_MASK,        /**< An error occurred during a read command. */
    HAL_SPI_SLAVE_EVENT_WR_ERR = SPIS_INT_WR_DATA_ERR_MASK,        /**< An error occurred during a write command. */
    HAL_SPI_SLAVE_EVENT_TIMEOUT_ERR = SPIS_INT_TMOUT_ERR_MASK      /**< A timeout is detected between configure read command and read command or configure write command and write command. */
} hal_spi_slave_callback_event_t;

/** @brief This enum defines the SPI slave commands. */
typedef enum {
    HAL_SPI_SLAVE_CMD_WS        = 0,       /**< Write Status command. */
    HAL_SPI_SLAVE_CMD_RS        = 1,       /**< Read Status command. */
    HAL_SPI_SLAVE_CMD_WR        = 2,       /**< Write Data command. */
    HAL_SPI_SLAVE_CMD_RD        = 3,       /**< Read Data command. */
    HAL_SPI_SLAVE_CMD_POWEROFF  = 4,       /**< POWER OFF command. */
    HAL_SPI_SLAVE_CMD_POWERON   = 5,       /**< POWER ON command. */
    HAL_SPI_SLAVE_CMD_CW        = 6,       /**< Configure Write command. */
    HAL_SPI_SLAVE_CMD_CR        = 7,        /**< Configure Read command. */
    HAL_SPI_SLAVE_CMD_CT        = 8        /**< Configure Type command. */
} hal_spi_slave_command_type_t;

/**
 * @}
 */


/**
 * @}
 * @}
 */
#endif


#ifdef HAL_RTC_MODULE_ENABLED
/**
 * @addtogroup HAL
 * @{
 * @addtogroup RTC
 * @{
 * @addtogroup hal_rtc_define Define
 * @{
 */

/** @brief  This macro defines a maximum number for backup data that used in #hal_rtc_set_data(),
  * #hal_rtc_get_data(), #hal_rtc_clear_data functions.
  */
#define HAL_RTC_BACKUP_BYTE_NUM_MAX     (12)

/**
 * @}
 */

/**
 * @defgroup hal_rtc_enum Enum
 * @{
 */

/** @brief RTC current time change notification period selections. */
typedef enum {
    HAL_RTC_TIME_NOTIFICATION_NONE = 0,                     /**< No need for a time notification. */
    HAL_RTC_TIME_NOTIFICATION_EVERY_SECOND = 1,             /**< Execute a callback function set by #hal_rtc_set_time_notification_period() for every second. */
    HAL_RTC_TIME_NOTIFICATION_EVERY_MINUTE = 2,             /**< Execute a callback function set by #hal_rtc_set_time_notification_period() for every minute. */
    HAL_RTC_TIME_NOTIFICATION_EVERY_HOUR = 3,               /**< Execute a callback function set by #hal_rtc_set_time_notification_period() for every hour. */
    HAL_RTC_TIME_NOTIFICATION_EVERY_DAY = 4,                /**< Execute a callback function set by #hal_rtc_set_time_notification_period() for every day. */
    HAL_RTC_TIME_NOTIFICATION_EVERY_MONTH = 5,              /**< Execute a callback function set by #hal_rtc_set_time_notification_period() for every month. */
    HAL_RTC_TIME_NOTIFICATION_EVERY_YEAR = 6,               /**< Execute a callback function set by #hal_rtc_set_time_notification_period() for every year. */
    HAL_RTC_TIME_NOTIFICATION_EVERY_SECOND_1_2 = 7,         /**< Execute a callback function set by #hal_rtc_set_time_notification_period() for every one-half of a second. */
    HAL_RTC_TIME_NOTIFICATION_EVERY_SECOND_1_4 = 8,         /**< Execute a callback function set by #hal_rtc_set_time_notification_period() for every one-fourth of a second. */
    HAL_RTC_TIME_NOTIFICATION_EVERY_SECOND_1_8 = 9          /**< Execute a callback function set by #hal_rtc_set_time_notification_period() for every one-eighth of a second. */
} hal_rtc_time_notification_period_t;
/** @brief This enum defines the type of RTC GPIO. */
typedef enum {
    HAL_RTC_GPIO_0 = 0,     /**< RTC GPIO 0. */
    HAL_RTC_GPIO_1 = 1,     /**< RTC GPIO 1. */
    HAL_RTC_GPIO_2 = 2,     /**< RTC GPIO 2. */
    HAL_RTC_GPIO_MAX
} hal_rtc_gpio_t;
/**
 * @}
 */

/** @defgroup hal_rtc_struct Struct
  * @{
  */

/** @brief RTC time structure definition. */
typedef struct {
    uint8_t rtc_sec;                                  /**< Seconds after minutes     - [0,59]  */
    uint8_t rtc_min;                                  /**< Minutes after the hour    - [0,59]  */
    uint8_t rtc_hour;                                 /**< Hours after midnight      - [0,23]  */
    uint8_t rtc_day;                                  /**< Day of the month          - [1,31]  */
    uint8_t rtc_mon;                                  /**< Months                    - [1,12]  */
    uint8_t rtc_week;                                 /**< Days in a week            - [0,6]   */
    uint8_t rtc_year;                                 /**< Years                     - [0,127] */
    uint16_t rtc_milli_sec;                           /**< Millisecond value, when in time API, this represents the read only register rtc_int_cnt - [0,32767]; when in alarm API, this parameter has no meaning. */
} hal_rtc_time_t;
/** @brief RTC GPIO control structure definition. */
typedef struct {
    hal_rtc_gpio_t rtc_gpio;        /**< Configure which GPIO will apply this setting. */
    bool is_enable_rtc_eint;        /**< Enable RTC EINT or not. */
    bool is_falling_edge_active;    /**< Configure RTC EINT as falling edge active or not. */
    bool is_enable_debounce;        /**< Enable RTC EINT debounce or not, if enabled, EINT debounce time is 4T*32k. */
} hal_rtc_eint_config_t;
/** @brief This structure defines the settings of RTC GPIO. */
typedef struct {
    hal_rtc_gpio_t rtc_gpio;        /**< Configure which GPIO will apply this setting. */
    bool is_analog;                 /**< Cinfugure RTC GPIO as analog or digtal mode. */
    bool is_input;                  /**< Cinfugure RTC GPIO as input direction or not. */
    bool is_pull_up;                /**< If RTC GPIO is pull mode, configure RTC GPIO pull up or not. */
    bool is_pull_down;              /**< If RTC GPIO is pull mode, configure RTC GPIO pull down or not. */
} hal_rtc_gpio_config_t;
/**
 * @}
 */

/**
 * @}
 * @}
 */
#endif


#ifdef HAL_EINT_MODULE_ENABLED
/**
 * @addtogroup HAL
 * @{
 * @addtogroup EINT
 * @{
 * @addtogroup hal_eint_enum Enum
 * @{
 */

/*****************************************************************************
* EINT
*****************************************************************************/
/** @brief EINT pins. */
typedef enum {
    HAL_EINT_NUMBER_0 = 0,
    HAL_EINT_NUMBER_1 = 1,
    HAL_EINT_NUMBER_2 = 2,
    HAL_EINT_NUMBER_3 = 3,
    HAL_EINT_NUMBER_4 = 4,
    HAL_EINT_NUMBER_5 = 5,
    HAL_EINT_NUMBER_6 = 6,
    HAL_EINT_NUMBER_7 = 7,
    HAL_EINT_NUMBER_8 = 8,
    HAL_EINT_NUMBER_9 = 9,
    HAL_EINT_NUMBER_10 = 10,
    HAL_EINT_NUMBER_11 = 11,
    HAL_EINT_NUMBER_12 = 12,
    HAL_EINT_NUMBER_13 = 13,
    HAL_EINT_NUMBER_14 = 14,
    HAL_EINT_NUMBER_15 = 15,
    HAL_EINT_NUMBER_16 = 16,
    HAL_EINT_NUMBER_17 = 17,
    HAL_EINT_NUMBER_18 = 18,
    HAL_EINT_NUMBER_19 = 19,
    HAL_EINT_UART_0_RX = 20,
    HAL_EINT_UART_1_RX = 21,
    HAL_EINT_RTC       = 22,
    HAL_EINT_PMU       = 23,
    HAL_EINT_NUMBER_MAX         /**< The total number of EINT channels (invalid EINT channel). */
} hal_eint_number_t;
/**
  * @}
  */
/**
 * @}
 * @}
 */
#endif

#ifdef HAL_GPT_MODULE_ENABLED
/**
 * @addtogroup HAL
 * @{
 * @addtogroup GPT
 * @{
 * @addtogroup hal_gpt_enum Enum
 * @{
 */

/*****************************************************************************
* GPT
*****************************************************************************/
/** @brief GPT port */
typedef enum {
    HAL_GPT_0 = 0,                          /**< GPT port 0. User defined. */
    HAL_GPT_1 = 1,                          /**< GPT port 1. Usee for CM4/DSP0/DSP1 to set a millisecond delay and get 1/32Khz free count. The clock source is 32Khz*/
    HAL_GPT_2 = 2,                          /**< GPT port 2. Usee for CM4/DSP0/DSP1 to set a microsecond delay and get microsecond free count. The clock source is 1Mhz*/
    HAL_GPT_3 = 3,                          /**< GPT port 3. User defined.*/
    HAL_GPT_4 = 4,                          /**< GPT port 4. User defined.*/
    HAL_GPT_5 = 5,                          /**< GPT port 5. Used for DSP0 as software GPT. The clock source is 32Khz.*/
    HAL_GPT_6 = 6,                          /**< GPT port 6. Used for DSP0 as software GPT. The clock source is 1Mhz.*/
    HAL_GPT_MAX_PORT = 7,                  /**< The total number of GPT ports (invalid GPT port). */
    HAL_GPT_MAX = 7
} hal_gpt_port_t;

/** @brief GPT clock source  */
typedef enum {
    HAL_GPT_CLOCK_SOURCE_32K = 0,            /**< Set the GPT clock source to 32kHz, 1 tick = 1/32768 seconds. */
    HAL_GPT_CLOCK_SOURCE_1M  = 1             /**< Set the GPT clock source to 1MHz, 1 tick = 1 microsecond.*/
} hal_gpt_clock_source_t;

/** @brief  The maximum time of millisecond timer.
  */
#define HAL_GPT_MAXIMUM_MS_TIMER_TIME   (130150523)

#define HAL_GPT_SW_NUMBER    16             /**< SW GPT support maximal task number, include ms and us sw timer*/

/**
  * @}
  */
/**
 * @}
 * @}
 */
#endif

#ifdef HAL_FLASH_MODULE_ENABLED
/**
 * @addtogroup HAL
 * @{
 * @addtogroup FLASH
 * @{
 */

/*****************************************************************************
* Flash
*****************************************************************************/

/** @defgroup hal_flash_define Define
 * @{
  */

/** @brief  This macro defines the Flash base address.
  */
#ifdef __EXT_BOOTLOADER__
#define HAL_FLASH_BASE_ADDRESS    (0x08000000)
#else
#define HAL_FLASH_BASE_ADDRESS    (0x28000000)
#endif

/**
  * @}
  */
/**
 * @}
 * @}
 */
#endif


#ifdef HAL_PWM_MODULE_ENABLED
/**
 * @addtogroup HAL
 * @{
 * @addtogroup PWM
 * @{
 * @addtogroup hal_pwm_enum Enum
 * @{
 */
/*****************************************************************************
* PWM
*****************************************************************************/
/** @brief The PWM channels */
typedef enum {
    HAL_PWM_0 = 0,                            /**< PWM channel 0. */
    HAL_PWM_1 = 1,                            /**< PWM channel 1. */
    HAL_PWM_MAX,                              /**< The total number of PWM channels (invalid PWM channel).*/
} hal_pwm_channel_t;


/** @brief PWM clock source options */
typedef enum {
    HAL_PWM_CLOCK_26MHZ = 0,                /**< PWM clock srouce 26MHz. */
    HAL_PWM_CLOCK_32KHZ = 2,                /**< PWM clock srouce 32kHz. */
    HAL_PWM_CLOCK_48MHZ = 3,                /**< PWM clock srouce 48MHz. */
    HAL_PWM_CLOCK_MAX,
} hal_pwm_source_clock_t ;

/**
  * @}
  */
/**
 * @}
 * @}
 */
#endif

#ifdef HAL_WDT_MODULE_ENABLED

/**
 * @addtogroup HAL
 * @{
 * @addtogroup WDT
 * @{
*/

/* @addtogroup hal_wdt_define Define
 * @{
 */
/** @brief  This enum define the max timeout value of WDT.  */
#define HAL_WDT_MAX_TIMEOUT_VALUE (1000)
/**
 * @}
 */

/**
 * @}
 * @}
 */

#endif

#ifdef HAL_CACHE_MODULE_ENABLED
/**
 * @addtogroup HAL
 * @{
 * @addtogroup CACHE
 * @{
 */

/*****************************************************************************
* Cache
*****************************************************************************/
/* NULL */

/**
 * @}
 * @}
 */
#endif

#ifdef HAL_GPC_MODULE_ENABLED
/**
 * @addtogroup HAL
 * @{
 * @addtogroup GPC
 * @{
 * @addtogroup hal_gpc_enum Enum
 * @{
 */
/** @brief GPC port */
typedef enum {
    HAL_GPC_0 = 0,                          /**< GPC port 0. */
    HAL_GPC_MAX_PORT                        /**< The total number of GPC ports (invalid GPC port). */
} hal_gpc_port_t;


/**
  * @}
  */
/**
 * @}
 * @}
 */
#endif


#ifdef HAL_SD_MODULE_ENABLED
/**
 * @addtogroup HAL
 * @{
 * @addtogroup SD
 * @{
 * @addtogroup hal_sd_enum Enum
 * @{
 */
/*****************************************************************************
* SD
*****************************************************************************/
/** @brief  This enum defines the SD/eMMC port. */
typedef enum {
    HAL_SD_PORT_0 = 0,                                             /**<  SD/eMMC port 0. */
    HAL_SD_PORT_1 = 1                                              /**<  SD/eMMC port 1. */
} hal_sd_port_t;

/**
  * @}
  */
/**
 * @}
 * @}
 */
#endif


#ifdef HAL_SDIO_SLAVE_MODULE_ENABLED
/**
 * @addtogroup HAL
 * @{
 * @addtogroup SDIO
 * @{
 * @addtogroup hal_sdio_enum Enum
 * @{
 */
/*****************************************************************************
* SDIO
*****************************************************************************/
/** @brief  This enum defines the SDIO port.  */
typedef enum {
    HAL_SDIO_SLAVE_PORT_0 = 0,                                             /**< SDIO slave port 0. */
} hal_sdio_slave_port_t;


/**
  * @}
  */
/**
 * @}
 * @}
 */
#endif


#ifdef HAL_SDIO_MODULE_ENABLED
/**
 * @addtogroup HAL
 * @{
 * @addtogroup SDIO
 * @{
 * @addtogroup hal_sdio_enum Enum
 * @{
 */
/*****************************************************************************
* SDIO
*****************************************************************************/
/** @brief  This enum defines the SDIO port.  */
typedef enum {
    HAL_SDIO_PORT_0 = 0,                                             /**< SDIO port 0. */
    HAL_SDIO_PORT_1 = 1                                              /**< SDIO port 1. */
} hal_sdio_port_t;


/**
  * @}
  */
/**
 * @}
 * @}
 */
#endif

#ifdef HAL_CLOCK_MODULE_ENABLED

/*****************************************************************************
* Clock
*****************************************************************************/

/**
 * @addtogroup HAL
 * @{
 * @addtogroup CLOCK
 * @{
 * @addtogroup hal_clock_enum Enum
 * @{
 *
 * @section CLOCK_CG_ID_Usage_Chapter HAL_CLOCK_CG_ID descriptions
 *
 * Each #hal_clock_cg_id is related to one CG. Please check the following parameters before controlling the clock.
 *
 * The description of API parameters for HAL_CLOCK_CG_ID is listed below:
 * | HAL_CLOCK_CG_ID            |Details                                                                            |
 * |----------------------------|-----------------------------------------------------------------------------------|
 * |\b HAL_CLOCK_CG_DMA         | The CG for DMA. It is controlled in DMA driver.|
 * |\b HAL_CLOCK_CG_SDIOMST_BUS | The CG for SDIO master bus. It is controlled in SDIO driver.|
 * |\b HAL_CLOCK_CG_SW_ASYS     | The CG for I2S1. It is controlled in I2S driver.|
 * |\b HAL_CLOCK_CG_SPISLV      | The CG for SPI slave. This CG should be enabled when it is connected to the master device if choosing a custom driver.|
 * |\b HAL_CLOCK_CG_SPIMST      | The CG for SPI master. It is controlled in SPI driver.|
 * |\b HAL_CLOCK_CG_SW_AUDIO    | The CG for I2S0. It is controlled in I2S driver.|
 * |\b HAL_CLOCK_CG_SDIOMST     | The CG for SDIO master. It is controlled in SDIO driver.|
 * |\b HAL_CLOCK_CG_UART1       | The CG for UART1. It is controlled in UART driver.|
 * |\b HAL_CLOCK_CG_UART2       | The CG for UART2. It is controlled in UART driver.|
 * |\b HAL_CLOCK_CG_I2C0        | The CG for I2C0. It is controlled in I2C driver.|
 * |\b HAL_CLOCK_CG_I2C1        | The CG for I2C1. It is controlled in I2C driver.|
 * |\b HAL_CLOCK_CG_CM_SYSROM   | The CG for system ROM. It cannot be disabled, otherwise the system will fail.|
 * |\b HAL_CLOCK_CG_SFC_SW      | The CG for serial flash controller. It cannot be disabled, otherwise the system will fail.|
 * |\b HAL_CLOCK_CG_SW_TRNG     | The CG for TRNG. It is controlled in TRNG driver.|
 * |\b HAL_CLOCK_CG_SW_XTALCTL  | The CG for crystal oscillator. It cannot be disabled, otherwise the system will fail.|
 * |\b HAL_CLOCK_CG_UART0       | The CG for UART0. It cannot be disabled, otherwise the system will fail.|
 * |\b HAL_CLOCK_CG_CRYPTO      | The CG for crypto engine. It is controlled in crypto engine driver.|
 * |\b HAL_CLOCK_CG_SDIOSLV     | The CG for SDIO slave. This CG should be enabled when it is connected to the master device if choosing a custom driver.|
 * |\b HAL_CLOCK_CG_PWM0        | The CG for PWM0. It is controlled in PWM driver.|
 * |\b HAL_CLOCK_CG_PWM1        | The CG for PWM1. It is controlled in PWM driver.|
 * |\b HAL_CLOCK_CG_PWM2        | The CG for PWM2. It is controlled in PWM driver.|
 * |\b HAL_CLOCK_CG_PWM3        | The CG for PWM3. It is controlled in PWM driver.|
 * |\b HAL_CLOCK_CG_PWM4        | The CG for PWM4. It is controlled in PWM driver.|
 * |\b HAL_CLOCK_CG_PWM5        | The CG for PWM5. It is controlled in PWM driver.|
 * |\b HAL_CLOCK_CG_SW_GPTIMER  | The CG for general purpose timer. It cannot be disabled, otherwise the system will fail.|
 * |\b HAL_CLOCK_CG_SW_AUXADC   | The CG for ADC. It is controlled in ADC driver.|
 */
/** @brief Use hal_clock_cg_id in Clock API. */
/*************************************************************************
 * Define clock gating registers and bit structure.
 * Note: Mandatory, modify clk_cg_mask in hal_clock.c source file, if hal_clock_cg_id has changed.
 *************************************************************************/
typedef enum {
    /* XO_PDN_PD_COND0 = 25 */
    HAL_CLOCK_CG_UART1                     =  0,        /* bit 1, PDN_COND0_FROM */
    HAL_CLOCK_CG_I2C0                      =  2,        /* bit 3, */
    HAL_CLOCK_CG_I2C1                      =  3,        /* bit 4, */
    HAL_CLOCK_CG_I2C2                          ,        /* RESERVED */
    HAL_CLOCK_CG_SLOW_DMA_0                =  5,        /* bit 6, */
    HAL_CLOCK_CG_SLOW_DMA_1                =  6,        /* bit 7, */
    HAL_CLOCK_CG_UART0                     =  16,        /* bit 16, */
    HAL_CLOCK_CG_AUXADC                    =  17,        /* bit 17, */

    /* XO_PDN_AO_COND0 = 13 */
    HAL_CLOCK_CG_PWM0                      = (0 + 32),    /* bit 0, XO_PDN_AO_COND0 */
    HAL_CLOCK_CG_PWM1                      = (1 + 32),    /* bit 1, */
    HAL_CLOCK_CG_SPM                       = (16 + 32),    /* bit 16, */
    HAL_CLOCK_CG_I2C_AO                    = (18 + 32),    /* bit 18, */
    HAL_CLOCK_CG_OSTIMER                   = (19 + 32),    /* bit 19, */
    HAL_CLOCK_CG_GPTIMER                   = (20 + 32),    /* bit 20, */

    /* XO_PDN_TOP_COND0 = 28 */
    HAL_CLOCK_CG_SPIMST0                   = (0 + 64),    /* bit 0, XO_PDN_TOP_COND0 */
    HAL_CLOCK_CG_AUD_INTBUS                = (4 + 64),    /* bit 4, */
    HAL_CLOCK_CG_AUD_GPSRC                 = (5 + 64),    /* bit 5, */
    HAL_CLOCK_CG_AUD_UPLINK                = (6 + 64),    /* bit 6, */
    HAL_CLOCK_CG_AUD_DWLINK                = (7 + 64),    /* bit 7, */
    HAL_CLOCK_CG_AUD_INTF0                 = (8 + 64),   /* bit 8, */
    HAL_CLOCK_CG_AUD_TEST                  = (10 + 64),   /* bit 10, */
    HAL_CLOCK_CG_AUD_ANC                   = (11 + 64),   /* bit 11, */
    HAL_CLOCK_CG_SFC                       = (17 + 64),   /* bit 17, */
    HAL_CLOCK_CG_SEJ                       = (21 + 64),   /* bit 21, */

    HAL_CLOCK_CG_CM4_DMA                   = (0 + 96),    /* bit 0, PDN_PD_COND0 */
    HAL_CLOCK_CG_SPIMST0_BUS               = (1 + 96),    /* bit 1, */
    HAL_CLOCK_CG_AESOTF                    = (16 + 96),    /* bit 16, */
    HAL_CLOCK_CG_TRNG                      = (19 + 96),    /* bit 19, */
    HAL_CLOCK_CG_TIU_BUS                   = (24 + 96),   /* bit 24, */

    HAL_CLOCK_CG_CMSYS_ROM                 = (16 + 128), /*PDN_TOP_COND0*/
    HAL_CLOCK_CG_END                       = (17 + 128)
} hal_clock_cg_id;


typedef enum{
    XO_CLK_26M   = 26000,
    OSC_CLK_139M = 138670,
    OSC_CLK_416M = 416000
}osc_clk_target_freq;

typedef enum {
    clk_dsp_dcm,
    clk_bus_dcm,
    clk_sfc_dcm
}hal_clk_dcm_id;


typedef enum {
    XO_CK,                 //26m/24m
    RTC_CK,                //32k
    PAD_SOC_CK,            //NA
    CHOP_CK,               //1.3.20
    EOSC_F32K,             //NA
    DCXO_F32K,             //NA
    XOSC_F32K,             //NA
}ref_clock;

typedef enum {
    NA,
    AD_26M_DBB_1P2 = 1,        //26M/48M
    NA_1 ,
    xo_ck,                     //26M/24M
    PAD_SOC_SRC_CK,                //NA
    AD_DCXO_CLK26M,     //26M/48M
    AD_OSC_CK = 13,                 //~348M
    AD_OSC_D3_CK,              //~128M
    AD_OSC_SYNC_CK,            //~8M
    AD_APLL1_CK,          //~530M
    clk_osc1_d2 = 21,               //192M
    clk_osc1_d5,               //76.8M
    rtc_ck,                            //32k
    _hf_fsys_ck,
    clk_osc1_d3,               //76.8M
    _hf_fsfc_ck,
    _hf_fspimst0_ck=28,
    _hf_faud_intbus_ck =34,
    _hf_faud_gpsrc_ck,
    _hf_faud_uplink_ck,
    _hf_faud_dwlink_ck,
    _f_faud_intf0_ck,
    _f_faud_engine_ck=40,
    _f_frtc_ck=44,
    _f_fxo_ck,                 //26M/24M
    f_fxo_d2_ck,               //13M/12M
    f_fosc_48m_ck,             //48M
    f_fosc_26m_ck,             //25.6M
    f_fosc_13m_ck,             //12.8M
}src_clock;


typedef enum {
    CLK_SYS_SEL,
    CLK_SFC_SEL,
#ifndef __EXT_BOOTLOADER__
    CLK_SPIMST0_SEL,
    CLK_AUD_BUS_SEL,
    CLK_AUD_GPSRC_SEL,
    CLK_AUD_INTERFACE0_SEL,
    CLK_AUD_ULCK_SEL,
    CLK_AUD_DLCK_SEL,
    CLK_26M_SEL,
    CLK_AUD_ENGINE_SEL,
    CLK_MCLK_SEL,
#endif
    NR_MUXS,
#ifndef __EXT_BOOTLOADER__
    CLK_PWM0_SEL = 0x10, // periferal
    CLK_PERI_NUM,
    CLK_PWM1_SEL,
#endif
} clock_mux_sel_id;

#ifdef __EXT_BOOTLOADER__
#define CLK_AUD_INTERFACE0_SEL 0
#define CLK_MCLK_SEL           0
#endif

typedef enum {
    CLK_APLL1,
    CLK_APLL2,
    CLK_OSC,
    NR_PLLS,
    CLK_XO
} clock_pll_id;

#ifdef HAL_DVFS_MODULE_ENABLED
typedef enum {
    DVFS_LOW_SPEED_26M = 0,
    DVFS_HALF_SPEED_83M,
    DVFS_FULL_SPEED_104M,
    DVFS_HIGH_SPEED_208M,
    DVFS_MAX_SPEED,
} dvfs_frequency_t;

#endif


/**
  * @}
  */

/**
 * @}
 * @}
 */
#endif




#ifdef HAL_SW_DMA_MODULE_ENABLED
/**
 * @addtogroup HAL
 * @{
 * @addtogroup SW_DMA
 * @{
 * @defgroup hal_sw_dma_define Define
 * @{
 */

/** @brief  The maximum number of SW DMA users.
 */
#define    SW_DMA_USER_NUM  (8)

/**
 * @}
 */


/**
 * @}
 * @}
 */
#endif

/*****************************************************************************
* Audio setting
*****************************************************************************/

/** @brief Audio device. */
typedef enum {
    HAL_MT2811_AUDIO_DEVICE_NONE               = 0x0000,  /**<  No audio device is on. */
    HAL_MT2811_AUDIO_DEVICE_MAIN_MIC_L         = 0x0001,  /**<  Stream in: main mic L. */
    HAL_MT2811_AUDIO_DEVICE_MAIN_MIC_R         = 0x0002,  /**<  Stream in: main mic R. */
    HAL_MT2811_AUDIO_DEVICE_MAIN_MIC_DUAL      = 0x0003,  /**<  Stream in: main mic L+R. */
    HAL_MT2811_AUDIO_DEVICE_DIGITAL_MIC_L      = 0x0010,  /**<  Stream in: digital mic L. */
    HAL_MT2811_AUDIO_DEVICE_DIGITAL_MIC_R      = 0x0020,  /**<  Stream in: digital mic R. */
    HAL_MT2811_AUDIO_DEVICE_DIGITAL_MIC_DUAL   = 0x0030,  /**<  Stream in: digital mic L+R. */

    HAL_MT2811_AUDIO_DEVICE_DAC_L              = 0x0100,  /**<  Stream out:speaker L. */
    HAL_MT2811_AUDIO_DEVICE_DAC_R              = 0x0200,  /**<  Stream out:speaker R. */
    HAL_MT2811_AUDIO_DEVICE_DAC_DUAL           = 0x0300,  /**<  Stream out:speaker L+R. */

    HAL_MT2811_AUDIO_DEVICE_I2S_MASTER         = 0x1000,  /**<  Stream in/out: I2S master role */
    HAL_MT2811_AUDIO_DEVICE_I2S_SLAVE          = 0x2000,  /**<  Stream in/out: I2S slave role */
    HAL_MT2811_AUDIO_DEVICE_EXT_CODEC          = 0x3000,   /**<  Stream out: external amp.&codec, stereo/mono */
    HAL_MT2811_AUDIO_DEVICE_DUMMY              = 0xFFFFFFFF,   /**<  for DSP structrue alignment */
} hal_mt2811_audio_device_t;

/** @brief audio channel selection define */
typedef enum {
    HAL_MT2811_AUDIO_DIRECT                     = 0, /**< A single interconnection, output equal to input. */
    HAL_MT2811_AUDIO_SWAP_L_R                   = 2, /**< L and R channels are swapped. That is (L, R) -> (R, L). */
    HAL_MT2811_AUDIO_BOTH_L                     = 3, /**< only output L channel. That is (L, R) -> (L, L). */
    HAL_MT2811_AUDIO_BOTH_R                     = 4, /**< only output R channel. That is (L, R) -> (R, R). */
    HAL_MT2811_AUDIO_MIX_L_R                    = 5, /**< L and R channels are mixed. That is (L, R) -> (L+R, L+R). */
    HAL_MT2811_AUDIO_MIX_SHIFT_L_R              = 6, /**< L and R channels are mixed and shift. That is (L, R) -> (L/2+R/2, L/2+R/2). */
    HAL_MT2811_AUDIO_CHANNEL_DUMMY              = 0xFFFFFFFF,   /**<  for DSP structrue alignment */
} hal_mt2811_audio_channel_selection_t;



#ifdef __cplusplus
}
#endif

#endif /* __HAL_PLATFORM_H__ */

