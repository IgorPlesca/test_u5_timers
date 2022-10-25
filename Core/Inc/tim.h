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
typedef enum Timer_e
{
	TIMER_1 = 0x00U,
	TIMER_2,
	TIMER_3,
	TIMER_4,
	TIMER_5,
	TIMER_6,
	TIMER_7,
	TIMER_8,
	TIMER_NUM_MAX,
} Timer_t;

typedef enum TimerChannel_e
{
	TIMER_CH1 = 0x00U,
	TIMER_CH2,
	TIMER_CH3,
	TIMER_CH4,
	TIMER_CH_NUM_MAX,
} TimerChannel_t;

typedef struct PwmChannel_s
{
	Timer_t 	   timer;
	TimerChannel_t timerChannel;
} PwmChannel_t;

/* USER CODE END Private defines */

void MX_TIM1_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* USER CODE BEGIN Prototypes */
void tim_PwmChannelConfig              (const PwmChannel_t *pwmChannel, uint32_t pwmFreqHz, uint32_t pwmPulseUsAcc);
void tim_PwmChannelSetPulseMicroseconds(const PwmChannel_t *pwmChannel, uint32_t pwmPulseUs);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

