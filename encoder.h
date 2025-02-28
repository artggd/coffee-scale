#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

#define CLK 2  // Rotary encoder CLK (A)
#define DT 3   // Rotary encoder DT (B)
#define SW 4   // Rotary encoder Button

int ratioWater = 15;  // Default ratio (1:15)
int lastEncoderState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 3;  // 3ms debounce (tuned for smooth rotation)

void updateEncoder() {
    int clkState = digitalRead(CLK);

    if (clkState != lastEncoderState && clkState == LOW) {  // Detect LOW → HIGH transition
        if (digitalRead(DT) == LOW) {
            ratioWater--;  // Counterclockwise
        } else {
            ratioWater++;  // Clockwise
        }

        // Limit range
        if (ratioWater < 1) ratioWater = 1;
        if (ratioWater > 30) ratioWater = 30;

        lastDebounceTime = millis();
    }

    lastEncoderState = clkState;
}

void setupEncoder() {
    pinMode(CLK, INPUT);
    pinMode(DT, INPUT);
    pinMode(SW, INPUT_PULLUP);  // Button input with pull-up
}

#endif
