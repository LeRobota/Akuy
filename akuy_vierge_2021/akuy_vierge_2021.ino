#include "infos.h"

// on inclu les librairies avec lesquelles on va travailler
#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>

// LEDS 
// on définie la pin sur laquelle la led est branchée
#define PIN D4

// on définie le nombre de led (utile dans le cas d'un strip)
#define NUMLED 1

// ici le type de ledstrip utilisé
Adafruit_NeoPixel strip(NUMLED, PIN, NEO_GRB + NEO_KHZ800);

// Si connexion non fonctionnelle 
/*
const IPAddress ip(192, 168, 1, 53);
const IPAddress gateway(192, 168, 1, 254);
const IPAddress dns (192,168,1,254);
const IPAddress subnet(255, 255, 255, 0);
*/
// mot(s) clé à chercher dans notre JSON
String keyword = String("\"aqi\":"); //on cherche le score AQI
String keyword2 = String("\"dominentpol\":"); //on cherche le polluant dominant

// on déclare deux nouvelles variables pour le score aqi et polluant dominant
float score_aqi; 
String poldom;

void setup() {
  // SETUP, fonction qui ne se lance qu'une fois au démarrage de l'Arduino
 
  // on démarre la communication série entre arduino et ordinateur
  Serial.begin(115200);
  delay(10);


  strip.begin(); // on initialise le ledstrip
  strip.setBrightness(lum); // on règle la luminosité

 // TEST DU WIFI (on voit si ça marche ou pas)
  Serial.print("Connexion au WiFi ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);    // On se connecte
  // WiFi.config(ip, gateway, subnet, dns); // si connexion impossible
//  WiFi.config(ip, dns, gateway); // si connexion impossible

  while (WiFi.status() != WL_CONNECTED) { // On attend
     strip.clear(); // on efface la couleur précédente
     strip.setPixelColor(0, 0, 0, 0);
     strip.show();   // on "update" la couleur
    delay(500);
    Serial.print(".");
    strip.clear(); // on efface la couleur précédente
     strip.setPixelColor(0, 0, 0, 255);
     strip.show();   // on "update" la couleur
     delay(500);
  }

  Serial.println("");  // on affiche les parametres 
  Serial.println("WiFi connectÃ©");  
  Serial.print("Adresse IP du module EPC: ");  
  Serial.println(WiFi.localIP());
  Serial.print("Adresse IP de la box : ");
  Serial.println(WiFi.gatewayIP());
}


// drapeau indiquant pendant l'analyse de la reponse du serveur
// si on est dans l'en tete HTTP (false) ou dans le contenu de 
// la ressource.
bool inBody = false;


void loop() {

  // PROGRAMME PRINCIPAL QUI VA TOURNER EN BOUCLE

// CONNEXION AU RESEAU
// on se connecte a l'adresse demandé en début de page
  Serial.print("Connexion au serveur : ");
  Serial.println(host);
  
  // On se place dans le role du  client en utilisant WifiClient
  WiFiClient client;

  // le serveur Web attend tradionnellement sur le port 80
  const int httpPort = 80;

  // Si la connexion echoue ca sera pour la prochaine fois
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    delay(10000);
    return;
    
  }

  
  // La connexion a reussie on forme le chemin 
  // URL  complexe composee du chemin et de deux 
  // questions contenant le score AQI et l'API key
  
  String url = String("/feed/") +"geo:"+ geoloc + "/?token=" + token;
  
  Serial.print("demande URL: ");
  Serial.println(url);
  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

  // On attend 1 seconde
  delay(1000);
  
  inBody = false; // on est dans l'en-tete
  
  // On lit les donnees recues, s'il y en a
  while(client.available()){
    String line = client.readStringUntil('\r');
        
    if (line.length() == 1) inBody = true; /* passer l'en-tete jusqu'à  une ligne vide */
    if (inBody) {  // ligne du corps du message, on cherche les mots clé
       int pos = line.indexOf(keyword);
       int pos2 = line.indexOf (keyword2);

      if (pos > 0) { /* mot clé trouvé */
       // indexOf donne la position du début du mot clé, en ajoutant sa longueur
       // on se place à la fin.
       pos += keyword.length();       

       score_aqi = atof(&line[pos]); 

     }
     //idem pour le mot clé 2
       if (pos2 > 0) { /* mot clÃƒÂ© trouvÃƒÂ© */
       // indexOf donne la position du dÃ©but du mot clÃ©, en ajoutant sa longueur
       // on se place Ã  la fin.
       pos2 += keyword2.length(); 
       line[pos2+5] = '\0';
}   

       poldom = &line[pos2+1]; 
       

     }/* fin récuperation du flotant */
    } /* fin de la recherche du mot clé */
   /* fin data avalaible */

// on écrit les données recueillies dans le moniteur série
    Serial.println();Serial.print ("score aqi = "); Serial.println(score_aqi); // temp en Kelvin
    Serial.println();Serial.print ("polluant dominant = "); Serial.println(poldom); // temp en Kelvin

  Serial.println("connexion fermee");

// ACTION SUR LA LED 

// si le score est inférieur à 50, la led s'éclaire en vert (bonne nouvelle)
  if ( score_aqi < 50.00) {
 
 strip.clear(); // on efface la couleur précédente
 strip.setPixelColor(0, 255, 0, 0);
 strip.show();   // on "update" la couleur
 Serial.println ("vert, tout baigne");
 
  }
// Mais si le score est supérieur à 50 MAIS inférieur à 100, la led s'éclaire en jaune (qualité passable)  
 else if ( score_aqi > 50.00 && score_aqi < 100.00) {
 
 strip.clear(); // on efface la couleur précédente
 strip.setPixelColor(0, 255, 255, 0);
 strip.show();   // on "update" la couleur
 Serial.println ("jaune, bof bof aujourd'hui");

   }

   // Mais si le score est supérieur à 100 MAIS inférieur à 150, la led s'éclaire en orange (qualité médiocre)  
 else if ( score_aqi > 100.00 && score_aqi < 150.00) {
 
 strip.clear(); // on efface la couleur précédente
 strip.setPixelColor(0, 100, 255, 0);
 strip.show();   // on "update" la couleur
 Serial.println ("orange, le running ça sera pour demain!");

   }
   
 // Mais si le score est supérieur à 150 MAIS inférieur à 200, la led s'éclaire en rouge (pic de pollution)  
 else if ( score_aqi > 150.00 && score_aqi < 200.00) {
 
 strip.clear(); // on efface la couleur précédente
 strip.setPixelColor(0, 0, 255, 0);
 strip.show();   // on "update" la couleur
 Serial.println ("rouge, ça craint ici. Rentre papy dans sa chambre et oublie la sortie en vélo");

   }
   
// Mais si le score est supérieur à 200 MAIS inférieur à 200, la led s'éclaire en violet (pic de pollution violent)
 else if ( score_aqi > 200.00 && score_aqi < 300.00) {
 
 strip.clear(); // on efface la couleur précédente
 strip.setPixelColor(0, 0, 255, 255);
 strip.show();   // on "update" la couleur
 Serial.println ("violet, ça craint vraiment ici... on sort pas de la maison, ambiance pot d'échappement");

   }  

// enfin, si le score est supérieur à 300, la led s'éclaire en marron (air extrèmmement pollué, danger ++++)
 else if ( score_aqi > 300) {
 
 strip.clear(); // on efface la couleur précédente
 strip.setPixelColor(0, 0, 255, 255);
 strip.show();   // on "update" la couleur
 Serial.println ("violet, ça craint vraiment ici... on sort pas de la maison, ambiance pot d'échappement");

   }   
  
 
// on finit enfin par un long délais de 5 minutes
  delay(300000); 
}
