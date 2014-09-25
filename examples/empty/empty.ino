#include "SPI.h"
#include "AccelStepper.h"
#include "nRF24L01.h"
#include "RF24.h"
#include "RF24_config.h"
#include "Cubetto.h"

Cubetto cubetto;

void setup() {
  cubetto.init();  
  cubetto.forward(1000);  
}


void loop() {
  
}
