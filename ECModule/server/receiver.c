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

#define ADC_NUM_CHANNELS 2
#define ADC_BUF_DEPTH 8

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

static const ADCConversionGroup adcgrpcfg = {
  TRUE,
  ADC_NUM_CHANNELS,
  NULL,
  NULL,
  0,                        /* CR1 */
  ADC_CR2_SWSTART,          /* CR2 */
  ADC_SMPR1_SMP_SENSOR(ADC_SAMPLE_144) | ADC_SMPR2_SMP_AN5(ADC_SAMPLE_56),
  0,                        /* SMPR2 */
  ADC_SQR1_NUM_CH(ADC_NUM_CHANNELS),
  0,
  ADC_SQR3_SQ1_N(ADC_CHANNEL_SENSOR) | ADC_SQR3_SQ2_N(ADC_CHANNEL_IN5)
};


/*--------------------  V a r i a b l e s  ---------------------------------*/

WORKING_AREA(wa_receiver, RECEIVER_THREAD_STACK_SIZE);
static adcsample_t samples[ADC_NUM_CHANNELS * ADC_BUF_DEPTH];

CANRxFrame thLevels;
CANRxFrame manipLevels;
CANRxFrame thPower;
CANRxFrame manipPower;

struct ManipStruct manipData;
struct ThrusterStruct thursterData;

uint8_t i;
char tempstr[10];		// String to hold Temperature data


/*--------------------  F u n c t i o n s  ---------------------------------*/

static void receiveAdcValues(void){
  int Vsense;
  int TCelsius;

  Vsense = (float)(samples[0] * 3300 / 0xfff) / 1000;
  TCelsius = ((Vsense - 0.76) / 2.5) + 25.0;

  sprintf(tempstr, "Temp: %d", TCelsius);
}

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

	/* Start ADC */
	adcStartConversion(&ADCD1, &adcgrpcfg, samples, ADC_BUF_DEPTH);

	while(TRUE){

		receiveFromCan();

		receiveAdcValues();

		//sprintf(message, "%d", manipData.motor[0]);
		
		err = netconn_write(Rnewconn, tempstr, strlen(tempstr), NETCONN_COPY);

		palTogglePad(GPIOD, GPIOD_LED6);
		chThdSleepMilliseconds(5000);
	}
}