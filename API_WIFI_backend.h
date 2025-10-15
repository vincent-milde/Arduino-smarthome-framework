/**
 * @brief WiFi backend for the Smart Home Framework; integration of the Home Assistant API
 *  and Endpoints
 * 
 */

 // Wifi section
#include <WiFi.h>
#include <PubSubClient.h>   // Used for MQTT client
#include "user_input.h"

// MQTT client
WifiClient espClient;
PubSubClient client(espClient);

/*
 
   __  __  ___ _____ _____   _____                 _   _                 
  |  \/  |/ _ \_   _|_   _| |  ___|   _ _ __   ___| |_(_) ___  _ __  ___ 
  | |\/| | | | || |   | |   | |_ | | | | '_ \ / __| __| |/ _ \| '_ \/ __|
  | |  | | |_| || |   | |   |  _|| |_| | | | | (__| |_| | (_) | | | \__ \
  |_|  |_|\__\_\|_|   |_|   |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
                                                                         
 
*/ 
/*
subscribe
callback
reconnect
setup
loop
*/