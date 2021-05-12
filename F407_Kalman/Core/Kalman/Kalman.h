/*
 * Kalman.h
 *
 *  Created on: 10 mai 2021
 *      Author: Théo
 */

#ifndef KALMAN_KALMAN_H_
#define KALMAN_KALMAN_H_

#include "../DSP/Include/arm_math.h"

#define STATE_VECTOR_MAX_SIZE 2	//n
#define INPUT_MAX_SIZE 2		//m
#define OUTPUT_MAX_SIZE 2		//l

typedef enum kalman_state_e{
	KALMAN_NOT_INIT,
	KALMAN_INIT_FAILED,
	KALMAN_OK
}kalman_state_e;

typedef struct kalman_t{
	arm_matrix_instance_f32 * F;		//A
	arm_matrix_instance_f32 F_trans;
	float F_trans_array[STATE_VECTOR_MAX_SIZE * STATE_VECTOR_MAX_SIZE];


	arm_matrix_instance_f32 * B;
	arm_matrix_instance_f32 * H;		//C
	arm_matrix_instance_f32 H_trans;
	float H_trans_array[STATE_VECTOR_MAX_SIZE * OUTPUT_MAX_SIZE];

	uint8_t x_size ;	//n
	uint8_t u_size ;	//m
	uint8_t z_size ;	//l

	arm_matrix_instance_f32 * x;
	arm_matrix_instance_f32 x_predict;
	float x_predict_array [STATE_VECTOR_MAX_SIZE];

	arm_matrix_instance_f32 u;
	float u_array [INPUT_MAX_SIZE];
	float ** u_array_ptr ;

	arm_matrix_instance_f32 z;
	float z_array [OUTPUT_MAX_SIZE];
	float ** z_array_ptr ;

	arm_matrix_instance_f32 inovation;
	float inovation_array[OUTPUT_MAX_SIZE];

	arm_matrix_instance_f32 * P ;	//Covariance estimation
	arm_matrix_instance_f32 P_predict;
	float P_predict_array[STATE_VECTOR_MAX_SIZE * STATE_VECTOR_MAX_SIZE];

	arm_matrix_instance_f32 * Q;	//Covariance système (process noise)

	arm_matrix_instance_f32 * R;	//Covariance mesure

	arm_matrix_instance_f32 S;	//Covariance innovation
	float S_array[OUTPUT_MAX_SIZE * OUTPUT_MAX_SIZE];

	arm_matrix_instance_f32 S_tmp;	//Calcul intérmédiaire cov. inovation
	float S_tmp_array[STATE_VECTOR_MAX_SIZE * OUTPUT_MAX_SIZE];

	arm_matrix_instance_f32 K;	//Kalman gain
	float K_array[STATE_VECTOR_MAX_SIZE * OUTPUT_MAX_SIZE];

	kalman_state_e state;

}kalman_t;

kalman_state_e KALMAN_Init(kalman_t *kalman, arm_matrix_instance_f32 * F,arm_matrix_instance_f32 * B, float ** u_array_ptr, arm_matrix_instance_f32 * H, float ** z_array_ptr, arm_matrix_instance_f32 * x,
		arm_matrix_instance_f32 * P, arm_matrix_instance_f32 * Q, arm_matrix_instance_f32 * R);
kalman_state_e KALMAN_Update(kalman_t * kalman);

#endif /* KALMAN_KALMAN_H_ */
