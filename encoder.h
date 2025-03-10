#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>
#include <functional>

class Encoder {
public:
  Encoder(int clkPin, int dtPin, int swPin)
    : clkPin(clkPin), dtPin(dtPin), swPin(swPin), position(0), lastEncoderState(HIGH), lastDebounceTime(0), buttonPressed(false), buttonPressTime(0) {}

  void setup() {
    pinMode(clkPin, INPUT);
    pinMode(dtPin, INPUT);
    pinMode(swPin, INPUT_PULLUP);  // Button input with pull-up
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
    } else if (buttonPressed) {
      unsigned long pressDuration = millis() - buttonPressTime;

      if (pressDuration >= 1000) {  // Long press threshold
        if (onButtonLongPress) {
          onButtonLongPress();
        }
        Serial.println("Long press detected");  // Debug message
        buttonPressed = false;  // Reset button state to avoid multiple detections
      }

      if (!currentButtonState) {
        buttonPressed = false;
        if (pressDuration < 1000) {  // Short press threshold
          if (onButtonShortPress) {
            onButtonShortPress();
          }
          Serial.println("Short press detected");  // Debug message
        }
      }
    }
  }

  void setOnPositionChangedCallback(std::function<void(int)> callback) {
    onPositionChanged = callback;
  }

  void setOnButtonLongPressCallback(std::function<void()> callback) {
    onButtonLongPress = callback;
  }

  void setOnButtonShortPressCallback(std::function<void()> callback) {
    onButtonShortPress = callback;
  }

  int getSwPin() const {
    return swPin;
  }

private:
  int clkPin;
  int dtPin;
  int swPin;
  int position;
  int lastEncoderState;
  unsigned long lastDebounceTime;
  bool buttonPressed;
  unsigned long buttonPressTime;
  std::function<void(int)> onPositionChanged;
  std::function<void()> onButtonLongPress;
  std::function<void()> onButtonShortPress;
};

#endif
