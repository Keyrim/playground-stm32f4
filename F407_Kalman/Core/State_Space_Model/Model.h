/*
 * Model.h
 *
 *  Created on: 3 juin 2021
 *      Author: Théo
 */

#ifndef STATE_SPACE_MODEL_MODEL_H_
#define STATE_SPACE_MODEL_MODEL_H_

#include "State_Space_Model.h"
#include "Kalman.h"

typedef enum state_vector_e{
	STATE_VECTOR_ANGLE_Y,
	STATE_VECTOR_ANGLE_RATE_Y,
	STATE_VECTOR_SIZE
}state_vector_e;

typedef enum control_vector_e{
	CONTROL_VECTOR_MOTOR,
	CONTROL_VECTOR_SIZE
}control_vector_e;

typedef enum measurement_vector_e{
	MEASUREMENT_VECTOR_ANGLE_Y,
	MEASUREMENT_VECTOR_GYRO_Y,
	MEASUREMENT_VECTOR_SIZE
}measurement_vector_e;

void MODEL_Init(State_Space_Model_t * ss, kalman_t * kalman);

#endif /* STATE_SPACE_MODEL_MODEL_H_ */
