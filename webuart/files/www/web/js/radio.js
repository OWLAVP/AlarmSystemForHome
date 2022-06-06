/*!
 *  Howler.js Radio Demo
 *  howlerjs.com
 *
 *  (c) 2013-2020, James Simpson of GoldFire Studios
 *  goldfirestudios.com
 *
 *  MIT License
 */

// Cache references to DOM elements.
var elms = ['station0', 'title0', 'live0', 'station1', 'title1', 'live1', 'station2', 'title2', 'live2', 'station3', 'title3', 'live3', 'station4', 'title4', 'live4'];
elms.forEach(function(elm) {
  window[elm] = document.getElementById(elm);
});
const state_player = document.getElementById('state_player');
/**
 * Radio class containing the state of our stations.
 * Includes all methods for playing, stopping, etc.
 * @param {Array} stations Array of objects with station details ({title, src, howl, ...}).
 */
var Radio = function(stations) {
  var self = this;

  self.stations = stations;
  self.index = 0;
  
  // Setup the display for each station.
  for (var i=0; i<self.stations.length; i++) {
    window['title' + i].innerHTML = '<b>' + self.stations[i].freq + '</b> ' + self.stations[i].title;
    window['station' + i].addEventListener('click', function(index) {
      var isNotPlaying = (self.stations[index].howl && !self.stations[index].howl.playing());
      
      // Stop other sounds or the current one.
      radio.stop();

      // If the station isn't already playing or it doesn't exist, play it.
      if (isNotPlaying || !self.stations[index].howl) {
        radio.play(index);
      }
    }.bind(self, i));
  }
};
Radio.prototype = {
  /**
   * Play a station with a specific index.
   * @param  {Number} index Index in the array of stations.
   */
  play: function(index) {
    var self = this;
    var sound;

    index = typeof index === 'number' ? index : self.index;
    var data = self.stations[index];

    // If we already loaded this track, use the current one.
    // Otherwise, setup and load a new Howl.
    if (data.howl) {
      sound = data.howl;
    } else {
      sound = data.howl = new Howl({
        src: data.src,
        html5: true, // A live stream can only be played through HTML5 Audio.
        format: ['mp3', 'aac'],
        duration: 0,
        fade: [0, 1, 1000, index],
      });
    }

    // Begin playing the sound.
    sound.play();

    // Toggle the display.
    self.toggleStationDisplay(index, true);

    // Keep track of the index we are currently playing.
    self.index = index;
  },

  /**
   * Stop a station's live stream.
   */
  stop: function() {
    var self = this;

    // Get the Howl we want to manipulate.
    var sound = self.stations[self.index].howl;

    // Toggle the display.
    self.toggleStationDisplay(self.index, false);

    // Stop the sound.
    if (sound) {
      sound.unload();
    }
  },

  /**
   * Toggle the display of a station to off/on.
   * @param  {Number} index Index of the station to toggle.
   * @param  {Boolean} state true is on and false is off.
   */
  toggleStationDisplay: function(index, state) {
    var self = this;

    // Highlight/un-highlight the row.
    window['station' + index].style.backgroundColor = state ? 'rgba(255, 255, 255, 0.33)' : '';

    // Show/hide the "live" marker.
    window['live' + index].style.opacity = state ? 1 : 0;

    // Show/hide the "playing" animation.
    state_player.style.opacity = state ? '1' : '0';
    //window['playing' + index].style.display = state ? 'block' : 'none';
  }
};

function stopPlaying(){
  radio.stop();
}
// Setup our new radio and pass in the stations.
var radio = new Radio([
  {
    freq: '',
    title: "HIT FM",
    src: 'https://hitfm.hostingradio.ru/hitfm96.aacp',
    howl: null
  },
  {
    freq: '',
    title: "Europa Plus",
    src: 'https://ep256.hostingradio.ru:8052/europaplus256.mp3',
    howl: null
  },
  {
    freq: '',
    title: "Авторадио",
    src: 'https://pub0101.101.ru:8443/stream/air/mp3/256/100',
    howl: null
  },
  {
    freq: '',
    title: "Наше радио",
    src: 'https://nashe1.hostingradio.ru/nashe-256',
    howl: null
  },
  {
    freq: '',
    title: "Solo Piano",
    src: 'https://pianosolo.streamguys1.com/live',
    howl: null
  }
]);
