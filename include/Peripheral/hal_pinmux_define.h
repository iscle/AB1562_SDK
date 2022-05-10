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

#ifndef __HAL_PINMUX_DEFINE_H__




#define __HAL_PINMUX_DEFINE_H__

#include "hal_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAL_GPIO_MODULE_ENABLED

#define HAL_GPIO_0_GPIO0    0
#define HAL_GPIO_0_SPDIF    1
#define HAL_GPIO_0_I2S_MST0_WS    2
#define HAL_GPIO_0_UART0_TXD    3
#define HAL_GPIO_0_PWM0    4
#define HAL_GPIO_0_CLKO0    5
#define HAL_GPIO_0_TEST_P    7
#define HAL_GPIO_0_CLKO2    8
#define HAL_GPIO_0_DMIC1_CLK    9
#define HAL_GPIO_0_RESETB_OUT    10
#define HAL_GPIO_0_DEBUGMON0    11

#define HAL_GPIO_1_GPIO1    0
#define HAL_GPIO_1_SPI_MST0_CS0    1
#define HAL_GPIO_1_I2S_MST0_RX    2
#define HAL_GPIO_1_UART0_RXD    3
#define HAL_GPIO_1_I2S_SLV0_CK    4
#define HAL_GPIO_1_I2C0_SCL    5
#define HAL_GPIO_1_DSP_JTDI    6
#define HAL_GPIO_1_TEST_N    7
#define HAL_GPIO_1_CLKO1    8
#define HAL_GPIO_1_DMIC1_DAT    9
#define HAL_GPIO_1_EINT1    10
#define HAL_GPIO_1_DEBUGMON1    11

#define HAL_GPIO_2_GPIO2    0
#define HAL_GPIO_2_SPI_MST0_CS0    1
#define HAL_GPIO_2_I2S_MST0_WS    2
#define HAL_GPIO_2_UART1_TXD    3
#define HAL_GPIO_2_I2S_SLV0_WS    4
#define HAL_GPIO_2_I2C0_SDA    5
#define HAL_GPIO_2_DSP_JRST    6
#define HAL_GPIO_2_TEST_BUS    7
#define HAL_GPIO_2_PAD_DFUNC_SFCS0    8
#define HAL_GPIO_2_DMIC0_DAT    9
#define HAL_GPIO_2_EINT2    10
#define HAL_GPIO_2_DEBUGMON2    11

#define HAL_GPIO_3_GPIO3    0
#define HAL_GPIO_3_SPI_MST0_MISO    1
#define HAL_GPIO_3_I2S_MST0_RX    2
#define HAL_GPIO_3_UART1_RXD    3
#define HAL_GPIO_3_I2S_SLV0_RX    4
#define HAL_GPIO_3_I2C0_SCL    5
#define HAL_GPIO_3_DSP_JTMS    6
#define HAL_GPIO_3_AUXADC3    7
#define HAL_GPIO_3_PAD_DFUNC_SOUT    8
#define HAL_GPIO_3_DMIC0_CLK    9
#define HAL_GPIO_3_EINT3    10
#define HAL_GPIO_3_DEBUGMON3    11

#define HAL_GPIO_4_GPIO4    0
#define HAL_GPIO_4_SPI_MST0_MOSI    1
#define HAL_GPIO_4_I2S_MST0_TX    2
#define HAL_GPIO_4_SPDIF    3
#define HAL_GPIO_4_I2S_SLV0_TX    4
#define HAL_GPIO_4_I2C1_SCL    5
#define HAL_GPIO_4_DSP_JTDO    6
#define HAL_GPIO_4_CTP1_AUXADC2    7
#define HAL_GPIO_4_PAD_DFUNC_SIN    8
#define HAL_GPIO_4_DMIC1_DAT    9
#define HAL_GPIO_4_EINT4    10
#define HAL_GPIO_4_DEBUGMON4    11

#define HAL_GPIO_5_GPIO5    0
#define HAL_GPIO_5_SPI_MST0_SCK    1
#define HAL_GPIO_5_I2S_MST0_CK    2
#define HAL_GPIO_5_UART1_RXD    3
#define HAL_GPIO_5_I2S_SLV0_CK    4
#define HAL_GPIO_5_I2C1_SDA    5
#define HAL_GPIO_5_DSP_JTDI    6
#define HAL_GPIO_5_CTP2_AUXADC1    7
#define HAL_GPIO_5_PAD_DFUNC_SFCK    8
#define HAL_GPIO_5_DMIC1_CLK    9
#define HAL_GPIO_5_EINT5    10
#define HAL_GPIO_5_DEBUGMON5    11

#define HAL_GPIO_6_GPIO6    0
#define HAL_GPIO_6_SPI_MST0_MISO    1
#define HAL_GPIO_6_I2S_MST0_MCLK    2
#define HAL_GPIO_6_UART1_TXD    3
#define HAL_GPIO_6_I2S_SLV0_WS    4
#define HAL_GPIO_6_I2C0_SDA    5
#define HAL_GPIO_6_DSP_JTCK    6
#define HAL_GPIO_6_AUXADC0    7
#define HAL_GPIO_6_UART0_RTS    8
#define HAL_GPIO_6_DMIC0_DAT    9
#define HAL_GPIO_6_EINT6    10
#define HAL_GPIO_6_DEBUGMON6    11

#define HAL_GPIO_7_GPIO7    0
#define HAL_GPIO_7_SPI_MST0_CS0    1
#define HAL_GPIO_7_I2S_MST0_WS    2
#define HAL_GPIO_7_UART1_RXD    3
#define HAL_GPIO_7_UART1_TXD    4
#define HAL_GPIO_7_I2C1_SDA    5
#define HAL_GPIO_7_DSP_JRST    6
#define HAL_GPIO_7_COEX_W1_DAT    7
#define HAL_GPIO_7_UART0_CTS    8
#define HAL_GPIO_7_DMIC1_CLK    9
#define HAL_GPIO_7_EINT7    10
#define HAL_GPIO_7_DEBUGMON7    11

#define HAL_GPIO_8_GPIO8    0
#define HAL_GPIO_8_SPI_MST0_MISO    1
#define HAL_GPIO_8_I2S_MST0_RX    2
#define HAL_GPIO_8_UART0_RTS    3
#define HAL_GPIO_8_PWM0    4
#define HAL_GPIO_8_I2C1_SCL    5
#define HAL_GPIO_8_DSP_JTMS    6
#define HAL_GPIO_8_SPDIF    7
#define HAL_GPIO_8_AUDIO_EXT_SYNC_EN    8
#define HAL_GPIO_8_DMIC1_DAT    9
#define HAL_GPIO_8_EINT8    10
#define HAL_GPIO_8_DEBUGMON8    11

#define HAL_GPIO_9_GPIO9    0
#define HAL_GPIO_9_SPI_MST0_MOSI    1
#define HAL_GPIO_9_I2S_MST1_TX    2
#define HAL_GPIO_9_UART0_CTS    3
#define HAL_GPIO_9_SPDIF    4
#define HAL_GPIO_9_PWM0    5
#define HAL_GPIO_9_DSP_JTMS    6
#define HAL_GPIO_9_COEX_W1_DAT    7
#define HAL_GPIO_9_COEX_W3_BT_WLAN    8
#define HAL_GPIO_9_DMIC0_CLK    9
#define HAL_GPIO_9_EINT9    10
#define HAL_GPIO_9_DEBUGMON9    11

#define HAL_GPIO_10_GPIO10    0
#define HAL_GPIO_10_SPI_MST0_SCK    1
#define HAL_GPIO_10_I2S_MST1_CK    2
#define HAL_GPIO_10_UART1_RTS    3
#define HAL_GPIO_10_SPDIF    4
#define HAL_GPIO_10_PWM1    5
#define HAL_GPIO_10_DSP_JTCK    6
#define HAL_GPIO_10_CLKO1    7
#define HAL_GPIO_10_COEX_W3_BT_ACT    8
#define HAL_GPIO_10_DMIC1_DAT    9
#define HAL_GPIO_10_EINT10    10
#define HAL_GPIO_10_DEBUGMON10    11

#define HAL_GPIO_11_GPIO11    0
#define HAL_GPIO_11_SPI_MST0_CS0    1
#define HAL_GPIO_11_I2S_MST1_WS    2
#define HAL_GPIO_11_UART1_CTS    3
#define HAL_GPIO_11_I2S_SLV0_WS    4
#define HAL_GPIO_11_CLKO0    5
#define HAL_GPIO_11_DSP_JTDI    6
#define HAL_GPIO_11_PAD_DFUNC_SFCS0    7
#define HAL_GPIO_11_COEX_W3_BT_PRI    8
#define HAL_GPIO_11_DMIC1_CLK    9
#define HAL_GPIO_11_EINT11    10
#define HAL_GPIO_11_DEBUGMON11    11

#define HAL_GPIO_12_GPIO12    0
#define HAL_GPIO_12_SPI_MST0_MISO    1
#define HAL_GPIO_12_I2S_MST1_RX    2
#define HAL_GPIO_12_UART1_RXD    3
#define HAL_GPIO_12_I2S_SLV0_RX    4
#define HAL_GPIO_12_I2C1_SDA    5
#define HAL_GPIO_12_DSP_JRST    6
#define HAL_GPIO_12_PAD_DFUNC_SOUT    7
#define HAL_GPIO_12_AUDIO_EXT_SYNC_EN    8
#define HAL_GPIO_12_DMIC0_DAT    9
#define HAL_GPIO_12_EINT12    10
#define HAL_GPIO_12_DEBUGMON12    11

#define HAL_GPIO_13_GPIO13    0
#define HAL_GPIO_13_SPI_MST0_MOSI    1
#define HAL_GPIO_13_I2S_MST1_MCLK    2
#define HAL_GPIO_13_UART1_TXD    3
#define HAL_GPIO_13_I2S_SLV0_TX    4
#define HAL_GPIO_13_I2C1_SCL    5
#define HAL_GPIO_13_DSP_JTDO    6
#define HAL_GPIO_13_PAD_DFUNC_SIN    7
#define HAL_GPIO_13_GPIO_PIP_SEL_0    8
#define HAL_GPIO_13_DMIC0_CLK    9
#define HAL_GPIO_13_EINT13    10
#define HAL_GPIO_13_DEBUGMON13    11

#define HAL_GPIO_14_GPIO14    0
#define HAL_GPIO_14_SPI_MST0_SIO2    1
#define HAL_GPIO_14_I2S_MST1_TX    2
#define HAL_GPIO_14_UART0_RTS    3
#define HAL_GPIO_14_I2S_SLV0_CK    4
#define HAL_GPIO_14_I2C0_SDA    5
#define HAL_GPIO_14_UART1_RTS    6
#define HAL_GPIO_14_PAD_DFUNC_SFCK    7
#define HAL_GPIO_14_GPIO_PIP_SEL_1    8
#define HAL_GPIO_14_DMIC1_DAT    9
#define HAL_GPIO_14_EINT14    10
#define HAL_GPIO_14_DEBUGMON14    11

#define HAL_GPIO_15_GPIO15    0
#define HAL_GPIO_15_SPI_MST0_SIO3    1
#define HAL_GPIO_15_I2S_MST1_CK    2
#define HAL_GPIO_15_UART0_CTS    3
#define HAL_GPIO_15_SPDIF    4
#define HAL_GPIO_15_I2C0_SCL    5
#define HAL_GPIO_15_UART1_CTS    6
#define HAL_GPIO_15_COEX_W3_BT_WLAN    7
#define HAL_GPIO_15_GPIO_PIP_SEL_2    8
#define HAL_GPIO_15_DMIC1_CLK    9
#define HAL_GPIO_15_EINT15    10
#define HAL_GPIO_15_DEBUGMON15    11

#define HAL_GPIO_16_GPIO16    0
#define HAL_GPIO_16_SPI_MST0_CS0    1
#define HAL_GPIO_16_I2S_MST0_MCLK    2
#define HAL_GPIO_16_UART1_RXD    3
#define HAL_GPIO_16_UART0_RTS    4
#define HAL_GPIO_16_I2C1_SDA    5
#define HAL_GPIO_16_SPI_MST0_SIO2    6
#define HAL_GPIO_16_COEX_W3_BT_ACT    7
#define HAL_GPIO_16_GPIO_PIP_SEL_3    8
#define HAL_GPIO_16_DMIC0_DAT    9
#define HAL_GPIO_16_EINT16    10
#define HAL_GPIO_16_DEBUGMON0    11

#define HAL_GPIO_17_GPIO17    0
#define HAL_GPIO_17_SPI_MST0_CS1    1
#define HAL_GPIO_17_I2S_MST0_TX    2
#define HAL_GPIO_17_UART1_TXD    3
#define HAL_GPIO_17_UART0_CTS    4
#define HAL_GPIO_17_I2C1_SCL    5
#define HAL_GPIO_17_SPI_MST0_SIO3    6
#define HAL_GPIO_17_COEX_W3_BT_PRI    7
#define HAL_GPIO_17_AUDIO_EXT_SYNC_EN    8
#define HAL_GPIO_17_DMIC0_CLK    9
#define HAL_GPIO_17_EINT17    10
#define HAL_GPIO_17_DEBUGMON1    11

#define HAL_GPIO_20_GPIO20    0
#define HAL_GPIO_20_SRCLKEN    1

#define HAL_GPIO_21_GPIO21    0
#define HAL_GPIO_21_WDTRSTB    1

#define HAL_GPIO_22_GPIO22    0
#define HAL_GPIO_22_I2C_AO_SCL    1

#define HAL_GPIO_23_GPIO23    0
#define HAL_GPIO_23_I2C_AO_SDA    1

#define HAL_GPIO_24_GPIO24    0
#define HAL_GPIO_24_PMIC_INT    1



#ifdef __cplusplus
}
#endif

#endif /*HAL_GPIO_MODULE_ENABLED*/

#endif /*__HAL_PINMUX_DEFINE_H__*/

