#include <SPI.h>
#include <MeOrion.h>

// set up motors
MeDCMotor left_motor(M1);
MeDCMotor right_motor(M2);


// only called once
void setup() {}


// moves the robot according to a left and right velocity
// the velocity can be negative in order to indicate direction
void moveRobot(int left_speed, int right_speed) {
  left_motor.run(left_speed);
  right_motor.run(right_speed);
}


// continuously runs
void loop() {
    moveRobot(-80, 170);
    delay(2000);
    moveRobot(170, -80);
    delay(2000);
    moveRobot(70, 70);
    delay(2000);
    left_motor.stop();
    right_motor.stop();
    delay(100);
}