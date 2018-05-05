/**
 * @file Side effect entity base class definitions.
 * @author kidthales
 * @copyright Team Awesome 2014
 */

/**
 * This is the base class for all side effects.
 * 
 * @class SideEffect
 */
var SideEffect = IgeEntityBox2d.extend({
  /**
   * The id of this class type.
   * 
   * @name SideEffect.prototype.classId
   */
  classId: 'SideEffect',

  /**
   * Initialize the side effect entity.
   * 
   * @name SideEffect.prototype.init
   * @function
   * @param {String}
   *          sideEffectType - The type of side effect.
   */
  init: function(sideEffectType) {
    var self = this;

    IgeEntityBox2d.prototype.init.call(this);

    /**
     * Get/set the general category that this entity falls under.
     * 
     * @name SideEffect.prototype.category
     * @function
     * @param {String}
     *          categoryName - The name of the category that this entity falls
     *          under.
     * @returns {String} - The name of the category that this entity falls
     *          under.
     */
    this.category('SideEffect');

    /**
     * Specifies the type of side effect.
     * 
     * @name SideEffect.prototype._sideEffectType
     */
    this._sideEffectType = sideEffectType;
  }
});

if(typeof (module) !== 'undefined' && typeof (module.exports) !== 'undefined') {
  module.exports = SideEffect;
}