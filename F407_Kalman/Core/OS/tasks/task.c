/*
 * task.c
 *
 *  Created on: 13 aout 2020
 *      Author: Theo
 */

#include "task.h"
#include "../scheduler/scheduler.h"
#include "../Data_Logger/Data_logger.h"
#include "../High_lvl/High_lvl.h"
#include "../events/events.h"
#include "../OS/time.h"
#include "../../config.h"


static system_t * sys;


static uint32_t end_time_filtering_gyro= 0;


void tasks_init(system_t * sys_){
	//On mémorise où se trouve notre structure système
	sys = sys_ ;

	//Activation des tâches
	SCHEDULER_enable_task(TASK_SCHEDULER, TRUE);
	SCHEDULER_enable_task(TASK_PRINTF, TRUE);
	SCHEDULER_enable_task(TASK_EVENT_CHECK, TRUE);
	SCHEDULER_enable_task(TASK_TELEMETRIE, TRUE);
	SCHEDULER_enable_task(TASK_LOGGER, TRUE);
	SCHEDULER_enable_task(TASK_ORIENTATION_UPDATE, TRUE);
	SCHEDULER_enable_task(TASK_GYRO_UPDATE, FALSE);
	SCHEDULER_enable_task(TASK_GYRO_FILTER, TRUE);
	SCHEDULER_enable_task(TASK_ACC_FILTER, TRUE);
	SCHEDULER_enable_task(TASK_HIGH_LVL, TRUE);
	SCHEDULER_enable_task(TASK_MONITORING, TRUE);
	SCHEDULER_enable_task(TASK_STUFF_MADE_HERE, TRUE);

}



void process_printf(uint32_t current_time_us){
	//mpu_t * mpu = &sys->sensors.mpu ;

	//printf("%f\t%f\n", sys->orientation.angular_position[ORIENTATION_ROLL], sys->orientation.angular_velocity[ORIENTATION_ROLL]);

	//printf("%f\t%f\t%f\n", sys->sensors.acc.raw[0], sys->sensors.acc.raw[1], sys->sensors.acc.raw[2]);
	//printf("%f\t%f\t%f\n", sys->sensors.gyro.raw[0], sys->sensors.gyro.raw[1], sys->sensors.gyro.raw[2]);
}


void process_gyro_update(uint32_t current_time_us){
	GYRO_ACC_update_dma(&sys->sensors.gyro);

}

void process_gyro_filter(uint32_t current_time_us){
	end_time_filtering_gyro = current_time_us ;

	GYRO_process_lpf(&sys->sensors.gyro);

}

void process_acc_update(uint32_t current_time_us){
	//ACC_update(&sys->sensors.acc);
}

void process_acc_filter(uint32_t current_time_us){
	ACC_process_lpf(&sys->sensors.acc);
}

void process_event_main(uint32_t current_time_us){
	UNUSED(current_time_us);
	EVENT_process(FALSE);
}

void process_orientation_update(uint32_t current_time_us){
	ORIENTATION_Update(&sys->orientation);
	REGULATION_ORIENTATION_Process();
	MOTORS_Change_output(&sys->prop.motors);
}

void process_task_scheduler(uint32_t current_time_us){
	SCHEDULER_task();
}

void process_telemetry(uint32_t current_time_us){
	TELEMETRY_Process(current_time_us);
}
void process_logger(uint32_t current_time_us){
	DATA_LOGGER_Main();
}

void process_high_lvl(uint32_t current_time_us){
	HIGH_LVL_Main(current_time_us);
}



void process_stuff_made_here(uint32_t current_time_us){
	if(sys->simulate)
		STATE_SPACE_Step(&sys->state_space);
}

void process_self_test(uint32_t current_time_us){
	sys->soft.cpu_load = SCHEDULER_get_cpu_load();
}


#define TASKS_START_TIME_US 2000000
#define DEFINE_TASK(id_param, priority_param,  task_function_param, desired_period_us_param, mode_param) { 	\
	.id = id_param,										\
	.static_priority = priority_param,					\
	.process = task_function_param,						\
	.desired_period_us = desired_period_us_param,		\
	.mode = mode_param,									\
	.last_execution_us = TASKS_START_TIME_US,			\
	.desired_next_start_us = TASKS_START_TIME_US + desired_period_us_param	\
}

#define PERIOD_US_FROM_HERTZ(hertz_param) (1000000 / hertz_param)

task_t tasks [TASK_COUNT] ={
	[TASK_PRINTF] = 						DEFINE_TASK(TASK_PRINTF, 						PRIORITY_LOW, 			process_printf, 			PERIOD_US_FROM_HERTZ(50), 					TASK_MODE_TIME),
	[TASK_EVENT_CHECK] = 					DEFINE_TASK(TASK_EVENT_CHECK, 					PRIORITY_EVENT, 		process_event_main, 		PERIOD_US_FROM_HERTZ(1), 					TASK_MODE_ALWAYS),
	[TASK_SCHEDULER] = 						DEFINE_TASK(TASK_SCHEDULER, 					PRIORITY_SCHEDULER, 	process_task_scheduler, 	PERIOD_US_FROM_HERTZ(1), 					TASK_MODE_ALWAYS),

	[TASK_GYRO_UPDATE] = 					DEFINE_TASK(TASK_GYRO_UPDATE, 					PRIORITY_REAL_TIME,	 	process_gyro_update, 		PERIOD_US_FROM_HERTZ(GYRO_FREQUENCY), 		TASK_MODE_TIME),
	[TASK_GYRO_FILTER] = 					DEFINE_TASK(TASK_GYRO_FILTER, 					PRIORITY_REAL_TIME,	 	process_gyro_filter, 		PERIOD_US_FROM_HERTZ(1), 					TASK_MODE_WAIT),

	[TASK_ACC_UPDATE] = 					DEFINE_TASK(TASK_ACC_UPDATE, 					PRIORITY_LOW,	 		process_acc_update, 		PERIOD_US_FROM_HERTZ(500), 					TASK_MODE_TIME),
	[TASK_ACC_FILTER] = 					DEFINE_TASK(TASK_ACC_FILTER, 					PRIORITY_REAL_TIME,	 	process_acc_filter, 		PERIOD_US_FROM_HERTZ(1), 					TASK_MODE_WAIT),
	[TASK_ORIENTATION_UPDATE] = 			DEFINE_TASK(TASK_ORIENTATION_UPDATE, 			PRIORITY_REAL_TIME,		process_orientation_update, PERIOD_US_FROM_HERTZ(1), 					TASK_MODE_WAIT),

	[TASK_HIGH_LVL] = 						DEFINE_TASK(TASK_HIGH_LVL, 						PRIORITY_HIGH,			process_high_lvl,			PERIOD_US_FROM_HERTZ(HIGH_LVL_frequency), 	TASK_MODE_TIME),

	[TASK_LOGGER] = 						DEFINE_TASK(TASK_LOGGER, 						PRIORITY_LOW,	 		process_logger, 			PERIOD_US_FROM_HERTZ(5), 					TASK_MODE_TIME),
	[TASK_TELEMETRIE] = 					DEFINE_TASK(TASK_TELEMETRIE, 					PRIORITY_MEDIUM,	 	process_telemetry, 			PERIOD_US_FROM_HERTZ(1000), 				TASK_MODE_TIME),
	[TASK_MONITORING] = 					DEFINE_TASK(TASK_MONITORING, 					PRIORITY_LOW,	 		process_self_test, 			PERIOD_US_FROM_HERTZ(50), 					TASK_MODE_TIME),
	[TASK_STUFF_MADE_HERE] = 				DEFINE_TASK(TASK_STUFF_MADE_HERE, 				PRIORITY_LOW,	 		process_stuff_made_here, 	PERIOD_US_FROM_HERTZ(100), 					TASK_MODE_TIME),


};


task_t * TASK_get_task(task_ids_t id){
	return &tasks[id];
}


