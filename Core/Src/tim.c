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
static TIM_HandleTypeDef *KT_pTimerHandlers[TIMER_NUM_MAX] =
{
		NULL,           /* TIMER_1 */
		NULL,           /* TIMER_2 */
		&htim3,         /* TIMER_3 */
		NULL,           /* TIMER_4 */
		NULL,           /* TIMER_5 */
		NULL,           /* TIMER_6 */
		NULL,           /* TIMER_7 */
		NULL,           /* TIMER_8 */
};

static uint32_t KT_TimerChannels[TIMER_CH_NUM_MAX] =
{
		TIM_CHANNEL_1,  /* TIMER_CH1 */
		TIM_CHANNEL_2,  /* TIMER_CH2 */
		TIM_CHANNEL_3,  /* TIMER_CH3 */
		TIM_CHANNEL_4,  /* TIMER_CH4 */
};

static uint32_t tim_GetTimerClock(void);
/* USER CODE END 0 */

TIM_HandleTypeDef htim3;

/* TIM3 init function */
void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 4000;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1000;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 500;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspInit 0 */

  /* USER CODE END TIM3_MspInit 0 */
    /* TIM3 clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();
  /* USER CODE BEGIN TIM3_MspInit 1 */

  /* USER CODE END TIM3_MspInit 1 */
  }
}
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(timHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspPostInit 0 */

  /* USER CODE END TIM3_MspPostInit 0 */

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**TIM3 GPIO Configuration
    PC7     ------> TIM3_CH2
    */
    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM3_MspPostInit 1 */

  /* USER CODE END TIM3_MspPostInit 1 */
  }

}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspDeInit 0 */

  /* USER CODE END TIM3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM3_CLK_DISABLE();
  /* USER CODE BEGIN TIM3_MspDeInit 1 */

  /* USER CODE END TIM3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void tim_PwmChannelConfig(PwmChannel_t pwmChannel, uint32_t pwmFreqHz, float pwmDutyCyclePercPrecision)
{
	TIM_HandleTypeDef 		*pTimHandler;
	uint32_t           		timChannel;

	uint32_t 				timerClock         = 0uL;
	uint32_t 				counterPeriod      = 0uL;
	uint32_t 				prescaler          = 0uL;
	TIM_ClockConfigTypeDef 	sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig      = {0};

	pTimHandler = KT_pTimerHandlers[pwmChannel.timer];
	if(NULL == pTimHandler)
	{
		Error_Handler();
	}

	timChannel  = KT_TimerChannels[pwmChannel.timerChannel];

	HAL_TIM_PWM_Stop(pTimHandler, timChannel);

	timerClock    = tim_GetTimerClock();
	counterPeriod = (uint32_t) (100uL * pwmFreqHz / pwmDutyCyclePercPrecision);
	prescaler     = (pwmFreqHz * timerClock) / counterPeriod;

	pTimHandler->Instance = TIM3;
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


void tim_PwmChannelSetDutyCycle(PwmChannel_t pwmChannel, float dutyCycle)
{
	TIM_HandleTypeDef	*pTimHandler;
	uint32_t         	timChannel;

	uint32_t 			pulseCounter 	= 0uL;
	uint32_t 			counterPeriod 	= 0uL;
	TIM_OC_InitTypeDef 	sConfigOC 		= {0};

	pTimHandler = KT_pTimerHandlers[pwmChannel.timer];
	if(NULL == pTimHandler)
	{
		Error_Handler();
	}

	timChannel  = KT_TimerChannels[pwmChannel.timerChannel];

	HAL_TIM_PWM_Stop(pTimHandler, timChannel);

	counterPeriod = pTimHandler->Init.Period;
	pulseCounter  = (uint32_t) ((counterPeriod * dutyCycle) / 100uL);

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = pulseCounter;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(pTimHandler, &sConfigOC, timChannel) != HAL_OK)
	{
		Error_Handler();
	}

	HAL_TIM_PWM_Start(pTimHandler, timChannel);
}

static uint32_t tim_GetTimerClock(void)
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
