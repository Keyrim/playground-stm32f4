/*
 * mpu.c
 *
 *  Created on: 26 nov. 2020
 *      Author: Théo
 */

#include "mpu.h"

/*
 *
 */
sensor_state_e MPU_init(mpu_t * mpu, sensor_connectivity_e connectivity, I2C_HandleTypeDef * hi2c, DMA_HandleTypeDef * hdma){

	//Vérification de la cochérence des paramètres entré
	if(hi2c == NULL)
		return SENSOR_INIT_FAILED ;

	if(connectivity % 2)
		if(hdma == NULL)
			return SENSOR_INIT_FAILED ;

	//Application des paramètres de connexion
	mpu->connectivity = connectivity ;
	if(hi2c != NULL)
		mpu->hi2c = hi2c ;
	if(hdma != NULL)
		mpu->hdma = hdma ;
	mpu->adresse = MPU6050_I2C_ADDR ;

	//Check la présence du mpu
	HAL_StatusTypeDef mpu_test ;
	switch(mpu->connectivity){
		case USE_I2C :
			mpu_test = HAL_I2C_IsDeviceReady(mpu->hi2c, mpu->adresse, 2, 2);
			break;

		default :
			mpu_test = HAL_ERROR ;
			break;
	}
	if(mpu_test != HAL_OK)
		return SENSOR_INIT_FAILED ;

	//Wakeup du mpu
	uint8_t wakeup_data = 0x00 ;
	switch(mpu->connectivity){
		case USE_I2C :
			mpu_test = HAL_I2C_Mem_Write(mpu->hi2c, mpu->adresse, MPU6050_PWR_MGMT_1, I2C_MEMADD_SIZE_8BIT, &wakeup_data, 1, 10);
			break;

		default :
			mpu_test = HAL_ERROR ;
			break;
	}
	if(mpu_test != HAL_OK)
		return SENSOR_INIT_FAILED ;

	mpu->state = SENSOR_INIT_OK ;
	return SENSOR_INIT_OK ;
}

/*
 *
 */
sensor_state_e MPU_init_gyro(mpu_t * mpu, MPU_gyro_range_e gyro_range){

}

/*
 *
 */
sensor_state_e MPU_init_acc(mpu_t * mpu, MPU_acc_range_e acc_range){

}
