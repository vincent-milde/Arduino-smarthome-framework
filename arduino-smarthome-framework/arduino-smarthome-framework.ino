#include "user_programm.h"

void setup(){
    framework_setup();
    user_setup();
}

void loop(){
    framework_loop();
    user_loop() ;
}