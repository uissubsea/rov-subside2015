#include "ch.h"
#include "hal.h"
#include "manip.h"
#include <math.h>


WORKING_AREA(wa_manip_thread, MANIP_THREAD_STACK_SIZE); // lager plass på stakken til tråden

int16_t Mverdier[4];
 CANRxFrame rxmsg;
struct Mvalues mvalues;


struct Mvalues{ 
	int8_t M1;
	int8_t M2;
	int8_t M3;
	int8_t M4;
	int8_t M5;
  int8_t M6;
};

static struct Mvalues receiveData() {   // mottar data fra spillkontroller

  /* uint8_t i; */

  canReceive(&CAND1, CAN_ANY_MAILBOX, &rxmsg, TIME_INFINITE); // Mottaker CANbus
  
  /* Set Thrust values to struct */
  if (rxmsg.SID == 1){      
  mvalues.M1 = rxmsg.data8[0];
  mvalues.M2 = rxmsg.data8[1];
  mvalues.M3 = rxmsg.data8[2];
  mvalues.M4 = rxmsg.data8[3];
  mvalues.M5 = rxmsg.data8[4];
  mvalues.M6 = rxmsg.data8[5];

  palTogglePad(GPIOD, GPIOD_LED4); // Sjekk for å se at en mottar data fra Styrespak

  }

  return mvalues;
}



uint16_t k=0;     // Tellevariabler
uint16_t l=0;
uint16_t r=0;
uint16_t n=0;
uint16_t o=0;
uint16_t p=0;
uint16_t m=0;
uint16_t u=0;
uint16_t b=0;
uint16_t v=0;



void pwmtellerM1(PWMDriver *pwmp){        // Tellefunksjon 1
 (void)pwmp;
 if (Mverdier[0]>0)
 {
   k++;
   l--;
 }
 else if (Mverdier[0]<0)
 {
   k--;
   l++;
 }
}
void pwmtellerM2(PWMDriver *pwmp){        // Tellefunksjon 2
 (void)pwmp;
 if (Mverdier[1]>0)
 {
   r++;
   n--;
 }
 else if (Mverdier[1]<0)
 {
   r--;
   n++;
 }
}
void pwmtellerM3(PWMDriver *pwmp){        // Tellefunksjon 3
 (void)pwmp;
 if (Mverdier[2]>0)
 {
   o++;
   p--;
 }
 else if (Mverdier[2]<0)
 {
   o--;
   p++;
 }
}
void pwmtellerM4(PWMDriver *pwmp){        // Tellefunksjon 4
 (void)pwmp;
 if (Mverdier[3]>0)
 {
   m++;
   u--;
 }
 else if (Mverdier[3]<0)
 {
   m--;
   u++;
 }
}
void pwmtellerM5(PWMDriver *pwmp){        // Tellefunksjon 5
 (void)pwmp;
 if (Mverdier[4]>0)
 {
   b++;
   v--;
 }
 else if (Mverdier[4]<0)
 {
   b--;
   v++;
 }
}



msg_t manip_thread(void *p){  // starter tråd
(void)p;

struct Mvalues verdier;



uint8_t i; // tellevariabel til for-løkke



  while(TRUE) {
     
    verdier = receiveData(); // kaller metode for å motta data fra spillkontroll

    Mverdier[0] = verdier.M1;   // signal høyre spak opp/ned - bom
    Mverdier[1] = verdier.M2;   // signal høyre spak høyre/venstre - klo opp/ned
    Mverdier[2] = verdier.M3;   // signal venstre spak høyre/venstre - rotasjon klo
    Mverdier[3] = verdier.M4;   // signal venstre spak opp/ned - stikke
    Mverdier[4] = verdier.M5;   // signal venstre knapp bak - åpne klo
    Mverdier[5] = verdier.M6;   // signal høyre knapp bak - lukke klo

    for (i = 0; i < 6; i++)
     {
       if(Mverdier[i] > 0)
       {
        switch(i){
          case 0:         // bom opp
              palSetPad(GPIOA, GPIOA_PIN9);       // setter PA9
              pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 5000)); // PFM på PA8
              pwmChangePeriod(&PWMD1, Mverdier[0]^-2*100000);      // endrer perioden på timer 1 med styrespaken  
            break;

          case 1:       // tilt klo opp
            palSetPad(GPIOA, GPIOA_PIN10);  // setter PA10
            palSetPad(GPIOC, GPIOC_PIN11);  // setter PC11
            pwmEnableChannel(&PWMD2, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD2, 5000)); // PFM på PA15
            pwmEnableChannel(&PWMD3, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, 5000)); // PFM på PC6
            pwmChangePeriod(&PWMD2, pow(Mverdier[1],-2)*100000);   // endrer perioden på timer 2 med styrespaken
            pwmChangePeriod(&PWMD3, pow(Mverdier[1],-2)*100000);   // endrer perioden på timer 3 med styrespaken
            break;

          case 2:       // rotasjon klo høyre
            palClearPad(GPIOC, GPIOC_PIN11);    // resetter PC11
            palSetPad(GPIOA, GPIOA_PIN10);      // setter PC10
            pwmEnableChannel(&PWMD3, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, 5000)); // PFM på PC6
            pwmEnableChannel(&PWMD2, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD2, 5000)); //PFM på PA15
            pwmChangePeriod(&PWMD2, pow(Mverdier[2],-2)*100000);   // endrer perioden på timer 2 med styrespaken
            pwmChangePeriod(&PWMD3, pow(Mverdier[2],-2)*100000);   // endrer perioden på timer 3 med styrespaken
            break;

           case 3:    // stikke opp
            palSetPad(GPIOC, GPIOC_PIN10);    // setter PC10
            pwmEnableChannel(&PWMD4, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 5000)); //PFM på PB6
            pwmChangePeriod(&PWMD4, pow(Mverdier[3],-2)*100000);   // endrer perioden på timer 4 med styrespaken
            break;

          case 4:     // åpne klo
            palSetPad(GPIOC, GPIOC_PIN12);    // setter PC12      
            pwmEnableChannel(&PWMD8, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 5000)); // PFM på PC7
            pwmChangePeriod(&PWMD8, pow(Mverdier[4],-2)*100000);   // endrer perioden på timer 8 med styrespaken
            break;

          case 5:     // lukke klo
            palClearPad(GPIOC, GPIOC_PIN12);   // resetter PC12
            pwmEnableChannel(&PWMD8, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 5000)); // PFM på PC7
            pwmChangePeriod(&PWMD8, pow(Mverdier[5],-2)*100000);   // endrer perioden på timer 8 med styrespaken
            break;

        }
       }
       else if(Mverdier[i]<0)
       {
        switch(i){
          case 0:   // bom ned
            palClearPad(GPIOA, GPIOA_PIN9); // PA9
            pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 5000)); //PA9
            pwmChangePeriod(&PWMD1, pow(Mverdier[0],-2)*100000);
            break;

          case 1:   // tilt klo ned
            palClearPad(GPIOC, GPIOC_PIN11);  // PC11
            palClearPad(GPIOA, GPIOA_PIN10);  // Pa10
            pwmEnableChannel(&PWMD2, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD2, 5000)); //PA15
            pwmEnableChannel(&PWMD3, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, 5000)); //PC6
            pwmChangePeriod(&PWMD2, pow(Mverdier[1],-2)*100000);
            pwmChangePeriod(&PWMD3, pow(Mverdier[1],-2)*100000);
            break;

          case 2:   // rotasjon klo venstre
            palSetPad(GPIOC, GPIOC_PIN11);    // PC11
            palClearPad(GPIOA, GPIOA_PIN10);  // PC10
            pwmEnableChannel(&PWMD3, 0, PWM_PPERCENTAGE_TO_WIDTH(&PWMD3, 5000)); //PC6
            pwmEnableChannel(&PWMD2, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD2, 5000)); //PA15
            pwmChangePeriod(&PWMD2, pow(Mverdier[2],-2)*100000);
            pwmChangePeriod(&PWMD3, pow(Mverdier[2],-2)*100000);
            break;
           
          case 3:   // stikke ned
            palClearPad(GPIOC, GPIOC_PIN10);  // PC10
            pwmEnableChannel(&PWMD4, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 5000)); //PB6
            pwmChangePeriod(&PWMD4, pow(Mverdier[3],-2)*100000);
            break;

        }
       }
       else
       {
        switch(i){

          case 0: // bom
            pwmDisableChannel(&PWMD1,0);  // slår av timer 1
            break;
            
          case 1: // tilt og roatasjon
            pwmDisableChannel(&PWMD2,0);  // slår av timer 2
            pwmDisableChannel(&PWMD3,0);  // slår av timer 3
            break;

          case 2: // stikke
            pwmDisableChannel(&PWMD4,0);  // slår av timer 4
            break;

          case 3: // klo
            pwmDisableChannel(&PWMD8,1);  // slår av timer 5

        }

        }
     } 

    }
  }

