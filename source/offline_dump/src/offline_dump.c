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

#include "offline_dump.h"

/* status of control block */
#define CONTROL_BLOCK_STATUS_ERASED         0xFF
#define CONTROL_BLOCK_STATUS_WRITTING       0xFE
#define CONTROL_BLOCK_STATUS_WRITE_FINISH   0xFC
#define CONTROL_BLOCK_STATUS_ERASING        0xF8
<<<<<<< HEAD

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define OFFLINE_MAX_CALLBACK_NUMBER         16
=======
#define CONTROL_BLOCK_STATUS_INVALID        0xA0

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define OFFLINE_MAX_CALLBACK_NUMBER         8

#ifndef __EXT_BOOTLOADER__
log_create_module(OFFLINE_DUMP, PRINT_LEVEL_INFO);
#define OFFLINE_DUMP_I(fmt, arg...)    LOG_I(OFFLINE_DUMP, fmt, ##arg)
#define OFFLINE_DUMP_W(fmt, arg...)    LOG_W(OFFLINE_DUMP, fmt, ##arg)
#define OFFLINE_DUMP_E(fmt, arg...)    LOG_E(OFFLINE_DUMP, fmt, ##arg)

#define OFFLINE_DUMP_MSGID_I(fmt, cnt, arg...)    LOG_MSGID_I(OFFLINE_DUMP, fmt, cnt, ##arg)
#define OFFLINE_DUMP_MSGID_W(fmt, cnt, arg...)    LOG_MSGID_W(OFFLINE_DUMP, fmt, cnt, ##arg)
#define OFFLINE_DUMP_MSGID_E(fmt, cnt, arg...)    LOG_MSGID_E(OFFLINE_DUMP, fmt, cnt, ##arg)
#else
#define OFFLINE_DUMP_I(fmt, arg...)
#define OFFLINE_DUMP_W(fmt, arg...)
#define OFFLINE_DUMP_E(fmt, arg...)

#define OFFLINE_DUMP_MSGID_I(fmt, cnt, arg...)
#define OFFLINE_DUMP_MSGID_W(fmt, cnt, arg...)
#define OFFLINE_DUMP_MSGID_E(fmt, cnt, arg...)
#endif /* __EXT_BOOTLOADER__ */

>>>>>>> db20e11 (second commit)

const char *magic_name[OFFLINE_REGION_MAX] = {
    "OFFLINE_DUMP_V1_SYSLOG",
    "OFFLINE_DUMP_V1_MINIDUMP",
    "OFFLINE_DUMP_V1_ERROR_LOG",
};

extern char build_date_time_str[];
extern char sw_verno_str[];

EXTERN BOOL g_is_fota_ongoing;

typedef struct {
    uint8_t type;
    char *magic_name;
    uint32_t start_address;
    uint32_t region_size;
    uint32_t cell_size;
    uint32_t cell_count;
    uint32_t cell_valid_size;
} offline_dump_info_t;

static volatile bool g_offline_dump_is_initialized[OFFLINE_REGION_MAX] = {false};
static offline_dump_info_t g_offline_dump_info_array[OFFLINE_REGION_MAX];
static volatile uint32_t g_offline_dump_current_sequence[OFFLINE_REGION_MAX] = {0};
static volatile bool g_offline_dump_region_is_written[OFFLINE_REGION_MAX] = {false};
static offline_callback_t g_offline_log_head_callback[OFFLINE_REGION_MAX][OFFLINE_MAX_CALLBACK_NUMBER] = {{NULL}, {NULL}, {NULL}};
<<<<<<< HEAD

static bool port_offline_dump_access_is_allowed(void)
{
    if (g_is_fota_ongoing == true) {
        return false;
    } else {
        return true;
    }
=======
static offline_callback_t g_offline_log_tail_callback[OFFLINE_REGION_MAX][OFFLINE_MAX_CALLBACK_NUMBER] = {{NULL}, {NULL}, {NULL}};

static bool port_offline_dump_access_is_allowed(offline_dump_region_type_t region_type)
{
    /* invalid region type */
    if (region_type >= OFFLINE_REGION_MAX) {
        return false;
    }

    /* flash maximum erasable times */
    if (g_offline_dump_current_sequence[region_type] >= 50000U) {
        return false;
    }

    if (g_is_fota_ongoing == true) {
        return false;
    }

    return true;
>>>>>>> db20e11 (second commit)
}

static uint16_t calculate_checksum(uint8_t *p_buf, uint32_t count)
{
    uint32_t sum;

    sum = 0;
    while (count > 1) {
        sum += *(uint16_t *)p_buf;
        count -= 2;
        p_buf += 2;
    }

    if (count > 0) {
        sum += *p_buf;
    }

    while (sum >> 16) {
        sum = (sum & 0xffff) + (sum >> 16);
    }

    return (uint16_t)(~sum);
}

static void check_offline_dump_region_integrity(offline_dump_region_type_t region_type)
{
    uint8_t status;
    uint8_t *p_payload;
<<<<<<< HEAD
=======
    uint8_t version_build_buffer[OFFLINE_BUILD_INFO_SIZE];
>>>>>>> db20e11 (second commit)
    uint32_t j;
    int32_t ongoing_cell;
    uint16_t header_size, checksum, calc_checksum;
    uint32_t total_block, curr_addr, curr_seq_number, max_seq_number;
    bool need_total_format, dump_is_empty;
    offline_dump_header_t *p_header;

    if (region_type >= OFFLINE_REGION_MAX) {
        return;
    }

    /* check dump status */
    ongoing_cell = -1;
    need_total_format = false;
    max_seq_number = 0;
    dump_is_empty = true;
    for (j = 0; j < g_offline_dump_info_array[region_type].cell_count; j++) {
        status = 0;
        p_header = (offline_dump_header_t *)(g_offline_dump_info_array[region_type].start_address + j * g_offline_dump_info_array[region_type].cell_size);
        PORT_FLASH_READ((uint32_t)&p_header->status, &status, 1);
        switch (status) {
<<<<<<< HEAD
=======
            case CONTROL_BLOCK_STATUS_INVALID:
                continue;
>>>>>>> db20e11 (second commit)
            case CONTROL_BLOCK_STATUS_ERASED:
                continue;
            case CONTROL_BLOCK_STATUS_WRITTING:
            case CONTROL_BLOCK_STATUS_ERASING:
                if (ongoing_cell >= 0) {
                    need_total_format = true;
                } else {
                    ongoing_cell = j;
                    continue;
                }
                break;
            case CONTROL_BLOCK_STATUS_WRITE_FINISH:
                curr_seq_number = 0;
                PORT_FLASH_READ((uint32_t)&p_header->seq_number, (uint8_t *)&curr_seq_number, sizeof(p_header->seq_number));
                if (curr_seq_number > max_seq_number) {
                    max_seq_number = curr_seq_number;
                }
<<<<<<< HEAD
                dump_is_empty = false;
=======
>>>>>>> db20e11 (second commit)
                break;
            default:
                need_total_format = true;
        }

        if (need_total_format == true) {
            break;
        }

        /* check magic number */
        checksum = 0;
        header_size = 0;
        PORT_FLASH_READ((uint32_t)&p_header->header_checksum, (uint8_t *)&checksum, sizeof(p_header->header_checksum));
        PORT_FLASH_READ((uint32_t)&p_header->header_size, (uint8_t *)&header_size, sizeof(p_header->header_size));

<<<<<<< HEAD
=======
        if (header_size != sizeof(offline_dump_header_t)) {
            need_total_format = true;
            break;
        }

>>>>>>> db20e11 (second commit)
        p_payload = (uint8_t *)malloc(header_size - 6);

        PORT_FLASH_READ((uint32_t)&p_header->header_size, p_payload, header_size - 6);
        calc_checksum = calculate_checksum(p_payload, header_size - 6);

        free(p_payload);

        if (checksum != calc_checksum) {
            need_total_format = true;
            break;
        }
<<<<<<< HEAD
=======

        /* verify SDK version */
        memset(version_build_buffer, 0, sizeof(version_build_buffer));
        PORT_FLASH_READ((uint32_t)&p_header->sdk_version, version_build_buffer, OFFLINE_BUILD_INFO_SIZE);
        if(strncmp((char *)version_build_buffer, sw_verno_str, OFFLINE_BUILD_INFO_SIZE) != 0) {
            status = CONTROL_BLOCK_STATUS_INVALID;
            PORT_FLASH_WRITE((uint32_t)&p_header->status, &status, sizeof(p_header->status));
            continue;
        }

        /* verify build time */
        memset(version_build_buffer, 0, sizeof(version_build_buffer));
        PORT_FLASH_READ((uint32_t)&p_header->build_time, version_build_buffer, OFFLINE_BUILD_INFO_SIZE);
        if(strncmp((char *)version_build_buffer, build_date_time_str, OFFLINE_BUILD_INFO_SIZE) != 0) {
            status = CONTROL_BLOCK_STATUS_INVALID;
            PORT_FLASH_WRITE((uint32_t)&p_header->status, &status, sizeof(p_header->status));
            continue;
        }
        dump_is_empty = false;
>>>>>>> db20e11 (second commit)
    }

    if (need_total_format == true) {
        /* Format the region  */
        total_block = (g_offline_dump_info_array[region_type].cell_count * g_offline_dump_info_array[region_type].cell_size) / SERIAL_FLASH_BLOCK_SIZE;
        curr_addr = g_offline_dump_info_array[region_type].start_address;
        for (j = 0; j < total_block; j++) {
            PORT_FLASH_ERASE(curr_addr, PORT_FLASH_BLOCK_SIZE);
            curr_addr += SERIAL_FLASH_BLOCK_SIZE;
        }
        g_offline_dump_current_sequence[region_type] = 0;
    } else {
        if (ongoing_cell >= 0) {
            total_block = g_offline_dump_info_array[region_type].cell_size / SERIAL_FLASH_BLOCK_SIZE;
            curr_addr = g_offline_dump_info_array[region_type].start_address + ongoing_cell * g_offline_dump_info_array[region_type].cell_size;
            for (j = 0; j < total_block; j++) {
                PORT_FLASH_ERASE(curr_addr, PORT_FLASH_BLOCK_SIZE);
                curr_addr += SERIAL_FLASH_BLOCK_SIZE;
            }
        }
        if (dump_is_empty == false) {
            g_offline_dump_current_sequence[region_type] = max_seq_number + 1;
        }
    }
}

static void initial_verno_build_time(offline_dump_header_t *p_header)
{
    uint8_t buffer[OFFLINE_BUILD_INFO_SIZE];
    uint32_t sdk_ver_cnt, build_time_cnt;

    sdk_ver_cnt = 1;
    PORT_FLASH_WRITE((uint32_t)&p_header->sdk_ver_cnt, (uint8_t *)&sdk_ver_cnt, sizeof(p_header->sdk_ver_cnt));
    build_time_cnt = OFFLINE_BUILD_TIME_COUNT;
    PORT_FLASH_WRITE((uint32_t)&p_header->build_time_cnt, (uint8_t *)&build_time_cnt, sizeof(p_header->build_time_cnt));
    memset(buffer, 0, sizeof(buffer));
    memcpy(buffer, sw_verno_str, (strlen(sw_verno_str) + 1));
    PORT_FLASH_WRITE((uint32_t)p_header->sdk_version, buffer, OFFLINE_BUILD_INFO_SIZE);
    memset(buffer, 0, sizeof(buffer));
    memcpy(buffer, build_date_time_str, (strlen(build_date_time_str) + 1));
    PORT_FLASH_WRITE((uint32_t)p_header->build_time, buffer, OFFLINE_BUILD_INFO_SIZE);
}

/* Call this API to do initialization of total block information. */
bool offline_dump_region_init(void)
{
    uint32_t i;

    for (i = 0; i < OFFLINE_REGION_MAX; i++) {
        if (g_offline_dump_is_initialized[i] == true) {
            continue;
        }

        /* Check whether FOTA allows us to write to FOTA */
<<<<<<< HEAD
        if (port_offline_dump_access_is_allowed() == false) {
=======
        if (port_offline_dump_access_is_allowed(i) == false) {
>>>>>>> db20e11 (second commit)
            continue;
        }

        if (PARTITION_TABLE[i].BinaryId != i) {
            continue;
        }

        if (PARTITION_TABLE[i].LoadAddressLow == 0x0) {
            continue;
        }

        if ((PARTITION_TABLE[i].CellSize == 0) || (PARTITION_TABLE[i].CellSize % SERIAL_FLASH_BLOCK_SIZE) != 0) {
            continue;
        }

        g_offline_dump_info_array[i].type               = PARTITION_TABLE[i].BinaryId + 1;
        g_offline_dump_info_array[i].magic_name         = (char *)magic_name[i];
        g_offline_dump_info_array[i].start_address      = (PARTITION_TABLE[i].LoadAddressLow & 0x0FFFFFFF);
        g_offline_dump_info_array[i].region_size        = PARTITION_TABLE[i].BinaryLengthLow;
        g_offline_dump_info_array[i].cell_size          = PARTITION_TABLE[i].CellSize;
        g_offline_dump_info_array[i].cell_count         = PARTITION_TABLE[i].BinaryLengthLow / PARTITION_TABLE[i].CellSize;
        g_offline_dump_info_array[i].cell_valid_size    = PARTITION_TABLE[i].CellSize - sizeof(offline_dump_header_t);
        check_offline_dump_region_integrity((offline_dump_region_type_t)i);

#ifndef __EXT_BOOTLOADER__
<<<<<<< HEAD
        LOG_I(common, "Offline dump type[%d] magic[%s] address[0x%08x] size[0x%x] cell_size[%d] cell_count[%d] cell_valid_size[%d] \r\n",
=======
        OFFLINE_DUMP_I("Offline dump type[%d] magic[%s] address[0x%08x] size[0x%x] cell_size[%d] cell_count[%d] cell_valid_size[%d] \r\n",
>>>>>>> db20e11 (second commit)
                                                g_offline_dump_info_array[i].type,
                                                g_offline_dump_info_array[i].magic_name,
                                                g_offline_dump_info_array[i].start_address,
                                                g_offline_dump_info_array[i].region_size,
                                                g_offline_dump_info_array[i].cell_size,
                                                g_offline_dump_info_array[i].cell_count,
                                                g_offline_dump_info_array[i].cell_valid_size);
#endif

        g_offline_dump_is_initialized[i] = true;
    }

    return true;
}

/* Call this API to alloc the next position to write to. */
bool offline_dump_region_alloc(offline_dump_region_type_t region_type, uint32_t *p_start_addr)
{
    uint8_t status;
    uint32_t i, curr_index, start_addr;
    offline_dump_header_t *p_header;

    offline_dump_region_init();

    if (g_offline_dump_is_initialized[region_type] == false) {
        return false;
    }

    /* Check whether FOTA allows us to write to FOTA */
<<<<<<< HEAD
    if (port_offline_dump_access_is_allowed() == false) {
=======
    if (port_offline_dump_access_is_allowed(region_type) == false) {
>>>>>>> db20e11 (second commit)
        return false;
    }

    if (region_type >= OFFLINE_REGION_MAX) {
        return false;
    }

    if (p_start_addr == NULL) {
        return false;
    }

    if (g_offline_dump_region_is_written[region_type] == true) {
        return false;
    }

    check_offline_dump_region_integrity(region_type);

    /* Calculate the position of dump cell */
    curr_index = g_offline_dump_current_sequence[region_type] % g_offline_dump_info_array[region_type].cell_count;
    start_addr = g_offline_dump_info_array[region_type].start_address + curr_index * g_offline_dump_info_array[region_type].cell_size;
    p_header = (offline_dump_header_t *)start_addr;

    *p_start_addr = start_addr + sizeof(offline_dump_header_t);

    /* mark current block cell as ERASING status */
    status = CONTROL_BLOCK_STATUS_ERASING;
    PORT_FLASH_WRITE((uint32_t)&p_header->status, &status, sizeof(p_header->status));

    /* Erase current block cell */
    for (i = 0; i < (g_offline_dump_info_array[region_type].cell_size / SERIAL_FLASH_BLOCK_SIZE); i++) {
        PORT_FLASH_ERASE((uint32_t)start_addr, HAL_FLASH_BLOCK_4K);
        start_addr += SERIAL_FLASH_BLOCK_SIZE;
    }

    /* mark current block cell as WRITTING status */
    status = CONTROL_BLOCK_STATUS_WRITTING;
    PORT_FLASH_WRITE((uint32_t)&p_header->status, &status, sizeof(p_header->status));

    g_offline_dump_region_is_written[region_type] = true;

    return true;
}

/* Call this API to write the dump data to flash, call be repeated many times */
bool offline_dump_region_write(offline_dump_region_type_t region_type, uint32_t curr_addr, uint8_t *data, uint32_t length)
{
    offline_dump_region_init();

    if (g_offline_dump_is_initialized[region_type] == false) {
        return false;
    }

    /* Check whether FOTA allows us to write to FOTA */
<<<<<<< HEAD
    if (port_offline_dump_access_is_allowed() == false) {
=======
    if (port_offline_dump_access_is_allowed(region_type) == false) {
>>>>>>> db20e11 (second commit)
        return false;
    }

    if (region_type >= OFFLINE_REGION_MAX) {
        return false;
    }

    if ((data == NULL) || (length == 0)) {
        return false;
    }

    if (g_offline_dump_region_is_written[region_type] == false) {
        return false;
    }

    /* write data to flash now */
    PORT_FLASH_WRITE(curr_addr, data, length);

    return true;
}

/* Call this API to end current dump. */
bool offline_dump_region_write_end(offline_dump_region_type_t region_type, uint32_t total_length)
{
    uint8_t buffer[32];
    uint8_t status, dump_version;
    uint16_t header_size, calc_checksum;
    uint32_t curr_index, start_addr, curr_seq_number, payload_size;
    offline_dump_header_t *p_header;

    if (g_offline_dump_is_initialized[region_type] == false) {
        return false;
    }

    /* Check whether FOTA allows us to write to FOTA */
<<<<<<< HEAD
    if (port_offline_dump_access_is_allowed() == false) {
=======
    if (port_offline_dump_access_is_allowed(region_type) == false) {
>>>>>>> db20e11 (second commit)
        return false;
    }

    if (region_type >= OFFLINE_REGION_MAX) {
        return false;
    }

    if (g_offline_dump_region_is_written[region_type] == false) {
        return false;
    }

    /* Calculate the position of dump cell */
    curr_index = g_offline_dump_current_sequence[region_type] % g_offline_dump_info_array[region_type].cell_count;
    start_addr = g_offline_dump_info_array[region_type].start_address + curr_index * g_offline_dump_info_array[region_type].cell_size;
    p_header = (offline_dump_header_t *)start_addr;

    /* Initialize the header */
    dump_version = OFFLINE_REGION_VERSION;
    PORT_FLASH_WRITE((uint32_t)&p_header->dump_version, &dump_version, sizeof(p_header->dump_version));
    PORT_FLASH_WRITE((uint32_t)&p_header->payload_type, &g_offline_dump_info_array[region_type].type, sizeof(p_header->payload_type));
    memset(buffer, 0, sizeof(buffer));
    strcpy((char *)buffer, g_offline_dump_info_array[region_type].magic_name);
    PORT_FLASH_WRITE((uint32_t)p_header->magic_name, buffer, sizeof(p_header->magic_name));
    payload_size = g_offline_dump_info_array[region_type].cell_size - sizeof(offline_dump_header_t);
    PORT_FLASH_WRITE((uint32_t)&p_header->payload_size, (uint8_t *)&payload_size, sizeof(p_header->payload_size));
    PORT_FLASH_WRITE((uint32_t)&p_header->payload_real_size, (uint8_t *)&total_length, sizeof(p_header->payload_real_size));
    curr_seq_number = g_offline_dump_current_sequence[region_type]++;
    PORT_FLASH_WRITE((uint32_t)&p_header->seq_number, (uint8_t *)&curr_seq_number, sizeof(p_header->seq_number));
#ifdef HAL_RTC_MODULE_ENABLED
    {
        hal_rtc_time_t time;
        hal_rtc_status_t status;
        uint8_t rtc_val[8];

        status = hal_rtc_get_time(&time);
        if (status == HAL_RTC_STATUS_OK) {
            rtc_val[0] = time.rtc_year;
            rtc_val[1] = time.rtc_mon;
            rtc_val[2] = time.rtc_day;
            rtc_val[3] = time.rtc_hour;
            rtc_val[4] = time.rtc_min;
            rtc_val[5] = time.rtc_sec;
            rtc_val[6] = 0;
            rtc_val[7] = 0;
            PORT_FLASH_WRITE((uint32_t)&p_header->rtc_time, rtc_val, sizeof(p_header->rtc_time));
        }
    }
#endif
    initial_verno_build_time(p_header);
    header_size = sizeof(offline_dump_header_t);
    PORT_FLASH_WRITE((uint32_t)&p_header->header_size, (uint8_t *)&header_size, sizeof(p_header->header_size));
    calc_checksum = calculate_checksum((uint8_t *)&p_header->header_size, header_size - 6);
    PORT_FLASH_WRITE((uint32_t)&p_header->header_checksum, (uint8_t *)&calc_checksum, sizeof(p_header->header_checksum));

    /* mark current block cell as WRITTING status */
    status = CONTROL_BLOCK_STATUS_WRITE_FINISH;
    PORT_FLASH_WRITE((uint32_t)&p_header->status, &status, sizeof(p_header->status));

    g_offline_dump_region_is_written[region_type] = false;

    return true;
}

bool offline_dump_region_read(offline_dump_region_type_t region_type, uint32_t curr_addr, uint8_t *buf, uint32_t length)
{
    offline_dump_region_init();

    if (g_offline_dump_is_initialized[region_type] == false) {
        return false;
    }

    /* Check whether FOTA allows us to write to FOTA */
<<<<<<< HEAD
    if (port_offline_dump_access_is_allowed() == false) {
=======
    if (port_offline_dump_access_is_allowed(region_type) == false) {
>>>>>>> db20e11 (second commit)
        return false;
    }

    if (region_type >= OFFLINE_REGION_MAX) {
        return false;
    }

    if (buf == NULL) {
        return false;
    }

    if (g_offline_dump_region_is_written[region_type] == true) {
        return false;
    }

    PORT_FLASH_READ(curr_addr, buf, length);

    return true;
}

bool offline_dump_region_query_seq_range(offline_dump_region_type_t region_type, uint32_t *p_min_seq, uint32_t *p_max_seq)
{
<<<<<<< HEAD
    uint32_t min_seq, max_seq;
=======
    bool mark_invalid;
    uint8_t status;
    uint32_t i;
    uint32_t min_seq, max_seq, index;
    offline_dump_header_t *p_header;
>>>>>>> db20e11 (second commit)

    offline_dump_region_init();

    if (g_offline_dump_is_initialized[region_type] == false) {
        return false;
    }

    if (region_type >= OFFLINE_REGION_MAX) {
        return false;
    }

    if ((p_min_seq == NULL) || (p_max_seq == NULL)) {
        return false;
    }

    if (g_offline_dump_region_is_written[region_type] == true) {
        return false;
    }

    if (g_offline_dump_current_sequence[region_type] == 0) {
        return false;
    }

    max_seq = g_offline_dump_current_sequence[region_type] - 1;
    if (g_offline_dump_current_sequence[region_type] >= g_offline_dump_info_array[region_type].cell_count) {
        min_seq = max_seq - (g_offline_dump_info_array[region_type].cell_count - 1);
    } else {
        min_seq = 0;
    }

<<<<<<< HEAD
    *p_min_seq = min_seq;
    *p_max_seq = max_seq;
=======
    /* find out max invalid block */
    if (min_seq > max_seq) {
        return false;
    }

    mark_invalid = false;
    for (i = min_seq; i <= max_seq; i++) {
        status = 0;
        index = (max_seq + min_seq - i) % g_offline_dump_info_array[region_type].cell_count;
        p_header = (offline_dump_header_t *)(g_offline_dump_info_array[region_type].start_address + index * g_offline_dump_info_array[region_type].cell_size);
        PORT_FLASH_READ((uint32_t)&p_header->status, &status, 1);
        if (status == CONTROL_BLOCK_STATUS_INVALID) {
            mark_invalid = true;
            break;
        }
    }

    /* update max/min number */
    if (mark_invalid == true) {
        if (max_seq == (max_seq + min_seq - i)) {
            return false;
        } else {
            *p_min_seq = max_seq + min_seq - i + 1;
            *p_max_seq = max_seq;
        }
    } else {
        *p_min_seq = min_seq;
        *p_max_seq = max_seq;
    }
>>>>>>> db20e11 (second commit)

    return true;
}

bool offline_dump_region_query_by_seq(offline_dump_region_type_t region_type, uint32_t seq, uint32_t *p_start_addr, uint32_t *p_total_length)
{
    uint32_t index;

    offline_dump_region_init();

    if (g_offline_dump_is_initialized[region_type] == false) {
        return false;
    }

    if (region_type >= OFFLINE_REGION_MAX) {
        return false;
    }

    if ((p_start_addr == NULL) || (p_total_length == NULL)) {
        return false;
    }

    if (g_offline_dump_region_is_written[region_type] == true) {
        return false;
    }

    index = seq % g_offline_dump_info_array[region_type].cell_count;
    *p_start_addr = g_offline_dump_info_array[region_type].start_address + index * g_offline_dump_info_array[region_type].cell_size;
    *p_total_length = g_offline_dump_info_array[region_type].cell_size;

    return true;
}

bool offline_dump_region_query_cell_size(offline_dump_region_type_t region_type, uint32_t *p_cell_size)
{
    offline_dump_region_init();

    *p_cell_size = 0;

    if (g_offline_dump_is_initialized[region_type] == false) {
        return false;
    }

    if (region_type >= OFFLINE_REGION_MAX) {
        return false;
    }

    if (p_cell_size == NULL) {
        return false;
    }

    *p_cell_size = g_offline_dump_info_array[region_type].cell_size;

    return true;
}

bool offline_dump_region_query_cell_valid_size(offline_dump_region_type_t region_type, uint32_t *p_valid_size)
{
    offline_dump_region_init();

    *p_valid_size = 0;

    if (g_offline_dump_is_initialized[region_type] == false) {
        return false;
    }

    if (region_type >= OFFLINE_REGION_MAX) {
        return false;
    }

    if (p_valid_size == NULL) {
        return false;
    }

    *p_valid_size = g_offline_dump_info_array[region_type].cell_valid_size;

    return true;
}

bool offline_dump_register_head_callback(offline_dump_region_type_t region_type, offline_callback_t user_callback)
{
    uint32_t i;

    if (region_type >= OFFLINE_REGION_MAX) {
        return false;
    }

<<<<<<< HEAD
=======
    /* Find the insertion point */
>>>>>>> db20e11 (second commit)
    for (i = 0; i < OFFLINE_MAX_CALLBACK_NUMBER; i++) {
        if (g_offline_log_head_callback[region_type][i] == NULL) {
            g_offline_log_head_callback[region_type][i] = user_callback;
            break;
        }
    }

<<<<<<< HEAD
=======
    /* Full, insertion error */
    if (i == OFFLINE_MAX_CALLBACK_NUMBER) {
        return false;
    }

    return true;
}

bool offline_dump_register_tail_callback(offline_dump_region_type_t region_type, offline_callback_t user_callback)
{
    uint32_t i;

    if (region_type >= OFFLINE_REGION_MAX) {
        return false;
    }

    /* Find the insertion point */
    for (i = 0; i < OFFLINE_MAX_CALLBACK_NUMBER; i++) {
        if (g_offline_log_tail_callback[region_type][i] == NULL) {
            g_offline_log_tail_callback[region_type][i] = user_callback;
            break;
        }
    }

    /* Queue full, insertion error */
    if (i == OFFLINE_MAX_CALLBACK_NUMBER) {
        return false;
    }

>>>>>>> db20e11 (second commit)
    return true;
}

void offline_dump_callback_head_handle(offline_dump_region_type_t region_type)
{
    uint32_t i;

    if (region_type >= OFFLINE_REGION_MAX) {
        return;
    }

    for (i = 0; i < OFFLINE_MAX_CALLBACK_NUMBER; i++) {
        if (g_offline_log_head_callback[region_type][i] != NULL) {
<<<<<<< HEAD
=======
            OFFLINE_DUMP_MSGID_I("head callback num[%d] add[0x%08x]", 2, i, g_offline_log_head_callback[region_type][i]);
>>>>>>> db20e11 (second commit)
            g_offline_log_head_callback[region_type][i]();
        }
    }
}
<<<<<<< HEAD
=======

void offline_dump_callback_tail_handle(offline_dump_region_type_t region_type)
{
    uint32_t i;

    if (region_type >= OFFLINE_REGION_MAX) {
        return;
    }

    for (i = 0; i < OFFLINE_MAX_CALLBACK_NUMBER; i++) {
        if (g_offline_log_tail_callback[region_type][i] != NULL) {
            OFFLINE_DUMP_MSGID_I("tail callback num[%d] add[0x%08x]", 2, i, g_offline_log_tail_callback[region_type][i]);
            g_offline_log_tail_callback[region_type][i]();
        }
    }
}





>>>>>>> db20e11 (second commit)
