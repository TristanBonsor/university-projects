/**
 * @file Steam entity definitions.
 * @author kidthales
 * @copyright Team Awesome 2014
 */

/**
 * The steam side effect entity.
 * 
 * @class Steam
 */
var Steam = SideEffect.extend({
  /**
   * The id of this class type.
   * 
   * @name Steam.prototype.classId
   */
  classId: 'Steam',

  /**
   * Initialize the steam entity.
   * 
   * @name Steam.prototype.init
   * @function
   */
  init: function() {
    var self = this;

    SideEffect.prototype.init.call(this, 'steam');

    /* CEXCLUDE */
    if(ige.isServer) {
      this.addComponent(IgeVelocityComponent);
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
      self.texture(ige.client.textures.steamSprites).dimensionsFromCell();
      self.addComponent(IgeAnimationComponent).depth(1);
      self.animation.define('steamAnimation', [1, 2, 3, 4, 5], 8, -1).cell(1);
      self.animation.start('steamAnimation');
    }
  },

  /**
   * For every frame, update the direction of the steam on the client (if
   * mounted to the scenegraph).
   * 
   * @name Steam.prototype.update
   * @function
   * @param {Context}
   *          ctx
   */
  update: function(ctx) {
    /* CEXCLUDE */
    if(ige.isServer) {
      var speed = 0.05;
      var directionEnum = Math.floor(Math.random() * 8);

      switch(directionEnum) {
        case 0:
          this.velocity.x(0);
          this.velocity.y(-speed);
          break;
        case 1:
          this.velocity.x(speed);
          this.velocity.y(-speed);
          break;
        case 2:
          this.velocity.x(speed);
          this.velocity.y(0);
          break;
        case 3:
          this.velocity.x(speed);
          this.velocity.y(speed);
          break;
        case 4:
          this.velocity.x(0);
          this.velocity.y(speed);
          break;
        case 5:
          this.velocity.x(-speed);
          this.velocity.y(speed);
          break;
        case 6:
          this.velocity.x(-speed);
          this.velocity.y(0);
          break;
        case 7:
          this.velocity.x(-speed);
          this.velocity.y(-speed);
          break;
      }
    }
    /* CEXCLUDE */

    IgeEntityBox2d.prototype.update.call(this, ctx);
  }
});

if(typeof (module) !== 'undefined' && typeof (module.exports) !== 'undefined') {
  module.exports = Steam;
}