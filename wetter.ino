#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Wire.h>
#include <stdio.h>
#include <string>
#include "arduino_secrets.h"
#define DOMAIN "wetter"
#define REFRESH_INTERVAL 300000 // milliseconds



// url Tübungen:
// /apps/thinghttp/send_request?api_key=PF5VV92LT40YEPS6
// url Albstadt:
// /apps/thinghttp/send_request?api_key=V67Q2ZYSWAZ0X9GR
// weather source: https://www.wetteronline.de/wetter/tuebingen
#define URL "/apps/thinghttp/send_request?api_key=PF5VV92LT40YEPS6"



// LEDs PIN 0-7, LSB PIN 0
// LED PIN 8 is on for negative numbers




// Networking


const char* host = "api.thingspeak.com";

ESP8266WebServer server(80);



// Loop


int binary;
int startServer;
int temperature;
bool ledControl[9];  // global array is initialized with 0
long slave = 0;



// Helper Functions


void run_counter() {
  slave++;
  Serial.print("Durchlauf Nr. ");
  Serial.println(slave);
  Serial.println();
  Serial.println();
}



void led_check() {


  digitalWrite(D0, HIGH);
  delay(500);
  digitalWrite(D1, HIGH);
  delay(500);
  digitalWrite(D2, HIGH);
  delay(500);
  digitalWrite(D3, HIGH);
  delay(500);
  digitalWrite(D4, HIGH);
  delay(500);
  digitalWrite(D5, HIGH);
  delay(500);
  digitalWrite(D6, HIGH);
  delay(500);
  digitalWrite(D7, HIGH);
  delay(500);
  digitalWrite(D8, HIGH);
  delay(1000);


  digitalWrite(D0, LOW);
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW);
  digitalWrite(D5, LOW);
  digitalWrite(D6, LOW);
  digitalWrite(D7, LOW);
  digitalWrite(D8, LOW);
  delay(300);
  digitalWrite(D0, HIGH);
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH);
  digitalWrite(D5, HIGH);
  digitalWrite(D6, HIGH);
  digitalWrite(D7, HIGH);
  digitalWrite(D8, HIGH);
  delay(300);
  digitalWrite(D0, LOW);
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW);
  digitalWrite(D5, LOW);
  digitalWrite(D6, LOW);
  digitalWrite(D7, LOW);
  digitalWrite(D8, LOW);
  delay(300);
  digitalWrite(D0, HIGH);
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH);
  digitalWrite(D5, HIGH);
  digitalWrite(D6, HIGH);
  digitalWrite(D7, HIGH);
  digitalWrite(D8, HIGH);
  delay(300);
  digitalWrite(D0, LOW);
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW);
  digitalWrite(D5, LOW);
  digitalWrite(D6, LOW);
  digitalWrite(D7, LOW);
  digitalWrite(D8, LOW);
  delay(300);
  digitalWrite(D0, HIGH);
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH);
  digitalWrite(D5, HIGH);
  digitalWrite(D6, HIGH);
  digitalWrite(D7, HIGH);
  digitalWrite(D8, HIGH);
  delay(300);
  digitalWrite(D0, LOW);
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW);
  digitalWrite(D5, LOW);
  digitalWrite(D6, LOW);
  digitalWrite(D7, LOW);
  digitalWrite(D8, LOW);
}



// returns the searched temp

int getDigit(String server_answer) {
  // helpfull links:
  // https://de.wikibooks.org/wiki/C-Programmierung:_Zeichenkettenfunktionen
  // https://www.geeksforgeeks.org/convert-string-char-array-cpp/


  int n = server_answer.length();
  char char_array[n + 1];
  strcpy(char_array, server_answer.c_str());        // cast string to char
  char trennzeichen[] = "\n";
  const char *zahl;
  zahl = strtok(char_array, trennzeichen);


  int i = 1;
  int suche;
  while (zahl != NULL) {
    printf("Token %d: %s\n", i++, zahl);
    zahl = strtok(NULL, trennzeichen);
    if (i == 20) {                      // temp stands at line 20; IMPORTANT: this line will variate on the url you use. All lines will be printed and you can
      // find the temperature there
      suche = atoi(zahl);
      Serial.print("###### Achtung die Zahl ist: ");
      Serial.print(suche);
      Serial.println(" ######");
    }
  }
  return suche;
}



// Set LEDs
// values are read from ledControl array

void led_control(int temperature) {

  if (temperature < 1) {       // wenn temp < 1° hat dann aktiviere die minus led
    digitalWrite(D8, HIGH);
  } else {
    digitalWrite(D8, LOW);
  }

  digitalWrite(D0, ledControl[0]);
  digitalWrite(D1, ledControl[1]);
  digitalWrite(D2, ledControl[2]);
  digitalWrite(D3, ledControl[3]);
  digitalWrite(D4, ledControl[4]);
  digitalWrite(D5, ledControl[5]);
  digitalWrite(D6, ledControl[6]);
  digitalWrite(D7, ledControl[7]);
}



// Convert a decimal to a binary number
// Put the result in the ledControl array

int make_bin(int dezi) {
  int zahl = dezi;
  int rest, ergebnis = 0;
  int faktor = 1;
  int i = 0;

  while (dezi) {

    rest = dezi % 2;
    dezi /= 2;

    if (i < 9) {                // we dont want a array out of index when a to big number (from failed connection) arrives
      ledControl[i] = rest;
    }
    i++;
    Serial.print("rest: ");
    Serial.println(rest);
    faktor *= 10;
    ergebnis += rest * faktor;
  }

  ergebnis *= 0.1;

  return ergebnis;
}



// Handle incoming requests

void handleRoot() {

  String response = String(temperature);

  Serial.println("Received request, sending: " + response);

  server.send(200, "text/plain", response);
}



// Make an AP
// Host a server
// Set up mDNS

void apTemp(String ssid, String password) {


  // Access Point

  WiFi.mode(WIFI_AP);

  Serial.print("Setting soft-AP ... ");

  Serial.println(WiFi.softAP(ssid, password) ? "Ready" : "Failed!");


  // Webserver

  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");


  // mDNS

  // Note: mDNS doesn't work with smartphones

  if (MDNS.begin(DOMAIN)) {  // address: DOMAIN.local
    Serial.println("MDNS started");
  }
}


// Get the latest temperature and update the LEDs
// Connect to a WiFi network
// Fetch the latest temperature from the API server
// Parse the value and convert it to binary, update LEDs
// Run this function every n minutes

int tempRefresh() {
  String response;
  int temp;


  // Establish WiFi connection

  WiFi.mode(WIFI_STA);
  WiFi.begin(SECRET_SSID, SECRET_PASS);

  Serial.print("\n\nConnecting to ");
  Serial.println(SECRET_SSID);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


  // Connect to the website

  Serial.print("Connecting to ");
  Serial.println(host);

  WiFiClient client;
  if (!client.connect(host, 80)) {
    Serial.println("connection failed");
  }


  // Send request

  client.print(String("GET ") + URL +
               " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  delay(500);


  // Parse response

  response = client.readString();

  Serial.print("response: ");
  Serial.println(response);

  temp = getDigit(response);

  Serial.print("temperature: ");
  Serial.println(String(temp));

  return temp;
}



void setup() {


  Serial.begin(115200);
  delay(100);


  pinMode(D0 , OUTPUT);
  pinMode(D1 , OUTPUT);
  pinMode(D2 , OUTPUT);
  pinMode(D3 , OUTPUT);
  pinMode(D4 , OUTPUT);
  pinMode(D5 , OUTPUT);
  pinMode(D6 , OUTPUT);
  pinMode(D7 , OUTPUT);
  pinMode(D8 , OUTPUT);


  led_check();
}



void loop() {
  // reruns every REFRESH_INTERVAL milliseconds



  /* Fetch latest temperature from API server */

  temperature = tempRefresh();



  /* Update LEDs */

  if (temperature > 100) {
    Serial.print("Bad value, not updating LEDs");
  }


  else {

    // Convert to binary

    binary = make_bin(temperature);
    Serial.println("Binary: " + String(binary));

    // Make binary array

    for (int i = 0; i < 8; i++) {
      Serial.print("LED Control " + String(i) + ": ");
      Serial.println(ledControl[i]);
    }

    // Update LEDs

    Serial.println("Updating LEDs");
    led_control(temperature); // reads values from ledControl array
  }



  /* Counter */

  run_counter();



  /* Make an AP with temperature as ssid and host a webserver */

  apTemp("Temperatur: " + String(temperature) + "°C", "");
  startServer = millis();
  
  for (;;) {
    if (millis() - startServer > REFRESH_INTERVAL) {
      break;
    }
    
    server.handleClient();
    MDNS.update();
    delay(50);
  }

  

  /* remove?

    // Reset the LEDs
    for (int i = 0; i < 9; i++) {
    ledControl[i] = false;
    }

  */
}


/* todo

    make something with button

    change city by sending a request with parameter city=Berlin for ex.

    better API / weather service (wttr.in)

*/
