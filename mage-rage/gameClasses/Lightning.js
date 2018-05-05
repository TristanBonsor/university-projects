/**
 * @file Lightning entity definitions.
 * @author kidthales
 * @copyright Team Awesome 2014
 */

/**
 * The lightning side effect entity.
 * 
 * @class Lightning
 */
var Lightning = SideEffect.extend({
  /**
   * The id of this class type.
   * 
   * @name Lightning.prototype.classId
   */
  classId: 'Lightning',

  /**
   * Initialize the lightning entity.
   * 
   * @name Lightning.prototype.init
   * @function
   */
  init: function() {
    var self = this;

    SideEffect.prototype.init.call(this, 'lightning');

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

      // Randomly determine initial direction.
      var speed = 0.5;
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

    if(!ige.isServer) {
      self.texture(ige.client.textures.lightningSprites).dimensionsFromCell();
      self.addComponent(IgeAnimationComponent).depth(1);
      self.animation.define('lightningAnimation', [1, 4, 2, 3], 8, -1).cell(1);
      self.animation.start('lightningAnimation');
    }
  }
});

if(typeof (module) !== 'undefined' && typeof (module.exports) !== 'undefined') {
  module.exports = Lightning;
}