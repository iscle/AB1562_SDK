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


#ifndef _TYPES_H_
#define _TYPES_H_

/**
 * @addtogroup Standard
 * @{
 * This section introduces the Standard APIs including terms and acronyms,
 * supported features, software architecture, details on how to use the Message, enums, structures, typedefs and functions.
 *
 */

/*!

     @defgroup PAGE_TYPES types.h
     @{
        @page PAGE_API_TYPES types.h
        types.h has APIs related to types.

*/

/*!
 *@file   types.h
 *@brief  Defines airoha common types
 *
 */

typedef unsigned long long  U64;
typedef signed long long    S64;
typedef unsigned int        U32;
typedef signed int          S32;
typedef unsigned short      U16;
typedef signed short        S16;
typedef unsigned char       U8;
typedef signed char         S8;
typedef void                VOID;
typedef unsigned char       BOOL;
typedef unsigned int        uint32_t;
typedef signed int          int32_t;
typedef unsigned short      uint16_t;
typedef signed short        int16_t;
typedef unsigned char       uint8_t;
typedef signed char         int8_t;

#ifndef NULL
#define NULL ((VOID *)0)
#endif
#ifndef TRUE
#define TRUE           (1)
#endif
#ifndef FALSE
#define FALSE          (0)
#endif

#define NO_OPT __attribute__((optimize("-O0")))
#define ALIGN(n) __attribute__((aligned(n)))
#ifdef WIN32
#define PACKED
#define WEAK
#else
#define PACKED __attribute__((packed))
#define WEAK __attribute__((weak))
#endif
#define UNUSED(p) ((void)(p))

////////////////////////////////////////////////////////////////////////////////
// Keywords ////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define PUBLIC
#define PRIVATE
#define REGISTER    register
#define CONST       const
#define EXTERN      extern
//#define STATIC      static
#define INLINE      inline
#define VOLATILE    volatile

#define CODE        const
#define CODE_PTR    const *

typedef VOID (*fFunc)(VOID);

////////////////////////////////////////////////////////////////////////////////
// Macro Prototypes ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * @brief get the offset of a member from the containing structure
 *
 * @param type         the type of the container struct this is embedded in.
 * @param member       the name of the member within the struct.
 */
#ifndef _WIN32
#define OFFSET_OF(type,member) ((SIZE)&(((type *)0)->member))
#else
#define OFFSET_OF(type,member) ((unsigned int)&(((type *)0)->member))
#endif

/**
 * @brief cast a member of a structure out to the containing structure
 * @param ptr             the pointer to the member.
 * @param type            the type of the container struct this is embedded in.
 * @param member          the name of the member within the struct.
 */
#define CONTAINER_OF(ptr,type,member) ((type *)((U8 *)(ptr) - OFFSET_OF(type,member)))

/* legacy */
#define OS_OFFSET_OF        OFFSET_OF
#define OS_CONTAINER_OF     CONTAINER_OF


////////////////////////////////////////////////////////////////////////////////
// Section Prototypes ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/* AR_IRAM mean put function to section .iram.__LINE__ */
#define _STR(s)     #s
#define STR(s)      _STR(s)
#define _CONS(a,b)  a##b
#define CONS(a,b)   _CONS(a,b)
#define FUNC_NAME(a)   int(a)
#define AT_IRAM_(a) __attribute__ ((section (a)))
#define AT_IRAM AT_IRAM_(STR(CONS(.iram,__LINE__)))

#define AT_BOOTLOADER __attribute__ ((section (".bootloader.text")))
#define AT_ESD        __attribute__ ((section (".esd")))

#if (AB1530B || AB1530C || AB1530D)
#define AT_ROM        __attribute__ ((section (".rom")))
#define AT_ROM_BOOK
#else
#define AT_ROM
#define AT_ROM_BOOK
#endif

////////////////////////////////////////////////////////////////////////////////
// Types ///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef U32 SIZE;
typedef S32 SIZE_T;

typedef U8  ADDR8;
typedef U16 ADDR16;
typedef U32 ADDR24;
typedef U32 ADDR32;

typedef union
{
    U16 value;
    struct
    {
        U8 B0;
        U8 B1;
    } in_byte;

} TWO_BYTE_UNION;

typedef union
{
    U32 value;
    struct
    {
        U8 B0;
        U8 B1;
        U8 B2;
        U8 B3;
    } in_byte;

} FOUR_BYTE_UNION;

typedef struct stru_byte
{
    U8 Bit0 : 1;
    U8 Bit1 : 1;
    U8 Bit2 : 1;
    U8 Bit3 : 1;
    U8 Bit4 : 1;
    U8 Bit5 : 1;
    U8 Bit6 : 1;
    U8 Bit7 : 1;

} BYTE_STRU;

typedef union union_byte
{
    U8 value;
    struct
    {
        U8 Bit0 : 1;
        U8 Bit1 : 1;
        U8 Bit2 : 1;
        U8 Bit3 : 1;
        U8 Bit4 : 1;
        U8 Bit5 : 1;
        U8 Bit6 : 1;
        U8 Bit7 : 1;

    } field;

} BYTE_UNION;

//- Input Tag
#define IN
//- Output Tag
#define OUT
//- Optional Tag : A Optional could NULL if you don't specify a value
#define OPT
//- Optional Input Tag : A Optional Input could input NULL if you don't specify a value
#define IN_OPT

/**
 * Optimization
 */
#define MOD_BY_8(a) ((a) & 7)
#define DIV_BY_8(a) ((a) >> 3)

/*!
    @}
*/

/*!
    @}
*/

#endif
