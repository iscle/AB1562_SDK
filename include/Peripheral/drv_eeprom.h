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

#ifndef _DRV_EEPROM_H_
#define _DRV_EEPROM_H_

/**
 * @addtogroup Peripheral
 * @{
 * This section introduces the Peripheral APIs including terms and acronyms,
 * supported features, software architecture, details on how to use the Message, enums, structures, typedefs and functions.
 *
 */

/*! 
     
     @defgroup PAGE_API_DRIVER_EEPROM drv_eeprom.h
     @{
        @page PAGE_API_DRIVER_EEPROM drv_eeprom.h 
        Drv_eeprom.h is a driver for EEPROM.
        
*/

#if (FEA_SUPP_EEPROM)

#include "config.h"

////////////////////////////////////////////////////////////////////////////////
// Constant Definitions ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef U16 EEPROM_ADDR;

#define SMALL_EEPROM_SELECTED                   1
#define LARGE_EEPROM_SELECTED                   2

#define EEPROM_HEADER_SECTOR                    0xFF

//for large/small EEPROM autodetect;
#define EEPROM_RESERVE_BYTES                    4

typedef U8 EEPROM_ERRCODE;
#define EEPROM_ERR_NO_ERR                       0x00
#define EEPROM_ERR_I2C_NO_ACK                   0x01
#define EEPROM_ERR_CRC_ERROR                    0x02
#define EEPROM_ERR_HEADER_CRC_ERROR             0x03
#define EEPROM_ERR_CANNOT_LOCATE_HEADER         0x04
#define EEPROM_ERR_DET_ERROR                    0x05
#define EEPROM_ERR_NO_INSIDE                    0x06
#define EEPROM_ERR_CMD_ERROR                    0x07

#define EEPROM_ERR_CRC16_ERROR                  0x10



#define EEPROM_CODEPATCH_ERROR                  0x11

#define EEPROM_EMPTY_SECTOR_TYPE                0x01

//speed mode misc option
#define SUPPORT_NOT_SYNC_CLK                    0x20
#define SUPPORT_FAST_READ_BYTES                 0x40
#define TOOL_SPI_FLASH_IS_USED                  0x80	// used by tool to determine if SPIFLASH is used


#define EEPROM_CRC16_Init                       0xffff  //crc16 linit value


#ifdef EEPROM_M24C16
//M24C16
#define PAGE_SIZE                               16
#define SIZE_MASK                               0x07
#define FAST_400K                               1

#else
//AT24C128
#define PAGE_SIZE                               32
#define SIZE_MASK                               0x3F
#define FAST_400K                               1
#endif



#define I2C_SPEED_USER_DEFINE                   3

#if SIZE_MASK > 0x07
#define LARGE_EEPROM 	                          1            //EEPROM address two bytes
#else
#define LARGE_EEPROM	                          0            //EEPROM address one byte
#endif


#define No_EEPROM_Inside                       (U8)0x00
#define EEPROM_Inside                          (U8)0x01

#define Read_AFE_Sector                        (U8)0x20
#define Read_CSR_Sector                        (U8)0x40
#define Read_All_Sector_OK                     (U8)0x80

#define EEPROM_Large                           (U8)0x02
#define EEPROM_Small                           (U8)0x00

#define NonResMem_ERR                          (U8)0x80

#define GPIO_EEPROM_WP                         (U8)0x01

////////////////////////////////////////////////////////////////////////////////
// Macro ///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


#define I2C_PINSDA                             (U8)0x06
#define HIGH                                   (U8)0x01
#define LOW                                    (U8)0x00

////////////////////////////////////////////////////////////////////////////////
// Type Definitions - Message Formats //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct
{
    U8                          setlen;            //EEPROM set length
    U8                          page_size;         //  in bytes
    U8                          eeprom_size_mask;  //
    U8                          eeprom_speed_mode;
} EEPROM_SETTING;



typedef struct
{
	U8 							header_loaded;
	U8							current_sector;
	U16							addr;
	U8 							no_polling_read_after_write;
	U8 							current_eeprom_speed_mode;
	U8 							misc_option;
	U8							large_eeprom_used;
	U8 							auto_retry;
	EEPROM_SETTING				eeprom_setting;
	//Do not insert any data here;
	//ADDR2_LEN2_TYPE 			addr_length_group[NUM_OF_SECTORS];
} EEPROM_CTL_TYPE;



typedef struct
{
	U8                          gEEPROM_Inside;
	U8                          gEEPROM_wp_mask;
	U8							large_eeprom_used;
	U8 							auto_retry;
	EEPROM_SETTING				eeprom_setting;
} EEPROM_PARA_STRU;


////////////////////////////////////////////////////////////////////////////////


/******************************************************************************
 * Function prototypes
 ******************************************************************************/
/**
 * DRV_EEPROM_I2C_SetCtrlAddr
 *
 * Setting I2C Memory block read/writel address
 *
 * @Author : Yugo <YugoSu@airoha.com.tw>
 *
 * @addr : EEPROM address
 */
EXTERN void DRV_EEPROM_I2C_SetCtrlAddr(
     EEPROM_ADDR addr);


/**
 * DRV_EEPROM_I2C_PageWrite
 *
 * Write a page data to EEPROM
 * Note: have write between two page issue
 *
 * @Author : Yugo <YugoSu@airoha.com.tw>
 *
 * @addr : EEPROM address
 * @content_ptr : the pointer of content data
 * @content_size : the size of content data
 * @Return :status of  I2C Error
 */
EXTERN U8 DRV_EEPROM_I2C_PageWrite(
    EEPROM_ADDR addr,
    U8 *content_ptr,
    U16 content_size);


/**
 * DRV_EEPROM_I2C_WriteBytes
 *
 * Write data to EEPROM by I2C  (content size > page size)
 *
 * @Author : Yugo <YugoSu@airoha.com.tw>
 *
 * @addr : EEPROM address
 * @content_ptr : the pointer of content data
 * @content_size : the size of content
 * @Return :status of  I2C Error
 */
EXTERN U8 DRV_EEPROM_I2C_WriteBytes(
    EEPROM_ADDR addr,
    U8 *content_ptr,
    U16 content_size);


/**
 * DRV_EEPROM_I2C_RandomReadBytes_Set
 *
 * Set random read  byte  EEPROM address (read first cycle)
 *
 * @Author : Yugo <YugoSu@airoha.com.tw>
 *
 * @addr : read EEPROM address
 * @Return : EEPROM ID
 */
EXTERN U8 DRV_EEPROM_I2C_RandomReadBytes_Set(
    EEPROM_ADDR addr);


/**
 * DRV_EEPROM_I2C_RandomReadBytes_Act
 *
 * Send EEPROM ID address and recevic data (read second cycle)
 *
 * @Author : Yugo <YugoSu@airoha.com.tw>
 *
 * @device_sel : EEPROM device ID
 * @size : Size of receivc data
 * @Return :status of  I2C Error
 */
EXTERN 	U8 DRV_EEPROM_I2C_RandomReadBytes_Act(
	U8 device_sel,
	U16 size,
	U8 *sram);


/**
 * DRV_EEPROM_I2C_RandomReadBytes_Act_Fast
 *
 * Send EEPROM ID address and recevic data (read second cycle)
 *
 * @Author : Yugo <YugoSu@airoha.com.tw>
 *
 * @device_sel : EEPROM device ID
 * @size : Size of receivc data
 * @in_ptr : pointer of receivc data (address)
 * @Return :status of  I2C Error
 */
EXTERN U8 DRV_EEPROM_I2C_RandomReadBytes_Act_Fast(
    U8 device_sel,
    U16 size,
    U8 *in_ptr);


/**
 * DRV_EEPROM_I2C_RandomReadByte
 *
 * Random read one byte from EEPROM
 *
 * @Author : Yugo <YugoSu@airoha.com.tw>
 *
 * @addr : EEPROM address
 * @err_ptr : status of  I2C Error
 * @Return :one byte at  read buffer contents[0]
 */
EXTERN U8 DRV_EEPROM_I2C_RandomReadByte(
    EEPROM_ADDR addr,
    U8 *err_ptr,
    U8 *sram);


/**
 * DRV_EEPROM_I2C_ReadBytes
 *
 * Read  Large amounts of data from EEPROM
 *
 * @Author : Yugo <YugoSu@airoha.com.tw>
 *
 * @addr : EEPROM address
 * @content_ptr : Pointer of content
 * @content_size : Length of receivc data
 * @Return :status of  EEPROM_ERR_NO_ERR
 */

EXTERN U8 DRV_EEPROM_I2C_ReadBytes(
    EEPROM_ADDR addr,
    U8 *content_ptr,
    U16 content_size );



/**
 * DRV_EEPROM_ReadBytes
 *
 * Read bytes from EEPROM
 *
 * @Author : Yugo <YugoSu@airoha.com.tw>
 *
 * @sram_addr : Sram address
 * @length : length of read bytes
 * @Return : status of  EEPROM_ERR_NO_ERR
 */
EXTERN U8 DRV_EEPROM_ReadBytes(
    U8 *sram_addr,
    U16 EEPROM_addr,
    U16 length);


/**
 * DRV_EEPROM_WriteBytes
 *
 * Write EEPROM and increase EEPROM address
 *
 * @Author : Yugo <YugoSu@airoha.com.tw>
 *
 * @sram_addr : Address with SRAM
 * @length : read EEPROM length
 * @Return : status of EEPROM error
 *
 */
EXTERN U8 DRV_EEPROM_WriteBytes(
    U8 *sram_addr,
    U16 EEPROM_addr,
    U16 length);


#endif /* FEA_SUPP_EEPROM */

/*!
    @}
*/

/*!
    @}
*/

#endif //_EEPROM_H

