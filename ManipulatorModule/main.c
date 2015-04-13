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
#include "manip/manip.h"

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


  
  chThdCreateStatic(wa_manip_thread, MANIP_THREAD_STACK_SIZE, MANIP_THREAD_PRIORITY,
                    manip_thread, NULL);

  while(TRUE){
    
  }

  }

