#ifndef TIMER_H_
#define TIMER_H_
void timer0_init(void);
void timer1_init(void);
uint32_t get_time_ms(void);
uint32_t get_time_s(void);
uint32_t get_time_m(void);
void delay_ms(uint16_t delay);
void delay_s(uint8_t delay);
void start_buzzer(uint8_t mode);
void stop_buzzer(void);
void beep(void);
void reset_soft_wdt(void);
uint32_t get_time_from_start(void);
void buzzer_stop(void);
void buzzer_non_block(uint16_t time_to_buz);
void set_time_from_motion_s(uint8_t time);
uint32_t get_time_from_motion_s();
uint32_t get_time_from_start_s();
void set_time_from_button_s(uint16_t time);
uint16_t get_time_from_button_s();
uint8_t get_beep_on(void);
void led_management(void);
#endif /* TIMER_H_ */
