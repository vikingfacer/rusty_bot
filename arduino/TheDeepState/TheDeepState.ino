
//#define DEBUG

#include <math.h>
<<<<<<< HEAD
//pins
const int Y = 21;
const int X = 20;
const int X2= 19;
const int Y2= 18;
//constants that are not constants cause arduino is for dummies
float init_x;float init_y; float init_x2; float init_y2;
=======
//pins 
// joystick 1 const dc motors
const int Y1 = 21;
const int X1 = 20;
const int stop1 = 12;

// joystick 2 const servos
const int Y2 = 18;
const int X2 = 19;
const int stop2 = 10;

//constants that are not constants cause arduino is for dummies
float init_x1;float init_y1;float init_x2;float init_y2;

>>>>>>> 22039ae48860b714c7ba0cbf98c240bae962a400

const float limit = 0.70710678118; //this is 45 degrees on the unit circle, which is our limit
int incoming_byte = 0;


enum speed{
  slow = 100,
  medium=200,
  high = 255,
};

void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  digitalWrite(5,1);
  digitalWrite(6,1);


  digitalWrite(2, 1);
  digitalWrite(3, 0);
  #ifdef DEBUG
  digitalWrite(4, 1);
  #else
  digitalWrite(4, 0);
  #endif

  pinMode(stop1, INPUT);
  pinMode(stop2, INPUT);

  Serial.begin(115200);

//  get intial values of the x y
<<<<<<< HEAD
  init_x = analogRead(X);
  init_y = analogRead(Y);
  init_x2= analogRead(X2);
  init_y2= analogRead(Y2);
  
  Serial.print(init_x);
=======
  init_x1 = analogRead(X1);
  init_y1 = analogRead(Y1);
  Serial.print(init_x1);
>>>>>>> 22039ae48860b714c7ba0cbf98c240bae962a400
  Serial.print(" ");
  Serial.print(init_y1);
  delay(100);
  digitalWrite(2,0);
  digitalWrite(2,1);
  
  init_x2 = analogRead(X2);
  init_y2 = analogRead(Y2);
  Serial.print(init_x2);
  Serial.print(" ");
  Serial.print(init_y2);
  delay(100);
  digitalWrite(2,0);

  
}

float unitize(const float& num, const float& mag);
float pythagerian(const float& x,const float& y);
char process_direction(const bool& inverte, const float& x_in, const float& y_in, const float& mag);
speed process_speed(const float& mag);
void push_to_serial(const char& dev, const int& x, const int& y, const int& init_x, const int& init_y);
//bool toggle_button(const int& pin);

  
bool dev1On = false;
bool dev2On = false;
bool toggle1 = false;
bool toggle2 = false;
  int last_read1 = 0;
  int last_read2 = 0;
 
  
void loop() {

  int current_read1 = 0;
  int current_read2 = 0;
  
  last_read1 = current_read1;
  last_read2 = current_read2;
  
  current_read1 = digitalRead(stop1);
  current_read2 = digitalRead(stop2);
  
  if(last_read1 != current_read1){
    dev1On = !dev1On;
    digitalWrite(5,dev1On);
  }

  if(last_read2 != current_read2){
    dev2On = !dev2On;
    digitalWrite(6,dev2On);

<<<<<<< HEAD
  float y2 = analogRead(Y2) - init_y2;
  float x2 = analogRead(X2) - init_x2;
  float mag2 = pythagerian(x2, y2);

  #ifdef DEBUG
  Serial.print(x);
  Serial.print(" , ");
  Serial.print(unitize(x,mag));
  Serial.print("  ");
  Serial.print(y);
  Serial.print(" , ");
  Serial.print(unitize(x,mag));
  Serial.print("  ");
  Serial.print(pythagerian(x, y));
  Serial.print(" dir: ");
  Serial.print(process_direction(x, y, mag));
  Serial.print("  ");
  Serial.print(process_speed(mag));
  Serial.print("\n");
  // joystick 2
  Serial.print(x2);
  Serial.print(" , ");
  Serial.print(unitize(x2,mag2));
  Serial.print("  ");
  Serial.print(y2);
  Serial.print(" , ");
  Serial.print(unitize(x2,mag2));
  Serial.print("  ");
  Serial.print(pythagerian(x2, y2));
  Serial.print(" dir: ");
  Serial.print(process_direction(x2, y2, mag2));
  Serial.print("  ");
  Serial.print(process_speed(mag2));
  Serial.print("\n");
  #else
  Serial.print(process_direction(x2, y2, mag2));
  Serial.print(" ");
  Serial.print(process_speed(mag2));
=======
  }
  
  #ifdef DEBUG
  Serial.print("button1: ");
  Serial.print(digitalRead(stop1));
  Serial.print("    ");
  Serial.print("button2: ");
  Serial.print(digitalRead(stop2));
>>>>>>> 22039ae48860b714c7ba0cbf98c240bae962a400
  Serial.print("\n");
  #endif
  
  if(dev1On){
    push_to_serial('m', X1, Y1, init_x1, init_y1);
  }  
  if(dev2On){
    push_to_serial('s', X2, Y2, init_x2, init_y2);
  }

  if (Serial.available()){
    digitalWrite(3,1);
    delay(100);
  }
  digitalWrite(3,0);
  delay(100);

}

float pythagerian(const float& x,const float& y){
  return sqrt(x*x+ y*y);
}

float unitize(const float& num, const float& mag){
  return num/mag;
}

char process_direction(const bool& inverted, const float& x_in, const float& y_in, const float& mag){
  float y = unitize(y_in, mag);
  float x = unitize(x_in, mag);
  boolean y_in_range = (y < limit) && (y > -limit);
  boolean x_in_range = (x < limit) && (x > -limit);
  boolean x_above_floor=(x < -.25 || x > .25);
  boolean y_above_floor=(y < -.25 || y > .25);

    if(y_in_range && x_in_range){
      return 's';
    }else
    if(x_above_floor || y_above_floor){
      if(!inverted){
        if (y > limit && x_in_range) {
          return 'b'; // values are inverted on the joystick
        }else
          if(y < -limit && x_in_range) {
            return 'f';// values are inverted on the joystick
          }else
          if(x > limit && y_in_range){
            return 'r';
          }else
          if(x < -limit && y_in_range){
            return 'l';
          }
      }else{
        if (y > limit && x_in_range) {
          return 'f'; // values are inverted on the joystick
        }else
          if(y < -limit && x_in_range) {
            return 'b';// values are inverted on the joystick
          }else
          if(x > limit && y_in_range){
            return 'l';
          }else
          if(x < -limit && y_in_range){
            return 'r';
          }
      }
    }

  return 's';
}

speed process_speed(const float& mag,const int& init_x, const int& init_y){
  // these are globals and suck
  float avg_mag = (init_x + init_y)/2;
  float noise_floor, slow_rang_top, mid_rang_top;
  noise_floor = avg_mag * .25;
  slow_rang_top = avg_mag * .5;
  mid_rang_top= avg_mag * .7;

  if(mag >= mid_rang_top){
    return high;
  }else
  if(mag <= mid_rang_top && mag > slow_rang_top){
    return medium;
  }else
  if(mag <= slow_rang_top && mag > noise_floor){
    return slow;
  }
  return slow;
}

// need to add title of device it represent ie: dc motors, servo
void push_to_serial(const char& dev, const int& X, const int& Y, const int& init_x, const int& init_y)
{
  
  //this sets up the "cordant system"
  float y = analogRead(Y) - init_y;
  float x = analogRead(X) - init_x;
  float mag = pythagerian(x, y);

  #ifdef DEBUG
  Serial.print("dev:");
  Serial.print(dev);
  Serial.print(" ,");
  Serial.print(x);
  Serial.print(" , ");
  Serial.print(unitize(x,mag));
  Serial.print("  ");
  Serial.print(y);
  Serial.print(" , ");
  Serial.print(unitize(x,mag));
  Serial.print("  ");
  Serial.print(pythagerian(x, y));
  Serial.print(" dir: ");
  Serial.print(process_direction(x, y, mag));
  Serial.print("  ");
  Serial.print(process_speed(mag, init_x, init_y));
  Serial.print("\n");
  #else
  Serial.print("I M ");
  Serial.print(process_direction(true, x, y, mag));
  Serial.print(" ");
  Serial.print(process_speed(mag, init_x, init_y));
  Serial.print("\n");
  #endif

}



