<?php // $Id: node.tpl.php,v 1.1.2.3 2009/11/17 09:02:35 ipwa Exp $ ?>

  <?php if ($submitted): ?>
    <div class="post-date">
	  <span class="post-month"><?php print $month; ?></span>
	  <span class="post-day"><?php print $day; ?></span>
	  <span class="post-year"><?php print $year; ?></span>
    </div>
  <?php endif; ?>
  
  <div class="node<?php if ($sticky) { print " sticky"; } ?><?php if (!$status) { print " node-unpublished"; } ?>">
    <?php if ($picture) {
      print $picture;
    }?>
    <div class="contenttitle">
    <h1><a href="<?php print $node_url?>"><?php print $title?></a></h1>
    <p><?php print format_date($node->created); ?>
    <?php if (!$page && isset($comment_link)) { // We're in teaser view ?>
       | <?php print $comment_link; ?>
    <?php }; ?>
    </p>
    </div>
    <div class="content"><?php print $content?></div>
    <?php if ($terms) { ?><div class="taxonomy"><?php print t('Tags: ') . $terms; ?></div><?php }; ?>
    <?php if ($links) { ?><div class="links">&raquo; <?php print $links?></div><?php }; ?>
  </div>

  <div class="clear"></div>
  <div class="postspace">

  </div>