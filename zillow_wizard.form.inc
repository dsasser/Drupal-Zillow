<?php

//variable_set('zillow_picture_path', 'properties');


/**
 * @file
 * Extensible wizard form example.
 */

/**
 * Extensible wizard form example.
 *
 * This is an example of a multistep form using a wizard style. It will include
 * the 'Previous' and 'Next' buttons when required, and a 'Finish' button at the
 * last stage of the form submission.
 *
 * This example is an extensible skeleton that can include (even
 * programatically) more steps. The demonstration form includes three steps,
 * each step having its own validation functions.
 *
 * How to extend this example:
 * - Steps are defined in the _zillow_searchform_steps() function. Include or alter
 *   the steps as you require.
 * - For each step, implement the corresponding 'form' function (see
 *   'zillow_searchform_wizard_search_info' for the first step in this example.)
 *   Each step is a regular form, and the wizard collects all the values of the
 *   included forms.
 * - Optionally, you may include custom validation functions using the regular
 *   validation hook (formname_validate). The wizard uses these validation
 *   functions for each step.
 * - The most important customization sttep is to change the submit handler and
 *   do whatever you want with the collected information. In this case, the
 *   example just shows the collected values in the various steps.
 * @ingroup zillow_searchform
 */

/**
 * Returns the list of steps and their associated forms. This has been separated
 * to clarify and easy the understanding of this example. You should edit this
 * function to include the steps your wizard/multistep form requires.
 *
 * @return $array
 *
 * @ingroup zillow_searchform
 */
function _zillow_searchform_steps() {
  return array(
      1 => array(
        'form' => 'zillow_searchform_wizard_search_info',
      ),
      2 => array(
        'form' => 'zillow_searchform_wizard_property_info',
      ),
      //3 => array(
      //  'form' => 'zillow_searchform_wizard_other_info',
      //),
      //4 => array(
      //  'form' => 'zillow_searchform_wizard_picture',
      //),
    );
}

/**
 * The primary formbuilder function for the wizard form. This is the form that
 * you should call with drupal_get_form() from your code, and it will include
 * the rest of the step forms defined. You are not required to change this
 * function, as this will handle all the step actions for you.
 *
 * This form has two defined submit handlers to process the different steps:
 *  - Previous: handles the way to get back one step in the wizard.
 *  - Next:     handles each step form submission,
 *
 * The third handler, the finish button handler, is the default form _submit
 * handler used to process the information.
 *
 * You are not required to change the next or previous handlers, but you must
 * change the zillow_searchform_wizard_sbumit handler to perform the operations you
 * need on the collected information.
 *
 * @ingroup zillow_searchform
 */
function zillow_searchform_wizard($form, &$form_state) {
global $user;
//dpm($user);
  // Initialize a description of the steps for the wizard.
  if (empty($form_state['step'])) {
    $form_state['step'] = 1;

    // This array contains the function to be called at each step to get the
    // relevant form elements. It will also store state information for each
    // step.
    $form_state['step_information'] = _zillow_searchform_steps();
  }
  $step = &$form_state['step'];
  drupal_set_title(t('Add Open House: Step @step', array('@step' => $step)));

  // Call the function named in $form_state['step_information'] to get the
  // form elements to display for this step.
  $form = $form_state['step_information'][$step]['form']($form, $form_state);

  // Show the 'previous' button if appropriate. Note that #submit is set to
  // a special submit handler, and that we use #limit_validation_errors to
  // skip all complaints about validation when using the back button. The
  // values entered will be discarded, but they will not be validated, which
  // would be annoying in a "back" button.
  if ($step > 1) {
    $form['prev'] = array(
      '#type' => 'submit',
      '#value' => t('Previous'),
      '#name' => 'prev',
      '#submit' => array('zillow_searchform_wizard_previous_submit'),
      '#limit_validation_errors' => array(),
    );
  }

  // Show the Next button only if there are more steps defined.
  if ($step < count($form_state['step_information'])) {
    // The Next button should be included on every step
    $form['next'] = array(
      '#type' => 'submit',
      '#value' => t('Next'),
      '#name' => 'next',
      '#submit' => array('zillow_searchform_wizard_next_submit'),
    );
  }
  else {
    // Just in case there are no more steps, we use the default submit handler
    // of the form wizard. Call this button Finish, Submit, or whatever you
    // want to show. When this button is clicked, the
    // zillow_searchform_wizard_submit handler will be called.
    $form['finish'] = array(
      '#type' => 'submit',
      '#value' => t('Create Property'),
    );
  }

  // Include each validation function defined for the different steps.
  if (function_exists($form_state['step_information'][$step]['form'] . '_validate')) {
    $form['next']['#validate'] = array($form_state['step_information'][$step]['form'] . '_validate');
  }

  return $form;
}

/**
 * Submit handler for the "previous" button.
 * - Stores away $form_state['values']
 * - Decrements the step counter
 * - Replaces $form_state['values'] with the values from the previous state.
 * - Forces form rebuild.
 *
 * You are not required to change this function.
 *
 * @ingroup zillow_searchform
 */
function zillow_searchform_wizard_previous_submit($form, &$form_state) {
  $current_step = &$form_state['step'];
  //dpm(&$form_state);
  //dpm($form_state['values']);
  $form_state['step_information'][$current_step]['stored_values'] = $form_state['values'];
  if ($current_step > 1) {
    $current_step--;
    //dpm($form_state['step_information'][$current_step]['stored_values']);
    $form_state['values'] = $form_state['step_information'][$current_step]['stored_values'];
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the 'next' button.
 * - Saves away $form_state['values']
 * - Increments the step count.
 * - Replace $form_state['values'] from the last time we were at this page
 *   or with array() if we haven't been here before.
 * - Force form rebuild.
 *
 * You are not required to change this function.
 *
 * @param $form
 * @param $form_state
 *
 * @ingroup zillow_searchform
 */
function zillow_searchform_wizard_next_submit($form, &$form_state) {
	//dpm($form_state);
  $current_step = &$form_state['step'];
  $form_state['step_information'][$current_step]['stored_values'] = $form_state['values'];

  if ($current_step < count($form_state['step_information'])) {
    $current_step++;
    if (!empty($form_state['step_information'][$current_step]['stored_values'])) {
      $form_state['values'] = $form_state['step_information'][$current_step]['stored_values'];
    }
    else {
      $form_state['values'] = array();
    }
    $form_state['rebuild'] = TRUE;  // Force rebuild with next step.
    return;
  }
}

/**
 * The previous code was a 'skeleton' of a multistep wizard form. You are not
 * required to change a line on the previous code (apart from defining your own
 * steps in the _zillow_searchform_steps() function.
 *
 * All the code included from here is the content of the wizard, the steps of
 * the form.
 *
 * First, let's show the defined steps for the wizard example.
 * @ingroup zillow_searchform
 */

/**
 * Returns form elements for the 'search info' page of the wizard. This is the
 * first step of the wizard, asking for two textfields: first name and last
 * name.
 *
 * @ingroup zillow_searchform
 */
function zillow_searchform_wizard_search_info($form, &$form_state) {
  $form['zillowsearch'] = array(
    '#title' => t('Search by Address'),
    '#type' => 'fieldset',
    '#description' => t('Description here'),
  );
  $form['zillowsearch']['address'] = array(
    '#title' => t('Address'),
    '#type' => 'textfield',
    '#description' => t('Example: 1234 Street Ave. NE.'),
    '#default_value' => !empty($form_state['values']['address']) ? $form_state['values']['address'] : '',
    //'#default_value' => '18520 Dierickx Court',
  );
  $form['zillowsearch']['city'] = array(
    '#title' => t('City'),
    '#type' => 'textfield',
    '#description' => t('Example: Portland.'),
	'#default_value' => !empty($form_state['values']['city']) ? $form_state['values']['city'] : '',
    //'#default_value' => 'gladstone',
  );
  $form['zillowsearch']['state'] = array(
    '#title' => t('State'),
    '#type' => 'textfield',
    '#description' => t('Example: OR'),
    '#default_value' => !empty($form_state['values']['state']) ? $form_state['values']['state'] : '',
    //'#default_value' => 'OR',
  );
  return $form;
} 


/**
 * Returns form elements for the 'property info' page of the wizard. This is the
 * second step of the wizard. 
 *
 * @ingroup zillow_searchform
 */
function zillow_searchform_wizard_property_info($form, &$form_state) {
  $form = array();
  $form['property_info_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Verify the informaiton is correct, then click Next'),
  );
  $form['property_info_fieldset']['street'] = array(
    '#type' => 'textfield',
    '#title' => t('Street Address'),
    '#description' => t('Street Address'),
    '#default_value' => !empty($form_state['property_info']->street) ? $form_state['property_info']->street : '',
    '#disabled' => TRUE,
  );
  $form['property_info_fieldset']['city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#description' => t('Street Name'),
    '#default_value' => !empty($form_state['property_info']->city) ? $form_state['property_info']->city : '',
    '#disabled' => TRUE,
  );
  $form['property_info_fieldset']['state'] = array(
    '#type' => 'textfield',
    '#title' => t('State'),
    '#description' => t('State'),
    '#default_value' => !empty($form_state['property_info']->state) ? $form_state['property_info']->state : '',
    '#disabled' => TRUE,
  );
  $form['property_info_fieldset']['zip'] = array(
    '#type' => 'textfield',
    '#title' => t('Zip Code'),
    '#description' => t('Hint: 97027'),
    '#default_value' => !empty($form_state['property_info']->zipcode) ? $form_state['property_info']->zipcode : '',
    '#disabled' => TRUE,
  );
  $form['property_details'] = array(
    '#type' => 'fieldset',
    '#title' => t('Additional Information'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['property_details']['usecode'] = array(
    '#type' => 'textfield',
    '#title' => t('Use code'),
    '#default_value' => !empty($form_state['property_info']->useCode) ? $form_state['property_info']->useCode : '',
    '#disabled' => TRUE,
  );
  $form['property_details']['bedrooms'] = array(
    '#type' => 'textfield',
    '#title' => t('Bedrooms'),
    '#default_value' => !empty($form_state['property_info']->bedrooms) ? $form_state['property_info']->bedrooms : '',
    '#disabled' => TRUE,
  );
  $form['property_details']['bathrooms'] = array(
    '#type' => 'textfield',
    '#title' => t('Bathrooms'),
    '#default_value' => !empty($form_state['property_info']->bathrooms) ? $form_state['property_info']->bathrooms : '',
    '#disabled' => TRUE,
  );
  $form['property_details']['year_built'] = array(
    '#type' => 'textfield',
    '#title' => t('Build year'),
    '#default_value' => !empty($form_state['property_info']->yearBuilt) ? $form_state['property_info']->yearBuilt : '',
    '#disabled' => TRUE,
  );
  $form['property_details']['finishedSqFt'] = array(
    '#type' => 'textfield',
    '#title' => t('Finished Square Feet'),
    '#default_value' => !empty($form_state['property_info']->finishedSqFt) ? $form_state['property_info']->finishedSqFt : '',
    '#disabled' => TRUE,
  );
  $form['property_details']['lotSizeSqFt'] = array(
    '#type' => 'textfield',
    '#title' => t('Lot Size Square Feet'),
    '#default_value' => !empty($form_state['property_info']->lotSizeSqFt) ? $form_state['property_info']->lotSizeSqFt : '',
    '#disabled' => TRUE,
  );
  $form['property_details']['latitude'] = array(
    '#type' => 'textfield',
    '#title' => t('latitude'),
    '#default_value' => !empty($form_state['property_info']->latitude) ? $form_state['property_info']->latitude : '',
    '#disabled' => TRUE,
  );
  $form['property_details']['longitude'] = array(
    '#type' => 'textfield',
    '#title' => t('Longitude'),
    '#default_value' => !empty($form_state['property_info']->longitude) ? $form_state['property_info']->longitude : '',
    '#disabled' => TRUE,
  );
  $form['property_details']['lastsoldprice'] = array(
    '#type' => 'textfield',
    '#title' => t('Last sold price'),
    '#default_value' => !empty($form_state['property_info']->lastSoldPrice) ? $form_state['property_info']->lastSoldPrice : '',
    '#disabled' => TRUE,
  );
  $form['property_details']['lastsoldDate'] = array(
    '#type' => 'textfield',
    '#title' => t('Last sold date'),
    '#default_value' => !empty($form_state['property_info']->lastSoldDate) ? $form_state['property_info']->lastSoldDate : '',
    '#disabled' => TRUE,
  );
  $form['property_details']['zpid'] = array(
    '#type' => 'value',
   '#default_value' => !empty($form_state['property_info']->zpid) ? $form_state['property_info']->zpid : '',
  );
  return $form;
}

/**
 * Returns form elements for the 'other info' page of the wizard. This is the
 * thid and last step of the example wizard.
 *
 * @ingroup zillow_searchform
 */
function zillow_searchform_wizard_other_info($form, &$form_state) {
  $form['#attributes']['enctype'] = 'multipart/form-data';
  
  $form['zillowsearch_other_fieldset'] = array(
    '#title' => t('Open House Details'),
    '#type' => 'fieldset',
    '#description' => t('Answer the questions about your Open House'),
  );
  $form['zillowsearch_other_fieldset']['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textarea',
    '#cols' => 40,
    '#rows' => 10,
    '#resizable' => TRUE,
    '#description' => t('Enter a description of your property, or describe a
                        unique parking situation your guests should know about.'),
    '#required' => TRUE,
  );
  $form['zillowsearch_other_fieldset']['food'] = array(
    '#type' => 'radios',
    '#title' => t('Will food be provided?'),
    '#options' => array(
      t('No'),
      t('Yes'),
    ),
    '#description' => t('Your guests are more likely to show if you offer food. It is not required.'),
    '#required' => TRUE,
  );
  $form['zillowsearch_other_fieldset']['hosted'] = array(
    '#type' => 'radios',
    '#title' => t('Is this Open house hosted?'),
    '#options' => array(
      t('No'),
      t('Yes'),
    ),
    '#description' => t('Answer Yes if you will be there, no if not.'),
    '#required' => TRUE,
  );

  /**
   *  SETUP THE DATE FIELD
   */

  // [...snip...] add many fields to your form

  // Creating the date/time element starts here

  // Provide a default date in the format YYYY-MM-DD HH:MM:SS.
  $date = '2008-12-31 00:00:00';

  // Provide a format using regular PHP format parts (see documentation on php.net).
  // If you're using a date_select, the format will control the order of the date parts in the selector,
  // rearrange them any way you like. Parts left out of the format will not be displayed to the user.
  $format = 'Y-m-d H:i';

  $form['zillowsearch_other_fieldset']['fromdate'] = array(
     '#type' => 'date_popup', // types 'date_text', 'date_timezone', and 'date_year_range' are also supported. See .inc file.
     '#title' => 'Date and time your Open House Starts',
     '#default_value' => $date, 
     '#date_format' => $format,
     '#date_label_position' => 'within', // See other available attributes and what they do in date_api_elements.inc
     '#date_increment' => 15, // Optional, used by the date_select and date_popup elements to increment minutes and seconds.
     '#date_year_range' => '-3:+3', // Optional, used to set the year range (back 3 years and forward 3 years is the default).
     '#required' => TRUE,
  );
 $form['zillowsearch_other_fieldset']['todate'] = array(
     '#type' => 'date_popup', // types 'date_text', 'date_timezone', and 'date_year_range' are also supported. See .inc file.
     '#title' => 'Date and time your Open House Ends',
     '#default_value' => $date, 
     '#date_format' => $format,
     '#date_label_position' => 'within', // See other available attributes and what they do in date_api_elements.inc
     '#date_increment' => 15, // Optional, used by the date_select and date_popup elements to increment minutes and seconds.
     '#date_year_range' => '-3:+3', // Optional, used to set the year range (back 3 years and forward 3 years is the default).
     '#required' => TRUE,
  );

  //$form['zillow_search_image_fieldset']['picture_1']['picture_upload_button'] = array(
  //  '#type' => 'button',
  //  '#title' => t('Upload'),
  //  '#size' => 48,
  //  '#description' => t('Upload a picture for the Open House.')
  //  
  return $form;
}

function zillow_searchform_wizard_picture() {  
  $form['picture'] = array(
    '#type' => 'fieldset',
    '#title' => t('Picture'),
    //'#access' => (!$register && variable_get('user_pictures', 0)),
  );
  $form['picture']['picture'] = array(
    '#type' => 'value',
    '#value' => NULL,
  );
  $form['picture']['picture_current'] = array(
    '#markup' => NULL,
  );
  $form['picture']['picture_upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload picture'),
    '#size' => 48,
    '#description' => t('An image for your Open House. Pictures larger than @dimensions pixels will be scaled down.', array('@dimensions' => variable_get('user_picture_dimensions', '85x85'))) . ' ' . filter_xss_admin(variable_get('user_picture_guidelines', '')),
  );
  $form['#validate'][] = 'zillow_searchform_wizard_picture_validate';
  return $form;
}
/**
 * Custom validation form for the 'search info' page of the wizard.
 *
 * This is the validation function for the second step of the wizard.
 *
 * @ingroup zillow_searchform
 */
function zillow_searchform_wizard_search_info_validate($form, &$form_state) {
  
include('sites/all/libraries/pillow.php'); //include the pillow library (zillow abstraction library)
  
  /**   
   * Below is an example of calling the API for the address for the exact address match "2114 Bigelow Ave", "Seattle, WA":
   * http://www.zillow.com/webservice/GetSearchResults.htm?zws-id=<ZWSID>&address=2114+Bigelow+Ave&citystatezip=Seattle%2C+WA
  */
  
  //Run form values through pillow (Zillow API), return if we find a property
  //Then we create a new "Open House" node with those values filled in
  
  //setup the variables we need to send to pillow
  $zillow_api_key = 'X1-ZWz1d7gzmlu8sr_7yjzz';
  $address = $form_state['values']['address'];
  $city = $form_state['values']['city'];
  $state = $form_state['values']['state'];
  //($city);
  //city state and zip need to be added together per zillow api
  $city_state_zip = ($city . ", " . $state);
  
  $pf = new Pillow_Factory( $zillow_api_key );
  //$pf will be an array with 1 or more Pillow_Property objects. Exact
  // matches will be found at $pf[0]


	$variable = $pf->findExactProperty(
	    $address,$city_state_zip,TRUE
	  );
	dpm($variable);
  $form_state['property_info'] = $variable;

  //dpm($form_state['property_info']);
  //get rid of the nested objects we dont need
  unset($form_state['property_info']->links);
  unset($form_state['property_info']->zestimate);
  //unset($form_state['property_info']->_factory);
  
  //$form_state['property_info'] = $property;
  //dpm($form_state['property_info']);
  if (!$form_state['property_info']->city) { //Did not get a city from Zillow, assume no property found
    form_set_error('city', "Could not find data" );  
  }
  
  
  //test the function
  //form_set_error('address', t('invalid'));
}


/**
 * Custom validation form for the 'property info' page of the wizard.
 *
 * This is the validation function for the second step of the wizard.
 * The city cannot be empty or be "San Francisco".
 *
 * @ingroup zillow_searchform
 */
function zillow_searchform_wizard_property_info_validate($form, &$form_state) {
  //if ($form_state['values']['city'] == 'San Francisco') {
   //form_set_error('city', t('You were warned not to enter "San Francisco"'));
  //}
}

/**
 * Custom validation form for the 'other info' page of the wizard.
 *
 * This is the validation function for the third step of the wizard.
 * The city cannot be empty or be "San Francisco".
 *
 * @ingroup zillow_searchform
 */
function zillow_searchform_wizard_other_info_validate($form, &$form_state) {
  //if ($form_state['values']['city'] == 'San Francisco') {
  //dpm('dpm called by: zillow_searchform_wizard_other_info_validate');
    //form_set_error('todate', t('You were warned not to enter "San Francisco"'));
  //}
}

/**
 * Custom validation for the 'picture' page of the wizard.
 *
 * This is the validation function for the fourth step of the wizard.
 */
function zillow_searchform_wizard_picture_validate($form, &$form_state) {
  //dpm($form_state['values']);
/// If required, validate the uploaded picture.
  $validators = array(
    'file_validate_is_image' => array(),
    'file_validate_size' => array(variable_get('user_picture_file_size', '30') * 1024), 
  );
  // Save the file as a temporary file.
  $file = file_save_upload('picture_upload', $validators);
  if ($file === FALSE) {
    form_set_error('picture_upload', t("Failed to upload the picture image; the %directory
    directory doesn't exist or is not writable.", array('%directory' =>
    variable_get('zillow_picture_path', 'properties'))));
  }
  elseif ($file !== NULL) {
    $form_state['values']['picture_upload'] = $file;
  }
}
// And now comes the magic of the wizard, the function that should handle all the
// inputs from the user on each different step.
/**
 * Wizard form submit handler.
 * - Saves away $form_state['values']
 * - Process all the form values.
 *
 * This demonstration handler just do a drupal_set_message() with the information
 * collected on each different step of the wizard.
 *
 * @param $form
 * @param $form_state
 *
 * @ingroup zillow_searchform
 */
function zillow_searchform_wizard_submit($form, &$form_state) {
  //dpm(node_load(9));
  $current_step = &$form_state['step'];
  $form_state['step_information'][$current_step]['stored_values'] = $form_state['values'];

  // In this case we've completed the final page of the wizard, so process the
  // submitted information.
  //drupal_set_message(t('This information was collected by this wizard:'));
  foreach ($form_state['step_information'] as $index => $value) {
  //  // Remove FAPI fields included in the values (form_token, form_id and form_build_id
  //  // This is not required, you may access the values using $value['stored_values']
  //  // but I'm removing them to make a more clear representation of the collected
  //  // information as the complete array will be passed through drupal_set_message().
    unset($value['stored_values']['form_id']);
    unset($value['stored_values']['form_build_id']);
    unset($value['stored_values']['form_token']);

  // Shows all the values
  //  drupal_set_message(t('Step @num collected the following values: <pre>@result</pre>',
  //                       array('@num' => $index, '@result' => print_r($value['stored_values'],
  //                                                                    TRUE))));
  }
  
  //print the property info for debugging
  //drupal_set_message(t('Property info'));
  //foreach ($form_state['property_info'] as $index => $value) {
  //  drupal_set_message(t('Index @num->@result', array('@num' => $index, '@result' => $value)));
  //}
  
  drupal_set_message(t('Your property has been created'));
  
  
  //only continue if we have a picture
  //moving picture handling to node
  //if (isset($form_state['step_information'][4]['stored_values']['picture_upload'])) {
  //  $picture = $form_state['step_information'][4]['stored_values']['picture_upload'];
  //      // If the picture is a temporary file move it to its final location and
  //      // make it permanent.
  //  if (($picture->status & FILE_STATUS_PERMANENT) == 0) {
  //    $info = image_get_info($picture->uri);
  //    $picture_directory = variable_get('file_default_scheme', 'public') . '://' .
  //                                      variable_get('zillow_picture_path', 'properties');
  //    // Prepare the pictures directory.
  //    file_prepare_directory($picture_directory, FILE_CREATE_DIRECTORY);
  //    $destination = file_stream_wrapper_uri_normalize($picture_directory . '/'
  //                                                    . $picture->filename);
  //    if ($picture = file_move($picture, $destination, FILE_EXISTS_RENAME)) {
  //      //file moved ok
  //      
  //                         //array('@image'=> $picture->filename, '@dest => $picture_directory'));
  //      $picture->status |= FILE_STATUS_PERMANENT;
  //      if(file_save($picture)) {
  //        //drupal_set_message('Your image was saved');
  //        drupal_set_message(t('Your image <em>@picture</em> was saved at <em>@dest</em>',
  //                       array('@picture' => $picture->filename, '@dest' =>
  //                             variable_get('file_public_path', conf_path() . '/files'). "/" .
  //                             variable_get('zillow_picture_path', 'properties'),
  //                          TRUE)));
  //        
  //        
  //      } else {
  //        drupal_set_error(array(),'Your image was not saved.');
  //      }
  //      
  //    }
  //  }
  //}
//zillow_searchform_nodesave($form, &$form_state);
  
  //dpm($form_state['values']);
  $node = new stdClass();
  $node->nid            = NULL;
  $node->title          = $form_state['step_information'][2]['stored_values']['street'];
  $node->type           = 'open';
  $node->language       = LANGUAGE_NONE;
  $node->created        = time();
  $node->changed        = $node->created;
  $node->status         = 1;         // Published?
  $node->promote        = 0;         // Display on front page?
  $node->sticky         = 0;         // Display top of page?
  $node->format         = 0;         // Filtered HTML?
  $node->revision       = 1;
  //node fields for property
  $node->field_prop_address_street['und'][0]['value']   = $form_state['step_information'][2]['stored_values']['street'];
  $node->field_prop_address_zip['und'][0]['value']      = $form_state['step_information'][2]['stored_values']['zip'];
  $node->field_prop_address_city['und'][0]['value']     = $form_state['step_information'][2]['stored_values']['city'];
  $node->field_prop_address_state['und'][0]['value']    = $form_state['step_information'][2]['stored_values']['state'];
 
  //host fields
  global $user;
  $node->field_host['und'][0]['target_id']				= $user->uid;
  
  /* Allowed values for master_column are wkt, latlon, bounds
 */
  $geoarray = array(
                    'wkt' 		=> '',
                    'geo_type'  => 'point',
                    'lat' 		=> $form_state['step_information'][2]['stored_values']['latitude'],
                    'lon' 		=> $form_state['step_information'][2]['stored_values']['longitude'],
                    'left' 		=> '',
                    'top'  		=> '',
                    'right'  	=> '',
                    );
  $geostuff = geofield_compute_values($geoarray, $master_column = 'latlon');
  //dpm($geostuff);
  
  $node->field_prop_geo['und'][] 						= $geostuff;
  $node->field_prop_baths['und'][0]['value']            = $form_state['step_information'][2]['stored_values']['bathrooms'];
  $node->field_prop_bedrooms['und'][0]['value']         = $form_state['step_information'][2]['stored_values']['bedrooms'];
  $node->field_prop_['und'][0]['value']                 = $form_state['step_information'][2]['stored_values']['usecode']; //TO DO: fix this, should be field_prop_usecode
  $node->field_prop_year['und'][0]['value']             = $form_state['step_information'][2]['stored_values']['year_built'];
  $node->field_prop_lotsize['und'][0]['value']          = $form_state['step_information'][2]['stored_values']['lotSizeSqFt'];
  $node->field_prop_sqfeet['und'][0]['value']           = $form_state['step_information'][2]['stored_values']['finishedSqFt'];
  $node->field_prop_zpid['und'][0]['value']             = $form_state['step_information'][2]['stored_values']['zpid'];
  $node->field_prop_sellprice['und'][0]['value']		= $form_state['step_information'][2]['stored_values']['lastsoldprice'];
  //node fields for the Open House
  $node->log                                            = t('Node saved: '.$node->title);
  //$node->body['und'][0]['value']                        = $form_state['step_information'][3]['stored_values']['description']; 
  //$node->field_open_food['und'][0]['value']             = $form_state['step_information'][3]['stored_values']['food'];
  //$node->field_open_hosted['und'][0]['value']           = $form_state['step_information'][3]['stored_values']['hosted'];
  //$node->field_open_date['und'][0]['value']             = $form_state['step_information'][3]['stored_values']['fromdate'];
  //$node->field_open_date['und'][0]['value2']            = $form_state['step_information'][3]['stored_values']['todate'];
  //node fields for images
  //$node->field_prop_images['und'][0]['value']           = '';

  // Save new node
  node_object_prepare($node);
  //echo "node prepare";
  //dpm($node);
  node_submit($node);
  node_save($node);
  //dpm($node);
 
  
  //send the user to the newly created node edit form to finalize the Open House
  
  $form_state['redirect'] = 'node/' . $node->nid . '/edit';


}

