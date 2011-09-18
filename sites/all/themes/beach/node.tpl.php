<?php
// $Id: node.tpl.php,v 1.7 2009/05/10 21:56:07 gibbozer Exp $
?><div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?> clear-block">

  <?php if (!$page && $submitted): ?>
    <div class="post-info">

      <?php if ($teaser || $preview): ?>
        <span class="post-date"><?php print (format_date($node->created, 'custom', 'F d, Y')) ?></span>
        <span class="node-author"><?php print t('By ') ?><?php print $name ?></span>
      <?php endif; ?>

      <?php if($comment_count && $teaser): ?>
        <span class="commment-count"><a rel="nofollow" href="<?php print $node_url ?>#comment-number" title="<?php print t('Comment on ') ?><?php print $title?>"><?php print $comment_count.' '. format_plural($node->comment_count, 'comment', 'comments') ?></a></span>
      <?php endif; ?>
    </div> <!--/post-info-->
  <?php endif; ?>

  <?php if (!$page): ?>
    <h2 class="node-title">
      <a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a>
    </h2>
  <?php endif; ?>

  <?php if (!$teaser && !$preview): ?>
    <h1 class="title"><?php print $title ?></h1>
    <?php if ($submitted): ?>
      <div class="node-info">
      <span class="node-author"><strong><?php print t('By ') ?><?php print $name ?></strong></span>
      <span class="post-date"><?php print (format_date($node->created, 'custom', 'F d, Y')) ?></span>
      </div>
      <?php if ($picture): ?><?php print $picture ?><?php endif; ?>
    <?php endif; ?>
  <?php endif; ?>


  <div class="content clear-block"><?php print $content ?></div>

  <?php if ($teaser && $readmore): ?>
    <p class="node-more clear-block"><a class="node-more" href="<?php print $node_url ?>" title="<?php print t('More on ').$title ?>"><?php print t('More') ?></a></p>
  <?php endif;?>

        <?php if ($terms): ?><div class="terms" title="<?php print t('Terms') ?>"><?php print $terms ?></div><?php endif;?>
	
    <?php if (!$teaser && $links): ?>
      <div class="node-links">
        <?php print $links; ?>
      </div>
    <?php endif; ?>

</div>