#include "light.h"
#include "data_transmit.h"

#define SENSE_RES_MAX 10000
#define SENSE_RES_MIN 1000
#define SERIES_RES  10000

uint32_t light_reading = 0;
uint32_t room_light_reading = 0;

#define max_period 1000

void thread_light(){
    RUN_ONLY_EVERY(max_period);
    light_reading = analogRead(LIGHT_PIN);
    light_reading = (uint32_t)((uint32_t)ANALOG_MAX * (uint32_t)SERIES_RES) / light_reading;
    light_reading -= SERIES_RES;
	
	room_light_reading = analogRead(ROOM_LIGHT_PIN);
    room_light_reading = (uint32_t)((uint32_t)ANALOG_MAX * (uint32_t)SERIES_RES) / room_light_reading;
    room_light_reading -= SERIES_RES;
	
	DT_READY(READY_LIGHT);
}
