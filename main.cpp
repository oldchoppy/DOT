#include <Arduino.h>

float LT4=65; //mm
float L43=112.88; //mm
float L32=L43; //mm
float L2G=93; //mm
float Xoffset=-159.38; //mm
float Yoffset=60;//mm
float Zoffset=15;//mm
float x;
float y;
float z;
float results[5];
void angles(float ,float , float, float);

void setup() {
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
  //Spherical Coords
  float rho;
  float theta;
  float phi;
  rho=sqrt(sq(x)+sq(y)+sq(z));
  theta=acos(z/rho); 
  phi=atan(y/x);
  float LT2X=sqrt(sq(rho)-sq(z));
  //float theta_d=theta*RAD_TO_DEG;
 // float theta2=0.84;
  float theta2_d=theta2*RAD_TO_DEG;
  double theta32=asin((LT2X-(L32*cos(theta2))-LT4)/L43);
  float theta32_d=theta32*RAD_TO_DEG;
  double z_cal=L2G+(L32*sin(theta2))-(L43*cos(theta32));
  //Serial.println(z_cal);
  float dz=abs(z_cal-z);
  //Serial.println(dz);
  float theta_servo_3_d=(90-theta2_d)+theta32_d;

  /*theta2=48.5;
  theta32=5.18;
  z_cal=65.12;
  dz=0.12;
  theta_servo_3_d=46.68;
  results[0]=theta2;
  results[1]=theta32;
  results[2]=z_cal;
  results[3]=dz;
  results[4]=theta_servo_3_d;*/
}