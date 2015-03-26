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

/**
 * @file web.c
 * @brief HTTP server wrapper thread code.
 * @addtogroup WEB_THREAD
 * @{
 */

#include "ch.h"

//#include "lwip/opt.h"
//#include "lwip/arch.h"
//#include "lwip/api.h"
#include "hal.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "server.h"

#define SERVER_THREAD_PORT 50000

#if LWIP_NETCONN

WORKING_AREA(wa_network_server, SERVER_THREAD_STACK_SIZE);

/**
 * @brief Parse Rov data stream
 * @details [long description]
 * 
 * @param string [description]
 * @return [description]
 */
static uint8_t* pagitrse_message(char *s){  
  
  uint8_t thValX = 0;
  uint8_t thValY = 0;
  uint8_t *array[2];
  uint32_t i;
  char *thX, *thY;  
    for(i = 0; i < sizeof(s)/sizeof(int); i++ ){
      
      /* Look For Thurster Data */
      if(s[i] == 'T'){
        palTogglePad(GPIOD, GPIOD_LED6);
        thX = s[i+3] + s[i+4];
        thValX = (uint8_t)atoi(thX);
        thY = s[i+6] + s[i+7];
        thValY = (uint8_t)atoi(thY);
      }
    }
  array[0] = thValX;
  array[1] = thValY;
  return array;
}

static void send_can_message(int ID, uint8_t *data){

  CANTxFrame txframe;
  uint8_t i;

  txframe.IDE = CAN_IDE_STD;
  txframe.SID = ID;
  txframe.RTR = CAN_RTR_DATA;
  txframe.DLC = 1;
  
  for(i = 0; i < sizeof(data)/sizeof(uint8_t); i++){
    txframe.data8[i] = data[i];
  
  canTransmit(&CAND1, CAN_ANY_MAILBOX, &txframe, MS2ST(100));

  palTogglePad(GPIOD, GPIOD_LED3);  /* Green.   */

}

}

static void server_serve(struct netconn *newconn) {
  struct netbuf *buf;
  char *data;
  u16_t len;
  err_t err;
  uint8_t *parsed;

  //uint8_t dataArray[8];

  while((err = netconn_recv(newconn, &buf)) == ERR_OK) {
    
    do{

      netbuf_data(buf, (void **)&data, &len);

      parsed = parse_message(data);

      err = netconn_write(newconn, &parsed, sizeof(&parsed), NETCONN_COPY);

      if (err != ERR_OK) {
        palSetPad(GPIOD, GPIOD_LED5);
      }
      //pwmEnableChannel(&PWMD4, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, value));
      
    } while (netbuf_next(buf) >= 0);
    netbuf_delete(buf);
  }
  netconn_close(newconn);
}

/**
 * Echo server Thread
 */
msg_t network_server(void *p) {
  struct netconn *conn, *newconn;
  err_t err;

  (void)p;

  /* Create a new TCP connection handle */
  conn = netconn_new(NETCONN_TCP);
  LWIP_ERROR("http_server: invalid conn", (conn != NULL), return RDY_RESET;);

  /* Bind to port 50000 with default IP address */
  netconn_bind(conn, NULL, SERVER_THREAD_PORT);

  /* Put the connection into LISTEN state */
  netconn_listen(conn);

  /* Goes to the final priority after initialization.*/
  chThdSetPriority(SERVER_THREAD_PRIORITY);

  while(1) {
    err = netconn_accept(conn, &newconn);
    if (err != ERR_OK)
      continue;
    char message[] = "Status OK";
    netconn_write(conn, &message, sizeof(message)-1, NETCONN_COPY);
    server_serve(newconn);
    netconn_delete(newconn);
  }
  return RDY_OK;
}

#endif /* LWIP_NETCONN */

/** @} */
