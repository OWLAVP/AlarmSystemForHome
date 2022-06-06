#include "main.h"

static uint32_t time_without_power_s=0;

uint8_t ADC_SINGLE_INIT=0;// This variable is use to ensure only one execution of the ADC_INIT()

static void ADC_PRESCALER_SET(uint8_t prescaler);
static void ADC_voltage_Reference(uint8_t Reference_Voltage);
static char is_external_pwr(void);
//################################################################################################################################################################################
//==============================================================|   ADC_INIT() Start	|=========================================================================================
//################################################################################################################################################################################

//	------------possible parameter---------
//	prescaler = 2,4,8,16,32,64,128
//  Voltage_Reference = AREF,AVCC,INTERNAL,INTERNAL1v1,INTERNAL2v56

void ADC_INIT(uint8_t prescaler, uint8_t Voltage_Reference)
{
    ADC_SINGLE_INIT=0; //when ADC_INIT() call by user so we dont need to again execute ADC_INIT() inside the analogRead()


    ADCSRA |= (1<<ADEN); // ADC Enable
    ADC_PRESCALER_SET(prescaler); // set sampling frequency by prescaler bit
    ADC_voltage_Reference(Voltage_Reference); //set voltage reference //by default AVCC has been selected
    ADMUX &= ~(1<<ADLAR); //left adjust off // Though by default left adjust off // this line use for more convenient for future developer
    //ADMUX |= (1 << ADLAR);//8  bit


    #if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny24A__)\
    || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny44A__)\
    || defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny84A__)
    ADCSRA |= (1<<ADEN); // ADC Enable
    ADC_PRESCALER_SET(prescaler); // set sampling frequency by prescaler bit
    ADC_voltage_Reference(Voltage_Reference); //set voltage reference //by default AVCC has been selected
    ADCSRB &= ~(1<<ADLAR); //left adjust off // Though by default left adjust off // this line use for more convenient for future developer
    #endif
}
//################################################################################################################################################################################
//==============================================================|   ADC_INIT() End	|=============================================================================================
//################################################################################################################################################################################



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//==========================================================|   ADC_PRESCALER_SET() Start	|=====================================================================================
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// This function selects sampling frequency of ADC by selecting the prescaler bit
// if we set ADC Prescaler 128 means (Division Factor 128 to maintain ADC sampling frequency below 250KHz)(16MHz/128=125KHz)
static void ADC_PRESCALER_SET(uint8_t prescaler)
{
    #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega16A__) || defined(__AVR_ATmega16P__) || defined(__AVR_ATmega16PA__)\
    || defined(__AVR_ATmega32__) || defined(__AVR_ATmega32A__)\
    || defined(__AVR_ATmega164__) || defined(__AVR_ATmega164A__) || defined(__AVR_ATmega164P__) || defined(__AVR_ATmega164PA__)\
    || defined(__AVR_ATmega324__) || defined(__AVR_ATmega324A__) || defined(__AVR_ATmega324P__) || defined(__AVR_ATmega324PA__)\
    || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)\
    || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__)\
    || defined(__AVR_ATmega8535__)\
    || defined(__AVR_ATmega8__)\
    || defined(__AVR_ATmega48__) || defined(__AVR_ATmega48A__) || defined(__AVR_ATmega48P__) || defined(__AVR_ATmega48PA__) || defined(__AVR_ATmega48PB__)\
    || defined(__AVR_ATmega88__) || defined(__AVR_ATmega88A__) || defined(__AVR_ATmega88P__) || defined(__AVR_ATmega88PA__) || defined(__AVR_ATmega88PB__)\
    || defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega168A__) || defined(__AVR_ATmega168PA__) || defined(__AVR_ATmega168PB__)\
    || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)\
    || defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny24A__)\
    || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny44A__)\
    || defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny84A__) || defined(__AVR_ATtiny85__)
    ADCSRA &= (~(1<<ADPS2)) & (~(1<<ADPS1)) & (~(1<<ADPS0)) ; // clear ADPS2:ADPS0 for initialization

    switch(prescaler)
    {
    case 0:
        ADCSRA |= (1<<ADPS0);//-----------------------------------  0 0 0 --------------------- 2
        break;
    case 2:
        ADCSRA |= (1<<ADPS0);//-----------------------------------  0 0 1 --------------------- 2
        break;
    case 4:
        ADCSRA |= (1<<ADPS1);//-----------------------------------  0 1 0 --------------------- 4
        break;
    case 8:
        ADCSRA |= (1<<ADPS1) | (1<<ADPS0);//----------------------  0 1 1 --------------------- 8
        break;
    case 16:
        ADCSRA |= (1<<ADPS2);//----------------------------------  1 0 0 --------------------- 16
        break;
    case 32:
        ADCSRA |= (1<<ADPS2) | (1<<ADPS0);//---------------------  1 0 1 --------------------- 32
        break;
    case 64:
        ADCSRA |= (1<<ADPS2) | (1<<ADPS1);//---------------------  1 1 0 --------------------- 64
        break;
    case 128:
        ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);//-------  1 1 1 --------------------- 124
        break;
    default:
        ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);//----------1 1 1 --------------------- 128
    }
    #endif
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//==========================================================|   ADC_PRESCALER_SET() End  |=======================================================================================
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//======================================================|   ADC_voltage_Reference() Start  |======================================================================================
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//This function selects the voltage reference
//by default AVCC has been selected
static void ADC_voltage_Reference(uint8_t Reference_Voltage)
{
    #if defined(__AVR_ATmega16__) || defined(__AVR_ATmega16A__) || defined(__AVR_ATmega16P__) || defined(__AVR_ATmega16PA__)\
    || defined(__AVR_ATmega32__) || defined(__AVR_ATmega32A__)\
    || defined(__AVR_ATmega8535__)\
    || defined(__AVR_ATmega8__)\
    || defined(__AVR_ATmega48__) || defined(__AVR_ATmega48A__) || defined(__AVR_ATmega48P__) || defined(__AVR_ATmega48PA__) || defined(__AVR_ATmega48PB__)\
    || defined(__AVR_ATmega88__) || defined(__AVR_ATmega88A__) || defined(__AVR_ATmega88P__) || defined(__AVR_ATmega88PA__) || defined(__AVR_ATmega88PB__)\
    || defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega168A__) || defined(__AVR_ATmega168PA__) || defined(__AVR_ATmega168PB__)\
    || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
    //----------------INTERNAL---------------
    //-------------0 1(reserved)-------------
    //ATmega16=2.56v           ATmega48=1.1v
    //ATmega32=2.56v           ATmega88=1.1v
    //ATmega8535=2.56v         ATmega168=1.1v
    //ATmega8=2.56v            ATmega328=1.1v

    if(Reference_Voltage==AREF)
    {
        ADMUX &= (~(1<<REFS1)) & (~(1<<REFS0));   //------------------------ 0 0 -------- AREF
    }
    else if (Reference_Voltage==AVCC)
    {
        ADMUX &= (~(1<<REFS1));    //-------------- 0 1 -------- AVCC
        ADMUX |= (1<<REFS0);
    }
    else if (Reference_Voltage==INTERNAL)
    {
        ADMUX |= /*(1<<REFS1) | */(1<<REFS0);   //-------------------- 1 1 -------- INTERNAL 2.56v or 1.1v and (1 0 reserved)
    }
    else
    {
        ADMUX &= (~(1<<REFS1));    //------------------------------------------- 0 1 -------- AVCC
        ADMUX |= (1<<REFS0);
    }
    #endif



    #if defined(__AVR_ATmega164__) || defined(__AVR_ATmega164A__) || defined(__AVR_ATmega164P__) || defined(__AVR_ATmega164PA__)\
    || defined(__AVR_ATmega324__) || defined(__AVR_ATmega324A__) || defined(__AVR_ATmega324P__) || defined(__AVR_ATmega324PA__)\
    || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)\
    || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__)
    //-----DUAL(1.1v & 2.56v) mode internal voltage reference supported
    //-------------INTERNAL1v1---------------INTERNAL2v56--------------
    //INTERNAL means by default 2.56v selected

    if(Reference_Voltage==AREF)
    {
        ADMUX &= (~(1<<REFS1)) & (~(1<<REFS0));   //------------------------ 0 0 -------- AREF
    }
    else if (Reference_Voltage==AVCC)
    {
        ADMUX &= (~(1<<REFS1));    //-------------- 0 1 -------- AVCC
        ADMUX |= (1<<REFS0);
    }
    else if (Reference_Voltage==INTERNAL1v1)
    {
        ADMUX |= (1<<REFS1);    //------- 1 0 -------- INTERNAL1v1----- 1.1v
        ADMUX &= (~(1<<REFS0));
    }
    else if (Reference_Voltage==INTERNAL2v56)
    {
        ADMUX |= (1<<REFS1) | (1<<REFS0);   //---------------- 1 1 -------- INTERNAL2v56---- 2.56v
    }
    else if (Reference_Voltage==INTERNAL)
    {
        ADMUX |= (1<<REFS1) | (1<<REFS0);   //-------------------- 1 1 -------- INTERNAL-------- 2.56v
    }
    else
    {
        ADMUX &= (~(1<<REFS1));    //------------------------------------------- 0 1 -------- AVCC
        ADMUX |= (1<<REFS0);
    }
    #endif



    #if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny24A__)\
    || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny44A__)\
    || defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny84A__)
    //----------------INTERNAL---------------
    //-------------1 1(reserved)-------------
    //ATtiny24=1.1v
    //ATtiny44=1.1v
    //ATtiny84=1.1v

    //NB:ATmega24,44,84:
    //	 when external(AREF) will be selected as reference .please avoid A0 channel for ADC read because A0 use as external(AREF) reference pin

    if(Reference_Voltage==AVCC)
    {
        ADMUX &= (~(1<<REFS1)) & (~(1<<REFS0));   //---------------------- 0 0 -------- AVCC
    }
    else if (Reference_Voltage==AREF)
    {
        ADMUX &= (~(1<<REFS1));    //-------------- 0 1 -------- AREF
        ADMUX |= (1<<REFS0);
    }
    else if (Reference_Voltage==INTERNAL)
    {
        ADMUX |= (1<<REFS1);    //------------ 1 0 -------- INTERNAL---------1.1v
        ADMUX &= ~(1<<REFS0);
    }
    else
    {
        ADMUX &= (~(1<<REFS1)) & (~(1<<REFS0));   //----------------------------------------------- 0 1 -------- AVCC
    }
    #endif
    #if defined(__AVR_ATtiny85__)
    //if (Reference_Voltage==INTERNAL){ADMUX |= (1 << REFS1) | (1 << REFS0);}
    #endif
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//======================================================|   ADC_voltage_Reference() End  |=======================================================================================
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//################################################################################################################################################################################
//===========================================================|   analogRead() Start	|=============================================================================================
//################################################################################################################################################################################
uint16_t analogRead(uint8_t input)
{
    ADCSRA |= (1<<ADEN);
    ADMUX &=  (~(1<<MUX3)) & (~(1<<MUX2)) & (~(1<<MUX1)) & (~(1<<MUX0));
    // Select the ADC reference voltage and channel. Must be done everytime because ADMUX can be overwritten in meantime
    switch(input)
    {
    case ANALOG0:
        ADMUX &=  (~(1<<MUX3)) & (~(1<<MUX2)) & (~(1<<MUX1)) & (~(1<<MUX0)); //---------- 0 0 0 0 0
        break;
    case ANALOG1:
        ADMUX |= (1<<MUX0);//-------------------------------------------------------------------------- 0 0 0 0 1
        break;
    case ANALOG2:
        ADMUX |= (1<<MUX1);//-------------------------------------------------------------------------- 0 0 0 1 0
        break;
    case ANALOG3:
        ADMUX |= (1<<MUX1) | (1<<MUX0);//-------------------------------------------------------------- 0 0 0 1 1
        break;
    case ANALOG4:
        ADMUX |= (1<<MUX2);//-------------------------------------------------------------------------- 0 0 1 0 0
        break;
    case ANALOG5:
        ADMUX |= (1<<MUX2) | (1<<MUX0);//-------------------------------------------------------------- 0 0 1 0 1
        break;
    case ANALOG6:
        ADMUX |= (1<<MUX2) | (1<<MUX1);//-------------------------------------------------------------- 0 0 1 1 0
        break;
    case ANALOG7:
        ADMUX |= (1<<MUX2) | (1<<MUX1) | (1<<MUX0);//-------------------------------------------------- 0 0 1 1 1
        break;
    case ANALOG8:
        ADMUX |= (1<<MUX3);         // ADC channel 8 - intelnal temperature sensor
        break;
    default:
        ADMUX &= (~(1<<MUX3)) & (~(1<<MUX2)) & (~(1<<MUX1)) & (~(1<<MUX0)); // set channel ADC0 default
    }
    ADCSRA |= (1 << ADSC);  // Start converting

    while((ADCSRA & (1<<ADIF)) == 0) {};

//    // Shut down the ADC to save power
    ADCSRA &= ~(1 << ADEN);

    return ADCW;
}
//################################################################################################################################################################################
//==============================================================|   analogRead() End	|=============================================================================================
//################################################################################################################################################################################
//**************************************************************************
int8_t get_int_temperature(void)
{
    return (int8_t)((get_filtered_adc(INT_TEMP) + CHIP_TEMP_ERROR_OFFSET) - 273);
}
//*******************************************************************************************************************

static char is_external_pwr(void)
{
    if(get_filtered_adc(BAT_LEVEL) > 600)
        return 1;
    else
        return 0;
}

//*******************************************************************************************************************

uint16_t get_filtered_adc(uint8_t pin)
{
    static uint16_t buf[3];
    buf[0] = analogRead(pin);
    buf[1] = analogRead(pin);
    buf[2] = analogRead(pin);
    uint16_t middle;
    if ((buf[0] <= buf[1]) && (buf[0] <= buf[2]))
    {
        middle = (buf[1] <= buf[2]) ? buf[1] : buf[2];
    }
    else
    {
        if ((buf[1] <= buf[0]) && (buf[1] <= buf[2]))
        {
            middle = (buf[0] <= buf[2]) ? buf[0] : buf[2];
        }
        else
        {
            middle = (buf[0] <= buf[1]) ? buf[0] : buf[1];
        }
    }
    return middle;
}

//*******************************************************************************************************************

void check_power(void)
{
    if(is_external_pwr())
        time_without_power_s = 0;
    else
        time_without_power_s++;
}
//*******************************************************************************************************************

void power_control(void)
{
    static uint32_t time_stamp_on = 0;
    static uint32_t time_stamp_off = 0;
//    static uint32_t time_stamp_bat = 0;
    static uint32_t my_timer = 0;
    static uint8_t report_on = 0;
    static uint8_t report_off = 0;
    //static uint8_t report_bat = 0;
    static uint8_t flag_time_on = 1;
    static uint8_t flag_time_off = 1;
    static uint8_t flag_guard = 0;

    if (get_time_ms() - my_timer >= 1000)
    {
        my_timer = get_time_ms();
        check_power();
    }

    if(time_without_power_s == 0)   // power+++
    {
        if(get_interval_power_on_report() != 0)
        {
            if(flag_guard == 0)
            {
                flag_guard = 1;
                //set_local_guard(1);
                delay_s(10);    //для инициализации датчиков
//                softuart_puts_p(PSTR("power +++."));
            }
            if(flag_time_on == 0)
            {
                time_stamp_on = get_time_s();
                flag_time_on = 1;
                report_on = 1;
            }
            if(report_on == 1)
            {
                if(flag_time_on == 1 && (time_stamp_on +  + (uint32_t)60*get_interval_power_on_report()) < get_time_s())
                {
                    report_on = 0;
                    flag_time_off = 1;
                    send_sms_admins("External power is on.");
                }
            }
        }
        return;
    }
    if(get_interval_power_off_report() != 0)  ///  power---
    {
        time_stamp_off = get_val(time_without_power_s);
        time_stamp_on = get_time_s();
        if(flag_guard == 1)
        {
            flag_guard = 0;
            //set_guard(0);
            //config.local_guard = 0;
//            softuart_puts_p(PSTR("power ---"));
        }

        if(flag_time_off == 1 && time_stamp_off > (uint32_t)60*get_interval_power_off_report())
        {
            report_off = 1;
        }
        if(report_off == 1)
        {
            report_off = 0;
            flag_time_on = 0;
            flag_time_off = 0;
            //report_bat = 1;
//            time_stamp_bat = get_time_s();
            send_sms_admins("External power is off.");
        }
//        if(report_bat == 1 && (get_time_s() > (time_stamp_bat + 60)))
//        {
//            softuart_puts_p(PSTR("check battery"));
//            time_stamp_bat = get_time_s();
//            if(get_battery_level() < 25)
//            {
//                report_bat = 0;
//                send_sms_admins("Battery LOW!!!");
//            }
//        }
    }
}

//*****************************************************************************
uint32_t get_time_without_power_s(void)
{
    return get_val(time_without_power_s);
}

//ISR(ADC_vect)
//{}
