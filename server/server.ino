//ESP32

// Sources
//ESP32 Espressif Documentation
//https://github.com/espressif/arduino-esp32/blob/409926ef778c8d0beabb53c09d6515d94b3fe00b/libraries/WiFi/examples/SimpleWiFiServer/SimpleWiFiServer.ino

#include <WiFi.h>
#include <ESP32Servo.h>

#define RXp2 16
#define TXp2 17

//wifi password
//must be 2.4GHZ => for iphone 12 and up select maximize compatability in personal hotspot settings
//Dhruv Hot Spot
const char* ssid     = "iPhone";
const char* password = "DhruveyGroove";
//Kunsh Hot Spot
// const char* ssid     = "@imthekunsh";
// const char* password = "bubjibing";

int left = 0;
int right = 0;

int stop = 0;

WiFiServer server(80);

Servo ForwardStop_servo;  // create servo object to control a servo
Servo LeftRight_servo;
int ForwardStopPort = 15;
int LeftRightPort = 2;

void Forward();
void Stop();
void Left();
void Right();

void setup()
{
    Serial.begin(115200);
    Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
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
            Serial.println(Serial2.readString());
            client.println();
            switch (currPos){
            // Bitstring : (Up)(Down)(Left)(Right) => ex. "0100" is Down
              case 1:
                Stop();
                break;
              case 2:
                Forward();
                break;
              case 3:
                Left();
                break;
              case 4:
                Right();
                break;
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
  ForwardStop_servo.attach(ForwardStopPort);

  ForwardStop_servo.detach();

  Serial.print("Forward");
}
void Stop(){
  ForwardStop_servo.attach(ForwardStopPort);

  ForwardStop_servo.detach();
  Serial.print("Stop");
}
void Right(){
  LeftRight_servo.attach(LeftRightPort);


  LeftRight_servo.detach();
  Serial.print("Right");
}
void Left(){
  LeftRight_servo.attach(LeftRightPort);


  LeftRight_servo.detach();
  Serial.print("Left");
}
