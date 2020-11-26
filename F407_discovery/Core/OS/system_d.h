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
#include "../Sensors/mpu.h"
#include "../../Drivers/Inc/Sequence_led.h"

typedef struct sensors_t{
	mpu_t mpu;
}sensors_t;

typedef struct{
	sequence_led_t led ;
	sensors_t sensors ;
}system_t;


#endif /* OS_SYSTEM_D_H_ */
