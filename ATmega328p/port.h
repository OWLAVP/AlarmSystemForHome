#ifndef PORT_H_
#define PORT_H_


#define FRONT_SENSOR_MASK (1<<0)
#define INSIDE_1_SENSOR_MASK  (1<<1) //PD4, PD5
#define INSIDE_2_SENSOR_MASK (1<<2)
#define SIDE_SENSOR_MASK  (1<<3) //PD4, PD5

void port_init(void);
void check_motion(void);
void check_button(void);
uint8_t get_motion_detected();
uint8_t is_btn_rst_pressed(void);
void check_single_sensor(void);
uint8_t get_single_motion_detected(void);
#endif /* PORT_H_ */
