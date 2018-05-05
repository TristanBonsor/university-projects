var GCP = (function(parent) {

	var GCP = parent = parent || {};

	GCP.load = function() {

		var loader;

		this.util.log({ message: 'Gallery Collections Project (GCP) - Copyright (C) 2014 Agog Pixel' });

		// Manifest loader.
		loader = new this.util.Loader({
			name: 'Core Loader',
			type: 'script',
			manifest: ['js/GCP.manifest.js', 'https://code.jquery.com/jquery-2.1.1.js'],
			onItemsLoaded: function() {

				// Javascript loader.
				var loader = new GCP.util.Loader({
					name: 'Javascript Loader',
					type: 'script',
					manifest: GCP.manifest.javascript,
					onItemsLoaded: function() {
						GCP.ui.load();
						// Stylesheet loader.
						/*var loader = new GCP.util.Loader({
							name: 'Stylesheet Loader',
							type: 'stylesheet',
							manifest: GCP.manifest.css,
							onItemsLoaded: function() {
								GCP.ui.load();
							}
						})*/;

						//loader.load(); // Load stylesheets.
					}
				});

				loader.load(); // Load javascript files.
			}
		});

		loader.load(); // Load manifest.
	};

	return parent;

}(GCP || {}));

var GCP = (function(parent) {

	var util = parent.util = parent.util || {};

	/**
	 * 
	 */
	util.log = function(data) { console.log('GCP: ' + data.message); };

	/**
	 * 
	 */
	util.Loader = function(config) {

		var self = this;

		this._name = config.name || 'Loader';
		this._type = config.type || '';

		this._manifest = config.manifest || [];

		this._onItemsLoaded = config.onItemsLoaded || function() {
			GCP.util.log({ message: 'Load completed!' });
		};

		this._loadCount = 0;

		if(this._type === 'script') {

			/**
			 * 
			 */
			this._loadItem = function(url) {
				var head = document.getElementsByTagName('head')[0];
				var script = document.createElement('script');

				script.type = 'text/javascript';
				script.src = url;

				// Bind the load event to the callback function.
				// There are several events for cross browser compatibility.
				script.onreadystatechange = this._onItemLoaded;
				script.onload = this._onItemLoaded;

				// Fire the loading.
				head.appendChild(script);
			};
		}
		else if(this._type === 'stylesheet') {

			/**
			 * 
			 */
			this._loadItem = function(url) {
				var head = document.getElementsByTagName('head')[0];
				var link = document.createElement('link');
				
				link.type = 'text/css';
				link.href = url;
				link.rel = 'stylesheet';

				link.onreadystatechange = this._onItemLoaded;
				link.onload = this._onItemLoaded;
				
				head.appendChild(link);
			}
		}

		this._onItemLoaded = function(event) {

			GCP.util.log({ message: self._name + ': ' + self._type + ' ' + (event.target.src || event.target.href) + ' successfully loaded!' });

			if(++self._loadCount >= self._manifest.length) {
				GCP.util.log({ message: self._name + ': all items successfully loaded!' });
				self._onItemsLoaded();
			}
		};
	};

	util.Loader.prototype = {

		load: function() {

			var i;
			for(i = 0; i < this._manifest.length; ++i) {
				this._loadItem(this._manifest[i]);
			}
		}
	};

	return parent;

}(GCP || {}));
