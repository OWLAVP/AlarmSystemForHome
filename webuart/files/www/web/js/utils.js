String.prototype.toDDHHMMSS = function() {
    var seconds = parseInt(this, 10); // don't forget the second param
    var d = Math.floor(seconds / (3600 * 24));
    var h = Math.floor(seconds % (3600 * 24) / 3600);
    var m = Math.floor(seconds % 3600 / 60);
    var s = Math.floor(seconds % 60);

    if (d < 10) {
        d = "0" + d;
    }
    if (h < 10) {
        h = "0" + h;
    }
    if (m < 10) {
        m = "0" + m;
    }
    if (s < 10) {
        s = "0" + s;
    }

    return d + 'д ' + h + 'ч ' + m + 'м ' + s + 'с';
};
/////////////////////////////////////////////////////////////////////////////////////////
function playSound(url) {
    var audio = new Audio(url);
    audio.play();
}

//////////////////////////////////////////////////////////////////
function changeSVGColorByID(elementID, color) {
    var svg = document.getElementById(elementID).contentDocument;
    var elements = svg.getElementsByTagName("path");
    for (var i = 0; i < elements.length; i++) {
        elements[i].style.fill = color;
    }
    document.getElementById(elementID).style.filter = "drop-shadow(0px 0px 6px rgba(255, 255, 255, .8))";
}