// Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>
// Include dependent SPI Library
#include <SPI.h>
#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

// OLED I2C address
#define I2C_ADDRESS 0x3C

// OLED setup using SSD1306Ascii
SSD1306AsciiWire oled;

RH_ASK rf_driver;

// Define ultrasonic sensor pins
const int trigPin = 9;
const int echoPin = 10;

void setup() {
    // Initialize serial communication and RF driver
    Serial.begin(9600);
    rf_driver.init();

    // Initialize I2C and OLED
    Wire.begin();
    oled.begin(&Adafruit128x64, I2C_ADDRESS);
    oled.setFont(Adafruit5x7);
    oled.clear();
    oled.setCursor(0, 0);
    oled.println("Sensor Ready :]");

    // Initialize ultrasonic sensor pins
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

void loop() {
    // Measure distance using ultrasonic sensor
    long duration;
    float distance;

    // Send a 10-microsecond pulse to trigger pin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Read the echo pin duration
    duration = pulseIn(echoPin, HIGH);

    // Calculate distance in cm
    distance = duration * 0.034 / 2;

    Serial.print("Distance: ");
    Serial.println(distance);

    oled.clear();
    oled.setCursor(0, 2);
    oled.set2X();
    oled.println("Please hover over the sensor to ring the bell!");

    // Check if the distance is less than 7 cm
    if (distance < 7) {
        const char *msg = "Object detected!";
        rf_driver.send((uint8_t *)msg, strlen(msg));
        rf_driver.waitPacketSent();
        Serial.println("Msg Sent: Object detected!");

        oled.clear();
        oled.setCursor(0, 2);  // Move to line 3
        oled.set2X();
        oled.println("Bell Ringing...");
        delay(4000);
    } else {
        Serial.println("No object detected.");
    }

    // Delay for stability
    delay(500);
}
