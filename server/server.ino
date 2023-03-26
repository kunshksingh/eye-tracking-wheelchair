// Sources
//ESP32 Espressif Documentation
//https://github.com/espressif/arduino-esp32/blob/409926ef778c8d0beabb53c09d6515d94b3fe00b/libraries/WiFi/examples/SimpleWiFiServer/SimpleWiFiServer.ino

#include <WiFi.h>

//wifi password
//must be 2.4GHZ => for iphone 12 and up select maximize compatability in personal hotspot settings
//Dhruv Hot Spot
const char* ssid     = "Dhruv's iphone";
const char* password = "DhruveyGroove";
//Kunsh Hot Spot
// const char* ssid     = "@imthekunsh";
// const char* password = "bubjibing";

int left = 0;
int right = 0;

int stop = 0;

WiFiServer server(80);

void setup()
{
    Serial.begin(9600);

    delay(10);

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
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.begin();

}

int value = 0;

void loop(){
 WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character
            client.println();
            currentLine = "";
        } else {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}