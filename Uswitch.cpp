#include "Uswitch.h"
#include "data_transmit.h"

#define max_period 1000

struct switchID switch_ID;
struct DHT temp_DHT;

void setup_switches(){
pinMode(FREEZER_PIN, INPUT_PULLUP); //enable pull-up resistor for occupancy sensor
pinMode(CABINET_PIN, INPUT_PULLUP); //enable pull-up resistor for occupancy sensor
pinMode(COFFEE_LO_LVL_PIN, INPUT_PULLUP); //enable pull-up resistor for occupancy sensor
pinMode(COFFEE_HI_LVL_PIN, INPUT_PULLUP); //enable pull-up resistor for occupancy sensor
}

void thread_switches(){

    RUN_ONLY_EVERY(max_period);
	
	//digital pins
	switch_ID.SW1 = digitalRead(FREEZER_PIN);
	switch_ID.SW2 = digitalRead(CABINET_PIN);
	switch_ID.SW3 = !digitalRead(COFFEE_LO_LVL_PIN);
	switch_ID.SW4 = digitalRead(COFFEE_HI_LVL_PIN);
	
	//analog pins
	switch_ID.SW5 = (analogRead(DISHWASHER_PIN) > 2000)?1:0;
	switch_ID.SW6 = (analogRead(OCC_PIN) > 2000)?1:0;
	switch_ID.SW7 = (analogRead(TOWEL_PIN) > 2000)?1:0;
	switch_ID.SW8 = analogRead(SCALE_PIN);
	
	
	
	DT_READY(READY_SWITCH);
}

void thread_temperature(){

	RUN_ONLY_EVERY(max_period);
	
	//bogus data - ignore. 
	temp_DHT.hum = 6.374;
	temp_DHT.temp = 2.437;
	// end bogus data
	
	DT_READY(READY_TEMP);
	
	
}
