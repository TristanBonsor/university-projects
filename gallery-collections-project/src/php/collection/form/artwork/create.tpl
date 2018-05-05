<div>

	<fieldset>

		<legend>Title</legend>

		<input type="text" <?= $this->name('title'); ?> value="<?= $this->data('title'); ?>">

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Artist</legend>

		<?= $this->buildSelectList('artist_full_names_key_value', $this->data('artist_full_name_key_value')); ?>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Collection</legend>

		<?= $this->buildSelectList('collections', $this->data('collection')); ?>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Artwork Type</legend>

		<?= $this->buildSelectList('types', $this->data('type')); ?>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Artwork Medium</legend>

		<?= $this->buildSelectList('mediums', $this->data('medium')); ?>

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

		<legend>Description</legend>

		<textarea <?= $this->name('description'); ?> <?= $this->form(); ?> rows="6" cols="50"><?= $this->data('description'); ?></textarea>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Images</legend>

		<div class="fileUploadsContainer"></div>

		<input type="button" name="add_file_button" value="Add File" class="addFileButton">

	</fieldset>

</div>

<div>

	<fieldset>

		<input type="submit" value="Save">

		<input type="reset" value="Cancel" class="createCancelButton">

	</fieldset>

</div>