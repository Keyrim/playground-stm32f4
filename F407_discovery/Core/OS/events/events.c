/*
 * events.c
 *
 *  Created on: 13 sept. 2020
 *      Author: Th�o
 */

#include "mask_def.h"
#include "flags.h"
#include "events.h"

#define CLEAR_FLAG_FLIGHT_MODE flags = MASK_and(flags, mask_flight_mode_clear)

//Ensemble des flags
static Mask_t flags ;

//Si false, init du module event
static bool_e initialized = FALSE ;




#define DEFINE_EVENT(event_function_param, nb_mask_param, event_type_param){  	\
		.function = event_function_param ,						\
		.nb_mask = nb_mask_param , 								\
		.type = event_type_param 								\
}

//D�finitions des events
//Attention !!!! nb_mask <= EVENT_NB_MASK_PER_EVENT_MAX sinon d�rapage :)
static Event_t events[EVENT_COUNT] ={
		//Events array
};


//Déclenchement des events
void EVENT_process_events(){
	if(initialized){
	//Pout chaque event
		for(uint32_t e = 0; e < EVENT_COUNT; e ++){
			//Si on a un event de transition
			if(events[e].type == EVENT_TYPE_HIGH_LVL){
				uint32_t m = 0 ;
				bool_e function_did_run_once = FALSE ;
				//On test chaque paires de masque
				while(m < events[e].nb_mask && !function_did_run_once){
					if(Mask_test_and(events[e].mask_and[m], flags)){
						if(Mask_test_or(events[e].mask_or[m], flags)){
							events[e].function(m);
							function_did_run_once = TRUE ;
						}
					}
					m++ ;
				}
			}
		}
	}
}

bool_e EVENT_Set_flag(Flags_t flag){
	return MASK_set_flag(&flags, flag);
}
bool_e EVENT_Clean_flag(Flags_t flag){
	return MASK_clean_flag(&flags, flag);
}

void EVENT_init(void){
	initialized = TRUE ;

}



