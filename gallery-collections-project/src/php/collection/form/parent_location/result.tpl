<div>

	<fieldset>

		<legend>Name</legend>

		<p><?= $this->data('name'); ?></p>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Address</legend>

		<p><?= $this->data('address'); ?></p>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>City</legend>

		<p><?= $this->data('city'); ?></p>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Province</legend>

		<p><?= $this->data('province'); ?></p>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Postal Code</legend>

		<p><?= $this->data('postal_code'); ?></p>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Phone Number</legend>

		<p><?= $this->data('phone_number'); ?></p>

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