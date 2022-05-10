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
<<<<<<< HEAD

#if defined(PROFILE_GFP_ENABLE)

#include "App_Gfp.h"
#include "bt_gap.h"

#include "App_Gfp_RhoHandler.h"
#include "Gfp_Bt.h"

extern BD_ADDR_T* BtAwsMce_GetAgentBdAddr(void);
//extern gfp_spp_rho_t;

#if 1
#define DBG_LOG_PROFILE_GFP_RHO(_message, arg_cnt, ...)     LOG_MSGID_I(GFP, _message, arg_cnt, ##__VA_ARGS__)
#else
#define DBG_LOG_PROFILE_GFP_RHO(_message, arg_cnt, ...)    printf( _message,  ##__VA_ARGS__)
#endif
=======
#if defined(AIR_GFP_ENABLE)
#include "gfp_features.h"
#ifdef GFP_MCSYNC_ENABLE
#include "log.h"
#include "gfps_api.h"
#include "App_Gfp.h"
#include "App_Gfp_RhoHandler.h"

>>>>>>> db20e11 (second commit)

/**************************************************************************************************
* Definition
**************************************************************************************************/
<<<<<<< HEAD
#define GFP_RHO_MSG_RECORD_HEADER_LENGTH (sizeof(gfp_rho_msg_record_t) - 1)
=======

#define APP_GFP_RHO_LOG_MSGID_I(_message, arg_cnt, ...) LOG_MSGID_I(APP_GFP, "[APP_GFP]RHO "_message, arg_cnt, ##__VA_ARGS__)

>>>>>>> db20e11 (second commit)
#define INVALID_MODULE_INDEX 0xFF

/**************************************************************************************************
* Enumeration
**************************************************************************************************/
<<<<<<< HEAD
enum
{
	GFP_RHO_MODULE_ID_SPP,
	GFP_RHO_MODULE_ID_NUM,
};
=======
typedef enum
{
    APP_GFP_RHO_MODULE_ID_GFP,
	APP_GFP_RHO_MODULE_ID_SPP,

	APP_GFP_RHO_MODULE_ID_NUM
}app_gfp_rho_module_id_enum;
>>>>>>> db20e11 (second commit)

/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct
{
<<<<<<< HEAD
	BOOL isLock;
	U16 totalQueueLen;
}gfp_rho_ctl_t;

typedef struct
{
	U16 msgId;
	U16 msgLen; //only data length
	U8 pMsgData[1];
}PACKED gfp_rho_msg_record_t;

typedef struct
{
	U8 moduleId;
	U16 length;
	U8 pData[1];
}PACKED gfp_rho_data_t;
=======
	U8 moduleId;
	U16 length; /* pData length only */
	U8 pData[0];
}PACKED app_gfp_rho_module_data_struct;

>>>>>>> db20e11 (second commit)

typedef struct
{
	U8 moduleId;
	U16 (*funcGetLength)(void);
	BOOL (*funcGetData)(U8 *pBuffer);
<<<<<<< HEAD
	BOOL (*funcApplyAgentData)(BD_ADDR_T *pBdAddr, U8 *pData, U16 length);
	BOOL (*funcApplyPartnerData)(BD_ADDR_T *pBdAddr, U8 *pData, U16 length);
}gfp_rho_module_ctl_t;
=======
	BOOL (*funcNewAgentProcess)(BD_ADDR_T *pBdAddr, U8 *pData, U16 length);
	BOOL (*funcNewPartnerProcess)(BD_ADDR_T *pBdAddr, U8 *pData, U16 length);
}app_gfp_rho_module_ctl_struct;
>>>>>>> db20e11 (second commit)

/**************************************************************************************************
* Variable
**************************************************************************************************/
<<<<<<< HEAD
gfp_rho_ctl_t gGfpRhoCtl;

static const gfp_rho_module_ctl_t gGfpRhoModuleCtl[GFP_RHO_MODULE_ID_NUM] =
{
	{GFP_RHO_MODULE_ID_SPP, gfp_spp_get_rho_data_length, gfp_spp_get_rho_data, gfp_spp_apply_agent_rho_data, gfp_spp_apply_partner_rho_data},
=======
static const app_gfp_rho_module_ctl_struct gAppGfpRhoModuleCtl[APP_GFP_RHO_MODULE_ID_NUM] =
{
    {APP_GFP_RHO_MODULE_ID_GFP, gfp_rho_get_data_length, gfp_rho_get_data, gfp_rho_new_agent_process, gfp_rho_new_partner_process},
	{APP_GFP_RHO_MODULE_ID_SPP, NULL, NULL, NULL, NULL},
>>>>>>> db20e11 (second commit)
};

/**************************************************************************************************
* Static Functions
**************************************************************************************************/
<<<<<<< HEAD
static U8 gfp_rho_find_module_index(U8 moduleId)
{
	U8 moduleIndex;

	for(moduleIndex = 0; moduleIndex < GFP_RHO_MODULE_ID_NUM; moduleIndex++)
	{
		if(gGfpRhoModuleCtl[moduleIndex].moduleId == moduleId)
=======
static U8 APP_Gfp_RhoGetIndexByModuleId(U8 moduleId)
{
	U8 moduleIndex;

	for (moduleIndex = 0; moduleIndex < APP_GFP_RHO_MODULE_ID_NUM; moduleIndex++)
	{
		if (gAppGfpRhoModuleCtl[moduleIndex].moduleId == moduleId)
>>>>>>> db20e11 (second commit)
		{
			return moduleIndex;
		}
	}

<<<<<<< HEAD
	return INVALID_MODULE_INDEX;
}

static U16 gfp_rho_get_all_module_data_length(void)
{
	U8 i;
	U16 len, length = 0;
	BD_ADDR_T *pAgBdAddr = BtAwsMce_GetAgentBdAddr();

	if( !pAgBdAddr ) return 0;

	for(i = 0; i < GFP_RHO_MODULE_ID_NUM; i++)
	{
		if(gGfpRhoModuleCtl[i].funcGetLength)
		{
			if((len = gGfpRhoModuleCtl[i].funcGetLength()))
			{
				length += len;
			}
		}
	}

	return length;
}

static BOOL gfp_rho_fill_data(U8 *pBuffer)
{
	U8 i, *pDataAddr;
	U16 len;

	if(!pBuffer)
	{
		DBG_LOG_PROFILE_GFP_RHO("[GFP_RHO] gfp_rho_fill_data: No Buffer\n",  0);
		return FALSE;
	}

	pDataAddr = pBuffer;
	for(i = 0; i < GFP_RHO_MODULE_ID_NUM; i++)
	{
		gfp_rho_data_t *pFillData = ((gfp_rho_data_t *)pDataAddr);

		pFillData->moduleId = gGfpRhoModuleCtl[i].moduleId;
		pFillData->length = 0;

		if(gGfpRhoModuleCtl[i].funcGetLength && gGfpRhoModuleCtl[i].funcGetData)
		{
			len = gGfpRhoModuleCtl[i].funcGetLength();
			pFillData->length = len;

			if(len)
			{
				if(!gGfpRhoModuleCtl[i].funcGetData(pFillData->pData))
				{
					DBG_LOG_PROFILE_GFP_RHO("[GFP_RHO] gfp_rho_fill_data: fail:%d\n",  1, gGfpRhoModuleCtl[i].moduleId);
					return FALSE;
				}
			}
		}

		pDataAddr += (len + sizeof(gfp_rho_data_t) - 1);
	}

	DBG_LOG_PROFILE_GFP_RHO("[GFP_RHO] gfp_rho_fill_data: OK\n",  0);
	return TRUE;
}

static BOOL gfp_rho_assign_data(U8 *pData, U16 *pApplyLen)
{
	U8 i, moduleIndex, *pDataAddr;

	if(!pData)
		return FALSE;

	*pApplyLen = 0;
	pDataAddr = pData;

	for(i = 0; i < GFP_RHO_MODULE_ID_NUM; i++)
	{
		gfp_rho_data_t *pAssignData = ((gfp_rho_data_t *)pDataAddr);
		moduleIndex = gfp_rho_find_module_index(pAssignData->moduleId);

		if(moduleIndex != INVALID_MODULE_INDEX && pAssignData->length && gGfpRhoModuleCtl[moduleIndex].funcApplyAgentData)
		{
			DBG_LOG_PROFILE_GFP_RHO("[GFP_RHO]gfp_rho_assign_data. mId:%d, length:%d\n", 2,  moduleIndex, pAssignData->length);

            //   gfp_spp_apply_agent_rho_data
			if(!gGfpRhoModuleCtl[moduleIndex].funcApplyAgentData(NULL, pAssignData->pData, pAssignData->length))
			{
				DBG_LOG_PROFILE_GFP_RHO("[GFP_RHO]gfp_rho_assign_data: fail. mId:%d\n",  1, pAssignData->moduleId);
				return FALSE;
			}
		}

		*pApplyLen += (pAssignData->length + sizeof(gfp_rho_data_t) - 1);
		pDataAddr += (pAssignData->length + sizeof(gfp_rho_data_t) - 1);
	}

	DBG_LOG_PROFILE_GFP_RHO("[GFP_RHO]gfp_rho_assign_data: OK!\n",  0);
	return TRUE;
}
=======
	return APP_GFP_RHO_MODULE_ID_NUM;
}

>>>>>>> db20e11 (second commit)

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
<<<<<<< HEAD
BOOL gfp_rho_lock()
{
	gGfpRhoCtl.isLock = TRUE;
	return TRUE;
}

BOOL gfp_rho_unlock()
{
	gGfpRhoCtl.isLock = FALSE;
	return TRUE;
}

BOOL gfp_rho_is_allow()
{
    BOOL isAllow = FALSE;
    isAllow = App_gfp_spp_is_allow_rho();
	return isAllow;
}


U16 gfp_rho_get_data_length(void) //IAP2_GetSppRhoDataLength
{
	U16 length;
	U16 lenModule;
	BD_ADDR_T *pAgBdAddr = BtAwsMce_GetAgentBdAddr();

	if(!pAgBdAddr)
	{
		length = 0;
	}
	else
	{
        length = gfp_spp_sizeof_gfp_spp_rho_st();
	}


    lenModule = gfp_rho_get_all_module_data_length();

	DBG_LOG_PROFILE_GFP_RHO("[GFP_RHO] gfp_rho_get_data_length:Len:%d lenModule=%d\n",  2, length, lenModule);

	return length+lenModule;
}

BOOL gfp_rho_get_data(U8 *pBuffer)
{
	if(!pBuffer)
	{
		DBG_LOG_PROFILE_GFP_RHO("[GFP_RHO] gfp_rho_get_data:No Buffer\n",  0);
		return FALSE;
	}

	if( !(gfp_rho_fill_data(pBuffer)))
	{
		DBG_LOG_PROFILE_GFP_RHO("[GFP_RHO] gfp_rho_get_data:Fill data Fail\n",  0);
		return FALSE;
	}

	if(gGfpRhoCtl.totalQueueLen)
	{
		DBG_LOG_PROFILE_GFP_RHO("[GFP_RHO] gfp_rho_get_data:remain Que:%d\n",  1, gGfpRhoCtl.totalQueueLen);
		//ASSERT(FALSE);
=======
BOOL APP_Gfp_RhoLock(void)
{
	return gfp_rho_lock();
}


BOOL APP_Gfp_RhoUnlock(void)
{
	return gfp_rho_unlock();
}


BOOL APP_Gfp_RhoIsLocked(void)
{
	return gfp_rho_is_locked();
}


BOOL APP_Gfp_RhoIsAllow(void)
{
    if (!gfp_is_enabled() ||
        (gfp_rho_is_allow() &&
         APP_Gfp_SppIsAllowRho()))
    {
        return TRUE;
    }

    return FALSE;
}


U16 APP_Gfp_RhoGetDataLength(void)
{
    U16 i, length = 0, module_data_length = 0;

    if (gfp_is_enabled())
    {
        for (i = 0; i < APP_GFP_RHO_MODULE_ID_NUM; i++)
        {
            module_data_length = 0;
            if (gAppGfpRhoModuleCtl[i].funcGetData &&
                gAppGfpRhoModuleCtl[i].funcGetLength)
            {
                module_data_length = gAppGfpRhoModuleCtl[i].funcGetLength();
            }

            if (module_data_length)
            {
                length += sizeof(app_gfp_rho_module_data_struct) + module_data_length;
            }
        }
    }

    return length;
}


BOOL APP_Gfp_RhoGetData(U8 *pBuffer)
{
	U16 i, length = 0, module_data_length = 0;
    BOOL ret = FALSE;
    app_gfp_rho_module_data_struct pFillData;

    if (!gfp_is_enabled())
    {
        return TRUE;
    }

	if (!pBuffer)
	{
		return FALSE;
	}

	for(i = 0; i < APP_GFP_RHO_MODULE_ID_NUM; i++)
	{
	    module_data_length = 0;
        if (gAppGfpRhoModuleCtl[i].funcGetData &&
            gAppGfpRhoModuleCtl[i].funcGetLength)
        {
            module_data_length = gAppGfpRhoModuleCtl[i].funcGetLength();
        }

        if (module_data_length)
        {
            pFillData.moduleId = i;
            pFillData.length = module_data_length;
            memcpy(pBuffer + length, &pFillData, sizeof(app_gfp_rho_module_data_struct));
            length += sizeof(app_gfp_rho_module_data_struct);
            ret = gAppGfpRhoModuleCtl[i].funcGetData(pBuffer + length);
            if (!ret)
            {
                return FALSE;
            }
            length += module_data_length;
        }
>>>>>>> db20e11 (second commit)
	}

	return TRUE;
}

<<<<<<< HEAD
//BtGfpApplyAgentRhoData    BtAmaApplyAgentRhoData
BOOL gfp_rho_apply_agent_data(BD_ADDR_T *pBdAddr, U8 *pData, U16 length)
{
	U8 *pApplyPos;//, msgNum;
	U16 applyLength = 0, len = length;

	UNUSED(pBdAddr);

	if(!length || !pData)
		return FALSE;

	pApplyPos = pData;

	//data
	if(!gfp_rho_assign_data(pApplyPos, &applyLength))
	{
		DBG_LOG_PROFILE_GFP_RHO("[GFP_RHO] gfp_rho_apply_agent_data:fail!!\n",  0);
		return FALSE;
	}

	DBG_LOG_PROFILE_GFP_RHO("[GFP_RHO] gfp_rho_apply_agent_data. length:%d, assign length:%d, pApplyPos:0x%x\n",  3, len, applyLength, (U32)pApplyPos);
	pApplyPos += applyLength;
	len -= applyLength;
	DBG_LOG_PROFILE_GFP_RHO("[GFP_RHO] gfp_rho_apply_agent_data:OK!!\n",  0);
	return TRUE;
}

BOOL gfp_rho_apply_partner_data(BD_ADDR_T *pBdAddr, U8 *pData, U16 length)
{
	U8 i;

=======

BOOL APP_Gfp_RhoNewAgentProcess(BD_ADDR_T *pBdAddr, U8 *pData, U16 length)
{
    U16 i, processed_length = 0;
    BOOL ret = FALSE;
    app_gfp_rho_module_data_struct pFillData;

    if (!gfp_is_enabled())
    {
        return TRUE;
    }

    if (!pData)
    {
        return FALSE;
    }

    do
    {
        if (length < sizeof(app_gfp_rho_module_data_struct))
        {
            APP_GFP_RHO_LOG_MSGID_I("Invalid length:%d. should >= %d", 2, length, sizeof(app_gfp_rho_module_data_struct));
            return FALSE;
        }
        memcpy(&pFillData, pData + processed_length, sizeof(app_gfp_rho_module_data_struct));

        i = APP_Gfp_RhoGetIndexByModuleId(pFillData.moduleId);
        if (APP_GFP_RHO_MODULE_ID_NUM != i)
        {
            processed_length += sizeof(app_gfp_rho_module_data_struct);
            if (processed_length + pFillData.length > length)
            {
                APP_GFP_RHO_LOG_MSGID_I("Invalid length:%d. should >= %d", 2, length, processed_length + pFillData.length);
                return FALSE;
            }

            if (gAppGfpRhoModuleCtl[i].funcNewAgentProcess)
            {
                ret = gAppGfpRhoModuleCtl[i].funcNewAgentProcess(pBdAddr, pData + processed_length, pFillData.length);
                if (!ret)
                {
                    APP_GFP_RHO_LOG_MSGID_I("funcNewAgentProcess of module id:%d fail", 1, pFillData.moduleId);
                    return FALSE;
                }
            }
            else
            {
                APP_GFP_RHO_LOG_MSGID_I("module id:%d has no funcNewAgentProcess", 1, pFillData.moduleId);
            }
            processed_length += pFillData.length;
        }
        else
        {
            APP_GFP_RHO_LOG_MSGID_I("unknown module id:%d", 1, pFillData.moduleId);
            return FALSE;
        }
    } while(processed_length < length);

    return TRUE;
}


BOOL APP_Gfp_RhoNewPartnerProcess(BD_ADDR_T *pBdAddr, U8 *pData, U16 length)
{
>>>>>>> db20e11 (second commit)
	UNUSED(pBdAddr);
	UNUSED(pData);
	UNUSED(length);

<<<<<<< HEAD
	for(i = 0; i < GFP_RHO_MODULE_ID_NUM; i++)
	{
		if(gGfpRhoModuleCtl[i].funcApplyPartnerData)
		{
			if(!gGfpRhoModuleCtl[i].funcApplyPartnerData(NULL, NULL, 0))
			{
				DBG_LOG_PROFILE_GFP_RHO("[GFP_RHO] gfp_rho_apply_partner_data:fail:%d!!\n",  1, gGfpRhoModuleCtl[i].moduleId);
				return FALSE;
			}
		}
	}

	DBG_LOG_PROFILE_GFP_RHO("[GFP_RHO] gfp_rho_apply_partner_data:OK!!\n",  0);
	return TRUE;
}


U16 BtGfpGetRhoDataLength()
{
	return gfp_rho_get_data_length();
}

BOOL BtGfpGetRhoData(U8 *pBuffer)
{
	return gfp_rho_get_data(pBuffer);
}

BOOL BtGfpApplyAgentRhoData(BD_ADDR_T *pBdAddr, U8 *pAwsMceData, U16 length)
{
    return gfp_rho_apply_agent_data(pBdAddr, pAwsMceData, length);
}

BOOL BtGfpApplyPartnerRhoData(BD_ADDR_T *pBdAddr, U8 *pAwsMceData, U16 length)
{
    return gfp_rho_apply_partner_data(pBdAddr, pAwsMceData, length);
}
#endif
=======
	U8 i;

    if (!gfp_is_enabled())
    {
        return TRUE;
    }

	for (i = 0; i < APP_GFP_RHO_MODULE_ID_NUM; i++)
	{
		if (gAppGfpRhoModuleCtl[i].funcNewPartnerProcess)
		{
		    /* Do not care about the return value, because it does not matter. */
			gAppGfpRhoModuleCtl[i].funcNewPartnerProcess(NULL, NULL, 0);
		}
	}

	return TRUE;
}

#endif /* GFP_MCSYNC_ENABLE */

#endif /* AIR_GFP_ENABLE */

>>>>>>> db20e11 (second commit)
