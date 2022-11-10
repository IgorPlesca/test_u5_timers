/*
 * servo.c
 *
 *  Created on: Oct 25, 2022
 *      Author: Igor
 */

#include <servoMotor.h>
#include "pwm.h"
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "debug.h"

/* Configuration of a servo motor model */
typedef struct ServoMotorModelConfig_s
{
	uint32_t pwmFreqHz;             /* PWM frequency for driving motor (in Hz)      */
	uint32_t pwmPulseUsPrecision;   /* PWM pulse duration precision    (in us)      */
	uint32_t pwmPulseUsMin;         /* PWM pulse minimum duration      (in us)      */
	uint32_t pwmPulseUsMax;         /* PWM pulse maximum duration      (in us)      */
	float    angleDegreeMin;        /* Minimum shaft angle             (in degrees) */
	float    angleDegreeMax;        /* Maximum shaft angle             (in degrees) */
} ServoMotorModelConfig_t;

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

/* The current model of the motors */
static ServoMotorModel_t m_ServoMotorModel             = SERVO_MODELS_NUM_MAX;
/* Tells if the current model of the motors was configured */
static bool              m_ServoMotorModelIsConfigured = false;

/**
 *  Initialize the motors with a specific model configuration
 */
void servoMotor_Init(ServoMotorModel_t motorModel)
{
	m_ServoMotorModel             = motorModel;
	m_ServoMotorModelIsConfigured = true;
}

/*
 * Start the motors
 */
void servoMotor_Start(void)
{
	if(true == m_ServoMotorModelIsConfigured)
	{
		for(ServoMotorNumber_t i=SERVO_MOTOR_1; i<SERVO_MOTOR_NUM_MAX; i++)
		{
			/* Select servo motor and it's relative PWM channel configurations */
			const ServoMotorModelConfig_t *servoConfig = &KT_ServoModelsConfig[m_ServoMotorModel];

			/* Configuration of the selected motor PWM channel */
			pwm_ConfigChannels(servoConfig->pwmFreqHz, servoConfig->pwmPulseUsPrecision);
		}
	}
}

/**
 * Set the angles for all the motors
 */
void servoMotor_SetAngles(const ServoMotorAngleConfig_t motorAngleConfig)
{
	PwmPulseConfig_t pwmPulseUsConfig;

	/* Check if motor number is configured */
	if(true == m_ServoMotorModelIsConfigured)
	{
		debug_print("\nNew Angles (deg) / Pulse duration (us)\n");
		/* Compute the PWM pulse duration for every configured servo motor */
		for(ServoMotorNumber_t i=SERVO_MOTOR_1; i<SERVO_MOTOR_NUM_MAX; i++)
		{
			/* Select servo motor and it's relative PWM channel configurations */
			const ServoMotorModelConfig_t *servoConfig = &KT_ServoModelsConfig[m_ServoMotorModel];
			float angleDegrees = motorAngleConfig[i];

			/* Check if the selected angle is between the acceptable limits of the selected motor model config. */
			if( ( angleDegrees >= servoConfig->angleDegreeMin) &&
			    ( angleDegrees <= servoConfig->angleDegreeMax) )
			{
				/* Compute the total shaft angle and PWM pulse duration intervals */
				uint16_t totalAngle   = servoConfig->angleDegreeMax - servoConfig->angleDegreeMin;
				uint32_t totalPulseUs = servoConfig->pwmPulseUsMax  - servoConfig->pwmPulseUsMin;

				/* Translate shaft angle in PWM pulse duration:
				 * Total Pulse Duration =  Minimum pulse duration + Angle related pulse duration */
				pwmPulseUsConfig[i] = servoConfig->pwmPulseUsMin +
						              (uint32_t)( (angleDegrees * totalPulseUs) / totalAngle );
			}
			else
			{
				/* Angle computation error: put to the minimum angle */
				pwmPulseUsConfig[i] = servoConfig->pwmPulseUsMin;
			}

			debug_print("SM%u: A = %6.2f, PD = %4lu\n", (i+1), angleDegrees, pwmPulseUsConfig[i]);
		}

		/* Set the PWM pulse duration to the configured motors PWM channels */
		pwm_ChannelsSetPulseDurations(pwmPulseUsConfig);
	}
}
