<?php
// $Id: views-view-fields.tpl.php,v 1.6 2008/09/24 22:48:21 merlinofchaos Exp $
/**
 * @file views-view-fields.tpl.php
 * Default simple view template to all the fields as a row.
 *
 * - $view: The view in use.
 * - $fields: an array of $field objects. Each one contains:
 *   - $field->content: The output of the field.
 *   - $field->raw: The raw data for the field, if it exists. This is NOT output safe.
 *   - $field->class: The safe class id to use.
 *   - $field->handler: The Views field handler object controlling this field. Do not use
 *     var_export to dump this object, as it can't handle the recursion.
 *   - $field->inline: Whether or not the field should be inline.
 *   - $field->inline_html: either div or span based on the above flag.
 *   - $field->separator: an optional separator that may appear before a field.
 * - $row: The raw result object from the query, with all data it fetched.
 *
 * @ingroup views_templates
 */
?>
<div class="titlesection">
  <h3 class="title"><?php print $fields['title']->content?></h3>
  <span class="date"><?php print $fields['created']->content?></span>
</div>
<div class="infoline">
  <span class="datasource"><?php print $fields['field_origin_value']->label.' : '.$fields['field_origin_value']->content?></span>
  <span class="category"><?php print $fields['tid']->content?></span>
</div>
<div class="content">
<?php print $fields['field_teaser_value']->content?>
&nbsp;<span class="readmore"><?php print str_replace('href=','alt = "' . t('Read more') . ' " title = "' . t('Read more') . '"  href=', $fields['view_node']->content)?></span>
</div>
