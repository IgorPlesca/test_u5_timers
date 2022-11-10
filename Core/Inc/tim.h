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
/* Timer definitions */
typedef enum PwmChannel_e
{
	PWM_CHANNEL_1 = 0x00,
	PWM_CHANNEL_2,
	PWM_CHANNEL_3,
	PWM_CHANNEL_4,

	PWM_CHANNEL_NUM_MAX,
} PwmChannel_t;

typedef uint32_t PwmPulseConfig_t[PWM_CHANNEL_NUM_MAX] ;

/* USER CODE END Private defines */

void MX_TIM1_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* USER CODE BEGIN Prototypes */
/*
 * Config the Timer Channel as PWM
 */
void tim_PwmChannelConfig(uint32_t pwmFreqHz, uint32_t pwmPulseUsPrecision);

/*
 * Set the Timer Channel PWM pulse duration (in us)
 */
void tim_PwmChannelSetPulseDuration(PwmPulseConfig_t pwmPulseUsConfig);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

