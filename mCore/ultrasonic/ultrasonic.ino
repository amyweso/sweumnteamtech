#include <MeBuzzer.h>
#include <MeUltrasonicSensor.h>
#include <SPI.h>

// Initialize buzzer
MeBuzzer buzzer;

// Initialize ultrasonic sensor (number indicates 
// the port that the ultrasonic is connected to)
MeUltrasonicSensor ultrasonic(2);

void setup() {
    Serial.begin(9600);
}

void loop() {
    Serial.print("Distance : ");
    Serial.print(ultrasonic.distanceCm());
    Serial.print(" cm");

    // there is an object less than 10 cm away
    if (ultrasonic.distanceCm() < 10) {
        // sound the buzzer
        buzzer.tone(262, 500);
        delay(500);

        // turn buzzer off
        buzzer.tone(0);
    }
    delay(100);
}