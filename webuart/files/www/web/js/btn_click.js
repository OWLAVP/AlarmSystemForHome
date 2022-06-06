function BtnClick() {
    "use strict";
    ///////////////////////////lamp/////////////////////////////////////////////////////
    btn_lamp.onclick = function() {
        window.setTimeout(resetPause, 1500);
        pause_lamp_upd = 1;
        hover_lamp_state = 0;
        if (connected == true)
        {
            var cmd_str = "lamp=" + (lamp_on_off === 1 ? 0 : 1);
            ws.send(cmd_str);
            console.log(cmd_str);
        }
    };

    function resetPause() {
        pause_bell_upd = 0;
        pause_lamp_upd = 0;
        setPauseSwitch(0);
        elementBell.style.animationName = "none";
        elementRightLower.style.animationName = "none";
        elementLeftLower.style.animationName = "none";
    }
    ///////////////////////////buzzer/////////////////////////////////////////////////////
    btnBell.onclick = function() {
        elementBell.style.animationName = "ring";
        elementRightLower.style.animationName = "stroke_opacity_in";
        elementLeftLower.style.animationName = "stroke_opacity_in";
        window.setTimeout(resetPause, 1000);
        pause_bell_upd = 1;
        hover_bell_state = 0;
        if (connected == true)
        {
            var cmd_str = "buzzer=" + (buzzer_on_off === 1 ? 0 : 1);
            ws.send(cmd_str);
            console.log(cmd_str);
        }
    };
    

    //////////////////////    guard.onclick = function() {////////////////////////////////
    checkbox_guard.onclick = function() {
        playSound('sound/button_click_on.mp3');

        if (connected == true)
        {
            var cmd_str = "main_guard=" + (guard_on_off === 1 ? 0 : 1);
            ws.send(cmd_str);
            console.log(cmd_str);
        }
        window.setTimeout(resetPause, 2000);
        setPauseSwitch(1);
        last_state_guard = "";
        flagSendCmd = 1;        
    };
    ////////////////////local_guard//////////////////////////////////
    checkbox_local_guard.onclick = function() {
        //playSound('sound/button_click_on.mp3');
        //setCookie('username', 'John', {secure: true, 'max-age': 3600});
        if (connected == true)
        {
            var cmd_str = "l_guard=" + (local_guard_on_off === 1 ? 0 : 1);
            ws.send(cmd_str);
            console.log(cmd_str);
        }
        window.setTimeout(resetPause, 2000);
        setPauseSwitch(1);
        last_state_local_guard = "";
        flagSendCmd = 1;
    };

    function TimerBtnGuard() {
        modal_busy_mcu.style.display = "none";
        flagSendCmd = 0;
    }

    //////////////////////////////////modal reboot//////////////////////////////////////////////////
    btn_modal_reboot_mcu_gsm.onclick = function() {
        //playSound('sound/button_click_on.mp3');
        if (connected == true)
        {
            var cmd_str = "reboot_all";
            ws.send(cmd_str);
            modal_busy_mcu.style.display = "block";
            flagSendCmd = 1;
            window.setTimeout(TimerBtnGuard, 10000);            
            console.log(cmd_str);
        }       

    };
    btn_modal_reboot_gsm.onclick = function() {
        //playSound('sound/button_click_on.mp3');
        if (connected == true)
        {
            var cmd_str = "reboot_gsm";
            ws.send(cmd_str);
            modal_busy_mcu.style.display = "block";
            flagSendCmd = 1;
            window.setTimeout(TimerBtnGuard, 10000);            
            console.log(cmd_str);
        }        
    };
    /////////////////////////send code//////////////////////////////////////////////
    btnSendBalanceCode.onclick = function() {
        playSound('sound/button_click_on.mp3');
        var strCode = balanceCode.value;
        //console.log(strCode);
        if (connected == true)
        {
            var cmd_str = "code=" + strCode;
            ws.send(cmd_str);
            modalUpBalance.style.display = "none";
            modal_busy_mcu.style.display = "block";
            flagSendCmd = 1;
            window.setTimeout(TimerBtnGuard, 5000);          
            console.log(cmd_str);
        }         
    };
}
//****************************************************************
function balanceDivOpen() {
    var state = div_balance.style.display;
    if (state.toString() === "block") {
        div_balance.style.display = "none";
    } else {
        div_balance.style.display = "block";
    }
}
function balanceDivClose() {
    div_balance.style.display = "none";
}
//****************************************************************
function errorMCUOpen() {
    var state = div_error_mcu.style.display;
    if (state.toString() === "block") {
        div_error_mcu.style.display = "none";
        console.log("close");
    } else {
        div_error_mcu.style.display = "block";
        console.log("open");
    }
}
function errorMCUClose() {
    div_error_mcu.style.display = "none";
}
//****************************************************************
function timeMCUOpen() {
    var state = div_clock_work_mcu.style.display;
    console.log("press");
    if (state.toString() === "block") {
        div_clock_work_mcu.style.display = "none";
    } else {
        div_clock_work_mcu.style.display = "block";
    }
}
function timeMCUClose() {
    div_clock_work_mcu.style.display = "none";
}
/////////////////////////btnRefreshDeposit////////////////////////////////////// 
function refreshDeposit(){
    if (connected == true)
    {
        ws.send("USSD");
        btnRefreshDeposit.classList.add('display_hide');
        spinner_balance.classList.remove('display_hide');
        window.setTimeout(TimerRefreshDeposit, 20000);        
        console.log(cmd_str);
    }      
}

function TimerRefreshDeposit() { // функция таймера (подсчёт количества секунд)
    btnRefreshDeposit.classList.remove('display_hide');
    spinner_balance.classList.add('display_hide');
}
//******************************************************************************
/////////////////////////btnUpDeposit////////////////////////////////////// 
function upDeposit(){
    var state = modalUpBalance.style.display;
    if (state.toString() === "block") {
        modalUpBalance.style.display = "none";
    } else {
        modalUpBalance.style.display = "block";
        balanceDivClose();
    }
}

//******************************************************************************

function chartOpen() {
    var state = div_chart.style.display;
    if (state.toString() === "block") {
        div_chart.style.display = "none";
    } else {
        div_chart.style.display = "block";
        showCharts('lastday');
    }
}
function chartClose() {
    div_chart.style.display = "none";
}
//***************************************************************************
function modalClose(){
    modalUpBalance.style.display = "none";
}
//***************************************************************************
function btnRebootDivOpen() {
    var state = modal_reboot.style.display;
    if (state.toString() === "block") {
        modal_reboot.style.display = "none";
    } else {
        modal_reboot.style.display = "block";
        timeMCUClose();
    }
}
function btnRebootDivClose() {
    modal_reboot.style.display = "none";
}

