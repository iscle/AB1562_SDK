#ifndef _AUDIO_BIST_SDK_H_
#define _AUDIO_BIST_SDK_H_


/**************************************************************************************************
* Definition
**************************************************************************************************/
/*vvv Please only choose one define vvv*/
#define AB1531_1532_CP_TEST
//#define AB1533_CP_TEST
/*^^^ Please only choose one define ^^^*/

/*Modify result GPIO and mode change GPIO here*/
#ifdef AB1531_1532_CP_TEST
	#define STATUS_GPIO_RESULT			GPIO28
	#define STATUS_GPIO_MODE			GPIO17
#elif defined(AB1533_CP_TEST)
	#define STATUS_GPIO_RESULT			GPIO15
	#define STATUS_GPIO_MODE			GPIO12
#else
	#define STATUS_GPIO_RESULT			GPIO15
	#define STATUS_GPIO_MODE			GPIO12
#endif

/*Delay enter deep sleep time*/
#define DELAY_TIME_ENTER_DEEP_SLEEP	150 //unit: ms

/**************************************************************************************************
* Public Function
**************************************************************************************************/
VOID AudioBIST_CP_L_Configuration (VOID);
VOID AudioBIST_CP_R_Configuration (VOID);


#endif //_AUDIO_BIST_SDK_H_
