/*****************************************************************************
DESCRIPTION Skeleton of the read-driver
*****************************************************************************/


/*--------------------  I n c l u d e   F i l e s  -------------------------*/

#include "ch.h"
#include "hal.h"
#include "receiver.h"
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include <string.h>

/*--------------------  C o n s t a n t s  ---------------------------------*/

/*--------------------  V a r i a b l e s  ---------------------------------*/

WORKING_AREA(wa_receiver, RECEIVER_THREAD_STACK_SIZE);

/*--------------------  T y p e s  -----------------------------------------*/

/*--------------------  F u n c t i o n s  ---------------------------------*/

msg_t receiver_thread(void *p){
	(void)p;
	err_t err;
	struct netconn *Rconn, *Rnewconn;

	char message[] = "Hello, from receiver";

	Rconn = netconn_new(NETCONN_TCP);

	netconn_bind(Rconn, NULL, 50001);

	netconn_listen(Rconn);

	do{
		err = netconn_accept(Rconn, &Rnewconn);
	} while(err != ERR_OK);
	

	while(TRUE){
		
		err = netconn_write(Rnewconn, message, strlen(message), NETCONN_COPY);

		palTogglePad(GPIOD, GPIOD_LED6);
		chThdSleepMilliseconds(1000);
	}
}