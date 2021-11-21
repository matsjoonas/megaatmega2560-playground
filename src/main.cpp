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

float p = 3.1415926;

class BinaryDisplay {
private:
    int16_t startX;
    int16_t startY;
    int16_t rectW;
    int16_t rectH;
    int16_t amountOfBitsToDraw;
    int state[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int totalBitLength = 8;
    uint16_t borderColor;
    uint16_t bgColor;
    bool isFirstDraw = true;

public:
    explicit BinaryDisplay(int16_t x, int16_t y, int16_t w, int16_t h, int16_t nrOfBitsToDraw, uint16_t color, uint16_t backgroundColor) {
        startX = x;
        startY = y;
        rectW = w;
        rectH = h;
        amountOfBitsToDraw = nrOfBitsToDraw;
        borderColor = color;
        bgColor = backgroundColor;
    }

    int *decToBinary(int n) const {
        static int binaryNum[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        int i = 0;
        int j = 0;
        // set all bits to 0 before doing anything
        for (j = 0; j < totalBitLength; ++j) {
            binaryNum[j] = 0;
        }
        while (n > 0) {
            binaryNum[totalBitLength - i - 1] = n % 2;
            n = n / 2;
            i++;
        }
        return binaryNum;
    }


    void draw(int n) {
        int *newState = decToBinary(n);

        int16_t x = startX;
        int16_t y = startY;
        int16_t gapSize = 4;

        Serial.print("y: ");
        Serial.println(y);

        int i;
        for (i = 0; i < amountOfBitsToDraw; ++i) {
            // we only need to draw if newstate is different from the old state
            int newBit = newState[totalBitLength - i - 1];
            int oldBit = state[totalBitLength - i - 1];

            // we need to draw al the bits in case of first render
            // afterwards we can update only the places that have changed
            if (newBit != oldBit || isFirstDraw) {
                if (newBit == 1) {
                    tft.fillRect(x, y, rectW, rectH, borderColor);
                } else {
                    tft.fillRect(x, y, rectW, rectH, bgColor);
                    tft.drawRect(x, y, rectW, rectH, borderColor);
                }
            }

            x = x - (rectW + gapSize);
        }

        // update state
        int j;
        for (j = 0; j < totalBitLength; ++j) {
            state[j] = newState[j];
        }

        isFirstDraw = false;
    }

    void show(int n) {
        draw(n);
    }
};

BinaryDisplay testDisplay8 = BinaryDisplay(100, 10, 8, 12, 8, ST77XX_WHITE, ST77XX_BLACK);
BinaryDisplay testDisplay4 = BinaryDisplay(100, 25, 8, 12, 4, ST77XX_WHITE, ST77XX_BLACK);












void testlines(uint16_t color) {
    tft.fillScreen(ST77XX_BLACK);
    for (int16_t x = 0; x < tft.width(); x += 6) {
        tft.drawLine(0, 0, x, tft.height() - 1, color);
        delay(0);
    }
    for (int16_t y = 0; y < tft.height(); y += 6) {
        tft.drawLine(0, 0, tft.width() - 1, y, color);
        delay(0);
    }

    tft.fillScreen(ST77XX_BLACK);
    for (int16_t x = 0; x < tft.width(); x += 6) {
        tft.drawLine(tft.width() - 1, 0, x, tft.height() - 1, color);
        delay(0);
    }
    for (int16_t y = 0; y < tft.height(); y += 6) {
        tft.drawLine(tft.width() - 1, 0, 0, y, color);
        delay(0);
    }

    tft.fillScreen(ST77XX_BLACK);
    for (int16_t x = 0; x < tft.width(); x += 6) {
        tft.drawLine(0, tft.height() - 1, x, 0, color);
        delay(0);
    }
    for (int16_t y = 0; y < tft.height(); y += 6) {
        tft.drawLine(0, tft.height() - 1, tft.width() - 1, y, color);
        delay(0);
    }

    tft.fillScreen(ST77XX_BLACK);
    for (int16_t x = 0; x < tft.width(); x += 6) {
        tft.drawLine(tft.width() - 1, tft.height() - 1, x, 0, color);
        delay(0);
    }
    for (int16_t y = 0; y < tft.height(); y += 6) {
        tft.drawLine(tft.width() - 1, tft.height() - 1, 0, y, color);
        delay(0);
    }
}

void testdrawtext(char *text, uint16_t color) {
    tft.setCursor(0, 0);
    tft.setTextColor(color);
    tft.setTextWrap(true);
    tft.print(text);
}

void testfastlines(uint16_t color1, uint16_t color2) {
    tft.fillScreen(ST77XX_BLACK);
    for (int16_t y = 0; y < tft.height(); y += 5) {
        tft.drawFastHLine(0, y, tft.width(), color1);
    }
    for (int16_t x = 0; x < tft.width(); x += 5) {
        tft.drawFastVLine(x, 0, tft.height(), color2);
    }
}

void testdrawrects(uint16_t color) {
    tft.fillScreen(ST77XX_BLACK);
    for (int16_t x = 0; x < tft.width(); x += 6) {
        tft.drawRect(tft.width() / 2 - x / 2, tft.height() / 2 - x / 2, x, x, color);
    }
}

void testfillrects(uint16_t color1, uint16_t color2) {
    tft.fillScreen(ST77XX_BLACK);
    for (int16_t x = tft.width() - 1; x > 6; x -= 6) {
        tft.fillRect(tft.width() / 2 - x / 2, tft.height() / 2 - x / 2, x, x, color1);
        tft.drawRect(tft.width() / 2 - x / 2, tft.height() / 2 - x / 2, x, x, color2);
    }
}

void testfillcircles(uint8_t radius, uint16_t color) {
    for (int16_t x = radius; x < tft.width(); x += radius * 2) {
        for (int16_t y = radius; y < tft.height(); y += radius * 2) {
            tft.fillCircle(x, y, radius, color);
        }
    }
}

void testdrawcircles(uint8_t radius, uint16_t color) {
    for (int16_t x = 0; x < tft.width() + radius; x += radius * 2) {
        for (int16_t y = 0; y < tft.height() + radius; y += radius * 2) {
            tft.drawCircle(x, y, radius, color);
        }
    }
}

void testtriangles() {
    tft.fillScreen(ST77XX_BLACK);
    uint16_t color = 0xF800;
    int t;
    int w = tft.width() / 2;
    int x = tft.height() - 1;
    int y = 0;
    int z = tft.width();
    for (t = 0; t <= 15; t++) {
        tft.drawTriangle(w, y, y, x, z, x, color);
        x -= 4;
        y += 4;
        z -= 4;
        color += 100;
    }
}

void testroundrects() {
    tft.fillScreen(ST77XX_BLACK);
    uint16_t color = 100;
    int i;
    int t;
    for (t = 0; t <= 4; t += 1) {
        int x = 0;
        int y = 0;
        int w = tft.width() - 2;
        int h = tft.height() - 2;
        for (i = 0; i <= 16; i += 1) {
            tft.drawRoundRect(x, y, w, h, 5, color);
            x += 2;
            y += 3;
            w -= 4;
            h -= 6;
            color += 1100;
        }
        color += 100;
    }
}

void tftPrintTest() {
    tft.setTextWrap(false);
    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(0, 30);
    tft.setTextColor(ST77XX_RED);
    tft.setTextSize(1);
    tft.println("Hello World!");
    tft.setTextColor(ST77XX_YELLOW);
    tft.setTextSize(2);
    tft.println("Hello World!");
    tft.setTextColor(ST77XX_GREEN);
    tft.setTextSize(3);
    tft.println("Hello World!");
    tft.setTextColor(ST77XX_BLUE);
    tft.setTextSize(4);
    tft.print(1234.567);
    delay(1500);
    tft.setCursor(0, 0);
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(0);
    tft.println("Hello World!");
    tft.setTextSize(1);
    tft.setTextColor(ST77XX_GREEN);
    tft.print(p, 6);
    tft.println(" Want pi?");
    tft.println(" ");
    tft.print(8675309, HEX); // print 8,675,309 out in HEX!
    tft.println(" Print HEX!");
    tft.println(" ");
    tft.setTextColor(ST77XX_WHITE);
    tft.println("Sketch has been");
    tft.println("running for: ");
    tft.setTextColor(ST77XX_MAGENTA);
    tft.print(millis() / 1000);
    tft.setTextColor(ST77XX_WHITE);
    tft.print(" seconds.");
}

void mediabuttons() {
    // play
    tft.fillScreen(ST77XX_BLACK);
    tft.fillRoundRect(25, 10, 78, 60, 8, ST77XX_WHITE);
    tft.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_RED);
    delay(500);
    // pause
    tft.fillRoundRect(25, 90, 78, 60, 8, ST77XX_WHITE);
    tft.fillRoundRect(39, 98, 20, 45, 5, ST77XX_GREEN);
    tft.fillRoundRect(69, 98, 20, 45, 5, ST77XX_GREEN);
    delay(500);
    // play color
    tft.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_BLUE);
    delay(50);
    // pause color
    tft.fillRoundRect(39, 98, 20, 45, 5, ST77XX_RED);
    tft.fillRoundRect(69, 98, 20, 45, 5, ST77XX_RED);
    // play color
    tft.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_GREEN);
}

void setup(void) {
    Serial.begin(9600);
    Serial.print(F("Hello! ST77xx TFT Test"));
    tft.initR(INITR_BLACKTAB);

    Serial.println(F("Initialized"));

    uint16_t time = millis();
    tft.fillScreen(ST77XX_BLACK);
    time = millis() - time;

    Serial.println(time, DEC);
    delay(500);
    Serial.println("done");
}

void loop() {
    testDisplay8.show(1);
    delay(300);
    testDisplay8.show(2);
    delay(300);
    testDisplay8.show(4);
    delay(300);
    testDisplay8.show(8);
    delay(300);
    testDisplay8.show(16);
    delay(300);
    testDisplay8.show(32);
    delay(300);
    testDisplay8.show(64);
    delay(300);
    testDisplay8.show(128);
    delay(300);
    testDisplay8.show(255);
    delay(300);
    testDisplay4.show(1);
    delay(300);
    testDisplay4.show(2);
    delay(300);
    testDisplay4.show(4);
    delay(300);
    testDisplay4.show(8);
}

