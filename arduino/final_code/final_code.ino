#include <Pixy2.h>
#include <Pixy2CCC.h>
#include <SPI.h>

// initialize pins
int echoPin = 2;
int trigPin = 3;
int redLight = 4;
int yellowLight = 5;
int leftMotor = 9;
int rightMotor = 10;

// the center of the object we are tracking
float cx = 0;

// false if sticker has not been 
// detected, true otherwise
bool leftDetected;
bool rightDetected;

// initialize Pixy camera
Pixy2 pixy;

// if the target is in the dead zone
// have the robot move straight
float dead_zone = 0.15;

// only occurs once when mBot starts up
void setup() {
  Serial.begin(9600);
  Serial.print("Starting...n");

  // set up I/O pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redLight, OUTPUT);
  pinMode(yellowLight, OUTPUT);
  pinMode(leftMotor, OUTPUT);
  pinMode(rightMotor, OUTPUT);
  pixy.init();
}


// normalizes a float
float mapfloat(long x, long in_min, long in_max, long out_min, long out_max) {
  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}


// moves the robot according to a left and right velocity
void moveRobot(int left_speed, int right_speed) {
  analogWrite(leftMotor, left_speed);
  analogWrite(rightMotor, right_speed);
}


// returns true if the path is blocked by an object
// returns false if the path is clear
bool pathBlocked() {
  // clears the tripPin condition
  digitalWrite(trigPin, LOW);
  delay(2);
  
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);
 
  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(echoPin, HIGH);
 
  // Calculating the distance
  int distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)

  // Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  return (distance < 20);
}


void detectObjects() {
  uint16_t blocks = pixy.ccc.getBlocks();
  float leftX;
  float rightX;

  // reset signature detections
  leftDetected = false;
  rightDetected = false;
    
  for (int i = 0; i < pixy.ccc.numBlocks; i++) {
    // gets the center x coordinate of the detected object
    int width = pixy.ccc.blocks[0].m_width;
    int x = pixy.ccc.blocks[0].m_x;
    cx = (x + (width / 2));
    cx = mapfloat(cx, 0, 320, -1, 1);

    // get the left sticker target
    if (pixy.ccc.blocks[i].m_signature == 1) {
      leftDetected = true;
      leftX = cx;
    } 
    
    // get the right sticker target
    if (pixy.ccc.blocks[i].m_signature == 2) {
      rightDetected = true;
      rightX = cx;
    }
  }

  // targets have been detected
  if (leftDetected && rightDetected) {
    digitalWrite(yellowLight, LOW);
    cx = (leftX + rightX) / 2;
  }

  // objects have not been detected
  // continue moving straight
  else {
    digitalWrite(yellowLight, HIGH);
    cx = 0.0;
  }
}


// returns true if the robot docked
// false otherwise
bool robotDocked() {
  return !leftDetected && !rightDetected;
}

// this loop continuously runs
void loop() {
  bool blocked = pathBlocked();
  bool docked = robotDocked();
  detectObjects();

  // no objects are in the path
  if (!blocked) {
    digitalWrite(redLight, LOW);
    if (!docked) {  
      // object is in the dead zone
      if (cx > -dead_zone && cx < dead_zone) {
        cx = 0;
      }
  
      // object is to the left
      if (cx < 0) {
        moveRobot(5, 220);
      }
  
      // object is to the right
      else if (cx > 0) {
        moveRobot(220, 5);
      }
  
      // object is in the middle
      else {
        moveRobot(200, 200);
      }
    } 
  }
  
  // objects are in the path
  else {
    digitalWrite(redLight, HIGH);
    moveRobot(0, 0);
  }

  delay(100);
}
