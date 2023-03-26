//ARDUINO

#define enA 9
#define in1 4
#define in2 5
#define enB 10
#define in3 6
#define in4 7

#define BIT0 12
#define BIT1 13

void Forward();
void Stop();
void Left();
void Right();
void custom_delay();

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(BIT0, INPUT);    
  pinMode(BIT1, INPUT);    
  Serial.begin(9600); // start serial communication with the Arduino Uno at 9600 baud
}

void loop() {
  Serial.print("B1");
  Serial.print(digitalRead(BIT1));
  Serial.print("B1end");
  Serial.println();
  Serial.print("B0");
  Serial.print(digitalRead(BIT0));
  Serial.print("B0end");
  Serial.println();
  delay(200);
}

void Forward(){
  Serial.println("Forward");
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, 255); // Send PWM signal to motor A
  analogWrite(enB, 255); // Send PWM signal to motor B
  custom_delay();
}
void Stop(){
  Serial.println("Stop");
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enA, 255); // Send PWM signal to motor A
  analogWrite(enB, 255); // Send PWM signal to motor B
  custom_delay();
}
void Right(){
  Serial.println("Right");
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, 255); // Send PWM signal to motor A
  analogWrite(enB, 255); // Send PWM signal to motor B
  custom_delay();
}
void Left(){
  Serial.println("Left");
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enA, 255); // Send PWM signal to motor A
  analogWrite(enB, 255); // Send PWM signal to motor B
  custom_delay();
}

void custom_delay(){
  delay(20);
}