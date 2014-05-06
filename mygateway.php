<?php

include 'cesSDKv0/cesApi.inc';

class gatewayAPI extends integralCES\cesApi {

	static private $instance;

	public static function get_instance() {

		if ( self::$instance == null ) {

			self::$instance = new gatewayAPI;

			self::$instance->Config->consumerKey = "Jgjmunstxqc7LQV4bVBNXckdsW8vfuqP";
			self::$instance->Config->consumerSecret = "2HRzPZ33TMfuSPMdvZtGTF5LmbNujAh3";
			self::$instance->Config->baseUrl = "http://169.254.226.5/cesinterop/interop";

		}

		return self::$instance;

	}

}

$api =  gatewayAPI::get_instance();


$payment = new integralCES\Payment();
$payment->buyer = "0001";
$payment->seller = "0002";
$payment->amount = 5;
$payment->concept = "preuba de ppago via api";

$payment = $api->CreatePayment( $payment );

$payment = $api->GetPayment( $payment->Id );

$buyer = $api->GetUser( $payment->buyer );
$seller = $api->GetUser ($payment->seller );

var_dump("payment");
echo "<br>";
var_dump($payment);
echo "<br>";
var_dump("buyer");
echo "<br>";
var_dump($buyer);
echo "<br>";
var_dump("seller");
echo "<br>";
var_dump($seller);

?>


