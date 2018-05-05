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

		<p><?= $this->data('number'); ?></p>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Room Name</legend>

		<p><?= $this->data('name'); ?></p>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Actions</legend>

		<input class="readRecordButton" type="button" value="View">
		<input class="updateRecordButton" type="button" value="Update">
		<input class="deleteRecordButton" type="button" value="Delete">

	</fieldset>

</div>