/*
 * mpu.c
 *
 *  Created on: 26 nov. 2020
 *      Author: Théo
 */

#include "mpu.h"


//Private functions
void convert_gyro(mpu_t * mpu){
	mpu->gyro_raw[GYRO_AXE_X] = (int16_t)(mpu->gyro_data[0] << 8 | mpu->gyro_data[1]);
	mpu->gyro_raw[GYRO_AXE_Y] = (int16_t)(mpu->gyro_data[2] << 8 | mpu->gyro_data[3]);
	mpu->gyro_raw[GYRO_AXE_Z] = (int16_t)(mpu->gyro_data[4] << 8 | mpu->gyro_data[5]);

	//Convertion
	mpu->gyro[GYRO_AXE_X] = (float)mpu->gyro_raw[GYRO_AXE_X] * mpu->gyro_sensi ;
	mpu->gyro[GYRO_AXE_Y] = (float)mpu->gyro_raw[GYRO_AXE_Y] * mpu->gyro_sensi ;
	mpu->gyro[GYRO_AXE_Z] = (float)mpu->gyro_raw[GYRO_AXE_Z] * mpu->gyro_sensi ;
}

void convert_acc(mpu_t * mpu){

}

/*
 *
 */
sensor_state_e MPU_init(mpu_t * mpu, I2C_HandleTypeDef * hi2c){
	//Etat par default
	mpu->state = SENSOR_NOT_INIT ;


	//Requiert un I2C pour fonctionner
	if(hi2c == NULL)
		return mpu->state ;

	//Application des paramètres de connexion
	mpu->hi2c = hi2c ;

	mpu->adresse = MPU6050_I2C_ADDR ;

	//Check la présence du mpu
	mpu->hal_state = HAL_I2C_IsDeviceReady(mpu->hi2c, mpu->adresse, 2, 2);
	if(mpu->hal_state != HAL_OK){
		mpu->state = SENSOR_NOT_DETECTED ;
		return mpu->state ;
	}


	//Wakeup du mpu
	uint8_t wakeup_data = 0x00 ;
	mpu->hal_state  = HAL_I2C_Mem_Write(mpu->hi2c, mpu->adresse, MPU6050_PWR_MGMT_1, I2C_MEMADD_SIZE_8BIT, &wakeup_data, 1, 10);
	if(mpu->hal_state != HAL_OK){
		mpu->state = SENSOR_ERROR ;
		return mpu->state ;
	}


	//Si on réussi tous les test, on dit qu'on est bueno
	mpu->state = SENSOR_IDDLE ;
	return mpu->state ;
}

/*
 *
 */
sensor_state_e MPU_init_gyro(mpu_t * mpu, MPU_gyro_range_e gyro_range){
	mpu->gyro_range = gyro_range ;

	//Si mpu non utilisable
	if(mpu->state != SENSOR_IDDLE)
		return SENSOR_ERROR ;

	//Configuration de la sensi de l'acc dans le mpu
	uint8_t temp ;
	mpu->hal_state = HAL_I2C_Mem_Read(mpu->hi2c, mpu->adresse, MPU6050_GYRO_CONFIG, I2C_MEMADD_SIZE_8BIT, &temp, 1, 2);
	temp = (temp & 0xE7) | (uint8_t)gyro_range << 3;
	if(mpu->hal_state == HAL_OK)
		mpu->hal_state = HAL_I2C_Mem_Write(mpu->hi2c, mpu->adresse, MPU6050_GYRO_CONFIG, I2C_MEMADD_SIZE_8BIT, &temp, 1, 2);

	if(mpu->hal_state != HAL_OK)
	{
		mpu->state = SENSOR_ERROR ;
		return SENSOR_ERROR ;
	}

	switch(gyro_range){
		case MPU_GYRO_250s :
			mpu->gyro_sensi = (float)1 / MPU6050_GYRO_SENS_250 ;
			break;
		case MPU_GYRO_500s :
			mpu->gyro_sensi = (float)1 / MPU6050_GYRO_SENS_500 ;
			break;
		case MPU_GYRO_1000s :
			mpu->gyro_sensi = (float)1 / MPU6050_GYRO_SENS_1000 ;
			break;
		case MPU_GYRO_2000s :
			mpu->gyro_sensi = (float)1 / MPU6050_GYRO_SENS_2000 ;
			break;
	}
	return mpu->state ;
}

/*
 *
 */
sensor_state_e MPU_init_acc(mpu_t * mpu, MPU_acc_range_e acc_range){
	mpu->acc_range = acc_range ;
	uint8_t temp ;

	//Configuration de la sensi de l'acc dans le mpu
	HAL_I2C_Mem_Read(mpu->hi2c, mpu->adresse, MPU6050_ACCEL_CONFIG, I2C_MEMADD_SIZE_8BIT, &temp, 1, 2);
	temp = (temp & 0xE7) | (uint8_t)acc_range << 3;
	HAL_I2C_Mem_Write(mpu->hi2c, mpu->adresse, MPU6050_ACCEL_CONFIG, I2C_MEMADD_SIZE_8BIT, &temp, 1, 2);


	switch(acc_range){
		case MPU_ACC_2G :
			mpu->acc_sensi = (float)1 / MPU6050_ACCE_SENS_2 ;
			break;
		case MPU_ACC_4G :
			mpu->acc_sensi = (float)1 / MPU6050_ACCE_SENS_4 ;
			break;
		case MPU_ACC_8G :
			mpu->acc_sensi = (float)1 / MPU6050_ACCE_SENS_8 ;
			break;
		case MPU_ACC_16G :
			mpu->acc_sensi = (float)1 / MPU6050_ACCE_SENS_16 ;
			break;
	}

	return mpu->state ;
}

/*
 *
 */
sensor_state_e MPU_update_gyro(mpu_t * mpu){

	if(mpu->state != SENSOR_IDDLE){
		return SENSOR_ERROR ;
	}

	//Lit les nouvelles valeurs
	mpu->hal_state = HAL_I2C_Mem_Read(mpu->hi2c, mpu->adresse, MPU6050_GYRO_XOUT_H, I2C_MEMADD_SIZE_8BIT, mpu->gyro_data, 6, 5);

	if(mpu->hal_state != HAL_OK){
		return SENSOR_ERROR ;
	}

	convert_gyro(mpu);
	return SENSOR_IDDLE ;

}

sensor_state_e MPU_update_gyro_dma(mpu_t * mpu){

	mpu->start_time_dma = TIME_us();
	mpu->hal_state = HAL_I2C_Mem_Read_DMA(mpu->hi2c, mpu->adresse, MPU6050_GYRO_XOUT_H, I2C_MEMADD_SIZE_8BIT, mpu->gyro_data, 6);
	mpu->dma_state = MPU_DMA_GYRO_IN_PROGRESS ;

	if(mpu->hal_state == HAL_OK)
		mpu->state = SENSOR_IN_PROGRESS ;
	else if(mpu->hal_state == HAL_BUSY)
		mpu->state = SENSOR_HAL_BUSY ;
	else
		mpu->state = SENSOR_ERROR ;

	return mpu->state ;
}

void MPU_dma_transmit_done(mpu_t * mpu){
	if(!mpu->dma_state)
		return ;	//Si pas de lecture dma en cour on ignore

	else if(mpu->dma_state == MPU_DMA_GYRO_IN_PROGRESS)
		convert_gyro(mpu);
	else if(mpu->dma_state == MPU_DMA_ACC_IN_PROGREE)
		convert_acc(mpu);
	mpu->end_time_dma = TIME_us();
	mpu->delta_time_dma = mpu->end_time_dma - mpu->start_time_dma ;
	mpu->dma_state = MPU_DMA_IDDLE ;

}
























