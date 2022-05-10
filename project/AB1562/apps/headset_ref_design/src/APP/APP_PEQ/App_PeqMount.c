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


#include "bt.h"
#include "App_PeqMount.h"
#include "Peq_NvkeyHandler.h"
#include "DSP_SDK.h"

log_create_module(APP_PEQ, PRINT_LEVEL_INFO);

/**************************************************************************************************
* Define
**************************************************************************************************/
#define PEQ_PARA_NVKEY_ID_START	0xF260
#define PEQ_PARA_NVKEY_ID_END	0xF27F

/**************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
	DSP_PEQ_USER,
	DSP_PEQ_SPEAKER,
	DSP_PEQ_FUNC_NUM
};

/**************************************************************************************************
* Public functions
**************************************************************************************************/
void APP_PeqMount_SendParaToDSP(U8 peqPhase, U16 peqParaNvkeyID)
{
    U8 *pAddr;
    U16 dataSize;

    if(peqParaNvkeyID < PEQ_PARA_NVKEY_ID_START || peqParaNvkeyID > PEQ_PARA_NVKEY_ID_END)
    {
        DBG_LOG_APP_PEQ( "[APP_PEQ]PEQ Para Nvkey ID NOT in range:%d", 1, peqParaNvkeyID);
        return;
    }

    if(((dataSize = NVKEY_GetPayloadLength(peqParaNvkeyID)) == 0) || ((pAddr = FW_GetMemory(dataSize)) == (U8 *)NULL))
    {
        DBG_LOG_APP_PEQ( "[APP_PEQ]Get PEQ Para Nvkey by NvkeyID:0x%X fail,size:%d, ptr:0x%x", 3, peqParaNvkeyID, dataSize, pAddr);
        return;
    }

    if( dataSize != NVKEY_ReadFullKey(peqParaNvkeyID, pAddr, dataSize))
    {
        DBG_LOG_APP_PEQ( "[APP_PEQ]GetParaAddrByNvkeyID read full key fail", 0);
        FW_FreeMemory(pAddr);
        return;
    }

    DBG_LOG_APP_PEQ( "[APP_PEQ]Set PEQ Para to DSP:%d, Nvkey ID:0x%X", 2, peqPhase, peqParaNvkeyID);
    switch(peqPhase)
    {
        case DSP_PEQ_USER:
            PEQ_UpdateParaByPtr(FUNC_PEQ_USER, ((PEQ_PARAMETER_STRU *)pAddr));
            break;
        case DSP_PEQ_SPEAKER:
            PEQ_UpdateParaByPtr(FUNC_PEQ_SPEAKER, ((PEQ_PARAMETER_STRU *)pAddr));
            break;
        default:
            break;
    }

    FW_FreeMemory(pAddr);
}

void APP_PeqMount_RealtimeToDSP(U8 peqPhase, U8 *pPeqPara, U16 length)
{
	PEQ_PARAMETER_STRU *pData;

	DBG_LOG_APP_PEQ( "[APP_PEQ]Realtime Set PEQ to DSP:%s", 1, APP_PeqPhaseLogString[peqPhase]);
	switch(peqPhase)
	{
		case DSP_PEQ_USER:
			//printf("Realtime to DSP FUNC_PEQ_USER");
			if((pData = FW_GetMemory(length)) != NULL)
			{
				FW_Memcpy(pData, pPeqPara, length);
				PEQ_UpdateParaByPtr(FUNC_PEQ_USER, pData);
				FW_FreeMemory(pData);
			}
			break;
		case DSP_PEQ_SPEAKER:
			//printf("Realtime to DSP FUNC_PEQ_SPEAKER");
			if((pData = FW_GetMemory(length)) != NULL)
			{
				FW_Memcpy(pData, pPeqPara, length);
				PEQ_UpdateParaByPtr(FUNC_PEQ_SPEAKER, pData);
				FW_FreeMemory(pData);
			}
			break;
		default:
			break;
	}
}

void APP_PeqMount_ResetFuncUser(void)
{
	PEQ_ResetPara(FUNC_PEQ_USER);
}
