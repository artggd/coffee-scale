#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

class Encoder {
public:
  Encoder(int clkPin, int dtPin, int swPin)
    : clkPin(clkPin), dtPin(dtPin), swPin(swPin), ratioWater(15), lastEncoderState(HIGH), lastDebounceTime(0) {}

  void setup() {
    pinMode(clkPin, INPUT);
    pinMode(dtPin, INPUT);
    pinMode(swPin, INPUT_PULLUP);  // Button input with pull-up
  }

  void update() {
    int clkState = digitalRead(clkPin);

    if (clkState != lastEncoderState && clkState == LOW) {  // Detect LOW → HIGH transition
      if (digitalRead(dtPin) == LOW) {
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

  int getRatioWater() const {
    return ratioWater;
  }

  bool isButtonPressed() const {
    return digitalRead(swPin) == LOW;
  }

private:
  int clkPin;
  int dtPin;
  int swPin;
  int ratioWater;
  int lastEncoderState;
  unsigned long lastDebounceTime;
  const unsigned long debounceDelay = 3;  // 3ms debounce (tuned for smooth rotation)
};

#endif
