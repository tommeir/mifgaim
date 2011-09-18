// $Id: README.txt,v 1.1 2009/10/17 18:06:31 jonahellison Exp $

=============================
Views UI: Edit Basic Settings
=============================
Compatibility: Views 6.x-2.x

   http://drupal.org/project/views_headers_footers

This module provides a separate interface that displays a list of views (defined by you, so you
can exclude certain views) and allows users with the correct permission to modify baisc settings
such as header, footer, title, empty text, and number of items to display.

======
How-To
======
- To define which views to display, visit "Site Confirugation" --> "Views" --> "Editable headers/footers"
- Make sure the user role has the "edit views basic settings" permission.
- The edit page is accessed via "Content management" --> "Edit views."  Tabs are also created for
views pages.

Please note that the "override" button will be hidden, so if your view uses multiple displays,
you will probably want to modify the header, footer, empty text, title fields to be "overriden," 
otherwise it will update the default display when the user goes in and modifies them.