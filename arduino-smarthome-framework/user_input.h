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
const char* WIFI_SSID = "YOUR_SSID";         // Your WiFi SSID
const char* WIFI_PASSWORD = "YOUR_PASSWORD"; // Your WiFi Password  
const char* MQTT_SERVER = "192.160.1.100"; // Your MQTT Broker IP Address
#define BAUDRATE 115200 // Baudrate for serial communication

/*
 
   _______  __    _    __  __ ____  _     _____ 
  | ____\ \/ /   / \  |  \/  |  _ \| |   | ____|
  |  _|  \  /   / _ \ | |\/| | |_) | |   |  _|  
  | |___ /  \  / ___ \| |  | |  __/| |___| |___ 
  |_____/_/\_\/_/   \_\_|  |_|_|   |_____|_____|
                                                
 
*

Write this in the topic section:
-----------------------------

void onLedCommand(String payload) {
  if (payload == "ON") digitalWrite(2, HIGH);
  else digitalWrite(2, LOW);
  }
---------------------------

Write this in the user setup:
--------------------------------
void user_setup() {
  pinMode(2, OUTPUT);
  mqtt_subscribe("home/test/led", onLedCommand);
}
-------------------------------

Serial and wifi connection is already handled in the framework, you may change
the baudrate in the framework types file


*/

/*
 
   _____           _          
  |_   _|__  _ __ (_) ___ ___ 
    | |/ _ \| '_ \| |/ __/ __|
    | | (_) | |_) | | (__\__ \
    |_|\___/| .__/|_|\___|___/
            |_|               
 
*/
#define MAX_TOPICS 10 //max number of mqtt topics to be used

void onLedCommand(String payload) {
  if (payload == "ON") digitalWrite(2, HIGH);
  else digitalWrite(2, LOW);
  }

/*
 
      _            _       _               _                        ______       _                
     / \   _ __ __| |_   _(_)_ __   ___   | |    ___   ___  _ __   / / ___|  ___| |_ _   _ _ __   
    / _ \ | '__/ _` | | | | | '_ \ / _ \  | |   / _ \ / _ \| '_ \ / /\___ \ / _ \ __| | | | '_ \  
   / ___ \| | | (_| | |_| | | | | | (_) | | |__| (_) | (_) | |_) / /  ___) |  __/ |_| |_| | |_) | 
  /_/   \_\_|  \__,_|\__,_|_|_| |_|\___/  |_____\___/ \___/| .__/_/  |____/ \___|\__|\__,_| .__/  
                                                           |_|                            |_|     
 
*/

void user_setup() {
  // Add your setup code here
  pinMode(2, OUTPUT);
  mqtt_subscribe("home/test/led", onLedCommand);
}

void user_loop() {
  // Add your loop code here
}
