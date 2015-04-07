#include "ch.h"
#include "hal.h"
#include "thruster.h"

static struct ThValues receiveData(void);

struct ThValues{
	uint16_t X;
	uint16_t Y;
	uint16_t Z;
	uint16_t ROT;
};

static struct ThValues receiveData() {
  CANRxFrame rxmsg;
  struct ThValues thvalues;
  /* uint8_t i; */

  canReceive(&CAND1, CAN_ANY_MAILBOX, &rxmsg, TIME_IMMEDIATE);
  palTogglePad(GPIOD, GPIOD_LED4);

  /* Set Thrust values to struct */

  thvalues.X = rxmsg.data8[0];
  thvalues.Y = rxmsg.data8[1];
  thvalues.ROT = rxmsg.data8[2];
  thvalues.Z = rxmsg.data8[3];
  
  return thvalues;
}


WORKING_AREA(wa_thruster, THRUSTER_THREAD_STACK_SIZE);

msg_t thruster_thread(void *p){
	(void)p;
	struct ThValues verdi;

/* Definerer korrigerings variabler */
uint16_t verdiB_X;
uint16_t verdiB_Y;
uint16_t verdiB_Z;
uint16_t korrigert;


	/* Run Forever */
	while(1){
		/* Motta Verdier gjennom canbus */
		verdi = receiveData();

		/* Kode til truster program */
		/* Skriv inn kode her ;) */

		korrigert = 70;
		/* Innverdi for X */
		verdiB_X = verdi.X - 128;
		verdiB_X = verdiB_X * korrigert/128;

		/* Innverdi for Y */
		verdiB_Y = verdi.Y - 128;
		verdiB_Y = verdiB_Y * korrigert/128;

		/* Innverdi for Z */
		verdiB_Z = verdi.Z - 128;
		verdiB_Z = verdiB_Z * korrigert/128;
		
		/* SKRIV COMMENT */
		if(verdiB_Z < 0){
			verdiB_Z = 5000 - abs(verdiB_Z) * 5000/100;
		}

		/* SKRIV COMMENT */
		else(verdiB_Z => 0){
			verdiB_Z = 5000 + abs(verdiB_Z) * 5000/100;
		}

		/* PROGRAM FOR VETRICAL THRUSTERS */
		/* MOTOR 5 */
		pwmEnableChannel(&PWMD8, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, verdiB_Z))
		
		/* MOTOR 6 */
		pwmEnableChannel(&PWMD8, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, verdiB_Z))

		/* MOTOR 7 */
		pwmEnableChannel(&PWMD8, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, verdiB_Z))
		
		/* MOTOR 8 */
		pwmEnableChannel(&PWMD8, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, verdiB_Z))
		

		/* PROGRAM FOR HORISONTAL THRUSTERS */ 

		/* Følgende funksjon kan benyttes te å sette pådrag til trustere
	 	* 
	 	* pwmEnableChannel(&PWMDx, kanal, PWM_PERCENTAGE_TO_WIDTH(&PWMDx, duty cycle))
	 	* 
	 	* kanal går mellom 0-3 hvor 3 er TIMx_CH4
	 	* 
	 	* dutyCycle kan ha verdi mellom 0 - 10000 hvor 10000 er 100%
	 	* Eksempel:
	 	* 
	 	* pwmEnableChannel(&PWMD1, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 2000)); */

	 	/* Eksempel: hente ut verdi
	 	verdi.X */
	}	 
	
}

