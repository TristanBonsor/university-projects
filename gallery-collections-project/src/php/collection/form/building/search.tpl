<div>

	<fieldset>

		<legend>Location</legend>

		<?= $this->buildSelectList('parent_location_names_key_value', $this->data('parent_location_id')); ?>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Number</legend>


		<?php // To be populated and enabled via ajax; datalist contents based on parent location selected. ?>
		<input type="text" <?= $this->name('number'); ?> value="<?= $this->data('number'); ?>">

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Name</legend>

		<?php // To be populated and enabled via ajax; datalist contents based on parent location selected. ?>
		<input type="text" <?= $this->name('name'); ?> value="<?= $this->data('name'); ?>">

	</fieldset>

</div>

<div>

	<fieldset>

		<input type="submit" value="Search">

		<input type="reset" value="Clear" class="searchClearButton">

	</fieldset>

</div>