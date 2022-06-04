#include <Pixy2.h>
#include <Pixy2CCC.h>
#include <SPI.h>

bool leftDetected;
bool rightDetected;
float leftX;
float rightX;
int yellowLight = 5;

Pixy2 pixy;

void setup() {
    Serial.begin(9600);
    Serial.print("Starting...\n");
    pinMode(yellowLight, OUTPUT);
    pixy.init();
}

// normalizes a float
float mapfloat(long x, long in_min, long in_max, long out_min, long out_max) {
  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}

void detectObjects() {
  uint16_t blocks = pixy.ccc.getBlocks();
  leftDetected = false;
  rightDetected = false;
    
  for (int i = 0; i < pixy.ccc.numBlocks; i++) {
    // gets the center x coordinate of the detected object
    int width = pixy.ccc.blocks[0].m_width;
    int x = pixy.ccc.blocks[0].m_x;
    float cx = (x + (width / 2));
    cx = mapfloat(cx, 0, 320, -1, 1);
    Serial.print("x value is ");
    Serial.println(cx);

    // get the left sticker target
    if (pixy.ccc.blocks[i].m_signature == 1) {
      Serial.println("Left detected");
      leftDetected = true;
      leftX = cx;
    } 
    
    // get the right sticker target
    if (pixy.ccc.blocks[i].m_signature == 2) {
      Serial.println("Right detected");
      rightDetected = true;
      rightX = cx;
    }
  }

  if (leftDetected && rightDetected) {
    digitalWrite(yellowLight, LOW);
    float cx = (leftX + rightX) / 2;
    Serial.print("Official x value is ");
    Serial.println(cx);
  }
  else {
    digitalWrite(yellowLight, HIGH);
    float cx = 0.0;
  }
}

void loop() {
  detectObjects();
}
