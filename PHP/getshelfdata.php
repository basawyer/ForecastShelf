/*
	ForecastShelf getshelfdata.php
		- Shelf returns data for current weather and next 3 days forecast using WeatherUnderground API
		- Data: "AAABBBCCCDDD"
		- AAA is 3 digits [Condition][WIND][TEMP]
		
		© Copyright Blake Sawyer 2012

*/

<?php
	
		/*
	  		DAY CONDITIONS STRINGS = "XXX"

	  	MODES (1st number):
			NA = 0;
	  		TStorms = 1;
	  		Rain = 2;
			Snowy = 3;
			Cloudy = 4;
			Mostly Cloudy = 5;
			Partly Cloudy = 6;
			Sunny = 7;

		WIND (2nd number):
			NA = 0;
			0-9 = 1;
			10-19 = 2;
			20-29 = 3;
			30+ = 4;

		TEMP (3rd number):
			NA = 0;
			<30 = 1;
			30-49 = 2;
			50-69 = 3;
			70+ = 4;


	  	*/
	
	$tstorm_strs = array("tstorms", "chancetstorms");
	$rain_strs = array("chancerain", "rain");
	$snowy_strs = array("chanceflurries","chancesleet","chancesnow","flurries","sleet","snow");
	$cloudy_strs = array("cloudy","chancerain","chanceflurries","chancesleet","chancesnow","flurries","chancetstorms");
	$mostlycloudy_strs = array("mostlycloudy","fog");
	$partlycloudy_strs = array("hazy","partlycloudy","partlysunny");
	$sunny_strs = array("sunny","clear","mostlysunny","unknown");
	
	$zipcode = "";
	if (isset($_GET['zip'])) {
		$zipcode = $_GET['zip'];
	}

	if($zipcode !== ""){
	
		

		$conditions_url = "http://api.wunderground.com/api/e88385d942573a97/forecast/q/" . $zipcode . ".json";
	  	$json_string = file_get_contents($conditions_url);
	  	$parsed_json = json_decode($json_string);
	  	
	  	$today_icon = $parsed_json->{"forecast"}->{"simpleforecast"}->{"forecastday"}[0]->{"icon"};
	  	$today_average_wind = $parsed_json->{'forecast'}->{'simpleforecast'}->{'forecastday'}[0]->{'avewind'}->{'mph'};
	  	$today_temp = $parsed_json->{'forecast'}->{'simpleforecast'}->{'forecastday'}[0]->{'high'}->{'fahrenheit'};
	  	$today_pop = $parsed_json->{'forecast'}->{'simpleforecast'}->{'forecastday'}[0]->{'pop'};

	  	$one_icon = $parsed_json->{'forecast'}->{'simpleforecast'}->{'forecastday'}[1]->{'icon'};
	  	$one_average_wind = $parsed_json->{'forecast'}->{'simpleforecast'}->{'forecastday'}[1]->{'avewind'}->{'mph'};
	  	$one_temp = $parsed_json->{'forecast'}->{'simpleforecast'}->{'forecastday'}[1]->{'high'}->{'fahrenheit'};
	  	$one_pop = $parsed_json->{'forecast'}->{'simpleforecast'}->{'forecastday'}[1]->{'pop'};

	  	$two_icon = $parsed_json->{'forecast'}->{'simpleforecast'}->{'forecastday'}[2]->{'icon'};
	  	$two_average_wind = $parsed_json->{'forecast'}->{'simpleforecast'}->{'forecastday'}[2]->{'avewind'}->{'mph'};
	  	$two_temp = $parsed_json->{'forecast'}->{'simpleforecast'}->{'forecastday'}[2]->{'high'}->{'fahrenheit'};
	  	$two_pop = $parsed_json->{'forecast'}->{'simpleforecast'}->{'forecastday'}[2]->{'pop'};

	  	$three_icon = $parsed_json->{'forecast'}->{'simpleforecast'}->{'forecastday'}[3]->{'icon'};
	  	$three_average_wind = $parsed_json->{'forecast'}->{'simpleforecast'}->{'forecastday'}[3]->{'avewind'}->{'mph'};
	  	$three_temp = $parsed_json->{'forecast'}->{'simpleforecast'}->{'forecastday'}[3]->{'high'}->{'fahrenheit'};
	  	$three_pop = $parsed_json->{'forecast'}->{'simpleforecast'}->{'forecastday'}[3]->{'pop'};
	

		$today_conditions = "";
		$one_conditions = "";
		$two_conditions = "";
		$three_conditions = "";
		

		//TODAY
		if(in_array($today_icon,$tstorm_strs)){

			if (strpos($today_icon,'chance') !== false) {
    			
    			if(intval($today_pop) > 25){
    				$today_conditions .= "4";

    			}
    			else{
    				$today_conditions .= "1";

    			}
			}
			else{

				$today_conditions .= "1";

			}

		}
		elseif(in_array($today_icon,$rain_strs)){

			if (strpos($today_icon,'chance') !== false) {
    			
    			if(intval($today_pop) > 50){
    				$today_conditions .= "2";

    			}
    			else{
    				$today_conditions .= "4";

    			}
			}
			else{

				$today_conditions .= "2";

			}

		}
		elseif(in_array($today_icon,$snowy_strs)){

			if (strpos($today_icon,'chance') !== false || $today_icon == "flurries") {
    			
    			if(intval($today_pop) > 50){
    				$today_conditions .= "3";

    			}
    			else{
    				$today_conditions .= "4";

    			}
			}
			else{

				$today_conditions .= "3";

			}

		}
		elseif(in_array($today_icon,$cloudy_strs)){

			$today_conditions .= "4";


		}
		elseif(in_array($today_icon,$mostlycloudy_strs)){

			$today_conditions .= "5";


		}
		elseif(in_array($today_icon,$partlycloudy_strs)){

			$today_conditions .= "6";


		}
		elseif(in_array($today_icon,$sunny_strs)){

			$today_conditions .= "7";


		}
		else{
			$today_conditions .= "0";
			
		}
		

		if(intval($today_average_wind) <= 9){
			$today_conditions .= "1";

		}
		elseif(intval($today_average_wind) <= 19){
			$today_conditions .= "2";

		}
		elseif(intval($today_average_wind) <= 29){
			$today_conditions .= "3";

		}
		elseif(intval($today_average_wind) >= 30){
			$today_conditions .= "4";

		}
		else{
			$today_conditions .= "0";

		}

		if(intval($today_temp) <= 29){
			$today_conditions .= "1";

		}
		elseif(intval($today_temp) <= 49){
			$today_conditions .= "2";

		}
		elseif(intval($today_temp) <= 69){
			$today_conditions .= "3";

		}
		elseif(intval($today_temp) >= 70){
			$today_conditions .= "4";

		}
		else{
			$today_conditions .= "0";

		}

		//NEXT DAY
		if(in_array($one_icon,$tstorm_strs)){

			if (strpos($one_icon,'chance') !== false) {
    			
    			if(intval($one_pop) > 25){
    				$one_conditions .= "4";
    			}
    			else{
    				$one_conditions .= "1";
    			}
			}
			else{

				$one_conditions .= "1";
			}

		}
		elseif(in_array($one_icon,$rain_strs)){

			if (strpos($one_icon,'chance') !== false) {
    			
    			if(intval($one_pop) > 50){
    				$one_conditions .= "2";
    			}
    			else{
    				$one_conditions .= "4";
    			}
			}
			else{

				$one_conditions .= "2";
			}

		}
		elseif(in_array($one_icon,$snowy_strs)){

			if (strpos($one_icon,'chance') !== false || $one_icon == "flurries") {
    			
    			if(intval($one_pop) > 50){
    				$one_conditions .= "3";
    			}
    			else{
    				$one_conditions .= "4";
    			}
			}
			else{

				$one_conditions .= "3";
			}

		}
		elseif(in_array($one_icon,$cloudy_strs)){

			$one_conditions .= "4";

		}
		elseif(in_array($one_icon,$mostlycloudy_strs)){

			$one_conditions .= "5";

		}
		elseif(in_array($one_icon,$partlycloudy_strs)){

			$one_conditions .= "6";

		}
		elseif(in_array($one_icon,$sunny_strs)){

			$one_conditions .= "7";

		}
		else{
			$one_conditions .= "0";
		}

		if(intval($one_average_wind) <= 9){
			$one_conditions .= "1";
		}
		elseif(intval($one_average_wind) <= 19){
			$one_conditions .= "2";
		}
		elseif(intval($one_average_wind) <= 29){
			$one_conditions .= "3";
		}
		elseif(intval($one_average_wind) >= 30){
			$one_conditions .= "4";
		}
		else{
			$one_conditions .= "0";
		}

		if(intval($one_temp) <= 29){
			$one_conditions .= "1";

		}
		elseif(intval($one_temp) <= 49){
			$one_conditions .= "2";

		}
		elseif(intval($one_temp) <= 69){
			$one_conditions .= "3";

		}
		elseif(intval($one_temp) >= 70){
			$one_conditions .= "4";

		}
		else{
			$one_conditions .= "0";

		}






		//2ND DAY
		if(in_array($two_icon,$tstorm_strs)){

			if (strpos($two_icon,'chance') !== false) {
    			
    			if(intval($two_pop) > 25){
    				$two_conditions .= "4";
    			}
    			else{
    				$two_conditions .= "1";
    			}
			}
			else{

				$two_conditions .= "1";
			}

		}
		elseif(in_array($two_icon,$rain_strs)){

			if (strpos($two_icon,'chance') !== false) {
    			
    			if(intval($two_pop) > 50){
    				$two_conditions .= "2";
    			}
    			else{
    				$two_conditions .= "4";
    			}
			}
			else{

				$two_conditions .= "2";
			}

		}
		elseif(in_array($two_icon,$snowy_strs)){

			if (strpos($two_icon,'chance') !== false || $two_icon == "flurries") {
    			
    			if(intval($two_pop) > 50){
    				$two_conditions .= "3";
    			}
    			else{
    				$two_conditions .= "4";
    			}
			}
			else{

				$two_conditions .= "3";
			}

		}
		elseif(in_array($two_icon,$cloudy_strs)){

			$two_conditions .= "4";

		}
		elseif(in_array($two_icon,$mostlycloudy_strs)){

			$two_conditions .= "5";

		}
		elseif(in_array($two_icon,$partlycloudy_strs)){

			$two_conditions .= "6";

		}
		elseif(in_array($two_icon,$sunny_strs)){

			$two_conditions .= "7";

		}
		else{
			$two_conditions .= "0";
		}

		if(intval($two_average_wind) <= 9){
			$two_conditions .= "1";
		}
		elseif(intval($two_average_wind) <= 19){
			$two_conditions .= "2";
		}
		elseif(intval($two_average_wind) <= 29){
			$two_conditions .= "3";
		}
		elseif(intval($two_average_wind) >= 30){
			$two_conditions .= "4";
		}
		else{
			$two_conditions .= "0";
		}

		if(intval($two_temp) <= 29){
			$two_conditions .= "1";

		}
		elseif(intval($two_temp) <= 49){
			$two_conditions .= "2";

		}
		elseif(intval($two_temp) <= 69){
			$two_conditions .= "3";

		}
		elseif(intval($two_temp) >= 70){
			$two_conditions .= "4";

		}
		else{
			$two_conditions .= "0";

		}




		//3RD DAY
		if(in_array($three_icon,$tstorm_strs)){

			if (strpos($three_icon,'chance') !== false) {
    			
    			if(intval($three_pop) > 25){
    				$three_conditions .= "4";
    			}
    			else{
    				$three_conditions .= "1";
    			}
			}
			else{

				$three_conditions .= "1";
			}

		}
		elseif(in_array($three_icon,$rain_strs)){

			if (strpos($three_icon,'chance') !== false) {
    			
    			if(intval($three_pop) > 50){
    				$three_conditions .= "2";
    			}
    			else{
    				$three_conditions .= "4";
    			}
			}
			else{

				$three_conditions .= "2";
			}

		}
		elseif(in_array($three_icon,$snowy_strs)){

			if (strpos($three_icon,'chance') !== false || $three_icon == "flurries") {
    			
    			if(intval($three_pop) > 50){
    				$three_conditions .= "3";
    			}
    			else{
    				$three_conditions .= "4";
    			}
			}
			else{

				$three_conditions .= "3";
			}

		}
		elseif(in_array($three_icon,$cloudy_strs)){

			$three_conditions .= "4";

		}
		elseif(in_array($three_icon,$mostlycloudy_strs)){

			$three_conditions .= "5";

		}
		elseif(in_array($three_icon,$partlycloudy_strs)){

			$three_conditions .= "6";

		}
		elseif(in_array($three_icon,$sunny_strs)){

			$three_conditions .= "7";

		}
		else{
			$three_conditions .= "0";
		}

		if(intval($three_average_wind) <= 9){
			$three_conditions .= "1";
		}
		elseif(intval($three_average_wind) <= 19){
			$three_conditions .= "2";
		}
		elseif(intval($three_average_wind) <= 29){
			$three_conditions .= "3";
		}
		elseif(intval($three_average_wind) >= 30){
			$three_conditions .= "4";
		}
		else{
			$three_conditions .= "0";
		}

		if(intval($three_temp) <= 29){
			$three_conditions .= "1";

		}
		elseif(intval($three_temp) <= 49){
			$three_conditions .= "2";

		}
		elseif(intval($three_temp) <= 69){
			$three_conditions .= "3";

		}
		elseif(intval($three_temp) >= 70){
			$three_conditions .= "4";

		}
		else{
			$three_conditions .= "0";

		}
		
		echo $today_conditions;
		echo $one_conditions;
		echo $two_conditions;
		echo $three_conditions;


	}

?>

