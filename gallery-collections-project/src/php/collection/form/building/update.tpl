<div>

	<fieldset>

		<legend>Location</legend>

		<p><?= $this->data('parent_location_name'); ?></p>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Number</legend>

		<input type="text" <?= $this->name('number'); ?> value="<?= $this->data('number'); ?>">

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Name</legend>

		<input type="text" <?= $this->name('name'); ?> value="<?= $this->data('name'); ?>">

	</fieldset>

</div>

<div>

	<fieldset>

		<input type="submit" value="Save">

		<button class="updateCancelButton">Cancel</button>

	</fieldset>

</div>