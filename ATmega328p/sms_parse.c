#include "main.h"

#define SIZE_OF_SMS_TEXT_BUF 160
#define MAX_SMS_LENGTH       160

static const char help_text[MAX_SMS_LENGTH+1] PROGMEM = {"helpset;helpget;report;adminphones;reboot;resetdefault;pay_code=;light=;"};
static const char help_get[MAX_SMS_LENGTH+1] PROGMEM = {"get:adminphones;accept_calls;btninterval;aftermotion;autoguard;mtime_sens_ms;mpause_ms;report_power"};
static const char help_set[MAX_SMS_LENGTH+1] PROGMEM = {"set:btninterval=;aftermotion=;accept_calls=;voice=;guard=;local_guard=;autoguard=;mtime_sens_ms=;mpause_ms=;pause_alarm_s=;report_power="};

static char* set_phones(char *phones, char *dest, char max_num);
static char* get_param(char *str, char *sms_text);
static char* set_param(char *ptr);

char find_phone_in_phone_list(char *phone, char lst)
{
    uint8_t i;

    if(lst==ADMIN_LIST)
    {
        for(i=0; i<TOTAL_ADMIN_NUMBER; i++)
        {
            if(get_phones(i)[0] == '+')
                if(memcmp(phone, &get_phones(i)[0], 13) == 0)
                {
//                    softuart_puts_p(PSTR("phone ok"));
                    return 1; // телефон найден в списке админов
                }
        }
    }
//    softuart_puts_p(PSTR("not phone"));
    return 0;
}

//*******************************************************************************************************************

void process_sms_body(char *ptr)
{
    uint8_t i, err;

    //beep_ms(10);

    if(memcmp_P(ptr, PSTR("set:"), 4) == 0)
    {
        if((find_phone_in_phone_list(get_phone_number(), ADMIN_LIST) == 0))
        {
            send_sms_p(PSTR("Access denied"), get_phone_number());
            return;
        }
        ptr += 4;
        i=0;
        err = 0;
        while(ptr && *ptr)
        {
            ptr = set_param(ptr);
            //softuart_puts_ln(ptr);
            if(ptr)
                i++;
            else
                err = 1;
        }
        if(i && !err)
        {
            EEPROM_update();
            send_sms_p(PSTR("ok"), get_phone_number());
        }
        else
        {
            eeprom_read_config(0); // возвращаем всё в зад
            send_sms_p(PSTR("error"), get_phone_number());
        }
    }


    else if(memcmp_P(ptr, PSTR("get:"), 4) == 0)
    {
        char sms_text[SIZE_OF_SMS_TEXT_BUF];
        if((find_phone_in_phone_list(get_phone_number(), ADMIN_LIST) == 0))
        {
            send_sms_p(PSTR("Access denied"), get_phone_number());
            return;
        }
        ptr += 4;
        sms_text[0] = 0; // терминируем строку
        i=0;
        err = 0;

        while(ptr && *ptr)
        {

            ptr = get_param(ptr, &sms_text[strlen(sms_text)]);
            // softuart_puts_ln(ptr);
            if(ptr)
                i++;
            else
            {
                err = 1;
                break;
            }
            if(strlen(sms_text) > MAX_SMS_LENGTH)
            {
                err = 2;
                break;
            }
        }
        if(i && !err)
        {
            send_sms(sms_text, get_phone_number());
//            softuart_puts_p(PSTR("send_sms\r\n"));
        }

        else if(err==1)
        {
            send_sms_p(PSTR("error"), get_phone_number());
        }

        else if(err==2)
        {
            send_sms_p(PSTR("resulting SMS text is too long"), get_phone_number());
        }

    }

    else if(memcmp_P(ptr, PSTR("resetdefault;"), 13) == 0)
    {
        if((find_phone_in_phone_list(get_phone_number(), ADMIN_LIST) == 0))
        {
            send_sms_p(PSTR("Access denied"), get_phone_number());
            return;
        }
        set_first_usage(0);
        EEPROM_update();
        send_sms_p(PSTR("Device reset default, send adminphones=NUMBER_PHONES"), get_phone_number());
        reset_mcu(1);
    }

    else if(memcmp_P(ptr, PSTR("reboot;"), 7) == 0)
    {
        if((find_phone_in_phone_list(get_phone_number(), ADMIN_LIST) == 0))
        {
            send_sms_p(PSTR("Access denied"), get_phone_number());
            return;
        }
        send_sms_p(PSTR("Reset command accepted."), get_phone_number());
        reset_mcu(1);
    }
    else if(memcmp_P(ptr, PSTR("help;"), 5) == 0)
    {
        if((find_phone_in_phone_list(get_phone_number(), ADMIN_LIST) == 0))
        {
            send_sms_p(PSTR("Access denied"), get_phone_number());
            return;
        }
        send_sms_p(help_text, get_phone_number());
    }
    else if(memcmp_P(ptr, PSTR("helpget;"), 8) == 0)
    {
        if((find_phone_in_phone_list(get_phone_number(), ADMIN_LIST) == 0))
        {
            send_sms_p(PSTR("Access denied"), get_phone_number());
            return;
        }
        send_sms_p(help_get, get_phone_number());
    }
    else if(memcmp_P(ptr, PSTR("helpset;"), 8) == 0)
    {
        if((find_phone_in_phone_list(get_phone_number(), ADMIN_LIST) == 0))
        {
            send_sms_p(PSTR("Access denied"), get_phone_number());
            return;
        }
        send_sms_p(help_set, get_phone_number());
    }
    else if(memcmp_P(ptr, PSTR("report;"), 7) == 0)
    {
        if((find_phone_in_phone_list(get_phone_number(), ADMIN_LIST) == 0))
        {
            send_sms_p(PSTR("Access denied"), get_phone_number());
            return;
        }
        send_sms_report(get_phone_number());
    }
    else if(memcmp_P(ptr, PSTR("adminphones="), 12) == 0)
    {
        if( get_admin_mode() == 0 || (find_phone_in_phone_list(get_phone_number(), ADMIN_LIST) == 1))
        {
            ptr+=12;
            ptr = set_phones(ptr, &get_phones(0)[0], TOTAL_ADMIN_NUMBER);
            set_admin_mode(1);
            EEPROM_update();
            send_sms_p(PSTR("Ok"), get_phone_number());
            //send_sms("ok", get_phone_number());
//            softuart_puts_p(PSTR("set admin ok"));
            return;
        }
        else
        {
            send_sms_p(PSTR("Access denied"), get_phone_number());
            return;
        }
    }
    else if(memcmp_P(ptr, PSTR("light="), 6) == 0)
    {
        char temp;

        ptr+=6;
        if(*ptr == '0')
            temp = 0;
        else if(*ptr == '1')
        {
            temp = 1;
        }
        else
            return;
        if(*++ptr != ';')
            return;
        if(temp == 1)
            DigitalWrite(PIN_LIGHT, HIGH);
        else if (temp == 0)
            DigitalWrite(PIN_LIGHT, LOW);
    }
}

//*******************************************************************************************************************

static char* get_param(char *str, char *sms_text)
{
    if(memcmp_P(str, PSTR("adminphones;"), 12) == 0)
    {
        uint8_t n, i;

        str += 12;
        sms_text += sprintf_P(sms_text, PSTR("adminphones="));
        n=0;
        for(i=0; i<TOTAL_ADMIN_NUMBER; i++)
        {
            if(get_phones(i)[0] != '+')
                break;
            sms_text += sprintf_P(sms_text, PSTR("%s,"), &get_phones(i)[0]);
            n++; // количество напечатанных телефонов
        }
        if(n)
            sms_text--;
        sprintf_P(sms_text, PSTR(";"));
        return str;
    }

    else if(memcmp_P(str, PSTR("btninterval;"), 12) == 0)
    {
        str += 12;
        sprintf_P(sms_text, PSTR("interval_after_button_m=%um;"), get_interval_after_button_m());
        return str;
    }

    else if(memcmp_P(str, PSTR("aftermotion;"), 12) == 0)
    {
        str += 12;
        sprintf_P(sms_text, PSTR("interval_after_motion_h=%uh;"), get_interval_after_motion_m());
        return str;
    }

    else if(memcmp_P(str, PSTR("autoguard;"), 10) == 0)
    {
        str += 10;
        sprintf_P(sms_text, PSTR("autoguard=%d;"), get_autoguard());
        return str;
    }

    else if(memcmp_P(str, PSTR("accept_calls;"), 13) == 0)
    {
        str += 13;
        sprintf_P(sms_text, PSTR("accept_calls=%d;"), get_accept_calls());
        return str;
    }

    else if(memcmp_P(str, PSTR("accept_sms;"), 13) == 0)
    {
        str += 13;
        sprintf_P(sms_text, PSTR("accept_sms=%d;"), get_accept_sms());
        return str;
    }

    else if(memcmp_P(str, PSTR("mtime_sens_ms;"), 13) == 0)
    {
        str += 13;
        sprintf_P(sms_text, PSTR("mtime_sens_ms=%u;"), (uint16_t)get_mtime_sens());
        return str;
    }

//	else if(memcmp_P(str, PSTR("mpause_ms;"), 10) == 0)
//	{
// 		str += 10;
// 		sprintf_P(sms_text, PSTR("mpause_ms=%u;"), (uint16_t)config.mpause_count*10);
// 		return str;
//	}

    return 0;
}

//*******************************************************************************************************************

static char* set_param(char *ptr)
{
    if(memcmp_P(ptr, PSTR("btninterval="), 12) == 0)
    {
        uint32_t interval;
        ptr+=12;
        if(isdigit(*ptr) == 0)
            return 0;
        interval = strtoul(ptr, &ptr, 10);
        if( interval>60000 )
            return 0;
        if(*ptr != ';')
            return 0;
        set_interval_after_button_m(interval);
        ptr++;
        return ptr;
    }

    if(memcmp_P(ptr, PSTR("aftermotion="), 12) == 0)
    {
        uint32_t interval;
        ptr+=12;
        if(isdigit(*ptr) == 0)
            return 0;
        interval = strtoul(ptr, &ptr, 10);
        if( interval>60000 )
            return 0;
        if(*ptr != ';')
            return 0;
        set_interval_after_motion_m(interval);
        ptr++;
        return ptr;
    }

    if(memcmp_P(ptr, PSTR("pause_alarm_s="), 14) == 0)
    {
        ptr+=14;
        if(isdigit(*ptr) == 0)
            return 0;
        uint8_t c = atoi(ptr);
        if(c > 250)
            return 0;
        if(*ptr != ';')
            return 0;
        set_pause_alarm_report_s(c);
        ptr++;
        return ptr;
    }

    if(memcmp_P(ptr, PSTR("guard="), 6) == 0)
    {
        char temp;

        ptr+=6;
        if(*ptr == '0')
            temp = 0;
        else if(*ptr == '1')
        {
            temp = 1;
        }
        else
            return 0;
        if(*++ptr != ';')
            return 0;
        set_local_guard(0);
        set_guard(temp);
        set_zone_mask(0xF0);
        ptr++;
        return ptr;
    }

    if(memcmp_P(ptr, PSTR("local_guard="), 12) == 0)
    {
        char temp;

        ptr+=12;
        if(*ptr == '0')
            temp = 0;
        else if(*ptr == '1')
        {
            temp = 1;
        }
        else
            return 0;
        if(*++ptr != ';')
            return 0;
        set_guard(0);
        set_local_guard(temp);
        set_zone_mask(0xF0);
        ptr++;
        return ptr;
    }

    if(memcmp_P(ptr, PSTR("autoguard="), 10) == 0)
    {
        char temp;

        ptr+=10;
        if(*ptr == '0')
            temp = 0;
        else if(*ptr == '1')
            temp = 1;
        else
            return 0;
        if(*++ptr != ';')
            return 0;
        set_autoguard(temp);
        ptr++;
        return ptr;
    }

    if(memcmp_P(ptr, PSTR("accept_calls="), 13) == 0)
    {
        char temp;

        ptr+=13;
        if(*ptr == '0')
            temp = 0;
        else if(*ptr == '1')
            temp = 1;
        else
            return 0;
        if(*++ptr != ';')
            return 0;
        set_accept_calls(temp);
        ptr++;
        return ptr;
    }

    if(memcmp_P(ptr, PSTR("accept_sms="), 13) == 0)
    {
        char temp;

        ptr+=13;
        if(*ptr == '0')
            temp = 0;
        else if(*ptr == '1')
            temp = 1;
        else
            return 0;
        if(*++ptr != ';')
            return 0;
        set_accept_sms(temp);
        ptr++;
        return ptr;
    }

    if(memcmp_P(ptr, PSTR("power_off_report_m="), 19) == 0)
    {
        char temp;

        ptr+=19;
        if(*ptr == '0')
            temp = 0;
        else if(*ptr == '1')
            temp = 1;
        else
            return 0;
        if(*++ptr != ';')
            return 0;
        set_interval_power_off_report(temp);
        ptr++;
        return ptr;
    }

    if(memcmp_P(ptr, PSTR("power_on_report_m="), 19) == 0)
    {
        char temp;

        ptr+=19;
        if(*ptr == '0')
            temp = 0;
        else if(*ptr == '1')
            temp = 1;
        else
            return 0;
        if(*++ptr != ';')
            return 0;
        set_interval_power_on_report(temp);
        ptr++;
        return ptr;
    }
	if(memcmp_P(ptr, PSTR("s_sensor="), 9) == 0)
	{
 		char temp;

 		ptr+=9;
 		if(*ptr == '0')
 			temp = 0;
 		else if(*ptr == '1')
 			temp = 1;
 		else
 			return 0;
 		if(*++ptr != ';')
 			return 0;
        set_single_sensor(temp);
 		ptr++;
 		return ptr;
	}

    if(memcmp_P(ptr, PSTR("mtime_sens_ms="), 13) == 0)
    {
        uint16_t c;
        ptr+=13;
        if(isdigit(*ptr) == 0)
            return 0;
        c = strtoul(ptr, &ptr, 10);
        if(c > 20000)
            return 0;
        if(*ptr != ';')
            return 0;
        set_mtime_sens(c);
        ptr++;
        return ptr;
    }

//	if(memcmp_P(ptr, PSTR("mpause_ms="), 10) == 0)
//	{
// 		uint32_t c;
// 		ptr+=10;
// 		if(isdigit(*ptr) == 0)
// 			return 0;
// 		c = strtoul(ptr, &ptr, 10);
// 		if(c > 2550)
// 			return 0;
// 		if(*ptr != ';')
// 			return 0;
// 		config.mpause_count = c/10;
// 		ptr++;
// 		return ptr;
//	}

    return 0;
}

//*******************************************************************************************************************
// принимает список телефонов через запятую, адрес назначения и максимальный размер области назначения
static char* set_phones(char *phones, char *dest, char max_num)
{
    uint8_t n, i;
    char *ptr = phones;
    n=0;

    for(uint8_t j = 0; j < TOTAL_ADMIN_NUMBER; j++)
    {
        get_phones(j)[0] = 0;
    }
    while(check_phone_string(ptr))
    {
        n++;
        if(n > max_num)
            return 0;
        ptr+=13;
        if(*ptr == ';')
            break;
        else if(*ptr == ',')
        {
            ptr++;
            continue;
        }
        else
            return 0;
    }
    ptr = phones;
    for(i=0; i<n; i++)
    {
        memcpy(&dest[i*14], ptr, 13);
        dest[i*14 + 13] = 0;
        ptr+=14;
    }
    memset(&dest[n*14], 0, 14*(max_num-n));
    set_admin_mode(1);
    return ptr;
}
//*******************************************************************************************************************
void set_my_phone(void)
{
    set_phones("+380713318090;", &get_phones(0)[0], 1);
    set_admin_mode(1);
}


