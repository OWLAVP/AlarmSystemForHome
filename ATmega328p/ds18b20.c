/**
 * Copyright (c) 2018, Łukasz Marcin Podkalicki <lpodkalicki@gmail.com>
 *
 * This is tinyAVR (ATtiny13, ATtiny25, ATtiny45, ATtiny85) library for DS18B20 temperature sensor.
 */

#include "main.h"
#include <util/crc16.h>

static TSDS18x20 DS18x20;
static TSDS18x20 *pDS18x20 = &DS18x20;

static uint8_t OWReset(TSDS18x20 *pDS18x20);
static void OWWriteBit(TSDS18x20 *pDS18x20, uint8_t bit);
static uint8_t OWReadBit(TSDS18x20 *pDS18x20);
static void OWWriteByte(TSDS18x20 *pDS18x20, uint8_t value);
static uint8_t OWReadByte(TSDS18x20 *pDS18x20);
static uint8_t OWCheckCRC(uint8_t *data, uint8_t length);
static uint8_t DS18x20_Init(TSDS18x20 *pDS18x20,volatile uint8_t *DS18x20_PORT,uint8_t DS18x20_PIN);
static uint8_t DS18x20_ReadROM(TSDS18x20 *pDS18x20);
static uint8_t DS18x20_MeasureTemperature(TSDS18x20 *pDS18x20);
static uint8_t DS18x20_ReadScratchPad(TSDS18x20 *pDS18x20);
static void DS18x20_SetResolution(TSDS18x20 *pDS18x20, uint8_t CONF_REG);
static void DS18x20_WriteScratchpad(TSDS18x20 *pDS18x20);
static int8_t DS18x20_TemperatureValue(TSDS18x20 *pDS18x20);
///*************************************************************************************************
static uint8_t OWReset(TSDS18x20 *pDS18x20)
{
    uint8_t result;

    *(pDS18x20->DS18x20_PORT-1) |= _BV(pDS18x20->DS18x20_PIN);
    _delay_us(480);

    *(pDS18x20->DS18x20_PORT-1) &= ~(_BV(pDS18x20->DS18x20_PIN));
    _delay_us(80);

    result = (*(pDS18x20->DS18x20_PORT-2)) & (_BV(pDS18x20->DS18x20_PIN));

    _delay_us(400);

    return result;
}

/* Send a bit over 1-Wire interface. */
static void OWWriteBit(TSDS18x20 *pDS18x20, uint8_t bit)
{
    *(pDS18x20->DS18x20_PORT-1) |= _BV(pDS18x20->DS18x20_PIN);

    if (bit)
    {
        //Wite 1
        _delay_us(15);
        *(pDS18x20->DS18x20_PORT-1) &= ~(_BV(pDS18x20->DS18x20_PIN));
        _delay_us(45);
    }
    else
    {
        //Write 0
        _delay_us(60);
        *(pDS18x20->DS18x20_PORT-1) &= ~(_BV(pDS18x20->DS18x20_PIN));
    }

    return;
}

/* Read a bit from the 1-Wire interface. */
static uint8_t OWReadBit(TSDS18x20 *pDS18x20)
{
    uint8_t result=0;

    *(pDS18x20->DS18x20_PORT-1) |= _BV(pDS18x20->DS18x20_PIN);
    _delay_us(4);

    *(pDS18x20->DS18x20_PORT-1) &= ~(_BV(pDS18x20->DS18x20_PIN));
    _delay_us(8);

    if ((*(pDS18x20->DS18x20_PORT-2) & (_BV(pDS18x20->DS18x20_PIN))))
        result=1;

    _delay_us(48);

    return result;
}

/* Send a byte over 1-Wire interface starting with most significant bit. */
static void OWWriteByte(TSDS18x20 *pDS18x20, uint8_t value)
{
    uint8_t mask = 0x01;

    while (mask)
    {
        OWWriteBit(pDS18x20,value & mask);
        mask <<= 1;
    }
}

/* Read a byte form the 1-Wire interface. */
static uint8_t OWReadByte(TSDS18x20 *pDS18x20)
{
    uint8_t data = 0x00;
    uint8_t mask = 0x01;

    while(mask)
    {
        if (OWReadBit(pDS18x20))
            data |= mask;
        mask <<= 1;
    }

    return data;
}

/* Calculates CRC value based on the example in Avr-libc reference manual. */
static uint8_t OWCheckCRC(uint8_t *data, uint8_t length)
{
    uint8_t i;
    uint8_t CRCvalue=0;

    for(i=0; i<length; i++)
        CRCvalue=_crc_ibutton_update(CRCvalue,*(data+i));

    return CRCvalue;
}

/* Init function for DS18S20. */
static uint8_t DS18x20_Init(TSDS18x20 *pDS18x20,volatile uint8_t *DS18x20_PORT,uint8_t DS18x20_PIN)
{
    //Init ports/pins to which DS18S20 is attached.
    pDS18x20->DS18x20_PORT = DS18x20_PORT;
    pDS18x20->DS18x20_PIN = DS18x20_PIN;

    //Set DS18S20 PIN as input one and PORT bit to 0
    *(pDS18x20->DS18x20_PORT) &= ~(_BV(pDS18x20->DS18x20_PIN));
    *(pDS18x20->DS18x20_PORT-1) &= ~(_BV(pDS18x20->DS18x20_PIN));

    if (OWReset(pDS18x20))
        return 1;

    //Check DS18x20 sensor type by reading the family code from the ROM code
    if (!DS18x20_ReadROM(pDS18x20))
        return 1;

    switch(pDS18x20->serialNumber[0])
    {
    case 0x28:
    {
        pDS18x20->SensorModel=DS18B20Sensor;
        // Set the resolution to its default value
        pDS18x20->scratchpad[4]=CONF_RES_9b;
    }
    break;
    case 0x10:
        pDS18x20->SensorModel=DS18S20Sensor;
        break;
    default:
        return 1;
    }

    return 0;
}


/* Reads DS18S20 64-bit ROM code without using the Search ROM procedure. */
static uint8_t DS18x20_ReadROM(TSDS18x20 *pDS18x20)
{
    uint8_t i;

    OWWriteByte(pDS18x20,READ_ROM);

    for(i=0; i<8; i++)
        pDS18x20->serialNumber[i]=OWReadByte(pDS18x20);

    if (OWCheckCRC(pDS18x20->serialNumber,7)==pDS18x20->serialNumber[7])
        return 1;
    else
        return 0;
}

/* This functions initiates a single temperature conversion and reads sensors scratchpad. */
static uint8_t DS18x20_MeasureTemperature(TSDS18x20 *pDS18x20)
{
    OWReset(pDS18x20);
    OWWriteByte(pDS18x20,SKIP_ROM);
    OWWriteByte(pDS18x20,CONVERT_T);

    switch(pDS18x20->scratchpad[4])
    {
    case CONF_RES_9b:
        _delay_ms(94);
        break;
    case CONF_RES_10b:
        _delay_ms(188);
        break;
    case CONF_RES_11b:
        _delay_ms(375);
        break;
    default:
        _delay_ms(750);
    }

    return (DS18x20_ReadScratchPad(pDS18x20));
}

/* Read the content of DS18S20 scratchpad and check the integrity with CRC. */
static uint8_t DS18x20_ReadScratchPad(TSDS18x20 *pDS18x20)
{
    uint8_t i;

    OWReset(pDS18x20);
    OWWriteByte(pDS18x20,SKIP_ROM);
    OWWriteByte(pDS18x20,READ_SCRATCHPAD);

    for(i=0; i<9; i++)
        pDS18x20->scratchpad[i]=OWReadByte(pDS18x20);

    if (OWCheckCRC(pDS18x20->scratchpad,8)==pDS18x20->scratchpad[8])
        return 1;
    else
        return 0;
}

/* Defines the resolution of the sensor. Doesn't have any effect on DS18S20. */
static void DS18x20_SetResolution(TSDS18x20 *pDS18x20, uint8_t CONF_REG)
{
    if (pDS18x20->SensorModel==DS18B20Sensor)
        pDS18x20->scratchpad[4]=CONF_REG;

    return;
}

/* Writes 2 bytes of data to the SA18S20 scratchpad (TH and TL registers).*/
static void DS18x20_WriteScratchpad(TSDS18x20 *pDS18x20)
{
    OWReset(pDS18x20);
    OWWriteByte(pDS18x20,SKIP_ROM);
    OWWriteByte(pDS18x20,WRITE_SCRATCHPAD);

    OWWriteByte(pDS18x20,pDS18x20->scratchpad[2]);
    OWWriteByte(pDS18x20,pDS18x20->scratchpad[3]);
    if (pDS18x20->SensorModel == DS18B20Sensor)
        OWWriteByte(pDS18x20,pDS18x20->scratchpad[4]);

    return;
}

/* Convert temperature reading to a floating point number. */
static int8_t DS18x20_TemperatureValue(TSDS18x20 *pDS18x20)
{
    int16_t Temperature=0;
    int8_t sign=1;

    Temperature = pDS18x20->scratchpad[1];
    Temperature <<= 8;
    Temperature |= pDS18x20->scratchpad[0];

    if (Temperature<0)
        sign=-1;

    if (pDS18x20->SensorModel==DS18S20Sensor)
        return Temperature * 0.5;

    switch(pDS18x20->scratchpad[4])
    {
    case CONF_RES_10b:
        return sign*(abs(Temperature)/4) * 0.25;
        break;
    case CONF_RES_11b:
        return sign*(abs(Temperature)/2) * 0.125;
    case CONF_RES_12b:
        return Temperature * 0.0625;
        break;
    case CONF_RES_9b:
        return (int8_t)(sign*(abs(Temperature)*0.125) * 0.5);
        break;
    default:
        return 0.0;
    }
}

void init_ds18b20(void)
{
    ///        // Init DS18B20 sensor
    for(uint8_t i = 0; i <= 5; i++)
    {
        if(i!=0)
            delay_ms(1000);
        if (DS18x20_Init(pDS18x20,&DS18B20_PORT,DS18B20_SENSOR))
        {
            softuart_puts_p(PSTR("Error!!! Can not find 1-Wire device!"));
        }
        else
        {
            softuart_puts_p(PSTR("1-Wire device detected."));
            // Set DS18B20 resolution to 9 bits.
            DS18x20_SetResolution(pDS18x20,CONF_RES_9b);
            DS18x20_WriteScratchpad(pDS18x20);
            break;
        }
    }
}

//**************************************************************************
int8_t get_18b20(void)
{
    for(uint8_t i = 0; i <= 3; i++)
    {
        if (DS18x20_MeasureTemperature(pDS18x20))
        {
            return DS18x20_TemperatureValue(pDS18x20);
        }
        else
        {
            init_ds18b20();
        }
    }
    return 0;
}
