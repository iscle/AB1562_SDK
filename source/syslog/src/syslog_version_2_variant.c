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

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "syslog.h"
#include "syslog_debug.h"
#include "mux.h"
#include "mux_port_common.h"
#include "nvkey.h"
#include "nvkey_list.h"
#include "hal_gpt.h"
#include "hal_uart.h"
#include "hal_uart_internal.h"
#include "serial_port_assignment.h"
<<<<<<< HEAD
=======
#include "exception_handler.h"
#include "offline_dump.h"
>>>>>>> db20e11 (second commit)

#ifdef MTK_NVDM_ENABLE
#include "nvdm.h"
#endif

/*********port layer*****************/
#define PORT_SYSLOG_UNUSED(a) (void)a

#define NO_INLINE __attribute__((noinline))

/* MSGID log string start address define */
#define PORT_SYSLOG_MSG_ADDR_OFFSET 0x06000000

/* syslog length limitation */
#define PORT_SYSLOG_MAX_ONE_LOG_SIZE (256 + 64)
#define PORT_SYSLOG_MAX_ONE_PLAIN_LOG_SIZE (256)

/* module and filter define */
#define PORT_SYSLOG_MODULE_FILTER_TOTAL_NUMBER 200
#define PORT_SYSLOG_MODULE_FILTER_STATUS_SIZE (MTK_MAX_CPU_NUMBER * 2 + PORT_SYSLOG_MODULE_FILTER_TOTAL_NUMBER)
#define PORT_SYSLOG_MAX_MODULE_FILTER_STATUS_SIZE (PORT_SYSLOG_MAX_CPU_NUMBER * 2 + PORT_SYSLOG_MODULE_FILTER_TOTAL_NUMBER)

<<<<<<< HEAD
=======
/* max drop log buffer size define */
#define MAX_DROP_BUFFER_SIZE                    38

>>>>>>> db20e11 (second commit)
/* syslog status define */
#define SYSLOG_INIT_NONE            0x00
#define SYSLOG_EARLY_INIT           0x01
#define SYSLOG_FULL_INIT            0x02
#define SYSLOG_EXCEPTION_INIT       0x03

/* RACE PROTOCOL */
#define RACE_TYPE_CMD_WITH_RESPONCE             0x5A
#define RACE_TYPE_RESPONCE                      0x5B
#define RACE_TYPE_CMD_WITHOUT_RESPONCE          0x5C
#define RACE_TYPE_NOTIFICATION                  0x5D

#define RACE_PROTOCAL_STRING_LOG                (0x0F00 | 0x10)
#define RACE_PROTOCAL_MSGID_LOG                 (0x0F00 | 0x11)
#define RACE_PROTOCAL_EXCEPTION_STRING_LOG      (0x0F00 | 0x12)
#define RACE_PROTOCAL_EXCEPTION_BINARY_LOG      (0x0F00 | 0x13)
#define RACE_PROTOCAL_ACTIVE_ASSERT             (0x0F00 | 0x14)
#define RACE_PROTOCAL_QUERY_VERSION_BUILDTIME   (0x0F00 | 0x15)
#define RACE_PROTOCAL_GET_LOG_FILTER_INFO       (0x0F00 | 0x16)
#define RACE_PROTOCAL_SET_LOG_FILTER_INFO       (0x0F00 | 0x17)
#define RACE_PROTOCAL_SAVE_LOG_SETTING          (0x0F00 | 0x18)
#define RACE_PROTOCAL_EXCEPTION_MSGID           (0x0F00 | 0x1A)
#define RACE_PROTOCAL_TLV_LOG                   (0x0F00 | 0x00)

<<<<<<< HEAD
=======
#ifndef __EXT_BOOTLOADER__
log_create_module(SYSLOG, PRINT_LEVEL_INFO);
#define SYSLOG_MSGID_I(fmt, cnt, arg...)    LOG_MSGID_I(SYSLOG, fmt, cnt, ##arg)
#define SYSLOG_MSGID_W(fmt, cnt, arg...)    LOG_MSGID_W(SYSLOG, fmt, cnt, ##arg)
#define SYSLOG_MSGID_E(fmt, cnt, arg...)    LOG_MSGID_E(SYSLOG, fmt, cnt, ##arg)
#else
#define SYSLOG_MSGID_I(fmt, cnt, arg...)
#define SYSLOG_MSGID_W(fmt, cnt, arg...)
#define SYSLOG_MSGID_E(fmt, cnt, arg...)
#endif /* __EXT_BOOTLOADER__ */

>>>>>>> db20e11 (second commit)
__attribute__ ((__section__(".log_filter_end"))) static const char fiter_end_flag[] = "[MODULE_FILTER_END]";

static uint32_t port_syslog_get_current_timestamp(void)
{
    uint32_t count = 0;
    uint64_t count64 = 0;

    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &count);
    count64 = ((uint64_t)count) * 1000 / 32768;

    return (unsigned int)count64;
}

static void port_syslog_tx_hook(uint32_t id, uint8_t *p_data, uint32_t timestamp)
{
    if ((id == RACE_PROTOCAL_STRING_LOG) || (id == RACE_PROTOCAL_MSGID_LOG)) {
        p_data[2] = timestamp & 0xFF;
        p_data[3] = (timestamp >> 8) & 0xFF;
        p_data[4] = (timestamp >> 16) & 0xFF;
        p_data[5] = (timestamp >> 24) & 0xFF;
    }
}

#define DROP_PROMPT_LOG ">>> log drop "
#define DROP_PROMPT_LOG_FIX_SIZE        (sizeof(DROP_PROMPT_LOG) - 1) /* substract \0 */

static uint32_t uint_to_str(uint32_t number, uint8_t *str_array)
{
    uint32_t i, value, bits;
    uint8_t byte_array[16];

    bits = 0;
    value = number;
    do {
        byte_array[bits++] = value % 10;
        value /= 10;
    } while (value);

    for (i = 0; i < bits; i++) {
        str_array[i] = byte_array[bits - i - 1] + '0';
    }

    return bits;
}

uint32_t port_syslog_drop_prompt_length(uint32_t drop_log_count)
{
    uint32_t drop_bit;

    drop_bit = 0;
    while (drop_log_count / 10) {
        drop_log_count /= 10;
        drop_bit++;
    }
    if (drop_log_count) {
        drop_bit++;
    }

#ifdef MTK_DEBUG_PLAIN_LOG_ENABLE
<<<<<<< HEAD
    return DROP_PROMPT_LOG_FIX_SIZE + drop_bit + 2; /* include the race header */
#else
    return 12 + DROP_PROMPT_LOG_FIX_SIZE + drop_bit + 2; /* include the race header */
=======
    return DROP_PROMPT_LOG_FIX_SIZE + drop_bit + 2; /* string + drop + /r/n */
#else
    return 12 + DROP_PROMPT_LOG_FIX_SIZE + drop_bit + 2; /* race header + string + drop + /r/n */
>>>>>>> db20e11 (second commit)
#endif
}

void port_syslog_drop_prompt(uint32_t timestamp, uint32_t drop_log_size, uint32_t drop_log_count, uint8_t *drop_prompt)
{
    uint32_t bits;
    uint8_t array[16];
    uint8_t *curr_drop_prompt;

    curr_drop_prompt = drop_prompt;

    /* race header */
#ifndef MTK_DEBUG_PLAIN_LOG_ENABLE
    uint32_t i = 0;
    array[i++] = 0x05;
    array[i++] = RACE_TYPE_NOTIFICATION;
    array[i++] = (drop_log_size - 4) & 0xFF;
    array[i++] = ((drop_log_size - 4) >> 8) & 0xFF;
    array[i++] = RACE_PROTOCAL_STRING_LOG & 0xFF;
    array[i++] = (RACE_PROTOCAL_STRING_LOG >> 8) & 0xFF;
    array[i++] = GET_CURRENT_CPU_ID() + 1;
    array[i++] = 0x00;
    array[i++] = timestamp & 0xFF;
    array[i++] = (timestamp >> 8) & 0xFF;
    array[i++] = (timestamp >> 16) & 0xFF;
    array[i++] = (timestamp >> 24) & 0xFF;
    memcpy(curr_drop_prompt, array, i);
    curr_drop_prompt += i;
#else
    PORT_SYSLOG_UNUSED(timestamp);
    PORT_SYSLOG_UNUSED(drop_log_size);
#endif

    /* drop log prefix */
    memcpy(curr_drop_prompt, DROP_PROMPT_LOG, DROP_PROMPT_LOG_FIX_SIZE);
    curr_drop_prompt += DROP_PROMPT_LOG_FIX_SIZE;

    /* drop log count */
    bits = uint_to_str(drop_log_count, array);
    memcpy(curr_drop_prompt, array, bits);
    curr_drop_prompt += bits;

    /* add \r\n */
    memcpy(curr_drop_prompt, "\r\n", 2);
}


static void port_syslog_local_cpu_enter_critical(uint32_t *cpu_irq_mask)
{
    hal_nvic_save_and_set_interrupt_mask(cpu_irq_mask);
}

static void port_syslog_local_cpu_exit_critical(uint32_t cpu_irq_mask)
{
    hal_nvic_restore_interrupt_mask(cpu_irq_mask);
}

#include "FreeRTOS.h"
#include "task.h"

#define MAX_NEST_TASK_NUMBER 16
static uint32_t g_syslog_user_task_number;
static TaskHandle_t g_syslog_user_task_context[MAX_NEST_TASK_NUMBER];
#define MAX_NEST_IRQ_NUMBER 64
static uint32_t g_syslog_user_irq_context[(MAX_NEST_IRQ_NUMBER + 31) / 32];

static bool port_syslog_anti_nest_check_begin(void)
{
    uint32_t i, j, cpu_irq_mask;
    TaskHandle_t task_context;

    port_syslog_local_cpu_enter_critical(&cpu_irq_mask);

    if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) {
        /* Init context */
        if (g_syslog_user_task_context[0] != NULL) {
            return false;
        }
        g_syslog_user_task_context[0] = (TaskHandle_t)1;
    } else if (HAL_NVIC_QUERY_EXCEPTION_NUMBER == HAL_NVIC_NOT_EXCEPTION) {
        /* Task context */
        i = 0;
        j = 0;
        task_context = xTaskGetCurrentTaskHandle();
        do {
            if (g_syslog_user_task_context[i] == task_context) {
                port_syslog_local_cpu_exit_critical(cpu_irq_mask);
                return false;
            }
            if (g_syslog_user_task_context[i] != NULL) {
                j++;
            }
            i++;
        } while (j < g_syslog_user_task_number);
        for (i = 0; i < MAX_NEST_TASK_NUMBER; i++) {
            if (g_syslog_user_task_context[i] == NULL) {
                g_syslog_user_task_context[i] = task_context;
                g_syslog_user_task_number++;
                break;
            }
        }
        assert(i < MAX_NEST_TASK_NUMBER);
    } else {
        /* IRQ context */
        if (g_syslog_user_irq_context[HAL_NVIC_QUERY_EXCEPTION_NUMBER / 32] & (1 << (HAL_NVIC_QUERY_EXCEPTION_NUMBER % 32))) {
            port_syslog_local_cpu_exit_critical(cpu_irq_mask);
            return false;
        }
        g_syslog_user_irq_context[HAL_NVIC_QUERY_EXCEPTION_NUMBER / 32] |= 1 << (HAL_NVIC_QUERY_EXCEPTION_NUMBER % 32);
    }

    port_syslog_local_cpu_exit_critical(cpu_irq_mask);

    return true;
}

static void port_syslog_anti_nest_check_end(void)
{
    uint32_t i, cpu_irq_mask;
    TaskHandle_t task_context;

    port_syslog_local_cpu_enter_critical(&cpu_irq_mask);

    if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) {
        /* Init context */
        g_syslog_user_task_context[0] = NULL;
    } else if (HAL_NVIC_QUERY_EXCEPTION_NUMBER == HAL_NVIC_NOT_EXCEPTION) {
        /* Task context */
        task_context = xTaskGetCurrentTaskHandle();
        for (i = 0; i < MAX_NEST_TASK_NUMBER; i++) {
            if (g_syslog_user_task_context[i] == task_context) {
                g_syslog_user_task_context[i] = NULL;
                g_syslog_user_task_number--;
                break;
            }
        }
    } else {
        /* IRQ context */
        g_syslog_user_irq_context[HAL_NVIC_QUERY_EXCEPTION_NUMBER / 32] &= ~(1 << (HAL_NVIC_QUERY_EXCEPTION_NUMBER % 32));
    }

    port_syslog_local_cpu_exit_critical(cpu_irq_mask);
}




#if !defined(MTK_DEBUG_LEVEL_PRINTF) &&  !defined(__EXT_BOOTLOADER__)

/* share syslog variable define */
#define PORT_SYSLOG_MAX_CPU_NUMBER 16

/*
 * Global share variable shared by all CPUs, make sure every member keep align access.
 * [cpu_module_filter_status]:
 *      cpu1_id                   1 byte
 *      cpu1_module_number        1 byte
 *      cpu2_id                   1 byte
 *      cpu2_module_number        1 byte
 *        ..                        ..
 *      cpuN_id                   1 byte
 *      cpuN_module_number        1 byte
 *      valid_cpu1_module1_log_switch   1 byte (high 4 bit)
 *      valid_cpu1_module1_print_level  1 byte (low 4 bit)
 *        ..                              ..
 *      valid_cpu1_moduleM_log_switch   1 byte (high 4 bit)
 *      valid_cpu1_moduleM_print_level  1 byte (low 4 bit)
 *      valid_cpu2_module1_log_switch   1 byte (high 4 bit)
 *      valid_cpu2_module1_print_level  1 byte (low 4 bit)
 *        ..                              ..
 *      valid_cpu2_moduleM_log_switch   1 byte (high 4 bit)
 *      valid_cpu2_moduleM_print_level  1 byte (low 4 bit)
 * [cpu_module_filter]:
 *      per-cpu needs to do address transfer to CM4 view for access by CM4 side.
 */
typedef struct {
    mux_handle_t handle;
    uint32_t init_phase;
    uint32_t lock_buffer;
<<<<<<< HEAD
=======
    uint32_t drop_count;
    uint32_t drop_flush_count;
>>>>>>> db20e11 (second commit)
    uint8_t cpu_log_switch[PORT_SYSLOG_MAX_CPU_NUMBER];
    uint8_t cpu_log_print_level[PORT_SYSLOG_MAX_CPU_NUMBER];
    uint8_t cpu_module_filter_status[PORT_SYSLOG_MAX_MODULE_FILTER_STATUS_SIZE]; /* log filter setting read from NVDM to let per-cpu do initialization with it's log filter array. */
    void *cpu_module_filter[PORT_SYSLOG_MAX_CPU_NUMBER]; /* Init to per-cpu's log filter array when per-cpu call log_set_filter() in it's init phase. */
} syslog_share_variable_t;

#ifdef MTK_SINGLE_CPU_ENV
ATTR_ZIDATA_IN_NONCACHED_SYSRAM_4BYTE_ALIGN static volatile syslog_share_variable_t syslog_share_variable;
static volatile syslog_share_variable_t *g_syslog_share_variable = &syslog_share_variable;
#else
#include "hal_resource_assignment.h"
#define PORT_SYSLOG_SHARE_VARIABLE_BEGIN HW_SYSRAM_PRIVATE_MEMORY_SYSLOG_VAR_START
static volatile syslog_share_variable_t *g_syslog_share_variable = (volatile syslog_share_variable_t *)(PORT_SYSLOG_SHARE_VARIABLE_BEGIN);
#endif /* MTK_SINGLE_CPU_ENV */

/* log filters collection */
#if defined(__GNUC__)
extern uint8_t _log_filter_start[];
extern uint8_t _log_filter_end[];

#define LOG_FILTER_START _log_filter_start
#define LOG_FILTER_END _log_filter_end
#endif /* __GNUC__ */

#define port_syslog_memory_remap_to_primary_cpu(cpu_id, address) address

/* SDK version and build time */
#ifdef MTK_CPU_NUMBER_0
extern char sw_verno_str[];
#else /* MTK_CPU_NUMBER_0 */
#include "memory_attribute.h"
#define DUMMY_SDK_VERSION_STRING "DUMMY_SDK_VERSION"
static char sw_verno_str[] = DUMMY_SDK_VERSION_STRING;
ATTR_LOG_VERSION log_bin_sw_verno_str[] = DUMMY_SDK_VERSION_STRING;
#endif /* MTK_CPU_NUMBER_0 */

extern char build_date_time_str[];

#ifdef MTK_SINGLE_CPU_ENV
#define PORT_SYSLOG_SDK_VERSION_BEGIN   sw_verno_str
#define PORT_SYSLOG_SDK_VERSION_LENGTH  strlen(sw_verno_str)
#define PORT_SYSLOG_BUILD_TIME_BEGIN    build_date_time_str
#define PORT_SYSLOG_BUILD_TIME_LENGTH   strlen(build_date_time_str)
static void port_syslog_build_time_sdk_version_copy(uint32_t cpu_id)
{
    strcpy((char *)(PORT_SYSLOG_SDK_VERSION_BEGIN + cpu_id * PORT_SYSLOG_SDK_VERSION_LENGTH), sw_verno_str);
    strcpy((char *)(PORT_SYSLOG_BUILD_TIME_BEGIN  + cpu_id * PORT_SYSLOG_BUILD_TIME_LENGTH),  build_date_time_str);
}
#else /* MTK_SINGLE_CPU_ENV */
#define PORT_SYSLOG_SDK_VERSION_BEGIN   HW_SYSRAM_PRIVATE_MEMORY_SYSLOG_VERSION_VAR_START
#define PORT_SYSLOG_SDK_VERSION_LENGTH  48
#define PORT_SYSLOG_BUILD_TIME_BEGIN    HW_SYSRAM_PRIVATE_MEMORY_SYSLOG_BUILD_TIME_VAR_START
#define PORT_SYSLOG_BUILD_TIME_LENGTH   32
static void port_syslog_build_time_sdk_version_copy(uint32_t cpu_id)
{
    strcpy((char *)(PORT_SYSLOG_SDK_VERSION_BEGIN + cpu_id * PORT_SYSLOG_SDK_VERSION_LENGTH), sw_verno_str);
    strcpy((char *)(PORT_SYSLOG_BUILD_TIME_BEGIN +  cpu_id * PORT_SYSLOG_BUILD_TIME_LENGTH),  build_date_time_str);
}
#endif /* MTK_SINGLE_CPU_ENV */

/* memory malloc and free */
#ifdef MTK_CPU_NUMBER_0

<<<<<<< HEAD
=======
static volatile bool g_primary_cpu_syslog_is_initialized = false;
>>>>>>> db20e11 (second commit)
static volatile bool g_exception_is_happen = false;

static void *port_syslog_malloc(uint32_t size)
{
    return malloc(size);
}

static void port_syslog_free(void *pdata)
{
    free(pdata);
}

/* syslog setting read/write to non-volatile memory */
#ifdef MTK_NVDM_ENABLE
uint32_t port_syslog_read_setting(char *name, uint8_t *p_setting, uint32_t size)
{
    uint32_t max_size;
    nvdm_status_t status;

    max_size = size;
    status = nvdm_read_data_item("SYSLOG", name, p_setting, &max_size);
    if (status != NVDM_STATUS_OK) {
        return false;
    }

    return true;
}

bool port_syslog_save_setting(char *name, uint8_t *p_setting, uint32_t size)
{
    nvdm_status_t status;

    status = nvdm_write_data_item("SYSLOG", name, NVDM_DATA_ITEM_TYPE_RAW_DATA, p_setting, size);
    if (status != NVDM_STATUS_OK) {
        return false;
    }

    return true;
}

bool port_syslog_save_is_available(void)
{
    return true;
}

#else /* MTK_NVDM_ENABLE */

uint32_t port_syslog_read_setting(char *name, uint8_t *p_setting, uint32_t size)
{
    PORT_SYSLOG_UNUSED(name);
    PORT_SYSLOG_UNUSED(p_setting);
    PORT_SYSLOG_UNUSED(size);

    return false;
}

bool port_syslog_save_setting(char *name, uint8_t *p_setting, uint32_t size)
{
    PORT_SYSLOG_UNUSED(name);
    PORT_SYSLOG_UNUSED(p_setting);
    PORT_SYSLOG_UNUSED(size);

    return false;
}

bool port_syslog_save_is_available(void)
{
    return false;
}

#endif /* MTK_NVDM_ENABLE */

static void port_syslog_early_init(void)
{
    hal_uart_config_t uart_config;

<<<<<<< HEAD
    uart_config.baudrate        = CONFIG_RACE_BAUDRATE;
=======
    uart_config.baudrate        = CONFIG_SYSLOG_BAUDRATE;
>>>>>>> db20e11 (second commit)
    uart_config.parity          = HAL_UART_PARITY_NONE;
    uart_config.stop_bit        = HAL_UART_STOP_BIT_1;
    uart_config.word_length     = HAL_UART_WORD_LENGTH_8;
    hal_uart_init(CONFIG_SYSLOG_INIT_STAGE_PORT, &uart_config);
#ifndef MTK_DEBUG_PLAIN_LOG_ENABLE
    hal_uart_set_software_flowcontrol(CONFIG_SYSLOG_INIT_STAGE_PORT, 0x11, 0x13, 0x77);
#endif
}

void pc_tool_command_handler(mux_handle_t handle, mux_event_t event,uint32_t data_len,void*user_data);
<<<<<<< HEAD
=======
void pc_tool_assert(uint8_t *data, uint32_t length);
>>>>>>> db20e11 (second commit)

typedef enum {
    RACE_PARSE_STATUS_CHANNEL,
    RACE_PARSE_STATUS_TYPE,
    RACE_PARSE_STATUS_LENGTH,
    RACE_PARSE_STATUS_ID,
} race_parse_status_t;

typedef struct {
    char *port_name;
    mux_port_t port;
    mux_port_setting_t setting;
    mux_handle_t *handle;
    uint32_t race_id_begin;
    uint32_t race_id_end;
    bool drop_race_header;   //true : drop race head   false: not drop
} user_mux_config_t;

mux_handle_t            syslog_mux_handle;
mux_handle_t            syslog_exception_handle;
mux_port_t              g_syslog_port;
uint32_t                g_syslog_port_index;
hal_uart_baudrate_t     g_syslog_baudrate;
print_level_t           g_cpu_level[2];
<<<<<<< HEAD

=======
bool    g_offline_log_is_enable;

mux_port_t query_syslog_port(void)
{
    if ((g_syslog_share_variable->init_phase == SYSLOG_EARLY_INIT)
            || (g_syslog_share_variable->init_phase == SYSLOG_INIT_NONE)
            || (g_primary_cpu_syslog_is_initialized == false)) {
        return CONFIG_SYSLOG_INIT_STAGE_PORT;
    }

    return g_syslog_port;
}

hal_uart_baudrate_t query_syslog_baudrate(void)
{
    if ((g_syslog_share_variable->init_phase == SYSLOG_EARLY_INIT)
            || (g_syslog_share_variable->init_phase == SYSLOG_INIT_NONE)
            || (g_primary_cpu_syslog_is_initialized == false)) {
        return CONFIG_SYSLOG_BAUDRATE;
    }

    return g_syslog_baudrate;
}
>>>>>>> db20e11 (second commit)

#ifdef MTK_DEBUG_PLAIN_LOG_ENABLE
static user_mux_config_t g_syslog_race_user_list[1];

static mux_buffer_t *g_mux_buffer;
static uint32_t      g_mux_buffer_offset, g_mux_buffer_idx,g_mux_buffer_counter,g_mux_buffer_left_data_len;

static void multi_buffer_fetch_init( mux_buffer_t buffers[],uint32_t buffers_counter)
{
    uint32_t i;
    g_mux_buffer = buffers;
    g_mux_buffer_offset = 0;
    g_mux_buffer_idx = 0;
    g_mux_buffer_counter = buffers_counter;
    g_mux_buffer_left_data_len = 0;
    for(i=0;i<buffers_counter;i++)
        g_mux_buffer_left_data_len+=buffers[i].buf_size;
}

static bool protocol_header_fetch(uint8_t *out_buf, uint32_t out_len)
{
    uint32_t i;

    if (g_mux_buffer_idx >= g_mux_buffer_counter) {
        return false;
    }

    if(g_mux_buffer_left_data_len<out_len)
        return false;

    for(i=0;i<out_len;i++,g_mux_buffer_left_data_len--,g_mux_buffer_offset++)
    {
        if(g_mux_buffer_offset>=g_mux_buffer[g_mux_buffer_idx].buf_size)
        {
            g_mux_buffer_idx++;
            g_mux_buffer_offset = 0;
            if(g_mux_buffer_idx>=g_mux_buffer_counter)
                assert(0);
        }
        *(out_buf+i) = *(g_mux_buffer[g_mux_buffer_idx].p_buf + g_mux_buffer_offset);
    }
    return true;
}

void race_tx_protocol_callback(mux_handle_t handle, const mux_buffer_t payload[],uint32_t buffers_counter,
                mux_buffer_t *head, mux_buffer_t *tail,void *user_data)
{
    uint32_t i;
    uint32_t total_size;
    uint32_t race_id;
    PORT_SYSLOG_UNUSED(user_data);
    for (i = 0; i < sizeof(g_syslog_race_user_list)/sizeof(user_mux_config_t); i++) {
        if(*g_syslog_race_user_list[i].handle == handle){
            //todo : use drop flag  for ATCI, RX is need drop; but TX is need pad
            if(syslog_mux_handle == handle) {
                head->p_buf = NULL;
                tail->p_buf = NULL;
                head->buf_size = 0;
                tail->buf_size = 0;
                return;
            }

            race_id = g_syslog_race_user_list[i].race_id_begin;
            break;
        }
    }

    total_size = 0;
    for (i = 0; i<buffers_counter; i++) {
        total_size += payload[i].buf_size;
    }

    /* Insert the race header here */
    head->p_buf[0] = 0x05;
    head->p_buf[1] = RACE_TYPE_NOTIFICATION;
    head->p_buf[2] = (uint8_t)((total_size + 2) & 0xFF);
    head->p_buf[3] = (uint8_t)(((total_size + 2) >> 8) & 0xFF);
    head->p_buf[4] = (uint8_t)(race_id & 0xFF);
    head->p_buf[5] = (uint8_t)((race_id >> 8) & 0xFF);
    head->buf_size = 6;

    tail->p_buf = NULL;
    tail->buf_size = 0;
}

void race_rx_protocol_callback(mux_handle_t *handle, mux_buffer_t buffers[],uint32_t buffers_counter, uint32_t *consume_len, uint32_t *package_len,void *user_data)
{
    uint32_t i, curr_consume_len;
    uint8_t *p_curr_pos;
    uint8_t race_channel, race_type;
    uint16_t race_length, race_id;

    race_length = 0;
    curr_consume_len = 0;
    *package_len = 0;
    *consume_len = 0;
    PORT_SYSLOG_UNUSED(p_curr_pos);
    PORT_SYSLOG_UNUSED(user_data);
    multi_buffer_fetch_init(buffers,buffers_counter);

    if ((protocol_header_fetch(&race_channel, 1) == false) || (race_channel != 0x05)) {
        *package_len = 0;
        *consume_len = g_mux_buffer_left_data_len;
        return;
    }

    if (protocol_header_fetch(&race_type, 1) == false) {
        *package_len = 0;
        *consume_len = g_mux_buffer_left_data_len;
        return;
    }
    if ((race_type < RACE_TYPE_CMD_WITH_RESPONCE) && (race_type > RACE_TYPE_NOTIFICATION)) {
        *package_len = 0;
        *consume_len = g_mux_buffer_left_data_len;
        return;
    }

    if (protocol_header_fetch((uint8_t *)&race_length, 2) == false) {
        *package_len = 0;
        *consume_len = g_mux_buffer_left_data_len;
        return;
    }

    if (protocol_header_fetch((uint8_t *)&race_id, 2)== false) {
        *package_len = 0;
        *consume_len = g_mux_buffer_left_data_len;
        return;
    }

    *handle = 0;
    for(i= 0;i<sizeof(g_syslog_race_user_list)/sizeof(user_mux_config_t);i++) {
        if ((g_syslog_race_user_list[i].race_id_begin <= race_id) &&
            (g_syslog_race_user_list[i].race_id_end >= race_id)) {
            *handle = *g_syslog_race_user_list[i].handle;
            break;
        }
    }

    if (*handle == 0) {
        assert(0);
    }

    if(race_id == 0x0F92){
        /* RACE cmd format: 05 5D length(2 byte) 0f92 at+test=? */
        *package_len = race_length - 2;
        *consume_len = 6;
    } else if ((race_id >= g_syslog_race_user_list[i].race_id_begin)
                && (race_id <= g_syslog_race_user_list[i].race_id_end)) {
        *package_len = race_length + 6 - 2 - 4;   //pc handle think the first byte is race ID
        *consume_len = 4;
    } else {
        /* race command */
        *package_len = race_length + 6 - 2;
        *consume_len = 0;
    }
}
#endif /* MTK_DEBUG_PLAIN_LOG_ENABLE */

void log_from_nvkey_init(void)
{
    uint16_t uart_nvkey_config;
    uint32_t syslog_nvkey_config;
    uint8_t  syslog_mode,syslog_port_num,cpu_level,check_byte;
    uint16_t rc;
    uint8_t  isLogging[5];

<<<<<<< HEAD
=======
    /* default syslog port config */
    g_syslog_port     = CONFIG_SYSLOG_RUNNING_STAGE_PORT;
    g_syslog_baudrate = CONFIG_SYSLOG_BAUDRATE;
    g_cpu_level[0]    = PRINT_LEVEL_INFO;
    g_cpu_level[1]    = PRINT_LEVEL_INFO;

>>>>>>> db20e11 (second commit)
    /* 0xF005 get user's uart configuartion */
    rc = NVKEY_ReadFullKey(NVKEYID_PERIPHERAL_DRV_UART_PARA, &uart_nvkey_config, 2);
    //uart_nvkey_config = BSWAP_16(uart_nvkey_config);
    if (rc == 0){
        return;
    }

    /* 0xF023 get special syslog configuartion */
    rc = NVKEY_ReadFullKey(NVKEYID_PERIPHERAL_1WIRE_LOGGING_SETTING, isLogging, 5);
    if (rc == 0){
        return;
    }

    if(isLogging[4] != 0){
        /* 0xF023 get special syslog configuartion */
        rc = NVKEY_ReadFullKey(NVKEYID_PERIPHERAL_1WIRE_LOGGING_SETTING, &syslog_nvkey_config, 4);
        if (rc == 0){
            return;
        }
    }
    else{
        /* 0xF007 get user's syslog configuartion */
        rc = NVKEY_ReadFullKey(NVKEYID_PERIPHERAL_LOG_SETTING, &syslog_nvkey_config, 4);
        //syslog_nvkey_config = BSWAP_32(syslog_nvkey_config);
        if (rc == 0){
            return;
        }
    }

    syslog_mode     = (uint8_t)(syslog_nvkey_config & 0xFF);
    syslog_port_num = (uint8_t)((syslog_nvkey_config >> 8) & 0xFF);
    cpu_level       = (uint8_t)((syslog_nvkey_config >> 16) & 0xFF);
    check_byte      = (uint8_t)((syslog_nvkey_config >> 24) & 0xFF);

    /* nvkey parameter error */
    if(check_byte != 0x5A ){
        /* default */
        g_syslog_port     = MUX_UART_0;
        g_syslog_baudrate = HAL_UART_BAUDRATE_3000000;
        g_cpu_level[0]    = PRINT_LEVEL_INFO;
        g_cpu_level[1]    = PRINT_LEVEL_INFO;
        return;
    }

    /* get port index */
    g_syslog_port_index     = syslog_port_num & 0x03;

    /* online log check & parse port type */
    if((syslog_mode & 0x10) != 0){//is online log
        g_syslog_port       = MUX_AIRAPP_0;
        g_cpu_level[0]      = PRINT_LEVEL_WARNING;
        g_cpu_level[1]      = PRINT_LEVEL_WARNING;
    }else{//not online log
        if((syslog_mode & 0x01) != 0){
            g_syslog_port = (mux_port_t)(MUX_UART_0 + g_syslog_port_index);
        }else if((syslog_mode & 0x02) != 0){
            //recently 1562 no usb port can use
            //g_syslog_port = MUX_USB_COM_1;
            g_syslog_port = (mux_port_t)(MUX_UART_0 + g_syslog_port_index);
        }else{
            g_syslog_port = MUX_UART_0;
        }
        g_cpu_level[0] = cpu_level &0x03;
        g_cpu_level[1] = (cpu_level >> 2) &0x03;
    }

    /* offline dump enable */
<<<<<<< HEAD
    // if((syslog_mode & 0x20) != 0) {
    //     ;
    // }
=======
    if((syslog_mode & 0x20) != 0) {
        g_offline_log_is_enable = true;
    } else {
        g_offline_log_is_enable = false;
    }
    SYSLOG_MSGID_I("SYSLOG offline dump enable %d", 1, g_offline_log_is_enable);
>>>>>>> db20e11 (second commit)

    /* parse syslog port baudrate */
    if(g_syslog_port_index == 0){
        //g_syslog_baudrate = (hal_uart_baudrate_t)(uart_nvkey_config& 0xFF);/* uart0 low byte*/
    }else if(g_syslog_port_index == 1){
        //g_syslog_baudrate = (hal_uart_baudrate_t)((uart_nvkey_config >> 8) & 0xFF);/* uart1 high byte*/
    }
    g_syslog_baudrate = (hal_uart_baudrate_t)(uart_nvkey_config& 0xFF);/* uart0 low byte*/

<<<<<<< HEAD
    LOG_MSGID_I(common,"SYSLOG NVKEY:check=0x%02x lvel=0x%02x port=0x%02x mode=0x%02x ",4,check_byte,cpu_level,syslog_port_num,syslog_mode);
=======
    SYSLOG_MSGID_I("SYSLOG NVKEY:check=0x%02x lvel=0x%02x port=0x%02x mode=0x%02x baudrate=%d",5,check_byte,cpu_level,syslog_port_num,syslog_mode,g_syslog_baudrate);
>>>>>>> db20e11 (second commit)
}

bool log_port_init(void)
{
#ifdef MTK_DEBUG_PLAIN_LOG_ENABLE
    mux_port_t      port;
    mux_status_t    status;
    mux_port_setting_t  setting;
    mux_protocol_t      protocol_callback;
    mux_port_buffer_t   query_port_buffer;

    g_syslog_race_user_list[0].port_name = "SYSLOG";
    g_syslog_race_user_list[0].port = CONFIG_SYSLOG_RUNNING_STAGE_PORT;
    g_syslog_race_user_list[0].setting.tx_buffer_size = 8*1024;
    g_syslog_race_user_list[0].setting.rx_buffer_size = 1024;
    g_syslog_race_user_list[0].setting.dev_setting.uart.uart_config.baudrate    = CONFIG_SYSLOG_BAUDRATE;
    g_syslog_race_user_list[0].setting.dev_setting.uart.uart_config.word_length = HAL_UART_WORD_LENGTH_8;
    g_syslog_race_user_list[0].setting.dev_setting.uart.uart_config.stop_bit    = HAL_UART_STOP_BIT_1;
    g_syslog_race_user_list[0].setting.dev_setting.uart.uart_config.parity      = HAL_UART_PARITY_NONE;
    g_syslog_race_user_list[0].setting.dev_setting.uart.flowcontrol_type        = MUX_UART_SW_FLOWCONTROL;
    g_syslog_race_user_list[0].handle = &syslog_mux_handle;
    g_syslog_race_user_list[0].race_id_begin = 0x0F10;
    g_syslog_race_user_list[0].race_id_end   = 0x0F20;
    g_syslog_race_user_list[0].drop_race_header = FALSE;

#ifdef MTK_NVDM_ENABLE
    port = CONFIG_SYSLOG_RUNNING_STAGE_PORT;
    mux_nvdm_init(port);
#endif
    port = g_syslog_race_user_list[0].port;
    query_port_buffer.count = 0;
    mux_query_user_port_numbers_from_nvdm("SYSLOG", (uint32_t*)&(query_port_buffer.count));

    if (query_port_buffer.count == 1) {
        //should be 1 as log only use one port
        status = mux_query_port_numbers_from_nvdm("SYSLOG",&query_port_buffer);
        if (status != MUX_STATUS_OK) {
            assert(0);    //debug
        }
        port = query_port_buffer.buf[0];
        status = mux_query_port_setting_from_nvdm(port, &setting);
        if(MUX_STATUS_OK == status) {
            memcpy(&g_syslog_race_user_list[0].setting,&setting,sizeof(setting));
            g_syslog_race_user_list[0].port= port;
        }
    }

    protocol_callback.rx_protocol_callback = race_rx_protocol_callback;
    protocol_callback.tx_protocol_callback = race_tx_protocol_callback;
    if (port == CONFIG_SYSLOG_RUNNING_STAGE_PORT) {
        hal_uart_deinit(port - MUX_UART_BEGIN + HAL_UART_0);
    }
    if (MUX_STATUS_OK != mux_init(port, &g_syslog_race_user_list[0].setting, &protocol_callback)) {
        assert(0);
    }
    g_syslog_port = g_syslog_race_user_list[0].port;

#else
    /* debug port */
    syslog_debug_uart_init();

    /* load config from nvkey */
    log_from_nvkey_init();

    /* syslog port init by race xport */
    extern void HCI_XPORT_Init(mux_port_t port, hal_uart_baudrate_t baudrate);
    HCI_XPORT_Init(g_syslog_port, g_syslog_baudrate);

#endif

    return true;
}

static void port_syslog_post_init(void)
{
#ifdef MTK_DEBUG_PLAIN_LOG_ENABLE
    uint32_t port_num;
    mux_status_t status;

    status = mux_query_user_port_numbers_from_nvdm("SYSLOG", &port_num);
    if (port_num > 1) {
        assert(0);    //log only regist on a port
    }
    port_num = g_syslog_port;
    mux_open(port_num, "SYSLOG", (mux_handle_t*)&g_syslog_share_variable->handle,pc_tool_command_handler,NULL);
    syslog_mux_handle = g_syslog_share_variable->handle;
    syslog_exception_handle  = g_syslog_share_variable->handle;
#else
    mux_status_t status;
    status = mux_open(g_syslog_port, "SYSLOG", (mux_handle_t*)&g_syslog_share_variable->handle,NULL,NULL);
    syslog_mux_handle = g_syslog_share_variable->handle;
    syslog_exception_handle = 0x55AAFF00 | g_syslog_port_index;
#endif
}

#endif /* MTK_CPU_NUMBER_0 */

<<<<<<< HEAD
=======
static bool print_drop_message(void)
{
    uint32_t tx_drop_size = 0, tx_free_size = 0;
    uint8_t  drop_package_buffer[MAX_DROP_BUFFER_SIZE];
    mux_buffer_t drop_buf;
    tx_drop_size = port_syslog_drop_prompt_length(g_syslog_share_variable->drop_count);
    mux_control(g_syslog_port, MUX_CMD_GET_TX_AVAIL, (mux_ctrl_para_t*)&tx_free_size);
    if (tx_free_size >= tx_drop_size) {
        memset(drop_package_buffer, 0, sizeof(drop_package_buffer));
        port_syslog_drop_prompt(port_syslog_get_current_timestamp(), tx_drop_size, g_syslog_share_variable->drop_count, (uint8_t*)drop_package_buffer);
        drop_buf.p_buf = drop_package_buffer;
        drop_buf.buf_size = tx_drop_size;
        mux_tx(g_syslog_share_variable->handle, &drop_buf, 1, &tx_drop_size);
        g_syslog_share_variable->drop_count = 0;
    } else {
        g_syslog_share_variable->drop_count += 1;
        return false;
    }

    return true;
}

>>>>>>> db20e11 (second commit)
static uint32_t port_syslog_send(uint32_t id, bool drop_flag, const uint8_t **p_data, uint32_t *p_len)
{
    mux_status_t status;
    uint32_t i, total_size;
    uint32_t counter;
    mux_buffer_t mux_buffers[6];
    mux_buffer_t *p_mux_buffer;
<<<<<<< HEAD
#ifndef MTK_DEBUG_PLAIN_LOG_ENABLE
    uint8_t race_header[6];
#endif
=======

#ifndef MTK_DEBUG_PLAIN_LOG_ENABLE
    uint8_t race_header[6];
#endif

>>>>>>> db20e11 (second commit)
    total_size = 0;
    counter = 0;
    for (i = 0; p_data[i] != NULL; i++) {
        total_size += p_len[i];
        mux_buffers[i+1].p_buf = (uint8_t*)p_data[i];
        mux_buffers[i+1].buf_size = p_len[i];
        counter++;
    }

    mux_buffers[i+1].p_buf = NULL;

#ifndef MTK_DEBUG_PLAIN_LOG_ENABLE
    /* Insert the race header here */
    race_header[0] = 0x05;
    race_header[1] = RACE_TYPE_NOTIFICATION;
    race_header[2] = (uint8_t)((total_size + 2) & 0xFF);
    race_header[3] = (uint8_t)(((total_size + 2) >> 8) & 0xFF);
    race_header[4] = (uint8_t)(id & 0xFF);
    race_header[5] = (uint8_t)((id >> 8) & 0xFF);

    mux_buffers[0].p_buf = race_header;
    mux_buffers[0].buf_size = 6;

    p_mux_buffer = mux_buffers;
    counter += 1;
#else
    PORT_SYSLOG_UNUSED(id);
    p_mux_buffer = &mux_buffers[1];
#endif

#ifdef MTK_CPU_NUMBER_0
    if (g_syslog_share_variable->init_phase == SYSLOG_EARLY_INIT) {
#ifndef MTK_DEBUG_PLAIN_LOG_ENABLE
        uart_send_polling(CONFIG_SYSLOG_INIT_STAGE_PORT, race_header, 6);
#endif
        /* send the payload */
        for (i = 0; p_data[i] != NULL; i++) {
            uart_send_polling(CONFIG_SYSLOG_INIT_STAGE_PORT, p_data[i], p_len[i]);
        }
        return total_size;
    } else if (g_syslog_share_variable->init_phase == SYSLOG_EXCEPTION_INIT) {
        mux_exception_send(syslog_exception_handle, p_mux_buffer, counter);
        return total_size;
    }
#endif

<<<<<<< HEAD
=======
    /* print flush drop count*/
    if (g_syslog_share_variable->drop_count != 0) {
        if (print_drop_message() == false) {
            return 0;
        }
    }

>>>>>>> db20e11 (second commit)
    do {
        status = mux_tx(g_syslog_share_variable->handle, p_mux_buffer, counter, &total_size);
    } while ((drop_flag == false) && (total_size == 0));

    if(status == MUX_STATUS_OK) {
        total_size = total_size - mux_buffers[0].buf_size;
    } else {
<<<<<<< HEAD
=======
        g_syslog_share_variable->drop_count += 1;
>>>>>>> db20e11 (second commit)
        total_size = 0;
    }

    return total_size;
}

<<<<<<< HEAD
extern bool kernel_task_send_offline_log_dump_msg(void);

=======
>>>>>>> db20e11 (second commit)
void log_buffer_lock(void)
{
    g_syslog_share_variable->lock_buffer = true;
}

void log_buffer_unlock(void)
{
    g_syslog_share_variable->lock_buffer = false;
}

bool query_log_buffer_lock_status(void)
{
<<<<<<< HEAD
    if (g_syslog_share_variable->lock_buffer == true) {
        return true;
    }
    return false;
=======
    return g_syslog_share_variable->lock_buffer;
}

void log_save_flash(void)
{
    uint32_t cpu_irq_mask;

    if (query_log_buffer_lock_status() == false) {
        return;
    }

    offline_log_dump_handler();

    /* unlock buffer */
    port_syslog_local_cpu_enter_critical(&cpu_irq_mask);
    log_buffer_unlock();
    port_syslog_local_cpu_exit_critical(cpu_irq_mask);

    SYSLOG_MSGID_W(">>> log flush drop %d", 1, g_syslog_share_variable->drop_flush_count);
    g_syslog_share_variable->drop_flush_count = 0;

    offline_dump_callback_tail_handle(OFFLINE_REGION_OFFLINE_LOG);
>>>>>>> db20e11 (second commit)
}

void log_buffer_flush(void)
{
<<<<<<< HEAD
    uint32_t cpu_irq_mask;
=======
#ifdef MTK_OFFLINELOG_ENABLE
    uint32_t cpu_irq_mask;
    /* nvkey feature not enable */
    if (g_offline_log_is_enable == false) {
        SYSLOG_MSGID_E("SYSLOG OFFLINE LOG NVKEY NOT EANBLE!!!", 0);
        return ;
    }
    offline_dump_callback_head_handle(OFFLINE_REGION_OFFLINE_LOG);
>>>>>>> db20e11 (second commit)
    port_syslog_local_cpu_enter_critical(&cpu_irq_mask);
    if (query_log_buffer_lock_status() == true) {
        port_syslog_local_cpu_exit_critical(cpu_irq_mask);
        return;
    }
    log_buffer_lock();
    port_syslog_local_cpu_exit_critical(cpu_irq_mask);
<<<<<<< HEAD
    if (kernel_task_send_offline_log_dump_msg() == false) {
        log_buffer_unlock();
    }
=======
#else
    return ;
#endif
>>>>>>> db20e11 (second commit)
}

#define MAX_URGENT_LOG_LENGTH                   256
#define MAX_ACTIVE_LOG_CMD_LENGTH               64

//RACE ID + MODULE NUMBER/2
#define MAX_PC_RX_DATA_SIZE                     256

typedef struct {
    uint8_t cpu_id;
    uint8_t module_number;
} module_filter_info_t;

#define SYSLOG_FILTER_VALID_MARK 0x80

NO_INLINE volatile uint8_t *find_start_of_cpu_log_filters(uint32_t cpu_id)
{
    uint32_t offset, i;
    module_filter_info_t *p_filter_info;

    offset = 2 * MTK_MAX_CPU_NUMBER;
    p_filter_info = (module_filter_info_t *)(g_syslog_share_variable->cpu_module_filter_status);
    for (i = 0; i < MTK_MAX_CPU_NUMBER; i++) {
        if ((p_filter_info[i].cpu_id & SYSLOG_FILTER_VALID_MARK) == 0) {
            break;
        }
        if ((p_filter_info[i].cpu_id & (~SYSLOG_FILTER_VALID_MARK)) == cpu_id) {
            break;
        }
        offset += p_filter_info[i].module_number;
    }

    return &(g_syslog_share_variable->cpu_module_filter_status[offset]);
}

NO_INLINE volatile void *qurey_cpu_module_filter_pointer(uint32_t cpu_id)
{
    return (log_control_block_t *)(g_syslog_share_variable->cpu_module_filter[cpu_id]);
}

NO_INLINE bool log_set_all_module_filter_off(log_switch_t log_switch)
{
    volatile uint8_t *p_filters_status;
    log_control_block_t *filters;
    module_filter_info_t *p_filter_info;
    uint32_t module_num, i, cpu_id;

    //only handle CM4
    if (log_switch > DEBUG_LOG_OFF) {
        return false;
    }

    cpu_id = GET_CURRENT_CPU_ID();

    for (cpu_id = 0; cpu_id < MTK_MAX_CPU_NUMBER; cpu_id++) {
        filters = (log_control_block_t *)g_syslog_share_variable->cpu_module_filter[cpu_id];
        if (filters == NULL) {
            return false;
        }
        p_filter_info = (module_filter_info_t *)g_syslog_share_variable->cpu_module_filter_status;
        if ((p_filter_info[cpu_id].cpu_id & SYSLOG_FILTER_VALID_MARK) == 0) {
            return false;
        }

        module_num = p_filter_info[cpu_id].module_number;
        p_filters_status = find_start_of_cpu_log_filters(cpu_id);
        for (i=0; i<module_num;i++) {
            p_filters_status[i] = ((log_switch << 4) | (p_filters_status[i]  & 0x0F));
        }
    }

    return true;
}
#ifdef MTK_CPU_NUMBER_0

#include "exception_handler.h"

#define CMD_CPU_NAME_MAX_LENGTRH        16
#define CMD_SDK_VERSION_MAX_LENGTRH     48
#define CMD_BUILD_TIME_MAX_LENGTRH      48

#define PC_COMMAND_ERROR_CODE_OK        0x00
#define PC_COMMAND_ERROR_CODE_FAIL      0x01

static volatile uint32_t g_curr_filter_save_type;
static volatile bool g_filter_save_is_need = false;

uint32_t port_syslog_receive(uint8_t *p_buf, uint32_t size)
{
    mux_buffer_t buffer;
    uint32_t len;
    buffer.p_buf = p_buf;
    buffer.buf_size = size;
    mux_rx(g_syslog_share_variable->handle, &buffer,&len);
    return len;
}

static void filter_config_load(void)
{
    uint32_t i, size;
    char *syslog_filter_buf;

    syslog_filter_buf = (char *)port_syslog_malloc(PORT_SYSLOG_MODULE_FILTER_STATUS_SIZE);
    assert(syslog_filter_buf != NULL);

    /* this part need each CPU to parse itself */
    size = PORT_SYSLOG_MODULE_FILTER_STATUS_SIZE;
    if (port_syslog_read_setting("module_filter", (uint8_t *)syslog_filter_buf, size) == true) {
        memcpy((uint8_t *)g_syslog_share_variable->cpu_module_filter_status, syslog_filter_buf, size);
    }

    size = 2 * MTK_MAX_CPU_NUMBER;
    if (port_syslog_read_setting("cpu_filter", (uint8_t *)syslog_filter_buf, size) == true) {
        for (i = 0; i < MTK_MAX_CPU_NUMBER; i++) {
            g_syslog_share_variable->cpu_log_switch[i] = syslog_filter_buf[i * 2];
            g_syslog_share_variable->cpu_log_print_level[i] = syslog_filter_buf[i * 2 + 1];
        }
    } else {
        for (i = 0; i < MTK_MAX_CPU_NUMBER; i++) {
            g_syslog_share_variable->cpu_log_switch[i] = DEBUG_LOG_ON;
            g_syslog_share_variable->cpu_log_print_level[i] = PRINT_LEVEL_DEBUG;
        }
    }

    port_syslog_free(syslog_filter_buf);
}

static void filter_cpu_config_save(void)
{
    uint32_t i;
    char syslog_filter_buf[2 * MTK_MAX_CPU_NUMBER];

    for (i = 0; i < MTK_MAX_CPU_NUMBER; i++) {
        syslog_filter_buf[i * 2] = g_syslog_share_variable->cpu_log_switch[i];
        syslog_filter_buf[i * 2 + 1] = g_syslog_share_variable->cpu_log_print_level[i];
    }

    port_syslog_save_setting("cpu_filter", (uint8_t *)syslog_filter_buf, sizeof(syslog_filter_buf));
}

static void filter_module_config_save(void)
{
    port_syslog_save_setting("module_filter", (uint8_t *)g_syslog_share_variable->cpu_module_filter_status, PORT_SYSLOG_MODULE_FILTER_STATUS_SIZE);
}

/* Below APIs is used also by AT command */
bool log_get_cpu_filter_number(uint32_t *p_cpu_number)
{
    if (p_cpu_number == NULL) {
        return false;
    }

    *p_cpu_number = MTK_MAX_CPU_NUMBER;

    return true;
}

bool log_get_cpu_filter_config(uint32_t cpu_id, log_switch_t *p_log_switch, print_level_t *p_print_level)
{
    if ((cpu_id >= MTK_MAX_CPU_NUMBER) || (p_log_switch == NULL) || (p_print_level == NULL)) {
        return false;
    }

    *p_log_switch  = g_syslog_share_variable->cpu_log_switch[cpu_id];
    *p_print_level = g_syslog_share_variable->cpu_log_print_level[cpu_id];

    return true;
}

bool log_get_module_filter_number(uint32_t cpu_id, uint32_t *p_module_number)
{
    log_control_block_t *filters;
    module_filter_info_t *p_filter_info;

    if ((cpu_id >= MTK_MAX_CPU_NUMBER) || (p_module_number == NULL)) {
        return false;
    }

    /* This filter has been remap already in log_set_filter() */
    filters = (log_control_block_t *)(g_syslog_share_variable->cpu_module_filter[cpu_id]);
    if (filters == NULL) {
        return false;
    }
    p_filter_info = (module_filter_info_t *)g_syslog_share_variable->cpu_module_filter_status;
    if ((p_filter_info[cpu_id].cpu_id & SYSLOG_FILTER_VALID_MARK) == 0) {
        return false;
    }
    *p_module_number = p_filter_info[cpu_id].module_number;

    return true;
}

bool log_get_module_filter_config(uint32_t cpu_id, uint32_t module_id, const char **p_module_name, log_switch_t *p_log_switch, print_level_t *p_print_level)
{
    volatile uint8_t *p_filters_status;
    log_control_block_t *filters;
    module_filter_info_t *p_filter_info;

    if ((cpu_id >= MTK_MAX_CPU_NUMBER) || (p_log_switch == NULL) || (p_print_level == NULL)) {
        return false;
    }

    p_filters_status = find_start_of_cpu_log_filters(cpu_id);
    filters = (log_control_block_t *)(g_syslog_share_variable->cpu_module_filter[cpu_id]);
    if (filters == NULL) {
        return false;
    }
    p_filter_info = (module_filter_info_t *)g_syslog_share_variable->cpu_module_filter_status;
    if ((p_filter_info[cpu_id].cpu_id & SYSLOG_FILTER_VALID_MARK) == 0) {
        return false;
    }

    if (module_id >= p_filter_info[cpu_id].module_number) {
        return false;
    }

    if (p_module_name != NULL) {
        *p_module_name = filters[module_id].module_name;
    }
    *p_log_switch = p_filters_status[module_id] >> 4;
    *p_print_level = p_filters_status[module_id] & 0x0F;

    return true;
}

bool log_set_cpu_filter_config(uint32_t cpu_id, log_switch_t log_switch, print_level_t print_level)
{
    if ((cpu_id >= MTK_MAX_CPU_NUMBER) ||
            (log_switch > DEBUG_LOG_OFF) || (print_level > PRINT_LEVEL_ERROR)) {
        return false;
    }

    g_syslog_share_variable->cpu_log_switch[cpu_id] = log_switch;
    g_syslog_share_variable->cpu_log_print_level[cpu_id] = print_level;

    return true;
}

bool log_set_module_filter_config(uint32_t cpu_id, char *module_name, log_switch_t log_switch, print_level_t print_level)
{
    uint32_t module_id, size_1, size_2;
    volatile uint8_t *p_filters_status;
    log_control_block_t *filters;
    module_filter_info_t *p_filter_info;

    if ((cpu_id >= MTK_MAX_CPU_NUMBER) ||
            (module_name == NULL) ||
            (log_switch > DEBUG_LOG_OFF) || (print_level > PRINT_LEVEL_ERROR)) {
        return false;
    }

    filters = (log_control_block_t *)g_syslog_share_variable->cpu_module_filter[cpu_id];
    if (filters == NULL) {
        return false;
    }
    p_filter_info = (module_filter_info_t *)g_syslog_share_variable->cpu_module_filter_status;
    if ((p_filter_info[cpu_id].cpu_id & SYSLOG_FILTER_VALID_MARK) == 0) {
        return false;
    }

    size_1 = strlen(module_name);
    for (module_id = 0; module_id < p_filter_info[cpu_id].module_number; module_id++) {
        size_2 = strlen(filters[module_id].module_name);
        if (size_1 != size_2) {
            continue;
        }
        if (!strcmp(filters[module_id].module_name, module_name)) {
            break;
        }
    }
    if (module_id >= p_filter_info[cpu_id].module_number) {
        return false;
    }

    p_filters_status = find_start_of_cpu_log_filters(cpu_id);
    p_filters_status[module_id] = (log_switch << 4) | print_level;

    return true;
}

void log_trigger_save_filter(void)
{
    g_curr_filter_save_type = 0xFF;
    g_filter_save_is_need = true;
}

bool log_query_save_filter(void)
{
    return !g_filter_save_is_need;
}

/* Below API should be called by idle or deamon task */
/* For ab1562 tool command handle by race service, save flash action can do in idle hook */
void log_save_filter(void)
{
    if (g_filter_save_is_need == false) {
        return;
    }

    /* Save log filter setting to NVDM */
    if (g_curr_filter_save_type & 0x0F) {
        filter_cpu_config_save();
    }
    if (g_curr_filter_save_type & 0xF0) {
        filter_module_config_save();
    }

    /* Reset the save flag. */
    g_filter_save_is_need = false;
}

<<<<<<< HEAD
=======
void pc_tool_assert(uint8_t *data, uint32_t length)
{
    typedef struct {
        uint8_t pktId;          /*!< Packet type, usually is 0x5*/
        uint8_t type;           /*!< Type field*/
        uint16_t length;        /*!< Type field*/
        uint16_t id;            /*!< ID field*/
    } __attribute__((packed)) *THIS_PC_CMD_STRU;

    THIS_PC_CMD_STRU pCmd = (THIS_PC_CMD_STRU)data;

    //05 5A 02 00 14 0F
    if (length < 6) {
        return ;
    }

    if ((pCmd->pktId == 0x05) && (pCmd->type == 0x5C) && (pCmd->length == 0x0002) &&(pCmd->id == 0x0F14)) {
        ATTR_LOG_STRING exp[] = "Asserted by PC logging tool"; \
        ATTR_LOG_STRING file[] = __FILE__; \
        light_assert(exp, file, __LINE__);
    }
}

>>>>>>> db20e11 (second commit)
void pc_tool_command_handler(mux_handle_t handle, mux_event_t event,uint32_t data_len,void*user_data)
{
    uint32_t i, filter_number;
    uint16_t race_id;
    uint8_t cpu_id, filter_info, error_code;
    uint8_t *p_data[2];
    uint8_t *pc_command_response_buf;
    int32_t pc_command_response_len;
    uint8_t *pc_received_data_buf;
    uint32_t pc_received_data_len;
    PORT_SYSLOG_UNUSED(handle);
    PORT_SYSLOG_UNUSED(user_data);
    log_switch_t log_switch;
    print_level_t print_level;
    log_control_block_t *filters;

    if (event == MUX_EVENT_READY_TO_WRITE) {
        return;
    }

    // only call port_syslog_receive one time, and parse data
    //pc_received_data_buf = port_syslog_malloc(MAX_PC_RX_DATA_SIZE);
    pc_received_data_buf = port_syslog_malloc(data_len);
    if (pc_received_data_buf == NULL) {
        assert(0);
    }
    memset(pc_received_data_buf, 0, data_len);

    pc_received_data_len = data_len;
    //read back all data
    if (port_syslog_receive(pc_received_data_buf, (uint32_t)data_len) != pc_received_data_len) {
        return;
    }

    pc_command_response_buf = port_syslog_malloc(MAX_URGENT_LOG_LENGTH);
    if (pc_command_response_buf == NULL) {
        assert(0);
    }
    memset(pc_command_response_buf, 0, MAX_URGENT_LOG_LENGTH);
    pc_command_response_len = 0;

    race_id = (pc_received_data_buf[1] << 8) | pc_received_data_buf[0];
    switch (race_id) {
        case RACE_PROTOCAL_ACTIVE_ASSERT:
            platform_assert("Asserted by PC logging tool", __FILE__, __LINE__);
            break;
        case RACE_PROTOCAL_QUERY_VERSION_BUILDTIME:
            cpu_id = pc_received_data_buf[2];
            if (cpu_id >= MTK_MAX_CPU_NUMBER) {
                goto error;
            }
            pc_command_response_buf[0] = 0;
            strncpy((char *)&pc_command_response_buf[1 + CMD_CPU_NAME_MAX_LENGTRH], (char *)(PORT_SYSLOG_SDK_VERSION_BEGIN + cpu_id * PORT_SYSLOG_SDK_VERSION_LENGTH), PORT_SYSLOG_SDK_VERSION_LENGTH);
            strncpy((char *)&pc_command_response_buf[1 + CMD_CPU_NAME_MAX_LENGTRH + CMD_SDK_VERSION_MAX_LENGTRH], (char *)(PORT_SYSLOG_BUILD_TIME_BEGIN + cpu_id * PORT_SYSLOG_BUILD_TIME_LENGTH), PORT_SYSLOG_BUILD_TIME_LENGTH);
            pc_command_response_len = 1 + 16 + 48 + 48;
            break;
        case RACE_PROTOCAL_GET_LOG_FILTER_INFO:
            cpu_id = pc_received_data_buf[2];
            if (cpu_id >= MTK_MAX_CPU_NUMBER) {
                goto error;
            }
            pc_command_response_buf[0] = cpu_id;
            log_get_module_filter_number(cpu_id, &filter_number);
            if (filter_number == 0) {
                goto error;
            }
            for (i = 0; i < filter_number / 2; i++) {
                log_get_module_filter_config(cpu_id, i * 2, NULL, &log_switch, &print_level);
                pc_command_response_buf[i + 1] = (log_switch << 3) | print_level;
                log_get_module_filter_config(cpu_id, i * 2 + 1, NULL, &log_switch, &print_level);
                pc_command_response_buf[i + 1] |= ((log_switch << 3) | print_level) << 4;
            }
            if (filter_number % 2) {
                log_get_module_filter_config(cpu_id, i * 2, NULL, &log_switch, &print_level);
                pc_command_response_buf[i + 1] = (log_switch << 3) | print_level;
            }
            pc_command_response_len = 1 + filter_number / 2 + filter_number % 2;
            break;
        case RACE_PROTOCAL_SET_LOG_FILTER_INFO:
            cpu_id = pc_received_data_buf[2];
            if (cpu_id >= MTK_MAX_CPU_NUMBER) {
                goto error;
            }
            error_code = PC_COMMAND_ERROR_CODE_OK;
            log_get_module_filter_number(cpu_id, &filter_number);
            if (filter_number == 0) {
                goto error;
            }
            filters = (log_control_block_t *)(g_syslog_share_variable->cpu_module_filter[cpu_id]);
            for (i = 0; i < filter_number/2 ; i++) {
                //if (port_syslog_receive(&filter_info, 1) != 1) {
                //    error_code = PC_COMMAND_ERROR_CODE_FAIL;
                //    break;
                //}
                filter_info = pc_received_data_buf[3+i];
                log_switch = (filter_info >> 3) & 0x01;
                print_level = filter_info & 0x03;
                log_set_module_filter_config(cpu_id, (char *)(filters[i * 2].module_name), log_switch, print_level);
                log_switch = (filter_info >> 7) & 0x01;
                print_level = (filter_info >> 4) & 0x03;
                log_set_module_filter_config(cpu_id, (char *)(filters[i * 2 + 1].module_name), log_switch, print_level);
            }
            if (filter_number % 2) {
                //if (port_syslog_receive(&filter_info, 1) != 1) {
                //    error_code = PC_COMMAND_ERROR_CODE_FAIL;
                //    break;
                //}
                filter_info = pc_received_data_buf[3+filter_number/2];
                log_switch = (filter_info >> 3) & 0x01;
                print_level = filter_info & 0x03;
                log_set_module_filter_config(cpu_id, (char *)(filters[i * 2].module_name), log_switch, print_level);
            }
            pc_command_response_buf[0] = error_code;
            pc_command_response_len = 1;
            break;
        case RACE_PROTOCAL_SAVE_LOG_SETTING:
            log_trigger_save_filter();
            goto error;
        default:
<<<<<<< HEAD
=======
            goto error;
>>>>>>> db20e11 (second commit)
            break;
    }

    p_data[0] = pc_command_response_buf;
    p_data[1] = NULL;
    port_syslog_send(race_id, false, (const uint8_t **)p_data, (uint32_t *)&pc_command_response_len);

error:
    port_syslog_free(pc_command_response_buf);
}


static volatile uint32_t g_exception_string_log_count = 0;
static volatile uint32_t g_exception_binary_log_count = 0;
void exception_syslog_callback(void)
{
<<<<<<< HEAD
    /* Reset status of syslog to make other CPU's log to be bypass. */
    g_syslog_share_variable->init_phase = SYSLOG_EXCEPTION_INIT;
=======
    /* port handle init before log full init done */
    if ((g_syslog_share_variable->init_phase == SYSLOG_EARLY_INIT)
            || (g_syslog_share_variable->init_phase == SYSLOG_INIT_NONE)
            || (g_primary_cpu_syslog_is_initialized == false)) {
        syslog_exception_handle = 0x55AAFF00 | CONFIG_SYSLOG_INIT_STAGE_PORT;
    }

>>>>>>> db20e11 (second commit)
    g_exception_string_log_count = 0;
    g_exception_binary_log_count = 0;

    mux_exception_init(syslog_exception_handle);

<<<<<<< HEAD
=======
    /* Reset status of syslog to make other CPU's log to be bypass. */
    g_syslog_share_variable->init_phase = SYSLOG_EXCEPTION_INIT;

>>>>>>> db20e11 (second commit)
    g_exception_is_happen = true;
}

/* This API will bypass all log APIs including exception APIs. */
void log_global_turn_off(void)
{
    /* Reset status of syslog to make other CPU's log to be bypass. */
    g_syslog_share_variable->init_phase = SYSLOG_INIT_NONE;
}

int log_print_exception_log(const char *message, ...)
{
    uint8_t *p_data[3];
    uint32_t p_len[2];
    va_list list;
    int32_t log_size;
    char frame_header[PORT_SYSLOG_MAX_ONE_PLAIN_LOG_SIZE];

    if (g_exception_is_happen == false) {
        return 0;
    }
    va_start(list, message);

    log_size = vsnprintf(frame_header, sizeof(frame_header), message, list);
    if (log_size < 0) {
        va_end(list);
        return 0;
    }
    if ((uint32_t)log_size >= sizeof(frame_header)) {
        log_size = sizeof(frame_header) - 1;
    }

    va_end(list);

#ifndef MTK_DEBUG_PLAIN_LOG_ENABLE
    p_data[0] = (uint8_t *)&g_exception_string_log_count;
    p_len[0] = 4;
    p_data[1] = (uint8_t *)frame_header;
    p_len[1] = log_size;
    p_data[2] = NULL;
#else
    p_data[0] = (uint8_t *)frame_header;
    p_len[0] = log_size;
    p_data[1] = NULL;
#endif
    port_syslog_send(RACE_PROTOCAL_EXCEPTION_STRING_LOG, false, (const uint8_t **)p_data, p_len);

    g_exception_string_log_count++;

    return log_size;
}

#define MAX_EXCEPTION_DATA_LENGTH 1024

void log_dump_exception_data(const uint8_t *data, uint32_t size)
{
    uint8_t *p_data[3];
    uint32_t p_len[2];
    uint8_t *curr_data;
    uint32_t curr_size;

    if (g_exception_is_happen == false) {
        return;
    }

    curr_data = (uint8_t *)data;
    while (size > 0) {
        if (size >= MAX_EXCEPTION_DATA_LENGTH) {
            curr_size = MAX_EXCEPTION_DATA_LENGTH;
            size -= MAX_EXCEPTION_DATA_LENGTH;
        } else {
            curr_size = size;
            size = 0;
        }
#ifndef MTK_DEBUG_PLAIN_LOG_ENABLE
        p_data[0] = (uint8_t *)&g_exception_binary_log_count;
        p_len[0] = 4;
        p_data[1] = curr_data;
        p_len[1] = curr_size;
        p_data[2] = NULL;
#else
        p_data[0] = curr_data;
        p_len[0] = curr_size;
        p_data[1] = NULL;
#endif
        port_syslog_send(RACE_PROTOCAL_EXCEPTION_BINARY_LOG, false, (const uint8_t **)p_data, p_len);
        curr_data += curr_size;
        g_exception_binary_log_count++;
    }
}


#define MAX_EXCEPTION_MSG_ID_PARAMETER_NUMBER 20
#define MAX_EXCEPTION_TLV_DUMP_DATA_SIZE      2048
#define MAX_EXCEPTION_USER_DUMP_BUFFER_COUNT  5

NO_INLINE static int32_t exception_binary_log_handler(log_type_t type,
                                                    uint8_t cpu_id,
                                                    const char *message,
                                                    const void **p_data,
                                                    uint32_t *p_length)
{
    uint8_t *p_tx_data[MAX_EXCEPTION_USER_DUMP_BUFFER_COUNT + 3];
    uint32_t p_tx_len[MAX_EXCEPTION_USER_DUMP_BUFFER_COUNT + 2];
    uint8_t tlv_header[12];
    uint32_t i, payload_size, buffer_count, offset;
    uint32_t res_len;

    /* Calculate total size of payload */
    payload_size = 0;
    buffer_count = 0;
    for (i = 0; p_data[i] != NULL; i++) {
        buffer_count++;
        if (buffer_count > MAX_EXCEPTION_USER_DUMP_BUFFER_COUNT) {
            return 0;
        }
        p_tx_data[i + 1] = (uint8_t *)p_data[i];
        p_tx_len[i + 1] = p_length[i];
        payload_size += p_length[i];
    }
    p_tx_data[i + 1] = NULL;

    /* Limit the max size of dump data when calling LOG_TLVDUMP_*() */
    if (g_syslog_share_variable->init_phase == SYSLOG_FULL_INIT) {
        if (payload_size > MAX_EXCEPTION_TLV_DUMP_DATA_SIZE) {
            payload_size = MAX_EXCEPTION_TLV_DUMP_DATA_SIZE;
        }
    }

    /* Initialize the TLV header */
    tlv_header[0] = cpu_id;
    tlv_header[1] = 0;
    tlv_header[2] = 0;
    tlv_header[3] = 0;
    tlv_header[4] = 0;
    tlv_header[5] = 0;
    offset = (uint32_t)message - PORT_SYSLOG_MSG_ADDR_OFFSET;
    tlv_header[6] = offset & 0xff;
    tlv_header[7] = (offset >> 8) & 0xff;
    tlv_header[8] = (offset >> 16) & 0xff;
    tlv_header[9] = (offset >> 24) & 0xff;
    /* syslog header */
    p_tx_data[0] = (uint8_t *)tlv_header;
    p_tx_len[0]  = 10;
    res_len = port_syslog_send(RACE_PROTOCAL_TLV_LOG|type, true, (const uint8_t **)p_tx_data, p_tx_len);

    return res_len;
}

void log_print_exception_msgid_log(uint8_t cpu_id, const char *message,uint32_t arg_cnt, ...)
{
    va_list list;
    uint32_t i, buffer[MAX_EXCEPTION_MSG_ID_PARAMETER_NUMBER];
    void *p_data[2];
    uint32_t p_length[2];

    va_start(list, arg_cnt);

    if (arg_cnt > MAX_EXCEPTION_MSG_ID_PARAMETER_NUMBER) {
        return;
    }

    for (i = 0; i < arg_cnt; i++) {
        buffer[i] = va_arg(list, uint32_t);
    }

    p_data[0]   = buffer;
    p_length[0] = arg_cnt * sizeof(uint32_t);
    p_data[1]   = NULL;
    p_length[1] = 0;

    exception_binary_log_handler(LOG_TYPE_EXCEPTION_MSGID_LOG, cpu_id, message, (const void **)p_data, p_length);

    va_end(list);
}

<<<<<<< HEAD
static volatile bool g_primary_cpu_syslog_is_initialized = false;
=======
>>>>>>> db20e11 (second commit)

bool log_uart_init(hal_uart_port_t port, hal_uart_baudrate_t baudrate)
{
    exception_config_type exception_config;

    PORT_SYSLOG_UNUSED(port);
    PORT_SYSLOG_UNUSED(baudrate);

    if (g_primary_cpu_syslog_is_initialized == true) {
        return false;
    }

    /* Initialize the global share variables */
    memset((syslog_share_variable_t *)g_syslog_share_variable, 0, sizeof(syslog_share_variable_t));

    port_syslog_early_init();

    g_syslog_share_variable->init_phase = SYSLOG_EARLY_INIT;

    exception_config.init_cb = NULL;
    exception_config.dump_cb = NULL;
    exception_register_callbacks(&exception_config);
    g_exception_is_happen = false;

    g_primary_cpu_syslog_is_initialized = true;

    return true;
}


bool log_init(syslog_port_type_t port_type, uint8_t port_index)
{
    PORT_SYSLOG_UNUSED(port_type);
    PORT_SYSLOG_UNUSED(port_index);
    /* must do log uart init first */
    if ((g_primary_cpu_syslog_is_initialized == false) ||(g_syslog_share_variable->init_phase != SYSLOG_EARLY_INIT)) {
        return false;
    }

    //hal_uart_deinit(CONFIG_SYSLOG_INIT_STAGE_PORT);

    /* load logging filters setting, from nvdm */
    filter_config_load();

    /* logging device post initialization */
    port_syslog_post_init();

    /* set cpu filter if online log */
    log_set_cpu_filter_config(0,DEBUG_LOG_ON,g_cpu_level[1]);

    /* syslog all init done */
    g_syslog_share_variable->init_phase = SYSLOG_FULL_INIT;

    return true;
}

#endif /* MTK_CPU_NUMBER_0 */

#if !defined(MTK_DEBUG_LEVEL_NONE)

bool log_set_filter(void)
{
    uint32_t i, cpu_id, offset;
    uint32_t runtime_filter_number;
    module_filter_info_t *p_filter_info;
    volatile uint8_t *p_filters;
    log_control_block_t *entries;

    if (g_syslog_share_variable->init_phase != SYSLOG_FULL_INIT) {
        return false;
    }

    cpu_id = GET_CURRENT_CPU_ID();

    port_syslog_build_time_sdk_version_copy(cpu_id);

    /* Calculate the number of runtime module filter */
    runtime_filter_number = (LOG_FILTER_END - LOG_FILTER_START) / sizeof(log_control_block_t);
    /* As this variable need to be by accessed by master CPU, so maybe need to do remap */
    entries = (log_control_block_t *)LOG_FILTER_START;
    g_syslog_share_variable->cpu_module_filter[cpu_id] = (void *)port_syslog_memory_remap_to_primary_cpu(cpu_id, (uint32_t)entries);

    p_filter_info = (module_filter_info_t *)g_syslog_share_variable->cpu_module_filter_status;

    for (i = 0; i < MTK_MAX_CPU_NUMBER; i++) {
        if (!(p_filter_info[i].cpu_id & SYSLOG_FILTER_VALID_MARK)) {
            break;
        }
        if ((p_filter_info[i].cpu_id & (~SYSLOG_FILTER_VALID_MARK)) == cpu_id) {
            if (p_filter_info[i].module_number != runtime_filter_number) {
                assert(0);
                return false;
            }
            return true;
        }
    }
    offset = i;

    p_filters = find_start_of_cpu_log_filters(cpu_id);
    for (i = 0; i < runtime_filter_number; i++) {
        if((uint32_t)(entries[i].log_switch) & 0xFFFFFFFE){entries[i].log_switch = 0x0;}
        if((uint32_t)(entries[i].print_level) & 0xFFFFFFFC){entries[i].print_level = 0x0;}
        p_filters[i] = ((uint8_t)(entries[i].log_switch) << 4) | (uint8_t)(entries[i].print_level);
    }
    p_filter_info[offset].cpu_id = cpu_id | SYSLOG_FILTER_VALID_MARK;
    p_filter_info[offset].module_number = runtime_filter_number;

    return true;
}

void filter_config_print_switch(void *handle, log_switch_t log_switch)
{
    uint32_t index, cpu_id;
    volatile uint8_t *p_filters;

    cpu_id = GET_CURRENT_CPU_ID();
    if (g_syslog_share_variable->cpu_module_filter[cpu_id] == NULL) {
        return;
    }

    index = ((uint32_t)handle - (uint32_t)LOG_FILTER_START) / sizeof(log_control_block_t);
    p_filters = find_start_of_cpu_log_filters(cpu_id);

    p_filters[index] &= 0x0F;
    p_filters[index] |= log_switch << 4;
}

void filter_config_print_level(void *handle, print_level_t log_level)
{
    uint32_t index, cpu_id;
    volatile uint8_t *p_filters;

    cpu_id = GET_CURRENT_CPU_ID();
    if (g_syslog_share_variable->cpu_module_filter[cpu_id] == NULL) {
        return;
    }

    index = ((uint32_t)handle - (uint32_t)LOG_FILTER_START) / sizeof(log_control_block_t);
    p_filters = find_start_of_cpu_log_filters(cpu_id);

    p_filters[index] &= 0xF0;
    p_filters[index] |= log_level;
}

static bool filter_runtime_check(const void *is_module_control, log_control_block_t *context, print_level_t level)
{
    uint32_t offset;
    volatile uint8_t *p_filters;

    /* If current CPU debug level is turn off, bypass the log. */
    if (g_syslog_share_variable->cpu_log_switch[GET_CURRENT_CPU_ID()] == DEBUG_LOG_OFF) {
        return false;
    }

    /* Check the address range to detect the seperate build log filter */
    if (((uint32_t)context >= (uint32_t)LOG_FILTER_END) || ((uint32_t)context < (uint32_t)LOG_FILTER_START)) {
        if ((is_module_control != NULL) &&
                ((context->log_switch == DEBUG_LOG_OFF) ||
                 (level < g_syslog_share_variable->cpu_log_print_level[GET_CURRENT_CPU_ID()]) ||
                 (level < context->print_level))) {
            return false;
        }

        return true;
    }

    /* For LOG_*()/LOG_MSGID_*()/LOG_TLVDUMP_*(), ignore it if
     * 1. If module's debug level is turn off or
     * 2. If current log level is lower than current CPU's debug level.
     * 3. If current log level is lower than module's debug level.
     */
    offset = ((uint32_t)context - (uint32_t)(g_syslog_share_variable->cpu_module_filter[GET_CURRENT_CPU_ID()])) / sizeof(log_control_block_t);
    p_filters = find_start_of_cpu_log_filters(GET_CURRENT_CPU_ID());
    if ((is_module_control != NULL) &&
            (((p_filters[offset] >> 4) == DEBUG_LOG_OFF) ||
             (level < g_syslog_share_variable->cpu_log_print_level[GET_CURRENT_CPU_ID()]) ||
             (level < (p_filters[offset] & 0x0F)))) {
        return false;
    }

    return true;
}

static const char *print_level_table[] = {"debug", "info", "warning", "error"};

#define change_level_to_string(level) \
    ((level) - PRINT_LEVEL_DEBUG <= PRINT_LEVEL_ERROR) ? print_level_table[level] : "debug"

log_create_module(common, PRINT_LEVEL_INFO);
log_create_module(MPLOG, PRINT_LEVEL_WARNING);

NO_INLINE static bool check_log_control(const void *is_module_control, log_control_block_t *context, print_level_t level)
{
    /* check whether syslog is initialized. */
    if (g_syslog_share_variable->init_phase == SYSLOG_INIT_NONE) {
        return false;
    }

    /* check whether syslog is exception mode. */
    if (g_syslog_share_variable->init_phase == SYSLOG_EXCEPTION_INIT) {
        return false;
    }

    /* check whether CPU control when not full initialize. */
    if (g_syslog_share_variable->init_phase == SYSLOG_EARLY_INIT) {
        if (GET_CURRENT_CPU_ID() != 0) {
            return false;
        }
    }

    /* check whether buffer is blocked by dump */
    if (g_syslog_share_variable->init_phase != SYSLOG_INIT_NONE) {
        if (g_syslog_share_variable->lock_buffer == true) {
<<<<<<< HEAD
=======
            g_syslog_share_variable->drop_flush_count += 1;
>>>>>>> db20e11 (second commit)
            return false;
        }
    }

    /* check whether debug level control when full initialize */
    if (g_syslog_share_variable->init_phase == SYSLOG_FULL_INIT) {
        if (filter_runtime_check(is_module_control, context, level) == false) {
            return false;
        }
    }

    return true;
}

#ifdef MTK_SYSLOG_SUB_FEATURE_STRING_LOG_SUPPORT
NO_INLINE static void string_log_handler(void *handle,
                               const char *func,
                               int line,
                               print_level_t level,
                               const char *message,
                               va_list list,
                               const void *data,
                               int data_len)
{
    uint8_t *p_data[4];
    uint32_t p_len[3];
    char *p_frame_header;
    char frame_header[PORT_SYSLOG_MAX_ONE_LOG_SIZE];
    int32_t step_size, log_size, max_size;
    log_control_block_t *context = (log_control_block_t *)handle;

    PORT_SYSLOG_UNUSED(data);
    PORT_SYSLOG_UNUSED(data_len);

    /* anti-nest check */
    if (port_syslog_anti_nest_check_begin() == false) {
        return;
    }

    /* check log control */
    if (check_log_control(func, context, level) == false) {
        port_syslog_anti_nest_check_end();
        return;
    }

#ifdef MTK_DEBUG_PLAIN_LOG_ENABLE
    max_size = sizeof(frame_header) - 2; /* reserved for \r\n */
#else
    max_size = sizeof(frame_header) - 6; /* reserved for TLV header */
#endif

    /* Format the log header and calculate the size */
#ifdef MTK_DEBUG_PLAIN_LOG_ENABLE
    p_frame_header = frame_header;
#else
    p_frame_header = &frame_header[6];
#endif
    if (func) {
        log_size = snprintf(p_frame_header, max_size,
                            "[M:%s C:%s F:%s L:%d]: ",
                            context->module_name,
                            change_level_to_string(level),
                            func,
                            line);
        if (log_size < 0) {
            port_syslog_anti_nest_check_end();
            return;
        }
        if (log_size >= max_size) {
            log_size = max_size - 1;
        }
    } else {
        log_size = 0;
    }

    /* Format the log string/arguments and calculate the size */
    max_size -= log_size;
    p_frame_header += log_size;
    step_size = vsnprintf(p_frame_header, max_size, message, list);
    if (step_size < 0) {
        port_syslog_anti_nest_check_end();
        return;
    }
    if (step_size >= max_size) {
        step_size = max_size - 1;
    }
    log_size += step_size;

#ifndef MTK_DEBUG_PLAIN_LOG_ENABLE
    /* fill CPU ID / reserve byte / timestamp */
    frame_header[0] = GET_CURRENT_CPU_ID() + 1;
    frame_header[1] = 0;
    frame_header[2] = 0;
    frame_header[3] = 0;
    frame_header[4] = 0;
    frame_header[5] = 0;
    port_syslog_tx_hook(RACE_PROTOCAL_STRING_LOG,(uint8_t*)frame_header, port_syslog_get_current_timestamp());
#else
    /* add \r\n in the end */
    frame_header[log_size] = '\r';
    frame_header[log_size + 1] = '\n';
#endif

    p_data[0] = (uint8_t *)frame_header;
    p_data[1] = NULL;
#ifndef MTK_DEBUG_PLAIN_LOG_ENABLE
    p_len[0] = log_size + 6;
#else
    p_len[0] = log_size + 2;
#endif

    port_syslog_send(RACE_PROTOCAL_STRING_LOG, true, (const uint8_t **)p_data, p_len);

    port_syslog_anti_nest_check_end();
}

void vprint_module_log(void *handle,
                       const char *func,
                       int line,
                       print_level_t level,
                       const char *message,
                       va_list list)
{
    string_log_handler(handle, func, line, level, message, list, NULL, 0);
}

/* For LOG_*() */
void print_module_log(void *handle,
                      const char *func,
                      int line,
                      print_level_t level,
                      const char *message, ...)
{
    va_list ap;

    va_start(ap, message);
    vprint_module_log(handle, func, line, level, message, ap);
    va_end(ap);
}

/* For printf() */
int __wrap_printf( const char *format, ... )
{
    va_list ap;

    va_start(ap, format);
    vprint_module_log(&LOG_CONTROL_BLOCK_SYMBOL(common), NULL, 0, 0, format, ap);
    va_end(ap);

    return 0;
}

void vdump_module_buffer(void *handle,
                         const char *func,
                         int line,
                         print_level_t level,
                         const void *data,
                         int length,
                         const char *message,
                         va_list list)
{
    string_log_handler(handle, func, line, level, message, list, data, length);
}

/* For LOG_HEXDUMP_*() */
void dump_module_buffer(void *handle,
                        const char *func,
                        int line,
                        print_level_t level,
                        const void *data,
                        int length,
                        const char *message, ...)
{
    va_list ap;

    va_start(ap, message);
    vdump_module_buffer(handle, func, line, level, data, length, message, ap);
    va_end(ap);
}
#else /* MTK_SYSLOG_SUB_FEATURE_STRING_LOG_SUPPORT */
void vprint_module_log(void *handle,
                       const char *func,
                       int line,
                       print_level_t level,
                       const char *message,
                       va_list list)
{
    PORT_SYSLOG_UNUSED(handle);
    PORT_SYSLOG_UNUSED(func);
    PORT_SYSLOG_UNUSED(line);
    PORT_SYSLOG_UNUSED(level);
    PORT_SYSLOG_UNUSED(message);
    PORT_SYSLOG_UNUSED(list);
}
void print_module_log(void *handle,
                      const char *func,
                      int line,
                      print_level_t level,
                      const char *message, ...)
{
    PORT_SYSLOG_UNUSED(handle);
    PORT_SYSLOG_UNUSED(func);
    PORT_SYSLOG_UNUSED(line);
    PORT_SYSLOG_UNUSED(level);
    PORT_SYSLOG_UNUSED(message);
}

int __wrap_printf( const char *format, ... )
{
    PORT_SYSLOG_UNUSED(format);

    return 0;
}
void vdump_module_buffer(void *handle,
                         const char *func,
                         int line,
                         print_level_t level,
                         const void *data,
                         int length,
                         const char *message,
                         va_list list)
{
    PORT_SYSLOG_UNUSED(handle);
    PORT_SYSLOG_UNUSED(func);
    PORT_SYSLOG_UNUSED(line);
    PORT_SYSLOG_UNUSED(level);
    PORT_SYSLOG_UNUSED(data);
    PORT_SYSLOG_UNUSED(length);
    PORT_SYSLOG_UNUSED(message);
    PORT_SYSLOG_UNUSED(list);
}
void dump_module_buffer(void *handle,
                        const char *func,
                        int line,
                        print_level_t level,
                        const void *data,
                        int length,
                        const char *message, ...)
{
    PORT_SYSLOG_UNUSED(handle);
    PORT_SYSLOG_UNUSED(func);
    PORT_SYSLOG_UNUSED(line);
    PORT_SYSLOG_UNUSED(level);
    PORT_SYSLOG_UNUSED(data);
    PORT_SYSLOG_UNUSED(length);
    PORT_SYSLOG_UNUSED(message);
}
#endif /* MTK_SYSLOG_SUB_FEATURE_STRING_LOG_SUPPORT */

#ifdef MTK_SYSLOG_SUB_FEATURE_BINARY_LOG_SUPPORT

#define MAX_TLV_DUMP_DATA_SIZE 2048

#define MAX_USER_DUMP_BUFFER_COUNT 5

NO_INLINE static int32_t binary_log_handler(void *handle,
                                  print_level_t level,
                                  log_type_t type,
                                  const char *message,
                                  const void **p_data,
                                  uint32_t *p_length)
{
    uint8_t *p_tx_data[MAX_USER_DUMP_BUFFER_COUNT + 3];
    uint32_t p_tx_len[MAX_USER_DUMP_BUFFER_COUNT + 2];
    uint8_t tlv_header[12];
    uint32_t i, payload_size, buffer_count, offset;
    uint32_t res_len;
    log_control_block_t *context = (log_control_block_t *)handle;

#ifdef MTK_DEBUG_PLAIN_LOG_ENABLE
    return 0;
#endif

    /* anti-nest check */
    if (port_syslog_anti_nest_check_begin() == false) {
        return 0;
    }

    /* check log control */
    if (check_log_control(p_data, context, level) == false) {
        port_syslog_anti_nest_check_end();
        return 0;
    }

    /* Calculate total size of payload */
    payload_size = 0;
    buffer_count = 0;
    for (i = 0; p_data[i] != NULL; i++) {
        buffer_count++;
        if (buffer_count > MAX_USER_DUMP_BUFFER_COUNT) {
            port_syslog_anti_nest_check_end();
            return 0;
        }
        p_tx_data[i + 1] = (uint8_t *)p_data[i];
        p_tx_len[i + 1] = p_length[i];
        payload_size += p_length[i];
    }
    p_tx_data[i + 1] = NULL;

    /* Limit the max size of dump data when calling LOG_TLVDUMP_*() */
    if (g_syslog_share_variable->init_phase == SYSLOG_FULL_INIT) {
        if (payload_size > MAX_TLV_DUMP_DATA_SIZE) {
            payload_size = MAX_TLV_DUMP_DATA_SIZE;
        }
    }

    /* Initialize the TLV header */
    if (type == LOG_TYPE_MSG_ID_LOG) {
        tlv_header[0] = GET_CURRENT_CPU_ID() + 1;
        tlv_header[1] = 0;
        tlv_header[2] = 0;
        tlv_header[3] = 0;
        tlv_header[4] = 0;
        tlv_header[5] = 0;
        port_syslog_tx_hook(RACE_PROTOCAL_STRING_LOG, (uint8_t*)tlv_header, port_syslog_get_current_timestamp());
        offset = (uint32_t)message - PORT_SYSLOG_MSG_ADDR_OFFSET;
        tlv_header[6] = offset & 0xff;
        tlv_header[7] = (offset >> 8) & 0xff;
        tlv_header[8] = (offset >> 16) & 0xff;
        tlv_header[9] = (offset >> 24) & 0xff;
        /* syslog header */
        p_tx_data[0] = (uint8_t *)tlv_header;
        p_tx_len[0]  = 10;
        res_len = port_syslog_send(RACE_PROTOCAL_MSGID_LOG, true, (const uint8_t **)p_tx_data, p_tx_len);
    }else if((type > LOG_TYPE_COMMON_LOG_END)&&(type<LOG_TYPE_SPECIAL_LOG_END)){
        tlv_header[0] = 0;
        p_tx_data[0] = (uint8_t *)tlv_header;
        p_tx_len[0]  = 0;
        res_len = port_syslog_send(RACE_PROTOCAL_TLV_LOG|type, true, (const uint8_t **)p_tx_data, p_tx_len);
    } else{
        res_len = 0;
    }

    port_syslog_anti_nest_check_end();
    return res_len;
}

#define MAX_SYSLOG_MSG_ID_PARAMETER_NUMBER 20

void log_print_msgid(void *handle,
                             print_level_t level,
                             const char *message,
                             uint32_t arg_cnt,
                             va_list list)
{
    uint32_t i, buffer[MAX_SYSLOG_MSG_ID_PARAMETER_NUMBER];
    void *p_data[2];
    uint32_t p_length[2];

#ifdef MTK_DEBUG_PLAIN_LOG_ENABLE
    const char *fun=" ";
    vprint_module_log(handle, fun, 0, level, message, list);
    return;
#endif

    if (arg_cnt > MAX_SYSLOG_MSG_ID_PARAMETER_NUMBER) {
        return;
    }

    for (i = 0; i < arg_cnt; i++) {
        buffer[i] = va_arg(list, uint32_t);
    }

    p_data[0] = buffer;
    p_data[1] = NULL;
    p_length[0] = arg_cnt * sizeof(uint32_t);
    p_length[1] = 0;
    binary_log_handler(handle, level, LOG_TYPE_MSG_ID_LOG, message, (const void **)p_data, p_length);
}

/* For LOG_MSGID_*() */
void print_module_msgid_log(void *handle,
                            print_level_t level,
                            const char *message,
                            uint32_t arg_cnt, ...)
{
    va_list list;

    va_start(list, arg_cnt);
    log_print_msgid(handle, level, message, arg_cnt, list);
    va_end(list);
}

/* For LOG_TLVDUMP_*() */
uint32_t dump_module_tlv_buffer(void *handle,
                                print_level_t level,
                                log_type_t type,
                                const void **p_data,
                                uint32_t *p_length)
{
    /* airlog filter hci log */
    if((g_syslog_port >= MUX_AIRAPP_BEGIN) && (g_syslog_port <= MUX_AIRAPP_END)){
        return 0;
    }else{
        return binary_log_handler(handle, level, type, NULL, p_data, p_length);
    }
    return 0;
}
#else
void log_print_msgid(void *handle,
                             print_level_t level,
                             const char *message,
                             uint32_t arg_cnt,
                             va_list list)
{
    PORT_SYSLOG_UNUSED(handle);
    PORT_SYSLOG_UNUSED(level);
    PORT_SYSLOG_UNUSED(message);
    PORT_SYSLOG_UNUSED(arg_cnt);
    PORT_SYSLOG_UNUSED(list);
}
void print_module_msgid_log(void *handle,
                            print_level_t level,
                            const char *message,
                            uint32_t arg_cnt, ...)
{
    PORT_SYSLOG_UNUSED(handle);
    PORT_SYSLOG_UNUSED(level);
    PORT_SYSLOG_UNUSED(message);
    PORT_SYSLOG_UNUSED(arg_cnt);
}

uint32_t dump_module_tlv_buffer(void *handle,
                                print_level_t level,
                                log_type_t type,
                                const void **p_data,
                                uint32_t *p_length)
{
    PORT_SYSLOG_UNUSED(handle);
    PORT_SYSLOG_UNUSED(level);
    PORT_SYSLOG_UNUSED(type);
    PORT_SYSLOG_UNUSED(p_data);
    PORT_SYSLOG_UNUSED(p_length);

    return 0;
}

#endif /* MTK_SYSLOG_SUB_FEATURE_BINARY_LOG_SUPPORT */

#else /* !MTK_DEBUG_LEVEL_NONE */

bool log_set_filter(void)
{
    return false;
}

void filter_config_print_switch(void *handle, log_switch_t log_switch)
{
    PORT_SYSLOG_UNUSED(handle);
    PORT_SYSLOG_UNUSED(log_switch);
}

void filter_config_print_level(void *handle, print_level_t log_level)
{
    PORT_SYSLOG_UNUSED(handle);
    PORT_SYSLOG_UNUSED(log_level);
}

void vprint_module_log(void *handle,
                       const char *func,
                       int line,
                       print_level_t level,
                       const char *message,
                       va_list list)
{
    PORT_SYSLOG_UNUSED(handle);
    PORT_SYSLOG_UNUSED(func);
    PORT_SYSLOG_UNUSED(line);
    PORT_SYSLOG_UNUSED(level);
    PORT_SYSLOG_UNUSED(message);
    PORT_SYSLOG_UNUSED(list);
}

void print_module_log(void *handle,
                      const char *func,
                      int line,
                      print_level_t level,
                      const char *message, ...)
{
    PORT_SYSLOG_UNUSED(handle);
    PORT_SYSLOG_UNUSED(func);
    PORT_SYSLOG_UNUSED(line);
    PORT_SYSLOG_UNUSED(level);
    PORT_SYSLOG_UNUSED(message);
}

int __wrap_printf( const char *format, ... )
{
    PORT_SYSLOG_UNUSED(format);

    return 0;
}

void vdump_module_buffer(void *handle,
                         const char *func,
                         int line,
                         print_level_t level,
                         const void *data,
                         int length,
                         const char *message,
                         va_list list)
{
    PORT_SYSLOG_UNUSED(handle);
    PORT_SYSLOG_UNUSED(func);
    PORT_SYSLOG_UNUSED(line);
    PORT_SYSLOG_UNUSED(level);
    PORT_SYSLOG_UNUSED(data);
    PORT_SYSLOG_UNUSED(length);
    PORT_SYSLOG_UNUSED(message);
    PORT_SYSLOG_UNUSED(list);
}

void dump_module_buffer(void *handle,
                        const char *func,
                        int line,
                        print_level_t level,
                        const void *data,
                        int length,
                        const char *message, ...)
{
    PORT_SYSLOG_UNUSED(handle);
    PORT_SYSLOG_UNUSED(func);
    PORT_SYSLOG_UNUSED(line);
    PORT_SYSLOG_UNUSED(level);
    PORT_SYSLOG_UNUSED(data);
    PORT_SYSLOG_UNUSED(length);
    PORT_SYSLOG_UNUSED(message);
}

void log_print_msgid(void *handle,
                             print_level_t level,
                             const char *message,
                             uint32_t arg_cnt,
                             va_list list)
{
    PORT_SYSLOG_UNUSED(handle);
    PORT_SYSLOG_UNUSED(level);
    PORT_SYSLOG_UNUSED(message);
    PORT_SYSLOG_UNUSED(arg_cnt);
    PORT_SYSLOG_UNUSED(list);
}

void print_module_msgid_log(void *handle,
                            print_level_t level,
                            const char *message,
                            uint32_t arg_cnt, ...)
{
    PORT_SYSLOG_UNUSED(handle);
    PORT_SYSLOG_UNUSED(level);
    PORT_SYSLOG_UNUSED(message);
    PORT_SYSLOG_UNUSED(arg_cnt);
}

uint32_t dump_module_tlv_buffer(void *handle,
                                print_level_t level,
                                log_type_t type,
                                const void **p_data,
                                uint32_t *p_length)
{
    PORT_SYSLOG_UNUSED(handle);
    PORT_SYSLOG_UNUSED(level);
    PORT_SYSLOG_UNUSED(type);
    PORT_SYSLOG_UNUSED(p_data);
    PORT_SYSLOG_UNUSED(p_length);

    return 0;
}

#endif /* !MTK_DEBUG_LEVEL_NONE */

#else //level 0 #if !defined(MTK_DEBUG_LEVEL_PRINTF) &&  !defined(__EXT_BOOTLOADER__)

typedef struct {
    uint8_t port_index;
} syslog_share_variable_t;

#ifdef MTK_SINGLE_CPU_ENV
ATTR_ZIDATA_IN_NONCACHED_SYSRAM_4BYTE_ALIGN static volatile syslog_share_variable_t syslog_share_variable;
static volatile syslog_share_variable_t *g_syslog_share_variable = &syslog_share_variable;
#else
#include "hal_resource_assignment.h"
#define PORT_SYSLOG_SHARE_VARIABLE_BEGIN HW_SYSRAM_PRIVATE_MEMORY_SYSLOG_VAR_START
static volatile syslog_share_variable_t *g_syslog_share_variable = (volatile syslog_share_variable_t *)(PORT_SYSLOG_SHARE_VARIABLE_BEGIN);
#endif /* MTK_SINGLE_CPU_ENV */

log_create_module(common, PRINT_LEVEL_INFO);
log_create_module(MPLOG, PRINT_LEVEL_WARNING);

#ifdef MTK_CPU_NUMBER_0

#include "serial_port_assignment.h"
#include "exception_handler.h"

static volatile bool g_primary_cpu_syslog_is_initialized = false;
static volatile bool g_exception_is_happen = false;

void exception_syslog_callback(void)
{
    g_exception_is_happen = true;
}

/* This API will bypass all log APIs including exception APIs. */
void log_global_turn_off(void)
{}

int log_print_exception_log(const char *message, ...)
{
    va_list list;
    int32_t log_size;
    char frame_header[PORT_SYSLOG_MAX_ONE_PLAIN_LOG_SIZE];

    if (g_exception_is_happen == false) {
        return 0;
    }

    va_start(list, message);

    log_size = vsnprintf(frame_header, sizeof(frame_header), message, list);
    if (log_size < 0) {
        va_end(list);
        return 0;
    }
    if ((uint32_t)log_size >= sizeof(frame_header)) {
        log_size = sizeof(frame_header) - 1;
    }

    va_end(list);

    hal_uart_send_polling(g_syslog_share_variable->port_index, (uint8_t *)frame_header, log_size);

    return log_size;
}

/* Internal API for exception data dump. */
void log_dump_exception_data(const uint8_t *data, uint32_t size)
{
    if (g_exception_is_happen == false) {
        return;
    }

    hal_uart_send_polling(g_syslog_share_variable->port_index, data, size);
}


bool log_uart_init(hal_uart_port_t port, hal_uart_baudrate_t baudrate)
{
    hal_uart_config_t uart_config;
    exception_config_type exception_config;

    if (g_primary_cpu_syslog_is_initialized == true) {
        return false;
    }

    uart_config.baudrate    = baudrate;
    uart_config.parity      = HAL_UART_PARITY_NONE;
    uart_config.stop_bit    = HAL_UART_STOP_BIT_1;
    uart_config.word_length = HAL_UART_WORD_LENGTH_8;
    hal_uart_init(port, &uart_config);

    exception_config.init_cb = NULL;
    exception_config.dump_cb = NULL;
    exception_register_callbacks(&exception_config);
    g_exception_is_happen = false;

    g_syslog_share_variable->port_index = CONFIG_SYSLOG_INIT_STAGE_PORT;

    g_primary_cpu_syslog_is_initialized = true;

    return true;
}

bool log_init(syslog_port_type_t port_type, uint8_t port_index)
{
    PORT_SYSLOG_UNUSED(port_type);
    PORT_SYSLOG_UNUSED(port_index);

    return false;
}

void log_save_filter(void)
{}

#endif /* MTK_CPU_NUMBER_0 */

bool log_set_filter(void)
{
    return false;
}

void filter_config_print_switch(void *handle, log_switch_t log_switch)
{
    PORT_SYSLOG_UNUSED(handle);
    PORT_SYSLOG_UNUSED(log_switch);
}

void filter_config_print_level(void *handle, print_level_t log_level)
{
    PORT_SYSLOG_UNUSED(handle);
    PORT_SYSLOG_UNUSED(log_level);
}

void vprint_module_log(void *handle,
                       const char *func,
                       int line,
                       print_level_t level,
                       const char *message,
                       va_list list)
{
    PORT_SYSLOG_UNUSED(handle);
    PORT_SYSLOG_UNUSED(func);
    PORT_SYSLOG_UNUSED(line);
    PORT_SYSLOG_UNUSED(level);
    PORT_SYSLOG_UNUSED(message);
    PORT_SYSLOG_UNUSED(list);
}

void print_module_log(void *handle,
                      const char *func,
                      int line,
                      print_level_t level,
                      const char *message, ...)
{
    PORT_SYSLOG_UNUSED(handle);
    PORT_SYSLOG_UNUSED(func);
    PORT_SYSLOG_UNUSED(line);
    PORT_SYSLOG_UNUSED(level);
    PORT_SYSLOG_UNUSED(message);
}

#ifndef __EXT_BOOTLOADER__
int __wrap_printf( const char *format, ... )
{
    va_list ap;
    int32_t log_size;
    uint32_t irq_mask;
    char frame_header[PORT_SYSLOG_MAX_ONE_PLAIN_LOG_SIZE];

    va_start(ap, format);
    log_size = vsnprintf(frame_header, sizeof(frame_header), format, ap);
    if (log_size < 0) {
        return -1;
    }
    if ((uint32_t)log_size >= (sizeof(frame_header))) {
        log_size = sizeof(frame_header) - 1;
    }
    va_end(ap);

    hal_nvic_save_and_set_interrupt_mask(&irq_mask);
#ifndef MTK_SINGLE_CPU_ENV
    while (hal_hw_semaphore_take(HW_SEMAPHORE_SYSLOG) != HAL_HW_SEMAPHORE_STATUS_OK);
#endif
    uart_send_polling(g_syslog_share_variable->port_index, (uint8_t *)frame_header, log_size);
#ifndef MTK_SINGLE_CPU_ENV
    while (hal_hw_semaphore_give(HW_SEMAPHORE_SYSLOG) != HAL_HW_SEMAPHORE_STATUS_OK);
#endif
    hal_nvic_restore_interrupt_mask(irq_mask);

    return log_size;
}
#else
int __wrap_printf( const char *format, ... )
{
    PORT_SYSLOG_UNUSED(format);
    return 0;
}
#endif

void vdump_module_buffer(void *handle,
                         const char *func,
                         int line,
                         print_level_t level,
                         const void *data,
                         int length,
                         const char *message,
                         va_list list)
{
    PORT_SYSLOG_UNUSED(handle);
    PORT_SYSLOG_UNUSED(func);
    PORT_SYSLOG_UNUSED(line);
    PORT_SYSLOG_UNUSED(level);
    PORT_SYSLOG_UNUSED(data);
    PORT_SYSLOG_UNUSED(length);
    PORT_SYSLOG_UNUSED(message);
    PORT_SYSLOG_UNUSED(list);
}

void dump_module_buffer(void *handle,
                        const char *func,
                        int line,
                        print_level_t level,
                        const void *data,
                        int length,
                        const char *message, ...)
{
    PORT_SYSLOG_UNUSED(handle);
    PORT_SYSLOG_UNUSED(func);
    PORT_SYSLOG_UNUSED(line);
    PORT_SYSLOG_UNUSED(level);
    PORT_SYSLOG_UNUSED(data);
    PORT_SYSLOG_UNUSED(length);
    PORT_SYSLOG_UNUSED(message);
}

void log_print_msgid(void *handle,
                             print_level_t level,
                             const char *message,
                             uint32_t arg_cnt,
                             va_list list)
{
    PORT_SYSLOG_UNUSED(handle);
    PORT_SYSLOG_UNUSED(level);
    PORT_SYSLOG_UNUSED(message);
    PORT_SYSLOG_UNUSED(arg_cnt);
    PORT_SYSLOG_UNUSED(list);
}


void print_module_msgid_log(void *handle,
                            print_level_t level,
                            const char *message,
                            uint32_t arg_cnt, ...)
{
    PORT_SYSLOG_UNUSED(handle);
    PORT_SYSLOG_UNUSED(level);
    PORT_SYSLOG_UNUSED(message);
    PORT_SYSLOG_UNUSED(arg_cnt);
}

uint32_t dump_module_tlv_buffer(void *handle,
                                print_level_t level,
                                log_type_t type,
                                const void **p_data,
                                uint32_t *p_length)
{
    PORT_SYSLOG_UNUSED(handle);
    PORT_SYSLOG_UNUSED(level);
    PORT_SYSLOG_UNUSED(type);
    PORT_SYSLOG_UNUSED(p_data);
    PORT_SYSLOG_UNUSED(p_length);

    return 0;
}

#endif /* !MTK_DEBUG_LEVEL_PRINTF */

