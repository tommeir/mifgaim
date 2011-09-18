  <div class="comment<?php if ($comment->status == COMMENT_NOT_PUBLISHED) print ' comment-unpublished'; ?>">
    <?php if ($picture) {
    print $picture;
  } ?>
    <div class="content"><?php print $content; ?></div>
    <div class="submitted"><?php print $submitted; ?></div>
    <div class="links"><?php print $links; ?></div>
  </div>
