#include "ch.h"
#include "hal.h"
#include "manip.h"

WORKING_AREA(wa_manip_thread, MANIP_THREAD_STACK_SIZE);

int16_t Mverdier[4];


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

  canReceive(&CAND1, CAN_ANY_MAILBOX, &rxmsg, TIME_INFINITE);
  
  palTogglePad(GPIOD, GPIOD_LED4);

  /* Set Thrust values to struct */

  mvalues.M1 = rxmsg.data16[0];
  mvalues.M2 = rxmsg.data16[1];
  mvalues.M3 = rxmsg.data16[2];
  mvalues.Rotasjon = rxmsg.data16[3];
  mvalues.Klo = rxmsg.data16[4];

  return mvalues;
}



uint16_t k=0;
uint16_t l=0;
uint16_t r=0;
uint16_t n=0;
uint16_t o=0;
uint16_t p=0;
uint16_t m=0;
uint16_t u=0;
uint16_t b=0;
uint16_t v=0;



void pwmtellerM1(PWMDriver *pwmp){
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
void pwmtellerM2(PWMDriver *pwmp){
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
void pwmtellerM3(PWMDriver *pwmp){
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
void pwmtellerM4(PWMDriver *pwmp){
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
void pwmtellerM5(PWMDriver *pwmp){
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



msg_t manip_thread(void *p){
(void)p;

struct Mvalues verdier;



uint8_t i;



  while(TRUE) {     //Sett inn styrespak


    verdier = receiveData();
    Mverdier[0] = verdier.M1;
    Mverdier[1] = verdier.M2;
    Mverdier[2] = verdier.M3;
    Mverdier[3] = verdier.Rotasjon;
    Mverdier[4] = verdier.Klo;

    for (i = 0; i < 5; i++)
     {
       if(Mverdier[i] > 0)
       {
        switch(i){
          case 0:
            palClearPad(GPIOC, GPIOC_PIN10);
            pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 5000)); //PA8
            break;

          case 1:
            palClearPad(GPIOC, GPIOC_PIN11);
            pwmEnableChannel(&PWMD2, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD2, 5000)); //PA15
            break;

          case 2:
            palClearPad(GPIOC, GPIOC_PIN12);
            pwmEnableChannel(&PWMD3, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, 5000)); //PC6
            break;

          case 3:
            palClearPad(GPIOA, GPIOA_PIN9);
            pwmEnableChannel(&PWMD4, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 5000)); //PB6
            break;

          case 4:
            palClearPad(GPIOA, GPIOC_PIN10);
            pwmEnableChannel(&PWMD8, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 5000)); //PC7
            break;

        }
       }
       else if(Mverdier[i]<0)
       {
        switch(i){
          case 0:
            palSetPad(GPIOC, GPIOC_PIN10);
            pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 5000));
            break;

          case 1:
            palSetPad(GPIOC, GPIOC_PIN11);
            pwmEnableChannel(&PWMD2, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD2, 5000)); //PA15
            break;

          case 2:
            palSetPad(GPIOC, GPIOC_PIN12);
            pwmEnableChannel(&PWMD3, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, 5000)); //PC6
            break;

          case 3:
            palSetPad(GPIOA, GPIOA_PIN9);
            pwmEnableChannel(&PWMD4, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 5000)); //PB6

            break;

          case 4:
            palSetPad(GPIOA, GPIOA_PIN10);
            pwmEnableChannel(&PWMD8, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 5000)); //PC7
            break;
        }
       }
       else
       {
        switch(i){

          case 0:
            pwmDisableChannel(&PWMD1,0);  
            break;
            
          case 1:
            pwmDisableChannel(&PWMD2,0);
            break;
          case 2:
            pwmDisableChannel(&PWMD3,0);
            break;

          case 3:
            pwmDisableChannel(&PWMD4,0);
            break;

          case 4:
            pwmDisableChannel(&PWMD8,2);
            break;
        }
        }
     } 

    }

}

