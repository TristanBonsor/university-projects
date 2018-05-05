/**
 * @file Lava entity definitions.
 * @author kidthales
 * @copyright Team Awesome 2014
 */

/**
 * The lava side effect entity.
 * 
 * @class Lava
 */
var Lava = SideEffect.extend({
  /**
   * The id of this class type.
   * 
   * @name Lava.prototype.classId
   */
  classId: 'Lava',

  /**
   * Initialize the lava entity.
   * 
   * @name Lava.prototype.init
   * @function
   */
  init: function() {
    var self = this;

    SideEffect.prototype.init.call(this, 'lava');

    /* CEXCLUDE */
    if(ige.isServer) {
      this.width(16).height(16).box2dBody({
        type: 'dynamic',
        linearDamping: 0.0,
        angularDamping: 0.1,
        allowSleep: true,
        bullet: false,
        gravitic: true,
        fixedRotation: false,
        fixtures: [{
          density: 1.0,
          friction: 0.5,
          restitution: 0.2,
          shape: {
            type: 'circle'
          }
        }]
      }).streamMode(1);
    }
    /* CEXCLUDE */

    if(!ige.isServer) {
      self.texture(ige.client.textures.lavaSprites).dimensionsFromCell();
      self.addComponent(IgeAnimationComponent).depth(1);
      self.animation.define('lavaAnimation', [1, 2, 3, 2], 8, -1).cell(1);
      self.animation.start('lavaAnimation');
    }
  }
});

if(typeof (module) !== 'undefined' && typeof (module.exports) !== 'undefined') {
  module.exports = Lava;
}