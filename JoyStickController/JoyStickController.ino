#include <ESP32Servo.h>

Servo ForwardBack_servo;  // create servo object to control a servo
Servo LeftRight_servo;
int ForwardBackPort = 15;
int LeftRightPort = 2;

void Forward();
void Stop();
void Left();
void Right();

void setup() {
  //myservo.attach(15);  // attaches the servo on pin 9 to the servo object
  ForwardBack_servo.attach(ForwardBackPort);
  ForwardBack_servo.attach(LeftRightPort);
  Serial.begin(9600);
}

void loop() {
  //for(int i = 0; i <= 360; i++){
    ForwardBack_servo.write(90);
    delay(1000);
    ForwardBack_servo.write(5);
    delay(1000);
    //LeftRight_servo.write(i);
    //Serial.println(i);
    //delay(15);
  //}
}

void Forward(){
  ForwardBack_servo.attach(ForwardBackPort);

  ForwardBack_servo.detach();
}
void Stop(){
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
