/**
 * @file Client definitions.
 * @author Isogenic Game Engine - modified by kidthales
 * @copyright Team Awesome 2014
 */

/**
 * Maintains state and behaviour of a game client.
 * 
 * @class Client
 */
var Client = IgeClass
        .extend({
          /**
           * The id of this class type.
           * 
           * @name Client.prototype.classId
           */
          classId: 'Client',

          /**
           * Initializes the client.
           * 
           * @name Client.prototype.init
           * @function
           */
          init: function() {
            var self = this;

            ige.showStats(1);
            ige.globalSmoothing(true);

            // Enable networking.
            ige.addComponent(IgeNetIoComponent);

            // Implement our game methods.
            this.implement(ClientNetworkEvents);

            // Create the HTML canvas.
            ige.createFrontBuffer(true);

            // Begin async loading of audio resources.
            ClientSound.init();

            // Load the splash screen.
            this._loadSplashScreen();
          },

          /**
           * Try to join the game. Begin loading textures, try connecting to
           * game server upon load completion.
           * 
           * @name Client.prototype._joinGame
           * @function
           */
          _joinGame: function() {
            var playerName = document.getElementById('mageRagePlayerNameField').value;

            var playerNameRegex = /^[a-zA-Z0-9]+$/;

            var validPlayerName = playerName.match(playerNameRegex);

            if(validPlayerName === null) {
              alert('Invalid name. Use one or more alphanumeric characters.');
              return;
            }

            ige.client._validPlayerName = validPlayerName;

            document.getElementById('mageRageDiv').style.display = 'none';

            ige.client._initOnTexturesLoaded();
            ige.client._loadTextures();
          },

          /**
           * Load the splash screen.
           * 
           * @name Client.prototype._loadSplashScreen
           * @function
           */
          _loadSplashScreen: function() {
            var self = this;

            var mageRageDiv = document.createElement('DIV');
            mageRageDiv.id = 'mageRageDiv';
            mageRageDiv.style.cssText = 'position:absolute;left: 37.5%;top: 25%;width: 410px;height: 410px;margin-left: -35px; margin-top: -35px;background-color:#eee;';

            var logo = document.createElement('IMG');
            logo.id = 'mageRageLogo';
            logo.src = 'assets/images/logo.png';
            logo.style.cssText = 'margin:0 auto;width:410px;';

            var infoLink = document.createElement('P');
            infoLink.id = 'whatIsMageRageLink';
            infoLink.style.cssText = 'text-align:center; font-family:Arial, Helvetica, sans-serif';
            infoLink.innerHTML = '<a href="">What is Mage Rage?</a>';

            infoLink.onclick = function() {
              window
                      .open('whatismagerage.html',
                            '',
                            'width=512,height=512,location=no,menubar=no,status=no,titlebar=no,toolbar=no');
              return false;
            };

            mageRageDiv.appendChild(logo);
            mageRageDiv.appendChild(infoLink);

            this._loadLoginBox(mageRageDiv);

            document.body.appendChild(mageRageDiv);
          },

          /**
           * Load the login box.
           * 
           * @name Client.prototype._loadLoginBox
           * @function
           * @param {DOMObject}
           *          parent - The parent DOM object that the login box will
           *          mount to.
           */
          _loadLoginBox: function(parent) {
            var self = this;

            var loginDiv = document.createElement('DIV');
            loginDiv.id = 'mageRageLoginDiv';

            var playerNameLabel = document.createElement('P');
            playerNameLabel.id = 'mageRagePlayerNameLabel';
            playerNameLabel.style.cssText = 'font-family:Arial, Helvetica, sans-serif;';
            playerNameLabel.innerHTML = '<b>Player Name:</b>';

            var playerNameField = document.createElement('INPUT');
            playerNameField.id = 'mageRagePlayerNameField';
            playerNameField.name = 'mageRagePlayerNameField';
            playerNameField.type = 'text';
            playerNameField.value = '';
            playerNameField.style.cssText = 'width:405px; height:35px; font-size:22px;';

            var loginButton = document.createElement('INPUT');
            loginButton.id = 'mageRageLoginButton';
            loginButton.name = 'mageRageLoginButton';
            loginButton.type = 'button';
            loginButton.value = 'Play';
            loginButton.onclick = self._joinGame;
            loginButton.style.cssText = 'background-color:#900; width:410px; height:45px; margin:10px auto 0;';

            loginDiv.appendChild(playerNameLabel);
            loginDiv.appendChild(playerNameField);
            loginDiv.appendChild(loginButton);
            parent.appendChild(loginDiv);
          },

          /**
           * Initialize the callback and event listener for the texture load
           * event. Begin engine, start networking, and play game on texture
           * load.
           * 
           * @name Client.prototype._initOnTexturesLoaded
           * @function
           */
          _initOnTexturesLoaded: function() {
            var self = this;

            // Begin game execution when all textures are loaded.
            ige.on('texturesLoaded', function() {
              ige.start(function(success) {
                // Check if the engine started successfully.
                if(success) {

                  // Try to start theme music once the resource has loaded.
                  ClientSound.musicLoadInterval = new IgeInterval()
                          .init(function() {
                            if(!ClientSound.musicLoadIntervalPlaying
                               && ClientSound.status.music) {
                              createjs.Sound.play('music', {
                                loop: -1,
                                volume: 0.5
                              });
                              ClientSound.musicLoadIntervalPlaying = true;
                            }
                            else if(ClientSound.musicLoadIntervalPlaying) {
                              this.cancel();
                            }
                          }, 1000);

                  // Start the networking.
                  ige.network
                          .start('http://localhost:2000', function() {
                            // Setup the network command listeners
                            ige.network.define('playerEntity',
                                               self._onPlayerEntity);

                            ige.network
                                    .define('postScores', self._onPostScores);

                            // Setup the network stream handler.
                            ige.network.addComponent(IgeStreamComponent)
                            // Render the simulation 160 milliseconds in the
                            // past.
                            .stream.renderLatency(80)
                            // Create a listener that will fire whenever an
                            // entity is created
                            // because of the incoming stream data.
                            .stream.on('entityCreated', function(entity) {
                              self.log('Stream entity created with ID: '
                                       + entity.id());

                              if(entity.category() === 'Player') {
                                // Mount the floating UI widgets associated with
                                // a player to
                                // the scene graph.
                                entity.mountFloatingUi(self.foregroundScene);

                                if(entity.id() === self.playerEntityId) {
                                  // Mount UI elements specific to client's own
                                  // player entity.
                                  entity.mountFixedUi(self.uiScene);
                                }
                              }
                            });

                            // Create our scene layers.

                            /**
                             * The main scene that holds all other scene layers.
                             * 
                             * @name Client.prototype.mainScene
                             */
                            self.mainScene = new IgeScene2d().id('mainScene');

                            /**
                             * The background scene layer.
                             * 
                             * @name Client.prototype.backgroundScene
                             */
                            self.backgroundScene = new IgeScene2d()
                                    .id('backgroundScene').layer(0)
                                    .mount(self.mainScene);

                            /**
                             * The foreground scene layer.
                             * 
                             * @name Client.prototype.foregroundScene
                             */
                            self.foregroundScene = new IgeScene2d()
                                    .id('foregroundScene').layer(1)
                                    .mount(self.mainScene);

                            /**
                             * The ui/hud scene layer.
                             * 
                             * @name Client.prototype.uiScene
                             */
                            self.uiScene = new IgeScene2d().id('uiScene')
                                    .layer(2).ignoreCamera(true)
                                    .mount(self.mainScene);

                            /**
                             * The main viewport and of the main scene.
                             * 
                             * @name Client.prototype.vp1
                             */
                            self.vp1 = new IgeViewport().id('vp1')
                                    .autoSize(true).scene(self.mainScene)
                                    .drawBounds(false).mount(ige);

                            // Create the texture map that will work as our
                            // "tile background".

                            /**
                             * Tile background texture map.
                             * 
                             * @name Client.prototype.textureMap1
                             */
                            self.textureMap1 = new IgeTextureMap().depth(0)
                                    .tileWidth(32).tileHeight(32)
                                    .translateTo(-384, -384, 0)
                                    .mount(self.backgroundScene);

                            var texIndex = self.textureMap1
                                    .addTexture(self.textures.room);

                            // Paint the map tiles.
                            for(var x = 0; x < 24; ++x) {
                              for(var y = 0; y < 24; ++y) {
                                self.textureMap1.paintTile(x,
                                                           y,
                                                           texIndex,
                                                           (y * 24 + x) + 1);
                              }
                            }

                            // Now set the texture map's cache data to dirty so
                            // it will be
                            // redrawn.
                            self.textureMap1.cacheDirty(true);

                            /**
                             * Player score board entity.
                             * 
                             * @name Client.prototype.scoreBoard
                             * @public
                             */
                            self.scoreBoard = new IgeFontEntity()
                                    .id('scoreBoard').depth(1000).width(100)
                                    .height(100).textAlignX(1)
                                    .colorOverlay('#daa520')
                                    .nativeFont('12pt Arial').nativeStroke(1)
                                    .nativeStrokeColor('#666666')
                                    .textLineSpacing(0)
                                    .translateTo(window.innerWidth / 2 - 100,
                                                 -window.innerHeight / 2 + 100,
                                                 0).mount(self.uiScene);

                            // Ask the server to create an entity for us.
                            ige.network.send('playerEntity', {
                              playerName: self._validPlayerName
                            });

                            // We don't create any entities here because the
                            // entities are created
                            // server-side and then streamed to the clients. If
                            // an entity is
                            // streamed to a client and the client doesn't have
                            // the entity in
                            // memory, the entity is automatically created.
                            // Woohoo!
                          });
                }
              });
            });
          },

          /**
           * Begin loading the textures.
           * 
           * @name Client.prototype._loadTextures
           * @function
           */
          _loadTextures: function() {
            /**
             * Maintains an index of game texture resources.
             * 
             * @name Client.prototype.textures
             */
            this.textures = {
              room: new IgeCellSheet('./assets/textures/rooms/room_B_768x768.png',
                                     24,
                                     24),
              mageSprites: new IgeCellSheet('./assets/textures/sprites/mage_sprite_sheet.png',
                                            12,
                                            16),
              lavaSprites: new IgeCellSheet('./assets/textures/sprites/lava_sprite_sheet.png',
                                            3,
                                            1),
              iceSprites: new IgeCellSheet('./assets/textures/sprites/ice_sprite_sheet.png',
                                           1,
                                           1),
              mudSprites: new IgeCellSheet('./assets/textures/sprites/mud_sprite_sheet.png',
                                           1,
                                           1),
              tornadoSprites: new IgeCellSheet('./assets/textures/sprites/tornado_sprite_sheet.png',
                                               5,
                                               1),
              steamSprites: new IgeCellSheet('./assets/textures/sprites/steam_sprite_sheet.png',
                                             5,
                                             1),
              lightningSprites: new IgeCellSheet('./assets/textures/sprites/lightning_sprite_sheet.png',
                                                 4,
                                                 1),
              beamParticle: new IgeTexture('./assets/textures/shapes/BeamParticle.js'),
              attributeBox: new IgeTexture('./assets/textures/shapes/AttributeBox.js'),
              deathParticle: new IgeTexture('./assets/textures/shapes/DeathParticle.js')
            };
          }
        });

if(typeof (module) !== 'undefined' && typeof (module.exports) !== 'undefined') {
  module.exports = Client;
}