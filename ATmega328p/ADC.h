#ifndef ADC_H_
#define ADC_H_

//-----------------This preprocessor ensure before compiling for the right MCU------------//
#if defined(__AVR_ATmega16__) || defined(__AVR_ATmega16A__) || defined(__AVR_ATmega16P__) || defined(__AVR_ATmega16PA__)
#elif defined(__AVR_ATmega32__) || defined(__AVR_ATmega32A__)
#elif defined(__AVR_ATmega164__) || defined(__AVR_ATmega164A__) || defined(__AVR_ATmega164P__) || defined(__AVR_ATmega164PA__)
#elif defined(__AVR_ATmega324__) || defined(__AVR_ATmega324A__) || defined(__AVR_ATmega324P__) || defined(__AVR_ATmega324PA__)
#elif defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
#elif defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__)
#elif defined(__AVR_ATmega8535__)
#elif defined(__AVR_ATmega8__)
#elif defined(__AVR_ATmega48__) || defined(__AVR_ATmega48A__) || defined(__AVR_ATmega48P__) || defined(__AVR_ATmega48PA__) || defined(__AVR_ATmega48PB__)
#elif defined(__AVR_ATmega88__) || defined(__AVR_ATmega88A__) || defined(__AVR_ATmega88P__) || defined(__AVR_ATmega88PA__) || defined(__AVR_ATmega88PB__)
#elif defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega168A__) || defined(__AVR_ATmega168PA__) || defined(__AVR_ATmega168PB__)
#elif defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
#elif defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny24A__)
#elif defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny44A__)
#elif defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny84A__) || defined(__AVR_ATtiny85__)
#else
	#error "no definitions available for this AVR"
#endif




//-----------------------------------------------------------------------------------------------------//
#include <avr/io.h>
#define AREF 10
#define AVCC 11
#define INTERNAL 12
#define INTERNAL1v1 13
#define INTERNAL2v56 14


//Analog
#define ANALOG0     0
#define ANALOG1     1
#define ANALOG2     2
#define ANALOG3     3
#define ANALOG4     4
#define ANALOG5     5
#define ANALOG6     6
#define ANALOG7     7
#define ANALOG8     8
//-----------------------------------------------------------------------------------------------------//

#define CHIP_TEMP_ERROR_OFFSET 	-47    	  // Offset calibration in degrees Celsius

//------------------------------------All the function--------------------------------------//
void ADC_INIT(uint8_t prescaler,uint8_t Voltage_Reference);// use this function from your main project

//------------------------------------------------------------------------------------------//

void ADC_sensor(void);

void check_power(void);
void power_control(void);

uint32_t get_time_without_power_s();
uint16_t get_filtered_adc(uint8_t pin);
uint32_t get_time_without_power_s(void);
int8_t get_int_temperature(void);
uint16_t analogRead(uint8_t input);
#endif /* ADC_H_ */

