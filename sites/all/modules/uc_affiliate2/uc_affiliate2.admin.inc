<?php
// $Id: uc_affiliate2.admin.inc,v 1.1.2.19 2010/05/13 18:57:03 bojanz Exp $


/**
 * @file
 * Admin functions for uc_affiliate2 module.
 *
 */

/**
 * The ensure the date and interval used in the reports remain consistant
 * across all pages we define them here.
 */
define('UC_AFFILIATE2_REPORT_DATE_FORMAT', 'M-Y');
define('UC_AFFILIATE2_REPORT_DATE_INTERVAL', '1 month');

/**
 * Define the affiliate users listing
 */
function uc_affiliate2_admin_manage() {
  $user_id = arg(3);
  if ($user_id && is_numeric($user_id)) {
    return uc_affiliate2_admin_users($user_id);
  }

  $clicks = array();
  $users = array();
  $orders = array();

  $result = db_query("SELECT aid, COUNT(click_id) as clicks FROM {uc_affiliate2_clicks} GROUP BY aid");

  while($count = db_fetch_array($result)) {
    $clicks[$count['aid']] = $count['clicks'];
  }

  $result = db_query("SELECT aid, COUNT(aid) as users FROM {uc_affiliate2_users} GROUP BY aid");

  while($count = db_fetch_array($result)) {
    $users[$count['aid']] = $count['users'];
  }

  $result = db_query("SELECT aid, COUNT(aid) as orders FROM {uc_affiliate2_orders} GROUP BY aid");

  while($count = db_fetch_array($result)) {
    $orders[$count['aid']] = $count['orders'];
  }

  // setup the table
  $header = array(
    array('data' => t('Username'), 'field' => 'u.name'),
    array('data' => t('Status'), 'field' => 'u.status'),
    array('data' => t('Member for'), 'field' => 'u.created', 'sort' => 'desc'),
    array('data' => t('Last access'), 'field' => 'u.access'),
    array('data' => t('Unique Clicks')),
    array('data' => t('Users')),
    array('data' => t('Orders')),
  );

  // define the query
  $sql = "SELECT u.uid, u.name, u.status, u.created, u.access
          FROM {users} u
            INNER JOIN {users_roles} ur ON u.uid = ur.uid
          WHERE ur.rid IN (SELECT r.rid
                           FROM {role} r
                             INNER JOIN {permission} p ON r.rid = p.rid
                           WHERE p.perm LIKE '%act as affiliate%')
          GROUP BY u.uid, u.name, u.status, u.created, u.access";

  $sql .= tablesort_sql($header);
  $result = db_query($sql);

  $status = array(t('blocked'), t('active'));

  while ($account = db_fetch_object($result)) {
    $rows[] = array(
      l($account->name, 'admin/store/affiliate/'. $account->uid),
      $status[$account->status],
      format_interval(time() - $account->created),
      $account->access ? t('@time ago', array('@time' => format_interval(time() - $account->access))) : t('never'),
      !empty($clicks[$account->uid]) ? $clicks[$account->uid] : '0',
      !empty($users[$account->uid]) ? $users[$account->uid] : '0',
      !empty($orders[$account->uid]) ? $orders[$account->uid] : '0',
    );
  }

  $output = "<p>". t("Click on an affiliate user to view their monthly sales and commissions.");
  $output .= theme('table', $header, $rows);
  $output .= theme('pager', NULL, 50, 0);

  return $output;
}

function uc_affiliate2_admin_users($uid) {
  // used to determine whether to show the paid form.
  $month_timestamp = arg(4);

  $account = user_load(array('uid' => $uid));

  // setup the table
  $header = array(
    array('data' => t('Month'), 'field' => 'date'),
    array('data' => t('Sales'), 'field' => 'sales'),
    array('data' => t('Commissions'), 'field' => 'commissions'),
    array('data' => t('Paid (click to edit)'), 'field' => 'paid'),
  );

  // define the query
  $sql = 'SELECT ac.commission, uo.order_total, uo.created
          FROM {uc_affiliate2_commission} AS ac
            LEFT OUTER JOIN {uc_orders} AS uo ON ac.order_id = uo.order_id
          WHERE ac.aid = %d';
  $result = db_query($sql, $uid);

  $total_sales = array();
  $total_commissions = array();
  $amounts = _uc_affiliate2_get_amounts('aid', $uid, 'date');

  while ($rec = db_fetch_array($result)) {
    $mondate = date(UC_AFFILIATE2_REPORT_DATE_FORMAT, $rec['created']);
    $total_sales[$mondate] = isset($total_sales[$mondate]) ? $total_sales[$mondate]+$rec['order_total'] : $rec['order_total'];
    $total_commissions[$mondate] = isset($total_commissions[$mondate]) ? $total_commissions[$mondate] + $rec['commission'] : $rec['commission'];
  }

  foreach ($total_sales as $date => $sales) {
    $timestamp = strtotime($date);
    $amount = ($amounts[$timestamp]) ? $amounts[$timestamp] : 0;

    // format paid field as link or form
    if ($month_timestamp && $date == $month_timestamp) {
      $amount = uc_currency_format($amount, false);
      $paid = drupal_get_form('uc_affiliate2_admin_users_paid_form', $amount, $uid, strtotime($date));
    }
    else {
      $amount = uc_currency_format($amount);
      $paid = l($amount, join('/', array(arg(0), arg(1), arg(2), arg(3), $date)));
    }

    // generate record
    $rows[] = array(
      str_replace('-', ' ', $date),
      uc_currency_format($sales),
      uc_currency_format($total_commissions[$date]),
      $paid
    );
  }

  $output = "<p>". t("Displaying affiliate sales and commissions for ") . theme('username', $account) .".";

  $output .= theme('table', $header, $rows);

  return $output;
}

function uc_affiliate2_admin_users_paid_form($form_state, $amount, $uid, $date) {
  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Amount paid'),
    '#default_value' => $amount,
    '#size' => 8,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $uid
  );
  $form['date'] = array(
    '#type' => 'value',
    '#value' => $date
  );

  $form['#redirect'] = 'admin/store/affiliate/reports/'. date(UC_AFFILIATE2_REPORT_DATE_FORMAT, $date);

  return $form;
}

function uc_affiliate2_admin_users_paid_form_validate($form, &$form_state) {
  $amount = $form_state['values']['amount'];

  if (!is_numeric($amount) || $amount < 0) {
    form_set_error('amount', t('Please specify an amount paid to this affiliate.'));
  }
}

function uc_affiliate2_admin_users_paid_form_submit($form, &$form_state) {
  $sign = variable_get('uc_currency_sign', '$');
  $amount = $form_state['values']['amount'];
  $uid = $form_state['values']['uid'];
  $date = $form_state['values']['date'];

  $rec = db_result(db_query('SELECT amount FROM {uc_affiliate2_paid} WHERE aid=%d AND date=%d', $uid, $date));
  if ($rec===FALSE) {
    db_query('INSERT INTO {uc_affiliate2_paid} (aid, date, amount) VALUES (%d, %d, %f)', $uid, $date, $amount);
  }
  else {
    db_query('UPDATE {uc_affiliate2_paid} SET amount=%f WHERE aid=%d AND date=%d', $amount, $uid, $date);
  }

  $account = user_load(array('uid' => $uid));
  drupal_set_message(t('Registered payment of @amount for !user during @date.', array('@amount' => uc_currency_format($amount), '!user' => theme('username', $account), '@date' => format_date($date, 'custom', 'M Y'))));

  return 'admin/store/affiliate/'. $uid;
}

/**
 * Define the banner/link settings form
 */
function uc_affiliate2_banner_settings() {
  $form['affiliate_banner_template'] = array(
    '#type' => 'textarea',
    '#title' => t('Affiliate Banner Templates'),
    '#description' => t('The IMG tag(s) around which the url of the affiliate is wrapped around. Insert as many as you want.'),
    '#default_value' => variable_get('affiliate_banner_template', ''),
    '#wysiwyg' => FALSE,
  );
  $form['affiliate_text_link_template'] = array(
    '#type' => 'textfield',
    '#title' => t('Affiliate Text Link Template'),
    '#description' => t('The text around which the url of the affiliate is wrapped around. HTML is allowed.'),
    '#default_value' => variable_get('affiliate_text_link_template', ''),
  );
  $form['affiliate_product_html_template'] = array(
    '#type' => 'textarea',
    '#title' => t('Affiliate Product HTML Template (used for Export)'),
    '#description' => t('When using the html export function in the affiliate panel, this template is used for each product.
                        Allowed placeholders: [LINK], [TITLE], [IMG], [DESCRIPTION], [PRICE].'),
    '#default_value' => variable_get('affiliate_product_html_template', '<div id="prodcontain"><a href="[LINK]"><img src="[IMG]"></a><br /><div id="description">description:</div>[DESCRIPTION]<br /><div id="price">[PRICE]</div></div>'),
    '#wysiwyg' => FALSE,
  );

  return system_settings_form($form);
}

/**
 * Define the commission settings form
 */
function uc_affiliate2_commission_settings() {
  $form['affiliate_hierarchy_depth'] = array(
    '#type' => 'textfield',
    '#title' => t('Affiliate hierarchy depth'),
    '#description' => t('Controls the depth to which affiliates referrals and commissions are tracked'),
    '#default_value' => variable_get('affiliate_hierarchy_depth', 5),
    '#size' => 3
  );
  $form['affiliate_commission_structure'] = array(
    '#type' => 'textfield',
    '#title' => t('Default Affiliate Commission Structure'),
    '#description' => t("Sets the default commission structure.  This should be a comma separated list of number with the first value corresponding to the first level in the hierarchy and each successive value corresponding to the next level in the hierarchy. Commission can either be represented as percentages by adding a '%' symbol after the number or as a fixed amount in the stores currency by a number with no symbol. Example: 5%,4%,2%,2%,1%"),
    '#default_value' => implode(',', variable_get('affiliate_commission_structure', array('5%','3%','2%','2%','1%'))),
    '#size' => 40
  );

  $roles = user_roles(FALSE, 'act as affiliate');

  foreach ($roles as $rid => $role) {
    $form['affiliate_settings_role_'. $rid] = array(
      '#type' => 'fieldset',
      '#title' => t('Settings for @role', array('@role' => $role)),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['affiliate_settings_role_'. $rid]['affiliate_commission_structure_'. $rid] = array(
      '#type' => 'textfield',
      '#title' => t('Affiliate Commission Structure for cookied role: @role', array('@role' => $role)),
      '#description' => t("Sets the commission structure for the role, when the user has returned via cookie. This should be a comma separated list of number with the first value corresponding to the first level in the hierarchy and each successive value corresponding to the next level in the hierarchy. Commission can either be represented as percentages by adding a '%' symbol after the number or as a fixed amount in the stores currency by a number with no symbol. Example: 5%,4%,2%,2%,1%"),
      '#default_value' => implode(',', variable_get('affiliate_commission_structure_'. $rid, array('5%','3%','2%','2%','1%'))),
      '#size' => 40
    );
  }

  return system_settings_form($form);
}

/**
 * Validate function for commission settings form.
 */
function uc_affiliate2_commission_settings_validate($form, &$form_state) {
  $roles = user_roles(FALSE, 'act as affiliate');
  foreach ($roles as $rid => $role) {
    $form_state['values']['affiliate_commission_structure_'. $rid] = explode(',', $form_state['values']['affiliate_commission_structure_'. $rid]);
  }
  $form_state['values']['affiliate_commission_structure'] = explode(',', $form_state['values']['affiliate_commission_structure']);
}

/**
 * Define the affiliate admin settings form
 */
function uc_affiliate2_admin_settings() {
  $form['affiliate_enable_upline'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable the "Upline" page for affiliates.'),
    '#description' => t('This page allows the affiliate to see the affiliates above him (if any) in a multilevel setup.'),
    '#default_value' => variable_get('affiliate_enable_upline', 0),
  );
    
  foreach (uc_order_status_list('general') as $status) {
    $statuses[$status['id']] = $status['title'];
  }
  $form['affiliate_commission_order_statuses'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Order status'),
    '#default_value' => variable_get('affiliate_commission_order_statuses', array('completed' => 'completed')),
    '#description' => t('Where in the order status(es) will the commissions be assigned.'),
    '#options' => $statuses,
  );
  $form['affiliate_invalid_redirect'] = array(
    '#type' => 'textfield',
    '#title' => t('On an invalid affiliate URL, redirect to'),
    '#description' => t('When an invalid username is used as an affiliate, the user will be redirected to this URL.'),
    '#default_value' => variable_get('affiliate_invalid_redirect', '')
  );

  // Only display this setting if Ubercart Payment is enabled. This prevents a WSOD.
  if (function_exists('_payment_method_list')) {
    foreach (_payment_method_list() as $method) {
      $payment_methods[$method['id']] = $method['name'];
    }
  
    $form['affiliate_no_commission_payment_methods'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Disable commission for these Payment methods'),
      '#default_value' => variable_get('affiliate_no_commission_payment_methods', array()),
      '#description' => t('You can disable commission for certain Payment methods'),
      '#options' => $payment_methods,
    );
  }

  $ip_masks = array(
    'hide' => t("Hide IP addresses from users"),
    'one' => t("Show the last number (*.*.*.123)"),
    'two' => t("Show the last 2 numbers (*.*.123.123)"),
    'first' => t("Show the first number (123.*.*.*)"),
    'full' => t("Show the full IP (primarily for debug purposes)"),
  );
  $form['affiliate_ip_masking'] = array(
    '#type' => 'radios',
    '#title' => t('Mask IP addresses that your affiliates can see'),
    '#default_value' => variable_get('affiliate_ip_masking', 'hide'),
    '#description' => t('To help affiliates determine who bought what, you can show them the IP address of your customers. This can also be a privacy issue. In the US, do not show more than 2 numbers. Choose wisely. The administrator (user 1) is not affected by this option.'),
    '#options' => $ip_masks,
  );
  
  $form['affiliate_cookie_tracking'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cookie tracking'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  
  $form['affiliate_cookie_tracking']['affiliate_set_cookie'] = array(
    '#type' => 'checkbox',
    '#title' => t('Set the affiliate cookie for the visiting user.'),
    '#description' => t('By setting the cookie, the affiliate gets credited even when no affiliate id is present in links visited.'),
    '#default_value' => variable_get('affiliate_set_cookie', 0),
  );
  $form['affiliate_cookie_tracking']['affiliate_cookie_lifetime'] = array(
    '#type' => 'textfield',
    '#title' => t('Cookie Lifetime'),
    '#description' => t('How long should the cookie be valid. Strtotime syntax.'),
    '#default_value' => variable_get('affiliate_cookie_lifetime', '1 year'),
  );
  $form['affiliate_cookie_tracking']['affiliate_subsequent_order_commissions'] = array(
    '#type' => 'radios',
    '#title' => t('Affiliate commissions for future orders placed by users they referred'),
    '#description' => t('If an affiliate cookie is present when a user creates an account, that user is associated with the affiliate. Using this option, you can control whether the affiliate associated with the user gets commissions, or whether any affiliate whose cookies may be present when the user places an order should get the commissions instead.'),
    '#options' => array(
      'none' => t('Only use affiliate cookies that are available at the time of order placement'),
      'enable' => t('Use affiliate cookies at the time of order placement, but if there are none, fall back on the affiliate that first referred the user'),
      'override' => t('Give commissions to the affiliate associated with the user, but if there is none, fall back on any affiliate cookies that may be present'),
      'exclusive' => t('Only give commissions to an affiliate that is associated with the user'),
    ),
    '#default_value' => variable_get('affiliate_subsequent_order_commissions', 'none'),
  );

  return system_settings_form($form);
}

function uc_affiliate2_admin_settings_validate($form, &$form_state) {
  $url = $form_state['values']['affiliate_invalid_redirect'];

  if (!empty($url) && ($url != check_url($url) || strpos($url, '://') !== FALSE)) {
    form_set_error('affiliate_invalid_redirect', t('Please enter a valid Drupal path, without the http and domain parts.'));
  }

  if (!strtotime($form_state['values']['affiliate_cookie_lifetime'])) {
    form_set_error('affiliate_cookie_lifetime', t('Invalid strtotime syntax.'));
  }
}

/**
 * Build a monthly report.
 */
function uc_affiliate2_monthly_report() {
   // The month we are viewing. If this is empty, the current month is assumed.
  $month = arg(4);

  $date_start = strtotime($month);
  if ($date_start === FALSE || $date_start == -1) { // PHP > 5.1.0 returns FALSE instead of -1
    $month = date(UC_AFFILIATE2_REPORT_DATE_FORMAT);
    $date_start = strtotime($month);
  }
  $date_end = strtotime('+'. UC_AFFILIATE2_REPORT_DATE_INTERVAL, $date_start);

  // Recreate the date to ensure its in the correct format.
  $month = date(UC_AFFILIATE2_REPORT_DATE_FORMAT, $date_start);

   // Used to determine whether to show the paid form
  $uid_external = arg(5);

  // setup the table
  $header = array(
    array('data' => t('Affiliate'), 'field' => 'aid'),
    array('data' => t('Sales'), 'field' => 'sales'),
    array('data' => t('Commissions'), 'field' => 'commissions'),
    array('data' => t('Paid (click to edit)'), 'field' => 'paid'),
  );

  // define the query
  $sql = 'SELECT ac.aid, ac.commission, uo.order_total FROM {uc_affiliate2_commission} AS ac LEFT OUTER JOIN {uc_orders} AS uo ON ac.order_id = uo.order_id WHERE uo.created BETWEEN %d AND %d';
  $result = db_query($sql, $date_start, $date_end);

  $total_sales = array();
  $total_commissions = array();
  $amounts = _uc_affiliate2_get_amounts('date', $date_start, 'aid');

  while ($rec = db_fetch_array($result)) {
    $aid = $rec['aid'];
    $total_sales[$aid] = isset($total_sales[$aid]) ? $total_sales[$aid] + $rec['order_total'] : $rec['order_total'];
    $total_commissions[$aid] = isset($total_commissions[$aid]) ? $total_commissions[$aid] + $rec['commission'] : $rec['commission'];
  }

  $user_results = db_query('SELECT name, uid FROM {users}');
  $users = array();

  while ($user = db_fetch_array($user_results)) {
    $users[$user['uid']] = $user['name'];
  }

  foreach ($total_sales as $uid => $sales) {
    $amount = ($amounts[$uid]) ? $amounts[$uid] : 0;
    $amount = uc_currency_format($amount);

    // format paid field as link or form
    if ($uid_external && is_numeric($uid_external) && $uid == $uid_external) {
      $paid = drupal_get_form('uc_affiliate2_admin_users_paid_form', $amount, $uid, $date_start);
    }
    else {
      $paid = l($amount, join('/', array(arg(0), arg(1), arg(2), arg(3), $month, $uid)));
    }

    $rows[] = array(
      $users[$uid],
      uc_currency_format($sales),
      uc_currency_format($total_commissions[$uid]),
      $paid
    );
  }

  $output = "<p>". t("Displaying affiliate sales and commissions for ") . str_replace('-', ' ', $month) .". ";
  $output .= t("Show ") . l(t('previous month'), join('/', array(arg(0), arg(1), arg(2), arg(3), date(UC_AFFILIATE2_REPORT_DATE_FORMAT, strtotime('-'. UC_AFFILIATE2_REPORT_DATE_INTERVAL, $date_start)))));
  $output .= t(" or ") . l(t('next month'), join('/', array(arg(0), arg(1), arg(2), arg(3), date(UC_AFFILIATE2_REPORT_DATE_FORMAT, $date_end)))) .'.</p>';

  $output .= theme('table', $header, $rows);

  return $output;
}

/**
 *
 */
function uc_affiliate2_order_commissions($order_id) {
  $result = db_query('SELECT ac.commission_id, ac.aid, u.name, ac.commission, ac.commission_notes
                      FROM {uc_affiliate2_commission} ac
                        LEFT JOIN {users} as u ON u.uid=ac.aid
                      WHERE ac.order_id=%d', $order_id);

  $context = array(
    'revision' => 'themed',
    'type' => 'product',
  );

  $rows = array();
  while ($commission = db_fetch_object($result)) {
    $rows[] = array(
      l($commission->name, 'user/'. $commission->aid),
      uc_price($commission->commission, $context),
      $commission->commission_notes,
      l('edit', 'admin/store/orders/'. $order_id .'/affiliates/'. $commission->commission_id .'/edit', array('attributes' => array('class' => 'edit'))) .' | '.
      l('remove', 'admin/store/orders/'. $order_id .'/affiliates/'. $commission->commission_id .'/remove')
    );
  }
  return theme('table', array('Affiliate', 'Commission', 'Notes', ''), $rows);
}

/**
 * Manually edit a commission on an order.
 */
function uc_affiliate2_order_edit_commissions($form_state, $order_id, $commission_id) {
  $commission = db_fetch_object(db_query("SELECT commission_id, aid, commission, commission_notes FROM {uc_affiliate2_commission} WHERE commission_id=%d", $commission_id));

  $form['commission_id'] = array(
    '#type' => 'hidden',
    '#value' => $commission->commission_id
  );
  $form['order_id'] = array(
    '#type' => 'hidden',
    '#value' => $order_id
  );
  $form['commission'] = array(
    '#type' => 'textfield',
    '#title' => t('Commission Amount'),
    '#default_value' => $commission->commission,
  );
  $form['commission_notes'] = array(
    '#type' => 'textfield',
    '#title' => t('Commission Note'),
    '#default_value' => $commission->commission_notes,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update commission'),
    '#suffix' => l(t('Cancel'), uc_referer_uri()),
  );
  return $form;
}

function uc_affiliate2_order_edit_commissions_submit($form, &$form_state) {
  $commission = db_fetch_object(db_query("SELECT * FROM {uc_affiliate2_commission} WHERE commission_id=%d", $form_state['values']['commission_id']));
  $commission->commission = $form_state['values']['commission'];
  $commission->commission_notes = $form_state['values']['commission_notes'];
  uc_affiliate2_commission_save($commission);
  $form_state['redirect'] = 'admin/store/orders/'. $form_state['values']['order_id'] .'/affiliates';
}

/**
 * Delete a commission from an order.
 */
function uc_affiliate2_order_remove_commissions($form_state, $order_id, $commission_id) {
  $form['commission_id'] = array(
    '#type' => 'hidden',
    '#value' => $commission_id
  );
  $form['order_id'] = array(
    '#type' => 'hidden',
    '#value' => $order_id
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Remove commission'),
    '#suffix' => l(t('Cancel'), uc_referer_uri()),
  );


  return $form;
}

function uc_affiliate2_order_remove_commissions_submit($form, &$form_state) {
  db_query('DELETE FROM {uc_affiliate2_commission} WHERE commission_id=%d', $form_state['values']['commission_id']);
  $form_state['redirect'] = 'admin/store/orders/'. $form_state['values']['order_id'] .'/affiliates';
}

/**
 * Form to associate attributes with products or classes.
 */
function uc_affiliate2_product_commission_form($form_state, $node) {
  drupal_add_js('misc/autocomplete.js');
  $form = array(
    '#theme' => 'uc_affiliate2_product_commission_form',
    '#cache' => TRUE,
  );

  $form['message'] = array(
    '#value' => t("This sets an alternative commission structure for this product. When appling commissions the first commission structure below that matches the affiliate will be used. If the commission structure is set to 0, the product won't be present in the generated links, and no commissions will be given for it. Otherwise this should be a comma separated list of number(s) with the first value corresponding to the first level in the hierarchy and each successive value corresponding to the next level in the hierarchy.  Example: 5%,4%,2%,2%,1%"),
  );

  $num_rules = 1 + db_result(db_query("SELECT count(*) FROM {uc_affiliate2_product_commission} WHERE nid = %d", $node->nid));

  $form['product_commissions']['#theme'] = 'uc_affiliate2_product_commission_form_current';
  $form['product_commissions']['#tree'] = TRUE;
  // add new rule form
  $rules = _uc_affiliate2_product_commission_rules();
  $form['product_commissions']['new']['rule'] = array(
    '#type' => 'select',
    '#options' => array('Add commissions for ...') + $rules,
    '#ahah' => array(
      'event' => 'change',
      'path' => 'uc_affiliate2/ahah/product_commissions_match',
      'wrapper' => 'product-commission-match',
      'method' => 'replace',
      'effect' => 'fade',
      'progress' => array(
      ),
    ),
  );
  $form['product_commissions']['new']['match'] = array(
    '#value' => '<div id="product-commission-match"></div>'
  );
  $form['product_commissions']['new']['model'] = array(
    '#type' => 'select',
    '#options' => uc_product_get_models($node),
  );
  $form['product_commissions']['new']['commission_structure'] = array(
    '#type' => 'textfield',
    '#size' => 12,
  );
  $form['product_commissions']['new']['weight'] = array(
    '#type' => 'weight',
    '#delta' => $num_rules,
    '#default_value' => -1 * $num_rules,
  );

  // Get commission structure for this product
  $commission_structure = 0;
  $result = db_query("SELECT * FROM {uc_affiliate2_product_commission} WHERE nid = %d ORDER BY weight", $node->nid);

  while ($product_commission = db_fetch_object($result)) {
    $pcid = $product_commission->pcid;
    $product_commission->commission_structure = unserialize($product_commission->commission_structure);
    $form['product_commissions'][$pcid]['weight'] = array(
      '#type' => 'weight',
      '#delta' => $num_rules,
      '#default_value' => $product_commission->weight
    );
    $form['product_commissions'][$pcid]['rule'] = array(
      '#value' => $rules[$product_commission->rule]
    );
    if (!empty($product_commission->rule_match)) {
      switch($product_commission->rule) {
        case 'user':
          $account = user_load($product_commission->rule_match);
          $form['product_commissions'][$pcid]['match'] = array(
            '#value' => theme('username', $account)
          );
          break;
        case 'role':
          $role = db_result(db_query("SELECT name FROM {role} WHERE rid = %d", $product_commission->rule_match));
          $form['product_commissions'][$pcid]['match'] = array(
            '#value' => $role
          );
          break;
      }
    }
    $form['product_commissions'][$pcid]['commission_structure'] = array(
      '#value' => implode(',', $product_commission->commission_structure)
    );
    $form['product_commissions'][$pcid]['model'] = array(
      '#value' => empty($product_commission->model) ? t('Any') : $product_commission->model
    );
    $form['product_commissions'][$pcid]['delete'] = array(
      '#type' => 'checkbox'
    );
  }

  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

function uc_affiliate2_product_commission_form_submit($form, &$form_state) {
  $nid = $form_state['values']['nid'];

  // save the new record
  if ($form_state['values']['product_commissions']['new']['commission_structure'] != '') {
    $comm = new stdClass();
    $comm->nid = $nid;
    $comm->model = $form_state['values']['product_commissions']['new']['model'];
    $comm->commission_structure = explode(',', $form_state['values']['product_commissions']['new']['commission_structure']);
    $comm->rule = $form_state['values']['product_commissions']['new']['rule'];
    $comm->rule_match = $form_state['values']['match'];
    if ($comm->rule == 'user') {
      $comm->rule_match = db_result(db_query("SELECT uid from {users} where name='%s'", $comm->rule_match));
    }
    $comm->weight = $form_state['values']['product_commissions']['new']['weight'];
    uc_affiliate2_product_commission_save($comm);
  }

  // delete the ones that have been checked.
  foreach ($form_state['values']['product_commissions'] as $pcid => $comm) {
    if ($comm['delete'] == 1) {
      // delete this record
      uc_affiliate2_product_commission_delete($pcid);
    }
    else if (!empty($comm['weight'])) {
      // set the weight
      db_query("UPDATE {uc_affiliate2_product_commission} set weight = %d WHERE pcid = %d", $comm['weight'], $pcid);
    }
  }
}



/**
 *
 */
function uc_affiliate2_ahah($context) {
  $func = '_uc_affiliate2_ahah_'. $context;
  $form_state['values'] = $_POST;
  if (function_exists($func)) {
    $func($form_state);
  }
  exit();
}

/**
 *
 */
function _uc_affiliate2_ahah_product_commissions_match($form_state) {
  switch ($form_state['values']['product_commissions']['new']['rule']) {
    case 'user':
      $form['match'] = array(
        '#type' => 'textfield',
        '#size' => 12,
        '#autocomplete_path' => 'user/autocomplete',
      );
      break;
    case 'role':
      $roles = array('Select role ...') + user_roles(FALSE, 'act as affiliate');
      $form['match'] = array(
        '#type' => 'select',
        '#options' => $roles,
      );
     break;
     default:
       $form['match'] = array('#value' => '');
  }

  $output = _uc_affiliate2_ahah_render($form, 'match');
  print drupal_to_js(array('data' => $output, 'status' => true));
}

/*
 * This function is largely based on the poll module, its been simplified for reuse.
 *   $fields is the specific form elements you want to attach via ahah,
 *   $name is the form fields array key... e.g. the name for $form['title'] is "title"
 */
function _uc_affiliate2_ahah_render($fields, $name) {
  $form_state = array('submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];
  // Add the new element to the stored form. Without adding the element to the
  // form, Drupal is not aware of this new elements existence and will not
  // process it. We retreive the cached form, add the element, and resave.
  $form = form_get_cache($form_build_id, $form_state);
  $form[$name] = $fields;
  form_set_cache($form_build_id, $form, $form_state);
  $form += array(
    '#post' => $_POST,
    '#programmed' => FALSE,
  );
  // Rebuild the form.
  $form = form_builder($_POST['form_id'], $form, $form_state);

  // Render the new output.
  $new_form = $form[$name];
  return drupal_render($new_form);
}


