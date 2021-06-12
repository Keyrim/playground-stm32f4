/*
 * State_Space_Model.h
 *
 *  Created on: 2 juin 2021
 *      Author: Théo
 */

#ifndef STATE_SPACE_MODEL_STATE_SPACE_MODEL_H_
#define STATE_SPACE_MODEL_STATE_SPACE_MODEL_H_

#include "../DSP/Include/arm_math.h"

#define STATE_VECTOR_MAX_SIZE 	3	//n
#define INPUT_MAX_SIZE 			2	//m
#define OUTPUT_MAX_SIZE 		3	//l

typedef struct State_Space_Model_t{

	uint8_t x_size ;	//n
	uint8_t u_size ;	//m
	uint8_t z_size ;	//l

	arm_matrix_instance_f32 * F;		//Matrice dynamique du système
	arm_matrix_instance_f32 F_trans;
	float F_trans_array[STATE_VECTOR_MAX_SIZE * STATE_VECTOR_MAX_SIZE];

	arm_matrix_instance_f32 * B;		//Matrice impact action extérieure sur le système

	arm_matrix_instance_f32 * H;		//Matrice d'observation
	arm_matrix_instance_f32 H_trans;
	float H_trans_array[STATE_VECTOR_MAX_SIZE * OUTPUT_MAX_SIZE];

	arm_matrix_instance_f32 x;
	float x_array [STATE_VECTOR_MAX_SIZE];
	arm_matrix_instance_f32 u;
	float u_array [INPUT_MAX_SIZE];
	arm_matrix_instance_f32 z;
	float z_array [OUTPUT_MAX_SIZE];

}State_Space_Model_t;

void STATE_SPACE_MODEL_Init(State_Space_Model_t * ss_model, arm_matrix_instance_f32 * F,arm_matrix_instance_f32 * B, arm_matrix_instance_f32 * H);
void STATE_SPACE_MODEL_Set_x0(State_Space_Model_t * ss_model, float * x_values);
void STATE_SPACE_MODEL_Step(State_Space_Model_t * ss_model);

#endif /* STATE_SPACE_MODEL_STATE_SPACE_MODEL_H_ */
