#include <SPI.h>
#include <Pixy.h>
#include <MeOrion.h>
Pixy pixy;

// motor controllers are connected to 
// Arduino pins 5, 6, 7, 8, 9, and 10
int myPins[6] = {5, 6, 7, 8, 9, 10};

// initialize the LED pin
int led_light = 4;

// the dead zone of the PixyCam
float dead_zone = 0.15;

// true if the robot has docked
bool docked = false;

// the center of the object we are tracking
float cx;

// false if sticker has not been 
// detected, true otherwise
bool left_detected;
bool right_detected;
 

// only occurs once when Arduino starts up
// initializes all the pins as output
void setup() {
  Serial.begin(9600);
  Serial.print("Starting...n");
  pixy.init();
  for (int i = 0; i < 6; i++) {
    pinMode(myPins[i], OUTPUT);
  }
  pinMode(led_light, OUTPUT);
}


// normalizes a float
float mapfloat(long x, long in_min, long in_max, long out_min, long out_max) {
  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}
 
 
// moves the robot according to a left and right velocity
// the velocity can be negative in order to indicate direction
void moveRobot(int left_speed, int right_speed) {
  if (left_speed >= 0) {
    digitalWrite(myPins[1], 0);
    digitalWrite(myPins[2], 1);
  }
  else {
    digitalWrite(myPins[1], 1);
    digitalWrite(myPins[2], 0);
  }
 
  if (right_speed >= 0) {
    digitalWrite(myPins[3], 0);
    digitalWrite(myPins[4], 1);
  }
  else {
    digitalWrite(myPins[3], 1);
    digitalWrite(myPins[4], 0);
  }
 
  analogWrite(myPins[0], abs(left_speed));
  analogWrite(myPins[5], abs(right_speed));
}


// returns true if the path is blocked by an object
// returns false if the path is clear
bool pathBlocked() {
  return false;
}


// enables the mBot buzzer
void enableBuzzer() {
  buzzerOn();
  delay(1000);
  buzzerOff();
  delay(1000);
}


void detectObjects() {
  uint16_t blocks;
  float left_x;
  float right_x;

  // grab the objects we want to track
  blocks = pixy.getBlocks();
    
  for (int i = 0; i < pixy.numBlocks; i++) {
    // gets the center x coordinate of the detected object
    int width = pixy.blocks[0].width;
    int x = pixy.blocks[0].x;
    cx = (x + (width / 2));
    cx = mapfloat(cx, 0, 320, -1, 1);

    // get the left sticker target
    if (pixy.blocks[i].signature == 1) {
      left_detected = true;
      left_x = cx;
    } 
    
    // get the right sticker target
    if (pixy.blocks[i].signature == 2) {
      right_detected = true;
      right_x = cx;
    }

    // get the docked signature
    if (pixy.blocks[i].signature == 3) {
      docked = pixy.blocks[i].width < 10.0;
    }
  }

  // targets have been detected
  if (left_detected && right_detected) {
    digitalWrite(led_light, LOW);
    cx = (left_x + right_x) / 2;
  }

  // objects have not been detected
  // continue moving straight
  else {
    digitalWrite(led_light, HIGH);
    cx = 0.0;
  }
}


// this loop continuously runs
// determines the turn and calls moveRobot
void loop() {
  detectedObjects();
  bool blocked = pathBlocked();

  // the path is clear
  if (!blocked && !docked) {
    if (cx > -dead_zone && cx < dead_zone) {
      cx = 0;
    }
    if (cx < 0) {
      moveRobot(-80, 170);
    }
    else if (cx > 0) {
      moveRobot(170, -80);
    }
    else {
      moveRobot(70, 70);
    }
    delay(1);
  }

  // path is not clear --> enable buzzer
  // and robot stops moving
  else if (blocked) {
      enableBuzzer();
  }
}
