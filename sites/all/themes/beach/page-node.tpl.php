<?php
// $Id: page-node.tpl.php,v 1.9 2009/05/12 15:13:31 gibbozer Exp $
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">

  <head>

    <title><?php print $head_title ?></title>
    <meta http-equiv="content-language" content="<?php print $language->language ?>" />
    <?php print $meta; ?>
    <?php print $head ?>
    <?php print $styles ?>
    <?php print $scripts ?>

    <!--[if IE 7]>
      <link type="text/css" rel="stylesheet" media="all" href="<?php print $base_path . $directory ?>/css/ie7.css" />
    <![endif]-->

    <!--[if lte IE 6]>
      <link type="text/css" rel="stylesheet" media="all" href="<?php print $base_path . $directory ?>/css/ie6.css" />
    <![endif]-->

    <?php if (theme_get_setting('iepngfix')) : ?>
    <!--[if lte IE 6]>
      <script type="text/javascript"> 
        $(document).ready(function(){ 
          $(document).pngFix(); 
        }); 
      </script> 
    <![endif]-->
    <?php endif; ?>

  </head>

  <body class="<?php print $body_classes ?>">

  <?php print $nav_access ?>

  <div id="top-wrapper" class="clear-block">
    <div id="header" class="section <?php print $container_class ?>">

        <div id="branding" class="clear-block">
          <?php if ($logo): ?>
            <a href="<?php print check_url($front_page) ?>" title="<?php print t('Home') ?>"><img id="logo" src="<?php print $logo ?>" alt="<?php print t('Site Logo')?>" /></a>
          <?php endif; ?>

          <?php if ($site_name): ?>
            <h1 id="<?php print $sitename_id ?>">
              <a href="<?php print check_url($front_page) ?>" title="<?php print t('Home') ?>"><?php print $site_name ?></a>
            </h1>
          <?php endif; ?>

          <?php if ($site_slogan): ?>
            <p id="slogan"><?php print $site_slogan ?></p>
          <?php endif; ?>
        </div> <!-- /branding -->

        <?php if (!empty($primary_menu)): ?>
          <div id="primary-menu">
            <h3 class="hidden">Primary Menu</h3>
              <?php print $primary_menu; ?>
          </div>
        <?php endif; ?>

    </div>
  </div> <!--/top-wrapper-->

  <div id="middle-wrapper" class="clear-block">
  <div id="middle-inner" class="clear-block">
  <div class="section <?php print $container_class ?> clear-block">

        <?php if (($feed_icons) || !empty($secondary_menu) || ($search_box)): ?>
          <div id="menu-bar" class="clear-block">

            <?php print $search_box ?>

            <?php if (!empty($secondary_menu)): ?>
              <h3 class="hidden">Secondary Menu</h3>
                <?php print $secondary_menu; ?>
            <?php endif; ?>

            <?php print $feed_icons ?>

          </div>
        <?php endif; ?>

    <div id="main-content" class="column">
    <div class="content-inner clear-block">

        <?php if ($breadcrumb): ?><div class="top-breadcrumb clear-block"><?php print $breadcrumb; ?></div><?php endif; ?>

        <?php if ($mission): ?>
          <div id="mission"><?php print $mission ?></div>
        <?php endif; ?>

        <?php if ($content_top): ?>
          <div id="top-content-block" class="content-block clear-block">
            <?php print $content_top ?>
          </div>
        <?php endif; ?>

        <?php if (!empty($messages)): ?><?php print $messages ?><?php endif; ?>

        <?php if (!empty($help)): ?><?php print $help ?><?php endif; ?>

        <?php if (!empty($tabs)): ?><?php print $tabs ?><?php endif; ?>

        <?php print $content ?>

<?php if ($links): ?><div class="node-links"><?php print $links; ?></div><?php endif; ?>

        <?php if ($content_bottom): ?>
          <div id="bottom-content-block" class="content-block clear-block">
            <?php print $content_bottom ?>
          </div>
        <?php endif; ?>

        <?php if ($breadcrumb): ?><div class="bottom-breadcrumb clear-block"><?php print $breadcrumb; ?></div><?php endif; ?>

    </div>
    </div>

    <?php if ($left): ?>
      <div id="sidebar-left" class="column sidebar">
      <?php print $left ?>
      </div>
    <?php endif; ?>

    <?php if ($right): ?>
      <div id="sidebar-right" class="column sidebar">
        <?php print $right ?>
      </div>
    <?php endif; ?>

  </div>
  </div> <!--/middle-inner-->
  </div> <!--/middle-wrapper-->


  <div id="bottom-wrapper" class="clear-block">
    <div id="footer" class="section <?php print $container_class ?>">

      <?php if ($footer_column_count !== 0): ?>
        <div class="wrap-<?php print $footer_column_count ?>-col column-wrapper clear-block">

          <?php if ($footer_1): ?>
            <div class="footer-column">
              <?php print $footer_1 ?>
            </div>
          <?php endif; ?>

          <?php if ($footer_2): ?>
            <div class="footer-column<?php print $column2_is_last ?>">
              <?php print $footer_2 ?>
            </div>
          <?php endif; ?>

          <?php if ($footer_3): ?>
            <div class="footer-column<?php print $column3_is_last ?>">
              <?php print $footer_3 ?>
            </div>
          <?php endif; ?>

          <?php if ($footer_4): ?>
            <div class="footer-column<?php print $column4_is_last ?>">
              <?php print $footer_4 ?>
            </div>
          <?php endif; ?>

        </div> <!-- /footer-column-wrap -->
      <?php endif; ?>

      <div id="credit-wrap" class="clear-block">
        <?php if ($footer_message): ?>
          <p id="site-info"><?php print $footer_message ?></p>
        <?php endif; ?>

        <?php print $to_top ?>

        <?php print $closure ?>
      </div> <!--/credit-wrap-->
    </div>
  </div> <!--/bottom-wrapper-->

  </body>
</html>