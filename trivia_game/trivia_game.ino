#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "json_parser.h"

#define WIFI_SSID "Summer-IOT"
#define WIFI_PASS "summeriot"

#define TFT_CS         5
#define TFT_RST        0
#define TFT_DC         4

// Instantiate an object of ST7735
// Making a copy of the Adafruit_ST7735 Blueprint and naming it tft
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

WiFiClientSecure client;

JsonParser j;

int i = 0, score = 0;
const char host[] = "opentdb.com";
const int httpsPort = 443;

String line = "";

String url = "/api.php?amount=1&category=22&difficulty=easy&type=multiple";

char answerOption = 0;


void connectWiFiUI() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.setCursor(30, 40);
  tft.println("Welcome to");
  tft.setCursor(30, 50);
  tft.println("Summer IoT");
  tft.setCursor(20, 80);
  tft.print("Connecting...");
  tft.setCursor(20, 120);
  tft.print("Monish Kapadia");
}

void drawProgress(int percentage) {
  if (percentage == 100) {
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setCursor(20, 80);
    tft.print("Connected...");
  }
  if (percentage == 0) {
    tft.fillRect(7, 62, 100, 10, ST77XX_BLACK);
  }
  tft.drawRect(5, 60, 118, 14, ST77XX_WHITE);
  tft.fillRect(7, 62, percentage * 114 / 100, 10, ST77XX_BLUE);
}


void connectWiFi()
{
  if (WiFi.status () == WL_CONNECTED) return;
  //connecting WiFi
  Serial.println ("connecting to WiFi");
  Serial.print(WIFI_SSID);
  Serial.print("/");
  Serial.print(WIFI_PASS);
  connectWiFiUI();
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  i = 0;
  while (WiFi.status () != WL_CONNECTED)
  {
    delay (500);
    Serial.print (".");
    i += 20;
    if (i > 90) {
      i = 0;
    }
    drawProgress(i);
  }
  drawProgress(100);
}

void welcomeScreen() {
  tft.fillScreen(ST77XX_RED);
  tft.setCursor(30, 55);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE, ST77XX_RED);
  tft.print("Trivia");

  for(int i=0; i <= 100; i++){
    tft.drawFastHLine(14, 14, i, ST77XX_WHITE);
    tft.drawFastHLine(114, 114, -i, ST77XX_WHITE);
    delay(20);
  }

  for(int i=0; i <= 100; i++){
    tft.drawFastVLine(114, 14, i, ST77XX_WHITE);
    tft.drawFastVLine(14, 114, -i, ST77XX_WHITE);
    delay(20);
  }
}

void get_trivia() {
  client.setInsecure();
  if (!client.connect(host, httpsPort)) {
    Serial.println("Connection failed");
  }

  Serial.println("Connection to " + String(host) + " successful");

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + String(host) + "\r\n" +
               "User-Agent: ESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
    line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  line = client.readStringUntil('\n');
  line = client.readStringUntil('\n');
  j.convertArray(line);
  client.stop();

}
void displayTrivia() {

  // Clear any extra values
  while (Serial.available()) {
    Serial.read();
  }
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 10);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.println("Q: " + j.question);
  tft.println("");
  tft.println("1: " + j.answers[0]);
  tft.println("2: " + j.answers[1]);
  tft.println("3: " + j.answers[2]);
  tft.println("4: " + j.answers[3]);

  
  tft.setCursor(30, 110);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.println("Score: " + String(score));
  
  // Wait for the actual value
  while (!Serial.available());
  answerOption = Serial.read();
  if (answerOption > '0' && answerOption <= '4') {
    if (j.answers[answerOption - '1'] == j.getCorrectAnswer()) {
      score += 4;
      tft.fillScreen(ST77XX_BLACK);
      tft.setCursor(20, 50);
      tft.setTextSize(2);
      tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
      tft.println("Correct!");
    } else {
      score -= 1;
      if(score < 0)
        score = 0;
      tft.fillScreen(ST77XX_BLACK);
      tft.setCursor(20, 50);
      tft.setTextSize(2);
      tft.setTextColor(ST77XX_RED, ST77XX_BLACK);
      tft.println("Wrong!");
    }
  } else {
    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(20, 50);
    tft.setTextSize(2);
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.println("Invalid!");
    delay(1000);
    displayTrivia();
  }
}

void setup() {
  Serial.begin(115200);
  tft.initR(INITR_144GREENTAB);
  tft.fillScreen(ST77XX_BLACK);
  Serial.println();
  connectWiFi();
  welcomeScreen();
  delay(2000);
}

void loop() {
  get_trivia();
  displayTrivia();
  delay(10);
}
