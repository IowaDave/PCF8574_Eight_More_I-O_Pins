

#include <Wire.h>
#include "EightMorePins.h"

EightMorePins myButtons;

void setup() {
  Wire.begin();

  Serial.begin (9600);
  Serial.println();
  Serial.println(myButtons.bitsAsString());  
  Serial.println("\nReady\n");
}

void loop() {

  myButtons.refresh();
  if (myButtons.changed()) {
    Serial.println(myButtons.bitsAsString());
    Serial.print("Pin #4 = ");
    Serial.println(myButtons.pin(4));
    Serial.println();
  }
}
