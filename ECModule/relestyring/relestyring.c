#include "ch.h"
#include "hal.h"

#include "relestyring.h"

WORKING_AREA(waRelestyring, RELE_THREAD_STACK_SIZE);

msg_t relestyring(void*arg){
	(void)arg;
	
	while(1){
		palTogglePad(GPIOD, GPIOD_LED3);
		chThdSleepMilliseconds(500);
	}
	
}