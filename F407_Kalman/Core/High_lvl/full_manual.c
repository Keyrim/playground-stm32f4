/*
 * full_manual.c
 *
 *  Created on: 10 avr. 2021
 *      Author: Théo
 */


#include "High_lvl.h"


void full_manual_entrance(system_t * sys){
	REGULATION_ORIENTATION_Set_Regulation_Mode(REGULATION_MODE_MANUAL);
}

void full_manual_main(system_t * sys, uint32_t time_us){
	//nada
}
