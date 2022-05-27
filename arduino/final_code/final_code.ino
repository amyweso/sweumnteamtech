// built in library
#include <SPI.h>

// from Pixy2 library
#include <Pixy2.h>

int echoPin = 2;
int trigPin = 3;
int ledLight = 4;

// only occurs once when mBot starts up
void setup() {
  Serial.begin(9600);
  Serial.print("Starting...n");
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledLight, OUTPUT);
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

  return (distance < 40);
}


// this loop continuously runs
void loop() {
  bool blocked = pathBlocked();
  Serial.print("Path is blocked: ");
  Serial.println(blocked);

  if (blocked) {
    digitalWrite(ledLight, HIGH);
  } else {
    digitalWrite(ledLight, LOW);
  }

  delay(500);
}
