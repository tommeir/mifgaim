$Id: README.txt,v 1.1.2.2 2009/12/17 09:37:48 psynaptic Exp $

AUTHOR/MAINTAINERS
==================

Created by Irakli Nadareishvili (aka irakli) of Phase2 Technology
  http://www.phase2technology.com/people/irakli

Co-maintained by Richard Burford (aka psynaptic) of Freestyle Systems
  http://freestylesystems.co.uk/


INSTALLATION
============

Follow the usual module installation procedure: http://drupal.org/node/70151


USAGE
=====

The edit links are automatically added to the content variable of blocks and nodes. If you happen to not print the content variable in your theme's template files you will need to print the block or node edit links in your node.tpl.php or block.tpl.php (or a derivative of):

<?php print $block_edit_links; ?>

or

<?php print $node_edit_links; ?>

The only requirement is that this variable is printed somewhere within the block or node div e.g.

<div class="block">
  <?php print $block_edit_links; ?>

or

<div class="node">
  <?php print $node_edit_links; ?>

This is so the jQuery code can find the blocks/nodes and the edit links within them.
