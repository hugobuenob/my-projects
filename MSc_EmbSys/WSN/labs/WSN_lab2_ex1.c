#include "contiki.h"
#include <stdio.h> /* For printf() */
#include "leds.h"
#include "serial-line.h"
#include "string.h"
#include "efr32mg12p432f1024gl125.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "si7021.h"
#include "board_cookie.h"
#include "util.h"
#include "imu.h" // For IMU
//#include "icm20648.h" // For IMU

process_event_t myevent;

PROCESS(serial_process, "Serial line process");
PROCESS(led_process, "LED process");
PROCESS(sensor_process, "Sensor process");

AUTOSTART_PROCESSES(&serial_process, &sensor_process, &led_process);

PROCESS_THREAD(serial_process, ev, data){

  static struct etimer et;

  PROCESS_BEGIN();
  myevent = process_alloc_event();
  etimer_set(&et, CLOCK_SECOND*2);

  while(1){
    PROCESS_WAIT_EVENT_UNTIL(ev == serial_line_event_message);
    printf("new packet from serial line %s \n",(char*)data);
    process_post(PROCESS_BROADCAST, myevent, data);
  }
PROCESS_END();

}

PROCESS_THREAD(sensor_process, ev, data) {

  static struct etimer timer;
  uint32_t hume = 0;
  int32_t temper = 0;

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

    PROCESS_WAIT_EVENT_UNTIL(ev == myevent);
    printf("Executing my sensor process ...\n");
    SI7021_measure(&hume, &temper);

    if (strcmp(data, "SH") == 0)
      printf("Humidity: %lu.%lu (%%)\n", hume/1000, hume%1000);
    else if (strcmp(data, "ST") == 0)
      printf("Temperature: %ld.%ld (ºC)\n", temper/1000, temper%1000);

    //PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    etimer_reset(&timer);
  }
  PROCESS_END();
}

PROCESS_THREAD(led_process, ev, data){
  //static struct etimer et;
  PROCESS_BEGIN();
  //etimer_set(&et, CLOCK_SECOND);
  while(1){
    //PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    PROCESS_WAIT_EVENT_UNTIL(ev == myevent);
    printf("Executing my LED process ...\n");
    //etimer_reset(&et);
    if (strcmp(data, "LG") == 0)
      leds_toggle(LEDS_GREEN);
    else if (strcmp(data, "LR") == 0)
      leds_toggle(LEDS_RED);
  }
  PROCESS_END();
}
