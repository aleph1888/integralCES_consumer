<?php

namespace integralCES;

require_once 'OAuth.inc';

class tokenTool {

	const DELIMITER = '[]';

	public $request_token;	// @vars OAuth\OAuthToken
	public $access_token;	// @vars OAtyh\OAuthToken

	function __construct() {
	
		$this->request_token = $this->get_token( 'request_token' );
		$this->access_token = $this->get_token( 'access_token' );
					
	}


	/**
	 *
	 * Saves token in a cookie
	 *
	 * @params string $token_type A value in { 'request_token', 'access_token' }
	 * @params string $key
	 * @params string $secret
	 */
	private function save_token( $token_type, $key, $secret ) {

                $info = $key . self::DELIMITER . $secret;
                setcookie( $token_type, $info, time() + 12000, '/');

	}


        /**
	 * Retrieves a stored token see save_token()
         *
         * @params string $token_type A value in { 'request_token', 'access_token' }
	 * 
	 * @returns OAuthToken or NULL
	 */
        private function get_token( $token_type ) {

		if ( ! isset( $_COOKIE[$token_type] ) )
			return NULL;

		$cookie = $_COOKIE[$token_type];

		if ( $cookie ) {
			$token = explode( self::DELIMITER, $cookie );
	                return new \OAuthToken( $token[0], $token[1] );
		} else {
			return NULL;
		}

        }


	/**
         *
         * @uses save_token()
         *
         * @params string $key
         * @params string $secret
         */
        public function save_request_token( $key, $secret ) {
	
		$this->save_token( 'request_token', $key, $secret );

        }


        /**
         *
         * @uses save_token()
         *
         * @params string $key
         * @params string $secret
         */
        public function save_access_token( $key, $secret ) {

                $this->save_token( 'access_token', $key, $secret );

        }


	 /**
         *
         * @uses get_token()
         *
         * @params string $key
         * @params string $secret
         */
        public function get_request_token() {

                return $this->get_token( 'request_token' );

        }


        /**
         *
         * @uses get_token()
         *
         * @params string $key
         * @params string $secret
         */
        public function get_access_token() {
	
                $access_token = $this->get_token( 'access_token' );

		return $access_token;

        }

}
