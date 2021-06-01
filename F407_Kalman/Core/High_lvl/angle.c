/*
 * angle.c
 *
 *  Created on: 16 mai 2021
 *      Author: Théo
 */


#include "High_lvl.h"

void angle_entrance(system_t * sys){
	REGULATION_ORIENTATION_Set_Regulation_Mode(REGULATION_MODE_ANGLE);
}

void angle_main(system_t * sys, uint32_t time_us){
	//nada
}
