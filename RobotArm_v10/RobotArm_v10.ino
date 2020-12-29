//AQUAMAN
//LAST FIX - NEW HOME POSITION

#include <Servo.h>
#include <math.h>
Servo servo_base;
Servo servo_mid;
Servo servo_end;

int servo_base_position;
int time_current;
int time_previous=0;
int time_interval=1000;
float x;
float x_translate;
float y;
float y_translate;
float r;
float phi;
float theta;
float phi_d;
float theta_d;
float x1;
float x2;
float y1;
float y2;
float servo_base_angle;
float servo_mid_angle;
float servo_end_angle;
float servo_mid_angle_prev;
float servo_end_angle_prev;
float line_x1;
float line_x2;
float line_y1;
float line_y2;
float r_origin;
float r_relative=0;
float rho;
float line_x_relative;
float line_y_relative;
float increments_n;
float increments_l=.03; // inches
float line_x_endpoint;
float line_y_endpoint;
float line_x_i;
float line_y_i;
float cos_rho;
float sin_rho;
float L2=sqrt(2)/2;
int L1=3;
char ch; //The Recieved Character
int count_character; //count how many characters there are
String gcode_mode_letter=""; //Store Info About Gcode Mode ("G","M",ETC)
String gcode_mode_number=""; //Store Info About Gcode Mode Number (Number After Letter)
String gcode_mode_name=""; //DEBUG - TELL ME WHAT THE GCODE DOES
String gcode_pos_x="";
String gcode_pos_y="";
float pos_x;
float pos_y;
float pos_x_prev;
float pos_y_prev;
int counter_first_move=0;
int pos_x_received;
int pos_y_received;

void setup() {
  // put your setup code here, to run once:
home_position();
delay(1000);
Serial.begin(9600);
Serial.println("System Ready");
}

void loop() {
  delay(100);
  // put your main code here, to run repeatedly:
if(Serial.available()>1){//If there is more than ONE THING in the BUFFER do SOMETHING
    ch=Serial.read();// a VARIABLE "ch" is EQUAL to SOMETHING
   }
  while(Serial.available()>0){ //while there is SOMETHING in the BUFFER do SOMETHING. if there is NOTHING to do, the BUFFER will CLEAR automatically
    gcode_mode_number="";
    pos_x_received=0;
    pos_y_received=0;
    switch(ch){
      case 'G': // if the VARIABLE is "G" do SOMETHING
        ch='0'; // RESET VARIABLE "ch" so we can fo into the WHILE LOOP
        while((ch>='0'&&ch<='9')||ch=='.'){ //WHILE VARIABLE "ch" is a NUMBER CHARACTER or DECIMAL POINT
          ch=Serial.read(); //READ the next VARIABLE in the BUFFER
          gcode_mode_number+=ch; //MAKE a STRING for NUMBERS/DECIMAL POINT ONLY
        }
        switch(gcode_mode_number.toInt()){//MAKE the STRING an INTEGER
            case 0://IF EQUAL to 0 do SOMETHING - G-Code G00
              gcode_mode_name="LINEAR MOVE";//HELPS FOR DEBUGGING, TELL ME WHAT KIND OF FUNCTION
              gcode_pos_x="";
              gcode_pos_y="";
              Serial.println(gcode_mode_name);//PRINT RESULT FOR DEBUGGING
              while(ch==' '){
                ch=Serial.read();
              }
              if(ch=='X'){// BECAUSE OF THE WHILE LOOP IN CASE 'G' THERE IS ALREADY A NEW VARIABLE PLACED IN "ch"
                //Serial.print("X POSITION:");//PRINT RESULT FOR DEBUGGING
                ch='0';//RESET THE VARIABLE TO ENTER WHILE LOOP
                while((ch>='0'&&ch<='9')||ch=='.'){//WHILE THE VARIABLE IS A NUMBER OR DECIMAL POINT DO SOMETHING
                  ch=Serial.read();//READ THE NEXT VARIABLE IN THE BUFFER
                  gcode_pos_x+=ch;//MAKE A STRING FOR NUMBERS/DECIMAL POINT
                }
                pos_x=gcode_pos_x.toFloat();
                pos_x_received=1;
                //Serial.println(pos_x);//PRINT THE RESULT FOR DEBUGGING
              }
              while(ch==' '){
                ch=Serial.read();
              }
              if(ch=='Y'){
                //Serial.print("Y POSITION:");//PRINT RESULT FOR DEBUGGING
                ch='0';//RESET THE VARIABLE TO ENTER WHILE LOOP
                while((ch>='0'&&ch<='9')||ch=='.'){//WHILE THE VARIABLE IS A NUMBER OR DECIMAL POINT DO SOMETHING
                  ch=Serial.read();//READ THE NEXT VARIABLE IN THE BUFFER
                  gcode_pos_y+=ch;//MAKE A STRING FOR NUMBERS/DECIMAL POINT
                }
                pos_y=gcode_pos_y.toFloat();
                pos_y_received=1;
                //Serial.println(pos_y);//PRINT THE RESULT FOR DEBUGGING           
              }
              if (pos_y_received==1&&pos_x_received==1&&counter_first_move==1){
                //Serial.println("I AM MOTORBOATING WITH DRAW_LINE");
                //Serial.println("HERE IS WHERE I WAS:");
                //Serial.print("X-");
                //Serial.print(pos_x_prev);
                //Serial.print(" Y-");
                //Serial.println(pos_y_prev);
                //Serial.print(1);
                draw_line(pos_x_prev,pos_y_prev,pos_x,pos_y);                
                pos_x_prev=pos_x;
                pos_y_prev=pos_y;
              }
              if (pos_y_received==1&&pos_x_received==1&&counter_first_move==0){
                //Serial.println("I AM MOTORBOATING FOR THE FIRST TIME WITH DRAW_POINT");//THIS IS WHERE THE draw_point SERVO FUNCTION GOES
                //Serial.print(1);
                draw_point(pos_x,pos_y);
                counter_first_move=1;
                pos_x_prev=pos_x;
                pos_y_prev=pos_y;
              }
            break;
            case 1://IF EQUAL to 1 do SOMETHING - G-Code G01
              gcode_mode_name="LINEAR MOVE";
              Serial.println(gcode_mode_name);
            break;
             case 20://IF EQUAL to 20 do SOMETHING - G-Code G20
              gcode_mode_name="SET UNITS TO INCHES";
              Serial.println(gcode_mode_name);
            break;
            case 21://IF EQUAL to 21 do SOMETHING - G-Code G21
              gcode_mode_name="SET UNITS TO MM";
              Serial.println(gcode_mode_name);
            break;
            case 28://IF EQUAL to 28 do SOMETHING - G-Code G28
              gcode_mode_name="GO HOME";
              Serial.println(gcode_mode_name);
              draw_point(0,0);
            break;
            case 90://IF EQUAL to 90 do SOMETHING - G-Code G90
              gcode_mode_name="USE ABSOLUTE COORDINATES";
              Serial.println(gcode_mode_name);
            break;
            case 91://IF EQUAL to 91 do SOMETHING - G-Code G91
              gcode_mode_name="USE RELATIVE COORDINATES";
              Serial.println(gcode_mode_name);
            break;
        }
    default:
    if (ch!='G'){
      ch=Serial.read();
    }
    }
    //Serial.println(gcode_mode_name);//THIS IS WHERE THE ACTION NEEDS TO BE - PLACE MOTOR CONTROL FUNCTIONS HERE
  }
    //Serial.println(gcode_mode_name);
    gcode_mode_name="";
    gcode_mode_number="";
    gcode_mode_letter="";
    //Serial.println(gcode_mode_name);
    //draw_line(0,0,0,0);
}
//____________________________________________________
// Go To Home Position
void home_position(){
  servo_mid.attach(5);
  servo_mid.write(110);
  delay(500);
  servo_end.attach(6);
  servo_end.write(62);
  delay(500);
  servo_base.attach(3);
  servo_base.write(50);
  delay(500);
  //servo_end.attach(6);
  //servo_end.write(130);
  //delay(500);
}
//________________________________________________
// Draw Any Point
void draw_point(float x,float y){
  x_translate=x-3;
  y_translate=y+2.63;
  r=sqrt(pow(x_translate,2)+pow(y_translate,2));
  phi=acos((r-L2)/(2*L1));
  theta=asin(x_translate/r);
  phi_d=phi*RAD_TO_DEG;
  theta_d=theta*RAD_TO_DEG;
  servo_base_angle=110-theta_d;
  servo_mid_angle=40+phi_d;
  servo_end_angle=62+phi_d;
  servo_mid_angle_prev=servo_mid.read();
  servo_end_angle_prev=servo_end.read();
  if (servo_mid_angle_prev<servo_mid_angle){
    servo_mid.write(servo_mid_angle);
    servo_end.write(servo_end_angle);
    delay(10);
  }
  else{
    servo_end.write(servo_end_angle);
    servo_mid.write(servo_mid_angle);
  }
  if (x_translate<0){
    servo_base.write(servo_base_angle);
  }
  if (x_translate>=0){
    servo_base.write(servo_base_angle);
  }
  /**servo_end.write(180);
  servo_mid.write(80);
  delay(2000);
  servo_end.write(100);
  servo_mid.write(150);
  delay(2000);**/
}
//____________________________________________________________________________
// Draw Line Increment
void draw_line_increment(float x1,float y1,float x2,float y2){
  draw_point(x1,y1);
  draw_point(x2,y2);
}
//____________________________________________________________________________
// Draw Line with Increments Between Two (2) Points
void draw_line(float line_x1,float line_y1,float line_x2,float line_y2){
  draw_point(line_x1,line_y1);
  line_x_relative=line_x2-line_x1;
  line_y_relative=line_y2-line_y1;
  line_x_endpoint=line_x2;
  line_y_endpoint=line_y2;
  r_origin=sqrt(pow(line_x_relative,2)+pow(line_y_relative,2));
  rho=asin(fabs(line_y_relative)/r_origin);
  cos_rho=cos(rho);
  sin_rho=sin(rho);
  increments_n=(r_origin/increments_l);
  if (line_x_relative>=0&&line_y_relative>=0){//X POSITIVE, Y POSITIVE
  for (float i=1;i<increments_n;i=i+1){
    line_x_i=(r_relative+increments_l)*cos_rho+line_x1;
    line_y_i=(r_relative+increments_l)*sin_rho+line_y1;
    draw_point(line_x_i,line_y_i);
    r_relative=r_relative+increments_l;
  }
  draw_point(line_x_endpoint,line_y_endpoint);
  r_relative=0;
  }
  if (line_x_relative>=0&&line_y_relative<0){//X POSITIVE, Y NEGATIVE
  for (float i=1;i<increments_n;i=i+1){
    line_x_i=(r_relative+increments_l)*cos_rho+line_x1;
    line_y_i=(((r_relative+increments_l)*sin_rho)*-1)+line_y1;
    draw_point(line_x_i,line_y_i);
    r_relative=r_relative+increments_l;
  }
  draw_point(line_x_endpoint,line_y_endpoint);
  r_relative=0;
  }
  if (line_x_relative<0&&line_y_relative>=0){//X NEGATIVE, Y POSITIVE
  for (float i=1;i<increments_n;i=i+1){
    line_x_i=(((r_relative+increments_l)*cos_rho)*-1)+line_x1;
    line_y_i=(r_relative+increments_l)*sin_rho+line_y1;
    draw_point(line_x_i,line_y_i);
    r_relative=r_relative+increments_l;
  }
  draw_point(line_x_endpoint,line_y_endpoint);
  r_relative=0;
  }
  if (line_x_relative<0&&line_y_relative<0){//X NEGATIVE, Y NEGATIVE
  for (float i=1;i<increments_n;i=i+1){
    line_x_i=(((r_relative+increments_l)*cos_rho)*-1)+line_x1;
    line_y_i=(((r_relative+increments_l)*sin_rho)*-1)+line_y1;
    draw_point(line_x_i,line_y_i);
    r_relative=r_relative+increments_l;
  }
  draw_point(line_x_endpoint,line_y_endpoint);
  r_relative=0;
  }
}

