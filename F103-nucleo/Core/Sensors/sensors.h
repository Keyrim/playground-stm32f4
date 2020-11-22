/*
 * sensors.h
 *
 *  Created on: Nov 22, 2020
 *      Author: Théo
 */

#ifndef SENSORS_SENSORS_H_
#define SENSORS_SENSORS_H_

typedef enum sensor_connectivity_e{
	USE_UART,
	USE_UART_DMA,
	USE_I2C,
	USE_I2C_DMA
}sensor_connectivity_e;

typedef enum sensor_init_e{
	SENSOR_INIT_OK,
	SENSOR_INIT_FAIL
}sensor_init_e;

typedef enum sensor_end_e{
	SENSOR_END_OK,
	SENSOR_END_TIME_OUT,
	SENSOR_END_CONNECTIVITY_BUSY,
	SENSOR_END_FAIL
}sensor_end_e;

typedef struct sensors_t{

}sensors_t;

#endif /* SENSORS_SENSORS_H_ */
