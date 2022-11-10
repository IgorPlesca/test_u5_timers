/*
 * servo.h
 *
 *  Created on: Oct 25, 2022
 *      Author: Igor
 */

#ifndef INC_SERVOMOTOR_H_
#define INC_SERVOMOTOR_H_

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
	SERVO_MOTOR_2,
	SERVO_MOTOR_3,
	SERVO_MOTOR_4,

	SERVO_MOTOR_NUM_MAX,
} ServoMotorNumber_t;

typedef float ServoMotorAngleConfig_t[SERVO_MOTOR_NUM_MAX];

/**
 *  Initialize the motors with a specific model configuration
 */
void servoMotor_Init(ServoMotorModel_t motorModel);

/*
 * Start the motors
 */
void servoMotor_Start(void);

/**
 * Set the angles for all the motors
 */
void servoMotor_SetAngles(const ServoMotorAngleConfig_t motorAngleConfig);

#endif /* INC_SERVOMOTOR_H_ */
