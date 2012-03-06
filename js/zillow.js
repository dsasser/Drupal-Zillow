(function($) {
    Drupal.behaviors.zillow = {
        attach: function(context, settings) {

	
            var start_lat = settings.zillow.start_address.lat;
            var start_lon = settings.zillow.start_address.lon;
            startlatlon = new google.maps.LatLng(start_lat, start_lon);
            var end_lat = settings.zillow.end_address.lat;
            var end_lon = settings.zillow.end_address.lon;
            endlatlon = new google.maps.LatLng(end_lat, end_lon);
            //alert(settings.zillow.waypoints[0]);
            var waypts = settings.zillow.waypoints[0];
			var gwaypoints = [];
//loop through waypoints to format for google
			$.each( waypts, function(i, l){
				gwaypoints.push({
					location:waypts[i].lat + ", " + waypts[i].lon,
					stopover:true
				});
				//console.log(waypts[i].lat + ", " + waypts[i].lon);
				
	
			 });
			console.log(gwaypoints);

            var end = "44.9798, -123.015";
            //end in Salem
            var geocoder;
            var directionsDisplay;
            var directionsService = new google.maps.DirectionsService();
            directionsDisplay = new google.maps.DirectionsRenderer();

            var map;

            //build a map
            var myOptions = {
                zoom: 8,
                center: startlatlon,
                mapTypeId: google.maps.MapTypeId.ROADMAP
            }
            map = new google.maps.Map(document.getElementById("map_canvas"), myOptions);
			//map = new google.maps.Map($('.geofieldMap'), myOptions);
            directionsDisplay.setMap(map);
			directionsDisplay.setPanel(document.getElementById("directionsPanel"));
			

            var mywaypts = [];
	       mywaypts.push({
	            	location:"44.9798 -123.015",
	            	stopover:true
			    });

			console.log(mywaypts);
			/*
					A sample DirectionsRequest is shown below:

					{
					  origin: "Chicago, IL",
					  destination: "Los Angeles, CA",
					  waypoints: [
					    {
					      location:"Joplin, MO",
					      stopover:false
					    },{
					      location:"Oklahoma City, OK",
					      stopover:true
					    }],
					  provideRouteAlternatives: false,
					  travelMode: TravelMode.DRIVING,
					  unitSystem: UnitSystem.IMPERIAL
					}
			*/
            var request = {
                origin: startlatlon,
                //start,
                destination: endlatlon,
                //end,
                waypoints: gwaypoints,
                optimizeWaypoints: true,
                travelMode: google.maps.TravelMode.DRIVING
            };
            directionsService.route(request,
            function(result, status) {
                if (status == google.maps.DirectionsStatus.OK) {
                    directionsDisplay.setDirections(result);
					console.log(result);
                }
            });
        }
    };
} (jQuery));


