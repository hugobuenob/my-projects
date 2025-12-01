/*
 * server_lab3_ex3.c
 *
 *  Created on: Nov 20, 2023
 *      Author: user
 */


#include "contiki.h"

#include <stdio.h>
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uiplib.h"

#include "leds.h"
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

#define MY_MESSAGE "Response from Server to Client"

static struct simple_udp_connection udp_conn;

uip_ipaddr_t source_address;

void udp_rx_callback (struct simple_udp_connection *c,
        const uip_ipaddr_t *source_addr,
        uint16_t source_port,
        const uip_ipaddr_t *dest_addr,
        uint16_t dest_port,
        const uint8_t *data, uint16_t datalen)

{
    printf("Received package from:");
    uiplib_ipaddr_print(source_addr);
    printf("%s\n", (char *) data);
    //simple_udp_sendto(&udp_conn, MY_MESSAGE, sizeof(MY_MESSAGE), source_addr);
    //printf("Response to client\n");
    source_address = *source_addr;

}

PROCESS(my_udp_server_process, "Server process");
AUTOSTART_PROCESSES(&my_udp_server_process);

PROCESS_THREAD(my_udp_server_process, ev, data)
{

  PROCESS_BEGIN();

  static struct etimer timer;

  printf("SERVER");

  NETSTACK_ROUTING.root_start();

  simple_udp_register(&udp_conn, UDP_SERVER_PORT, NULL, UDP_CLIENT_PORT, &udp_rx_callback);

  etimer_set(&timer, CLOCK_SECOND * 2);

  while(1)
  {
    /* Wait for the periodic timer to expire and then restart the timer. */
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    if(NETSTACK_ROUTING.node_is_reachable() && NETSTACK_ROUTING.get_root_ipaddr(&source_address))
    {
        simple_udp_sendto(&udp_conn, "ST", sizeof("ST"), &source_address);
        //simple_udp_sendto(&udp_conn, "SH", sizeof("SH"), &source_addr);
        //simple_udp_sendto(&udp_conn, "LR", sizeof("LR"), &source_addr);
        //simple_udp_sendto(&udp_conn, "LG", sizeof("LG"), &source_addr);

        printf("Requested actions to the client\n");

    } else {
        printf("The client node is not reachable yet...\n");
    }
    etimer_reset(&timer);
  }

  PROCESS_END();
}
