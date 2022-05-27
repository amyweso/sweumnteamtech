/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    BuzzerTest.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/07/24
 * @brief   Description: this file is sample code for buzzer
 *
 * Function List:
 * 1. void buzzerOn()
 * 2. void buzzerOff()
 *
 * \par History:
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2015/07/24     1.0.0            Rebuild the old lib.
 */

#include "MeOrion.h"

void setup() {}

// buzzer continually buzzes on and off
void loop() {
  buzzerOn();
  delay(1000);
  buzzerOff();
  delay(1000);
}
