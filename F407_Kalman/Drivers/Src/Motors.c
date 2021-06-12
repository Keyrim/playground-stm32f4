/*
 * Motors.c
 *
 *  Created on: Dec 13, 2020
 *      Author: Théo
 */

#include "../Inc/Motors.h"
#include "math.h"

#ifndef MOTOR_MAX_POWER
	#define MOTOR_MAX_POWER 500
#endif

void MOTORS_Init(motors_t * motors, TIM_HandleTypeDef * htim, float * input, motor_direction_e direction, bool_e start){
	motors->htim = htim ;
	motors->input = input;
	motors->direction = direction ;
	motors->duty[0] = 0 ;
	motors->duty[1] = 0 ;
	motors->duty[2] = 0 ;
	motors->duty[3] = 0 ;
	if(start){
		motors->state = MOTORS_STATE_RUNNING ;
		motors->hal_state = HAL_TIM_PWM_Start(htim, TIM_CHANNEL_1);
		motors->hal_state = HAL_TIM_PWM_Start(htim, TIM_CHANNEL_2);
		motors->hal_state = HAL_TIM_PWM_Start(htim, TIM_CHANNEL_3);
		motors->hal_state = HAL_TIM_PWM_Start(htim, TIM_CHANNEL_4);
	}
}
void MOTORS_Start(motors_t * motors){
	motors->state = MOTORS_STATE_RUNNING ;
	motors->hal_state = HAL_TIM_PWM_Start(motors->htim, TIM_CHANNEL_1);
	motors->hal_state = HAL_TIM_PWM_Start(motors->htim, TIM_CHANNEL_2);
	motors->hal_state = HAL_TIM_PWM_Start(motors->htim, TIM_CHANNEL_3);
	motors->hal_state = HAL_TIM_PWM_Start(motors->htim, TIM_CHANNEL_4);
}

void MOTORS_Stop(motors_t * motors){
	motors->state = MOTORS_STATE_IDDLE ;
	motors->hal_state = HAL_TIM_PWM_Stop(motors->htim, TIM_CHANNEL_1);
	motors->hal_state = HAL_TIM_PWM_Stop(motors->htim, TIM_CHANNEL_2);
	motors->hal_state = HAL_TIM_PWM_Stop(motors->htim, TIM_CHANNEL_3);
	motors->hal_state = HAL_TIM_PWM_Stop(motors->htim, TIM_CHANNEL_4);
}

void MOTORS_Change_output(motors_t * motors){

	static int16_t moteur_0_offset;
	switch(motors->direction){
		case MOTOR_DIRECTION_NORMAL:
			//Pas plus de 1000 en consigne
			motors->duty[0] = MIN(MOTOR_MAX_POWER, motors->input[0]);
//			motors->duty[1] = MIN(MOTOR_MAX_POWER, motors->input[1]);
//			motors->duty[2] = MIN(MOTOR_MAX_POWER, motors->input[2]);
//			motors->duty[3] = MIN(MOTOR_MAX_POWER, motors->input[3]);

			//Racine carré pour annulé le fait que la force générée par l'hélice est proportionnelle au carrée de la vitesse
			motors->duty[0] = sqrtf(1000*motors->duty[0]);
//			motors->duty[1] = sqrtf(1000*motors->duty[1]);
//			motors->duty[2] = sqrtf(1000*motors->duty[2]);
//			motors->duty[3] = sqrtf(1000*motors->duty[3]);

			moteur_0_offset = 999;
			break;

		case MOTOR_DIRECTION_BIDIRECTIONAL:
			//Pas plus de 1000 en consigne
			motors->duty[0] = MIN(MOTOR_MAX_POWER, motors->input[0]);
//			motors->duty[1] = MIN(MOTOR_MAX_POWER, motors->input[1]);
//			motors->duty[2] = MIN(MOTOR_MAX_POWER, motors->input[2]);
//			motors->duty[3] = MIN(MOTOR_MAX_POWER, motors->input[3]);

			//Pas moins de -1000
			motors->duty[0] = MAX(-MOTOR_MAX_POWER, motors->duty[0]);
//			motors->duty[1] = MAX(-MOTOR_MAX_POWER, motors->duty[1]);
//			motors->duty[2] = MAX(-MOTOR_MAX_POWER, motors->duty[2]);
//			motors->duty[3] = MAX(-MOTOR_MAX_POWER, motors->duty[3]);

			//Racine carré comme expliqué avant
			if(motors->duty[0] >= 0)
				motors->duty[0] = 0.5f*sqrtf(1000*motors->duty[0]);
			else
				motors->duty[0] = -0.5f*sqrtf(-1000*motors->duty[0]);
//			if(motors->duty[1] >= 0)
//				motors->duty[1] = 0.5f*sqrtf(1000*motors->duty[1]);
//			else
//				motors->duty[1] = -0.5f*sqrtf(-1000*motors->duty[1]);
//			if(motors->duty[2] >= 0)
//				motors->duty[2] = 0.5f*sqrtf(1000*motors->duty[2]);
//			else
//				motors->duty[2] = -0.5f*sqrtf(-1000*motors->duty[2]);
//			if(motors->duty[3] >= 0)
//				motors->duty[3] = 0.5f*sqrtf(1000*motors->duty[3]);
//			else
//				motors->duty[3] = -0.5f*sqrtf(-1000*motors->duty[3]);


			moteur_0_offset = 1499;
			break;
	}

	motors->htim->Instance->CCR1 = (int16_t)motors->duty[0]+moteur_0_offset;
//	motors->htim->Instance->CCR2 = (int16_t)motors->duty[1]+moteur_0_offset;
//	motors->htim->Instance->CCR3 = (int16_t)motors->duty[2]+moteur_0_offset;
//	motors->htim->Instance->CCR4 = (int16_t)motors->duty[3]+moteur_0_offset;
}


















