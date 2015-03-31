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

#define ADC_NUM_CHANNELS 1

#define ADC_BUF_DEPTH 8

static adcsample_t samples[ADC_NUM_CHANNELS * ADC_BUF_DEPTH];

static const ADCConversionGroup adcgrpcfg = {
  TRUE,
  ADC_NUM_CHANNELS,
  NULL,
  NULL,
  0,                        /* CR1 */
  ADC_CR2_SWSTART,          /* CR2 */
  ADC_SMPR1_SMP_AN12(ADC_SAMPLE_56) | ADC_SMPR1_SMP_AN11(ADC_SAMPLE_56) |
  ADC_SMPR1_SMP_SENSOR(ADC_SAMPLE_144) | ADC_SMPR1_SMP_VREF(ADC_SAMPLE_144),
  0,                        /* SMPR2 */
  ADC_SQR1_NUM_CH(ADC_NUM_CHANNELS),
  ADC_SQR2_SQ8_N(ADC_CHANNEL_SENSOR) | ADC_SQR2_SQ7_N(ADC_CHANNEL_VREFINT),
  ADC_SQR3_SQ6_N(ADC_CHANNEL_IN12)   | ADC_SQR3_SQ5_N(ADC_CHANNEL_IN11) |
  ADC_SQR3_SQ4_N(ADC_CHANNEL_IN12)   | ADC_SQR3_SQ3_N(ADC_CHANNEL_IN11) |
  ADC_SQR3_SQ2_N(ADC_CHANNEL_IN12)   | ADC_SQR3_SQ1_N(ADC_CHANNEL_IN11)
};


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
  /*
  if (rov_data.th[0] > 0){
    palSetPad(GPIOD, GPIOD_LED6);
  }
  else if(rov_data.th[0] < 0)
  {
    palTogglePad(GPIOD, GPIOD_LED6);
  }
  else
  {
    palClearPad(GPIOD, GPIOD_LED6);
  }
  */

  return rov_data;

}


static void send_can_message(int ID, int16_t *data){

  CANTxFrame txframe;
  uint8_t i;

  txframe.IDE = CAN_IDE_STD;
  txframe.SID = ID;
  txframe.RTR = CAN_RTR_DATA;
  txframe.DLC = 4;
  
  for(i = 0; i < 4; i++){
    txframe.data8[i] = data[i];
  }
  canTransmit(&CAND1, CAN_ANY_MAILBOX, &txframe, TIME_IMMEDIATE);

  palTogglePad(GPIOD, GPIOD_LED3);  /* Green.   */

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
      /*
      rov_data.th[0] = 30;
      rov_data.th[1] = 30;
      rov_data.th[2] = 30;
      rov_data.th[3] = 30;
  */

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

  adcStartConversion(&ADCD1, &adcgrpcfg, samples, ADC_BUF_DEPTH);

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
