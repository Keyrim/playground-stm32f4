/*
 * Model.c
 *
 *  Created on: 3 juin 2021
 *      Author: Théo
 */


#include "Model.h"

//Model's dimension
#define X_SIZE STATE_VECTOR_SIZE
#define U_SIZE CONTROL_VECTOR_SIZE
#define Z_SIZE MEASUREMENT_VECTOR_SIZE



//State space model matrix
static arm_matrix_instance_f32 F ;
static arm_matrix_instance_f32 B ;
static arm_matrix_instance_f32 H ;

static float F_array[STATE_VECTOR_SIZE*STATE_VECTOR_SIZE] = {0.9974f,  0.009975f,  -0.515f, 0.9942f};
static float B_array[STATE_VECTOR_SIZE*CONTROL_VECTOR_SIZE] = {0.0008511f, 0.17f};
static float H_array[MEASUREMENT_VECTOR_SIZE*STATE_VECTOR_SIZE] = {1, 0, 0, 1};

static float x0[] = {0, 0};

//Kalman matrix
static arm_matrix_instance_f32 R ;
static arm_matrix_instance_f32 Q ;
static arm_matrix_instance_f32 P_predict ;

static float R_array[MEASUREMENT_VECTOR_SIZE*MEASUREMENT_VECTOR_SIZE] = {
		284, 0,
		0, 14
};

static float Q_array[STATE_VECTOR_SIZE*STATE_VECTOR_SIZE] = {
		0.001, 0,
		0, 0.01};

static float P_array[STATE_VECTOR_SIZE*STATE_VECTOR_SIZE] = {
		1, 0,
		0, 1};


void MODEL_Init(State_Space_Model_t * ss, kalman_t * kalman)
{
	//State space model Init
	arm_mat_init_f32(&F, X_SIZE, X_SIZE, F_array);
	arm_mat_init_f32(&B, X_SIZE, U_SIZE, B_array);
	arm_mat_init_f32(&H, Z_SIZE, X_SIZE, H_array);
	STATE_SPACE_MODEL_Init(ss, &F, &B, &H);
	STATE_SPACE_MODEL_Set_x0(ss, x0);

	//Kalman filter Init
	arm_mat_init_f32(&R, Z_SIZE, Z_SIZE, R_array);
	arm_mat_init_f32(&Q, X_SIZE, X_SIZE, Q_array);
	arm_mat_init_f32(&P_predict, X_SIZE, X_SIZE, P_array);
	KALMAN_Init(kalman, ss, &P_predict, &Q, &R);
	//KALMAN_Predict(kalman);
}
