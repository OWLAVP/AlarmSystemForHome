function celsiumFormatter(v, axis) {
  return v.toFixed(axis.tickDecimals) + " С";
}

var optionsLastDay = {
  xaxis: {
    autoScale: "none",
    mode: "time",
    timezone: "browser",
    minTickSize: [30, "minute"],
    min: ((new Date().getTime()) / 1000 - 86400),
    max: (new Date()).getTime() / 1000,
    color: "black",
  },
  yaxes: [{
    position: "left",
    axisLabel: 'Температура',
    color: "black",
    tickFormatter: celsiumFormatter
  }],
  xaxes: [{
    mode: "time",
    timezone: "browser",
    minTickSize: [1, "hour"],
    min: ((new Date().getTime()) / 1000 - 86400),
    max: (new Date()).getTime() / 1000,
    color: "black",
    position: 'bottom',
    axisLabel: 'Время',
    show: true,
    showTickLabels: 'all',
    showMinorTicks: true,
    gridLines: true,
  }],
  grid: {
    color: "#ffffff",
    backgroundColor: {
      colors: ["#330066", "#070707"]
    },
    hoverable: true
  },
  series: {
    grow: {
      active: true,
      valueIndex: 1,
      stepDelay: 10,
      steps: 50,
      stepMode: "linear",
      stepDirection: "up"
    },
    lines: {
      show: true,
      fill: true,
      zero: false,
      lineWidth: 3,
      steps: false,
    },
    points: {
      show: false,
      radius: 0,
      symbol: "area",
    }
  },
  shadowSize: 2,
  highlightColor: "#ff0000",
  legend: {
    position: "nw",
    show: true,
  },
  tooltip: true,
  tooltipOpts: {
    content: "%s %y.0",
    //dateFormat: "%Y-%0m-%0d",
    shifts: {
      x: -30,
      y: -50
    }
  },
};
var optionsLastWeek = {
  xaxis: {
    autoScale: "none",
    mode: "time",
    timezone: "browser",
    minTickSize: [3, "hour"],
    min: ((new Date().getTime()) / 1000 - 604800),
    max: (new Date()).getTime() / 1000,
    color: "black",
  },
  yaxes: [{
    position: "left",
    axisLabel: 'Температура',
    color: "black",
    tickFormatter: celsiumFormatter
  }],
  xaxes: [{
    mode: "time",
    timezone: "browser",
    minTickSize: [3, "hour"],
    min: ((new Date().getTime()) / 1000 - 604800),
    max: (new Date()).getTime() / 1000,
    color: "black",
    position: 'bottom',
    axisLabel: 'Время',
    show: true,
    showTickLabels: 'all',
    showMinorTicks: true,
    gridLines: true,

  }, ],
  grid: {
    color: "#ffffff",
    backgroundColor: {
      colors: ["#330066", "#070707"]
    },
    hoverable: true,
    //clickable: true,
  },
  series: {
    grow: {
      active: true,
      valueIndex: 1,
      stepDelay: 10,
      steps: 50,
      stepMode: "linear",
      stepDirection: "up"
    },
    lines: {
      show: true,
      fill: true,
      zero: false,
      lineWidth: 3,
      steps: false,
    },
    points: {
      show: false,
      radius: 0,
      symbol: "area",
    }
  },
  shadowSize: 2,
  highlightColor: "#ff0000",
  legend: {
    position: "nw",
    show: true,
  },
  tooltip: true,
  tooltipOpts: {
    content: "%s %y.0",
    //dateFormat: "%Y-%0m-%0d",
    shifts: {
      x: -30,
      y: -50
    }
  }
};
var optionsLastMonth = {
  xaxis: {
    autoScale: "none",
    mode: "time",
    timezone: "browser",
    minTickSize: [1, "day"],
    min: ((new Date().getTime()) / 1000 - 2592000),
    max: (new Date()).getTime() / 1000,
    color: "black",
  },
  yaxes: [{
    position: "left",
    axisLabel: 'Температура',
    color: "black",
    tickFormatter: celsiumFormatter
  }],
  xaxes: [{
    mode: "time",
    timezone: "browser",
    minTickSize: [1, "day"],
    min: ((new Date().getTime()) / 1000 - 2592000),
    max: (new Date()).getTime() / 1000,
    color: "black",
    position: 'bottom',
    axisLabel: 'Время',
    show: true,
    showTickLabels: 'all',
    showMinorTicks: true,
    gridLines: true,

  }, ],
  grid: {
    color: "#ffffff",
    backgroundColor: {
      colors: ["#330066", "#070707"]
    },
    hoverable: true,
    //clickable: true,
  },
  series: {
    grow: {
      active: true,
      valueIndex: 1,
      stepDelay: 10,
      steps: 50,
      stepMode: "linear",
      stepDirection: "up"
    },
    lines: {
      show: true,
      fill: true,
      zero: false,
      lineWidth: 3,
      steps: false,
    },
    points: {
      show: false,
      radius: 0,
      symbol: "area",
    }
  },
  shadowSize: 2,
  highlightColor: "#ff0000",
  legend: {
    position: "nw",
    show: true,
  },
  tooltip: true,
  tooltipOpts: {
    content: "%s %y.0",
    //dateFormat: "%Y-%0m-%0d",
    shifts: {
      x: -30,
      y: -50
    }
  }
};
var optionsLastYear = {
  xaxis: {
    autoScale: "none",
    mode: "time",
    timezone: "browser",
    minTickSize: [1, "month"],
    min: ((new Date().getTime()) / 1000 - 31104000),
    max: (new Date()).getTime() / 1000,
    color: "black",
  },
  yaxes: [{
    position: "left",
    axisLabel: 'Температура',
    color: "black",
    tickFormatter: celsiumFormatter
  }],
  xaxes: [{
    mode: "time",
    timezone: "browser",
    minTickSize: [1, "month"],
    min: ((new Date().getTime()) / 1000 - 31104000),
    max: (new Date()).getTime() / 1000,
    color: "black",
    position: 'bottom',
    axisLabel: 'Время',
    show: true,
    showTickLabels: 'all',
    showMinorTicks: true,
    gridLines: true,

  }, ],
  grid: {
    color: "#ffffff",
    backgroundColor: {
      colors: ["#330066", "#070707"]
    },
    hoverable: true,
    //clickable: true,
  },
  series: {
    grow: {
      active: true,
      valueIndex: 1,
      stepDelay: 10,
      steps: 50,
      stepMode: "linear",
      stepDirection: "up"
    },
    lines: {
      show: true,
      fill: true,
      zero: false,
      lineWidth: 3,
      steps: false,
    },
    points: {
      show: false,
      radius: 0,
      symbol: "area",
    }
  },
  shadowSize: 2,
  highlightColor: "#ff0000",
  legend: {
    position: "nw",
    show: true,
  },
  tooltip: true,
  tooltipOpts: {
    content: "%s %y.0",
    //dateFormat: "%Y-%0m-%0d",
    shifts: {
      x: -30,
      y: -50
    }
  }
};

var ext_temperature = {
  label: "На улице",
  color: "#ff0066",
  points: {
    fillColor: "#ff0066"
  },
};
var int_temperature = {
  //data: sqlite2, 
  label: "MK",
  color: "#af5fff",
  points: {
    fillColor: "#af5fff"
  },
};



function showTooltip(x, y, contents) {
  $('<div id="tooltip">' + contents + '</div>').css({
    position: 'absolute',
    display: 'none',
    top: y + 5,
    left: x + 5,
    border: '1px solid #fdd',
    padding: '2px',
    'background-color': '#fee'
  }).appendTo("body").fadeIn(200);
}

var previousPoint = null;
$("#flotPlaceHolder").bind("plothover", function(event, pos, item) {
  if (item) {
    if (previousPoint != item.dataIndex) {
      previousPoint = item.dataIndex;

      $("#tooltip").remove();
      var x = item.datapoint[0].toFixed(0),
        y = item.datapoint[1].toFixed(0);

      showTooltip(item.pageX, item.pageY, "(" + x + "," + y + ")");
    }
  } else {
    $("#tooltip").remove();
    previousPoint = null;
  }
});

function updateCharts(ext_data, int_data, setting) {
  ext_temperature.data = ext_data;
  int_temperature.data = int_data;
  //console.log(ext_temperature.data);
  $.plot("#placeholder", [int_temperature, ext_temperature], setting);
}

function showCharts(period) /* функция обновления */ {
  $.ajax({
    async: true,
    type: "GET",
    url: "from_db.php?period=" + period,
    contentType: "application/json; charset=utf-8",
    dataType: "json",
    timeout: 1000,
    cache: false,
    error: function(req, err) {
      console.log('db = ' + err);
    },
    success: function(response) {
      var res_1 = [];
      var res_2 = [];
      //console.log(response);
      for (var i in response) {
        var temp = response[i];

        if (temp[0] === 1) {
          //console.log(JSON.stringify([temp[1], temp[2]]));
          // expected output: "[57,63]"
          res_1.push([temp[1], temp[2]]);
        } else if (temp[0] === 2)
          res_2.push([temp[1], temp[2]]);
      }
      if (period === "lastday") {
        updateCharts(res_2, res_1, optionsLastDay);
      } else if (period === "lastweek") {
        updateCharts(res_2, res_1, optionsLastWeek);
      } else if (period === "lastmonth") {
        updateCharts(res_2, res_1, optionsLastMonth);
      } else if (period === "lastyear") {
        updateCharts(res_2, res_1, optionsLastYear);
      }
      //updateCharts(res_2, res_1, period);
      //console.log(res_1);
    }
  });
}