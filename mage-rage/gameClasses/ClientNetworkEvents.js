/**
 * @file ClientNetworkEvents definition used and implemented by the client.
 * @author Isogenic Game Engine - modified by kidthales.
 * @copyright Team Awesome 2014
 */

/**
 * Indexes all client network event handlers.
 * 
 * @namespace
 */
var ClientNetworkEvents = {
  /**
   * Is called when a network packet with the "playerEntity" command is received
   * by the client from the server. This is the server telling us which entity
   * is our player entity so that we can track it with the main camera!
   * 
   * @name ClientNetworkEvents.prototype._onPlayerEntity
   * @function
   * @param {Data}
   *          data - The data object that contains any data sent from the
   *          server.
   * @public
   */
  _onPlayerEntity: function(data) {
    if(ige.$(data)) {
      // Track our player with the camera.
      ige.client.vp1.camera.trackTranslate(ige.$(data), 50);
      // ige.client.playerEntityId = data;
    }
    else {
      // The client has not yet received the entity via the network stream so
      // lets ask the stream to tell us when it creates a new entity and then
      // check if that entity is the one we should be tracking!
      var self = this;
      self._eventListener = ige.network.stream
              .on('entityCreated',
                  function(entity) {
                    if(entity.id() === data) {
                      // Tell the camera to track out player entity.
                      ige.client.vp1.camera.trackTranslate(ige.$(data), 50);

                      // Turn off the listener for this event now that we have
                      // found and
                      // started tracking our player entity.
                      ige.network.stream
                              .off('entityCreated',
                                   self._eventListener,
                                   function(result) {
                                     if(!result) {
                                       this
                                               .log('Could not disable event listener!',
                                                    'warning');
                                     }
                                   });
                    }
                  });
    }

    // Save the player entity id so we can mount its floating UI widgets later.
    // This whole setup is not ideal.
    ige.client.playerEntityId = data;
  },

  /**
   * Is called when a network packet with the "postScores" command is received
   * by the client from the server.
   * 
   * @name ClientNetworkEvents.prototype._onPostScores
   * @function
   * @param {Data}
   *          scores - The data object that contains current score sent from the
   *          server.
   * @public
   */
  _onPostScores: function(scores) {
    var list = [];

    if(scores['air']) {
      list.push(scores['air']);
    }

    if(scores['earth']) {
      list.push(scores['earth']);
    }

    if(scores['fire']) {
      list.push(scores['fire']);
    }
    if(scores['water']) {
      list.push(scores['water']);
    }

    list.sort(function(a, b) {
      var x = parseInt(a.kills);
      var y = parseInt(b.kills);
      
      if(x > y) { return -1; }
      if(x < y) { return 1; }
      
      var w = parseInt(a.deaths);
      var z = parseInt(b.deaths);
      
      if(w < y) { return -1; }
      if(w > z) { return 1; }
      
      return 0;
    });

    var listStr = '';
    for(var i = 0; i < list.length; ++i) {
      listStr += (list[i].kills + ' ' + list[i].deaths + ' ' + list[i].name + '\n');
    }

    ige.client.scoreBoard.text(listStr);
  }
};

if(typeof (module) !== 'undefined' && typeof (module.exports) !== 'undefined') {
  module.exports = ClientNetworkEvents;
}