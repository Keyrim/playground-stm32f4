/*
 * system_d.h
 *
 *  Created on: Nov 21, 2020
 *      Author: Théo
 */

#ifndef OS_SYSTEM_D_H_
#define OS_SYSTEM_D_H_

#include "macro_types.h"
#include "stm32f1xx_hal.h"
#include "../../Drivers/Inc/Sequence_led.h"



typedef struct{
	sequence_led_t led ;
}system_t;


#endif /* OS_SYSTEM_D_H_ */
