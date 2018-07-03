// #ifndef MESSAGE_H
// #define MESSAGE_H

// #include "Arduino.h"
// #define ARDUINOLAND // uncomment for arduino land

#ifdef ARDUINOLAND
  #include "Arduino.h"
#else

  #include <iostream>
  using std::cout;
  using std::endl;

#endif

// //this is the defination of the message
typedef struct message{
  char Type;
  int Pin;
  int Action;
  char Mode; // this only applies to the digital pins for now

} Message;

//

bool Degree_in_Range(int degree);

void receiveData(int byteCount); // this is the call back function for i2c

class MessageHandler {

public:
  static void process_message( Message msg);

  static void reset_msg(Message &msg);

  static Message create_message(int* mes,int  BufCount) {

  Message msg;

  #ifdef ARDUINOLAND
    Serial.print("buffer: ");
  #else
    cout << "buffer: " << endl;
  #endif

  for(int i = 0; i <= BufCount; i++)
  {
    #ifdef ARDUINOLAND
      Serial.print (mes[i]);
      Serial.print(", ");
    #else
      cout<< mes[i] << ", " << endl;
    #endif

  }
    if(mes[0] == 0x7E)
    {
      // sets the type of the message
      if(1 <= BufCount)
        msg.Type = mes[1];

      //. init mode to 0 its only used for digital
      msg.Mode = 'k';

      if( msg.Type == 'd' || msg.Type == 'D'){
        if(2 <= BufCount)
          msg.Pin = mes[2];

        if(3 <= BufCount)
          msg.Mode = mes[3];

        if(4 <= BufCount)
          msg.Action = mes[4];
      }
      else
      {
        if(2 <= BufCount)
          msg.Pin = mes[2];

        if(3 <= BufCount)
          msg.Action = mes[3];
      }
    }
  #ifdef ARDUINOLAND
    Serial.print ((char) msg.Type);
    Serial.print(" ");
    Serial.print (msg.Pin);
    Serial.print(" ");
    Serial.print ( msg.Action);
    Serial.println(" ");
  #else
    cout << (char) msg.Type << " " << msg.Pin << " " << msg.Action << endl;
  #endif
  return msg;
};

};
// #endif
