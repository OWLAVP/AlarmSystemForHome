var guard_on_off = 0;
var local_guard_on_off = 0;
var lamp_on_off = 0;
var buzzer_on_off = 0;

var len_answer = 7; /* количество блоков разделённых & в ответе от MK */
var flagSendCmd = 0;

var lastDeposit = "";
var last_state_guard = "";
var last_state_local_guard = "";
var last_state_temp_int = "";
var last_state_temp_ext = "";
var last_level_signal_gsm = "";
var last_state_battery = "";
var last_err_mcu = "";
var last_state_lamp = "";
var last_state_buzzer = "";
var last_state_mcu = "";
var last_state_motion_insise = "";
var state_sensor = "";
var vars = "";

var hover_lamp_state = 0;
var pause_lamp_upd = 0;
var hover_bell_state = 0;
var pause_bell_upd = 0;


const guard = document.getElementById('guard');
const local_guard = document.getElementById("local_guard");
const elementIdTimeNow = document.getElementById('time_now');
const depositElement = document.getElementById('deposit');
const int_temp = document.getElementById('int_temp');
const ext_temp = document.getElementById('ext_temp');
const icon_state_temp_mcu = document.querySelector('.icon-state-temp-mcu');
const dataBattery = document.getElementById('data_battery');
const svg_ext_temp = document.getElementById('svg_ext_temp');


const reg_mdm = document.querySelector('.no_signal');
const signal_gsm_strip_1 = document.querySelector('.strip_1');
const signal_gsm_strip_2 = document.querySelector('.strip_2');
const signal_gsm_strip_3 = document.querySelector('.strip_3');
const signal_gsm_strip_4 = document.querySelector('.strip_4');
const signal_gsm_strip_5 = document.querySelector('.strip_5');


const btn_lamp = document.getElementById('btn_lamp');
const checkbox_guard = document.getElementById('checkbox_guard');
const checkbox_local_guard = document.getElementById('checkbox_local_guard');
const btn_buzzer = document.getElementById('btn_buzzer');

const iconStateDeposit = document.getElementById('icon-state-deposit');
const btnRefreshDeposit = document.getElementById("refresh_balance");
const spinner_balance = document.getElementById("spinner_upd_dep");
const modalUpBalance = document.getElementById('modal_deposit');
const closeModal = document.querySelector('.close-modal');


const btnSendBalanceCode = document.getElementById('btn_send_balance_code');
const balanceCode = document.getElementById('balance_code');

const btn_reboot = document.getElementById('btn_reboot');
const modal_reboot = document.getElementById('modal_reboot');
const btn_modal_reboot_gsm = document.getElementById("btn_reboot_gsm");
const btn_modal_reboot_mcu_gsm = document.getElementById("btn_reboot_mcu_gsm");
const modal_busy_mcu = document.getElementById("modal_busy_mcu");

const btn_clock_work_mcu = document.getElementById("btn_clock_work_mcu");
const div_clock_work_mcu = document.getElementById("clock_work_mcu");
const close_clock_work_mcu = document.getElementById("close_clock_work_mcu");

const icon_error_mcu = document.getElementById("icon-error-mcu");//
const btn_error_mcu = document.getElementById("err_gsm");
const div_error_mcu = document.getElementById("error_mcu");
const close_error_mcu = document.getElementById("close_error_mcu");

const btn_balance = document.getElementById("btn_balance");
const div_balance = document.getElementById("balance");
const close_div_balance = document.getElementById("close_div_balance");

const btn_modal_chart = document.getElementById("btn_modal_chart");
const div_chart = document.getElementById("chart");
const close_chart = document.getElementById("close-chart");

const iconStateBattery = document.getElementById("icon-state-battery");
