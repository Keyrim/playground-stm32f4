/*
 * task.c
 *
 *  Created on: 13 aout 2020
 *      Author: Theo
 */

#include "task.h"
#include "../scheduler/scheduler.h"

static system_t * sys;

void tasks_init(system_t * sys_){
	//On mémorise où se trouve notre structure système
	sys = sys_ ;

	//Activation des tâches
	SCHEDULER_enable_task(TASK_LED, TRUE);
	SCHEDULER_enable_task(TASK_PRINTF, TRUE);
	SCHEDULER_enable_task(TASK_GYRO, TRUE);

}

void task_main_print_f(uint32_t current_time_us){
	printf("%lu\t%lu\t%lu\t%f\n",sys->sensors.gyro.mpu->delta_time_dma,TASK_get_task(TASK_GYRO)->it_real_period_us, SCHEDULER_get_cpu_load(), sys->sensors.gyro.raw[GYRO_AXE_X]);
}


void task_main_led(uint32_t current_time_us){
	LED_SEQUENCE_play(&sys->led, current_time_us);
}

void task_it_gyro(uint32_t current_time_us){
	GYRO_update(&sys->sensors.gyro);
}

#define DEFINE_TASK(id_param, priority_param,  task_function_param, desired_period_us_param) { 	\
	.id = id_param,										\
	.static_priority = priority_param,					\
	.it = task_function_param,					\
	.it_desired_period_us = desired_period_us_param		\
}

#define PERIOD_US_FROM_HERTZ(hertz_param) (1000000 / hertz_param)

task_t tasks [TASK_COUNT] ={
		[TASK_PRINTF] = 		DEFINE_TASK(TASK_PRINTF, 			PRIORITY_HIGH, 			task_main_print_f, 			PERIOD_US_FROM_HERTZ(30)),
		[TASK_LED] = 			DEFINE_TASK(TASK_LED, 				PRIORITY_LOW,	 		task_main_led, 				PERIOD_US_FROM_HERTZ(10)),
		[TASK_GYRO] = 			DEFINE_TASK(TASK_GYRO, 				PRIORITY_HIGH,	 		task_it_gyro, 				PERIOD_US_FROM_HERTZ(2000))
};

task_t * TASK_get_task(task_ids_t id){
	return &tasks[id];
}


