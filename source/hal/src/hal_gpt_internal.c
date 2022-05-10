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
 
#include "hal_gpt.h"

#ifdef HAL_GPT_MODULE_ENABLED
#include "hal_gpt_internal.h"
#include "hal_nvic.h"
#include "hal_nvic_internal.h"
//#include "hal_log.h"
#include "memory_attribute.h"
//#include "hal_clock.h"
//#include "hal_clock_internal.h"
#include <assert.h>

//GPT_REGISTER_T           *gp_gpt[HAL_GPT_MAX_PORT] = {GPT0, GPT1, GPT2, GPT3, GPT4, GPT5, GPT6, GPT7, GPT8, GPT9};
//GPT_REGISTER_GLOABL_T    *gp_gpt_glb                                     = {GPTGLB};
ATTR_ZIDATA_IN_TCM gpt_context_t g_gpt_context[HAL_GPT_SUPPORT_INT_CHANNEL];
volatile gpt_sw_context_t gpt_sw_context;
ATTR_ZIDATA_IN_TCM hal_gpt_running_status_t gpt_running_state[HAL_GPT_SUPPORT_DELAY_CHANNEL];
const hal_gpt_port_t        gpt_context_mapping[HAL_GPT_SUPPORT_INT_CHANNEL] = {HAL_GPT_SUPPORT_GPT_MAPPING};

uint32_t gpt_get_context_index(hal_gpt_port_t gpt_port)
{
    uint32_t i;
    
    for(i=0;i<HAL_GPT_SUPPORT_INT_CHANNEL;i++) {
        if(gpt_port == gpt_context_mapping[i]) {
            return i;
        }
    }
    return HAL_GPT_CONTEXT_INDEX_ERROR;
}

ATTR_TEXT_IN_TCM uint32_t gpt_current_count(GPT_REGISTER_T *gpt)
{
    return gpt->GPT_COUNT;

}

ATTR_TEXT_IN_TCM uint32_t gpt_convert_ms_to_32k_count(uint32_t ms)
{
    return ((uint32_t)(ms * 32 + (7 * ms) / 10 + (6 * ms) / 100 + (8 * ms) / 1000));
}


ATTR_TEXT_IN_TCM void gpt_open_clock_source(void)
{
#if 0
    if ( (GPT_REG(GPT_PDN_COND2) & GPT_PDN_MASK) != 0) {
        GPT_REG(GPT_PDN_CLRD2) = GPT_PDN_MASK;
    }
#endif
}


ATTR_TEXT_IN_TCM void  gpt_start_free_run_timer(GPT_REGISTER_T *gpt, uint32_t clock_source, uint32_t divide)
{
    gpt_open_clock_source();

    gpt->GPT_CLK = clock_source | divide;

    /* set to free run mode, open clock source and start counter */
    gpt->GPT_CON_UNION.GPT_CON = (uint32_t)GPT_CLOCK_UNGATE | (uint32_t)GPT_MODE_FREE_RUN | GPT_COUNT_START;

}

ATTR_TEXT_IN_TCM void gpt_delay_time(GPT_REGISTER_T *gpt, const uint32_t count)
{
    uint32_t current = 0;
    uint32_t  temp0 = 0;
    uint32_t  temp1 = 0;
    current = gpt_current_count(gpt);

    while (temp1 <= count){
        temp0 = gpt_current_count(gpt);

        if (temp0 > current) {
            temp1 = temp0 - current;
         } else {
            temp1= (0xffffffff - (current - temp0)) + 1;
        }
    }

}



void gpt_reset_default_timer(uint32_t gpt_port)
{
    GPT(gpt_port)->GPT_IRQ_EN = 0;                       /* disable interrupt */
    GPT(gpt_port)->GPT_CON_UNION.GPT_CON = 0;            /* disable timer     */
    GPT(gpt_port)->GPT_CLR = GPT_COUNT_CLEAR;            /* clear counter value */

    while (GPTGLB->GPT_CLRSTA & (1<<gpt_port));
    GPT(gpt_port)->GPT_CLK     = GPT_CLOCK_32KHZ;  /* default 32Mhz, divide 1 */
    GPT(gpt_port)->GPT_IRQ_ACK = GPT_IRQ_FLAG_ACK; /* clear interrupt status */
    GPT(gpt_port)->GPT_COMPARE = 0xffffffff;       /* set max threshold value */

}



uint32_t gpt_save_and_mask_interrupt(GPT_REGISTER_T *gpt)
{
    volatile uint32_t mask;

    mask = gpt->GPT_IRQ_EN;

    gpt->GPT_IRQ_EN  &= ~GPT_IRQ_ENABLE;

    return mask;
}

void gpt_restore_interrupt(GPT_REGISTER_T *gpt, uint32_t mask)
{
    gpt->GPT_IRQ_EN = mask;
}


void gpt_interrupt_handler(hal_nvic_irq_t irq_number)
{
    uint32_t i,index;
    volatile uint32_t mask;
    volatile uint32_t state;
    volatile uint32_t enable;

    irq_number = irq_number;

    for (i = 0; i < HAL_GPT_MAX_PORT; i++) {
        index = gpt_get_context_index(i);
        if (index == HAL_GPT_CONTEXT_INDEX_ERROR) {
            continue;
        }
        
        /* get gpt irq status */
        state  = GPT(i)->GPT_IRQ_STA;
        enable = GPT(i)->GPT_IRQ_EN;

        mask = gpt_save_and_mask_interrupt(GPT(i));

        if ((state & GPT_IRQ_FLAG_STA) && (enable & GPT_IRQ_ENABLE)) {
            /* clear interrupt status */
            GPT(i)->GPT_IRQ_ACK = GPT_IRQ_FLAG_ACK;
            
#ifdef GPT_DEBUG_LOG
            if (i != HAL_GPT_SW_MS_PORT) {
                //    log_hal_info("[GPT%d]GPT_IRQ_STA = 0x%x\r\n", (int)i, (int)GPT(i)->GPT_IRQ_STA);
            }
#endif

            if (g_gpt_context[index].callback_context.callback != NULL) {
                g_gpt_context[index].callback_context.callback(g_gpt_context[index].callback_context.user_data);
            }

        }
        gpt_restore_interrupt(GPT(i), mask);

    }

}



void gpt_nvic_register(void)
{
    static bool is_init = false;

    if (is_init == false) {
        hal_nvic_disable_irq(GPT_IRQn);
        hal_nvic_register_isr_handler(GPT_IRQn, gpt_interrupt_handler);
        hal_nvic_enable_irq(GPT_IRQn);
        is_init = true;

    }
}
uint32_t gpt_sw_get_current_time_us(GPT_REGISTER_T *gpt)
{
    uint32_t count;
        
    count = gpt_current_count(gpt);
    
    /*clear count*/
    GPT(HAL_GPT_SW_US_PORT)->GPT_CLR = GPT_COUNT_CLEAR;
    while (GPTGLB->GPT_CLRSTA & (1 << HAL_GPT_SW_US_PORT));

    return count;
}


uint32_t gpt_sw_get_current_time_ms(GPT_REGISTER_T *gpt)
{
    volatile uint32_t time;
    uint32_t count, time_s, time_ms;

    count = gpt_current_count(gpt);

    time_s = count / 32768;
    time_ms = ((count % 32768) * 1000 + 16384) / 32768;
    time = time_s * 1000 + time_ms;

    /*clear count*/
    GPT(HAL_GPT_SW_MS_PORT)->GPT_CLK= GPT_CLOCK_13MHZ | (uint32_t)GPT_DIVIDE_1;
    GPT(HAL_GPT_SW_MS_PORT)->GPT_CLR = GPT_COUNT_CLEAR;
    while (GPTGLB->GPT_CLRSTA & (1 << HAL_GPT_SW_MS_PORT));
    GPT(HAL_GPT_SW_MS_PORT)->GPT_CLK= GPT_CLOCK_32KHZ | (uint32_t)GPT_DIVIDE_1;

    return time;
}

uint32_t gpt_sw_get_free_timer(void)
{
    uint32_t i;

    for (i = 0; i < HAL_GPT_SW_NUMBER; i++) {
        if (gpt_sw_context.timer[i].is_used != true) {
            return i;
        }
    }

    return HAL_GPT_SW_NUMBER;
}
extern hal_nvic_status_t hal_nvic_save_and_set_interrupt_mask_special(uint32_t *mask);
extern hal_nvic_status_t hal_nvic_restore_interrupt_mask_special(uint32_t mask);
hal_gpt_status_t gpt_sw_timer_init(uint32_t handle, uint32_t timeout_time, gpt_callback_context_t *callback_context, sw_gpt_type_t sw_type)
{
    uint32_t current_time;
    uint32_t mask;
    uint32_t timer;
    hal_gpt_port_t gpt_port;
    static sw_gpt_type_t sw_type_port[HAL_GPI_SW_TYPE_NUMBER];
    void *p_sw_type;

    if ((handle & HAL_GPT_SW_MAGIC) != HAL_GPT_SW_MAGIC) {
        return HAL_GPT_STATUS_INVALID_PARAMETER;
    }

    timer = handle & HAL_GPT_SW_HANDLE_MASK;

    if (callback_context->callback == NULL) {
        return HAL_GPT_STATUS_INVALID_PARAMETER;
    }

    if (gpt_sw_context.timer[timer].is_running == true) {
        return HAL_GPT_STATUS_ERROR;
    }

    if (gpt_sw_context.timer[timer].is_used != true) {
        return HAL_GPT_STATUS_ERROR;
    }

    if ((timeout_time > HAL_GPT_MAXIMUM_MS_TIMER_TIME)&&(sw_type == SW_GPT_MS_TYPE)){
        return HAL_GPT_STATUS_INVALID_PARAMETER;
    }

    if (sw_type == SW_GPT_MS_TYPE) {
        gpt_port = HAL_GPT_SW_MS_PORT;
        sw_type_port[SW_GPT_MS_TYPE] = SW_GPT_MS_TYPE;
        p_sw_type = (void*)(&sw_type_port[SW_GPT_MS_TYPE]);
    }

    if (sw_type == SW_GPT_US_TYPE) {
        gpt_port = HAL_GPT_SW_US_PORT;
        sw_type_port[SW_GPT_US_TYPE] = SW_GPT_US_TYPE;
        p_sw_type = (void*)(&sw_type_port[SW_GPT_US_TYPE]);
    }

    /*ensure this section not to be interrupt*/
    hal_nvic_save_and_set_interrupt_mask_special(&mask);

    /* hal api to control timer*/
    gpt_sw_context.is_sw_gpt = true;

    if (gpt_sw_context.is_first_init[sw_type] == false) {
        hal_gpt_init(gpt_port);
        hal_gpt_register_callback(gpt_port, (hal_gpt_callback_t)gpt_sw_handler, p_sw_type);
        gpt_sw_context.is_first_init[sw_type] = true;
    }
    hal_gpt_stop_timer(gpt_port);

	current_time = 0;
    if (gpt_sw_context.is_start_from_isr[sw_type] == false) {
        if (sw_type == SW_GPT_MS_TYPE) {
            current_time = gpt_sw_get_current_time_ms(GPT(gpt_port));
        }

        if (sw_type == SW_GPT_US_TYPE) {
            current_time = gpt_sw_get_current_time_us(GPT(gpt_port));
        }
        
    }

    gpt_sw_context.absolute_time[sw_type] +=  current_time;
    gpt_sw_context.running_timer_count++;

    gpt_sw_context.timer[timer].is_running          = true;
    gpt_sw_context.timer[timer].time_out[sw_type]   = gpt_sw_context.absolute_time[sw_type] + timeout_time;
    gpt_sw_context.timer[timer].callback_context.callback   = callback_context->callback;
    gpt_sw_context.timer[timer].callback_context.user_data  = callback_context->user_data;
    gpt_sw_context.timer[timer].timer_type          = sw_type;

    gpt_sw_start_timer(sw_type);

    /*restore mask*/
    gpt_sw_context.is_sw_gpt = false;
    hal_nvic_restore_interrupt_mask_special(mask);
    return HAL_GPT_STATUS_OK;

}


hal_gpt_status_t gpt_sw_timer_stop(uint32_t handle,sw_gpt_type_t sw_type)
{
    uint32_t current_time;
    uint32_t mask;
    uint32_t timer;
    hal_gpt_port_t gpt_port;

    if ((handle & HAL_GPT_SW_MAGIC) != HAL_GPT_SW_MAGIC) {
        return HAL_GPT_STATUS_INVALID_PARAMETER;
    }

    timer = handle & HAL_GPT_SW_HANDLE_MASK;

    if (gpt_sw_context.timer[timer].is_running != true) {
        return HAL_GPT_STATUS_ERROR;
    }

    if (gpt_sw_context.timer[timer].is_used != true) {
        return HAL_GPT_STATUS_ERROR;
    }

   if (sw_type == SW_GPT_MS_TYPE) {
        gpt_port = HAL_GPT_SW_MS_PORT;
    }

    if (sw_type == SW_GPT_US_TYPE) {
        gpt_port = HAL_GPT_SW_US_PORT;
    }


    /*ensure this section not to be interrupt*/
    hal_nvic_save_and_set_interrupt_mask_special(&mask);

    if (timer == gpt_sw_context.current_timer) {
        gpt_sw_context.is_sw_gpt = true;
        hal_gpt_stop_timer(gpt_port);
        gpt_sw_context.is_sw_gpt = false;

        current_time = 0;
        if (gpt_sw_context.is_start_from_isr[sw_type] == false) {
            if (sw_type == SW_GPT_MS_TYPE) {
                current_time = gpt_sw_get_current_time_ms(GPT(gpt_port));
            }

            if (sw_type == SW_GPT_US_TYPE) {
                current_time = gpt_sw_get_current_time_us(GPT(gpt_port));
            }
        } else {
            current_time = 0;
        }

        gpt_sw_context.absolute_time[sw_type] +=  current_time;
        gpt_sw_context.running_timer_count--;
        gpt_sw_context.timer[timer].is_running = false;

        if (gpt_sw_context.running_timer_count != 0) {
            gpt_sw_start_timer(sw_type);
        } else {
        }
    }else   {
        gpt_sw_context.running_timer_count--;
        gpt_sw_context.timer[timer].is_running = false;
    }	
    
    hal_nvic_restore_interrupt_mask_special(mask);

    return HAL_GPT_STATUS_OK;

}

hal_gpt_status_t gpt_sw_get_remaining_time(uint32_t handle,uint32_t *remaing_time,sw_gpt_type_t sw_type)
{
    uint32_t current_time;
    uint32_t total;
    uint32_t mask;
    uint32_t timer;
	uint32_t count, time_s, time_ms;

    if ((handle & HAL_GPT_SW_MAGIC) != HAL_GPT_SW_MAGIC) {
        return HAL_GPT_STATUS_INVALID_PARAMETER;
    }

    timer = handle & HAL_GPT_SW_HANDLE_MASK;

    if (gpt_sw_context.timer[timer].is_used != true) {
        return HAL_GPT_STATUS_ERROR;
    }

    /*ensure this section not to be interrupt*/
    hal_nvic_save_and_set_interrupt_mask(&mask);

	current_time = 0;
    if (sw_type == SW_GPT_MS_TYPE) {
		count 	= gpt_current_count(GPT(HAL_GPT_SW_MS_PORT));
	    time_s 	= count / 32768;
	    time_ms = ((count % 32768) * 1000 + 16384) / 32768;
	    current_time = time_s * 1000 + time_ms;
    }

    if (sw_type == SW_GPT_US_TYPE) {
        current_time = gpt_current_count(GPT(HAL_GPT_SW_US_PORT));
    }

    total = gpt_sw_context.absolute_time[sw_type] + current_time;

    if (gpt_sw_context.timer[timer].time_out[sw_type] > total) {
        *remaing_time = gpt_sw_context.timer[timer].time_out[sw_type] - total;
    } else {
        *remaing_time = 0;
    }

    hal_nvic_restore_interrupt_mask(mask);

    return HAL_GPT_STATUS_OK;

}


void gpt_sw_start_timer(sw_gpt_type_t sw_type)
{
    uint32_t minimum_time, current_timer;

    if (gpt_sw_context.is_start_from_isr[sw_type] == true) {
        return;
    }

    gpt_sw_get_minimum_left_time(&minimum_time, &current_timer,sw_type);
    gpt_sw_context.current_timer = current_timer;

    /*restore this absolute time*/
    gpt_sw_context.last_absolute_time[sw_type] = gpt_sw_context.absolute_time[sw_type];

    gpt_sw_context.is_sw_gpt = true;
    if (SW_GPT_MS_TYPE == sw_type) {
        hal_gpt_start_timer_ms(HAL_GPT_SW_MS_PORT, minimum_time, HAL_GPT_TIMER_TYPE_ONE_SHOT);
    }

    if (SW_GPT_US_TYPE == sw_type) {
        hal_gpt_start_timer_us(HAL_GPT_SW_US_PORT, minimum_time, HAL_GPT_TIMER_TYPE_ONE_SHOT);
    }
    
    gpt_sw_context.is_sw_gpt = false;
    
}

uint32_t gpt_sw_absolute_value(uint32_t timer_number,sw_gpt_type_t sw_type)
{
    uint32_t time_out_delta;
    uint32_t current_delta;
    uint32_t ret_value;

    time_out_delta = gpt_sw_delta(gpt_sw_context.timer[timer_number].time_out[sw_type], gpt_sw_context.last_absolute_time[sw_type]);
    current_delta  = gpt_sw_delta(gpt_sw_context.absolute_time[sw_type], gpt_sw_context.last_absolute_time[sw_type]);

    if (time_out_delta > current_delta) {
        /*timer has not been expired*/
        ret_value = gpt_sw_delta(gpt_sw_context.timer[timer_number].time_out[sw_type], gpt_sw_context.absolute_time[sw_type]);
    } else {
        /*timer has been expired*/
        ret_value =  0;
    }

    return ret_value;
}
void gpt_sw_get_minimum_left_time(uint32_t *minimum_time, uint32_t *number,sw_gpt_type_t sw_type)
{
    uint32_t i;
    uint32_t minimum = 0xffffffff;
    uint32_t data;

    for (i = 0; i < HAL_GPT_SW_NUMBER; i++) {
        if ((gpt_sw_context.timer[i].is_running == true)&&(gpt_sw_context.timer[i].timer_type == sw_type)) {
            data = gpt_sw_absolute_value(i,sw_type);

            if (data < minimum) {
                minimum = data;
                *number = i;
            }
        }
    }

    *minimum_time = minimum;
}


void gpt_sw_handler(sw_gpt_type_t *sw_type_port)
{
    uint32_t i;
    volatile uint32_t time;
    volatile uint32_t data;
    sw_gpt_type_t sw_type;

    if (gpt_sw_context.used_timer_count != 0) {
        sw_type = *sw_type_port;
        
        gpt_sw_context.is_sw_gpt = true;
        if (sw_type == SW_GPT_MS_TYPE) {                
            hal_gpt_stop_timer(HAL_GPT_SW_MS_PORT);
            time = gpt_sw_get_current_time_ms(GPT(HAL_GPT_SW_MS_PORT));
        }

        if (sw_type == SW_GPT_US_TYPE) {
            hal_gpt_stop_timer(HAL_GPT_SW_US_PORT);
            time = gpt_sw_get_current_time_us(GPT(HAL_GPT_SW_US_PORT));
        }
        gpt_sw_context.is_sw_gpt = false;

        gpt_sw_context.absolute_time[sw_type] += time;
        gpt_sw_context.is_start_from_isr[sw_type] = true;

        //printf("\r\n sw_type=%d\r\n",sw_type);
        for (i = 0; i < HAL_GPT_SW_NUMBER; i++) {
            if ((gpt_sw_context.timer[i].is_running == true) && (gpt_sw_context.timer[i].timer_type == sw_type)) {

                data = gpt_sw_absolute_value(i,sw_type);
                //printf("i=%d, data=%d\r\n", i, data);

                if (data == 0) {
                    gpt_sw_context.timer[i].is_running = false;
                    gpt_sw_context.running_timer_count--;

                    gpt_sw_context.timer[i].callback_context.callback(gpt_sw_context.timer[i].callback_context.user_data);
                }
            }
        }

        gpt_sw_context.is_start_from_isr[sw_type] = false;

        if (gpt_sw_context.running_timer_count != 0) {
            gpt_sw_start_timer(sw_type);
        }
    }

}


#endif /* HAL_GPT_MODULE_ENABLED */


