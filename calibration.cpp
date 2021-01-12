#include <Arduino.h>
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
int servo1_pin=3;
int servo2_pin=5;
int servo3_pin=6;
int servo4_pin=4;
int servo5_pin=7;
String servo_num;
int servo_num_int;
String servo_ang;
int selected_servo;
void servo_move(int, int);


void setup() {
  Serial.begin(9600);
  servo1.attach(servo1_pin);
  servo1.write(90);
  servo2.attach(servo2_pin);
  servo2.write(10);
  servo3.attach(servo3_pin);
  servo3.write(160);
  servo4.attach(servo4_pin);
  servo5.attach(servo5_pin);
  servo5.write(90);
  // put your setup code here, to run once:
}

void loop() {
  Serial.println ("Enter Servo Number 1-5");
  while(!Serial.available()){}
  servo_num=Serial.readString();
  servo_num_int=servo_num.toInt();
  Serial.println("Enter Angle 0-180");
  while(!Serial.available()){}
  servo_ang=Serial.readString();
  servo_move(servo_num.toInt(),servo_ang.toInt());
}
void servo_move(int selected_servo, int degrees){
    switch (selected_servo)
  {
  case 1:
    Serial.println("Using Servo"+String(selected_servo)+" at "+degrees+" degrees");
    servo1.write(degrees);
    break;
  case 2:
    Serial.println("Using Servo"+String(selected_servo)+" at "+degrees+" degrees");
    servo2.write(degrees);
    break;
  case 3:
    Serial.println("Using Servo"+String(selected_servo)+" at "+degrees+" degrees");
    servo3.write(degrees);
    break;
  case 4:
    Serial.println("Using Servo"+String(selected_servo)+" at "+degrees+" degrees");
    servo4.write(degrees);
    break;
  case 5:
    Serial.println("Using Servo"+String(selected_servo)+" at "+degrees+" degrees");
    servo5.write(degrees);
    break;    
  default:
    break;
  }
}
