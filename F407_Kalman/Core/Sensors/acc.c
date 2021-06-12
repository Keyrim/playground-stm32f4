/*
 * acc.c
 *
 *  Created on: 1 déc. 2020
 *      Author: Theo
 */


#include "acc.h"
#include "math.h"
#include "../OS/events/events.h"

static void acc_data_callback(void);

float filter_config [3] = {0.04f, 1.6f, -0.64f};
//float filter_config [3] = {0.002267573696f, 1.904761905f, -0.9070294785f};
//float filter_config [3] = {0.1f, 0.9f, 0.0f};

void ACC_init(acc_t * acc, mpu_t * mpu, float * angle_output){

	FILTER_init(&acc->filters[ACC_AXE_X], filter_config, FILTER_NO_FILTERING);
	FILTER_init(&acc->filters[ACC_AXE_Y], filter_config, FILTER_NO_FILTERING);
	FILTER_init(&acc->filters[ACC_AXE_Z], filter_config, FILTER_NO_FILTERING);


	acc->mpu = mpu ;
	acc->raw = acc->mpu->acc ;
	acc->angle_output = angle_output;

	acc->offsets[0] = 0.0 ;
	acc->offsets[1] = 0.0 ;
	acc->offsets[2] = 0.0 ;

	switch(MPU_init_acc(acc->mpu, MPU_ACC_4G, acc_data_callback)){
		case SENSOR_REQUEST_OK:
			EVENT_Set_flag(FLAG_ACC_OK);
			break;
		case SENSOR_REQUEST_FAIL:
			break;
		case SENSOR_REQUEST_BUSY:
			break;
	}

}

//void ACC_update(acc_t * acc){
//	switch(MPU_update_acc(acc->mpu)){
//		case SENSOR_REQUEST_OK:
//			EVENT_Set_flag(FLAG_ACC_DATA_READY);
//			break;
//		case SENSOR_REQUEST_FAIL:
//			EVENT_Clean_flag(FLAG_ACC_OK);
//			break;
//		case SENSOR_REQUEST_BUSY:
//			break;
//	}
//}

//bool_e ACC_calibrate(acc_t * acc, uint16_t iteration){
//	static uint16_t compteur = 0;
//
//	static float sum[3] = {0};
//
//	if(!compteur){
//		sum[0] = 0 ;
//		sum[1] = 0 ;
//		sum[2] = 0 ;
//	}
//
//	sum[0] += acc->raw[0] + acc->offsets[0];
//	sum[1] += acc->raw[1] + acc->offsets[1];
//	sum[2] += acc->raw[2] + acc->offsets[2] - 1;	//Pour le YAW on doit avoir 1g quand le drone est à l'horizontale
//
//	compteur ++;
//
//	if(compteur == iteration){
//		acc->offsets[0] = sum[0] / (float)iteration;
//		acc->offsets[1] = sum[1] / (float)iteration;
//		acc->offsets[2] = sum[2] / (float)iteration;
//		compteur = 0;
//		return TRUE;
//	}
//
//	return FALSE;
//
//}
//
//void ACC_update_dma(acc_t * acc){
//	switch(MPU_update_acc_dma(acc->mpu)){
//		case SENSOR_REQUEST_OK:
//			break;
//		case SENSOR_REQUEST_FAIL:
//			EVENT_Clean_flag(FLAG_ACC_OK);
//			break;
//		case SENSOR_REQUEST_BUSY:
//			break;
//	}
//}

void ACC_process_lpf(acc_t * acc){
	//Offset correction first
	acc->raw[0] -= acc->offsets[0];
	acc->raw[1] -= acc->offsets[1];
	acc->raw[2] -= acc->offsets[2];
	//Then the actual low pass filter
	acc->filtered[ACC_AXE_X] = FILTER_process(&acc->filters[ACC_AXE_X], acc->raw[ACC_AXE_X]);
	acc->filtered[ACC_AXE_Y] = FILTER_process(&acc->filters[ACC_AXE_Y], acc->raw[ACC_AXE_Y]);
	acc->filtered[ACC_AXE_Z] = FILTER_process(&acc->filters[ACC_AXE_Z], acc->raw[ACC_AXE_Z]);
	//We warn the system that new data are available
	EVENT_Set_flag(FLAG_ACC_FILTERED_DATA_READY);
}

static float acc_correction(float x);
static float acc_correction(float x){
	x = ( x > 0) ? 180 - x : -180 -x ;
	return x ;
}
void ACC_process_angle(acc_t * acc){
	if(acc->angles == NULL)
		return;
	//Total acceleration
	float acc_total = acc->filtered[0] * acc->filtered[0] ;
	acc_total += acc->filtered[1] * acc->filtered[1] ;
	acc_total += acc->filtered[2] * acc->filtered[2] ;
	acc_total = sqrtf(acc_total);

	if(acc_total)	//To avoid /0
	{
		if(absolute(acc->filtered[ORIENTATION_YAW]) < acc_total)	//To avoid asin x with x greater than 1
			acc->angles[ORIENTATION_PITCH] = asinf(acc->filtered[ORIENTATION_YAW] / acc_total) * 57.32f;
//		if(absolute(acc->filtered[ORIENTATION_PITCH]) < acc_total) 	//To avoid asin x with x greater than 1
//			acc->angles[ORIENTATION_YAW] = -asinf(acc->filtered[ORIENTATION_PITCH] / acc_total) * 57.32f;


		if(acc->filtered[ORIENTATION_ROLL] < 0)
		{
//			acc->angles[ORIENTATION_YAW] = acc_correction(oacc->angles[ORIENTATION_YAW]);
			acc->angles[ORIENTATION_PITCH] = acc_correction(acc->angles[ORIENTATION_PITCH]);
		}
		if(acc->angle_output)
			*acc->angle_output = acc->angles[ORIENTATION_PITCH];
	}

	//Petite correction à la main super clean
	acc->angles[ORIENTATION_PITCH] += 3.7603f ;

}

static void acc_data_callback(void){
	EVENT_Set_flag(FLAG_ACC_DATA_READY);
}






