#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <math.h>
#include <Adafruit_GFX.h> // Core graphics library
#include <Fonts/FreeSansBold9pt7b.h>
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>
#include <Wire.h>

#define TFT_CS    10
#define TFT_RST   8
#define TFT_DC    9

#define TFT_SCLK 13
#define TFT_MOSI 11

uint32_t delayMS;

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void show_page() {
    tft.setFont(&FreeSansBold9pt7b);
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_RED);
    tft.setCursor(14, 22);
    tft.print("AZ-Delivery");
    tft.drawFastHLine(0, 35, 128, ST77XX_GREEN);
    tft.drawTriangle(1, 45, 28, 70, 55, 45, ST77XX_WHITE);
    tft.fillTriangle(78, 70, 104, 45, 127, 70, 0xA3F6);
    tft.drawRect(1, 80, 50, 30, ST77XX_BLUE);
    tft.fillRoundRect(78, 80, 50, 30, 5, 0x2D4E);
    tft.fillCircle(25, 135, 15, 0x5BA9);
    tft.drawCircle(102, 135, 15, ST77XX_GREEN);
    tft.drawLine(45, 150, 80, 40, ST77XX_ORANGE);
}
void changing_value(uint8_t value) {
    if(value < 10) {
        tft.setCursor(15, 88);
        tft.print("0");
        tft.print(value);
    }
    else {
        tft.setCursor(15, 88);
        tft.print(value);
    }
}

void setup() {
    Serial.begin(9600);
    tft.initR(INITR_GREENTAB);
    tft.fillScreen(ST77XX_BLACK);
    delay(500);
    show_page();
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setTextSize(2);
    tft.setFont(); // reset font to default
}

void loop() {
    for(uint8_t i = 0; i < 100; i++) {
        changing_value(i);
        // delay(100);
    }
}
