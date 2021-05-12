/*
 * Kalman.c
 *
 *  Created on: 10 mai 2021
 *      Author: Théo
 */

#include "Kalman.h"


kalman_state_e KALMAN_Init(kalman_t *kalman, arm_matrix_instance_f32 * F,arm_matrix_instance_f32 * B, float ** u_array_ptr, arm_matrix_instance_f32 * H, float ** z_array_ptr, arm_matrix_instance_f32 * x,
		arm_matrix_instance_f32 * P, arm_matrix_instance_f32 * Q, arm_matrix_instance_f32 * R){

	//Dimensions
	kalman->x_size = F->numCols;
	kalman->u_size = B->numCols;
	kalman->z_size = H->numRows;

	//State space matricies init
	kalman->F = F;
	arm_mat_init_f32(&kalman->F_trans, kalman->x_size, kalman->x_size, kalman->F_trans_array);
	arm_mat_trans_f32(kalman->F, &kalman->F_trans);

	kalman->B = B;
	kalman->H = H;

	//H transposed init and calcul
	arm_mat_init_f32(&kalman->H_trans, H->numCols, H->numRows, kalman->H_trans_array);
	arm_mat_trans_f32(H, &kalman->H_trans);

	//X and X predict init
	kalman->x = x ;
	arm_mat_init_f32(&kalman->x_predict, kalman->x_size, 1, kalman->x_predict_array);
	for(uint8_t i =0; i > kalman->x_size; i++)
		kalman->x_predict.pData[i] = kalman->x->pData[i];

	//u vector init
	arm_mat_init_f32(&kalman->u, kalman->u_size, 1, kalman->u_array);
	kalman->u_array_ptr = u_array_ptr ;

	//z vector init
	arm_mat_init_f32(&kalman->z, kalman->z_size, 1, kalman->z_array);
	kalman->z_array_ptr = z_array_ptr ;

	//Inovation init
	arm_mat_init_f32(&kalman->inovation, kalman->z_size, 1, kalman->inovation_array);

	//Covariance
	kalman->Q = Q;
	kalman->R = R;
	kalman->P = P;
	arm_mat_init_f32(&kalman->S, kalman->z_size, kalman->z_size, kalman->S_array);

	arm_mat_init_f32(&kalman->S_tmp, kalman->z_size, kalman->x_size, kalman->S_tmp_array);

	//Kalman gain
	arm_mat_init_f32(&kalman->K, kalman->x_size, kalman->z_size, kalman->K_array);

	kalman->state = KALMAN_OK;

	return KALMAN_OK ;
}

kalman_state_e KALMAN_Update(kalman_t * kalman){

	//	---------------	Update ----------------

	//Copy paste new z values
	for(uint8_t z = 0; z < kalman->z_size; z++)
		kalman->z.pData[z] = *kalman->z_array_ptr[z];

	//Inovation
	arm_mat_mult_f32(kalman->H, &kalman->x_predict, &kalman->inovation);
	arm_mat_sub_f32(&kalman->z, &kalman->inovation, &kalman->inovation);

	//Covariance inovation
	arm_mat_mult_f32(kalman->H, &kalman->x_predict, &kalman->S_tmp);
	arm_mat_mult_f32(&kalman->S_tmp, &kalman->H_trans, &kalman->S);

	arm_mat_add_f32(&kalman->S, kalman->R, &kalman->S);
	arm_mat_inverse_f32(&kalman->S, &kalman->S);

	//Kalman gain
	arm_mat_mult_f32(&kalman->P_predict, &kalman->H_trans, &kalman->K);
	arm_mat_mult_f32(&kalman->K, &kalman->S, &kalman->K);

	//State estimation
	arm_mat_mult_f32(&kalman->K, &kalman->inovation, kalman->x);
	arm_mat_add_f32(&kalman->x_predict, kalman->x, kalman->x);

	return KALMAN_OK;
}

kalman_state_e KALMAN_Predict(kalman_t * kalman){

	//Predict next state
	arm_mat_mult_f32(kalman->F, kalman->x, &kalman->x_predict);

	//Predict next estimation covariance
	arm_mat_mult_f32(kalman->F, kalman->P, &kalman->P_predict);
	arm_mat_mult_f32(&kalman->P_predict, &kalman->F_trans, &kalman->P_predict);
	arm_mat_add_f32(&kalman->P_predict, kalman->Q, &kalman->P_predict);

	return KALMAN_OK ;
}














