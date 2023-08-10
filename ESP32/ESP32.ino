#include "Wire.h"

void setup(){
 Wire.begin(8);
 Serial.begin(115200);
}

void loop(){
          Serial.println("hello1");
    Wire.requestFrom(8, 1, false);
      Serial.println("hello2");
    Serial.print(Wire.read());
    Wire.beginTransmission(8);
    Wire.write("hello World");
    Serial.print("  ");
    Serial.println(Wire.endTransmission(true));


    delay(1000);


}