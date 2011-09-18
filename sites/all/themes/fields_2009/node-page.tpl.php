  <div class="node<?php if ($sticky) { print " sticky"; } ?><?php if (!$status) { print " node-unpublished"; } ?>">

    <?php  if ($page == 0) { ?>


      <h2>
        <a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title; ?></a>
      </h2>

    <?php } ?>

    <div class="content"><?php print $content?></div>

    <?php if ($links): ?>
    <div class="links">&raquo; <?php print $links ?></div>
    <?php endif; ?>

  </div>
