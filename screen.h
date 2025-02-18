#include <Wire.h>
#include <U8g2lib.h>

class Screen {
public:
    Screen(): u8g2(U8G2_R0, U8X8_PIN_NONE) {}

    void begin() {
        u8g2.begin();
        u8g2.enableUTF8Print();
    }

    void displayCoffeeWeight(float weight) {
        u8g2.setFont(u8g2_font_logisoso24_tr);
        u8g2.setCursor(0, 25);
        u8g2.print(weight, 1);
    }

    void displayWaterWeight(int weight) {
        u8g2.setFont(u8g2_font_logisoso24_tr);
        String weightString = String(weight);
        int x = 128 - u8g2.getStrWidth(weightString.c_str());
        u8g2.setCursor(x, 25);
        u8g2.print(weight);
    }

    void displayLabels() {
        u8g2.setFont(u8g2_font_6x13_tf);
        u8g2.setCursor(5, 35);
        u8g2.print("caf\u00E9");

        int x = 128 - u8g2.getStrWidth("eau") - 5;
        u8g2.setCursor(x, 35);
        u8g2.print("eau");
    }

    void displayRatio(int coffee, int water) {
        u8g2.drawRFrame(35, 40, 58, 24, 5);
        u8g2.setFont(u8g2_font_10x20_te);
        String ratioString = String(coffee) + ":" + String(water);
        int x = 35 + (58 - u8g2.getStrWidth(ratioString.c_str())) / 2;
        int y = 40 + (24 - 20) / 2 + 20 - 4;
        u8g2.setCursor(x, y);
        u8g2.print(ratioString);
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