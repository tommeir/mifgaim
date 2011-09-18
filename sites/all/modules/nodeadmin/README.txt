/**
 * Content Administration module (nodeadmin)
 * Simple and speedy interface for content administration.
 * 
 * By Joe Turgeon [http://arithmetric.com]
 * Licensed under GPL version 2
 * Version 2009-04-23
 */

This module provides a dynamic interface for content administration.

It is intended to supplement Drupal's built-in content management page,
and features these improvements over the standard interface:

-- Uses AHAH to show node previews and node edit forms within the existing page.

-- Uses AJAX to perform searches, apply filters, and retrieve the results
without reloading the entire page.

-- Adds filters by author and text content (using Drupal's built-in search).

-- Shows how many nodes exist within a given set of filters, and allows
direct navigation to any page of results.

-- The built-in content management page requires the 'administer nodes' access
permission, which grants access to a broad range of content-related operations.
This module's page is accessible to users granted a permission for only that
purpose. Through this page, normal content permissions are enforced, so content
can be viewed, modified, or deleted only by users who have these permissions.

JavaScript is required for most of these features. Also, this module requires
PHP version 5.2.

Installation:

1. Download the package and extract the files to your modules directory.

2. Enable the module on the Modules configuration page.

3. Grant the appropriate users permission to 'access content administration'
on the Permissions page.

Usage:

1. Go to the Content Administration page provided by this module under the 
Administer >> Content Management menu.

2. You should see a set of characteristics by which to filter nodes, and a
list of nodes with icons to view, edit, delete. Clicking on these buttons
will display the node rendered, an edit form for the node, or a confirmation
form to delete the node.

For more information, to report an issue, or to support this module's development,
see this module's project page at:
http://drupal.org/project/nodeadmin/

Known issues:

-- Wysiwyg editors (like FCKeditor, TinyMCE, and others) are not yet working
within the Content Administration page.

Thanks:

To Lullabot for contributing a free set of GPL-licensed icons at:
http://www.lullabot.com/articles/free_gpl_icons_lullacons_pack_1

To Christian Bach for the jQuery tablesorter plugin that provided
inspiration and arrow icons for the table headers.
