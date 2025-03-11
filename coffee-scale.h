#ifndef COFFEE_SCALE_H
#define COFFEE_SCALE_H

#include "screen.h"
#include "encoder.h"
#include "timer.h"
#include "WeighingMode.h"
#include "CoffeeWeighingMode.h"
#include "WaterWeighingMode.h"
#include "ScaleData.h"
#include <HX711_ADC.h>
#include <esp_sleep.h>

// Define the calibration factor
const float CALIBRATION_FACTOR = 2199.24;
const int INITIAL_RATIO = 12;
const int WEIGHT_THRESHOLD = 10;                 // Threshold to start the timer

class CoffeeScale {
public:
  CoffeeScale(Screen& screen, Encoder& encoder, HX711_ADC& scale, Timer& timer)
    : screen(screen),
      encoder(encoder),
      scale(scale),
      timer(timer),
      scaleData(),
      coffeeWeighingMode(new CoffeeWeighingMode(screen, scale, scaleData, INITIAL_RATIO)),
      waterWeighingMode(new WaterWeighingMode(screen, scale, timer, scaleData, WEIGHT_THRESHOLD)),
      currentMode(coffeeWeighingMode),
      lastActivityTime(millis()) {
    encoder.setOnIncrementCallback([this](int increment) {
      onActivity();
      currentMode->onIncrement(increment);
    });
    encoder.setOnButtonLongPressCallback([this]() {
      onActivity();
      onButtonLongPress();
    });
    encoder.setOnButtonShortPressCallback([this]() {
      onActivity();
      currentMode->onButtonShortPress();
    });
  }

  ~CoffeeScale() {
    delete coffeeWeighingMode;
    delete waterWeighingMode;
  }

  void begin() {
    screen.begin();
    encoder.setup();
    scale.begin();
    scale.start(2000, true);
    scale.setCalFactor(CALIBRATION_FACTOR);
  }

  void update() {
    encoder.update();
    scale.update();
    currentMode->update();
  }

private:
  Screen& screen;
  Encoder& encoder;
  HX711_ADC& scale;
  Timer& timer;
  ScaleData scaleData;
  WeighingMode* coffeeWeighingMode;
  WeighingMode* waterWeighingMode;
  WeighingMode* currentMode;
  unsigned long lastActivityTime;

  void onButtonLongPress() {
    if (currentMode == coffeeWeighingMode) {
      currentMode = waterWeighingMode;
    } else {
      currentMode = coffeeWeighingMode;
      timer.stop();
    }
  }

  void onActivity() {
    lastActivityTime = millis();
  }
};

#endif
