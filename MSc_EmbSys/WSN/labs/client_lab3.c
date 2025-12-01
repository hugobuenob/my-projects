/*
 * client_lab3.c
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

#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_PORT 5678

#define MY_MESSAGE "Test UDP Client to Server"

static struct simple_udp_connection udp_conn;

uip_ipaddr_t dest_address;

void udp_rx_callback (struct simple_udp_connection *c,
        const uip_ipaddr_t *source_addr,
        uint16_t source_port,
        const uip_ipaddr_t *dest_addr,
        uint16_t dest_port,
        const uint8_t *data, uint16_t datalen)

{
    printf("Received package from: ");
    uiplib_ipaddr_print(source_addr);
    printf(" Data: %s\n", (char *) data);
}






PROCESS(my_udp_client_process, "Client process");
AUTOSTART_PROCESSES(&my_udp_client_process);

PROCESS_THREAD(my_udp_client_process, ev, data)
{
    static struct etimer timer;

    PROCESS_BEGIN();

    printf("CLIENT");


    simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL, UDP_SERVER_PORT, udp_rx_callback);


    etimer_set(&timer, CLOCK_SECOND * 2);


    while(1)
    {

      /* Wait for the periodic timer to expire and then restart the timer. */
      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
      if(NETSTACK_ROUTING.node_is_reachable() && NETSTACK_ROUTING.get_root_ipaddr(&dest_address))
      {
          simple_udp_sendto(&udp_conn, MY_MESSAGE, sizeof(MY_MESSAGE), &dest_address);
          printf("Sent message to server\n");
      }
      else
      {
          printf("The node is not reachable yet...\n");
      }


      etimer_reset(&timer);

    }


  PROCESS_END();
}


