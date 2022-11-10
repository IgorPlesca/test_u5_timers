/*
 * pwm.c
 *
 *  Created on: Nov 10, 2022
 *      Author: Igor
 */

#include "pwm.h"
#include "tim.h"

static const TimerPwmChanel_t pwmChanelMap[PWM_CHANNEL_NUM_MAX] =
{
		TIMER_PWM_CHANNEL_1,
		TIMER_PWM_CHANNEL_2,
		TIMER_PWM_CHANNEL_3,
		TIMER_PWM_CHANNEL_4
};

/*
 * Config the PWM Channels
 */
void pwm_ConfigChannels(uint32_t pwmFreqHz, uint32_t pwmPulseUsPrecision)
{
	PwmChannel_t i;

	/* Config all PWM channels */
	for(i=PWM_CHANNEL_1; i<PWM_CHANNEL_NUM_MAX; i++)
	{
		tim_PwmChannelConfig(pwmChanelMap[i], pwmFreqHz, pwmPulseUsPrecision);
	}
}

/*
 * Set pulse durations (in us) the PWM Channels
 */
void pwm_ChannelsSetPulseDurations(PwmPulseConfig_t pwmPulseUsConfig)
{
	PwmChannel_t i;

	/* Stop all PWM channels */
	for(i=PWM_CHANNEL_1; i<PWM_CHANNEL_NUM_MAX; i++)
	{
		tim_PwmChannelStop(pwmChanelMap[i]);
	}

	/* Set PWM pulse */
	for(i=PWM_CHANNEL_1; i<PWM_CHANNEL_NUM_MAX; i++)
	{
		tim_PwmChannelSetPulse(pwmChanelMap[i], pwmPulseUsConfig[i]);
	}

	/* Start all PWM channels */
	for(i=PWM_CHANNEL_1; i<PWM_CHANNEL_NUM_MAX; i++)
	{
		tim_PwmChannelStart(pwmChanelMap[i]);
	}
}
