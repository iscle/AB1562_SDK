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

#ifndef _DRV_SPIFLH_H_
#define _DRV_SPIFLH_H_

/**
 * @addtogroup Peripheral
 * @{
 * This section introduces the Peripheral APIs including terms and acronyms,
 * supported features, software architecture, details on how to use the Message, enums, structures, typedefs and functions.
 *
 */

/*! 
     
     @defgroup PAGE_API_DRIVER_SPI_FLASH drv_spiflash.h
     @{
        @page PAGE_API_DRIVER_SPI_FLASH drv_spiflash.h 
        Drv_spiflash.h is a driver for SPI flash.
        
*/


////////////////////////////////////////////////////////////////////////////////
// Constant Definitions ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//===============command===================
#define SPIFLH_WRITE_ENABLE 		0x06
#define SPIFLH_WRITE_DISABLE 		0x04
#define SPIFLH_READ_ID 				0x9F
#define SPIFLH_READ_STATUS_REG		0x05
#define SPIFLH_WRITE_STATUS_REG		0x01
#define SPIFLH_READ_DATA 			0x03
#define SPIFLH_2IO_READ_DATA 		0xBB
#define SPIFLH_DUAL_READ_DATA 		0x3B
#define SPIFLH_4IO_READ_DATA 		0x70
#define SPIFLH_QUAD_READ_DATA 		0xEB
#define SPIFLH_PAGE_PROGRAM 		0x02
#define SPIFLH_4IO_PAGE_PROGRAM 	0x32
#define SPIFLH_QUAD_PAGE_PROGRAM 	0x38
#define SPIFLH_READ_SCU_REG			0x2B
#define SPIFLH_WRITE_SCU_REG		0x2F
#define SPIFLH_GANG_BLOCK_LOCK		0x7E
#define SPIFLH_GANG_BLOCK_UNLOCK	0x98
#define SPIFLH_SPI_SECTOR_ERASE 	0x20
#define SPIFLH_SPI_CHIP_ERASE 		0x60
#define SPIFLH_READ_STATUS2_REG		0x35
#define SPIFLH_READ_STATUS3_REG		0x15
#define SPIFLH_WRITE_STATUS2_REG	0x31
#define SPIFLH_WRITE_STATUS3_REG	0x11

// ===============status=====================
#define SPIFLH_ERR_NO_ERR					0x00
#define SPIFLH_ERR_PROG_ERR					0x01
#define SPIFLH_ERR_ERASE_ERR				0x02
#define SPIFLH_ERR_CANNOT_LOCATE_HEADER		0x03
#define SPIFLH_ERR_CRC_ERR					0x04
#define SPIFLH_ERR_NULL_DST_ADDR            0x05
//========================================
#define SPIFLH_STATUS_WIP_BIT	0x01
#define SPIFLH_STATUS_WEL_BIT	0x02
//========================================
#define SPIFLH_SCU_CP_MODE		0x10
#define SPIFLH_SCU_P_FAIL		0x20
#define SPIFLH_SCU_E_FAIL		0x40
#define SPIFLH_SCU_WPSEL		0x80

//=================buffer size===============
#define SPI_LARGE_BUFFER_SIZE 40
#define SPI_SMALL_BUFFER_SIZE 5

#define SPIFLH_PAGE_SIZE (SPI_LARGE_BUFFER_SIZE-8)

#define NEWSPIFLH_PAGE_SIZE    256



/* Manufacturer ID */
#define SPIFLH_MANUFACTURER_MXIC         (U8)0xC2
#define SPIFLH_MANUFACTURER_WB           (U8)0xEF
#define SPIFLH_MANUFACTURER_GD           (U8)0xC8
#define SPIFLH_MANUFACTURER_MD			 (U8)0x51
#define SPIFLH_MANUFACTURER_FM           (U8)0xA1

/* Memory Type */
#define SPIFLH_MEMORYTYPE_MX25U          (U8)0x25
#define SPIFLH_MEMORYTYPE_MX25R          (U8)0x28
#define SPIFLH_MEMORYTYPE_MX25L          (U8)0x20
#define SPIFLH_MEMORYTYPE_W25Q           (U8)0x50
#define SPIFLH_MEMORYTYPE_GD25LQ         (U8)0x60
#define SPIFLH_MEMORYTYPE_GD25Q          (U8)0x40
#define SPIFLH_MEMORYTYPE_MD25D          (U8)0x40
#define SPIFLH_MEMORYTYPE_FM25Q          (U8)0x40

/* Memory Identify */
/* MX25U Series */
#define SPIFLH_MEMORYTYPE2_MX25U4033E    (U8)0x33
#define SPIFLH_MEMORYTYPE2_MX25U8033E    (U8)0x34
#define SPIFLH_MEMORYTYPE2_MX25U1635E    (U8)0x35
/* MX25R Series */
#define SPIFLH_MEMORYTYPE2_MX25R4035F    (U8)0x13
#define SPIFLH_MEMORYTYPE2_MX25R8035F    (U8)0x14
#define SPIFLH_MEMORYTYPE2_MX25R1635F    (U8)0x15
#define SPIFLH_MEMORYTYPE2_MX25R3235F    (U8)0x16
/* MX25L Series */
#define SPIFLH_MEMORYTYPE2_MX25L8035E    (U8)0x14
/* M25Q Series */
#define SPIFLH_MEMORYTYPE2_W25Q40BW      (U8)0x13
#define SPIFLH_MEMORYTYPE2_W25Q80BW      (U8)0x14
/* GD25LQ Series */
#define SPIFLH_MEMORYTYPE2_GD25LQ40      (U8)0x13
#define SPIFLH_MEMORYTYPE2_GD25LQ80      (U8)0x14
#define SPIFLH_MEMORYTYPE2_GD25LQ16      (U8)0x15
/* GD25Q Series */
#define SPIFLH_MEMORYTYPE2_GD25Q80C      (U8)0x14
#define SPIFLH_MEMORYTYPE2_GD25Q32C      (U8)0x16
#define SPIFLH_MEMORYTYPE2_GD25Q64C      (U8)0x17
#define SPIFLH_MEMORYTYPE2_GD25Q128C     (U8)0x18
/* MD25D Series */
#define SPIFLH_MEMORYTYPE2_MD25D40		 (U8)0x13
#define SPIFLH_MEMORYTYPE2_MD25D80       (U8)0x14
/* FM25Q Series */
#define SPIFLH_MEMORYTYPE2_FM25Q08A      (U8)0x14

#define SPIFLH_COMBINED_INFO(ManuID,MemType,MemDensity) \
    (((U32)(ManuID))|((U32)(MemType)<<8)|((U32)(MemDensity <<16)))


/* MX25U Series */
#define SPIFLH_MXIC_MX25U4033E           \
    SPIFLH_COMBINED_INFO(                \
        SPIFLH_MANUFACTURER_MXIC,        \
        SPIFLH_MEMORYTYPE_MX25U,         \
        SPIFLH_MEMORYTYPE2_MX25U4033E)
#define SPIFLH_MXIC_MX25U8033E           \
    SPIFLH_COMBINED_INFO(                \
        SPIFLH_MANUFACTURER_MXIC,        \
        SPIFLH_MEMORYTYPE_MX25U,         \
        SPIFLH_MEMORYTYPE2_MX25U8033E)
#define SPIFLH_MXIC_MX25U1635E           \
    SPIFLH_COMBINED_INFO(                \
        SPIFLH_MANUFACTURER_MXIC,        \
        SPIFLH_MEMORYTYPE_MX25U,         \
        SPIFLH_MEMORYTYPE2_MX25U1635E)

/* MX25R Series */
#define SPIFLH_MXIC_MX25R4035F           \
    SPIFLH_COMBINED_INFO(                \
        SPIFLH_MANUFACTURER_MXIC,        \
        SPIFLH_MEMORYTYPE_MX25R,         \
        SPIFLH_MEMORYTYPE2_MX25R4035F)
#define SPIFLH_MXIC_MX25R8035F           \
    SPIFLH_COMBINED_INFO(                \
        SPIFLH_MANUFACTURER_MXIC,        \
        SPIFLH_MEMORYTYPE_MX25R,         \
        SPIFLH_MEMORYTYPE2_MX25R8035F)
#define SPIFLH_MXIC_MX25R1635F           \
    SPIFLH_COMBINED_INFO(                \
        SPIFLH_MANUFACTURER_MXIC,        \
        SPIFLH_MEMORYTYPE_MX25R,         \
        SPIFLH_MEMORYTYPE2_MX25R1635F)
#define SPIFLH_MXIC_MX25R3235F           \
    SPIFLH_COMBINED_INFO(                \
        SPIFLH_MANUFACTURER_MXIC,        \
        SPIFLH_MEMORYTYPE_MX25R,         \
        SPIFLH_MEMORYTYPE2_MX25R3235F)

/* MX25L Series */
#define SPIFLH_MXIC_MX25L8035E           \
    SPIFLH_COMBINED_INFO(                \
        SPIFLH_MANUFACTURER_MXIC,        \
        SPIFLH_MEMORYTYPE_MX25L,         \
        SPIFLH_MEMORYTYPE2_MX25L8035E)

/* W25Q Series */
#define SPIFLH_WB_W25Q40BW               \
    SPIFLH_COMBINED_INFO(                \
        SPIFLH_MANUFACTURER_WB,          \
        SPIFLH_MEMORYTYPE_W25Q,          \
        SPIFLH_MEMORYTYPE2_W25Q40BW)
#define SPIFLH_WB_W25Q80BW               \
    SPIFLH_COMBINED_INFO(                \
        SPIFLH_MANUFACTURER_WB,          \
        SPIFLH_MEMORYTYPE_W25Q,          \
        SPIFLH_MEMORYTYPE2_W25Q80BW)

/* GD25LQ Series */
#define SPIFLH_GD_GD25LQ40               \
    SPIFLH_COMBINED_INFO(                \
        SPIFLH_MANUFACTURER_GD,          \
        SPIFLH_MEMORYTYPE_GD25LQ,        \
        SPIFLH_MEMORYTYPE2_GD25LQ40)
#define SPIFLH_GD_GD25LQ80               \
    SPIFLH_COMBINED_INFO(                \
        SPIFLH_MANUFACTURER_GD,          \
        SPIFLH_MEMORYTYPE_GD25LQ,        \
        SPIFLH_MEMORYTYPE2_GD25LQ80)
#define SPIFLH_GD_GD25LQ16               \
    SPIFLH_COMBINED_INFO(                \
        SPIFLH_MANUFACTURER_GD,          \
        SPIFLH_MEMORYTYPE_GD25LQ,        \
        SPIFLH_MEMORYTYPE2_GD25LQ16)

/* GD25Q Series */
#define SPIFLH_GD_GD25Q80C               \
    SPIFLH_COMBINED_INFO(                \
        SPIFLH_MANUFACTURER_GD,          \
        SPIFLH_MEMORYTYPE_GD25Q,         \
        SPIFLH_MEMORYTYPE2_GD25Q80C)
#define SPIFLH_GD_GD25Q32C               \
    SPIFLH_COMBINED_INFO(                \
        SPIFLH_MANUFACTURER_GD,          \
        SPIFLH_MEMORYTYPE_GD25Q,         \
        SPIFLH_MEMORYTYPE2_GD25Q32C)
#define SPIFLH_GD_GD25Q64C               \
    SPIFLH_COMBINED_INFO(                \
        SPIFLH_MANUFACTURER_GD,          \
        SPIFLH_MEMORYTYPE_GD25Q,         \
        SPIFLH_MEMORYTYPE2_GD25Q64C)
#define SPIFLH_GD_GD25Q128C              \
    SPIFLH_COMBINED_INFO(                \
        SPIFLH_MANUFACTURER_GD,          \
        SPIFLH_MEMORYTYPE_GD25Q,         \
        SPIFLH_MEMORYTYPE2_GD25Q128C)

/* MD25D Series */
#define SPIFLH_MD_MD25D40                \
    SPIFLH_COMBINED_INFO(                \
        SPIFLH_MANUFACTURER_MD,          \
        SPIFLH_MEMORYTYPE_MD25D,         \
        SPIFLH_MEMORYTYPE2_MD25D40)
#define SPIFLH_MD_MD25D80                \
    SPIFLH_COMBINED_INFO(                \
        SPIFLH_MANUFACTURER_MD,          \
        SPIFLH_MEMORYTYPE_MD25D,         \
        SPIFLH_MEMORYTYPE2_MD25D80)

/* FM25Q Series */
#define SPIFLH_FM_FM25Q08A               \
    SPIFLH_COMBINED_INFO(                \
        SPIFLH_MANUFACTURER_FM,          \
        SPIFLH_MEMORYTYPE_FM25Q,         \
        SPIFLH_MEMORYTYPE2_FM25Q08A)


/* External Flash Size */
#define SPIFLH_SIZE_4M_BIT       (U8)4
#define SPIFLH_SIZE_8M_BIT       (U8)8
#define SPIFLH_SIZE_16M_BIT      (U8)16
#define SPIFLH_SIZE_32M_BIT      (U8)32
#define SPIFLH_SIZE_64M_BIT      (U8)64
#define SPIFLH_SIZE_128M_BIT     (U8)128


////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


typedef struct stru_spiflh_data_buffer_large
{
	U8 contents[SPI_LARGE_BUFFER_SIZE];
} SPIFLH_DATA_BUFFER_LARGE_STRU;

typedef struct stru_spiflh_data_buffer_small
{
	U8 contents[SPI_SMALL_BUFFER_SIZE];
} SPIFLH_DATA_BUFFER_SMALL_STRU;

typedef struct stru_spiflh_control
{
    SPIFLH_DATA_BUFFER_LARGE_STRU spiflhLargeBuf;
    SPIFLH_DATA_BUFFER_SMALL_STRU spiflhSmallBuf;
} SPIFLH_CONTROL_STRU;


////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN SPIFLH_CONTROL_STRU gSPIFLHControl;


////////////////////////////////////////////////////////////////////////////////
// Constant Definitions ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Function Declarations ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN U32 DRV_SPIFLH_Initial(VOID);
EXTERN U8   DRV_SPIFLH_Protection (U8 protect_bit);
EXTERN U8   DRV_SPIFLH_ChipErase (VOID);
EXTERN U8   DRV_SPIFLH_SectorErase (U32 addr);
EXTERN U8   DRV_SPIFLH_Block_32KB_Erase (U32 addr);
EXTERN U8   DRV_SPIFLH_Block_64KB_Erase (U32 addr);
EXTERN U8   DRV_SPIFLH_PageProgram (U32 flash_addr, U8 *content_ptr, U8 *crc_ptr);
EXTERN U8   DRV_SPIFLH_PageRead (U32 flash_addr, U8 *content_ptr);
EXTERN U8   DRV_SPIFLH_ByteProgram(U32 flash_addr,U8 * content_ptr, U16 length);	
EXTERN U8   DRV_SPIFLH_ByteRead(U32 flash_addr, U8 *content_ptr, U16 length);
EXTERN U8   DRV_SPIFLH_GetFlashSize(VOID);
EXTERN U8   DRV_SPIFLH_GetLockBitUnLock(VOID);


//EXTERN BOOL DRV_SPIFLH_IsExtFlhExist(VOID);
EXTERN U8   DRV_SPIFLH_ReadFlashID(U8 *content_ptr);
EXTERN U8   DRV_SPIFLH_ReadRegister (U8 cmd);
EXTERN U8 DRV_SPIFLH_WriteRegister (U8 cmd, U8 value);
EXTERN U8 DRV_SPIFLH_WriteSingleByteCmd (U8 cmd);
EXTERN U8 DRV_SPIFLH_ChipUnlock (VOID);
EXTERN U8 DRV_SPIFLH_ChipLock (VOID);
EXTERN U8 DRV_SPIFLH_PageReadBytes(U32 flash_addr, U8 *content_ptr, U16 Read_count );


EXTERN U8 DRV_SPIFLH_PageWriteVerify(U32 flash_addr, U8*content_ptr, U8 CRC8);

EXTERN VOID DRV_SPIFLH_ChipLock_M25R (VOID);


/*!
    @}
*/

/*!
    @}
*/

#endif /* _DRV_SPIFLH_H_ */
