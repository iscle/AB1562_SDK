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

/* Includes ------------------------------------------------------------------*/
#include "memory_attribute.h"
#include "exception_config.h"
#include "exception_handler.h"
#include "exception_portable.h"
#include "hal_dwt.h"
#include "hal_uart.h"
#include "hal_gpt.h"
#include "nvkey.h"
#include "nvkey_list.h"
<<<<<<< HEAD
=======
#include "hal_core_status.h"
>>>>>>> db20e11 (second commit)

#ifdef HAL_WDT_MODULE_ENABLED
#include "hal_wdt.h"
#endif

#ifdef MTK_SYSTEM_HANG_TRACER_ENABLE
#include "systemhang_tracer.h"
#endif /* MTK_SYSTEM_HANG_TRACER_ENABLE */

#ifdef MTK_MEMORY_MONITOR_ENABLE
#include "memory_monitor.h"
#endif /* MTK_MEMORY_MONITOR_ENABLE */

#ifdef MTK_BOOTREASON_CHECK_ENABLE
#include "bootreason_check.h"
#endif /* MTK_BOOTREASON_CHECK_ENABLE */

#if (EXCEPTION_MEMDUMP_MODE & EXCEPTION_MEMDUMP_MINIDUMP)
#include "offline_dump.h"
// #include "memory_map.h"
#endif /* EXCEPTION_MEMDUMP_MODE */

#ifdef MTK_SWLA_ENABLE
#include "swla.h"
#endif /* MTK_SWLA_ENABLE */


/* Private define ------------------------------------------------------------*/
#if  defined ( __GNUC__ )
  #ifndef __weak
    #define __weak   __attribute__((weak))
  #endif /* __weak */
#endif /* __GNUC__ */

#define EXCEPTION_CONFIGURATIONS_MAX                    6

#define EXCEPTION_STACK_WORDS                           (384)


/* Private typedef -----------------------------------------------------------*/
typedef struct
{
    int items;
    exception_config_type configs[EXCEPTION_CONFIGURATIONS_MAX];
} exception_config_t;

typedef struct
{
    uint32_t items;
    memory_region_type regions[EXCEPTION_CONFIGURATIONS_MAX];
} exception_user_regions_t;

typedef struct
{
    uint32_t count;
    uint32_t timestamp;
    uint32_t timestamp_32k;
    uint32_t reason;
    assert_expr_t *assert_expr;
} exception_info_t;

/* Total 138 word, 552 Byte */
typedef struct
{
    /* General core registers */
    uint32_t ar0;
    uint32_t ar1;
    uint32_t ar2;
    uint32_t ar3;
    uint32_t ar4;
    uint32_t ar5;
    uint32_t ar6;
    uint32_t ar7;
    uint32_t ar8;
    uint32_t ar9;
    uint32_t ar10;
    uint32_t ar11;
    uint32_t ar12;
    uint32_t ar13;
    uint32_t ar14;
    uint32_t ar15;
    uint32_t ar16;
    uint32_t ar17;
    uint32_t ar18;
    uint32_t ar19;
    uint32_t ar20;
    uint32_t ar21;
    uint32_t ar22;
    uint32_t ar23;
    uint32_t ar24;
    uint32_t ar25;
    uint32_t ar26;
    uint32_t ar27;
    uint32_t ar28;
    uint32_t ar29;
    uint32_t ar30;
    uint32_t ar31;
    uint32_t ar32;
    uint32_t ar33;
    uint32_t ar34;
    uint32_t ar35;
    uint32_t ar36;
    uint32_t ar37;
    uint32_t ar38;
    uint32_t ar39;
    uint32_t ar40;
    uint32_t ar41;
    uint32_t ar42;
    uint32_t ar43;
    uint32_t ar44;
    uint32_t ar45;
    uint32_t ar46;
    uint32_t ar47;
    uint32_t ar48;
    uint32_t ar49;
    uint32_t ar50;
    uint32_t ar51;
    uint32_t ar52;
    uint32_t ar53;
    uint32_t ar54;
    uint32_t ar55;
    uint32_t ar56;
    uint32_t ar57;
    uint32_t ar58;
    uint32_t ar59;
    uint32_t ar60;
    uint32_t ar61;
    uint32_t ar62;
    uint32_t ar63;
    /* Window option special registers */
    uint32_t windowbase;
    uint32_t windowstart;
    /* Loop option special registers */
    uint32_t lbeg;
    uint32_t lend;
    uint32_t lcount;
    /* Shift amount special registers */
    uint32_t sar;
    /* Comparison special registers */
    uint32_t scompare;
    /* Exception and Interrupt option special registers */
    uint32_t exccause;
    uint32_t excvaddr;
    uint32_t pc;
    uint32_t epc1;
    uint32_t epc2;
    uint32_t epc3;
    uint32_t epc4;
    uint32_t epc5;
    uint32_t epc6;
    uint32_t epcnmi;
    uint32_t depc;
    uint32_t ps;
    uint32_t eps2;
    uint32_t eps3;
    uint32_t eps4;
    uint32_t eps5;
    uint32_t eps6;
    uint32_t epsnmi;
    uint32_t excsave1;
    uint32_t excsave2;
    uint32_t excsave3;
    uint32_t excsave4;
    uint32_t excsave5;
    uint32_t excsave6;
    uint32_t excsavenmi;
    uint32_t intenable;
    uint32_t interrupt;
    /* Bool option special registers */
    uint32_t br;
    /* Coprocessor option special registers */
    uint32_t cpenable;
    /* Debug option special registers */
    uint32_t debugcause;
    uint32_t ibreakenable;
    uint32_t ibreaka0;
    uint32_t ibreaka1;
    uint32_t dbreaka0;
    uint32_t dbreaka1;
    uint32_t dbreakc0;
    uint32_t dbreakc1;
    /* DSP engine special registers( 8 Byte aligned ) */
    uint64_t aep0;
    uint64_t aep1;
    uint64_t aep2;
    uint64_t aep3;
    uint64_t aep4;
    uint64_t aep5;
    uint64_t aep6;
    uint64_t aep7;
    uint64_t aeq0;
    uint64_t aeq1;
    uint64_t aeq2;
    uint64_t aeq3;
    uint32_t ae_ovf_sar;
    uint32_t ae_bithead;
    uint32_t ae_ts_fts_bu_bp;
    uint32_t ae_sd_no;
    uint32_t ae_cbegin0;
    uint32_t ae_cend0;
} exception_context_t;


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* exception handler's stack */
__attribute__ ((aligned (16))) static unsigned int exception_stack[EXCEPTION_STACK_WORDS] = {0};
unsigned int *exception_stack_pointer = &exception_stack[EXCEPTION_STACK_WORDS - 4];

/* exception user configuration area */
static exception_config_t exception_config;
static exception_user_regions_t exception_user_regions;

/* exception reboot configuration area */
static unsigned int reboot_flag = 0;

/* assert information area */
static assert_expr_t assert_expr;

/* exception information area */
static exception_info_t exception_info;
exception_info_t *exception_info_pointer = &exception_info;

/* exception context area */
__attribute__ ((aligned (8))) static exception_context_t exception_context;
exception_context_t *exception_context_pointer = &exception_context;

/* exception dump memory regions in region_init.c */
extern memory_region_type memory_regions[];

/* exception minidump area */
#if (EXCEPTION_MEMDUMP_MODE & EXCEPTION_MEMDUMP_MINIDUMP)
uint32_t minidump_base_address = 0;
exception_minidump_header_t minidump_header;
#endif /* EXCEPTION_MEMDUMP_MODE */

/* exception wdt mode area */
static uint32_t exception_wdt_mode = 0;

/* exception dump configuration area */
/* repeat 4 Byets for checking */
volatile exception_config_mode_t exception_dump_mode = {0};



/* Private functions ---------------------------------------------------------*/
/******************************************************************************/
/*            Exception's wdt timeout Functions                               */
/******************************************************************************/
void exception_feed_wdt(void)
{
#ifdef HAL_WDT_MODULE_ENABLED
    hal_wdt_feed(HAL_WDT_FEED_MAGIC);
#endif
}

void exception_enable_wdt_reset(void)
{
#ifdef HAL_WDT_MODULE_ENABLED
    hal_wdt_config_t wdt_config;
    wdt_config.mode = HAL_WDT_MODE_RESET;
    wdt_config.seconds = 30;
    hal_wdt_disable(HAL_WDT_DISABLE_MAGIC);
    hal_wdt_init(&wdt_config);
    hal_wdt_enable(HAL_WDT_ENABLE_MAGIC);
#endif
    exception_wdt_mode = EXCEPTION_WDT_RESET;
}

void exception_disable_wdt_reset(void)
{
#ifdef HAL_WDT_MODULE_ENABLED
    if (exception_wdt_mode == EXCEPTION_WDT_RESET)
    {
        hal_wdt_disable(HAL_WDT_DISABLE_MAGIC);
    }
#endif
}

void exception_enable_wdt_interrupt(void)
{
    if (exception_wdt_mode != EXCEPTION_WDT_INTERRUPT)
    {
#ifdef HAL_WDT_MODULE_ENABLED
        hal_wdt_config_t wdt_config;
        wdt_config.mode = HAL_WDT_MODE_INTERRUPT;
        wdt_config.seconds = 30;
        hal_wdt_disable(HAL_WDT_DISABLE_MAGIC);
        hal_wdt_init(&wdt_config);
        hal_wdt_enable(HAL_WDT_ENABLE_MAGIC);
#endif
        exception_wdt_mode = EXCEPTION_WDT_INTERRUPT;
    }
}

/******************************************************************************/
/*            Exception's assert Functions                                    */
/******************************************************************************/
__attribute__((optimize ("-O0"))) void platform_assert(const char *expr, const char *file, int line)
{
<<<<<<< HEAD
    static uint32_t primask_backup_assert = 0;
=======

    static uint32_t primask_backup_assert = 0;
  
    /* it will return directly if it is in exception status */
    if (hal_core_status_read(HAL_CORE_DSP) == HAL_CORE_EXCEPTION){
        exception_printf("please don't call assert when cpu is in exception status!!! lr = 0x%x\r\n",(uint32_t)__builtin_return_address(0));
        return;
    }
>>>>>>> db20e11 (second commit)

    primask_backup_assert = XTOS_SET_INTLEVEL(4);

#ifdef MTK_BOOTREASON_CHECK_ENABLE
    bootreason_set_flag_assert_reset();
#endif /* MTK_BOOTREASON_CHECK_ENABLE */

    assert_expr.is_valid = 1;
    assert_expr.expr = expr;
    assert_expr.file = file;
    assert_expr.line = line;
    *((volatile unsigned int *) 0xFFFFFFF1) = 1;
    for (;;);

    /* Just to avoid compiler warnings. */
    ( void ) primask_backup_assert;
}

__attribute__((optimize ("-O0"))) void light_assert(const char *expr, const char *file, int line)
{
    static uint32_t primask_backup_assert = 0;
<<<<<<< HEAD

=======
  
    /* it will return directly if it is in exception status */
    if (hal_core_status_read(HAL_CORE_DSP) == HAL_CORE_EXCEPTION){
        exception_printf("please don't call assert when cpu is in exception status!!! lr = 0x%x\r\n",(uint32_t)__builtin_return_address(0));
        return;
    }
  
>>>>>>> db20e11 (second commit)
    primask_backup_assert = XTOS_SET_INTLEVEL(4);

#ifdef MTK_BOOTREASON_CHECK_ENABLE
    bootreason_set_flag_assert_reset();
#endif /* MTK_BOOTREASON_CHECK_ENABLE */

    assert_expr.is_valid = 2;
    assert_expr.expr = expr;
    assert_expr.file = file;
    assert_expr.line = line;
    *((volatile unsigned int *) 0xFFFFFFF1) = 1;
    for (;;);

    /* Just to avoid compiler warnings. */
    ( void ) primask_backup_assert;
}

void exception_get_assert_expr(const char **expr, const char **file, int *line)
{
    if (assert_expr.is_valid) {
        *expr = assert_expr.expr;
        *file = assert_expr.file;
        *line = assert_expr.line;
    } else {
        *expr = NULL;
        *file = NULL;
        *line = 0;
    }
}

void exception_print_assert_info(void)
{
    if (assert_expr.is_valid == 1)
    {
        exception_printf("DSP0 assert failed: %s, file: %s, line: %d\r\n",
                        assert_expr.expr,
                        assert_expr.file,
                        (int)assert_expr.line);
    }
    else if (assert_expr.is_valid == 2)
    {
        exception_msgid(EXCEPTION_CPU_ID_MASTER,
                        "DSP0 assert failed: %s, file: %s, line: %d\r\n",
                        3,
                        assert_expr.expr,
                        assert_expr.file,
                        (int)assert_expr.line);
    }
}

/******************************************************************************/
/*            Exception's config Functions                                    */
/******************************************************************************/
void exception_dump_config(int flag)
{
    reboot_flag = (unsigned int)flag;
}

U16 exception_dump_config_init(void)
{
   
	U16 nvkey_status = 0;

	U16 size = 4;

    /* get user's exception dump configuartion */
    nvkey_status = NVKEY_ReadFullKey(NVKEYID_SYSETM_EXCEPTION_DUMP_CONFIG, (void *)&exception_dump_mode.exception_mode, size);

<<<<<<< HEAD
	#if (EXCEPTION_MEMDUMP_MODE & EXCEPTION_MEMDUMP_MINIDUMP)
    /* check if minidump is enable */
    if(exception_dump_mode.exception_mode & EXCEPTION_MEMDUMP_MINIDUMP)
    {
        /* offline dump init */
        offline_dump_region_init();
    }
    #endif /* EXCEPTION_MEMDUMP_MODE */

	
=======
>>>>>>> db20e11 (second commit)
	printf("exception_nodump:%d\r\n",exception_dump_mode.exception_mode_t.exception_nodump);
	printf("exception_fulldump_text:%d\r\n", exception_dump_mode.exception_mode_t.exception_fulldump_text);
	printf("exception_fulldump_binary:%d\r\n", exception_dump_mode.exception_mode_t.exception_fulldump_binary);
	printf("exception_minidump:%d\r\n",exception_dump_mode.exception_mode_t.exception_minidump);
	printf("reset_after_dump:%d\r\n", exception_dump_mode.exception_mode_t.reset_after_dump);
	printf("wdt_reset_mode:%d\r\n", exception_dump_mode.exception_mode_t.wdt_reset_mode);
	printf("mask_irq_check_assert:%d\r\n", exception_dump_mode.exception_mode_t.mask_irq_check_assert);

	printf("[Dump config]:0x%x\r\n", (unsigned int)exception_dump_mode.exception_mode);
	
	return nvkey_status;

}


void exception_dump_config_check(void)
{
    uint8_t mode = 0;

    /* check mode */
    if(exception_dump_mode.exception_mode == 0)
    {
        /* error status, exception_dump_mode should not be 0 */
        exception_dump_mode.exception_mode =   (EXCEPTION_DUMP_MODE << 24) |
                                (EXCEPTION_DUMP_MODE << 16) |
                                (EXCEPTION_DUMP_MODE <<  8) |
                                (EXCEPTION_DUMP_MODE <<  0) ;
    }

    mode = (uint8_t)(exception_dump_mode.exception_mode & 0xff);
    if ((mode != (uint8_t)((exception_dump_mode.exception_mode >>  8) & 0xff)) ||
        (mode != (uint8_t)((exception_dump_mode.exception_mode >> 16) & 0xff)) ||
        (mode != (uint8_t)((exception_dump_mode.exception_mode >> 24) & 0xff)))
    {
        /* error status, exception_dump_mode.exception_mode should be a 4 Byets-repeated data*/
        exception_dump_mode.exception_mode =   (EXCEPTION_DUMP_MODE << 24) |
                                (EXCEPTION_DUMP_MODE << 16) |
                                (EXCEPTION_DUMP_MODE <<  8) |
                                (EXCEPTION_DUMP_MODE <<  0) ;
    }
}

/******************************************************************************/
/*            Exception's Reboot Functions                                    */
/******************************************************************************/
__weak void exception_reboot(void)
{
    /* It is defined as a weak function.
     * It needs to be implemented in project.
     */
#ifdef HAL_WDT_MODULE_ENABLED
    hal_wdt_config_t wdt_config;
    wdt_config.mode = HAL_WDT_MODE_RESET;
    wdt_config.seconds = 3;
    hal_wdt_disable(HAL_WDT_DISABLE_MAGIC);
    hal_wdt_init(&wdt_config);
    hal_wdt_enable(HAL_WDT_ENABLE_MAGIC);
    // hal_wdt_software_reset();
    while(1);
#endif
    return;
}

static unsigned int reboot_check(void)
{
    return reboot_flag;
}

/******************************************************************************/
/*            Exception's regitser callbacks Functions                        */
/******************************************************************************/
exception_status_t exception_register_callbacks(exception_config_type *cb)
{
    int i;

    if (exception_config.items >= EXCEPTION_CONFIGURATIONS_MAX)
    {
        return EXCEPTION_STATUS_ERROR;
    }
    else
    {
        /* check if it is already registered */
        for (i = 0; i < exception_config.items; i++)
        {
            if ( exception_config.configs[i].init_cb == cb->init_cb
                && exception_config.configs[i].dump_cb == cb->dump_cb)
                {
                    return EXCEPTION_STATUS_ERROR;
                }
        }
        exception_config.configs[exception_config.items].init_cb = cb->init_cb;
        exception_config.configs[exception_config.items].dump_cb = cb->dump_cb;
        exception_config.items++;
        return EXCEPTION_STATUS_OK;
    }
}

exception_status_t exception_register_regions(memory_region_type *region)
{
    if (exception_user_regions.items >= EXCEPTION_CONFIGURATIONS_MAX)
    {
        return EXCEPTION_STATUS_ERROR;
    }
    else
    {
        exception_user_regions.regions[exception_user_regions.items].region_name = region->region_name;
        exception_user_regions.regions[exception_user_regions.items].start_address = region->start_address;
        exception_user_regions.regions[exception_user_regions.items].end_address = region->end_address;
        exception_user_regions.regions[exception_user_regions.items].is_dumped = region->is_dumped;
        exception_user_regions.items++;
        return EXCEPTION_STATUS_OK;
    }
}

/******************************************************************************/
/*            Exception's minidump Functions                              */
/******************************************************************************/
#if (EXCEPTION_MEMDUMP_MODE & EXCEPTION_MEMDUMP_MINIDUMP)
void printf_dummy(const char *message, ...)
{
    /* Just to avoid compiler warnings. */
    ( void ) message;
}

void msgid_dummy(uint8_t cpu_id, const char *message, uint32_t arg_cnt, ...)
{
    /* Just to avoid compiler warnings. */
    ( void ) cpu_id;
    ( void ) message;
    ( void ) arg_cnt;
}

static exception_status_t exception_minidump_check_address(uint32_t address)
{
    uint32_t i = 0;
    uint32_t start, end;

    for (i = 0; ; i++) {

        if( !memory_regions[i].region_name )
        {
            break;
        }

        start = (uint32_t)(memory_regions[i].start_address);
        end   = (uint32_t)(memory_regions[i].end_address);

        if((address >= start) && (address <= end))
        {
            return EXCEPTION_STATUS_OK;
        }

    }

    return EXCEPTION_STATUS_ERROR;
}

static exception_status_t exception_minidump_update_latest_index(uint32_t index)
{
    char flag = (char)(index & 0xff);

    if (index > 0xff)
    {
        return EXCEPTION_STATUS_ERROR;
    }

    hal_rtc_set_data(EXCEPTION_MINIDUMP_LATEST_INDEX_OFFSET,
                    (const char *)&flag,
                    1);

    return EXCEPTION_STATUS_OK;
}

exception_status_t exception_minidump_region_query_info(uint32_t index, uint32_t *address, uint32_t *length)
{
    uint32_t start_num, end_num = 0;

    if((address == NULL) || (length == NULL) || (index == 0))
    {
        return EXCEPTION_STATUS_ERROR;
    }

    if(true == offline_dump_region_query_seq_range(OFFLINE_REGION_MINI_DUMP, &start_num, &end_num))
    {
        if((end_num >= start_num) || (index <= (end_num - start_num + 1)))
        {
            if(true == offline_dump_region_query_by_seq(OFFLINE_REGION_MINI_DUMP, index + start_num - 1, address, length))
            {
                return EXCEPTION_STATUS_OK;
            }
        }
    }

    return EXCEPTION_STATUS_ERROR;
}

exception_status_t exception_minidump_region_query_count(uint32_t *count)
{
    uint32_t start_num, end_num = 0;

    if(count == NULL)
    {
        return EXCEPTION_STATUS_ERROR;
    }

    if(true == offline_dump_region_query_seq_range(OFFLINE_REGION_MINI_DUMP, &start_num, &end_num))
    {
        if(end_num >= start_num)
        {
            *count = end_num - start_num + 1;
            return EXCEPTION_STATUS_OK;
        }
    }

    return EXCEPTION_STATUS_ERROR;
}

exception_status_t exception_minidump_region_query_latest_index(uint32_t *index)
{
    uint32_t dump_count;
    uint8_t latest_index;

    /* get minidump total count */
    if(EXCEPTION_STATUS_OK != exception_minidump_region_query_count(&dump_count))
    {
        *index = 0;
        return EXCEPTION_STATUS_ERROR;
    }

    /* get latest_index from rtc register */
    hal_rtc_get_data(EXCEPTION_MINIDUMP_LATEST_INDEX_OFFSET,
                    (char *)&latest_index,
                    1);

    if(latest_index > dump_count)
    {
        /* latest index and total dump count are not matched */
        *index = 0;
        return EXCEPTION_STATUS_ERROR;
    }
    else if((latest_index == 0) && (dump_count != 0))
    {
        /* rtc module does not keep the index */
        *index = dump_count;
    }
    else
    {
        *index = (uint32_t)latest_index;
    }

    return EXCEPTION_STATUS_OK;
}

exception_status_t exception_minidump_get_header_info(uint32_t address, uint8_t **header_address, uint32_t *size)
{
    if(address == 0)
    {
        return EXCEPTION_STATUS_ERROR;
    }

    *header_address = (uint8_t *)(address + sizeof(offline_dump_header_t));
    *size = MINIDUMP_HEADER_LENGTH;

    return EXCEPTION_STATUS_OK;
}

exception_status_t exception_minidump_get_assert_info(uint32_t address, char **file, uint32_t *line)
{
    exception_minidump_header_t *header = NULL;

    if(address == 0)
    {
        return EXCEPTION_STATUS_ERROR;
    }

    header = (exception_minidump_header_t *)(address + sizeof(offline_dump_header_t));
    *file = (char *)((header->assert_info).file);
    *line = (header->assert_info).line;

    /* Check if the file is in Flash region */
    // TODO: fix it
    // if(((uint32_t)(*file) > (ROM_BASE + ROM_LENGTH)) || ((uint32_t)(*file) < ROM_BASE))
    // {
    //     return EXCEPTION_STATUS_ERROR;
    // }

    return EXCEPTION_STATUS_OK;
}

exception_status_t exception_minidump_get_context_info(uint32_t address, uint8_t **context_address, uint32_t *size)
{
    if(address == 0)
    {
        return EXCEPTION_STATUS_ERROR;
    }

    *context_address = (uint8_t *)(address + sizeof(offline_dump_header_t) + MINIDUMP_HEADER_LENGTH);
    *size = sizeof(exception_context_t);

    return EXCEPTION_STATUS_OK;
}

exception_status_t exception_minidump_get_stack_info(uint32_t address, uint8_t **stack_address, uint32_t *size)
{
    if(address == 0)
    {
        return EXCEPTION_STATUS_ERROR;
    }

    *stack_address =    (uint8_t *)(address
                        + sizeof(offline_dump_header_t)
                        + MINIDUMP_HEADER_LENGTH
                        + MINIDUMP_CONTEXT_LENGTH);
    *size = MINIDUMP_CURRENTSTACK_LENGTH;

    return EXCEPTION_STATUS_OK;
}

exception_status_t exception_minidump_check_not_duplicated(void)
{
    uint32_t dump_count = 0;
    uint32_t dump_addr, dump_len;
    char *dump_assert_file;
    uint32_t dump_assert_line;
    exception_minidump_header_t *dump_header;
    uint32_t dump_header_size;
    exception_context_t *dump_context;
    uint32_t dump_context_size;

    /* get minidump total count */
    if(EXCEPTION_STATUS_OK != exception_minidump_region_query_count(&dump_count))
    {
        /* can not get minidump info, return ok for do this minidump */
        return EXCEPTION_STATUS_OK;
    }

    /* check existing minidump one by one */
    for(; dump_count > 0; dump_count--)
    {
        /* get minidump info */
        if(EXCEPTION_STATUS_OK != exception_minidump_region_query_info(dump_count,
                                                                       &dump_addr,
                                                                       &dump_len))
        {
            /* can not get minidump info, return ok for do this minidump */
            return EXCEPTION_STATUS_OK;
        }

        /* get minidump header info */
        if(EXCEPTION_STATUS_OK != exception_minidump_get_header_info(dump_addr,
                                                                     (uint8_t **)(&dump_header),
                                                                     &dump_header_size))
        {
            /* can not get minidump info, return ok for do this minidump */
            return EXCEPTION_STATUS_OK;
        }

        /* get minidump assert info */
        if(EXCEPTION_STATUS_OK != exception_minidump_get_assert_info(dump_addr,
                                                                     &dump_assert_file,
                                                                     &dump_assert_line))
        {
            /* if exception is not assert, exception_minidump_get_assert_info() always return error */
            /* do nothing */
        }

        /* get minidump context info */
        if(EXCEPTION_STATUS_OK != exception_minidump_get_context_info(dump_addr,
                                                                      (uint8_t **)(&dump_context),
                                                                      &dump_context_size))
        {
            /* can not get minidump info, return ok for do this minidump */
            return EXCEPTION_STATUS_OK;
        }

        /* TODO: add slaves' minidump check */

        /* check assert info */
        if( (exception_info.assert_expr->is_valid != 0) &&
            (dump_assert_file == exception_info.assert_expr->file) &&
            (dump_assert_line == exception_info.assert_expr->line))
        {
            /* same means this assert has been saved */
            /* change the latest index to this saved minidump's index */
            exception_minidump_update_latest_index(dump_count);
            return EXCEPTION_STATUS_ERROR;
        }

        /* check other exceptions' pc, lr and exception type*/
        if( (exception_info.assert_expr->is_valid == 0) &&
            (dump_context->pc == exception_context.pc) &&
            (*((uint32_t *)dump_context + (dump_context->windowbase * 4)) ==
             *((uint32_t *)&exception_context + (exception_context.windowbase * 4))) &&
            (dump_header->reason == exception_info.reason))
        {
            /* same means this exception has been saved */
            /* change the latest index to this saved minidump's index */
            exception_minidump_update_latest_index(dump_count);
            return EXCEPTION_STATUS_ERROR;
        }
    }

    return EXCEPTION_STATUS_OK;
}
#endif /* EXCEPTION_MEMDUMP_MODE & EXCEPTION_MEMDUMP_MINIDUMP */

/******************************************************************************/
/*            Exception's init Functions                                      */
/******************************************************************************/
static void ReviseAddressRegsitersLocation(exception_context_t *pEC)
{
    unsigned int shift = 0;
    unsigned int i = 0;
    unsigned int buff[EXCEPTION_AR_COUNT] = {0};

    shift = pEC->windowbase * 4;
    for(i = 0; i < EXCEPTION_AR_COUNT; i++)
    {
        buff[(i + shift) % EXCEPTION_AR_COUNT] = *((unsigned int *)((unsigned int)pEC + i * 4));
    }
    for(i = 0; i < EXCEPTION_AR_COUNT; i++)
    {
        *((unsigned int *)((unsigned int)pEC + i * 4)) = buff[i];
    }
}

ATTR_TEXT_IN_IRAM void exception_init(void)
{
    //int i;

    xthal_set_cacheattr(0x22222211);

#if (configUSE_FLASH_SUSPEND == 1)
    Flash_ReturnReady();
#endif

    /* enable wdt reset mode for prevent exception flow hang */
    exception_enable_wdt_reset();

#ifdef MTK_BOOTREASON_CHECK_ENABLE
    bootreason_set_flag_exception_reset();
#endif /* MTK_BOOTREASON_CHECK_ENABLE */

#if defined(MTK_SYSTEM_HANG_TRACER_ENABLE_O1) || defined(MTK_SYSTEM_HANG_TRACER_ENABLE_O2)
    systemhang_exception_enter_trace();
#endif /* defined(MTK_SYSTEM_HANG_TRACER_ENABLE_O1) || defined(MTK_SYSTEM_HANG_TRACER_ENABLE_O2) */

    /* check exception dump configuration if is ok */
    exception_dump_config_check();

    /* update core status for other module usage */
    exception_core_status_update();

    /* enable exception log service */
    exception_log_service_init();

	if (exception_dump_mode.exception_mode == 0){
		exception_dump_mode.exception_mode_t.reset_after_dump = TRUE;
		exception_dump_mode.exception_mode_t.exception_fulldump_binary = TRUE;
	}


#if (EXCEPTION_MEMDUMP_MODE & EXCEPTION_MEMDUMP_TEXT) || (EXCEPTION_MEMDUMP_MODE & EXCEPTION_MEMDUMP_BINARY)
#ifdef MTK_SWLA_ENABLE
    if((exception_dump_mode.exception_mode & EXCEPTION_MEMDUMP_TEXT) || (exception_dump_mode.exception_mode & EXCEPTION_MEMDUMP_BINARY))
    {
        /* add a record for exception to mark the end of the last thread */
        const uint8_t ucExceptionRec[5] = "excp";
        const uint32_t xExceptionRec = (uint32_t)(ucExceptionRec[0] | (ucExceptionRec[1] << 8) | (ucExceptionRec[2] << 16) | (ucExceptionRec[3] << 24));
        SLA_RamLogging(xExceptionRec);
    }
<<<<<<< HEAD
=======
    extern void SLA_MemoryCallbackInit(void);
    SLA_MemoryCallbackInit();
>>>>>>> db20e11 (second commit)
#endif /* MTK_SWLA_ENABLE */
#endif /* EXCEPTION_MEMDUMP_MODE */

    /* ARx has shift in exceptionContext, so need to revise it */
    ReviseAddressRegsitersLocation(exception_context_pointer);

    /* Update Exception Number */
    exception_info.count += 1;

	if (exception_info.count <= 2){			
		exception_printf("exccause = 0x%x\r\n",exception_context_pointer->exccause);	
		exception_printf("pc = 0x%x\r\n",exception_context_pointer->pc);
		
	}else{	

		while(1);	
	}

    /* Get current time stamp */
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M , &(exception_info.timestamp));
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K , &(exception_info.timestamp_32k));

    if (exception_dump_mode.exception_mode_t.reset_after_dump == TRUE){
	    exception_dump_config(DISABLE_WHILELOOP_MAGIC);
    }

}

/******************************************************************************/
/*            Exception's context dump Functions                              */
/******************************************************************************/
static void exception_dump_context(exception_context_t * const context_regs)
{
	
    /* NMI interrupt */
    if(context_regs->epsnmi != 0x0)
    {
        /* change ps and epsnmi */
        context_regs->ps     = (context_regs->ps) ^ (context_regs->epsnmi);
        context_regs->epsnmi = (context_regs->ps) ^ (context_regs->epsnmi);
        context_regs->ps     = (context_regs->ps) ^ (context_regs->epsnmi);
    }

    /* Debug exception */
    if(context_regs->debugcause != 0x0)
    {
        /* clear PS.EXCM because PS.EXCM is 0 when Debug exception happens */
        context_regs->ps &= 0xffffffef;
    }

#if (EXCEPTION_MEMDUMP_MODE & EXCEPTION_MEMDUMP_TEXT) || (EXCEPTION_MEMDUMP_MODE & EXCEPTION_MEMDUMP_BINARY)
    if((exception_dump_mode.exception_mode & EXCEPTION_MEMDUMP_TEXT) || (exception_dump_mode.exception_mode & EXCEPTION_MEMDUMP_BINARY))
    {
        /* feed wdt to keep time for context dump */
        exception_feed_wdt();

        /* General core registers */
        exception_printf("DSP0 Register dump begin:\r\n");
        exception_printf("ar0  = 0x%08x\r\n", context_regs->ar0);
        exception_printf("ar1  = 0x%08x\r\n", context_regs->ar1);
        exception_printf("ar2  = 0x%08x\r\n", context_regs->ar2);
        exception_printf("ar3  = 0x%08x\r\n", context_regs->ar3);
        exception_printf("ar4  = 0x%08x\r\n", context_regs->ar4);
        exception_printf("ar5  = 0x%08x\r\n", context_regs->ar5);
        exception_printf("ar6  = 0x%08x\r\n", context_regs->ar6);
        exception_printf("ar7  = 0x%08x\r\n", context_regs->ar7);
        exception_printf("ar8  = 0x%08x\r\n", context_regs->ar8);
        exception_printf("ar9  = 0x%08x\r\n", context_regs->ar9);
        exception_printf("ar10  = 0x%08x\r\n", context_regs->ar10);
        exception_printf("ar11  = 0x%08x\r\n", context_regs->ar11);
        exception_printf("ar12  = 0x%08x\r\n", context_regs->ar12);
        exception_printf("ar13  = 0x%08x\r\n", context_regs->ar13);
        exception_printf("ar14  = 0x%08x\r\n", context_regs->ar14);
        exception_printf("ar15  = 0x%08x\r\n", context_regs->ar15);
        exception_printf("ar16  = 0x%08x\r\n", context_regs->ar16);
        exception_printf("ar17  = 0x%08x\r\n", context_regs->ar17);
        exception_printf("ar18  = 0x%08x\r\n", context_regs->ar18);
        exception_printf("ar19  = 0x%08x\r\n", context_regs->ar19);
        exception_printf("ar20  = 0x%08x\r\n", context_regs->ar20);
        exception_printf("ar21  = 0x%08x\r\n", context_regs->ar21);
        exception_printf("ar22  = 0x%08x\r\n", context_regs->ar22);
        exception_printf("ar23  = 0x%08x\r\n", context_regs->ar23);
        exception_printf("ar24  = 0x%08x\r\n", context_regs->ar24);
        exception_printf("ar25  = 0x%08x\r\n", context_regs->ar25);
        exception_printf("ar26  = 0x%08x\r\n", context_regs->ar26);
        exception_printf("ar27  = 0x%08x\r\n", context_regs->ar27);
        exception_printf("ar28  = 0x%08x\r\n", context_regs->ar28);
        exception_printf("ar29  = 0x%08x\r\n", context_regs->ar29);
        exception_printf("ar30  = 0x%08x\r\n", context_regs->ar30);
        exception_printf("ar31  = 0x%08x\r\n", context_regs->ar31);
        exception_printf("ar32  = 0x%08x\r\n", context_regs->ar32);
        exception_printf("ar33  = 0x%08x\r\n", context_regs->ar33);
        exception_printf("ar34  = 0x%08x\r\n", context_regs->ar34);
        exception_printf("ar35  = 0x%08x\r\n", context_regs->ar35);
        exception_printf("ar36  = 0x%08x\r\n", context_regs->ar36);
        exception_printf("ar37  = 0x%08x\r\n", context_regs->ar37);
        exception_printf("ar38  = 0x%08x\r\n", context_regs->ar38);
        exception_printf("ar39  = 0x%08x\r\n", context_regs->ar39);
        exception_printf("ar40  = 0x%08x\r\n", context_regs->ar40);
        exception_printf("ar41  = 0x%08x\r\n", context_regs->ar41);
        exception_printf("ar42  = 0x%08x\r\n", context_regs->ar42);
        exception_printf("ar43  = 0x%08x\r\n", context_regs->ar43);
        exception_printf("ar44  = 0x%08x\r\n", context_regs->ar44);
        exception_printf("ar45  = 0x%08x\r\n", context_regs->ar45);
        exception_printf("ar46  = 0x%08x\r\n", context_regs->ar46);
        exception_printf("ar47  = 0x%08x\r\n", context_regs->ar47);
        exception_printf("ar48  = 0x%08x\r\n", context_regs->ar48);
        exception_printf("ar49  = 0x%08x\r\n", context_regs->ar49);
        exception_printf("ar50  = 0x%08x\r\n", context_regs->ar50);
        exception_printf("ar51  = 0x%08x\r\n", context_regs->ar51);
        exception_printf("ar52  = 0x%08x\r\n", context_regs->ar52);
        exception_printf("ar53  = 0x%08x\r\n", context_regs->ar53);
        exception_printf("ar54  = 0x%08x\r\n", context_regs->ar54);
        exception_printf("ar55  = 0x%08x\r\n", context_regs->ar55);
        exception_printf("ar56  = 0x%08x\r\n", context_regs->ar56);
        exception_printf("ar57  = 0x%08x\r\n", context_regs->ar57);
        exception_printf("ar58  = 0x%08x\r\n", context_regs->ar58);
        exception_printf("ar59  = 0x%08x\r\n", context_regs->ar59);
        exception_printf("ar60  = 0x%08x\r\n", context_regs->ar60);
        exception_printf("ar61  = 0x%08x\r\n", context_regs->ar61);
        exception_printf("ar62  = 0x%08x\r\n", context_regs->ar62);
        exception_printf("ar63  = 0x%08x\r\n", context_regs->ar63);
        /* Window option special registers */
        exception_printf("windowbase  = 0x%08x\r\n", context_regs->windowbase);
        exception_printf("windowstart  = 0x%08x\r\n", context_regs->windowstart);
        /* Loop option special registers */
        exception_printf("lbeg  = 0x%08x\r\n", context_regs->lbeg);
        exception_printf("lend  = 0x%08x\r\n", context_regs->lend);
        exception_printf("lcount  = 0x%08x\r\n", context_regs->lcount);
        /* Shift amount special registers */
        exception_printf("sar  = 0x%08x\r\n", context_regs->sar);
        /* Comparison special registers */
        exception_printf("scompare  = 0x%08x\r\n", context_regs->scompare);
        /* Exception and Interrupt option special registers */
        exception_printf("exccause  = 0x%08x\r\n", context_regs->exccause);
        exception_printf("excvaddr  = 0x%08x\r\n", context_regs->excvaddr);
        exception_printf("pc  = 0x%08x\r\n", context_regs->pc);
        exception_printf("epc1  = 0x%08x\r\n", context_regs->epc1);
        exception_printf("epc2  = 0x%08x\r\n", context_regs->epc2);
        exception_printf("epc3  = 0x%08x\r\n", context_regs->epc3);
        exception_printf("epc4  = 0x%08x\r\n", context_regs->epc4);
        exception_printf("epc5  = 0x%08x\r\n", context_regs->epc5);
        exception_printf("epc6  = 0x%08x\r\n", context_regs->epc6);
        exception_printf("epcnmi  = 0x%08x\r\n", context_regs->epcnmi);
        exception_printf("depc  = 0x%08x\r\n", context_regs->depc);
        exception_printf("ps  = 0x%08x\r\n", context_regs->ps);
        exception_printf("eps2  = 0x%08x\r\n", context_regs->eps2);
        exception_printf("eps3  = 0x%08x\r\n", context_regs->eps3);
        exception_printf("eps4  = 0x%08x\r\n", context_regs->eps4);
        exception_printf("eps5  = 0x%08x\r\n", context_regs->eps5);
        exception_printf("eps6  = 0x%08x\r\n", context_regs->eps6);
        exception_printf("epsnmi  = 0x%08x\r\n", context_regs->epsnmi);
        exception_printf("excsave1  = 0x%08x\r\n", context_regs->excsave1);
        exception_printf("excsave2  = 0x%08x\r\n", context_regs->excsave2);
        exception_printf("excsave3  = 0x%08x\r\n", context_regs->excsave3);
        exception_printf("excsave4  = 0x%08x\r\n", context_regs->excsave4);
        exception_printf("excsave5  = 0x%08x\r\n", context_regs->excsave5);
        exception_printf("excsave6  = 0x%08x\r\n", context_regs->excsave6);
        exception_printf("excsavenmi  = 0x%08x\r\n", context_regs->excsavenmi);
        exception_printf("intenable  = 0x%08x\r\n", context_regs->intenable);
        exception_printf("interrupt  = 0x%08x\r\n", context_regs->interrupt);
        /* Bool option special registers */
        exception_printf("br  = 0x%08x\r\n", context_regs->br);
        /* Coprocessor option special registers */
        exception_printf("cpenable  = 0x%08x\r\n", context_regs->cpenable);
        /* Debug option special registers */
        exception_printf("debugcause  = 0x%08x\r\n", context_regs->debugcause);
        exception_printf("ibreakenable  = 0x%08x\r\n", context_regs->ibreakenable);
        exception_printf("ibreaka0  = 0x%08x\r\n", context_regs->ibreaka0);
        exception_printf("ibreaka1  = 0x%08x\r\n", context_regs->ibreaka1);
        exception_printf("dbreaka0  = 0x%08x\r\n", context_regs->dbreaka0);
        exception_printf("dbreaka1  = 0x%08x\r\n", context_regs->dbreaka1);
        exception_printf("dbreakc0  = 0x%08x\r\n", context_regs->dbreakc0);
        exception_printf("dbreakc1  = 0x%08x\r\n", context_regs->dbreakc1);
        /* DSP engine special registers */
        exception_printf("aep0_L  = 0x%08x\r\n", (uint32_t)(context_regs->aep0 & 0xffffffff));
        exception_printf("aep0_H  = 0x%08x\r\n", (uint32_t)((context_regs->aep0 >> 32) & 0xffffffff));
        exception_printf("aep1_L  = 0x%08x\r\n", (uint32_t)(context_regs->aep1 & 0xffffffff));
        exception_printf("aep1_H  = 0x%08x\r\n", (uint32_t)((context_regs->aep1 >> 32) & 0xffffffff));
        exception_printf("aep2_L  = 0x%08x\r\n", (uint32_t)(context_regs->aep2 & 0xffffffff));
        exception_printf("aep2_H  = 0x%08x\r\n", (uint32_t)((context_regs->aep2 >> 32) & 0xffffffff));
        exception_printf("aep3_L  = 0x%08x\r\n", (uint32_t)(context_regs->aep3 & 0xffffffff));
        exception_printf("aep3_H  = 0x%08x\r\n", (uint32_t)((context_regs->aep3 >> 32) & 0xffffffff));
        exception_printf("aep4_L  = 0x%08x\r\n", (uint32_t)(context_regs->aep4 & 0xffffffff));
        exception_printf("aep4_H  = 0x%08x\r\n", (uint32_t)((context_regs->aep4 >> 32) & 0xffffffff));
        exception_printf("aep5_L  = 0x%08x\r\n", (uint32_t)(context_regs->aep5 & 0xffffffff));
        exception_printf("aep5_H  = 0x%08x\r\n", (uint32_t)((context_regs->aep5 >> 32) & 0xffffffff));
        exception_printf("aep6_L  = 0x%08x\r\n", (uint32_t)(context_regs->aep6 & 0xffffffff));
        exception_printf("aep6_H  = 0x%08x\r\n", (uint32_t)((context_regs->aep6 >> 32) & 0xffffffff));
        exception_printf("aep7_L  = 0x%08x\r\n", (uint32_t)(context_regs->aep7 & 0xffffffff));
        exception_printf("aep7_H  = 0x%08x\r\n", (uint32_t)((context_regs->aep7 >> 32) & 0xffffffff));
        exception_printf("aeq0_L  = 0x%08x\r\n", (uint32_t)(context_regs->aeq0 & 0xffffffff));
        exception_printf("aeq0_H  = 0x%08x\r\n", (uint32_t)((context_regs->aeq0 >> 32) & 0xffffffff));
        exception_printf("aeq1_L  = 0x%08x\r\n", (uint32_t)(context_regs->aeq1 & 0xffffffff));
        exception_printf("aeq1_H  = 0x%08x\r\n", (uint32_t)((context_regs->aeq1 >> 32) & 0xffffffff));
        exception_printf("aeq2_L  = 0x%08x\r\n", (uint32_t)(context_regs->aeq2 & 0xffffffff));
        exception_printf("aeq2_H  = 0x%08x\r\n", (uint32_t)((context_regs->aeq2 >> 32) & 0xffffffff));
        exception_printf("aeq3_L  = 0x%08x\r\n", (uint32_t)(context_regs->aeq3 & 0xffffffff));
        exception_printf("aeq3_H  = 0x%08x\r\n", (uint32_t)((context_regs->aeq3 >> 32) & 0xffffffff));
        exception_printf("ae_ovf_sar  = 0x%08x\r\n", context_regs->ae_ovf_sar);
        exception_printf("ae_bithead  = 0x%08x\r\n", context_regs->ae_bithead);
        exception_printf("ae_ts_fts_bu_bp  = 0x%08x\r\n", context_regs->ae_ts_fts_bu_bp);
        exception_printf("ae_sd_no  = 0x%08x\r\n", context_regs->ae_sd_no);
        exception_printf("ae_cbegin0  = 0x%08x\r\n", context_regs->ae_cbegin0);
        exception_printf("ae_cend0  = 0x%08x\r\n", context_regs->ae_cend0);
        exception_printf("DSP0 Register dump end:\r\n");
    }
#endif /* EXCEPTION_MEMDUMP_MODE */

#if (EXCEPTION_MEMDUMP_MODE & EXCEPTION_MEMDUMP_MINIDUMP)
    if(exception_dump_mode.exception_mode & EXCEPTION_MEMDUMP_MINIDUMP)
    {
        uint32_t current_addr = 0;
        bool ret = false;

        /* feed wdt to keep time for context dump */
        exception_feed_wdt();

        if(minidump_base_address != 0)
        {
            /* dump core registers */
            current_addr = minidump_base_address + MINIDUMP_HEADER_LENGTH;
            /* feed wdt to keep time for context dump */
            exception_feed_wdt();
            ret = offline_dump_region_write(OFFLINE_REGION_MINI_DUMP,
                                            current_addr,
                                            (uint8_t *)&exception_context,
                                            MINIDUMP_CONTEXT_LENGTH);
            if(ret != true)
            {
                minidump_base_address = 0;
                return;
            }

            minidump_header.context_size = MINIDUMP_CONTEXT_LENGTH;
        }
    }
#endif /* EXCEPTION_MEMDUMP_MODE */
}

/******************************************************************************/
/*            Exception's memory dump Functions                               */
/******************************************************************************/
exception_status_t exception_check_is_instruction_memory(uint32_t start_address, uint32_t end_address)
{
    if((start_address >= 0x04580000) && (start_address < 0x045C0000))
    {
        return EXCEPTION_STATUS_OK;
    }
    else if((start_address >= 0x04600000) && (start_address < 0x04604000))
    {
        return EXCEPTION_STATUS_OK;
    }

    return EXCEPTION_STATUS_ERROR;

    (void) end_address;
}

#if (EXCEPTION_MEMDUMP_MODE & EXCEPTION_MEMDUMP_TEXT) || (EXCEPTION_MEMDUMP_MODE & EXCEPTION_MEMDUMP_BINARY)
static void exception_dump_region_info(const memory_region_type *static_regions, exception_user_regions_t *user_regions)
{
    uint32_t i = 0;
    unsigned int *current, *end;

    /* static regions */
    for (i = 0; ; i++)
    {
        if ( !static_regions[i].region_name )
        {
            break;
        }
        if ( !static_regions[i].is_dumped )
        {
            continue;
        }
        current = (unsigned int *)((uint32_t)(static_regions[i].start_address) & 0xfffffffc);
        end     = (unsigned int *)(static_regions[i].end_address);
        if(current < end)
        {
            exception_printf("Region-%s: start_address = 0x%x, end_address = 0x%x\r\n", static_regions[i].region_name, (unsigned int)current, (unsigned int)end);
        }
    }

    /* dynamic regions */
    for (i = 0; ((i < user_regions->items) && (i < EXCEPTION_CONFIGURATIONS_MAX)); i++)
    {
        if((user_regions->regions)[i].is_dumped)
        {
            current = (unsigned int *)((uint32_t)((user_regions->regions)[i].start_address) & 0xfffffffc);
            end     = (unsigned int *)((user_regions->regions)[i].end_address);
            if(current < end)
            {
                exception_printf("Region-%s: start_address = 0x%x, end_address = 0x%x\r\n", (user_regions->regions)[i].region_name, (unsigned int)current, (unsigned int)end);
            }
        }
    }
}
#endif /* EXCEPTION_MEMDUMP_MODE */

#if (EXCEPTION_MEMDUMP_MODE & EXCEPTION_MEMDUMP_TEXT)
static void exception_dump_region_data_text(const memory_region_type *static_regions, exception_user_regions_t *user_regions)
{
    uint32_t i = 0;
    unsigned int *current, *end;

    /* static regions */
    for (i = 0; ; i++)
    {
        if ( !static_regions[i].region_name )
        {
            break;
        }
        if ( !static_regions[i].is_dumped )
        {
            continue;
        }
        current = (unsigned int *)((uint32_t)(static_regions[i].start_address) & 0xfffffffc);
        end     = (unsigned int *)(static_regions[i].end_address);
        for (; current < end; current += 4)
        {
            if (*(current + 0) == 0 &&
                *(current + 1) == 0 &&
                *(current + 2) == 0 &&
                *(current + 3) == 0 )
            {
                continue;
            }
            /* feed wdt to keep time for output data */
            exception_feed_wdt();
            /* output data */
            exception_printf("0x%08x: %08x %08x %08x %08x\r\n",
                            (unsigned int)current,
                            *(current + 0),
                            *(current + 1),
                            *(current + 2),
                            *(current + 3));
        }
    }

    /* dynamic regions */
    for (i = 0; ((i < user_regions->items) && (i < EXCEPTION_CONFIGURATIONS_MAX)); i++)
    {
        if((user_regions->regions)[i].is_dumped)
        {
            current = (unsigned int *)((uint32_t)((user_regions->regions)[i].start_address) & 0xfffffffc);
            end     = (unsigned int *)((user_regions->regions)[i].end_address);
            for (; current < end; current += 4)
            {
                if (*(current + 0) == 0 &&
                    *(current + 1) == 0 &&
                    *(current + 2) == 0 &&
                    *(current + 3) == 0 )
                {
                    continue;
                }
                /* feed wdt to keep time for output data */
                exception_feed_wdt();
                /* output data */
                exception_printf("0x%08x: %08x %08x %08x %08x\r\n",
                                (unsigned int)current,
                                *(current + 0),
                                *(current + 1),
                                *(current + 2),
                                *(current + 3));
            }
        }
    }
}
#endif /* EXCEPTION_MEMDUMP_MODE */

#if (EXCEPTION_MEMDUMP_MODE & EXCEPTION_MEMDUMP_BINARY)
static void exception_dump_region_data_binary(const memory_region_type *static_regions, exception_user_regions_t *user_regions)
{
    uint32_t i;
    int32_t j;
    unsigned int *current, *end;
    uint32_t data[32];

    /* static regions */
    for (i = 0; ; i++) {
        if ( !static_regions[i].region_name )
        {
            break;
        }
        if ( !static_regions[i].is_dumped )
        {
            continue;
        }
        current = (unsigned int *)((uint32_t)(static_regions[i].start_address) & 0xfffffffc);
        end     = (unsigned int *)(static_regions[i].end_address);
        if(current < end)
        {
            /* feed wdt to keep time for output data */
            exception_feed_wdt();
            /* output data */
            if(exception_check_is_instruction_memory((uint32_t)current, (uint32_t)end) != EXCEPTION_STATUS_OK)
            {
                log_dump_exception_data((const uint8_t *)current, (uint32_t)end - (uint32_t)current);
            }
            else
            {
                /*  Because the instruction memory can only be accessed by l32i,
                    readback the data in temp buffer and output the temp buffer */
                for( ; current < end; current += 32)
                {
                    if ((end - current) >= 32)
                    {
                        /* copy 32 words data into data buffer */
                        for(j = 0; j < 32; j++)
                        {
                            data[j] = *(current + j);
                        }
                        /* feed wdt to keep time for output data */
                        exception_feed_wdt();
                        /* output data */
                        log_dump_exception_data((const uint8_t *)&data, 32*4);
                    }
                    else
                    {
                        /* copy the left words data into data buffer */
                        for(j = 0; j < (end - current); j++)
                        {
                            data[j] = *(current + j);
                        }
                        /* feed wdt to keep time for output data */
                        exception_feed_wdt();
                        /* output data */
                        log_dump_exception_data((const uint8_t *)&data, (end - current)*4);
                    }
                }
            }
        }
    }

    /* dynamic regions */
    for (i = 0; ((i < user_regions->items) && (i < EXCEPTION_CONFIGURATIONS_MAX)); i++)
    {
        if((user_regions->regions)[i].is_dumped)
        {
            current = (unsigned int *)((uint32_t)((user_regions->regions)[i].start_address) & 0xfffffffc);
            end     = (unsigned int *)((user_regions->regions)[i].end_address);
            if(current < end)
            {
                /* feed wdt to keep time for output data */
                exception_feed_wdt();
                /* output data */
                if(exception_check_is_instruction_memory((uint32_t)current, (uint32_t)end) != EXCEPTION_STATUS_OK)
                {
                    log_dump_exception_data((const uint8_t *)current, (uint32_t)end - (uint32_t)current);
                }
                else
                {
                    /*  Because the instruction memory can only be accessed by l32i,
                        readback the data in temp buffer and output the temp buffer */
                    for( ; current < end; current += 32)
                    {
                        if ((end - current) >= 32)
                        {
                            /* copy 32 words data into data buffer */
                            for(j = 0; j < 32; j++)
                            {
                                data[j] = *(current + j);
                            }
                            /* feed wdt to keep time for output data */
                            exception_feed_wdt();
                            /* output data */
                            log_dump_exception_data((const uint8_t *)&data, 32*4);
                        }
                        else
                        {
                            /* copy the left words data into data buffer */
                            for(j = 0; j < (end - current); j++)
                            {
                                data[j] = *(current + j);
                            }
                            /* feed wdt to keep time for output data */
                            exception_feed_wdt();
                            /* output data */
                            log_dump_exception_data((const uint8_t *)&data, (end - current)*4);
                        }
                    }
                }
            }
        }
    }
}
#endif /* EXCEPTION_MEMDUMP_MODE */

#if (EXCEPTION_MEMDUMP_MODE & EXCEPTION_MEMDUMP_MINIDUMP)
static void exception_dump_region_data_minidump(const memory_region_type *static_regions, exception_user_regions_t *user_regions)
{
<<<<<<< HEAD
    uint32_t current_addr = 0;
    bool ret = false;
    uint32_t sp = 0;

    /* get current stack - 16 */
    sp = *(uint32_t *)((uint32_t)(&exception_context) + exception_context.windowbase * 4 * 4 + 4) - 16;

    /* static regions */
    if(minidump_base_address != 0)
    {
        /* dump current stacks */
        current_addr = minidump_base_address + MINIDUMP_HEADER_LENGTH + MINIDUMP_CONTEXT_LENGTH;
        ret = exception_minidump_check_address(sp);
        if(ret == EXCEPTION_STATUS_OK)
        {
            /* feed wdt to keep time for output data */
            exception_feed_wdt();
            /* output data */
            ret = offline_dump_region_write(OFFLINE_REGION_MINI_DUMP,
                                            current_addr,
                                            (uint8_t *)(sp),
                                            MINIDUMP_CURRENTSTACK_LENGTH);
            if(ret != true)
            {
                minidump_base_address = 0;
                return;
            }
            minidump_header.regions[0].address = sp;
            minidump_header.regions[0].size = MINIDUMP_CURRENTSTACK_LENGTH ;
        }
        /* Current stack is not a valid value, so jump this region and set its begin address as 0xffffffff - MINIDUMP_CURRENTSTACK_LENGTH */
        else
        {
            minidump_header.regions[0].address = 0xffffffff - MINIDUMP_CURRENTSTACK_LENGTH;
            minidump_header.regions[0].size = MINIDUMP_CURRENTSTACK_LENGTH ;
        }
    }

    /* Just to avoid compiler warnings. */
    ( void ) static_regions;
    ( void ) user_regions;
}
=======
#ifdef MTK_MINIDUMP_ENHANCE_ENABLE
    uint32_t current_addr = 0;
    uint32_t region_size = 0;
    uint32_t data[32];
    uint32_t *current, *end;
    bool ret = false;
    uint32_t i = 0;
    uint32_t j = 0;
    uint32_t k = 0;
    uint32_t name_len = 0;
    uint32_t region_remain_size = 0;

    /* dump current stacks */
    current_addr = minidump_base_address + MINIDUMP_HEADER_LENGTH + MINIDUMP_CONTEXT_LENGTH;

    /* static regions */
    if (minidump_base_address != 0) {

        /* get remain size to do minidump */
        region_remain_size = MINIDUMP_TOTAL_SIZE - MINIDUMP_HEADER_LENGTH - MINIDUMP_CONTEXT_LENGTH;

        /* have space to dump */
        if (region_remain_size > 0) {
            /* overlist region list */
            for (i = 0;; i++) {

                /* it is no remain space to dump or region name over 20 */
                if ((region_remain_size <= 0) || (k >= 20)) {
                    break;
                }

                /* region is NULL */
                if (!static_regions[i].region_name) {
                    break;
                }

                /* is need to dump */
                if (!static_regions[i].is_dumped) {
                    continue;
                }

                /* get start address,end address and size */
                current = (unsigned int *)(((uint32_t)(static_regions[i].start_address)) & 0xfffffffc);
                end     = static_regions[i].end_address;
                region_size = (uint32_t)end - (uint32_t)current;

                /* name len max is array len 20 */
                name_len = (uint32_t)strlen(static_regions[i].region_name) > (sizeof(minidump_header.regions[k].name) / sizeof(minidump_header.regions[k].name[0])) ?
                           (sizeof(minidump_header.regions[k].name) / sizeof(minidump_header.regions[k].name[0])) : (uint32_t)strlen(static_regions[i].region_name) ;

                memcpy(minidump_header.regions[k].name, static_regions[i].region_name, name_len);
                minidump_header.regions[k].address = (uint32_t)current;

                /* remain size is enough to dump region */
                if (region_remain_size > region_size) {
                    region_remain_size -= region_size;

                    minidump_header.regions[k].size = region_size;
                } else {
                    /* remain size is not enough to dump region */
                    minidump_header.regions[k].size = region_remain_size;
                    region_remain_size = 0;
                }

                if (exception_check_is_instruction_memory((uint32_t)current, (uint32_t)end) != EXCEPTION_STATUS_OK) {
                    /* feed wdt to keep time for output data */
                    exception_feed_wdt();
                    /* output data */

                    ret = offline_dump_region_write(OFFLINE_REGION_MINI_DUMP, current_addr, (uint8_t *)static_regions[i].start_address, minidump_header.regions[k++].size);
                    if (ret != true) {
                        minidump_base_address = 0;
                        return;
                    }
                    current_addr += region_size;
                } else {

                    /* update end address */
                    end = (uint32_t *)((uint32_t)current + minidump_header.regions[k++].size);

                    /*  Because the instruction memory can only be accessed by l32i,
                        readback the data in temp buffer and output the temp buffer */
                    for (; current < end; current += 32) {
                        if ((end - current) >= 32) {
                            /* copy 32 words data into data buffer */
                            for (j = 0; j < 32; j++) {
                                data[j] = *(current + j);
                            }
                            /* feed wdt to keep time for output data */
                            exception_feed_wdt();
                            ret = offline_dump_region_write(OFFLINE_REGION_MINI_DUMP, current_addr, (uint8_t *)&data, 32 * 4);
                            if (ret != true) {
                                minidump_base_address = 0;
                                return;
                            }

                            current_addr += 32 * 4;
                        } else {
                            /* copy the left words data into data buffer */
                            for (j = 0; j < (uint32_t)(end - current); j++) {
                                data[j] = *(current + j);
                            }
                            /* feed wdt to keep time for output data */
                            exception_feed_wdt();

                            ret = offline_dump_region_write(OFFLINE_REGION_MINI_DUMP, current_addr, (uint8_t *)&data, (end - current) * 4);
                            if (ret != true) {
                                minidump_base_address = 0;
                                return;
                            }
                            current_addr += (end - current) * 4;

                        }
                    }
                }
            }
        }


        /* dynamic regions */
        for (i = 0; ((i < user_regions->items) && (i < EXCEPTION_CONFIGURATIONS_MAX)); i++) {
            /* it is no remain space to dump or region name over 20 */
            if ((region_remain_size <= 0) || (k >= 20)) {
                break;
            }

            /* need dump ? */
            if ((user_regions->regions)[i].is_dumped) {
                current = (unsigned int *)(((uint32_t)(user_regions->regions[i].start_address)) & 0xfffffffc);
                end     = user_regions->regions[i].end_address;
                region_size = (uint32_t)end - (uint32_t)current;

                name_len = (uint32_t)strlen(user_regions->regions[i].region_name) > (sizeof(minidump_header.regions[k].name) / sizeof(minidump_header.regions[k].name[0])) ?
                           (sizeof(minidump_header.regions[k].name) / sizeof(minidump_header.regions[k].name[0])) : (uint32_t)strlen(user_regions->regions[i].region_name) ;

                memcpy(minidump_header.regions[k].name, user_regions->regions[i].region_name, name_len);
                minidump_header.regions[k].address = (uint32_t)current;

                /* remain size is enough to dump */
                if (region_remain_size > region_size) {
                    minidump_header.regions[k].size = region_size;
                } else {
                    /* remain size is not enough to dump,we just dump remain size */
                    minidump_header.regions[k].size = region_remain_size;
                    region_remain_size = 0;
                }

                if (current < end) {
                    /* output data */
                    if (exception_check_is_instruction_memory((uint32_t)current, (uint32_t)end) != EXCEPTION_STATUS_OK) {
                        /* feed wdt to keep time for output data */
                        exception_feed_wdt();
                        /* output data */
                        ret = offline_dump_region_write(OFFLINE_REGION_MINI_DUMP, current_addr, (uint8_t *)user_regions->regions[i].start_address, minidump_header.regions[k++].size);
                        if (ret != true) {
                            minidump_base_address = 0;
                            return;
                        }
                        current_addr += region_size;
                    } else {
                        end = (uint32_t *)((uint32_t)current + minidump_header.regions[k++].size);
                        /*  Because the instruction memory can only be accessed by l32i,
                        	readback the data in temp buffer and output the temp buffer */
                        for (; current < end; current += 32) {
                            if ((end - current) >= 32) {
                                /* copy 32 words data into data buffer */
                                for (j = 0; j < 32; j++) {
                                    data[j] = *(current + j);
                                }

                                /* feed wdt to keep time for output data */
                                exception_feed_wdt();
                                ret = offline_dump_region_write(OFFLINE_REGION_MINI_DUMP, current_addr, (uint8_t *)&data, 32 * 4);
                                if (ret != true) {
                                    minidump_base_address = 0;
                                    return;
                                }

                                current_addr += 32 * 4;
                            } else {
                                /* copy the left words data into data buffer */
                                for (j = 0; j < (uint32_t)(end - current); j++) {
                                    data[j] = *(current + j);
                                }
                                /* feed wdt to keep time for output data */
                                exception_feed_wdt();

                                ret = offline_dump_region_write(OFFLINE_REGION_MINI_DUMP, current_addr, (uint8_t *)&data, (end - current) * 4);
                                if (ret != true) {
                                    minidump_base_address = 0;
                                    return;
                                }
                                current_addr += (end - current) * 4;
                            }
                        }
                    }
                }
            }
        }
    }
#else

	uint32_t current_addr = 0;
	bool ret = false;
	uint32_t sp = 0;

	/* get current stack - 16 */
	sp = *(uint32_t *)((uint32_t)(&exception_context) + exception_context.windowbase * 4 * 4 + 4) - 16;

	/* static regions */
	if(minidump_base_address != 0)
	{
		/* dump current stacks */
		current_addr = minidump_base_address + MINIDUMP_HEADER_LENGTH + MINIDUMP_CONTEXT_LENGTH;
		ret = exception_minidump_check_address(sp);
		if(ret == EXCEPTION_STATUS_OK)
		{
			/* feed wdt to keep time for output data */
			exception_feed_wdt();
			/* output data */
			ret = offline_dump_region_write(OFFLINE_REGION_MINI_DUMP,
											current_addr,
											(uint8_t *)(sp),
											MINIDUMP_CURRENTSTACK_LENGTH);
			if(ret != true)
			{
				minidump_base_address = 0;
				return;
			}
			minidump_header.regions[0].address = sp;
			minidump_header.regions[0].size = MINIDUMP_CURRENTSTACK_LENGTH ;
		}
		/* Current stack is not a valid value, so jump this region and set its begin address as 0xffffffff - MINIDUMP_CURRENTSTACK_LENGTH */
		else
		{
			minidump_header.regions[0].address = 0xffffffff - MINIDUMP_CURRENTSTACK_LENGTH;
			minidump_header.regions[0].size = MINIDUMP_CURRENTSTACK_LENGTH ;
		}
	}

	/* Just to avoid compiler warnings. */
	( void ) static_regions;
	( void ) user_regions;

#endif
}

>>>>>>> db20e11 (second commit)
#endif /* EXCEPTION_MEMDUMP_MODE */

static void exception_dump_memory(void)
{
    /* Memory Dump */
#if (EXCEPTION_MEMDUMP_MODE & EXCEPTION_MEMDUMP_TEXT) || (EXCEPTION_MEMDUMP_MODE & EXCEPTION_MEMDUMP_BINARY)
    if((exception_dump_mode.exception_mode & EXCEPTION_MEMDUMP_TEXT) || (exception_dump_mode.exception_mode & EXCEPTION_MEMDUMP_BINARY))
    {
        /* feed wdt to keep time for memory dump */
        exception_feed_wdt();

        /* Print Regions' information */
        exception_printf("DSP0 Regions Information:\r\n");
        exception_dump_region_info(memory_regions, &exception_user_regions);

        #if (EXCEPTION_MEMDUMP_MODE & EXCEPTION_MEMDUMP_TEXT)
        if(exception_dump_mode.exception_mode & EXCEPTION_MEMDUMP_TEXT)
        {
            /* Print Regions' data */
            exception_dump_region_data_text(memory_regions, &exception_user_regions);
        }
        #endif /* EXCEPTION_MEMDUMP_MODE */

        #if (EXCEPTION_MEMDUMP_MODE & EXCEPTION_MEMDUMP_BINARY)
        if(exception_dump_mode.exception_mode & EXCEPTION_MEMDUMP_BINARY)
        {
            /* Print Memory one by one regions */
            exception_printf("DSP0 Regions Data:\r\n");
            /* Print Regions' data */
            exception_dump_region_data_binary(memory_regions, &exception_user_regions);
        }
        #endif /* EXCEPTION_MEMDUMP_MODE */

        /* dump cm4 end log */
        exception_printf("\r\nDSP0 memory dump completed.\r\n");
    }
#endif /* EXCEPTION_MEMDUMP_MODE */

#if (EXCEPTION_MEMDUMP_MODE & EXCEPTION_MEMDUMP_MINIDUMP)
    if(exception_dump_mode.exception_mode & EXCEPTION_MEMDUMP_MINIDUMP)
    {
        /* feed wdt to keep time for memory dump */
        exception_feed_wdt();

        /* dump memory data into flash */
        exception_dump_region_data_minidump(memory_regions, &exception_user_regions);
    }
#endif /* EXCEPTION_MEMDUMP_MODE */
}

/******************************************************************************/
/*            Exception's dump processor Functions                            */
/******************************************************************************/
void exception_dump_preprocess(uint32_t fault_type)
{
    exception_info.reason = fault_type;
    exception_info.assert_expr = &assert_expr;

#if (EXCEPTION_MEMDUMP_MODE & EXCEPTION_MEMDUMP_TEXT) || (EXCEPTION_MEMDUMP_MODE & EXCEPTION_MEMDUMP_BINARY)
    if((exception_dump_mode.exception_mode & EXCEPTION_MEMDUMP_TEXT) || (exception_dump_mode.exception_mode & EXCEPTION_MEMDUMP_BINARY))
    {
<<<<<<< HEAD
        /* feed wdt to keep time for preprocess */
        exception_feed_wdt();

        /* Genie start message */
        exception_printf("<<<<<<<< LOG START LOG START LOG START LOG START LOG START <<<<<<<<\r\n");

        exception_printf("\r\nDSP0 Fault Dump:\r\n");
        exception_print_assert_info();
        /* NMI interrupt */
        if(exception_context.epsnmi != 0x0)
        {
            exception_printf("\r\nIn NMI Fault Handler\r\n");
        }
        /* Debug exception */
        if(exception_context.debugcause != 0x0)
        {
            exception_printf("\r\nIn Debug Monitor Fault Handler\r\n");
        }
        exception_printf("Exception Count = 0x%08x\r\n", (unsigned int)exception_info.count);
        exception_printf("Exception Time = 0x%08x\r\n", (unsigned int)exception_info.timestamp);
        exception_printf("Exception Reason = 0x%08x\r\n", fault_type);
    }
#endif /* EXCEPTION_MEMDUMP_MODE */
=======
        /* Genie start message */
        exception_printf("<<<<<<<< LOG START LOG START LOG START LOG START LOG START <<<<<<<<\r\n");
	}
#endif 

	/* feed wdt to keep time for preprocess */
	exception_feed_wdt();

	exception_printf("\r\nDSP0 Fault Dump:\r\n");
	exception_print_assert_info();
	/* NMI interrupt */
	if(exception_context.epsnmi != 0x0)
	{
		exception_printf("\r\nIn NMI Fault Handler\r\n");
	}
	/* Debug exception */
	if(exception_context.debugcause != 0x0)
	{
		exception_printf("\r\nIn Debug Monitor Fault Handler\r\n");
	}
	exception_printf("Exception Count = 0x%08x\r\n", (unsigned int)exception_info.count);
	exception_printf("Exception Time = 0x%08x\r\n", (unsigned int)exception_info.timestamp);
	exception_printf("Exception Reason = 0x%08x\r\n", fault_type);

>>>>>>> db20e11 (second commit)

#if (EXCEPTION_MEMDUMP_MODE & EXCEPTION_MEMDUMP_MINIDUMP)
    if(exception_dump_mode.exception_mode & EXCEPTION_MEMDUMP_MINIDUMP)
    {
        /* feed wdt to keep time for preprocess */
        exception_feed_wdt();

        minidump_header.reason = fault_type;
        if (assert_expr.is_valid)
        {
            minidump_header.assert_info.is_valid = assert_expr.is_valid;
            minidump_header.assert_info.expr = assert_expr.expr;
            minidump_header.assert_info.file = assert_expr.file;
            minidump_header.assert_info.line = assert_expr.line;
        }
        else
        {
            minidump_header.assert_info.is_valid = false;
        }

        /* feed wdt to keep time for reset the latest index*/
        exception_feed_wdt();
        /* reset the latest index */
        exception_minidump_update_latest_index(0x0);

        /* check if there has been same minidump on the flash */
        if(exception_minidump_check_not_duplicated() == EXCEPTION_STATUS_OK)
        {
            /* there is not the same minidump on the flash, do this minidump */
            /* clean FOTA status to prevent FOTA upgrade after reboot */
            exception_minidump_fota_status_clean();

            /* feed wdt to keep time for offline dump init*/
            exception_feed_wdt();
            /* offline dump init should be done in system init stage */
            /* for prevent that offline dump status is corrupted, do it again in here */
            offline_dump_region_init();

            /* feed wdt to keep time for offline dump alloc */
            exception_feed_wdt();
            /*  Get the minidump region address */
            if(offline_dump_region_alloc(OFFLINE_REGION_MINI_DUMP, &minidump_base_address) != true)
            {
                minidump_base_address = 0;
            }
        }
        else
        {
            /* there is the same minidump on the flash, drop this dump */
            minidump_base_address = 0;
        }
    }
#endif /* EXCEPTION_MEMDUMP_MODE */
}

void exception_dump_postprocess(void)
{
<<<<<<< HEAD
	int i = 0;

#if (EXCEPTION_MEMDUMP_MODE & EXCEPTION_MEMDUMP_TEXT) || (EXCEPTION_MEMDUMP_MODE & EXCEPTION_MEMDUMP_BINARY)
		if((exception_dump_mode.exception_mode & EXCEPTION_MEMDUMP_TEXT) || (exception_dump_mode.exception_mode & EXCEPTION_MEMDUMP_BINARY))
		{
			for (i = 0; i < exception_config.items; i++)
			{
				if (exception_config.configs[i].init_cb)
				{
					/* feed wdt to keep time for init callback */
					exception_feed_wdt();
					/* run init callback */
					exception_config.configs[i].init_cb();
				}
			}
		}
=======
    int i = 0;

#if (EXCEPTION_MEMDUMP_MODE & EXCEPTION_MEMDUMP_TEXT) || (EXCEPTION_MEMDUMP_MODE & EXCEPTION_MEMDUMP_BINARY)
    if((exception_dump_mode.exception_mode & EXCEPTION_MEMDUMP_TEXT) || (exception_dump_mode.exception_mode & EXCEPTION_MEMDUMP_BINARY))
    {
		#ifdef MTK_SWLA_ENABLE
			extern void SLA_MemoryCallbackInit(void);
		#endif 
		
        for (i = 0; i < exception_config.items; i++)
        {
            #ifdef MTK_SWLA_ENABLE
            if (exception_config.configs[i].init_cb && (exception_config.configs[i].init_cb != SLA_MemoryCallbackInit)) 
            #else
            if (exception_config.configs[i].init_cb ) 
            #endif
            {
              /* feed wdt to keep time for init callback */
              exception_feed_wdt();
              /* run init callback */
              exception_config.configs[i].init_cb();
            }
        }
    }
>>>>>>> db20e11 (second commit)
#endif /* EXCEPTION_MEMDUMP_MODE */


#if (EXCEPTION_MEMDUMP_MODE & EXCEPTION_MEMDUMP_TEXT) || (EXCEPTION_MEMDUMP_MODE & EXCEPTION_MEMDUMP_BINARY)
    if((exception_dump_mode.exception_mode & EXCEPTION_MEMDUMP_TEXT) || (exception_dump_mode.exception_mode & EXCEPTION_MEMDUMP_BINARY))
    {
       

        /* feed wdt to keep time for postprocess */
        exception_feed_wdt();

        for (i = 0; i < exception_config.items; i++)
        {
            if (exception_config.configs[i].dump_cb)
            {
                /* feed wdt to keep time for dump callback */
                exception_feed_wdt();
                /* do dump callback */
                exception_config.configs[i].dump_cb();
            }
        }

        /* Genie complete message */
        exception_printf("<<<<<<<< LOG END LOG END LOG END LOG END LOG END <<<<<<<<\r\n");
    }
#endif /* EXCEPTION_MEMDUMP_MODE */

#if (EXCEPTION_MEMDUMP_MODE & EXCEPTION_MEMDUMP_MINIDUMP)
    if(exception_dump_mode.exception_mode & EXCEPTION_MEMDUMP_MINIDUMP)
    {
        uint32_t dump_count = 0;

        /* feed wdt to keep time for postprocess */
        exception_feed_wdt();

        /* write mini dump header */
        if(minidump_base_address != 0)
        {
            exception_minidump_header_update();

            /* feed wdt to keep time for minidump header */
            exception_feed_wdt();
            /* save header into flash */
            offline_dump_region_write(OFFLINE_REGION_MINI_DUMP,
                                    minidump_base_address,
                                    (uint8_t *)&minidump_header,
                                    sizeof(minidump_header));

            /* feed wdt to keep time for minidump ending */
            exception_feed_wdt();
            /* update status to minidump control block */
            if(offline_dump_region_write_end(OFFLINE_REGION_MINI_DUMP, MINIDUMP_TOTAL_SIZE) == true)
            {
                /* update latest index */
                exception_minidump_region_query_count(&dump_count);
                exception_minidump_update_latest_index(dump_count);

                /* log service post-process, such as save log to flash */
                exception_log_service_post_process();
            }
        }
    }
#endif /* EXCEPTION_MEMDUMP_MODE */
}

/******************************************************************************/
/*            DSP Processor Exceptions Handlers                               */
/******************************************************************************/
void exception_dsp_fault_handler(void)
{
    /* dump exception time, fault type, etc */
    exception_dump_preprocess(exception_context_pointer->exccause);

    /* dump the context when the exception happens */
    exception_dump_context(exception_context_pointer);

    /* dump the memory */
    exception_dump_memory();

    /* finish the dump */
    exception_dump_postprocess();

    /* check if reboot */
    if (reboot_check() == DISABLE_WHILELOOP_MAGIC)
    {
        exception_reboot();
    }
	
    /* disable wdt reset mode for entering while loop */
    /* maybe wdt has been changed to interrupt mode for triggering the NMI interrupt */
    exception_disable_wdt_reset();

    while (1);
}
