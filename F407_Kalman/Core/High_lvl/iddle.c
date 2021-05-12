/*
 * iddle.c
 *
 *  Created on: 10 avr. 2021
 *      Author: Théo
 */

#include "../High_lvl/High_lvl.h"

void idle_entrance(system_t * sys){
	REGULATION_ORIENTATION_Set_Regulation_Mode(REGULATION_MODE_OFF);
}

void idle_main(system_t * sys, uint32_t time_us){
	//For now we just do nada
}

