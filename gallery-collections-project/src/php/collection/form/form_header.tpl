<form <?= $this->formId(); ?> class="formTable" <?= $this->action(); ?> method="post">

	<fieldset>

		<legend class="formTitle"><?= ucfirst($this->access_type) . ' ' . ucwords(str_replace('_', ' ', $this->record_type)); ?></legend>

		<input type="hidden" name="record_type" value="<?= $this->record_type; ?>">

		<?= $this->recordId(); ?>