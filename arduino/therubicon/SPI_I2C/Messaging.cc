#include "Messaging.h"

boolean Degree_in_Range(int degree){
  return degree <= 180 && degree >= 0;
}


void process_message( Message msg){
  if(msg.Type != 0){
  Serial.print("Type: ");
  Serial.print(msg.Type);
  Serial.print(" ");

  Serial.print("Pin: ");
  Serial.print(msg.Pin);
  Serial.print(" ");

  Serial.print("Mode: ");
  Serial.print((char) msg.Mode);
  Serial.print(" ");

  Serial.print("Action: ");
  Serial.print((char) msg.Action);
  Serial.print(" ");
  }

    switch (msg.Type){
      case 's':
      case 'S':
//        Serial.println(index_of_pin(pm, 'S', cur_msg.Pin), DEC);
        if( Degree_in_Range(msg.Action)){

          if(msg.Pin == Servo1Pin) Servo1.write(msg.Action);
          if(msg.Pin == Servo2Pin) Servo2.write(msg.Action);
        }
      break;
      case 'd':
      case 'D':
          if(msg.Mode == 'w' || msg.Mode == 'W'){
            pinMode(msg.Pin, OUTPUT);
          }else
          if(msg.Mode == 'r' || msg.Mode == 'R'){
            pinMode(msg.Pin, INPUT);
          }
          digitalWrite(msg.Pin, msg.Action);
      break;

      default:
//        Serial.println("Message is not good");
      break;
    }
}

Message create_spi_message(int* mes)
{
  // Message is created from ~ (0x7E),<Type>, <Pin>, <Mode if digital>, <Action>
  Message msg;
  msg.Mode = 0;
  Serial.print("SPI: ");
  for(int i = 0; i <= pos; i++)
  {
    Serial.print (mes[i], HEX);
    Serial.print(", ");
    if(mes[i] == 0x7E)
    {

      if(i + 1 <= pos)
        msg.Type = mes[i + 1];

      if(msg.Type == 'd' || msg.Type == 'D'){

        if(i + 2 <= pos)
          msg.Mode = mes[i + 2];

        if(i + 3 <= pos)
          msg.Pin = mes[i + 3];

        if(i + 4 <=pos)
          msg.Action = mes[i + 4];
      }

    }
  }
  Serial.print('\n');
  return msg;
}

void reset_msg(Message &msg)
{
  msg.Pin = 0;
  msg.Mode = 0;
  msg.Action = 0;
  msg.Type = 0;
}


/****** I2C functions****************/
// callback for received data
void receiveData(int byteCount)
{
  int number;
  while(Wire.available()) {
    number = Wire.read();
  }
  if(pos < sizeof i2cBuf){
    i2cBuf[i2cBufCount++] = number;
    if(number == '\n') {
      i2c_process_it = true;
    }
  }
}

Message create_i2c_message(int* mes)
{
  Message msg;
  Serial.print("i2c buffer: ");
  for(int i = 0; i <= i2cBufCount; i++)
  {
    Serial.print (mes[i]);
    Serial.print(", ");
  }
    if(mes[0] == 0x7E)
    {
      // sets the type of the message
      if(1 <= i2cBufCount)
        msg.Type = mes[1];

      //. init mode to 0 its only used for digital
      msg.Mode = 'k';

      if( msg.Type == 'd' || msg.Type == 'D'){
        if(2 <= i2cBufCount)
          msg.Pin = mes[2];

        if(3 <= i2cBufCount)
          msg.Mode = mes[3];

        if(4 <= i2cBufCount)
          msg.Action = mes[4];
      }
      else
      {
        if(2 <= i2cBufCount)
          msg.Pin = mes[2];

        if(3 <= i2cBufCount)
          msg.Action = mes[3];
      }
    }

  Serial.print ((char) msg.Type);
  Serial.print(" ");
  Serial.print (msg.Pin);
  Serial.print(" ");
  Serial.print ( msg.Action);
  Serial.println(" ");

  return msg;
}
