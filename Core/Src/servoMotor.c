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

/* Configuration of a servo motor model */
typedef struct ServoMotorModelConfig_s
{
	uint32_t pwmFreqHz;             /* PWM frequency for driving motor (in Hz)      */
	uint32_t pwmPulseUsPrecision;   /* PWM pulse duration precision    (in us)      */
	uint32_t pwmPulseUsMin;         /* PWM pulse minimum duration      (in us)      */
	uint32_t pwmPulseUsMax;         /* PWM pulse maximum duration      (in us)      */
	uint16_t angleDegreeMin;        /* Minimum shaft angle             (in degrees) */
	uint16_t angleDegreeMax;        /* Maximum shaft angle             (in degrees) */
} ServoMotorModelConfig_t;

/* Configuration state of a servo motor */
typedef struct ServoMotorConfigState_s
{
	ServoMotorModel_t motorModel;         /* The model of the motor     */
	bool              isMotorConfigured;  /* Is the motor it configured */
} ServoMotorConfigState_t;

/* Servo Motor models configurations */
static const ServoMotorModelConfig_t KT_ServoModelsConfig[SERVO_MODELS_NUM_MAX] =
{
		/* Servo Motor DM996 */
		{
				.pwmFreqHz           =   50uL, /* 20 ms PWM period duration      ->   50 Hz  */
				.pwmPulseUsPrecision =    1uL, /* to be investigated             ->    1 us  */
				.pwmPulseUsMin       =  500uL, /* 0.5 ms of minimum pulse length ->  500 us  */
				.pwmPulseUsMax       = 2500uL, /* 2.5 ms of maximum pulse length -> 2500 us  */
				.angleDegreeMin      =     0u, /* minimum shaft angle            ->    0 deg */
				.angleDegreeMax      =   180u, /* maximum shaft angle            ->  180 deg */
		},
};

/* PWM configuration for relative motors */
static const PwmChannel_t KT_PwmChannelConfig[SERVO_MOTOR_NUM_MAX] =
{
		{TIMER_1, TIM_CHANNEL_1},     /* PWM Channel for Servo Motor 1 */
};

/* Current servo motor config status for motors */
static ServoMotorConfigState_t m_ServoMotorConfigState[SERVO_MOTOR_NUM_MAX] =
{
		{SERVO_MOTOR_NUM_MAX, false}, /* Configuration State for Servo Motor 1 */
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
		m_ServoMotorConfigState[motorNum].motorModel        = motorModel;
		m_ServoMotorConfigState[motorNum].isMotorConfigured = true;
	}
}

/*
 * Start the selected motor PWM channel
 */
void servoMotor_StartPwm(ServoMotorNumber_t motorNum)
{
	/* Check if motor number is correct and if it's configured */
	if(motorNum < SERVO_MOTOR_NUM_MAX)
	{
		if(true == m_ServoMotorConfigState[motorNum].isMotorConfigured)
		{
			/* Select servo motor and it's relative PWM channel configurations */
			const ServoMotorModelConfig_t *servoConfig = &KT_ServoModelsConfig[m_ServoMotorConfigState[motorNum].motorModel];
			const PwmChannel_t            *pwmChannel  = &KT_PwmChannelConfig[motorNum];

			/* Configuration of the selected motor PWM channel */
			tim_PwmChannelConfig(pwmChannel, servoConfig->pwmFreqHz, servoConfig->pwmPulseUsPrecision);
		}
	}
}

/**
 * Set the Angle for a selected motor
 */
void servoMotor_SetAngle(ServoMotorNumber_t motorNum, float angleDegrees)
{
	/* Check if motor number is correct and if it's configured */
	if(motorNum < SERVO_MOTOR_NUM_MAX)
	{
		if(true == m_ServoMotorConfigState[motorNum].isMotorConfigured)
		{
			/* Select servo motor and it's relative PWM channel configurations */
			const ServoMotorModelConfig_t *servoConfig = &KT_ServoModelsConfig[m_ServoMotorConfigState[motorNum].motorModel];
			const PwmChannel_t            *pwmChannel  = &KT_PwmChannelConfig[motorNum];

			/* Check if the selected angle is between the acceptable limits of the selected motor model config. */
			if( ( angleDegrees >= (float) servoConfig->angleDegreeMin) &&
                ( angleDegrees <= (float) servoConfig->angleDegreeMax) )
			{
				/* Compute the total shaft angle and PWM pulse duration intervals */
				uint16_t totalAngle   = servoConfig->angleDegreeMax - servoConfig->angleDegreeMin;
				uint32_t totalPulseUs = servoConfig->pwmPulseUsMax  - servoConfig->pwmPulseUsMin;

				/* Translate shaft angle in PWM pulse duration:
				 * Total Pulse Duration =  Minimum pulse duration + Angle related pulse duration */
				uint32_t pwmPulseUs = servoConfig->pwmPulseUsMin +
						              (uint32_t)( (angleDegrees * totalPulseUs) / totalAngle );

				/* Set the PWM pulse duration to the selected motor PWM channel */
				tim_PwmChannelSetPulseDuration(pwmChannel, pwmPulseUs);
			}
		}
	}
}
