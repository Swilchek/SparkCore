#ifndef __data_transmit_h
#define __data_transmit_h

#include "general.h"

enum TRANS_BITS{
    READY_CURRENT,
	READY_TEMP,
    READY_LIGHT,
	READY_SWITCH,
	READY_SENVACURRENT,
    READY_HEARTBEAT,
    LEN_TRANS
};

extern uint16_t DT__bits; // used to determine what is ready to be sent. Use DT_READY(signal) instead

#define DT_READY(signal) (DT__bits |= (1<<(signal)))
#define IS_DT_READY(signal) ((1<<(signal)) bitand DT__bits)

void setup_transmit();
void thread_heartbeat();
void send_data();
void thread_serialprint();
#endif
