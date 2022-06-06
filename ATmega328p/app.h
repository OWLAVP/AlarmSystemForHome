#ifndef APP_H_
#define APP_H_

#define FLAG_ALARM_COURTYARD    1
#define FLAG_ALARM_COOK_ROOM    2
#define FLAG_ALARM_FRONT        3
#define FLAG_ALARM_SIDE         4
#define FLAG_ALARM_TEMPERATURE  5
#define FLAG_ALARM_TEMPERATURE_INT  6
#define FLAG_ALARM_SMOKE        7
#define FLAG_ALARM_SINGLE        8
#define FLAG_ALARM_NO_INTERNET      9
#define FLAG_ALARM_HAVE_INTERNET      10
#define TIMER_RST_LOCAL 600
#define INTERVAL_CHECK_TEMPERATURE_S    60

void send_sms_report(char *phone);
void check_temperature_smoke(void);
void check_guard(void);
void check_alarm(void);
int freeRam ();
void ram_free(void);
uint8_t get_flag_alarm(void);
void set_flag_alarm(uint8_t flag);

void check_light(void);
void set_light(uint8_t state);
uint8_t get_light(void);
uint8_t get_flag_for_send(void);
void set_flag_for_send(uint8_t flag);
uint8_t get_flg_rst(void);
void set_flg_rst(uint8_t flag);
int8_t get_temperature_18b20(void);
int8_t get_temperature_mcu(void);
uint8_t get_flag_motion_inside(void);
void set_flag_motion_inside(uint8_t flag);
#endif /* APP_H_ */

