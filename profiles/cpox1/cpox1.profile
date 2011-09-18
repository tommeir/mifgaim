<?php
// $Id: cpox1.profile,v 1.0.1.1 2010/05/01 00:20:00 asaphk Exp $
/*****DONT USE drupal_flush_all_caches() !!!!!! ********/
/**
 * Return an array of the modules to be enabled when this profile is installed.
 *
 * @return
 *   An array of modules to enable.
 *
 *
 * 
 */
function cpox1_profile_modules() {
	return array(
		'block',
		'admin_menu', 'nice_menus',
		'text', 'optionwidgets',
		'number',
		'cpo_special',
		'filter',
		'node',
		'system',
		'user',
		'menu',
		'path',
		'search',
		'taxonomy',
		'translation',
		'locale',
		'php',
		'config_perms',
		'install_profile_api',
		'token',
		'pathauto',
		'nodeformsettings',
		'content',
		'fieldgroup','number',
		'optionwidgets','text','link','nodereference',
		'imageapi',
		'features',
		'imagefield','imagefield_tokens',
		'filefield',
		'imagecache','imagecache_ui',
		'imagecache_canvasactions',
		'imageapi','imageapi_gd',
		'libraries',
		'nodewords','nodewords_basic','nodewords_extra','nodewords_verification_tags',
		'pathauto',
		'captcha',
		'webform',
		'wysiwyg',
		'imce',
		'imce_wysiwyg',
		'better_formats',
		'views','views_ui','views_ui_basic','views_slideshow','views_bulk_operations','views404','taxonomy_menu_hierarchy',
		'transliteration','print_mail','print','taxonomy_menu','compact_forms','patch_manager',
		'filefield_paths','skinr',
		'globalredirect','views_customfield','vertical_tabs','nodeformcols',
		'nodeblock',
		'webformblock',
		'page_title',
		'googleanalytics',
		'translation404',
		'devel',
		'faq',
		'i18n','i18nblocks','i18ncck','i18ncontent','i18nmenu',
		'i18nstrings','i18nsync','i18ntaxonomy','i18nviews',
        'xmlsitemap','xmlsitemap_taxonomy','xmlsitemap_node',
		'xmlsitemap_menu','xmlsitemap_i18n','xmlsitemap_engines',
		'date_api','date','date_locale','date_popup',
		'date_timezone',
		'ckeditor_link',
		'taxonomy_manager',
		'role_delegation',
		'backup_migrate_files',
		'backup_migrate',
		'sitedoc',
		'menu_editor',
		'protect_critical_users',
		'roleassign',
		'email_confirm',
		//'administerusersbyrole',
		'systeminfo',
		'edit_term',
		'admin_hover',
		'nodeadmin',
		'taxonomy_delegate',
		'view_alias',
		'node_edit_protection',
		'password_change',
		'agreement',
		'menu_perms',
		'showaliases',
		'ghs',
		'revision_all',
		'readonlymode',
		'user_settings_access',
		'simple_menu_settings',
		'author_select',
		'pwn',
//		'settings_audit_log',
		'bettermenus',
		'submitagain',
		'systemmask',
		'search_config','report','ctools','recaptcha'
      );
}

/**
 * Return a description of the profile for the initial installation screen.
 *
 * @return
 *   An array with keys 'name' and 'description' describing this profile,
 *   and optional 'language' to override the language selection for
 *   language-specific profiles.
 */
function cpox1_profile_details() {
	return array(
		'name' => 'CPO x1.v1',
		'description' => 'Quick setup for new X1V1 based website'
	);
}

/**
 * Return a list of tasks that this profile supports.
 *
 * @return
 *   A keyed array of tasks the profile will perform during
 *   the final stage. The keys of the array will be used internally,
 *   while the values will be displayed to the user in the installer
 *   task list.
 */
function cpox1_profile_task_list() {}

/**
 * Perform any final installation tasks for this profile.
 *
 * The installer goes through the profile-select -> locale-select
 * -> requirements -> database -> profile-install-batch
 * -> locale-initial-batch -> configure -> locale-remaining-batch
 * -> finished -> done tasks, in this order, if you don't implement
 * this function in your profile.
 *
 * If this function is implemented, you can have any number of
 * custom tasks to perform after 'configure', implementing a state
 * machine here to walk the user through those tasks. First time,
 * this function gets called with $task set to 'profile', and you
 * can advance to further tasks by setting $task to your tasks'
 * identifiers, used as array keys in the hook_profile_task_list()
 * above. You must avoid the reserved tasks listed in
 * install_reserved_tasks(). If you implement your custom tasks,
 * this function will get called in every HTTP request (for form
 * processing, printing your information screens and so on) until
 * you advance to the 'profile-finished' task, with which you
 * hand control back to the installer. Each custom page you
 * return needs to provide a way to continue, such as a form
 * submission or a link. You should also set custom page titles.
 *
 * You should define the list of custom tasks you implement by
 * returning an array of them in hook_profile_task_list(), as these
 * show up in the list of tasks on the installer user interface.
 *
 * Remember that the user will be able to reload the pages multiple
 * times, so you might want to use variable_set() and variable_get()
 * to remember your data and control further processing, if $task
 * is insufficient. Should a profile want to display a form here,
 * it can; the form should set '#redirect' to FALSE, and rely on
 * an action in the submit handler, such as variable_set(), to
 * detect submission and proceed to further tasks. See the configuration
 * form handling code in install_tasks() for an example.
 *
 * Important: Any temporary variables should be removed using
 * variable_del() before advancing to the 'profile-finished' phase.
 *
 * @param $task
 *   The current $task of the install system. When hook_profile_tasks()
 *   is first called, this is 'profile'.
 * @param $url
 *   Complete URL to be used for a link or form action on a custom page,
 *   if providing any, to allow the user to proceed with the installation.
 *
 * @return
 *   An optional HTML string to display to the user. Only used if you
 *   modify the $task, otherwise discarded.
 */
function cpox1_profile_tasks(&$task, $url) {
    //Include
	install_include(cpox1_profile_modules());
	// Set themes
  $theme='fusion_core';
 
  install_disable_theme("garland");

  //db_query("UPDATE {system} SET status = 1 WHERE type = 'theme' and name = '%s'", $theme);
 // system_initialize_theme_blocks($theme);
	install_default_theme($theme);
  install_admin_theme('cleanr');

	/*
	* Default roles, users and permissions
	*/
	$master_rid = install_add_role('master');
	$manager_rid = install_add_role('manager');
	$editor_rid = install_add_role('editor');

    install_add_user('manager', 'manager', 'manager@cpo-test.co.il', array('manager'), 1);
    install_add_user('editor', 'editor', 'editor@cpo-test.co.il', array('editor'), 1);
    install_add_user_to_role(1, $master_rid);

	install_add_permissions($master_rid, array('use admin toolbar', 'access administration menu',
	'display drupal links', 'collapse format fieldset by default', 'collapsible format selection',
	'show format selection for blocks', 'show format selection for comments', 'show format selection for nodes',
	'show format tips', 'show more format tips link', 'administer blocks', 'use PHP for block visibility',
	'administer CAPTCHA settings', 'skip CAPTCHA', 'access comments', 'administer comments', 'post comments',
	'post comments without approval', 'administer administration theme', 'administer clean-urls',
	'administer content node settings', 'administer date-time', 'administer error reporting',
	'administer file system', 'administer modules', 'administer performance', 'administer site information',
	'administer site maintenance', 'administer themes', 'administer user profile fields',
	'display site building menu', 'display site configuration menu',
	'Use PHP input for field settings (dangerous - grant with care)',
	'administer custom breadcrumbs', 'use php in custom breadcrumbs', 'view date repeats',
	'administer date tools', 'access devel information', 'display source code', 'execute php code', 'switch users',
	'administer faq', 'administer faq order', 'create faq', 'edit faq', 'edit own faq', 'view faq page',
	'administer features', 'manage features', 'administer filters', 'administer google analytics',
	'opt-in or out of tracking', 'use PHP for tracking visibility', 'administer all languages',
	'administer translations', 'administer imageapi', 'administer imagecache', 'flush imagecache',
	'administer imce(execute PHP)', 'administer languages', 'translate interface', 'administer mailchimp',
	'administer menu', 'access content', 'administer content types', 'administer nodes', 'delete revisions',
	'revert revisions', 'view revisions', 'administer meta tags', 'edit canonical URL meta tag',
	'edit meta tag ABSTRACT', 'edit meta tag COPYRIGHT', 'edit meta tag DESCRIPTION', 'edit meta tag KEYWORDS',
	'edit meta tag REVISIT-AFTER', 'edit meta tag ROBOTS', 'administer page titles', 'set page title',
	'administer url aliases', 'create url aliases', 'administer pathauto', 'notify of path changes',
	'administer recaptcha', 'administer search', 'search content', 'use advanced search', 'access statistics',
	'view post access counter', 'access administration pages', 'access site reports', 'administer actions',
	'administer files', 'administer site configuration', 'select different theme', 'administer taxonomy',
	'translate content', 'Administer Translation404', 'access user profiles', 'administer permissions',
	'administer users', 'change own username', 'access all views', 'administer views', 'use views exporter',
	'edit views basic settings', 'access own webform submissions', 'access webform results', 'clear webform results',
	'create webforms', 'edit own webform submissions', 'edit own webforms', 'edit webform submissions',
	'edit webforms', 'use PHP for additional processing', 'administer xmlsitemap', 'administer date tools',
	'access admin_hover',
	'access backup and migrate',
	'access backup files',
	'administer backup and migrate',
	'delete backup files',
	'perform backup',
	'restore from backup',
	'administer Compact Forms',
	'menu edit devel',
	'menu edit features',
	'menu edit navigation',
	'menu edit primary-links',
	'menu edit secondary-links',
	'administer devel',
	'administer navigation',
	'administer primary-links',
	'administer secondary-links',
	'create nodeblock content',
	'create page content',
	'delete any nodeblock content',
	'delete any page content',
	'delete own nodeblock content',
	'delete own page content',
	'edit any nodeblock content',
	'edit any page content',
	'edit own nodeblock content',
	'edit own page content',
	'access content administration',
	'edit location meta tag',
	'edit meta tag Dublin Core CONTRIBUTOR',
	'edit meta tag Dublin Core CREATOR',
	'edit meta tag Dublin Core DATE',
	'edit meta tag Dublin Core DESCRIPTION',
	'edit meta tag Dublin Core PUBLISHER',
	'edit meta tag Dublin Core TITLE',
	'edit meta tag PICS-LABEL',
	'edit short URL meta tag',
	'edit Bing Webmaster Center verification meta tag',
	'edit Google Webmaster Tools verification meta tag',
	'edit Yahoo! Site Explorer verification meta tag',
	'administer patch manager',
	'access print',
	'administer print',
	'node-specific print configuration',
	'use PHP for link visibility',
	'access send to friend',
	'share permissions',
	'share permissions through roles',
	'access report',
	'administer report',
	'assign all roles',
	'assign editor role',
	'assign manager role',
	'assign master role',
	'assign roles',
	'search by category',
	'search by node type',
	'use keyword search',
	'see aliases',
	'View standard menu settings',
	'view site documentation',
	'access skinr',
	'access skinr classes',
	'administer skinr',
	'access system information',
	'administer system information',
	'User Settings'));
	install_add_permissions($manager_rid, array('use admin toolbar', 'access administration menu',
	'show format selection for blocks', 'show format selection for comments', 'show format selection for nodes',
	'show format tips', 'show more format tips link', 'skip CAPTCHA', 'access comments', 'administer comments',
	'post comments', 'post comments without approval', 'administer site information', 'administer site maintenance',
	'display site building menu', 'display site configuration menu', 'view date repeats', 'administer faq order',
	'create faq', 'edit faq', 'edit own faq', 'view faq page', 'flush imagecache', 'administer menu', 'access content',
	'administer nodes', 'delete revisions', 'revert revisions', 'view revisions', 'administer meta tags',
	'edit meta tag DESCRIPTION', 'edit meta tag KEYWORDS', 'administer page titles', 'set page title',
	'administer url aliases', 'create url aliases', 'search content', 'use advanced search', 'access statistics',
	'view post access counter', 'access administration pages', 'access site reports', 'administer site configuration',
	'administer taxonomy', 'access user profiles', 'administer users', 'change own username',
	'access own webform submissions', 'access webform results', 'clear webform results',
	'edit own webform submissions', 'edit own webforms', 'edit webform submissions', 'edit webforms',
	'menu edit navigation',
	'menu edit primary-links',
	'menu edit secondary-links',
	'administer navigation',
	'administer primary-links',
	'administer secondary-links',
	'create page content',
	'delete any page content',
	'delete own page content',
	'edit any nodeblock content',
	'edit any page content',
	'edit own nodeblock content',
	'edit own page content',
	'access content administration',
	'access print',
	'access send to friend',
	'access report',
	'search by category',
	'search by node type',
	'use keyword search',
	'see aliases',
	'View standard menu settings',
	'view site documentation',
	'access skinr',
	'access skinr classes',
	'access system information',
	'User Settings'));
	install_add_permissions($editor_rid, array('use admin toolbar', 'access administration menu',
	'show format selection for comments', 'show format selection for nodes', 'show format tips',
	'show more format tips link', 'skip CAPTCHA', 'access comments', 'administer comments', 'post comments',
	'post comments without approval', 'display site building menu', 'display site configuration menu', 'create faq',
	'edit faq', 'edit own faq', 'view faq page', 'access content',  'administer nodes',  'view revisions',
	'edit meta tag DESCRIPTION', 'edit meta tag KEYWORDS', 'set page title', 'create url aliases', 'search content',
	'use advanced search', 'access administration pages', 'access user profiles',
	'menu edit navigation',
	'menu edit primary-links',
	'menu edit secondary-links',
	'administer navigation',
	'administer primary-links',
	'administer secondary-links',
	'create page content',
	'delete any page content',
	'delete own page content',
	'edit any nodeblock content',
	'edit any page content',
	'edit own nodeblock content',
	'edit own page content',
	'access content administration',
	'access print',
	'access send to friend',
	'access report',
	'search by category',
	'search by node type',
	'use keyword search',
	'see aliases',
	'View standard menu settings',
	'User Settings'));
	install_add_permissions(2, array('access comments', 'post comments', 'post comments without approval',
	'access content', 'search content', 'view faq page', 'access user profiles',
	'access print',
	'access send to friend',
	'search by category',
	'search by node type',
	'use keyword search')); //authenticate
	install_add_permissions(1, array('access comments', 'access content', 'search content',
	'view faq page', 'access print',
	'access send to friend',
	'search by category',
	'search by node type',
	'use keyword search')); // Anonimus


  

	// Configure input formats
	install_format_set_roles(array($master_rid,$manager_rid,$editor_rid), 2); // Full html
	install_format_set_roles(array($master_rid,$manager_rid), 3); // PHP
	
 

	// Initialize a bunch of variables for sane defaults
	cpox1_initialize_variables();

	// Update the menu, clear views cache, ...
	menu_rebuild();
	
	views_invalidate_cache();
	
	// Example for creating, placing and disabling system and custom blocks:
	//$aboutsite = install_create_custom_block("ArrayShift was built for the 2009 Do It With Drupal conference in 24 hours. It's based on (read, a shameless clone of) <a href='http://stackoverflow.com'>Stack Overflow</a>.", t('What the..?'), 2);
	//$moreblock = install_create_custom_block('<br clear="both" /><h2>Looking for more? Browse the complete <a href="/questions">list of questions</a>, or <a href="/tags">popular tags</a>. Help us answer <a href="/unanswered">unanswered questions</a>.</h2>', t('Looking for more'), 2);
	//$notwhatyouwant = install_create_custom_block("<h2>Not the answer you're looking for? Browse <a href='/questions'>other questions</a> or <a href='/questions/ask'>ask your own.</a></h2>", t('Not what you want?'), 2);
	//install_add_block('block', $aboutsite, 'as_theme', 1, -10, 'right', 0, '', 0, 0, t('What the...?'));
	//install_add_block('block', $moreblock, 'as_thtme', 1, -1, 'footer', 1, 'front*');
	//install_add_block('block', $notwhatyouwant, 'as_theme', 1, -1, 'footer', 1, 'node/*');
	install_disable_block('user', 0, 'fusion_core');
	install_disable_block('user', 1, 'fusion_core');
	install_disable_block('system', 0, 'fusion_core');
  variable_set('nice_menus_menu_1', 'primary-links:0');
  variable_set('nice_menus_name_1','main menu');
  variable_set('nice_menus_type_1','down');
	install_set_block('nice_menus', 1, 'fusion_core', 'main_nav', 0, NULL, NULL, NULL, NULL, '<none>');	
	
 
	
  // Insert default user-defined node types into the database. For a complete
  // list of available node type attributes, refer to the node type API
  // documentation at: http://api.drupal.org/api/HEAD/function/hook_node_info.
  $types = array(
    array(
      'type' => 'page',
      'name' => st('Page'),
      'module' => 'node',
      'description' => st("A <em>page</em>, similar in form to a <em>story</em>, is a simple method for creating and displaying information that rarely changes, such as an \"About us\" section of a website. By default, a <em>page</em> entry does not allow visitor comments and is not featured on the site's initial home page."),
      'custom' => TRUE,
      'modified' => TRUE,
      'locked' => FALSE,
      'help' => '',
      'min_word_count' => '',
    ),
    array(
      'type' => 'nodeblock',
      'name' => st('NodeBlock'),
      'module' => 'node',
      'description' => st(""),
      'custom' => TRUE,
      'modified' => TRUE,
      'locked' => FALSE,
      'help' => '',
      'min_word_count' => '',
    ),
  );

  foreach ($types as $type) {
    $type = (object) _node_type_set_defaults($type);
    node_type_save($type);
  }
  //mark nodeblock & webform as a node-block
  variable_set('nodeblock_nodeblock', 1);
  variable_set('nodeblock_webform', 1);
  
  // Default nodes not be promoted & e published
  variable_set('node_options_page', array('status'));
  variable_set('node_options_nodeblock', array('status'));
  variable_set('node_options_webform', array('status'));
  variable_set('node_options_faq', array('status'));
  variable_set('node_options_image', array('status'));

  //disable comments   
  variable_set('comment_page', COMMENT_NODE_DISABLED);
  variable_set('comment_nodeblock', COMMENT_NODE_DISABLED);
  variable_set('comment_webform', COMMENT_NODE_DISABLED); 
  variable_set('comment_faq', COMMENT_NODE_DISABLED);
  variable_set('comment_image', COMMENT_NODE_DISABLED);  
  variable_set('faq_display',  hide_answer);
  // Don't display date and author information for page nodes by default.
  $theme_settings = variable_get('theme_settings', array());
  $theme_settings['toggle_node_info_page'] = FALSE;
  $theme_settings['toggle_node_info_nodeblock'] = FALSE;
  $theme_settings['toggle_node_info_webform'] = FALSE;
  $theme_settings['toggle_node_info_faq'] = FALSE;
  $theme_settings['toggle_node_info_image'] = FALSE; 
  $theme_settings['toggle_search'] = FALSE;  
  variable_set('theme_settings', $theme_settings);
  $fusion_core_setting = variable_get('theme_fusion_core_settings',array());
  $fusion_core_setting['toggle_language_switcher']=FALSE;
  variable_set('theme_fusion_core_settings', $fusion_core_setting);
  // Update the menu router information.
 
 
 
  //build the frontpage node
  $node1->type = 'page';
  $node1->uid = '1';
  $node1->status = '1';
  $node1->title = st('frontpage');
  $node1->path = 'content/front-page';
  node_save($node1);
  variable_set('site_frontpage', 'content/front-page');
  
  //build 'about us'
  
  $node2->type = 'page';
  $node2->uid = '1';
  $node2->status = '1';
  $node2->title = st('About');
  $node2->path = 'content/about';
  node_save($node2);
  
  //build 'contant us' webform node
  $node3->type = 'webform';
  $node3->uid = '1';
  $node3->status = '1';
  $node3->title = st('Contact');
  $node3->path = 'content/contact-us';
  $node3->webform['components'][3]=array ('name'=>'Name', 'type'=>'textfield');
  $node3->webform['components'][2]=array ('name'=>'Email', 'type'=>'textfield', 'email'=>'1', 'mandatory'=>'1');
  $node3->webform['components'][1]=array ('name'=>'Content', 'type'=>'textarea');
  $node3->webform['email'] = 'asaph@cpo.co.il';
  $node3->webform['email_from_name'] = 'default';
  $node3->webform['email_from_address'] = 'default';
  $node3->webform['email_subject'] = 'default';
  $node3->webform['roles'][0] = '1';
  $node3->webform['roles'][1] = '2';
  $node3->webform['submit_limit']='-1';
  $node3->build_mode = 0;
  $node3->readmore = FALSE;
  $node3->format = '0';
  
  node_save($node3);
//configure wysiwyg
  db_query("UPDATE {wysiwyg} SET editor = '%s' WHERE format = %d", 'ckeditor', 2);
  db_query("INSERT INTO {wysiwyg} (format, editor) VALUES (%d, '%s')", 1, 'ckeditor');
  db_query("UPDATE {languages} SET prefix='en' WHERE name='English'");
  db_query("UPDATE {system} SET weight=99 WHERE name='cpo_special'");
  //set primery links menu
 $menuitem = array(
  'link_path' => '<front>', 
  'link_title' => st('Home'),
  'menu_name'=>'primary-links',
  );
  menu_link_save($menuitem);
  
 $menuitem1 = array(
  'link_path' => 'faq',
  'link_title' => st('FAQ'),
  'menu_name'=>'primary-links',
  );
  menu_link_save($menuitem1);
  
  $menuitem2 = array(
  'link_path' => 'node/3',
  'link_title' => st('Contact'),
  'menu_name'=>'primary-links',
  );
  menu_link_save($menuitem2);
  
  $menuitem3 = array(
  'link_path' => 'node/2',
  'link_title' => st('About'),
  'menu_name'=>'primary-links',
  );
  menu_link_save($menuitem3);
  
  menu_rebuild();

}

/**
 * Implementation of hook_form_alter().
 *
 * Allows the profile to alter the site-configuration form. This is
 * called through custom invocation, so $form_state is not populated.
 */
function cpox1_form_alter(&$form, $form_state, $form_id) {
  //if ($form_id == 'install_select_profile') {
    //drupal_set_message('GOTCHA!');
    //drupal_set_message('GOTCHA:' . $form_id);
  //}
  if ($form_id == 'install_configure') {
    // Set defaults
	$server = explode('.', $_SERVER['HTTP_HOST']);
	$subdomain = $server[0];
    //$form['site_information']['site_name']['#default_value'] = st('CPO X1V1');
    $form['site_information']['site_name']['#default_value'] = $subdomain;
    $form['site_information']['site_mail']['#default_value'] = 'asaph@cpo.co.il';    $form['admin_account']['account']['name']['#default_value'] = 'master';
    $form['admin_account']['account']['mail']['#default_value'] = 'asaph@cpo.co.il';
    //$form['admin_account']['account']['pass']['#default_value'] = $subdomain;
    $form['server_settings']['date_default_timezone']['#default_value'] = '10800';
    $form['server_settings']['update_status_module']['#default_value'][0] = '0';
	// Set message - enter password only
    //drupal_set_message('NOTICE: All info has been setup - you should just enter a password in both fields and leave everything else as it is configured now. The password should be the same as the sitename - the same as the subdomain');
	}
}

function cpox1_initialize_variables() {
  // Basics
  variable_set('configurable_timezones', 0);
  variable_set('date_default_timezone_name', 'Asia/Jerusalem');
  variable_set('admin_toolbar', array('layout' => 'vertical', 'position' => 'nw', 'blocks' => array('admin-create' => -1, 'admin-menu' => 1, 'admin-devel' => -1)));
  variable_set('globalredirect_trailingzero', 2);
  variable_set('user_register', 0);
  variable_set('views_hide_help_message', 1);
  variable_set('views_exposed_filter_any_label', 'new_any');
  //variable_set('theme_settings', 'a:18:{s:11:"toggle_logo";i:1;s:11:"toggle_name";i:1;s:13:"toggle_slogan";i:0;s:14:"toggle_mission";i:1;s:24:"toggle_node_user_picture";i:0;s:27:"toggle_comment_user_picture";i:0;s:13:"toggle_search";i:0;s:14:"toggle_favicon";i:1;s:20:"toggle_primary_links";i:0;s:22:"toggle_secondary_links";i:0;s:20:"toggle_node_info_faq";i:0;s:24:"toggle_node_info_webform";i:0;s:12:"default_logo";i:1;s:9:"logo_path";s:0:"";s:11:"logo_upload";s:0:"";s:15:"default_favicon";i:1;s:12:"favicon_path";s:0:"";s:14:"favicon_upload";s:0:"";}');

  // Image processing and management
  variable_set('imce_roles_profiles', array('3' => array('weight' => '-10', 'pid' => '2'), '4' => array('weight' => '-5', 'pid' => '2'), '5' => array('weight' => '0', 'pid' => '2'), '2' => array('weight' => 11, 'pid' => '2'), '1' => array('weight' => 12, 'pid' => '0')));
  variable_set('imce_settings_textarea', '');
  variable_set('imce_settings_replace', 0);
  variable_set('imce_settings_thumb_method', 'scale_and_crop');
  variable_set('image_jpeg_quality', 95);
  variable_set('image_toolkit', 'gd');
  variable_set('imageapi_image_toolkit', 'imageapi_gd');
  
  // Language stuff
  variable_set('language_negotiation', 1);
  variable_set('i18n_selection_mode', 'simple');
  variable_set('i18n_hide_translation_links', 1);
  variable_set('i18n_translation_switch', 0);
  $language_default->language ='he'; 
  $language_default->name = 'Hebrew';
  $language_default->native = 'עברית';
  $language_default->direction = '1';
  $language_default->enabled = 1;
  $language_default->plurals = '2';
  $language_default->formula = '$n!=1';
  $language_default->prefix = 'he';
  $language_default->weight = '0';
  variable_set('language_default', $language_default);
  variable_set('language_count', 2);
  // Captcha
  // --> NEED TO GET KEY FOR reCAPTCHA!!
  variable_set('captcha_log_wrong_responses', 0);
  variable_set('captcha_persistence', 3);
  variable_set('captcha_default_validation', 1);
  variable_set('captcha_default_challenge', 'captcha/Math');
  db_query("UPDATE {captcha_points} SET type='default' WHERE form_id LIKE 'user_%'");
  // Nodewords
  //variable_set('nodewords_head', 'a:19:{s:9:"canonical";s:9:"canonical";s:9:"copyright";s:9:"copyright";s:11:"description";s:11:"description";s:8:"keywords";s:8:"keywords";s:8:"abstract";i:0;s:21:"bing_webmaster_center";i:0;s:14:"dc.contributor";i:0;s:10:"dc.creator";i:0;s:7:"dc.date";i:0;s:14:"dc.description";i:0;s:12:"dc.publisher";i:0;s:8:"dc.title";i:0;s:22:"google_webmaster_tools";i:0;s:8:"location";i:0;s:10:"pics-label";i:0;s:13:"revisit-after";i:0;s:6:"robots";i:0;s:8:"shorturl";i:0;s:19:"yahoo_site_explorer";i:0;}');
  //variable_set('nodewords_edit', 'a:19:{s:11:"description";s:11:"description";s:8:"keywords";s:8:"keywords";s:8:"abstract";i:0;s:21:"bing_webmaster_center";i:0;s:9:"canonical";i:0;s:9:"copyright";i:0;s:14:"dc.contributor";i:0;s:10:"dc.creator";i:0;s:7:"dc.date";i:0;s:14:"dc.description";i:0;s:12:"dc.publisher";i:0;s:8:"dc.title";i:0;s:22:"google_webmaster_tools";i:0;s:8:"location";i:0;s:10:"pics-label";i:0;s:13:"revisit-after";i:0;s:6:"robots";i:0;s:8:"shorturl";i:0;s:19:"yahoo_site_explorer";i:0;}');
  variable_set('nodewords_enable_user_metatags', 0);
  variable_set('nodewords_list_repeat', 0);
  variable_set('nodewords_use_frontpage_tags', 0);
  variable_set('nodewords_max_size', 250);
  //variable_set('nodewords_list_robots', 'a:6:{s:7:"noindex";s:7:"noindex";s:9:"noarchive";i:0;s:8:"nofollow";i:0;s:5:"noodp";i:0;s:9:"nosnippet";i:0;s:6:"noydir";i:0;}');

  //print
  variable_set('print_html_link_teaser', 0);
  variable_set('print_html_show_link', '1');
  variable_set('print_html_link_use_alias', 0);
  variable_set('print_html_link_class', "print-page");
  variable_set('print_html_node_link_visibility', "0");
  variable_set('print_html_node_link_pages', "");
  variable_set('print_html_sys_link_visibility', "1");
  variable_set('print_html_sys_link_pages', "");
  variable_set('print_html_book_link', "1");
  variable_set('print_html_new_window', 0);
  variable_set('print_html_sendtoprinter', 0);
  variable_set('print_html_windowclose', 1);
  
  
  variable_set('print_html_book_link', "1");
  variable_set('print_html_link_pos', 0);
  variable_set('print_html_link_teaser', 0);
  variable_set('print_html_link_use_alias', 0);
  variable_set('print_mail_book_link', "1");
  variable_set('print_mail_hourly_threshold', "3");
  variable_set('print_mail_link_class', "print-mail");
  variable_set('print_mail_link_pos', 0);
  variable_set('print_mail_link_teaser', 0);
  variable_set('print_mail_link_use_alias', 0);
  variable_set('print_mail_node_link_pages', "");
  variable_set('print_mail_node_link_visibility', "0");
  variable_set('print_mail_send_option_default', "sendpage");
  variable_set('print_mail_show_link', "1");
  variable_set('print_mail_sys_link_pages', "");
  variable_set('print_mail_sys_link_visibility', "1");
  variable_set('print_mail_teaser_choice', 1);
  variable_set('print_mail_teaser_default', 1);
  variable_set('print_robots_noarchive', 0);
  variable_set('print_robots_nofollow', 1);
  variable_set('print_robots_noindex', 1);
  
    drupal_cron_run();
}
function cpox1_install() {
  _autolocale_install_po_files();

}
