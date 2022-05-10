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
#include "memory_attribute.h"
#include "hal_flash_custom_memorydevice.h"
#include "ab1562.h"
#include "hal_flash_sf.h"
#include "hal_flash_opt.h"
#include "hal_flash_combo_init.h"
#include "hal_flash_combo_sfi_defs.h"
#include "hal_flash_custom_sfi.h"


/*************************************************************************
* FUNCTION
*  custom_setSFI()
*
* DESCRIPTION
*   This routine aims to set SFI
*
* PARAMETERS
*
* RETURNS
*  None
*
* GLOBALS AFFECTED
*
*************************************************************************/

//-----------------------------------------------------------------------------
// MCP Serial Flash EMI/SFI settings
//-----------------------------------------------------------------------------

// include type defines (to be modified by EMI/SFI owner)
ATTR_RODATA_IN_RAM

const CMEMEntrySFIList combo_mem_hw_list = {   // (to be renamed by SFI owner)
    "COMBO_MEM_SFI",
    COMBO_SFI_VER,           // SFI structure version
    SFI_COMBO_COUNT,   // defined in custom_Memorydevice.h
    {

        {
<<<<<<< HEAD
            // MTKSIP_2523_SF_32_01
=======
            // SIP XM25QU32C
>>>>>>> db20e11 (second commit)
            {
                // HW config 78Mhz Start
                0x00010000,  // SFI_MAC_CTL
                0xEB0B5770,  // SFI_DIRECT_CTL
<<<<<<< HEAD
                0x52F80000,     //  SFI_MISC_CTL
                0x00000001,     //  SFI_MISC_CTL2
                0x0,     // 1st SFI_DLY_CTL_2
                0x18000018,         // 1st SFI_DLY_CTL_3
=======
                0x52F80310,     //  SFI_MISC_CTL
                0x00000000,     //  SFI_MISC_CTL2
                0x0,     // 1st SFI_DLY_CTL_2
                0x06000006,         // 1st SFI_DLY_CTL_3
>>>>>>> db20e11 (second commit)
                0x00040004,           // DRIVING
                0,                  // Reserved
                0,  // 2nd SFI_DLY_CTL_4
                0  // 2nd SFI_DLY_CTL_5
            },  // HW config End
            {
<<<<<<< HEAD
                SPI, 1, 0x06, SPI, 2, 0x01, 0x42, SPI,
                2, 0x05, 0x01, SPI, 2, 0xC0, 0x02, SF_UNDEF,
=======
                SPI, 1, 0x50, SPI, 3, 0x01, 0x02, 0x02,
                SF_UNDEF,
>>>>>>> db20e11 (second commit)
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0
            },
            {
                // HW config 26Mhz Start
                0x00010000,  // SFI_MAC_CTL
<<<<<<< HEAD
                0xEB0B5770,  // SFI_DIRECT_CTL
                0x52F80000,     //  SFI_MISC_CTL
=======
                0x0C0B7771,  // SFI_DIRECT_CTL
                0x52F80310,     //  SFI_MISC_CTL
                0x00000001,     //  SFI_MISC_CTL2
                0x0,     // 1st SFI_DLY_CTL_2
                0x0,         // 1st SFI_DLY_CTL_3
                0x0,           // DRIVING
                0                 // Reserved
            }   // HW config End
        },
        {
            // SIP XM25QU16C
            {
                // HW config 78Mhz Start
                0x00010000,  // SFI_MAC_CTL
                0xEB0B5770,  // SFI_DIRECT_CTL
                0x52F80310,     //  SFI_MISC_CTL
                0x00000000,     //  SFI_MISC_CTL2
                0x0,     // 1st SFI_DLY_CTL_2
                0x06000006,         // 1st SFI_DLY_CTL_3
                0x00040004,           // DRIVING
                0,                  // Reserved
                0,  // 2nd SFI_DLY_CTL_4
                0  // 2nd SFI_DLY_CTL_5
            },  // HW config End
            {
                SPI, 1, 0x50, SPI, 3, 0x01, 0x02, 0x02,
                SF_UNDEF,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0
            },
            {
                // HW config 26Mhz Start
                0x00010000,  // SFI_MAC_CTL
                0x0C0B7771,  // SFI_DIRECT_CTL
                0x52F80310,     //  SFI_MISC_CTL
>>>>>>> db20e11 (second commit)
                0x00000001,     //  SFI_MISC_CTL2
                0x0,     // 1st SFI_DLY_CTL_2
                0x0,         // 1st SFI_DLY_CTL_3
                0x0,           // DRIVING
                0                 // Reserved
            }   // HW config End
        },
        {
            // GD25LQ32C
            {
                // HW config 78Mhz Start
                0x00010000,  // SFI_MAC_CTL
                0xEB0B5770,  // SFI_DIRECT_CTL
                0x52F80310,     //  SFI_MISC_CTL
                0x00000000,     //  SFI_MISC_CTL2
                0x0,            // 1st SFI_DLY_CTL_2
                0x05000005,         // 1st SFI_DLY_CTL_3
                0x00040004,           // DRIVING
                0,                  // Reserved
                0,  // 2nd SFI_DLY_CTL_4
                0  // 2nd SFI_DLY_CTL_5
            },  // HW config End
            {
                SPI, 1, 0x50, SPI, 3, 0x01, 0x02, 0x02,
                SPI, 2, 0x05, 0x01, SPI, 2, 0x77, 0xFF, SF_UNDEF,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            },
            {
                // HW config 26Mhz Start
                0x00010000,  // SFI_MAC_CTL
                0x0C0B7771,  // SFI_DIRECT_CTL
                0x52F80310,     //  SFI_MISC_CTL
                0x00000001,     //  SFI_MISC_CTL2
                0x0,     // 1st SFI_DLY_CTL_2
                0x0,         // 1st SFI_DLY_CTL_3
                0x0,           // DRIVING
                0                 // Reserved
            }   // HW config End
        },
        {
            // MTKSIP_2523_SF_32_03
            {
                // HW config 78Mhz Start
                0x00010000,  // SFI_MAC_CTL
                0xEB0B5770,  // SFI_DIRECT_CTL
                0x52F80000,     //  SFI_MISC_CTL
                0x00000001,     //  SFI_MISC_CTL2
                0x0,     // 1st SFI_DLY_CTL_2
                0x18000018,         // 1st SFI_DLY_CTL_3
                0x00040004,           // DRIVING
                0,                  // Reserved
                0,  // 2nd SFI_DLY_CTL_4
                0  // 2nd SFI_DLY_CTL_5
            },  // HW config End
            {
                SPI, 1, 0x06, SPI, 2, 0x01, 0x42, SPI,
                2, 0x05, 0x01, SPI, 2, 0xC0, 0x02, SF_UNDEF,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0
            },
            {
                // HW config 26Mhz Start
                0x00010000,  // SFI_MAC_CTL
                0xEB0B5770,  // SFI_DIRECT_CTL
                0x52F80000,     //  SFI_MISC_CTL
                0x00000001,     //  SFI_MISC_CTL2
                0x0,     // 1st SFI_DLY_CTL_2
                0x0,         // 1st SFI_DLY_CTL_3
                0x0,           // DRIVING
                0                 // Reserved
            }   // HW config End
        },
        {
            // MTKSIP_2523_SF_32_04
            {
                // HW config 78Mhz Start
                0x00010000,  // SFI_MAC_CTL
                0x0C0B7771,  // SFI_DIRECT_CTL
                0x52F80310,     //  SFI_MISC_CTL
                0x00000001,     //  SFI_MISC_CTL2
                0x0,     // 1st SFI_DLY_CTL_2
                0x17000017,         // 1st SFI_DLY_CTL_3
                0x00040004,           // DRIVING
                0,                  // Reserved
                0,  // 2nd SFI_DLY_CTL_4
                0  // 2nd SFI_DLY_CTL_5
            },  // HW config End
            {
                SPI, 1, 0x50, SPI, 3, 0x01, 0x02, 0x02,
                SPI, 1, 0x38, QPI, 2, 0xC0, 0x32, SF_UNDEF,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0
            },
            {
                // HW config 26Mhz Start
                0x00010000,  // SFI_MAC_CTL
                0x0C0B7771,  // SFI_DIRECT_CTL
                0x52F80310,     //  SFI_MISC_CTL
                0x00000001,     //  SFI_MISC_CTL2
                0x0,     // 1st SFI_DLY_CTL_2
                0x0,         // 1st SFI_DLY_CTL_3
                0x0,           // DRIVING
                0                 // Reserved
            }   // HW config End
        },
        {
            // MTKSIP_2523_SF_32_01
            {
                // HW config 78Mhz Start
                0x00010000,  // SFI_MAC_CTL
                0xEB0B5770,  // SFI_DIRECT_CTL
                0x52F80300,     //  SFI_MISC_CTL
                0x00000000,     //  SFI_MISC_CTL2
                0x0,     // 1st SFI_DLY_CTL_2
                0x18000018,         // 1st SFI_DLY_CTL_3
                0x00040004,           // DRIVING
                0,                  // Reserved
                0,  // 2nd SFI_DLY_CTL_4
                0  // 2nd SFI_DLY_CTL_5
            },  // HW config End
            {
                SPI, 1, 0x50, SPI, 3, 0x01, 0x02, 0x02,
                SF_UNDEF, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0
            },
            {
                // HW config 26Mhz Start
                0x00010000,  // SFI_MAC_CTL
                0xEB0B5770,  // SFI_DIRECT_CTL
                0x52F80000,     //  SFI_MISC_CTL
                0x00000001,     //  SFI_MISC_CTL2
                0x0,     // 1st SFI_DLY_CTL_2
                0x0,         // 1st SFI_DLY_CTL_3
                0x0,           // DRIVING
                0                 // Reserved
            }   // HW config End
        },
        {
            // W25Q16EW
            {
                // HW config 78Mhz Start
                0x00010000,  // SFI_MAC_CTL
                0xEB0B5770,  // SFI_DIRECT_CTL
                0x52F80310,     //  SFI_MISC_CTL
                0x00000000,     //  SFI_MISC_CTL2
                0x0,     // 1st SFI_DLY_CTL_2
                0x06000006,         // 1st SFI_DLY_CTL_3
                0x00040004,           // DRIVING
                0,                  // Reserved
                0,  // 2nd SFI_DLY_CTL_4
                0  // 2nd SFI_DLY_CTL_5
            },  // HW config End
            {
                SPI, 1, 0x50, SPI, 3, 0x01, 0x02, 0x02, SF_UNDEF,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0
            },
            {
                // HW config 26Mhz Start
                0x00010000,  // SFI_MAC_CTL
                0x0C0B7771,  // SFI_DIRECT_CTL
                0x52F80300,     //  SFI_MISC_CTL
                0x00000001,     //  SFI_MISC_CTL2
                0x0,     // 1st SFI_DLY_CTL_2
                0x0,         // 1st SFI_DLY_CTL_3
                0x0,           // DRIVING
                0                 // Reserved
            }   // HW config End
        },
        {
            // GD25LQ16C
            {
                // HW config 78Mhz Start
                0x00010000,  // SFI_MAC_CTL
                0xEB0B5770,  // SFI_DIRECT_CTL
                0x52F80300,     //  SFI_MISC_CTL
                0x00000000,     //  SFI_MISC_CTL2
                0x0,            // 1st SFI_DLY_CTL_2
                0x0C00000C,         // 1st SFI_DLY_CTL_3
                0x00040004,           // DRIVING
                0,                  // Reserved
                0,  // 2nd SFI_DLY_CTL_4
                0  // 2nd SFI_DLY_CTL_5
            },  // HW config End
            {
                SPI, 1, 0x50, SPI, 3, 0x01, 0x02, 0x02,
                SPI, 2, 0x05, 0x01, SPI, 2, 0x77, 0xFF, SF_UNDEF,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            },
            {
                // HW config 26Mhz Start
                0x00010000,  // SFI_MAC_CTL
                0x0C0B7771,  // SFI_DIRECT_CTL
                0x52F80310,     //  SFI_MISC_CTL
                0x00000001,     //  SFI_MISC_CTL2
                0x0,     // 1st SFI_DLY_CTL_2
                0x0,         // 1st SFI_DLY_CTL_3
                0x0,           // DRIVING
                0                 // Reserved
            }   // HW config End
        }

    }
};



/* SFC Driving registers */
#define SFIO_CFG_WP           ((volatile unsigned int *)(TOP_MISC_CFG_BASE + 0x200))
#define SFIO_CFG_CS           ((volatile unsigned int *)(TOP_MISC_CFG_BASE + 0x204))
#define SFIO_CFG_CK           ((volatile unsigned int *)(TOP_MISC_CFG_BASE + 0x208))
#define SFIO_CFG_IN           ((volatile unsigned int *)(TOP_MISC_CFG_BASE + 0x20C))
#define SFIO_CFG_OUT          ((volatile unsigned int *)(TOP_MISC_CFG_BASE + 0x210))
#define SFIO_CFG_HOLD         ((volatile unsigned int *)(TOP_MISC_CFG_BASE + 0x214))

#define SFI0_CFG_DRV_BIT_MASK  (0x07)
#define SFI0_CFG_PUPD_BIT_MASK (0x07 << 3)
#define SFIO_CFG_PU            (0x02 << 3)
#define SFIO_CFG_PD            (0x06 << 3)

int sfi_index, CS_COUNT_SFI;
ATTR_TEXT_IN_RAM signed char custom_setSFI(void)
{
    const CMEMEntrySFI *sfi;

    uint32_t tmp_driving;

    SFC->RW_SF_MISC_CTL |= 0x10;
    SFC->RW_SF_DLY_CTL3 = 0x06000006;

    sfi_index = CMEM_EMIINIT_Index(); // API CMCP_EMIINIT_Index() is defined in hal_flash_combo_init.h

    if (sfi_index < 0){
        while (1);
    }

    /*--------------------------------------------------------------------------
     * Step 1.
     * 1. Switch mode QPI/SPI Quad
     * 2. Set Burst/Wrap length
     *--------------------------------------------------------------------------*/
    sfi = &combo_mem_hw_list.List[sfi_index]; // the structure name "combo_mem_hw_list" can be renamed by SFI owner

#if defined(__SFI_CLK_78MHZ__) || defined(__SFI_CLK_80MHZ__)
    // Device Initialization
    SFI_Dev_Command_List(0, sfi->DevInit_78M);

    /*--------------------------------------------------------------------------
     * Step 2. Initialize Serial Flash Control Registers
     *--------------------------------------------------------------------------*/
    SFC->RW_SF_MAC_CTL = sfi->HWConf_78M[SFC_MAC_CTL];
    SFC->RW_SF_DIRECT_CTL = sfi->HWConf_78M[SFC_DR_CTL];

    SFC->RW_SF_MISC_CTL = sfi->HWConf_78M[SFC_MISC_CTL];
    SFC->RW_SF_MISC_CTL2 = sfi->HWConf_78M[SFC_MISC_CTL2];
    SFC->RW_SF_DLY_CTL2 = sfi->HWConf_78M[SFC_DLY_CTL2];
    SFC->RW_SF_DLY_CTL3 = sfi->HWConf_78M[SFC_DLY_CTL3];

    tmp_driving = *SFIO_CFG_WP & (~(SFI0_CFG_DRV_BIT_MASK | SFI0_CFG_PUPD_BIT_MASK));
    *SFIO_CFG_WP = tmp_driving | (sfi->HWConf_78M[SFC_Driving]) | SFIO_CFG_PU;
    tmp_driving = *SFIO_CFG_CS & (~(SFI0_CFG_DRV_BIT_MASK | SFI0_CFG_PUPD_BIT_MASK));
    *SFIO_CFG_CS = tmp_driving | (sfi->HWConf_78M[SFC_Driving]) | SFIO_CFG_PU;
    tmp_driving = *SFIO_CFG_CK & (~(SFI0_CFG_DRV_BIT_MASK | SFI0_CFG_PUPD_BIT_MASK));
    *SFIO_CFG_CK = tmp_driving | (sfi->HWConf_78M[SFC_Driving]) | SFIO_CFG_PD;
    tmp_driving = *SFIO_CFG_IN & (~(SFI0_CFG_DRV_BIT_MASK | SFI0_CFG_PUPD_BIT_MASK));
    *SFIO_CFG_IN = tmp_driving | (sfi->HWConf_78M[SFC_Driving]) | SFIO_CFG_PU;
    tmp_driving = *SFIO_CFG_OUT & (~(SFI0_CFG_DRV_BIT_MASK | SFI0_CFG_PUPD_BIT_MASK));
    *SFIO_CFG_OUT = tmp_driving | (sfi->HWConf_78M[SFC_Driving]) | SFIO_CFG_PU;
    tmp_driving = *SFIO_CFG_HOLD & (~(SFI0_CFG_DRV_BIT_MASK | SFI0_CFG_PUPD_BIT_MASK));
    *SFIO_CFG_HOLD = tmp_driving | (sfi->HWConf_78M[SFC_Driving]) | SFIO_CFG_PU;

#else
#error "Undefined BB chips of SFC 130MHz"
#endif /* __SFI_CLK_78MHZ__ */

    /*--------------------------------------------------------------------------
     * Step 3. EFuse Post process of IO driving/ Sample clk delay/ Input delay
     * Currently, only for MT6250 and MT6260
     *--------------------------------------------------------------------------*/

    return 0;

}


#define CLK_CONDB               ((volatile unsigned int *)(CONFIG_BASE+0x0104))
#define ACFG_CLK_UPDATE         ((volatile unsigned short *)(ANA_CFGSYS_BASE+0x0150))

#define RG_VCORE_VOSEL   ((volatile unsigned short *)(PMU_SD_base + 0x230))
#define RG_VSF_VOSEL     ((volatile unsigned short *)(PMU_SD_base + 0x1D0))



ATTR_TEXT_IN_RAM int custom_setSFIExt()
{

    // init SFI & SF device (QPI / wrap ...etc)
    custom_setSFI();

    return 0;
}


#else   /* ! HAL_FLASH_MODULE_ENABLED*/

#include "memory_attribute.h"
ATTR_TEXT_IN_RAM signed char custom_setSFI(void)
{
    return 0;
}

ATTR_TEXT_IN_RAM int custom_setSFIExt()
{
    return 0;
}

#endif //#ifdef HAL_FLASH_MODULE_ENABLED

