#include <SPI.h>
#include <Wire.h>
#include <Servo.h>
#include <LiquidCrystal.h>
//#include "Messaging.h"

int const SLAVE_ADDRESS = 0x04;
int i2cBuf[100] = {0};
int i2cBufCount = 0;
boolean i2c_process_it;

Servo Servo1;
const int Servo1Pin = 12;

Servo Servo2;
const int Servo2Pin = 11;


// this is the display
LiquidCrystal lcd(1,2,4,5,6,7);

int buf [100];
volatile byte pos;
volatile boolean spi_process_it;


////this is the defination of the message
typedef struct message{
  char Type;
  int Pin;
  int Action;
  char Mode; // this only applies to the digital pins for now
} Message;

// forward declarations
void process_message( Message msg);
Message create_spi_message(int* mes);
Message create_i2c_message(int* mes);
void receiveData(int byteCount);
void reset_msg(Message &msg);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // debugging

  /**** SERVOS ****/
  Servo1.attach(Servo1Pin); // servo1 set up
  Servo1.write(90);

  Servo2.attach(Servo2Pin);
  Servo2.write(90);

  /*****DISPLAY*****/
  lcd.begin(16,2);

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
  i2c_process_it = false;
  // get ready for an iterrupt
  spi_process_it = false;

  pinMode(10, OUTPUT);

}

ISR (SPI_STC_vect){

  byte c = SPDR; // grab byte from SPI Data Register

  if(pos < sizeof buf){

    buf [pos++] = c;

    if(c == '\n') {
      spi_process_it = true;
    }
  }
}

// main loop - wait for flag set in interrupt routine
void loop (void)
{
    int iop;

    if (spi_process_it)
    {
      for(int i = 0; i <= pos; i++){
        Serial.print(buf[i]);
        Serial.print(", ");
      }
      Serial.print('\n');

      buf [pos] = 0;
      digitalWrite(13, HIGH);
      digitalWrite(13, LOW);
      digitalWrite(13, HIGH);
      
      Serial.println("got spi message");
      process_message(buf, pos);
      pos =0;
      spi_process_it = false;
      
    }  // end of flag set

    if (i2c_process_it)
    {
      Serial.print("i2c: ");
      for(int i = 0; i <= i2cBufCount; i++){
        Serial.print(i2cBuf[i]);
        Serial.print(", ");
      }
      Serial.print('\n');

      
      i2cBuf [i2cBufCount] = 0;
      process_message(i2cBuf, i2cBufCount);
      i2cBufCount = 0;
      i2c_process_it = false;
    }

}  // end of loop

boolean Degree_in_Range(int degree){
  return degree <= 180 && degree >= 0;
}

void process_servo(int Pin, int Degree){
 if( Degree_in_Range(Degree)){
  
    if(Pin == Servo1Pin) Servo1.write(Degree);
    if(Pin == Servo2Pin) Servo2.write(Degree);
    
 }else{
  Serial.print(Degree);
  Serial.print(" Out of Degree range Obama!\n");
 }
}

void process_digital(int Pin, int Mode, int Action){
//    if mode r action does not matter 
    if(Mode == 'w' || Mode == 'W'){
       pinMode(Pin, OUTPUT);
    }else
      if(Mode == 'r' || Mode == 'R'){
        pinMode(Pin, INPUT);
    }
    digitalWrite(Pin, Action);
}

void process_message(int* msg, int msgSize)
{
//    first byte is ~
//    msg[1] ::= s, d, a, l, S, D, A, L 
    int Pin;
    int Degree;
    int Action;
    int Mode;

    switch (msg[1]){
      case 's':
      case 'S':
//        Serial.println(index_of_pin(pm, 'S', cur_msg.Pin), DEC);
          Pin = msg[2];
          Degree = msg[3];
          process_servo( Pin, Degree);
          
      break;
      case 'd':
      case 'D':
          Pin = msg[2];
          Mode= msg[3];   
          Action= msg[4];
          process_digital(Pin, Mode, Action);
      break;

      default:
//        Serial.println("Message is not good");
      break;
    }
}


///****** I2C functions****************/
//// callback for received data
void receiveData(int byteCount){
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

