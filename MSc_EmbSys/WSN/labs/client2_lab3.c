/*
 * client_lab3.c
 *
 *  Created on: Nov 20, 2023
 *      Author: user
 */


#include "contiki.h"
#include "contiki-default-conf.h"

#include <stdio.h>
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uiplib.h"

#include "si7021.h"
#include "string.h"
#include "efr32mg12p432f1024gl125.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "board_cookie.h"
#include "util.h"
#include "imu.h"


#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_PORT 5678

#define MY_MESSAGE "Test UDP Client to Server"

static struct simple_udp_connection udp_conn;

uip_ipaddr_t dest_address;
static uint32_t hume = 0;
static int32_t temper = 0;

char humidity[80];
char temperature[80];

void udp_rx_callback (struct simple_udp_connection *c,
        const uip_ipaddr_t *source_addr,
        uint16_t source_port,
        const uip_ipaddr_t *dest_addr,
        uint16_t dest_port,
        const uint8_t *data, uint16_t datalen)

{
    printf("Received package from: ");
    uiplib_ipaddr_print(source_addr);
    printf("\nData: %s\n", (char *) data);

    //printf("Humidity: %lu.%lu (%%)\n", hume/1000, hume%1000);
    //printf("Temperature: %ld.%ld (ºC)\n", temper/1000, temper%1000);
}


PROCESS(my_udp_client_process, "Client process");
PROCESS(sensor_process, "Sensor process");
AUTOSTART_PROCESSES(&my_udp_client_process, &sensor_process);


PROCESS_THREAD(sensor_process, ev, data) {

  static struct etimer timer;

  PROCESS_BEGIN();
  printf("Hello, world. Welcome to Contiki-NG!\n");
  //GPIO_PinModeSet(gpioPortF, 8, gpioModePushPull, 1); // consum, set as out pin (PushPull)
  GPIO_PinModeSet(gpioPortF, 9, gpioModePushPull, 1); // T/H, set as out pin (PushPull)
  GPIO_PinOutSet(gpioPortF, 9);
  // T/H, enabled
  I2CSPM_Init_TypeDef i2cspmInit = I2CSPM_INIT_DEFAULT; // I2C configuration
  I2CSPM_Init(&i2cspmInit);
  // I2C configuration
  SI7021_init();
  // TEMP/HUM Sensor config
  etimer_set(&timer, CLOCK_SECOND*2);

  while(1){

	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    SI7021_measure(&hume, &temper);

    sprintf(humidity, "Humidity: %lu.%lu (%%)\n", hume/1000, hume%1000);
    sprintf(temperature, "Temperature: %ld.%ld (ºC)\n", temper/1000, temper%1000);

    if(NETSTACK_ROUTING.node_is_reachable() && NETSTACK_ROUTING.get_root_ipaddr(&dest_address))
          {
              simple_udp_sendto(&udp_conn, MY_MESSAGE, sizeof(MY_MESSAGE), &dest_address);

              simple_udp_sendto(&udp_conn, humidity, sizeof(humidity), &dest_address);
              simple_udp_sendto(&udp_conn, temperature, sizeof(temperature), &dest_address);

              printf("Sent message to server\n");

          } else {
              printf("The node is not reachable yet...\n");
          }

    etimer_reset(&timer);
  }
  PROCESS_END();
}

PROCESS_THREAD(my_udp_client_process, ev, data)
{
    //static struct etimer timer;

    PROCESS_BEGIN();

    printf("CLIENT");
    simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL, UDP_SERVER_PORT, udp_rx_callback);

    //etimer_set(&timer, CLOCK_SECOND * 2);

    /*
    while(1)
    {

      // Wait for the periodic timer to expire and then restart the timer.
      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
      if(NETSTACK_ROUTING.node_is_reachable() && NETSTACK_ROUTING.get_root_ipaddr(&dest_address))
      {
          simple_udp_sendto(&udp_conn, MY_MESSAGE, sizeof(MY_MESSAGE), &dest_address);



          printf("Sent message to server\n");
      } else {
          printf("The node is not reachable yet...\n");
      }
      etimer_reset(&timer);
    }
*/


  PROCESS_END();
}
