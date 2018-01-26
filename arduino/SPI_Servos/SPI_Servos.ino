#include <SPI.h>
#include <Wire.h>
#include <Servo.h>
//#include "Pin_Map.h"

#define SLAVE_ADDRESS 0x04
int number = 0;
int state = 0;

Servo Servo1;
const int Servo1Pin = 12;

Servo Servo2;
const int Servo2Pin = 11;

int buf [100];
volatile byte pos;
volatile boolean process_it;

typedef struct message{
  char Type;
  int Pin;
  int Action;
} Message;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // debugging 
  
  Servo1.attach(Servo1Pin); // servo1 set up 
  Servo1.write(90);

  Servo2.attach(Servo2Pin);
  Servo2.write(90);
  /**** SPI ******/
  pinMode(MISO,OUTPUT);
  SPI.attachInterrupt();pos = 0;
  
  // turn spi into a slave mode
  SPCR |= _BV(SPE);


  /**** I2C ******/
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
 
  // define callbacks for i2c communication
  Wire.onReceive(receiveData);

  // get ready for an iterrupt
  process_it = false;

}

ISR (SPI_STC_vect){

  byte c = SPDR; // grab byte from SPI Data Register

  if(pos < sizeof buf){

    buf [pos++] = c;
  
    if(c == '\n') {
      process_it = true;
    }
  
  }
}

// main loop - wait for flag set in interrupt routine
void loop (void)
{ 
    int iop;
    Message cur_msg;
    if (process_it)
    {
      buf [pos] = 0;  
      digitalWrite(13, HIGH);
      digitalWrite(13, LOW);
      cur_msg = create_message(buf);
      // digitalWrite(cur_msg.Pin, cur_msg.Action);
      pos = 0;
      process_it = false;
      digitalWrite(13, HIGH);
    }  // end of flag set
//    Serial.println(buf, DEC)
    switch (cur_msg.Type){
      case 's':
      case 'S':
//        Serial.println(index_of_pin(pm, 'S', cur_msg.Pin), DEC);
        if( Degree_in_Range(cur_msg.Action)){
                
          if(cur_msg.Pin == Servo1Pin) Servo1.write(cur_msg.Action); 
          if(cur_msg.Pin == Servo2Pin) Servo2.write(cur_msg.Action); 
        }
      break;
      case 'd':
      case 'D':
          digitalWrite(cur_msg.Pin, cur_msg.Action);        
      break;

      default:
//        Serial.println("Message is not good");
        break;
      }
      reset_msg(cur_msg);
}  // end of loop

Message create_message(int* mes)
{
  // Message is created from ~ (0x7E),<Type>, <Pin>, <Action>
  Message msg;
  Serial.print("SPI: ");  
  for(int i = 0; i <= pos; i++)
  {
    Serial.print (mes[i], HEX);
    Serial.print(", ");
    if(mes[i] == 0x7E)
    {
      if(i + 1 <= pos)
        msg.Type = mes[i + 1];
        
      if(i + 2 <= pos)
        msg.Pin = mes[i + 2];
        
      if(i + 3 <=pos)
        msg.Action = mes[i + 3];
    }
  }
  Serial.print('\n');
  return msg;
}
void reset_msg(Message &msg){
  msg.Pin = 0;
  msg.Action = 0;
  msg.Type = 0;
}
/****** I2C functions****************/
// callback for received data
void receiveData(int byteCount){
 int number;
 while(Wire.available()) {
  number = Wire.read();
 
  if (number == 1){
   if (state == 0){
    digitalWrite(13, HIGH); // set the LED on
    state = 1;
   } else{
    digitalWrite(13, LOW); // set the LED off
    state = 0;
   }

  }
 
//  if(number==2) {
//   number = (int)temp;
//  }
 }
  Serial.print("I2C: ");
  Serial.println(number, HEX);
}
boolean Degree_in_Range(int degree){
  return degree <= 180 && degree >= 0;
}




