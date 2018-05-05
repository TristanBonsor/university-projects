<div>

	<fieldset>

		<legend>Artwork Type</legend>

		<input type="text" <?= $this->name('type'); ?> value="<?= $this->data('type'); ?>">

	</fieldset>

</div>

<div>

	<fieldset>

		<input type="submit" value="Save">

		<input type="reset" value="Cancel" class="createCancelButton">

	</fieldset>

</div>