#ifndef UART_H_
#define UART_H_


void init_uart(void);
char get_byte_from_queue(void);
uint8_t is_queue_not_empty(void);
void uart_send_byte(char data);
void uart_send_buf(char* buf, char len);
void uart_send_str(char *str);
void uart_send_str_p(const char *str);
void reset_uart_queue(void);
void uart_flush(void);



#endif /* UART_H_ */
