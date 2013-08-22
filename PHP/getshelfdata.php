<?php
	date_default_timezone_set('America/New_York');
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

		GUSTS (3rd number):
			NA = 0;
			0-9 = 1;
			10-19 = 2;
			20-29 = 3;
			30+ = 4;

	  	*/
	  	
	function get_new_data($zip){
	
		$tstorm_strs = array("tstorms", "chancetstorms");
		$rain_strs = array("chancerain", "rain");
		$snowy_strs = array("chanceflurries","chancesleet","chancesnow","flurries","sleet","snow");
		$cloudy_strs = array("cloudy","chancerain","chanceflurries","chancesleet","chancesnow","flurries","chancetstorms");
		$mostlycloudy_strs = array("mostlycloudy","fog");
		$partlycloudy_strs = array("hazy","partlycloudy","partlysunny");
		$sunny_strs = array("sunny","clear","mostlysunny","unknown");
		
		$now_url = "http://api.wunderground.com/api/<API Removed>/conditions/q/" . $zip . ".json";
		$now_string = file_get_contents($now_url);
		$parsed_now_json = json_decode($now_string);
		
		$forecast_url = "http://api.wunderground.com/api/<API Removed>/forecast/q/" . $zip . ".json";
	  	$json_string = file_get_contents($forecast_url);
	  	$parsed_json = json_decode($json_string);
	  	
	  	$today_icon = $parsed_now_json->{"current_observation"}->{"icon"};
		$today_average_wind = $parsed_now_json->{"current_observation"}->{"wind_mph"};
		$today_gust = $parsed_now_json->{"current_observation"}->{"wind_gust_mph"};
	  	$today_pop = $parsed_json->{'forecast'}->{'simpleforecast'}->{'forecastday'}[0]->{'pop'};

	  	$one_icon = $parsed_json->{'forecast'}->{'simpleforecast'}->{'forecastday'}[1]->{'icon'};
	  	$one_average_wind = $parsed_json->{'forecast'}->{'simpleforecast'}->{'forecastday'}[1]->{'avewind'}->{'mph'};
	  	$one_gust = $parsed_json->{'forecast'}->{'simpleforecast'}->{'forecastday'}[1]->{'maxwind'}->{'mph'};
	  	$one_pop = $parsed_json->{'forecast'}->{'simpleforecast'}->{'forecastday'}[1]->{'pop'};

	  	$two_icon = $parsed_json->{'forecast'}->{'simpleforecast'}->{'forecastday'}[2]->{'icon'};
	  	$two_average_wind = $parsed_json->{'forecast'}->{'simpleforecast'}->{'forecastday'}[2]->{'avewind'}->{'mph'};
	  	$two_gust = $parsed_json->{'forecast'}->{'simpleforecast'}->{'forecastday'}[2]->{'maxwind'}->{'mph'};
	  	$two_pop = $parsed_json->{'forecast'}->{'simpleforecast'}->{'forecastday'}[2]->{'pop'};

	  	$three_icon = $parsed_json->{'forecast'}->{'simpleforecast'}->{'forecastday'}[3]->{'icon'};
	  	$three_average_wind = $parsed_json->{'forecast'}->{'simpleforecast'}->{'forecastday'}[3]->{'avewind'}->{'mph'};
	  	$three_gust = $parsed_json->{'forecast'}->{'simpleforecast'}->{'forecastday'}[3]->{'maxwind'}->{'mph'};
	  	$three_pop = $parsed_json->{'forecast'}->{'simpleforecast'}->{'forecastday'}[3]->{'pop'};
	

		$today_conditions = "";
		$one_conditions = "";
		$two_conditions = "";
		$three_conditions = "";
		

		//TODAY
		if(in_array($today_icon,$tstorm_strs)){


			$today_conditions .= "1";


		}
		elseif(in_array($today_icon,$rain_strs)){


			$today_conditions .= "2";

		}
		elseif(in_array($today_icon,$snowy_strs)){


			$today_conditions .= "3";


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
			return NULL;
			
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

		if(intval($today_gust) <= 9){
			$today_conditions .= "9";

		}
		elseif(intval($today_gust) <= 19){
			$today_conditions .= "9";

		}
		elseif(intval($today_gust) <= 29){
			$today_conditions .= "9";

		}
		elseif(intval($today_gust) >= 30){
			$today_conditions .= "9";

		}
		else{
			$today_conditions .= "0";

		}






		//NEXT DAY
		if(in_array($one_icon,$tstorm_strs)){

			if (strpos($one_icon,'chance') !== false) {
    			
    				if(intval($one_pop) > 60){
    					$one_conditions .= "1";
    				}
    				else{
    					$one_conditions .= "4";
    				}
			}
			else{

				$one_conditions .= "1";
			}

		}
		elseif(in_array($one_icon,$rain_strs)){

			if (strpos($one_icon,'chance') !== false) {
    			
    			if(intval($one_pop) > 60){
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
    			
    			if(intval($one_pop) > 60){
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
			return NULL;
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

		if(intval($one_gust) <= 9){
			$one_conditions .= "9";
		}
		elseif(intval($one_gust) <= 19){
			$one_conditions .= "9";
		}
		elseif(intval($one_gust) <= 29){
			$one_conditions .= "9";
		}
		elseif(intval($one_gust) >= 30){
			$one_conditions .= "9";
		}
		else{
			$one_conditions .= "0";
			
		}
		
		
		

		//2ND DAY
		if(in_array($two_icon,$tstorm_strs)){

			if (strpos($two_icon,'chance') !== false) {
    			
    			if(intval($two_pop) > 60){
    				$two_conditions .= "1";
    			}
    			else{
    				$two_conditions .= "4";
    			}
			}
			else{

				$two_conditions .= "1";
			}

		}
		elseif(in_array($two_icon,$rain_strs)){

			if (strpos($two_icon,'chance') !== false) {
    			
    			if(intval($two_pop) > 60){
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
    			
    			if(intval($two_pop) > 60){
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
			return NULL;
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

		if(intval($two_gust) <= 9){
			$two_conditions .= "9";
		}
		elseif(intval($two_gust) <= 19){
			$two_conditions .= "9";
		}
		elseif(intval($two_gust) <= 29){
			$two_conditions .= "9";
		}
		elseif(intval($two_gust) >= 30){
			$two_conditions .= "9";
		}
		else{
			$two_conditions .= "0";
		}



		//3RD DAY
		if(in_array($three_icon,$tstorm_strs)){

			if (strpos($three_icon,'chance') !== false) {
    			
    			if(intval($three_pop) > 60){
    				$three_conditions .= "1";
    			}
    			else{
    				$three_conditions .= "4";
    			}
			}
			else{

				$three_conditions .= "1";
			}

		}
		elseif(in_array($three_icon,$rain_strs)){

			if (strpos($three_icon,'chance') !== false) {
    			
    			if(intval($three_pop) > 60){
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
    			
    			if(intval($three_pop) > 60){
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
			return NULL;
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

		if(intval($three_gust) <= 9){
			$three_conditions .= "9";
		}
		elseif(intval($three_gust) <= 19){
			$three_conditions .= "9";
		}
		elseif(intval($three_gust) <= 29){
			$three_conditions .= "9";
		}
		elseif(intval($three_gust) >= 30){
			$three_conditions .= "9";
		}
		else{
			$three_conditions .= "0";
		}
		
		return $today_conditions . $one_conditions . $two_conditions . $three_conditions;
					
	}
	
	
	
	$zipcode = "";
	if (isset($_GET['zip'])) {
		$zipcode = $_GET['zip'];
	}

	$dataToReturn = "";
	
	if($zipcode !== ""){

		//echo "got zip\n";
	
		$reading = fopen('current.txt','r');
		$writing = fopen('current.tmp','w');
		$found = false;
		$today = date("F j, Y, g:i a");
		
		while(!feof($reading)){
		
			//echo "looping through lines\n";
			$line = trim(fgets($reading));
			
			$parts = explode("\t",$line);
			if(count($parts) == 3 && $parts[0] == $zipcode){
			
				//Found the last data for zip code
				//echo "found last data\n";
				$found = true;
				$data = $parts[2];
				$timestamp = strtotime($parts[1]);
				$minutes = (strtotime($today) - $timestamp)/60;
				//echo "\tts: " . $timestamp . "\n";
				//echo "\ttoday: " . $today . "\n";
				//echo "\tmins: " . $minutes . "\n";
				
				if($minutes < 6){
				
					//echo "data not 60 mins old, sending\n";
					$dataToReturn = $data;
					
				}
				else{
					
					//echo "need to update old data\n";
					//Need to refresh data from Wunderground APIs
					$dataToReturn = get_new_data($zipcode);
					if($dataToReturn == NULL){
						$dataToReturn = $data;
					}
					
					$line = $parts[0] . "\t" . $today . "\t" . $dataToReturn . "\n";
						
						
					
				}
				
			}
			
			if(strlen($line) > 0){
				fputs($writing,$line."\n");
			}
		
		}
		
		if(!$found){

			//echo "no data for zip, need to update\n";
		
			$dataToReturn = get_new_data($zipcode);
			
			if($dataToReturn){
				$newLine = $zipcode . "\t" . $today . "\t" . $dataToReturn . "\n";
				fputs($writing,$newLine);
			}
				
		}
			
		fclose($writing);
		fclose($reading);
		rename('current.tmp','current.txt');
					
	}
	//$dataToReturn = "519729339149";

	if($dataToReturn){				
    		echo '#' . time() . '#' . '<' . $dataToReturn . '>';
    	}
    	
?>