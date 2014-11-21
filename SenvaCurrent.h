#ifndef __SenvaCurrent_h
#define __SenvaCurrent_h

#include "general.h"

struct currentID{
    float ICS1;
    float ICS2;
    float ICS3;    
};

extern struct currentID ICSx;

//void setup_senvaCurrent();
void thread_senvaCurrent();

#endif