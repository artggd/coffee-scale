#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>
#include <functional>

class Encoder {
public:
  Encoder(int clkPin, int dtPin, int swPin)
    : clkPin(clkPin), dtPin(dtPin), swPin(swPin), position(0), lastEncoderState(HIGH), lastDebounceTime(0) {}

  void setup() {
    pinMode(clkPin, INPUT);
    pinMode(dtPin, INPUT);
    pinMode(swPin, INPUT_PULLUP);  // Button input with pull-up
  }

  int getSwPin() const {
    return swPin;
  }
void update() {
    int clkState = digitalRead(clkPin);

    // Handle encoder rotation
    if (clkState != lastEncoderState && clkState == LOW) {  // Detect LOW → HIGH transition
      if (digitalRead(dtPin) == LOW) {
        position--;  // Counterclockwise
      } else {
        position++;  // Clockwise
      }

      lastDebounceTime = millis();
      if (onPositionChanged) {
        onPositionChanged(position);
      }
    }

    lastEncoderState = clkState;

    // Handle button press
    bool currentButtonState = (digitalRead(swPin) == LOW);

    if (currentButtonState && !buttonPressed) {
      buttonPressed = true;
      buttonPressTime = millis();
      Serial.println("Button pressed");  // Debug message
    } else if (!currentButtonState && buttonPressed) {
      buttonPressed = false;
      unsigned long pressDuration = millis() - buttonPressTime;

      if (pressDuration < 1000) {
        if (onButtonShortPress) {
          onButtonShortPress();
        }
        Serial.println("Short press detected");  // Debug message
      } else {
        if (onButtonLongPress) {
          onButtonLongPress();
        }
        Serial.println("Long press detected");  // Debug message
      }
    }
}


  int getPosition() const {
    return position;
  }

  void setOnPositionChangedCallback(const std::function<void(int)>& callback) {
    onPositionChanged = callback;
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
  int position;
  int lastEncoderState;
  unsigned long lastDebounceTime;
  bool buttonPressed = false;
  unsigned long buttonPressTime = 0;
  const unsigned long debounceDelay = 3;  // 3ms debounce (tuned for smooth rotation)

  std::function<void(int)> onPositionChanged;
  std::function<void()> onButtonLongPress;
  std::function<void()> onButtonShortPress;
};

#endif
