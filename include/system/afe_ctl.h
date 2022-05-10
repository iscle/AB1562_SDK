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

#ifndef _AFE_CTL_H_
#define _AFE_CTL_H_

#include "log.h"


#define AFE_INIT_ALL_SUPPORT          0xFF

#define AFE_MSGID_I(fmt,cnt,arg...)        LOG_MSGID_I(AFE,fmt,cnt,##arg)
#define AFE_MSGID_W(fmt,cnt,arg...)        LOG_MSGID_W(AFE,fmt,cnt,##arg)
#define AFE_MSGID_E(fmt,cnt,arg...)        LOG_MSGID_E(AFE,fmt,cnt,##arg)

enum
{
    AFE_DISABLE,
    AFE_ENABLE,
};

typedef struct csr_init_data
{
    U8 type;
    U32 addr;
    U32 val;
}PACKED CSR_INIT_DATA_STRU;

typedef struct csr_init_array
{
    U8 enable;
    U8 number;
    CSR_INIT_DATA_STRU data[0];
}PACKED CSR_INIT_ARRAY_STRU, *PTR_CSR_INIT_ARRAY_STRU;

typedef struct reg_i2c_init_data
{
    U8 type;
    U16 addr;
    U16 val;
}PACKED REG_I2C_INIT_DATA_STRU;

typedef struct reg_i2c_init_array
{
    U8 enable;
    U8 number;
    REG_I2C_INIT_DATA_STRU data[0];
}PACKED REG_I2C_INIT_ARRAY_STRU, *PTR_REG_I2C_INIT_ARRAY_STRU;

typedef struct csr_data
{
    U32 addr;
    U32 val;
}PACKED CSR_DATA_STRU;

typedef struct csr_array
{
    U8 enable;
    U8 number;
    CSR_DATA_STRU data[0];
}PACKED CSR_ARRAY_STRU, *PTR_CSR_ARRAY_STRU;

typedef struct reg_i2c_data
{
    U16 addr;
    U16 val;
}REG_I2C_DATA_STRU;

typedef struct reg_i2c_array
{
    U8 enable;
    U8 number;
    REG_I2C_DATA_STRU data[0];
}PACKED REG_I2C_ARRAY_STRU, *PTR_REG_I2C_ARRAY_STRU;

EXTERN U32 gPMU_INT_Status;

EXTERN VOID AFE_Init_from_NVKEY_CSR(VOID);
EXTERN VOID AFE_Init_from_NVKEY_Reg_I2C(VOID);
EXTERN VOID AFE_Patch_from_NVKEY_CSR(VOID);
EXTERN VOID AFE_Patch_from_NVKEY_Reg_I2C(VOID);

EXTERN U32 GET_PMU_INT_STATUS(VOID);

#endif /* _AFE_CTL_H_ */

