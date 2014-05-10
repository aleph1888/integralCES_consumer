<?php

include '../../myAPI.inc';

$api = myAPI::get_instance();

$api->get_access_token( $_GET );

Header( "Location: " . $api->Config->callback_url );

exit();
