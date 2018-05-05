/**
 * @file Wall entity definitions.
 * @author kidthales
 * @copyright Team Awesome 2014
 */

/**
 * Maintains state and behaviour of a vertical/horizontal wall entity suitable
 * for client/server streaming. Streams transform data. Note that there is no
 * explicit texture - this entity servers as a transparent contact box.
 * 
 * @param {String}
 *          id - Unique identifier.
 * @param {IgePoint}
 *          pos - (x,y,z) of center point of wall.
 * @param {Number}
 *          width
 * @param {Number}
 *          height
 * @class Wall
 */
var Wall = IgeEntityBox2d.extend({
  /**
   * The id of this class type.
   * 
   * @name Wall.prototype.classId
   */
  classId: 'Wall',

  /**
   * Initializes the wall entity. Called from the IgeEntity constructor.
   * 
   * @name Wall.prototype.init
   * @function
   * @param {String}
   *          id - Unique identifier.
   * @param {IgePoint}
   *          pos - (x,y,z) of center point of wall.
   * @param {Number}
   *          width
   * @param {Number}
   *          height
   */
  init: function(id, pos, width, height) {
    var self = this;

    IgeEntityBox2d.prototype.init.call(this);

    /* CEXCLUDE */
    if(ige.isServer) {
      this.id(id).translateTo(pos.x, pos.y, pos.z).width(width).height(height)
              .box2dBody({
                type: 'static',
                allowSleep: true,
                fixtures: [{
                  shape: {
                    type: 'rectangle'
                  }
                }]
              }).streamMode(1);
    }
    /* CEXCLUDE */

    /**
     * Get/set the general category that this entity falls under.
     * 
     * @name Wall.prototype.category
     * @function
     * @param {String}
     *          categoryName - The name of the category that this entity falls
     *          under.
     * @returns {String} - The name of the category that this entity falls
     *          under.
     */
    this.category('Wall');

    /**
     * The wall identifier.
     * 
     * @name Wall.prototype._wallId
     * @public
     */
    this._wallId = id;
  }
});

if(typeof (module) !== 'undefined' && typeof (module.exports) !== 'undefined') {
  module.exports = Wall;
}