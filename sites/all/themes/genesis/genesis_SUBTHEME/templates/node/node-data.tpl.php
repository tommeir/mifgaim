<?php 
// $Id: node.tpl.php,v 1.1.2.8 2009/05/19 00:05:00 jmburnz Exp $

/**
 * @file node.tpl.php
 * Theme implementation to display a node.
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: Node body or teaser depending on $teaser flag.
 * - $picture: The authors picture of the node output from
 *   theme_user_picture().
 * - $date: Formatted creation date (use $created to reformat with
 *   format_date()).
 * - $links: Themed links like "Read more", "Add new comment", etc. output
 *   from theme_links().
 * - $name: Themed username of node author output from theme_user().
 * - $node_url: Direct url of the current node.
 * - $terms: the themed list of taxonomy term links output from theme_links().
 * - $submitted: themed submission information output from
 *   theme_node_submitted().
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type, i.e. story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the node. Increments each time it's output.
 *
 * Helper variables:
 * - $node_id: Outputs a unique id for each node.
 * - $classes: Outputs dynamic classes for advanced themeing.
 *
 * Node status variables:
 * - $teaser: Flag for the teaser state.
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the node.
 * - $readmore: Flags true if the teaser content of the node cannot hold the
 *   main body content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 * @see genesis_preprocess_node()
 */

?>
<?php if ($node->field_download_file[0]['view']||$node->field_downloads_links[0]['url']):?>
		  <div class="tabitem ti-resources">
			<div class="title"><h2><?php print t('Resources')?></h2></div>
			<div class="content">
			<?php if ($node->field_teaser[0]['view']):?>
		      <div class="Teaser">
				<span class="label"><?php print t('Teaser');?></span>
				<span class="value"><?php print $node->field_teaser[0]['view'] ;?></span>
		      </div>
			<?php endif;?>
		      <div class="download">
			    <span class="label"><?php print t('Downloads and links');?></span>
				<div class="value">
				  <?php 
			        if ($node->field_download_file[0]['view']) {
			          foreach ($node->field_download_file as $file) { print $file['view']; }
				    }
				    if ($node->field_downloads_links[0]) {
			        foreach ($node->field_downloads_links as $link) { print l($link['title'], $link['url'],array('fragment' => $link['fragment'], 'attributes'=>array('external' => TRUE),'query' => $link['query'])); 

					//dsm($link);
					}
				    }
			      ?>
				</div>
		      </div>
		    </div>
		  </div>
		<?php endif;?>
	    <div class="tabitem ti-overview">
		  <div class="title"><h2><?php print t('Overview')?></h2></div>
		  <div class="content">
			<div class="body">
			      <span class="label"><?php print t('Description');?></span>
			      <div class="value"><?php print $node->content['body']['#value']?></div>
			  </div>
			
			<?php if ($node->field_date_start[0]['view']):?>
		      <div class="released">
				<span class="label"><?php print t('Released')?></span>
				<span class="value"><?php print $node->field_date_start[0]['view'];?></span>
			  </div>
			<?php endif;?>
			<?php if ($node->field_data_link[0]['value']):?>
			<div class="field_data_link">
			  <span class="label"><?php print t('Additional info')?></span>
			  <span class="value"><?php print l(t('More info'),$node->field_data_link[0]['value'])?></span>
			</div>
			<?php endif;?>
			<?php if ($node->field_date_end[0]['view']):?>
			  <div class="updated">
				<span class="label"><?php print t('Last updated');?></span>
				<span class="value"><?php print $node->field_date_end[0]['view'];?></span>
			  </div>
			<?php endif;?>
			<?php if ($node->field_license[0]['view']):?>
			  <div class="license">
				<span class="label"><?php print t('License');?></span>
				<span class="value">
				    <?php print l($node->field_license[0]['view'],'taxonomy/term/'.$node->field_license[0]['value'])?>
				</span>
			  </div>
			<?php endif;?>
			<?php if ($node->field_version[0]['value']):?>
			  <div class="varsion">
				<span class="label"><?php print t('Version');?></span>
				<span class="value"><?php print $node->field_version[0]['value']?></span>
			  </div>
			<?php endif;?>
			<?php if ($node->field_frequency[0]['value']):?>
			  <div class="frequency">
				<span class="label"><?php print t('Frequency');?></span>
				<span class="value"><?php print $node->field_frequency[0]['value']?></span>
			  </div>
			  <?php endif;?>
			  <?php if ($node->field_precision[0]['view']):?>
			  <div class="precision">
				<span class="label"><?php print t('Precision');?></span>
				<span class="value"><?php print $node->field_precision[0]['view']?></span>
			  </div>
			<?php endif;?>
			<?php if ($node->field_geo_cover[0]['view']):?>
			  <div class="geographic">
				<span class="label"><?php print t('Geographic');?></span>
				<span class="value">
				  <?php foreach ($node->field_geo_cover as $term):?>
				  <?php $geooarr[] = l($term['view'], 'taxonomy/term/'.$term['value']);?>
				  <?php endforeach;?>
				  <?php print implode(", ", $geooarr);?>
				</span>
			  </div>
			<?php endif;?>
			
			  <?php if ($node->field_data_structure[0]):?>
			      <div class="structure">
			         <span class="label"><?php print t('Data Structure');?></span>
				   <div class="value">
				    <?php foreach ($node->field_data_structure as $term):?>
				        <?php $array[] = l($term['view'], 'taxonomy/term/'.$term['value']);?>
				    <?php endforeach;?>
				    <?php print implode(", ", $array);?>
				  </div>
			      </div>
			  <?php endif;?>
			  <div class="category">
				<span class="label"><?php print t('Category');?></span>
				<span class="value"><?php print l($node->field_category[0]['view'], 'taxonomy/term/'.$node->field_category[0]['value']);?></span>
			  </div>
			<?php if($node->field_free_tags[0]['value']):?>
			  <div class="tags">
			    <div class="label"><?php print t('Tags');?></div>
			    <div class="value">
  			      <div class="block-inner">
			        <div class="block-content">
				      <div class="view view-tags view-id-tags view-display-id-block_1 view-dom-id-4">
				        <div class="view-content">
				          <?php $is = 0;?>
						  <?php $ti = 1; $ttags = count($node->field_free_tags); ?>
 			              <?php foreach($node->field_free_tags as $id => $term):?>
  		                    <?php $is <= 9 ? $more = '"' : $more = ' tmore" style="display: none;"'; ?>
						    <?php $is++; ?>
				            <div class="views-row views-row-<?php print $id . $more; ?>>
  					          <div class="views-field-name">
					            <span class="field-content">
								<?php $ti == $ttags ? $ttext = '' : $ttext = ', '; ?>
						        <?php $tlink = l($term['view'],'taxonomy/term/'. $term['value'],array('html' => true)); print $tlink . $ttext;?>
						        </span>
						      </div>
					        </div>
							<?php $ti++; ?>
					      <?php endforeach;?>
					      <?php if ($is>9):?><div id="tmore" class="more"><?php print l(t('More tags'),'')?></div><?php endif;?>
				        </div>
				      </div>
				    </div>
			      </div>
			    </div>
			  </div>
			<?php endif;?>
			  
		  </div>
		</div>
		<div class="tabitem ti-datas">
		  <div class="title"><h2><?php print t('Data Source')?></h2></div>
		  <div class="content">
			<?php if ($node->field_origin[0]['view']):?>
		      <div class="author">
				<span class="label"><?php print t('Source');?></span>
				<span class="value"><?php print l($node->field_origin[0]['view'], 'taxonomy/term/'.$node->field_origin[0]['value']);?></span>
		      </div>
			<?php endif;?>
			 <?php if ($node->field_contact_text[0]['value']): ?>
		      <div class="details">
				<span class="label"><?php print t('Contact information')?></span>
				<div class="value">
				  <div class="text"><?php print $node->field_contact_text[0]['value']?></div>
				  <?php print $node->field_contact_link[0]['view']?>
				</div>
		      </div>
			<?php endif;?>
		  </div>
		</div>