/**
 * @file Side effect emitter entity definitions.
 * @author kidthales
 * @copyright Team Awesome 2014
 */

/**
 * The side effect emitter. Queues side effects when beams collide, instantiates
 * side effect during update.
 * 
 * @class SideEffectEmitter
 */
var SideEffectEmitter = IgeEntity.extend({
  /**
   * The id of this class type.
   * 
   * @name SideEffectEmitter.prototype.classId
   */
  classId: 'SideEffectEmitter',

  /**
   * Initialize the side effect emitter entity.
   * 
   * @name SideEffectEmitter.prototype.init
   * @function
   */
  init: function() {
    var self = this;

    IgeEntity.prototype.init.call(this);

    /**
     * Contains type of side effects to be generated along wiht their
     * corresponding spawn position.
     * 
     * @name SideEffectEmitter.prototype._queue
     * @public
     */
    this._queue = [];
  },

  /**
   * Append a side effect that is desired to be generated.
   * 
   * @name SideEffectEmitter.prototype.append
   * @function
   * @param {Object}
   *          sideEffect - An object containing a 'pos' property that references
   *          an IgePoint as well as a 'type' property that specifies the side
   *          effect type.
   */
  append: function(sideEffect) {
    if(ige.isServer) {
      this._queue.push(sideEffect);
    }
  },

  /**
   * For every frame, if the side effect queue is not empty, empty it by
   * generating the desired side effects and mounting them to the foreground
   * scene.
   * 
   * @name SideEffectEmitter.prototype.update
   * @function
   * @param {Context}
   *          ctx
   */
  update: function(ctx) {
    IgeEntity.prototype.update.call(this, ctx);

    /* CEXCLUDE */
    if(ige.isServer) {
      for(var i = 0; i < this._queue.length; ++i) {
        var sideEffect = this._queue[i];

        var x = sideEffect.pos.x;
        var y = sideEffect.pos.y;
        var z = sideEffect.pos.z;

        switch(sideEffect.type) {
          case 'lava':
            new Lava().translateTo(x, y, z).lifeSpan(10000)
                    .mount(ige.server.backgroundScene);
            break;
          case 'ice':
            new Ice().translateTo(x, y, z).lifeSpan(10000)
                    .mount(ige.server.backgroundScene);
            break;
          case 'mud':
            new Mud().translateTo(x, y, z).lifeSpan(10000)
                    .mount(ige.server.backgroundScene);
            break;
          case 'tornado':
            new Tornado().translateTo(x, y, z).lifeSpan(10000)
                    .mount(ige.server.backgroundScene);
            break;
          case 'steam':
            new Steam().translateTo(x, y, z).lifeSpan(10000)
                    .mount(ige.server.backgroundScene);
            break;
          case 'lightning':
            new Lightning().translateTo(x, y, z).lifeSpan(10000)
                    .mount(ige.server.backgroundScene);
            break;
        }
      }
      this._queue = [];
    }
    /* CEXCLUDE */
  }
});

if(typeof (module) !== 'undefined' && typeof (module.exports) !== 'undefined') {
  module.exports = SideEffectEmitter;
}