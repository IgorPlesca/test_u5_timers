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

typedef enum ServoMotor_e
{
	SERVO_MOTOR_1 = 0x00U,

	SERVO_MOTOR_NUM_MAX,
} ServoMotor_t;

typedef struct ServoMotorConfig_s
{
	uint32_t pwmFreqHz;
	uint32_t pwmPulseUsAcc;
	uint32_t pwmPulseUsMin;
	uint32_t pwmPulseUsMax;
	float    angleDegreeMin;
	float    angleDegreeMax;
} ServoMotorConfig_t;


void servoMotor_Init    (ServoMotor_t motor, ServoMotorConfig_t *servoConfig);
void servoMotor_SetAngle(ServoMotor_t motor, float angleDegrees);

#endif /* INC_SERVOMOTOR_H_ */
