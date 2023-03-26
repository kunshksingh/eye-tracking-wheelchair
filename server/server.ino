// Sources
//ESP32 Espressif Documentation
//https://github.com/espressif/arduino-esp32/blob/409926ef778c8d0beabb53c09d6515d94b3fe00b/libraries/WiFi/examples/SimpleWiFiServer/SimpleWiFiServer.ino

#include <WiFi.h>
#include <ESP32Servo.h>


//wifi password
//must be 2.4GHZ => for iphone 12 and up select maximize compatability in personal hotspot settings
//Dhruv Hot Spot
// const char* ssid     = "Dhruv's iphone";
// const char* password = "DhruveyGroove";
//Kunsh Hot Spot
const char* ssid     = "@imthekunsh";
const char* password = "bubjibing";

int left = 0;
int right = 0;

int stop = 0;

WiFiServer server(80);

Servo ForwardBack_servo;  // create servo object to control a servo
Servo LeftRight_servo;
int ForwardBackPort = 15;
int LeftRightPort = 2;

void Forward();
void Backward();
void Left();
void Right();

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
    bool movePos = true;
    int currPos = 1;
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character
            client.println();
            switch (currPos){
            // Bitstring : (Up)(Down)(Left)(Right) => ex. "0100" is Down
              case 1:
                Forward();
              case 2:
                Backwards();
              case 3:
                Left();
              case 4:
                Right();
            } 
            currentLine = ""; 
            movePos = true;
            currPos = 1;

        } else {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
          if (c == '1'){
            movePos = false;
          }
          if (movePos){
            currPos += 1;
          }
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
void Forward(){
  ForwardBack_servo.attach(ForwardBackPort);

  ForwardBack_servo.detach();
}
void Backwards(){
  ForwardBack_servo.attach(ForwardBackPort);

  ForwardBack_servo.detach();
}
void Right(){
  LeftRight_servo.attach(LeftRightPort);


  LeftRight_servo.detach();
}
void Left(){
  LeftRight_servo.attach(LeftRightPort);


  LeftRight_servo.detach();
}
