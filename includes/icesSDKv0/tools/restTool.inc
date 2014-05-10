<?php

namespace integralCES;

require_once 'OAuth.inc';
require_once 'tokenTool.inc';

class restTool {

	private $Config;			// @var integralCES\Configuration

	private $request_token_url;		// @var string Ces server OAuth request token url
	private $authorization_url;		// @var string Ces server OAuth authentification url
	private $acces_token_url;		// @var string Ces server OAUth acces token url

	private $gateway_url;			// @var string Ces server End point
	public $tokenTool;			// @var integralCES\tokenTool
	

	function __construct( $Config ) {

		// API entry point
		$this->Config = $Config;

		// OAuth urls
		$base_url = $Config->base_url;
		$this->request_token_url = $base_url . '/oauth/request_token';
		$this->authorization_url = $base_url . '/oauth/authorize';
		$this->access_token_url = $base_url . '/oauth/access_token';

		// Server endpoint
		$this->gateway_url = $base_url . '/gateway';

		// token helper
		$this->tokenTool = new tokenTool();

	}

	/**
	 * Perform some validation son current access_token
	 *
	 * @params OAuth\OAauthToken
	 *
	 * @returns bool
	 */
	function is_valid_token( $token ) {
		
		//TODO
		Return TRUE;

	}


	/**
	 *
	 * Returns if exists a valid access token
	 *
	 * @returns bool
	 */
	function exists_acess_token() {

		$token = $this->tokenTool->get_access_token();
		return $this->is_valid_token( $token );
		
	}


	/**
	 *
	 * Request a token to CES Server,
	 * then returns CES Server authorization login,
	 * then redirectos to $site_callback_url.
	 *
	 * $params string $callback_url Where application will be redirected after user login in returned url.
	 *
	 * $returns string curl_error if request token fails, else authorization url.
	 */
	public function get_authorization_url( $callback_url ) {
		
		// Establish an OAuth consumer based on consumer key and secret
		$key = $this->Config->consumer_key;
		$secret = $this->Config->consumer_secret;
		$sig_method = new \OAuthSignatureMethod_HMAC_SHA1();
   		$oauth_consumer = new \OAuthConsumer( $key, $secret );

   		// create an OAuth Request to obtain request token
    		$oauth_params = array( "oauth_callback" => $this->Config->callback_url );
		$tokenRequest = \OAuthRequest::from_consumer_and_token(
						$oauth_consumer, 
						NULL, 
						'GET', 
						$this->request_token_url, 
						$oauth_params
				);

   		// sign the request using HMAC_SHA1 signature method
   		$tokenRequest->sign_request( $sig_method, $oauth_consumer, NULL);

    		// convert the request parameters to a URL
    		$token_request_url = $tokenRequest->to_url();

		// excecute request for a request token / secret
    		$session = curl_init($token_request_url);

		curl_setopt($session, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($session, CURLOPT_SSL_VERIFYPEER, 0);

		$response = curl_exec($session);

		if ( ! $response ) {
			$response = curl_error($session);
        		return $response;

    		}
    		curl_close($session);

		// retrieve the returned request token and request secret
    		parse_str($response, $params);
    		$oauth_token = $params['oauth_token'];
    		$oauth_token_secret = $params['oauth_token_secret'];

		// saves the request token and secret to be used after the user has provided access
		$this->tokenTool->save_request_token( $oauth_token, $oauth_token_secret );

		// genereate authorization url
		$oauth_url = $this->authorization_url . 
				'?oauth_token=' . $oauth_token . 
				'&oauth_callback=' . urlencode( $callback_url );
		
		return $oauth_url;

	}

	/**
	 * 
	 * Gets $oauth_verifier and request for an access_token, then saves it on cookie
	 *
	 * @params string $oauth_verifier Authorization token after user logsin on CES Serve
	 */
	public function get_access_token( $oauth_verifier ) {
		
		// Create OAuth request for an access token
                $key = $this->Config->consumer_key;
                $secret = $this->Config->consumer_secret;
		$sig_method = new \OAuthSignatureMethod_HMAC_SHA1();

                $oauth_consumer = new \OAuthConsumer( $key, $secret );
		
		$access_token = $this->tokenTool->get_request_token();

		$access_request = \OAuthRequest::from_consumer_and_token($oauth_consumer, $access_token, 'GET', $this->access_token_url);

		//Set verifier returned with the request token
		$access_request->set_parameter('oauth_verifier', $oauth_verifier);

		// Sign the request using HMAC_SHA1 signature method
		$access_request->sign_request($sig_method, $oauth_consumer, $access_token);

		// Execute access token request
		$request  = $access_request->to_url();
		$session = curl_init($request);

		curl_setopt($session, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($session, CURLOPT_SSL_VERIFYPEER, 0);

		$response = curl_exec($session);
		if ( ! $response) {
			$response = curl_error($session);
			error_log("no response");
			return $response;
    		}

		curl_close($session);

                $oauth_params = array( "oauth_callback" => $this->Config->callback_url );

   		// retrieve the access_token and access_token_secret  
    		parse_str($response, $oauth_params);

		$this->tokenTool->save_access_token( $oauth_params["oauth_token"], $oauth_params["oauth_token_secret"] );

	}


	/**
	 * Perfoms a "Consumer key + token" query against a given service resource.
	 *
	 * @params string Url path to CES Server resource from a configured entrypoint.
	 * @params array $parameters
	 * @params string $type A value in {'GET', 'POST', 'PUT'}
	 * @returns json curl response
	 */
	public function execQuery( $resource, $parameters, $type ) {
	
		// Establish an OAuth consumer based on consumer key and secret
		$key = $this->Config->consumer_key;
		$secret = $this->Config->consumer_secret;
		$url = $this->gateway_url . '/'. $resource;

 		$consumer = new \OAuthConsumer($key, $secret);
		$sig_method = new \OAuthSignatureMethod_HMAC_SHA1; 

		$req = \OAuthRequest::from_consumer_and_token($consumer, $this->tokenTool->get_access_token(), $type, $url); 
		$sig_method = new \OAuthSignatureMethod_HMAC_SHA1();
		$req->sign_request( $sig_method, $consumer, $this->tokenTool->get_access_token() );
		$req->get_normalized_http_method();

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
