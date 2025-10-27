/**************************************************************************
 * 
 * 
 * 
 * /*
 * '##:::::'##:'########:'##::::::::'######:::'#######::'##::::'##:'########:
 *  ##:'##: ##: ##.....:: ##:::::::'##... ##:'##.... ##: ###::'###: ##.....::
 *  ##: ##: ##: ##::::::: ##::::::: ##:::..:: ##:::: ##: ####'####: ##:::::::
 *  ##: ##: ##: ######::: ##::::::: ##::::::: ##:::: ##: ## ### ##: ######:::
 *  ##: ##: ##: ##...:::: ##::::::: ##::::::: ##:::: ##: ##. #: ##: ##...::::
 *  ##: ##: ##: ##::::::: ##::::::: ##::: ##: ##:::: ##: ##:.:: ##: ##:::::::
 * . ###. ###:: ########: ########:. ######::. #######:: ##:::: ##: ########:
 * :...::...:::........::........:::......::::.......:::..:::::..::........::
 * 
 * 
 * Change the settings to your own wifi and mqtt broker settings
 * add your topics to be used by Home assistant
 * provide your setup and loop function below 
 * 
 * 
 *          DO NOT USE ANY DELAYS! ONLY MILLIS LOGIC ALLOWED
 * 
 * Enjoy!
 * 
 * **************************************************************************/

/*

  __        ___  __ _            _   _   _                 
  \ \      / (_)/ _(_)  ___  ___| |_| |_(_)_ __   __ _ ___ 
   \ \ /\ / /| | |_| | / __|/ _ \ __| __| | '_ \ / _` / __|
    \ V  V / | |  _| | \__ \  __/ |_| |_| | | | | (_| \__ \
     \_/\_/  |_|_| |_| |___/\___|\__|\__|_|_| |_|\__, |___/
                                                 |___/     
 
*/

//Add your wifi password here: 
const char* WIFI_SSID = "YOUR SSID";         // Your WiFi SSID
const char* WIFI_PASSWORD = "YOUR PASSWORD";     // Your WiFi Password  
const char* MQTT_SERVER = "YOUR MQTT BROKER";       // Your MQTT Broker IP Address
#define BAUDRATE 115200              // Baudrate for serial communication

/*
 
   _____                                            _               _   _   _                 
  |  ___| __ __ _ _ __ ___   _____      _____  _ __| | __  ___  ___| |_| |_(_)_ __   __ _ ___ 
  | |_ | '__/ _` | '_ ` _ \ / _ \ \ /\ / / _ \| '__| |/ / / __|/ _ \ __| __| | '_ \ / _` / __|
  |  _|| | | (_| | | | | | |  __/\ V  V / (_) | |  |   <  \__ \  __/ |_| |_| | | | | (_| \__ \
  |_|  |_|  \__,_|_| |_| |_|\___| \_/\_/ \___/|_|  |_|\_\ |___/\___|\__|\__|_|_| |_|\__, |___/
                                                                                    |___/     
 
*/

#define MAX_TOPICS 10 //max number of mqtt topics to be used

// MQTT credentials (if used)
const char* MQTT_USER = "homeassistant";    // optional
const char* MQTT_PASSWORD = "yourpassword"; // optional

// Device identifiers for HA auto-discovery
const char* DEVICE_NAME = "esp32_led";  // unique name for the backend
const char* DEVICE_FRIENDLY_NAME = "Desk lamp"; // useful name for the frontend

