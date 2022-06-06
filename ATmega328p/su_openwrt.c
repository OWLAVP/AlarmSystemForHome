#include "main.h"

static char data[SOFTUART_IN_BUF_SIZE-1];
static uint8_t flag_internet = 1;
static uint32_t timestamp_send = 0;

static char* openwrt_to_string(void);

//*********************************************************************************************
static char* openwrt_to_string(void)
{
    static uint8_t state=0;
    static uint8_t i;
    static uint32_t time_stamp;
    unsigned char ch;

    if((get_time_s() - time_stamp) > 10)
    {
        if(state)
        {
//            error_code1 = state;
            state = 0;
        }
    }
    if(softuart_kbhit())
    {
        time_stamp = get_time_s();
        ch = softuart_getchar();
        //softuart_putchar(ch);
        switch(state)
        {
        case 0:
            if((ch != 0) && (ch != '\n'))
            {
                data[0] = ch;
                i = 1;
                state = 3;
            }
            break;
        case 3:
            data[i] = ch;
            i++;
            if(i>=SOFTUART_IN_BUF_SIZE-1)
            {
                i=0;
                state = 0; // переполнение входного буфера
                break;
            }
            if(i>=2)
            {
                if((data[i-1] == '\n'))
                {
                    //data[i-2] = 0;
                    state = 0;
                    return data;
                }
            }
            break;
        }
    }
    return 0;
}

//***************************************************
uint8_t get_message_from_openwrt()
{
    char *ptr;
    ptr = openwrt_to_string();
    if(ptr)
    {
        if(ptr[0] == 'a' && ptr[1] == 'v')
        {
            ptr += 2;
            if(strstr_P(ptr, PSTR("lamp=1")))
            {
                set_light(1);
            }
            else if(strstr_P(ptr, PSTR("lamp=0")))
            {
                set_light(0);
            }
            else if(strstr_P(ptr, PSTR("m_g=1")))
            {
                set_local_guard(0);
                set_guard(1);
                set_flg_rst(1);
                EEPROM_update();
            }
            else if(strstr_P(ptr, PSTR("m_g=0")))
            {
                set_guard(0);
                set_flg_rst(1);
                EEPROM_update();
            }
            else if(strstr_P(ptr, PSTR("l_g=1")))
            {
                set_guard(0);
                set_local_guard(1);
                set_flg_rst(1);
                set_flag_motion_inside(0);
                EEPROM_update();
            }
            else if(strstr_P(ptr, PSTR("l_g=0")))
            {
                set_local_guard(0);
                set_flag_motion_inside(0);
                set_flg_rst(1);
                EEPROM_update();
            }
            else if(strstr_P(ptr, PSTR("buz=0")))
            {
                buzzer_stop();
            }
            else if(strstr_P(ptr, PSTR("buz=1")))
            {
                buzzer_non_block(30000);
            }
            else if(strstr_P(ptr, PSTR("ussd")))
            {
                send_USSD(0);
            }
            else if(strstr_P(ptr, PSTR("reboot_all")))
            {
                reset_mcu(1);
                return 1;
            }
            else if(strstr_P(ptr, PSTR("reboot_gsm")))
            {
                reset_mcu(0);
                return 1;
            }
            else if(strstr_P(ptr, PSTR("ping0")))
            {
                set_val(flag_internet, 0);
            }
            else if(strstr_P(ptr, PSTR("ping1")))
            {
                set_val(flag_internet, 1);
            }
            else if(strstr_P(ptr, PSTR("*10")))
            {
                char _str[16];
                ptr += 5;
                memcpy(_str, ptr, 16);
                top_up_balance(_str);
            }
            send_state();
            return 1;
        }
    }
    return 0;
}
//*********************************************************************************************
void timer_send_state(void)
{
    if(get_flg_rst() == 1)
    {
        set_flag_for_send(0);
        set_flg_rst(0);
    }
    else if(get_time_s() >= timestamp_send + 30)
    {
        send_state();
    }
}
//*********************************************************************************************

void send_state(void)
{
        char report[120];
        char *ptr;

        report[0] = '+';
        report[1] = 'U';
        report[2] = ':';
        ptr = report;
        ptr += 3;
        ptr += sprintf_P(ptr, PSTR("d=%s&"), get_arr_dep());//deposit simcard
        ptr += sprintf_P(ptr, PSTR("g=%u&"), get_guard());//get state guard
        ptr += sprintf_P(ptr, PSTR("lg=%u&"), get_local_guard());// get state local guard
        ptr += sprintf_P(ptr, PSTR("ti=%d&"), get_temperature_mcu());//get mcu temperature
        ptr += sprintf_P(ptr, PSTR("te=%d&"), get_temperature_18b20());// get ext 18b20 temperature
        ptr += sprintf_P(ptr, PSTR("m=%u&"), get_reset_count_mcu()); //get count reset mcu
        ptr += sprintf_P(ptr, PSTR("sig=%u&"), get_signal_strength());// get signal from gsm
        ptr += sprintf_P(ptr, PSTR("r=%u&"), get_registered_in_gsm_network());// get state regitered gsm

        if(get_time_without_power_s() == 0)
        {
            ptr += sprintf_P(ptr, PSTR("b=%u&"), 110); //if have ext power -> in battery 110%
        }
        else
        {
            ptr += sprintf_P(ptr, PSTR("b=%u&"), get_battery_level());// if not ext power get battery level

        }
        ptr += sprintf_P(ptr, PSTR("s=%u&"), get_flag_for_send()); // sent flag alarm
        ptr += sprintf_P(ptr, PSTR("l=%u&"), get_light()); // get state light
        ptr += sprintf_P(ptr, PSTR("z=%u&"), get_beep_on()); // get state buzzer
        ptr += sprintf_P(ptr, PSTR("mi=%u&"), get_flag_motion_inside()); // get state motion_inside
        ptr += sprintf_P(ptr, PSTR("bm=%u&"), get_flag_busy_mdm());//flag busy modem
        ptr += sprintf_P(ptr, PSTR("t=%lu"), get_time_s()); //get time in second from start mcu
        unsigned short crc = Crc8(report, strlen(report));

        ptr += sprintf_P(ptr, PSTR(";%u"), crc); // get crc8
        softuart_puts_ln(report); // send report to openwrt
        timestamp_send = get_time_s();
}



//*************************************************************************************************
void check_internet(void)
{
    static uint8_t _temp_flag = 0;

    if(flag_internet == 0 && _temp_flag == 0)
    {
        _temp_flag = 1;
        send_sms_admins(PSTR("No Internet"));
    }
    if(flag_internet == 1 && _temp_flag == 1)
    {
        _temp_flag = 0;
        send_sms_admins(PSTR("Internet OK"));
    }
}
//***************************************************************************
//void test_crc(char* str)
//{
//    char *ptr;
//    ptr = strtok(str, ";");
//    //softuart_puts_ln(ptr);
//    unsigned char crc = Crc8(ptr, strlen(ptr));
//
//    char debug_buffer[15];
//    sprintf(debug_buffer,"crc=%u", crc);
//    softuart_puts_ln(debug_buffer);
//}

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

unsigned char Crc8(char *pcBlock, unsigned char len)
{
    unsigned char crc = 0xFF;

    while (len--)
        crc = Crc8Table[crc ^ *pcBlock++];

    return crc;
}
