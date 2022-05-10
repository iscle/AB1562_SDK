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
#include "race_cmd.h"
#include "APP_HfpScoHandler.h"
#include "drv_sco.h"

<<<<<<< HEAD
#if (defined(MTK_INEAR_ENHANCEMENT_ENABLE) || defined(MTK_DUALMIC_INEAR_ENABLE))
=======
#if (defined(AIR_TXNR_1_MIC_INEAR_ENABLE) || defined(AIR_TXNR_2_MIC_INEAR_ENABLE))
>>>>>>> db20e11 (second commit)
#define dumpsize 48
#else
#define dumpsize 36 //sizeof(S16)*(AEC_PRELIM_COEF_SIZE+AEC_REF_GAIN_SIZE)
#endif
#define buffersize dumpsize*5


extern BOOL AEC_NR_DumpEnable(U8 state);
extern U16 DSP_DataDump_CheckRemainSize(VOID);
extern BOOL DSP_DataDump_ExtractData(U8* Buf, U32 Length);


static U32 app_racecmd_airdump_race_handler(Handler app, U16 id, void *msg, U32 id_ext);
static U32 app_racecmd_airdump_dsp_handler(Handler handler, U16 id, void *msg, U32 ext_id);

static const HandlerData gAppAirDumpRaceHandle = { app_racecmd_airdump_race_handler };
static const HandlerData gAppAirDumpDspHandle = { app_racecmd_airdump_dsp_handler };

static U8 gAppAirDumpRaceChannel;

static U32 app_racecmd_airdump_race_handler(Handler app, U16 id, void *msg, U32 id_ext)
{
	UNUSED(app);
	UNUSED(id);
	UNUSED(id_ext);

	U8 rsp;
	BD_ADDR_T *pBdAddr;
	MSG_ID_RACE_TO_APP_AIRDUMP_CMD_T *pMsg = (MSG_ID_RACE_TO_APP_AIRDUMP_CMD_T *)msg;

	rsp = AEC_NR_DumpEnable(pMsg->status);
	RACE_AirDump_Send(pMsg->channelId,RACE_TYPE_RESPONSE,(U8*)&rsp,1); // Send command response

	if(rsp)
		return 0;

	if(pMsg->status == 0)
		gAppAirDumpRaceChannel = 0; //disabled if dump end
    else
        gAppAirDumpRaceChannel = pMsg->channelId;

    pBdAddr = drv_sco_get_bdaddr();
    if(pBdAddr)
    {
        APP_HfpSco_SetRetransmissionEnable(pBdAddr,pMsg->status? 1:0);
    }
	return 0;
}



static void app_racecmd_airdump_dsp_event_handler(void)
{
	U8 Buff_DUMP[buffersize];
	if(DSP_DataDump_CheckRemainSize()>= buffersize)
	{
		DSP_DataDump_ExtractData((U8*)&Buff_DUMP,buffersize);
		if(gAppAirDumpRaceChannel)
			RACE_AirDump_Send(gAppAirDumpRaceChannel,RACE_TYPE_NOTIFICATION,(U8*)&Buff_DUMP, buffersize); // Send dump data
	}

}

static U32 app_racecmd_airdump_dsp_handler(Handler handler, U16 id, void *msg, U32 ext_id)
{
	UNUSED(handler);
	UNUSED(ext_id);
	UNUSED(msg);

	switch(id)
	{
		case MESSAGE_ID_AIRDUMP:
			app_racecmd_airdump_dsp_event_handler();
			break;
	}

	return 0;
}

void APP_RaceCmd_AirDump_Init(void)
{
    RACE_AirDump_RegisterAppHandler((Handler)&gAppAirDumpRaceHandle);
    drv_sco_set_airdump_handle((Handler)&gAppAirDumpDspHandle);
}

