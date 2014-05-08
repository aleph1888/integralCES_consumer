<?php

namespace integralCES;

require_once 'OAuth.inc';

class restTool {


	private $api;

	function __construct( $api ) {

		$this->api = $api;

	}

	public function execQuery( $resource, $parameters, $type ) {

	
		// Establish an OAuth consumer based on consumer key and secret
		$key = $this->api->Config->consumerKey;
		$secret = $this->api->Config->consumerSecret;
		$url = $this->api->Config->baseUrl . '/'. $resource;

 
		$consumer = new \OAuthConsumer($key, $secret);
		$sig_method = new \OAuthSignatureMethod_HMAC_SHA1; 
 
		$req = \OAuthRequest::from_consumer_and_token($consumer, null, $type, $url, null); 
		$sig_method = new \OAuthSignatureMethod_HMAC_SHA1(); // Línea para oAuth
		$req->sign_request($sig_method, $consumer, null);  // Línea para oAuth
 
		//Perform query
		$ch = curl_init();
		$url = $req->to_url();

		curl_setopt($ch,CURLOPT_RETURNTRANSFER,true);
		curl_setopt($ch, CURLOPT_VERBOSE, 1);
	 
		if( $type == "POST" ){
 
			curl_setopt($ch, CURLOPT_POST, 1);
			curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode( $parameters ) );  //añadimos el contenido en JSON
			curl_setopt($ch, CURLOPT_HTTPHEADER, array('Content-type: application/json')); //añadimos el header para que acepte el JSON
		}
		else if( $type == "GET" ){
			$get_str = "";
	                foreach( $parameters as $key=>$val )
        	              $get_str .= $key.'='. urlencode($val).'&';
	                $get_str = substr($get_str, 0, -1);
	                $url .= '&' . $get_str;
	                curl_setopt($ch, CURLOPT_POST, false);
	                curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "GET");
		}
		else if($type == "PUT"){
 
			curl_setopt($ch, CURLOPT_POST, 1);
			curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "PUT");
			curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode( $parameters ) ); //añadimos el contenido en JSON
			curl_setopt($ch, CURLOPT_HTTPHEADER, array('Content-type: application/json')); //añadimos el header para que acepte el JSON
 
		}
	
		curl_setopt($ch, CURLOPT_URL, $url);

		$response = curl_exec($ch);

		return $response;

	}

}
