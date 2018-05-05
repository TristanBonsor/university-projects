/**
 * @file Player entity definitions.
 * @author kidthales
 * @copyright Team Awesome 2014
 */

/**
 * Maintains state and behaviour of a player entity suitable for client/server
 * streaming. Streams transform, facing, shooting, and mageType data.
 * 
 * @param {String}
 *          clientId - The id of the client that this entity will be controlled
 *          by.
 * @param {String}
 *          mageType - The mage type of the player, 'air', 'earth', 'fire',
 *          'water'.
 * @class Player
 */
var Player = IgeEntityBox2d
        .extend({
          /**
           * The id of this class type.
           * 
           * @name Player.prototype.classId
           */
          classId: 'Player',

          /**
           * Initializes the player entity. Called from the IgeEntity
           * constructor.
           * 
           * @name Player.prototype.init
           * @function
           * @param {String}
           *          clientId - The id of the client that this entity will be
           *          controlled by.
           * @param {String}
           *          mageType - The mage type of the player, 'air', 'earth',
           *          'fire', 'water'.
           */
          init: function(clientId, mageType, playerName) {
            var self = this;

            IgeEntityBox2d.prototype.init.call(this);

            /* CEXCLUDE */
            if(ige.isServer) {
              // Establish the player physics and control on the server.
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
              }).addComponent(PlayerControlComponent).streamMode(1); // Stream
              // to
              // clients.

              /**
               * Player's magic point recharge interval object. Instantiated and
               * exists on server.
               * 
               * @name Player.prototype._magicPointRechargeInterval
               * @public
               */
              self._magicPointRechargeInterval = new IgeInterval()
                      .init(function() {
                              if(self._currentMagicPoints < self._MAX_MAGIC_POINTS) {
                                self.applyMagicPointDamage(-2);

                                self._currentMagicPoints = (self._currentMagicPoints < self._MAX_MAGIC_POINTS)
                                        ? self._currentMagicPoints
                                        : self._MAX_MAGIC_POINTS;
                              }
                            },
                            1000);

              /**
               * Player's hit point recharge interval object. Instantiated and
               * exists on server.
               * 
               * @name Player.prototype._hitPointRechargeInterval
               * @public
               */
              self._hitPointRechargeInterval = new IgeInterval()
                      .init(function() {
                        if(self._currentHitPoints < self._MAX_HIT_POINTS) {
                          self.applyHitPointDamage(-1);
                        }
                      }, 10000);

              /**
               * Player's process lava damage interval object. Instantiated and
               * exists on server.
               * 
               * @name Player.prototype._lavaDamageInterval
               * @public
               */
              self._lavaDamageInterval = new IgeInterval().init(function() {
                if(self._onLava) {
                  self.applyHitPointDamage(2, 'lava');
                }
              }, 100);

              /**
               * Player's process steam damage interval object. Instantiated and
               * exists on server.
               * 
               * @name Player.prototype._steamDamageInterval
               * @public
               */
              self._steamDamageInterval = new IgeInterval().init(function() {
                if(self._onSteam) {
                  self.applyHitPointDamage(1, 'steam');
                }
              }, 100);

              /**
               * Player's death interval object. Used for determining length of
               * player death animation, and what to do upon its completion.
               * Instantiated and exists on server.
               * 
               * @name Player.prototype._deathInterval
               * @public
               */
              self._deathInterval = new IgeInterval();
            }
            /* CEXCLUDE */

            if(!ige.isServer) {
              // Establish the player appearance and control on the client.
              self.addComponent(IgeAnimationComponent).depth(1);
              self.texture(ige.client.textures.mageSprites)
                      .dimensionsFromCell();
              self.addComponent(PlayerControlComponent);

              // Player floating widgets - these are shared between all clients.

              /**
               * Floating player name font entity object. Instantiated and
               * exists on client.
               * 
               * @name Player.prototype._playerNameFontEntity
               * @public
               */
              self._playerNameFontEntity = new IgeFontEntity()
                      .id(self.id() + 'playerNameFontEntity').depth(1000)
                      .width(100).height(20).textAlignX(1)
                      .colorOverlay('#ffffff').nativeFont('12pt Arial')
                      .nativeStroke(1).nativeStrokeColor('#666666')
                      .textLineSpacing(0).bindData(self, '_playerName', '', '');

              /**
               * Floating player current hit point attribute box entity object.
               * Instantiated and exists on client.
               * 
               * @name Player.prototype._currentHitPointsAttributeBox
               * @public
               */
              self._currentHitPointsAttributeBox = new AttributeBox()
                      .id(self.id() + 'currentHitPointsAttributeBox')
                      .depth(-1000).width(50).height(10)
                      .colorOverlay('#ff0000').bindData(self,
                                                        '_currentHitPoints',
                                                        '_MAX_HIT_POINTS');

              /**
               * Floating player current magic point attribute box entity
               * object. Instantiated and exists on client.
               * 
               * @name Player.prototype._currentMagicPointsAttributeBox
               * @public
               */
              self._currentMagicPointsAttributeBox = new AttributeBox()
                      .id(self.id() + 'currentMagicPointsAttributeBox')
                      .depth(-1000).width(50).height(10)
                      .colorOverlay('#0000ff').bindData(self,
                                                        '_currentMagicPoints',
                                                        '_MAX_MAGIC_POINTS');

              /**
               * Floating player death particle emitter object. Instantiated and
               * exists on client.
               * 
               * @name Player.prototype._deathParticleEmitter
               * @public
               */
              self._deathParticleEmitter = new IgeParticleEmitter()
                      .id(self.id() + 'deathParticleEmitter')
                      .particle(DeathParticle).lifeBase(2500)
                      .quantityTimespan(1000).quantityBase(60)
                      .translateVarianceX(-16, 16).scaleBaseX(0.2)
                      .scaleBaseY(0.2).scaleLockAspect(true)
                      .rotateVariance(0, 360).opacityBase(1.0)
                      .velocityVector(new IgePoint(0, -0.1, 0),
                                      new IgePoint(-0.2, -0.1, 0),
                                      new IgePoint(0.2, -0.25, 0))
                      .linearForceVector(new IgePoint(0, 0.12, 0),
                                         new IgePoint(0, 0, 0),
                                         new IgePoint(0, 0, 0))
                      .deathScaleBaseX(0).deathScaleVarianceX(0, 1)
                      .deathScaleBaseY(0.7).deathRotateBase(0)
                      .deathRotateVariance(-360, 360).deathOpacityBase(0.0)
                      .depth(1).width(10).height(10);

              // Player fixed widgets - these are mapped to one and only one
              // client.

              /**
               * Fixed player current hit points font entity object.
               * Instantiated and exists on client.
               * 
               * @name Player.prototype._currentHitPointsFontEntity
               * @public
               */
              self._currentHitPointsFontEntity = new IgeFontEntity()
                      .id(self.id() + 'currentHitPointsFontEntity')
                      .depth(-1000).width(100).height(35).textAlignX(1)
                      .colorOverlay('#ff0000').nativeFont('12pt Arial')
                      .nativeStroke(1).nativeStrokeColor('#000000')
                      .textLineSpacing(0).bindData(self,
                                                   '_currentHitPoints',
                                                   '',
                                                   ' HP');

              /**
               * Fixed player magic hit points font entity object. Instantiated
               * and exists on client.
               * 
               * @name Player.prototype._currentMagicPointsFontEntity
               * @public
               */
              self._currentMagicPointsFontEntity = new IgeFontEntity()
                      .id(self.id() + 'currentMagicPointsFontEntity').depth(1)
                      .width(100).height(35).textAlignX(1)
                      .colorOverlay('#0000ff').nativeFont('12pt Arial')
                      .nativeStroke(1).nativeStrokeColor('#000000')
                      .textLineSpacing(0).bindData(self,
                                                   '_currentMagicPoints',
                                                   '',
                                                   ' MP');

              /**
               * Fixed player kill count font entity object. Instantiated and
               * exists on client.
               * 
               * @name Player.prototype._killsFontEntity
               * @public
               */
              self._killsFontEntity = new IgeFontEntity()
                      .id(self.id() + 'killsFontEntity').depth(1).width(100)
                      .height(35).textAlignX(1).colorOverlay('#00ff00')
                      .nativeFont('12pt Arial').nativeStroke(1)
                      .nativeStrokeColor('#000000').textLineSpacing(0)
                      .bindData(self, '_kills', '', ' Kills');

              /**
               * Fixed player death count font entity object. Instantiated and
               * exists on client.
               * 
               * @name Player.prototype._deathsFontEntity
               * @public
               */
              self._deathsFontEntity = new IgeFontEntity()
                      .id(self.id() + 'deathsFontEntity').depth(1).width(100)
                      .height(35).textAlignX(0).colorOverlay('#00ff00')
                      .nativeFont('12pt Arial').nativeStroke(1)
                      .nativeStrokeColor('#000000').textLineSpacing(0)
                      .bindData(self, '_deaths', '', ' Deaths');
            }

            /**
             * Get/set the general category that this entity falls under.
             * 
             * @name Player.prototype.category
             * @function
             * @param {String}
             *          categoryName - The name of the category that this entity
             *          falls under.
             * @returns {String} - The name of the category that this entity
             *          falls under.
             */
            this.category('Player');

            /**
             * Unique identifier of the client that controls the player entity.
             * 
             * @name Player.prototype._clientId
             * @public
             */
            this._clientId = clientId;

            /**
             * Identifies the type of mage for the player entity.
             * 
             * @name Player.prototype._mageType
             * @public
             */
            this._mageType = mageType;

            /**
             * The name of the player.
             * 
             * @name Player.prototype._playerName
             * @public
             */
            this._playerName = playerName;

            /**
             * Indicates the last translation, within world space, of the player
             * entity.
             * 
             * @name Player.prototype._lastTranslate
             * @public
             */
            this._lastTranslate = this._translate.clone();

            /**
             * Indicates the current facing of the player entity.
             * 
             * @name Player.prototype._facing
             */
            this._facing = 'down';

            /**
             * Indicates the current shoot status of the player entity.
             * 
             * @name Player.prototype._shoot
             * @public
             */
            this._shoot = 'off';

            /**
             * Maximum possible hit points.
             * 
             * @name Player.prototype._MAX_HIT_POINTS
             * @public
             */
            this._MAX_HIT_POINTS = 100;

            /**
             * Current hit points.
             * 
             * @name Player.prototype._currentHitPoints
             * @public
             */
            this._currentHitPoints = 100;

            /**
             * Maximum possible magic points.
             * 
             * @name Player.prototype._MAX_MAGIC_POINTS
             * @public
             */
            this._MAX_MAGIC_POINTS = 100;

            /**
             * Current hit magic points.
             * 
             * @name Player.prototype._currentMagicPoints
             * @public
             */
            this._currentMagicPoints = 100;

            /**
             * Current kill count.
             * 
             * @name Player.prototype._kills
             * @public
             */
            this._kills = 0;

            /**
             * Current death count.
             * 
             * @name Player.prototype._deaths
             * @public
             */
            this._deaths = 0;

            /**
             * On lava status flag.
             * 
             * @name Player.prototype._onLava
             * @public
             */
            this._onLava = false;

            /**
             * On ice status flag.
             * 
             * @name Player.prototype._onIce
             * @public
             */
            this._onIce = false;

            /**
             * On mud status flag.
             * 
             * @name Player.prototype._onMud
             * @public
             */
            this._onMud = false;

            /**
             * On tornado status flag.
             * 
             * @name Player.prototype._onTornado
             * @public
             */
            this._onTornado = false;

            /**
             * On steam status flag.
             * 
             * @name Player.prototype._onSteam
             * @public
             */
            this._onSteam = false;

            /**
             * Thrown status flag.
             * 
             * @name Player.prototype._thrown
             * @public
             */
            this._thrown = false;

            /**
             * Player dead status flag.
             * 
             * @name Player.prototype._dead
             * @public
             */
            this._dead = false;

            /**
             * The last type of damage dealt to player.
             * 
             * @name Player.prototype._lastDamageType
             * @public
             */
            this._lastDamageType = '';

            // Register the stream sections for this entity.
            this.streamSections(['transform',
                                 'facing',
                                 'shoot',
                                 'mageType',
                                 'playerName',
                                 'currentHitPoints',
                                 'currentMagicPoints',
                                 'kills',
                                 'deaths']);
          },

          /**
           * Sets the stream data updates from the server for this entity. This
           * method is not usually explicitly called and is used by the engine.
           * 
           * @name Player.prototype.streamSectionData
           * @function
           * @param {String}
           *          sectionId - The id of a given stream section.
           * @param {Data}
           *          data - The data to be streamed in a given section.
           * @returns {Data|undefined} - Any new stream data from the server, if
           *          it exists.
           */
          streamSectionData: function(sectionId, data) {
            if(sectionId === 'facing') {
              if(!ige.isServer) {
                if(data) {
                  // We have been given new data!
                  this._facing = data;
                }
              }
              else {
                // Return current data.
                return this._facing;
              }
            }
            else if(sectionId === 'shoot') {
              if(!ige.isServer) {
                if(data) {
                  // We have been given new data!
                  this._shoot = data;
                }
                else {
                  // No new data.
                  this._shoot = 'off';
                }
              }
              else {
                // Return current data.
                return this._shoot;
              }
            }
            else if(sectionId === 'mageType') {
              if(!ige.isServer) {
                if(data) {
                  // We have been given new data!
                  this._mageType = data;
                }
              }
              else {
                // Return current data.
                return this._mageType;
              }
            }
            else if(sectionId === 'playerName') {
              if(!ige.isServer) {
                if(data) {
                  // We have been given new data!
                  this._playerName = data;
                }
              }
              else {
                // Return current data.
                return this._playerName;
              }
            }
            else if(sectionId === 'currentHitPoints') {
              if(!ige.isServer) {
                if(data) {
                  // We have been given new data!
                  this._currentHitPoints = parseInt(data);
                }
              }
              else {
                // Return current data.
                return this._currentHitPoints;
              }
            }
            else if(sectionId === 'currentMagicPoints') {
              if(!ige.isServer) {
                if(data) {
                  // We have been given new data!
                  this._currentMagicPoints = parseInt(data);
                }
              }
              else {
                // Return current data.
                return this._currentMagicPoints;
              }
            }
            else if(sectionId === 'kills') {
              if(!ige.isServer) {
                if(data) {
                  // We have been given new data!
                  this._kills = parseInt(data);
                }
              }
              else {
                // Return current data.
                return this._kills;
              }
            }
            else if(sectionId === 'deaths') {
              if(!ige.isServer) {
                if(data) {
                  // We have been given new data!
                  this._deaths = parseInt(data);
                }
              }
              else {
                // Return current data.
                return this._deaths;
              }
            }
            else {
              // Any other stream data sections forward to the parent class to
              // deal with.
              return IgeEntity.prototype.streamSectionData.call(this,
                                                                sectionId,
                                                                data);
            }
          },

          /**
           * Mounts all player associated floating UI widgets (name and hit
           * point, magic point boxes) to the specified scene.
           * 
           * @name Player.prototype.mountFloatingUi
           * @function
           * @param {IgeScene2d}
           *          scene - The scene to mount UI widgets to..
           */
          mountFloatingUi: function(scene) {
            this._playerNameFontEntity.mount(scene);
            this._currentHitPointsAttributeBox.mount(scene);
            this._currentMagicPointsAttributeBox.mount(scene);
            this._deathParticleEmitter.particleMountTarget(scene).mount(scene);
          },

          /**
           * Mounts all player associated fixed UI widgets (hit point, magic
           * point, kills, deaths) to the specified scene.
           * 
           * @name Player.prototype.mountFixedUi
           * @function
           * @param {IgeScene2d}
           *          scene - The scene to mount UI widgets to..
           */
          mountFixedUi: function(scene) {
            this._currentHitPointsFontEntity.mount(scene);
            this._currentMagicPointsFontEntity.mount(scene);
            this._killsFontEntity.mount(scene);
            this._deathsFontEntity.mount(scene);
          },

          /**
           * Subtracts the specified value from the player's current hit points.
           * Optionally specify the type of damage.
           * 
           * @name Player.prototype.applyHitPointDamage
           * @function
           * @param {Number}
           *          value - The amount to subtract from the current hit
           *          points.
           * @param {String}
           *          type - The type of damage dealt.
           */
          applyHitPointDamage: function(value, type) {
            this._currentHitPoints = this._currentHitPoints - value;

            if(type) {
              this._lastDamageType = type;
            }
          },

          /**
           * Subtracts the specified value from the player's current magic
           * points.
           * 
           * @name Player.prototype.applyMagicPointDamage
           * @function
           * @param {Number}
           *          value - The amount to subtract from the current magic
           *          points.
           */
          applyMagicPointDamage: function(value) {
            this._currentMagicPoints = this._currentMagicPoints - value;
          },

          /**
           * Sets the type of mage for the player. Determines the animation
           * sequences and appearance.
           * 
           * @name Player.prototype.setMageType
           * @function
           * @param {String}
           *          type - One of 'air', 'earth', 'fire', 'water'.
           */
          setMageType: function(type) {
            switch(type) {
              // Air mage.
              case 'air':
                this._defineAirMageAnimations();
                this._playerNameFontEntity.colorOverlay('#ffffff');
                break;

              // Earth mage.
              case 'earth':
                this._defineEarthMageAnimations();
                this._playerNameFontEntity.colorOverlay('#964b00');
                break;

              // Fire mage.
              case 'fire':
                this._defineFireMageAnimations();
                this._playerNameFontEntity.colorOverlay('#ff0000');
                break;

              // Water mage.
              case 'water':
                this._defineWaterMageAnimations();
                this._playerNameFontEntity.colorOverlay('#0000ff');
                break;
            }

            this._mageType = type;
          },

          /**
           * For every frame, update the appearance of the player on the client
           * (if mounted to the scenegraph).
           * 
           * @name Player.prototype.update
           * @function
           * @param {Context}
           *          ctx - The canvas context to render to.
           */
          update: function(ctx) {
            var self = this;

            /* CEXCLUDE */
            if(ige.isServer) {
              if(!this._dead && this._currentHitPoints <= 0) {
                this._dead = true;
                this._deaths++;

                this._deathInterval.init(function() {
                  if(self._dead) {
                    self._dead = false;

                    self._currentHitPoints = self._MAX_HIT_POINTS;

                    var x = Math.floor(Math.random() * 600) - 300;
                    var y = Math.floor(Math.random() * 600) - 300;

                    self.translateTo(x, y, 0);

                    this.cancel();
                  }
                }, 5000);

                switch(this._lastDamageType) {
                  case 'air':
                    if(ige.server.mages['air'] && this._mageType !== 'air') {
                      ige.server.mages['air']._kills++;
                    }
                    break;
                  case 'earth':
                    if(ige.server.mages['earth'] && this._mageType !== 'earth') {
                      ige.server.mages['earth']._kills++;
                    }
                    break;
                  case 'fire':
                    if(ige.server.mages['fire'] && this._mageType !== 'fire') {
                      ige.server.mages['fire']._kills++;
                    }
                    break;
                  case 'water':
                    if(ige.server.mages['water'] && this._mageType !== 'water') {
                      ige.server.mages['water']._kills++;
                    }
                    break;
                  /*
                   * case 'lava': if(ige.server.mages['earth'] && this._mageType
                   * !== 'earth') { ige.server.mages['earth']._kills += 0.5; }
                   * if(ige.server.mages['fire'] && this._mageType !== 'fire') {
                   * ige.server.mages['fire']._kills += 0.5; } break; case
                   * 'ice': if(ige.server.mages['air'] && this._mageType !==
                   * 'air') { ige.server.mages['air']._kills += 0.5; }
                   * if(ige.server.mages['earth'] && this._mageType !== 'earth') {
                   * ige.server.mages['earth']._kills += 0.5; } break; case
                   * 'mud': if(ige.server.mages['earth'] && this._mageType !==
                   * 'earth') { ige.server.mages['earth']._kills += 0.5; }
                   * if(ige.server.mages['water'] && this._mageType !== 'water') {
                   * ige.server.mages['water']._kills += 0.5; } break; case
                   * 'tornado': if(ige.server.mages['air'] && this._mageType !==
                   * 'air') { ige.server.mages['air']._kills += 0.5; }
                   * if(ige.server.mages['earth'] && this._mageType !== 'earth') {
                   * ige.server.mages['earth']._kills += 0.5; } break; case
                   * 'steam': if(ige.server.mages['fire'] && this._mageType !==
                   * 'fire') { ige.server.mages['fire']._kills += 0.5; }
                   * if(ige.server.mages['water'] && this._mageType !== 'water') {
                   * ige.server.mages['water']._kills += 0.5; } break; case
                   * 'lightning': if(ige.server.mages['air'] && this._mageType
                   * !== 'air') { ige.server.mages['air']._kills += 0.5; }
                   * if(ige.server.mages['fire'] && this._mageType !== 'fire') {
                   * ige.server.mages['fire']._kills += 0.5; } break;
                   */
                }
              }
            }
            /* CEXCLUDE */

            if(!ige.isServer) {
              // Set the type of mage that will be displayed.
              this.setMageType(this._mageType);

              // Update position of player UI widgets.
              this._updateUiPositions();

              // Test for death.
              if(this._currentHitPoints <= 0) {
                this._dead = true;
                this._deathParticleEmitter.start();
              }
              else {
                this._dead = false;
                self._deathParticleEmitter.stopAndKill();
              }

              // Set the current animation based on direction.
              this._setAnimation();

              // Set the depth to the y co-ordinate which basically
              // makes the entity appear further in the foreground
              // the closer they become to the bottom of the screen.
              this.depth(this._translate.y);

              // Store our current translation data for use in the next update.
              this._lastTranslate = this._translate.clone();

            }

            IgeEntityBox2d.prototype.update.call(this, ctx);
          },

          /**
           * Destroy the player entity and clean up all resources.
           * 
           * @name Player.prototype.destroy
           * @function
           */
          destroy: function() {
            this._destroyUi();
            IgeEntityBox2d.prototype.destroy.call(this);
          },

          /**
           * As no font sheet exists, we cannot leverage IGE's robust UI
           * entities (that provide CSS-like styling options) for fixed UI
           * widgets. Thus we must explicitly update fixed UI positions (in case
           * of window dimension changes), as well as floating UI widget
           * positions (which follow the player).
           * 
           * @name Player.prototype._updateUiPositions
           * @function
           */
          _updateUiPositions: function() {
            var playerX = this.translate().x();
            var playerY = this.translate().y();

            // Update death particle emitter to new player position.
            this._deathParticleEmitter.translateTo(playerX, playerY, 0);

            // Update floating UI widgets.

            var mpAttrBoxX = playerX;
            var mpAttrBoxY = playerY - this.height() / 2;

            var hpAttrBoxX = playerX;
            var hpAttrBoxY = mpAttrBoxY
                             - this._currentMagicPointsAttributeBox.height();

            var playerNameX = playerX;
            var playerNameY = hpAttrBoxY
                              - this._currentHitPointsAttributeBox.height();

            this._currentMagicPointsAttributeBox.translateTo(mpAttrBoxX,
                                                             mpAttrBoxY,
                                                             0);
            this._currentHitPointsAttributeBox.translateTo(hpAttrBoxX,
                                                           hpAttrBoxY,
                                                           0);
            this._playerNameFontEntity.translateTo(playerNameX, playerNameY, 0);

            // Update fixed UI widgets.

            var statusBoxX = -window.innerWidth / 2 + 35;
            var statusBoxY = window.innerHeight / 2 - 35;

            this._currentHitPointsFontEntity.translateTo(statusBoxX,
                                                         statusBoxY,
                                                         0);

            this._currentMagicPointsFontEntity
                    .translateTo(statusBoxX
                                         + this._currentHitPointsFontEntity
                                                 .width(),
                                 statusBoxY,
                                 0);

            this._killsFontEntity
                    .translateTo(this._currentMagicPointsFontEntity.translate()
                                         .x()
                                         + this._currentMagicPointsFontEntity
                                                 .width(),
                                 statusBoxY,
                                 0);

            this._deathsFontEntity.translateTo(this._killsFontEntity
                                                       .translate().x()
                                                       + this._killsFontEntity
                                                               .width(),
                                               statusBoxY,
                                               0);
          },

          /**
           * Destroy all UI widgets.
           * 
           * @name Player.prototype._destroyUi
           * @function
           */
          _destroyUi: function() {
            if(!ige.isServer) {
              this._playerNameFontEntity.destroy();
              this._currentHitPointsAttributeBox.destroy();
              this._currentMagicPointsAttributeBox.destroy();

              this._deathParticleEmitter.destroy();

              this._currentHitPointsFontEntity.destroy();
              this._currentMagicPointsFontEntity.destroy();
              this._killsFontEntity.destroy();
              this._deathsFontEntity.destroy();
            }
          },

          /**
           * Set current player animation, based on displacement.
           * 
           * @name Player.prototype._setAnimation
           * @function
           */
          _setAnimation: function() {
            var oldX = this._lastTranslate.x;
            var oldY = this._lastTranslate.y * 2;
            var currX = this.translate().x();
            var currY = this.translate().y() * 2;
            var distX = currX - oldX;
            var distY = currY - oldY;

            if(this._dead) {
              this._setDeathAnimation();

              if(ClientSound.status.playerDeath) {
                createjs.Sound.play('playerDeath');
              }

              return;
            }

            if(distX == 0 && distY == 0) {
              // No movement.
              this._setStationaryAnimation();
            }
            else if(distX == 0) {
              // Moving vertically only.
              this._setVerticalAnimation(distY);
            }
            else if(distY == 0) {
              // Moving horizontally only.
              this._setHorizontalAnimation(distX);
            }
            else {
              // Moving diagonally.
              this._setDiagonalAnimation(distX, distY);
            }

            if(this._shoot === 'on') {
              if(ClientSound.status.beamFire) {
                createjs.Sound.play('beamFire', {
                  volume: 0.025
                });
              }
            }
          },

          /**
           * Define air mage animation.
           * 
           * @name Player.prototype._defineAirMageAnimation
           * @function
           */
          _defineAirMageAnimations: function() {
            this.animation.define('walkDown', [73, 74, 75, 74], 8, -1).animation
                    .define('shootWalkDown', [85, 86, 87, 86], 8, -1).animation
                    .define('walkLeft', [121, 122, 123, 122], 8, -1).animation
                    .define('shootWalkLeft', [133, 134, 135, 134], 8, -1).animation
                    .define('walkRight', [169, 170, 171, 170], 8, -1).animation
                    .define('shootWalkRight', [181, 182, 183, 182], 8, -1).animation
                    .define('walkUp', [49, 50, 51, 50], 8, -1).animation
                    .define('shootWalkUp', [61, 62, 63, 62], 8, -1).animation
                    .define('walkDownLeft', [97, 98, 99, 98], 8, -1).animation
                    .define('shootWalkDownLeft', [109, 110, 111, 110], 8, -1).animation
                    .define('walkDownRight', [145, 146, 147, 146], 8, -1).animation
                    .define('shootWalkDownRight', [157, 158, 159, 158], 8, -1).animation
                    .define('walkUpLeft', [25, 26, 27, 26], 8, -1).animation
                    .define('shootWalkUpLeft', [37, 38, 39, 38], 8, -1).animation
                    .define('walkUpRight', [1, 2, 3, 2], 8, -1).animation
                    .define('shootWalkUpRight', [13, 14, 15, 14], 8, -1).animation
                    .define('shootDown', [85], 8, -1).animation
                    .define('shootLeft', [133], 8, -1).animation
                    .define('shootRight', [181], 8, -1).animation
                    .define('shootUp', [61], 8, -1).animation
                    .define('shootDownLeft', [109], 8, -1).animation
                    .define('shootDownRight', [157], 8, -1).animation
                    .define('shootUpLeft', [37], 8, -1).animation
                    .define('shootUpRight', [13], 8, -1).animation
                    .define('death',
                            [85, 109, 133, 37, 61, 13, 181, 157],
                            16,
                            -1).cell(73);

            this._restCell = 73;
          },

          /**
           * Define earth mage animation.
           * 
           * @name Player.prototype._defineEarthMageAnimation
           * @function
           */
          _defineEarthMageAnimations: function() {
            this.animation.define('walkDown', [76, 77, 78, 77], 8, -1).animation
                    .define('shootWalkDown', [88, 89, 90, 89], 8, -1).animation
                    .define('walkLeft', [124, 125, 126, 125], 8, -1).animation
                    .define('shootWalkLeft', [136, 137, 138, 137], 8, -1).animation
                    .define('walkRight', [172, 173, 174, 173], 8, -1).animation
                    .define('shootWalkRight', [184, 185, 186, 185], 8, -1).animation
                    .define('walkUp', [52, 53, 54, 53], 8, -1).animation
                    .define('shootWalkUp', [64, 65, 66, 65], 8, -1).animation
                    .define('walkDownLeft', [100, 101, 102, 101], 8, -1).animation
                    .define('shootWalkDownLeft', [112, 113, 114, 113], 8, -1).animation
                    .define('walkDownRight', [148, 149, 150, 149], 8, -1).animation
                    .define('shootWalkDownRight', [160, 161, 162, 161], 8, -1).animation
                    .define('walkUpLeft', [28, 29, 30, 29], 8, -1).animation
                    .define('shootWalkUpLeft', [40, 41, 42, 41], 8, -1).animation
                    .define('walkUpRight', [4, 5, 6, 5], 8, -1).animation
                    .define('shootWalkUpRight', [16, 17, 18, 17], 8, -1).animation
                    .define('shootDown', [88], 8, -1).animation
                    .define('shootLeft', [136], 8, -1).animation
                    .define('shootRight', [184], 8, -1).animation
                    .define('shootUp', [64], 8, -1).animation
                    .define('shootDownLeft', [112], 8, -1).animation
                    .define('shootDownRight', [160], 8, -1).animation
                    .define('shootUpLeft', [40], 8, -1).animation
                    .define('shootUpRight', [16], 8, -1).animation
                    .define('death',
                            [88, 112, 136, 40, 64, 16, 184, 160],
                            16,
                            -1).cell(76);

            this._restCell = 76;
          },

          /**
           * Define fire mage animation.
           * 
           * @name Player.prototype._defineFireMageAnimation
           * @function
           */
          _defineFireMageAnimations: function() {
            this.animation.define('walkDown', [79, 80, 81, 80], 8, -1).animation
                    .define('shootWalkDown', [91, 92, 93, 92], 8, -1).animation
                    .define('walkLeft', [127, 128, 129, 128], 8, -1).animation
                    .define('shootWalkLeft', [139, 140, 141, 140], 8, -1).animation
                    .define('walkRight', [175, 176, 177, 176], 8, -1).animation
                    .define('shootWalkRight', [187, 188, 189, 188], 8, -1).animation
                    .define('walkUp', [55, 56, 57, 56], 8, -1).animation
                    .define('shootWalkUp', [67, 68, 69, 68], 8, -1).animation
                    .define('walkDownLeft', [103, 104, 105, 104], 8, -1).animation
                    .define('shootWalkDownLeft', [115, 116, 117, 116], 8, -1).animation
                    .define('walkDownRight', [151, 152, 153, 152], 8, -1).animation
                    .define('shootWalkDownRight', [163, 164, 165, 164], 8, -1).animation
                    .define('walkUpLeft', [31, 32, 33, 32], 8, -1).animation
                    .define('shootWalkUpLeft', [43, 44, 45, 44], 8, -1).animation
                    .define('walkUpRight', [7, 8, 9, 8], 8, -1).animation
                    .define('shootWalkUpRight', [19, 20, 21, 20], 8, -1).animation
                    .define('shootDown', [91], 8, -1).animation
                    .define('shootLeft', [139], 8, -1).animation
                    .define('shootRight', [187], 8, -1).animation
                    .define('shootUp', [67], 8, -1).animation
                    .define('shootDownLeft', [115], 8, -1).animation
                    .define('shootDownRight', [163], 8, -1).animation
                    .define('shootUpLeft', [43], 8, -1).animation
                    .define('shootUpRight', [19], 8, -1).animation
                    .define('death',
                            [91, 115, 139, 43, 67, 19, 187, 163],
                            16,
                            -1).cell(79);

            this._restCell = 79;
          },

          /**
           * Define water mage animation.
           * 
           * @name Player.prototype._defineWaterMageAnimation
           * @function
           */
          _defineWaterMageAnimations: function() {
            this.animation.define('walkDown', [82, 83, 84, 83], 8, -1).animation
                    .define('shootWalkDown', [94, 95, 96, 95], 8, -1).animation
                    .define('walkLeft', [130, 131, 132, 131], 8, -1).animation
                    .define('shootWalkLeft', [142, 143, 144, 143], 8, -1).animation
                    .define('walkRight', [178, 179, 180, 179], 8, -1).animation
                    .define('shootWalkRight', [190, 191, 192, 191], 8, -1).animation
                    .define('walkUp', [58, 59, 60, 59], 8, -1).animation
                    .define('shootWalkUp', [70, 71, 72, 71], 8, -1).animation
                    .define('walkDownLeft', [106, 107, 108, 107], 8, -1).animation
                    .define('shootWalkDownLeft', [118, 119, 120, 119], 8, -1).animation
                    .define('walkDownRight', [154, 155, 156, 155], 8, -1).animation
                    .define('shootWalkDownRight', [166, 167, 168, 167], 8, -1).animation
                    .define('walkUpLeft', [34, 35, 36, 35], 8, -1).animation
                    .define('shootWalkUpLeft', [46, 47, 48, 47], 8, -1).animation
                    .define('walkUpRight', [10, 11, 12, 11], 8, -1).animation
                    .define('shootWalkUpRight', [22, 23, 24, 23], 8, -1).animation
                    .define('shootDown', [94], 8, -1).animation
                    .define('shootLeft', [142], 8, -1).animation
                    .define('shootRight', [190], 8, -1).animation
                    .define('shootUp', [70], 8, -1).animation
                    .define('shootDownLeft', [118], 8, -1).animation
                    .define('shootDownRight', [166], 8, -1).animation
                    .define('shootUpLeft', [46], 8, -1).animation
                    .define('shootUpRight', [22], 8, -1).animation
                    .define('death',
                            [94, 118, 142, 46, 70, 22, 190, 166],
                            16,
                            -1).cell(82);

            this._restCell = 82;
          },

          /**
           * Select player death animation.
           * 
           * @name Player.prototype._setDeathAnimation
           * @function
           */
          _setDeathAnimation: function() {
            this.animation.select('death');
          },

          /**
           * Select player stationary animation.
           * 
           * @name Player.prototype._setStationaryAnimation
           * @function
           */
          _setStationaryAnimation: function() {
            this.animation.stop();

            if(this._shoot === 'on') {
              // Use stationary shoot animation.
              switch(this._facing) {
                case 'down':
                  this.animation.select('shootDown');
                  break;
                case 'up':
                  this.animation.select('shootUp');
                  break;
                case 'left':
                  this.animation.select('shootLeft');
                  break;
                case 'right':
                  this.animation.select('shootRight');
                  break;
                case 'upLeft':
                  this.animation.select('shootUpLeft');
                  break;
                case 'downLeft':
                  this.animation.select('shootDownLeft');
                  break;
                case 'upRight':
                  this.animation.select('shootUpRight');
                  break;
                case 'downRight':
                  this.animation.select('shootDownRight');
                  break;
              }
            }
            else {
              // Use stationary animation.
              switch(this._facing) {
                case 'down':
                  this.animation.select('walkDown');
                  break;
                case 'up':
                  this.animation.select('walkUp');
                  break;
                case 'left':
                  this.animation.select('walkLeft');
                  break;
                case 'right':
                  this.animation.select('walkRight');
                  break;
                case 'upLeft':
                  this.animation.select('walkUpLeft');
                  break;
                case 'downLeft':
                  this.animation.select('walkDownLeft');
                  break;
                case 'upRight':
                  this.animation.select('walkUpRight');
                  break;
                case 'downRight':
                  this.animation.select('walkDownRight');
                  break;
              }
            }
          },

          /**
           * Select player horizontal movement animation.
           * 
           * @name Player.prototype._setHorizontalAnimation
           * @function
           */
          _setHorizontalAnimation: function(distX) {
            if(distX < 0) {
              // Moving left.
              if(this._shoot === 'on') {
                this.animation.select('shootWalkLeft');
              }
              else {
                this.animation.select('walkLeft');
              }
            }
            else {
              // Moving right.
              if(this._shoot === 'on') {
                this.animation.select('shootWalkRight');
              }
              else {
                this.animation.select('walkRight');
              }
            }
          },

          /**
           * Select player vertical movement animation.
           * 
           * @name Player.prototype._setVerticalAnimation
           * @function
           */
          _setVerticalAnimation: function(distY) {
            if(distY < 0) {
              // Moving up.
              if(this._shoot === 'on') {
                this.animation.select('shootWalkUp');
              }
              else {
                this.animation.select('walkUp');
              }
            }
            else {
              // Moving down.
              if(this._shoot === 'on') {
                this.animation.select('shootWalkDown');
              }
              else {
                this.animation.select('walkDown');
              }
            }
          },

          /**
           * Select player diagonal movement animation.
           * 
           * @name Player.prototype._setDiagonalAnimation
           * @function
           * @param {distX}
           *          distX - The displacement x component.
           * @param {distY}
           *          distY - The displacement y component.
           */
          _setDiagonalAnimation: function(distX, distY) {
            if(distX < 0) {
              // Moving left.
              if(distY < 0) {
                // Moving up.
                if(this._shoot === 'on') {
                  this.animation.select('shootWalkUpLeft');
                }
                else {
                  this.animation.select('walkUpLeft');
                }
              }
              else {
                // Moving down.
                if(this._shoot === 'on') {
                  this.animation.select('shootWalkDownLeft');
                }
                else {
                  this.animation.select('walkDownLeft');
                }
              }
            }
            else {
              // Moving right.
              if(distY < 0) {
                // Moving up.
                if(this._shoot === 'on') {
                  this.animation.select('shootWalkUpRight');
                }
                else {
                  this.animation.select('walkUpRight');
                }
              }
              else {
                // Moving down.
                if(this._shoot === 'on') {
                  this.animation.select('shootWalkDownRight');
                }
                else {
                  this.animation.select('walkDownRight');
                }
              }
            }
          }
        });

if(typeof (module) !== 'undefined' && typeof (module.exports) !== 'undefined') {
  module.exports = Player;
}