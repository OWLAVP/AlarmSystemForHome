
#include "main.h"

config_t config;
static config_t *p_config = &config;
//*******************************************************************************************************************

void eeprom_read_config(char start)
{

    eeprom_read_block((void*)&config, 0, sizeof(config_t));
    if(config.first_usage != 0xAB)
    {
        memset(&config, 0, sizeof(config_t));
        softuart_puts_p(PSTR("eeprom_first_set"));
        config.interval_after_button_m = 3;
        config.interval_after_motion_m = 60;
        set_interval_power_off_report(1);  //minute
        set_interval_power_on_report(2); //minute
        config.autoguard = 1;
        config.zone_mask = 0xF0;
        config.mtime_sens = 7000;
//        config.mpause_count = 50;
        config.pause_alarm_report_s = 5;
        config.first_usage = 0xAB;
        memcpy_P(p_config->admin_phone[0], PSTR("+380713318090"), 13);
        eeprom_save_config();

    }
    if(start)
    {
        set_time_from_motion_s(config.time_from_motion_s);
    }
}

//*******************************************************************************************************************
void eeprom_save_config(void)
{
    //softuart_puts_p(PSTR("eeprom_save_config"));
    config.time_from_motion_s = get_time_from_motion_s();
    eeprom_write_block((void*)&config, 0, sizeof(config_t));
}

//*******************************************************************************************************************

void EEPROM_update(void)
{
    eeprom_update_block((void*)&config, 0, sizeof(config_t));
}

//******************************************************************
void set_accept_calls(uint8_t on_off)
{
    p_config->accept_calls = on_off;
}
uint8_t get_accept_calls(void)
{
   return p_config->accept_calls;
}
//**********************************************************************
void set_accept_sms(uint8_t on_off)
{
    p_config->accept_sms = on_off;
}
uint8_t get_accept_sms(void)
{
   return p_config->accept_sms;
}
//***********************************************
uint8_t get_interval_power_off_report(void)
{
    return p_config->interval_power_off_report_m;
}
void set_interval_power_off_report(uint8_t minute)
{
    p_config->interval_power_off_report_m = minute;
}
//***********************************************
uint8_t get_interval_power_on_report(void)
{
    return p_config->interval_power_on_report_m;
}
void set_interval_power_on_report(uint8_t minute)
{
    p_config->interval_power_on_report_m = minute;
}

//***********************************************
uint8_t get_guard(void)
{
    return p_config->guard;
}
void set_guard(uint8_t state)
{
    p_config->guard = state;
}

//***********************************************
uint8_t get_local_guard(void)
{
    return p_config->local_guard;
}
void set_local_guard(uint8_t state)
{
    p_config->local_guard = state;
}

//**********************************************************************
char* get_phones(uint8_t  i)
{
    char *ptr = p_config->admin_phone[i];
    return ptr;
}

//**********************************************************************
void set_reset_count_mcu(uint8_t cnt)
{
    p_config->reset_count_mcu = cnt;
}
uint8_t get_reset_count_mcu(void)
{
   return p_config->reset_count_mcu;
}
void inc_reset_count_mcu(void)
{
    p_config->reset_count_mcu++;
}

//***********************************************
uint8_t get_first_m(void)
{
    return p_config->first_m;
}
void set_first_m(uint8_t state)
{
    p_config->first_m = state;
}

//***********************************************
char get_zone_mask(void)
{
    return p_config->zone_mask;
}
void set_zone_mask(char state)
{
    p_config->zone_mask = state;
}

//***********************************************
uint16_t get_mtime_sens(void)
{
    return p_config->mtime_sens;
}
void set_mtime_sens(uint16_t time)
{
    p_config->mtime_sens = time;
}

//***********************************************
uint16_t get_interval_after_motion_m(void)
{
    return p_config->interval_after_motion_m;
}
void set_interval_after_motion_m(uint16_t time)
{
    p_config->interval_after_motion_m = time;
}

//***********************************************
uint8_t get_autoguard(void)
{
    return p_config->autoguard;
}
void set_autoguard(uint8_t state)
{
    p_config->autoguard = state;
}

//***********************************************
uint16_t get_interval_after_button_m(void)
{
    return p_config->interval_after_button_m;
}
void set_interval_after_button_m(uint16_t time)
{
    p_config->interval_after_button_m = time;
}

//***********************************************
uint8_t get_first_usage(void)
{
    return p_config->first_usage;
}
void set_first_usage(uint8_t state)
{
    p_config->first_usage = state;
}

//***********************************************
uint8_t get_admin_mode(void)
{
    return p_config->admin_mode;
}
void set_admin_mode(uint8_t state)
{
    p_config->admin_mode = state;
}

//***********************************************
uint8_t get_pause_alarm_report_s(void)
{
    return p_config->pause_alarm_report_s;
}
void set_pause_alarm_report_s(uint8_t state)
{
    p_config->pause_alarm_report_s = state;
}

//***********************************************
uint8_t get_single_sensor(void)
{
    return p_config->single_sensor;
}
void set_single_sensor(uint8_t state)
{
    p_config->single_sensor = state;
}

//***********************************************
