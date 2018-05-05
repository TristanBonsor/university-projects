<div>

	<fieldset>

		<legend>Title</legend>

		<p><?= $this->data('title'); ?></p>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Artist</legend>

		<p><?= $this->data('artist_full_name'); ?></p>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Collection</legend>

		<p><?= $this->data('collection'); ?></p>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Artwork Type</legend>

		<p><?= $this->data('type'); ?></p>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Artwork Medium</legend>

		<p><?= $this->data('medium'); ?></p>

	</fieldset>

</div>

<div>

	<fieldset>

		<div>

			<fieldset>

				<legend>Parent Location</legend>

				<p><?= $this->data('parent_location_name'); ?></p>

			</fieldset>

		</div>

		<div>

			<fieldset>

				<legend>Building</legend>

				<p><?= $this->data('building_number_name'); ?></p>

			</fieldset>

		</div>

		<div>

			<fieldset>

				<legend>Room</legend>

				<p><?= $this->data('room_number_name'); ?></p>

			</fieldset>

		</div>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Description</legend>

		<p><?= $this->data('description'); ?></p>

	</fieldset>

</div>

<div>

	<fieldset>

		<legend>Images</legend>

<?php foreach($this->data('images') as $image) { echo $image; } ?>

	</fieldset>

</div>