#include "main.h"

#define UART_BUF_SIZE 256
//
#define USART_BAUDRATE 9600 // Desired Baud Rate
#define BAUD_PRESCALER (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)
#define ASYNCHRONOUS (0<<UMSEL00) // USART Mode Selection
#define DISABLED    (0<<UPM00)
#define EVEN_PARITY (2<<UPM00)
#define ODD_PARITY  (3<<UPM00)
#define PARITY_MODE  DISABLED // USART Parity Bit Selection
#define ONE_BIT (0<<USBS0)
#define TWO_BIT (1<<USBS0)
#define STOP_BIT ONE_BIT      // USART Stop Bit Selection
#define FIVE_BIT  (0<<UCSZ00)
#define SIX_BIT   (1<<UCSZ00)
#define SEVEN_BIT (2<<UCSZ00)
#define EIGHT_BIT (3<<UCSZ00)
#define DATA_BIT   EIGHT_BIT  // USART Data Bit Selection

static char uart_in_buf[UART_BUF_SIZE];
static unsigned char head;
static unsigned char tail;

void init_uart()
{
	UBRR0H = BAUD_PRESCALER >> 8;
	UBRR0L = BAUD_PRESCALER;

	// Set Frame Format
	UCSR0C = ASYNCHRONOUS | PARITY_MODE | STOP_BIT | DATA_BIT;
    /* Enable USART receiver and transmitter and receive complete interrupt */
    UCSR0B = _BV(RXCIE0)|(1<<RXEN0)|(1<<TXEN0);

	head = tail = 0;
}

//*******************************************************************************************************************

// USART Receiver interrupt service routine
ISR(USART_RX_vect)
{
	char status, data;

	status=UCSR0A;
	data=UDR0;
	if ((status & ((1<<FE0) | (1<<DOR0)))==0)
	{
		uart_in_buf[head] = data;
		head = (head+1)&(UART_BUF_SIZE-1);
	}
}


//*******************************************************************************************************************

char get_byte_from_queue(void)
{
	char data;
	data = uart_in_buf[tail];
	tail = (tail+1)&(UART_BUF_SIZE-1);
	return data;
}

//*******************************************************************************************************************

uint8_t is_queue_not_empty(void)
 {

	if(head!=tail)

		return TRUE;
	else
		return FALSE;
}

//*******************************************************************************************************************

void uart_send_buf(char* buf, char len)
{
	while(len--)
	{
		while( (UCSR0A & (1<<UDRE0)) == 0 ); // ждем готовности буферного регистра
		UDR0 = *(buf++);
	}
}

//*******************************************************************************************************************

void uart_send_str(char *str)
{
	while(*str)
	{
		while( (UCSR0A & (1<<UDRE0)) == 0 ); // ждем готовности буферного регистра
		UDR0 = *str++;
	}
}

//*******************************************************************************************************************

void uart_send_str_p(const char *progmem_s)
{
	register char c;

	while ((c = pgm_read_byte(progmem_s++))) {
		uart_send_byte(c);
	}
}
//*******************************************************************************************************************

void uart_send_byte(char data)
{
	while( (UCSR0A & (1<<UDRE0)) == 0 ); // ждем готовности буферного регистра
	UDR0 = data;
}

//*******************************************************************************************************************

void reset_uart_queue(void)
{
	tail = head = 0;
}

void uart_flush(void)
{
	volatile char dummy;
	uint32_t time_stamp = get_time_s() + 3;
	while ( UCSR0A & (1<<RXC0) )
	{
        if(get_time_s() > time_stamp)
            return;
		dummy = UDR0;
		dummy = dummy;
	}
}








