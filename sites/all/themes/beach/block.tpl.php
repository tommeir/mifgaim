<?php
// $Id: block.tpl.php,v 1.3 2009/05/09 00:02:17 gibbozer Exp $
?><div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="block block-<?php print $block->module ?> clear-block">
<?php if ($block->subject): ?>
  <h2 class="block-title"><?php print $block->subject ?></h2>
<?php endif;?>

  <div class="content clear-block">
    <?php print $block->content ?>
  </div>
</div>