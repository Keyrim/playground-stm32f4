/*
 * Data_logger.c
 *
 *  Created on: 22 déc. 2020
 *      Author: theom
 */

#include "Data_logger.h"
#include "string.h"
#include "../OS/tasks/task.h"
#include "../OS/scheduler/scheduler.h"
#include "../High_lvl/High_lvl.h"
#include "../State_Space_Model/Model.h"
static system_t * sys ;





#define DEFINE_DATA(id_, ptr_, format_, name_, use_) {	\
	data_list[id_].id = id_ ;			\
	data_list[id_].data = ptr_ ;		\
	data_list[id_].format = format_;	\
	data_list[id_].name = name_;			\
	data_list[id_].len_name = sizeof(name_)-1;	\
	data_list[id_].use_format = use_;				\
}


typedef enum data_logger_state_e{
	LOGGER_IDDLE,
	LOGGER_TRANSMIT_CONFIG,
	LOGGER_LOG
}data_logger_state_e;


static data_t data_list[DATA_ID_COUNT] ;
static data_logger_state_e state = LOGGER_IDDLE;
static bool_e config_not_sent = TRUE ;
static bool_e start_flag = FALSE;
static bool_e send_config_flag = FALSE ;
static bool_e stop_flag = FALSE;
static uint8_t tmp[30];
static uint8_t tmp_len ;
static uint8_t id_init_counter = 0;
static bool_e is_any_data_used = FALSE;

//Data name def
static uint8_t name_roll_angle[] = "Angle ROLL";
static uint8_t name_pitch_angle[] = "Angle PITCH";

static uint8_t name_roll_angle_acc[] = "AnglAcc ROLL";
static uint8_t name_pitch_angle_acc[] = "AnglACC PITCH";

static uint8_t name_roll_gyro [] = "Gyro ROLL";
static uint8_t name_pitch_gyro[] = "Gyro PITCH";
static uint8_t name_yaw_gyro[] = "Gyro YAW";

static uint8_t name_roll_acc[] = "Acc ROLL";
static uint8_t name_pitch_acc[] = "Acc PITCH";
static uint8_t name_yaw_acc[] = "Acc YAW";

static uint8_t name_roll_acc_raw[] = "Acc Raw ROLL";
static uint8_t name_pitch_acc_raw[] = "Acc Raw PITCH";
static uint8_t name_yaw_acc_raw[] = "Acc Raw YAW";


static uint8_t name_roll_gyro_raw [] = "Gyro raw ROLL";
static uint8_t name_pitch_gyro_raw[] = "Gyro raw PITCH";
static uint8_t name_yaw_gyro_raw[] = "Gyro raw YAW";

static uint8_t name_state_vector_angle_y[] = "State Angle Y";
static uint8_t name_state_vector_gyro_y[] = "State Gyro Y";
static uint8_t name_measurement_angle_y[] = "Msrmt Angle y";
static uint8_t name_measurement_gyro_y[] = "Msrmt Gyro Y";
static uint8_t name_kalman_predict_angle_y[] = "Predict Angle Y";
static uint8_t name_kalman_predict_gyro_y[] = "Predict Gyro Y";

static uint8_t name_simulation_output1[] = "Simu Y1";

static uint8_t name_config_request[] = "Send Config";
static uint8_t name_start_transfer[] = "Start Transfer";
static uint8_t name_stop_transfer[] = "Stop Transfer";
static uint8_t name_idle_mode[] = "Idle Mode";
static uint8_t name_full_manual_mode[] = "Full Manual";
static uint8_t name_start_simulation[] = "Start Simulation";

static uint8_t name_mcu_load[] = "CPU Load";

static uint8_t name_input_pwm1[] = "PWM 1";


void DATA_LOGGER_Init(system_t * sys_){

	sys = sys_;

	//	-----------------------------------------------	Définitions des data	-----------------------------------------------------------------
	//Angle
	DEFINE_DATA(DATA_ID_ROLL_ANGLE, (uint8_t*)&sys->orientation.angular_position[ORIENTATION_ROLL], 									DATA_FORMAT_16B_FLOAT_1D, 	name_roll_angle,							NOT_USED);
	DEFINE_DATA(DATA_ID_PITCH_ANGLE, (uint8_t*)&sys->orientation.angular_position[ORIENTATION_PITCH], 									DATA_FORMAT_16B_FLOAT_1D, 	name_pitch_angle,							USED_AS_OUTPUT);

	//Angles acc
	DEFINE_DATA(DATA_ID_ROLL_ACC_ANGLE, (uint8_t*)&sys->sensors.acc.angles[ORIENTATION_ROLL], 											DATA_FORMAT_16B_FLOAT_1D, 	name_roll_angle_acc,						NOT_USED);
	DEFINE_DATA(DATA_ID_PITCH_ACC_ANGLE, (uint8_t*)&sys->sensors.acc.angles[ORIENTATION_PITCH], 										DATA_FORMAT_16B_FLOAT_1D, 	name_pitch_angle_acc,						NOT_USED);

	//Angle rate
	DEFINE_DATA(DATA_ID_ROLL_GYRO, (uint8_t*)&sys->sensors.gyro.filtered[ORIENTATION_ROLL], 											DATA_FORMAT_16B_FLOAT_1D, 	name_roll_gyro, 							NOT_USED);
	DEFINE_DATA(DATA_ID_PITCH_GYRO, (uint8_t*)&sys->sensors.gyro.filtered[ORIENTATION_PITCH], 											DATA_FORMAT_16B_FLOAT_1D, 	name_pitch_gyro, 							NOT_USED);
	DEFINE_DATA(DATA_ID_YAW_GYRO, (uint8_t*)&sys->sensors.gyro.filtered[ORIENTATION_YAW], 												DATA_FORMAT_16B_FLOAT_1D, 	name_yaw_gyro, 			 					NOT_USED);

	//Acceleration
	DEFINE_DATA(DATA_ID_ROLL_ACC, (uint8_t*)&sys->sensors.acc.filtered[ORIENTATION_ROLL], 												DATA_FORMAT_16B_FLOAT_3D, 	name_roll_acc, 								NOT_USED);
	DEFINE_DATA(DATA_ID_PITCH_ACC, (uint8_t*)&sys->sensors.acc.filtered[ORIENTATION_PITCH], 											DATA_FORMAT_16B_FLOAT_3D, 	name_pitch_acc, 							NOT_USED);
	DEFINE_DATA(DATA_ID_YAW_ACC, (uint8_t*)&sys->sensors.acc.filtered[ORIENTATION_YAW], 												DATA_FORMAT_16B_FLOAT_3D, 	name_yaw_acc, 								NOT_USED);

	//Acceleration
	DEFINE_DATA(DATA_ID_ROLL_ACC_RAW, (uint8_t*)&sys->sensors.acc.raw[ORIENTATION_ROLL], 												DATA_FORMAT_16B_FLOAT_3D, 	name_roll_acc_raw, 							NOT_USED);
	DEFINE_DATA(DATA_ID_PITCH_ACC_RAW, (uint8_t*)&sys->sensors.acc.raw[ORIENTATION_PITCH], 												DATA_FORMAT_16B_FLOAT_3D, 	name_pitch_acc_raw, 						NOT_USED);
	DEFINE_DATA(DATA_ID_YAW_ACC_RAW, (uint8_t*)&sys->sensors.acc.raw[ORIENTATION_YAW], 													DATA_FORMAT_16B_FLOAT_3D, 	name_yaw_acc_raw, 							NOT_USED);

	//Angle Rate raw
	DEFINE_DATA(DATA_ID_ROLL_GYRO_RAW, (uint8_t*)&sys->sensors.gyro.raw[ORIENTATION_ROLL], 												DATA_FORMAT_16B_FLOAT_1D, 	name_roll_gyro_raw, 						NOT_USED);
	DEFINE_DATA(DATA_ID_PITCH_GYRO_RAW, (uint8_t*)&sys->sensors.gyro.raw[ORIENTATION_PITCH], 											DATA_FORMAT_16B_FLOAT_1D, 	name_pitch_gyro_raw,						NOT_USED);
	DEFINE_DATA(DATA_ID_YAW_GYRO_RAW, (uint8_t*)&sys->sensors.gyro.raw[ORIENTATION_YAW], 												DATA_FORMAT_16B_FLOAT_1D, 	name_yaw_gyro_raw, 							NOT_USED);

	//State space stuff
	DEFINE_DATA(DATA_ID_STATE_VECTOR_ANGLE_Y, (uint8_t*)&sys->ss.x_array[STATE_VECTOR_ANGLE_Y], 										DATA_FORMAT_16B_FLOAT_1D, 	name_state_vector_angle_y, 					USED_AS_OUTPUT);
	DEFINE_DATA(DATA_ID_STATE_VECTOR_GYRO_Y, (uint8_t*)&sys->ss.x_array[STATE_VECTOR_ANGLE_RATE_Y], 									DATA_FORMAT_16B_FLOAT_1D, 	name_state_vector_gyro_y, 					USED_AS_OUTPUT);
	DEFINE_DATA(DATA_ID_MEASUREMENTS_ANGLE_Y, (uint8_t*)&sys->ss.z_array[MEASUREMENT_VECTOR_ANGLE_Y], 									DATA_FORMAT_16B_FLOAT_1D, 	name_measurement_angle_y, 					USED_AS_OUTPUT);
	DEFINE_DATA(DATA_ID_MEASUREMENTS_GYRO_Y, (uint8_t*)&sys->ss.z_array[MEASUREMENT_VECTOR_GYRO_Y], 									DATA_FORMAT_16B_FLOAT_1D, 	name_measurement_gyro_y, 					USED_AS_OUTPUT);

	//Kalman stuff
	DEFINE_DATA(DATA_ID_KALMAN_STATE_VECTOR_ANGLE_Y_PREDICT, (uint8_t*)&sys->kalman.x_predict_array[STATE_VECTOR_ANGLE_Y], 				DATA_FORMAT_16B_FLOAT_1D, 	name_kalman_predict_angle_y, 				USED_AS_OUTPUT);
	DEFINE_DATA(DATA_ID_KALMAN_STATE_VECTOR_ANGLE_RATE_Y_PREDICT, (uint8_t*)&sys->kalman.x_predict_array[STATE_VECTOR_ANGLE_RATE_Y],	DATA_FORMAT_16B_FLOAT_1D, 	name_kalman_predict_gyro_y, 				USED_AS_OUTPUT);

	DEFINE_DATA(DATA_ID_SIMULATION_OUTPUT1,	 NULL, 																						DATA_FORMAT_16B_FLOAT_2D, 	name_simulation_output1, 					NOT_USED);

	DEFINE_DATA(DATA_ID_MCU_LOAD,	 (uint8_t*)&sys->soft.cpu_load, 																	DATA_FORMAT_16B_FLOAT_2D, 	name_mcu_load, 								USED_AS_OUTPUT);

	//Buttons
	DEFINE_DATA(DATA_ID_CONFIG_REQUEST, 	NULL, 																				DATA_FORMAT_0B_BUTTON, 		name_config_request, 						USED_AS_INPUT );
	DEFINE_DATA(DATA_ID_START_TRANSFER, 	NULL, 																				DATA_FORMAT_0B_BUTTON, 		name_start_transfer, 		 				USED_AS_INPUT);
	DEFINE_DATA(DATA_ID_STOP_TRANSFER, 		NULL, 																				DATA_FORMAT_0B_BUTTON, 		name_stop_transfer, 						USED_AS_INPUT);
	DEFINE_DATA(DATA_ID_IDLE_MODE, 			NULL, 																				DATA_FORMAT_0B_BUTTON, 		name_idle_mode, 							USED_AS_INPUT);
	DEFINE_DATA(DATA_ID_FULL_MANUAL_MODE, 	NULL, 																				DATA_FORMAT_0B_BUTTON, 		name_full_manual_mode, 						USED_AS_INPUT);
	DEFINE_DATA(DATA_ID_START_SIMULATION, 	NULL, 																				DATA_FORMAT_0B_BUTTON, 		name_start_simulation, 						USED_AS_INPUT);


	DEFINE_DATA(DATA_ID_INPUT_PWM_1, 		NULL, 																				DATA_FORMAT_16B_INT16, 	name_input_pwm1, 							USED_AS_INPUT);


}

void DATA_LOGGER_Start(){
	start_flag = TRUE;
}

void DATA_LOGGER_Stop(){
	stop_flag = TRUE;
}

void DATA_LOGGER_Main(void){

	static data_logger_state_e previous_state 	= LOGGER_TRANSMIT_CONFIG ;
	bool_e entrance = previous_state != state ;
	previous_state = state ;

	switch(state){
		case LOGGER_IDDLE :
			if(entrance){
				//Quand on passe en mode iddle en envoit l'id du boutton de config toute les 1/2 secondes
				SCHEDULER_reschedule_task(TASK_LOGGER, 500000);
			}

			tmp_len = DATA_LOGGER_Get_Data_Config(DATA_ID_CONFIG_REQUEST, tmp);
			TELEMETRY_Send_Data(tmp, tmp_len);

			//Si on a reçu une requête pour envoyer la config, on y go
			if(send_config_flag){
				//Reset flags
				start_flag = FALSE;
				stop_flag = FALSE;
				send_config_flag = FALSE;
				//Change the state
				if(config_not_sent){
					id_init_counter = 0;
					is_any_data_used = FALSE;
					state = LOGGER_TRANSMIT_CONFIG;

				}
				else
					state = LOGGER_LOG;
			}
			else if(start_flag)
			{
				start_flag = FALSE;
				stop_flag = FALSE;
				state = LOGGER_LOG;
			}
			break;
		case LOGGER_TRANSMIT_CONFIG :

			if(entrance)
				SCHEDULER_reschedule_task(TASK_LOGGER, 10000);

			//If the data[id_init_counter] is used
			while(data_list[id_init_counter].use_format == NOT_USED && id_init_counter < DATA_ID_COUNT){
				id_init_counter ++ ;
			}
			if(id_init_counter < DATA_ID_COUNT){
				is_any_data_used = TRUE;
				//Get a data config
				tmp_len = DATA_LOGGER_Get_Data_Config(id_init_counter++, tmp);
				//Send the data config
				TELEMETRY_Send_Data(tmp, tmp_len);
			}

			else
			{
				if(is_any_data_used)
				{
					//config_not_sent = FALSE ;
					state = LOGGER_LOG ;
				}
				else
					state = LOGGER_IDDLE ;

			}

			break;

		case LOGGER_LOG:
			if(entrance)
				SCHEDULER_reschedule_task(TASK_LOGGER, 10000);	//Période d'envoie des données

			if(stop_flag)
				state = LOGGER_IDDLE;

			for(uint8_t d = 0; d < DATA_ID_COUNT; d++)
			{
				if(data_list[d].use_format == USED_AS_OUTPUT && (data_list[d].data != NULL))
				{
					tmp_len = DATA_LOGGER_Get_Data_Value(d, tmp);
					TELEMETRY_Send_Data(tmp, tmp_len);
				}
			}
			break;
	}

}

void DATA_LOGGER_Reception(uint8_t * input_buffer){
	uint8_t id = input_buffer[0];
	uint16_t tmp_uint_16 = 0 ;
	int16_t tmp_int_16 = 0;
	switch(data_list[id].format){
		case DATA_FORMAT_0B_BUTTON:
			//We can create an action for each id
			switch(id)
			{
				default:
					break;
				case DATA_ID_CONFIG_REQUEST:
					send_config_flag = TRUE;
					break;
				case DATA_ID_START_TRANSFER:
					DATA_LOGGER_Start();
					break;
				case DATA_ID_STOP_TRANSFER:
					DATA_LOGGER_Stop();
					break;
				case DATA_ID_IDLE_MODE:
					HIGH_LVL_Set_Mode(HIGH_LVL_STATE_IDLE);
					break;
				case DATA_ID_FULL_MANUAL_MODE:
					HIGH_LVL_Set_Mode(HIGH_LVL_STATE_FULL_MANUAL);
					break;
				case DATA_ID_START_SIMULATION:
					sys->simulate = 1 - sys->simulate;
					break;
			}
			break;
		case DATA_FORMAT_8B:
			break;
		case DATA_FORMAT_8B_FLOAT_0D:
			break;
		case DATA_FORMAT_8B_FLOAT_1D:
			break;
		case DATA_FORMAT_8B_PWM:
			tmp_uint_16 = 4*input_buffer[1];
			break;
		case DATA_FORMAT_16B_INT16:
			tmp_int_16 = (int16_t)input_buffer[1] << 8;
			tmp_int_16 += (int16_t)input_buffer[2];
			switch(id){
				case DATA_ID_INPUT_PWM_1:
					sys->regulation.motor_consigne = MIN(500, tmp_int_16);
					break;
				default:
					break;
			}

			break;
		case DATA_FORMAT_16B_UINT16:
			tmp_uint_16 = (uint16_t)input_buffer[1] << 8;
			tmp_uint_16 += (uint16_t)input_buffer[2];
			switch(id){
				case DATA_ID_INPUT_PWM_1:
					sys->regulation.motor_consigne = MIN(130, tmp_uint_16);
					break;
				default:
					break;
			}

		case DATA_FORMAT_16B_FLOAT_1D:
			break;
		default:
			break;

	}
}

uint8_t DATA_LOGGER_Get_Data_Value(data_id_e id, uint8_t * buffer){

//	uint16_t tmp_u16 = 0;
	int16_t tmp_16 = 0;
	float tmp_float = 0;

	//Premier octet contient l'id
	buffer[0] = id ;

	//Formatage de la donnée en fonction du type et du format
	switch(data_list[id].format){
		case DATA_FORMAT_8B:
			buffer[1] = *data_list[id].data ;
			break;
		case DATA_FORMAT_8B_PWM:
			break;
		case DATA_FORMAT_8B_FLOAT_0D:
			tmp_float = *(float*)data_list[id].data ;
			buffer[1] = (int8_t)tmp_float;
			break;
		case DATA_FORMAT_8B_FLOAT_1D:
			break;
		case DATA_FORMAT_16B:
			break;
		case DATA_FORMAT_16B_FLOAT_1D:
			tmp_float = *(float*)data_list[id].data;
			tmp_16 = tmp_float * 10;
			buffer[1] = (uint8_t)((tmp_16 >> 8) & 0b11111111) ;
			buffer[2] = (uint8_t)(tmp_16 & 0b11111111);
			break;
		case DATA_FORMAT_16B_FLOAT_2D:
			tmp_float = *(float*)data_list[id].data;
			tmp_16 = tmp_float * 100;
			buffer[1] = (uint8_t)((tmp_16 >> 8) & 0b11111111) ;
			buffer[2] = (uint8_t)(tmp_16 & 0b11111111);
			break;
		case DATA_FORMAT_16B_FLOAT_3D:
			tmp_float = *(float*)data_list[id].data;
			tmp_16 = tmp_float * 1000;
			buffer[1] = (uint8_t)((tmp_16 >> 8) & 0b11111111) ;
			buffer[2] = (uint8_t)(tmp_16 & 0b11111111);
			break;
		case DATA_FORMAT_16B_UINT16:
			tmp_16 = *(uint16_t*)data_list[id].data ;
			buffer[1] = (uint8_t)((tmp_16 >> 8) & 0b11111111) ;
			buffer[2] = (uint8_t)(tmp_16 & 0b11111111);
			break;
		case DATA_FORMAT_16B_INT16:
			tmp_16 = *(int16_t*)data_list[id].data ;
			buffer[1] = (uint8_t)((tmp_16 >> 8) & 0b11111111) ;
			buffer[2] = (uint8_t)(tmp_16 & 0b11111111);
			break;
		default:
			break;
	}

	//Return the len of data
	return (data_list[id].format / 20) + 1;

}

uint8_t DATA_LOGGER_Get_Data_Config(data_id_e id, uint8_t * buffer){
	//New data so we start with 255
	buffer[0] = 255;
	//Then name len
	buffer[1] = data_list[id].len_name;
	//Then format
	buffer[2] = data_list[id].format ;
	//Taille du nom
	buffer[3] = id	 ;
	if(data_list[id].use_format == USED_AS_INPUT)	//Si cette donnée sert d'entré pour le système, on l'indique en rajoute 0b10000000 sur l'id d'origine
		buffer[3] += 128 ;
	//Copie du nom
	memcpy(&buffer[4], data_list[id].name, data_list[id].len_name);

	//And that s it
	return 4 + data_list[id].len_name ;
}

data_t * DATA_LOGGER_Get_Data(data_id_e id){
	return &data_list[id];
}



