#include <SPI.h>
#include <Servo.h>


Servo Servo1;
const int Servo1Pin = 12;

Servo Servo2;
const int Servo2Pin = 11;

int buf [100];
volatile byte pos;
volatile boolean process_it;

typedef struct message{
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
  
  pinMode(MISO,OUTPUT);

  // turn spi into a slave mode
  SPCR |= _BV(SPE);

  // get ready for an iterrupt
  pos = 0;
  process_it = false;

  SPI.attachInterrupt();
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
  Message cur_msg;
  if (process_it)
  {
    buf [pos] = 0;  
    digitalWrite(13, HIGH);
    digitalWrite(13, LOW);
    cur_msg = process_message(buf);
//    digitalWrite(cur_msg.Pin, cur_msg.Action);
    pos = 0;
    process_it = false;
    digitalWrite(13, HIGH);
  }  // end of flag set
    if(cur_msg.Pin == Servo1Pin && (cur_msg.Action <= 180) && (cur_msg.Action >= 0)){
      Servo1.write(cur_msg.Action); 
    }

    if(cur_msg.Pin == Servo2Pin && cur_msg.Action <= 180 && cur_msg.Action >= 0){
      Servo2.write(cur_msg.Action); 
    }
    
}  // end of loop

Message process_message(int* mes)
{
  // Message is created from ~ (0x7E), <Pin>, <Action>
  Message msg;
  for(int i = 0; i <= pos; i++)
  {
    Serial.print (mes[i], HEX);
    Serial.print(", ");
    if(mes[i] == 0x7E)
    {
      if(i + 1 <= pos)
        msg.Pin = mes[i + 1];

      if(i + 2 <=pos)
        msg.Action = mes[i + 2];
    }
    
  }
  Serial.print('\n');
  return msg;
}





