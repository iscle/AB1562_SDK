#ifndef _DRV_RECORD_H_
#define _DRV_RECORD_H_
#include "bt.h"

/******************************************************************************
 * Definition
*******************************************************************************/
#define RACE_ADAPTIVE_ANC_CHECK_CMD_LD_MODE            0x00
#define RACE_ADAPTIVE_ANC_CHECK_CMD_IH_DETECT_MODE     0x08
#if LAB_TUNNING_LIB
#define DSP_RECORD_FRAME_SIZE (U32)480
#define DSP_RECORD_BUFFER 10
#define RACE_ADAPTIVE_ANC_CHECK_CMD_RECORD_MODE_MIC_SZ              0x03 //trigger adaptive FF ANC and return mic data(echo ref & FB mic)
#define RACE_ADAPTIVE_ANC_CHECK_CMD_RECORD_MODE_MIC_WZ              0x04 //trigger adaptive FF ANC and return mic data(FF mic & FB mic)
#define RACE_ADAPTIVE_ANC_CHECK_CMD_RECORD_MODE_CALIBRATION_SETTING 0x05 //Set ANC input for FBC calibration
#define RACE_ADAPTIVE_ANC_CHECK_CMD_RECORD_MODE_FBC                 0x06 //trigger Hybrid ANC and return data(FB mic & FBC data)
#define RACE_ADAPTIVE_ANC_CHECK_RECORD_SIZE DSP_RECORD_FRAME_SIZE*200
typedef enum
{
    RECORD_MODE_NULL,
    RECORD_MODE_MIC_SZ,
    RECORD_MODE_MIC_WZ,
    RECORD_MODE_FBC,
}record_mode_t;

enum
{
	RECORD_TIMER_RESUME_DELAY,
};


#endif//LAB_TUNNING_LIB

#define RACE_ADAPTIVE_ANC_CHECK_RSP_SUCCESS 0x00
#define RACE_ADAPTIVE_ANC_CHECK_RSP_FAIL    0xFF



/******************************************************************************
 * Structure
*******************************************************************************/

/**************************************************************************************************
* Public Function
**************************************************************************************************/
#if LAB_TUNNING_LIB
BOOL drv_recorder_init(U8 channelId, U32 recordSize);
BOOL drv_recorder_start(record_mode_t mode);
BOOL drv_recorder_stop(BOOL is_suspend);
BOOL drv_recorder_is_streaming(void);
BOOL drv_recorder_set_pause(BOOL isPause);
void drv_recorder_resume_delay(U32 delay);
#endif//LAB_TUNNING_LIB

#endif//_DRV_RECORD_H_
