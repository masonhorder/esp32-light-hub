#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>

#define LED_PIN     13
#define NUM_LEDS    50



Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
// void setup(){
//   pixels.begin();
// }

// void loop(){
//   for(int i = 0; i<NUM_LEDS; i++){
//     if(i%3 == 0){
//       pixels.setPixelColor(i, pixels.Color(255,255,255));
//     }
//     else if(i%3 == 1){
//       pixels.setPixelColor(i, pixels.Color(255,255,255));
//     }
//     else{
//       pixels.setPixelColor(i, pixels.Color(255,255,255));
//     }
//   }
//   pixels.show();
//   delay(1000000);
// }



// Replace with your network credentials
const char* ssid     = "name";
const char* password = "pwd";

// Set web server port number to 80
WiFiServer server(80);

// Decode HTTP GET value
String redString = "0";
String greenString = "0";
String blueString = "0";
int pos1 = 0;
int pos2 = 0;
int pos3 = 0;
int pos4 = 0;

// Variable to store the HTTP req  uest
String header;



const int resolution = 8;

unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;

void setup() {
  Serial.begin(9600);

  
  // attach the channel to the GPIO to be controlled

  
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  pixels.begin();
}

void loop(){
  WiFiClient client = server.available();

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");
    String currentLine = "";                
    while (client.connected() && currentTime - previousTime <= timeoutTime) {            
      currentTime = millis();
      if (client.available()) {
        char c = client.read();
        Serial.write(c);                    
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            if(header.indexOf("GET /?r") >= 0) {
              pos1 = header.indexOf('r');
              pos2 = header.indexOf('g');
              pos3 = header.indexOf('b');
              pos4 = header.indexOf('&');
              redString = header.substring(pos1+1, pos2);
              greenString = header.substring(pos2+1, pos3);
              blueString = header.substring(pos3+1, pos4);
              Serial.println(redString+blueString+greenString);
              // for(int pixel; pixel < NUM_LEDS; pixel++){
                // pixels.setPixelColor(0, pixels.Color(greenString.toInt(),redString.toInt(),blueString.toInt()));
              pixels.fill(pixels.Color(greenString.toInt(),redString.toInt(),blueString.toInt()),0, NUM_LEDS);
              pixels.show();
            }
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}