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
#include "ab1562.h"
#include "ept_gpio_drv.h"
#include "ept_eint_drv.h"
#include "bsp_gpio_ept_define.h"
#include "bsp_gpio_ept_config.h"

GPIO_BASE_REGISTER_T *ept_gpio_base = (GPIO_BASE_REGISTER_T *)(GPIO_BASE);
GPIO_CFG0_REGISTER_T *ept_gpio_cfg0 = (GPIO_CFG0_REGISTER_T *)(IO_CFG_0_BASE);

const uint32_t GPIO_REG_PIN_MASK[1] = {EPT_GPIO_PIN_MASK_0};

//#define EPT_DEBUG_LOG


/**
  * @brief  write the joint data about GPIO mode to GPIO mode register one by one
  * @param  None
  * @retval None
  */
static void gpio_mode_init(void)
{
#ifdef GPIO_MODE_REG_MAX_NUM
    uint32_t i, j;
    uint32_t reg_mask[GPIO_MODE_REG_MAX_NUM];
    uint32_t gpio_num;
    uint32_t gpio_mode[GPIO_MODE_REG_MAX_NUM] = { GPIO_MODE_ALL_VALUE };

    for (i = 0; i < GPIO_MODE_REG_MAX_NUM; i++) {

        reg_mask[i] = 0;

        for (j = 0; j < GPIO_MODE_ONE_REG_CONTROL_NUM; j++) {

            gpio_num = j + i * GPIO_MODE_ONE_REG_CONTROL_NUM; /*GPIO number*/

            /*check the mask bit is available*/
            if ((1 << (gpio_num % 32)) &  GPIO_REG_PIN_MASK[gpio_num / 32]) {
                reg_mask[i] |=  0xf << (GPIO_MODE_ONE_CONTROL_BITS * j);
            }
        }
    }

    for (i = 0; i < GPIO_MODE_REG_MAX_NUM; i++) {
        ept_gpio_base->GPIO_MODE.CLR[i] = reg_mask[i];
        ept_gpio_base->GPIO_MODE.SET[i] = gpio_mode[i];

#ifdef EPT_DEBUG_LOG
        log_hal_msgid_info("GPIO_MODE[%d]: =0x%.8x, reg_mask[%d]=0x%.8x\r\n", 4, i, gpio_mode[i], i, reg_mask[i]);
#endif

    }
#endif
}

/**
  * @brief  write the joint data about GPIO direction configuration to GPIO direction register one by one
  * @param  None
  * @retval None
  */
static void gpio_dir_init(void)
{
#ifdef GPIO_DIR_REG_MAX_NUM
    uint32_t dir_temp = GPIO_DIR_ALL_VALUE;

        ept_gpio_base->GPIO_DIR.CLR = GPIO_REG_PIN_MASK[0];
        ept_gpio_base->GPIO_DIR.SET = dir_temp;
    #ifdef EPT_DEBUG_LOG
        log_hal_msgid_info("dir_temp = 0x%.8x\r\n", 1, dir_temp);
    #endif

#endif

}

/**
  * @brief  write the joint data about GPIO pull configuration about pullsel to GPIO pullsel register one by one
  * @param  None
  * @retval None
  */
static void gpio_pu_init(void)
{
#ifdef GPIO_PU_REG_MAX_NUM
    uint32_t pullsel_temp[GPIO_PU_REG_MAX_NUM] = { GPIO_PU_ALL_VALUE };
    uint32_t reg_mask[GPIO_PU_REG_MAX_NUM] = { GPIO_PU_PD_ALL_MASK };

    ept_gpio_cfg0->GPIO_PU[0].CLR = reg_mask[0];
    ept_gpio_cfg0->GPIO_PU[0].SET = pullsel_temp[0];

    ept_gpio_cfg0->GPIO_PU[1].CLR = reg_mask[1];
    ept_gpio_cfg0->GPIO_PU[1].SET = pullsel_temp[1];

#ifdef EPT_DEBUG_LOG
    log_hal_msgid_info("cfg0_PU0: reg_mask[0]=0x%.8x\r\n", 1, reg_mask[0]);
    log_hal_msgid_info("cfg0_PU1: reg_mask[1]=0x%.8x\r\n", 1, reg_mask[1]);
    log_hal_msgid_info("cfg0_PU0: pullsel_temp[0]=0x%.8x\r\n", 1, pullsel_temp[0]);
    log_hal_msgid_info("cfg0_PU1: pullsel_temp[1]=0x%.8x\r\n", 1, pullsel_temp[1]);
#endif

#endif
}

/**
  * @brief  write the joint data about GPIO pull configuration about pullen to GPIO pullen register one by one
  * @param  None
  * @retval None
  */
static void gpio_pd_init(void)
{

#ifdef GPIO_PD_REG_MAX_NUM
    uint32_t pullsel_temp[GPIO_PD_REG_MAX_NUM] = { GPIO_PD_ALL_VALUE };
    uint32_t reg_mask[GPIO_PD_REG_MAX_NUM] = { GPIO_PU_PD_ALL_MASK };

    ept_gpio_cfg0->GPIO_PD[0].CLR = reg_mask[0];
    ept_gpio_cfg0->GPIO_PD[0].SET = pullsel_temp[0];

    ept_gpio_cfg0->GPIO_PD[1].CLR = reg_mask[1];
    ept_gpio_cfg0->GPIO_PD[1].SET = pullsel_temp[1];

#ifdef EPT_DEBUG_LOG
    log_hal_msgid_info("cfg0_PD0: reg_mask[0]=0x%.8x\r\n", 1, reg_mask[0]);
    log_hal_msgid_info("cfg0_PD1: reg_mask[1]=0x%.8x\r\n", 1, reg_mask[1]);
    log_hal_msgid_info("cfg0_PD0: pullsel_temp[0]=0x%.8x\r\n", 1, pullsel_temp[0]);
    log_hal_msgid_info("cfg0_PD1: pullsel_temp[1]=0x%.8x\r\n", 1, pullsel_temp[1]);
#endif

#endif
}



/**
  * @brief  write the joint data about GPIO output data to GPIO output data register one by one
  * @param  None
  * @retval None
  */
static void gpio_output_init(void)
{
#ifdef GPIO_OUTPUT_LEVEL_REG_MAX_NUM
    uint32_t out_temp = GPIO_OUTPUT_LEVEL_ALL_VALUE;

        ept_gpio_base->GPIO_DOUT.CLR = GPIO_REG_PIN_MASK[0];
        ept_gpio_base->GPIO_DOUT.SET = out_temp;
    #ifdef EPT_DEBUG_LOG
        log_hal_msgid_info("out_temp = 0x%.8x\r\n", 1, out_temp);
    #endif
#endif
}

//switch adc pin to digital in case of leakage
static void adc_switch_digital(void)
{
    ept_gpio_cfg0->GPIO_G.SET = 0x7f;

}
/**
  * @brief  Main program to make the configuration of EPT to take effect
  * @param  None
  * @retval None
  */
void bsp_ept_gpio_setting_init(void)
{
#ifdef EPT_DEBUG_LOG
    log_hal_msgid_info("call bsp_ept_gpio_setting_init\r\n", 0);
#endif

    gpio_output_init();
    gpio_dir_init();
    gpio_mode_init();
    gpio_pu_init();
    gpio_pd_init();
    adc_switch_digital();

}

