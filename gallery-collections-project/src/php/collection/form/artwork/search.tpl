<div>

	<fieldset>

		<legend>Title</legend>

		<?= $this->buildTextDataListBox('title', 'titles'); ?>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Artist</legend>

		<div>
		
			<fieldset>
			
				<legend>First Name</legend>
			
				<?= $this->buildTextDataListBox('artist_first_name', 'artist_first_names'); ?>
			
			</fieldset>
		
		</div>
		
		<div>
		
			<fieldset>
			
				<legend>Last Name</legend>
			
				<?= $this->buildTextDataListBox('artist_last_name', 'artist_last_names'); ?>
			
			</fieldset>
			
		</div>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Collection</legend>

		<?= $this->buildCheckBoxes('collections', $this->data('collections_checked')); ?>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Artwork Type</legend>

		<?= $this->buildCheckBoxes('types', $this->data('types_checked')); ?>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Artwork Medium</legend>

		<?= $this->buildCheckBoxes('mediums', $this->data('mediums_checked')); ?>

	</fieldset>

</div>

<div>

	<fieldset>

		<div>

			<fieldset>

				<legend>Location</legend>

				<?= $this->buildSelectList('parent_location_names_key_value', $this->data('parent_location_id')); ?>

			</fieldset>

		</div>

		<div>

			<fieldset>

				<legend>Building</legend>

				<?php if(!$this->data('building_id')) { ?>
				<?php // To be populated and enabled via ajax; select contents based on parent location selected. ?>
				<select <?= $this->name('building_number_names_key_value'); ?> <?= $this->form(); ?> disabled></select>
				<?php } else { ?>
				<?= $this->buildSelectList('building_number_names_key_value', $this->data('building_id')); ?>
				<?php } ?>
	
			</fieldset>

		</div>

		<div>

			<fieldset>

				<legend>Room</legend>

				<?php if(!$this->data('room_id')) { ?>
				<?php // To be populated and enabled via ajax; select contents based on building selected. ?>
				<select <?= $this->name('room_number_names_key_value'); ?> <?= $this->form(); ?> disabled></select>
				<?php } else { ?>
				<?= $this->buildSelectList('room_number_names_key_value', $this->data('room_id')); ?>
				<?php } ?>

			</fieldset>

		</div>

	</fieldset>

</div>

<div>

	<fieldset>

		<input type="submit" value="Search">

		<input type="reset" value="Clear" class="searchClearButton">

	</fieldset>

</div>