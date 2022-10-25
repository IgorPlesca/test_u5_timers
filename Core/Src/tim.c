/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
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
/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* USER CODE BEGIN 0 */
typedef struct TimerConfig_s
{
	TIM_TypeDef       *instance;
	TIM_HandleTypeDef *handler;
} TimerConfig_s;

static TimerConfig_s KT_TimerConfig[TIMER_NUM_MAX] =
{
		{NULL, NULL  }, /* TIMER_1 */
		{TIM2, &htim2}, /* TIMER_2 */
		{NULL, NULL  }, /* TIMER_3 */
		{NULL, NULL  }, /* TIMER_4 */
		{NULL, NULL  }, /* TIMER_5 */
		{NULL, NULL  }, /* TIMER_6 */
		{NULL, NULL  }, /* TIMER_7 */
		{NULL, NULL  }, /* TIMER_8 */
};

static uint32_t KT_TimerChannels[TIMER_CH_NUM_MAX] =
{
		TIM_CHANNEL_1,  /* TIMER_CH1 */
		TIM_CHANNEL_2,  /* TIMER_CH2 */
		TIM_CHANNEL_3,  /* TIMER_CH3 */
		TIM_CHANNEL_4,  /* TIMER_CH4 */
};

static uint32_t tim_GetTimerBusClock(void);
/* USER CODE END 0 */

TIM_HandleTypeDef htim2;

/* TIM2 init function */
void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 4;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 19999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspInit 0 */

  /* USER CODE END TIM2_MspInit 0 */
    /* TIM2 clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();
  /* USER CODE BEGIN TIM2_MspInit 1 */

  /* USER CODE END TIM2_MspInit 1 */
  }
}
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(timHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspPostInit 0 */

  /* USER CODE END TIM2_MspPostInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**TIM2 GPIO Configuration
    PB11     ------> TIM2_CH4
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM2_MspPostInit 1 */

  /* USER CODE END TIM2_MspPostInit 1 */
  }

}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspDeInit 0 */

  /* USER CODE END TIM2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM2_CLK_DISABLE();
  /* USER CODE BEGIN TIM2_MspDeInit 1 */

  /* USER CODE END TIM2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void tim_PwmChannelConfig(PwmChannel_t *pwmChannel, uint32_t pwmFreqHz, uint32_t pwmPulseUsAcc)
{
	TIM_HandleTypeDef 		*pTimHandler;
	uint32_t           		timChannel;

	uint32_t 				timerBusClock      = 0uL;
	uint32_t 				timerClock         = 0uL;
	uint32_t 				counterPeriod      = 0uL;
	uint32_t 				prescaler          = 0uL;
	TIM_ClockConfigTypeDef 	sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig      = {0};

	pTimHandler = KT_TimerConfig[pwmChannel->timer].handler;
	if(NULL == pTimHandler)
	{
		Error_Handler();
	}

	timChannel  = KT_TimerChannels[pwmChannel->timerChannel];

	timerBusClock = tim_GetTimerBusClock();
	timerClock    = 1000000uL     / pwmPulseUsAcc;
	prescaler     = timerBusClock / timerClock;
	counterPeriod = timerClock    / pwmFreqHz;

	HAL_TIM_PWM_Stop(pTimHandler, timChannel);
	pTimHandler->Instance = KT_TimerConfig[pwmChannel->timer].instance;
	pTimHandler->Init.Prescaler = prescaler;
	pTimHandler->Init.CounterMode = TIM_COUNTERMODE_UP;
	pTimHandler->Init.Period = counterPeriod;
	pTimHandler->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	pTimHandler->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(pTimHandler) != HAL_OK)
	{
		Error_Handler();
	}

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(pTimHandler, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_TIM_PWM_Init(pTimHandler) != HAL_OK)
	{
		Error_Handler();
	}

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(pTimHandler, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}

}

void tim_PwmChannelSetPulseMicroseconds(PwmChannel_t *pwmChannel, uint32_t pwmPulseUs)
{
	TIM_HandleTypeDef	*pTimHandler;
	uint32_t         	timChannel;

	TIM_OC_InitTypeDef 	sConfigOC 		= {0};

	pTimHandler = KT_TimerConfig[pwmChannel->timer].handler;
	if(NULL == pTimHandler)
	{
		Error_Handler();
	}

	timChannel  = KT_TimerChannels[pwmChannel->timerChannel];

	HAL_TIM_PWM_Stop(pTimHandler, timChannel);

	sConfigOC.OCMode     = TIM_OCMODE_PWM1;
	sConfigOC.Pulse      = pwmPulseUs;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(pTimHandler, &sConfigOC, timChannel) != HAL_OK)
	{
		Error_Handler();
	}

	HAL_TIM_PWM_Start(pTimHandler, timChannel);
}

static uint32_t tim_GetTimerBusClock(void)
{
	uint32_t hclk = 0uL;
	uint8_t  apb2Divider = 0u;
	uint32_t pFLatency = 0uL;
	uint32_t timerClock = 0uL;
	RCC_ClkInitTypeDef pRCC_ClkInitStruct;

	/* Timers clock comes from APB2 */
	/* APB2 = HCLK / APB2Divider */
	hclk = HAL_RCC_GetHCLKFreq();
	HAL_RCC_GetClockConfig(&pRCC_ClkInitStruct, &pFLatency);
	switch(pRCC_ClkInitStruct.APB2CLKDivider)
	{
		case RCC_HCLK_DIV1 : apb2Divider =  1u; break;
		case RCC_HCLK_DIV2 : apb2Divider =  2u; break;
		case RCC_HCLK_DIV4 : apb2Divider =  4u; break;
		case RCC_HCLK_DIV8 : apb2Divider =  8u; break;
		case RCC_HCLK_DIV16: apb2Divider = 16u; break;
		default            : apb2Divider =  1u; break;
	}

	timerClock = hclk / apb2Divider;

	return timerClock;
}
/* USER CODE END 1 */
