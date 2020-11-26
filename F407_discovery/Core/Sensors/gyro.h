/*
 * gyro.h
 *
 *  Created on: Nov 22, 2020
 *      Author: Théo
 */

#ifndef SENSORS_GYRO_H_
#define SENSORS_GYRO_H_


#include "stm32f4xx_hal.h"
#include "sensors.h"

/* Default I2C address */
#define MPU6050_I2C_ADDR			0xD0

typedef enum gyros_e{
	GYRO_MPU6050,
	GYRO_COUNT
}gyros_e;


typedef struct gyro_t{
	//Properties related to the hardware configuration
	I2C_HandleTypeDef * hi2c ;
	gyros_e name ;
	sensor_connectivity_e connectivity ;
	HAL_StatusTypeDef hal_test ;


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


#endif /* SENSORS_GYRO_H_ */
