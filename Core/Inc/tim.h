/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.h
  * @brief   This file contains all the function prototypes for
  *          the tim.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern TIM_HandleTypeDef htim1;

/* USER CODE BEGIN Private defines */
/* Timer PWM Channels enumeratives */
typedef enum TimerPwmChanel_e
{
	TIMER_PWM_CHANNEL_1 = 0x00,
	TIMER_PWM_CHANNEL_2,
	TIMER_PWM_CHANNEL_3,
	TIMER_PWM_CHANNEL_4,

	TIMER_PWM_CHANNEL_NUM_MAX
} TimerPwmChanel_t;
/* USER CODE END Private defines */

void MX_TIM1_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* USER CODE BEGIN Prototypes */
/*
 * Config the Timer Channel as PWM
 */
void tim_PwmChannelConfig(TimerPwmChanel_t timerPwmChannel, uint32_t pwmFreqHz, uint32_t pwmPulseDurationUsGranularity);

/*
 *  Stop the selected Timer's PWM
 */
void tim_PwmChannelStop(TimerPwmChanel_t timerPwmChannel);

/*
 *  Configure the pulse of the selected Timer's PWM
 */
void tim_PwmChannelSetPulse(TimerPwmChanel_t timerPwmChannel, uint32_t pwmPulseDurationUs);

/*
 *  Start the selected Timer's PWM
 */
void tim_PwmChannelStart(TimerPwmChanel_t timerPwmChannel);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

