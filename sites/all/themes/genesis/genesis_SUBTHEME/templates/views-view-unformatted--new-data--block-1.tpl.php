<?php
// $Id: views-view-unformatted.tpl.php,v 1.6 2008/10/01 20:52:11 merlinofchaos Exp $
/**
 * @file views-view-unformatted.tpl.php
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */
$class='views-row';
?>
<?php if (!empty($title)): ?>
  <h3><?php print $title; ?></h3>
<?php endif; ?>
<div id="views-accordion-ui" style="height: 200px;">
<?php foreach ($rows as $id => $row): ?>
    <?php if (count($rows)==$id+1) { $class.= ' last'; } ?>
	<div class="<?php print $class?>">
	<?php print $row; ?>
	</div>
<?php endforeach; ?>
</div>