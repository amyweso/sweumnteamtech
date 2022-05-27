// built in library
#include <SPI.h>

// from Pixy2 library
#include <Pixy2.h>
#include <Pixy2CCC.h>

// initialize Pixy camera
Pixy2 pixy;

// initialize pins
int echoPin = 2;
int trigPin = 3;
int redLight = 4;
int yellowLight = 5;
int leftMotor = 9;
int rightMotor = 10;

// only occurs once when mBot starts up
void setup() {
  Serial.begin(9600);
  Serial.print("Starting...n");
  pixy.init();

  // set up I/O pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redLight, OUTPUT);
  pinMode(yellowLight, OUTPUT);
  pinMode(leftMotor, OUTPUT);
  pinMode(rightMotor, OUTPUT);
}


// normalizes a float
float mapfloat(long x, long in_min, long in_max, long out_min, long out_max) {
  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
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

  return (distance < 30);
}


// this loop continuously runs
void loop() {
  bool blocked = pathBlocked();
  Serial.print("Path is blocked: ");
  Serial.println(blocked);

  if (blocked) {
    digitalWrite(redLight, HIGH);
    digitalWrite(leftMotor, LOW);
    digitalWrite(rightMotor, LOW);
  } else {
    digitalWrite(redLight, LOW);
    digitalWrite(leftMotor, HIGH);
    digitalWrite(rightMotor, HIGH);
  }

  delay(500);
}
