/* Copyright Statement:
 *
 * (C) 2020  Airoha Technology Corp. All rights reserved.
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

#ifndef __GC_CTL_H__
#define __GC_CTL_H__


/******************************************************************************
 * Common Structure of 2 wire command
 ******************************************************************************/

typedef struct stru_RF_2Wire_CMD{
    U8 CmdNum;
    U8 DIV2_NEGIF;
/*
    struct {
        U8 DIV2:1;
        U8 NEGIF:1;
        U8 Rsvd1:6;
    }field;
*/
    U8 Rsvd2[2];
}RF_2Wire_CMD, * RF_2Wire_CMD_PTR;

typedef struct stru_RF_2Wire_CMD_DATA{
    U16 RFCR_ADDR;
    U8 CMD_DLEN_RW;
/*
    struct {
        U8 CMD_DLEN:3;
        U8 Rsvd1:1;
        U8 RW:1;
        U8 Rsvd2:3;
    }field;
*/
    U8 DLY;
    U8 DATA1[4];
}RF_2Wire_CMD_DATA, * RF_2Wire_CMD_DATA_PTR;

typedef struct stru_RF_2Wire_CMD_Tx_Pwr_DATA{
    U16 RFCR_ADDR;
    U8 CMD_DLEN;
/*
    struct {
        U8 CMD_DLEN:3;
        U8 Rsvd1:1;
        U8 RW:1;
        U8 Rsvd2:3;
    }field;
*/
    U8 DLY;
    U16 DATA1[2];
}RF_2Wire_CMD_Tx_Pwr_DATA, * RF_2Wire_CMD_Tx_Pwr_DATA_PTR;


typedef struct stru_RF_2Wire_CMD_DATA2{
    U16 RFCR_ADDR;
    U8 CMD_DLEN_RW;
/*
    struct {
        U8 CMD_DLEN:3;
        U8 Rsvd1:1;
        U8 RW:1;
        U8 Rsvd2:3;
    }field;
*/
    U8 DLY;
    U8 DATA1[4];
    U8 DATA2[4];
}RF_2Wire_CMD_DATA2, * RF_2Wire_CMD_DATA2_PTR;

typedef struct stru_RF_2Wire_BrTxOnCmd{
    RF_2Wire_CMD TRx_CMD_Header;
    RF_2Wire_CMD_DATA2 TRx_CMD_DATA1;
    RF_2Wire_CMD_Tx_Pwr_DATA TRx_CMD_DATA2;
    RF_2Wire_CMD_DATA TRx_CMD_DATA3;
}RF_2Wire_BrTxOnCmd, * RF_2Wire_BrTxOnCmd_PTR;

typedef struct stru_RF_2Wire_BrRxOnCmd{
    RF_2Wire_CMD TRx_CMD_Header;
    RF_2Wire_CMD_DATA2 TRx_CMD_DATA1;
    RF_2Wire_CMD_DATA TRx_CMD_DATA2;
}RF_2Wire_BrRxOnCmd, * RF_2Wire_BrRxOnCmd_PTR;

typedef struct stru_RF_2Wire_BrTRxOffCmd{
    RF_2Wire_CMD TRx_CMD_Header;
    RF_2Wire_CMD_DATA TRx_CMD_DATA1;
}RF_2Wire_BrTRxOffCmd, * RF_2Wire_BrTRxOffCmd_PTR;

/******************************************************************************
 * External vairable
 ******************************************************************************/

/* basic data rate */
EXTERN  U16  g_DPC_Value_BTD_BDR_table[64];
EXTERN  U8   g_DPC_Index_BTD_BDR_table[64];
EXTERN  U16  g_tx_pwr_rf_btpa_BTD_BDR_table[64][1];

/* enhanced data rate */
EXTERN  U16  g_DPC_Value_BTD_EDR_table[64];
EXTERN  U8   g_DPC_Index_BTD_EDR_table[64];
EXTERN  U16  g_tx_pwr_rf_btpa_BTD_EDR_table[64][1];

EXTERN RF_2Wire_BrTxOnCmd BrTxOnCmd_ASIC;
EXTERN RF_2Wire_BrTxOnCmd EdrTxOnCmd_ASIC;
EXTERN RF_2Wire_BrTxOnCmd BrleTxOnCmd_ASIC;

#endif
