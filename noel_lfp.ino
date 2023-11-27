#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Wire.h>

#include <TimeLib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>;
#include <Fonts/FreeSerifBoldItalic9pt7b.h> 
     // Gras + Italique (en 9, 12, 18, et 24 pts)
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
#include <Fonts/FreeMonoBoldOblique18pt7b.h>
#include <Fonts/FreeMonoBoldOblique24pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>              // Gras (en 9, 12, 18, et 24 pts)
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <Fonts/FreeMono9pt7b.h>  
//-----------Setup Screen-------------------
#define OLED_RESET LED_BUILTIN
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define pinLED1 D3
#define pinLED2 D4



void setup() {

  display.setRotation(90);
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();

  pinMode(pinLED1, OUTPUT);
  pinMode(pinLED2, OUTPUT);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.display();
 
}

void loop() {

  analogWrite(pinLED1, random(120, 255));
  analogWrite(pinLED2, random(120, 255));

  delay(random(10, 100));
}
