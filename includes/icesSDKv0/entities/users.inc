<?php

namespace integralCES;

Class User {

	public $id;		// @vars int 	CES identifier
	public $name;		// @vars string Account name
	public $accounts;	// @vars array of integralCES\Account


	/**
	 *
	 * @params object $ob_user An object with at least fields:
         *              (int) id,
         *      	(string) name,
         *              (array of objects) accounts [ see integralCES\Account ]
	 */
	public function __construct( $ob_user ) {

                if ( $ob_user ) {
                        $this->id = $ob_user->user_id;
                        $this->name = $ob_user->user_name;

                        foreach( $ob_user->accounts as $account )
                                $this->accounts[] = new Account( $account );
                }

	}

	
	/**
	 *
	 * @retuns string representing <ul> lists of accounts
	 */
	public function accounts_to_ul() {

		$str_output = "<ul>\n";
		$li = "<li id='%s'> Name: %s | balance: %s</li>\n";
		
		if ( $this->accounts ) {
			foreach( $this->accounts as $account )
				$str_output .= sprintf( $li, $account->id, $account->name, $account->balance );
		}
		$str_output .= "</ul>\n";
		return $str_output;

	}

}


Class Users {

	private $api;		// @vars integralCES\cesApi

	public function __construct ( $api ) {

		$this->api = $api;
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
			return new User( json_decode( $user ) );
		else
			return NULL;
	
	}


	/**
	 *
	 * @params int $user_id	
	 *
	 * @returns integralCES\User entity
	 */
        public function get( $user_id ) {

		$restTool = new restTool( $this->api->Config );

                $params = array( 	
				'type'=>'user', 
				'id' => $user_id 
			);

                $user = $restTool->execQuery( 'interop', $params, "GET" );

		if ( $user )
			return $this->load_user_from_object( json_decode( $user ) );
		else
			return NULL;


        }

}
