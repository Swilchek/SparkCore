#ifndef __light_h
#define __light_h

#include "general.h"

extern uint32_t light_reading;
extern uint32_t room_light_reading;

void thread_light();

#endif
