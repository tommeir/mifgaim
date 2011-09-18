// $Id: README.txt,v 1.9 2010/02/25 19:18:47 ceardach Exp $

PERMIT OWN PERMISSIONS

Users in roles possessing the 'share permissions' permission are given the
ability to access the permissions administration page like a user with
'administer permissions', but they can only see permissions that exist in one of
their roles.

No configuration necessary (or available), except for granting selected roles
the 'share permissions' capability at http://example.com/admin/user/permissions
(where example.com is the URL of your web site).


Installation
------------

Copy the pwn directory into your website's sites/all/modules directory, then
activate the module by visiting your site's modules page.

If you are using drush, you may enter the following commands:
  drush dl pwn
  drush enable pwn


Requirements
------------

  - Drupal 6 or later, though there is no reason it could not be backported


API
----

pwn_compare_permissions($permsA, $permsB)
  Returns TRUE if all permissions in permsB are in permsA, otherwise it returns
  FALSE because permsB has some value that permsA does not have

pwn_get_permissions_for_roles($role_ids)
  Returns all permissions associated with the set of roles provided

pwn_grant_role($rid, $account = NULL)
  Checks to see if the given account could grant the given role when comparing
  which permissions each has.  If the role has any permissions the given account
  does not have access to, then this will return FALSE.

pwn_user_access_any($permA, $permB, ...)
  Returns TRUE if the user has any of the permissions.  Effectively a wrapper
  function for user_access().


Hooks Implemented
-----------------

hook_menu_alter()
  Adds the ability to access the permissions matrix if the user has the 'share
  permissions' permission

hook_form_user_admin_perm_alter()
  Hides all permissions in the permissions matrix that the current user does
  not have themselves

hook_form_user_register_alter()
  Adds ability to grant roles for users with 'share permissions through roles'
  permission

hook_form_user_profile_form_alter()
  Adds ability to grant roles for users with 'share permissions through roles'
  permission

hook_user()
  Validates modifications to form made in hook_form_user_profile_form_alter()


Additional Behaviors
--------------------

Permissions:
  - share permissions
    Allows a user to assign and revoke permissions to roles, where the
    permissions available are only composed of permissions the user currently
    has.
  - share permissions through roles
    Allows a user to assign and revoke roles to users, as long as those roles
    only contain permissions the user current has.  If any role contains a
    permission the user does not have themselves, they will not be able to grant
    or revoke that role from another user.
