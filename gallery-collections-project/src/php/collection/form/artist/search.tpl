<div>

	<fieldset>

		<legend>First Name</legend>

		<?= $this->buildTextDataListBox('first_name', 'first_names'); ?>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Last Name</legend>

		<?= $this->buildTextDataListBox('last_name', 'last_names'); ?>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Phone Number</legend>

		<?= $this->buildTextDataListBox('phone_number', 'phone_numbers'); ?>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Email</legend>

		<?= $this->buildTextDataListBox('email', 'emails'); ?>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Address</legend>

		<?= $this->buildTextDataListBox('address', 'addresses'); ?>

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

		<input type="submit" value="Search">

		<input type="reset" value="Clear" class="searchClearButton">

	</fieldset>

</div>