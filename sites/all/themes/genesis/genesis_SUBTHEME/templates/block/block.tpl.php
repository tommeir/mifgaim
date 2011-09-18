<?php 
// $Id: block.tpl.php,v 1.1.2.7 2009/05/19 00:05:00 jmburnz Exp $

/**
 * @file block.tpl.php
 * Theme implementation to display a block.
 *
 * Available variables:
 * - $block->subject: Block title.
 * - $block->content: Block content.
 * - $block->module: Module that generated the block.
 * - $block->delta: This is a numeric id connected to each module.
 * - $block->region: The block region embedding the current block.
 *
 * Helper variables:
 * - $block_id: Outputs a unique id for each block.
 * - $classes: Outputs dynamic classes for advanced themeing.
 * - $edit_links: Outputs hover style links for block configuration and editing.
 * - $block_zebra: Outputs 'odd' and 'even' dependent on each block region.
 * - $zebra: Same output as $block_zebra but independent of any block region.
 * - $block_id: Counter dependent on each block region.
 * - $id: Same output as $block_id but independent of any block region.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * @see template_preprocess()
 * @see template_preprocess_block()
 * @see genesis_preprocess_block()
 */

/**
 * Block Edit Links
 * To disable block edit links remove or comment out the $edit_links variable 
 * then unset the block-edit.css in your subhtemes .info file.
 */
 //block-views-dataitems-block_1.tpl.php
?>
<?php if ($block_id == 'block-webform-client-block-7'): ?>
<div class="block" id="webformblock4data"><div id="formtabs">
<ul>
<li><a href="#block-webform-client-block-7"><?php print t('Feeback');?></a></li>
<li><a href="#block-webform-client-block-3"><?php print t('Ideas and suggestions');?></a></li>
<li><a href="#block-webform-client-block-8"><?php print t('Date Request');?></a></li>
</ul>
<?php endif; ?>
<div id="<?php print $block_id; ?>" class="<?php print $classes; ?>">
  <div class="block-inner">

    <?php if ($block->subject): ?>
      <h3 class="block-title"><?php 
	    if($block_id == "block-views-dataitems-block_1") {
	      print t('Latest sets') . '<span class="small-link"><a href="/data/all">'. t('See all ') . ' ' .cpo_special_data_count() . ' ' . t(' datasets').'</a></span>';
		}else{	   
	      print $block->subject; 
		} ?>
	  </h2>
    <?php endif; ?>

    <div class="block-content"><?php print $block->content ?></div>

    <?php print $edit_links; ?>

  </div>
</div> <!-- /block -->
<?php if ($block_id == 'block-webform-client-block-8'): ?>
</div></div>
<?php endif; ?>