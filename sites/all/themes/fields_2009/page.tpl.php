<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language ?>" xml:lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">

<head>
  <title><?php print $head_title ?></title>
  <?php print $head ?>
  <?php print $styles ?>
  <?php print $scripts ?>

  <script type="text/javascript"><?php /* Needed to avoid Flash of Unstyle Content in IE */ ?> </script>

  <link rel="stylesheet" type="text/css" href="<?php print base_path(). path_to_theme(); ?>/drupal_base.css"/>

  <!--[if IE 6]>
  <link rel="stylesheet" type="text/css" href="<?php print base_path(). path_to_theme(); ?>/ie.css"/>
  <![endif]-->

</head>
<body>

<div id="wrapper">

<div id="column994"> <!--START COLUMN 994px-->

  <div id="header"> <!--START HEADER-->

    <div id="logo">
      <a href="<?php print base_path(); ?>"><img src="<?php print base_path() . path_to_theme(); ?>/logo.png" alt="Site Name" title="Site Name"/></a>
    </div>

  </div> <!--END HEADER-->

  <div id="container"> <!--START CONTAINER-->

    <?php if (isset($primary_links)) { ?>
    <div id="primary">
      <?php print theme('links', $primary_links) ?>
    </div>
    <?php } ?>

    <div class="clean">&nbsp;</div>

    <div id="column950"> <!--START COLUMN 950-->

      <div id="header_block"> <!--START HEADER BLOCK-->
        <?php if ($header) { print $header; } ?>
      </div> <!--END HEADER BLOCK-->

      <div class="clean">&nbsp;</div>

      <?php if ( $middle_left || $middle_center || $middle_right ) { ?>
      <div id="middle_blocks"> <!--START MIDDLE BLOCKS-->
        <div id="middle_blocks_bottom"> <!--START MIDDLE BLOCKS_BOTTOM-->
          <div id="middle_left"> <!--START MIDDLE LEFT-->
            <?php if ($middle_left) { print $middle_left; } ?>
          </div> <!--END MIDDLE LEFT-->
          <div id="middle_center"> <!--START MIDDLE CENTER-->
            <?php if ($middle_center) { print $middle_center; } ?>
          </div> <!--END MIDDLE CENTER-->
          <div id="middle_right"> <!--START MIDDLE RIGHT-->
            <?php if ($middle_right) { print $middle_right; } ?>
          </div> <!--END MIDDLE RIGHT-->
          <div class="clean">&nbsp;</div>
        </div> <!--END MIDDLE BLOCKS_BOTTOM-->
      </div> <!--END MIDDLE BLOCKS-->
      <?php } else { ?>
      <div id="top_shadow">&nbsp;</div>
      <?php } ?>

      <div id="main" <?php if (!$sidebar) { print "class=\"full\""; }  ?> > <!--START MAIN (content + sidebar)-->

        <div id="content"> <!--START CONTENT-->

          <h1 id="page_title"><?php print $title ?></h1>

          <?php // print $breadcrumb ?>

          <div class="tabs"><?php print $tabs ?></div>
          <?php if ($show_messages) { print $messages; } ?>
          <?php print $help ?>
          <?php print $content; ?>

        </div> <!--END CONTENT-->

        <?php if ($sidebar) { ?>
        <div id="sidebar"> <!--START SIDEBAR-->
          <?php print $sidebar; ?>
        </div> <!--END SIDEBAR-->
        <?php } ?>
        <div class="clean">&nbsp;</div>

      </div> <!--END MAIN (content + sidebar)-->

    </div> <!--END COLUMN 950-->

  </div> <!--END CONTAINER-->

  <div id="footer"> <!--START FOOTER-->
    <?php if (isset($secondary_links)) { ?>
    <div id="secondary">
      <?php print theme('links', $secondary_links) ?>
    </div>
    <?php } ?>

    <div id="footer_contact">
      Fields Template by <a href="http://www.themes-drupal.org">Themes-Drupal.org</a>, powered by <a href="http://www.finex.org">FiNeX.org</a>
    </div>
  </div> <!--END FOOTER-->

</div> <!--END COLUMN 994px-->

<div id="footer_message">
  <?php print $footer_message ?>
</div>

</div>

<?php print $closure ?>


</body>
</html>
