#ifndef SCREEN_H
#define SCREEN_H

#include <Wire.h>
#include <U8g2lib.h>

class Screen {
public:
  Screen()
    : u8g2(U8G2_R0, U8X8_PIN_NONE) {}

  void begin() {
    u8g2.begin();
    u8g2.enableUTF8Print();
  }

  void displayWeightOnLeft(float weight, const String& label) {
    u8g2.setFont(u8g2_font_logisoso24_tr);
    u8g2.setCursor(0, 25);
    u8g2.print(weight, 1);

    u8g2.setFont(u8g2_font_6x13_tf);
    u8g2.setCursor(5, 35);
    u8g2.print(label);
  }

  void displayWeightOnRight(int weight, const String& label) {
    u8g2.setFont(u8g2_font_logisoso24_tr);
    String weightString = String(weight);
    int x = 128 - u8g2.getStrWidth(weightString.c_str());
    u8g2.setCursor(x, 25);
    u8g2.print(weight);

    u8g2.setFont(u8g2_font_6x13_tf);
    int x_label = 128 - u8g2.getStrWidth(label.c_str()) - 5;
    u8g2.setCursor(x_label, 35);
    u8g2.print(label);
  }

  void displayCenteredBoxText(const String& text) {
    int width = 65;
    int height = 24;
    u8g2.drawRFrame(35, 40, width, height, 5);
    u8g2.setFont(u8g2_font_10x20_te);
    int x = 35 + (width - u8g2.getStrWidth(text.c_str())) / 2;
    int y = 40 + (height - 20) / 2 + 20 - 4;
    u8g2.setCursor(x, y);
    u8g2.print(text);
  }

  void displayRatio(int coffee, int water) {
    displayCenteredBoxText(String(coffee) + ":" + String(water));
  }

  void displayWaterWeighingMode(int currentWater, int targetWater, float coffeeWeight, int minutes, int seconds) {
    u8g2.clearBuffer();

    displayWeightOnLeft(currentWater, "eau");

    // Coffee Weight (Small, bottom-aligned with the ratio/timer box on the left)
    u8g2.setFont(u8g2_font_6x13_tf);
    int coffeeY = 64;  // Aligns to the bottom of the display
    u8g2.setCursor(5, coffeeY);
    u8g2.print(coffeeWeight, 1);

    displayWeightOnRight(targetWater, "cible");

    // Timer (Replaces Ratio)
    char timeString[6];
    sprintf(timeString, "%02d:%02d", minutes, seconds);
    displayCenteredBoxText(timeString);
  }

  void displayCoffeeWeighingMode(float coffeeWeight, int targetWaterWeight, int ratio) {
    u8g2.clearBuffer();
    displayWeightOnLeft(coffeeWeight, "caf\u00E9");
    displayWeightOnRight(targetWaterWeight, "eau");
    displayRatio(1, ratio);
    u8g2.sendBuffer();
  }

  void clearBuffer() {
    u8g2.clearBuffer();
  }

  void sendBuffer() {
    u8g2.sendBuffer();
  }

private:
  U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
};

#endif
