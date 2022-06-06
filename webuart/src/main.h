#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
#include <sys/syslog.h>
#include <time.h>
#include "sqlite/sqlite3.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <signal.h>
#include <sys/stat.h>
#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/time.h>
#include <inttypes.h>
#include <sys/un.h>

#define TRUE    1
#define FALSE   0

unsigned long get_cur_time_ms();

