
    <?php
    
    if ($form['#form_id'] == 'open_node_form') {
        //print render($form['#attributes']);
        if ($form['#action'] == '/node/add/open') {
            echo '<h1>You are adding a new one</h1>';
                hide($form['field_prop_address_zip']);
                hide($form['field_prop_address_city']);
                hide($form['field_prop_address_state']);
                hide($form['field_prop_address_lat']);
                hide($form['field_prop_address_lon']);
                hide($form['field_prop_baths']);
                hide($form['field_prop_bedrooms']);
                hide($form['field_prop_']); //TODO: fix this weirdly named field
                hide($form['field_prop_year']);
                hide($form['field_prop_lotsize']);
                hide($form['field_prop_sqfeet']);
                hide($form['field_prop_zpid']);
                hide($form['field_prop_address_street']);
                hide($form['body']);
                hide($form['field_open_dates']);
                hide($form['field_open_food']);
                hide($form['field_open_hosted']);
                //hide($form['field_prop_images']);
                //hide($form['actions']['submit']);
                $form['actions']['preview']['#value']="Next"; //Change Preview label to Submit, hide Submit
                /*
                [preview] => Array
                [#access] => 1
                [#type] => submit
                [#value] => Preview
                [#weight] => 10
                [#submit] => Array
                */
                
            
        } else {
            //echo '<h1>You are editing an existing one</h1>';
        }


        print drupal_render_children($form);
       
        
        /*
        echo '<pre>';
        print_r($form['actions']);
        echo '</pre>';
        */
    }
    ?>
