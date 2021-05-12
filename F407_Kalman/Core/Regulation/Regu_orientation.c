/*
 * Regu_orientation.c
 *
 *  Created on: Dec 13, 2020
 *      Author: Théo
 */


#include "Regu_orientation.h"
static regu_orientation_t * regu_orientation ;
static orientation_t * orientation ;

//	------------------------------	Angular Speed Regulation settings	-----------------------------------------
float filter_pid_velocity[3] = {0.02326917552f, 1.694915254f, -0.7181844298f};

float pid_velocity_settings[PID_NB_SETTINGS] = {0.4f, 0.02f, 0.008f, REGU_ORIENTATION_FREQUENCY, 300};



//	------------------------------	Angular Pos Regulation settings	-----------------------------------------
float filter_pid_angle[3] = {1.0f, 0.0f, 0.0f};
float pid_angle_settings[PID_NB_SETTINGS] = {6.0f, 0.0f, 0.0f, REGU_ORIENTATION_FREQUENCY, 500};


void REGULATION_ORIENTATION_Init(regu_orientation_t * regu_orientation_, orientation_t * orientation_, int16_t * output){
	orientation = orientation_ ;
	regu_orientation = regu_orientation_ ;
	regu_orientation->mode = REGULATION_MODE_OFF ;
	regu_orientation->output = output ;


	//Init PID angular speed
	PID_init(&regu_orientation->pid_angular_speed, pid_velocity_settings, FILTER_SECOND_ORDER, filter_pid_velocity);

	//Init PID angular position
	PID_init(&regu_orientation->pid_angular_pos, pid_angle_settings, FILTER_FIRST_ORDER, filter_pid_angle);


}

void REGULATION_ORIENTATION_Set_Regulation_Mode(regulation_mode_e regu_mode){
	regu_orientation->mode = regu_mode;
}

void REGULATION_ORIENTATION_Process(void){

	switch(regu_orientation->mode){
		case REGULATION_MODE_OFF:
			*regu_orientation->output = 0;
			break;
		case REGULATION_MODE_MANUAL:
			*regu_orientation->output = regu_orientation->motor_consigne;
			break;
	}

}














