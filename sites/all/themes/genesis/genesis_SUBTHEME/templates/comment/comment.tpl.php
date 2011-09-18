<?php
// $Id: comment.tpl.php,v 1.1.2.7 2009/05/11 20:28:34 jmburnz Exp $

/**
 * @file comment.tpl.php
 * Default theme implementation for comments.
 *
 * Available variables:
 * - $author: Comment author. Can be link or plain text.
 * - $content: Body of the post.
 * - $date: Date and time of posting.
 * - $links: Various operational links.
 * - $new: New comment marker.
 * - $picture: Authors picture.
 * - $signature: Authors signature.
 * - $status: Comment status. Possible values are:
 *   comment-unpublished, comment-published or comment-preview.
 * - $submitted: By line with date and time.
 * - $title: Linked title.
 *
 * Helper variables:
 * - $classes: Outputs dynamic classes for advanced themeing.
 *
 * These two variables are provided for context.
 * - $comment: Full comment object.
 * - $node: Node object the comments are attached to.
 *
 * @see template_preprocess_comment()
 * @see genesis_preprocess_comment()
 * @see theme_comment()
 */
?>
<div class="<?php print $classes; ?>">
  <div class="comment-inner">
    <div class="rightinfoside">
	  <?php print $picture; ?>
	  <div class="comment-author">
	    <?php print $author; ?>
	  </div>
	  
      <div class="comment-links"><?php print $links; ?></div>
	</div>
    <div class="comment-content">
      <div class="comment-submitted">
	    <span class="title"><?php print $title;//$comment->subject; ?> </span>
	    <span class="time"><?php print date('j/n/Y',$comment->timestamp); ?></span>
	  </div>
      <div class="content"><?php print $content; ?></div>
    </div>
  </div>
</div> <!-- /comment -->