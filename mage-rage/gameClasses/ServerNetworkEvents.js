/**
 * @file ServerNetworkEvents definitions used and implemented by the server.
 * @author Isogenic Game Engine - modified by kidthales.
 * @copyright Team Awesome 2014
 */

/**
 * Indexes all server network event handlers.
 * 
 * @namespace
 */
var ServerNetworkEvents = {
  /**
   * Called when the network tells us a new client has connected to the server.
   * This is the point we can return true to reject the client connection if we
   * wanted to.
   * 
   * @name ServerNetworkEvents.prototype._onPlayerConnect
   * @function
   * @param {Socket}
   *          socket
   * @public
   */
  _onPlayerConnect: function(socket) {
    if(ige.server.availableMageTypes.length === 0) {
      // All four mage types are in use - reject the client connection.
      return true;
    }
    // Don't reject the client connection.
    return false;
  },

  /**
   * Called when the network tells us a client has disconnected from the server.
   * This is where we recycle and destroy any player/client related resources.
   * 
   * @name ServerNetworkEvents.prototype._onPlayerDisconnect
   * @function
   * @param {String}
   *          clientId
   * @public
   */
  _onPlayerDisconnect: function(clientId) {
    if(ige.server.players[clientId]) {
      var mageType = ige.server.players[clientId]._mageType;
      // Recycle the mage type.
      ige.server.availableMageTypes.push(mageType);

      // Remove the player from the game.
      ige.server.players[clientId].destroy();

      // Remove the reference to the player entity so that we don't leak
      // memory.
      delete ige.server.players[clientId];
      delete ige.server.mages[mageType];
    }
  },

  /**
   * Called when the network tells us a client wants to create a new player
   * entity. This is where we create a new player if there are less than 4
   * players and the client is not already being streamed its own player entity.
   * 
   * @name ServerNetworkEvents.prototype._onPlayerEntity
   * @function
   * @param {Data}
   *          data - Additional data sent from the client.
   * @param {String}
   *          clientId
   * @public
   */
  _onPlayerEntity: function(data, clientId) {
    if(!ige.server.players[clientId]) {
      // Create a new player entity.
      var player = ige.server.playerFactory(clientId,
                                            data.playerName,
                                            ige.server.foregroundScene);

      ige.server.players[clientId] = player;
      ige.server.mages[player._mageType] = player;

      var x = Math.floor(Math.random() * 600) - 300;
      var y = Math.floor(Math.random() * 600) - 300;

      player.translateTo(x, y, 0);

      // Tell the client to track their player entity.
      ige.network.send('playerEntity', player.id(), clientId);
    }
  },
  
  /**
   * Called when the network tells us a client has pressed the key to move its
   * player entity left.
   * 
   * @name ServerNetworkEvents.prototype._onPlayerLeftDown
   * @function
   * @param {Data}
   *          data - Additional data sent from the client.
   * @param {String}
   *          clientId
   * @public
   */
  _onPlayerLeftDown: function(data, clientId) {
    ige.server.players[clientId].playerControl.controls.left = true;
  },

  /**
   * Called when the network tells us a client has released the key to move its
   * player entity left.
   * 
   * @name ServerNetworkEvents.prototype._onPlayerLeftUp
   * @function
   * @param {Data}
   *          data - Additional data sent from the client.
   * @param {String}
   *          clientId
   * @public
   */
  _onPlayerLeftUp: function(data, clientId) {
    ige.server.players[clientId].playerControl.controls.left = false;
  },

  /**
   * Called when the network tells us a client has pressed the key to move its
   * player entity right.
   * 
   * @name ServerNetworkEvents.prototype._onPlayerRightDown
   * @function
   * @param {Data}
   *          data - Additional data sent from the client.
   * @param {String}
   *          clientId
   * @public
   */
  _onPlayerRightDown: function(data, clientId) {
    ige.server.players[clientId].playerControl.controls.right = true;
  },

  /**
   * Called when the network tells us a client has released the key to move its
   * player entity right.
   * 
   * @name ServerNetworkEvents.prototype._onPlayerRightUp
   * @function
   * @param {Data}
   *          data - Additional data sent from the client.
   * @param {String}
   *          clientId
   * @public
   */
  _onPlayerRightUp: function(data, clientId) {
    ige.server.players[clientId].playerControl.controls.right = false;
  },

  /**
   * Called when the network tells us a client has pressed the key to move its
   * player entity up.
   * 
   * @name ServerNetworkEvents.prototype._onPlayerUpDown
   * @function
   * @param {Data}
   *          data - Additional data sent from the client.
   * @param {String}
   *          clientId
   * @public
   */
  _onPlayerUpDown: function(data, clientId) {
    ige.server.players[clientId].playerControl.controls.up = true;
  },

  /**
   * Called when the network tells us a client has released the key to move its
   * player entity up.
   * 
   * @name ServerNetworkEvents.prototype._onPlayerUpUp
   * @function
   * @param {Data}
   *          data - Additional data sent from the client.
   * @param {String}
   *          clientId
   * @public
   */
  _onPlayerUpUp: function(data, clientId) {
    ige.server.players[clientId].playerControl.controls.up = false;
  },

  /**
   * Called when the network tells us a client has pressed the key to move its
   * player entity down.
   * 
   * @name ServerNetworkEvents.prototype._onPlayerDownDown
   * @function
   * @param {Data}
   *          data - Additional data sent from the client.
   * @param {String}
   *          clientId
   * @public
   */
  _onPlayerDownDown: function(data, clientId) {
    ige.server.players[clientId].playerControl.controls.down = true;
  },

  /**
   * Called when the network tells us a client has released the key to move its
   * player entity down.
   * 
   * @name ServerNetworkEvents.prototype._onPlayerDownUp
   * @function
   * @param {Data}
   *          data - Additional data sent from the client.
   * @param {String}
   *          clientId
   * @public
   */
  _onPlayerDownUp: function(data, clientId) {
    ige.server.players[clientId].playerControl.controls.down = false;
  },

  /**
   * Called when the network tells us a client has pressed the key to shoot.
   * 
   * @name ServerNetworkEvents.prototype._onPlayerShootDown
   * @function
   * @param {Data}
   *          data - Additional data sent from the client.
   * @param {String}
   *          clientId
   * @public
   */
  _onPlayerShootDown: function(data, clientId) {
    ige.server.players[clientId].playerControl.controls.shoot = true;
  },

  /**
   * Called when the network tells us a client has released the key to shoot.
   * 
   * @name ServerNetworkEvents.prototype._onPlayerShootUp
   * @function
   * @param {Data}
   *          data - Additional data sent from the client.
   * @param {String}
   *          clientId
   * @public
   */
  _onPlayerShootUp: function(data, clientId) {
    ige.server.players[clientId].playerControl.controls.shoot = false;
  }
};

if(typeof (module) !== 'undefined' && typeof (module.exports) !== 'undefined') {
  module.exports = ServerNetworkEvents;
}