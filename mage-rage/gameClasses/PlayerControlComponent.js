/**
 * @file PlayerControlComponent entity mixin for use by a Player entity.
 * @author kidthales
 * @copyright Team Awesome 2014
 */

/**
 * Facilitates client side player control input and server side player control
 * processing. This class is intended to be added as a component to a player
 * entity.
 * 
 * @param {Player}
 *          entity - The player that this component is being added to.
 * @class PlayerControlComponent
 */
var PlayerControlComponent = IgeEntity.extend({
  /**
   * The id of this class type.
   * 
   * @name PlayerControlComponent.prototype.classId
   */
  classId: 'PlayerControlComponent',

  /**
   * The id of this component type.
   * 
   * @name PlayerControlComponent.prototype.componentId
   */
  componentId: 'playerControl',

  /**
   * Initializes the player control component. Called when added to a given
   * player entity.
   * 
   * @name PlayerControlComponent.prototype.init
   * @function
   * @param {Player}
   *          entity - The player that this component is being added to.
   */
  init: function(entity) {
    var self = this;

    /**
     * Stores the entity that this component has been added to.
     * 
     * @name PlayerControlComponent.prototype._entity
     * @public
     */
    this._entity = entity;

    /**
     * Stores current boolean status for left, right, up, down, and shoot
     * inputs.
     * 
     * @name PlayerControlComponent.prototype.controls
     */
    this.controls = {
      left: false,
      right: false,
      up: false,
      down: false,
      shoot: false
    };

    /**
     * Stores the current velocity magnitude for a player entity.
     * 
     * @name PlayerControlComponent.prototype._speed
     * @public
     */
    /**
     * Stores the default velocitu magnitude for a player entity.
     * 
     * @name PlayerControlComponent.prototype._DEFAULT_SPEED
     * @public
     */
    this._speed = this._DEFAULT_SPEED = 0.1;

    /**
     * The velocity magnitude of the player when on mud.
     * 
     * @name PlayerControlComponent.prototype._mudSpeed
     * @public
     */
    this._mudSpeed = 0.025;

    /**
     * The delta x velocity magnitude of the player when on ice.
     * 
     * @name PlayerControlComponent.prototype._iceSpeedX
     * @public
     */
    /**
     * The delta y velocity magnitude of the player when on ice.
     * 
     * @name PlayerControlComponent.prototype._iceSpeedY
     * @public
     */
    this._iceSpeedX = this._iceSpeedY = 0.025;

    /**
     * The previous x component of the player velocity.
     * 
     * @name PlayerControlComponent.prototype._prevVelocityX
     * @public
     */
    /**
     * The previous y component of the player velocity.
     * 
     * @name PlayerControlComponent.prototype._prevVelocityY
     * @public
     */
    this._prevVelocityX = this._prevVelocityY = 0;

    // Setup the control system.
    ige.input.mapAction('left', ige.input.key.a);
    ige.input.mapAction('right', ige.input.key.d);
    ige.input.mapAction('up', ige.input.key.w);
    ige.input.mapAction('down', ige.input.key.s);
    ige.input.mapAction('shoot', ige.input.key.space);

    // Add the playerComponent behaviour to the entity.
    this._entity.addBehaviour('playerControlComponent_behaviour', this._behaviour);
  },

  /**
   * Called every frame by the engine when this entity is mounted to the
   * scenegraph. Resolves player control behaviour for both client and server.
   * New beam particles are instantiated here when the shoot input is active and
   * valid.
   * 
   * @name PlayerControlComponent.prototype._behaviour
   * @function
   * @param {Context}
   *          ctx - The canvas context to render to.
   * @public
   */
  _behaviour: function(ctx) {
    if(!ige.isServer) {
      if(ige.input.actionState('left')) {
        if(!this.playerControl.controls.left) {
          // Record the new state.
          this.playerControl.controls.left = true;

          // Tell the server about our control change.
          ige.network.send('playerControlLeftDown');
        }
      }
      else {
        if(this.playerControl.controls.left) {
          // Record the new state.
          this.playerControl.controls.left = false;

          // Tell the server about our control change.
          ige.network.send('playerControlLeftUp');
        }
      }

      if(ige.input.actionState('right')) {
        if(!this.playerControl.controls.right) {
          // Record the new state.
          this.playerControl.controls.right = true;

          // Tell the server about our control change.
          ige.network.send('playerControlRightDown');
        }
      }
      else {
        if(this.playerControl.controls.right) {
          // Record the new state.
          this.playerControl.controls.right = false;

          // Tell the server about our control change.
          ige.network.send('playerControlRightUp');
        }
      }

      if(ige.input.actionState('up')) {
        if(!this.playerControl.controls.up) {
          // Record the new state.
          this.playerControl.controls.up = true;

          // Tell the server about our control change.
          ige.network.send('playerControlUpDown');
        }
      }
      else {
        if(this.playerControl.controls.up) {
          // Record the new state.
          this.playerControl.controls.up = false;

          // Tell the server about our control change.
          ige.network.send('playerControlUpUp');
        }
      }

      if(ige.input.actionState('down')) {
        if(!this.playerControl.controls.down) {
          // Record the new state.
          this.playerControl.controls.down = true;

          // Tell the server about our control change.
          ige.network.send('playerControlDownDown');
        }
      }
      else {
        if(this.playerControl.controls.down) {
          // Record the new state.
          this.playerControl.controls.down = false;

          // Tell the server about our control change.
          ige.network.send('playerControlDownUp');
        }
      }

      if(ige.input.actionState('shoot')) {
        if(!this.playerControl.controls.shoot) {
          // Record the new state.
          this.playerControl.controls.shoot = true;

          // Tell the server about our control change.
          ige.network.send('playerControlShootDown');
        }
      }
      else {
        if(this.playerControl.controls.shoot) {
          // Record the new state.
          this.playerControl.controls.shoot = false;

          // Tell the server about our control change.
          ige.network.send('playerControlShootUp');
        }
      }
    }

    /* CEXCLUDE */
    if(ige.isServer) {
      if(this._dead) {
        return;
      }

      if(!this._onTornado && !this._thrown) {
        // Determine facing and velocity.
        var facing = '', facing_prefix = '', facing_postfix = '';

        var speed = 0;
        var restVelocityX = 0;
        var restVelocityY = 0;
        var velocityX = 0;
        var velocityY = 0;

        if(this._onMud) {
          speed = this.playerControl._mudSpeed;
        }
        else if(this._onIce) {
          restVelocityX = this.playerControl._prevVelocityX;
          restVelocityY = this.playerControl._prevVelocityY;
        }
        else {
          speed = this.playerControl._speed;
        }

        if(this.playerControl.controls.up) {
          if(this._onIce) {
            velocityY = this.playerControl._prevVelocityY
                        - this.playerControl._iceSpeedY;
          }
          else {
            velocityY = -speed;
          }

          this.velocity.y(velocityY);
          this.playerControl._prevVelocityY = velocityY;

          facing_prefix = 'up';
        }
        else if(this.playerControl.controls.down) {
          if(this._onIce) {
            velocityY = this.playerControl._prevVelocityY
                        + this.playerControl._iceSpeedY;
          }
          else {
            velocityY = speed;
          }

          this.velocity.y(velocityY);
          this.playerControl._prevVelocityY = velocityY;

          facing_prefix = 'down';
        }
        else {
          this.velocity.y(restVelocityY);
        }

        if(this.playerControl.controls.left) {
          if(this._onIce) {
            velocityX = this.playerControl._prevVelocityX
                        - this.playerControl._iceSpeedX;
          }
          else {
            velocityX = -speed;
          }

          this.velocity.x(velocityX);
          this.playerControl._prevVelocityX = velocityX;

          if(facing_prefix === '') {
            facing_postfix = 'left';
          }
          else {
            facing_postfix = 'Left';
          }
        }
        else if(this.playerControl.controls.right) {
          if(this._onIce) {
            velocityX = this.playerControl._prevVelocityX
                        + this.playerControl._iceSpeedX;
          }
          else {
            velocityX = speed;
          }

          this.velocity.x(velocityX);
          this.playerControl._prevVelocityX = velocityX;

          if(facing_prefix === '') {
            facing_postfix = 'right';
          }
          else {
            facing_postfix = 'Right';
          }
        }
        else {
          this.velocity.x(restVelocityX);
        }

        // Determine final facing.
        facing = facing_prefix + facing_postfix;

        if(facing !== '') {
          this._facing = facing;
        }
      }
      else { // Tornado is effecting movement.
        if(this._onTornado) {
          this.velocity.x(0);
          this.velocity.y(0);
        }

        // Randomly determine facing due to tornado.
        var facingIndex = Math.floor(Math.random() * 8);

        switch(facingIndex) {
          case 0:
            this._facing = 'up';
            break;
          case 1:
            this._facing = 'down';
            break;
          case 2:
            this._facing = 'left';
            break;
          case 3:
            this._facing = 'right';
            break;
          case 4:
            this._facing = 'upLeft';
            break;
          case 5:
            this._facing = 'downLeft';
            break;
          case 6:
            this._facing = 'upRight';
            break;
          case 7:
            this._facing = 'downRight';
            break;
        }
      }

      // Determine shooting behaviour.
      if(this.playerControl.controls.shoot && this._currentMagicPoints > 0) {
        var x = this.worldPosition().x;
        var y = this.worldPosition().y;

        var vx = 0, vy = 0;

        this.applyMagicPointDamage(1);
        this._shoot = 'on';

        // Determine beam particle velocity based on current facing.
        switch(this._facing) {
          case 'up':
            vy = -1;
            break;
          case 'down':
            vy = 1;
            break;
          case 'left':
            vx = -1;
            break;
          case 'right':
            vx = 1;
            break;
          case 'upLeft':
            vx = -1;
            vy = -1;
            break;
          case 'downLeft':
            vx = -1;
            vy = 1;
            break;
          case 'upRight':
            vx = 1;
            vy = -1;
            break;
          case 'downRight':
            vx = 1;
            vy = 1;
            break;
          default:
            return;
            break;
        }

        // Create a new beam particle with the velocity determined above and
        // emanating from the current player facing.
        ige.server.beamParticleFactory(this._mageType,
                                       this._facing,
                                       new IgePoint(x, y, 0),
                                       new IgePoint(vx, vy, 0));
      }
      else {
        this._shoot = 'off';
      }
    }
    /* CEXCLUDE */
  }
});

if(typeof (module) !== 'undefined' && typeof (module.exports) !== 'undefined') {
  module.exports = PlayerControlComponent;
}