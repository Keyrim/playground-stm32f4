/*
 * mpu.c
 *
 *  Created on: 26 nov. 2020
 *      Author: Théo
 */

#include "mpu.h"
#include "../OS/time.h"



//Ces trois tableaux sont liés et permettent de savoir pour un I2C donné, quelle est le mpu associé
static I2C_HandleTypeDef * I2C_handlers[NB_MPU_USED] ;


static mpu_t * mpu_handlers[NB_MPU_USED];
static uint8_t mpu_init_compteur = 0 ;

//Private function prototypes
static void MPU_dma_transmit_complete(mpu_t * mpu);
//static void MPU_dma_transmit_half_complete(mpu_t * mpu);

static void convert_gyro(mpu_t * mpu);
static void convert_acc(mpu_t * mpu);

//Private functions def
static void convert_gyro(mpu_t * mpu){
	mpu->gyro_raw[GYRO_AXE_X] = (int16_t)(mpu->gyro_data[0] << 8 | mpu->gyro_data[1]);
	mpu->gyro_raw[GYRO_AXE_Y] = (int16_t)(mpu->gyro_data[2] << 8 | mpu->gyro_data[3]);
	mpu->gyro_raw[GYRO_AXE_Z] = (int16_t)(mpu->gyro_data[4] << 8 | mpu->gyro_data[5]);

	//Convertion
	mpu->gyro[GYRO_AXE_X] = (float)mpu->gyro_raw[GYRO_AXE_X] * mpu->gyro_sensi ;
	mpu->gyro[GYRO_AXE_Y] = (float)mpu->gyro_raw[GYRO_AXE_Y] * mpu->gyro_sensi ;
	mpu->gyro[GYRO_AXE_Z] = (float)mpu->gyro_raw[GYRO_AXE_Z] * mpu->gyro_sensi ;
}
static void convert_acc(mpu_t * mpu){
	mpu->acc_raw[ACC_AXE_X] = (int16_t)(mpu->acc_data[0] << 8 | mpu->acc_data[1]);
	mpu->acc_raw[ACC_AXE_Y] = (int16_t)(mpu->acc_data[2] << 8 | mpu->acc_data[3]);
	mpu->acc_raw[ACC_AXE_Z] = (int16_t)(mpu->acc_data[4] << 8 | mpu->acc_data[5]);

	//Convertion
	mpu->acc[ACC_AXE_X] = (float)mpu->acc_raw[ACC_AXE_X] * mpu->acc_sensi ;
	mpu->acc[ACC_AXE_Y] = (float)mpu->acc_raw[ACC_AXE_Y] * mpu->acc_sensi ;
	mpu->acc[ACC_AXE_Z] = (float)mpu->acc_raw[ACC_AXE_Z] * mpu->acc_sensi ;
}

/*
 *
 */
sensor_request_e MPU_init(mpu_t * mpu, I2C_HandleTypeDef * hi2c){


	//Requiert un SPI pour fonctionner
	if(hi2c == NULL)
		return SENSOR_REQUEST_FAIL ;



	//Etat par default
	mpu->state = SENSOR_NOT_INIT ;



	//Définit d'après la doc pour les registres du mpu
	mpu->gyro_data = &mpu->data[8] ;
	mpu->acc_data = &mpu->data[0] ;


	//Application des paramètres de connexion
	mpu->hi2c = hi2c ;

	mpu->adresse = MPU6050_I2C_ADDR ;

	mpu_handlers[mpu_init_compteur] = mpu ;
	I2C_handlers[mpu_init_compteur++] = hi2c ;

	//Wakeup mpu
	uint8_t wakeup_data = 0x80 ;

	mpu->hal_state = HAL_I2C_Mem_Write(mpu->hi2c, mpu->adresse, MPU6050_PWR_MGMT_1, I2C_MEMADD_SIZE_8BIT, &wakeup_data, 1, 10);

	HAL_Delay(15);

	//Clock select
	uint8_t mpu_clock_sel_pll_gyroz = 0x03;
	mpu->hal_state = HAL_I2C_Mem_Write(mpu->hi2c, mpu->adresse, MPU6050_PWR_MGMT_1, I2C_MEMADD_SIZE_8BIT, &mpu_clock_sel_pll_gyroz, 1, 10);


	HAL_Delay(15);


	//Pour l'init on ne s'occupe pas des cas HAL busy etc, c'est bon ou c'est pas bon c est tout^^
	if(mpu->hal_state != HAL_OK){
		mpu->state = SENSOR_ERROR ;
		return SENSOR_REQUEST_FAIL ;
	}

	mpu->state = SENSOR_IDDLE ;
	return SENSOR_REQUEST_OK;
}

/*
 *
 */
sensor_request_e MPU_init_gyro(mpu_t * mpu, MPU_gyro_range_e gyro_range, void (*gyro_data_callback)){

	mpu->gyro_range = gyro_range ;
	mpu->gyro_data_callback = gyro_data_callback ;

	//Si mpu non utilisable
	if(mpu->state != SENSOR_IDDLE)
		return SENSOR_REQUEST_FAIL ;


	//Configuration de la sensi du gyro dans le mpu
	uint8_t temp ;
	mpu->hal_state = HAL_I2C_Mem_Read(mpu->hi2c, mpu->adresse, MPU6050_GYRO_CONFIG, I2C_MEMADD_SIZE_8BIT, &temp, 1, 2);
	temp = (temp & 0xE7) | (uint8_t)gyro_range << 3;
	if(mpu->hal_state == HAL_OK)
		mpu->hal_state = HAL_I2C_Mem_Write(mpu->hi2c, mpu->adresse, MPU6050_GYRO_CONFIG, I2C_MEMADD_SIZE_8BIT, &temp, 1, 2);



	if(mpu->hal_state != HAL_OK)
	{
		mpu->state = SENSOR_ERROR ;
		return SENSOR_REQUEST_FAIL ;
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

	return SENSOR_REQUEST_OK;
}

/*
 *
 */
sensor_request_e MPU_init_acc(mpu_t * mpu, MPU_acc_range_e acc_range, void (*acc_data_callback)){
	mpu->acc_range = acc_range ;
	mpu->acc_data_callback = acc_data_callback ;
	//Si mpu non utilisable
	if(mpu->state != SENSOR_IDDLE)
		return SENSOR_REQUEST_FAIL ;


	//Configuration de la sensi de l'acc dans le mpu
	uint8_t temp ;
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
	return SENSOR_REQUEST_OK;
}

/*
 *
 */




sensor_request_e MPU_update_all_dma(mpu_t * mpu){

	mpu->hal_state = HAL_I2C_Mem_Read_DMA(mpu->hi2c, mpu->adresse, MPU6050_ACCEL_XOUT_H, I2C_MEMADD_SIZE_8BIT, mpu->data, 14);

	if(mpu->hal_state == HAL_OK)
	{
		mpu->state = SENSOR_BUSY ;
		mpu->dma_state = MPU_DMA_ACC_AND_GYRO_IN_PROGRESS;
	}
	else if(mpu->hal_state == HAL_BUSY)
	{
		return SENSOR_REQUEST_BUSY;
	}
	else
	{
		mpu->state = SENSOR_ERROR ;
		return SENSOR_REQUEST_FAIL ;
	}

	return SENSOR_REQUEST_OK;
}



static void MPU_dma_transmit_complete(mpu_t * mpu){

	switch(mpu->dma_state){
		case MPU_DMA_GYRO_IN_PROGRESS:
			convert_gyro(mpu);
			mpu->gyro_data_callback();
			break;

		case MPU_DMA_ACC_IN_PROGRESS:
			convert_acc(mpu);
			mpu->acc_data_callback();
			break;

		case MPU_DMA_ACC_AND_GYRO_IN_PROGRESS:
			convert_gyro(mpu);
			convert_acc(mpu);
			mpu->gyro_data_callback();
			mpu->acc_data_callback();
			break;
		case MPU_DMA_IDDLE:
		default:
			break;

	}
	mpu->dma_state = MPU_DMA_IDDLE ;
}
//static void MPU_dma_transmit_half_complete(mpu_t * mpu){
//
//	if(mpu->dma_state == MPU_DMA_ACC_AND_GYRO_IN_PROGRESS)
//	{
//		//A la moitié donc les données de l'acc sont arrivées
//		convert_acc(mpu);
//		mpu->acc_data_callback();
//	}
//}



void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	if(hi2c == I2C_handlers[0])
		MPU_dma_transmit_complete(mpu_handlers[0]);
}

























