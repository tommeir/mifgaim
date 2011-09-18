# $Id: README.txt,v 1.3 2009/05/12 22:34:51 gibbozer Exp $


Drupal Beach Theme
------------------

 Project page : http://drupal.org/project/beach
 Provider : Drupal Thailand (http://drupal.in.th/)
 Maintainers : (Trible "S")
   1. Sugree Phatanapherom (http://sugree.com/)
   2. Suksit Sripitchayaphan (http://suksit.com/)
   3. Sungsit Sawaiwan (http://webzer.net/)


Using Beach Theme Settings
--------------------------

1. Go to "/admin/build/themes/settings/beach"

2. Choose one from 6 levels of page width you wish, the theme default width is 840px (Medium). Note: Narrow and Medium Width may not suite 2 sidebars layout.

3. Choose "IE Transparent PNG Fix" features if you intent to support IE6 and below. This feature will add a jQuery plugin to fix ugly gray background for tranparent PNG image. Read more documentation at http://jquery.andreaseberhard.de/pngFix/.

4. If you want to override theme default stylesheet, please read inside custom-sample.css file which tell you how to "Add Customized Stylesheet". This method will keep your customized style separate from the default and easy to upgrade and safe you from losing it when Drupal Beach new version is released.

5. Breadcrumbs and "Back to Top link" : Just check or uncheck the box to toggle them.

6. Don't forget to "Save Configuration"

7. DONE!


Performance Boost
-----------------

In production site, you should consider to enable "Page Cache" for performance boost. To configure "Page Cache" go to "/admin/settings/performance" then set the options you wish there.