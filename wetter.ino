#include <ESP8266WiFi.h>
#include <Wire.h>  // This library is already built in to the Arduino IDE
#include <stdio.h>
#include <string>

 

// LEDs PIN 0-7, LSB PIN 0
// LED PIN 8 is on for negative numbers

 

// url Tübungen:
// /apps/thinghttp/send_request?api_key=PF5VV92LT40YEPS6
// url Albstadt:
// /apps/thinghttp/send_request?api_key=V67Q2ZYSWAZ0X9GR
// weather source: https://www.wetteronline.de/wetter/tuebingen

 

const char* ssid = "+++"; // put your router name
const char* password = "+++";// put your password
const char* host = "api.thingspeak.com";
bool done;
int output;
bool ledControll[9];  // global array is initialized with 0
long slave = 0;

 

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

 

int make_bin(int dezi) {
  int zahl, rest, ergebnis = 0, faktor = 1, i = 0;
  zahl = dezi;
  while (dezi) {
    rest = dezi % 2;
    dezi = dezi / 2;

 

    if(i < 9){                  // we dont want a array out of index when a to big number (from failed connection) arrives
      ledControll[i] = rest;
    }
    i++;
    Serial.print("rest: ");
    Serial.println(rest);
    faktor *= 10;
    ergebnis = ergebnis + rest * faktor;
  }
  ergebnis *= 0.1;
  return ergebnis;
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

 

void led_controll(){

 

  if (output < 1) {       // wenn temp < 1° hat dann aktiviere die minus led
    digitalWrite(D8, HIGH);
  } else {
    digitalWrite(D8, LOW);
  }
  digitalWrite(D0, ledControll[0]);
  digitalWrite(D1, ledControll[1]);
  digitalWrite(D2, ledControll[2]);
  digitalWrite(D3, ledControll[3]);
  digitalWrite(D4, ledControll[4]);
  digitalWrite(D5, ledControll[5]);
  digitalWrite(D6, ledControll[6]);
  digitalWrite(D7, ledControll[7]);
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

 

  // We start by connecting to a WiFi network

 

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

 

  WiFi.begin(ssid, password);//, password

 

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  //ledCheck();
}

 

void loop() {
  Serial.print("connecting to ");
  Serial.println(host);

 

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
  }

 

  // We now create a URI for the request
  String url = "/apps/thinghttp/send_request?api_key=PF5VV92LT40YEPS6";
  Serial.print("Requesting URL: ");
  Serial.println(url);
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  delay(500);

 

  // Read all the lines of the reply from server and print them to Serial
  done = false;
  while (done == false) {
    String server_answer = client.readString(); //String server_answer = client.readStringUntil('\r');
    output = getDigit(server_answer);           // warning: if connection failed, server_answer gets a large number
    //Serial.println("server_answer: ");
    //Serial.println(server_answer);
    done = true;
  }
  Serial.print("output: ");
  //output = -5; //test um gewählte temperaturen binär anzeigen zu lassen
  Serial.println(output);
  
  // reset of the current leds, needed if corrupted number was submitted
  for (int i = 0; i < 9; i++){
    ledControll[i] = false;
  }
  
  int binar = make_bin(output);   // led outputs are saved in ledControll

 

  for (int i = 0; i < 8; i++) {
    Serial.print("LED Controll");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(ledControll[i]);
  }
  if (output < 100){            // if output get a corrupted int from failed connection, do not update leds
    led_controll();
  } 
  run_counter();
}
