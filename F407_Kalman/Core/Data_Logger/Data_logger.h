/*
 * Data_logger.h
 *
 *  Created on: 22 déc. 2020
 *      Author: theom
 */

#ifndef DATA_LOGGER_DATA_LOGGER_H_
#define DATA_LOGGER_DATA_LOGGER_H_

#include "../OS/system_d.h"

typedef enum data_id_e{

	DATA_ID_ROLL_ANGLE,
	DATA_ID_PITCH_ANGLE,

	DATA_ID_ROLL_ACC_ANGLE,
	DATA_ID_PITCH_ACC_ANGLE,

	DATA_ID_ROLL_GYRO,
	DATA_ID_PITCH_GYRO,
	DATA_ID_YAW_GYRO,

	DATA_ID_ROLL_GYRO_RAW,
	DATA_ID_PITCH_GYRO_RAW,
	DATA_ID_YAW_GYRO_RAW,

	DATA_ID_ROLL_ACC,
	DATA_ID_PITCH_ACC,
	DATA_ID_YAW_ACC,

	DATA_ID_ROLL_ACC_RAW,
	DATA_ID_PITCH_ACC_RAW,
	DATA_ID_YAW_ACC_RAW,

	//State space vector
	DATA_ID_STATE_VECTOR_ANGLE_Y,
	DATA_ID_STATE_VECTOR_GYRO_Y,

	//Kalman's estimation of state space vector
	DATA_ID_KALMAN_STATE_VECTOR_ANGLE_Y_PREDICT,
	DATA_ID_KALMAN_STATE_VECTOR_ANGLE_RATE_Y_PREDICT,

	DATA_ID_CONSIGNE_ANGLE_ROLL,

	DATA_ID_SIMULATION_OUTPUT1,

	DATA_ID_MCU_LOAD,


	//Inputs
	DATA_ID_INPUT_PWM_1,

	//Buttons
	DATA_ID_CONFIG_REQUEST,		//Si je reçois ça, j'envoit la configuration des données
	DATA_ID_START_TRANSFER,
	DATA_ID_STOP_TRANSFER,
	DATA_ID_IDLE_MODE,
	DATA_ID_FULL_MANUAL_MODE,
	DATA_ID_START_SIMULATION,

	DATA_ID_COUNT
}data_id_e;

typedef enum data_format_e{
	//O octet
	DATA_FORMAT_0B_BUTTON = 0,	//Envoyé par un autre périphérique => déclenche une action sur le drone potentiellement
	//1 octet
	DATA_FORMAT_8B = 20,
	DATA_FORMAT_8B_PWM,
	DATA_FORMAT_8B_FLOAT_1D,
	DATA_FORMAT_8B_FLOAT_0D,
	//2 octets
	DATA_FORMAT_16B = 40,
	DATA_FORMAT_16B_FLOAT_1D,
	DATA_FORMAT_16B_FLOAT_2D,
	DATA_FORMAT_16B_FLOAT_3D,
	DATA_FORMAT_16B_UINT16,
	DATA_FORMAT_16B_INT16,
}data_format_e;

typedef enum use_format_e{
	NOT_USED,
	USED_AS_OUTPUT,
	USED_AS_INPUT
}use_format_e;


typedef struct data_t{
	uint8_t len;
	uint8_t * data;
	uint8_t id;
	data_format_e format;
	uint8_t * name ;
	uint8_t len_name;
	use_format_e use_format;
}data_t;


void DATA_LOGGER_Init(system_t * sys_);
void DATA_LOGGER_Main(void);
void DATA_LOGGER_Reception(uint8_t * input_buffer);
data_t * DATA_LOGGER_Get_Data(data_id_e id);
uint8_t DATA_LOGGER_Get_Data_Value(data_id_e id, uint8_t * buffer);
uint8_t DATA_LOGGER_Get_Data_Config(data_id_e id, uint8_t * buffer);

#endif /* DATA_LOGGER_DATA_LOGGER_H_ */
