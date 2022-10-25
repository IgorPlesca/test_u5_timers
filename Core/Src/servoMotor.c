/*
 * servo.c
 *
 *  Created on: Oct 25, 2022
 *      Author: Igor
 */

#include <servoMotor.h>
#include <string.h>
#include <stdbool.h>

/* Servo Motor models configurations */
static const ServoMotorConfig_t KT_ServoModelsConfig[SERVO_MODELS_NUM_MAX] =
{
		/* Servo Motor DM996 */
		{
				.pwmFreqHz      =   50uL, /* 20ms PWM period duration      ->   50 Hz */
				.pwmPulseUsAcc  =    1uL, /* to be investigated                       */
				.pwmPulseUsMin  =  500uL, /* 0.5ms of minimum pulse length ->  500 us */
				.pwmPulseUsMax  = 2500uL, /* 2.5ms of maximum pulse length -> 2500 us */
				.angleDegreeMin =     0u, /*   0 degrees of minimum angle             */
				.angleDegreeMax =   180u, /* 180 degrees of maximum angle             */
		},
};

/* Current servo motor config for initiated motors */
static ServoMotorConfig_t m_ServoConfig[SERVO_MOTOR_NUM_MAX] = {0};

/* Current servo motor config status for motors */
static bool               m_ServoConfigStatus[SERVO_MOTOR_NUM_MAX] = {false};

/* PWM configuration for relative motors */
static PwmChannel_t       m_PwmChannelConfig[SERVO_MOTOR_NUM_MAX] =
{
		{TIMER_1, TIM_CHANNEL_1}, /* Servo Motor 1 */
};

/**
 *  Initialize the selected motor with a specific model configuration
 */
void servoMotor_Init(ServoMotorNumber_t motorNum, ServoMotorModel_t motorModel)
{
	/* Check if motor number and motor model are correct */
	if( (motorNum   < SERVO_MOTOR_NUM_MAX ) &&
		(motorModel < SERVO_MODELS_NUM_MAX) )
	{
		/* Servo Motor config */
		(void) memcpy((void*)&m_ServoConfig[motorNum],
			          (void*)&KT_ServoModelsConfig[motorModel],
					  sizeof(m_ServoConfig[motorNum]));

		/* Timer PWM configuration */
		tim_PwmChannelConfig(&m_PwmChannelConfig[motorNum],
				              m_ServoConfig[motorNum].pwmFreqHz,
				              m_ServoConfig[motorNum].pwmPulseUsAcc);

		/* Servo/PWM Config end */
		m_ServoConfigStatus[motorNum] = true;
	}
}

/**
 * Set the Angle for a selected motor
 */
void servoMotor_SetAngle(ServoMotorNumber_t motor, float angleDegrees)
{
	/* Check if motor number is correct and if it's configured */
	if( (motor < SERVO_MOTOR_NUM_MAX)        &&
	    (true == m_ServoConfigStatus[motor]) )
	{
		/* Select servo motor and it's relative PWM channel configurations */
		ServoMotorConfig_t *servoConfig = &m_ServoConfig[motor];
		PwmChannel_t	   *pwmChannel  = &m_PwmChannelConfig[motor];

		/* Check if the selected angle is between the acceptable limits */
		if( ( angleDegrees >= (float) servoConfig->angleDegreeMin) &&
			( angleDegrees <= (float) servoConfig->angleDegreeMax) )
		{
			/* Compute the total shaft angle and PWM pulse duration intervals */
			uint16_t totalAngle   = servoConfig->angleDegreeMax - servoConfig->angleDegreeMin;
			uint32_t totalPulseUs = servoConfig->pwmPulseUsMax  - servoConfig->pwmPulseUsMin;

			/* Translate shaft angle in PWM pulse duration:
			 *    Minimum pulse duration + Angle related pulse duration */
			uint32_t pwmPulseUs = servoConfig->pwmPulseUsMin +
								  (uint32_t)( (angleDegrees * totalPulseUs) / totalAngle );

			/* Set the PWM pulse duration to the relative motor PWM channel */
			tim_PwmChannelSetPulseMicroseconds(pwmChannel, pwmPulseUs);
		}
	}
}
