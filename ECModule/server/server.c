/*****************************************************************************
DESCRIPTION Skeleton of the read-driver
*****************************************************************************/


/*--------------------  I n c l u d e   F i l e s  -------------------------*/

#include "ch.h"
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "hal.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "server.h"

/*--------------------  C o n s t a n t s  ---------------------------------*/

#define SERVER_THREAD_PORT 50000

/*--------------------  V a r i a b l e s  ---------------------------------*/

WORKING_AREA(wa_network_server, SERVER_THREAD_STACK_SIZE);

struct netconn *conn, *newconn;
struct RovData rov_data;
err_t err;

CANTxFrame manip;   // CanBus message frame containing data M1-M4
CANTxFrame manip2;  // CanBus message frame containing data M5
CANTxFrame ThFrame;  // CanBus message frame containing Thrusterdata

/*--------------------  T y p e s  -----------------------------------------*/

 struct RovData
 {
   char ds[2][20];
   int16_t th[4];
   int16_t manip[5];
 } rovdata;


/*--------------------  F u n c t i o n s  ---------------------------------*/

 static struct RovData parse_message(char *string){
  char *token;
  struct RovData rov_data;
  uint8_t i = 0;
  char str[50];

  strcpy(str, string);

  token = strtok(str, ",");

  strcpy(rov_data.ds[0], token);
  while (token != NULL){
    if (i < 4){
      rov_data.th[i] = atoi(token);
    }
    else
    {
      rov_data.manip[i] = atoi(token);
    }
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

  canTransmit(&CAND1, 1, &manip, TIME_IMMEDIATE);
  canTransmit(&CAND1, 2, &manip2, TIME_IMMEDIATE);

  palTogglePad(GPIOD, GPIOD_LED3);  /* Green.   */

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
    
      err = netconn_write(newconn, "", 1, NETCONN_COPY);

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
  //chThdSetPriority(SERVER_THREAD_PRIORITY);

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

/** @} */
