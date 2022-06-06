#ifndef _GSM_H_
#define _GSM_H_

#define NET_BUF_SIZE	   320
#define TIME_CHECK_REG		180

#define RESPON_OK			"OK"
#define RESPON_CLCC         "+CLCC:"
#define RESPON_SMS			"+CPMS:"
#define RESPON_CREG          "+CREG: 0,1"
#define RESPON_END			"\n"
#define RESPON_OK_GPRS		"+SAPBR: 1,1"
#define RESPON_OK_CSQ       "+CSQ:"
#define RESPON_OK_CMGL      "+CMGL:"
#define RESPON_ERROR	    "ERROR"
#define RESPON_CMSS      "+CMSS:"
#define RESPON_CMGS      "+CMGS:"

#define GSM_OK				1
#define GSM_CALL_ACTIVE		2   //	Состояние вызова - активное голосовое соединение.
#define GSM_CALL_HELD		3	//	Состояние вызова - удерживаемый.
#define GSM_CALL_END		4	//	Состояние вызова - разъединение.
#define GSM_CALL_OUT_DIAL	5   //	Состояние вызова - исходящий в режиме набора адреса.
#define GSM_CALL_OUT_BEEP	6	//	Состояние вызова - исходящий в режиме дозвона.
#define GSM_CALL_IN_BEEP	7	//	Состояние вызова - входящий  в режиме дозвона.
#define GSM_CALL_IN_WAIT	8	//	Состояние вызова - входящий  в режиме ожидания.
#define GSM_CALL_ERR		0


///////////// filename  ////////////////////
#define AUDIO_GUARD_OFF           "off-guard"
#define AUDIO_GUARD_ON            "ohrana"
#define ALARM_LOCAL_GUARD_ON      "local-on" ///
#define AUDIO_ALARM_COURTYARD     "a-dvor"
#define AUDIO_ALARM_COOK_ROOM     "a-kuh"
#define AUDIO_ALARM_FRONT         "a-front"
#define AUDIO_ALARM_SIDE          "a-side"
#define AUDIO_MENU           "menu"
#define AUDIO_REPORT_SEND    "r-send"
#define AUDIO_LIGHT_ON            "l-on"
#define AUDIO_LIGHT_OFF           "l-off"
#define AUDIO_ALARM_TEMPERATURE   "tempa"
#define AUDIO_ALARM_SMOKE        "ugar"


#define BUSY_MODEM  1
#define FREE_MODEM  0
#define REBOOT_MODEM  2
char *get_phone_number(void);
uint8_t send_sms(char *str, char *phone);
uint8_t send_sms_p(const char *str, char *phone);
uint8_t check_phone_string(char *ptr);
uint8_t get_message_from_mdm(void);
uint8_t get_sms(void);
uint8_t delete_all_sms(void);
uint8_t get_signal_strength(void);
uint8_t check_registration(void);
void incoming_call_processing(void);
void reset_mcu(uint8_t all);
uint8_t setup_init();
uint8_t call_to(char *phone, char *file_amr);
void alarm_play_file(char * file);
uint8_t get_error_code1();
uint16_t get_error_gsm();
void set_error_gsm(uint16_t _err);
char* find_char(char *str, char ch);
char send_USSD(uint8_t _flag_sms);
void check_deposit(void);
void led_gsm(void);
void send_sms_admins(const char *_msg);
uint8_t map_s(uint16_t x, uint16_t in_min, uint16_t in_max, uint8_t out_min, uint8_t out_max);
char* get_arr_dep(void);
uint8_t get_registered_in_gsm_network(void);
uint8_t get_battery_level(void);
void top_up_balance(char * code);
uint8_t get_flag_busy_mdm(void);
#endif
