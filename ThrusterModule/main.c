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
#include "thruster/thruster.h"

/*
 * No Loopback, 500KBaud, automatic wakeup, automatic recover
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
static PWMConfig pwmcfg = {
  1000000,                                    /* 10kHz PWM clock frequency.   */
  20000,                                      /* PWM period (in ticks).    */
  NULL,                                     /* No Callback */
  {
    {PWM_OUTPUT_ACTIVE_HIGH, NULL},         /*TIMx Channel 1 */
    {PWM_OUTPUT_ACTIVE_HIGH, NULL},         /*TIMx Channeø 2 */
    {PWM_OUTPUT_ACTIVE_HIGH, NULL},
    {PWM_OUTPUT_ACTIVE_HIGH, NULL}
  },
  /* HW dependent part.*/
  0,
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

  palSetPadMode(GPIOB, GPIOB_CANRX, PAL_MODE_ALTERNATE(9));
  palSetPadMode(GPIOB, GPIOB_CANTX, PAL_MODE_ALTERNATE(9));

  canStart(&CAND1, &cancfg);

  chThdSleepMilliseconds(1000);
  
  /*
   * Initialize PWM Timers 1, 2 and 8 with same config
   */
  pwmStart(&PWMD1, &pwmcfg);
  pwmStart(&PWMD2, &pwmcfg);
  pwmStart(&PWMD8, &pwmcfg);
  
  /* Create Thruster thread */

  chThdCreateStatic(wa_thruster, sizeof(wa_thruster), NORMALPRIO + 1, thruster_thread, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state, when the button is
   * pressed the test procedure is launched with output on the serial
   * driver 2.
   */
  while (TRUE) {
    chThdSleepMilliseconds(10000);
  }
}
