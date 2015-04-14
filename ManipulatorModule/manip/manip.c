#include "ch.h"
#include "hal.h"
#include "manip.h"

WORKING_AREA(wa_manip_thread, MANIP_THREAD_STACK_SIZE);

uint16_t i = 1;         // Lager teller variabel i    
uint16_t j = 1;         // Lager teller variabel j 

static struct Mvalues receiveData(void);

struct Mvalues{
	int16_t M1;
	int16_t M2;
	int16_t M3;
	int16_t Rotasjon;
	int16_t Klo;
};

static struct Mvalues receiveData() {
  CANRxFrame rxmsg;
  struct Mvalues mvalues;
  /* uint8_t i; */

  canReceive(&CAND1, CAN_ANY_MAILBOX, &rxmsg, MS2ST(100));
  
  palTogglePad(GPIOD, GPIOD_LED4);

  /* Set Thrust values to struct */

  mvalues.M1 = rxmsg.data8[0];
  mvalues.M2 = rxmsg.data8[1];
  mvalues.M3 = rxmsg.data8[2];
  mvalues.Rotasjon = rxmsg.data8[3];
  mvalues.Klo = rxmsg.data8[4];

  return mvalues;
}


void pwmteller(PWMDriver *pwmp){
 (void)pwmp;
 i++;
}



msg_t manip_thread(void *p){
(void)p;

struct Mvalues verdier;


verdier = receiveData();


  while(TRUE) {     //Sett inn styrespak
    
    verdier = receiveData();

   // pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 1000)); //PA8
  //  if(verdier.M1 >0)
 //   {
        palClearPad(GPIOC,GPIOC_PIN11);
      //  palTogglePad(GPIOD, GPIOD_LED6);
        pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 5000)); //PA8

        pwmEnableChannel(&PWMD2, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD2, 5000)); //PA15

        pwmEnableChannel(&PWMD3, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, 5000)); //PC6

        pwmEnableChannel(&PWMD4, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 5000)); //PB6

        pwmEnableChannel(&PWMD8, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 5000)); //PC7

      
       
      // pwmChangePeriod(&PWMD4, 10);
      // chThdSleepMilliseconds(3000);
      // pwmDisableChannel(&PWMD4, 1);
      // pwmEnableChannel(&PWMD3, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, 1000));
  //  }
    if(verdier.M1 <0)
    {
      palSetPad(GPIOC, GPIOC_PIN11);     // Snur retning på motorer
      palTogglePad(GPIOD, GPIOD_LED6);
    //  palSetPad(GPIOA, GPIOA_PIN14);
    //  palSetPad(GPIOC, GPIOC_PIN10);
    //  palSetPad(GPIOC, GPIOC_PIN11);
    //  palSetPad(GPIOC, GPIOC_PIN12);

        pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 1000)); //PA8

        pwmEnableChannel(&PWMD2, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD2, 1000)); //PA15

        pwmEnableChannel(&PWMD3, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, 1000)); //PC6

        pwmEnableChannel(&PWMD4, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 1000)); //PB6

        pwmEnableChannel(&PWMD8, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 1000)); //PC7
      }
      else 
      {
              pwmDisableChannel(&PWMD1,0);

      }
      
      // pwmChangePeriod(&PWMD4, 100);
      // pwmChangePeriod(&PWMD4, 20);
      // chThdSleepMilliseconds(3000);
      // pwmDisableChannel(&PWMD4, 1);
      // palClearPad(GPIOD, GPIOD_PIN3);
      // pwmEnableChannel(&PWMD3, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, 1000));
    }

}

