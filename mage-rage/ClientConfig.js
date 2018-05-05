/**
 * @file Client configuration.
 * @author Isogenic Game Engine - modified by kidthales.
 * @copyright Team Awesome 2014
 */

/**
 * Client configuration.
 * 
 * @namespace
 */
var igeClientConfig = {
  /**
   * List of paths to custom and standard game scripts, used by the client, such
   * as './gameClasses/MyClassFile.js' or './client.js'.
   * 
   * @name igeClientConfig.prototype.include
   */
  include: [// Custom game JS scripts.
      './gameClasses/BeamParticle.js',
      './gameClasses/Player.js',
      './gameClasses/ClientNetworkEvents.js',
      './gameClasses/PlayerControlComponent.js',
      './gameClasses/Wall.js',
      './gameClasses/SideEffect.js',
      './gameClasses/Lava.js',
      './gameClasses/Ice.js',
      './gameClasses/Mud.js',
      './gameClasses/Tornado.js',
      './gameClasses/Steam.js',
      './gameClasses/Lightning.js',
      './gameClasses/AttributeBox.js',
      './gameClasses/DeathParticle.js',
      './gameClasses/ClientSound.js',
      // External CDNs
      'http://code.createjs.com/createjs-2013.12.12.min.js',//'http://code.createjs.com/soundjs-0.5.2.min.js',
      // Standard game scripts.
      './client.js',
      './index.js']
};

if(typeof (module) !== 'undefined' && typeof (module.exports) !== 'undefined') {
  module.exports = igeClientConfig;
}