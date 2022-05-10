/* Copyright Statement:
 *
 * (C) 2005-2016  MediaTek Inc. All rights reserved.
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. ("MediaTek") and/or its licensors.
 * Without the prior written permission of MediaTek and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 * You may only use, reproduce, modify, or distribute (as applicable) MediaTek Software
 * if you have agreed to and been bound by the applicable license agreement with
 * MediaTek ("License Agreement") and been granted explicit permission to do so within
 * the License Agreement ("Permitted User").  If you are not a Permitted User,
 * please cease any access or use of MediaTek Software immediately.
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT MEDIATEK SOFTWARE RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES
 * ARE PROVIDED TO RECEIVER ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

#include "hal_flash.h"
#ifdef HAL_FLASH_MODULE_ENABLED

#include <assert.h>
#include "hal_flash_custom_memorydevice.h"
#include "string.h"
#include "hal_flash_disk_internal.h"
#include "hal_flash_mtd_internal.h"
#include "hal_flash_disk.h"
#include "hal_flash_mtd_sf_dal.h"
#include "hal_flash.h"
#include "memory_attribute.h"
#include "hal_cache.h"
#include "hal_nvic.h"
#include "hal_wdt.h"
#include "syslog.h"


extern NOR_FLASH_DISK_Data EntireDiskDriveData;

#define ENTIRE_DISK_DRIVER_DATA &EntireDiskDriveData


#define RAW_DISK0_DRIVER_DATA &FlashDiskDriveData[RAW_DISK0]
#define TC01_DISK_DRIVER_DATA &FlashDiskDriveData[TC01_DISK]


/*******************************************************************//**
 * Invoke before flash read/write/erase operation.
 * used when __NOR_SUPPORT_RAW_DISK__ or __UBL_NOR_FULL_DRIVER__ is defined.
 * accessed by backup partition and LG partition. (!__UBL__)
 * also access LG partition in bootloader (__UBL__ || __UBL_NOR_FULL_DRIVER__)
 **********************************************************************/
static void MountDevice(NOR_FLASH_DISK_Data *D)
{
    int32_t Result;

    if (D->is_mount) {
        return;
    }

    Result = D->MTDDriver->MountDevice(D->MTDData, (void *)&D->FlashInfo);


    if (Result < FS_NO_ERROR) {
        ASSERT(0);
    }

    D->is_mount = true;
}


/*******************************************************************//**
 * Read data from flash raw disk
 *
 * @par Catagory:
 * NOR RAW DISK
 *
 * @param[in] disk The number of disk to be read
 * @param[in] addr Start address relative to specified disk
 * @param[in] data_ptr Buffer for storing read data
 * @param[in] len The size of data_ptr
 *
 * @remarks
 * 1. File system non-block operation will be finished and then do raw disk read opertaion
 *
 * @return
 * RAW_DISK_ERR_WRONG_ADDRESS: Illegal read address
 * RAW_DISK_ERR_NONE: Read successful
 **********************************************************************/
ATTR_TEXT_IN_RAM int32_t readRawDiskData(NOR_FLASH_DISK_Data *D, uint32_t addr, uint8_t *data_ptr, uint32_t len)
{
    uint32_t phyAddr;
    uint32_t diskSize = D->DiskSize;
    int32_t result;

    if ((addr >= diskSize) || (len > diskSize) || ((addr + len) > diskSize)) {
        return RAW_DISK_ERR_WRONG_ADDRESS;
    }
    phyAddr = (uint32_t)((NOR_Flash_MTD_Data *)D->MTDData)->BaseAddr + addr;

    if (!D->is_mount) {
        MountDevice(D);
    }


    do {
        result = SF_DAL_CheckDeviceReady(D->MTDData);
    } while (FS_FLASH_ERASE_BUSY == result);

    memcpy(data_ptr, (void *)phyAddr, len);


    return RAW_DISK_ERR_NONE;
}


#ifdef __NOR_FULL_DRIVER__

/*******************************************************************//**
 * Write data into flash raw disk
 *
 * @par Catagory:
 * NOR RAW DISK
 *
 * @param[in] disk The number of disk to be read
 * @param[in] addr Start address relative to specified disk
 * @param[in] data_ptr Buffer for storing read data
 * @param[in] len The size of data_ptr
 *
 * @remarks
 * 1. File system non-block operation will be finished and then do raw disk read opertaion
 *
 * @return
 * RAW_DISK_ERR_WRONG_ADDRESS: Illegal program start address
 * RAW_DISK_ERR_NO_SPACE: No enough space to write len bytes data
 * RAW_DISK_ERR_PROG_FAIL: Program fail
 * RAW_DISK_ERR_NONE: Program successful
 **********************************************************************/


int32_t writeRawDiskData(NOR_FLASH_DISK_Data *D, uint32_t addr, uint8_t *data_ptr, uint32_t len)
{
    uint32_t dest, src;
    uint32_t diskSize = D->DiskSize;
    int32_t result = FLASH_DISK_DONE;
    uint32_t DoneLength = 0;
    uint32_t mask;

    if (addr >= diskSize) {
        return RAW_DISK_ERR_WRONG_ADDRESS;
    }

    if ((len > diskSize) || ((addr + len) > diskSize)) {
        return RAW_DISK_ERR_NO_SPACE;
    }

    if (!D->is_mount) {
        MountDevice(D);
    }


    while (DoneLength < len) {
        dest = (uint32_t)((NOR_Flash_MTD_Data *)D->MTDData)->BaseAddr + addr + DoneLength;
        src = (uint32_t)data_ptr + DoneLength;
        hal_nvic_save_and_set_interrupt_mask(&mask);
        D->MTDDriver->MapWindow(D->MTDData, BlockIndex((void *)D->MTDData, addr + DoneLength), 0);
        hal_nvic_restore_interrupt_mask(mask);

        // if dest address not word align or write length is one, write one byte at a time
        if (((uint32_t)dest & (sizeof(uint16_t) - 1)) || ((len - DoneLength) == 1)) {
            result = D->MTDDriver->ProgramData(D->MTDData, (void *)dest, (void *)src, 1);
            if (result != FLASH_DISK_DONE) {
                break;
            }
            DoneLength++;
        } // dest address is word align
        else {
            uint32_t blockSize = BlockSize(D->MTDData, BlockIndex(D->MTDData, addr + DoneLength));
            uint32_t programBytes = len - DoneLength;
            //calculate the block address boundary
            uint32_t nextBlkAddr = (dest + blockSize) & (~(blockSize - 1));

            //uint32_t next128ByteAddr = (dest+ 128) & (~(128 -1));


            //if the data write across block boundary, shrink into a block
            if (dest + programBytes > (nextBlkAddr)) {
                programBytes = nextBlkAddr - dest;
            }

            //round down to multiple of word
            programBytes = (programBytes) & (~(sizeof(uint16_t) - 1));

            // program a word should be word align (MTD limitation)
            if (programBytes == sizeof(uint16_t) && (src % sizeof(uint16_t))) {
                uint16_t Cell;
                uint8_t *b = (uint8_t *) &Cell;
                b[0] = *((uint8_t *)src);
                b[1] = *((uint8_t *)src + 1);
                result = D->MTDDriver->ProgramData(D->MTDData, (void *)dest, (void *)&Cell, programBytes);
                if (result != FLASH_DISK_DONE) {
                    break;
                }
            } else {
                if (programBytes & 0x1) { //must be Sibley flash
                    ASSERT_RET(0, 0);
                } else {
                    result = D->MTDDriver->ProgramData(D->MTDData, (void *)dest, (void *)src, programBytes);
                }
                if (result != FLASH_DISK_DONE) {
                    break;
                }
            }
            DoneLength += programBytes;
        }
    }


    if (result != FLASH_DISK_DONE) {
        return RAW_DISK_ERR_PROG_FAIL;
    }
    return RAW_DISK_ERR_NONE;

}

/*******************************************************************//**
 * Erase a block of flash raw disk
 *
 * @par Catagory:
 * NOR RAW DISK
 *
 * @param[in] disk The number of disk to be read
 * @param[in] blkIdx The block index to be erased
 *
 * @remarks
 * 1. File system non-block operation will be finished and then do raw disk read opertaion
 *
 * @return
 * RAW_DISK_ERR_WRONG_ADDRESS: Illegal block index to be erased
 * RAW_DISK_ERR_ERASE_FAIL: Erase fail
 * RAW_DISK_ERR_NONE: Program successful
 **********************************************************************/
int32_t eraseRawDiskBlock(NOR_FLASH_DISK_Data *D, uint32_t blkIdx)
{

    int32_t result;
    uint32_t block_index = (blkIdx & 0xFFFFFFF);
    if (!D->is_mount) {
        MountDevice(D);
    }

    if (block_index >= D->FlashInfo.TotalBlocks) { //TotalBlocks are initialized in MountDevice()
        return RAW_DISK_ERR_WRONG_ADDRESS;
    }

    result = D->MTDDriver->EraseBlock(D->MTDData, blkIdx);

    if (result == FLASH_DISK_DONE) {
        return RAW_DISK_ERR_NONE;
    }
    return RAW_DISK_ERR_ERASE_FAIL;
}

#endif



extern void Custom_NOR_Init(void);

hal_flash_status_t flash_init_status = HAL_FLASH_STATUS_ERROR_NO_INIT;
int32_t NOR_init(void)
{
    //only init flash one time
    if (flash_init_status == HAL_FLASH_STATUS_ERROR_NO_INIT) {
        Custom_NOR_Init();
        MountDevice(ENTIRE_DISK_DRIVER_DATA);
        flash_init_status = HAL_FLASH_STATUS_OK;
    }
    return ERROR_NOR_SUCCESS;
}

ATTR_TEXT_IN_RAM hal_flash_status_t get_NOR_init_status(void)
{
    return flash_init_status;
}

int32_t NOR_ReadPhysicalPage(uint32_t block_idx, uint32_t page_idx, uint8_t *data_ptr)
{
    NOR_FLASH_DISK_Data *D = ENTIRE_DISK_DRIVER_DATA;
    NOR_Flash_MTD_Data *mtdData = (NOR_Flash_MTD_Data *)D->MTDData;
    int32_t result;

    uint32_t addr = ((uint32_t)BlockAddress((void *)mtdData , block_idx) - (uint32_t)mtdData->BaseAddr)  + (page_idx << NOR_FLASH_PAGE_SHIFT);

    result = readRawDiskData(D, addr, data_ptr, NOR_FLASH_PAGE_SIZE);

    if (result != RAW_DISK_ERR_NONE) {
        return ERROR_NOR_READ;
    }
    return ERROR_NOR_SUCCESS;
}

#if defined(__NOR_FULL_DRIVER__)
int32_t NOR_ProgramPhysicalPage(uint32_t block_idx, uint32_t page_idx, uint8_t *data_ptr)
{
    NOR_FLASH_DISK_Data *D = ENTIRE_DISK_DRIVER_DATA;
    NOR_Flash_MTD_Data *mtdData = (NOR_Flash_MTD_Data *)D->MTDData;
    int32_t result;

    uint32_t addr = ((uint32_t)BlockAddress((void *)mtdData , block_idx) - (uint32_t)mtdData->BaseAddr)  + (page_idx << NOR_FLASH_PAGE_SHIFT);

    result = writeRawDiskData(D, addr, data_ptr, NOR_FLASH_PAGE_SIZE);
    if (result != RAW_DISK_ERR_NONE) {
        return ERROR_NOR_PROGRAM;
    }
    return ERROR_NOR_SUCCESS;
}

int32_t NOR_ErasePhysicalBlock(uint32_t block_idx)
{
    int32_t result;
    result = eraseRawDiskBlock(ENTIRE_DISK_DRIVER_DATA, block_idx);
    if (result != RAW_DISK_ERR_NONE) {
        return ERROR_NOR_ERASE;
    }
    return ERROR_NOR_SUCCESS;
}
#endif //__UBL_NOR_FULL_DRIVER__

uint32_t NOR_BlockSize(uint32_t block_idx)
{
    NOR_FLASH_DISK_Data *D = ENTIRE_DISK_DRIVER_DATA;
    NOR_Flash_MTD_Data *mtdData = (NOR_Flash_MTD_Data *)D->MTDData;
    return BlockSize(mtdData, block_idx);
}

uint32_t NOR_BlockIndex(uint32_t block_addr)
{
    NOR_FLASH_DISK_Data *D = ENTIRE_DISK_DRIVER_DATA;
    NOR_Flash_MTD_Data *mtdData = (NOR_Flash_MTD_Data *)D->MTDData;

    return BlockIndex(mtdData, block_addr);
}

// Return value:
//    ERROR_NOR_OVERRANGE: address out of NOR flash size
//    ERROR_NOR_SUCCESS: block_addr to *block_index, *offset translation successful
int32_t NOR_Addr2BlockIndexOffset(uint32_t block_addr, uint32_t *block_index, uint32_t *offset)
{
    NOR_FLASH_DISK_Data *D = ENTIRE_DISK_DRIVER_DATA;
    NOR_Flash_MTD_Data *mtdData = (NOR_Flash_MTD_Data *)D->MTDData;

#ifdef __UBL__
    block_addr = block_addr & (~(SFC_GENERIC_FLASH_BANK_MASK));
#endif

    *block_index = BlockIndex(mtdData, block_addr);
    if (*block_index == INVALID_BLOCK_INDEX) {
        return ERROR_NOR_OVERRANGE;
    }

    *offset = block_addr % BlockSize(mtdData, *block_index);

    return ERROR_NOR_SUCCESS;
}

// Translate block_index and offset to block address (*addr)
// Return value:
//    ERROR_NOR_SUCCESS
int32_t NOR_BlockIndexOffset2Addr(uint32_t block_index, uint32_t offset, uint32_t *addr)
{
    NOR_FLASH_DISK_Data *D = ENTIRE_DISK_DRIVER_DATA;
    NOR_Flash_MTD_Data *mtdData = (NOR_Flash_MTD_Data *)D->MTDData;

    *addr = (uint32_t)BlockAddress(mtdData, block_index) - (uint32_t)mtdData->BaseAddr + offset;

    return ERROR_NOR_SUCCESS;
}

//-----------------------------------------------------------------------------------------------
// MT2523 Raw Disk API for IOT
//-----------------------------------------------------------------------------------------------
hal_flash_status_t get_rawdisk_error_code(int32_t ori_err_code)
{
    if (ori_err_code == RAW_DISK_ERR_WRONG_ADDRESS) {
        return HAL_FLASH_STATUS_ERROR_WRONG_ADDRESS;
    } else if (ori_err_code == RAW_DISK_ERR_NO_SPACE) {
        return HAL_FLASH_STATUS_ERROR_NO_SPACE;
    } else if (ori_err_code == RAW_DISK_ERR_PROG_FAIL) {
        return HAL_FLASH_STATUS_ERROR_PROG_FAIL;
    } else if (ori_err_code == RAW_DISK_ERR_ERASE_FAIL) {
        return HAL_FLASH_STATUS_ERROR_ERASE_FAIL;
    } else if (ori_err_code == HAL_FLASH_STATUS_ERROR_LOCKED) { //mutex lock
        return HAL_FLASH_STATUS_ERROR_ERASE_FAIL;
    } else {
        ASSERT_RET(0, HAL_FLASH_STATUS_ERROR);
    }
}
hal_flash_block_t get_block_size_from_address(uint32_t address)
{
    uint32_t block_size;
    uint32_t block_index;

    block_index = NOR_BlockIndex(address);
    if (block_index == INVALID_BLOCK_INDEX) {
        ASSERT_RET(0, (hal_flash_block_t)(-1));
    }
    block_size = NOR_BlockSize(block_index);
    if (block_size == 0x1000) {
        return HAL_FLASH_BLOCK_4K;
    } else if (block_size == 0x8000) {
        return HAL_FLASH_BLOCK_32K;
    } else if (block_size == 0x10000) {
        return HAL_FLASH_BLOCK_64K;
    } else {
        ASSERT_RET(0, (hal_flash_block_t)(-1));
    }
}

//BL and FreeRTOS always do flash init
hal_flash_status_t hal_flash_init(void)
{
    NOR_init();
    return HAL_FLASH_STATUS_OK;
}
//Do nothing for flash deinit
hal_flash_status_t hal_flash_deinit(void)
{
    return HAL_FLASH_STATUS_OK;
}


static int32_t flash_sw_protected_check(uint32_t address)
{
    uint32_t index,i;
    int32_t result = 0;


#if (LABTEST)
        return result;
#else
    //define the sotfware protect region
    flash_sw_protect_area_config_t region_cfg_tbl[] = {
        {0x00000000, 0x9000}   //protect the partition table and bootloader area
    };

    index = (sizeof(region_cfg_tbl) / sizeof(region_cfg_tbl[0]));
    for(i = 0; i < index; i++) {
<<<<<<< HEAD
        if (address < region_cfg_tbl[i].length) {
            result |= -1;
=======
        if ((address >= region_cfg_tbl[i].address) && (address < ( region_cfg_tbl[i].address + region_cfg_tbl[i].length))) {
            result = 1;
>>>>>>> db20e11 (second commit)
        }
    }
    return result;
#endif
}

//Notice: please call get_block_size_from_address() first to get block size
hal_flash_status_t hal_flash_erase(uint32_t start_address, hal_flash_block_t block)
{
    int32_t result;
    uint32_t block_index;
    uint32_t erase_type = 0;
    int32_t address;
    int32_t length;

    if (0 != flash_sw_protected_check(start_address & 0x00FFFFFF)){
        #if defined(__EXT_BOOTLOADER__)
        bl_printf("[Error] address: 0x%x , LR: 0x%x \r\n", start_address, (uint32_t)__builtin_return_address(0));
        hal_wdt_software_reset();
        #else
        LOG_MSGID_E(common, "[Error] address: 0x%x , LR: 0x%x \r\n", 2, start_address, (uint32_t)__builtin_return_address(0));
        assert(0);
        #endif
    }

    if (HAL_FLASH_BLOCK_4K == block) {
        length = 0x1000;
        if ((start_address & 0xFFF) != 0) {
            return HAL_FLASH_STATUS_ERROR_WRONG_ADDRESS;
        }
        erase_type = 0x1;
    } else if (HAL_FLASH_BLOCK_32K == block) {
        length = 0x8000;
        if ((start_address & 0x7FFF) != 0) {
            return HAL_FLASH_STATUS_ERROR_WRONG_ADDRESS;
        }
        erase_type = 0x2;
    } else if (HAL_FLASH_BLOCK_64K == block) {
        length = 0x10000;
        if ((start_address & 0xFFFF) != 0) {
            return HAL_FLASH_STATUS_ERROR_WRONG_ADDRESS;
        }
        erase_type = 0x4;
    } else {
        return HAL_FLASH_STATUS_ERROR_WRONG_ADDRESS;
    }

    start_address = start_address & (~HAL_FLASH_BASE_ADDRESS);
    block_index = NOR_BlockIndex(start_address);
    if (block_index == INVALID_BLOCK_INDEX) {
        return HAL_FLASH_STATUS_ERROR_WRONG_ADDRESS;
    }

    block_index |= (erase_type << 28);
    result = eraseRawDiskBlock(ENTIRE_DISK_DRIVER_DATA, block_index);
    if (result != RAW_DISK_ERR_NONE) {
        return get_rawdisk_error_code(result);
    }

#ifdef HAL_CACHE_MODULE_ENABLED
    address = start_address | HAL_FLASH_BASE_ADDRESS;

    /* Address should be alignment with cashe line size*/
    if (hal_cache_is_cacheable(address)) {
        uint32_t addr;
        for (addr = (address & CACHE_LINE_ALIGNMENT_MASK); addr <= ((address + length + CACHE_LINE_SIZE) & CACHE_LINE_ALIGNMENT_MASK); addr += CACHE_LINE_SIZE) {
            hal_cache_invalidate_one_cache_line(addr);
        }
    }
#endif
    return HAL_FLASH_STATUS_OK;
}

hal_flash_status_t hal_flash_read(uint32_t start_address, uint8_t *buffer, uint32_t length)
{
    NOR_FLASH_DISK_Data *D = ENTIRE_DISK_DRIVER_DATA;
    int32_t result;

    start_address = start_address & (~HAL_FLASH_BASE_ADDRESS);
    result = readRawDiskData(D, start_address, buffer, length);

    if (result != RAW_DISK_ERR_NONE) {
        return get_rawdisk_error_code(result);
    }
    return HAL_FLASH_STATUS_OK;
}

hal_flash_status_t hal_flash_write(uint32_t address, const uint8_t *data, uint32_t length)
{
    NOR_FLASH_DISK_Data *D = ENTIRE_DISK_DRIVER_DATA;
    int32_t result;

    if (0 != flash_sw_protected_check(address & 0x00FFFFFF)){
        #if defined(__EXT_BOOTLOADER__)
        bl_printf("[Error] address: 0x%x , LR: 0x%x \r\n", address, (uint32_t)__builtin_return_address(0));
        hal_wdt_software_reset();
        #else
        LOG_MSGID_E(common, "[Error] address: 0x%x , LR: 0x%x \r\n", 2, address, (uint32_t)__builtin_return_address(0));
        assert(0);
        #endif
    }

    address = address & (~HAL_FLASH_BASE_ADDRESS);
    result = writeRawDiskData(D, address, (uint8_t *)data, length);
    if (result != RAW_DISK_ERR_NONE) {
        return get_rawdisk_error_code(result);
    }

    #ifdef HAL_CACHE_MODULE_ENABLED
    address |= HAL_FLASH_BASE_ADDRESS;
    /* Address should be alignment with cashe line size*/
    if(hal_cache_is_cacheable(address)){
       uint32_t addr;
       for(addr=(address&CACHE_LINE_ALIGNMENT_MASK); addr<=((address+length+CACHE_LINE_SIZE) & CACHE_LINE_ALIGNMENT_MASK); addr+=CACHE_LINE_SIZE) {
            hal_cache_invalidate_one_cache_line(addr);
       }
    }
    #endif

    return HAL_FLASH_STATUS_OK;
}

/* should be called after hal_flash_init  */
hal_flash_status_t hal_flash_get_disk_size(uint32_t* size)
{
    *size = EntireDiskDriveData.DiskSize;
    if (*size == 0) {
        return HAL_FLASH_STATUS_ERROR;
    } else {
        return HAL_FLASH_STATUS_OK;
    }
}

#ifdef __SECURITY_OTP__
#ifdef __SECURITY_OTP_WRITE__

int32_t OTP_Write_Data(NOR_FLASH_DISK_Data *D, uint32_t otp_addr, uint8_t *data_ptr, uint32_t len)
{
    int32_t result = FLASH_DISK_DONE;

    if (otp_addr >= ((SF_MTD_Data *)D->MTDData)->OTPLength) {
        return RAW_DISK_ERR_WRONG_ADDRESS;
    }

    if ((otp_addr + len) > (((SF_MTD_Data *)D->MTDData)->OTPLength + ((SF_MTD_Data *)D->MTDData)->OTPBase)) {
        return RAW_DISK_ERR_NO_SPACE;
    }

    if (!D->is_mount) {
        MountDevice(D);
    }

    if (D->MTDDriver->OTPAccess) {
        result = D->MTDDriver->OTPAccess(D->MTDData, FS_OTP_WRITE, (uint32_t)otp_addr, (void *)data_ptr, len);
    }

    if (result != FLASH_DISK_DONE) {
        return RAW_DISK_ERR_PROG_FAIL;
    }
    return RAW_DISK_ERR_NONE;

}
#endif

int32_t OTP_Read_Data(NOR_FLASH_DISK_Data *D, uint32_t otp_addr, uint8_t *data_ptr, uint32_t len)
{
    int32_t result = FLASH_DISK_DONE;

    if (otp_addr >= ((SF_MTD_Data *)D->MTDData)->OTPLength) {
        return RAW_DISK_ERR_WRONG_ADDRESS;
    }

    if ((otp_addr + len) > (((SF_MTD_Data *)D->MTDData)->OTPLength + ((SF_MTD_Data *)D->MTDData)->OTPBase)) {
        return RAW_DISK_ERR_NO_SPACE;
    }

    if (!D->is_mount) {
        MountDevice(D);
    }

    if (D->MTDDriver->OTPAccess) {
        result = D->MTDDriver->OTPAccess(D->MTDData, FS_OTP_READ, (uint32_t)otp_addr, (void *)data_ptr, len);
    }

    if (result != FLASH_DISK_DONE) {
        return RAW_DISK_ERR_PROG_FAIL;
    }
    return RAW_DISK_ERR_NONE;

}

#ifdef __SECURITY_OTP_WRITE__

int32_t OTP_Lock(NOR_FLASH_DISK_Data *D)
{
    int32_t result = FLASH_DISK_DONE;

    if (!D->is_mount) {
        MountDevice(D);
    }

    if (D->MTDDriver->OTPAccess) {
        result = D->MTDDriver->OTPAccess(D->MTDData, FS_OTP_LOCK, 0, NULL, 0);
    }

    if (result != FLASH_DISK_DONE) {
        return RAW_DISK_ERR_PROG_FAIL;
    }
    return RAW_DISK_ERR_NONE;

}


hal_flash_status_t hal_flash_otp_write(uint32_t address, const uint8_t *data, uint32_t length)
{
    NOR_FLASH_DISK_Data *D = ENTIRE_DISK_DRIVER_DATA;
    int32_t result;

    result = OTP_Write_Data(D, address, (uint8_t *)data, length);
    if (result != RAW_DISK_ERR_NONE) {
        return get_rawdisk_error_code(result);
    }
    return HAL_FLASH_STATUS_OK;
}
#endif

hal_flash_status_t hal_flash_otp_read(uint32_t start_address, uint8_t *buffer, uint32_t length)
{
    NOR_FLASH_DISK_Data *D = ENTIRE_DISK_DRIVER_DATA;
    int32_t result;

    result = OTP_Read_Data(D, start_address, buffer, length);
    if (result != RAW_DISK_ERR_NONE) {
        return get_rawdisk_error_code(result);
    }
    return HAL_FLASH_STATUS_OK;
}

#ifdef __SECURITY_OTP_WRITE__
hal_flash_status_t hal_flash_otp_lock(void)
{
    NOR_FLASH_DISK_Data *D = ENTIRE_DISK_DRIVER_DATA;
    int32_t result;

    result = OTP_Lock(D);
    if (result != RAW_DISK_ERR_NONE) {
        return get_rawdisk_error_code(result);
    }
    return HAL_FLASH_STATUS_OK;
}
#endif
#endif

#else //! HAL_FLASH_MODULE_ENABLED
signed int NOR_init(void)
{
    return 0;
}
#endif //#ifdef HAL_FLASH_MODULE_ENABLED

