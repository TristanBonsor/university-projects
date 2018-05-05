var GCP = (function(parent) {

	var manifest = parent.manifest = parent || {};

	manifest.javascript = [
	    'js/jquery/jquery.cookie.js',
	    'js/GCP.ui.js'
	];

	manifest.css = [
	    'css/GCP.css'
	];

	return parent;

}(GCP || {}));