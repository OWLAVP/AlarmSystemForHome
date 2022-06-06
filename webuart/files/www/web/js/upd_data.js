var motion_sensors = "";
var state_local_guard = "";
var pause_switch = 0;

function setPauseSwitch(state_switch){
    pause_switch = state_switch;
    
}
//////////////////////////////////////////////////////////////////////////////////////////
function checkDeposit() {
    var strDeposit = vars[0];
    var intDeposit = strDeposit.match(/-?\d+\.\d+|-?\d+/);
    if (lastDeposit !== strDeposit) {
        lastDeposit = strDeposit;
        if (intDeposit) {
            depositElement.innerHTML = "На сим-карте " + intDeposit + " Руб";
        }
        if (intDeposit < 2) {
            depositElement.style.color = '#ff000c';
            iconStateDeposit.classList.add('exclamation_point_top_bar');
        } else {
            depositElement.style.color = '#ffffff';
            iconStateDeposit.classList.remove('exclamation_point_top_bar');
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
function checkGuard() {
    motion_sensors = vars[9];
    var state_guard = vars[1];

//    if (last_state_guard === state_guard && state_sensor === motion_sensors && last_state_local_guard === state_local_guard) {
//        return;
//    }
    if(pause_switch === 1){
        return;
    }
    last_state_guard = state_guard;
    state_sensor = motion_sensors;
    //console.log(state_guard);
    if (state_guard === "g=1") {
        //guard.innerHTML = "<text_guard>Активирован режим охраны</text_guard>";
        CheckSensors("kitchen_mot", "s=1", motion_sensors);
        CheckSensors('courtyard_mot', "s=2", motion_sensors);
        CheckSensors('street_mot', 's=3', motion_sensors);
        CheckSensors('garden_mot', "s=4", motion_sensors);
        document.getElementById("checkbox_guard").checked = true;
        document.getElementById("checkbox_local_guard").checked = false;
        guard_on_off = 1;
        local_guard_on_off = 0;
        if(motion_sensors === "s=0"){
            document.getElementById('top-bar').classList.remove('alarm-background');   
        }
        else{
            document.getElementById('top-bar').classList.add('alarm-background');
        }
        //console.log("g1");
    } else if (state_guard === 'g=0') {
        //guard.innerHTML = "<text_guard>Охрана отключена</text_guard>";
        setSensorsIconOff();
        document.getElementById("checkbox_guard").checked = false;
        guard_on_off = 0;
        checkLocalGuard();
    }

}
///////////////////////////////////////////////////////////////////////////////////
function checkLocalGuard() {
    state_local_guard = vars[2].toString();
    //console.log(pause_switch);
    if(pause_switch === 1){
        return;
    }
    last_state_local_guard = state_local_guard;
    //console.log(state_local_guard);
    if (state_local_guard === 'lg=1') {
        sensorOffSVGColorByID('kitchen_mot');
        sensorOffSVGColorByID('courtyard_mot');
        CheckSensors('street_mot', 's=3', motion_sensors);
        CheckSensors('garden_mot', 's=4', motion_sensors);
        //local_guard.innerHTML = "<text_guard>активирован</text_guard>";
        //local_guard.style.borderColor = "#00ff80";
        local_guard_on_off = 1;
        document.getElementById("checkbox_local_guard").checked = true;
        if(motion_sensors === "s=3" || motion_sensors === "s=4"){
            document.getElementById('top-bar').classList.add('alarm-background');   
        }
        else{
            document.getElementById('top-bar').classList.remove('alarm-background');
        }
        //////////////////////////////motion inside////////////////////////////////////////////////////
    } else if (state_local_guard === 'lg=0') {
        //local_guard.innerHTML = "<text_guard>отключен</text_guard>";
        //local_guard.style.borderColor = "#ff0000";
        local_guard_on_off = 0;
        document.getElementById("checkbox_local_guard").checked = false;
        setSensorsIconOff();
        document.getElementById('top-bar').classList.remove('alarm-background'); 
    }

}

function checkMotionInside() {
    if (state_local_guard === 'lg=1') {
        if (last_state_motion_insise === vars[12].toString()) {
            return;
        }
        last_state_motion_insise = vars[12].toString;
        if (vars[12] === 'mi=1') {
            setSensorsIconShieldYellow('street_mot');
            setSensorsIconShieldYellow('garden_mot');
        } else if (vars[12] === 'mi=3') {
            CheckSensors('street_mot', 's=3', motion_sensors);
        } else if (vars[12] === 'mi=4') {
            CheckSensors('garden_mot', 's=4', motion_sensors);
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////
function checkTemperatureMCU() {
    var temp_int = vars[3];
    var res_temp_int = temp_int.match(/-?\d+/);
    if (res_temp_int.toString() !== last_state_temp_int) {
        last_state_temp_int = res_temp_int.toString();
        if (res_temp_int) {
            int_temp.innerHTML = res_temp_int.toString();
        }
        if (res_temp_int > 65) {
            icon_state_temp_mcu.style.color = '#ff000c';
            icon_state_temp_mcu.classList.add('data_avr');
            icon_state_temp_mcu.classList.add('exclamation_point_top_bar');
        } else {
            icon_state_temp_mcu.classList.remove('data_avr');
            icon_state_temp_mcu.classList.remove('exclamation_point_top_bar');
            icon_state_temp_mcu.style.color = '#ffffff';
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////
function checkExtTemperature() {
    var temp_ext = vars[4];
    var res_temp_ext = temp_ext.match(/-?\d+/);

    if (res_temp_ext.toString() !== last_state_temp_ext) {
        last_state_temp_ext = temp_ext;
        if (res_temp_ext > 0) {
            ext_temp.innerHTML = "+ " + res_temp_ext;
            ext_temp.style.color = "#ffffff";
            changeSVGColorByID("svg_ext_temp", "white");
            if (res_temp_ext > 35) {
                ext_temp.style.color = "#ffa03a";
                changeSVGColorByID("svg_ext_temp", "#ffa03a");
            }
        } else if (res_temp_ext < 0) {
            ext_temp.innerHTML = res_temp_ext.toString();
            ext_temp.style.color = "#9122ff";
            changeSVGColorByID("svg_ext_temp", "#9122ff");
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////
function checkSignalGSM() {
    var signal = vars[6];
    var res_signal = signal.match(/\d+/);
    if (vars[7] === 'r=1' && last_level_signal_gsm !== vars[7]) {
        last_level_signal_gsm = vars[7];
        reg_mdm.innerHTML = "";
        if (res_signal > 80) {
            signal_gsm_strip_1.style.backgroundColor = '#ffffff';
            signal_gsm_strip_2.style.backgroundColor = '#ffffff';
            signal_gsm_strip_3.style.backgroundColor = '#ffffff';
            signal_gsm_strip_4.style.backgroundColor = '#ffffff';
            signal_gsm_strip_5.style.backgroundColor = '#ffffff';
        } else if (res_signal > 60) {
            signal_gsm_strip_1.style.backgroundColor = '#ffffff';
            signal_gsm_strip_2.style.backgroundColor = '#ffffff';
            signal_gsm_strip_3.style.backgroundColor = '#ffffff';
            signal_gsm_strip_4.style.backgroundColor = '#ffffff';
            signal_gsm_strip_5.style.backgroundColor = '#FFFFFF49';
        } else if (res_signal > 40) {
            signal_gsm_strip_1.style.backgroundColor = '#ffffff';
            signal_gsm_strip_2.style.backgroundColor = '#ffffff';
            signal_gsm_strip_3.style.backgroundColor = '#ffffff';
            signal_gsm_strip_4.style.backgroundColor = '#FFFFFF49';
            signal_gsm_strip_5.style.backgroundColor = '#FFFFFF49';
        } else if (res_signal > 20) {
            signal_gsm_strip_1.style.backgroundColor = '#ffffff';
            signal_gsm_strip_2.style.backgroundColor = '#ffffff';
            signal_gsm_strip_3.style.backgroundColor = '#FFFFFF49';
            signal_gsm_strip_4.style.backgroundColor = '#FFFFFF49';
            signal_gsm_strip_5.style.backgroundColor = '#FFFFFF49';
        } else if (res_signal > 0) {
            signal_gsm_strip_1.style.backgroundColor = '#e01b24';
            signal_gsm_strip_2.style.backgroundColor = '#FFFFFF49';
            signal_gsm_strip_3.style.backgroundColor = '#FFFFFF49';
            signal_gsm_strip_4.style.backgroundColor = '#FFFFFF49';
            signal_gsm_strip_5.style.backgroundColor = '#FFFFFF49';
        }
        //reg_mdm.innerHTML = "OK";
        //reg_mdm.classList.remove('blink');
    } else if (vars[7] === 'r=0' && last_level_signal_gsm !== vars[7]) {
        last_level_signal_gsm = vars[7];
        signal_gsm_strip_1.style.backgroundColor = '#FFFFFF49';
        signal_gsm_strip_2.style.backgroundColor = '#FFFFFF49';
        signal_gsm_strip_3.style.backgroundColor = '#FFFFFF49';
        signal_gsm_strip_4.style.backgroundColor = '#FFFFFF49';
        signal_gsm_strip_5.style.backgroundColor = '#FFFFFF49';
        reg_mdm.innerHTML = "&times;";
    }
}

////////////////////////////////////////////////////////////////////////////////////
function checkBatteryLevel() {
    var battery = vars[8];
    var res_battery = battery.match(/\d+/);
    if (last_state_battery === res_battery.toString()) {
        return;
    }
    last_state_battery = res_battery.toString();
    if (res_battery) {
        dataBattery.textContent = res_battery + "%";
        if (res_battery > 100) {
            dataBattery.style.display = "none";
            return;
        }
        dataBattery.style.display = "block";
        if (res_battery < 25) {
            dataBattery.style.color = "#FF0000FF";
            iconStateBattery.classList.add('exclamation_point_top_bar');
        } else {
            dataBattery.style.color = "#ffffff";
            iconStateBattery.classList.remove('exclamation_point_top_bar');
        }
    }
}
////////////////////////////////////////////////////////////////////////////////////
function checkErrorMCU() {
    var str_err_gsm = vars[5];
    var res_err_gsm = str_err_gsm.match(/\d+/);

    if (res_err_gsm && last_err_mcu !== res_err_gsm.toString()) {
        last_err_mcu = res_err_gsm.toString();
        btn_error_mcu.textContent = res_err_gsm.toString();
    }
}

///////////////////////////////////////////////////////////////////////////////////
function checkStateLamp() {
    var state_lamp = vars[10].toString();
/*    if (last_state_lamp === state_lamp) {
        return;
    }*/
    if(pause_lamp_upd === 1){
        return;
    }
    if(hover_lamp_state === 1){
        return;
    }
    last_state_lamp = state_lamp;
    if (state_lamp === 'l=2') {
        lamp_on_off = 1;
        //console.log(state_lamp);
        document.getElementById("lampCheck").checked = true;
        switchLamp();
    } else {
        document.getElementById("lampCheck").checked = false;
        switchLamp();
        lamp_on_off = 0;
    }
}

///////////////////////////////////////////////////////////////////////////////////
function checkStateBuzzer() {
    var state_buzzer = vars[11];
/*    if (last_state_buzzer === state_buzzer) {
        return;
    }*/

    //console.log(state_buzzer);
    if(pause_bell_upd === 1){
        return;
    }
    if(hover_bell_state === 1){
        return;
    }
    last_state_buzzer = state_buzzer;
    if (state_buzzer === 'z=1') {
        buzzer_on_off = 1;
        document.getElementById("bellCheck").checked = true;
        RingOn();
        //console.log("on");
    } else {
        buzzer_on_off = 0;
        document.getElementById("bellCheck").checked = false;
        RingOff();
    }
}

//////////////////////////////////////////////////////////////////////////////////
function checkBusyMCU() {
    var state_mcu = vars[13];
    if (last_state_mcu === state_mcu) {
        return;
    }
    last_state_mcu = state_mcu;
    if (vars[13] === 'bm=1' && flagSendCmd === 0) {
        document.getElementById("modal_busy_mcu").style.display = "block";
    } else {
        document.getElementById("modal_busy_mcu").style.display = "none";
    }
}

////////////////////////////////////////////////////////////////////////////////////
function checkTimeMCU() {
    var timeAVR = vars[14];
    var res_timeAVR = timeAVR.match(/\d+/);
    if (res_timeAVR) {
        document.getElementById("timeAVR").innerHTML = res_timeAVR.toString().toDDHHMMSS();
    } else {
        document.getElementById("timeAVR").innerHTML = "0";
    }

}

//////////////////////////////////////////////////////////////////////////////////
function ShowData(data) {
    "use strict";
    vars = data.split("&"); /* разбор строки принятой от MK */
    /*    var online = document.getElementById("online");
        if (dataAVR.indexOf("not_msg_from_avr") > -1) {
            online.innerHTML = "<online_tag>оффлайн</online_tag>";
            online.style.color = '#ff0000';
            online.classList.add('blink_text');
        } else {
            online.innerHTML = "<online_tag>онлайн</online_tag>";
            online.style.color = '#00ff80';
            online.classList.remove('blink_text');
        }*/

    /////////////////////local time///////////////////////////////////////////////////////////////
    var timeNow = new Date().toLocaleString();
    elementIdTimeNow.innerHTML = timeNow.toString();
    ////////////////////////////////////////////////////////////////////////////////////////////////
    if (vars.length > len_answer) {
        ////////////////////deposit/////////////////////////////////////////////////////////////////////////
        checkDeposit();
        /////////////////////////////////sensors////////////////////////////////////////////////////////////////
        checkGuard();
        checkMotionInside();
        //////////////////////////////////mcu_temperature/////////////////////////////////////////////////////////
        checkTemperatureMCU();
        ////////////////////////////////////ext temperature///////////////////////////////////////////////////////////////
        checkExtTemperature();
        ///////////////////////////registration gsm////////////////////////////////////////////
        checkSignalGSM();
        ///////////////////////////////////battery//////////////////////////////////////////////////////////////////////////////////
        checkBatteryLevel();
        ///////////////////////////////////error mcu////////////////////////////////////////////////////////
        checkErrorMCU();
        //////////////////////////////light////////////////////////////////////////////////////
        checkStateLamp();
        //////////////////////////////buzzer////////////////////////////////////////////////////
        checkStateBuzzer();
        //////////////////////////////busy_mdm////////////////////////////////////////////////////
        checkBusyMCU();
        ////////////////////////////////time mcu/////////////////////////////////////////////////////////////////////////////////////
        checkTimeMCU();
    }
    //console.log(vars);
}
