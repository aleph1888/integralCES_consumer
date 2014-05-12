<?php

namespace integralCES;

require_once( 'entities/configuration.inc' );
require_once( 'tools/restTool.inc' );

require_once( 'entities/payments.inc' );
require_once( 'entities/users.inc' );
require_once( 'entities/accounts.inc' );


/**
 * IntegralCES API main entry point.
 * Provides managers to connect, send and read data from IntegralCES API
 * as well as holds configuration/authorization data.
 */
class cesApi {


	/**
	 * Configuration instance
	 * @var \integralCES\Configuration
	 */
	public $Config;		// @var integralCES\Configuration

	public $Payments;	// @var integralCES\Payments
	public $Users;		// @var integralCES\Users
	public $Accounts;	// @var integralCES\Accounts

	function __construct() {

		// default config setup
		$this->Config = new configuration();
		
		// entities
		$this->Users = new Users( $this );
		$this->Payments = new Payments( $this );
		$this->Accounts = new Accounts( $this );

	}


	/**
	 *
	 * Executes a query towards CES server with Key consumer, expecting response.
	 *
	 * @return string Context name
	 */
	public function check_client_connection() {

		// token and REST manager
                $restTool = new restTool( $this->Config );
		$result = $restTool->execQuery( 'interop', array( "type" => "client", "id" => "0") , "GET" );
		return $result;
		
	}


	/**
	 *
	 * 
	 * @returns string Tokenized Authorization url in CES Server
	 */
	public function get_authorization_url( $callback_url ) {

		// token and REST manager
                $restTool = new restTool( $this->Config );
		return $restTool->get_authorization_url( $callback_url );		

	}


	/*
	*
	* To be called after get_authorization_url( $callback_url ) link is performed
 	* Will save access_token in cookie.
	*
	* @params array $oauth_verifier, commonly, just pass $_GET
	*/
	public function get_access_token( $oauth_verifier ) {

                // token and REST manager
                $restTool = new restTool( $this->Config );
                $restTool->get_access_token( $oauth_verifier );

	}


	/**
	 *
	 * @return int Buyer user id
	 */
	private function _get_logged_buyer_id() {

		if ( $this->restTool->exists_valid_token() )
			return $this->restTool->get_logged_user();
		else
			return 0;

	}
	
	/****************************************
	 *             USER                     *
  	 ****************************************/
         
	/**
	 *
         * @ return integralCES\User Current logged in user
	 */
        function getBuyerUser() {
	
		if ( is_buyer_logged() ) 			
			return $this->user->getUser( $nUserId );
		else
			return NU;

        }


}

