This (uc_affiliate2) is a module providing affiliate functionality for Ubercart.

Initial drupal.org version based on code by:
********************************************
joe turgeon (http://arithmetric.com)
jhuckabee
VitaLife
thierry_gd (from Drupal affiliate module)
davegan

The code first used has since then been rewritten.

Features
********
User based tracking
Role based tracking
Monthly report of sales/commissions per affiliate in the admin section
Ability to create affiliate links (both text and image)
Product level link generation (e.g. generate a link for the actual product vs just the website), export to CSV or HTML
Ability to add cookies so that affiliates get credit for return visits even though they didn't go through original affiliate link.
Per product commissions
Multiple levels of commissioning
Fixed-rate and percentage commissions.
Token and Conditional Actions integration.

Notes
*****
- The admin ( user uid: 1) can't be an affiliate
- Create a role, give it affiliate permission, and then assign users to that role.
If you assign two roles with affiliate permissions to a user, bad things happen :)
- When the user is assigned to multiple roles, the last one with affiliate permissions is taken as the current one.
- The path for affiliate links defaults to /affiliate/[uid]
  However, this can be overridden by adding the following line
  $conf['uc_affiliate2_path'] = 'my_affiliate_path';
  in the site's settings.php file

Upgrading from 2.0-rc2
**********************************
- You must resave your affilite settings for the commission cancellation to work
- Commission cancellation (for previously added orders) might leave incorrect admin counters, because of a earlier time&date bug
- The orders counter works only for orders added after the upgrade
- The "All orders" tab shows only the orders added after the upgrade.
- Be sure to run update.php!

Sponsors
********
Aaron Schiff (http://www.ecopaper.com/)
Polish Your Image (http://polishyourimage.com)
A Mountain Top, LLC (http://amountaintop.com)
Health Curve, LLC (http://www.myhealthcurve.com) 