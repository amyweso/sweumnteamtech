#include <SPI.h>
#include <Pixy.h>
#include <MeOrion.h>
Pixy pixy;

// motor controllers are connected to 
// Arduino pins 5, 6, 7, 8, 9, and 10
int myPins[6] = {5, 6, 7, 8, 9, 10};

float deadZone = 0.15;
int baseSpeed = 130; 
int cont = 0;
int signature, x, y, width, height;
float cx, cy, area;
 

// only occurs once when Arduino starts up
// initializes all the pins as output
void setup() {
  Serial.begin(9600);
  Serial.print("Starting...n");
  pixy.init();
  for (int i = 0; i < 6; i++) {
    pinMode(myPins[i], OUTPUT);
  }
}


// normalizes a float
float mapfloat(long x, long in_min, long in_max, long out_min, long out_max) {
  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}


// returns the center x value of the detected object
float pixyCheck() {
  static int i = 0;
  int j;
  uint16_t blocks;
  char buf[32];

  // grab the objects we want to track
  blocks = pixy.getBlocks();
 
  // objects have been detected
  if (blocks) {
    // gets basic information about the detected object
    signature = pixy.blocks[0].signature;
    height = pixy.blocks[0].height;
    width = pixy.blocks[0].width;

    // gets the location of the object
    x = pixy.blocks[0].x;
    y = pixy.blocks[0].y;

    // gets the center point of the object
    cx = (x + (width / 2));
    cy = (y + (height / 2));
    cx = mapfloat(cx, 0, 320, -1, 1);
    cy = mapfloat(cy, 0, 200, 1, -1);

    // gets the area of the object
    area = width * height;
  }

  // objects have not been detected
  // continue moving straight
  else {
    cont += 1;
    if (cont == 100) {
      cont = 0;
      cx = 0;
    }
  }
  return cx;
}
 
 
// moves the robot according to a left and right velocity
// the velocity can be negative in order to indicate direction
void moveRobot(int leftSpeed, int rightSpeed) {
  if (leftSpeed >= 0) {
    digitalWrite(myPins[1], 0);
    digitalWrite(myPins[2], 1);
  }
  else {
    digitalWrite(myPins[1], 1);
    digitalWrite(myPins[2], 0);
  }
 
  if (rightSpeed >= 0) {
    digitalWrite(myPins[3], 0);
    digitalWrite(myPins[4], 1);
  }
  else {
    digitalWrite(myPins[3], 1);
    digitalWrite(myPins[4], 0);
  }
 
  analogWrite(myPins[0], abs(leftSpeed));
  analogWrite(myPins[5], abs(rightSpeed));
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


// this loop continuously runs
// determines the turn and calls moveRobot
void loop() {
  // the path is clear
  if (!pathBlocked()) {
    float turn = pixyCheck();
    if (turn > -deadZone && turn < deadZone) {
      turn = 0;
    }
    if (turn < 0) {
      moveRobot(-80, 170);
    }
    else if (turn > 0) {
      moveRobot(170, -80);
    }
    else {
      moveRobot(70, 70);
    }
    delay(1);
  }

  // path is not clear --> enable buzzer
  // and robot stops moving
  else {
      enableBuzzer();
  }
}
