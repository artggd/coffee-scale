#ifndef COFFEE_WEIGHING_MODE_H
#define COFFEE_WEIGHING_MODE_H

#include "WeighingMode.h"
#include "screen.h"
#include "ScaleData.h"
#include <HX711_ADC.h>

class CoffeeWeighingMode : public WeighingMode {
public:
  CoffeeWeighingMode(Screen& screen, HX711_ADC& scale, ScaleData& scaleData, int initialRatio)
    : screen(screen), scale(scale), scaleData(scaleData), ratio(initialRatio), filteredWeight(0), alpha(0.1) {}

  void update() override {
    // Get raw weight data from the scale
    float rawWeight = scale.getData();

    // Apply a simple low-pass filter
    filteredWeight = (alpha * rawWeight) + ((1 - alpha) * filteredWeight);

    // Ensure the filtered weight is not negative when close to zero
    if (abs(filteredWeight) < 0.1) {
      filteredWeight = 0;
    }

    scaleData.coffeeWeight = filteredWeight;
    scaleData.targetWaterWeight = round(scaleData.coffeeWeight * ratio);
    screen.displayCoffeeWeighingMode(scaleData.coffeeWeight, scaleData.targetWaterWeight, ratio);
  }

  void onIncrement(int increment) {
    ratio += increment;
    if (ratio < 1) {
      ratio = 1;
    }
  }

  void onButtonShortPress() override {
    scale.tare();
    filteredWeight = 0;  // Reset the filtered weight after tare
  }

private:
  Screen& screen;
  HX711_ADC& scale;
  ScaleData& scaleData;
  int ratio;
  float filteredWeight;
  const float alpha;  // Smoothing factor for the low-pass filter
  const int INITIAL_RATIO = 12;
};

#endif
