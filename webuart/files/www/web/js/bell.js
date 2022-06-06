var btnBell = document.getElementById("bellCheck");
var elementBell = document.getElementById("elementBell"); //elementBell
var elementMainBell = document.getElementById("elementMainBell"); //main
var elementMainBellAfter = document.getElementById("elementMainBellAfter");
var elementInHTop = document.getElementById("elementInHTop"); //Stroke glass
var elementRightLower = document.getElementById("rightLower"); //filament
var elementRightLarge = document.getElementById("rightLarge"); //rightLarge
var elementLeftLower = document.getElementById("leftLower"); //filament
var elementLeftLarge = document.getElementById("leftLarge"); //rightLarge
var elementTongue = document.getElementById("elementTongue"); //Tongue
var elementInVerticalCenter = document.getElementById("elementInVerticalCenter"); //elementInVerticalCenter
var elementInHBottomRight = document.getElementById("elementInHBottomRight"); //elementInHBottomRight

var id_interval = 0;


function hoverBtnBell() {
   btnBell.onmouseover = function() {
      hover_bell_state = 1;
      if(buzzer_on_off === 1){
         elementBell.style.animationName = "none";
         elementTongue.style.animationName = "none";
         elementInHBottomRight.style.animationName = "none";
         elementInHBottomRight.style.strokeOpacity = "0.3";
         elementInHTop.style.animationName = "none";
         elementInHTop.style.strokeOpacity = "0.3";
         hover_bell_state = 1;
      }
      else{
         elementRightLower.style.strokeOpacity = "1";
         elementLeftLower.style.strokeOpacity = "1";
      }
   };
   btnBell.onmouseleave = function() {
      hover_bell_state = 0;
      if(buzzer_on_off === 1){
         
         elementBell.style.animationName = "ring";
         elementTongue.style.animationName = "move";
         elementInHBottomRight.style.animationName = "stroke_move_bottom"; //stroke_move_top
         elementInHTop.style.animationName = "stroke_move_bottom";
         elementInHBottomRight.style.strokeOpacity = "1";
         elementInHTop.style.strokeOpacity = "1";
      }
      else{
         elementRightLower.style.strokeOpacity = "0";
         elementLeftLower.style.strokeOpacity = "0";
      }      
   };
}


function RingOff() {
   elementMainBell.style.strokeDasharray = "0,154 0,154";
   elementBell.style.animationName = "none";
   elementTongue.style.animationName = "none";
   elementLeftLarge.style.animationName = "none";
   elementRightLarge.style.animationName = "none";
   elementRightLower.style.animationName = "none";
   elementLeftLower.style.animationName = "none";
   elementInHBottomRight.style.animationName = "none";
   elementInHTop.style.animationName = "none";
   elementTongue.style.strokeOpacity = "0";
   elementTongue.style.transition = "stroke-opacity 0.5s step-start";
   elementInHBottomRight.style.strokeOpacity = "0.3";
   elementInHTop.style.strokeOpacity = "0.3";
}

function RingOn() {
   elementMainBell.style.strokeDasharray = "0,0 308,0";
   elementBell.style.animationName = "ring";
   elementTongue.style.animationName = "move";
   elementLeftLarge.style.animationName = "stroke_opacity_out";
   elementRightLarge.style.animationName = "stroke_opacity_out";
   elementRightLower.style.animationName = "stroke_opacity_in";
   elementLeftLower.style.animationName = "stroke_opacity_in";
   elementInHBottomRight.style.animationName = "stroke_move_bottom"; //stroke_move_top
   elementInHTop.style.animationName = "stroke_move_bottom";
   elementTongue.style.strokeOpacity = "1";
   elementTongue.style.transition = "stroke-opacity 0.5s step-end";
   elementInHBottomRight.style.strokeOpacity = "1";
   elementInHTop.style.strokeOpacity = "1";
}

function switchState() {
   if (btnBell.checked == true) {
      RingOn();
   } else {
      RingOff();
   }
}