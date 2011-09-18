<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language; ?>" xml:lang="<?php print $language->language; ?>">
<head>
  <title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <?php print $scripts; ?>
  <!--[if lt IE 7]>
    <link rel="stylesheet" href="<?php print $base_path . $directory; ?>/ie.css" type="text/css">
  <![endif]-->
</head>

<body class="<?php print $body_classes; ?>">
<div class="container">
	
	<div id="header">
		<div class="titles">
		  <?php if ($site_name): ?>
		    <h1 id="site-name">
		      <a href="<?php print $base_path ?>" title="<?php print t('Home') ?>"><?php print $site_name ?></a>
		    </h1>
		  <?php endif; ?>
		  <?php if ($site_slogan): ?>
		    <div class="slogan"><h2><?php print $site_slogan; ?></h2></div>
		  <?php endif; ?>
		</div>
	</div>
	
	
	<div id="content">	
		<?php if ($breadcrumb): ?>
            <div id="breadcrumb">
          <?php print $breadcrumb; ?>
            </div><!-- /breadcrumb -->
        <?php endif; ?>
         
		<?php if ($tabs): ?>
          <div id="content-tabs">
            <?php print $tabs; ?>
          </div>
        <?php endif; ?>
	
	<div class="post-frame">
		<div class="post-frame-top"></div>		  
		   
		   <?php print $content_top; ?>
		            
			<div class="post-content">
				<?php print $messages; ?>
				
				<?php print $content ?>
				<div class="clear"></div>
			</div>
			
			<?php print $content_bottom; ?>
			<div class="post-footer"></div>
		<span class="post-frame-bottom"></span>
	</div></div>
	
	
	
	
<div id="sidebar">
	<?php if ($primary_links): ?>
		<div class="widget grassland-navigation-widget" id="navigation">
			<span class="widget-top"></span>
			<div class="widget-centre">
        		<h3>Navigation</h3>
          		<?php print menu_tree($menu_name = 'primary-links'); ?>    	
			</div>
			<span class="widget-bottom"></span>
		</div>
	<?php endif; ?>	
				
	<div class="widget grassland-navigation-widget">
		<span class="widget-top"></span>
		<div class="widget-centre">
			<?php print $left ?>
			<?php print $right ?>			
		</div>
		<span class="widget-bottom"></span>
	</div>
	
	<?php if ($search_box): ?>
		<div class="widget widget_search ">
			<span class="widget-top"></span>
			<div class="widget-centre">
				<h3>Search</h3>
	        	<?php print $search_box; ?>    		
			</div>
			<span class="widget-bottom"></span>
		</div>
	<?php endif; ?>
</div>		
	
	
	
<div id="footer">

			<div class="footer-frame-top"></div>	
			<div class="footer-content"><?php print $footer ?></div>
			<div class="post-footer"></div>
			<span class="footer-frame-bottom"></span>
			
			<span class="footer-link"><?php print $footer_message ?></span>
			
		</div>
	</div>
<?php print $closure ?>
</body>
</html>