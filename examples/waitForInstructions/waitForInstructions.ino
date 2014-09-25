/*
 Initialize Cubetto and waits for instructions over Serial1:
 
 F : forward
 L : left 90°
 R : right 90°
 
 created 10 Jun 14 by Matteo Loglio
  
 This example code is in the public domain.
 */

#include <Cubetto.h>

Cubetto cubetto;

void setup() {
  cubetto.init();
}

void loop() {
  cubetto.waitInstructions();
}
