/*
 Controlling a servo position using a potentiometer (variable resistor)
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Knob
*/

// Include the Servo library 
#include <Servo.h> 

// Create a servo object 
Servo Servo1;
Servo Servo2; 
//Servo Servo2; 
const int buttonPin1 = 2;     // the number of the pushbutton pin
const int buttonPin2 = 3;
const int buttonPin3 = 10;

const int ledPin1 =  13;      // the number of the LED pin
const int ledPin2 = 7;
const int ledPin3 = 6;

const int servoPin1 = 46;     //using mega thats why
const int servoPin2 = 8;

volatile byte ButtonState1 = LOW;
volatile byte ButtonState2 = LOW;

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
    
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT);

  attachInterrupt(digitalPinToInterrupt(buttonPin1), changeButtonState1, HIGH);  
  attachInterrupt(digitalPinToInterrupt(buttonPin2), changeButtonState2, HIGH);

   // We need to attach the servo to the used pin number 
   Servo1.attach(servoPin1);
   Servo1.write(90);
   Servo2.attach(servoPin2);
   Servo2.write(90);
}

void loop(){ 

  if(digitalRead(buttonPin3) == HIGH){
    MoveServoUp(Servo1, ButtonState1);
    MoveServoDown(Servo1, ButtonState2);
    digitalWrite(ledPin1, HIGH);
  }else{
    MoveServoUp(Servo2, ButtonState1);
    MoveServoDown(Servo2, ButtonState2); 
    digitalWrite(ledPin1, LOW);

  }
}

void changeButtonState1(){
  ButtonState1 = !ButtonState1;
}

void changeButtonState2(){
  ButtonState2 = !ButtonState2;
}

// these interupts should change a bool in the body not trigger code in a substate. 
void MoveServoUp(Servo ser, byte buttonState){
      // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin3, HIGH);
    int ServoCD = 0;
    if(ser.read() < 180){
    ServoCD = 1 + ser.read();
    }
    else{
      ServoCD = 179;
    }
    ser.write(ServoCD);
    delay(10);
  } else {
    // turn LED off:
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin3, LOW);
  }
}

void MoveServoDown(Servo ser, byte buttonState){
    // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, HIGH);
    int ServoCD = 0;
    if(ser.read() > 0){
    ServoCD = -1 + ser.read();
    }else{
      ServoCD = 1;
    }
    ser.write(ServoCD);
    delay(10);
  } else {
    // turn LED off:
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
  }
}

