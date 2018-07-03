#include "Messaging.h"
#include <iostream>


int main() {

  int testMessage[] = {'~', 'i', 'd', 20, 'w', 1};
  MessageHandler::create_message(testMessage, 3);

  return 0;
}
