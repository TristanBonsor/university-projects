<div>

	<fieldset>

		<legend>Artwork Collection</legend>

		<input type="text" <?= $this->name('collection'); ?> value="<?=$this->data('collection'); ?>">

	</fieldset>

</div>

<div>

	<fieldset>

		<input type="submit" value="Save">

		<input type="reset" value="Cancel" class="createCancelButton">

	</fieldset>

</div>