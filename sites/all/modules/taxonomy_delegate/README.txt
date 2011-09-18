$Id $ 
Taxonomy Delegate Module
------------------------

Introduction
------------

This module allows an administrator with "administer taxonomy" permission to delegate the
administration of a vocabulary to a non-admin role.

The role to which this authority has been delegated may add, modify, and delete terms from
a vocabulary. They may, not however, modify the vocabulary itself, nor may they create new
vocabularies.

Given that Forums are controlled by a vocabulary, one might give the ability to control
them to someone who has no other administrative rights. They can then change the
"containers" (parent terms) and forums (child terms). Other popular modules that are
vocabulary driven are Image Gallery and Glossary, so they are functions that can be easily
delegated. There are many ways in which this module can help.

Sponsor
-------
This module's development was underwritten by Seblin Hosting (http://www.seblin.com) for
a project I was working on for them. They were gracious enough to allow me to contribute
it to the Drupal community.

Installation
------------

Normal module installation procedures.

Settings
--------

Delegation of authority is done on the vocabulary edit page.

There is a settings page that allows you to hide the advanced option on the term page
if the administrator-delegate does not have the "administer taxonomy" permission.

Permissions
-----------

There are no new permissions for "Taxonomy Delegate." The roles must have "access content"
granted.

Menus
-----

This module adds a tab to the Administer >> Content management >> Categories page. Use the
"Delegation" tab to view which vocabularies (categories) have been delegated to which roles.

If a user is assigned to a role to which a vocabulary has been delegated, they will see a main ("Navigation") menu item to "Administer My Categories."
