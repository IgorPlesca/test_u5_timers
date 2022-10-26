/*
 * servo.c
 *
 *  Created on: Oct 25, 2022
 *      Author: Igor
 */

#include <servoMotor.h>
#include "tim.h"
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* Configuration of a servo motor */
typedef struct ServoMotorConfig_s
{
	uint32_t pwmFreqHz;             /* PWM frequency for driving motor (in Hz)      */
	uint32_t pwmPulseUsPrecision;   /* PWM pulse duration precision    (in us)      */
	uint32_t pwmPulseUsMin;         /* PWM pulse minimum duration      (in us)      */
	uint32_t pwmPulseUsMax;         /* PWM pulse maximum duration      (in us)      */
	uint16_t angleDegreeMin;        /* Minimum shaft angle             (in degrees) */
	uint16_t angleDegreeMax;        /* Maximum shaft angle             (in degrees) */
} ServoMotorConfig_t;

/* Servo Motor models configurations */
static const ServoMotorConfig_t KT_ServoModelsConfig[SERVO_MODELS_NUM_MAX] =
{
		/* Servo Motor DM996 */
		{
				.pwmFreqHz           =   50uL, /* 20ms PWM period duration      ->   50 Hz */
				.pwmPulseUsPrecision =    1uL, /* to be investigated                       */
				.pwmPulseUsMin       =  500uL, /* 0.5ms of minimum pulse length ->  500 us */
				.pwmPulseUsMax       = 2500uL, /* 2.5ms of maximum pulse length -> 2500 us */
				.angleDegreeMin      =     0u, /*   0 degrees of minimum shaft angle       */
				.angleDegreeMax      =   180u, /* 180 degrees of maximum shaft angle       */
		},
};

/* Current servo motor config for initiated motors */
static ServoMotorModel_t m_ServoMotorMap[SERVO_MOTOR_NUM_MAX] = {0};

/* Current servo motor config status for motors */
static bool               m_ServoMotorMapStatus[SERVO_MOTOR_NUM_MAX] = {false};

/* PWM configuration for relative motors */
static const PwmChannel_t KT_PwmChannelConfig[SERVO_MOTOR_NUM_MAX] =
{
		{TIMER_1, TIM_CHANNEL_1}, /* PWM Channel for Servo Motor 1 */
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
		/* Map the wanted servo motor with a servo motor model */
		m_ServoMotorMap[motorNum]       = motorModel;
		m_ServoMotorMapStatus[motorNum] = true;
	}
}

/*
 * Start the selected motor PWM channel
 */
void servoMotor_StartPwm(ServoMotorNumber_t motorNum)
{
	/* Check if motor number is correct and if it's configured */
	if( (motorNum < SERVO_MOTOR_NUM_MAX)        &&
		(true == m_ServoMotorMapStatus[motorNum]) )
	{
		/* Timer PWM configuration */
		tim_PwmChannelConfig(&KT_PwmChannelConfig[motorNum],
				              KT_ServoModelsConfig[m_ServoMotorMap[motorNum]].pwmFreqHz,
				              KT_ServoModelsConfig[m_ServoMotorMap[motorNum]].pwmPulseUsPrecision);
	}
}

/**
 * Set the Angle for a selected motor
 */
void servoMotor_SetAngle(ServoMotorNumber_t motorNum, float angleDegrees)
{
	/* Check if motor number is correct and if it's configured */
	if( (motorNum < SERVO_MOTOR_NUM_MAX)        &&
	    (true == m_ServoMotorMapStatus[motorNum]) )
	{
		/* Select servo motor and it's relative PWM channel configurations */
		const ServoMotorConfig_t *servoConfig = &KT_ServoModelsConfig[m_ServoMotorMap[motorNum]];
		const PwmChannel_t       *pwmChannel  = &KT_PwmChannelConfig[motorNum];

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
			tim_PwmChannelSetPulseDuration(pwmChannel, pwmPulseUs);
		}
	}
}
