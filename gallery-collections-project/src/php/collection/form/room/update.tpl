<div>

	<fieldset>

		<legend>Location</legend>

		<p><?= $this->data('parent_location_name'); ?></p>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Building Number</legend>

		<p><?= $this->data('building_number'); ?></p>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Building Name</legend>

		<p><?= $this->data('building_name'); ?></p>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Room Number</legend>

		<input type="text" <?= $this->name('number'); ?> value="<?= $this->data('number'); ?>">

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Room Name</legend>

		<input type="text" <?= $this->name('name'); ?> value="<?= $this->data('name'); ?>">

	</fieldset>

</div>

<div>

	<fieldset>

		<input type="submit" value="Save">

		<button class="updateCancelButton">Cancel</button>

	</fieldset>

</div>