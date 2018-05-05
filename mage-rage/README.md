#Mage Rage: The Four Elements

Mage Rage: The Four Elements is a 4 player action/arcade game involving combat between air, earth, fire, and water wizards (mages). Utilizing the Isogenic Game Engine (IGE), Mage Rage is built on top of Nodejs and JavaScript and provides a fast paced browser game experience.

##Example Installation: Debian 'Wheezy'

Initially, this project will use a Debian 'Wheezy' system as a reference for server side environments. Note that these steps will change after November 2014 as Debian 'Jessie' will become the new stable Debian release.

To host a game of Mage Rage the following components are required:

* A web server that can serve the HTML and JavaScript. See https://wiki.debian.org/LaMp.

* Nodejs and npm. Unfortunately both of these packages are not available from the default repositories.

  * Get Nodejs from Debian Backports.
     
    * Open `/etc/apt/sources.list` with your editor of choice (and proper user permissions).
    
    * Add the following line to your sources.list file: `deb http://ftp.ca.debian.org/debian/ wheezy-backports main contrib non-free`.
      
    * Save the file and exit the editor.
    
    * Type the following (with proper user permissions): `aptitude update; aptitude upgrade`.
    
    * Type the following (with proper user permissions): `aptitude -t wheezy-backports install nodejs nodejs-legacy`.
    
  * Get npm.

    * `curl https://www.npmjs.org/install.sh | sudo sh`


* Get the latest IGE prototype build.

  * Get ige_prototype from https://github.com/coolbloke1324/ige_prototype.
  
  * Put ige_prototype in a public folder accessible to the web server. Type `ln -s /path/to/ige_prototype /path/to/ige` (same directory). This creates a softlink as IGE expects the project folder to be 'ige' and not 'ige_prototype'.
  
  * Now type `cd /path/to/ige/server; npm install`. This will enable multiplayer functionality for IGE.

* Get the latest Mage Rage build.

  * Get Mage Rage from https://github.com/kidthales/Mage-Rage-The-Four-Elements.

  * Put Mage Rage in the same folder that the ige_prototype folder is in.
  
  * Open `/path/to/Mage-Rage-The-Four-Elements/client.js` with your editor of choice. On line 158 you will see: `ige.network.start('http://localhost:2000', function() {`. Change 'http://localhost:2000' to a url:port string suitable for your web server and where Mage Rage is publicly located on it.

* Start the game server.

  * Type `cd /path/to/ige/server`.

  * Type `nohup node ige.js -g /path/to/Mage-Rage-The-Four-Elements &`.

  * The game server should now be running and listening for incomming connections on the port you have specified.

Users can now join the game by navigating to `http://your.domain.name/path/to/Mage-Rage-The-Four-Elements`.
