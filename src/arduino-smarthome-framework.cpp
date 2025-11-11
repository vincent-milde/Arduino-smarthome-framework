#include <Arduino.h>
#include "config.h"

void setup(){
    framework_setup();
    device_setup();
} 

void loop(){
    framework_loop();
    device_loop() ;
}