//
// Created by Daniel Santos on 1/7/18.
//

#ifndef RFT_01_CPROTOCOL_H
#define RFT_01_CPROTOCOL_H

#include <stdint.h>


/**
 * Forward declaration of CPServer
 * */
class CPServer;

/**
 * CPMessage (Communication Protocol Message)
 * This class defines the Communication Protocol Message used in the
 * transmitter/receiver communication.
 * */
class CPMessage {
public:
    /**
     * Default constructor. Properties assign to zero by default.
     * */
    CPMessage();
    /**
     *  Helper constructor.
     * */
    CPMessage(const uint8_t& address, const uint16_t& payload);
    /**
     * Construct message object from 32 bits in the following order:
     * |   8-bits   8-bits   16-bits
     * |    META   ADDRESS   PAYLOAD
     * */
    explicit CPMessage(uint32_t message);
    ~CPMessage();

    /**
     * Compiles the meta data, address, and the payload in a 32-bits int.
     *
     * The 32-bits are structured as follow:
     * |   8-bits   8-bits   16-bits
     * |    META   ADDRESS   PAYLOAD
     * */
    uint32_t compile() const;

private:
    friend CPServer;    //< This allows the sever to write to the meta data.

private:
    uint8_t     _meta;      //< Used by the CPSever class to identify messages.
    uint8_t     _address;   //< 8-bits address target of the message action.
    uint16_t    _payload;   //< Payload of the message (CPAction)
};


#endif //RFT_01_CPROTOCOL_H
