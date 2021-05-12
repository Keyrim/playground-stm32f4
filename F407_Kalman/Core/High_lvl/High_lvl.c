/*
 * Flight_mode.c
 *
 *  Created on: Dec 13, 2020
 *      Author: Théo
 */



#include "../High_lvl/High_lvl.h"

static system_t * sys ;
static high_level_state_names_e current_state = HIGH_LVL_STATE_IDLE;
static high_level_state_names_e next_state = HIGH_LVL_STATE_IDLE;



//	-------------------------	Define flights modes	-------------------------
#define DEF_HIGH_LVL_STATE(name_param, function_entrance_param, function_main_param) { 	\
	.name = name_param,																	\
	.function_main = function_main_param,												\
	.function_entrance = function_entrance_param										\
}

static high_lvl_state_t high_lvl_states[HIGH_LVL_STATE_COUNT] = {
		[HIGH_LVL_STATE_IDLE] = 			DEF_HIGH_LVL_STATE(HIGH_LVL_STATE_IDLE, 				idle_entrance, 				idle_main),
		[HIGH_LVL_STATE_FULL_MANUAL] = 		DEF_HIGH_LVL_STATE(HIGH_LVL_STATE_FULL_MANUAL, 			full_manual_entrance, 		full_manual_main),

};

//	-------------------------	Public functions	-------------------------
void HIGH_LVL_Init(system_t * sys_){
	sys = sys_;
	sys->soft.high_lvl_state = &current_state;
}

//Dont call it from an it plz for now ..
void HIGH_LVL_Set_Mode(high_level_state_names_e new){
	next_state = new ;
}

void HIGH_LVL_Main(uint32_t time_us){
	//Check if we have to change the current flight mode
	if(next_state != current_state){

		//Call the leave function associated to the current(previous) flight mode if it exists
		if(high_lvl_states[current_state].function_leave != NULL)
			high_lvl_states[current_state].function_leave(sys);

		//Call the entrance function associated to the next flight mode if it exists
		if(high_lvl_states[next_state].function_entrance != NULL)
			high_lvl_states[next_state].function_entrance(sys);

		//Change the current flight mode
		current_state = next_state ;
	}
	//Call the function associated to the current flight mode
	if(high_lvl_states[current_state].function_main != NULL)
		high_lvl_states[current_state].function_main(sys, time_us);

}



//	-------------------------	Functions definitions	-------------------------






















