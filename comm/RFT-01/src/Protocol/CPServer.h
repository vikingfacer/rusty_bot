//
// Created by Daniel Santos on 1/7/18.
//

#ifndef RFT_01_CPROTOCOL_H
#define RFT_01_CPROTOCOL_H

#include <stdint.h>

/**
 * CPAction (Communication Protocol Action)
 * This enum defines all the action possible for a message.
 *
 * Note: the action must be 8 bits long (8-bits). Actions ID greater than
 * 15 (0xFF) are not allowed.
 * */
enum Action {
    OFF = 0x00,
    ON  = 0x01
};

/**
 * CPServer (Communication Protocol Server)
 * Defines the implementation of the server for the communication protocol.
 * */
class CPServer {
public:
    CPServer();
    ~CPServer();

    void send();

};


#endif //RFT_01_CPROTOCOL_H
