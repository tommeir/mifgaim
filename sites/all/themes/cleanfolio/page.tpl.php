<?php
// $Id: page.tpl.php,v 1.4 2010/01/19 14:51:55 sunn Exp $
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
<head>
<?php print $head ?>
<title><?php print $head_title ?></title>
<?php print $styles ?>
<?php print $scripts ?>
</head>
  <body<?php print phptemplate_body_class($leftside, $rightside); ?>>
<div id="header">
	<div class="site-title"><a href="<?php print base_path() ?>"><?php print $site_name ?></a></div>
</div><!-- end of header -->
<div id="container">
		<div id="nav">
    <?php if (!empty($primary_links)): ?>
      <?php print theme('links', $primary_links, array('class' => 'links primary-links')); ?>
    <?php endif; ?>
		</div><!-- end of nav -->
		<div id="slash"><?php print $slash ?></div><!-- end of slash -->
		<?php if ($messages): ?><div id="welcome"><?php print $messages ?></div><?php endif ?>
		<div id="content-area">
	  <div id="leftside">
				<?php print $leftside; ?>
				<div class="tabs"><?php print $tabs ?><?php print $tabs2 ?></div>
				<?php print $content ?>
			</div><!-- end of leftside -->
			<div id="rightside">
				<?php print $rightside; ?>
			</div><!-- end of rightside -->
			<div class="clear"></div>
		</div><!-- end of content-area -->
</div><!-- end of container -->
<div id="footer">
    <?php if (!empty($primary_links)): ?>
      <?php print theme('links', $primary_links, array('class' => 'links primary-links')); ?>
    <?php endif; ?>
    <p><?php print $footer_message ?></p>
</div>
</div>
</body>
<?php print $closure ?>
</html>
