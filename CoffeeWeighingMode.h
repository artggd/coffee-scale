#ifndef COFFEE_WEIGHING_MODE_H
#define COFFEE_WEIGHING_MODE_H

#include "WeighingMode.h"
#include "screen.h"
#include "ScaleData.h"
#include <HX711_ADC.h>

class CoffeeWeighingMode : public WeighingMode {
public:
  CoffeeWeighingMode(Screen& screen, HX711_ADC& scale, ScaleData& scaleData, int initialRatio)
    : screen(screen), scale(scale), scaleData(scaleData), ratio(initialRatio) {}

  void update() override {
    scaleData.coffeeWeight = scale.getData();
    scaleData.targetWaterWeight = round(scaleData.coffeeWeight * ratio);
    screen.displayCoffeeWeighingMode(scaleData.coffeeWeight, scaleData.targetWaterWeight, ratio);
  }

  void onPositionChanged(int position) override {
    ratio = position + INITIAL_RATIO;
  }

  void onButtonShortPress() override {
    scale.tare();
  }

private:
  Screen& screen;
  HX711_ADC& scale;
  ScaleData& scaleData;
  int ratio;
  const int INITIAL_RATIO = 12;
};

#endif
