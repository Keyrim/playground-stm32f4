/*
 * gyro.h
 *
 *  Created on: Nov 22, 2020
 *      Author: Théo
 */

#ifndef SENSORS_GYRO_H_
#define SENSORS_GYRO_H_


#include "stm32f1xx_hal.h"

typedef enum gyros_e{
	GYRO_MPU6050,
	GYRO_COUNT
}gyros_e;


typedef struct gyro_t{
	//Properties related to the hardware configuration
	I2C_HandleTypeDef * hi2c ;
	gyros_e name ;
	sensor_connectivity_e connectivity ;

	//Angle x
	float x ;
	float x_raw ;

	//Angle y
	float y ;
	float y_raw ;

	//Angle z
	float z ;
	float z_raw ;

	//Rapport (degrees/s / bit)
	float sensi ;
}gyro_t;

sensor_init_e GYRO_init(gyro_t * gyro, gyros_e name, I2C_HandleTypeDef * hi2c, sensor_connectivity_e connectivity);
sensor_end_e GYRO_update(gyro_t * gyro);

#endif /* SENSORS_GYRO_H_ */
