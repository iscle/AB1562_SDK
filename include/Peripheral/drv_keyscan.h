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

#ifndef _DRV_KEYSCAN_H_
#define _DRV_KEYSCAN_H_

/**
 * @addtogroup Peripheral
 * @{
 * This section introduces the Peripheral APIs including terms and acronyms,
 * supported features, software architecture, details on how to use the Message, enums, structures, typedefs and functions.
 *
 */

/*!

     @defgroup PAGE_API_DRIVER_KEYSCAN drv_keyscan.h
     @{
        @page PAGE_API_DRIVER_KEYSCAN drv_keyscan.h
        Drv_keyscan.h is a driver for key scan.

*/

#include "bit.h"
#include <types.h>
#include "hw_sfr.h"
#include "drv_keyscan_hal.h"


 /*!

     @defgroup PAGE_API_DRIVER_KEYSCAN drv_keyscan.h
     @{
        @page PAGE_API_DRIVER_KEYSCAN drv_keyscan.h
        DRV_KEYSCAN.h is an convenient KEYSCAN API set which allow user to do KEYSCAN operation without complex hardware controller.
*/

/***************************************************/
/*Macro Definition*/
/***************************************************/
/*!
     @name Macros
     @{
 */
/*!@brief Keyscan interrupt mode selection
*/
#define KS_INT_MODE_CONTINUOUS  0x0000
/*!@brief Keyscan interrupt mode selection
*/
#define KS_INT_MODE_DIFFERENCE  0x0080

/*!@brief Keyscan Debounce mode selection
*/
#define KS_DEB_MODE_DIFFERENCE  0x0090

/*!@brief Keyscan GPIO enable selection
*/
#define KS_IO_EN_DISABLE        0x0000
/*!@brief Keyscan GPIO enable selection
*/
#define KS_IO_EN_ENABLE         0x0040


/*!@brief  Keyscan software enable selection
*/
#define KS_SW_EN_DISABLE        0x0000
/*!@brief  Keyscan software enable selection
*/
#define KS_SW_EN_ENABLE         0x0020


/*!@brief  Keyscan debounce enable selection
*/
#define KS_DEB_EN_DISABLE       0x0000
/*!@brief  Keyscan debounce enable selection
*/
#define KS_DEB_EN_ENABLE        0x0010


/*!@brief  Keyscan clock gate enable selection
*/
#define KS_CLK_EN_DISABLE       0x0000
/*!@brief  Keyscan clock gate enable selection
*/
#define KS_CLK_EN_ENABLE        0x0002


/*!@brief  Keyscan enable selection
*/
#define KS_EN_DISABLE           0x0000
/*!@brief  Keyscan enable selection
*/
#define KS_EN_ENABLE            0x0001

/*!
    @}
*/

/* Keyscan EOD key code */
#define KS_EOD_KEY_RELEASE              0xFF             //      EOD, key empty
#define KS_EOD_FIFO_OVERFLOW            0xFE             //      EOD
#define KS_EOD_FIFO_FULL_KEY_OVERFLOW   0xFD             //8 Key+EOD, FIFO full, normal & keynum>8
#define KS_EOD_FIFO_FULL_NORMAL         0xFC             //  Key+EOD, FIFO full, normal
#define KS_EOD_FIFO_FULL_KEY_RELEASE    0xFB             //      EOD, FIFO full
#define KS_EOD_FIFO_FULL_GHOST_KEY      0xFA             //  Key+EOD, FIFO full, ghost key
#define KS_EOD_GHOST_KEY                0xF9             //  Key+EOD, ghost key
#define KS_EOD_KEY_OVERFLOW             0xF8             //8 Key+EOD, normal & keynum>8
#define KS_EOD_NORMAL                   0xF7             //  Key+EOD, normal


#define DRV_KEYSCAN_MSG_NORMAL          MSG_MESSAGE_PIO_BASE + 0
#define DRV_KEYSCAN_MSG_HAVE_GHOST      MSG_MESSAGE_PIO_BASE + 1


/***************************************************/
/*Type Definition*/
/***************************************************/


 /*!@brief GPIO switch time selection  */
typedef enum
{

	KS_IO_SW_TIME_SEL_1T = 0x0000,  ///< 7.82 us
    KS_IO_SW_TIME_SEL_2T = 0x1000,  /**< 7.82 us*2 */
    KS_IO_SW_TIME_SEL_3T = 0x2000,  /**< 7.82 us*3 */
    KS_IO_SW_TIME_SEL_4T = 0x3000,  /**< 7.82 us*4 */
    KS_IO_SW_TIME_SEL_8T = 0x4000,  /**< 7.82 us*8 */
    KS_IO_SW_TIME_SEL_12T = 0x5000, /**< 7.82 us*12 */
    KS_IO_SW_TIME_SEL_16T = 0x6000, /**< 7.82 us*16 */
    KS_IO_SW_TIME_SEL_20T = 0x7000  /**< 7.82 us*20 */
}KS_IO_SW_TIME;


/*! @brief HW keyscan rate delay cycle selection */
typedef enum
{
    /*!no extra delay*/
	KS_RATE_DLY_0T =      0x0000,
    /*!4.0638 ms*/
    KS_RATE_DLY_520T =    0x0100,
    /*!8.1276 ms*/
    KS_RATE_DLY_1040T =   0x0200,
	/*!12.1914 ms*/
    KS_RATE_DLY_1560T =   0x0300,
	/*!16.2552 ms*/
    KS_RATE_DLY_2080T =   0x0400,
	/*!20.3235 ms*/
    KS_RATE_DLY_2600T =   0x0500,
	/*!24.3873 ms*/
    KS_RATE_DLY_3120T =   0x0600,
	/*!28.4511 ms*/
    KS_RATE_DLY_3640T =   0x0700
}KS_RATE_DLY;

/*! @brief Structure for parameter of @ref Drv_KeyScanInit */
typedef struct
{
    /*! Io switch time*/
	KS_IO_SW_TIME swt;
	/*! Keyscan rate*/
    KS_RATE_DLY dly;
    /*! Number of Row. Max is 8*/
    U8 row_num;
	/*! Number of Column. Max is 18*/
    U8 col_num;
	/*! Row IOs */
    U8 row[8];
	/*! Column IOs*/
    U8 col[18];
}KS_INIT_STRUCTURE;

/***************************************************/
/*API Definition*/
/***************************************************/
EXTERN U32 gKeyIOStatus;

/*!
    @brief Set Keyscan Initial parameters:\n

	-Row/Column number\n
	-"key id" press/release\n
	-debounce times\n
	-Row and Column IO numbers\n

    @param kis Keyscan Initial parameters setting @ref KS_INIT_STRUCTURE
	@param cb This is a callback function to know which key ID pressed
*/
//void DRV_KeyScan_Init(KS_INIT_STRUCTURE kis, void(*cb)(U8 *, U8, U8));
EXTERN void DRV_KeyScan_Init(KS_INIT_STRUCTURE kis);


/*!
    @brief Enable the Keyscan hardware handler
*/
EXTERN void DRV_KeyScan_En();

/*!
    @brief Disable the Keyscan hardware handler
*/
void DRV_KeyScan_Dis();

/*!
    @}
*/

/*!
    @}
*/

#endif /* _DRV_KEYSCAN_H_ */
