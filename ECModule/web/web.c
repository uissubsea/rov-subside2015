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

/*
 * This file is a modified version of the lwIP web server demo. The original
 * author is unknown because the file didn't contain any license information.
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
#include <stdio.h>

#include "web.h"

#define SERVER_THREAD_PORT 50000

#if LWIP_NETCONN

WORKING_AREA(wa_network_server, SERVER_THREAD_STACK_SIZE);

static void server_serve(struct netconn *newconn) {
  struct netbuf *buf;
  char *data;
  u16_t len;
  err_t err;

  while((err = netconn_recv(newconn, &buf)) == ERR_OK) {
    
    do{
      netbuf_data(buf, (void **)&data, &len);
      err = netconn_write(newconn, data, len, NETCONN_COPY);
      
      if (err != ERR_OK) {
        palSetPad(GPIOD, GPIOD_LED5);
      }
      else if(data[1] == '1'){
        //pwmEnableChannel(&PWMD4, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 1000));
        pwmChangePeriod(&PWMD4, 1000);
      }
      else if(data[1] == '2'){
        //pwmEnableChannel(&PWMD4, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 3000));
        pwmChangePeriod(&PWMD4, 100);
      }
      else if(data[1] == '3'){
        //pwmEnableChannel(&PWMD4, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 6000));
        pwmChangePeriod(&PWMD4, 10);
      }else{
        pwmEnableChannel(&PWMD4, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 0));
      }
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

  /* Bind to port 80 (HTTP) with default IP address */
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
