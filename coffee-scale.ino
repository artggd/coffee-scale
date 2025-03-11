#include "screen.h"
#include "encoder.h"
#include "coffee-scale.h"
#include "timer.h"
#include <HX711_ADC.h>

// Define constants for pins
const int CLK_PIN = 2;
const int DT_PIN = 3;
const int SW_PIN = 4;
const int HX711_DOUT_PIN = 6;
const int HX711_SCK_PIN = 5;

Screen screen;
Encoder encoder(CLK_PIN, DT_PIN, SW_PIN);
HX711_ADC scale(HX711_DOUT_PIN, HX711_SCK_PIN);
Timer timer;
CoffeeScale coffeeScale(screen, encoder, scale, timer);

void setup() {
  coffeeScale.begin();
}

void loop() {
  coffeeScale.update();
}
