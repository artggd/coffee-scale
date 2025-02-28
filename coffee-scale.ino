#include "screen.h"
#include "encoder.h"

Screen screen;
const float coffeeWeight = 25.4;  // Example: 25.4g of coffee

void setup() {
    setupEncoder();  // Initialize encoder
    screen.begin();  // Initialize OLED display

    screen.clearBuffer();
    screen.displayCoffeeWeight(coffeeWeight);  
    screen.displayWaterWeight(round(coffeeWeight * ratioWater));  // Round water weight
    screen.displayLabels();
    screen.displayRatio(1, ratioWater);
    screen.sendBuffer();
}

void loop() {
    updateEncoder();  // Check encoder state in loop (no interrupts)

    static int lastRatio = 0;

    if (ratioWater != lastRatio) {
        screen.clearBuffer();
        screen.displayCoffeeWeight(coffeeWeight);
        screen.displayWaterWeight(round(coffeeWeight * ratioWater));  // Round water weight
        screen.displayLabels();
        screen.displayRatio(1, ratioWater);
        screen.sendBuffer();

        lastRatio = ratioWater;
    }

    delay(5);  // Small delay for stability
}
