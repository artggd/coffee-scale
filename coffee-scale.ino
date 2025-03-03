#include "screen.h"
#include "encoder.h"

Screen screen;
Encoder encoder(2, 3, 4);         // CLK, DT, SW pins
const float coffeeWeight = 25.4;  // Example: 25.4g of coffee
int targetWaterWeight;
bool weighingWater = false;
unsigned long modeSwitchTime = 0;  // Time when button is pressed
unsigned long startTime = 0;       // Time when water mode starts
float calibrationFactor = 2199.24;

void setup() {
  encoder.setup();  // Initialize encoder
  screen.begin();   // Initialize OLED display

  targetWaterWeight = round(coffeeWeight * encoder.getRatioWater());  // Initial water weight

  screen.clearBuffer();
  screen.displayWeightOnLeft(coffeeWeight, "caf\u00E9");
  screen.displayWeightOnRight(targetWaterWeight, "eau");
  screen.displayRatio(1, encoder.getRatioWater());
  screen.sendBuffer();
}

void loop() {
  encoder.update();  // Check encoder state in loop

  static int lastRatio = 0;

  // Detect button press (hold for 1 second to switch modes)
  if (encoder.isButtonPressed()) {
    if (modeSwitchTime == 0) {
      modeSwitchTime = millis();  // Start timing the press
    } else if (millis() - modeSwitchTime >= 1000) {
      weighingWater = !weighingWater;  // Toggle water weighing mode
      startTime = millis();            // Reset timer when entering water mode
      modeSwitchTime = 0;              // Reset button timing
    }
  } else {
    modeSwitchTime = 0;  // Reset if button is released early
  }

  if (!weighingWater && encoder.getRatioWater() != lastRatio) {
    targetWaterWeight = round(coffeeWeight * encoder.getRatioWater());  // Update target weight
    screen.clearBuffer();
    screen.displayWeightOnLeft(coffeeWeight, "caf\u00E9");
    screen.displayWeightOnRight(targetWaterWeight, "eau");
    screen.displayRatio(1, encoder.getRatioWater());
    screen.sendBuffer();

    lastRatio = encoder.getRatioWater();
  }

  if (weighingWater) {
    unsigned long elapsed = (millis() - startTime) / 1000;  // Get elapsed time in seconds
    int minutes = elapsed / 60;
    int seconds = elapsed % 60;

    screen.clearBuffer();
    screen.displayWaterWeighingMode(0, targetWaterWeight, coffeeWeight, minutes, seconds);
    screen.sendBuffer();
  }

  delay(5);
}
