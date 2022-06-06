#include "main.h"

static uint8_t flag_alarm = 0;
static uint8_t light = 0;
static uint8_t flag_for_send = 0;// flag alarm sending to router
static uint8_t flg_rst = 0; //flag update eeprom ant reset flag_for_send(flag alarm->web)
static int8_t temperature_18b20 = 0;
static int8_t temperature_mcu = 0;
static uint8_t flag_motion_inside = 0;

static void main_guard(void);
static void main_guard(void)
{
    static uint8_t flag_motion = 0;
    static uint8_t motion_cnt = 0;
    static uint32_t first_time_stamp = 0;
    uint8_t motion_d = get_motion_detected();
    char zone = get_zone_mask();
    if(motion_d && flag_alarm == 0)
    {
        if(get_first_m() == 0)
        {
            if(motion_cnt == 0)//if first motion - return
            {
                first_time_stamp = get_time_ms();// sensitivity 3 sec
                motion_cnt = 1;
//                softuart_puts_p(PSTR("main_f_motion"));
                return;
            }
        }
        else motion_cnt = 0;
        if(motion_cnt == 1 && (get_time_ms() < (first_time_stamp + 1000)))
        {
            return;
        }
        motion_cnt = 0;
        set_time_from_motion_s(0); // обнуляем таймер с момента последнего зафиксированного движения
        flag_motion = 1;
        buzzer_non_block(60000); //50sec
        if(motion_d & FRONT_SENSOR_MASK)
        {
            if(zone == 0xB0 || zone == 0xD0 || zone == 0x70)
            {
                set_zone_mask(0xF0);
                set_guard(0);
                EEPROM_update();
                flag_motion = 0;
            }
            else
            {
                set_zone_mask(0xE0);
            }
            flag_alarm = FLAG_ALARM_FRONT;
        }
        else if(motion_d & INSIDE_1_SENSOR_MASK)
        {
            if(zone == 0xE0 || zone == 0xB0 || zone == 0x70)
            {
                set_zone_mask(0xF0);
                set_guard(0);
                EEPROM_update();
                flag_motion = 0;
            }
            else
            {
                set_zone_mask(0xD0);
            }
            flag_alarm = FLAG_ALARM_COURTYARD;
        }
        else if(motion_d & INSIDE_2_SENSOR_MASK)
        {
            if(zone == 0xD0 || zone == 0xE0 || zone == 0x70)
            {
                set_zone_mask(0xF0);
                set_guard(0);
                flag_motion = 0;
                EEPROM_update();
            }
            else
            {
                set_zone_mask(0xB0);
            }
            flag_alarm = FLAG_ALARM_COOK_ROOM;
        }
        else if(motion_d & SIDE_SENSOR_MASK)
        {
            if(zone == 0xB0 || zone == 0xD0 || zone == 0xE0)
            {
                set_zone_mask(0xF0);
                set_guard(0);
                EEPROM_update();
                flag_motion = 0;
            }
            else
            {
                set_zone_mask(0x70);
            }
            flag_alarm = FLAG_ALARM_SIDE;
        }
    }
    if(motion_cnt == 1 && (get_time_ms() > (first_time_stamp + get_mtime_sens())))
    {
        //softuart_puts_p(PSTR("rst motion")); //in 3 sec 2 motions
        motion_cnt = 0;
        return;
    }
    if(flag_motion && ((get_time_from_motion_s() > (uint32_t)3600*get_interval_after_motion_m())))
    {
        flag_motion = 0;
        set_zone_mask(0xF0);
        //softuart_puts_p(PSTR("Guard is recovery"));
        flag_for_send = 0;
        if(get_accept_sms() == 1)
            send_sms_admins("Guard is recovery after motion timer.");
    }
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////
static void local_guard(void);
static void local_guard(void)
{
    static uint8_t motion_cnt = 0;
    uint8_t motion_d = get_motion_detected();
    static uint32_t first_time_stamp = 0;

    if(motion_d && flag_alarm == 0)
    {
        if(get_first_m() == 0)
        {
            if(motion_cnt == 0)//if first motion - return
            {
                first_time_stamp = get_time_ms();
                motion_cnt = 1;
                //uart_send_str("f_motion\r\n");
                //softuart_puts_p(PSTR("f_motion"));
                return;
            }
        }
        else motion_cnt = 0;
        if(motion_cnt == 1 && (get_time_ms() < (first_time_stamp + 1000)))
        {
            return;
        }
        set_time_from_motion_s(0);
        if((motion_d & INSIDE_1_SENSOR_MASK) || (motion_d & INSIDE_2_SENSOR_MASK))
        {
            flag_motion_inside = 1;
            motion_cnt = 0;
            //beep();
            //softuart_puts_p(PSTR("motion_inside"));
        }
        if(!flag_motion_inside)
        {
            buzzer_non_block(10000);
            flag_motion_inside = 1;
            if(motion_d & FRONT_SENSOR_MASK)
            {
                //softuart_puts_p(PSTR("FLAG_WARN_FRONT"));
                flag_alarm = FLAG_ALARM_FRONT;
                flag_motion_inside = FLAG_ALARM_FRONT;
            }
            if(motion_d & SIDE_SENSOR_MASK)
            {
               // softuart_puts_p(PSTR("FLAG_WARN_SIDE"));
                flag_alarm = FLAG_ALARM_SIDE;
                flag_motion_inside = FLAG_ALARM_SIDE;
            }
        }
    }
    if(motion_cnt == 1 && (get_time_ms() > (first_time_stamp + 7000)))
    {
        //softuart_puts_p(PSTR("rst motion")); //in 3 sec 2 motions
        motion_cnt = 0;
        return;
    }
    if(flag_motion_inside == 1 && get_time_from_motion_s() > TIMER_RST_LOCAL)
    {
        //softuart_puts_p(PSTR("flag_motion_inside=0"));
        beep();
        flag_motion_inside = 0;
        flag_for_send = 0;
    }
}

//*******************************************************************************************************************
static void single_sens(void);
static void single_sens(void)
{
    static uint8_t flag_motion = 0;
    static uint8_t motion_cnt = 0;
    static uint32_t first_time_stamp = 0;
    uint8_t motion_d = get_single_motion_detected();
    if(motion_d && flag_alarm == 0)
    {
        if(get_first_m() == 0)
        {
            if(motion_cnt == 0)//if first motion - return
            {
                first_time_stamp = get_time_ms();
                motion_cnt = 1;
                //set_time_from_motion_s(0); // обнуляем таймер с момента последнего зафиксированного движения
                //softuart_puts_p(PSTR("single_first_motion"));
                //ram_free();
                return;
            }
        }
        else motion_cnt = 0;
        if(motion_cnt == 1 && (get_time_ms() < (first_time_stamp + 2100)))
        {
            return;
        }
        if(flag_motion == 0)
        {
            flag_motion = 1;
            //buzzer_non_block(1000); //1sec
            beep();
            flag_alarm = FLAG_ALARM_SINGLE;
            //softuart_puts_p(PSTR("FLAG_ALARM_SINGLE"));
            //ram_free();
            //uart_send_str("FLAG_ALARM_SINGLE\r\n");
        }
        // send_state();
    }
    if(motion_cnt == 1 && (get_time_ms() > (first_time_stamp + 10000)))
    {
        //softuart_puts_p(PSTR("SINGLE rst motion")); //in 3 sec 2 motions
        //uart_send_str("SINGLE rst motion\r\n");
        motion_cnt = 0;
    }
    if(flag_motion && (get_time_ms() > (first_time_stamp + 300000)))
    {
        flag_motion = 0;
        beep();
        delay_ms(200);
        beep();
        flag_for_send = 0;
        //softuart_puts_p(PSTR("rst flag_motion"));
        //uart_send_str("rst flag_motion\r\n");
    }
    return;
}

//*******************************************************************************************************************

//*******************************************************************************************************************
static void send_alarm_signal_if_needed(void);
static void send_alarm_signal_if_needed(void)
{
    if(flag_alarm == 0)
    {
        return;
    }

    flag_for_send = flag_alarm;

    switch(flag_alarm)
    {
    case 0:
        break;
    case FLAG_ALARM_FRONT:
        alarm_play_file(AUDIO_ALARM_FRONT);
        //softuart_puts_p(PSTR("ALARM_FRONT"));
        break;
    case FLAG_ALARM_COURTYARD:
        alarm_play_file(AUDIO_ALARM_COURTYARD);
        break;
    case FLAG_ALARM_COOK_ROOM:
        alarm_play_file(AUDIO_ALARM_COOK_ROOM);
        break;
    case FLAG_ALARM_SIDE:
        alarm_play_file(AUDIO_ALARM_SIDE);
        break;
    case FLAG_ALARM_SMOKE:
        alarm_play_file(AUDIO_ALARM_SMOKE);
        break;
    case FLAG_ALARM_TEMPERATURE:
        alarm_play_file(AUDIO_ALARM_TEMPERATURE);
        break;
    case FLAG_ALARM_TEMPERATURE_INT:
        alarm_play_file(AUDIO_ALARM_TEMPERATURE);
        break;
    default:
        break;
    }
    flag_alarm = 0;
    set_guard(0);
    stop_buzzer();
}

//*******************************************************************************************************************
static void guard_timer(void);
static void guard_timer(void)
{
    if(!get_guard())
    {
        uint16_t t = get_time_from_button_s();
        if(!get_autoguard() && t!=0xFFFF)
        {
            set_autoguard(1);
            EEPROM_update();
        }
        if((t > 60*get_interval_after_button_m()) && (t != 0xFFFF))
        {
            set_guard(1);
            set_local_guard(0);
//            softuart_puts_p(PSTR("ON"));
            EEPROM_update();
            set_time_from_button_s(0xFFFF);
            beep();
            delay_ms(100);
            beep();
            if(get_accept_calls() == 1)
                alarm_play_file(AUDIO_GUARD_ON);
            if(get_accept_sms() == 1)
                send_sms_admins("Guard is on by button.");
        }
    }
}
//*******************************************************************************************************************
void check_guard(void)
{
    if(get_guard())
    {
        main_guard();
        send_alarm_signal_if_needed();
    }
    else if(get_local_guard())
    {
        local_guard();
        if(get_single_sensor() == 1)
            single_sens();
        send_alarm_signal_if_needed();
    }
    guard_timer();
}

//*******************************************************************************************************************

void send_sms_report(char *phone)
{
    uint32_t time;
    uint16_t d, h, m, s;
    char sms[160];
    char *ptr;
    send_USSD(0);
    time = get_time_s();
    d = time/(3600UL*24UL);
    time -= d*(3600UL*24UL);
    h = time/3600UL;
    time -= h*3600UL;
    m = time/60;
    s = time - m*60;
    ptr = sms;

    ptr += sprintf_P(ptr, PSTR("deposit=%s;"), get_arr_dep());

    ptr += sprintf_P(ptr, PSTR("t=%ud%uh%um%us;"), d, h, m, s);

    ptr += sprintf_P(ptr, PSTR("guard=%d;"), get_guard());

    ptr += sprintf_P(ptr, PSTR("l_g=%d;"), get_local_guard());

//    time = get_time_from_motion_s();
//    d = time/(3600UL*24UL);
//    time -= d*(3600UL*24UL);
//    h = time/3600UL;
//    time -= h*3600UL;
//    m = time/60;
//    s = time - m*60;
//    ptr += sprintf_P(ptr, PSTR("l_m=%ud%uh%um%us;"), d, h, m, s);

    ptr += sprintf_P(ptr, PSTR("temp=%dC;"), get_temperature_mcu());

    ptr += sprintf_P(ptr, PSTR("mcu=%u;"), get_reset_count_mcu());

    ptr += sprintf_P(ptr, PSTR("g_r=%u;"), get_error_code1());

    ptr += sprintf_P(ptr, PSTR("g=%hu;"), get_error_gsm());

    ptr += sprintf_P(ptr, PSTR("signal_gsm=%d%%;"), get_signal_strength());

    //ptr += sprintf_P(ptr, PSTR("b=%d%%;"), mdm_get_battery_level());

    uint32_t time_w_p = get_time_without_power_s();
    if(time_w_p == 0)
    {
        ptr += sprintf_P(ptr, PSTR("e_power=%u;"), 1);
    }
    else
    {
        ptr += sprintf_P(ptr, PSTR("battery=%d%%;"), get_battery_level());

    }
    send_sms(sms, phone);
}


//********************************************************************************************************************
void check_temperature_smoke(void)
{

    static uint32_t time_stamp_s = 0;
    static uint8_t flag = 0;
    if(flag == 1)
        return;
    if(get_time_s() > (time_stamp_s + INTERVAL_CHECK_TEMPERATURE_S))
    {
        time_stamp_s = get_time_s();
        temperature_18b20 = get_18b20();
        temperature_mcu = get_int_temperature();
        if(temperature_18b20 > 65)
        {
            flag = 1;
//            softuart_puts_p(PSTR("Alarm! TEMPERATURE!!!"));
            set_flag_alarm(FLAG_ALARM_TEMPERATURE);
        }
        if(temperature_mcu > 65)
        {
            flag = 1;
//            softuart_puts_p(PSTR("Alarm! Int TEMPERATURE!!!"));
            set_flag_alarm(FLAG_ALARM_TEMPERATURE_INT);
        }
    }
}

//*******************************************************************************************************************
void set_flag_alarm(uint8_t flag)
{
    flag_alarm = flag;
}
//*******************************************************************************************************************
uint8_t get_flag_alarm(void)
{
    return get_val(flag_alarm);
}

//*******************************************************************************************************************
int freeRam ()
{
    extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

//*******************************************************************************************************************
void ram_free(void)
{
    char ch[10];
    itoa(freeRam(), ch, 10);
    softuart_puts_ln(ch);
}

//**************************************************************************
void check_light(void)
{
    static uint32_t delay_light = 0;
    if(light == 0)
        return;
    else if(light == 1)
    {
        light = 2;
        delay_light = get_time_m() + 10;
    }
    else if(light == 2 && (get_time_m() > delay_light))
    {
        DigitalWrite(PIN_LIGHT, LOW);
        light = 0;
    }
}
void set_light(uint8_t state)
{
    if(state == 1)
    {
        DigitalWrite(PIN_LIGHT, HIGH);
    }
    else
    {
        DigitalWrite(PIN_LIGHT, LOW);
    }
    light = state;
}
uint8_t get_light(void)
{
    return get_val(light);
}
//*******************************************
uint8_t get_flag_for_send(void)
{
    return get_val(flag_for_send);
}
void set_flag_for_send(uint8_t flag)
{
    set_val(flag_for_send, flag);
}
//*******************************************
uint8_t get_flg_rst(void)
{
    return get_val(flg_rst);
}
void set_flg_rst(uint8_t flag)
{
    set_val(flg_rst, flag);
}
//*******************************************
int8_t get_temperature_18b20(void)
{
    return get_val(temperature_18b20);
}
//*******************************************
int8_t get_temperature_mcu(void)
{
    return get_val(temperature_mcu);
}
//*******************************************
uint8_t get_flag_motion_inside(void)
{
    return get_val(flag_motion_inside);
}
void set_flag_motion_inside(uint8_t flag)
{
    set_val(flag_motion_inside, flag);
}
//*******************************************
