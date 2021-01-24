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
float theta32;
float theta32_d;
float z_cal;
float dz=0;
float theta_servo_3_d;
float theta43;
float theta_servo_4_d;
float dz_prev=100;

float Xoffset=-159.38; //mm
float Yoffset=100;//mm
float Zoffset=15;//mm
float servo1_offset=0; //determine from calibration in degrees
float servo2_offset=0;
float servo3_offset=0;
float servo4_offset=45;
float servo5_offset=0;
float servo1_home=0;
float servo2_home=135;
float servo3_home=135;
float servo4_home=40;
float x;
float y;
float z;
float results[5];
void angles(float ,float , float);
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
Serial.println("INPUT X ");
while (!Serial.available()){}
x=(Serial.readString()).toFloat();
Serial.println("X is "+String(x));
Serial.println("INPUT Y ");
while (!Serial.available()){}
y=(Serial.readString()).toFloat();
Serial.println("Y is "+String(y));
Serial.println("INPUT Z ");
while (!Serial.available()){}
z=(Serial.readString()).toFloat();
Serial.println("Z is "+String(z));

x=x+Xoffset;
y=y+Yoffset;
z=z+Zoffset; 
//angles(x,y,z, .8464);
angles(x,y,z);

}

void loop() {
  //Serial.println(results[0]);
  //delay(1000);
  // put your main code here, to run repeatedly:
  Serial.println("INPUT X ");
while (!Serial.available()){}
x=(Serial.readString()).toFloat();
Serial.println("X is "+String(x));
Serial.println("INPUT Y ");
while (!Serial.available()){}
y=(Serial.readString()).toFloat();
Serial.println("Y is "+String(y));
Serial.println("INPUT Z ");
while (!Serial.available()){}
z=(Serial.readString()).toFloat();
Serial.println("Z is "+String(z));

x=x+Xoffset;
y=y+Yoffset;
z=z+Zoffset; 
//angles(x,y,z, .8464);
angles(x,y,z);
}

void angles(float x, float y, float z){
  Serial.println(x);
  Serial.println(y);
  Serial.println(z);
  //Spherical Coords only need to calculate one time this is an area of improvement
  rho=sqrt(sq(x)+sq(y)+sq(z));//3d hypotenuse
  theta=acos(z/rho); //angle between z axis and rho
  phi=atan(y/x);//angle of servo 1
  float LT2X=sqrt(sq(rho)-sq(z));//Calculate 3D hypotenuse
  //need to calculate the following iteratively
  for (float theta2_iterate=0;theta2_iterate<=90;theta2_iterate=theta2_iterate+0.5){
    theta32=asin((LT2X-(L32*cos(theta2_iterate*DEG_TO_RAD))-LT4)/L43);//Calculate theta32
    theta32_d=theta32*RAD_TO_DEG;//Convert theta32 to degrees
    z_cal=L2G+(L32*sin(theta2_iterate*DEG_TO_RAD))-(L43*cos(theta32));//calculate z
    //Serial.println(z_cal);
    dz=abs(z_cal-z);//use this value to determine accuracy (closer to 0 is better)
    //Serial.println(dz);
    theta_servo_3_d=180-((90-theta2_iterate)+theta32_d);//use this value for the servo 3 angle in degrees
    theta43=90-theta32_d;
    theta_servo_4_d=theta43;
//add an IF statement here comparing the dz value, if lower then save all parameters in an array for later
    if (dz<=dz_prev&&isnan(dz)==0){
    results[0]=theta2_iterate;
    results[1]=phi;
    if (x<0){
      results[1]=PI+phi;
      Serial.println(results[1]);
    }
    results[2]=theta_servo_4_d;
    results[3]=dz;
    results[4]=theta_servo_3_d;
    }
    dz_prev=dz;
  }
  move(results[1]*RAD_TO_DEG,results[0],results[4],results[2]);
  Serial.println("dz is "+String(results[3]));
  Serial.println(String(results[0]*DEG_TO_RAD));
  //Serial.println("servo 3 "+String(results[0]));
}
void move(float theta1, float theta2, float theta3, float theta4){
  //home servo 2 to prevent arm from crashing
  home_servo2();
  servo1.write(theta1+servo1_offset);
  servo2.write(theta2+servo2_offset);
  servo3.write(theta3+servo3_offset);
  servo4.write(theta4+servo4_offset);


}

void home_servo_all(){
 servo2.write(servo2_home+servo2_offset);
 servo1.write(servo1_home+servo1_offset);
 servo3.write(servo3_home+servo3_offset);
 servo4.write(servo4_home);
}
void home_servo2(){
 servo2.write(servo2_home+servo2_offset);
} 