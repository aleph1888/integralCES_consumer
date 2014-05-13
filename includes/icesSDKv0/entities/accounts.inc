<?php

namespace integralCES;

Class Account {

        public $id;             // @vars int
        public $name;           // @vars string
        public $balance;        // @vars int

       
	 /**
         *
         * @params object $ob_account { (int) id, (string) name, (int) balance }
         */
        function __construct( $ob_account = NULL ) {

                if ( isset( $ob_account->id ) ) {
                        $this->id = $ob_account->id;
                        $this->name = $ob_account->name;
                        $this->balance = $ob_account->balance;

                }

        }

}

Class Accounts {

        private $api;           // @vars integralCES\cesApi

        public function __construct ( $api ) {

                $this->api = $api;
        }


       /**
        *
        * @params string $account_name
        *
        * @returns integralCES\Account entity
        */
       public function get( $account_name ) {

                $restTool = new restTool( $this->api->Config );

                $params = array(
                                'type'=>'account',
                                'id' => $account_name
                        );

                $account = $restTool->execQuery( 'interop', $params, "GET" );

		if ( $account )
	                return new Account( json_decode( $account ) );
		else
			return NULL;

        }

}

