 #include "API_backend.h"


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

void onLedCommand(String payload) {
  if (payload == "ON") {
    digitalWrite(2, HIGH);
  } else {
    digitalWrite(2, LOW);
  }
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