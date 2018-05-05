/**
 * @file Client sound object definitions.
 * @author kidthales
 * @copyright Team Awesome 2014
 */

/**
 * Singleton client sound load status object.
 * 
 * @class ClientSound
 */
var ClientSound = {
  /**
   * The path to the audio assets.
   * 
   * @name ClientSound.prototype.path
   * @public
   */
  path: './assets/audio/',

  /**
   * The audio resource manifest.
   * 
   * @name ClientSound.prototype.manifest
   * @public
   */
  manifest: [{
    id: 'music', // Ok
    src: './assets/audio/mage_rage_theme.ogg'
  }, {
    id: 'beamBeamContact', // No
    src: './assets/audio/beam_beam_contact.ogg'
  }, {
    id: 'beamFire', // Ok
    src: './assets/audio/beam_fire.ogg'
  }, {
    id: 'beamPlayerContact', // beam destroy sound
    src: './assets/audio/beam_player_contact.ogg'
  }, {
    id: 'playerDeath', // Ok
    src: './assets/audio/player_death.ogg'
  }, {
    id: 'tornado', // Ok
    src: './assets/audio/tornado.ogg'
  }],

  /**
   * Load status object for music, beamBeamContact, beamFire, beamPlayerContact,
   * playerDeath, and tornado audio resources.
   * 
   * @name ClientSound.prototype.status
   * @public
   */
  status: {
    music: false,
    beamBeamContact: false,
    beamFire: false,
    beamPlayerContact: false,
    playerDeath: false,
    tornado: false
  },

  /**
   * Music load interval playing flag.
   * 
   * @name ClientSound.prototype.musicLoadIntervalPlaying
   * @public
   */
  musicLoadIntervalPlaying: false,

  /**
   * Initialize the client sound object.
   * 
   * @name ClientSound.prototype.init
   * @function
   */
  init: function() {
    if(!createjs.Sound.initializeDefaultPlugins()) { return; }

    var self = this;

    createjs.Sound.alternateExtensions = ['mp3'];

    createjs.Sound.addEventListener("fileload", function(event) {
      self.status[event.id] = true;
      console.log('Audio file (' + event.src + ') loaded successfully');
    });

    createjs.Sound.registerManifest(this.manifest);

    // This fails to notify when mage_rage_theme.ogg loads.
    /*
     * var queue = new createjs.LoadQueue();
     * 
     * queue.installPlugin(createjs.Sound);
     * 
     * queue.addEventListener('fileload', function(event) {
     * self.status[event.item.id] = true; console.log('Audio file (' +
     * event.item.src + ') loaded successfully'); });
     * 
     * queue.loadManifest(this.manifest);
     */
  }
};

if(typeof (module) !== 'undefined' && typeof (module.exports) !== 'undefined') {
  module.exports = ClientSound;
}