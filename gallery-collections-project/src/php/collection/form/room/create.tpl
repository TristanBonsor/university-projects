<div>

	<fieldset>

		<legend>Location</legend>

		<?= $this->buildSelectList('parent_location_names_key_value', $this->data('parent_location_id')); ?>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Building</legend>

		<?= $this->buildSelectList('building_number_names_key_value', $this->data('building_id')); ?>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Number</legend>

		<?php if(!$this->data('parent_location_id')) { ?>
		<?php // To be populated and enabled via ajax; datalist contents based on parent location selected. ?>
		<input type="text" <?= $this->name('number'); ?> value="<?= $this->data('number'); ?>" disabled>
		<?php } else { ?>
		<input type="text" <?= $this->name('number'); ?> value="<?= $this->data('number'); ?>">
		<?php } ?>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Name</legend>

		<?php if(!$this->data('parent_location_id')) { ?>
		<?php // To be populated and enabled via ajax; datalist contents based on parent location selected. ?>
		<input type="text" <?= $this->name('name'); ?> value="<?= $this->data('name'); ?>" disabled>
		<?php } else { ?>
		<input type="text" <?= $this->name('name'); ?> value="<?= $this->data('name'); ?>">
		<?php } ?>

	</fieldset>

</div>

<div>

	<fieldset>

		<input type="submit" value="Save" disabled>

		<input type="reset" value="Cancel" class="createCancelButton">

	</fieldset>

</div>