#include "ch.h"
#include "hal.h"
#include "thruster.h"
#include <math.h>
#include  <stdlib.h>
static struct ThValues receiveData(void);

static void sendData(void);

CANRxFrame rxmsg;
CANTxFrame txmsg;

struct ThValues{
  int16_t X;
  int16_t Y;
  int16_t Z;
  int16_t ROT;
};

static struct ThValues receiveData() {
  
  struct ThValues thvalues;
  /* uint8_t i; */

  canReceive(&CAND1, 1, &rxmsg, TIME_INFINITE);
  
  palTogglePad(GPIOD, GPIOD_LED4);

  if (rxmsg.SID == 0x10){

  /* Set Thrust values to struct */

  thvalues.X = rxmsg.data16[0];
  thvalues.Y = rxmsg.data16[1];
  thvalues.ROT = rxmsg.data16[3];
  thvalues.Z = rxmsg.data16[2];
  
  return thvalues;
}
else{
  receiveData();
}
}

static void sendData(){
  
}


WORKING_AREA(wa_thruster, THRUSTER_THREAD_STACK_SIZE);

msg_t thruster_thread(void *p){
<<<<<<< HEAD
  (void)p;
  struct ThValues verdi;





 
double skalering = 1 ; //nyttes for å øke pådraget til motorer, basert på styrerspak innput
double theta, thetaInn, r ; 
int16_t absXinn ,absYinn; // absoluttverdi av innverdiene X og Y 
int16_t Xinn , Yinn, Zinn; // lokalt lagret verdi fra can, -1000-1000
double M1, M2, M3, M4, M5, M6, M7, M8; // paadraget til motorene I % -1000til1000
int16_t M1pwm, M2pwm, M3pwm, M4pwm, M5pwm, M6pwm, M7pwm, M8pwm; // verdi 500-1000
double M1Foer_kor, M2Foer_kor, M3Foer_kor, M4Foer_kor ;
int16_t brukIcos;


/* Horisontal */
pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 750));
//pwmEnableChannel(&PWMD1, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 750)); //DENNE E BOGUS
pwmEnableChannel(&PWMD1, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 750));
pwmEnableChannel(&PWMD1, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 750));
pwmEnableChannel(&PWMD2, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD2, 750));

/* Vertikal */
pwmEnableChannel(&PWMD8, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 750));
pwmEnableChannel(&PWMD8, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 750)); 
pwmEnableChannel(&PWMD8, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 750));
pwmEnableChannel(&PWMD8, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 750));

while(TRUE){
verdi = receiveData();

Xinn = verdi.X * 1000/1000;
Yinn = verdi.Y * 1000/1000;
Zinn = verdi.Z * 1000/1000;
absXinn = abs(Xinn);
absYinn = abs(Yinn);

    /* PROGRAM FOR HORISONTAL THRUSTERS */ 

    r = sqrt((absXinn*absXinn) + (absYinn*absYinn));
    // if (100/2 < r) {
    //  r = 100/2;
    //}

    //theta = 0;



    theta = atan2(Yinn,Xinn);

    //thetarund = theta;
    /*thetaInn = thetaInn *(180/M_PI); //rad => deg

    if (Xinn>0 && Yinn>0){ // I kvadrant 
      theta = thetaInn;
    }else if (Xinn<0 && Yinn>0){ // II kvadrant 
      theta = thetaInn + 180; 
    }else if (Xinn<0 && Yinn<0){ // III kvadrant 
      theta = thetaInn - 180;
    }else if (Xinn>0 && Yinn<0){ // IV kvadrant 
      theta = thetaInn;
    }
*/
//ok//
      brukIcos = (int16_t) ((M_PI/4 - theta));
      /* Horisontal bevegelse 2015 */
    M1Foer_kor =  r * cos(brukIcos) *  skalering;
    M2Foer_kor =  r * cos(M_PI/4 - theta) * skalering;
    M3Foer_kor = -M2Foer_kor ;
    M4Foer_kor = -M1Foer_kor ;


M1 = M1Foer_kor;
M2 = M2Foer_kor;
M3 = M3Foer_kor;
M4 = M4Foer_kor;

/* Horisontal */
M1pwm = (int16_t) ((M1*250/1414) + 750);  // M1*250/1000)1044
M2pwm = (int16_t) ((M2*250/1414) + 750);  // M1*250/1000)
M3pwm = (int16_t) ((M3*250/1414) + 750);  // M1*250/1000)
M4pwm = (int16_t) ((M4*250/1414) + 750);  // M1*250/1000)

/* Vertikal */
M5pwm = (int16_t) ((Zinn*250/1000) + 750);  // M1*250/1000)
M6pwm = (int16_t) ((Zinn*250/1000) + 750);  // M1*250/1000)
M7pwm = (int16_t) ((Zinn*250/1000) + 750);  // M1*250/1000)
M8pwm = (int16_t) ((Zinn*250/1000) + 750);  // M1*250/1000)

/* Horisontal */
pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, M1pwm));
//pwmEnableChannel(&PWMD1, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, M2pwm)); // DENNE E BOGUS
pwmEnableChannel(&PWMD1, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, M2pwm)); 
pwmEnableChannel(&PWMD1, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, M3pwm));
pwmEnableChannel(&PWMD2, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD2, M4pwm));

/* Vertikal */
pwmEnableChannel(&PWMD8, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, M5pwm));
pwmEnableChannel(&PWMD8, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, M6pwm)); 
pwmEnableChannel(&PWMD8, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, M7pwm));
pwmEnableChannel(&PWMD8, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, M8pwm));
=======
	(void)p;
	struct ThValues verdi;
	verdi.Z = 20;
	verdi.X = 20;
	verdi.Y = 20;
	verdi.ROT = 20;





	/* Run Forever */
	while(1){
		/* Motta Verdier gjennom canbus */
		
		verdi = receiveData();

		chThdSleepMilliseconds(1);
		/* Kode til truster program */
		/* Skriv inn kode her ;) */

		/* PROGRAM FOR VETRICAL THRUSTERS */
		/* MOTOR 5 */
		//pwmEnableChannel(&PWMD4, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, verdi.X*70));
		
		/* MOTOR 6 */
		//pwmEnableChannel(&PWMD4, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, verdi.Z*70));

		// MOTOR 7 
		//pwmEnableChannel(&PWMD4, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, verdi.ROT*70));
		
		/* MOTOR 8 */
		//pwmEnableChannel(&PWMD4, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, verdi.Y*70));
		

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
	 	pwmEnableChannel(&PWMD8, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 5000));
	 	pwmEnableChannel(&PWMD8, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 5000));
	 	pwmEnableChannel(&PWMD8, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 5000));
	 	pwmEnableChannel(&PWMD8, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 5000));
	 	pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 5000));
	 	pwmEnableChannel(&PWMD1, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 5000));
	 	pwmEnableChannel(&PWMD1, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 5000));
	 	pwmEnableChannel(&PWMD2, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD2, 5000));

	}	 
	
>>>>>>> mdevel
}


}
