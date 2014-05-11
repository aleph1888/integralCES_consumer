integralCES_consumer
====================

Provides payment gateway API for http://integralces.net

Whats this for
----------------------------------
A [PHP API](https://github.com/aleph1888/integralCES_consumer/tree/master/includes/icesSDKv0) for [IntegralCES_interop](https://github.com/aleph1888/integralCES_interop). 

Visit its Drupal integralCES [issue](https://drupal.org/project/issues/1367140).


API & consumer
-------------
This consumer is just an example, you don't need to take it all, just the API [icesSDKv0](https://github.com/aleph1888/integralCES_consumer/tree/master/includes/icesSDKv0).


Procedure
--------------
1) [Wrap API entrypoint](https://github.com/aleph1888/integralCES_consumer/blob/master/includes/myAPI.inc) with your own singleton class and configure it; you will need to [get in contact](integralces.net) to receive credentials.

2) Get the singleton and check for valid Client (representing your website) connection.
<pre>
 $api = myAPI::get_instance();
</pre>

Check for a valid connection before start making querys:
<pre>
 $myContextName = $api->check_client_connection();
</pre>

3) Get a login url to redirect your debited user for authentification:
<pre>
 $redirect_url = $api->get_authorization_url( $my_callback_url );
</pre>

What goes under:
a) API will generate a *request token* by making curl request saving it on a [cookie](https://github.com/aleph1888/integralCES_consumer/blob/master/includes/icesSDKv0/tools/tokenTool.inc) and then use it to return the *login url* on integralCES server.
b) After successfully login, CES Server will call your [API](https://github.com/aleph1888/integralCES_consumer/blob/master/includes/icesSDKv0/tools/requestAccessToken.php) providing an *authorization token* which will be saved in a cookie. 
c) API will then request for an *access_token* and save it on a cookie.
d) Then, API will redirect to *my_callback_url*.

4) Check for your loged user:
<pre>
 $logged_user = $api->Users->get_logged_user();
</pre>

5) Make a payment:
<pre>
 $payment = new integralCES\Payment();
 $payment->buyer = $_POST["txtBuyer"];
 $payment->seller = $_POST["txtSeller"];
 $payment->amount = $_POST["txtAmount"];
 $payment->concept = $_POST["txtConcept"];
 $payment = $api->Payments->create_payment( $payment );
</pre>

CES Server will check that your *buyer* is the same that is logged in your site.

5.1) You can check now for your payment *id*, *state*, and any *errors notification*:
<pre>
 $str_payment = sprintf( "%s, with status %s %s", $payment->id, $payment->get_state(), $payment->result );
 $html = str_replace( '%%section_payment_id%%', "Payment id: " . $str_payment, $html );
</pre>

6) Retrieve a single payment by id
<pre>
 $payment = $api->Payments->get_payment( $payment->id );
</pre> 

7) Retrieve a user by account name
<pre>
 $user = $api->Users->get_user( $my_user_name );
 $buyer = $api->Users->get_user( $payment->buyer );
 $seller = $api->Users->get_user( $payment->seller );
</pre>

See on action
----------------
Testing development [server](http://cicicdev.enredaos.net/cesinterop)


Doc
---------------
[Documentation](https://wiki.enredaos.net/index.php?title=COOPFUND-DEV#integralCES_interop)

[Developer](http://www.integralces.net/doc/developer)

Contribute
--------------
BTC @ 1DNxbBeExzv7JvXgL6Up5BSUvuY4gE8q4A
