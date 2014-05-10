<?php

namespace integralCES;

Class Payment {

	public $id;

	public $buyer;		// @vars string Four digits (fill with 0)
	public $seller;		// @vars string Four digits (fill with 0)	
	public $amount;		// @vars int	Amount in cents
	public $concept;	// @vars string	

	public $state;
	public $result;	// @vars string, to set if $state = 11
	
	function __construct( $data = NULL ) {

		if ( $data ) {
		        $this->id = $data->id;
        	        $this->buyer = $data->buyer;
                	$this->seller = $data->seller;

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
        function create_payment( $payment ) {

                $restTool = new restTool( $this->api->Config );
                $params = (array) $payment;

		$result = $restTool->execQuery( 'interop', $params, "POST"  );

                $json_result = json_decode($result);

		// If we cannot cast into a json, then assume getting a php exception
		if ( isset( $json_result ) ) {
			$payment->id = $json_result->id;
		        $payment->result = $json_result->result;
	                $payment->state = $json_result->state;
		} else {
			// TODO: whats this?
			$error = explode( "=>", $result)[1];
			$error = str_replace ('<em class="placeholder">', '', $error );
			$error = str_replace ('</em>', '', $error );
			$error = explode( '"', $error )[1];			

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
        public function get_payment( $transaction_id ) {

		$restTool = new restTool( $this->api->Config );

                $params = array( 	
				'type'=>'payment', 
				'id' => $transaction_id 
			);

                $result = $restTool->execQuery( 'interop', $params, "GET" );

		
                $payment = new Payment( json_decode( $result ) );

                return $payment;

        }	

}
