<?php
// $Id: uc_affiliate2.ca.inc,v 1.1.2.1 2010/02/15 02:55:14 univate Exp $

/**
 * @file
 * This file contains the Conditional Actions hooks and functions necessary to make the
 * affiliate-related entity, conditions, events, and actions work.
 */

/******************************************************************************
 * Conditional Actions Hooks                                                  *
 ******************************************************************************/

/**
 * Implementation of hook_ca_entity().
 *
 * An entity is defined in order to get the recurring fee down to token in the
 * email.
 */
function uc_affiliate2_ca_entity() {
  // CA entity for a affiliate commission.
  $entities['uc_affiliate2_commission'] = array(
    '#title' => t('Affiliate Commission'),
    '#type' => 'object',
  );

  return $entities;
}

/**
 * Implementation of hook_ca_predicate().
 */
function uc_affiliate2_ca_predicate() {
  $predicates = array();

  $predicates['uc_affiliate2_apply_commission'] = array(
    '#title' => t('Send email to affiliate on new commission.'),
    '#trigger' => 'uc_affiliate2_apply_commission',
    '#class' => 'notification',
    '#status' => 0,
    '#weight' => 2,
    '#conditions' => array(
      '#operator' => 'AND',
      '#conditions' => array(
        array(
          '#name' => 'uc_affiliate2_commission_amount',
          '#title' => t('If the commission amount is greater then zero.'),
          '#argument_map' => array(
            'affiliate_commission' => 'affiliate_commission',
          ),
          '#settings' => array(
            'affiliate_commission_value' => 0,
            'affiliate_commission_comparision' => 'greater',
          ),
        ),
      ),
    ),
    '#actions' => array(
      array(
        '#name' => 'uc_affiliate2_send_email',
        '#title' => t('Send affiliate commission email.'),
        '#argument_map' => array(
          'order' => 'order',
          'affiliate_commission' => 'affiliate_commission',
        ),
        '#settings' => array(
          'from' => uc_store_email_from(),
          'addresses' => '[affiliate-email]',
          'subject' => uc_get_message('uc_affiliate2_commission_subject'),
          'message' => uc_get_message('uc_affiliate2_commission_message'),
          'format' => 1,
        ),
      )
    ),
  );

  return $predicates;
}

/**
 * Implementation of hook_ca_condition().
 */
function uc_affiliate2_ca_condition() {
  $conditions['uc_affiliate2_has_affiliate'] = array(
    '#title' => t('User has affiliate'),
    '#description' => t('Check if the user making the purchase has an affiliate.'),
    '#category' => t('Order: Affiliate2'),
    '#callback' => 'uc_affiliate2_condition_has_affiliate',
    '#arguments' => array(
      'order' => array('#entity' => 'uc_order', '#title' => t('Order')),
    ),
  );
  $conditions['uc_affiliate2_commission_amount'] = array(
    '#title' => t('Check commission amount'),
    '#description' => t('Check if the commission amount is within the parameters below.'),
    '#category' => t('Order: Affiliate2'),
    '#callback' => 'uc_affiliate2_condition_commission_amount',
    '#arguments' => array(
      'affiliate_commission' => array('#entity' => 'uc_affiliate2_commission', '#title' => t('Affiliate Commission')),
    ),
  );

  $conditions['uc_affiliate2_affiliate_level'] = array(
    '#title' => t('Check affiliate level'),
    '#description' => t('Returns TRUE if the current affiliate level is within the parameters below.'),
    '#category' => t('Order: Affiliate2'),
    '#callback' => 'uc_affiliate2_condition_affiliate_level',
    '#arguments' => array(
      'affiliate_commission' => array('#entity' => 'uc_affiliate2_commission', '#title' => t('Affiliate Commission')),
    ),
  );

  return $conditions;
}

/**
 * Implementation of hook_ca_action().
 */
function uc_affiliate2_ca_action() {
  // Send an email to an order with a role expiration
  $actions['uc_affiliate2_set_commission'] = array(
    '#title' => t('Override the commission percentage.'),
    '#category' => t('Affiliate'),
    '#callback' => 'uc_affiliate2_set_commission',
    '#arguments' => array(
      'order' => array(
        '#entity' => 'uc_order',
        '#title' => t('Order'),
      ),
      'affiliate_commission' => array(
        '#entity' => 'uc_affiliate2_commission',
        '#title' => t('Affiliate Commission'),
      ),
    ),
  );
  $actions['uc_affiliate2_send_email'] = array(
    '#title' => t('Send an affiliate email.'),
    '#category' => t('Notification'),
    '#callback' => 'uc_affiliate2_action_commission_email',
    '#arguments' => array(
      'order' => array(
        '#entity' => 'uc_order',
        '#title' => t('Order'),
      ),
      'affiliate_commission' => array(
        '#entity' => 'uc_affiliate2_commission',
        '#title' => t('Affiliate Commission'),
      ),
    ),
  );


  return $actions;
}

/**
 * Implementation of hook_ca_trigger().
 */
function uc_affiliate2_ca_trigger() {
  $order = array(
    '#entity' => 'uc_order',
    '#title' => t('Order'),
  );
  $affiliate_commission = array(
    '#entity' => 'uc_affiliate2_commission',
    '#title' => t('Affiliate commission'),
  );

  $triggers['uc_affiliate2_apply_commission'] = array(
    '#title' => t('Affiliate commission being applied'),
    '#category' => t('Affiliate2'),
    '#arguments' => array(
      'order' => $order,
      'affiliate_commission' => $affiliate_commission,
    ),
  );
  return $triggers;
}

/**
 * Check if the order contains a renewal product.
 *
 * @param $order
 *   The order object.
 * @param $recurring_fee
 *   The recurring fee object.
 * @param $settings
 *   The order settings.
 */
function uc_affiliate2_condition_has_affiliate($order, $settings) {
  // check session variable and the database for affiliate for the user making
  // the purchase.
  if ($_SESSION['affiliate'] > 0) {
    return TRUE;
  }
  if (_uc_affiliate2_get_user($uid) > 0) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Check the affiliate level.
 *
 * @see uc_affiliate2_condition_affiliate_level_form()
 */
function uc_affiliate2_condition_affiliate_level($affiliate_commision, $settings) {
  switch ($settings['affiliate_level_comparison']) {
    case 'less':
      return $affiliate_commission->level < $settings['affiliate_level_value'];
    case 'less_equal':
      return $affiliate_commission->level <= $settings['affiliate_level_value'];
    case 'equal':
      return $affiliate_commission->level == $settings['affiliate_level_value'];
    case 'greater_equal':
      return $affiliate_commission->level >= $settings['affiliate_level_value'];
    case 'greater':
      return $affiliate_commission->level > $settings['affiliate_level_value'];
  }
}

/**
 * @see uc_affiliate2_condition_affiliate_level()
 */
function uc_affiliate2_condition_affiliate_level_form($form_state, $settings = array()) {
  $form['affiliate_level_value'] = array(
    '#type' => 'textfield',
    '#title' => t('Affiliate level value'),
    '#description' => t('Specify a value to compare the affiliate level against.'),
    '#default_value' => $settings['affiliate_level_value'],
    '#size' => 16,
  );

  $options = array(
    'less' => t('Total is less than specified value.'),
    'less_equal' => t('Total is less than or equal to specified value.'),
    'equal' => t('Total is equal to specified value.'),
    'greater_equal' => t('Total is greater than or equal to specified value.'),
    'greater' => t('Total is greater than specified value.'),
  );
  $form['affiliate_level_comparison'] = array(
    '#type' => 'radios',
    '#title' => t('Affiliate level comparison type'),
    '#options' => $options,
    '#default_value' => isset($settings['affiliate_level_comparison']) ? $settings['affiliate_level_comparison'] : 'less_equal',
  );

  return $form;
}

/**
 * Check the affiliate commission amount.
 *
 * @see uc_affiliate2_condition_commission_amount_form()
 */
function uc_affiliate2_condition_commission_amount($affiliate_commision, $settings) {
  switch ($settings['affiliate_commission_comparison']) {
    case 'less':
      return $affiliate_commission->commission < $settings['affiliate_commission_value'];
    case 'less_equal':
      return $affiliate_commission->commission <= $settings['affiliate_commission_value'];
    case 'equal':
      return $affiliate_commission->commission == $settings['affiliate_commission_value'];
    case 'greater_equal':
      return $affiliate_commission->commission >= $settings['affiliate_commission_value'];
    case 'greater':
      return $affiliate_commission->commission > $settings['affiliate_commission_value'];
  }
}

/**
 * @see uc_affiliate2_condition_commission_amount_form()
 */
function uc_affiliate2_condition_commission_amount_form($form_state, $settings = array()) {
  $form['affiliate_commission_value'] = array(
    '#type' => 'textfield',
    '#title' => t('Affiliate commission value'),
    '#description' => t('Specify a value to compare the affiliate commission amount against.'),
    '#default_value' => $settings['affiliate_commission_value'],
    '#size' => 16,
  );

  $options = array(
    'less' => t('Total is less than specified value.'),
    'less_equal' => t('Total is less than or equal to specified value.'),
    'equal' => t('Total is equal to specified value.'),
    'greater_equal' => t('Total is greater than or equal to specified value.'),
    'greater' => t('Total is greater than specified value.'),
  );
  $form['affiliate_commission_comparison'] = array(
    '#type' => 'radios',
    '#title' => t('Affiliate commission comparison type'),
    '#options' => $options,
    '#default_value' => isset($settings['affiliate_commission_comparison']) ? $settings['affiliate_commission_comparison'] : 'greater',
  );

  return $form;
}
/**
 *
 * @see uc_affiliate2_set_commission_form()
 */
function uc_affiliate2_set_commission($order, &$affiliate_commission, $settings) {
  $affiliate_commission->commission = $affiliate_commission->data['total'] * $settings['affiliate_commission_pct'] / 100;
}

/**
 *
 * @see uc_affiliate2_set_commission()
 */
function uc_affiliate2_set_commission_form($form_state, $settings = array()) {
  $form['affiliate_commission_pct'] = array(
    '#type' => 'textfield',
    '#title' => t('Affiliate Commission Percent'),
    '#description' => t('Enter a value to override the default commission percentage.'),
    '#default_value' => $settings['affiliate_commission_pct']
  );

  return $form;
}

/**
 * Send an email with order and affiliate commission replacement tokens.
 *
 * The recipients, subject, and message fields take order token replacements.
 *
 * @see uc_affiliate2_action_commission_email_form()
 */
function uc_affiliate2_action_commission_email($order, $affiliate_commission, $settings) {
  $account = uc_order_user_load($order);

  // Token replacements for the subject and body
  $settings['replacements'] = array(
    'global' => NULL,
    'order' => $order,
    'user' => $account,
    'affiliate' => $affiliate_commission,
  );

  // Replace tokens and parse recipients.
  $recipients = array();
  $addresses = token_replace_multiple($settings['addresses'], $settings['replacements']);
  foreach (explode("\n", $addresses) as $address) {
    $recipients[] = trim($address);
  }

  // Send to each recipient.
  foreach ($recipients as $email) {
    $sent = drupal_mail('uc_affiliate2', 'action-mail', $email, uc_store_mail_recipient_language($email), $settings, $settings['from']);

    if (!$sent['result']) {
      watchdog('ca', 'Attempt to e-mail @email concerning order @order_id failed.', array('@email' => $email, '@order_id' => $order->order_id), WATCHDOG_ERROR);
    }
  }
}

/**
 * Email settings form.
 *
 * @see uc_affiliate2_action_commission_email()
 */
function uc_affiliate2_action_commission_email_form($form_state, $settings = array()) {
  // ca_build_email_form() function seems to be incorrectly part of uc_roles so
  // we add this hack to include the uc_roles.ca.inc here.
  module_load_include('inc', 'uc_roles', 'uc_roles.ca');
  return ca_build_email_form($form_state, $settings, array('global', 'order', 'user', 'affiliate'));
}
