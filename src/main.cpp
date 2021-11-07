#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

DHT_Unified dht(22, DHT22);
uint32_t delayMS;

class LedOctet {
    int pins[8]{};
    int nrOfPins = {8};

public:
    explicit LedOctet(const int providedPins[8]) {
        int i;
        for (i = 0; i < nrOfPins; ++i) {
            pins[i] = providedPins[i];
        }
    }

    int * decToBinary(int n) {
        static int binaryNum[8];
        int i = 0;
        while (n > 0) {
            binaryNum[i] = n % 2;
            n = n / 2;
            i++;
        }

        return binaryNum;
    }

    void write(int value) {
        int *binArray = decToBinary(value);
        int i;

        for (i = 0; i < nrOfPins; ++i) {
            digitalWrite(pins[i], binArray[i]);
        }
    }

    void setup() {
        int i;
        for (i = 0; i < nrOfPins; ++i) {
            pinMode(pins[i], OUTPUT);
        }
    }

    void clear() {
        int i;
        for (i = 0; i < nrOfPins; ++i) {
            digitalWrite(pins[i], LOW);
        }
    }

    void healthCheck() {
        int i;
        int checkValues[8] = {1, 2, 4, 8, 16, 32, 64, 128};
        for (i = 0; i < nrOfPins; ++i) {
            write(checkValues[i]);
            delay(500);
        }
        clear();
    }
};

int temperatureLedPins[8] = {52, 50, 48, 46, 44, 42, 40, 38};
LedOctet temperatureLeds(temperatureLedPins);


void setup() {
    Serial.begin(9600);
    // Initialize device.
    dht.begin();
    Serial.println(F("DHTxx Unified Sensor Example"));
    // Print temperature sensor details.
    sensor_t sensor;
    dht.temperature().getSensor(&sensor);
    Serial.println(F("------------------------------------"));
    Serial.println(F("Temperature Sensor"));
    Serial.print(F("Sensor Type: "));
    Serial.println(sensor.name);
    Serial.print(F("Driver Ver:  "));
    Serial.println(sensor.version);
    Serial.print(F("Unique ID:   "));
    Serial.println(sensor.sensor_id);
    Serial.print(F("Max Value:   "));
    Serial.print(sensor.max_value);
    Serial.println(F("°C"));
    Serial.print(F("Min Value:   "));
    Serial.print(sensor.min_value);
    Serial.println(F("°C"));
    Serial.print(F("Resolution:  "));
    Serial.print(sensor.resolution);
    Serial.println(F("°C"));
    Serial.println(F("------------------------------------"));
    // Print humidity sensor details.
    dht.humidity().getSensor(&sensor);
    Serial.println(F("Humidity Sensor"));
    Serial.print(F("Sensor Type: "));
    Serial.println(sensor.name);
    Serial.print(F("Driver Ver:  "));
    Serial.println(sensor.version);
    Serial.print(F("Unique ID:   "));
    Serial.println(sensor.sensor_id);
    Serial.print(F("Max Value:   "));
    Serial.print(sensor.max_value);
    Serial.println(F("%"));
    Serial.print(F("Min Value:   "));
    Serial.print(sensor.min_value);
    Serial.println(F("%"));
    Serial.print(F("Resolution:  "));
    Serial.print(sensor.resolution);
    Serial.println(F("%"));
    Serial.println(F("------------------------------------"));
    // Set delay between sensor readings based on sensor details.
    delayMS = sensor.min_delay / 1000;

    Serial.println(F("Initializing LED pins"));
    temperatureLeds.setup();
    temperatureLeds.healthCheck();
}

void loop() {
    // Delay between measurements.
    delay(delayMS);
    // Get temperature event and print its value.
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
        Serial.println(F("Error reading temperature!"));
    } else {
        Serial.print(F("Temperature: "));
        Serial.print(event.temperature);
        Serial.println(F("°C"));
    }
    // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
        Serial.println(F("Error reading humidity!"));
    } else {
        Serial.print(F("Humidity: "));
        Serial.print(event.relative_humidity);
        Serial.println(F("%"));
    }
}

/*
#include <Wire.h>
#include <SPI.h>
#include <RTClib.h>

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


void setup() {
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
}

void loop() {
    DateTime now = rtc.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    delay(2000);
}
 */



/*
const int ON_TIME = 500;
const int OFF_TIME = 500;

// the setup function runs once when you press reset or power the board
void setup() {
    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(ON_TIME);                      // wait for a second
    Serial.println("ON_TIME");
    Serial.println(ON_TIME);
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(OFF_TIME);                       // wait for a second
    Serial.println("OFF_TIME");
    Serial.println(OFF_TIME);
}
 */

/*
int led = 13; // Built-in LED is connected to pin 13 int brightness = 0;
int fadeAmount = 5;
int brightness = 0;

void setup() {
    pinMode(led, OUTPUT);
}

void loop() {
    analogWrite(led, brightness);
    brightness = brightness + fadeAmount;
    if (brightness <= 0 || brightness >= 255) {
        fadeAmount = -fadeAmount;
    }
    delay(30);
}
 */