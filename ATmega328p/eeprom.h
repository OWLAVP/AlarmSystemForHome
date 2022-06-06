#ifndef EEPROM_H_
#define EEPROM_H_

#define TOTAL_ADMIN_NUMBER      5
#define ADMIN_LIST				1

typedef struct
{
    uint32_t time_from_motion_s;
    uint8_t reset_count_mcu;
    //uint32_t reset_not_reg;
    uint16_t interval_after_button_m;
    uint16_t interval_after_motion_m;
    uint8_t interval_power_off_report_m;
    uint8_t interval_power_on_report_m;
    uint8_t pause_alarm_report_s;
    uint8_t single_sensor;
    uint16_t mtime_sens;
    uint8_t admin_mode;
    char admin_phone[TOTAL_ADMIN_NUMBER][14];
    //char device_number_phone[13];
    //char bal_arr[9]; //string balance
    //char last_event[100];
    uint8_t guard;
    uint8_t local_guard;
    uint8_t autoguard;
    uint8_t zone_mask;
    uint8_t first_usage; // по величине этого параметра определяется чистота EEPROM
    uint8_t first_m;
    uint8_t accept_calls;
    uint8_t accept_sms;

} config_t;


void eeprom_read_config(char start);
void eeprom_save_config(void);
void EEPROM_update(void);
void EEPROM_save_local_guard(void);

uint8_t get_interval_power_off_report(void);
void set_interval_power_off_report(uint8_t minute);
uint8_t get_interval_power_on_report(void);
void set_interval_power_on_report(uint8_t minute);
void set_device_number_phone(char *number);
char* get_device_number_phone(void);
uint8_t get_pause_alarm_report_s(void);
uint8_t get_accept_calls(void);
void set_accept_calls(uint8_t on_off);
uint8_t get_accept_sms(void);
void set_accept_sms(uint8_t on_off);
uint8_t get_guard(void);
void set_guard(uint8_t state);
uint8_t get_local_guard(void);
void set_local_guard(uint8_t state);
char* get_phones(uint8_t  i);
void set_reset_count_mcu(uint8_t cnt);
uint8_t get_reset_count_mcu(void);
void inc_reset_count_mcu(void);
uint8_t get_first_m(void);
void set_first_m(uint8_t state);
char get_zone_mask(void);
void set_zone_mask(char state);
uint16_t get_mtime_sens(void);
void set_mtime_sens(uint16_t time);
uint16_t get_interval_after_motion_m(void);
void set_interval_after_motion_m(uint16_t time);
uint8_t get_autoguard(void);
void set_autoguard(uint8_t state);
uint16_t get_interval_after_button_m(void);
void set_interval_after_button_m(uint16_t time);
uint8_t get_first_usage(void);
void set_first_usage(uint8_t state);
uint8_t get_admin_mode(void);
void set_admin_mode(uint8_t state);
uint8_t get_pause_alarm_report_s(void);
void set_pause_alarm_report_s(uint8_t state);
uint8_t get_single_sensor(void);
void set_single_sensor(uint8_t state);
#endif /* EEPROM_H_ */


