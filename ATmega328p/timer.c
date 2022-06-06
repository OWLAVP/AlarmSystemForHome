#include "main.h"

static uint32_t time_ms=0;
static uint32_t time_s=0;
static uint32_t time_m=0;
static uint8_t buz_on = 0;
static uint16_t buz_counter = 0;
static uint8_t beep_on = 0;
static uint16_t beep_counter = 0;
static uint16_t soft_wdt = 0;
static uint16_t reset_s_wdt = 0;
static volatile uint32_t time_from_motion_s=0; // таймер с момента последнего зафиксированного движения
static uint16_t time_from_button_s=0xFFFF; // таймер с момента последнего нажатия на кнопку

static void inc_time_from_motion(void);

//*******************************************************************************************************************
void timer0_init(void)
{
    // Clear registers
    TCCR0A = 0;
    TCCR0B = 0;
    TCNT0 = 0;

    // 1506.0240963855422 Hz (16000000/((165+1)*64))
    OCR0A = 165;
    // CTC
    TCCR0A |= (1 << WGM01);
    // Prescaler 64
    TCCR0B |= (1 << CS01) | (1 << CS00);
//  // 1250 Hz (8000000/((24+1)*256))
//  OCR0A = 24;
//  // CTC
//  TCCR0A |= (1 << WGM01);
//  // Prescaler 256
//  TCCR0B |= (1 << CS02);

//    // 1506.0240963855422 Hz (8000000/((82+1)*64))
//  OCR0A = 82;
//  // CTC
//  TCCR0A |= (1 << WGM01);
//  // Prescaler 64
//  TCCR0B |= (1 << CS01) | (1 << CS00);

    // 1506.0240963855422 Hz (1000000/((82+1)*8))
//  OCR0A = 82;
//  // CTC
//  TCCR0A |= (1 << WGM01);
//  // Prescaler 8
//  TCCR0B |= (1 << CS01);
}

// период переполнения 10 мс
void timer1_init(void)
{
    cli(); // stop interrupts
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;

    // 100 Hz (16000000/((624+1)*256))
    OCR1A = 624;
    // CTC
    TCCR1B |= (1 << WGM12);
    // Prescaler 256
    TCCR1B |= (1 << CS12);
    // Output Compare Match A Interrupt Enable
    TIMSK1 |= (1 << OCIE1A);

//  // 100 Hz (8000000/((1249+1)*64))
//  OCR1A = 1249;
//  // CTC
//  TCCR1B |= (1 << WGM12);
//  // Prescaler 64
//  TCCR1B |= (1 << CS11) | (1 << CS10);
//  // Output Compare Match A Interrupt Enable
//  TIMSK1 |= (1 << OCIE1A);

//  // 100 Hz (1000000/((1249+1)*8))
//  OCR1A = 1249;
//  // CTC
//  TCCR1B |= (1 << WGM12);
//  // Prescaler 8
//  TCCR1B |= (1 << CS11);
//  // Output Compare Match A Interrupt Enable
//  TIMSK1 |= (1 << OCIE1A);
}

//*******************************************************************************************************************
void buzzer_stop(void)
{
    buz_on = 0;
    buz_counter = 0;
    beep_on = 0;
    DigitalWrite(PIN_BUZZER, LOW);
}
//***************************************************************
void buzzer_non_block(uint16_t time_to_buz)
{
    cli();
    beep_on = 1;
    beep_counter = time_to_buz;
    start_buzzer(0);
    sei();
}
//*******************************************************************************************************************

static void buzzer_control(void);
static void buzzer_control(void)
{
    if(beep_on)
    {
        if(beep_counter >= 10)
            beep_counter -= 10;
        else
        {
            beep_counter = 0;
            beep_on = 0;
            stop_buzzer();
        }
    }
}
//**********************************************************
void beep(void)
{
    char tsreg=SREG;
    cli();
    TIMSK0 |= (1 << OCIE0A);
    SREG = tsreg;
    sei();
}

//*******************************************************************************************************************
ISR(TIMER0_COMPA_vect)
{
    static uint8_t i = 0;
    DigitalLevelToggle(PIN_BEEP);
    i++;
    if(i > 100)
    {
        DigitalWrite(PIN_BEEP, LOW);
        i = 0;
        TIMSK0 &= ~(1<< OCIE0A);
    }
}
// прерывание вызывается с периодом 10 мс
ISR(TIMER1_COMPA_vect)
{
    static uint8_t i=0;
    static uint8_t j=0;
    led_management();
    led_gsm();
    check_motion();
    check_button();
    check_single_sensor();
    buzzer_control();
    time_ms+=10;
    i++;
    if(i>=100) // 1 секунда
    {
        i=0;
        time_s++;
        //time_from_start_s++;
        if(time_from_button_s < 0xFFFF)
            time_from_button_s++;
        soft_wdt++;
        j++;
        if(j>=60)
        {
            j=0;
            time_m++;
        }
        if(soft_wdt > 600) // 1200 секунд 20 минут
        {
            if(reset_s_wdt < 0xFFFF)
                reset_s_wdt++;
            reset_mcu(1);
        }
        //check_power();
        inc_time_from_motion();
        if(get_guard() == 1 || (time_from_button_s < 0xFFFF))
        {
            DigitalLevelToggle(RED_LED);
        }

    }
    _WDT_RESET(); // сброс сторожевого таймера
}

//// управление светодиодами. Вызывается с периодом 10 мс
void led_management(void)
{
    static uint8_t i = 0;


    if(get_local_guard() == 1)
    {
        i++;
        if(i > 30) // every 300ms
        {
            DigitalLevelToggle(RED_LED);
            i = 0;
        }
    }
    else if(get_guard() == 0)
        DigitalWrite(RED_LED, LOW);
}
//*******************************************************************************************************************

uint32_t get_time_ms(void)
{
    return get_val(time_ms);
}
//*******************************************************************************************************************

uint32_t get_time_s(void)
{
    return get_val(time_s);
}

//*******************************************************************************************************************

uint32_t get_time_m(void)
{
    return get_val(time_m);
}

//*******************************************************************************************************************

void reset_soft_wdt(void)
{
    set_val(soft_wdt, 0);
}

//*******************************************************************************************************************
static void inc_time_from_motion(void)
{
    time_from_motion_s++; // счетчик переполнится через 136 лет
}

//*******************************************************************************************************************
// задержка на х милисекунд
void delay_ms(uint16_t delay)
{
    uint32_t time_stamp = get_time_ms() + delay;
    _SLEEP();
    while((get_time_ms() < time_stamp))
    {
        while(is_queue_not_empty()) // тем временем, проверяем входной буфер
            get_message_from_mdm();
        while(softuart_kbhit())// recv softuart
            get_message_from_openwrt();
    }
}

//*******************************************************************************************************************
// задержка на х секунд
void delay_s(uint8_t delay)
{
    uint32_t time_stamp = get_time_s() + delay;
    _SLEEP();
    while((get_time_s() < time_stamp))
    {
        while(is_queue_not_empty()) // тем временем, проверяем входной буфер
            get_message_from_mdm();
        while(softuart_kbhit())// recv softuart
            get_message_from_openwrt();
    }
}
//*******************************************************************************************************************
// звуковой сигнал
void start_buzzer(uint8_t mode)
{
    DigitalWrite(PIN_BUZZER, HIGH);
//   DigitalWrite(PIN_BUZZER2, HIGH);
}

//*******************************************************************************************************************
// звуковой сигнал
void stop_buzzer(void)
{
    DigitalWrite(PIN_BUZZER, LOW);
//    DigitalWrite(PIN_BUZZER2, LOW);
}

//*******************************************************************************************************************

//////////////////////////////////////////////////
void set_time_from_motion_s(uint8_t time)
{
    set_val(time_from_motion_s, time);
}

uint32_t get_time_from_motion_s()
{
    return get_val(time_from_motion_s);
}

//////////////////// time_from_button_s //////////////////////////
void set_time_from_button_s(uint16_t time)
{
    set_val(time_from_button_s, time);
}

uint16_t get_time_from_button_s()
{
    return get_val(time_from_button_s);
}

//*******************************************************************************************************************

uint8_t get_beep_on(void)
{
    return get_val(beep_on);
}
//*******************************************************************************


