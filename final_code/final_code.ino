// built in libraries
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>

// from MakeBlockDrive library
#include <MeMCore.h>
#include <MeOrion.h>

// from Pixy2 library
#include <Pixy2.h>

Pixy2 pixy;

// set up motors
MeDCMotor left_motor(M1);
MeDCMotor right_motor(M2);

// Initialize ultrasonic sensor (number indicates 
// the port that the ultrasonic is connected to)
MeUltrasonicSensor ultrasonic(2);

// if the target is in the dead zone
// have the robot move straight
float dead_zone = 0.15;

// the center of the object we are tracking
float cx;

// false if sticker has not been 
// detected, true otherwise
bool left_detected;
bool right_detected;
bool docked = false; 

// only occurs once when mBot starts up
void setup() {
  Serial.begin(115200);
  Serial.print("Starting...n");
  pixy.init();
  pinMode(LED_BUILTIN, OUTPUT);
}


// normalizes a float
float mapfloat(long x, long in_min, long in_max, long out_min, long out_max) {
  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}
 
 
// moves the robot according to a left and right velocity
// the velocity can be negative in order to indicate direction
void moveRobot(int left_speed, int right_speed) {
  left_motor.run(left_speed);
  right_motor.run(right_speed);
}


// returns true if the path is blocked by an object
// returns false if the path is clear
bool pathBlocked() {
  return ultrasonic.distanceCm() < 10;
}


void detectObjects() {
  uint16_t blocks;
  float left_x;
  float right_x;

  // grab the objects we want to track
  blocks = pixy.ccc.getBlocks();
    
  for (int i = 0; i < pixy.ccc.numBlocks; i++) {
    // gets the center x coordinate of the detected object
    int width = pixy.ccc.blocks[0].width;
    int x = pixy.ccc.blocks[0].x;
    cx = (x + (width / 2));
    cx = mapfloat(cx, 0, 320, -1, 1);

    // get the left sticker target
    if (pixy.ccc.blocks[i].signature == 1) {
      left_detected = true;
      left_x = cx;
    } 
    
    // get the right sticker target
    if (pixy.ccc.blocks[i].signature == 2) {
      right_detected = true;
      right_x = cx;
    }

    // get the docked signature
    if (pixy.ccc.blocks[i].signature == 3) {
      docked = pixy.ccc.blocks[i].width < 10.0;
    }
  }

  // targets have been detected
  if (left_detected && right_detected) {
    digitalWrite(LED_BUILTIN, LOW);
    cx = (left_x + right_x) / 2;
  }

  // objects have not been detected
  // continue moving straight
  else {
    digitalWrite(LED_BUILTIN, HIGH);
    cx = 0.0;
  }
}


// this loop continuously runs
// determines the turn and calls moveRobot
void loop() {
  detectObjects();
  bool blocked = pathBlocked();

  // the path is clear
  if (!blocked && !docked) {
    buzzerOff();
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
  }

  // path is not clear, enable buzzer
  // and robot stops moving
  else if (blocked && !docked) {
      left_motor.stop();
      right_motor.stop();
      buzzerOn();
  }

  // robot has docked
  else {
    left_motor.stop();
    right_motor.stop();
    buzzerOff();
  }

  delay(500);
}
