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

#define ADC_NUM_CHANNELS 2

#define ADC_BUF_DEPTH 8

/* Global Variables */

static adcsample_t samples[ADC_NUM_CHANNELS * ADC_BUF_DEPTH];
struct netconn *conn, *newconn;

struct RovData rov_data;
err_t err;

/* Can transmit messages for manipulator */
CANTxFrame manip;
CANTxFrame manip2;


/* Configuration Structs */

static const ADCConversionGroup adcgrpcfg = {
  TRUE,
  ADC_NUM_CHANNELS,
  NULL,
  NULL,
  0,                        /* CR1 */
  ADC_CR2_SWSTART,          /* CR2 */
  ADC_SMPR1_SMP_SENSOR(ADC_SAMPLE_144) | ADC_SMPR2_SMP_AN5(ADC_SAMPLE_56),
  0,                        /* SMPR2 */
  ADC_SQR1_NUM_CH(ADC_NUM_CHANNELS),
  0,
  ADC_SQR3_SQ1_N(ADC_CHANNEL_SENSOR) | ADC_SQR3_SQ2_N(ADC_CHANNEL_IN5)
};

/* Main Program */


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
   char ds[2][20];
   int16_t th[4];
   int16_t manip[5];
 } rovdata;

 static struct RovData parse_message(char *string){
  char *token;
  struct RovData rov_data;
  uint8_t i = 0;
  char str[20];

  strcpy(str, string);

  token = strtok(str, ",");

  strcpy(rov_data.ds[0], token);
  while (token != NULL){
    rov_data.th[i] = atoi(token);
    token = strtok(NULL, ",");
    i++;
  }

  return rov_data;

}


static void send_can_message(int ID, int16_t *data){

  uint8_t i;

  manip.IDE = CAN_IDE_STD;
  manip.SID = ID;
  manip.RTR = CAN_RTR_DATA;
  manip.DLC = 8;

  manip2.IDE = CAN_IDE_STD;
  manip2.SID = ID;
  manip2.RTR = CAN_RTR_DATA;
  manip2.DLC = 2;
  
  for(i = 0; i < 4; i++){
    manip.data16[i] = data[i];
  }

  /* Send last motor value */
  manip2.data16[0] = data[4];

  canTransmit(&CAND1, CAN_ANY_MAILBOX, &manip, TIME_IMMEDIATE);
  //canTransmit(&CAND1, 2, &manip2, TIME_IMMEDIATE);

  palTogglePad(GPIOD, GPIOD_LED3);  /* Green.   */

}

/**
 * @brief Send info to Client
 * @details This function will
 * send data back to client for implementation
 * in RovControl
 */
static void send_info(void){
  char tempstr[10];
  int Vsense;
  int TCelsius;

  Vsense = (float)(samples[0] * 3300 / 0xfff) / 1000;
  TCelsius = ((Vsense - 0.76) / 2.5) + 25.0;

  sprintf(tempstr, "%d, %d", rov_data.th[0], rov_data.th[1]);

  err = netconn_write(newconn, tempstr, strlen(tempstr), NETCONN_COPY);

}


static void server_serve(void) {
  struct netbuf *buf;
  void *data;
  u16_t len;
  err_t err;
  

  while((err = netconn_recv(newconn, &buf)) == ERR_OK) {
    
    do{

      netbuf_data(buf, &data, &len);

      rov_data = parse_message(data);
    
      send_info();

      /* Send parsed data onto canbus */
      send_can_message(0x1, rov_data.th);

      if (err != ERR_OK) {
        palSetPad(GPIOD, GPIOD_LED5);
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

  /*
   * Starts an ADC continuous conversion.
   */
  adcStartConversion(&ADCD1, &adcgrpcfg, samples, ADC_BUF_DEPTH);

  while(1) {
    err = netconn_accept(conn, &newconn);
    if (err != ERR_OK)
      continue;
    char message[] = "Status OK";
    netconn_write(conn, message, sizeof(message)-1, NETCONN_COPY);
    server_serve();
    netconn_delete(newconn);
  }
  return RDY_OK;
}

#endif /* LWIP_NETCONN */

/** @} */
