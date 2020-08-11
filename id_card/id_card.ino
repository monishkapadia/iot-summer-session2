#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include "images.h"

#define TFT_CS         5
#define TFT_RST        0
#define TFT_DC         4

// Instantiate an object of ST7735
// Making a copy of the Adafruit_ST7735 Blueprint and naming it tft
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void id_page_1() {
  tft.fillScreen(ST77XX_RED);
  tft.drawRGBBitmap(34, 10, user_small, USER_SMALL_WIDTH, USER_SMALL_HEIGHT);
  tft.setCursor(10, 80);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE, ST77XX_RED);
  tft.println("Name:Monish Kapadia");
  tft.setCursor(10, 95);
  tft.println("Age:24 years");
  tft.setCursor(10, 110);
  tft.println("School:NYU");
}

void id_page_2() {
  tft.fillScreen(ST77XX_BLACK);
  tft.drawRGBBitmap(0, 0, user_big, USER_BIG_WIDTH, USER_BIG_HEIGHT);
}


void setup() {
  // put your setup code here, to run once:
  tft.initR(INITR_144GREENTAB);
  tft.fillScreen(ST77XX_BLACK);
}

void loop() {
  // put your main code here, to run repeatedly:
  id_page_1();
  delay(20000);
  id_page_2();
  delay(20000);
}
