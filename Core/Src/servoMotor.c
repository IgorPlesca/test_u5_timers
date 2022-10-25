/*
 * servo.c
 *
 *  Created on: Oct 25, 2022
 *      Author: Igor
 */

#include <servoMotor.h>
#include <stdbool.h>

static ServoMotorConfig_t m_ServoConfig[SERVO_MOTOR_NUM_MAX] = {0};
static bool               m_ServoConfigStatus[SERVO_MOTOR_NUM_MAX] = {false};
static PwmChannel_t       m_PwmChannelConfig[SERVO_MOTOR_NUM_MAX] =
{
		{TIMER_2, TIM_CHANNEL_4}, /* Servo Motor 1 */
};

void servoMotor_Init(ServoMotor_t motor, ServoMotorConfig_t *servoConfig)
{
	if(motor < SERVO_MOTOR_NUM_MAX)
	{
		/* Servo Motor config */
		m_ServoConfig[motor].pwmFreqHz      = servoConfig->pwmFreqHz;
		m_ServoConfig[motor].pwmPulseUsAcc  = servoConfig->pwmPulseUsAcc;
		m_ServoConfig[motor].pwmPulseUsMin  = servoConfig->pwmPulseUsMin;
		m_ServoConfig[motor].pwmPulseUsMax  = servoConfig->pwmPulseUsMax;
		m_ServoConfig[motor].angleDegreeMin = servoConfig->angleDegreeMin;
		m_ServoConfig[motor].angleDegreeMax = servoConfig->angleDegreeMax;
		tim_PwmChannelConfig(&m_PwmChannelConfig[motor], servoConfig->pwmFreqHz, servoConfig->pwmPulseUsAcc);

		/* Servo/PWM Config end */
		m_ServoConfigStatus[motor]          = true;
	}
}

void servoMotor_SetAngle(ServoMotor_t motor, float angleDegrees)
{
	ServoMotorConfig_t *servoConfig;
	PwmChannel_t	   *pwmChannel;
	float totalAngle;
	float totalPulseUs;
	uint32_t            pwmPulseUs;

	if(motor < SERVO_MOTOR_NUM_MAX)
	{
		if(true == m_ServoConfigStatus[motor])
		{
			servoConfig = &m_ServoConfig[motor];
			pwmChannel  = &m_PwmChannelConfig[motor];
			if( ( angleDegrees >= servoConfig->angleDegreeMin) &&
				( angleDegrees <= servoConfig->angleDegreeMax) )
			{
				totalAngle   = servoConfig->angleDegreeMax - servoConfig->angleDegreeMin;
				totalPulseUs = servoConfig->pwmPulseUsMax  - servoConfig->pwmPulseUsMin;

				pwmPulseUs = servoConfig->pwmPulseUsMin + (uint32_t) ( (angleDegrees * totalPulseUs)/totalAngle);
				tim_PwmChannelSetPulseMicroseconds(pwmChannel, pwmPulseUs);
			}
		}
	}
}
