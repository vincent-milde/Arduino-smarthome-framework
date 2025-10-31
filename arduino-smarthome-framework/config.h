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
 * ADD SOME NICE WORDS HERE FOR THE USER
 * 
 * 
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

#define WIFI_SSID     "YOUR SSID"              // Your WiFi SSID
#define WIFI_PASSWORD "YOUR PASSWORD"          // Your WiFi Password  

/*
 
   __  __  ___ _____ _____   ____       _   _   _                 
  |  \/  |/ _ \_   _|_   _| / ___|  ___| |_| |_(_)_ __   __ _ ___ 
  | |\/| | | | || |   | |   \___ \ / _ \ __| __| | '_ \ / _` / __|
  | |  | | |_| || |   | |    ___) |  __/ |_| |_| | | | | (_| \__ \
  |_|  |_|\__\_\|_|   |_|   |____/ \___|\__|\__|_|_| |_|\__, |___/
                                                        |___/     
//
//  If you do not use a custom username and password for your mqtt broker
//  leave these commented out
//
//  Default Home assistant MQTT-broker is 192.168.1.10
//  Default Home assistant port is 1883
//

*/

#define MQTT_SERVER   "192.168.1.10"        // Your MQTT Broker IP Address
#define MQTT_PORT 1883                      // Your MQTT Broker Port  
//#define MQTT_USER     "homeassistant"     // optional
//#define MQTT_PASSWORD "yourpassword"      // optional
 

/*
 
   ____             _                     _   _   _                 
  |  _ \  _____   _(_) ___ ___   ___  ___| |_| |_(_)_ __   __ _ ___ 
  | | | |/ _ \ \ / / |/ __/ _ \ / __|/ _ \ __| __| | '_ \ / _` / __|
  | |_| |  __/\ V /| | (_|  __/ \__ \  __/ |_| |_| | | | | (_| \__ \
  |____/ \___| \_/ |_|\___\___| |___/\___|\__|\__|_|_| |_|\__, |___/
                                                          |___/     
 
*/

#define DEVICE_NAME "YOUR NAME" // Name of the device
#define DEVICE_ID   "YOUR ID"   // Unique ID of the device
#define DEVICE_ROOM "YOUR ROOM" // Room where the device is located


/* ===============================================================
   DEVICE TYPE CONFIGURATION

   Supported options:
     "sensor"     - For devices that publish values (temperature, etc.)
     "switch"     - For controllable outputs (relays, LEDs)
     "light"      - For dimmable or RGB lights
     "custom"     - For manual MQTT handling (advanced users) -> edit custom.h

   Uncomment ONE of the following:
   =============================================================== */

//#define DEVICE_TYPE "sensor"
//#define DEVICE_TYPE "switch"
//#define DEVICE_TYPE "light"
    //#define LIGHT_PIN 2   // Define the pin connected to the light
    //define BRIGHTNESS_SUPPORT // Uncomment to enable brightness control
    //define COLOR_SUPPORT      // Uncomment to enable color control (RGB lights)
//#define DEVICE_TYPE "custom"

/*
 
   _____                                            _     
  |  ___| __ __ _ _ __ ___   _____      _____  _ __| | __ 
  | |_ | '__/ _` | '_ ` _ \ / _ \ \ /\ / / _ \| '__| |/ / 
  |  _|| | | (_| | | | | | |  __/\ V  V / (_) | |  |   <  
  |_|  |_|  \__,_|_| |_| |_|\___| \_/\_/ \___/|_|  |_|\_\ 
                                                          
 
*/

/* ===============================================================
   HOME ASSISTANT AUTO DISCOVERY
   Enables automatic entity registration in Home Assistant.

   Notes:
   - Requires Home Assistant MQTT Discovery enabled.
   - Works best with DEVICE_TYPE set to "sensor", "switch", or "light".
   - "custom" devices must define their own topics manually.
   =============================================================== */
   
#define AUTO_DISCOVERY     //enable or disable home assistant auto discovery
#define MAX_TOPICS 10      //max number of mqtt topics to be used
#define BAUDRATE 115200    // Baudrate for serial communication




/* -------------- END OF CONFIGURATION -------------- */

/*
 
   _   _  ___    _____ ____ ___ _____ 
  | \ | |/ _ \  | ____|  _ \_ _|_   _|
  |  \| | | | | |  _| | | | | |  | |  
  | |\  | |_| | | |___| |_| | |  | |  
  |_| \_|\___/  |_____|____/___| |_|  
                                      
 
*/

// ==================== DEVICE TYPE LOGIC ====================
#if defined(DEVICE_TYPE)

  #if strcmp(DEVICE_TYPE, "sensor") == 0
    #include "device_sensor.h"
  
  #elif strcmp(DEVICE_TYPE, "switch") == 0
    #include "device_switch.h"
  
  #elif strcmp(DEVICE_TYPE, "light") == 0
    #include "device_light.h"

  #elif strcmp(DEVICE_TYPE, "custom") == 0
    #include "device_custom.h"

  #else
    #error "Unsupported DEVICE_TYPE! Allowed: sensor, switch, light, custom"
  #endif

#else
  #error "DEVICE_TYPE not defined! Please define it in config.h"
#endif

