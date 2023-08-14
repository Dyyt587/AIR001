#include "Wire.h"

void setup(){
 Wire.begin();
 Serial.begin(115200);
 Serial.println("h");
}

void loop(){

    Wire.beginTransmission(4);
    Wire.write("12\r\n");

    Serial.println(Wire.endTransmission(true));

     Serial.println("h");
    delay(1000);


}