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

#ifndef _DRV_KEYPAD_H_
#define _DRV_KEYPAD_H_

#include "types.h"
#include "os_memory.h"

/***************************************************/
/*Macro Definition*/
/***************************************************/


/***************************************************/
/*Type Definition*/
/***************************************************/

enum{
    KEYID0,
    KEYID1,
    KEYID2,
    KEYID3,
    KEYID4,
    KEYID5,
    KEYID6,
    KEYID7,
    KEY_NO_MAX,
};

enum {
    PWRKEY_RELEASE,
    PWRKEY_PRESS,
};

#define HAL_PINMUX_KEYPAD   108
#define HAL_PINMUX_LINEIN   109
#define HAL_PINMUX_SYSPWR   110

#define USE_GPIO00          0x00000001
#define USE_GPIO01          0x00000002
#define USE_GPIO02          0x00000004
#define USE_GPIO03          0x00000008
#define USE_GPIO04          0x00000010
#define USE_GPIO05          0x00000020
#define USE_GPIO06          0x00000040
#define USE_GPIO07          0x00000080

#define DRV_KEYPAD_MSG_NORMAL          MSG_MESSAGE_KEYPAD_BASE + 0

#define DRV_KEYPAD_MSG_REGENKEY        MSG_MESSAGE_KEYPAD_BASE + 1

#define DRV_KEYPAD_MSG_LINEIN          MSG_MESSAGE_KEYPAD_BASE + 2

EXTERN U32 gPiobitMap;
EXTERN OSMQ OSMQ_PWRKEY_INTR_Flag;

EXTERN void drv_pwrkey_init(void);
EXTERN U32 DRV_RegenKey_Getstatus(void);
EXTERN void DRV_PWRKEY_ChkChgState(void);
EXTERN VOID DRV_PWRKEY_MsgHdlr(VOID);
EXTERN VOID DRV_Keypad_MsgHdlr(VOID);
EXTERN VOID DRV_Keypad_ChkState(uint32_t index);


/*!
    @brief Set Keyscan Initial parameters:\n

    -Row/Column number\n
    -"key id" press/release\n
    -debounce times\n
    -Row and Column IO numbers\n

    @param kis Keyscan Initial parameters setting @ref KS_INIT_STRUCTURE
    @param cb This is a callback function to know which key ID pressed
*/
EXTERN void DRV_KeyPad_Init(void);


/*!
    @brief Set KeyPad Setting parameters:\n

    - PioBitEn\n
    -debounce times\n

    @param PioBitEn KeyPad enable pio bit mapping to gpio number
    @param Deb_time 0x00(10msec),0x01(20msec),0x02(30msec),0x03(40msec)
    @param PioWake  Wakeup pin number.

*/
EXTERN void DRV_KeyPad_Setting(U32 PioBitEn, U8 Deb_time, U32 PioWakeup);


/*!
    @brief Set Regen key Setting parameters:\n

    @param Deb_En   regen key enable debounce, as keypad 0x00(10msec),0x01(20msec),0x02(30msec),0x03(40msec)
    @param waken_EN   Regen key Wakeup function enable or disable.

*/
EXTERN void DRV_RegenKey_Setting(U8 Deb_En, U8 Wakeup_En);

EXTERN void DRV_KeyPad_Mapping(void);
EXTERN void DRV_RTC_GPIO_Setting(U8 rtc_gpio, bool is_falling);

/*!
    @brief Set Line In Pin Setting parameters:\n

    @param no   Input Lin In Pin number.

*/
EXTERN void DRV_LineIn_Pin_Setting(U8 no);

EXTERN void DRV_SYS_POWER_LEV_Pin_Setting(void);

EXTERN void DRV_KeyPad_INTR(VOID);

#endif /* _DRV_KEYPAD_H_ */

