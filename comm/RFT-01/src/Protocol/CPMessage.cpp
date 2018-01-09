//
// Created by Daniel Santos on 1/7/18.
//

#include "CPMessage.h"

CPMessage::CPMessage() : _meta(0), _address(0), _payload(0) {}

CPMessage::CPMessage(const uint8_t& address, const uint16_t& payload) :
        _address(address), _payload(payload) {}

CPMessage::CPMessage(uint32_t message) {
    // Extract meta data
    this->_meta = (message & 0xFF000000) >> 24;

    // Extract address
    this->_address = (message & 0x00FF0000) >> 16;

    // Extract payload
    this->_payload = (message & 0x0000FFFF);
}

CPMessage::~CPMessage() = default;

uint32_t CPMessage::compile() const {
    uint32_t msg = 0x00;

    // Insert the meta data in the message
    msg += (this->_meta << 24);

    // Insert the address in the message
    msg += (this->_address << 16);

    // Insert the payload in the message
    msg += (this->_payload);

    return msg;
}
