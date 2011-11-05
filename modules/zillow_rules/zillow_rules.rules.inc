<?php

/**
 * @file
 * Zillow Rules code: actions, conditions and events.
 */

/**
 * Implements hook_rules_action_info().
 *
 * Declares any meta-data about actions for Rules in a big, associative, nested
 * array. See also hook_rules_action_info in the rules.api.php file, and the
 * online documentation at http://drupal.org/node/878928.
 */
function zillow_rules_rules_action_info() {
  $actions = array(
    // The base-level keys in the array are the machine names for the actions,
    // and by default also the function name for the action callback. Prefix
    // with your module name to avoid name conflicts.
    'zillow_rules_action_new_tour' => array(
      'label' => t('Add selected VBO items to Tour'), // Name displayed to admins
      'group' => t('Zillow'), // Used for grouping actions in select lists
    ),
  );

  // Don't forget to return the array of action information!
  return $actions;
}

function zillow_rules_action_new_tour() {
	dpm("Rules action fired");

}

function zillow_rules_rules_event_info() {
  $events = array();

  if (module_exists('views')) {
    // Events are, like the other Rules plugins in this example, keyed by their
    // machine name – by default also the callback function. Prefix with your
    // module name to avoid name conflicts.
    $events['zillow_rules_event_view_render'] = array(
      'label' => t('A view is going to be rendered'),
      'group' => t('Zillow'),
      // If the event provides any parameters to Rules, these are described with
      // the 'variables' property.
      'variables' => array(
        // Each provided variable is keyed by a given machine name, and work
        // very similar to how parameters and provided variables are desribed in
        // conditions and actions.
        'view_name' => array(
          'type' => 'text',
          'label' => t('view name'),
        ),
      ),
    );
  }

  return $events;
}