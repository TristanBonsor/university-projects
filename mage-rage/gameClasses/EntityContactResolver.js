/**
 * @file EntityContactResolver definitions.
 * @author kidthales
 * @copyright Team Awesome 2014
 */

/**
 * Contains all server side body contact resolution logic for entity collisions.
 * This class is intended to be added as a component to the server.
 * 
 * @class EntityContactResolver
 */
var EntityContactResolver = IgeEntity
        .extend({
          /**
           * The id of this class type.
           * 
           * @name EntityContactResolver.prototype.classId
           */
          classId: 'EntityContactResolver',

          /**
           * The id of this component type.
           * 
           * @name EntityContactResolver.prototype.componentId
           */
          componentId: 'contactResolver',

          /**
           * Initializes the entity contact resolver component. Called when
           * added to the server.
           * 
           * @name EntityContactResolver.prototype.init
           * @function
           */
          init: function() {
            var self = this;

            /**
             * Look up table for differing beam collisions/combination
             * resolution functions.
             * 
             * @name EntityContactResolver.prototype.sideEffectIndex
             * @public
             */
            this.sideEffectIndex = {
              air: {
                earth: function(pos) {
                  ige.server.sideEffectFactory('tornado', pos);
                },
                fire: function(pos) {
                  ige.server.sideEffectFactory('lightning', pos);
                },
                water: function(pos) {
                  ige.server.sideEffectFactory('ice', pos);
                }
              },
              earth: {
                air: function(pos) {
                  ige.server.sideEffectFactory('tornado', pos);
                },
                fire: function(pos) {
                  ige.server.sideEffectFactory('lava', pos);
                },
                water: function(pos) {
                  ige.server.sideEffectFactory('mud', pos);
                }
              },
              fire: {
                air: function(pos) {
                  ige.server.sideEffectFactory('lightning', pos);
                },
                earth: function(pos) {
                  ige.server.sideEffectFactory('lava', pos);
                },
                water: function(pos) {
                  ige.server.sideEffectFactory('steam', pos);
                }
              },
              water: {
                air: function(pos) {
                  ige.server.sideEffectFactory('ice', pos);
                },
                earth: function(pos) {
                  ige.server.sideEffectFactory('mud', pos);
                },
                fire: function(pos) {
                  ige.server.sideEffectFactory('steam', pos);
                }
              }
            };

            /**
             * Event handler for the beginning of contact between to entities.
             * Delegates resolution logic based on the entities involved.
             * 
             * @name EntityContactResolver.prototype.begin
             * @function
             * @param {Contact}
             *          contact - Contact data.
             */
            this.begin = function(contact) {
              var entityA = contact.igeEntityA();
              var entityB = contact.igeEntityB();

              // Find out what kind of entity contact to resolve.

              if(contact.igeEitherCategory('Player')
                 && contact.igeEitherCategory('BeamParticle')) {
                // Resolve player/beam particle.
                self._playerBeamParticleBegin(entityA, entityB, contact);
              }
              else if(contact.igeEitherCategory('Player')
                      && contact.igeEitherCategory('SideEffect')) {
                // Resolve player/side effect.
                self._playerSideEffectBegin(entityA, entityB, contact);
              }
              else if(contact.igeEitherCategory('Player')
                      && contact.igeEitherCategory('Wall')) {
                // Resolve player/wall.
                self._playerWallBegin(entityA, entityB, contact);
              }
              else if(contact.igeEitherCategory('Wall')
                      && contact.igeEitherCategory('BeamParticle')) {
                // Resolve beam particle/wall.
                self._beamParticleWallBegin(entityA, entityB, contact);
              }
              else if(contact.igeEitherCategory('Wall')
                      && contact.igeEitherCategory('SideEffect')) {
                // Resolve side effect/wall.
                self._sideEffectWallBegin(entityA, entityB, contact);
              }
              else if(entityA._beamType !== undefined
                      && entityB._beamType !== undefined) {
                // Resolve beam particle/beam particle.
                self._beamParticleBeamParticleBegin(entityA, entityB, contact);
              }
            };

            /**
             * Event handler for the end of contact between to entities.
             * Delegates resolution logic based on the entities involved.
             * 
             * @name EntityContactResolver.prototype.end
             * @function
             * @param {Contact}
             *          contact - Contact data.
             */
            this.end = function(contact) {
              var entityA = contact.igeEntityA();
              var entityB = contact.igeEntityB();

              if(contact.igeEitherCategory('Player')
                 && contact.igeEitherCategory('BeamParticle')) {
                self._playerBeamParticleEnd(entityA, entityB, contact);
              }
              else if(contact.igeEitherCategory('Player')
                      && contact.igeEitherCategory('SideEffect')) {
                // Resolve player/side effect.
                self._playerSideEffectEnd(entityA, entityB, contact);
              }
              else if(contact.igeEitherCategory('BeamParticle')
                      && contact.igeEitherCategory('Wall')) {
                self._beamParticleWallEnd(entityA, entityB, contact);
              }
            };

            /**
             * Event handler for any pre-solving of contact between to entities.
             * Delegates resolution logic based on the entities involved.
             * 
             * @name EntityContactResolver.prototype.preSolver
             * @function
             * @param {Contact}
             *          contact - Contact data.
             */
            this.preSolver = function(contact) {
              var entityA = contact.igeEntityA();
              var entityB = contact.igeEntityB();

              // Find out what kind of entity contact to resolve.

              if(contact.igeEitherCategory('Player')
                 && contact.igeEitherCategory('BeamParticle')) {
                // Resolve player/beam particle.
                self._playerBeamParticlePreSolver(entityA, entityB, contact);
              }
              else if(contact.igeEitherCategory('Player')
                      && contact.igeEitherCategory('SideEffect')) {
                // Resolve player/side effect.
                self._playerSideEffectPreSolver(entityA, entityB, contact);
              }
              else if(contact.igeEitherCategory('BeamParticle')
                      && contact.igeEitherCategory('SideEffect')) {
                // Resolve beam particle/side effect.
                self
                        ._beamParticleSideEffectPreSolver(entityA,
                                                          entityB,
                                                          contact);
              }
              else if(entityA._beamType !== undefined
                      && entityB._beamType !== undefined) {
                // Resolve beam particle/beam particle.
                self._beamParticleBeamParticlePreSolver(entityA,
                                                        entityB,
                                                        contact);
              }
            };
          },

          /**
           * Resolve the beginning of contact between player and beam particle
           * entities.
           * 
           * @name EntityContactResolver.prototype._playerBeamParticleBegin
           * @function
           * @param {Entity}
           *          entityA
           * @param {Entity}
           *          entityB
           * @param {Contact}
           *          contact - Contact data.
           * @public
           */
          _playerBeamParticleBegin: function(entityA, entityB, contact) {
            var entities = this._separate(entityA, entityB, 'Player');

            // Resolve beam strikes on mages of a differing type.
            if(entities.primary._mageType !== entities.other._beamType) {
              entities.primary.applyHitPointDamage(1, entities.other._beamType);
              entities.other.destroy();
            }
          },

          /**
           * Resolve the beginning of contact between player and side effect
           * entities. Applies damage to player if side effect is lightning.
           * 
           * @name EntityContactResolver.prototype._playerSideEffectBegin
           * @function
           * @param {Entity}
           *          entityA
           * @param {Entity}
           *          entityB
           * @param {Contact}
           *          contact - Contact data.
           * @public
           */
          _playerSideEffectBegin: function(entityA, entityB, contact) {
            var entities = this._separate(entityA, entityB, 'Player');

            var player = entities.primary;
            var sideEffect = entities.other;

            switch(sideEffect._sideEffectType) {
              case 'lava':
                player._onLava = true;
                break;
              case 'ice':
                player._onIce = true;
                break;
              case 'mud':
                player._onMud = true;
                break;
              case 'tornado':
                player._onTornado = true;
                break;
              case 'steam':
                player._onSteam = true;
                break;
              case 'lightning':
                player.applyHitPointDamage(2, 'lightning');
                break;
            }
          },

          /**
           * Resolve the beginning of contact between player and wall entities.
           * 
           * @name EntityContactResolver.prototype._playerWallBegin
           * @function
           * @param {Entity}
           *          entityA
           * @param {Entity}
           *          entityB
           * @param {Contact}
           *          contact - Contact data.
           * @public
           */
          _playerWallBegin: function(entityA, entityB, contact) {
            var entities = this._separate(entityA, entityB, 'Player');

            var player = entities.primary;
            if(player._thrown) {
              player._thrown = false;
              player.velocity.x(0);
              player.velocity.y(0);
              player.applyHitPointDamage(3, 'tornado');
            }
          },

          /**
           * Resolve the beginning of contact between two beam particle
           * entities.
           * 
           * @name EntityContactResolver.prototype._beamParticleBeamParticleBegin
           * @function
           * @param {Entity}
           *          beamParticleA
           * @param {Entity}
           *          beamParticleB
           * @param {Contact}
           *          contact - Contact data.
           * @public
           */
          _beamParticleBeamParticleBegin: function(beamParticleA,
                                                   beamParticleB,
                                                   contact) {
            // Beams of differing types will cause side effects!
            if(beamParticleA._beamType !== beamParticleB._beamType) {
              var typeA = beamParticleA._beamType;
              var typeB = beamParticleB._beamType;
              var pos = beamParticleA.worldPosition();

              this.sideEffectIndex[typeA][typeB](pos);

              // Destroy beams when they collide if they differ in type.
              beamParticleA.destroy();
              beamParticleB.destroy();
            }
          },

          /**
           * Resolve the beginning of contact between wall and beam particle
           * entities.
           * 
           * @name EntityContactResolver.prototype._beamParticleWallBegin
           * @function
           * @param {Entity}
           *          entityA
           * @param {Entity}
           *          entityB
           * @param {Contact}
           *          contact - Contact data.
           * @public
           */
          _beamParticleWallBegin: function(entityA, entityB, contact) {
            var entities = this._separate(entityA, entityB, 'BeamParticle');

            if(!entities.primary._inPlayer) {
              entities.primary.destroy();
            }
          },

          /**
           * Resolve the beginning of contact between side effect and wall
           * entities.
           * 
           * @name EntityContactResolver.prototype._sideEffectWallBegin
           * @function
           * @param {Entity}
           *          entityA
           * @param {Entity}
           *          entityB
           * @param {Contact}
           *          contact - Contact data.
           * @public
           */
          _sideEffectWallBegin: function(entityA, entityB, contact) {
            var entities = this._separate(entityA, entityB, 'SideEffect');

            if(entities.primary._sideEffectType === 'lightning') {
              var lightning = entities.primary;
              var wall = entities.other;
              var angle = 0;
              var power = 0;

              if(wall._wallId === 'topWall') {
                angle = Math.random() * 3.14159 + 3.14159;
                power = -0.5;
              }
              else if(wall._wallId === 'bottomWall') {
                angle = Math.random() * 3.14159;
                power = -0.5;
              }
              else if(wall._wallId === 'leftWall') {
                angle = Math.random() * 3.14159 + 4.71238;
                power = 0.5;
              }
              else if(wall._wallId === 'rightWall') {
                angle = Math.random() * 3.14159 + 1.57079;
                power = 0.5;
              }

              lightning.velocity.byAngleAndPower(angle, power, false);
            }
          },

          /**
           * Resolve the end of contact between player and beam particle
           * entities.
           * 
           * @name EntityContactResolver.prototype._playerBeamParticleEnd
           * @function
           * @param {Entity}
           *          entityA
           * @param {Entity}
           *          entityB
           * @param {Contact}
           *          contact - Contact data.
           * @public
           */
          _playerBeamParticleEnd: function(entityA, entityB, contact) {
            var entities = this._separate(entityA, entityB, 'BeamParticle');

            entities.primary._inPlayer = false;
          },

          /**
           * Resolve the end of contact between player and side effect entities.
           * 
           * @name EntityContactResolver.prototype._playerSideEffectEnd
           * @function
           * @param {Entity}
           *          entityA
           * @param {Entity}
           *          entityB
           * @param {Contact}
           *          contact - Contact data.
           * @public
           */
          _playerSideEffectEnd: function(entityA, entityB, contact) {
            var entities = this._separate(entityA, entityB, 'Player');

            var player = entities.primary;
            var sideEffect = entities.other;

            switch(sideEffect._sideEffectType) {
              case 'lava':
                player._onLava = false;
                break;
              case 'ice':
                player._onIce = false;
                break;
              case 'mud':
                player._onMud = false;
                break;
              case 'tornado':
                player._onTornado = false;
                player._thrown = true;
                player.velocity.byAngleAndPower(2 * Math.random() * 3.14159,
                                                0.3,
                                                false);
                break;
              case 'steam':
                player._onSteam = false;
                break;
              case 'lightning':
                if(Math.random() < 0.05) {
                  ige.server.sideEffectFactory('lightning', sideEffect
                          .worldPosition());
                }
                break;
            }
          },

          /**
           * Resolve the end of contact between beam particle and wall entities.
           * 
           * @name EntityContactResolver.prototype._beamParticleWallEnd
           * @function
           * @param {Entity}
           *          entityA
           * @param {Entity}
           *          entityB
           * @param {Contact}
           *          contact - Contact data.
           * @public
           */
          _beamParticleWallEnd: function(entityA, entityB, contact) {
            var entities = this._separate(entityA, entityB, 'BeamParticle');

            if(!entities.primary._inPlayer) {
              entities.primary.destroy();
            }
          },

          /**
           * Resolve the pre-solving of contact between player and beam particle
           * entities.
           * 
           * @name EntityContactResolver.prototype._playerBeamParticlePreSolver
           * @function
           * @param {Entity}
           *          entityA
           * @param {Entity}
           *          entityB
           * @param {Contact}
           *          contact - Contact data.
           * @public
           */
          _playerBeamParticlePreSolver: function(entityA, entityB, contact) {
            var entities = this._separate(entityA, entityB, 'Player');

            // Allow beams to pass through mages of the same type.
            if(entities.primary._mageType === entities.other._beamType) {
              // Cancel the contact.
              contact.SetEnabled(false);
            }
          },

          /**
           * Resolve the pre-solving of contact between player and side effect
           * entities.
           * 
           * @name EntityContactResolver.prototype._playerSideEffectPreSolver
           * @function
           * @param {Entity}
           *          entityA
           * @param {Entity}
           *          entityB
           * @param {Contact}
           *          contact - Contact data.
           * @public
           */
          _playerSideEffectPreSolver: function(entityA, entityB, contact) {
            contact.SetEnabled(false);
          },

          /**
           * Resolve the pre-solving of contact between beam particle and side
           * effect entities.
           * 
           * @name EntityContactResolver.prototype._beamParticleSideEffectPreSolver
           * @function
           * @param {Entity}
           *          entityA
           * @param {Entity}
           *          entityB
           * @param {Contact}
           *          contact - Contact data.
           * @public
           */
          _beamParticleSideEffectPreSolver: function(entityA, entityB, contact) {
            contact.SetEnabled(false);
          },

          /**
           * Resolve the pre-solving of contact between two beam particle
           * entities.
           * 
           * @name EntityContactResolver.prototype._beamParticleBeamParticlePreSolver
           * @function
           * @param {Entity}
           *          beamParticleA
           * @param {Entity}
           *          beamParticleB
           * @param {Contact}
           *          contact - Contact data.
           * @public
           */
          _beamParticleBeamParticlePreSolver: function(beamParticleA,
                                                       beamParticleB,
                                                       contact) {
            // Allow beams to pass through beams of the same type.
            if(beamParticleA._beamType === beamParticleB._beamType) {
              // Cancel the contact.
              contact.SetEnabled(false);
            }
          },

          /**
           * Classify the given entities into an object containing primary and
           * other field; which entity is primary is specified by the category.
           * If category does not match either entity, the returned object will
           * contain undefined fields.
           * 
           * @name EntityContactResolver.prototype._separate
           * @function
           * @param {Entity}
           *          entityA
           * @param {Entity}
           *          entityB
           * @param {String}
           *          category
           * @public
           */
          _separate: function(entityA, entityB, category) {
            var primary, other;

            if(entityA.category() === category) {
              primary = entityA;
              other = entityB;
            }
            else if(entityB.category() === category) {
              primary = entityB;
              other = entityA;
            }

            return {
              primary: primary,
              other: other
            };
          }
        });

if(typeof (module) !== 'undefined' && typeof (module.exports) !== 'undefined') {
  module.exports = EntityContactResolver;
}