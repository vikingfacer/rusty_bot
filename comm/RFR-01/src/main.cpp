#include <Arduino.h>
#include <RCSwitch.h>

#include "Protocol/CPMessage.h"

RCSwitch rcSwitch = RCSwitch();

void setup() {
    Serial.begin(115200);
    rcSwitch.enableReceive(0);
}

void loop() {
    if (rcSwitch.available()) {
        Serial.print("Data arrived: ");

        auto inVal = static_cast<uint32_t >(rcSwitch.getReceivedValue());

        CPMessage inMsg = CPMessage(inVal);

        Serial.print("Meta: ");
        Serial.println(inMsg._meta);

        Serial.print("Address: ");
        Serial.println(inMsg._address);

        Serial.print("Payload: ");
        Serial.println(inMsg._payload);

        delay(5000);
    }
}
