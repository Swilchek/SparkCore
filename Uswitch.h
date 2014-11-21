#ifndef __Uswitch_h
#define __Uswitch_h

#include "general.h"

struct switchID{
    bool SW1;
    bool SW2;
    bool SW3;
	bool SW4;
	bool SW5;
	bool SW6;
	bool SW7;
	float SW8;
};

struct DHT{
	float hum;
	float temp;
};

extern struct switchID switch_ID;
extern struct DHT temp_DHT;

void setup_switches();
void thread_switches();
void thread_temperature();

#endif