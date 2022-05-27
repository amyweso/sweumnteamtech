#include <MeBuzzer.h>

MeBuzzer buzzer;

void setup() {}

void loop() {
    // 700 Hz and 1 second
    buzzer.tone(700, 1000);
    delay(1000);
}

// buzzer continually buzzes on and off
// void loop() {
//   buzzerOn();
//   delay(1000);
//   buzzerOff();
//   delay(1000);
// }