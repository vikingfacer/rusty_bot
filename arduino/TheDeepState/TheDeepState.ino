#include <math.h>
//pins
const int Y = 21;
const int X = 14;

//constants that are not constants cause arduino is for dummies
float init_x;float init_y;

const float limit = 0.70710678118; //this is 45 degrees on the unit circle, which is our limit

enum speed{
  slow = 100,
  medium=200,
  high = 255,
};

void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);
  digitalWrite(2, 1);
  pinMode(19, INPUT);
  Serial.begin(115200);

//  get intial values of the x y
  init_x = analogRead(X);
  init_y = analogRead(Y);
  Serial.print(init_x);
  Serial.print(" ");
  Serial.print(init_y);

}

float unitize(const float& num, const float& mag);
float pythagerian(const float& x,const float& y);
char process_direction(const float& x_in, const float& y_in, const float& mag);
speed process_speed(const float& mag);

void loop() {

  //this sets up the "cordant system"
  float y = analogRead(Y) - init_y;
  float x = analogRead(X) - init_x;
  float mag = pythagerian(x, y);


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

  delay(1000);

}

float pythagerian(const float& x,const float& y){
  return sqrt(x*x+ y*y);
}

float unitize(const float& num, const float& mag){
  return num/mag;
}

char process_direction(const float& x_in, const float& y_in, const float& mag){
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
  }
  return 's';
}

speed process_speed(const float& mag){
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
