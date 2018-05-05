<div>

	<fieldset>

		<legend>First Name</legend>

		<input type="text" <?= $this->name('first_name'); ?> value="<?= $this->data('first_name'); ?>">

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Last Name</legend>

		<input type="text" <?= $this->name('last_name'); ?> value="<?= $this->data('last_name'); ?>">

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Phone Number</legend>

		<input type="text" <?= $this->name('phone_number'); ?> value="<?= $this->data('phone_number'); ?>">

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Email</legend>

		<input type="text" <?= $this->name('email'); ?> value="<?= $this->data('email'); ?>">

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Address</legend>

		<input type="text" <?= $this->name('address'); ?> value="<?= $this->data('address'); ?>">

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>City</legend>

		<?= $this->buildTextDataListBox('city', 'cities'); ?>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Province</legend>

		<?= $this->buildTextDataListBox('province', 'provinces'); ?>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Postal Code</legend>

		<?= $this->buildTextDataListBox('postal_code', 'postal_codes'); ?>

	</fieldset>

</div>

<div>

	<fieldset>

		<input type="submit" value="Save">

		<input type="reset" value="Cancel" class="createCancelButton">

	</fieldset>

</div>