#include "general.h"
#include "json.h"

#define SCAN_PERIOD 1000

#define BIG_BUF_LEN 800
char big_char_buf[BIG_BUF_LEN];

TcpClientObj client;

uint16_t DT__bits = 0;

void setup_transmit(){
    Serial.println("setting up transmit");
    client.init(server, port, endpoint);
    client.print();
    delay(1000); // DO NOT DELETE - needed for Windows based server functionality
}

uint16_t send_light(char **buf, uint16_t len){
    if(not IS_DT_READY(READY_LIGHT)){
        return len;
    }
    len = json_continue(buf, len);
	len = json_object(buf, len, "LIGHT");
    len = put_json(buf, len, VAR_SET_COMMA, "PhotoR", (light_reading * 1.0), 0);
	len = put_json(buf, len, VAR_SET_END, "Room_PhotoR", (room_light_reading * 1.0), 0);
	
    return len;
}

uint16_t send_temperature(char **buf, uint16_t len){
    if(not IS_DT_READY(READY_TEMP)){
        return len;
    }
    len = json_continue(buf, len);
    len = json_object(buf, len, "DHT");
    len = put_json(buf, len, VAR_SET_COMMA, "hum", temp_DHT.hum, 2);
    len = put_json(buf, len, VAR_SET_END, "tempC", temp_DHT.temp, 2);
    return len;
}

uint16_t send_switches(char **buf, uint16_t len){
	if(not IS_DT_READY(READY_SWITCH)){
		return len;
	}
	len = json_continue(buf, len);
    len = json_object(buf, len, "switchID");
    len = put_json(buf, len, VAR_SET_COMMA, "SW1", switch_ID.SW1, 0);
    len = put_json(buf, len, VAR_SET_COMMA, "SW2", switch_ID.SW2, 0);
	len = put_json(buf, len, VAR_SET_COMMA, "SW3", switch_ID.SW3, 0);
	len = put_json(buf, len, VAR_SET_COMMA, "SW4", switch_ID.SW4, 0);
	len = put_json(buf, len, VAR_SET_COMMA, "SW5", switch_ID.SW5, 0);
	len = put_json(buf, len, VAR_SET_COMMA, "SW6", switch_ID.SW6, 0);
	len = put_json(buf, len, VAR_SET_COMMA, "SW7", switch_ID.SW7, 0);
    len = put_json(buf, len, VAR_SET_END, "SW8", (switch_ID.SW8 * 1.0), 0);
		
	return len;
}

uint16_t send_senvaCurrent(char **buf, uint16_t len){
	if(not IS_DT_READY(READY_SENVACURRENT)){
		return len;
	}
	len = json_continue(buf, len);
    len = json_object(buf, len, "currentID");
    len = put_json(buf, len, VAR_SET_COMMA, "ICS1", ICSx.ICS1, 2);
    len = put_json(buf, len, VAR_SET_END, "ICS2", ICSx.ICS2, 2);
	return len;
}

uint16_t send_heartbeat(char **buf, uint16_t len){
    static uint16_t i = 0;
    if(not IS_DT_READY(READY_HEARTBEAT)){
        return len;
    }
    len = json_continue(buf, len);
    len = put_json(buf, len, VAR_SET, "heartbeat", i, 0);
    i++;
    return len;
}

void thread_heartbeat(){
    RUN_ONLY_EVERY(1000);
    DT_READY(READY_HEARTBEAT);
}

void thread_serialprint(){
	RUN_ONLY_EVERY(1000);

//The code below is used primarily for functionality testing
	Serial.println("Polling.................................................");
	
	Serial.print("FREEZER DOOR: "); 
    Serial.println(switch_ID.SW1);
	Serial.print("CABINET DOOR: "); 
    Serial.println(switch_ID.SW2);
	Serial.print("COFFEE LEVEL LOW: "); 
    Serial.println(switch_ID.SW3);
	Serial.print("COFFEE LEVEL HI: "); 
    Serial.println(switch_ID.SW4);
	Serial.print("Dishwasher Door: "); Serial.print(analogRead(DISHWASHER_PIN)); Serial.print(" (raw): ");
	Serial.print("Dishwasher Door: "); 
    Serial.println(switch_ID.SW5);
	Serial.print("Occupancy: "); 
    Serial.println(switch_ID.SW6);
	Serial.print("Paper Towels: "); 
    Serial.println(switch_ID.SW7);
	Serial.print("Beer Shelf: "); 
    Serial.println(switch_ID.SW8);
	
	Serial.print("ICS1: "); Serial.print(analogRead(CURRENT_PIN1)); Serial.print(" (raw): ");
    Serial.print(ICSx.ICS1); Serial.println("A");
	Serial.print("ICS2: "); Serial.print(analogRead(CURRENT_PIN2)); Serial.print(" (raw): ");
    Serial.print(ICSx.ICS2); Serial.println("A");
	
	Serial.print("Fridge Temp (C): "); 
    Serial.println(temp_DHT.temp);
	Serial.print("Fridge Humidity (%): "); 
    Serial.println(temp_DHT.hum);
	
	Serial.print("Fridge light reading: "); 
    Serial.println(light_reading);
	Serial.print("Room light reading: "); 
    Serial.println(room_light_reading);	

}

void send_data(){

	RUN_ONLY_EVERY(SCAN_PERIOD);

    char *buf = big_char_buf;

    client.flush();
    //client.print_flush();

    if(not DT__bits){
        return;
    }
    if(not client.transmit_available()){
        return;
    }

    uint16_t len = BIG_BUF_LEN;

    len = json_start(&buf, len);

    len = send_heartbeat(&buf, len);
    len = send_light(&buf, len);
    len = send_temperature(&buf, len);
	len = send_switches(&buf, len);
	len = send_senvaCurrent(&buf, len);

    len = json_end(&buf, len); // end of data

    if(len == 0){
        Serial.println("Error: Length was not enough");
        return;
    }

    //send_to_tcp(buf + 1, len, &client, big_char_buf, BIG_BUF_LEN - len);
    client.transmit(buf + 1, len, big_char_buf, BIG_BUF_LEN - len);
    DT__bits = 0;
	
}
