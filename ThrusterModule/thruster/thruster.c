#include "ch.h"
#include "hal.h"
#include "thruster.h"
#include <math.h>
static struct ThValues receiveData(void);

struct ThValues{
	int16_t X;
	int16_t Y;
	int16_t Z;
	int16_t ROT;
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
int16_t verdiB_X;
int16_t verdiB_Y;
int16_t verdiB_Z;
int16_t korrigert =100;
int16_t M1, M2, M3, M4;
int16_t M1velger, M2velger, M3velger, M4velger;
double r;
double theta;
double thetaInn;
double M1Foer_kor, M2Foer_kor, M3Foer_kor, M4Foer_kor;

double skalering =1 ;
int inversPropellKar[2] [10] ={
 {1,2,3,4,5,6,7,8,9,10}, // Fremover Thrust retning (pluss)
 {1,3,3,4,5,6,7,8,9,10}};  // bakover   Thrust retning (negativ)



	/* Run Forever */
	while(1){
		/* Motta Verdier gjennom canbus */
		verdi = receiveData();

		/* Kode til truster program */
		/* Skriv inn kode her ;) */

		/* Definerer variabler */
		korrigert = 100;
		
		/* Innverdi for X */
		verdiB_X = verdi.X * korrigert/1000;

		/* Innverdi for Y */
		verdiB_Y = verdi.Y * korrigert/1000;

		/* Innverdi for Z */
		verdiB_Z = verdi.Z * korrigert/1000;
		
		/* Gjør bergenet verdier om til PWM verdi for negativt paadrag */
		if(verdiB_Z < 0){
			verdiB_Z = 750 + (verdiB_Z * 2.5);
		}

		/* Gjør bergenet verdier om til PWM verdi for de resterende paadragene */
		else/* if (verdiB_Z >= 0)*/{
			verdiB_Z = 750 + (verdiB_Z * 2.5);
		}


		/* PROGRAM FOR VETRICAL THRUSTERS */
		/* MOTOR 5 */
		pwmEnableChannel(&PWMD8, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, verdiB_Z));
		
		/* MOTOR 6 */
		pwmEnableChannel(&PWMD8, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, verdiB_Z));

		/* MOTOR 7 */
		pwmEnableChannel(&PWMD8, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, verdiB_Z));
		
		/* MOTOR 8 */
		pwmEnableChannel(&PWMD8, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, verdiB_Z));
		

		/* PROGRAM FOR HORISONTAL THRUSTERS */ 

		r = (double) sqrt(verdi.X^2*verdi.Y^2)/2;
		if (100/2 < r) {
			r = 100/2;
		}

		theta = 0;
		thetaInn = (double) atan(verdi.Y/verdi.X);
		thetaInn = thetaInn *(180/M_PI); //rad => deg

		if (verdi.X>0 && verdi.Y>0){ /* I kvadrant */
			theta = thetaInn;
		}else if (verdi.X<0 && verdi.Y>0){ /* II kvadrant */
			theta = thetaInn + 180; 
		}else if (verdi.X<0 && verdi.Y<0){ /* III kvadrant */
			theta = thetaInn - 180;
		}else if (verdi.X>0 && verdi.Y<0){ /* IV kvadrant */
			theta = thetaInn;
		}
		


		
		/* Horisontal bevegelse 2015 */
		M1Foer_kor =  r*cos( 45 -theta) * skalering;
		M2Foer_kor =  r*cos(-45 -theta) * skalering;
		M3Foer_kor = -M2Foer_kor ;
		M4Foer_kor = -M1Foer_kor ;


		/* Konverterer fra double til int for å få en verdi som kan  */
		M1velger = (int16_t) M1Foer_kor;
		M2velger = (int16_t) M2Foer_kor;
		M3velger = (int16_t) M3Foer_kor;
		M4velger = (int16_t) M4Foer_kor;

			/*______linearisering av propell karraktestikk_______*/
		
		/* korrigerer for  ulinært forhol mellom thrust og pådrag denne vil ikke fungere ved negativt pådrag
		, dersom karraktestikk ikke er lik for frem/revers må det legges til to matriser, en for hver retning  */
		if (M1Foer_kor>= 0)
			{ M1Foer_kor = M1Foer_kor * inversPropellKar[1][M1velger]  ;}
		else { M1Foer_kor = M1Foer_kor * inversPropellKar[2][M1velger] ;}

		
		if (M2Foer_kor>= 0)
			{ M2Foer_kor = M2Foer_kor * inversPropellKar[2][M1velger]  ;}
		else { M2Foer_kor = M2Foer_kor * inversPropellKar[2][M2velger] ;}


		if (M3Foer_kor>= 0)
			{ M3Foer_kor = M3Foer_kor * inversPropellKar[1][M3velger]  ;}
		else { M3Foer_kor = M3Foer_kor * inversPropellKar[2][M3velger] ;}

		
		if (M4Foer_kor>= 0)
			{ M4Foer_kor = M4Foer_kor * inversPropellKar[1][M4velger]  ;} 
		else { M4Foer_kor = M4Foer_kor * inversPropellKar[2][M4velger] ;}




		M1 = ((int16_t) M1Foer_kor)*100;
		M2 = ((int16_t) M2Foer_kor)*100;
		M3 = ((int16_t) M3Foer_kor)*100;
		M4 = ((int16_t) M4Foer_kor)*100;
		 /*Følgende funksjon kan benyttes for å sette pådrag til trustere
	 	* 
	 	* pwmEnableChannel(&PWMDx, kanal, PWM_PERCENTAGE_TO_WIDTH(&PWMDx, duty cycle))
	 	* 
	 	* kanal går mellom 0-3 hvor 3 er TIMx_CH4
	 	* 
	 	* dutyCycle kan ha verdi mellom 0 - 10000 hvor 10000 er 100%
	 	* Eksempel:
	 	* 
	 	* pwmEnableChannel(&PWMD1, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 2000)); 

	 	 Eksempel: hente ut verdi
	 	verdi.X */
	}	 
	
}

/* Test */

