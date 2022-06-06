#ifndef SU_OPENWRT_H_INCLUDED
#define SU_OPENWRT_H_INCLUDED


uint8_t get_message_from_openwrt();
void timer_send_state(void);
void check_internet(void);
void send_state(void);
unsigned char Crc8(char *pcBlock, unsigned char len);

#endif // SU_OPENWRT_H_INCLUDED
