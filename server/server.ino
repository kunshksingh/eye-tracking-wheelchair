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

Servo ForwardServo;  // create servo object to control a servo
Servo LeftServo;
Servo RightServo;
int ForwardPort = 15;
int LeftPort = 2;
int RightPort = 4;

void Forward();
void Stop();
void Left();
void Right();

void setup()
{
  //setup motor positions 

    ForwardServo.attach(ForwardPort);
    LeftServo.attach(LeftPort);
    RightServo.attach(RightPort);
    //90 to 0
    ForwardServo.write(90);
    delay(100);
    ForwardServo.detach();
    //90 to 180
    LeftServo.write(90);
    delay(100);
    LeftServo.detach();
    //90 to 180
    RightServo.write(90);
    delay(100);
    RightServo.detach();


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
  ForwardServo.attach(ForwardPort);
  ForwardServo.write(0);
  delay(100);
  ForwardServo.detach();
  Serial.print("Forward");
}
void Stop(){
  ForwardServo.attach(ForwardPort);
  LeftServo.attach(LeftPort);
  RightServo.attach(RightPort);
  //90 to 0
  ForwardServo.write(90);
  delay(100);
  ForwardServo.detach();
  //90 to 180
  LeftServo.write(90);
  delay(100);
  LeftServo.detach();
  //90 to 180
  RightServo.write(90);
  delay(100);
  RightServo.detach();
  Serial.print("Stop");
}
void Right(){
  RightServo.attach(RightPort);
  RightServo.write(180);
  delay(100);
  RightServo.detach();
  Serial.print("Right");
}
void Left(){
  LeftServo.attach(LeftPort);
  LeftServo.write(180);
  delay(100);
  LeftServo.detach();
  Serial.print("Left");
}
