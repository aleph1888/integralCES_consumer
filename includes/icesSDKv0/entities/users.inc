<?php

namespace integralCES;

Class User {

	public $id;		// @vars int 	CES identifier
	public $balance;	// @vars int	Account balance
	public $exchange;	// @vars string Account exchange 4-digit
	public $name;		// @vars string Account name

}

Class Users {

	private $api;		// @vars integralCES\cesApi

	public function __construct ( $api ) {

		$this->api = $api;
	}


	/**
	 *
	 * @params object $ob_user An object with at least fields {id, balance, name}
	 *
	 * @returns integraCES\User entity
	 */
	public function load_user_from_object( $ob_user ) {

	       	$user = new User();

		if ( $ob_user ) {
	                $user->id = $ob_user->id;
        	        $user->balance = $ob_user->balance;
              		$user->exchange = substr( $ob_user->name, 0, 4 );	
              		$user->name = substr( $ob_user->name, 4 );
		}

                return $user;
		
	}


	/** 
	 *
	 * Checks for current user logged based on token management.
	 *
	 * @returns integralCES\User entity
	 */
	public function get_logged_user() {

		$tokenTool = new tokenTool();

		if ( ! $tokenTool->get_access_token() )
			return NULL;

		$restTool = new restTool( $this->api->Config );

		$params = array( "type" => "logged", "id"=> "0" );
		$user = $restTool->execQuery( 'interop', $params, "GET" );

		if ( $user)
			return $this->load_user_from_object( json_decode( $user ) );
		else
			return NULL;
	
	}


	/**
	 *
	 * @params int $user_id	
	 *
	 * @returns integralCES\User entity
	 */
        public function get_user( $user_id ) {

		$restTool = new restTool( $this->api->Config );

                $params = array( 	
				'type'=>'user', 
				'id' => $user_id 
			);

                $user = $restTool->execQuery( 'interop', $params, "GET" );

		return $this->load_user_from_object( json_decode( $user ) );

        }

}
