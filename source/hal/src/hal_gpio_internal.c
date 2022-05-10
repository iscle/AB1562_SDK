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

#include "hal_gpio.h"

#ifdef HAL_GPIO_MODULE_ENABLED
#include "hal_gpio_internal.h"
#include "ab1562.h"

#ifdef __cplusplus
extern "C" {
#endif

extern GPIO_BASE_REGISTER_T *gpio_base;

const hal_gpio_cfg_reg_t gpio_cfg_table[] = {
<<<<<<< HEAD
       //DRV_REG     shift    IES_REG    shift     PD_REG         PU_REG        shift     PUPD_REG      R0_REG       R1_REG      shift       RDSEL_RG  shift    SMT_REG       SR_REG      shift
=======
       //DRV_REG   shift IES_REG    shift  PD_REG   PU_REG     shift PUPD_REG      R0_REG       R1_REG    shift  RDSEL_RG  shift  SMT_REG    SR_REG      shift
>>>>>>> db20e11 (second commit)
    {   0xa20c0000,  0, 0xa20c0040,  0, 0xa20c0060, 0xa20c0080,  0, 0xffffffff, 0xffffffff, 0xffffffff, 0xff,  0xa20c00a0,  0, 0xa20c00d0, 0xa20c00f0,  0, },  // HAL_GPIO_0
    {   0xa20c0000,  2, 0xa20c0040,  1, 0xa20c0060, 0xa20c0080,  1, 0xffffffff, 0xffffffff, 0xffffffff, 0xff,  0xa20c00a0,  2, 0xa20c00d0, 0xa20c00f0,  1, },  // HAL_GPIO_1
    {   0xa20c0000,  4, 0xa20c0040,  2, 0xa20c0060, 0xa20c0080,  2, 0xffffffff, 0xffffffff, 0xffffffff, 0xff,  0xa20c00a0,  4, 0xa20c00d0, 0xa20c00f0,  2, },  // HAL_GPIO_2
    {   0xa20c0000,  6, 0xa20c0040,  3, 0xa20c0060, 0xa20c0080,  3, 0xffffffff, 0xffffffff, 0xffffffff, 0xff,  0xa20c00a0,  6, 0xa20c00d0, 0xa20c00f0,  3, },  // HAL_GPIO_3
    {   0xa20c0000,  8, 0xa20c0040,  4, 0xa20c0060, 0xa20c0080,  4, 0xffffffff, 0xffffffff, 0xffffffff, 0xff,  0xa20c00a0,  8, 0xa20c00d0, 0xa20c00f0,  4, },  // HAL_GPIO_4
    {   0xa20c0000, 10, 0xa20c0040,  5, 0xa20c0060, 0xa20c0080,  5, 0xffffffff, 0xffffffff, 0xffffffff, 0xff,  0xa20c00a0, 10, 0xa20c00d0, 0xa20c00f0,  5, },  // HAL_GPIO_5
    {   0xa20c0000, 12, 0xa20c0040,  6, 0xa20c0060, 0xa20c0080,  6, 0xffffffff, 0xffffffff, 0xffffffff, 0xff,  0xa20c00a0, 12, 0xa20c00d0, 0xa20c00f0,  6, },  // HAL_GPIO_6
    {   0xa20c0000, 14, 0xa20c0040,  7, 0xa20c0060, 0xa20c0080,  7, 0xffffffff, 0xffffffff, 0xffffffff, 0xff,  0xa20c00a0, 14, 0xa20c00d0, 0xa20c00f0,  7, },  // HAL_GPIO_7
    {   0xa20c0000, 16, 0xa20c0040,  8, 0xa20c0060, 0xa20c0080,  8, 0xffffffff, 0xffffffff, 0xffffffff, 0xff,  0xa20c00a0, 16, 0xa20c00d0, 0xa20c00f0,  8, },  // HAL_GPIO_8
    {   0xa20c0000, 18, 0xa20c0040,  9, 0xa20c0060, 0xa20c0080,  9, 0xffffffff, 0xffffffff, 0xffffffff, 0xff,  0xa20c00a0, 18, 0xa20c00d0, 0xa20c00f0,  9, },  // HAL_GPIO_9
    {   0xa20c0000, 20, 0xa20c0040, 10, 0xa20c0060, 0xa20c0080, 10, 0xffffffff, 0xffffffff, 0xffffffff, 0xff,  0xa20c00a0, 20, 0xa20c00d0, 0xa20c00f0, 10, },  // HAL_GPIO_10
    {   0xa20c0000, 22, 0xa20c0040, 11, 0xa20c0060, 0xa20c0080, 11, 0xffffffff, 0xffffffff, 0xffffffff, 0xff,  0xa20c00a0, 22, 0xa20c00d0, 0xa20c00f0, 11, },  // HAL_GPIO_11
	{   0xa20c0000, 24, 0xa20c0040, 12, 0xa20c0060, 0xa20c0080, 12, 0xffffffff, 0xffffffff, 0xffffffff, 0xff,  0xa20c00a0, 24, 0xa20c00d0, 0xa20c00f0, 12, },  // HAL_GPIO_12
    {   0xa20c0000, 26, 0xa20c0040, 13, 0xa20c0060, 0xa20c0080, 13, 0xffffffff, 0xffffffff, 0xffffffff, 0xff,  0xa20c00a0, 26, 0xa20c00d0, 0xa20c00f0, 13, },  // HAL_GPIO_13
    {   0xa20c0000, 28, 0xa20c0040, 14, 0xa20c0060, 0xa20c0080, 14, 0xffffffff, 0xffffffff, 0xffffffff, 0xff,  0xa20c00a0, 28, 0xa20c00d0, 0xa20c00f0, 14, },  // HAL_GPIO_14
    {   0xa20c0000, 30, 0xa20c0040, 15, 0xa20c0060, 0xa20c0080, 15, 0xffffffff, 0xffffffff, 0xffffffff, 0xff,  0xa20c00a0, 30, 0xa20c00d0, 0xa20c00f0, 15, },  // HAL_GPIO_15
    {   0xa20c0010,  0, 0xa20c0040, 16, 0xa20c0060, 0xa20c0080, 16, 0xffffffff, 0xffffffff, 0xffffffff, 0xff,  0xa20c00b0,  0, 0xa20c00d0, 0xa20c00f0, 16, },  // HAL_GPIO_16
    {   0xa20c0010,  2, 0xa20c0040, 17, 0xa20c0060, 0xa20c0080, 17, 0xffffffff, 0xffffffff, 0xffffffff, 0xff,  0xa20c00b0,  2, 0xa20c00d0, 0xa20c00f0, 17, },  // HAL_GPIO_17
};


void gpio_get_state(hal_gpio_pin_t gpio_pin, gpio_state_t *gpio_state)
{

    uint32_t mode;
    uint32_t dir;
    uint32_t din;
    uint32_t dout;
    uint32_t pu;
    uint32_t pd;
    uint32_t pupd;
    uint32_t r0;
    uint32_t r1;

    gpio_pull_type_t pull_type;
    uint32_t temp;
    uint32_t shift;
    uint32_t reg_index;
    uint32_t bit_index;
    hal_gpio_driving_current_t driving_value;

    //const char *direct[2] = {"input", "output"};
    //const char *pull_state[10] = {"disable_pull", "PU_R", "PD_R", "PU_R0","PD_R0", "PU_R0_R1", "PD_R0_R1", "PUPD_Error","PU_R1","PD_R1"};


    reg_index = gpio_pin / 8;
    bit_index = (gpio_pin % 8) * 4;
    mode = (gpio_base->GPIO_MODE.RW[reg_index] >> (bit_index) & 0xf);

    reg_index = gpio_pin / 32;
    bit_index = gpio_pin % 32;
    dir  = (gpio_base->GPIO_DIR.RW >> (bit_index) & 0x1);
    din  = (gpio_base->GPIO_DIN.R >> (bit_index) & 0x1);
    dout = (gpio_base->GPIO_DOUT.RW >> (bit_index) & 0x1);

    pu = 0xf;
    pd = 0xf;
    pupd = 0xf;
    r0   = 0xf;
    r1   = 0xf;

    shift = 0xff;
    pull_type = GPIO_PUPD_ERR;

    if (gpio_cfg_table[gpio_pin].pupd_shift != 0xff) {
        shift = gpio_cfg_table[gpio_pin].pupd_shift;
        pu = (GPIO_REG32(gpio_cfg_table[gpio_pin].pu_reg) >> shift) & 0x01;
        pd = (GPIO_REG32(gpio_cfg_table[gpio_pin].pd_reg) >> shift) & 0x01;

        temp = (pu << 4) + pd;

        if (temp == 0x00) {
            pull_type = GPIO_NO_PULL;
        } else if (temp == 0x10) {
            pull_type = GPIO_PU_R;
        } else if (temp == 0x01) {
            pull_type = GPIO_PD_R;
        } else if (temp == 0xff) {
            pull_type = GPIO_PUPD_ERR;
        }
    } else if (gpio_cfg_table[gpio_pin].pupd_r0_r1_shift != 0xff) {
        shift = gpio_cfg_table[gpio_pin].pupd_r0_r1_shift;
        pupd = (GPIO_REG32(gpio_cfg_table[gpio_pin].pupd_reg) >> shift) & 0x01;
        r0 = (GPIO_REG32(gpio_cfg_table[gpio_pin].r0_reg) >> shift) & 0x01;
        r1 = (GPIO_REG32(gpio_cfg_table[gpio_pin].r1_reg) >> shift) & 0x01;

        temp = (pupd << 8) + (r0 << 4) + r1;

        if (temp == 0x010) {
            pull_type = GPIO_PU_R0;
        } else if (temp == 0x001) {
            pull_type = GPIO_PU_R1;
        } else if (temp == 0x110) {
            pull_type = GPIO_PD_R0;
        } else if (temp == 0x101) {
            pull_type = GPIO_PD_R1;
        } else if (temp == 0x011) {
            pull_type = GPIO_PU_R0_R1;
        } else if (temp == 0x111) {
            pull_type = GPIO_PD_R0_R1;
        } else if ((temp == 0x100) || (temp == 0x000)) {
            pull_type = GPIO_NO_PULL;
        } else if (temp == 0xfff) {
            pull_type = GPIO_PUPD_ERR;
        }

    }


    hal_gpio_get_driving_current((hal_gpio_pin_t)gpio_pin, &driving_value);

    gpio_state->mode = mode;
    gpio_state->dir  = dir;
    gpio_state->din  = din;
    gpio_state->dout = dout;
    gpio_state->pull_type = pull_type;
    gpio_state->current_type = (uint8_t)driving_value;

}

#ifdef __cplusplus
}
#endif

#endif

