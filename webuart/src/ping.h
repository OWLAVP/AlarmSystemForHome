#include "main.h"


#define PING_OK 1
#define NO_PING   -1

// ping packet size
#define PING_PKT_S 64

// Gives the timeout delay for receiving packets
// in seconds
#define RECV_TIMEOUT 1

static int sockfd;
const static char ip_addr[] = "77.88.8.8";
static struct sockaddr_in addr_con;
static unsigned long ping_time_ms = 0;

void close_ping(void);
// ping packet structure
struct ping_pkt
{
    struct icmphdr hdr;
    char msg[PING_PKT_S-sizeof(struct icmphdr)];
};

//*********************************************************************
static unsigned short checksum(struct ping_pkt *b, int len)
{
    unsigned short *buf = (unsigned short *)b;
    unsigned int sum=0;
    unsigned short result;

    for ( sum = 0; len > 1; len -= 2 )
        sum += *buf++;
    if ( len == 1 )
        sum += *(unsigned char*)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

//**********************************************************************
int init_icmp()
{
    struct timeval tv_out;
    tv_out.tv_sec = RECV_TIMEOUT;
    tv_out.tv_usec = 0;
    //struct timespec tfs;
    int ttl_val=64;

    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if(sockfd < 0)
    {
        printf("\nSocket file descriptor not received!!\n");
        return 0;
    }
    else
        printf("\nSocket file descriptor %d received\n", sockfd);

    // set socket options at ip to TTL and value to 64,
    // change to what you want by setting ttl_val
    if (setsockopt(sockfd, SOL_IP, IP_TTL, &ttl_val, sizeof(ttl_val)) != 0)
    {
        printf("\nSetting socket options to TTL failed!\n");
        return 0;
    }
    else
    {
        printf("\nSocket set to TTL..\n");
    }

    // setting timeout of recv setting
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof tv_out) != 0)
    {
        printf("\nSetting socket timeout failed!\n");
        return 0;
    }
    else
    {
        printf("\nSocket set timeout %ds\n", RECV_TIMEOUT);
    }

    addr_con.sin_addr.s_addr = inet_addr(ip_addr);
    return 1;
}
//**********************************************************************
static int send_ping(int ping_sockfd, struct sockaddr_in *ping_addr)
{
    socklen_t addr_len;
    struct ping_pkt pckt;
    struct sockaddr_in r_addr;
    struct timespec time_start, time_end;
    int i;

    //filling packet
    bzero(&pckt, sizeof(pckt));

    pckt.hdr.type = ICMP_ECHO;
    pckt.hdr.un.echo.id = getpid();

    for ( i = 0; i < sizeof(pckt.msg)-1; i++ )
        pckt.msg[i] = i+'0';

    pckt.msg[i] = 0;
    pckt.hdr.checksum = checksum(&pckt, sizeof(pckt));

    //send packet
    clock_gettime(CLOCK_MONOTONIC, &time_start);
    if ( sendto(ping_sockfd, &pckt, sizeof(pckt), 0, (struct sockaddr*) ping_addr, sizeof(*ping_addr)) <= 0)
    {
        printf("\nPacket Sending Failed!\n");
        return 0;
    }

    //receive packet
    addr_len=sizeof(r_addr);

    if ( recvfrom(ping_sockfd, &pckt, sizeof(pckt), 0, (struct sockaddr*)&r_addr, &addr_len) <= 0 )
    {
        printf("\nPacket receive failed!\n");
        return 0;
    }
    else
    {
        clock_gettime(CLOCK_MONOTONIC, &time_end);

        if(!(pckt.hdr.type ==69 && pckt.hdr.code==0))
        {
            printf("Error..Packet received with ICMP type %d code %d\n", pckt.hdr.type, pckt.hdr.code);
            return 0;
        }
        else
        {
            printf("ping OK\n");
            return 1;
        }
    }
    return 0;
}

//**********************************************************************
int ping(void)
{
    static int flag_not_ping = FALSE;
    static int is_ping_host = TRUE;
    static char cnt_ok = 5;

    if(get_cur_time_ms() > ping_time_ms)
    {
        static unsigned long cnt_not_ping = 0;

        int result = send_ping(sockfd, &addr_con);
        if (result == 0)
        {
            cnt_not_ping++;
            ping_time_ms = get_cur_time_ms() + 10000;
            printf("Timeout.\n");
            if(cnt_not_ping >= 6)
            {
                ping_time_ms = get_cur_time_ms() + 30000;
                if(!flag_not_ping)
                {
                    flag_not_ping = TRUE;
                    is_ping_host = FALSE;
                    printf("msg_not_ping\n");
                    return NO_PING;
                }
            }
        }
        else
        {
            cnt_not_ping = 0;
            ping_time_ms = get_cur_time_ms() + 30000;
            flag_not_ping = FALSE;
            if(!is_ping_host)
            {
                if(cnt_ok > 0)
                {
                    cnt_ok--;
                    return FALSE;
                }
                cnt_ok = 5;
                printf("msg_send\n");
                is_ping_host = TRUE;
                return PING_OK;
            }
        }
    }
    return FALSE;
}

//**********************************************************************
void close_ping(void)
{
    close(sockfd);
}
