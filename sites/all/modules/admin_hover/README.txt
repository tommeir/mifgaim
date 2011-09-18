# $Id: README.txt,v 1.2.2.1 2009/01/02 18:32:44 conortm Exp $

Admin:hover
Conor McNamara (conortm) < www.conortm.com >
Made possible by DPCI < www.databasepublish.com >

DESCRIPTION:
This module adds administrative links to nodes and blocks for users with permission to carry out those actions.  These links conveniently appear when the user mouses over an item to administer, and then disappear on mouse out.

REQUIREMENTS:
This module requires the Token module (http://drupal.org/project/token).
This module currently works with themes that implement node templates with containing divs whose id attributes are of the pattern "node-%nid", where %nid is the node's numeric nid.  Similarly, this module is compatible with themes that implement block templates with containing divs whose id attributes are of the pattern "block-%module-%delta".  Also, this module will only work if the user's browser has javascript enabled.

INSTALLATION:
No special instructions.  Just copy the entire admin_hover directory to your sites/all/modules (or sites/[site]/modules) directory.  Grant permission to 'access admin_hover' for the desired roles, and those users will have access to the actions they have permission to carry out.

QUESTIONS/FEEDBACK:
...are welcome! email < me at conortm dot com >
