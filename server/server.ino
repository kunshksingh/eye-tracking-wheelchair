//ESP32

// Sources
//ESP32 Espressif Documentation
//https://github.com/espressif/arduino-esp32/blob/409926ef778c8d0beabb53c09d6515d94b3fe00b/libraries/WiFi/examples/SimpleWiFiServer/SimpleWiFiServer.ino

#include <WiFi.h>
#include <ESP32Servo.h>

#define RXp2 16
#define TXp2 17
#define BITpin0 22
#define BITpin1 23

//wifi password
//must be 2.4GHZ => for iphone 12 and up select maximize compatability in personal hotspot settings
//Dhruv Hot Spot
//const char* ssid     = "iPhone";
//const char* password = "DhruveyGroove";
//Kunsh Hot Spot
 const char* ssid     = "@imthekunsh";
 const char* password = "bubjibing";

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
  pinMode(BITpin0,OUTPUT);
  pinMode(BITpin1,OUTPUT);
  //setup motor positions 

    ForwardServo.attach(ForwardPort);
    LeftServo.attach(LeftPort);
    RightServo.attach(RightPort);
    //90 to 0
    ForwardServo.write(120);
    delay(100);
    ForwardServo.detach();
    //90 to 180
    LeftServo.write(60);
    delay(100);
    LeftServo.detach();
    //90 to 180
    RightServo.write(80);
    delay(100);
    RightServo.detach();


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
        // Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character
            client.println();
            switch (currPos){
            // Bitstring : (Up)(Down)(Left)(Right) => ex. "0100" is Down
              case 1:
                digitalWrite(BITpin0, HIGH);
                digitalWrite(BITpin1, LOW);
                Stop();
                break;
              case 2:
                digitalWrite(BITpin0, LOW);
                digitalWrite(BITpin1, HIGH);
                Forward();
                break;
              case 3:
                digitalWrite(BITpin0, HIGH);
                digitalWrite(BITpin1, HIGH);
                Left();
                break;
              case 4:
                digitalWrite(BITpin0, LOW);
                digitalWrite(BITpin1, LOW);
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
  LeftServo.detach();
  RightServo.detach();
  ForwardServo.attach(ForwardPort);
  ForwardServo.write(0);
  delay(10);
  Serial.print("Forward");
}
void Stop(){
  ForwardServo.attach(ForwardPort);
  LeftServo.attach(LeftPort);
  RightServo.attach(RightPort);
  //90 to 0
  ForwardServo.write(90);
  delay(10);
  ForwardServo.detach();
  //90 to 180
  LeftServo.write(90);
  delay(10);
  LeftServo.detach();
  //90 to 180
  RightServo.write(90);
  delay(10);
  RightServo.detach();
  Serial.print("Stop");
}
void Right(){
  ForwardServo.detach();
  LeftServo.detach();
  RightServo.attach(RightPort);
  RightServo.write(180);
  delay(10);
  Serial.print("Right");
}
void Left(){
  ForwardServo.detach();
  RightServo.detach();
  LeftServo.attach(LeftPort);
  LeftServo.write(180);
  delay(10);
  Serial.print("Left");
}
