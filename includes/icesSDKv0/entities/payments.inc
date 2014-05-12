<?php

namespace integralCES;

Class Payment {

	public $id;

	public $buyer_account;		// @vars ingregralCES\Account
	public $seller_account;		// @vars integralCES\Account	
	public $amount;			// @vars int	Amount in cents
	public $concept;		// @vars string	

	public $state;
	public $result;	// @vars string, to set if $state = 11
	

	function __construct( $data = NULL ) {

		if ( $data ) {
		        $this->id = $data->id;
        	        $this->buyer_account = $data->buyer_account;
                	$this->seller_account = $data->seller_account;

	                $this->amount = $data->amount;
	                $this->concept = $data->concept;
        	        $this->state = $data->state;
		}

	}


	/**
	 *
	 * @param int $state A valid TRANSFER_INTERFACE::STATE
	 *
	 * @returns string Returns related literal
	 */
	public function get_state() {

	    $string = array(
	      0 => 'NEW',
	      1 => 'TRIGGERED',
	      2 => 'ACCEPTED',
	      3 => 'COMMITTED',
	      4 => 'ARCHIVED',
	      5 => 'REJECTED',
	      6 => 'DISCARDED',
	      7 => 'REVOKE TRIGGERED',
	      8 => 'REVOKE ACCEPTED',
	      9 => 'REVOKE REJECTED',
	      10 => 'REVOKED',
	      11 => 'ERROR',
	    );

    	    return $string[ (int)$this->state ];

  	}

}


Class Payments {

	private $api;	// @vars integralCES\cesApi


	public function __construct ( $api ) {
		
		$this->api = $api;

	}	


	/**
	*
	* Process a $payment request creation and returns same object with "id" field setted.
	*
 	* @params integralCES\Payment
	*
	* @return integralCES\Payment
	*/
        function create( $payment ) {

                $restTool = new restTool( $this->api->Config );

        	$error = "";
        	if ( ! isset( $payment->buyer_account->name ) )
                	$error = "Missing or invalid buyer account. ";
        	
        	if ( ! isset( $payment->seller_account->name ) )
                	$error .= "Missing or invalid seller account. ";

        	if ( $error ) {
                	$payment->state = 11;
                	$payment->result = $error;
                	$payment->id= 0;
			return $payment;
		}

                $params["buyer_account"] = $payment->buyer_account->name;
                $params["seller_account"] = $payment->seller_account->name;
                $params["amount"] = $payment->amount;
                $params["concept"] = $payment->concept;

		$result = $restTool->execQuery( 'interop', $params, "POST"  );

                $json_result = json_decode( $result );

		// If we cannot cast into a json, then assume getting a php exception
		if ( isset( $json_result ) ) {

			$payment->id = $json_result->id;
		        $payment->result = $json_result->result;
	                $payment->state = $json_result->state;

			//Update balances
			$payment->buyer_account = $this->api->Accounts->get( $payment->buyer_account->name );
			$payment->seller_account = $this->api->Accounts->get( $payment->seller_account->name );

		} else {

			$payment->id = 0;
		        $payment->result = $error;
	                $payment->state = 11;

		}

                return $payment;

        }


	/**
	 *
	 * @params int $transaction_id	
	 *
	 * @returns integralCES\Payment entity
	 */
        public function get( $transaction_id ) {

		$restTool = new restTool( $this->api->Config );

                $params = array( 	
				'type'=>'payment', 
				'id' => $transaction_id 
			);

                $result = $restTool->execQuery( 'interop', $params, "GET" );

		if ( $result )
                	return new Payment( json_decode( $result ) );
		else 
			return NULL;

        }	

}
