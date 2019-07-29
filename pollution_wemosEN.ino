#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>

// pin for RGB LED
#define PIN D4

// How many leds ?
#define NUMLED 1

Adafruit_NeoPixel strip(NUMLED, PIN, NEO_GRB + NEO_KHZ800);


// Here your personnal Wi-fi and password
const char* ssid     = "********";
const char* password = "**********";

const char* host     = "api.waqi.info";                           // Host
const char* token   = "**************"; // obtain your access token here : https://aqicn.org/data-platform/token/#/
const char* geoloc     = "******;******"; // obtain your coordinates here : https://www.coordonnees-gps.fr/

// Keywords we'll need : air pollution index and dominent polluant
String keyword = String("\"aqi\":"); 
String keyword2 = String("\"dominentpol\":");

float score_aqi; 
String poldom;

void setup() {
 
  Serial.begin(115200);
  delay(10);
  
  strip.begin(); // initializing ledstrip
  strip.setBrightness(255);

 // testing wi-fi network
  Serial.print("Connexion au WiFi ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);   
  
  while (WiFi.status() != WL_CONNECTED) {
     strip.clear(); 
     strip.setPixelColor(0, 0, 0, 0);
     strip.show(); 
    delay(500);
    Serial.print(".");
    strip.clear(); 
     strip.setPixelColor(0, 0, 0, 255);
     strip.show();  
     delay(500);
  }

  Serial.println(""); 
  Serial.println("WiFi connectÃ©");  
  Serial.print("Adresse IP du module EPC: ");  
  Serial.println(WiFi.localIP());
  Serial.print("Adresse IP de la box : ");
  Serial.println(WiFi.gatewayIP());
}

bool inBody = false;

void loop() {

   Serial.print("Connecting to the server : ");
  Serial.println(host);
  

  WiFiClient client;
  const int httpPort = 80;

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  
  // complete URL
  String url = String("/feed/") +"geo:"+ geoloc + "/?token=" + token;
  
  Serial.print("asking for URL: ");
  Serial.println(url);
  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

    delay(1000);
  
  inBody = false;
  
   while(client.available()){
    String line = client.readStringUntil('\r');
        
    if (line.length() == 1) inBody = true; 
    if (inBody) {  
       int pos = line.indexOf(keyword);
       int pos2 = line.indexOf (keyword2);

      if (pos > 0) { /* mot clé trouvé */
      
       pos += keyword.length();       
       score_aqi = atof(&line[pos]); 

     }
   
       if (pos2 > 0) {
       pos2 += keyword2.length(); 
       line[pos2+5] = '\0';
}   

       poldom = &line[pos2+1]; 
       
     }
    } 

    Serial.println();Serial.print ("score aqi = "); Serial.println(score_aqi); 
    Serial.println();Serial.print ("polluant dominant = "); Serial.println(poldom);

  Serial.println("connexion fermee");

// LED Actions

  if ( score_aqi < 50.00) {
 
 strip.clear(); // clearing led
 strip.setPixelColor(0, 255, 0, 0);
 strip.show();   updating led
 Serial.println ("green, enjoy fresh air");
   }


 else if ( score_aqi > 50.00 && score_aqi < 100.00) {
 
 strip.clear();
 strip.setPixelColor(0, 255, 255, 0);
 strip.show();  
 Serial.println ("yellow, not so good");

   }

 else if ( score_aqi > 100.00 && score_aqi < 150.00) {
 
 strip.clear();
 strip.setPixelColor(0, 100, 255, 0);
 strip.show();  
 Serial.println ("orange, smells like pollution");

   }
   

 else if ( score_aqi > 150.00 && score_aqi < 200.00) {
 
 strip.clear();
 strip.setPixelColor(0, 0, 255, 0);
 strip.show();   
 Serial.println ("red, looks bad for grandpa");

   }
   

 else if ( score_aqi > 200.00 && score_aqi < 300.00) {
 
 strip.clear(); 
 strip.setPixelColor(0, 0, 255, 255);
 strip.show();  
 Serial.println ("violet, very HIGH pollution stay and home and enjoy your last moments");

   }  


 else if ( score_aqi > 300) {
 
 strip.clear(); 
 strip.setPixelColor(0, 0, 255, 255);
 strip.show();   
 Serial.println ("brown, RUN AWAY");

   }   
 
  delay(300000); 
}
