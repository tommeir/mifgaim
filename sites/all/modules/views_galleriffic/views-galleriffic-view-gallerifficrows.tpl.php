<?php
// $Id:
?>
<li>

  <a class="thumb" href="<?php print $fields['slide_field']->raw; ?>" title="<?php print $fields['title_field']->content; ?>">
    <img src="<?php print $fields['thumbnail_field']->raw; ?>" alt="<?php print $fields['title_field']->content; ?>" />
  </a>
  <div class="caption">
  <?php if($fields['slide_field']->download_original): ?>
		<div class="download">
			<a href="/<?php print $fields['slide_field']->filepath; ?>">Download Original</a>
		</div>
  <?php endif; ?>
		<div class="image-title"><?php print $fields['title_field']->content; ?></div>
		<div class="image-desc"><?php print $fields['description_field']->content; ?></div>
	</div>
</li>

