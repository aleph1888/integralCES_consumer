<?php
require_once drupal_get_path('module', 'ces_bank') . '/ces_bank.logic.inc';
require_once drupal_get_path('module', 'ces_bank') . '/ces_bank.module';

function _ces_bank_index ( $type, $id ) {

	if ( $type == "user" ) {
		$bank = new bank();
		$account = $bank->getAccount( (int)$id );
		return array(
		 "id" => $account["id"],
		 "name" => $account["name"],
		 "balance" => $account["balance"]);
		
	} else {
	
		$bank = new bank();
		$transaction = $bank->ces_transaction_load( (int) $id );		
		return array(
			"id" =>  $transaction["id"],
			"buyer" => $bank->getTransactionToAccount( $transaction )["id"],
			"seller" => $bank->getTransactionFromAccount( $transaction )["id"],
			"amount" => $transaction["amount"],
			"concept" => $transaction["concept"],
			"state" => $transaction["state"]
		);
	}


}

function _ces_bank_create( $params ) {
	
	$bank = new bank();
	$record = array(
		"fromaccount" => $params["buyer"],
		"toaccount" => $params["seller"],
		"amount" => $params["amount"],
		"concept" => $params["concept"],		
		);
	$bank->createTransaction( $record );
	$result = $bank->applyTransaction( $record['id'] );
	$bank = new bank();
	$transaction = $bank->getTransaction( $record['id'] );
	return array( 
		"id" => $record['id'], 
		"result" => $result, 
		"state" => $transaction["state"] 
		);
}

function _ces_bank_update( $params ) {

	$bank = new bank();
	$result = $bank->applyTransaction( $params['id'] );

	return $result;

}

function _ces_bank_access( $op ) {
error_log($op);
         return true;

}

?>
