/*
 * servo.h
 *
 *  Created on: Oct 25, 2022
 *      Author: Igor
 */

#ifndef INC_SERVOMOTOR_H_
#define INC_SERVOMOTOR_H_

#include <stdint.h>
#include "tim.h"

/* List of the accepted servo motor models */
typedef enum ServoMotorModel_e
{
	SERVO_MODEL_DM996 = 0x00,

	SERVO_MODELS_NUM_MAX,
} ServoMotorModel_t;

/* List of the accepted servo motor indexes */
typedef enum ServoMotorNumber_e
{
	SERVO_MOTOR_1 = 0x00U,

	SERVO_MOTOR_NUM_MAX,
} ServoMotorNumber_t;

/* Configuration of a servo motor*/
typedef struct ServoMotorConfig_s
{
	uint32_t pwmFreqHz;
	uint32_t pwmPulseUsAcc;
	uint32_t pwmPulseUsMin;
	uint32_t pwmPulseUsMax;
	uint16_t angleDegreeMin;
	uint16_t angleDegreeMax;
} ServoMotorConfig_t;

/**
 *  Initialize the selected motor with a specific model configuration
 */
void servoMotor_Init    (ServoMotorNumber_t motorNum, ServoMotorModel_t motorModel);

/**
 * Set the Angle for a selected motor
 */
void servoMotor_SetAngle(ServoMotorNumber_t motorNum, float angleDegrees);

#endif /* INC_SERVOMOTOR_H_ */
