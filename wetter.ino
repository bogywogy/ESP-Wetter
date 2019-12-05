#include <ESP8266WiFi.h>
#include <Wire.h>
#include <stdio.h>

const char* ssid = "AlbWetter";
const char* password = "*********";
const char* host = "api.thingspeak.com";
char binarstr[9];
char kalte[6];
int temperatur;

int make_bin(int dezi) {
  int rest, ergebnis = 0, faktor = 1;
  while (dezi) {
    rest = dezi % 2;
    dezi /= 2;
    ergebnis = ergebnis + rest * faktor;
    faktor *= 10;
  }
  return ergebnis;
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

  digitalWrite(D0, LOW);
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW);
  digitalWrite(D5, LOW);
  digitalWrite(D6, LOW);
  digitalWrite(D7, LOW);
  digitalWrite(D8, LOW);

  // Connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
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

  // Create an URL for the request
  String url = "/apps/thinghttp/send_request?api_key=V67Q2ZYSWAZ0X9GR";
  Serial.print("Requesting URL: ");
  Serial.println(url);
  Serial.println();
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  delay(500);

  // Read all lines of the reply from server and check if they match a pattern
  bool done = false;
  bool minus = false;
  while (done == false) {
    String lline = client.readStringUntil('\r');
    // String lline = "06<spa";

    if (lline[1] =="-"[0]) {
      kalte[0] = lline[2];
      temperatur = (int)kalte[0] - 48;
      if (int(lline[3]) == 48 || int(lline[3]) == 49 || int(lline[3]) == 50 || int(lline[3]) == 51 || int(lline[3]) == 52 || int(lline[3]) == 53 || int(lline[3]) == 54 || int(lline[3]) == 55 || int(lline[3]) == 56 || int(lline[3]) == 57) {
        kalte[1] = lline[3];
        temperatur = (int)kalte[1] - 48 + temperatur * 10;
      }

      done = true;
      minus = true;

    } else {

      if (int(lline[1]) == 48 || int(lline[1]) == 49 || int(lline[1]) == 50 || int(lline[1]) == 51 || int(lline[1]) == 52 || int(lline[1]) == 53 || int(lline[1]) == 54 || int(lline[1]) == 55 || int(lline[1]) == 56 || int(lline[1]) == 57) {
        kalte[0] = lline[1];
        temperatur = (int)kalte[0] - 48;

        done = true;

        if (int(lline[2]) == 48 || int(lline[2]) == 49 || int(lline[2]) == 50 || int(lline[2]) == 51 || int(lline[2]) == 52 || int(lline[2]) == 53 || int(lline[2]) == 54 || int(lline[2]) == 55 || int(lline[2]) == 56 || int(lline[2]) == 57) {
          kalte[1] = lline[2];
          temperatur = (int)kalte[1] - 48 + temperatur * 10;
        }
      }
    }
  }

  Serial.print("Temperatur: ");
  Serial.println(temperatur);


  int binar = make_bin(temperatur);

  sprintf(binarstr, "%d", binar);           // cast int to string
  Serial.print("Als Binärzahl im String: ");
  Serial.println(binarstr);
  Serial.println();

  for (int i = 0; i < 8; i++) {
    Serial.print("Die einzelnen Werte des Strings: ");
    Serial.println(binarstr[i]);
  }
  Serial.println();

  // Determine length and shift
  int len = 0;
  for (int i = 0; i < 8; i++) {
    if (int(binarstr[i])) {
      len+=1;
    }
  }
  int shift = 8 - len;

  if (int(binarstr[7-shift]) == 49) {
    digitalWrite(D7, HIGH);
  } else {
    digitalWrite(D7, LOW);
  }

  if (int(binarstr[6-shift]) == 49) {
    digitalWrite(D6, HIGH);
  } else {
    digitalWrite(D6, LOW);
  }

  if (int(binarstr[5-shift]) == 49) {
    digitalWrite(D5, HIGH);
  } else {
    digitalWrite(D5, LOW);
  }

  if (int(binarstr[4-shift]) == 49) {
    digitalWrite(D4, HIGH);
  } else {
    digitalWrite(D4, LOW);
  }

  if (int(binarstr[3-shift]) == 49) {
    digitalWrite(D3, HIGH);
  } else {
    digitalWrite(D3, LOW);
  }

  if (int(binarstr[2-shift]) == 49) {
    digitalWrite(D2, HIGH);
  } else {
    digitalWrite(D2, LOW);
  }

  if (int(binarstr[1-shift]) == 49) {
    digitalWrite(D1, HIGH);
  } else {
    digitalWrite(D1, LOW);
  }

  if (minus == true) {
    Serial.print("Setting minus/negative LED");
    digitalWrite(D0, HIGH);
  } else {
    digitalWrite(D0, LOW);
  }

}
