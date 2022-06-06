#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED
#define F_CPU 16000000UL
//#define F_CPU 14745600UL

//DS18B20 sensor
#define DS18B20_SENSOR	PB0
#define DS18B20_PORT	PORTB

#define BTN_DEFAULT     B, 1
#define SINGLE_SENSOR   B, 2
#define GREEN_LED	    B, 3
#define RED_LED	    	B, 5

#define PIN_PWR_GSM		D, 4
#define PIN_BUZZER		D, 5
#define PIN_BEEP        D, 6
#define PIN_LIGHT       D, 7

#define SENSOR_1	    C, 0
#define SENSOR_2	    C, 1
#define SENSOR_3	    C, 2
#define SENSOR_4	    C, 3
#define BTN_GUARD	    C, 4
#define BTN_LOCAL_GUARD	C, 5
#define PORT_SENSOR     PINC
#define BAT_LEVEL       ANALOG6
#define SMOKE_DETECTOR  ANALOG7
#define INT_TEMP        ANALOG8

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <avr/eeprom.h>

#include "IO_Macros.h"
#include "softuart.h"
#include "timer.h"
#include "UART.h"
#include "port.h"
#include "GSM.h"
#include "ADC.h"
#include "sms_parse.h"
#include "eeprom.h"
#include "app.h"
#include "ds18b20.h"
#include "su_openwrt.h"



#endif // MAIN_H_INCLUDED
