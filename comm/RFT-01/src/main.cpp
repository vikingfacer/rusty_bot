#include <Arduino.h>
#include <RCSwitch.h>

#include "PinMap.h"
#include "Protocol/CPMessage.h"
#include "Protocol/CPServer.h"

RCSwitch rcSwitch = RCSwitch();

void setup() {
    Serial.begin(115200);
    rcSwitch.enableTransmit(10);
}

void loop() {
    uint8_t addr = 0x04;
    uint16_t payload = 0x5476;

    CPMessage msg = CPMessage(addr, payload);

    Serial.println("Sending...");
    rcSwitch.send(msg.compile(), 32);
    Serial.println("Sent!");
    delay(1000);
}
