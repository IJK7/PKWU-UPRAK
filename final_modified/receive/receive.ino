#include <RH_ASK.h>
#include <SPI.h> // Required by RH_ASK
#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

#define I2C_ADDRESS 0x3C

SSD1306AsciiWire oled;
RH_ASK rf_driver;
int Speaker = 3;

//Receiver on pin 11
//Transmitter on pin 12

void setup() {
  Serial.begin(9600);
  Wire.begin(); // Initialize I2C

  // Initialize OLED
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.setFont(Adafruit5x7);
  oled.clear();
  oled.setCursor(0, 0);
  oled.println("Bell Ready! :D");

  rf_driver.init();
  pinMode(Speaker, OUTPUT);
}

void loop() {
  uint8_t buf[24];
  uint8_t buflen = sizeof(buf);

  if (rf_driver.recv(buf, &buflen)) {
    String msg = String((char*)buf);
    unsigned long ms = millis();

    // Convert to hours, minutes, seconds
    unsigned long seconds = ms / 1000;
    unsigned int hours = seconds / 3600;
    unsigned int minutes = (seconds % 3600) / 60;
    unsigned int secs = seconds % 60;

    // Format time as hh:mm:ss
    char timeStr[10];
    sprintf(timeStr, "%02u:%02u:%02u", hours, minutes, secs);

    Serial.print("Message Received: ");
    Serial.println(msg);
    Serial.print("Time: ");
    Serial.println(timeStr);

    Bell();

    // Show message and timestamp on OLED
    oled.clear();
    oled.setCursor(0, 0);
    oled.println(msg);
    oled.setCursor(0, 40);
    oled.print("Time: ");
    oled.println(timeStr);

    delay(2000); // Show message for 2 seconds
  }
}

void Bell() {
  analogWrite(Speaker, 80); delay(500); analogWrite(Speaker, 0); delay(50);
  analogWrite(Speaker, 64); delay(1000); analogWrite(Speaker, 0); delay(50);
  analogWrite(Speaker, 150); delay(300); analogWrite(Speaker, 0); delay(50);
  analogWrite(Speaker, 64); delay(2000); analogWrite(Speaker, 0);
}
