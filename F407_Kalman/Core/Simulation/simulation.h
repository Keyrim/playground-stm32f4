/*
 * simulation.h
 *
 *  Created on: 29 avr. 2021
 *      Author: Théo
 */

#ifndef SIMULATION_SIMULATION_H_
#define SIMULATION_SIMULATION_H_

#include "../DSP/Include/arm_math.h"

#define STATE_VECTOR_MAX_SIZE 2

typedef struct state_space_t{
	//State space matrix
	arm_matrix_instance_f32 * A;
	arm_matrix_instance_f32 * B;
	arm_matrix_instance_f32 * C;

	//State vector
	arm_matrix_instance_f32 X ;
	float32_t X_array[STATE_VECTOR_MAX_SIZE];

	//Arm error ..
	uint8_t arm_error;

}state_space_t;

void STATE_SPACE_Init(state_space_t * ss, arm_matrix_instance_f32 * A, float32_t * init_value);
void STATE_SPACE_Step(state_space_t * ss);

#endif /* SIMULATION_SIMULATION_H_ */
