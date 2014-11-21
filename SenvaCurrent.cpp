#include "SenvaCurrent.h"
#include "data_transmit.h"

#define max_period 1000

struct currentID ICSx;

/********************************************************
senva Current reader setup information - relative to 0-5VDC only
enter the "Sensed Amperage Range" setting value into the ICS#_A 
variable in order to correctly scale the output value for n.io
********************************************************/

float ICS1_A = 15.0; //5, 10, 20, 30, 60, 120, or 0 to disable
float ICS2_A = 10.0; //5, 10, 20, 30, 60, 120, or 0 to disable
float ICS3_A = 0.0; //5, 10, 20, 30, 60, 120, or 0 to disable

//float ICS_k = 1.18; // k-factor to correct for voltage drop on sensor using voltage divider for 3.3V limit

void thread_senvaCurrent(){

    RUN_ONLY_EVERY(max_period);
	
	ICSx.ICS1 = 0;
	ICSx.ICS2 = 0;
	ICSx.ICS3 = 0;	
	
	if (analogRead(CURRENT_PIN1) > 30)
		ICSx.ICS1 = (((float)analogRead(CURRENT_PIN1))/((float)ANALOG_MAX))*ICS1_A;
	if (analogRead(CURRENT_PIN2) > 30) 	
		ICSx.ICS2 = (((float)analogRead(CURRENT_PIN2))/((float)ANALOG_MAX))*ICS2_A;
	

    //The code below is used primarily for functionality testing
	/*
	ICS1 = analogRead(CURRENT_PIN1);
		Serial.print(ICS1); Serial.println(" = raw");
	ICS1 = ICS1/ANALOG_MAX;
		//Serial.print(ICS1); Serial.println(" = ICS1/ANALOG_MAX");
	ICS1 = ICS1*ICS1_A;
		Serial.print(ICS1); Serial.println(" = ICS1*ICS1_A");
	ICS1 = (((float)analogRead(CURRENT_PIN1))/((float)ANALOG_MAX))*ICS1_A;
		Serial.print(ICS1); Serial.println(" = 1_line");
	*/
	
	DT_READY(READY_SENVACURRENT);
}
