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

#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
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

 struct RovData
 {
   char ds[2][15];
   int16_t th[4];
 } rovdata;

 static struct RovData parse_message(char *string){
  char *token;
  struct RovData rov_data;
  uint8_t i = 0;

  for (token = strtok(string, ";"); token != NULL; token = strtok(NULL, ";")){
    strcpy(rov_data.ds[i], token);
    i++;
  }

  /* Parse data String */
  i = 0;

  for (token = strtok(rov_data.ds[1], ","); token != NULL; token = strtok(NULL, ",")){
    rov_data.th[i] = atoi(token);
    i++;
  }

  
  return rov_data;
 }



static void send_can_message(int ID, int16_t *data){

  CANTxFrame txframe;
  uint8_t i;

  txframe.IDE = CAN_IDE_STD;
  txframe.SID = ID;
  txframe.RTR = CAN_RTR_DATA;
  txframe.DLC = 1;
  
  for(i = 0; i < 3; i++){
    txframe.data8[i] = data[i];
  
  canTransmit(&CAND1, CAN_ANY_MAILBOX, &txframe, TIME_IMMEDIATE);

  palTogglePad(GPIOD, GPIOD_LED3);  /* Green.   */

}

}

static void server_serve(struct netconn *newconn) {
  struct netbuf *buf;
  struct RovData rov_data;
  void *data;
  u16_t len;
  err_t err;
  //uint8_t dataArray[8];

  while((err = netconn_recv(newconn, &buf)) == ERR_OK) {
    
    do{

      netbuf_data(buf, &data, &len);

      rov_data = parse_message(data);
      
      err = netconn_write(newconn, rov_data.ds[0], strlen(rov_data.ds[0]), NETCONN_COPY);

      /* Send parsed data onto canbus */
      send_can_message(0x1, rov_data.th);

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
    netconn_write(conn, message, sizeof(message)-1, NETCONN_COPY);
    server_serve(newconn);
    netconn_delete(newconn);
  }
  return RDY_OK;
}

#endif /* LWIP_NETCONN */

/** @} */
