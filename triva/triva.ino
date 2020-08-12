#include <Adafruit_ST7735.h>
#include <Adafruit_GFX.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "JsonParser.h"


// WiFi credentials
#define WIFI_SSID "Summer-IOT"
#define WIFI_PASS "summeriot"

#define CUSTOM_PURPLE 0xD04C

// TFT screen pins
#define TFT_CS         16
#define TFT_RST        0
#define TFT_DC         4

// OpenTDB server
const char host[] = "opentdb.com";
const int httpsPort = 443;

String url = "/api.php?amount=1&type=multiple";

// Instantiate an object of ST7735
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

WiFiClientSecure client;

JsonParser j;

void drawProgress(int percentage) {
  if (percentage == 0) {
    tft.fillRect(6, 52, 116, 10, ST77XX_BLACK);
  }
  tft.drawRect(4, 50, 120, 14, ST77XX_WHITE);
  tft.fillRect(6, 52, 116*percentage/100, 10, ST77XX_WHITE);
}


void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  tft.setCursor(25, 30);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);
  tft.print("Welcome to IoT");

  tft.setCursor(25, 110);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);
  tft.print("Monish Kapadia");

  tft.setCursor(10, 80);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.print("Connecting WiFi");
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    drawProgress(i);
    i+=20;
    if(i > 90) {
      i = 0;
    }
    delay(500);
  }
  drawProgress(100);
  tft.setCursor(10, 80);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.print("Connected to WiFi");
  delay(500);
}

void get_trivia() {
  client.setInsecure(); // Do connections whatsoever don't try to authenticate
  if (!client.connect(host, httpsPort)) {
    Serial.println("Connection error");
    return;
  }
  client.println(String("GET ") + url + " HTTP/1.1");
  client.println("Host: " + String(host));
  client.println("User-Agent: ESP8266");
  client.println("Connection: close");
  client.println();

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("Question received");
      break;
    }

  }
  String line = client.readStringUntil('\n');
  line = client.readStringUntil('\n');
  j.convertArray(line);
  client.stop();
}

// TFT Print Trivia
void printTrivia() {
  // Clear any extra values
  while (Serial.available()) {
    Serial.read();
  }
  // Clear the screen
  tft.fillScreen(CUSTOM_PURPLE);
  tft.setTextSize(1);
  tft.setCursor(0, 0);

  tft.setTextColor(ST77XX_WHITE , CUSTOM_PURPLE);
  tft.println("Q: " + j.question);
  tft.println("1: " + j.answers[0]);
  tft.println("2: " + j.answers[1]);
  tft.println("3: " + j.answers[2]);
  tft.println("4: " + j.answers[3]);

  Serial.println("Q: " + j.question);
  Serial.println("1: " + j.answers[0]);
  Serial.println("2: " + j.answers[1]);
  Serial.println("3: " + j.answers[2]);
  Serial.println("4: " + j.answers[3]);

  // wait until a user enters any information on serial monitor
  while (!Serial.available());
  // < 1/2/3/4 > print invalid response
  char answerGiven = Serial.read();
  // ascii '1', '2', '3', '4'
  // '5' - '1' = 52 - 49 = 4
  if (answerGiven - '1' > 3 || answerGiven - '1' < 0) {
    // invalid response
    tft.println("Invalid response");
    Serial.println("Invalid response");
    printTrivia();                        //Recursion
  } else {
    // valid response and check if you have the right answer
    if (j.answers[answerGiven - '1'] == j.getCorrectAnswer()) {
      Serial.println("You are right");
      tft.setTextSize(2);
      tft.setTextColor(ST77XX_WHITE , ST77XX_BLUE);
      rightScreen();
      tft.setTextSize(1);

    } else {
      tft.setTextSize(2);
      tft.setTextColor(ST77XX_WHITE , ST77XX_BLUE);
      tft.println("Wrong");
      tft.setTextSize(1);
      tft.println("Right answer is: " + j.getCorrectAnswer());
      wrongScreen();
    }
  }
}

void welcomeScreen() {
  tft.fillScreen(ST77XX_RED);

  tft.setTextSize(2);
  tft.setCursor(25, 55);
  tft.setTextColor(ST77XX_WHITE);

  tft.print("TRIVIA!");
  
// Animation for Box
// Looping 0 -100 , loop counter decides the line length
  for (int i = 0; i <= 100; i++) {
//    Draw line on topleft to top right (14,14)
    tft.drawFastHLine(14, 14, i, ST77XX_WHITE);
//    Starting at the bottom right to bottom left (114,114)
    tft.drawFastHLine(114, 114, -i, ST77XX_WHITE);
//    Speed of animation
    delay(20);
  }
  for (int i = 0; i <= 100; i++) {
    tft.drawFastVLine(114, 14, i, ST77XX_WHITE);
    tft.drawFastVLine(14, 114, -i, ST77XX_WHITE);
    delay(20);
  }

}

void rightScreen() {
  tft.fillScreen(ST77XX_GREEN);

  tft.setTextSize(2);
  tft.setCursor(25, 55);
  tft.setTextColor(ST77XX_WHITE);

  tft.print("RIGHT!");
  
// Animation for Box
// Looping 0 -100 , loop counter decides the line length
  for (int i = 0; i <= 100; i++) {
//    Draw line on topleft to top right (14,14)
    tft.drawFastHLine(14, 14, i, ST77XX_WHITE);
//    Starting at the bottom right to bottom left (114,114)
    tft.drawFastHLine(114, 114, -i, ST77XX_WHITE);
//    Speed of animation
    delay(20);
  }
  for (int i = 0; i <= 100; i++) {
    tft.drawFastVLine(114, 14, i, ST77XX_WHITE);
    tft.drawFastVLine(14, 114, -i, ST77XX_WHITE);
    delay(20);
  }

}

void wrongScreen() {
  tft.fillScreen(ST77XX_RED);

  tft.setTextSize(2);
  tft.setCursor(25, 55);
  tft.setTextColor(ST77XX_WHITE);

  tft.print("WRONG!");
  
// Animation for Box
// Looping 0 -100 , loop counter decides the line length
  for (int i = 0; i <= 100; i++) {
//    Draw line on topleft to top right (14,14)
    tft.drawFastHLine(14, 14, i, ST77XX_WHITE);
//    Starting at the bottom right to bottom left (114,114)
    tft.drawFastHLine(114, 114, -i, ST77XX_WHITE);
//    Speed of animation
    delay(20);
  }
  for (int i = 0; i <= 100; i++) {
    tft.drawFastVLine(114, 14, i, ST77XX_WHITE);
    tft.drawFastVLine(14, 114, -i, ST77XX_WHITE);
    delay(20);
  }

}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  tft.initR(INITR_144GREENTAB);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextWrap(true);
  connectWiFi();
  welcomeScreen();
  delay(2000);
}

void loop() {
  get_trivia();
  printTrivia();
  delay(10);
}
