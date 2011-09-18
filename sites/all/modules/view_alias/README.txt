VIEW ALIAS

This module aids in the bulk creation and deletion of SEO friendly view aliases.

In the past I've worked on several sites that utilize a single view which takes in a single taxonomy term id to display a list
of related items.  And on these sites maintaining the url aliases for them was a pain to do by hand, so I wrote this module
to do the repetition for me.


SETUP
1. untar the tarball into your drupal modules directory
2. enable the module
3. visit admin/build/path/pathauto for configuration options.

Generating Aliases
View Alias is integrated with pathauto, (admin/build/path/pathauto).  So expand "View Alias Settings"
settings fieldset to select the views to alias.

Steps to make a view avaialbe to view_alias:
1. View must exist and have a page display.
2. Under Arguments, View must have an arg of "Taxonomy: TERM ID".
3. Under Validator options:
  a. Set Validator to Taxonomy Term, then select your term vocabulary.
  b. Set Argument Type to "Term ID"

In the 6.x version the views with term arguments and page displays are
automatically displayed for you to choose from.  Simply select the views
to alias and check the "Bulk generate aliases ..." box and Save the
configuration to kick off the generation.


/** NOT DONE YET **/
Recurring Aliases
This works of hook_taxonomy to update aliases when terms are updated, created or deleted.
For each view:
1. check "Create/Update/Delete aliases for <view-name> on term creation"
2. select the "Vocabulary to alias" from the select box.
3. Save configuration.


FAQ
WHY NOT JUST USE <view-name>/<term-name>?
I kept running into duplicate terms when not using the full path.  I also found that people were turned off by some of the special characters that showed up in the browser bar.

NO TOKENS?
Yep, no tokens, I didn't really see the need for them.  But if you have a good
reason for them.. let me know and we can figure something out.

Eric Mckenna, Phase2 Technology
emckenna@phase2technology.com
