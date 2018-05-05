/**
 * @file Mud entity definitions.
 * @author kidthales
 * @copyright Team Awesome 2014
 */

/**
 * The mud side effect entity.
 * 
 * @class Mud
 */
var Mud = SideEffect.extend({
  /**
   * The id of this class type.
   * 
   * @name Mud.prototype.classId
   */
  classId: 'Mud',

  /**
   * Initialize the mud entity.
   * 
   * @name Mud.prototype.init
   * @function
   */
  init: function() {
    var self = this;

    SideEffect.prototype.init.call(this, 'mud');

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
      self.texture(ige.client.textures.mudSprites).dimensionsFromCell()
              .depth(1);
    }
  }
});

if(typeof (module) !== 'undefined' && typeof (module.exports) !== 'undefined') {
  module.exports = Mud;
}