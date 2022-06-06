#include "main.h"

static uint8_t motion_detected = 0;
static uint8_t single_motion_detected = 0;
static void sensor_port_init(void);
static void button_port_init(void);
static void led_port_init(void);
static char is_motion(void);
static uint8_t is_single_sensor(void);
static uint8_t is_btn_guard_pressed(void);
static uint8_t is_btn_guard_local_pressed(void);
//*******************************************************************************************************************
static void sensor_port_init(void)
{
    PinMode(SENSOR_1, INPUT);
    DigitalWrite(SENSOR_1, HIGH);
    PinMode(SENSOR_2, INPUT);
    DigitalWrite(SENSOR_2, HIGH);
    PinMode(SENSOR_3, INPUT);
    DigitalWrite(SENSOR_3, HIGH);
    PinMode(SENSOR_4, INPUT);
    DigitalWrite(SENSOR_4, HIGH);
    PinMode(SINGLE_SENSOR, INPUT);
    DigitalWrite(SINGLE_SENSOR, LOW);
//    DDRD &= ~0x30;
//	PORTD &= ~0x30;
}

//*******************************************************************************************************************
static void button_port_init(void)
{
// 	DDRC |= 1<<3;
// 	PORTC &= ~(1<<3);
    PinMode(BTN_GUARD, INPUT);
    DigitalWrite(BTN_GUARD, HIGH);
    PinMode(BTN_LOCAL_GUARD, INPUT);
    DigitalWrite(BTN_LOCAL_GUARD, HIGH);

}

//*******************************************************************************************************************
static void led_port_init(void)
{
    PinMode(RED_LED, OUTPUT);
    DigitalWrite(RED_LED, LOW);
    PinMode(GREEN_LED, OUTPUT);
    DigitalWrite(GREEN_LED, LOW);
}

//*******************************************************************************************************************
void port_init(void)
{
    //PORTB = 0;
    //DDRB = 1<<2; // hold FT232RL in reset

    //PORTA = 0; //disable SLEEP mode with RTS=0; DTR =0
    //DDRA =  (1<<7)|(1<<5);

    //PORTD = 0;
    //DDRD  = (1<<4)|(1<<1); // TXD, BUZ, LED

    //PORTC &= ~((1<<6)|(1<<7)); // реле выключены
    //DDRC |= (1<<6)|(1<<7); // порты управления реле
    PinMode(PIN_PWR_GSM, OUTPUT);
    DigitalWrite(PIN_PWR_GSM, LOW);

    PinMode(PIN_BUZZER, OUTPUT);
    DigitalWrite(PIN_BUZZER, LOW);

    PinMode(PIN_BEEP, OUTPUT);
    DigitalWrite(PIN_BEEP, LOW);
    PinMode(PIN_LIGHT, OUTPUT);
    DigitalWrite(PIN_LIGHT, LOW);
    //PORTC &= ~((1<<6)|(1<<7)); // реле выключены
    //DDRC |= (1<<6)|(1<<7); // порты управления реле
    sensor_port_init();
//	sensor_board_detect_port_init();
    led_port_init();
    button_port_init();
}
////*******************************************************************************************************************

////*******************************************************************************************************************
static char is_motion(void)
{
	//char state_char[25];
//	itoa(bit,state_char,10);
//	uart_send_str(state_char);
//	uart_send_str("\r\n");
    char bit = PORT_SENSOR;
    //bit = ~(bit << 4);
    return ((bit << 4) & get_zone_mask());
    //return ((bit << 4) ^ config.zone_mask);
}

/////////////////////////////////////
static uint8_t is_single_sensor(void)
{
    if(!DigitalRead(SINGLE_SENSOR))
        return 0;
    else
    {
        return 1;
    }
}
//***************************************************************************************
//// функция вызывается с периодом 10 мс
void check_single_sensor(void)
{
    static uint8_t cnt = 0;
    static uint8_t pause_count = 0;
    char motion;

    motion = is_single_sensor();
    if(motion && pause_count == 0)
    {
        cnt++;
        if(cnt >= 3)
        {
            single_motion_detected = 1;
            pause_count = 2;
            cnt = 0;
        }
    }
    else
    {
        cnt =0;

    }
    if(pause_count > 0)
    {
        pause_count--;

    }
    else
    {
        single_motion_detected = 0;
    }
}

uint8_t get_single_motion_detected(void)
{
    return single_motion_detected;
}
//// функция вызывается с периодом 10 мс
void check_motion(void)
{
    static uint8_t cnt = 0;
    static uint8_t pause_count = 0;
    char motion;

    motion = is_motion() & is_motion();
    motion = (motion >> 4);
    if(motion && pause_count == 0)
    {
        cnt++;
        if(cnt >= 3)
        {
            motion_detected = motion;
            pause_count = 2;
            cnt = 0;
        }
    }
    else
    {
        cnt =0;

    }
    if(pause_count > 0)
    {
        pause_count--;

    }
    else
    {
        motion_detected = 0;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t get_motion_detected()
{
//	            	char state_char[25];
//                itoa(motion_detected,state_char,10);
//                softuart_puts_ln(state_char);
//                uart_send_str(state_char);
//                uart_send_str("\r\n");
    uint8_t temp = motion_detected;
    motion_detected = 0;
    return temp;
    //return motion_detected;
}

///////////////////////////////////////////////////////
static uint8_t is_btn_guard_pressed(void)
{
    if(DigitalRead(BTN_GUARD))
        return 0;
    else{
        return 1;
    }

}
//*******************************************************************************************************************
static uint8_t is_btn_guard_local_pressed(void)
{
    if(DigitalRead(BTN_LOCAL_GUARD))
        return 0;
    else
    {
        return 1;
    }
}
////*******************************************************************************************************************
uint8_t is_btn_rst_pressed(void)
{
    if(DigitalRead(BTN_DEFAULT))
        return 0;
    else{
        return 1;
    }

}
//*******************************************************************************************************************

//// функция вызывается с периодом 10 мс
void check_button(void)
{
    static char press_button = 0;
    static char button_state = 0;

    if(is_btn_guard_pressed())
    {
        if(!get_guard())
        {
            if(press_button < 15)
                press_button++;
            else if(button_state == 0)
            {
                set_time_from_button_s(0);
                set_local_guard(0);
                set_zone_mask(0xF0);
                button_state = 1;
                set_flg_rst(1);
                EEPROM_update();
                beep();
            }
        }
        if(get_guard() || get_time_from_button_s() != 0xFFFF)
        {
            if(press_button < 15)
                press_button++;
            else if(button_state == 0)
            {
                set_guard(0);
                set_local_guard(0);
                set_time_from_button_s(0xFFFF);
                button_state = 1;
                set_flg_rst(1);
                EEPROM_update();
                beep();
            }
        }

    }
    else if(is_btn_guard_local_pressed())
    {
        set_time_from_button_s(0xFFFF);
        if(!get_local_guard())
        {
            if(press_button < 15)
                press_button++;
            else if(button_state == 0)
            {
                set_local_guard(1);
                set_guard(0);
                button_state = 1;
                set_flg_rst(1);
                EEPROM_update();
                beep();
            }
        }
        else
        {
            if(press_button < 15)
                press_button++;
            else if(button_state == 0)
            {
                set_local_guard(0);
                set_guard(0);
                set_flag_motion_inside(0);
                EEPROM_update();
                button_state = 1;
                set_flg_rst(1);
                beep();
            }
        }
    }
    else
    {
        press_button = 0;
        button_state = 0;
    }
}
//
////*******************************************************************************************************************






