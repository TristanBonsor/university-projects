<div>

	<fieldset>

		<legend>Name</legend>

		<input type="text" <?= $this->name('name'); ?> value="<?=$this->data('name'); ?>">

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Address</legend>

		<input type="text" <?= $this->name('address'); ?> value="<?=$this->data('address'); ?>">

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>City</legend>

		<input type="text" <?= $this->name('city'); ?> value="<?=$this->data('city'); ?>">

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Province</legend>

		<input type="text" <?= $this->name('province'); ?> value="<?=$this->data('province'); ?>">

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Postal Code</legend>

		<input type="text" <?= $this->name('postal_code'); ?> value="<?=$this->data('postal_code'); ?>">

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Phone Number</legend>

		<input type="text" <?= $this->name('phone_number'); ?> value="<?=$this->data('phone_number'); ?>">

	</fieldset>

</div>

<div>

	<fieldset>

		<input type="submit" value="Save">

		<input type="reset" value="Cancel" class="createCancelButton">

	</fieldset>

</div>