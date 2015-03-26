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

//#include "lwipthread.h"

#include "server/server.h"
#include "relestyring/relestyring.h"

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
 * ADC Sample buffer
 */
//static adcsample_t samples[ADC_GRP1_NUM_CHANNELS * ADC_GRP1_BUF_DEPTH];


/*
 * Transmitter thread.
 */
static WORKING_AREA(can_tx_wa, 256);
static msg_t can_tx(void * p) {
  CANTxFrame txmsg;

  (void)p;
  chRegSetThreadName("transmitter");
  txmsg.IDE = CAN_IDE_STD;
  txmsg.SID = 0x012;
  txmsg.RTR = CAN_RTR_DATA;
  txmsg.DLC = 1;
  txmsg.data8[0] = 0x1;

  while (!chThdShouldTerminate()) {
    if(palReadPad(GPIOA, GPIOA_BUTTON)){
      canTransmit(&CAND1, CAN_ANY_MAILBOX, &txmsg, MS2ST(100));
      chThdSleepMilliseconds(1000);
    }
  }
  return 0;
}


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

  //palSetPadMode(GPIOB, GPIOB_CANRX, PAL_MODE_ALTERNATE(9));
  //palSetPadMode(GPIOB, GPIOB_CANTX, PAL_MODE_ALTERNATE(9));

  canStart(&CAND1, &cancfg);

  /*
   * Creates the LWIP threads (it changes priority internally).
   */
  //chThdCreateStatic(wa_lwip_thread, LWIP_THREAD_STACK_SIZE, NORMALPRIO + 1,
  //                  lwip_thread, NULL);

  /*
   * Creates the HTTP thread (it changes priority internally).
   */
  //chThdCreateStatic(wa_network_server, sizeof(wa_network_server), NORMALPRIO + 1,
  //                  network_server, NULL);

  
  chThdCreateStatic(waRelestyring, sizeof(waRelestyring), NORMALPRIO + 1,
                    relestyring, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state.
   */
  while (TRUE) {
    palTogglePad(GPIOD, GPIOD_LED4);
    chThdSleepMilliseconds(1000);
  }
}
