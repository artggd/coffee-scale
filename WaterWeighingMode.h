#ifndef WATER_WEIGHING_MODE_H
#define WATER_WEIGHING_MODE_H

#include "WeighingMode.h"
#include "screen.h"
#include "timer.h"
#include "ScaleData.h"
#include <HX711_ADC.h>

class WaterWeighingMode : public WeighingMode {
public:
  WaterWeighingMode(Screen& screen, HX711_ADC& scale, Timer& timer, ScaleData& scaleData, int threshold)
    : screen(screen), scale(scale), timer(timer), scaleData(scaleData), threshold(threshold), tareDone(false) {}

  void update() override {
    int minutes, seconds;
    timer.getElapsedTime(minutes, seconds);

    int currentWaterWeight = scale.getData();

    if (!timer.isRunning() && tareDone && currentWaterWeight > threshold) {
      timer.start();
    }

    screen.clearBuffer();
    screen.displayWaterWeighingMode(currentWaterWeight, scaleData.targetWaterWeight, scaleData.coffeeWeight, minutes, seconds);
    screen.sendBuffer();
  }

  void onButtonShortPress() override {
    scale.tare();
    tareDone = true;
  }

  void onPositionChanged(int position) override {
    // No action needed for position change in water weighing mode
  }

private:
  Screen& screen;
  HX711_ADC& scale;
  Timer& timer;
  ScaleData& scaleData;
  int threshold;
  bool tareDone;
};

#endif
