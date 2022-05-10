/* Copyright Statement:
 *
 * (C) 2015  Airoha Technology Corp. All rights reserved.
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


#include "types.h"
//#include "rc.h"
#include "bt_controller.h"
#include "dsp.h"

#include "transform.h"

#include "message.h"
#include "bt.h"

#include "bt_connection.h"

//#include "streamhci.h"
#include "App.h"
#include "rofs.h"

#include "drv_usb_mass_storage.h"
#include "feature_config.h"

#include "stream.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "system.h"
#include "multi_pool_heap.h"
#include "hal_core_status.h"
<<<<<<< HEAD
=======
#include "hal.h"
>>>>>>> db20e11 (second commit)

#ifdef MTK_SYSTEM_HANG_TRACER_ENABLE
#include "systemhang_tracer.h"
#endif /* MTK_SYSTEM_HANG_TRACER_ENABLE */


////////////////////////////////////////////////////////////////////////////////
// Function Declarations ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#if( configUSE_MALLOC_FAILED_HOOK == 1 )
void vApplicationMallocFailedHook(void *pucPoolBuffer)
{
    #ifdef MTK_SUPPORT_HEAP_DEBUG
    extern uint8_t ucHeap[];
    if((uint32_t)pucPoolBuffer == (uint32_t)ucHeap) {
        /* dump all heap blocks */
        //mp_dump_status(&ucHeap);

        configASSERT(0);
    }
    #else
    (void)pucPoolBuffer;
    #endif /* MTK_SUPPORT_HEAP_DEBUG */

}
#endif

<<<<<<< HEAD
#if( configUSE_IDLE_HOOK == 1 )
void vApplicationIdleHook(void)
{
#ifdef MTK_SYSTEM_HANG_TRACER_ENABLE
    systemhang_set_safe_duration(SYSTEMHANG_USER_CONFIG_COUNT_TOTAL - 1, 60*20);
=======
extern void log_save_flash(void);
#if( configUSE_IDLE_HOOK == 1 )
void vApplicationIdleHook(void)
{
    static uint32_t last_count = 0;
    uint32_t current_count = 0, time_count = 0;

#ifdef MTK_SYSTEM_HANG_TRACER_ENABLE
    systemhang_set_safe_duration(SYSTEMHANG_USER_CONFIG_COUNT_TOTAL - 1, 60*5);
>>>>>>> db20e11 (second commit)

#if (configUSE_TICKLESS_IDLE == 0)
    /* If tickless mode is not enabled, sometimes the idle task will run more than 1 min */
    /* For avoiding the wdt timeout in this case, feed wdt in every idle task's loop */
    systemhang_wdt_feed_in_task_switch();
#endif /* configUSE_TICKLESS_IDLE != 2 */
#endif /* MTK_SYSTEM_HANG_TRACER_ENABLE */
<<<<<<< HEAD
=======

    /* offline log flush handle */
    log_save_flash();

    /* every >1s to dump system info */
    {
        extern VOID osStatusInfo(VOID);
        hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &current_count);
        hal_gpt_get_duration_count(last_count, current_count, &time_count);
        if(time_count > 32768 * 1){
            osStatusInfo();
            last_count = current_count;
        }
    }
>>>>>>> db20e11 (second commit)
}
#endif

/**
 * The entry of C code
 */
#define UNUSED(p) ((void)(p))
extern void system_init(void);

int main (VOID)
{
    /* Do system initialization, eg: hardware, nvdm. */
    system_init();

    BT_Connection_Init();

    /*host tasks creation*/
    DSP_Init();

    APP_Init();
    hal_core_status_write(HAL_CORE_DSP,HAL_CORE_ACTIVE);
    vTaskStartScheduler();
    /* If all is well, the scheduler will now be running, and the following line
    will never be reached.  If the following line does execute, then there was
    insufficient FreeRTOS heap memory available for the idle and/or timer tasks
    to be created.  See the memory management section on the FreeRTOS web site
    for more details. */
    for (;;);

	  return 0;
}


