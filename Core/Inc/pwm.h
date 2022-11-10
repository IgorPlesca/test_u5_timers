/*
 * pwm.h
 *
 *  Created on: Nov 10, 2022
 *      Author: Igor
 */

#ifndef INC_PWM_H_
#define INC_PWM_H_

#include <stdint.h>

/* PWM Chanels definitions */
typedef enum PwmChannel_e
{
	PWM_CHANNEL_1 = 0x00,
	PWM_CHANNEL_2,
	PWM_CHANNEL_3,
	PWM_CHANNEL_4,

	PWM_CHANNEL_NUM_MAX,
} PwmChannel_t;

/* Type of the array of pulse durations for all PWM channels */
typedef uint32_t PwmPulseConfig_t[PWM_CHANNEL_NUM_MAX] ;

/*
 * Config the PWM Channels
 */
void pwm_ConfigChannels(uint32_t pwmFreqHz, uint32_t pwmPulseUsPrecision);

/*
 * Set pulse durations (in us) for the PWM Channels
 */
void pwm_ChannelsSetPulseDurations(PwmPulseConfig_t pwmPulseUsConfig);

#endif /* INC_PWM_H_ */
