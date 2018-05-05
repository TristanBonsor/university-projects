/**
 * @file Server configuration.
 * @author Isogenic Game Engine - modified by kidthales.
 * @copyright Team Awesome 2014
 */

/**
 * Server configuration.
 * 
 * @namespace
 */
var config = {
  /**
   * List of id's and paths to custom and standard game scripts, used by the
   * server, such as {name: 'MyClassName', path:
   * './gameClasses/MyClassFileName'}
   * 
   * @name config.prototype.include
   */
  include: [
  // ,
  {
    name: 'BeamParticle',
    path: './gameClasses/BeamParticle'
  }, {
    name: 'EntityContactResolver',
    path: './gameClasses/EntityContactResolver'
  }, {
    name: 'Player',
    path: './gameClasses/Player'
  }, {
    name: 'PlayerControlComponent',
    path: './gameClasses/PlayerControlComponent'
  }, {
    name: 'ServerNetworkEvents',
    path: './gameClasses/ServerNetworkEvents'
  }, {
    name: 'Wall',
    path: './gameClasses/Wall'
  }, {
    name: 'SideEffectEmitter',
    path: './gameClasses/SideEffectEmitter'
  }, {
    name: 'SideEffect',
    path: './gameClasses/SideEffect'
  }, {
    name: 'Lava',
    path: './gameClasses/Lava'
  }, {
    name: 'Ice',
    path: './gameClasses/Ice'
  }, {
    name: 'Mud',
    path: './gameClasses/Mud'
  }, {
    name: 'Tornado',
    path: './gameClasses/Tornado'
  }, {
    name: 'Steam',
    path: './gameClasses/Steam'
  }, {
    name: 'Lightning',
    path: './gameClasses/Lightning'
  }, {
    name: 'DeathParticle',
    path: './gameClasses/DeathParticle'
  }]
};

if(typeof (module) !== 'undefined' && typeof (module.exports) !== 'undefined') {
  module.exports = config;
}