#ifndef __general_h
#define __general_h

// General include for all files.
// Takes care of selecting arduino and choosing pins, etc


#define SPARK_SILLY
#ifdef SPARK_SILLY // Right now the include scheme is screwy because spark is screwed up
 #include "allduino.h"
 //#include "DHT.h"
 //#include "ptsmall.h"
 #include "tcp_client.h"
#else
 #include <allduino.h>
 //#include <DHT.h>
 //#include <ptsmall.h>
 #include <tcp_client.h>
#endif

#include <inttypes.h>
#include "data_transmit.h"
#include "light.h"
//#include "temperature.h"
#include "Uswitch.h"
#include "SenvaCurrent.h"



//Analog Pins
//A3 -- SPI_SCK
//A4 -- SPI_MISO
//A5 -- SPI_MOSI
#define LIGHT_PIN A0
#define ROOM_LIGHT_PIN A1
#define DISHWASHER_PIN A2
#define OCC_PIN A3
#define SCALE_PIN A4 //spare pin
#define CURRENT_PIN1 A5  //electric water kettle
#define CURRENT_PIN2 A6	 //Keurig coffee maker
#define TOWEL_PIN A7  

//Digital Pins
// D0 I2C_SDA
// D1 I2c_SCL
#define FREEZER_PIN D2
#define CABINET_PIN D3
#define COFFEE_LO_LVL_PIN D4
#define COFFEE_HI_LVL_PIN D5

//#define OCC_PIN D5
//#define DHT_PIN D6
//D7 -- LED

enum MODES{
    SERIAL_MODE,
    TCP_MODE,
    BOTH_MODE,
    NONE_MODE
};

#define TX_MODE BOTH_MODE

extern char endpoint[];
extern byte server[4];
extern const uint16_t port;

#endif
