<?php
// $Id: box.tpl.php,v 1.1.2.3 2009/05/11 20:28:33 jmburnz Exp $

/**
 * @file box.tpl.php
 * Theme implementation to display a box.
 *
 * Available variables:
 * - $title: Box title.
 * - $content: Box content.
 *
 * @see template_preprocess()
 */
?>
<div class="box">
  <div class="box-inner inner">
    <?php if ($title): ?>
      <h2 class="box-title"><?php print $title ?></h2>
    <?php endif; ?>
    <div class="box-content">
      <?php print $content ?>
    </div>
  </div>
</div> <!-- /box -->