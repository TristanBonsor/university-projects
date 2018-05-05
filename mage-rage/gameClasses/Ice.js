/**
 * @file Ice entity definitions.
 * @author kidthales
 * @copyright Team Awesome 2014
 */

/**
 * The ice side effect entity.
 * 
 * @class Ice
 */
var Ice = SideEffect.extend({
  /**
   * The id of this class type.
   * 
   * @name Ice.prototype.classId
   */
  classId: 'Ice',

  /**
   * Initialize the ice entity.
   * 
   * @name Ice.prototype.init
   * @function
   */
  init: function() {
    var self = this;

    SideEffect.prototype.init.call(this, 'ice');

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
      self.texture(ige.client.textures.iceSprites).dimensionsFromCell()
              .depth(1);
    }
  }
});

if(typeof (module) !== 'undefined' && typeof (module.exports) !== 'undefined') {
  module.exports = Ice;
}