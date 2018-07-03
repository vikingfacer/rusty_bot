#include <SPI.h>
#include <Wire.h>
#include <Servo.h>
#include <LiquidCrystal.h>
#include "Messaging.h"

//#include "Pin_Map.h"

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
    Message cur_spi_msg;
    Message cur_i2c_msg;
    if (spi_process_it)
    {
      buf [pos] = 0;
      digitalWrite(13, HIGH);
      digitalWrite(13, LOW);
      cur_spi_msg = create_i2c_message(buf);
      // digitalWrite(cur_msg.Pin, cur_msg.Action);
      pos = 0;
      spi_process_it = false;
      digitalWrite(13, HIGH);
      Serial.println("got spi message");
      process_message(cur_spi_msg);
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
      cur_i2c_msg = create_i2c_message(i2cBuf);
      i2cBufCount = 0;
      i2c_process_it = false;
      process_message(cur_i2c_msg);

    }

    //  reset the messages
//    process_message(cur_spi_msg);
//    process_message(cur_i2c_msg);

    reset_msg(cur_spi_msg);
    reset_msg(cur_i2c_msg);


}  // end of loop
