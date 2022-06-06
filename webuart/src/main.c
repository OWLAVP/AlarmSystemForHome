#include "ping.h"
#include "ttl.h"
#include "handshake.h"
//#include "ws.h"
#include "write_sqlite.h"
#define DELAY_SEND_TO_AVR_MS        100;

const static char msg_not_ping[] = "avping0";
const static char msg_ping_ok[] = "avping1";
static int state_internet = 0;
const static char UPDATE[] = "av\r\n";
/**
 * @brief Called when a client connects to the server.
 *
 * @param client Client connection. The @p client parameter is used
 * in order to send messages and retrieve informations about the
 * client.
 */
void onopen(ws_cli_conn_t *client)
{
    char *cli;
    cli = ws_getaddress(client);
    #ifndef DISABLE_VERBOSE
    printf("Connection opened, addr: %s\n", cli);
    #endif
}

/**
 * @brief Called when a client disconnects to the server.
 *
 * @param client Client connection. The @p client parameter is used
 * in order to send messages and retrieve informations about the
 * client.
 */
void onclose(ws_cli_conn_t *client)
{
    char *cli;
    cli = ws_getaddress(client);
    #ifndef DISABLE_VERBOSE
    printf("Connection closed, addr: %s\n", cli);
    #endif
}

/**
 * @brief Called when a client connects to the server.
 *
 * @param client Client connection. The @p client parameter is used
 * in order to send messages and retrieve informations about the
 * client.
 *
 * @param msg Received message, this message can be a text
 * or binary message.
 *
 * @param size Message size (in bytes).
 *
 * @param type Message type.
 */
void onmessage(ws_cli_conn_t *client,
               const unsigned char *msg, uint64_t size, int type)
{
    char *cli;

    cli = ws_getaddress(client);
    if(memcmp(msg, "update", 6) == 0)
    {
        ws_sendframe_txt(client, (const char*)get_buff_from_avr());
    }
    else if(memcmp(msg, "lamp=0", 6) == 0)
    {
        send_serial("avlamp=0\r\n");
    }
    else if(memcmp(msg, "lamp=1", 6) == 0)
    {
        send_serial("avlamp=1\r\n");
    }
    else if(memcmp(msg, "buzzer=1", 8) == 0)
    {
        send_serial("avbuz=1\r\n");
    }
    else if(memcmp(msg, "buzzer=0", 8) == 0)
    {
        send_serial("avbuz=0\r\n");
    }
    else if(memcmp(msg, "main_guard=1", 12) == 0)
    {
        send_serial("avm_g=1\r\n");
    }
    else if(memcmp(msg, "main_guard=0", 12) == 0)
    {
        send_serial("avm_g=0\r\n");
    }
    else if(memcmp(msg, "l_guard=1", 9) == 0)
    {
        send_serial("avl_g=1\r\n");
    }
    else if(memcmp(msg, "l_guard=0", 9) == 0)
    {
        send_serial("avl_g=0\r\n");
    }
    else if(memcmp(msg, "reboot_all", 10) == 0)
    {
        send_serial("avreboot_all\r\n");
    }
    else if(memcmp(msg, "reboot_gsm", 10) == 0)
    {
        send_serial("avreboot_gsm\r\n");
    }
    else if(memcmp(msg, "USSD", 4) == 0)
    {
        send_serial("ussd\r\n");
    }

    else if(memcmp(msg, "code=", 5) == 0)
    {
        char code[25];
        char *ptr = (char*)msg;
        ptr += 5;
        sprintf(code, "%s\r\n", ptr);
        send_serial(code);
    }

    //printf("I receive a message: %s (size: %" PRId64 ", type: %d), from: %s\n", msg, size, type, cli);
}

/**
 * @brief Main routine.
 *
 * @note After invoking @ref ws_socket, this routine never returns,
 * unless if invoked from a different thread.
 */
int main(void)
{
    printf("Начнем?\n");

    struct ws_events evs;
    evs.onopen    = &onopen;
    evs.onclose   = &onclose;
    evs.onmessage = &onmessage;
    ws_socket(&evs, 8080, 1, 1000); /* Never returns. */
    if(init_icmp() == 0)
        return (0);
    if(init_serial() == 0)
        return (0);

    printf("Поехали!\n");
    static unsigned long timestamp_send = 0;

    while(1)
    {
        if(get_cur_time_ms() > timestamp_send)
        {
            timestamp_send = get_cur_time_ms() + DELAY_SEND_TO_AVR_MS;
            send_serial(UPDATE);
        }

        WriteSQLITE(get_buff_from_avr());

        state_internet = ping();
        if (state_internet == 1)
        {
            send_serial(msg_ping_ok);
        }
        else if (state_internet == -1)
        {
            send_serial(msg_not_ping);
        }

        sleep(0);
    }
    close_serial();
    close_ping();
    return (0);
}



unsigned long get_cur_time_ms()
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    unsigned long time_ms = time.tv_sec * 1000 + (time.tv_nsec / 1000000);
    return time_ms;
}

