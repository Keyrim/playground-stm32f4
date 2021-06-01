/*
 * Flight_mode.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Théo
 */

#ifndef HIGH_LVL_HIGH_LVL_H_
#define HIGH_LVL_HIGH_LVL_H_

#include "../OS/system_d.h"


typedef enum high_level_state_names_e{
	HIGH_LVL_STATE_IDLE = 0x00,
	HIGH_LVL_STATE_FULL_MANUAL,
	HIGL_LVL_STATE_ANGLE,
	HIGH_LVL_STATE_COUNT
}high_level_state_names_e;

typedef struct high_lvl_state_t{
	high_level_state_names_e name;
	void (*function_entrance)(system_t * sys);
	void (*function_main)(system_t * sys, uint32_t);
	void (*function_leave)(system_t * sys);
}high_lvl_state_t;


//	-------------------------	Prototypes	-------------------------
void HIGH_LVL_Init(system_t * sys_);
void HIGH_LVL_Set_Mode(high_level_state_names_e new);
void HIGH_LVL_Main(uint32_t time_us);

//	-------------------------	Prototypes	-------------------------

void idle_entrance(system_t * sys);
void idle_main(system_t * sys, uint32_t time_us);

void full_manual_entrance(system_t * sys);
void full_manual_main(system_t * sys, uint32_t time_us);

void angle_entrance(system_t * sys);
void angle_main(system_t * sys, uint32_t time_us);


#endif /* HIGH_LVL_HIGH_LVL_H_ */
