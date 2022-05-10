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
#include "hal.h"


/*
    1, get HAL_UART0_RTS_PIN pin state.

*    - Step 1. Call #hal_gpio_get_pin_num_from_gpio_var_name(HAL_UART0_RTS_PIN) to get the pin number that used for UART0_RTS.
*    - Step 2. Call #hal_gpio_get_input(uart0_rts_pin, data) to get the UART0_RTS's state.

      Code:
             hal_gpio_pin_t uart0_rts_pin;
             hal_gpio_data_t data;

             uart0_rts_pin = hal_gpio_get_pin_num_from_gpio_var_name(HAL_UART0_RTS_PIN);
             hal_gpio_get_input(uart0_rts_pin, &data);



    2, config HAL_EINTKEY0_EINT pin.


*    - Step 1. Call #hal_gpio_get_eint_num_from_eint_var_name(HAL_EINTKEY0_EINT) to get the eint number that used for HAL_EINTKEY0_EINT.
*    - Step 2. Call #hal_eint_xxx to config BSP_EINTKEY0_EINT.


    Code:
        hal_eint_number_t eint_num;

        eint_num = hal_gpio_get_eint_num_from_eint_var_name(HAL_EINTKEY0_EINT);

        if (HAL_EINT_STATUS_OK !=  hal_eint_init(eint_number, &eint_config)) {
                // error handle
        }
        if (HAL_EINT_STATUS_OK !=  hal_eint_register_callback(eint_number, callback, user_data)) {
                // error handle
        }

*/


#define RTC_GPIO_NUM  (3)
#define RTC_GPIO_BASE_NUM  (0x80)

#define RTC_GPIO_0    (RTC_GPIO_BASE_NUM + 0)
#define RTC_GPIO_1    (RTC_GPIO_BASE_NUM + 1)
#define RTC_GPIO_2    (RTC_GPIO_BASE_NUM + 2)

<<<<<<< HEAD
=======
#define BOND_DIS_EMEM_MASK   (1 << 27)
#define BOND_DIS_WBANC_MASK  (1 << 28)
#define BOND_DIS_HYBANC_MASK (1 << 29)
#define BOND_DIS_AUDEQ_MASK  (1 << 30)
#define BOND_PKG_TYPE_0_MASK (1 << 31)
#define BOND_PKG_TYPE_1_MASK (1 << 0)

>>>>>>> db20e11 (second commit)
typedef enum {
    GPIO_PULL_UP = 0,
    GPIO_PULL_DOWN = 1,
    GPIO_PULL_DISABLE = 2,
} gpio_pull_state_t;

typedef enum {
    HAL_UART0_TXD_PIN            = 0x00,
    HAL_UART0_RXD_PIN            = 0x01,
    HAL_UART0_RTS_PIN            = 0x02,
    HAL_UART0_CTS_PIN            = 0x03,
    HAL_UART1_TXD_PIN            = 0x04,
    HAL_UART1_RXD_PIN            = 0x05,
    HAL_UART1_RTS_PIN            = 0x06,
    HAL_UART1_CTS_PIN            = 0x07,

    HAL_SPI_MST0_CS0_PIN         = 0x08,
    HAL_SPI_MST0_CS1_PIN         = 0x09,
    HAL_SPI_MST0_MISO_PIN        = 0x0A,
    HAL_SPI_MST0_MOSI_PIN        = 0x0B,
    HAL_SPI_MST0_SIO2_PIN        = 0x0C,
    HAL_SPI_MST0_SIO3_PIN        = 0x0D,

    HAL_I2S_MST0_CK_PIN          = 0x0E,
    HAL_I2S_MST0_MCLK_PIN        = 0x0F,
    HAL_I2S_MST0_RX_PIN          = 0x10,
    HAL_I2S_MST0_TX_PIN          = 0x11,
    HAL_I2S_MST0_WS_PIN          = 0x12,
    HAL_I2S_MST1_CK_PIN          = 0x13,
    HAL_I2S_MST1_MCLK_PIN        = 0x14,
    HAL_I2S_MST1_RX_PIN          = 0x15,
    HAL_I2S_MST1_TX_PIN          = 0x16,
    HAL_I2S_MST1_WS_PIN          = 0x17,

    HAL_I2S_SLV0_CK_PIN          = 0x18,
    HAL_I2S_SLV0_RX_PIN          = 0x19,
    HAL_I2S_SLV0_TX_PIN          = 0x1A,
    HAL_I2S_SLV0_WS_PIN          = 0x1B,

    BSP_EINTKEY0_PIN             = 0x1C,
    BSP_EINTKEY1_PIN             = 0x1D,
    BSP_EINTKEY2_PIN             = 0x1E,
    BSP_EINTKEY3_PIN             = 0x1F,

    BSP_DMIC0_CLK                = 0x20,
    BSP_DMIC0_DAT                = 0x21,
    BSP_DMIC1_CLK                = 0x22,
    BSP_DMIC1_DAT                = 0x23,

    BSP_CTP_CH1                  = 0x24,
    BSP_CTP_CH2                  = 0x25,

    NTC_ADC_PIN                  = 0x26,

    HAL_POWER_KEY_PIN            = 0x27,
    MMI_ADUIO_CHANNEL_DETECT_PIN = 0x28,

    MMI_KEY1_PIN                 = 0x29,
    MMI_KEY2_PIN                 = 0x2A,
    MMI_KEY3_PIN                 = 0x2B,
    MMI_KEY4_PIN                 = 0x2C,
    MMI_KEY5_PIN                 = 0x2D,
    MMI_KEY6_PIN                 = 0x2E,
    MMI_KEY7_PIN                 = 0x2F,
    MMI_KEY8_PIN                 = 0x30,
    MMI_KEY9_PIN                 = 0x31,
    MMI_KEY10_PIN                = 0x32,

    DSP_AUDIO_OUTPUT_PIN         = 0x33,
    HAL_SPDIF_TX_PIN             = 0x34,

    HAL_I2C0_SDA_PIN             = 0x35,
    HAL_I2C0_SCL_PIN             = 0x36,
    HAL_I2C1_SDA_PIN             = 0x37,
    HAL_I2C1_SCL_PIN             = 0x38,

    GPIO_VAR_NAME_MAX,
    GPIO_VAR_NAME_NOT_DEFINED    = 0xFF,
} gpio_var_name_t;

typedef enum {
    HAL_EINTKEY0_EINT    = 0x00,
    HAL_EINTKEY1_EINT    = 0x01,
    HAL_EINTKEY2_EINT    = 0x02,
    HAL_EINTKEY3_EINT    = 0x03,

    EINT_VAR_NAME_MAX,
    EINT_VAR_NAME_NOT_DEFINED = 0xFF,
} eint_var_name_t;

#if 0
typedef struct {
    hal_gpio_pin_t gpio_num;
    hal_pinmux_select_t gpio_pinmux;
    gpio_pull_state_t gpio_pupd;
    hal_gpio_direction_t gpio_direction;
    hal_gpio_data_t gpio_output;
    gpio_var_name_t gpio_var_name; /*same to tool define*/
    eint_var_name_t eint_var_name;
    uint8_t reserve;
} gpio_info_t;
#endif
typedef struct {
    uint8_t gpio_num;
    uint8_t gpio_pinmux;
    uint8_t gpio_pupd;
    uint8_t gpio_direction;
    uint8_t gpio_output;
    uint8_t gpio_var_name; /*same to tool define*/
    uint8_t eint_var_name;
    uint8_t reserve;
} gpio_info_t;

typedef struct {
    uint8_t total_gpio_num;
    gpio_info_t gpio_info[HAL_GPIO_MAX + RTC_GPIO_NUM];
} gpio_nvkey_t;


hal_gpio_pin_t hal_gpio_get_pin_num_from_gpio_var_name(gpio_var_name_t gpio_var_name);
hal_gpio_pin_t hal_gpio_get_pin_num_from_eint_var_name(eint_var_name_t eint_var_name);
hal_gpio_status_t hal_gpio_get_all_pin_config_from_eint_var_name(gpio_nvkey_t *all_gpio_nvkey);
hal_eint_number_t hal_gpio_get_eint_num_from_eint_var_name(eint_var_name_t eint_var_name);
gpio_var_name_t hal_gpio_get_gpio_var_name(hal_gpio_pin_t gpio_num);
hal_gpio_status_t hal_gpio_set_output_with_gpio_var_name(gpio_var_name_t gpio_var_name, hal_gpio_data_t gpio_data);
void    gpio_config_all(void);
void    rtc_gpio_config_all(void);
