#include "main.h"


int main(void)
{
    _WDT_RESET();
    WDTCSR = (1<<WDE) | (1<<WDP0) | (1<<WDP3);
    if(is_btn_rst_pressed())
    {
        set_first_usage(0);
        EEPROM_update();
        cli(); // запрещаем прерывания и ждем перезагрузки по сторожевому таймеру
        while(1);
    }
    port_init();
    timer0_init();
    timer1_init();
    softuart_init();
    init_uart();
    ADC_INIT(128, INTERNAL);
    softuart_turn_rx_on();
    sei();
    softuart_puts_p(PSTR("Start...\r\n"));
    eeprom_read_config(1);

    if (setup_init())
    {
        send_USSD(1);
        softuart_puts_p(PSTR("setup_ok\r\n"));
    }
    delete_all_sms();
    init_ds18b20();
    reset_soft_wdt();
    power_control();
    //ram_free();
    softuart_puts_p(PSTR("end main"));
    beep();
    delay_ms(200);
    beep();
    delay_ms(200);
    beep();
    delay_ms(200);

    for(;;)
    {
        reset_soft_wdt();
        incoming_call_processing();
        check_guard();
        check_registration();
        check_deposit();
        power_control();
        check_temperature_smoke();
        get_sms();
        check_light();
        timer_send_state();
        check_internet();

        while(is_queue_not_empty()) // recv uart
        {
            get_message_from_mdm();
        }
        while(softuart_kbhit())// recv softuart
        {
            get_message_from_openwrt();
        }
        _SLEEP();
    }
    return 0;
}
