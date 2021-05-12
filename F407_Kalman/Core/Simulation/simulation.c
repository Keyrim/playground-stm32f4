/*
 * simulation.c
 *
 *  Created on: 29 avr. 2021
 *      Author: Théo
 */


#include "simulation.h"

void STATE_SPACE_Init(state_space_t * ss, arm_matrix_instance_f32 * A, float32_t * init_value){
	//State space matrix
	ss->A = A ;

	//Set state vector initial values

	for(int i = 0; i < A->numCols; i++)
		ss->X_array[i] = init_value[i];
	arm_mat_init_f32(&ss->X, A->numCols, 1, ss->X_array);


	//Arm error set to false
	ss->arm_error = 0;


}
void STATE_SPACE_Step(state_space_t * ss){

	if(arm_mat_mult_f32(ss->A, &ss->X, &ss->X) != ARM_MATH_SUCCESS){
		ss->arm_error = 1 ;
	}
}
