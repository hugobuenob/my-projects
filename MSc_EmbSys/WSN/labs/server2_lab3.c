/*
 * server_lab3.c
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

#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_PORT 5678

#define MY_MESSAGE "Response from Server to Client"

static struct simple_udp_connection udp_conn;

void udp_rx_callback (struct simple_udp_connection *c,
        const uip_ipaddr_t *source_addr,
        uint16_t source_port,
        const uip_ipaddr_t *dest_addr,
        uint16_t dest_port,
        const uint8_t *data, uint16_t datalen)

{
    printf("Received package from:");
    uiplib_ipaddr_print(source_addr);
    printf("Data: %s\n", (char *) data);

    //simple_udp_sendto(&udp_conn, MY_MESSAGE, sizeof(MY_MESSAGE),source_addr);
    //printf("Sent message to client\n");

}



PROCESS(my_udp_server_process, "Server process");
AUTOSTART_PROCESSES(&my_udp_server_process);

PROCESS_THREAD(my_udp_server_process, ev, data)
{

  PROCESS_BEGIN();

  printf("SERVER");

  NETSTACK_ROUTING.root_start();

  simple_udp_register(&udp_conn, UDP_SERVER_PORT, NULL, UDP_CLIENT_PORT, &udp_rx_callback);


  PROCESS_END();
}
