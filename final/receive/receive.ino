#include <RH_ASK.h>
#include <SPI.h> // Required by RH_ASK
#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

#define I2C_ADDRESS 0x3C

SSD1306AsciiWire oled;
RH_ASK rf_driver;
int Speaker = 3;

void setup() {
  Serial.begin(9600);
  Wire.begin(); // Initialize I2C

  // Initialize OLED
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.setFont(Adafruit5x7);
  oled.clear();
  oled.setCursor(0, 0);
  oled.println("Receiver Ready! :D");

  rf_driver.init();
  pinMode(Speaker, OUTPUT);
}

void loop() {
  uint8_t buf[24];
  uint8_t buflen = sizeof(buf);

  if (rf_driver.recv(buf, &buflen)) {
    String msg = String((char*)buf);
    Serial.print("Message Received: ");
    Serial.println(msg);
    
    Bell();

    // Show message on OLED
    oled.clear();
    oled.setCursor(0, 0);
    oled.println("Message:");
    oled.println(msg);
    delay(5000); // Show message for 5 seconds
  }
}

void Bell() {
  analogWrite(Speaker, 80); delay(500); analogWrite(Speaker, 0); delay(50);
  analogWrite(Speaker, 64); delay(1000); analogWrite(Speaker, 0); delay(50);
  analogWrite(Speaker, 150); delay(300); analogWrite(Speaker, 0); delay(50);
  analogWrite(Speaker, 64); delay(2000); analogWrite(Speaker, 0);
}
