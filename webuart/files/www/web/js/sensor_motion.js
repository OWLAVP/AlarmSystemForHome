function CheckSensors(idElement, src, data) {
    sensorOnSVGColorByID(idElement);
    if (data.indexOf(src) > -1) {
        setSensorsIconAlarm(idElement);
        playSound('sound/bell_ring.mp3');
    } else {
        sensorOnSVGColorByID(idElement);
    }
}

///////////////////////////////////////////////////////////////////////
function setSensorsIconOff() {
    sensorOffSVGColorByID('street_mot');
    sensorOffSVGColorByID('courtyard_mot');
    sensorOffSVGColorByID('kitchen_mot');
    sensorOffSVGColorByID('garden_mot');
}

/////////////////////////////////////////////////////////////////////
function sensorOffSVGColorByID(idEl) {
    var svg = document.getElementById(idEl).contentDocument;
    var elements = svg.getElementById("path2");
    var elements2 = svg.getElementById("path4");//green
    var elements3 = svg.getElementById("path60686");//yellow
    var elements4 = svg.getElementById("path55736");//red
    elements.style.fill = "#FFFFFF49";
    elements2.style.fill = "#FFFFFF49";
    elements2.style.strokeOpacity = "0";
    elements2.style.fillOpacity = "0";
    elements3.style.strokeOpacity = "0";
    elements3.style.fillOpacity = "0";
    elements4.style.strokeOpacity = "0";
    elements4.style.fillOpacity = "0";
}

function sensorOnSVGColorByID(idEl) {
    var svg_icon = document.getElementById(idEl).contentDocument;
    document.getElementById(idEl).style.filter = "drop-shadow(0px 0px 6px rgba(255, 255, 255, .8))";
    var elements = svg_icon.getElementById('path2');
    var elements2 = svg_icon.getElementById("path4");
    var elements3 = svg_icon.getElementById("path60686");//yellow
    var elements4 = svg_icon.getElementById("path55736");
    //console.log(elements);
    elements.style.fill = "rgb(255 255 255)";
    elements2.style.fill = "#21FF00FF";
    elements2.style.strokeOpacity = "1";
    elements2.style.fillOpacity = "1";
    elements3.style.strokeOpacity = "0";
    elements3.style.fillOpacity = "0";
    elements4.style.strokeOpacity = "0";
    elements4.style.fillOpacity = "0";

}

///////////////////////////////////////////////////////////////////////
function setSensorsIconShieldYellow(idEl) {
    var svg = document.getElementById(idEl).contentDocument;
    document.getElementById(idEl).style.filter = "drop-shadow(0px 0px 6px rgba(255, 255, 255, .8))";
    var elements = svg.getElementById("path2");
    var elements2 = svg.getElementById("path4");
    var elements3 = svg.getElementById("path55736");
    var elements4 = svg.getElementById("path60686");
    elements.style.fill = "#ffffff";
    elements2.style.strokeOpacity = "0";
    elements2.style.fillOpacity = "0";
    elements3.style.strokeOpacity = "0";
    elements3.style.fillOpacity = "0";
    elements4.style.strokeOpacity = "1";
    elements4.style.fillOpacity = "1";
}

///////////////////////////////////////////////////////////////////////
function setSensorsIconAlarm(idEl) {
    var svg = document.getElementById(idEl).contentDocument;
    document.getElementById(idEl).style.filter = "drop-shadow(0px 0px 6px rgba(255, 255, 255, .8))";
    var elements = svg.getElementById("path2");
    var elements2 = svg.getElementById("path4");
    var elements3 = svg.getElementById("path55736");//red
    var elements4 = svg.getElementById("path60686");
    elements.style.fill = "#ffffff";
    elements2.style.strokeOpacity = "0";
    elements2.style.fillOpacity = "0";
    elements3.style.strokeOpacity = "1";
    elements3.style.fillOpacity = "1";
    elements4.style.strokeOpacity = "0";
    elements4.style.fillOpacity = "0";
}

//////////////////////////////////////////////////////////////////////////////////////////////////
