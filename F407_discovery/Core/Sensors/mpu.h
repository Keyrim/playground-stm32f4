/*
 * mpu.h
 *
 *  Created on: 26 nov. 2020
 *      Author: Théo
 */

#ifndef SENSORS_MPU_H_
#define SENSORS_MPU_H_

#include "stm32f4xx_hal.h"
#include "sensors.h"

/* Default I2C used */
#ifndef MPU6050_I2C
	#define	MPU6050_I2C					I2C1
	#define MPU6050_I2C_PINSPACK		I2C_PinsPack_1
#endif

/* Default I2C clock */
#ifndef MPU6050_I2C_CLOCK
#define MPU6050_I2C_CLOCK			400000
#endif

/* Default I2C address */
#define MPU6050_I2C_ADDR			0xD0

/* Who I am register value */
#define MPU6050_I_AM				0x68
#define MPU9250_I_AM				0x71
#define MPU9255_I_AM				0x73

/* MPU6050 registers */
#define MPU6050_AUX_VDDIO			0x01
#define MPU6050_SMPLRT_DIV			0x19
#define MPU6050_CONFIG				0x1A
#define MPU6050_GYRO_CONFIG			0x1B
#define MPU6050_ACCEL_CONFIG		0x1C
#define MPU6050_MOTION_THRESH		0x1F
#define MPU6050_INT_PIN_CFG			0x37
#define MPU6050_INT_ENABLE			0x38
#define MPU6050_INT_STATUS			0x3A
#define MPU6050_ACCEL_XOUT_H		0x3B
#define MPU6050_ACCEL_XOUT_L		0x3C
#define MPU6050_ACCEL_YOUT_H		0x3D
#define MPU6050_ACCEL_YOUT_L		0x3E
#define MPU6050_ACCEL_ZOUT_H		0x3F
#define MPU6050_ACCEL_ZOUT_L		0x40
#define MPU6050_TEMP_OUT_H			0x41
#define MPU6050_TEMP_OUT_L			0x42
#define MPU6050_GYRO_XOUT_H			0x43
#define MPU6050_GYRO_XOUT_L			0x44
#define MPU6050_GYRO_YOUT_H			0x45
#define MPU6050_GYRO_YOUT_L			0x46
#define MPU6050_GYRO_ZOUT_H			0x47
#define MPU6050_GYRO_ZOUT_L			0x48
#define MPU6050_MOT_DETECT_STATUS	0x61
#define MPU6050_SIGNAL_PATH_RESET	0x68
#define MPU6050_MOT_DETECT_CTRL		0x69
#define MPU6050_USER_CTRL			0x6A
#define MPU6050_PWR_MGMT_1			0x6B
#define MPU6050_PWR_MGMT_2			0x6C
#define MPU6050_FIFO_COUNTH			0x72
#define MPU6050_FIFO_COUNTL			0x73
#define MPU6050_FIFO_R_W			0x74
#define MPU6050_WHO_AM_I			0x75

/* Gyro sensitivities in °/s */
#define MPU6050_GYRO_SENS_250		((float) 131)
#define MPU6050_GYRO_SENS_500		((float) 65.5)
#define MPU6050_GYRO_SENS_1000		((float) 32.8)
#define MPU6050_GYRO_SENS_2000		((float) 16.4)

/* Acce sensitivities in g */
#define MPU6050_ACCE_SENS_2			((float) 16384)
#define MPU6050_ACCE_SENS_4			((float) 8192)
#define MPU6050_ACCE_SENS_8			((float) 4096)
#define MPU6050_ACCE_SENS_16		((float) 2048)

//Accelerometer range
typedef enum MPU_acc_range_e {
	MPU_ACC_2G = 0x00,
	MPU_ACC_4G = 0x01,
	MPU_ACC_8G = 0x02,
	MPU_ACC_16G = 0x03
} MPU_acc_range_e;

//Gyroscope range
typedef enum MPU_gyro_range_e{
	MPU_GYRO_250s = 0x00,
	MPU_GYRO_500s = 0x01,
	MPU_GYRO_1000s = 0x02,
	MPU_GYRO_2000s = 0x03
} MPU_gyro_range_e;

typedef struct{
	//Hal periph
	I2C_HandleTypeDef * hi2c ;
	DMA_HandleTypeDef * hdma ;

	//Mpu global state
	sensor_state_e state ;

	//Mpu settings
	uint8_t adresse ;
	sensor_connectivity_e connectivity ;

	//Gyroscope stuff
	int16_t gyro_raw[3];
	float gyro[3];
	MPU_gyro_range_e gyro_range ;
	float gyro_sensi ;

	//Accelerometer stuff
	int16_t acc_raw[3];
	float acc[3];
	MPU_acc_range_e acc_range ;
	float acc_sensi ;
}mpu_t;


//Init functions for the mpu
sensor_state_e MPU_init(mpu_t * mpu, sensor_connectivity_e connectivity, I2C_HandleTypeDef * hi2c, DMA_HandleTypeDef * hdma);
sensor_state_e MPU_init_gyro(mpu_t * mpu, MPU_gyro_range_e gyro_range);
sensor_state_e MPU_init_acc(mpu_t * mpu, MPU_acc_range_e acc_range);






#endif /* SENSORS_MPU_H_ */