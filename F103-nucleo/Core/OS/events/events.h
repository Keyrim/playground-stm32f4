/*
 * events.h
 *
 *  Created on: 13 sept. 2020
 *      Author: Th�o
 */


#ifndef EVENTS_EVENTS_H_
#define EVENTS_EVENTS_H_


#include "mask_def_enum.h"
#include "mask.h"


#define EVENT_NB_MASK_PER_EVENT_MAX 7

typedef enum{
	//Event list
	EVENT_COUNT
}Id_Events_t;

typedef enum{
	EVENT_TYPE_HIGH_LVL,
	EVENT_TYPE_ERROR
}Event_type_t;

typedef struct{
	Mask_t mask_and[EVENT_NB_MASK_PER_EVENT_MAX] ;	//Masques de conditions
	Mask_t mask_or[EVENT_NB_MASK_PER_EVENT_MAX] ;	//Masques de déclenchement
	uint32_t nb_mask ;								//Nombre de paires de masque
	Event_type_t type ;								//Type d'évenement
	void (*function)(mask_def_ids_t mask_id);
}Event_t;


void EVENT_process_events();
void EVENT_init(void);

//Set and clean flags
bool_e EVENT_Set_flag(Flags_t flag);
bool_e EVENT_Clean_flag(Flags_t flag);



#endif /* EVENTS_EVENTS_H_ */
