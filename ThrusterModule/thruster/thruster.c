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
uint16_t M1, M2, M3, M4;
double r;
double theta;
double thetaInn;
double M1Foer_kor, M2Foer_kor, M3Foer_kor, M4Foer_kor;

double skalering =1 ;
int inversPropellKar[2] [10] =
 {1, 2,3,4,5,6,7,8,9,10}, // Fremover Thrust retning (pluss)
 {1, 2,3,4,5,6,7,8,9,10}  // bakover   Thrust retning (negativ)



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
			verdiB_Z = 750 - abs(verdiB_Z) * 2.5;
		}

		/* Gjør bergenet verdier om til PWM verdi for de resterende paadragene */
		else(verdiB_Z => 0){
			verdiB_Z = 750 + abs(verdiB_Z) * 2.5;
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
8
		r = (double) sqrt(verdi.X^2*verdi.Y^2)/2;
		if 100/2 < r{
			r = 100/2;
		}

		theta = 0;
		thetaInn = (double) atand(verdi.Y/verdi.X);

		if verdi.X>0 && verdi.Y>0{ /* I kvadrant */
			theta = thetaInn;
		}else verdi.X<0 && verdi.Y>0{ /* II kvadrant */
			theta = thetaInn + 180; 
		}else verdi.X<0 && verdi.Y<0{ /* III kvadrant */
			theta = thetaInn - 180;
		}else verdi.X>0 && verdi.Y<0{ /* IV kvadrant */
			theta = thetaInn;
		}
		
		/*______linearisering av propell karraktestikk_______*/





		
		/* Horisontal bevegelse 2015 */
		M1Foer_kor =  r*cos( 45 -theta) * skalering;
		M2Foer_kor =  r*cos(-45 -theta) * skalering;
		M3Foer_kor = -M2Foer_kor ;
		M4Foer_kor = -M1Foer_kor ;


		/* Konverterer fra double til uint */
		M1 = (uint16_t) M1Foer_kor;
		M2 = (uint16_t) M2Foer_kor;
		M3 = (uint16_t) M3Foer_kor;
		M4 = (uint16_t) M4Foer_kor;

		/* korrigerer for  ulinært forhol mellom thrust og pådrag denne vil ikke fungere ved negativt pådrag
		, dersom karraktestikk ikke er lik for frem/revers må det legges til to matriser, en for hver retning  */
		if (M1>= 0)
			{ M1 = M1 * inversPropellKar[1][M1] } ;
		else { M1 = M1 * inversPropellKar[2][M1] };

		
		if (M2>= 0)
			{ M2 = M2 * inversPropellKar[2][M1] } ;
		else { M2 = M2 * inversPropellKar[2][M2] };


		if (M3>= 0)
			{ M3 = M3 * inversPropellKar[1][M3] } ;
		else { M3 = M3 * inversPropellKar[2][M3] };

		
		if (M4>= 0)
			{ M4 = M4 * inversPropellKar[1][M4] } ;
		else { M4 = M4 * inversPropellKar[2][M4] };








		/* Psaudo, korrigering av pådrag med invers thruster kar.  
		M1= M1* lesDataFraGraf(M1);
		Lag for M2-M3;
		

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

/* Test */

