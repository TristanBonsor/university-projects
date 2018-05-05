/**
 * @file Attribute box entity definitions.
 * @author kidthales
 * @copyright Team Awesome 2014
 */

/**
 * Binds to two entity properties for the purpose of displaying a ration bar.
 * 
 * @class AttributeBox
 */
var AttributeBox = IgeEntity.extend({
  /**
   * The id of this class type.
   * 
   * @name AttributeBox.prototype.classId
   */
  classId: 'AttributeBox',

  /**
   * Initializes the attribute box entity.
   * 
   * @name AttributeBox.prototype.init
   * @function
   */
  init: function() {
    var self = this;

    IgeEntity.prototype.init.call(this);

    if(!ige.isServer) {
      self.texture(ige.client.textures.attributeBox);
    }

    /**
     * Object that maintains reference to two entity properties, keyed to 'a'
     * and 'b'.
     * 
     * @name AttributeBox.prototype._bindDataObject
     * @public
     */
    this._bindDataObject = {
      a: 1,
      b: 1
    };

    /**
     * The name of the current attribute to bind to.
     * 
     * @name AttributeBox.prototype._bindDataCurrAttrProperty
     * @public
     */
    this._bindDataCurrAttrProperty = 'a';

    /**
     * The name of the maximum attribute to bind to.
     * 
     * @name AttributeBox.prototype._bindDataMaxAttrProperty
     * @public
     */
    this._bindDataMaxAttrProperty = 'b';

    /**
     * The background color.
     * 
     * @name AttributeBox.prototype._backgroundColor
     * @public
     */
    this._backgroundColor = '#000000';

    /**
     * The color overlay.
     * 
     * @name AttributeBox.prototype._colorOverlay
     * @public
     */
    this._colorOverlay = '#000000';

    return this;
  },

  /**
   * Set the background color.
   * 
   * @name AttributeBox.prototype.backgroundColor
   * @function
   * @param {String}
   *          color - An RGB string.
   */
  backgroundColor: function(color) {
    this._backgroundColor = color;

    return this;
  },

  /**
   * Set the color overlay.
   * 
   * @name AttributeBox.prototype.colorOverlay
   * @function
   * @param {String}
   *          color - An RGB string.
   */
  colorOverlay: function(color) {
    this._colorOverlay = color;

    return this;
  },

  /**
   * Bind the attribute box to the specified properties on the specified object.
   * 
   * @name AttributeBox.prototype.bindData
   * @function
   * @param {Object}
   *          obj - The object containing the properties to bind to.
   * @param {String}
   *          currAttrPropName - The current value property.
   * @param {String}
   *          maxAttrPropName - The maximum value property.
   */
  bindData: function(obj, currAttrPropName, maxAttrPropName) {
    if(obj !== undefined
       && currAttrPropName !== undefined
       && maxAttrPropName !== undefined) {
      this._bindDataObject = obj;
      this._bindDataCurrAttrProperty = currAttrPropName;
      this._bindDataMaxAttrProperty = maxAttrPropName;
    }

    return this;
  }
});

if(typeof (module) !== 'undefined' && typeof (module.exports) !== 'undefined') {
  module.exports = AttributeBox;
}