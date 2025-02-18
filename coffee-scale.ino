#include "screen.h"

float coffeeWeight = 10.0;
int ratioCoffee = 1;
int ratioWater = 16;
float weightStep = 0.1;

Screen screen;

void setup() {
    screen.begin();
}

void loop() {
    screen.clearBuffer();

    coffeeWeight += weightStep;
    if (coffeeWeight > 20.0) {
        coffeeWeight = 10.0;
    }

    int waterWeight = coffeeWeight * ratioWater / ratioCoffee;

    screen.displayCoffeeWeight(coffeeWeight);
    screen.displayWaterWeight(waterWeight);
    screen.displayLabels();
    screen.displayRatio(ratioCoffee, ratioWater);

    screen.sendBuffer();

    delay(10);
}