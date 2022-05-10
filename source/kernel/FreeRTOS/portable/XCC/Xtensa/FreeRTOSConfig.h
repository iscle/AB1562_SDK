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

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H
#if !defined(__ASSEMBLER__)
#include <stdint.h>
#include "os_trace_callback.h"
#endif /* !defined(__ASSEMBLER__) */

/* XT_USE_THREAD_SAFE_CLIB is defined in xtensa_config.h and can be
   overridden from the compiler/make command line. */
#undef XT_USE_THREAD_SAFE_CLIB
#define XT_USE_THREAD_SAFE_CLIB 0
#if (XT_USE_THREAD_SAFE_CLIB > 0u)
  #if XT_HAVE_THREAD_SAFE_CLIB
    #define configUSE_NEWLIB_REENTRANT		1
  #else
    #error "Error: thread-safe C library support not available for this C library."
  #endif
#else
  #define configUSE_NEWLIB_REENTRANT		0
#endif

/* Required for configuration-dependent settings */
#include "xtensa_config.h"

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * Note that the default heap size is deliberately kept small so that
 * the build is more likely to succeed for configurations with limited
 * memory.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *----------------------------------------------------------*/

#define configUSE_PREEMPTION			1
#define configUSE_IDLE_HOOK				1
#ifndef __EXT_BOOTLOADER__
#define configUSE_TICKLESS_IDLE         1
#endif
#define configUSE_TICK_HOOK				0

#define configTICK_RATE_HZ			    (1000)

/* Default clock rate for simulator */
#define configCPU_CLOCK_HZ				(2000000)

/* This has impact on speed of search for highest priority */
#define configMAX_PRIORITIES			( 7 )

#define INCLUDE_pcTaskGetTaskName        1
#define configUSE_MALLOC_FAILED_HOOK     1

/* Minimal stack size. This may need to be increased for your application */
/* NOTE: The FreeRTOS demos may not work reliably with stack size < 4KB.  */
/* The Xtensa-specific examples should be fine with XT_STACK_MIN_SIZE.    */
/* NOTE: the size is defined in terms of StackType_t units not bytes.     */
#if !(defined XT_STACK_MIN_SIZE)
#error XT_STACK_MIN_SIZE not defined, did you include xtensa_config.h ?
#endif

//#define configMINIMAL_STACK_SIZE		XT_STACK_MIN_SIZE
#define configMINIMAL_STACK_SIZE		(2048 >> 2) //4Byte

/* The Xtensa port uses a separate interrupt stack. Adjust the stack size */
/* to suit the needs of your specific application.                        */
/* NOTE: the size is defined in bytes.                                    */
#ifndef configISR_STACK_SIZE
#define configISR_STACK_SIZE			2048 //Byte
#endif

/* Minimal heap size to make sure examples can run on memory limited
   configs. Adjust this to suit your system. */
#if( LABTEST)
#define configTOTAL_HEAP_SIZE			( ( size_t ) (200 * 1024) )
#else
<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
#define configTOTAL_HEAP_SIZE			( ( size_t ) (110 * 1024) )
#else
#define configTOTAL_HEAP_SIZE			( ( size_t ) (108 * 1024) )
#endif
#endif

#define configMAX_TASK_NAME_LEN			( 8 )
#define configUSE_TRACE_FACILITY		1		/* Used by vTaskList in main.c */
#define configUSE_STATS_FORMATTING_FUNCTIONS	0	/* Used by vTaskList in main.c */
#define configUSE_TRACE_FACILITY_2      0		/* Provided by Xtensa port patch */
#define configBENCHMARK					0		/* Provided by Xtensa port patch */
#define configUSE_16_BIT_TICKS			0
#define configIDLE_SHOULD_YIELD			0
#define configQUEUE_REGISTRY_SIZE		0
#define configUSE_MUTEXES				1
#define configUSE_RECURSIVE_MUTEXES		1
#define configUSE_COUNTING_SEMAPHORES	1
#define configCHECK_FOR_STACK_OVERFLOW	2

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 			0
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )

/* Set the following definitions to 1 to include the API function, or zero
   to exclude the API function. */

#define INCLUDE_vTaskPrioritySet			1
#define INCLUDE_uxTaskPriorityGet			1
#define INCLUDE_vTaskDelete					1
#define INCLUDE_vTaskCleanUpResources		0
#define INCLUDE_vTaskSuspend				1
#define INCLUDE_vTaskDelayUntil				1
#define INCLUDE_vTaskDelay					1
#define INCLUDE_uxTaskGetStackHighWaterMark	1

/* The priority at which the tick interrupt runs.  This should probably be
   kept at 1. */
#define configKERNEL_INTERRUPT_PRIORITY		1

/* The maximum interrupt priority from which FreeRTOS.org API functions can
   be called.  Only API functions that end in ...FromISR() can be used within
   interrupts. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY	XCHAL_EXCM_LEVEL

/* Test FreeRTOS timers (with timer task) and more. */
/* Some files don't compile if this flag is disabled */
#define configUSE_TIMERS                    1
#define configTIMER_TASK_PRIORITY           2
#define configTIMER_QUEUE_LENGTH            25
#define configTIMER_TASK_STACK_DEPTH        (4096 >> 2) //4Byte
#define INCLUDE_xTimerPendFunctionCall      1
#define INCLUDE_eTaskGetState               1
#define configUSE_QUEUE_SETS                1

/* Specific config for XTENSA (these can be deleted and they will take default values) */

#if (!defined XT_SIMULATOR) && (!defined XT_BOARD)
#define configXT_SIMULATOR                  1   /* Simulator mode */
#define configXT_BOARD                      0   /* Board mode */
#endif

#if (!defined XT_INTEXC_HOOKS)
#define configXT_INTEXC_HOOKS               1   /* Exception hooks used by certain tests */
#endif

#if configUSE_TRACE_FACILITY_2
#define configASSERT_2						1	/* Specific to Xtensa port */
#endif /* configUSE_TRACE_FACILITY_2 */

#define configEXPECTED_IDLE_TIME_BEFORE_SLEEP 5
#if !defined(__ASSEMBLER__)
extern void port_sleepMode(int xExpectedIdleTime);
#define portSUPPRESS_TICKS_AND_SLEEP( xExpectedIdleTime ) port_sleepMode(xExpectedIdleTime)
#endif /* !defined(__ASSEMBLER__) */

#ifndef __EXT_BOOTLOADER__
#define configGENERATE_RUN_TIME_STATS 1
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()
#endif

<<<<<<< HEAD
=======
#define OS_PROFILING_GPT_PERIOD_MS (1 * 1000) //1s

>>>>>>> db20e11 (second commit)
#define INCLUDE_xTaskGetIdleTaskHandle 1
#define INCLUDE_xTimerGetTimerDaemonTaskHandle 1
#define INCLUDE_xTaskGetCurrentTaskHandle 1

#endif /* FREERTOS_CONFIG_H */

