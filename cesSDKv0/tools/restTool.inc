<?php

namespace integralCES;

require_once 'OAuth.inc';

class restTool {


	private $api;

	function __construct( $api ) {

		$this->api = $api;

	}

	public function execQuery( $resource, $params, $type ) {

	
		// Establish an OAuth consumer based on consumer key and secret
		$key = $this->api->Config->consumerKey;
		$secret = $this->api->Config->consumerSecret;
		$consumer = new \OAuthConsumer($key, $secret, NULL);

		// Setup OAuth request
		$url = $this->api->Config->baseUrl . '/'. $resource;

		$request = \OAuthRequest::from_consumer_and_token($consumer, NULL, $type, $url, $params);

		// Sign the constructed OAuth request using HMAC-SHA1
		$request->sign_request(new \OAuthSignatureMethod_HMAC_SHA1(), $consumer, NULL);

		// Make signed OAuth request to the API server
		$ch = curl_init();
	
		if ( $type == "POST" ) {
			curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode( $params ) );		
			curl_setopt($ch, CURLOPT_POST, true);
		} else {
			//var_dump($params);exit();
			//Url encode
			$get_str = "";
			foreach( $params as $key=>$val )  
				$get_str .= $key.'='. urlencode($val).'&'; 
			$get_str = substr($get_str, 0, -1); 
			$url .= '?' . $get_str;
			curl_setopt($ch, CURLOPT_POST, false);
		}

		curl_setopt($ch, CURLOPT_URL, $url);

		curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
		curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1);
		curl_setopt($ch, CURLOPT_HTTPHEADER, array( 'Content-Type: application/json', $request->to_header() ) ); 
		$response = curl_exec($ch);
		$info = curl_getinfo($ch);
		curl_close($ch);

		return $response;

	}

}
