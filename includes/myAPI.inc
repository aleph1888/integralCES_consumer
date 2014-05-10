<?php

include 'icesSDKv0/cesApi.inc';

class myAPI extends integralCES\cesApi {

        static private $instance;

        public static function get_instance() {

                if ( self::$instance == null ) {

                        self::$instance = new myAPI;

			//Test server
                        self::$instance->Config->consumer_key = "6fkm9oB4wUugyTGfiBR2oxQU6HE8Z8qV";
                        self::$instance->Config->consumer_secret = "3rLQiS63Wzsx3fsjWZefCTgtcj3MWw8u";
                        self::$instance->Config->base_url = "http://cicicdev.enredaos.net/cesinterop";
                        self::$instance->Config->callback_url = "http://cicicdev.enredaos.net/integralCES_consumer/index.php";

			//Local server
                        self::$instance->Config->consumer_key = "y26emm4CCu5zF5RGP4nRQ6JHQE39KpU4";
                        self::$instance->Config->consumer_secret = "sS3pNHXThDfPW2rfPu8ydbXHuVHBJfru";
                        self::$instance->Config->base_url = "http://169.254.226.5/cesinterop";
                        self::$instance->Config->callback_url = "http://169.254.226.5/integralCES_consumer/index.php";

                }

                return self::$instance;

        }

}

