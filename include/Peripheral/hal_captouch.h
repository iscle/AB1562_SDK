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

#ifndef __HAL_CAPTOUCH_H__
#define __HAL_CAPTOUCH_H__

#include "hal_platform.h"
<<<<<<< HEAD

=======
#include "log.h"

#define HAL_CAPTOUCH_LOG_PRINT(fmt, cnt, arg...)             LOG_MSGID_I(Captouch, fmt, cnt, ##arg)
>>>>>>> db20e11 (second commit)

#ifdef HAL_CAPTOUCH_MODULE_ENABLED
//#include "hal_keypad_table.h"
#include "types.h"
#include "nvkey_captouch_mgr_nvstruc.h"
#include "ab1562.h"
/**
 * @addtogroup HAL
 * @{
 * @addtogroup CAPTOUCH
 * @{
 * This section introduces the captouch driver APIs including terms and acronyms, supported features,
 * software architecture, details on how to use this driver, enums, structures and functions.

 * @section HAL_CAPTOUCH_Terms_Chapter Terms and acronyms
 *
 *
 * |Terms                         |Details                                                                 |
 * |------------------------------|------------------------------------------------------------------------|
 * |\b GPIO                       | For an introduction to General Purpose Inputs-Outputs, please refer to the @ref GPIO module in HAL. |
 * |\b Captouch                   | Capacitive touch controler is used for human interfacing. When a human finger touches the sensing part, usually a part of system HW, it forms a capacitance where one plate is the conductive layer and the other being the human finger.|
 * @section HAL_CAPTOUCH_Features_Chapter Supported features
 *
 * This module provides a generic design to get external touch events.
 * The captouch provides two ways to use, the normal usage as below, by the detail description of automatic calibration usage.
 *
 * - \b Normal \b usage.\n
 *  In this usage, an interrupt is triggered whenever a pad is pressed or released.
 *  A callback function can be registered for the interrupt. The callback function is invoked whenever a pad is pressed or released.
 *  #hal_captouch_get_event() can be used in the callback function to get the key event and the key position number.
 * - \b tunning \b usange.\n
 *  In this usage, the user can tune the coarse cap by #hal_captouch_tune_control().\n
 * @}
 * @}
 */

/**
 * @addtogroup HAL
 * @{
 * @addtogroup CAPTOUCH
 * @{
 * @section HAL_CAPTOUCH_Driver_Usage_Chapter How to use this driver
 *
 * - \b Using \b the \b normal.\n
 *    To use the catpouch driver in normal mode, please refer to @ref GPIO datasheet to determine which GPIOs should be selected to pinmux to captouch pins.
 *    Then call #hal_captouch_init() to manually set the channel bit map, register the callback function, the default coarse cap and the threshold value.
 *    After calling #hal_captouch_init(), the driver can work.
 *    If a key is pressed or released on the pad, the pad triggers an interrupt to call the callback function.
 *    The user should use #hal_captouch_get_event() in the callback function to get the key event and the key position number.
 *    To ensure a reliable key scan, do not overload the callback function. Let the callback return as quickly as possible.
 *  - Step1. Call #hal_captouch_init() to initialize the captouch module.
 *  - Step2. Call #hal_captouch_deinit() to de-initialize the captouch module if it is no longer in use.
 *  - Sample code:
 *    @code
 *      hal_captouch_config_t config;
 *      hal_captouch_tune_data_t tune_data;
 *      uint32_t i;
 *
 *      config.callback.callbck  = user_captouch_callback;           // Register callback
 *      config.callback.userdata = NULL;                             // User's callback parameter pointer
 *      config.channel_bit_map   = 0xff;                             // Sets the channel0~7 enable
 *      for(i=0;i<8;i++) {
 *          config.high_thr[i] = 210;                                // Sets the high threshold value
 *          config.low_thr[i]  = 100;                                // Sets the low threshold value
 *          config.coarse_cap[i] = 2;                                // Sets the coarse cap value
 *      }
 *      hal_captouch_init(&config);
 *
 *      ...
 *      hal_captouch_deinit();                                       // De-initialize the captouch module if it is no longer in use.
 *    @endcode
 *    @code
 *       // Callback function. This function should be registered with #hal_captouch_init().
 *       void user_captouch_callback (void *user_data)
 *       {
 *          // Get the key press or release event.
 *          hal_captouch_event_t key_event;
 *          uint32_t key_symbol;
 *          hal_captouch_status_t ret;
 *
 *          while(1) {
 *              ret = hal_captouch_get_event(&key_event);
 *              if (ret == HAL_CAPTOUCH_STATUS_NO_EVENT) {
 *                  break;
 *              }
 *           }
 *       }
 *
 *    @endcode
 *
 * - \b Using \b the \b tunning \b usage.\n
 *    Then tunning usage is to find the proper coarse cap value by channel.
 *  - Step1. Call #hal_captouch_init() to initialize the captouch module.
 *    Step2. Call #hal_captouch_tune_control() to find the proper coarse cap value.
 *  - Step3. Call #hal_captouch_deinit() to de-initialize the captouch module if it is no longer in use.
 *  - Sample code:
 *    @code
 *      hal_captouch_config_t config;
 *      hal_captouch_tune_data_t tune_data;
 *      uint32_t i;
 *
 *      config.callback.callbck  = user_captouch_callback;           // Register callback
 *      config.callback.userdata = NULL;                             // User's callback parameter pointer
 *      config.channel_bit_map   = 0xff;                             // Sets the channel0~7 enable
 *      for(i=0;i<8;i++) {
 *          config.high_thr[i] = 210;                                // Sets the high threshold value
 *          config.low_thr[i]  = 100;                                // Sets the low threshold value
 *          config.coarse_cap[i] = 2;                                // Sets the coarse cap value
 *      }
 *      hal_captouch_init(&config);
 *
 *      for(i=0;i<8;i++) {
 *          // The tune_data can be stored in memory, and filled by #hal_captouch_init().
 *          hal_captouch_tune_control(i,HAL_CAPTOUCH_TUNE_SW_AUTO,&tune_data);  // If tuning the 8 channels.
 *      }
 *      ...
 *      hal_captouch_deinit();                                       // De-initialize the captouch module if it is no longer in use.
 *    @endcode
 * @}
 * @}
 */


/**
 * @addtogroup HAL
 * @{
 * @addtogroup CAPTOUCH
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup hal_captouch_typedef Typedef
  * @{
  */

/** @brief  This function registers a callback function when in a normal usage.
 *          This function is called after the key is pressed or released in the captouch ISR routine.
 * @param [in] user_data: This variable pointer is defined by the user to record the data.
 */
typedef void (*hal_captouch_callback_t)(void *user_data);


/**
  * @}
  */

/** @defgroup hal_keypad_enum Enum
  * @{
  */



/** @brief This enum defines captouch status */
typedef enum {
    HAL_CAPTOUCH_STATUS_UNEXPECTED_ERROR = -7,      /**< An unexpected error. */
    HAL_CAPTOUCH_STATUS_CHANNEL_ERROR = -6,      /**< The channel number error. */
    HAL_CAPTOUCH_STATUS_NO_EVENT = -5,      /**< No event in buffer. */
    HAL_CAPTOUCH_STATUS_UNINITIALIZED = -4,      /**< The driver is uninitialized. */
    HAL_CAPTOUCH_STATUS_INITIALIZED = -3,      /**< The driver is initialized. */
    HAL_CAPTOUCH_STATUS_ERROR = -2,      /**< A function error. */
    HAL_CAPTOUCH_STATUS_INVALID_PARAMETER = -1,      /**< An invalid parameter. */
    HAL_CAPTOUCH_STATUS_OK = 0                     /**< No error occurred during the function call. */
} hal_captouch_status_t;

/** @brief This enum defines key status */
typedef enum {
    HAL_CAP_TOUCH_KEY_RELEASE = 0,        /**< A key is released. */
    HAL_CAP_TOUCH_KEY_PRESS = 1,        /**< A key is pressed. */
    HAL_CAP_TOUCH_KEY_OPERATION_MAX
} hal_captouch_key_state_t;
/**
  * @}
  */

/** @defgroup hal_captouch_struct Struct
  * @{
  */


/** @brief This structure defines callback parameter */
typedef struct {
    hal_captouch_callback_t callback;                       /**< The callback function.*/
    void *user_data;                                        /**< The callback function parameter pointer.*/
} hal_captouch_callback_context_t;

/** @brief This structure defines configuration parameter */
typedef struct {
    hal_captouch_callback_context_t callback;              /**< The callback parameter.*/
    uint8_t channel_bit_map;                               /**< The channel bit map; one bit specifies one channel. */
    uint8_t mavg_r[3];                                 /**< Touch control moving avergae times. */
    uint8_t avg_s[3];                                  /**< Touch control ADC in average times. */
    uint8_t coarse_cap[3];                                 /**< The channel coarse cap value. */
    S16 high_thr[3];                                      /**< The channel high threshold. */
    S16 low_thr[3];                                       /**< The channel low threshold. */
    S16 high_nthr[3];                                     /**< The channel high Nthreshold. */
    S16 low_nthr[3];                                      /**< The channel low Nthreshold. */
} hal_captouch_config_t;


/** @brief This structure defines tune type */
typedef enum {
    HAL_CAPTOUCH_TUNE_HW_AUTO = 0,                          /**< Specify the hardware automatic tune mode. */
    HAL_CAPTOUCH_TUNE_SW_AUTO = 1                           /**< Specify the software automatic tune mode. */
} hal_captouch_tune_type_t;


/** @brief This structure defines power mode type */
typedef enum {
    HAL_CAPTOUCH_MODE_NORMAL = 0,                          /**< Specify the normal mode */
    HAL_CAPTOUCH_MODE_LOWPOWER = 1                          /**< Specify the low-power mode */
} hal_captouch_lowpower_type_t;

/** @brief This structure defines clk */
typedef enum {
    HAL_CAPTOUCH_CLK_32K = 0,                         /**< Specify clock 32k */
    HAL_CAPTOUCH_CLK_16K = 1,                         /**< Specify clock 16k */
    HAL_CAPTOUCH_CLK_8K = 2,                          /**< Specify clock 8k */
    HAL_CAPTOUCH_CLK_4K = 3,                          /**< Specify clock 4k */
    HAL_CAPTOUCH_CLK_2K = 4,                          /**< Specify clock 2k */
    HAL_CAPTOUCH_CLK_1K = 5                           /**< Specify clock 1k */
} hal_captouch_clock_t;

/** @brief This structure defines tune data */
typedef struct {
    uint8_t coarse_cap;                                    /**< Specify the coarese cap value. */
    S16 avg_adc;                                       /**< Specify the current average adc value. */
    S16 vadc;                                          /**< Specify the current adc value. */
    S16 mavg_dbg;                                      /**< Specify the current moving average adc value. */
    S16 man;                                           /**< Specify the current fine cap vavlue of the manual mode. */
    S16 fine_cap;                                      /**< Specify the calibration value. */
    S16 cfine_sw_dbg;                                  /**< Specify the cfine base value. */
    S16 adc_debounce;                                  /**< Specify the adc debounce state. */
    S16 cfine_sw_add_mavg_dbg;                         /**< Specify the cfine value add moving avg adc*/
    uint8_t noise_cnt_race;
} hal_captouch_tune_data_t;

/** @brief This structure defines coarse cap calibration */
typedef struct {
    uint8_t result;
    uint8_t coarse_value;
}hal_captouch_cal_result_t;

typedef struct {
    hal_captouch_cal_result_t coarse_cal[4];
}hal_captouch_coarse_cal_t;

/** @brief This structure defines key event data */
typedef struct {
    hal_captouch_key_state_t state;                         /**< Specifies the key status, such as released or pressed etc. */
    uint8_t channel;                                     /**< Specifies the key position number on the channel.*/
    uint32_t time_stamp;                                  /**< Specifies the time stamp of the event on the channel.*/
} hal_captouch_event_t;

/** @brief This structure defines channel */
typedef enum {
    HAL_CAPTOUCH_CHANNEL_0 = 0,                             /**< Specifies the channel0. */
    HAL_CAPTOUCH_CHANNEL_1 = 1,                             /**< Specifies the channel1. */
    HAL_CAPTOUCH_CHANNEL_2 = 2,                             /**< Specifies the channel2. */
    HAL_CAPTOUCH_CHANNEL_MAX = 3                            /**< Specifies the maximum channel number. */
} hal_captouch_channel_t;

/** @brief This structure defines longpress type */
typedef enum {
    HAL_CAPTOUCH_LONGPRESS_SHUTDOWN = 0,                        /**< Specify captouch for longpress shutdown. */
    HAL_CAPTOUCH_LONGPRESS_WAKEUP = 1,                           /**< Specify captouch for longpress wakeup. */
    HAL_CAPTOUCH_LONGPRESS_SHUTDOWN_VBUS = 2            /**< Specify captouch for longpress shutdown with vbus */
} hal_captouch_longpress_type_t;

/**
  * @}
  */

/**
 * @brief     This function initializes the captouch module.
 *            Call this function if the captouch is required.
 * @return    #HAL_CAPTOUCH_STATUS_OK, if operation is successful.\n
 * @sa        #hal_captouch_deinit()
 */
hal_captouch_status_t hal_captouch_init(void);

/**
 * @brief     This function deinitializes the captouch module.
 *            Call this function if the captouch is no longer in use.
 * @return    #HAL_CAPTOUCH_STATUS_OK, if operation is successful.\n
 * @sa        #hal_captouch_init()
 */
hal_captouch_status_t hal_captouch_deinit(void);

/**
 * @brief     This function enables the channel. It will enable the sensing of the channel interrupt, and the channel wakeup interrupt.
 *            This API and the #hal_captouch_channel_disable() are codependent. It must be called after #hal_captouch_init() if necessary.
 * @param[in] channel is the channel number. For more details, please refer to #hal_captouch_channel_t.
 * @return    #HAL_CAPTOUCH_STATUS_OK, if the operation is successful.\n
 * @sa        #hal_captouch_channel_disable()
 */
hal_captouch_status_t hal_captouch_channel_enable(hal_captouch_channel_t channel);


/**
 * @brief     This function disables the channel. It will disable the sensing, the channel interrupt, and the channel wakeup interrupt.
 *            This API and the #hal_captouch_channel_enable() are the independ, it must be called after #hal_captouch_init() if necessary.
 * @param[in] channel is the channel number. For more details, please refer to #hal_captouch_channel_t.
 * @return    #HAL_CAPTOUCH_STATUS_OK, if the operation is successful.\n
 * @sa        #hal_captouch_channel_enable()
 */
hal_captouch_status_t hal_captouch_channel_disable(hal_captouch_channel_t channel);

/**
 * @brief     This function gets the key event data.
 * @param[out] event is the pointer to key event data. For more details, please refer to #hal_captouch_event_t.
 * @return    #HAL_CAPTOUCH_STATUS_OK, if the operation is successful.\n
 */
hal_captouch_status_t hal_captouch_get_event(hal_captouch_event_t *event);

/**
 * @brief     This function changes the high threshold and the low threshold by channel.
 * @param[in] channel is the channel number. For more details, please refer to #hal_captouch_channel_t.
 * @param[in] high_thr is the high threshold value. When the average adc value is greater than this value, it will trigger the press event.
 *            The range of the value should be -256<=high_thr<=255.
 * @param[in] low_thr is the low threshold value. When the average adc value is less than this value, it will trigger the release event.
 *            The range of the value should be -256<=high_thr<=255.
 * @return    #HAL_CAPTOUCH_STATUS_OK, if the operation is successful.\n
 */

hal_captouch_status_t hal_captouch_set_threshold(hal_captouch_channel_t channel, S16 high_thr, S16 low_thr);
//hal_captouch_status_t hal_captouch_set_threshold(hal_captouch_channel_t channel,int32_t high_thr, int32_t low_thr);

/**
 * @brief     This function changes the high  noise threshold and the low noise threshold by channel.
 * @param[in] channel is the channel number. For more details, please refer to #hal_captouch_channel_t.
 * @param[in] high_thr is the high threshold value. When the average adc value is greater than this value, it will trigger the press event.
 *            The range of the value should be -256<=high_thr<=255.
 * @param[in] low_thr is the low threshold value. When the average adc value is less than this value, it will trigger the release event.
 *            The range of the value should be -256<=high_thr<=255.
 * @return    #HAL_CAPTOUCH_STATUS_OK, if the operation is successful.\n
 */

hal_captouch_status_t hal_captouch_set_Nthreshold(hal_captouch_channel_t channel, S16 high_thr, S16 low_thr);



/**
 * @brief     This function changes the fine cap value. This API can be used only in manual mode.
 * @param[in] channel is the channel number. For more details, please refer to #hal_captouch_channel_t.
 * @param[in] fine is the fine cap value. If in the same coarse cap value, the fine cap value is bigger, the average and the vadc will be smaller.
 *            The range of the value should be -64<=fine<=63.
 * @return    #HAL_CAPTOUCH_STATUS_OK, if the operation is successful.\n
 */

hal_captouch_status_t hal_captouch_set_fine_cap(hal_captouch_channel_t channel, S8 fine);
//hal_captouch_status_t hal_captouch_set_fine_cap(hal_captouch_channel_t channel,int32_t fine);


/**
 * @brief     This function change the coarese cap value.
 * @param[in] channel is the channel number. For more details, please refer to #hal_captouch_channel_t.
 * @param[in] coarse is the corase cap value. If in the same fine cap value, the coarse cap value is bigger, the average and the vadc will be smaller.
 *            The range of the value should be 0<=coarse<=7.
 * @return    #HAL_CAPTOUCH_STATUS_OK, if the operation is successful.\n
 */
hal_captouch_status_t hal_captouch_set_coarse_cap(hal_captouch_channel_t channel, uint32_t coarse);

/**
 * @brief     This function tunes the calibration value by channel.
 * @param[in] channel is the channel number. For more details, please refer to #hal_captouch_channel_t.
 * @param[in] tune_type is the tune type mode. For more details, please refer to #hal_captouch_tune_type_t.
 * @param[out] data is the pointer tune result.
 * @return    #HAL_CAPTOUCH_STATUS_OK, if the operation is successful.\n
 */
hal_captouch_status_t hal_captouch_tune_control(hal_captouch_channel_t channel, hal_captouch_tune_type_t tune_type);

/**
 * @brief     This function change the coarese cap value.
 * @param[in] channel is the channel number. For more details, please refer to #hal_captouch_channel_t.
 * @param[in] mavg_val is the touch control moving average times.
 * @param[in] avg_val is the touch control ADC in average times.
 * @return    #HAL_CAPTOUCH_STATUS_OK, if the operation is successful.\n
 */
hal_captouch_status_t hal_captouch_set_avg(hal_captouch_channel_t channel, uint8_t mavg_val, uint8_t avg_val);

/**
 * @brief     This function controls when the captouch goes into normal or low-power mode. It can be used after #hal_captouch_init().
 * @param[in] lowpower_type is the type value. For more details, please refer to #hal_captouch_lowpower_type_t.
 * @return    #HAL_CAPTOUCH_STATUS_OK, if the operation is successful.\n
 */
hal_captouch_status_t hal_captouch_lowpower_control(hal_captouch_lowpower_type_t lowpower_type);
void hal_captouch_disable_sw_tune(void);
void hal_captouch_coarse_cap_tune(hal_captouch_coarse_cal_t* coarse_cal);
hal_captouch_status_t hal_captouch_power_on_init(bool is_en);
void hal_captouch_set_vbus_flag(bool is_chgin);
void hal_captouch_set_lpsd_chk_vbus(bool is_en);
#ifdef __cplusplus
}
#endif
/**
* @}
* @}
*/
#endif /* HAL_CAPTOUCH_MODULE_ENABLED */
#endif /* __HAL_CAPTOUCH_H__ */

