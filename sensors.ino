#include "general.h"

//byte server[] = {192, 168, 100, 90}; // Boulder Windows
byte server[] = {54, 173, 59, 93}; // C3.Large Instance - Duration Test Server

// Break Room
const uint16_t port = 8182;
char endpoint[] = "/break_room"; // Break Room

// Dev
//const uint16_t port = 8183;
//char endpoint[] = "/lab_fridge"; // Break Room


uint32_t place = 0;

void setup(){
    // Essential
    Spark.disconnect();
    Serial.begin(115200);

    // setup threads
    //setup_temperature();
	setup_switches();

    //Setup rest
    setup_transmit();
    Serial.println("Started");
}

void loop(){
    HEARTBEAT_WATCHDOG();   // flash light and ensure system is running well.
    thread_heartbeat();     // send a heartbeat over TCP
    thread_light();         // get light (photoresistor) data
	thread_switches();		// get multiple discrete switches data
	thread_temperature();	// artificial temp data
	thread_senvaCurrent();  // get current reading from Senva Current sensors
	send_data();            // send all the data we have gotten
	thread_serialprint();	// debugging serial printout, organized
	
}

