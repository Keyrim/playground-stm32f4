/*
 * system_d.h
 *
 *  Created on: Nov 21, 2020
 *      Author: Théo
 */

#ifndef OS_SYSTEM_D_H_
#define OS_SYSTEM_D_H_

#include "macro_types.h"
#include "stm32f4xx_hal.h"
#include "../Sensors/gyro.h"
#include "../Sensors/acc.h"
#include "../../Drivers/Inc/Motors.h"
#include "../Estimators/orientation.h"
#include "../Regulation/Regu_orientation.h"
#include "../config.h"
#include "../Telemetry/Telemetry.h"
#include "../State_Space_Model/State_Space_Model.h"
#include "../State_Space_Model/Kalman.h"

typedef struct sensors_t{
	//Sensors
	gyro_t gyro ;
	acc_t acc ;

	//Sensors list
	mpu_t mpu ;
}sensors_t;


typedef struct soft_t{
	uint8_t * high_lvl_state ;
	float cpu_load ;
}soft_t;

typedef struct prop_t{
	motors_t motors;
	int16_t thrust_consigne[4];
}prop_t;

typedef struct system_t{
	State_Space_Model_t ss;
	kalman_t kalman;
	sensors_t sensors ;
	orientation_t orientation ;
	regu_orientation_t regulation ;
	soft_t soft;
	prop_t prop;
	telemetry_t telemetry ;
	bool_e simulate ;
}system_t;


#endif /* OS_SYSTEM_D_H_ */
