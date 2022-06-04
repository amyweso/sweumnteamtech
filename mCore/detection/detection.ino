#include <Pixy2.h>
#include <Pixy2CCC.h>
#include <SPI.h>

Pixy2 pixy;

void setup() {
    Serial.begin(9600);
    Serial.print("Starting...\n");
    pixy.init();
}

void loop() {
    pixy.ccc.getBlocks();

    // if it detects blocks, print them
    if (pixy.ccc.numBlocks) {
        Serial.print("Detected ");
        Serial.println(pixy.ccc.numBlocks);
        for (int i = 0; i < pixy.ccc.numBlocks; i++) {
            Serial.print(" block ");
            Serial.print(i);
            Serial.print(": ");
            pixy.ccc.blocks[i].print();
        }
    }
}
