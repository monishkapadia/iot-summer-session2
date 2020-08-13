#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include "NTPClientUpdated.h"
//weather libraries
#include <OpenWeatherMapCurrent.h>
#include <OpenWeatherMapForecast.h>

//5day/3hr forecast
// 3hr - 6hr - 9hr
#define MAX_FORECASTS 3

// WiFi credentials
#define WIFI_SSID "Summer-IOT"
#define WIFI_PASS "summeriot"

// Date and time initializations
WiFiUDP ntpUDP;
const long utcOffsetInSeconds = -4 * 3600; // (3600*4) for daylight savings, -18000 for UTC-5
NTPClientUpdated timeClient(ntpUDP, utcOffsetInSeconds);
String dateStamp, timeStamp;


// Weather Settings
String OPEN_WEATHER_MAP_LANGUAGE = "en";
String OPEN_WEATHER_MAP_APP_ID = "d4b26391d35fe98cace3b9f4e49c6ff9";
String OPEN_WEATHER_MAP_LOCATION = "New York,NY,US";
const boolean IS_METRIC = true; // Celsius data, make it false for farenheit


//weather objects
OpenWeatherMapCurrentData currentWeather;
OpenWeatherMapForecastData forecasts[MAX_FORECASTS]; // 3hr [0] - 6 hr [1] - 9 hr [2]


void connectWiFi() {
  Serial.print("Connecting to " + String(WIFI_SSID));
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to " + String(WIFI_SSID));
}

void updateWeather() {
  OpenWeatherMapCurrent currentWeatherClient;
  currentWeatherClient.setMetric(IS_METRIC);
  currentWeatherClient.setLanguage(OPEN_WEATHER_MAP_LANGUAGE);
  currentWeatherClient.updateCurrent(&currentWeather, OPEN_WEATHER_MAP_APP_ID, OPEN_WEATHER_MAP_LOCATION);

  OpenWeatherMapForecast forecastWeatherClient;
  forecastWeatherClient.setMetric(IS_METRIC);
  forecastWeatherClient.setLanguage(OPEN_WEATHER_MAP_LANGUAGE);
  forecastWeatherClient.updateForecasts(forecasts, OPEN_WEATHER_MAP_APP_ID, OPEN_WEATHER_MAP_LOCATION, MAX_FORECASTS);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(2000000);
  connectWiFi();
  timeClient.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  timeClient.update();
  updateWeather();
  dateStamp = timeClient.getFormattedDate();
  timeStamp = timeClient.getFormattedTime();
  Serial.println("Time: " + timeStamp);
  Serial.println("Date: " + dateStamp);
  Serial.println("Temp: " + String(int(round(currentWeather.temp))) + "°C");
  Serial.println("Description: " + currentWeather.description);
  Serial.println("Sunrise: " + timeClient.getFormattedTime(true, currentWeather.sunrise + utcOffsetInSeconds));
  Serial.println("Sunset: " + timeClient.getFormattedTime(true, currentWeather.sunset + utcOffsetInSeconds));
  Serial.println("-----------------------------------------");
  Serial.println("In 3hrs");
  Serial.println("Temp: " + String(int(round(forecasts[0].temp))) + "°C");
  Serial.println("Description: " + forecasts[0].description);
  Serial.println("-----------------------------------------");
  Serial.println("In 6hrs");
  Serial.println("Temp: " + String(int(round(forecasts[1].temp))) + "°C");
  Serial.println("Description: " + forecasts[1].description);
  Serial.println("-----------------------------------------");
  Serial.println("In 9hrs");
  Serial.println("Temp: " + String(int(round(forecasts[2].temp))) + "°C");
  Serial.println("Description: " + forecasts[2].description);
  delay(10000);
}
