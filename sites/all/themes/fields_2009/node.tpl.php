  <div class="node<?php if ($sticky) { print " sticky"; } ?><?php if (!$status) { print " node-unpublished"; } ?>">

    <?php  if ($page == 0) { ?>
      <h2>
        <a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title; ?></a>
      </h2>
    <?php } ?>

    <div class="submitted"><?php print format_date($node->created, 'custom', 'd M Y'); ?></div>
<!--     <span class="submitted"><?php //print $submitted?></span> -->

    <div class="content"><?php print $content?></div>

    <?php if ($terms || $links){ ?>
    <div class="node_infos">
      <?php if ($terms) { ?>
        <div class="taxonomy">Archiviato in: <?php print $terms?></div>
      <?php } ?>


      <?php if ($links){ ?>
      <div class="links">&raquo; <?php print $links ?></div>
      <?php } ?>
      </div>
    <?php } ?>

  </div>

