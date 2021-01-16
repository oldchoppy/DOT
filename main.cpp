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
float LT4=65; //mm
float L43=112.88; //mm
float L32=L43; //mm
float L2G=93; //mm
float rho;
float theta;
float phi;
float Xoffset=-159.38; //mm
float Yoffset=60;//mm
float Zoffset=15;//mm
float servo1_offset=0; //determine from calibration in degrees
float servo2_offset=15;
float servo3_offset=-45;
float servo4_offset=10;
float servo5_offset=0;
float x;
float y;
float z;
float results[5];
void angles(float ,float , float, float);
void move(float, float, float, float);
void gripper_toggle(bool); //True is open, False is closed
void home_servo2();
void home_servo_all();

void setup() {
//init servos
servo2.attach(servo2_pin);
home_servo2();
servo1.attach(servo1_pin);
servo3.attach(servo3_pin);
servo4.attach(servo4_pin);
servo5.attach(servo5_pin);
home_servo_all();

//Gein serial and ask for input
Serial.begin(9600);
Serial.print("INPUT X ");
while (!Serial.available()){}
x=(Serial.readString()).toFloat();
Serial.println("X is "+String(x));
Serial.println("INPUT Y ");
while (!Serial.available()){}
y=(Serial.readString()).toFloat();
Serial.println("Y is "+String(y));
Serial.print("INPUT Z ");
while (!Serial.available()){}
z=(Serial.readString()).toFloat();
Serial.println("Y is "+String(z));

x=x+Xoffset;
y=y+Yoffset;
z=z+Zoffset; 
angles(x,y,z, .8464);


}

void loop() {
  //Serial.println(results[0]);
  //delay(1000);
  // put your main code here, to run repeatedly:
}

void angles(float x, float y, float z, float theta2){
  Serial.println(x);
  Serial.println(y);
  Serial.println(z);
  //Spherical Coords only need to calculate one time this is an area of improvement
  rho=sqrt(sq(x)+sq(y)+sq(z));//3d hypotenuse
  theta=acos(z/rho); //angle between z axis and rho
  phi=atan(y/x);//angle of servo 1
  float LT2X=sqrt(sq(rho)-sq(z));//Calculate 3D hypotenuse
  float theta2_d=theta2*RAD_TO_DEG;//Convert theta 2 to degrees
  //need to calculate the following iteratively
  float theta32=asin((LT2X-(L32*cos(theta2))-LT4)/L43);//Calculate theta32
  float theta32_d=theta32*RAD_TO_DEG;//Convert theta32 to degrees
  float z_cal=L2G+(L32*sin(theta2))-(L43*cos(theta32));//calculate z
  //Serial.println(z_cal);
  float dz=abs(z_cal-z);//use this value to determine accuracy (closer to 0 is better)
  //Serial.println(dz);
  float theta_servo_3_d=(90-theta2_d)+theta32_d;//use this value for the servo 3 angle in degrees

//add an IF statement here comparing the dz value, if lower then save all parameters in an array for later
  results[0]=theta2;
  results[1]=theta32;
  results[2]=z_cal;
  results[3]=dz;
  results[4]=theta_servo_3_d;
}
void move(float theta1, float theta2, float theta3, float theta4){
  //home servo 2 to prevent arm from crashing


}

void home_servo_all(){
 servo2.write(135+servo2_offset);
 servo1.write(90+servo1_offset);
 servo3.write(120);
}
void home_servo2(){
 servo2.write(120+servo2_offset);
} 