# Arduino with PixyCam Tracking Example

## Their Model
The tutorial for this model can be found [here](https://www.computervision.zone/courses/object-following-robot-with-arduino/) and the example video can be found [here](https://www.youtube.com/watch?v=w_krOCBk1DE)

## Our Model
Our model uses an mBot with an Arduino and a PixyCam as well as a motion detector

## Differences in the Models
* Their model uses 6 motor controllers instead of two motor controllers connected on the mBot
    * their motor controllers are connected to Arduino pins 5, 6, 7, 8, 9, and 10
    * the mBot's controllers are connected to pins 5 (M1) and 6 (M2) according to this [link](https://forum.makeblock.com/t/difference-between-ports-and-pins/4311/4)
* We need to be able to write code in order to stop the robot from moving once the robot is in a certain range of the target
* We also need to be able to write a procedure using the motion detector to stop the robot when an unexpected object enters the path

## Requirements
* [Makeblock official library](https://forum.makeblock.com/t/how-can-i-program-mbot-using-arduino-ide-instead-of-mblock/5401)
* [PixyCam library](https://pixycam.com/downloads-pixy2/)