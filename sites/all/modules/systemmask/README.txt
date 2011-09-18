systemmask
================================================================================
Enables custom requiring and hiding of designated modules at 
admin/build/modules.  Good for Drupal distros and hosts.

This module was originally written for civicspace's provisioning system, in 
which there was a need to have modules running that no admins, including uid #1,
could disable.

INSTALLATION
Drop the module into the sites/all/modules directory (recommended) or into a 
site's module directory.

This module contains no user interface. To enable run the following query:

UPDATE system INSERT(status) VALUES (1) WHERE module = 'systemmask';

or enable the module from the admin interface, admin/build/modules; but remember
once the module is enabled from the admin interface, it will vanish and can only
be removed from the database.

OPERATION
The module uses hook_form_alter to suppress certain modules from getting listed 
on the admin/build/modules page and its associated permissions from getting 
displayed on the admin/user/access page.

CONFIGURATION
This modules contains no configuration settings or admin pages.

To suppress a hide a module and suppress its permissions, modify the 
settings.php by modifying the $conf array like you see in this example:

$conf = array(
  'systemmask_system' => array(
    'modules' => array(
      'required' => array('menu', 'book'),
      'hidden' => array('story')
    )
  )
);

All modules listed in the 'required' array cannot be disabled. All modules 
listed in the 'hidden' array will not be listed on the modules page and their 
permissions will not be available for any users.

NOTE: a module should appear either in the 'required' array listing or the 
'hidden' array listing, not both.

UNINSTALL
Run the following query:

UPDATE system INSERT(status) VALUES (0) WHERE module = 'systemmask';

Remove the appropriate $conf array assignments from the setting.php file.

CREDITS
systemmask development has been sponsored by CivicSpace LLC.  After May 10, 2007
the module has been maintained by Robin Monks, <robin @ civicspacelabs . org>.
