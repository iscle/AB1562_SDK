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
#include "Peq_NvkeyHandler.h"
#include "Peq_Nvkey.h"
#include "App_PeqMount.h"
<<<<<<< HEAD

=======
#include "drv_anc.h"
>>>>>>> db20e11 (second commit)
/**************************************************************************************************
* Define
**************************************************************************************************/
#define PEQ_AUDIO_PATH_ERROR		0xFF
<<<<<<< HEAD

=======
>>>>>>> db20e11 (second commit)
/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct
{
	U8 audioPathId;
	PEQ_AUDIO_PATH_STRU *pAudioPath; //several sets of PEQ parameters
}PEQ_AUDIO_PATH_CTL_STRU;

/**************************************************************************************************
* Variables
**************************************************************************************************/
static U8 gDefaultAudioPathNum;
static U8 gCustomAudioPathNum;
static PEQ_AUDIO_PATH_CTL_STRU gPeqDefaultCtl[PEQ_PATH_NUM]; //[path, several sets of PEQ]
static PEQ_AUDIO_PATH_CTL_STRU gPeqCustomCtl[PEQ_PATH_NUM];

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static U8 app_PeqNvkey_GetAudioPathPhaseNum(U8 index, U8 groupIndex, U8 nvkeyMode);
static U8 app_PeqNvkey_GetAudioPathPhase(U8 index, U8 groupIndex, U8 phaseIndex, U8 nvkeyMode);
static U16 app_PeqNvkey_GetAudioPathNvkeyId(U8 index, U8 groupIndex, U8 phaseIndex, U8 nvkeyMode);
static U8 app_PeqNvkey_GetAudioPathIndex(U8 index, U8 nvkeyMode);

/**************************************************************************************************
* Static functions
**************************************************************************************************/
static U8 app_PeqNvkey_GetAudioPathPhaseNum(U8 index, U8 groupIndex, U8 nvkeyMode)
{
	U8 *pAddr = (nvkeyMode == PEQ_LOAD_DEFAULT) ? (U8 *)gPeqDefaultCtl[index].pAudioPath :
												  (U8 *)gPeqCustomCtl[index].pAudioPath ;

    if(!pAddr)
        return 0;

	pAddr += sizeof(U16); //numOfSet
	for(U8 indexMode = 0; indexMode < groupIndex; indexMode++)
	{
		U8 phaseNum = ((PEQ_ALL_SET_STRU *)pAddr)->numOfPhase;
		pAddr += sizeof(U16); //numOfPhase
		pAddr += sizeof(PEQ_SINGLE_PHASE_STRU) * phaseNum; //singleSetPhase
	}

	return (U8)((PEQ_ALL_SET_STRU *)pAddr)->numOfPhase;
}

static U8 app_PeqNvkey_GetAudioPathPhase(U8 index, U8 groupIndex, U8 phaseIndex, U8 nvkeyMode)
{
	U8 *pAddr = (nvkeyMode == PEQ_LOAD_DEFAULT) ? (U8 *)gPeqDefaultCtl[index].pAudioPath :
												  (U8 *)gPeqCustomCtl[index].pAudioPath ;

    if(!pAddr)
        return 0;

	pAddr += sizeof(U16); //numOfSet
	for(U8 indexMode = 0; indexMode < groupIndex; indexMode++)
	{
		U8 phaseNum = ((PEQ_ALL_SET_STRU *)pAddr)->numOfPhase;
		pAddr += sizeof(U16); //numOfPhase
		pAddr += sizeof(PEQ_SINGLE_PHASE_STRU) * phaseNum; //singleSetPhase
	}

	pAddr += sizeof(U16); //numOfPhase
	for(U8 indexPhase = 0; indexPhase < phaseIndex; indexPhase++)
	{
		pAddr += sizeof(PEQ_SINGLE_PHASE_STRU); //singleSetPhase
	}

	return (U8)((PEQ_SINGLE_PHASE_STRU *)pAddr)->peqPhase;
}

static U16 app_PeqNvkey_GetAudioPathNvkeyId(U8 index, U8 groupIndex, U8 phaseIndex, U8 nvkeyMode)
{
	U8 *pAddr = (nvkeyMode == PEQ_LOAD_DEFAULT) ? (U8 *)gPeqDefaultCtl[index].pAudioPath :
												  (U8 *)gPeqCustomCtl[index].pAudioPath ;

    if(!pAddr)
        return 0;

	pAddr += sizeof(U16); //numOfSet
	for(U8 indexMode = 0; indexMode < groupIndex; indexMode++)
	{
		U8 phaseNum = ((PEQ_ALL_SET_STRU *)pAddr)->numOfPhase;
		pAddr += sizeof(U16); //numOfPhase
		pAddr += sizeof(PEQ_SINGLE_PHASE_STRU) * phaseNum; //singleSetPhase
	}

<<<<<<< HEAD
	pAddr += sizeof(U16); //numOfPhase
=======
	U16 numOfPhase_set2;

	memcpy(&numOfPhase_set2, pAddr, sizeof(U16));
	pAddr += sizeof(U16); //numOfPhase

	if(numOfPhase_set2 <= 1)
		goto GetPeqPhase;

>>>>>>> db20e11 (second commit)
	for(U8 indexPhase = 0; indexPhase < phaseIndex; indexPhase++)
	{
		pAddr += sizeof(PEQ_SINGLE_PHASE_STRU); //singleSetPhase
	}

	//return ((PEQ_SINGLE_PHASE_STRU *)pAddr)->PEQParaNvkeyID;
<<<<<<< HEAD

=======
GetPeqPhase:
>>>>>>> db20e11 (second commit)
	pAddr += sizeof(U16); //peqPhase
	//U16 result = ((*(pAddr + sizeof(U8)) << 8) | *(pAddr));
	//printf("result:%X", result);
	return ((*(pAddr + sizeof(U8)) << 8) | *(pAddr));
}

static U8 app_PeqNvkey_GetAudioPathIndex(U8 peqComponent, U8 nvkeyMode) //return pPeqCtl index
{
	U8 index, audioPathNum = 0;
	PEQ_AUDIO_PATH_CTL_STRU *pPeqCtl = (PEQ_AUDIO_PATH_CTL_STRU *)NULL;

	if(nvkeyMode == PEQ_LOAD_DEFAULT)
	{
		audioPathNum = gDefaultAudioPathNum;
		pPeqCtl = gPeqDefaultCtl;
	}
	else if(nvkeyMode == PEQ_LOAD_CUSTOM)
	{
		audioPathNum = gCustomAudioPathNum;
		pPeqCtl = gPeqCustomCtl;
	}

	for(index = 0; index < audioPathNum; index++)
	{
		if(peqComponent == pPeqCtl[index].audioPathId)
		{
			return index;
		}
	}

	return PEQ_AUDIO_PATH_ERROR;
}

/**************************************************************************************************
* Public functions
**************************************************************************************************/
void APP_PeqNvkey_Init(void)
{
    U8* pAddr;
    U8* pAddrTemp;
    U16 dataSize = NVKEY_GetPayloadLength(NVKEYID_APP_PEQ_AUDIO_PATH_TABLE);

    if(dataSize == 0 || ((pAddr = FW_GetMemory(dataSize)) == (U8 *)NULL))
    {
        DBG_LOG_APP_PEQ( "[APP_PEQ] APP_PeqNvkey_Init fail datasize:%d, memory Ptr:0x%x", 2, dataSize, pAddr);
    }

    if(NVKEY_ReadFullKey(NVKEYID_APP_PEQ_AUDIO_PATH_TABLE, pAddr, dataSize) != dataSize)
    {
        DBG_LOG_APP_PEQ( "[APP_PEQ] APP_PeqNvkey_Init read full key fail", 0);
        FW_FreeMemory(pAddr);
        return;
    }

    pAddrTemp = pAddr;
    gDefaultAudioPathNum = (U8)((PEQ_AUDIO_PATH_TABLE_STRU *)pAddr)->numOfAudioPath;
    pAddr += sizeof(U16); //numOfAudioPath
    for(U8 numIndex = 0; numIndex < gDefaultAudioPathNum; numIndex++)
    {
        U8 *pPeqData;
        U16 nvkeyID;
        U8 audioPathId = (U8)(*((U8 *)pAddr));

        pAddr += sizeof(U16); //audioPathId
        nvkeyID = (*((U8 *)pAddr) | (*((U8 *)pAddr + sizeof(U8)) << 8));
        pAddr += sizeof(U16); //nvkeyId

        dataSize = NVKEY_GetPayloadLength(nvkeyID);
        if(dataSize == 0 || ((pPeqData = FW_GetMemory(dataSize)) == (U8 *)NULL))
        {
            continue;
        }
<<<<<<< HEAD
        
=======

>>>>>>> db20e11 (second commit)
        if(NVKEY_ReadFullKey(nvkeyID, pPeqData, dataSize) != dataSize)
        {
            FW_FreeMemory(pPeqData);
            continue;
        }

        if(gPeqDefaultCtl[numIndex].pAudioPath)
        {
            FW_FreeMemory((U8 *)gPeqDefaultCtl[numIndex].pAudioPath);
            gPeqDefaultCtl[numIndex].pAudioPath = NULL;
        }

        gPeqDefaultCtl[numIndex].audioPathId = audioPathId; //component
        gPeqDefaultCtl[numIndex].pAudioPath = (PEQ_AUDIO_PATH_STRU *)pPeqData; //PEQ parameters
    }
    FW_FreeMemory(pAddrTemp);
}

U8 APP_PeqNvkey_GetPEQGroupNum(U8 peqComponent, U8 nvkeyMode)
{
	U8 index = app_PeqNvkey_GetAudioPathIndex(peqComponent, nvkeyMode);

	if(index == PEQ_AUDIO_PATH_ERROR)
	{
		return 0;
	}

	return (nvkeyMode == PEQ_LOAD_DEFAULT) ? (U8)(gPeqDefaultCtl[index].pAudioPath->numOfSet) : (U8)(gPeqCustomCtl[index].pAudioPath->numOfSet);
}

U8 APP_PeqNvkey_GetPhaseNum(U8 peqComponent, U8 groupIndex, U8 nvkeyMode)
{
	if(groupIndex >= APP_PeqNvkey_GetPEQGroupNum(peqComponent, nvkeyMode))
	{
		return 0;
	}

	return app_PeqNvkey_GetAudioPathPhaseNum(app_PeqNvkey_GetAudioPathIndex(peqComponent, nvkeyMode), groupIndex, nvkeyMode);
}

U8 APP_PeqNvkey_GetPhase(U8 peqComponent, U8 groupIndex, U8 phaseIndex, U8 nvkeyMode)
{
	if(phaseIndex >= APP_PeqNvkey_GetPhaseNum(peqComponent, groupIndex, nvkeyMode))
	{
		return 0xFF;
	}

	return app_PeqNvkey_GetAudioPathPhase(app_PeqNvkey_GetAudioPathIndex(peqComponent, nvkeyMode), groupIndex, phaseIndex, nvkeyMode);
}

U16 APP_PeqNvkey_GetNvkeyId(U8 peqComponent, U8 groupIndex, U8 phaseIndex, U8 nvkeyMode)
{
	if(phaseIndex >= APP_PeqNvkey_GetPhaseNum(peqComponent, groupIndex, nvkeyMode))
	{
		return 0xFFFF;
	}
<<<<<<< HEAD

=======
#ifdef AIR_PEQ_SPEAKER_ENBALE
	if(phaseIndex != 0)
	{
		if(drv_anc_is_playing() == TRUE)
		{
			DBG_LOG_APP_PEQ( "[APP_PEQ] PEQ_ANC_ENABLE_NVKID", 0);
			return app_PeqNvkey_GetAudioPathNvkeyId(app_PeqNvkey_GetAudioPathIndex(peqComponent, nvkeyMode), 1, phaseIndex, nvkeyMode);
		}
			DBG_LOG_APP_PEQ( "[APP_PEQ] PEQ_ANC_DISABLE_NVKID", 0);
			return app_PeqNvkey_GetAudioPathNvkeyId(app_PeqNvkey_GetAudioPathIndex(peqComponent, nvkeyMode), 0, phaseIndex, nvkeyMode);
	}
#endif
	DBG_LOG_APP_PEQ("[APP_PEQ] Pre peq nvkey = %X, path index = %X, group index = %X, nvkey mode = %X", 4, app_PeqNvkey_GetAudioPathNvkeyId(app_PeqNvkey_GetAudioPathIndex(peqComponent, nvkeyMode), groupIndex, phaseIndex, nvkeyMode), app_PeqNvkey_GetAudioPathIndex(peqComponent, nvkeyMode), groupIndex, nvkeyMode);
>>>>>>> db20e11 (second commit)
	return app_PeqNvkey_GetAudioPathNvkeyId(app_PeqNvkey_GetAudioPathIndex(peqComponent, nvkeyMode), groupIndex, phaseIndex, nvkeyMode);
}


bool APP_PeqNvkey_IsCustomNvkeyEmpty(U8 peqComponent)
{
	U8 index = app_PeqNvkey_GetAudioPathIndex(peqComponent, PEQ_LOAD_CUSTOM);

	return	((index == PEQ_AUDIO_PATH_ERROR) || (gPeqCustomCtl[index].pAudioPath->numOfSet == 0));
}

