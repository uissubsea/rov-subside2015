#include "ch.h"
#include "hal.h"
#include "thruster.h"
#include <math.h>
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
  CANRxFrame rxmsg;
  struct ThValues thvalues;
  /* uint8_t i; */

  canReceive(&CAND1, CAN_ANY_MAILBOX, &rxmsg, TIME_INFINITE);
  
  palTogglePad(GPIOD, GPIOD_LED4);

  /* Set Thrust values to struct */

  thvalues.X = rxmsg.data16[0];
  thvalues.Y = rxmsg.data16[1];
  thvalues.ROT = rxmsg.data16[2];
  thvalues.Z = rxmsg.data16[3];
  
  return thvalues;
}

static void sendData(){
  s
}


WORKING_AREA(wa_thruster, THRUSTER_THREAD_STACK_SIZE);

msg_t thruster_thread(void *p){
	(void)p;
	struct ThValues verdi;





 
double skalering = 1 ; //nyttes for å øke pådraget til motorer, basert på styrerspak innput
double theta, thetaInn, r ; 
int16_t absXinn ,absYinn; // absoluttverdi av innverdiene X og Y 
int16_t Xinn , Yinn; // lokalt lagret verdi fra can, -1000-1000
double M1, M2, M3, M4; // paadraget til motorene I % -1000til1000
int16_t M1pwm, M2pwm, M3pwm, M4pwm; // verdi 500-1000
double M1Foer_kor, M2Foer_kor, M3Foer_kor, M4Foer_kor ;


while(TRUE){
verdi = receiveData();


Xinn = verdi.X * 1000/1000;
Yinn = verdi.Y * 1000/1000;
absXinn = abs(Xinn);
absYinn = abs(Yinn);

    /* PROGRAM FOR HORISONTAL THRUSTERS */ 

    r = sqrt((absXinn^2)*(absYinn^2));
    // if (100/2 < r) {
    //  r = 100/2;
    //}

    //theta = 0;
    thetaInn = atan(Yinn/Xinn);
    thetaInn = thetaInn *(180/M_PI); //rad => deg

    if (Xinn>0 && Yinn>0){ // I kvadrant 
      theta = thetaInn;
    }else if (Xinn<0 && Yinn>0){ // II kvadrant 
      theta = thetaInn + 180; 
    }else if (Xinn<0 && Yinn<0){ // III kvadrant 
      theta = thetaInn - 180;
    }else if (Xinn>0 && Yinn<0){ // IV kvadrant 
      theta = thetaInn;
    }

//ok//

      /* Horisontal bevegelse 2015 */
    M1Foer_kor =  r*cos( 45 -theta) * skalering;
   // M2Foer_kor =  r*cos(-45 -theta) * skalering;
   // M3Foer_kor = -M2Foer_kor ;
   // M4Foer_kor = -M1Foer_kor ;


M1 = M1Foer_kor;


M1pwm = (int16_t) ((M1*250/1414) + 750);  // M1*250/1000)


pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, M1pwm));
}

}
