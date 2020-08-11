#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>


#define WIFI_SSID "Summer-IOT"
#define WIFI_PASS "summeriot"

const char host[] = "opentdb.com";
const int httpsPort = 443;

WiFiClientSecure client;


String url = "/api.php?amount=1&category=22&difficulty=easy&type=multiple";


void get_trivia() {
  client.setInsecure(); // Do connections whatsoever don't try to authenticate
  if(!client.connect(host, httpsPort)) {
    Serial.println("Connection error");
    return;
  }


  client.println(String("GET ") + url + " HTTP/1.1");
  client.println("Host: " + String(host));
  client.println("User-Agent: ESP8266");
  client.println("Connection: close");
  client.println();

  while(client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("Header received");
      break;
    }
    
  }
  String line = client.readStringUntil('\n');
  line = client.readStringUntil('\n');
  Serial.println(line);
  
  client.stop();
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to " + String(WIFI_SSID));
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected to " + String(WIFI_SSID));

  get_trivia();
  
}

void loop() {
  
}
