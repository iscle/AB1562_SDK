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

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

typedef U8 BOOT_CODE;
#define BOOT_CODE_FROM_DEEP_SLEEP       (BOOT_CODE)(0)
#define BOOT_CODE_WATCHDOG_RESET        (BOOT_CODE)(1)
#define BOOT_CODE_SOFTWARE_RESET        (BOOT_CODE)(2)
#define BOOT_CODE_HARDWARE_RESET        (BOOT_CODE)(3)
#define BOOT_CODE_CHARGER_RESET         (BOOT_CODE)(4)
#define BOOT_CODE_REGEN_RESET           (BOOT_CODE)(5)
#define BOOT_CODE_POWEROFF_FAIL_RESET   (BOOT_CODE)(6)
#define BOOT_CODE_CAPTOUCH_RESET   		(BOOT_CODE)(7)
#define BOOT_CODE_UNKNOWN_REASON        (BOOT_CODE)(-1)

typedef U32 POWER_SAVING_LOCK_BIT;
#define SLEEP_LOCK_OS                   (POWER_SAVING_LOCK_BIT)(1 << 0)
#define SLEEP_LOCK_GPIO                 (POWER_SAVING_LOCK_BIT)(1 << 1)
#define SLEEP_LOCK_RESERVED             (POWER_SAVING_LOCK_BIT)(1 << 2)

typedef union union_sys_boot_reason
{
    U16 value;
    struct stru_sys_boot_reason
    {
        BOOL SoftwareRst             : 1;
        BOOL HwRst                   : 1;
        BOOL WatchdogRst             : 1;
        BOOL DeepSleep               : 1;
        BOOL charger                 : 1;
        BOOL regen                   : 1;
        BOOL powerofffail            : 1;
        BOOL reserved_b10            : 1;
        BOOL reserved_b11            : 1;
        BOOL reserved_b12            : 1;
        BOOL reserved_b13            : 1;
        BOOL reserved_b14            : 1;
        BOOL reserved_b15            : 1;
    } field;

} SYS_BOOT_REASON_UNION;

typedef U8 SYS_MODE;
#define SYS_POWER_MODE_ACTIVE       ((U8)0)
#define SYS_POWER_MODE_FROZEN       ((U8)1)
#define SYS_POWER_MODE_SLEEP        ((U8)2)
#define SYS_POWER_MODE_DEEP_SLEEP   ((U8)3)
#define SYS_POWER_MODE_SHUTDOWN     ((U8)4)

typedef struct stru_sys_boot_ctl
{
    SYS_BOOT_REASON_UNION Reason;
    U8 AdaptivePowerLv;
    U8 ConstrainedPowerLv;
} SYS_BOOT_CTL_STRU;
EXTERN SYS_BOOT_CTL_STRU gSysBootCtl;


EXTERN U8   SYS_BOOT_REASON_STATUS(VOID);
EXTERN BOOL SYS_CHECK_POWER_STATE(VOID);
EXTERN VOID SYS_SET_POWER_STATE(VOID);
EXTERN VOID SYS_CLEAR_POWER_STATE(VOID);
EXTERN VOID SYS_PWR_SAVING_LOCK(POWER_SAVING_LOCK_BIT lockBit);
EXTERN VOID SYS_PWR_SAVING_UNLOCK(POWER_SAVING_LOCK_BIT unLockBit);
EXTERN U32 SYS_PWR_SAVING_STATUS(VOID);
EXTERN U8 osTaskFrozenMode(VOID);


#endif

