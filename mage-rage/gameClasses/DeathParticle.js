/**
 * @file Death particle entity definitions.
 * @author kidthales
 * @copyright Team Awesome 2014
 */

/**
 * Renderable particle emitted by a death particle emitter upon a players death.
 * 
 * @class DeathParticle
 */
var DeathParticle = IgeEntity.extend({
  /**
   * The id of this class type.
   * 
   * @name DeathParticle.prototype.classId
   */
  classId: 'DeathParticle',

  /**
   * Initialize the death particle.
   * 
   * @name DeathParticle.prototype.init
   * @function
   * @param {IgeParticleEmitter}
   *          emitter - The particle emitter that created this particle.
   */
  init: function(emitter) {
    this._emitter = emitter;

    IgeEntity.prototype.init.call(this);

    this.addComponent(IgeVelocityComponent);

    this.texture(ige.client.textures.deathParticle).width(50).height(50)
            .mount(ige.client.foregroundScene);

  },

  /**
   * Destroy the death particle entity and release associated resources.
   * 
   * @name DeathParticle.prototype.destroy
   * @function
   */
  destroy: function() {
    // Remove ourselves from the emitter
    if(this._emitter !== undefined) {
      this._emitter._particles.pull(this);
    }
    IgeEntity.prototype.destroy.call(this);
  }
});

if(typeof (module) !== 'undefined' && typeof (module.exports) !== 'undefined') {
  module.exports = DeathParticle;
}