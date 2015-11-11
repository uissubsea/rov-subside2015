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
#include "manip/automanip.h"


/*===========================================================================*/
/* CAN Bus related.                                                          */
/*===========================================================================*/

/*
 * Internal loopback mode, 500KBaud, automatic wakeup, automatic recover
 * from abort mode.
 * See section 22.7.7 on the STM32 reference manual.
 */

static const CANConfig cancfg = {
  CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP,
  CAN_BTR_SJW(0) | CAN_BTR_TS2(1) |
  CAN_BTR_TS1(8) | CAN_BTR_BRP(6)
};


/*
 * PWM configuration structure.
 * Cyclic callback enabled, channels 1 and 4 enabled without callbacks,
 * the active state is a logic one.
 */
<<<<<<< HEAD
static PWMConfig pwmcfg = {
  10000,                                    /* 10kHz PWM clock frequency.   */
  10,                                    /* PWM period 1S (in ticks).    */
  pwmteller,
=======
static PWMConfig pwmcfgM1 = {
  10000,                                  /* 10kHz PWM clock frequency.   */
  20,                                    /* PWM period 1S (in ticks).    */
  pwmtellerM1,
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
static PWMConfig pwmcfgM2 = {
  10000,                                  /* 10kHz PWM clock frequency.   */
  20,                                    /* PWM period 1S (in ticks).    */
  pwmtellerM2,
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
static PWMConfig pwmcfgM3 = {
  10000,                                  /* 10kHz PWM clock frequency.   */
  20,                                    /* PWM period 1S (in ticks).    */
  pwmtellerM3,
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
static PWMConfig pwmcfgM4 = {
  10000,                                  /* 10kHz PWM clock frequency.   */
  20,                                    /* PWM period 1S (in ticks).    */
  pwmtellerM4,
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
static PWMConfig pwmcfgM5 = {
  10000,                                  /* 10kHz PWM clock frequency.   */
  20,                                    /* PWM period 1S (in ticks).    */
  pwmtellerM5,
>>>>>>> mdevel
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

  palSetPadMode(GPIOB, GPIOB_PIN8, PAL_MODE_ALTERNATE(9));
  palSetPadMode(GPIOB, GPIOB_SDA, PAL_MODE_ALTERNATE(9));

  canStart(&CAND1, &cancfg);
  /*
   * Starter Timerene på de navngitte utgangene
   */

  pwmStart(&PWMD1, &pwmcfgM1);    // Knytter Timer 1 med metoden pwmcfgM1
  pwmStart(&PWMD2, &pwmcfgM2);    // Knytter Timer 2 med metoden pwmcfgM2
  pwmStart(&PWMD3, &pwmcfgM3);    // Knytter Timer 3 med metoden pwmcfgM3
  pwmStart(&PWMD4, &pwmcfgM4);    // Knytter Timer 4 med metoden pwmcfgM4
  pwmStart(&PWMD8, &pwmcfgM5);    // Knytter Timer 8 med metoden pwmcfgM5

//palTogglePad(GPIOD, GPIOD_LED3);
  
  chThdCreateStatic(wa_manip_thread, MANIP_THREAD_STACK_SIZE, NORMALPRIO + 1,
                    manip_thread, NULL);    // starter tråden
  chThdCreateStatic(wa_automanip_thread, AUTOMANIP_THREAD_STACK_SIZE, NORMALPRIO + 1,
                    automanip_thread, NULL);    // starter tråden

  while(TRUE){
    chThdSleepMilliseconds(5000);
    palTogglePad(GPIOD, GPIOD_LED3);
  }

}

