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

 // ***************************************************************************
 // * Global symbol
 // ***************************************************************************
  #include <xtensa/coreasm.h>
  #include <xtensa/cacheasm.h>
  #include <xtensa/cacheattrasm.h>

 // ***************************************************************************
 // * Internal symbol
 // ***************************************************************************
.equ    WDT_Base,           0xA2090000
.equ    WDT_Disable,        0x10
.equ    WDT1_Base,          0xA2090030
.equ    WDT1_Disable,       0x40

.equ    MDM_BASE,       0x04201A48

 // ***************************************************************************
 // * External symbol
 // ***************************************************************************
    .extern main
    .extern SYS_IRAMLOADER
 // ***************************************************************************
 // * Reset Vector
 // ***************************************************************************
.begin  literal_prefix  .ResetVector
.section .ResetVector.text, "ax"
.align 4
.global _ResetVector
_ResetVector :
        j _ResetHandler
        .size    _ResetVector, . - _ResetVector
        .align 4
        .literal_position
        .align 4
        .global _ResetHandler
_ResetHandler:
        movi    a0, 0
        wsr     a0, INTENABLE

        wsr     a0, DBREAKC0
        wsr     a0, DBREAKC1
        dsync

        movi    a0, 0
        wsr     a0, EXCSAVE_1
        wsr     a0, EXCSAVE_2
        wsr     a0, EXCSAVE_3
        wsr     a0, EXCSAVE_4
        wsr     a0, EXCCAUSE
        wsr     a0, EPC_1
        wsr     a0, EPC_2
        wsr     a0, EPC_3
        wsr     a0, EPC_4
        wsr     a0, EPS_2
        wsr     a0, EPS_3
        wsr     a0, EPS_4

        wsr     a0, LCOUNT
        wsr     a0, LBEG
        wsr     a0, LEND

        wsr     a0, BR
        wsr     a0, CPENABLE

        j _STARTUP_

.end literal_prefix

 // ***************************************************************************
 // * start_up
 // * <null>
 // *
 // * system start up
 // ***************************************************************************
.begin  literal_prefix  .Reset
.section .Reset.text, "ax"
.global _STARTUP_
.align 4
.literal_position
_STARTUP_ :
        /* disable wdt */
        movi    a0 , WDT_Base
        movi    a1 , WDT_Disable
        s32i    a1 , a0 , 0
        movi    a0 , WDT1_Base
        movi    a1 , WDT1_Disable
        s32i    a1 , a0 , 0
        movi    a1 , 0

#ifdef RTLSIM
        movi    a0 , 0x04201A48
        movi    a2 , 1
        s32i    a2 , a0 , 0
#endif
        movi    a0 , _DynamicVectors_start
        wsr     a0 , VECBASE
        movi    a0 , 0

        call0   CachePreInit

#ifdef RTLSIM
        movi    a0 , 0x04201A48
        movi    a2 , 2
        s32i    a2 , a0 , 0
#endif

        #ifdef __XTENSA_CALL0_ABI__
        movi    sp , __stack
        movi    a3 , PS_UM | PS_INTLEVEL(4) //PS.UM = 1, PS.EXCM = 0, PS.INTLEVEL = 4
        #else
        movi    sp , __stack - 16
        addi    a4 , sp , 32
        s32e    a4 , sp , -12
        movi    a3 , PS_WOE_ABI | PS_UM | PS_INTLEVEL(4) //PS.WOE = 1, PS.UM = 1, PS.EXCM = 0, PS.INTLEVEL = 4
        #endif
        wsr     a3 , PS
        rsync

        /* enable dsp co-processor */
        movi    a3 , 3
        wsr     a3 , CPENABLE

#ifdef RTLSIM
        movi    a0 , 0x04201A48
        movi    a2 , 3
        s32i    a2 , a0 , 0
#endif
        /* region loader init */
        movi    a6,_region_loader_load
        movi    a7,_region_loader_start
        movi    a8,_region_loader_end
        call0   InitTextData0

        /* region init then jump to main function */
        call0   RegionInitAndJumpMain

_STARTUP_END_:
        j _STARTUP_END_


.section .Reset.text, "ax"
.align 4
.literal_position
InitTextData0:
        beq     a7 , a8 , InitTextDataRet0
InitTextDataLoop0:
        l32i    a9 , a6 , 0
        s32i    a9 , a7 , 0
        addi    a6 , a6 , 4
        addi    a7 , a7 , 4
        bne     a7 , a8 , InitTextDataLoop0
InitTextDataRet0:
        ret

.section .Reset.text, "ax"
.align 4
CachePreInit:
        /* invalidate dcache */
        movi    a2 , 64
        movi    a3 , 0
        loop    a2 , IcacheInvalidate
        dii     a3 , 0
        dii     a3 , 32
        addi    a3 , a3, 64
        /* invalidate icache */
IcacheInvalidate:
        movi    a2 , 32
        movi    a3 , 0
        loop    a2 , CacheRemap
        iii     a3 , 0
        iii     a3 , 64
        iii     a3 , 128
        iii     a3 , 192
        addi    a3 , a3, 0x100
CacheRemap:
        isync
        /* set Region0 I/D bus cacheable */
        movi    a4 , 0x00000004
        movi    a3 , 0x00000000
        witlb   a4 , a3
        isync
        wdtlb   a4 , a3
        isync
        /* set Region1 non-cacheable */
        movi    a4 , 0x00000004
        movi    a3 , 0x20000000
        witlb   a4 , a3
        isync
        wdtlb   a4 , a3
        isync
        /* set Region2 non-cacheable */
        movi    a4 , 0x40000002
        movi    a3 , 0x40000000
        witlb   a4 , a3
        isync
        wdtlb   a4 , a3
        isync
        /* set Region3 non-cacheable */
        movi    a4 , 0x60000002
        movi    a3 , 0x60000000
        witlb   a4 , a3
        isync
        wdtlb   a4 , a3
        isync
        /* remap Region4 to Region0 and set Region4 non-cacheable*/
        movi    a4 , 0x00000002
        movi    a3 , 0x80000000
        witlb   a4 , a3
        isync
        wdtlb   a4 , a3
        isync
        /* set Region5 non-cacheable */
        movi    a4 , 0xa0000002
        movi    a3 , 0xa0000000
        witlb   a4 , a3
        isync
        wdtlb   a4 , a3
        isync
        /* set Region6 non-cacheable */
        movi    a4 , 0xc0000002
        movi    a3 , 0xc0000000
        witlb   a4 , a3
        isync
        wdtlb   a4 , a3
        isync
        /* set Region7 non-cacheable */
        movi    a4 , 0xe0000002
        movi    a3 , 0xe0000000
        witlb   a4 , a3
        isync
        wdtlb   a4 , a3
        isync
        ret

.end literal_prefix

// ***************************************************************************
// * start_up
// * <null>
// *
// * system region loader
// ***************************************************************************
.begin  literal_prefix  .region_loader
.section .region_loader.text, "ax"
.align 4
.literal_position
InitTextData:
        beq     a7 , a8 , InitTextDataRet
InitTextDataLoop:
        l32i    a9 , a6 , 0
        s32i    a9 , a7 , 0
        addi    a6 , a6 , 4
        addi    a7 , a7 , 4
        bne     a7 , a8 , InitTextDataLoop
InitTextDataRet:
        ret

.section .region_loader.text, "ax"
.align 4
.literal_position
InitBss:
        beq     a6 , a7 , InitBssRet
InitBssLoop:
        movi    a8 , 0
        s32i    a8 , a6 , 0
        addi    a6 , a6 , 4
        bne     a6 , a7 , InitBssLoop
InitBssRet:
        ret

.section .region_loader.text, "ax"
.align 4
.literal_position
RegionInitAndJumpMain:
RegionInit:
        /* dynamic vectores init */
        movi    a6,_DynamicVectors_load
        movi    a7,_DynamicVectors_start
        movi    a8,_DynamicVectors_end
        call0   InitTextData

#ifdef RTLSIM
        movi    a0 , 0x04201A48
        movi    a2 , 4
        s32i    a2 , a0 , 0
#endif

        /* iram code init */
        //movi    a6,_iram_text_load
        //movi    a7,_iram_text_start
        //movi    a8,_iram_text_end
        //call0   InitTextData

        /* dram data init */
        movi    a6,_data_load
        movi    a7,_data_start
        movi    a8,_data_end
        call0   InitTextData

#ifdef RTLSIM
        movi    a0 , 0x04201A48
        movi    a2 , 5
        s32i    a2 , a0 , 0
#endif

#ifndef RTLSIM
        /* dram bss init */
        movi    a6 , _bss_start
        movi    a7 , _bss_end
        call0   InitBss
#endif


#ifdef RTLSIM
        movi    a0 , 0x04201A48
        movi    a2 , 7
        s32i    a2 , a0 , 0
#endif

        /* iram code init */
        movi    a10,_iram_text_load
        movi    a11,_iram_text_start
        movi    a12,_iram_text_end
        call8   SYS_IRAMLOADER

JumpMain:
        #ifdef __XTENSA_CALL0_ABI__
        call0   main
        #else
        call8   main
        #endif
MainReturn:
        j MainReturn

.end literal_prefix

