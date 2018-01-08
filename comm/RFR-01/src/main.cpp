#include <Arduino.h>
#include <RCSwitch.h>

RCSwitch rcSwitch = RCSwitch();

void setup() {
    Serial.begin(115200);
    rcSwitch.enableReceive(0);
}

void loop() {
    if (rcSwitch.available()) {
        Serial.print("Data arrived: ");
        Serial.println(rcSwitch.getReceivedValue());
    }
}
