#define MAX_BUFF 128

static int serial_port = 0;
static char read_buf[MAX_BUFF];

//-------------------------------------------------------------
int init_serial(void);
void send_serial(const char * cmd_str);
void close_serial(void);

int set_interface_attribs (int fd, int speed, int parity);
void set_blocking (int fd, int should_block);

static unsigned char Crc8(char * pcBlock, unsigned int len);

//*****************************************************************
int init_serial(void)
{
    // Open the serial port. Change device path as needed (currently set to an standard FTDI USB-UART cable type device)
    serial_port = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_SYNC);

    // Create new termios struct, we call it 'tty' for convention
    struct termios tty;

    // Read in existing settings, and handle any error
    if(tcgetattr(serial_port, &tty) != 0)
    {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        syslog(LOG_DEBUG, "failed to open: %d, %s", serial_port, strerror(errno));
        return 0;
    }

    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)
    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

    tty.c_cc[VTIME] = 1;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 100;

    // Set in/out baud rate to be 9600
    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

    // Save tty settings, also checking for error
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0)
    {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return 0;
    }
    else
    {
        syslog(LOG_DEBUG, "opened serial_port=%d for reading", serial_port);
        return 1;
    }
    return 0;

}
//*****************************************************************
void send_serial(const char * cmd_str)
{
    static char *ptr;
    static char temp_buf[MAX_BUFF-1];
    unsigned char str_crc = 0;
    int num_bytes = 0;
    unsigned char crc = 0;

    tcflush(serial_port, TCOFLUSH);
    write(serial_port, (char*)cmd_str, strlen(cmd_str));
    usleep ((7 + 128) * 100);// sleep enough to transmit the 7 plus// receive 128:  approx 100 uS per char transmit
    memset(&temp_buf, '\0', sizeof(temp_buf));
    tcflush(serial_port, TCIFLUSH);
    num_bytes = read(serial_port, &temp_buf, sizeof(temp_buf));

    if(num_bytes < 7)
    {
        //printf("num_bytes < 7\n");
        return;
    }
    ptr = temp_buf;
    ptr = strchr(temp_buf, ';');
    if(ptr)
    {
        ptr++;
        str_crc = (unsigned char)atoi(ptr);
    }
    else
    {
        return;
    }

    for(int i=0; i < num_bytes; i++)
    {
        if (temp_buf[0] == '+' && temp_buf[1] == 'U'&& temp_buf[2] == ':')
        {
            if(temp_buf[i] == ';')
            {
                temp_buf[i] = '\0';
                crc = Crc8(temp_buf, strlen(temp_buf));
                if(str_crc == crc)
                {
                    ptr = temp_buf;
                    ptr += 3;
                    memmove(read_buf, ptr, strlen(ptr));
                    break;
                }
                else
                {
                    printf("CRC8 err\n");
                    syslog(LOG_DEBUG, "CRC8 err: %u != %u", str_crc, crc);
                    break;
                }
                //printf("CRC8 message: %u \r\n", crc);
            }
        }
        else
        {
            printf("Error resv from serial\n");
            break;
        }
    }
}
//*****************************************************************
void close_serial(void)
{
    close(serial_port);
}
//*****************************************************************
const unsigned char Crc8Table[256] =
{
    0x00, 0x31, 0x62, 0x53, 0xC4, 0xF5, 0xA6, 0x97,
    0xB9, 0x88, 0xDB, 0xEA, 0x7D, 0x4C, 0x1F, 0x2E,
    0x43, 0x72, 0x21, 0x10, 0x87, 0xB6, 0xE5, 0xD4,
    0xFA, 0xCB, 0x98, 0xA9, 0x3E, 0x0F, 0x5C, 0x6D,
    0x86, 0xB7, 0xE4, 0xD5, 0x42, 0x73, 0x20, 0x11,
    0x3F, 0x0E, 0x5D, 0x6C, 0xFB, 0xCA, 0x99, 0xA8,
    0xC5, 0xF4, 0xA7, 0x96, 0x01, 0x30, 0x63, 0x52,
    0x7C, 0x4D, 0x1E, 0x2F, 0xB8, 0x89, 0xDA, 0xEB,
    0x3D, 0x0C, 0x5F, 0x6E, 0xF9, 0xC8, 0x9B, 0xAA,
    0x84, 0xB5, 0xE6, 0xD7, 0x40, 0x71, 0x22, 0x13,
    0x7E, 0x4F, 0x1C, 0x2D, 0xBA, 0x8B, 0xD8, 0xE9,
    0xC7, 0xF6, 0xA5, 0x94, 0x03, 0x32, 0x61, 0x50,
    0xBB, 0x8A, 0xD9, 0xE8, 0x7F, 0x4E, 0x1D, 0x2C,
    0x02, 0x33, 0x60, 0x51, 0xC6, 0xF7, 0xA4, 0x95,
    0xF8, 0xC9, 0x9A, 0xAB, 0x3C, 0x0D, 0x5E, 0x6F,
    0x41, 0x70, 0x23, 0x12, 0x85, 0xB4, 0xE7, 0xD6,
    0x7A, 0x4B, 0x18, 0x29, 0xBE, 0x8F, 0xDC, 0xED,
    0xC3, 0xF2, 0xA1, 0x90, 0x07, 0x36, 0x65, 0x54,
    0x39, 0x08, 0x5B, 0x6A, 0xFD, 0xCC, 0x9F, 0xAE,
    0x80, 0xB1, 0xE2, 0xD3, 0x44, 0x75, 0x26, 0x17,
    0xFC, 0xCD, 0x9E, 0xAF, 0x38, 0x09, 0x5A, 0x6B,
    0x45, 0x74, 0x27, 0x16, 0x81, 0xB0, 0xE3, 0xD2,
    0xBF, 0x8E, 0xDD, 0xEC, 0x7B, 0x4A, 0x19, 0x28,
    0x06, 0x37, 0x64, 0x55, 0xC2, 0xF3, 0xA0, 0x91,
    0x47, 0x76, 0x25, 0x14, 0x83, 0xB2, 0xE1, 0xD0,
    0xFE, 0xCF, 0x9C, 0xAD, 0x3A, 0x0B, 0x58, 0x69,
    0x04, 0x35, 0x66, 0x57, 0xC0, 0xF1, 0xA2, 0x93,
    0xBD, 0x8C, 0xDF, 0xEE, 0x79, 0x48, 0x1B, 0x2A,
    0xC1, 0xF0, 0xA3, 0x92, 0x05, 0x34, 0x67, 0x56,
    0x78, 0x49, 0x1A, 0x2B, 0xBC, 0x8D, 0xDE, 0xEF,
    0x82, 0xB3, 0xE0, 0xD1, 0x46, 0x77, 0x24, 0x15,
    0x3B, 0x0A, 0x59, 0x68, 0xFF, 0xCE, 0x9D, 0xAC
};
//*****************************************************************
static unsigned char Crc8(char *pcBlock, unsigned int len)
{
    unsigned char crc = 0xFF;

    while (len--)
        crc = Crc8Table[crc ^ *pcBlock++];

    return crc;
}
//*****************************************************************
char* get_buff_from_avr(void)
{
    char *ptr = read_buf;
    return ptr;
}
//*****************************************************************

