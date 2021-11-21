#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <math.h>
#include <Adafruit_GFX.h> // Core graphics library
#include <Fonts/FreeSansBold9pt7b.h>
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>
#include <Wire.h>
#include <RTClib.h>

#define TFT_CS    10
#define TFT_RST   8
#define TFT_DC    9

#define TFT_SCLK 13
#define TFT_MOSI 11

#define BG_COLOR ST77XX_BLACK

uint32_t delayMS;

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

float p = 3.1415926;

RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {
        "Sunday",
        "Monday",
        "Tuesday",
        "Wednesday",
        "Thursday",
        "Friday",
        "Saturday"
};


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
    explicit BinaryDisplay(int16_t x, int16_t y, int16_t w, int16_t h, int16_t nrOfBitsToDraw, uint16_t color,
                           uint16_t backgroundColor) {
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

class TimeDisplay {
private:
    int16_t startX;
    int16_t startY;
    uint16_t textColor;
    uint16_t bgColor;
    int currentHour = 0;
    int currentMinute = 0;
    int currentSecond = 0;

public:
    explicit TimeDisplay(int16_t x, int16_t y, uint16_t color, uint16_t backgroundColor) {
        startX = x;
        startY = y;
        textColor = color;
        bgColor = backgroundColor;
    }


    void draw(int hour, int minute, int second) {
        if (currentHour != hour) {
            tft.fillRect(startX, startY, 27, 17, bgColor);
        }
        if (currentMinute != minute) {
            tft.fillRect(startX + 35, startY, 27, 17, bgColor);
        }
        if (currentSecond != second) {
            tft.fillRect(startX + 70, startY, 27, 17, bgColor);
        }

        tft.setCursor(startX, startY);
        tft.setTextColor(textColor);
        tft.setTextSize(2);
        tft.print(hour, HEX);
        tft.print(":");
        tft.print(minute, HEX);
        tft.print(":");
        tft.print(second, HEX);

        currentHour = hour;
        currentMinute = minute;
        currentSecond = second;
    }

    void show(int hour, int minute, int second) {
        draw(hour, minute, second);
    }
};

class DateDisplay {
private:
    int16_t startX;
    int16_t startY;
    uint16_t textColor;
    uint16_t bgColor;
    int currentYear = 0;
    int currentMonth = 0;
    int currentDay = 0;

public:
    explicit DateDisplay(int16_t x, int16_t y, uint16_t color, uint16_t backgroundColor) {
        startX = x;
        startY = y;
        textColor = color;
        bgColor = backgroundColor;
    }


    void draw(int year, int month, int day) {
        if (currentYear != year) {
            tft.fillRect(startX, startY, 37, 17, bgColor);
        }

        if (currentMonth != month) {
            tft.fillRect(startX + 46, startY, 16, 17, bgColor);
        }

        if (currentDay != day) {
            tft.fillRect(startX + 70, startY, 27, 17, bgColor);
        }

        tft.setCursor(startX, startY);
        tft.setTextColor(textColor);
        tft.setTextSize(2);
        tft.print(year, HEX);
        tft.print(".");
        tft.print(month, HEX);
        tft.print(".");
        tft.print(day, HEX);

        currentYear = year;
        currentMonth = month;
        currentDay = day;
    }

    void show(int hour, int minute, int second) {
        draw(hour, minute, second);
    }
};

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

BinaryDisplay testDisplay8 = BinaryDisplay(100, 25, 8, 12, 8, ST77XX_YELLOW, BG_COLOR);
BinaryDisplay testDisplay4 = BinaryDisplay(100, 65, 8, 12, 4, ST77XX_CYAN, BG_COLOR);
DateDisplay dateDisplay = DateDisplay(17, 100, ST77XX_WHITE, BG_COLOR);
TimeDisplay timeDisplay = TimeDisplay(15, 130, ST77XX_WHITE, BG_COLOR);


void healthCheck() {
    // run health check
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
}

void setup(void) {
    Serial.begin(9600);
    delay(2000);

    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC. Make sure that you've connected the RTC to the correct pins");
        Serial.flush();
        abort();
    }

    Serial.println("Let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));


    // When time needs to be re-set on a previously configured device, the
    // following line sets the RTC to the date & time this sketch was compiled
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));

    Serial.print(F("ST77xx TFT Test"));
    tft.initR(INITR_BLACKTAB);
    Serial.println(F("Initialized"));

    tft.setCursor(42, 10);
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(0);
    tft.println("Temperature");
    tft.setCursor(60, 50);
    tft.println("Humidity");

    testDisplay8.show(0);
    testDisplay4.show(0);

    delay(500);


    //Check();
    Serial.println("done");
}

void loop() {
    DateTime now = rtc.now();
    timeDisplay.show(now.hour(), now.minute(), now.second());
    dateDisplay.show(now.year(), now.month(), now.day());

    delay(200);
}

