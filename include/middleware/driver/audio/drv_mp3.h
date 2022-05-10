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

#ifndef _DRV_MP3_
#define _DRV_MP3_

#ifdef MP3_LOCAL_PLAYBACK
/**************************************************************************************************
* Enumeration
**************************************************************************************************/
enum
{
	DRIVE_eMMC = 0,
	DRIVE_USB,
	DRIVE_INTERNAL_FLASH,
	DRIVE_SD_CARD,
	DRIVE_NUM
};//Drive Type

enum
{
    DRV_MP3_ID_INIT_OK,
    DRV_MP3_ID_POWER_OFF_REQ,
};

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void drv_mp3_init_drive(Handler app, U8 type);
BOOL drv_mp3_deinit_drive();
BOOL drv_mp3_start();
BOOL drv_mp3_stop(BOOL is_suspend);

void drv_mp3_resume();
void drv_mp3_pause();
void drv_mp3_track_next();
void drv_mp3_track_prev();
void drv_mp3_track_head();
void drv_mp3_track_end();
void drv_mp3_track_last();
void drv_mp3_play_once();
void drv_mp3_play_order();
void drv_mp3_play_once_repeat();
void drv_mp3_play_all_repeat();
void drv_mp3_play_shuffle();

BOOL drv_mp3_is_dsp_playing();
BOOL drv_mp3_is_user_playing();
void drv_mp3_remember_last_status();
#endif //MP3_LOCAL_PLAYBACK

#endif //_DRV_MP3_
