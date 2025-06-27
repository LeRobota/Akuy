#include "infos.h" 
#include <ESP8266WiFi.h>
#include <FastLED.h>

#define DATA_PIN D4
#define NUM_LEDS 1

CRGB leds[NUM_LEDS];

String keyword = "\"aqi\":";
float score_aqi;

unsigned long previousRequestTime = 0;
const unsigned long interval = 1800000;

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(lum);

  Serial.print("Connexion WiFi");
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    leds[0] = CRGB::Blue;
    FastLED.show();
    Serial.print(".");
  }

  Serial.println("\nWiFi connecté");
  Serial.println(WiFi.localIP());


  AQIrequest();
  previousRequestTime = millis();
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - previousRequestTime >= interval) {
    AQIrequest();
    previousRequestTime = currentTime;
  }

}

void AQIrequest() {
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("Connexion échouée");
    return;
  }

  String url = "/feed/geo:" + String(geoloc) + "/?token=" + token;
  client.print(String("GET ") + url + " HTTP/1.1\r\nHost: " + host + "\r\nConnection: close\r\n\r\n");

  delay(1000); 
  bool inBody = false;

  while (client.available()) {
    String line = client.readStringUntil('\r');

    if (line.length() == 1) inBody = true;
    if (inBody) {
      int pos = line.indexOf(keyword);
      if (pos > 0) {
        score_aqi = atof(&line[pos + keyword.length()]);
        break;
      }
    }
  }

  Serial.print("Score AQI : ");
  Serial.println(score_aqi);

  setColorBasedOnAQI(score_aqi);
}

void setColorBasedOnAQI(float aqi) {
  if (aqi < 50) leds[0] = CRGB::Green;          
  else if (aqi < 100) leds[0] = CRGB::Yellow;
  else if (aqi < 150) leds[0] = CRGB::Orange;    
  else if (aqi < 200) leds[0] = CRGB::Red;    
  else if (aqi < 300) leds[0] = CRGB::Violet; 
  else leds[0] = CRGB::Brown;

  FastLED.show();
}
