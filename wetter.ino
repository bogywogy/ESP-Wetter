#include <ESP8266WiFi.h>
#include <Wire.h>  // This library is already built in to the Arduino IDE
#include <stdio.h>

const char* ssid = "AlbWetter"; // put your router name
const char* password = "********";// put your password
const char* host = "api.thingspeak.com";
char kalte[6];
String nu = "0";
String ei = "1";
String zw = "2";
String dr = "3";
String vi = "4";
String fu = "5";
String se = "6";
String si = "7";
String ac = "8";
String ne = "9";
bool done = false;
char binarstr[9];
bool minus=false;

int i;
int ii;
int iii;
int output;


int make_bin(int dezi){ // hier wird die integer zahl übergeben
  int zahl, rest, ergebnis=0, faktor=1;
  zahl=dezi;
  while (dezi){
    rest = dezi % 2;
    dezi = dezi / 2;
    
    faktor *= 10;
    ergebnis = ergebnis + rest * faktor;
    }
  ergebnis *= 0.1;
  return ergebnis;
}


void setup() {
  Serial.begin(115200);
  delay(100);

  pinMode(D0 ,OUTPUT);
  pinMode(D1 ,OUTPUT);
  pinMode(D2 ,OUTPUT);
  pinMode(D3 ,OUTPUT);
  pinMode(D4 ,OUTPUT);
  pinMode(D5 ,OUTPUT);
  pinMode(D6 ,OUTPUT);
  pinMode(D7 ,OUTPUT);
  pinMode(D8 ,OUTPUT);

  digitalWrite(D0, LOW);
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW);
  digitalWrite(D5, LOW);
  digitalWrite(D6, LOW);
  digitalWrite(D7, LOW);
  digitalWrite(D8, LOW);

 // We start by connecting to a WiFi network
 
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
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void loop(){
  Serial.print("connecting to ");
  Serial.println(host);
 
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
  }
 
  // We now create a URI for the request
  String url = "/apps/thinghttp/send_request?api_key=V67Q2ZYSWAZ0X9GR";
  Serial.print("Requesting URL: ");
  Serial.println(url);
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  delay(500);
 
  // Read all the lines of the reply from server and print them to Serial
   
    while(done == false){
      String lline = client.readStringUntil('\r');
    
            if(lline[1]=="-"[0]){
        kalte[0] = lline[2];
        i=(int)kalte[0];
        i-=48;
        output=i;
        
        if(lline[3] == nu[0]|| lline[3] == ei[0]|| lline[3] == zw[0]|| lline[3] == dr[0]|| lline[3] == vi[0]|| lline[3] == fu[0]|| lline[3] == se[0]|| lline[3] == si[0]|| lline[3] == ac[0]|| lline[3] == ne[0]){
          kalte[1] = lline[3];
          ii=(int)kalte[1];
          ii-=48;
          i*=10;
          ii+=i;
          output=ii;
        }
        
        done=true;
        minus=true;

        }else{
          
          if(lline[1] == nu[0]|| lline[1] == ei[0]|| lline[1] == zw[0]|| lline[1] == dr[0]|| lline[1] == vi[0]|| lline[1] == fu[0]|| lline[1] == se[0]|| lline[1] == si[0]|| lline[1] == ac[0]|| lline[1] == ne[0]){
            kalte[0] = lline[1];
            i=(int)kalte[0];
            i-=48;
            output=i;
            done = true;
            if(lline[2] == nu[0]|| lline[2] == ei[0]|| lline[2] == zw[0]|| lline[2] == dr[0]|| lline[2] == vi[0]|| lline[2] == fu[0]|| lline[2] == se[0]|| lline[2] == si[0]|| lline[2] == ac[0]|| lline[2] == ne[0]){
              kalte[1] = lline[2];
              ii=(int)kalte[1];
              ii-=48;
              i*=10;
              ii+=i;
              output=ii;
            }
          }
        }
      }
      Serial.print("output: ");
      Serial.println(output);
  int binar = make_bin(output);
  
  sprintf(binarstr, "%d", binar);                                       // cast int to string
  Serial.print("Binärzahl im String: ");
  Serial.println(binarstr);
  
  for(int i = 0; i < 8; i++){
    Serial.print("Binärzahl an index_");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(binarstr[i]);
  }

  if(int(binarstr[9]) == 49){//######################################## anstat den p- variablen kann man auch direkt die Pins ansprechen ! ##############################
    //p0 = HIGH;
    digitalWrite(D9, HIGH);
  }//else{ p0 = LOW;}
  
  if(int(binarstr[8]) == 49){
    //p1 = HIGH;
    digitalWrite(D8, HIGH);
  }//else{ p1 = LOW;}
  
  if(int(binarstr[7]) == 49){
    //p2 = HIGH;
    digitalWrite(D7, HIGH);
  }//else{ p2 = LOW;}
  
  if(int(binarstr[6]) == 49){
    //p3 = HIGH;
    digitalWrite(D6, HIGH);
  }//else{ p3 = LOW;}
  
  if(int(binarstr[5]) == 49){
    //p4 = HIGH;
    digitalWrite(D5, HIGH);
  }//else{ p4 = LOW;}
  
  if(int(binarstr[4]) == 49){
    //p5 = HIGH;
    digitalWrite(D4, HIGH);
  }//else{ p5 = LOW;}
  
  if(int(binarstr[3]) == 49){
    //p7 = HIGH;
    digitalWrite(D3, HIGH);
  }//else{ p7 = LOW;}
  
  if(int(binarstr[2]) == 49){
    //p8 = HIGH;
    digitalWrite(D2, HIGH);
  }//else{ p8 = LOW;}

  if(int(binarstr[1]) == 49){
    //p8 = HIGH;
    digitalWrite(D1, HIGH);
  }//else{ p8 = LOW;}

  if(int(binarstr[0]) == 49){
    //p8 = HIGH;
    digitalWrite(D0, HIGH);
  }//else{ p8 = LOW;}
    }
