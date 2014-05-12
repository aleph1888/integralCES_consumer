<?php
include 'includes/myAPI.inc';

// Get instance
$api = myAPI::get_instance();

// Retrieve client identification
$render_section_client_key = $api->Config->consumer_key;
$render_section_client_secret = $api->Config->consumer_secret;
$render_section_client_base_url = $api->Config->base_url;

// Check for a valid Client connection
$render_section_client_connection = $api->check_client_connection();

// Retrieve current logged in user
$logged_user = $api->Users->get_logged_user();

// Render template/index.html
$html = file_get_contents( 'templates/index.html' );

// Client section
$html = str_replace( '%%section_client_key%%', $render_section_client_key, $html );
$html = str_replace( '%%section_client_secret%%', $render_section_client_secret, $html );
$html = str_replace( '%%section_client_base_url%%', $render_section_client_base_url, $html );
$html = str_replace( '%%section_client_connection%%', $render_section_client_connection, $html );

// User section
if ( isset( $logged_user->id) ) {

	$render_section_user = $logged_user->name;
	$render_section_user_accounts = $logged_user->accounts_to_ul();
	$render_section_user_login_url = $api->Config->base_url . '/' . 'user/logout';

} else {

	$render_section_user = 'No current logged in user.';
	$render_section_user_accounts = "Login to list your accounts.";
	$render_section_user_login_url = $api->get_authorization_url( 'http://169.254.226.5/integralCES_consumer/index.php' );

}

$html = str_replace( '%%section_user%%', $render_section_user, $html );
$html = str_replace( '%%section_user_accounts%%', $render_section_user_accounts, $html );
$html = str_replace( '%%section_user_login_url%%', $render_section_user_login_url, $html );

// Process FORM
if (
	isset( $_POST["txtBuyer"] ) &&
	isset( $_POST["txtSeller"] ) &&
	isset( $_POST["txtAmount"] ) &&
	isset( $_POST["txtConcept"] )
 ) {

	// create payment
	$payment = new integralCES\Payment();

	// check for accounts, you can stop process here if not getting two valid integralCES\Account objects
	$payment->buyer_account = $api->Accounts->get( $_POST["txtBuyer"] );
	$payment->seller_account = $api->Accounts->get( $_POST["txtSeller"] );

	// fill data
        $payment->amount = $_POST["txtAmount"];
        $payment->concept = $_POST["txtConcept"];

	// create payment
	$payment = $api->Payments->create( $payment );

	// retrieve a payment example
	// $payment = $api->Payments->get( $payment->id );

	// fill template
	$str_payment = sprintf( "%s, with status %s %s", $payment->id, $payment->get_state(), $payment->result );

	$html = str_replace( '%%section_payment_id%%', "Payment id: " . $str_payment, $html );

        $buyer_account_name = ( isset( $payment->buyer_account->name ) ? $payment->buyer_account->name : $_POST["txtBuyer"] );
        $buyer_balance = ( isset( $payment->buyer_account->balance ) ? $payment->buyer_account->balance : 0  );

        $seller_account_name = ( isset( $payment->seller_account->name ) ? $payment->seller_account->name : $_POST["txtSeller"] );
        $seller_balance = ( isset( $payment->seller_account->balance ) ? $payment->seller_account->balance : 0  );

     	$html = str_replace( '%%section_payment_buyer%%', $buyer_account_name, $html );
        $html = str_replace( '%%section_payment_buyer_balance%%', $buyer_balance, $html );

     	$html = str_replace( '%%section_payment_seller%%', $seller_account_name, $html );
        $html = str_replace( '%%section_payment_seller_balance%%', $seller_balance, $html );

	$html = str_replace( '%%section_payment_amount%%', $payment->amount, $html );
	$html = str_replace( '%%section_payment_concept%%', $payment->concept, $html );

	$html = str_replace( '%%section_action%%', "Payment processed.", $html );

} else {

	// set default values and wait for submit
	$html = str_replace( '%%section_payment_id%%', "", $html );

	$html = str_replace( '%%section_payment_buyer%%', "", $html );
	$html = str_replace( '%%section_payment_buyer_balance%%', "0", $html );

	$html = str_replace( '%%section_payment_seller%%', "", $html );
	$html = str_replace( '%%section_payment_seller_balance%%', "0", $html );

	$html = str_replace( '%%section_payment_amount%%', "0", $html );
	$html = str_replace( '%%section_payment_concept%%', "Some description", $html );
	$html = str_replace( '%%section_action%%', "Fill form and make payment.", $html );

}

echo $html;

?>


