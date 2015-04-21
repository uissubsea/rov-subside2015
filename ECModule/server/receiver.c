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
#include <stdio.h>

/*--------------------  C o n s t a n t s  ---------------------------------*/

/*--------------------  T y p e s  -----------------------------------------*/

struct ManipStruct
{
	int16_t motor[5];		// Levels for each motor
	int16_t power[5];		// Power for each motor
};

struct ThrusterStruct
{
	int16_t motor[8];		// Levels for each motor
	int16_t power[8];		// Power for each motor
};

/*--------------------  V a r i a b l e s  ---------------------------------*/

WORKING_AREA(wa_receiver, RECEIVER_THREAD_STACK_SIZE);

CANRxFrame thLevels;
CANRxFrame manipLevels;
CANRxFrame thPower;
CANRxFrame manipPower;

struct ManipStruct manipData;
struct ThrusterStruct thursterData;

uint8_t i;


/*--------------------  F u n c t i o n s  ---------------------------------*/


static void receiveFromCan(void){
	/* Receive messages from canbus, If the messages are not there
	   it times out immediately */

	//canReceive(&CAND1, 2, &manipPower, TIME_IMMEDIATE);
	canReceive(&CAND1, 3, &manipLevels, TIME_IMMEDIATE);

	for (i = 0; i < 4; i++){
		manipLevels.data16[i] = manipData.motor[i];
	}

}

msg_t receiver_thread(void *p){
	(void)p;
	err_t err;
	struct netconn *Rconn, *Rnewconn;

	char message[20];

	Rconn = netconn_new(NETCONN_TCP);

	netconn_bind(Rconn, NULL, 50001);

	netconn_listen(Rconn);

	do{
		err = netconn_accept(Rconn, &Rnewconn);
	} while(err != ERR_OK);
	
	/* Set low priority as things that happen here are not
	   very important */
	chThdSetPriority(LOWPRIO);

	while(TRUE){

		receiveFromCan();

		sprintf(message, "%d", manipData.motor[0]);
		
		err = netconn_write(Rnewconn, message, strlen(message), NETCONN_COPY);

		palTogglePad(GPIOD, GPIOD_LED6);
		chThdSleepMilliseconds(5000);
	}
}