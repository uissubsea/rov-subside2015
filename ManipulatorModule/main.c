/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"
uint16_t i = 1;         // Lager teller variabel i    
uint16_t j = 1;         // Lager teller variabel j 
int VerdiJoy;
int Joyakse1;
int Joyakse2;
int Joyakse3;
int Joyakse4;
int Joyakse5;


static void pwmteller(PWMDriver *pwmp){
 (void)pwmp;
 if (palReadPad(GPIOA,GPIOA_BUTTON))
 {
 
 }
}
/*
 * PWM configuration structure.
 * Cyclic callback enabled, channels 1 and 4 enabled without callbacks,
 * the active state is a logic one.
 */
static PWMConfig pwmcfg = {
  10000,                                    /* 10kHz PWM clock frequency.   */
  100000,                                    /* PWM period 1S (in ticks).    */
  pwmteller,
  {
    {PWM_OUTPUT_ACTIVE_HIGH, NULL}, /* Kanal 1 */
    {PWM_OUTPUT_ACTIVE_HIGH, NULL}, /* Kanal 2 */
    {PWM_OUTPUT_ACTIVE_HIGH, NULL}, /* Kanal 3 */
    {PWM_OUTPUT_ACTIVE_HIGH, NULL}  /* Kanal 4 */
  },
  /* HW dependent part.*/
  0,
  0
};



/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /*
   * Starter Timerene på de navngitte utgangene
   */

  pwmStart(&PWMD1, &pwmcfg);    // Starter Timer 1
  pwmStart(&PWMD2, &pwmcfg);    // Starter Timer 2
  pwmStart(&PWMD3, &pwmcfg);    // Starter Timer 3
  pwmStart(&PWMD4, &pwmcfg);    // Starter Timer 4
  pwmStart(&PWMD8, &pwmcfg);    // Starter Timer 8


  while(TRUE) {     //Sett inn styrespak
    pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 1000)); //PA8
    if(VerdiJoy>=0)
    {
      if(Joyakse1)
      {
        pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 1000)); //PA8
      }
      else if(Joyakse2)
      {
        pwmEnableChannel(&PWMD2, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD2, 1000)); //PA15
      }
      else if(Joyakse3)
      {
        pwmEnableChannel(&PWMD3, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, 1000)); //PA15
      }
      else if(Joyakse4)
      {
        pwmEnableChannel(&PWMD4, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 1000)); //PA15
      }
      else if(Joyakse5)
      {
        pwmEnableChannel(&PWMD8, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 1000)); //PA15
      }
      
       
      // pwmChangePeriod(&PWMD4, 10);
      // chThdSleepMilliseconds(3000);
      // pwmDisableChannel(&PWMD4, 1);
      // pwmEnableChannel(&PWMD3, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, 1000));
    }
    else
    {
    //  palSetPad(GPIOA, GPIOA_PIN13);     // Snur retning på motorer
    //  palSetPad(GPIOA, GPIOA_PIN14);
    //  palSetPad(GPIOC, GPIOC_PIN10);
    //  palSetPad(GPIOC, GPIOC_PIN11);
    //  palSetPad(GPIOC, GPIOC_PIN12);

      if(Joyakse1)
      {
        pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 1000)); //PA8
      }
      else if(Joyakse2)
      {
        pwmEnableChannel(&PWMD2, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD2, 1000)); //PA15
      }
      else if(Joyakse3)
      {
        pwmEnableChannel(&PWMD3, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, 1000)); //PA15
      }
      else if(Joyakse4)
      {
        pwmEnableChannel(&PWMD4, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 1000)); //PA15
      }
      else if(Joyakse5)
      {
        pwmEnableChannel(&PWMD8, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 1000)); //PA15
      }
      
       
      // pwmChangePeriod(&PWMD4, 100);
      // pwmChangePeriod(&PWMD4, 20);
      // chThdSleepMilliseconds(3000);
      // pwmDisableChannel(&PWMD4, 1);
      // palClearPad(GPIOD, GPIOD_PIN3);
      // pwmEnableChannel(&PWMD3, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, 1000));
    }
    pwmDisableChannel(&PWMD4,1);
  }

  }

