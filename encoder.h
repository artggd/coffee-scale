#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>
#include <functional>
#include <BfButton.h>

class Encoder {
public:
  Encoder(int clkPin, int dtPin, int swPin)
    : clkPin(clkPin), dtPin(dtPin), swPin(swPin), position(0), button(BfButton::STANDALONE_DIGITAL, swPin, true, LOW), lastClkState(HIGH), lastDtState(HIGH), lastDebounceTime(0) {
    instance = this;  // Set the static instance pointer
  }

  void setup() {
    pinMode(clkPin, INPUT);
    pinMode(dtPin, INPUT);
    pinMode(swPin, INPUT_PULLUP);

    // Initialize previous states
    lastClkState = digitalRead(clkPin);
    lastDtState = digitalRead(dtPin);

    // Attach interrupts for CLK
    attachInterrupt(digitalPinToInterrupt(clkPin), handleClkInterrupt, CHANGE);

    // Set up ButtonFever for the button
    button.onPress(handleButtonPress)
          .onPressFor(handleButtonPress, 1000);
  }

  void update() {
    button.read();  // Update the button state
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
  volatile int position;
  int lastClkState;
  int lastDtState;
  unsigned long lastDebounceTime;
  BfButton button;
  static Encoder* instance;  // Static pointer to the instance
  std::function<void(int)> onPositionChanged;
  std::function<void()> onButtonLongPress;
  std::function<void()> onButtonShortPress;

  static void handleClkInterrupt() {
    unsigned long currentTime = millis();

    // Debounce delay
    if (currentTime - instance->lastDebounceTime < 5) {
      return;
    }

    int clkState = digitalRead(instance->clkPin);
    int dtState = digitalRead(instance->dtPin);

    // Check for valid transitions to determine rotation direction
    if (clkState == HIGH && dtState == LOW && instance->lastClkState == LOW && instance->lastDtState == HIGH) {
      instance->position++;
      Serial.print(F("Direction = Clockwise | Position = "));
      Serial.println(instance->position);
    } else if (clkState == HIGH && dtState == HIGH && instance->lastClkState == LOW && instance->lastDtState == LOW) {
      instance->position--;
      Serial.print(F("Direction = Counterclockwise | Position = "));
      Serial.println(instance->position);
    }

    // Update previous states
    instance->lastClkState = clkState;
    instance->lastDtState = dtState;
    instance->lastDebounceTime = currentTime;

    if (instance->onPositionChanged) {
      instance->onPositionChanged(instance->position);
    }
  }

  static void handleButtonPress(BfButton* btn, BfButton::press_pattern_t pattern) {
    if (pattern == BfButton::SINGLE_PRESS) {
      Serial.println("Short press detected");
      if (instance->onButtonShortPress) {
        instance->onButtonShortPress();
      }
    } else if (pattern == BfButton::LONG_PRESS) {
      Serial.println("Long press detected");
      if (instance->onButtonLongPress) {
        instance->onButtonLongPress();
      }
    }
  }
};

// Initialize the static member
Encoder* Encoder::instance = nullptr;

#endif
