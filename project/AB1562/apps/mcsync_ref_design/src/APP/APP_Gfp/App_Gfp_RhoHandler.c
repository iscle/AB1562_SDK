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
#if defined(AIR_GFP_ENABLE)
#include "gfp_features.h"
#ifdef GFP_MCSYNC_ENABLE
#include "log.h"
#include "gfps_api.h"
#include "App_Gfp.h"
#include "App_Gfp_RhoHandler.h"


/**************************************************************************************************
* Definition
**************************************************************************************************/

#define APP_GFP_RHO_LOG_MSGID_I(_message, arg_cnt, ...) LOG_MSGID_I(APP_GFP, "[APP_GFP]RHO "_message, arg_cnt, ##__VA_ARGS__)

#define INVALID_MODULE_INDEX 0xFF

/**************************************************************************************************
* Enumeration
**************************************************************************************************/
typedef enum
{
    APP_GFP_RHO_MODULE_ID_GFP,
	APP_GFP_RHO_MODULE_ID_SPP,

	APP_GFP_RHO_MODULE_ID_NUM
}app_gfp_rho_module_id_enum;

/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct
{
	U8 moduleId;
	U16 length; /* pData length only */
	U8 pData[0];
}PACKED app_gfp_rho_module_data_struct;


typedef struct
{
	U8 moduleId;
	U16 (*funcGetLength)(void);
	BOOL (*funcGetData)(U8 *pBuffer);
	BOOL (*funcNewAgentProcess)(BD_ADDR_T *pBdAddr, U8 *pData, U16 length);
	BOOL (*funcNewPartnerProcess)(BD_ADDR_T *pBdAddr, U8 *pData, U16 length);
}app_gfp_rho_module_ctl_struct;

/**************************************************************************************************
* Variable
**************************************************************************************************/
static const app_gfp_rho_module_ctl_struct gAppGfpRhoModuleCtl[APP_GFP_RHO_MODULE_ID_NUM] =
{
    {APP_GFP_RHO_MODULE_ID_GFP, gfp_rho_get_data_length, gfp_rho_get_data, gfp_rho_new_agent_process, gfp_rho_new_partner_process},
	{APP_GFP_RHO_MODULE_ID_SPP, NULL, NULL, NULL, NULL},
};

/**************************************************************************************************
* Static Functions
**************************************************************************************************/
static U8 APP_Gfp_RhoGetIndexByModuleId(U8 moduleId)
{
	U8 moduleIndex;

	for (moduleIndex = 0; moduleIndex < APP_GFP_RHO_MODULE_ID_NUM; moduleIndex++)
	{
		if (gAppGfpRhoModuleCtl[moduleIndex].moduleId == moduleId)
		{
			return moduleIndex;
		}
	}

	return APP_GFP_RHO_MODULE_ID_NUM;
}


/**************************************************************************************************
* Public Functions
**************************************************************************************************/
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
	}

	return TRUE;
}


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
	UNUSED(pBdAddr);
	UNUSED(pData);
	UNUSED(length);

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

