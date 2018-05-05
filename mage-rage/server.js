/**
 * @file Server definitions.
 * @author Isogenic Game Engine - modified by kidthales
 * @copyright Team Awesome 2014
 */

/**
 * Maintains state and behaviour of a game server.
 * 
 * @param {Options}
 *          options
 * @class Server
 */
var Server = IgeClass
        .extend({
          /**
           * The id of this class type.
           * 
           * @name Server.prototype.classId
           */
          classId: 'Server',

          /**
           * Is this the server?.
           * 
           * @name Server.prototype.Server
           */
          Server: true,

          /**
           * Initializes the server.
           * 
           * @name Server.prototype.init
           * @function
           * @param {Options}
           *          options
           */
          init: function(options) {
            var self = this;
            ige.timeScale(1);

            /**
             * Holds references to all current player entities. Look up by
             * client id.
             * 
             * @name Server.prototype.players
             * @public
             */
            this.players = {};

            /**
             * Holds reference to all current player entities. Look up by mage
             * type.
             * 
             * @name Server.prototype.mages
             * @public
             */
            this.mages = {};

            /**
             * Holds the available mage types.
             * 
             * @name Server.prototype.availableMageTypes
             * @public
             */
            this.availableMageTypes = ['air', 'earth', 'fire', 'water'];

            /**
             * Holds references to the any walls in world space. Look up by wall
             * id.
             * 
             * @name Server.prototype.walls
             * @public
             */
            this.walls = {};

            // Add the server-side game methods / event handlers.
            this.implement(ServerNetworkEvents);

            // Add physics and setup physics world
            ige.addComponent(IgeBox2dComponent).box2d.sleep(true).box2d
                    .gravity(0, 0).box2d.createWorld().box2d.start();

            // Add the networking component.
            ige.addComponent(IgeNetIoComponent)
            // Start the network server.
            .network.start(2000, function() {
              // Networking has started so start the game engine.
              ige.start(function(success) {
                // Check if the engine started successfully.
                if(success) {
                  // Defined in ./gameClasses/ServerNetworkEvents.js.

                  ige.network.define('playerEntity', self._onPlayerEntity);

                  ige.network.define('postScores');

                  ige.network.define('playerControlLeftDown',
                                     self._onPlayerLeftDown);
                  ige.network.define('playerControlRightDown',
                                     self._onPlayerRightDown);
                  ige.network.define('playerControlUpDown',
                                     self._onPlayerUpDown);
                  ige.network.define('playerControlDownDown',
                                     self._onPlayerDownDown);
                  ige.network.define('playerControlShootDown',
                                     self._onPlayerShootDown);

                  ige.network.define('playerControlLeftUp',
                                     self._onPlayerLeftUp);
                  ige.network.define('playerControlRightUp',
                                     self._onPlayerRightUp);
                  ige.network.define('playerControlUpUp', self._onPlayerUpUp);
                  ige.network.define('playerControlDownUp',
                                     self._onPlayerDownUp);
                  ige.network.define('playerControlShootUp',
                                     self._onPlayerShootUp);

                  ige.network.on('connect', self._onPlayerConnect);
                  ige.network.on('disconnect', self._onPlayerDisconnect);

                  // Add the network stream component.
                  ige.network.addComponent(IgeStreamComponent)
                  // Send a stream update once every 30 milliseconds.
                  .stream.sendInterval(30)
                  // Start the stream.
                  .stream.start();

                  // Accept incoming network connections.
                  ige.network.acceptConnections(true);

                  /**
                   * Posts scores to clients on a regular interval.
                   * 
                   * @name Server.prototype.postScoresInterval
                   * @public
                   */
                  self.postScoresInterval = new IgeInterval().init(function() {
                    var player;
                    var scores = {};

                    if(self.mages['air']) {
                      player = self.mages['air'];
                      scores.air = {
                        name: player._playerName,
                        kills: player._kills,
                        deaths: player._deaths
                      };
                    }

                    if(self.mages['earth']) {
                      player = self.mages['earth'];
                      scores.earth = {
                        name: player._playerName,
                        kills: player._kills,
                        deaths: player._deaths
                      };
                    }

                    if(self.mages['fire']) {
                      player = self.mages['fire'];
                      scores.fire = {
                        name: player._playerName,
                        kills: player._kills,
                        deaths: player._deaths
                      };
                    }

                    if(self.mages['water']) {
                      player = self.mages['water'];
                      scores.water = {
                        name: player._playerName,
                        kills: player._kills,
                        deaths: player._deaths
                      };
                    }

                    ige.network.send('postScores', scores);
                  }, 10000);

                  /**
                   * The main scene that holds all other scene layers.
                   * 
                   * @name Server.prototype.mainScene
                   * @public
                   */
                  self.mainScene = new IgeScene2d().id('mainScene');

                  /**
                   * The background scene layer.
                   * 
                   * @name Server.prototype.backgroundScene
                   * @public
                   */
                  self.backgroundScene = new IgeScene2d().id('backgroundScene')
                          .mount(self.mainScene);

                  /**
                   * The foreground scene layer.
                   * 
                   * @name Server.prototype.foregroundScene
                   * @public
                   */
                  self.foregroundScene = new IgeScene2d().id('foregroundScene')
                          .mount(self.mainScene);

                  /**
                   * Queues and creates desired side effect entities.
                   * 
                   * @name Server.prototype.sideEffectEmitter
                   * @public
                   */
                  self.sideEffectEmitter = new SideEffectEmitter()
                          .mount(self.mainScene);

                  // Create the room boundaries in box2d.

                  self.walls['topWall'] = self
                          .wallFactory('topWall',
                                       new IgePoint(-16, -392, 0),
                                       768,
                                       64,
                                       self.foregroundScene);

                  self.walls['bottomWall'] = self
                          .wallFactory('bottomWall',
                                       new IgePoint(-16, 308, 0),
                                       768,
                                       32,
                                       self.foregroundScene);

                  self.walls['leftWall'] = self
                          .wallFactory('leftWall',
                                       new IgePoint(-352, 0, 0),
                                       32,
                                       768,
                                       self.foregroundScene);

                  self.walls['rightWall'] = self
                          .wallFactory('rightWall',
                                       new IgePoint(320, 0, 0),
                                       32,
                                       768,
                                       self.foregroundScene);

                  /**
                   * The main viewport and of the main scene.
                   * 
                   * @name Server.prototype.vp1
                   */
                  self.vp1 = new IgeViewport().id('vp1').autoSize(true)
                          .scene(self.mainScene).drawBounds(true).mount(ige);

                  // Setup global entity contact listener.

                  self.addComponent(EntityContactResolver);

                  ige.box2d.contactListener(
                  // Listen for when contact's begin
                  self.contactResolver.begin,
                  // Listen for when contact's end
                  self.contactResolver.end,
                  // Handle pre-solver events
                  self.contactResolver.preSolver);
                }
              });
            });
          },

          /**
           * Creates a new player entity for a given client.
           * 
           * @name Server.prototype.playerFactory
           * @function
           * @param {String}
           *          clientId
           * @param {String}
           *          playerName
           * @param {Scene}
           *          scene - The scene to mount this player to.
           * @returns {Player}
           */
          playerFactory: function(clientId, playerName, scene) {
            // Randomly select an available mage type.
            var index = Math.floor(Math.random()
                                   * this.availableMageTypes.length);
            var mageType = this.availableMageTypes[index];

            // Remove selected mage type from availability list.
            this.availableMageTypes.splice(index, 1);

            var player = new Player(clientId, mageType, playerName)
                    .mount(scene);

            return player;
          },

          /**
           * Creates a new side effect entity mounted to the foreground scene.
           * 
           * @name Server.prototype.sideEffectFactory
           * @function
           * @param {String}
           *          type
           * @param {IgePoint}
           *          pos
           * @returns {SideEffect}
           */
          sideEffectFactory: function(type, pos) {
            switch(type) {
              case 'lava':
                ige.server.sideEffectEmitter.append({
                  type: 'lava',
                  pos: pos
                });
                break;
              case 'ice':
                ige.server.sideEffectEmitter.append({
                  type: 'ice',
                  pos: pos
                });
                break;
              case 'mud':
                ige.server.sideEffectEmitter.append({
                  type: 'mud',
                  pos: pos
                });
                break;
              case 'tornado':
                ige.server.sideEffectEmitter.append({
                  type: 'tornado',
                  pos: pos
                });
                break
              case 'steam':
                ige.server.sideEffectEmitter.append({
                  type: 'steam',
                  pos: pos
                });
                break;
              case 'lightning':
                ige.server.sideEffectEmitter.append({
                  type: 'lightning',
                  pos: pos
                });
                break
            }
          },

          /**
           * Creates a new beam particle entity mounted to the foreground scene.
           * 
           * @name Server.prototype.beamParticleFactory
           * @function
           * @param {String}
           *          type
           * @param {String}
           *          facing
           * @param {IgePoint}
           *          pos
           * @param {IgePoint}
           *          velocity
           * @returns {BeamParticle}
           */
          beamParticleFactory: function(type, facing, pos, vel) {
            new BeamParticle(type, facing).translateTo(pos.x, pos.y, pos.z).velocity
                    .x(vel.x).velocity.y(vel.y).lifeSpan(3000)
                    .mount(this.foregroundScene);
          },

          /**
           * Creates a new wall entity.
           * 
           * @name Server.prototype.wallFactory
           * @function
           * @param {String}
           *          id - Unique identifier.
           * @param {IgePoint}
           *          pos - (x,y,z) of center point of wall.
           * @param {Number}
           *          width
           * @param {Number}
           *          height
           * @param {Scene}
           *          scene - The scene to mount this player to.
           * @returns {Wall}
           */
          wallFactory: function(id, pos, width, height, scene) {
            var wall = new Wall(id, pos, width, height).mount(scene);

            return wall;
          }
        });

if(typeof (module) !== 'undefined' && typeof (module.exports) !== 'undefined') {
  module.exports = Server;
}