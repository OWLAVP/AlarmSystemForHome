var btnLamp = document.getElementById("lampCheck");
var elementMain = document.getElementById("elementMain"); //main
var elementStrokeGlass = document.getElementById("elementGlass"); //Stroke glass
var elementFilament = document.getElementById("elementFilament"); //filament
var elementFilamentAfter = document.getElementById("elementFilamentAfter");  //filament after
var elementsRays = document.getElementById("rays"); //rays
var id_interval = 0;


function hoverBtnLamp() {
   btnLamp.onmouseover = function() {
      hover_lamp_state = 1;
      if(lamp_on_off === 0){
         elementMain.style.stroke = "#ffffff";
         animateOn();         
      }
   };
   btnLamp.onmouseleave = function() {
      hover_lamp_state = 0;
      if(lamp_on_off === 0){
         elementMain.style.stroke = "#ffffff4d";
         animateOff();
      }
   };
}


function LampOff() {
   elementMain.style.stroke = "#ffffff4d";

   elementStrokeGlass.style.strokeOpacity = "0";

   elementFilament.style.strokeDasharray = "0,21 0,21";
   elementFilament.style.filter = "drop-shadow(0px 0px 0px rgba(255, 255, 255, 0))";

   elementsRays.style.strokeDasharray = "0,6 0,6";
   elementsRays.style.filter = "drop-shadow(0px 0px 0px rgba(255, 255, 255, 0))";
}

function LampOn() {
   elementMain.style.stroke = "#2e3289";

   elementStrokeGlass.style.strokeOpacity = "1";

   clearInterval(id_interval);

   elementFilament.style.filter = "drop-shadow(0px 0px 2px rgba(255, 255, 255, .7))";
   elementFilament.style.strokeDasharray = "0,0 42,0"; //
   elementFilament.style.strokeOpacity = "1";

   elementsRays.style.strokeDasharray = "0,0 12,0";
   elementsRays.style.strokeOpacity = "1";
   elementsRays.style.stroke = "#feff00";
   elementsRays.style.filter = "drop-shadow(0px 0px 2px rgba(255, 255, 255, .7))";
}

function switchLamp() {
   if (btnLamp.checked == true) {
      LampOn();
   } else {
      LampOff();
      clearInterval(id_interval);
   }
}

function animateOn() {
    clearInterval(id_interval);
    elementFilament.style.strokeDasharray = "0,0 21,0 0,0 21,0";
    id_interval = setInterval(animateFilament, 350);
}

function animateOff() {
    clearInterval(id_interval);
    elementFilament.style.strokeDasharray = "0,10 0,10 0,11 0,11";
}

function animateFilament(){
   var state = elementFilament.style.strokeDasharray;
   if(state === "0px, 10px, 0px, 10px, 0px, 11px, 0px, 11px" || state === "0, 10, 0, 10, 0, 11, 0, 11"){
      elementFilament.style.strokeDasharray = "0,0 21,0 0,0 21,0";
   }
   else if(state === "0px, 0px, 21px, 0px, 0px, 0px, 21px, 0px" || state === "0, 0, 21, 0, 0, 0, 21, 0"){
      elementFilament.style.strokeDasharray = "0,10 0,10 0,11 0,11";
   }
}