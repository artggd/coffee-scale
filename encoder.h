#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>
#include <functional>

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
      if (onRatioChanged) {
        onRatioChanged(ratioWater);
      }
    }

    lastEncoderState = clkState;

    if (digitalRead(swPin) == LOW && !buttonPressed) {
      buttonPressed = true;
      buttonPressTime = millis();
    } else if (digitalRead(swPin) == HIGH && buttonPressed) {
      buttonPressed = false;
      if (millis() - buttonPressTime >= 1000) {
        if (onButtonLongPress) {
          onButtonLongPress();
        }
      }
    }

    if (digitalRead(swPin) == LOW && !buttonPressed) {
      buttonPressed = true;
      buttonPressTime = millis();
    } else if (digitalRead(swPin) == HIGH && buttonPressed) {
      buttonPressed = false;
      if (millis() - buttonPressTime < 1000) {
        if (onButtonShortPress) {
          onButtonShortPress();
        }
      } else {
        if (onButtonLongPress) {
          onButtonLongPress();
        }
      }
    }
  }

  int getRatioWater() const {
    return ratioWater;
  }

  void setOnRatioChangedCallback(const std::function<void(int)>& callback) {
    onRatioChanged = callback;
  }

  void setOnButtonLongPressCallback(const std::function<void()>& callback) {
    onButtonLongPress = callback;
  }

  void setOnButtonShortPressCallback(const std::function<void()>& callback) {
    onButtonShortPress = callback;
  }


private:
  int clkPin;
  int dtPin;
  int swPin;
  int ratioWater;
  int lastEncoderState;
  unsigned long lastDebounceTime;
  bool buttonPressed = false;
  unsigned long buttonPressTime = 0;
  const unsigned long debounceDelay = 3;  // 3ms debounce (tuned for smooth rotation)

  std::function<void(int)> onRatioChanged;
  std::function<void()> onButtonLongPress;
  std::function<void()> onButtonShortPress;

};

#endif
