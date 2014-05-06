<?php

namespace integralCES;

require_once( 'entities/configuration.inc' );
require_once( 'tools/restTool.inc' );
require_once( 'entities/payment.inc' );
require_once( 'entities/user.inc' );


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
	public $Config;

	function __construct() {

		// default config setup
		$this->Config = new configuration();

	}

	function CreatePayment( $payment ) {

		$restTool = new restTool( $this );
		$params = (array) $payment;

		$result = $restTool->execQuery( 'interop', $params, "POST"  );
		$result = json_decode($result);

		$payment->result = $result->result;
		$payment->Id = $result->id;
		$payment->state = $result->state;
		return $payment;

	}

	function GetPayment( $paymentId ) {

		$restTool = new restTool( $this );

		$params = array( 'type'=>'payment', 'id' =>  $paymentId );

		$result = $restTool->execQuery( 'interop', $params, "GET" );
		$result = json_decode( $result );

		$payment = new Payment();
		$payment->Id = $result->id;
		$payment->buyer = $result->buyer;
		$payment->seller = $result->seller;

		$payment->amount = $result->amount ;
		$payment->concept = $result->concept ;
		$payment->state = $result->state ;

		return $payment;

	}

	function getUser( $userId ) {

        	$restTool = new restTool( $this );

                $params = array( 'type'=>'user', 'id' =>  $userId );

                $result = $restTool->execQuery( 'interop', $params, "GET" );
                $result = json_decode( $result );

                $user = new User();
                $user->Id = $result->id;
                $user->balance = $result->balance;
		$user->name = $result->name;

                return $user;

	}


}

