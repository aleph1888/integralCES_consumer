<?php

namespace integralCES;

Class Payment {

	public $Id;

	public $buyer;
	public $seller;
	public $amount;
	public $concept;

	public $state;

	public function getState($state) {
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
    	    return $string[$this->state];
  	}

}
