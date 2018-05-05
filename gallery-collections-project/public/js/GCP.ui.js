var GCP = (function(parent) {

	var ui = parent.ui = parent.ui || {};

	var _accessTypes = ['search', 'create', 'update', 'read'];

	var _recordTypes = ['artwork', 'artist', 'artwork_collection', 'artwork_type', 'artwork_medium', 'parent_location', 'building', 'room'];

	var _accessTabs = [{
		value: 'search',
		text: 'Search'
	}, {
		value: 'create',
		text: 'Create'
	}];

	var _recordTabs = [{
		value: 'artwork',
		text: 'Artwork'
	}, {
		value: 'artist',
		text: 'Artist'
	}, {
		value: 'artwork_collection',
		text: 'Artwork Collection'
	}, {
		value: 'artwork_type',
		text: 'Artwork Type'
	}, {
		value: 'artwork_medium',
		text: 'Artwork Medium'
	}, {
		value: 'parent_location',
		text: 'Location'
	}, {
		value: 'building',
		text: 'Building'
	}, {
		value: 'room',
		text: 'Room'
	}];

	var _isUiDisplay = true;

	/**
	 * 
	 */
	var readCookies = function() {
		GCP.util.log( { message: 'reading cookies...'});

		if($.inArray($.cookie('access_type'), _accessTypes) === -1) {
			$.cookie('access_type', 'search');
		}

		if($.inArray($.cookie('record_type'), _recordTypes) === -1) {
			$.cookie('record_type', 'artwork');
		}
	};

	/**
	 * 
	 */
	var createTab = function(config) {
		var tab = $('<li/>', {
			id: config.typeValue + 'Tab',
			'class': 'tab ' + config.type + 'Tab',
			data: {
				type: config.type,
				typeValue: config.typeValue
			}
		});

		$('<a/>', {
			href: '#',
			text: config.text
		}).appendTo(tab);

		return tab;
	};

	var createShadowBox = function(config) {
		var shadowBox = $('<div/>', {
			id: 'shadowBox'
		}).css({
			position: 'fixed',
			width: '100%',
			height: '100%',
			left: '0',
			top: '0',
			background: 'rgba(0, 0, 0, 0.8)',
			'z-index': '10'
		});

		var container = $('<div/>', {
			
		}).css({
			position: 'relative',
			top: '50%',
			transform: 'translateY(-50%)',
			width: config.width,
			height: config.height,
			overflow: 'auto',
			margin: 'auto',
			background: 'rgba(221, 221, 221, 0.6)',
			'border-radius': '1em',
			'-webkit-border-radius': '1em',
			'-moz-border-radius': '1em'
		}).append(config.content).appendTo(shadowBox);

		$('<button/>', {
			id: 'shadowBoxCloseButton',
			text: 'Close'
		}).css({
			position: 'absolute',
			left: '92.5%'
		}).prependTo(container);

		return shadowBox
	};

	/**
	 * 
	 */
	var createTabs = function(type, data) {
		var tabset = $('<ul/>', {'class': 'tabset'});
		$.each(data, function(index, value) {
			tabset.append(createTab({
				type: type,
				typeValue: value.value,
				text: value.text
			}));
		});

		return tabset;
	}

	/**
	 * 
	 */
	var createUiContainers = function() {
		$('<div/>', {
			id: 'innerUiShim'
		}).appendTo('#innerUiContainer');

		$.each(_accessTabs, function(index, value) {
			var accessIndex = index;
			var accessValue = value.value;

			$.each(_recordTabs, function(index, value) {
				$('<div/>', {
					id: value.value + '-' + accessValue + 'UiContainer',
					'class': 'uiContainer',
					hidden: true
				}).appendTo('#innerUiShim');
			});
		});
	};

	/**
	 * 
	 */
	var initUiEventHandlerDelegation = function() {
		GCP.util.log( { message: 'initializing event handler delegation...' } );

		$.fn.clearForm = function() {
			  return this.each(function() {
			    var type = this.type, tag = this.tagName.toLowerCase();
			    if (tag == 'form')
			      return $(':input',this).clearForm();
			    if (type == 'text' || type == 'password' || tag == 'textarea')
			      this.value = '';
			    else if (type == 'checkbox' || type == 'radio')
			      this.checked = false;
			    else if (tag == 'select')
			      this.selectedIndex = -1;
			  });
			};

		/**
		 * Artwork search form submission.
		 */
		$(document).on('submit', '#artwork-searchForm', function(event) {
			event.preventDefault();

			var searchQuery = {
				record_type: $('input[name="record_type"]', this).val(),
				criteria:    $(this).serialize()
			};

			$.post('search.php', searchQuery).done(function(data) {
				$('#searchResultsContainer').html(data);
				$('#toggleUiDisplayButton').trigger('click');
			});
		});

		/**
		 * Artwork create form submission.
		 */
		$(document).on('submit', '#artwork-createForm', function(event) {
			event.preventDefault();

			var createQuery = {
				record_type: $('input[name="record_type"]', this).val(),
				criteria:    $(this).serialize(),
			};

			var form = $(this);

			$.post('create.php', createQuery).done(function(data) {
				var uploads = form.find('input[name="uploaded_files[]"]');
				var uploadData = new FormData();

				$.each(uploads, function(key, value) {
					uploadData.append(key, value.files[0]);
				});

				$.ajax({
					url: 'create.php?files&artwork_id=' + data,
					type: 'POST',
					data: uploadData,
					cache: false,
					dataType: 'json',
					processData: false,
					contentType: false,
					success: function(data, textStatus, jqXHR) {
						form.clearForm();
						form.find('.fileUploadsContainer').html('');

						refreshUi();

						$('#shadowBox').remove();

						if(typeof data.error === 'undefined') {
							var shadowBox = createShadowBox({
								width: '50%',
								height: '50%',
								content: '<div id="createSuccess"><strong>Record Created</strong></div>'
							}).appendTo('#mainContent');
						}
						else {
							var shadowBox = createShadowBox({
								width: '50%',
								height: '50%',
								content: '<div id="createError"><strong>Error: ' + data.error + '</strong></div>'
							}).appendTo('#mainContent');
						}

						shadowBox.fadeOut(4000, function() { shadowBox.remove(); });
					},
					error: function(jqXHR, textStatus, errorThrown) {
						form.clearForm();
						form.find('.fileUploadsContainer').html('');

						refreshUi();

						$('#shadowBox').remove();

						var shadowBox = createShadowBox({
							width: '50%',
							height: '50%',
							content: '<div id="createError"><strong>Image Upload Failed: ' + textStatus + '</strong></div>'
						}).appendTo('#mainContent');

						shadowBox.fadeOut(4000, function() { shadowBox.remove(); });
					}
				});
			});
		});

		/**
		 * Artwork update form submission.
		 */
		$(document).on('submit', '#artwork-updateForm', function(event) {
			event.preventDefault();

			var form = $(this);

			var updateQuery = {
					record_type: $('input[name="record_type"]', form).val(),
					criteria:    $(this).serialize()
			};

			var result = $('#artwork-resultForm' + $('input[name="record_id"]', form).val());

			var uploads = form.find('input[name="uploaded_files[]"]');
			var uploadData = new FormData();

			$.each(uploads, function(key, value) {
				uploadData.append(key, value.files[0]);
			});

			$.ajax({
				url: 'update.php?files&artwork_id=' + $('input[name="record_id"]', form).val(),
				type: 'POST',
				data: uploadData,
				cache: false,
				dataType: 'json',
				processData: false,
				contentType: false,
				success: function(data, textStatus, jqXHR) {
					$.post('update.php', updateQuery).done(function(data) {

						refreshUi();

						$('#shadowBox').remove();

						result.replaceWith(data);

						var shadowBox = createShadowBox({
							width: '50%',
							height: '50%',
							content: '<div id="updateSuccess"><strong>Record Updated</strong></div>'
						}).appendTo('#mainContent');

						shadowBox.fadeOut(4000, function() { shadowBox.remove(); });
					});
				},
				error: function(jqXHR, textStatus, errorThrown) {

					refreshUi();

					$('#shadowBox').remove();

					var shadowBox = createShadowBox({
						width: '50%',
						height: '50%',
						content: '<div id="updateError"><strong>Update Error: ' + textStatus + '</strong></div>'
					}).appendTo('#mainContent');

					shadowBox.fadeOut(4000, function() { shadowBox.remove(); });
				}
			});
		});

		$(document).on('submit', '#artist-searchForm', function(event) {
			event.preventDefault();

			var searchQuery = {
				record_type: $('input[name="record_type"]', this).val(),
				criteria:    $(this).serialize()
			};

			$.post('search.php', searchQuery).done(function(data) {
				$('#searchResultsContainer').html(data);
				$('#toggleUiDisplayButton').trigger('click');
			});
		});

		$(document).on('submit', '#artist-createForm', function(event) {
			event.preventDefault();

			var createQuery = {
				record_type: $('input[name="record_type"]', this).val(),
				criteria:    $(this).serialize(),
			};

			var form = $(this);

			$.post('create.php', createQuery).done(function(data) {
				form.clearForm();

				refreshUi();

				$('#shadowBox').remove();

				var shadowBox = createShadowBox({
					width: '50%',
					height: '50%',
					content: '<div id="createSuccess"><strong>Record Created</strong></div>'
				}).appendTo('#mainContent');

				shadowBox.fadeOut(4000, function() { shadowBox.remove(); });
			});
		});
		
		$(document).on('submit', '#artist-updateForm', function(event) {
			event.preventDefault();

			var form = $(this);

			var updateQuery = {
					record_type: $('input[name="record_type"]', form).val(),
					criteria:    $(this).serialize()
			};

			var result = $('#artist-resultForm' + $('input[name="record_id"]', form).val());

			$.post('update.php', updateQuery).done(function(data) {

				refreshUi();

				$('#shadowBox').remove();

				result.replaceWith(data);

				var shadowBox = createShadowBox({
					width: '50%',
					height: '50%',
					content: '<div id="updateSuccess"><strong>Record Updated</strong></div>'
				}).appendTo('#mainContent');

				shadowBox.fadeOut(4000, function() { shadowBox.remove(); });
			});
		});
		
		$(document).on('submit', '#artwork_type-searchForm', function(event) {
			event.preventDefault();

			var searchQuery = {
				record_type: $('input[name="record_type"]', this).val(),
				criteria:    $(this).serialize()
			};

			$.post('search.php', searchQuery).done(function(data) {
				$('#searchResultsContainer').html(data);
				$('#toggleUiDisplayButton').trigger('click');
			});
		});
		
		$(document).on('submit', '#artwork_type-createForm', function(event) {
			event.preventDefault();

			var createQuery = {
				record_type: $('input[name="record_type"]', this).val(),
				criteria:    $(this).serialize(),
			};

			var form = $(this);

			$.post('create.php', createQuery).done(function(data) {
				form.clearForm();

				refreshUi();

				$('#shadowBox').remove();

				var shadowBox = createShadowBox({
					width: '50%',
					height: '50%',
					content: '<div id="createSuccess"><strong>Record Created</strong></div>'
				}).appendTo('#mainContent');

				shadowBox.fadeOut(4000, function() { shadowBox.remove(); });
			});
		});
		
		$(document).on('submit', '#artwork_type-updateForm', function(event) {
			event.preventDefault();

			var form = $(this);

			var updateQuery = {
					record_type: $('input[name="record_type"]', form).val(),
					criteria:    $(this).serialize()
			};

			var result = $('#artwork_type-resultForm' + $('input[name="record_id"]', form).val());

			$.post('update.php', updateQuery).done(function(data) {

				refreshUi();

				$('#shadowBox').remove();

				result.replaceWith(data);

				var shadowBox = createShadowBox({
					width: '50%',
					height: '50%',
					content: '<div id="updateSuccess"><strong>Record Updated</strong></div>'
				}).appendTo('#mainContent');

				shadowBox.fadeOut(4000, function() { shadowBox.remove(); });
			});
		});

		
		$(document).on('submit', '#artwork_medium-searchForm', function(event) {
			event.preventDefault();

			var searchQuery = {
				record_type: $('input[name="record_type"]', this).val(),
				criteria:    $(this).serialize()
			};

			$.post('search.php', searchQuery).done(function(data) {
				$('#searchResultsContainer').html(data);
				$('#toggleUiDisplayButton').trigger('click');
			});
		});
		
		$(document).on('submit', '#artwork_medium-createForm', function(event) {
			event.preventDefault();

			var createQuery = {
				record_type: $('input[name="record_type"]', this).val(),
				criteria:    $(this).serialize(),
			};

			var form = $(this);

			$.post('create.php', createQuery).done(function(data) {
				form.clearForm();

				refreshUi();

				$('#shadowBox').remove();

				var shadowBox = createShadowBox({
					width: '50%',
					height: '50%',
					content: '<div id="createSuccess"><strong>Record Created</strong></div>'
				}).appendTo('#mainContent');

				shadowBox.fadeOut(4000, function() { shadowBox.remove(); });
			});
		});

		$(document).on('submit', '#artwork_medium-updateForm', function(event) {
			event.preventDefault();

			var form = $(this);

			var updateQuery = {
					record_type: $('input[name="record_type"]', form).val(),
					criteria:    $(this).serialize()
			};

			var result = $('#artwork_medium-resultForm' + $('input[name="record_id"]', form).val());

			$.post('update.php', updateQuery).done(function(data) {

				refreshUi();

				$('#shadowBox').remove();

				result.replaceWith(data);

				var shadowBox = createShadowBox({
					width: '50%',
					height: '50%',
					content: '<div id="updateSuccess"><strong>Record Updated</strong></div>'
				}).appendTo('#mainContent');

				shadowBox.fadeOut(4000, function() { shadowBox.remove(); });
			});
		});

		
		$(document).on('submit', '#artwork_collection-searchForm', function(event) {
			event.preventDefault();

			var searchQuery = {
				record_type: $('input[name="record_type"]', this).val(),
				criteria:    $(this).serialize()
			};

			$.post('search.php', searchQuery).done(function(data) {
				$('#searchResultsContainer').html(data);
				$('#toggleUiDisplayButton').trigger('click');
			});
		});
		
		$(document).on('submit', '#artwork_collection-createForm', function(event) {
			event.preventDefault();

			var createQuery = {
				record_type: $('input[name="record_type"]', this).val(),
				criteria:    $(this).serialize(),
			};

			var form = $(this);

			$.post('create.php', createQuery).done(function(data) {
				form.clearForm();

				refreshUi();

				$('#shadowBox').remove();

				var shadowBox = createShadowBox({
					width: '50%',
					height: '50%',
					content: '<div id="createSuccess"><strong>Record Created</strong></div>'
				}).appendTo('#mainContent');

				shadowBox.fadeOut(4000, function() { shadowBox.remove(); });
			});
		});
		
		$(document).on('submit', '#artwork_collection-updateForm', function(event) {
			event.preventDefault();

			var form = $(this);

			var updateQuery = {
					record_type: $('input[name="record_type"]', form).val(),
					criteria:    $(this).serialize()
			};

			var result = $('#artwork_collection-resultForm' + $('input[name="record_id"]', form).val());

			$.post('update.php', updateQuery).done(function(data) {

				refreshUi();

				$('#shadowBox').remove();

				result.replaceWith(data);

				var shadowBox = createShadowBox({
					width: '50%',
					height: '50%',
					content: '<div id="updateSuccess"><strong>Record Updated</strong></div>'
				}).appendTo('#mainContent');

				shadowBox.fadeOut(4000, function() { shadowBox.remove(); });
			});
		});

		$(document).on('submit', '#parent_location-searchForm', function(event) {
			event.preventDefault();

			var searchQuery = {
				record_type: $('input[name="record_type"]', this).val(),
				criteria:    $(this).serialize()
			};

			$.post('search.php', searchQuery).done(function(data) {
				$('#searchResultsContainer').html(data);
				$('#toggleUiDisplayButton').trigger('click');
			});
		});
		
		$(document).on('submit', '#parent_location-createForm', function(event) {
			event.preventDefault();

			var createQuery = {
				record_type: $('input[name="record_type"]', this).val(),
				criteria:    $(this).serialize(),
			};

			var form = $(this);

			$.post('create.php', createQuery).done(function(data) {
				form.clearForm();

				refreshUi();

				$('#shadowBox').remove();

				var shadowBox = createShadowBox({
					width: '50%',
					height: '50%',
					content: '<div id="createSuccess"><strong>Record Created</strong></div>'
				}).appendTo('#mainContent');

				shadowBox.fadeOut(4000, function() { shadowBox.remove(); });
			});
		});
		
		$(document).on('submit', '#parent_location-updateForm', function(event) {
			event.preventDefault();

			var form = $(this);

			var updateQuery = {
					record_type: $('input[name="record_type"]', form).val(),
					criteria:    $(this).serialize()
			};

			var result = $('#parent_location-resultForm' + $('input[name="record_id"]', form).val());

			$.post('update.php', updateQuery).done(function(data) {

				refreshUi();

				$('#shadowBox').remove();

				result.replaceWith(data);

				var shadowBox = createShadowBox({
					width: '50%',
					height: '50%',
					content: '<div id="updateSuccess"><strong>Record Updated</strong></div>'
				}).appendTo('#mainContent');

				shadowBox.fadeOut(4000, function() { shadowBox.remove(); });
			});
		});
		
		$(document).on('submit', '#building-searchForm', function(event) {
			event.preventDefault();

			var searchQuery = {
				record_type: $('input[name="record_type"]', this).val(),
				criteria:    $(this).serialize()
			};

			$.post('search.php', searchQuery).done(function(data) {
				$('#searchResultsContainer').html(data);
				$('#toggleUiDisplayButton').trigger('click');
			});
		});
		
		$(document).on('submit', '#building-createForm', function(event) {
			event.preventDefault();

			var createQuery = {
				record_type: $('input[name="record_type"]', this).val(),
				criteria:    $(this).serialize(),
			};

			var form = $(this);

			$.post('create.php', createQuery).done(function(data) {
				form.clearForm();

				refreshUi();

				$('#shadowBox').remove();

				var shadowBox = createShadowBox({
					width: '50%',
					height: '50%',
					content: '<div id="createSuccess"><strong>Record Created</strong></div>'
				}).appendTo('#mainContent');

				shadowBox.fadeOut(4000, function() { shadowBox.remove(); });
			});
		});
		
		$(document).on('submit', '#building-updateForm', function(event) {
			event.preventDefault();

			var form = $(this);

			var updateQuery = {
					record_type: $('input[name="record_type"]', form).val(),
					criteria:    $(this).serialize()
			};

			var result = $('#building-resultForm' + $('input[name="record_id"]', form).val());

			$.post('update.php', updateQuery).done(function(data) {

				refreshUi();

				$('#shadowBox').remove();

				result.replaceWith(data);

				var shadowBox = createShadowBox({
					width: '50%',
					height: '50%',
					content: '<div id="updateSuccess"><strong>Record Updated</strong></div>'
				}).appendTo('#mainContent');

				shadowBox.fadeOut(4000, function() { shadowBox.remove(); });
			});
		});
		
		$(document).on('submit', '#room-searchForm', function(event) {
			event.preventDefault();

			var searchQuery = {
				record_type: $('input[name="record_type"]', this).val(),
				criteria:    $(this).serialize()
			};

			$.post('search.php', searchQuery).done(function(data) {
				$('#searchResultsContainer').html(data);
				$('#toggleUiDisplayButton').trigger('click');
			});
		});
		
		$(document).on('submit', '#room-createForm', function(event) {
			event.preventDefault();

			var createQuery = {
				record_type: $('input[name="record_type"]', this).val(),
				criteria:    $(this).serialize(),
			};

			var form = $(this);

			$.post('create.php', createQuery).done(function(data) {
				form.clearForm();

				refreshUi();

				$('#shadowBox').remove();

				var shadowBox = createShadowBox({
					width: '50%',
					height: '50%',
					content: '<div id="createSuccess"><strong>Record Created</strong></div>'
				}).appendTo('#mainContent');

				shadowBox.fadeOut(4000, function() { shadowBox.remove(); });
			});
		});
		
		$(document).on('submit', '#room-updateForm', function(event) {
			event.preventDefault();

			var form = $(this);

			var updateQuery = {
					record_type: $('input[name="record_type"]', form).val(),
					criteria:    $(this).serialize()
			};

			var result = $('#room-resultForm' + $('input[name="record_id"]', form).val());

			$.post('update.php', updateQuery).done(function(data) {

				refreshUi();

				$('#shadowBox').remove();

				result.replaceWith(data);

				var shadowBox = createShadowBox({
					width: '50%',
					height: '50%',
					content: '<div id="updateSuccess"><strong>Record Updated</strong></div>'
				}).appendTo('#mainContent');

				shadowBox.fadeOut(4000, function() { shadowBox.remove(); });
			});
		});
		
		$(document).on('click', '#backupButton', function(event) {
			event.preventDefault();
			
			if(confirm('Download all data?')) {
				$.post('backup.php', {}).done(function(data) {
					location.href = data;
				});
			}
		});
		
		/**
		 * 
		 */
		$(document).on('click', '#toggleUiDisplayButton', function(event) {
			$('#innerUiShim').slideToggle();

			_isUiDisplay = !_isUiDisplay

			if(_isUiDisplay) {
				$(this).html('Hide');
			}
			else {
				$(this).html('Show');
			}
		});

		/**
		 * 
		 */
		$(document).on('click', '#shadowBoxCloseButton, .updateCancelButton', function(event) {
			event.preventDefault();
			$('#shadowBox').remove();
		});

		/**
		 * 
		 */
		$(document).on('click', '.createCancelButton', function(event) {
			event.preventDefault();

			var form =$(this).closest('form');
			form.find('input[name="number"]').prop('disabled', 'disabled');
			form.find('input[name="name"]').prop('disabled', 'disabled');
			form.find('select[name="building_number_names_key_value"]').prop('disabled', 'disabled');
			form.find('input[type="submit"]').prop('disabled', 'disabled');
			form.clearForm();
			$('#fileUploadsContainer').html('');
			$('#shadowBox').remove();
		});

		/**
		 * 
		 */
		$(document).on('click', '.searchClearButton', function(event) {
			event.preventDefault();

			var form = $(this).closest('form');
			form.clearForm();
			if(form.attr('id') !== 'room-searchForm') {
				form.find('select[name="building_number_names_key_value"]').prop('disabled', 'disabled');
				form.find('select[name="room_number_names_key_value"]').prop('disabled', 'disabled');
			}

		});
		
		/**
		 * 
		 */
		$(document).on('click', '.tab', function(event) {
			$('.selectedTab.' + $(this).data('type') + 'Tab').removeClass('selectedTab');
			$('.selectedUi').removeClass('selectedUi').prop('hidden', 'hidden');
			$.cookie($(this).data('type'), $(this).data('typeValue'));
			requestUi();
			
			if(!_isUiDisplay) {
				$('#innerUiShim').slideToggle();
				$('#toggleUiDisplayButton').html('Hide');
				_isUiDisplay = true;
			}
		});

		/**
		 * 
		 */
		$(document).on('change', 'select[name="parent_location_names_key_value"]', function(event) {
			var form = $(this).closest('form');
			
			if($(this).val() === '') {
				if(form.attr('id') !== 'room-searchForm') {
					form.find('input[name="number"]').prop('disabled', 'disabled');
					form.find('input[name="name"]').prop('disabled', 'disabled');
					form.find('input[type="submit"]').prop('disabled', 'disabled');
				}
			}
			else {
				if(form.attr('id') !== 'room-createForm') {
					form.find('input[name="number"]').prop('disabled', '');
					form.find('input[name="name"]').prop('disabled', '');
					form.find('input[type="submit"]').prop('disabled', '');
				}
			}

			$.post('request_location_change.php', { input_name: 'building_number_names_key_value', parent_id: $(this).val(), form_id: form.attr('id') }).done(function(data) {
				form.find('select[name="building_number_names_key_value"]').replaceWith(data).prop('disabled', '');
				form.find('select[name="room_number_names_key_value"]').prop('disabled', 'disabled').empty();
			});
		});

		/**
		 * 
		 */
		$(document).on('change', 'select[name="building_number_names_key_value"]', function(event) {
			var form = $(this).closest('form');
			
			if(form.attr('id') === 'room-createForm') {
				form.find('input[name="number"]').prop('disabled', '');
				form.find('input[name="name"]').prop('disabled', '');
				form.find('input[type="submit"]').prop('disabled', '');
			}
			
			$.post('request_location_change.php', { input_name: 'room_number_names_key_value', parent_id: $(this).val(), form_id: form.attr('id') }).done(function(data) {
				form.find('select[name="room_number_names_key_value"]').replaceWith(data);
			});
		});

		/**
		 * 
		 */
		$(document).on('click', '.readRecordButton', function(event) {
			event.preventDefault();

			var form = $(this).parents('form:first');
			var record_type = form.find('input[name="record_type"]').val();
			var record_id = form.find('input[name="record_id"]').val();

			GCP.util.log({ message: 'Request Form: ' + record_type + '-read' });

			$.post('request_form.php', { access_type: 'read', record_type: record_type, record_id: record_id }).done(function(data) {
				$('#mainContent').append(createShadowBox({
					width: '75%',
					height: '75%',
					content: data
				}));
			});
		});

		/**
		 * 
		 */
		$(document).on('click', '.updateRecordButton', function(event) {
			event.preventDefault();
			
			var form = $(this).parents('form:first');
			var record_type = form.find('input[name="record_type"]').val();
			var record_id = form.find('input[name="record_id"]').val();

			GCP.util.log({ message: 'Request Form: ' + record_type + '-update' });

			$.post('request_form.php', { access_type: 'update', record_type: record_type, record_id: record_id }).done(function(data) {
				$('#mainContent').append(createShadowBox({
					width: '75%',
					height: '75%',
					content: data
				}));
			});
		});

		/**
		 * 
		 */
		$(document).on('click', '.deleteRecordButton', function(event) {
			event.preventDefault();
			
			var form = $(this).parents('form:first');
			var record_type = form.find('input[name="record_type"]').val();
			var record_id = form.find('input[name="record_id"]').val();
			
			if(confirm('Are you sure you want to delete this record?')) {
				$.post('delete.php', {record_type: record_type, record_id: record_id}).done(function(data) {
					form.remove();

					var num = Number($('#resultCount').text());
					$('#resultCount').text(--num);

					refreshUi();

					var shadowBox = createShadowBox({
						width: '50%',
						height: '50%',
						content: data
					}).appendTo('#mainContent');

					shadowBox.fadeOut(4000, function() { shadowBox.remove(); });
				});
			}
			
		});
	};

	/**
	 * 
	 */
	$(document).on('change', 'input[name="uploaded_files[]"]', function(event) {
		var file = $(this)[0].files[0];

		if(!file.type.match(/image.*/)) return;

		var preview = $(this).parents('.fileUploadContainer:first').find('.filePreview');

		if(window.File && window.FileList && window.FileReader) {
			var img = document.createElement("img");
			img.classList.add('thumbnail');
			img.file = file;
			preview.append(img);

			var reader = new FileReader();
			reader.onload = (function(aImg) {
				return function(event) {
					aImg.src = event.target.result;
				};
			})(img);
			reader.readAsDataURL(file);
		}
		else {
			$('<p/>', {
				text: 'Preview Unavalaible'
			}).appendTo(preview);
		}
	});
	
	/**
	 * 
	 */
	$(document).on('click', '.addFileButton', function(event) {
		event.preventDefault();
		$(this).parents('form:first').find('.fileUploadsContainer').append($.parseHTML('<div class="fileUploadContainer"><fieldset><legend>File</legend><input type="file" name="uploaded_files[]"><div class="filePreview"></div><button class="removeFileButton">Remove File</button></fieldset></div>'));
	});

	/**
	 * 
	 */
	$(document).on('click', '.removeFileButton', function(event) {
		$(this).parents('.fileUploadContainer:first').remove();
	});

	/**
	 * 
	 */
	var requestUi = function() {
		GCP.util.log({ message: 'requesting user interface...' });

		var access_type = $.cookie('access_type');
		var record_type = $.cookie('record_type');

		var idPrefix = record_type + '-' + access_type;

		$('#' + access_type + 'Tab').addClass('selectedTab');
		$('#' + record_type + 'Tab').addClass('selectedTab');

		var newUi = $('#' + idPrefix + 'UiContainer').addClass('selectedUi');

		if(!$('#' + idPrefix + 'Form').length) {
			GCP.util.log({ message: 'Request Form: ' + idPrefix });

			$.post('request_form.php', { access_type: access_type, record_type: record_type }).done(function(data) {
				newUi.append(data);
			});
		}

		newUi.prop('hidden', '');
	};

	var refreshUi = function() {
		GCP.util.log({ message: 'refreshing user interface...' });

		$.each(_accessTabs, function(index, value) {
			var accessIndex = index;
			var accessValue = value.value;

			$.each(_recordTabs, function(index, value) {
				var form_id = '#' + value.value + '-' + accessValue + 'Form';
				var form = $(form_id);

				if(form.length) {
					var values = { access_type: accessValue };

					$.each(form.serializeArray(), function(i, field) {
						values[field.name] = field.value;
					});

					values['types_checked[]'] = [];
					values['mediums_checked[]'] = [];

					if(accessValue === 'search' && value.value === 'artwork') {
						$('input[name="types_checked[]"]:checked', form).each(function() {
							values['types_checked[]'].push($(this).val());
						});
						
						$('input[name="mediums_checked[]"]:checked', form).each(function() {
							values['mediums_checked[]'].push($(this).val());
						});
					}

					var uploads = false;

					if(accessValue === 'create' && value.value === 'artwork') {
						// Save file input elements, append to form after replace data :)
						// ie. Detach, store, reinsert on new form load.
						uploads = $('#fileUploadsContainer').detach();
					}

					GCP.util.log({ message: 'Refresh Form: ' + value.value + '-' + accessValue })

					$.post('request_form.php', values).done(function(data) {
						form.replaceWith(data);

						if(accessValue === 'create' && value.value === 'artwork') {
							// Reattach file inputs to new form.
							$('#fileUploadsContainer').replaceWith(uploads);
						}
					});
				}
			});
		});
	}

	ui.load = function() {
		GCP.util.log( { message: 'loading user interface...' } );

		initUiEventHandlerDelegation();

		$('<button/>', {
			id: 'toggleUiDisplayButton',
			text: 'Hide'
		}).prependTo('#outerUiContainer');

		$('#accessTabsetContainer').append(createTabs('access_type', _accessTabs));
		$('#recordTabsetContainer').append(createTabs('record_type', _recordTabs));

		createUiContainers();

		readCookies();

		requestUi();
	};

	return parent;

}(GCP || {}));