/*
 * Regu_orientation.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Théo
 */

#ifndef REGULATION_REGU_ORIENTATION_H_
#define REGULATION_REGU_ORIENTATION_H_

#include "../Estimators/orientation.h"
#include "../../Drivers/Inc/Pid.h"
#include "../config.h"

#define REGU_ORIENTATION_FREQUENCY GYRO_FREQUENCY

typedef enum regulation_mode_e{
	REGULATION_MODE_OFF,
	REGULATION_MODE_MANUAL,
	REGULATION_MODE_ANGLE,
	REGULATION_MODE_VELOCITY
}regulation_mode_e;

typedef struct regu_orientation_t{
	orientation_t * orientation ;
	regulation_mode_e mode ;
	PID_t pid_angular_speed ;
	PID_t pid_angular_pos ;
	float consigne_angular_pos;
	float consigne_angular_speed;
	int16_t motor_consigne;
	int16_t  * output;		//Output for the motor
}regu_orientation_t;

void REGULATION_ORIENTATION_Init(regu_orientation_t * regu_orientation_, orientation_t * orientation_, int16_t * output);
void REGULATION_ORIENTATION_Set_Regulation_Mode(regulation_mode_e regu_mode);
void REGULATION_ORIENTATION_Process(void);

#endif /* REGULATION_REGU_ORIENTATION_H_ */
