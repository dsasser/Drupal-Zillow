<?php
/**
* @file
* Administration page callbacks for the zillow_field module.
*/
/**
* Form builder. Configure annotations.
*
* @ingroup forms
* @see system_settings_form().
*/
function zillow_field_admin_settings() {
    // sets the Zillow API key value necessary for this module
	$form['zillow_field_admin_settings'] = array(
		'#name' => 'zillow_api_key',
		'#type' => 'textfield',
		'#title' => t('Zillow API Key.'),
		'#default_value' => variable_get('zillow_field_api_key', array('')),
		'#description' => t('A text field for your Zillow API. 
		<a href="http://www.zillow.com/howto/api/APIOverview.htm">Get one.</a>'),
	);
	$form['#submit'][] = 'zillow_field_admin_settings_submit';
	return system_settings_form($form);
}

function zillow_field_admin_settings_submit($form, $form_state) {
	//dpm($form_state['input']['zillow_api_key']);
	variable_set('zillow_field_api_key', $form_state['input']['zillow_api_key']);
}

function zillow_field_admin_settings_validate($form, $form_state) {
	switch ($form_state['input']['zillow_api_key']) {
	    case '0':
	        form_set_error('zillow_field_admin_settings', 'Enter something more useful');
	        break;
	    case '':
	        form_set_error('zillow_field_admin_settings', 'You should enter something');
	        break;
	} 
}