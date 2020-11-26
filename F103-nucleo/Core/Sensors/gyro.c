/*
 * gyro.c
 *
 *  Created on: Nov 22, 2020
 *      Author: Théo
 */


#include "gyro.h"

sensor_init_e GYRO_init(gyro_t * gyro, gyros_e name, I2C_HandleTypeDef * hi2c, sensor_connectivity_e connectivity){
	sensor_init_e to_return = SENSOR_INIT_FAIL ;

	//Config la structure gyro_t
	gyro->connectivity = connectivity ;
	gyro->hi2c = hi2c ;
	gyro->name = name ;

	return to_return ;

}

