/**
 * @file BeamParticle entity definition.
 * @author kidthales
 * @copyright Team Awesome 2014
 */

/**
 * Maintains state and behaviour beam particle suitable for client/server
 * streaming. Streams transform, beamType, and direction data.
 * 
 * @param {String}
 *          beamType - One of 'air', 'earth', 'fire', or 'water'.
 * @param {String}
 *          direction - One of 'up', 'down', 'left', 'right', 'upLeft',
 *          'downLeft', 'upRight', 'downRight'.
 * @class BeamParticle
 */
var BeamParticle = IgeEntityBox2d.extend({
  /**
   * The id of this class type.
   * 
   * @name BeamParticle.prototype.classId
   */
  classId: 'BeamParticle',

  /**
   * Initializes the beam particle entity. Called from the IgeEntity
   * constructor.
   * 
   * @name BeamParticle.prototype.init
   * @function
   * @param {String}
   *          beamType - One of 'air', 'earth', 'fire', or 'water'.
   * @param {String}
   *          direction - One of 'up', 'down', 'left', 'right', 'upLeft',
   *          'downLeft', 'upRight', 'downRight'.
   */
  init: function(beamType, direction) {
    var self = this;

    IgeEntityBox2d.prototype.init.call(this);

    /* CEXCLUDE */
    if(ige.isServer) {
      this.addComponent(IgeVelocityComponent);
      this.box2dBody({
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
      self.texture(ige.client.textures.beamParticle);
    }

    /**
     * Get/set the general category that this entity falls under.
     * 
     * @name BeamParticle.prototype.category
     * @function
     * @param {String}
     *          categoryName - The name of the category that this entity falls
     *          under.
     * @returns {String} - The name of the category that this entity falls
     *          under.
     */
    this.category('BeamParticle');

    /**
     * Identifies the type of beam for the beam particle entity.
     * 
     * @name BeamParticle.prototype._beamType
     * @public
     */
    this._beamType = beamType;

    /**
     * Identifies the direction of a beam particle entity.
     * 
     * @name BeamParticle.prototype._direction
     * @public
     */
    this._direction = direction;

    /**
     * Flags if the beam particle is currently in a player (spawn point).
     * 
     * @name BeamParticle.prototype._inPlayer
     * @public
     */
    this._inPlayer = true;

    this.streamSections(['transform', 'beamType', 'direction']);
  },

  /**
   * Sets the stream data updates from the server for this entity. This method
   * is not usually explicitly called and is used by the engine.
   * 
   * @name BeamParticle.prototype.streamSectionData
   * @function
   * @param {String}
   *          sectionId - The id of a given stream section.
   * @param {Data}
   *          data - The data to be streamed in a given section.
   */
  streamSectionData: function(sectionId, data) {
    if(sectionId === 'beamType') {
      if(!ige.isServer) {
        if(data) {
          // We have been given new data!
          this._beamType = data;
        }
      }
      else {
        // Return current data.
        return this._beamType;
      }
    }
    else if(sectionId === 'direction') {
      if(!ige.isServer) {
        if(data) {
          // We have been given new data!
          this._direction = data;
        }
      }
      else {
        // Return current data.
        return this._direction;
      }
    }
    else {
      // Any other stream data sections forward to the parent class to
      // deal with.
      return IgeEntity.prototype.streamSectionData.call(this, sectionId, data);
    }
  },
  
  /**
   * Destroys the beam particle entity and associated resources. Triggers sound
   * effect on the client.
   * 
   * @name BeamParticle.prototype.destroy
   * @function
   */
  destroy: function() {
    if(!ige.isServer) {
      if(ClientSound.status.beamPlayerContact) {
        createjs.Sound.play('beamPlayerContact', {volume: 0.15});
      }
    }
    IgeEntityBox2d.prototype.destroy.call(this);
  }
});

if(typeof (module) !== 'undefined' && typeof (module.exports) !== 'undefined') {
  module.exports = BeamParticle;
}