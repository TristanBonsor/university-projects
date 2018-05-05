/**
 * @file Tornado entity definitions.
 * @author kidthales
 * @copyright Team Awesome 2014
 */

/**
 * The tornado side effect entity.
 * 
 * @class Tornado
 */
var Tornado = SideEffect
        .extend({
          /**
           * The id of this class type.
           * 
           * @name Tornado.prototype.classId
           */
          classId: 'Tornado',

          /**
           * Initialize the tornado entity.
           * 
           * @name Tornado.prototype.init
           * @function
           */
          init: function() {
            var self = this;

            SideEffect.prototype.init.call(this, 'tornado');

            /* CEXCLUDE */
            if(ige.isServer) {
              this.addComponent(IgeVelocityComponent);
              this.width(32).height(32).box2dBody({
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
              self.texture(ige.client.textures.tornadoSprites)
                      .dimensionsFromCell();
              self.addComponent(IgeAnimationComponent).depth(1);
              self.animation.define('tornadoAnimation', [1, 2, 3, 4, 5], 8, -1)
                      .cell(1);
              self.animation.start('tornadoAnimation');
            }
          },

          /**
           * For every frame, update the direction of the tornado on the client
           * (if mounted to the scenegraph). Triggers tornado sound effect on
           * client.
           * 
           * @name Tornado.prototype.update
           * @function
           * @param {Context}
           *          ctx
           */
          update: function(ctx) {
            /* CEXCLUDE */
            if(ige.isServer) {
              var speed = 0.2;
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
              if(ClientSound.status.tornado) {
                createjs.Sound.play('tornado', {
                  volume: 0.01
                });
              }
            }

            IgeEntityBox2d.prototype.update.call(this, ctx);
          }
        });

if(typeof (module) !== 'undefined' && typeof (module.exports) !== 'undefined') {
  module.exports = Tornado;
}