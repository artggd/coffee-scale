#ifndef WEIGHING_MODE_H
#define WEIGHING_MODE_H

class WeighingMode {
public:
  virtual ~WeighingMode() = default;
  virtual void update() = 0;
  virtual void onButtonShortPress() = 0;
  virtual void onIncrement(int increment) = 0;
};

#endif
