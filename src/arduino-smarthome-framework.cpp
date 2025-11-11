#include <Arduino.h>
#include "config.h"

void setup(){
    framework_setup();
    user_setup();
} 

void loop(){
    framework_loop();
    user_loop() ;
}