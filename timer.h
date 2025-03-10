#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

class Timer {
public:
  Timer()
    : startTime(0), running(false) {}

  void start() {
    startTime = millis();
    running = true;
  }

  void stop() {
    running = false;
  }

  unsigned long getElapsedTime() const {
    if (running) {
      return (millis() - startTime) / 1000;  // Return elapsed time in seconds
    }
    return 0;
  }

  void getElapsedTime(int& minutes, int& seconds) const {
    unsigned long elapsed = getElapsedTime();
    minutes = elapsed / 60;
    seconds = elapsed % 60;
  }

  bool isRunning() const {
    return running;
  }

private:
  unsigned long startTime;
  bool running;
};

#endif
