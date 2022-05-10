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

#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

////////////////////////////////////////////////////////////////////////////////
// CONSTANT DEFINITIONS ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef U32 INTR_ID;
typedef VOID (*INTERRUPT_HDLR)(VOID);

/*!
 *   @name Interrupt Categorie
 */

/*!
 *   @brief uart interrupt ID
 */
#define INTR_ID_UART                        ((INTR_ID)(0))
/*!
 *   @brief i2c interrupt ID
 */
#define INTR_ID_I2C                         ((INTR_ID)(1))
/*!
 *   @brief spi interrupt ID
 */
#define INTR_ID_SPI                         ((INTR_ID)(2))
/*!
 *   @brief sdc interrupt ID
 */
#define INTR_ID_SDC                         ((INTR_ID)(3))
/*!
 *   @brief usb interrupt ID
 */
#define INTR_ID_USB                         ((INTR_ID)(4))
/*!
 *   @brief aio adc interrupt ID
 */
#define INTR_ID_AIO_ADC                     ((INTR_ID)(5))
/*!
 *   @brief srcA interrupt ID
 */
#define INTR_ID_PWM1                       ((INTR_ID)(6))
/*!
 *   @brief srcB interrupt ID
 */
#define INTR_ID_PWM2                       ((INTR_ID)(7))
/*!
 *   @brief reserved interrupt
 */
#define INTR_ID_PS                           ((INTR_ID)(8))
/*!
 *   @brief p192 interrupt ID
 */
#define INTR_ID_P192                        ((INTR_ID)(9))
/*!
 *   @brief p256 interrupt ID
 */
#define INTR_ID_P256                        ((INTR_ID)(10))
/*!
 *   @brief reserved interrupt
 */
#define INTR_ID_GDMA                        ((INTR_ID)(11))
/*!
 *   @brief reserved interrupt
 */
#define INTR_ID_FFT_PEQ                     ((INTR_ID)(12))
/*!
 *   @brief reserved interrupt
 */
#define INTR_ID_MDSP_IDMA                    ((INTR_ID)(13))
/*!
 *   @brief reserved interrupt
 */
#define INTR_ID_MSDP                        ((INTR_ID)(14))
/*!
 *   @brief reserved interrupt
 */
#define INTR_ID_WDT                         ((INTR_ID)(15))
/*!
 *   @brief keyscan interrupt
 */
#define INTR_ID_UART2                       ((INTR_ID)(16))
/*!
 *   @brief pmu interrupt ID
 */
#define INTR_ID_PMU                         ((INTR_ID)(17))
/*!
 *   @brief reserved interrupt
 */
#define INTR_ID_RTC                          ((INTR_ID)(18))
/*!
 *   @brief aes interrupt ID
 */
#define INTR_ID_AES                          ((INTR_ID)(19))
/*!
 *   @brief reserved interrupt
 */
#define INTR_ID_LPSCAN                       ((INTR_ID)(20))
/*!
 *   @brief reserved interrupt
 */
#define INTR_ID_TOUCHKEY                    ((INTR_ID)(21))
/*!
 *   @brief reserved interrupt
 */
#define INTR_ID_GPIO                         ((INTR_ID)(22))
/*!
 *   @brief level 2 i2c sensor dma
 */
#define INTR_ID_I2CDMA                     ((INTR_ID)(23))
/*!
 *   @brief level 2 spi sensor dma
 */
#define INTR_ID_SPIDM                       ((INTR_ID)(24))
/*!
 *   @brief level 2 timer interrupt
 */
#define INTR_ID_TIMER                       ((INTR_ID)(25))
/*!
 *   @brief level 2 ddma interrupt
 */
#define INTR_ID_DDMA                         ((INTR_ID)(26))
/*!
 *   @brief level 2 sw interrupt ID
 */
#define INTR_ID_RESERVE27                    ((INTR_ID)(27))
/*!
 *   @brief level 2 sw interrupt ID
 */
#define INTR_ID_RESERVE28                    ((INTR_ID)(28))
/*!
 *   @brief level 2 sw interrupt ID
 */
#define INTR_ID_RESERVE29                   ((INTR_ID)(29))
/*!
 *   @brief level 2 sw interrupt ID
 */
#define INTR_ID_RESERVE30                   ((INTR_ID)(30))
/*!
 *   @brief level 2 sw interrupt ID
 */
#define INTR_ID_SW                          ((INTR_ID)(31))

////////////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
 /**
  * @brief The function is used to register interrupt handler.
  *
  * @param IntrId   interrupt ID
  * @param IntrHdlr Interrupt handler function
  */
EXTERN VOID INTR_RegisterHandler(
    IN      INTR_ID IntrId,
    IN      INTERRUPT_HDLR IntrHdlr);

/**
 * @breif The function is used to cancel the interrupt handler to the specific interrupt.
 *
 * @param IntrId The interrupt ID
 */
EXTERN VOID INTR_CancelHandler(
    IN      INTR_ID IntrId);

/**
 * @brief The function saves current interrupt setting and disable interrupt
 *
 * @param mask   saved interrupt setting
 */
EXTERN VOID INTR_save_and_set_interrupt_mask(U32 *mask);

/**
 * @brief This function restores the interrupt setting
 *
 * @param mask   interrupt setting to restore
 */
EXTERN VOID INTR_restore_interrupt_mask(U32 mask);


#endif
