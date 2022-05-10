/*
 * FreeRTOS Kernel V10.1.1
 * Copyright (C) 2018 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */
#define SLEEP_IT FALSE


/*-----------------------------------------------------------
 * Implementation of functions defined in portable.h for the ARM CM4F port.
 *----------------------------------------------------------*/
#include <assert.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
//#include "port_tick.h"
//#include "hal_clock.h"
#include "hal_nvic.h"
#include "hal_gpt.h"
#include "types.h"
#include "system.h"
#include "hal_captouch.h"
#include "hal_platform.h"
#include "log.h"
#include "hal_sleep_manager.h"
#include "hal_sleep_manager_platform.h"
#include "hal_sleep_manager_internal.h"
#include "hal_rtc.h"
#include "hal_pmu.h"
#include "hal_wdt.h"
#include "nvkey.h"
#include "nvkey_list.h"

#ifdef MTK_SYSTEM_HANG_TRACER_ENABLE
#include "systemhang_tracer.h"
#endif /* MTK_SYSTEM_HANG_TRACER_ENABLE */

#if ( configUSE_TICKLESS_IDLE == 1 )

GPT_REGISTER_T *os_gpt0 = OS_GPT0;
OS_GPT_REGISTER_GLOABL_T *os_gpt_glb = OS_GPTGLB;
U16 systemCounter;
U32 lastTick;
U32 last32KTick;
U32 clkSum;
U32 sleepRemainder;

extern void xPortSysTickHandler(void);
extern void osStatusNotify(void);
extern U32 systemTick;
extern U32 LC_TimerStatusCheck(U32 SleepTime, U32* btsleeptime);
extern void      xthal_set_intenable( unsigned );
extern unsigned  xthal_get_interrupt( void );
extern U8  osTaskStatus(VOID);
extern U8  isKernelInit;
extern U32 sleepTime;
extern U32 frozenTime;
extern void DRV_PMU_StoreSignal(U8 Signal);
extern U32 port_read_btClk(VOID);
extern VOID port_btClk_26M(VOID);
extern VOID port_btClk_32K(VOID);
extern hal_rtc_status_t hal_rtc_set_alarm_by_second(uint32_t second);
extern U8 IsLowPwrMode;
extern bool uart_agree_logging_enter_sleep(void);
extern sleep_management_time_check_t sleep_management_time_check;
#if 0
U32 remainder;
void port_sleep_tick_calibration(void)
{
    U32 cur32kClk;
    U32 tickCompensate = 0;
    U32 curTick    = xTaskGetTickCount();

    port_btClk_26M();

    //hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &cur32kClk);
    cur32kClk = port_read_btClk();

    U32 tickDiff = curTick   - lastTick;
    U32 clkDiff  = cur32kClk - last32KTick;

    //tickDiff = tickDiff*32768/1000;
    U32 tickDifftmp = tickDiff*16/5;

    remainder += tickDiff - tickDifftmp*5;

    if(remainder >= 5)
    {
        tickDifftmp++;
    }

    if(clkDiff >= tickDifftmp)
    {
        tickCompensate = (clkDiff - tickDifftmp + clkSum)/4;

        if(tickCompensate)
        {
            if(tickCompensate < 10)
            {
                for(U32 i = 0; i < tickCompensate; i++)
                {
                    xTaskIncrementTick();
                }
            }
            LOG_COMMON_MSGID_INFO("tickCompensate[%d] tickDiff[%d] clkDiff[%d] clkSum[%d] cur32kClk[%d] curTick[%d]",6
                , tickCompensate, tickDifftmp, clkDiff, clkSum, cur32kClk, curTick);

        }
        clkSum      = (clkDiff - tickDifftmp + clkSum) - tickCompensate*4;
        last32KTick = cur32kClk;
        lastTick    = curTick;
    }

}
#endif

void os_gpt_start_ms_for_tickless(uint32_t timeout_time_ms)
{
    uint32_t mask;

    if(timeout_time_ms > 130150523 )
    {
        assert(0);
    }


    hal_nvic_save_and_set_interrupt_mask(&mask);
    os_gpt0->GPT_IRQ_ACK = 0x1;
    os_gpt0->GPT_CON_UNION.GPT_CON |= (1 << 16);
    os_gpt0->GPT_CLK= 0x10;
    os_gpt0->GPT_CON_UNION.GPT_CON &= ~(1 << 16);
    os_gpt0->GPT_CLR      = 0x1;
    while (os_gpt_glb->OS_GPT_CLRSTA & 0x1);

    os_gpt0->GPT_CON_UNION.GPT_CON &= ~(3<<8);
    os_gpt0->GPT_IRQ_EN =0x1;
    os_gpt0->GPT_COMPARE= (timeout_time_ms * 32 + (7 * timeout_time_ms) / 10 + (6 * timeout_time_ms) / 100 + (8 * timeout_time_ms) / 1000);
    while (os_gpt_glb->OS_GPT_WCOMPSTA & 0x1);
    os_gpt0->GPT_CON_UNION.GPT_CON |= 0x01;



    hal_nvic_disable_irq(OS_GPT_IRQn);
    // this callback should not execute, because tickless will clear & stop this timer before unmask all IRQ when exit sleep
    hal_nvic_register_isr_handler(OS_GPT_IRQn, NULL);
    hal_nvic_enable_irq(OS_GPT_IRQn);

    os_gpt_glb->OS_GPT_IRQMSK    = 0x0;  //DSP0 IRQ enable
    os_gpt_glb->OS_GPT_WAKEUPMSK = 0x0;  //DSP0 wakeup enable

    hal_nvic_restore_interrupt_mask(mask);
    return ;
}

void os_gpt_stop_for_tickless()
{

    uint32_t mask;
    LOG_COMMON_MSGID_INFO("Wake Up From Sleep,OS GPT g_compare_value:0x%x,OS_GPTGLB->OS_GPT_IRQSTA:0x%x \r\n",2,os_gpt0->GPT_COMPARE,os_gpt_glb ->OS_GPT_IRQSTA);
    hal_nvic_save_and_set_interrupt_mask(&mask);

    /*diable interrupt*/
    os_gpt0->GPT_IRQ_EN &= 0x0;
    /* stop timer */
    os_gpt0->GPT_CON_UNION.GPT_CON_CELLS.EN = 0x0;
    os_gpt0->GPT_IRQ_ACK = 0x1;
    os_gpt0->GPT_IRQ_EN = 0;                       /* disable interrupt */
    os_gpt0->GPT_CON_UNION.GPT_CON = 0;            /* disable timer     */
    os_gpt0->GPT_CLR = 0x1;                       /* clear counter value */
    while (os_gpt_glb->OS_GPT_CLRSTA & 0x1);
    os_gpt0->GPT_CLK = 0xc;

    hal_nvic_restore_interrupt_mask(mask);

    return ;

}


void os_gpt_interrupt_handle(hal_nvic_irq_t irq_number)
{
    UNUSED(irq_number);
    os_gpt0->GPT_IRQ_ACK = 0x01;


#if 0
    static U8 flag111 = 0;
    if(flag111 == 0)
    {
        flag111 = 1;
        hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &last32KTick);
        printf("tickCompensate last32KTick[%d]",last32KTick);
    }
#endif

    /* Run FreeRTOS tick handler*/
    xPortSysTickHandler();

    //port_sleep_tick_calibration();

    systemCounter++;

    if(isKernelInit && systemCounter > 1000)
    {
        systemTick ++;
<<<<<<< HEAD
        osStatusNotify();
=======
        //osStatusNotify();
>>>>>>> db20e11 (second commit)
        systemCounter = 0;
    }
}

void os_gpt_init(uint32_t ms)
{
    /* set 13 divided with 13M source */
    os_gpt0->GPT_CON_UNION.GPT_CON |= (1 << 16);   // disable clock before config
    os_gpt0->GPT_CLK = 0xc;
    os_gpt0->GPT_CON_UNION.GPT_CON &= ~(1 << 16);   // enable clock
    os_gpt0->GPT_COMPARE = ms * 1000;

    /* clear */
    os_gpt0->GPT_CLR = 0x01;
    while (os_gpt0->GPT_COUNT);

    /* enable IRQ */
    os_gpt0->GPT_IRQ_EN = 0x1;
    /* enable GPT0 clk and repeat mode and enable GPT0 */
    os_gpt0->GPT_CON_UNION.GPT_CON |= 0x101;

    /* register and enable IRQ */
    hal_nvic_register_isr_handler(OS_GPT_IRQn, (hal_nvic_isr_t)os_gpt_interrupt_handle);
    hal_nvic_enable_irq(OS_GPT_IRQn);
    os_gpt_glb->OS_GPT_IRQMSK &= 0x2;
    //os_gpt_glb->OS_GPT_WAKEUPMSK &= 0x2;  // mask as system will dead when boot-up, must unmask after sleep<->wake is ok

    //hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &last32KTick);
    last32KTick = port_read_btClk();
    LOG_COMMON_MSGID_INFO("tickCompensate init last32KTick=%d", 1, last32KTick);
}

void os_gpt0_pause(void)
{
    //os_gpt0->GPT_CON_UNION.GPT_CON &= 0xFFFFFFFE;
}



void vPortSetupTimerInterrupt(void)
{
#ifdef TEMP_FPGA_ML1S_HACKS
/* Tick has to be 20 times slower when using slow bitfiles */
extern bool use_slow_bitfile;
if (use_slow_bitfile==true)
    os_gpt_init(portTICK_PERIOD_MS*20); /* 1tick = 1ms */
else
#endif //TEMP_FPGA_ML1S_HACKS
    os_gpt_init(portTICK_PERIOD_MS); /* 1tick = 1ms */

}

ATTR_TEXT_IN_TCM void portIdleMode(void)
{
    U32 before_sleep_time;
    U32 after_sleep_time;

    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &before_sleep_time);
    __asm__ __volatile__(" dsync             \n"
     " waiti 0           \n"
     " isync             \n");
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &after_sleep_time);
    frozenTime += (after_sleep_time - before_sleep_time);
}

void port_sleepMode_internal(int xExpectedIdleTime)
{
    U32 before_sleep_time;
    U32 after_sleep_time;
    U32 tickDiff;
    U32 enter_wdt_before_time,enter_wdt_after_time;
    U32 exit_wdt_before_time,exit_wdt_after_time;
    U32 set_ost_before_time,set_ost_after_time;
<<<<<<< HEAD

=======
    hal_gpt_status_t gpt_ret;
>>>>>>> db20e11 (second commit)
    if(IsLowPwrMode)
    {
        if(osTaskStatus())
        {
            /* there is task ready */
            return;
        }

        U32 irq_state = xthal_get_interrupt();
        U32 int_en    = xthal_get_intenable();

        if(irq_state & int_en)
        {
            /* do not sleep if there is interrupt */
            return;
        }
    }

    if(xExpectedIdleTime < SLEEP_MANAGEMENT_TOTAL_GUARD_TIME + 1)
    {
        portIdleMode();
        return;
    }

<<<<<<< HEAD
=======
    /* stop the os profiling timer */
    extern uint32_t os_profiling_gpt_handle;
    (void)hal_gpt_sw_stop_timer_ms(os_profiling_gpt_handle);

>>>>>>> db20e11 (second commit)
    os_gpt0_pause();
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &before_sleep_time);

    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &enter_wdt_before_time);
#ifdef MTK_SYSTEM_HANG_TRACER_ENABLE
    /* make sure the sleep time does not overflow the wdt limitation. */
    uint32_t sleep_time_sec = ((xExpectedIdleTime) / configTICK_RATE_HZ);
    if (sleep_time_sec > (HAL_WDT_MAX_TIMEOUT_VALUE - 10))
    {
        sleep_time_sec = (HAL_WDT_MAX_TIMEOUT_VALUE - 10);
        xExpectedIdleTime = sleep_time_sec * configTICK_RATE_HZ;
        /* maybe xExpectedIdleTime is still larger than OS GPT reload value, it will be cut again */
        /* in any case, wdt timeout value must be larger than sleep time */
    }
    /* in here, sleep_time_sec is between 0 and HAL_WDT_MAX_TIMEOUT_VALUE - 10 */
    extern void hal_wdt_enter_sleep(uint32_t seconds);
    /* disable wdt and config wdt into reset mode */
    hal_wdt_enter_sleep(sleep_time_sec + 10);
    /* after here, wdt is in reset mode for prevent sleep flow hang */
#endif /* MTK_SYSTEM_HANG_TRACER_ENABLE */
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &enter_wdt_after_time);

    /* set wake up timer since rtos wakeup time is fast than bt timer */
    hal_sleep_manager_set_sleep_time((uint32_t)xExpectedIdleTime * portTICK_PERIOD_MS - SLEEP_MANAGEMENT_TOTAL_GUARD_TIME);
    hal_sleep_manager_enter_sleep_mode(HAL_SLEEP_MODE_SLEEP);

    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &exit_wdt_before_time);
    #ifdef MTK_SYSTEM_HANG_TRACER_ENABLE
    /* restore wdt status to the configuration before sleep */
    extern void hal_wdt_exit_sleep(void);
    hal_wdt_exit_sleep();
<<<<<<< HEAD
    systemhang_set_safe_duration(SYSTEMHANG_USER_CONFIG_COUNT_TOTAL - 1, 60*20);
=======
    systemhang_set_safe_duration(SYSTEMHANG_USER_CONFIG_COUNT_TOTAL - 1, 60*5);
>>>>>>> db20e11 (second commit)
    #endif /* MTK_SYSTEM_HANG_TRACER_ENABLE */
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &exit_wdt_after_time);
    
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &after_sleep_time);
    
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &set_ost_before_time);
    vPortSetupTimerInterrupt();
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &set_ost_after_time);
    
    /* adjust tick */
    U32 tmpDiff     = (after_sleep_time - before_sleep_time)*1000;;
    tickDiff        = tmpDiff/32768;
    sleepRemainder += tmpDiff - tickDiff*32768;

    if(tickDiff)
    {
        sleepTime += tickDiff;

        LOG_COMMON_MSGID_INFO("Wake Up From Sleep, after[%d] before[%d] Expect[%dms], Actually[%dms] sleepRemainder[%d]\n", 5
                                                                        , after_sleep_time
                                                                        , before_sleep_time
                                                                        , xExpectedIdleTime * portTICK_PERIOD_MS
                                                                        , tickDiff
                                                                        , sleepRemainder
                                                                        );
        if(tickDiff > (U32)xExpectedIdleTime)
        {
            LOG_COMMON_MSGID_INFO("enter_wdt_time, Diff[0x%x] before[0x%x] after[0x%x] ", 3
                                                                        , (enter_wdt_after_time-enter_wdt_before_time)
                                                                        , enter_wdt_before_time
                                                                        , enter_wdt_after_time
                                                                        );
            LOG_COMMON_MSGID_INFO("exit_wdt_time, Diff[0x%x] before[0x%x] after[0x%x] ", 3
                                                                        , (exit_wdt_after_time-exit_wdt_before_time)
                                                                        , exit_wdt_before_time
                                                                        , exit_wdt_after_time
                                                                        );

            LOG_COMMON_MSGID_INFO("set_ost_time, Diff[0x%x] before[0x%x] after[0x%x] ", 3
                                                                        , (set_ost_after_time-set_ost_before_time)
                                                                        , set_ost_before_time
                                                                        , set_ost_after_time
                                                                        );

            LOG_COMMON_MSGID_INFO("slp_time_check :set_slp_timer:[%dms],pure_sleep:[%dms],backup:[%dus],restore:[%dus]", 4, sleep_management_time_check.set_sleep_time,
                                                                    (sleep_management_time_check.pure_sleep_time*1000)/32768,
                                                                    (sleep_management_time_check.backup_time*1000000) / 32768,
                                                                    (sleep_management_time_check.restore_time*1000000) / 32768);

            sleep_management_debug_dump_backup_restore_time();
            configASSERT(FALSE);
        }

        vTaskStepTick(tickDiff - 1);
        xTaskIncrementTick();

        if(sleepRemainder > 32768)
        {
            sleepRemainder -= 32768;
            xTaskIncrementTick();
        }
<<<<<<< HEAD
=======

    }
    /* restart the os profiling timer */
    extern uint32_t os_profiling_gpt_handle;
    extern VOID osDumpTaskCounter(VOID);
    gpt_ret = hal_gpt_sw_start_timer_ms(os_profiling_gpt_handle, OS_PROFILING_GPT_PERIOD_MS, (hal_gpt_callback_t)osDumpTaskCounter, NULL);
    if(HAL_GPT_STATUS_OK != gpt_ret){
        LOG_COMMON_MSGID_INFO("os profiling timer is not re-started:%d", 1, gpt_ret);
>>>>>>> db20e11 (second commit)
    }
}

#define MAX_SLEEP_TIME_60S          (pdMS_TO_TICKS(60*1000))

ATTR_TEXT_IN_TCM void port_sleepMode(int xExpectedIdleTime)
{
    UNUSED(xExpectedIdleTime);

#if !(LABTEST)
    U32 mask;
    U32 realExpectIdleTime;

#if (SLEEP_IT)
    U8 AdaptedPowerLv = 2;
#else

    U8 AdaptedPowerLv = (gSysBootCtl.AdaptivePowerLv > gSysBootCtl.ConstrainedPowerLv) ?
                        (gSysBootCtl.ConstrainedPowerLv) :
                        (gSysBootCtl.AdaptivePowerLv);


#endif
    if(realExpectIdleTime > MAX_SLEEP_TIME_60S)
    {
        realExpectIdleTime = MAX_SLEEP_TIME_60S;
    }

    mask = (uint32_t)XTOS_SET_INTLEVEL(4);

    if(!IsLowPwrMode)
    {
        if(osTaskStatus())
        {
            /* there is task ready */
            goto leave;
        }

        U32 irq_state = xthal_get_interrupt();
        U32 int_en    = xthal_get_intenable();

        if(irq_state & int_en)
        {
            /* do nothing if there is interrupt */
            goto leave;
        }
    }

    /* do not sleep if sleep lock */
    if(!SYS_PWR_SAVING_STATUS())
    {
        if(AdaptedPowerLv == SYS_POWER_MODE_ACTIVE)
        {
            /* bypass */
            portIdleMode();
        }
        else if(osTaskFrozenMode() || hal_sleep_manager_get_lock_status() || (uart_agree_logging_enter_sleep() == false))
        {
            portIdleMode();
        }
        else
        {
            switch (AdaptedPowerLv)
            {
                case SYS_POWER_MODE_FROZEN:
                    portIdleMode();
                    break;

                case SYS_POWER_MODE_SLEEP:
                    port_sleepMode_internal(xExpectedIdleTime);
                    break;

                case SYS_POWER_MODE_DEEP_SLEEP:
                    hal_captouch_lowpower_control(HAL_CAPTOUCH_MODE_LOWPOWER);
                    DRV_PMU_StoreSignal(1 << 3);

                    if(rtc_config.enable)
                    {
                        LOG_COMMON_MSGID_INFO("rtc_alarm, timer[%ds]", 1, rtc_config.timer_s);
                        hal_rtc_set_alarm_by_second(rtc_config.timer_s);
                        hal_rtc_enable_alarm();
                    }
                    hal_rtc_enter_rtc_mode();
                    break;
                default :
                    break;
            }
        }
    }
leave:
    XTOS_RESTORE_INTLEVEL(mask);
#endif
}

#if (SLEEP_IT)
TimerHandle_t sleep_timer;
U32 BeforeTime;
U32 flag1 = 1;

U32 startTick;
U32 startTime;

void sleep_test_timer( TimerHandle_t pxExpiredTimer )
{
    if(flag1)
    {
        flag1 = 0;
        startTick = xTaskGetTickCount();
        hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &startTime);
    }

    U32 before_sleep_time;
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &before_sleep_time);
    printf("sleep test timer expired tick[%d] before_sleep_time[%d] timeDiff[%d]\n startTick[%d] startTime[%d]"
        ,xTaskGetTickCount(),before_sleep_time,  (before_sleep_time - BeforeTime)*1000/32768,
        startTick,startTime);

    BeforeTime = before_sleep_time;
}

void sleep_test(void)
{
    sleep_timer = xTimerCreate( "OS_Dpc", 1000, pdTRUE, NULL, sleep_test_timer );
    xTimerStart(sleep_timer, 0);

    printf("sleep test timer start\n");
}
#endif

#endif /* configUSE_TICKLESS_IDLE */

