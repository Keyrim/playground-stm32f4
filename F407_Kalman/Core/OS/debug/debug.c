/*
 * debug.c
 *
 *  Created on: Nov 21, 2020
 *      Author: Théo
 */



#include "debug.h"
#include "retarget.h"

extern UART_HandleTypeDef huart1 ;

void DEBUG_init(void){
	RetargetInit(&huart1);
	printf("Debuger is init\n");
}

