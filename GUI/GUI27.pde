import processing.serial.*;

PShape button_check;
PShape button_slot;
PShape button_indicator;
int button_pos_x=20;
int button_pos_y=20;
int button_pos_y2=60;
int button_width=50;
int button_height=25;
int button_status;
String button_title1="BUTTON 1";
String button_title2="BUTTON 2";
String button_title_save="SAVE";
String button_title_load="LOAD";
String button_title_print="PRINT";
String button_title_polygon="POLYGON";
String button_title_connect="CONNECT";
String button_title_connected="CONNECTED";
int button_corner=10;
int[] button_color_false={0,125,255};
int[] button_color_true={145,200,255};
int button_menu_width=80;
int button_menu_height=40;
int window_sketch_x=20;
int window_sketch_y=200;
int window_sketch_height=500;
int window_sketch_width=window_sketch_height/4*6;
String window_sketch_title="Sketch Window:";
int window_sketch_bounds_x;
int window_sketch_bounds_y;
int window_sketch_flag=0;
int window_sketch_coordinate_x;
int window_sketch_coordinate_y;
int window_sketch_coordinate_x_close;
int window_sketch_coordinate_y_close;
int window_sketch_coordinate_x_previous;
int window_sketch_coordinate_y_previous;
int[] sketch_array_x=new int[1];
int[] sketch_array_y=new int[1];
int[] sketch_array_x2=new int[1];
int[] sketch_array_y2=new int[1];
int sketch_array_length;
int shape_complete=0;
PrintWriter GCODE;
int button_menu_save_flag=0;//USE THIS FLAG TO PREVENT MULTIPLE SAVE WINDOWS FROM POPULATING FOR ONE CLICK.
int button_menu_update_flag=0;//USE THIS FLAG TO PREVENT USING THE RENDERER WHILE BEGINSHAPE FUNCTION IS CALLED.
int button_menu_load_flag=0;//USE THIS FLAG TO PREVENT MULTIPLE WINDOWS POPULATING
int button_menu_print_flag=0;//USE THIS FLAG TO PRINT ONLY ONCE
int button_menu_connect_flag=0;//USE THIS FLAG TO CONNECT ONLY ONCE
int button_menu_polygon_flag=0;//USE THIS FLAG TO USE POLYGON BUTTON ONLY ONCE
int button_menu_polygon_status_flag=0;//USE THIS FLAG TO INITIATE THE SKETCH FUNCTION FOR POLYGONS
Serial Port1;
String serial_port_array[];

void setup() {
  //Serial Port1;
  size(1024,1024);
  background(192,192,192);
  textSize(24);
  // Create Buttons
  //button_create_ON(button_pos_x,button_pos_y,button_width,button_height,button_title1,button_corner);
  //button_create_ON(button_pos_x,button_pos_y2,button_width,button_height,button_title2,button_corner);
  // Create Sketch Window
  sketch_window_create();
  //SETUP GCODE FILE
  GCODE_FILE_SETUP();
  //CREATE SAVE BUTTON IN OFF POSITION
  button_menu_create_save_OFF();
  //CREATE LOAD BUTTON IN OFF POSITION
  button_menu_create_load_OFF();
  //CREATE PRINT BUTTON IN OFF POSITION
  button_menu_create_print_OFF();
  //CREATE CONNECT BUTTON IN OFF POSITION
  button_menu_create_connect_OFF();
}

void draw(){
  //button_update(); //update the button 1 every loop
  button_menu_save_update();//update the save button
  button_menu_load_update();//update the load button
  button_menu_print_update();//update the print button
  button_menu_polygon_update();//update the polygon button
  button_menu_connect_update();//update the connect button
  sketch_update_polygon();//update the polygon sketch function
  text_window_drawbot_clear();
}
//_________________________________________________________________
//THIS FUNCTION CREATES A TEXT WINDOW CONTAINING THE LAST MESSAGE FROM THE DRAWBOT
void text_window_drawbot_type(){
  button_menu_create(window_sketch_x,window_sketch_y+window_sketch_height+200,window_sketch_width,button_menu_height*2,button_title_connect,0,0,0);
}
//_________________________________________________________________
//THIS FUNCTION CREATES AN EMPTY TEXT WINDOW
void text_window_drawbot_clear(){
  button_menu_create(window_sketch_x,window_sketch_y+window_sketch_height+200,window_sketch_width,button_menu_height*2,button_title_connect,0,0,0);
}
//_________________________________________________________________
//This function updates the CONNECT button
void button_menu_connect_update(){
  if(button_menu_connect_flag==0&&button_menu_update_flag==0&&(mousePressed==true)&&(mouseButton==LEFT)&&(mouseX>window_sketch_x+300)&&(mouseX<window_sketch_x+button_menu_width+360)&&(mouseY>window_sketch_y+window_sketch_height+20)&&mouseY<window_sketch_y+window_sketch_height+20+button_menu_height){
   button_menu_connect_flag=1;
   button_menu_create_connect_ON();
  }
}
//_________________________________________________________________
//This function will create a CONNECT button in ON state
void button_menu_create_connect_ON(){
  serial_port_array=Serial.list();
  if(serial_port_array.length!=0){
  printArray(Serial.list());
  Port1 = new Serial(this, Serial.list()[0], 9600);
  button_menu_create(window_sketch_x+300,window_sketch_y+window_sketch_height+20,button_menu_width+80,button_menu_height,button_title_connected,button_color_true[0],button_color_true[1],button_color_true[2]);
  }
  if(serial_port_array.length==0){
    print("error drawbot not found - plug in the drawbot");
    text("error drawbot not found -plug in the drawbot",0,100);
    button_menu_connect_flag=0;
  }
}
//_________________________________________________________________
//This Function will create a CONNECT button in OFF state
void button_menu_create_connect_OFF(){
  button_menu_create(window_sketch_x+300,window_sketch_y+window_sketch_height+20,button_menu_width+60,button_menu_height,button_title_connect,button_color_false[0],button_color_false[1],button_color_false[2]);
}
//_________________________________________________________________
//This function updates the polygon sketch function
void sketch_update_polygon(){
  if (button_menu_polygon_status_flag==1){
    sketch();
    GCODE();
  }
}
//_________________________________________________________________
//This function updates the POLYGON button
void button_menu_polygon_update(){
  if((button_menu_polygon_status_flag==1&&button_menu_update_flag==0)||(button_menu_polygon_flag==0&&button_menu_update_flag==0&&(mousePressed==true)&&(mouseButton==LEFT)&&(mouseX>window_sketch_x+window_sketch_width+20)&&(mouseX<window_sketch_x+window_sketch_width+20+button_menu_width+50)&&(mouseY>window_sketch_y)&&mouseY<window_sketch_y+button_menu_height)){
   button_menu_create_polygon_ON();
   button_menu_polygon_status_flag=1;
   button_menu_polygon_flag=1;
  }
  else{if(button_menu_update_flag==0){
    button_menu_create_polygon_OFF();
  }
 }
}
//_________________________________________________________________
//This function will create a sketch POLYGON button in ON state
void button_menu_create_polygon_ON(){
  button_menu_create(window_sketch_x+window_sketch_width+20,window_sketch_y,button_menu_width+50,button_menu_height,button_title_polygon,button_color_true[0],button_color_true[1],button_color_true[2]);
}
//_________________________________________________________________
//This Function will create a sketch POLYGON button in OFF state
void button_menu_create_polygon_OFF(){
  button_menu_create(window_sketch_x+window_sketch_width+20,window_sketch_y,button_menu_width+50,button_menu_height,button_title_polygon,button_color_false[0],button_color_false[1],button_color_false[2]);
}
//_________________________________________________________________
//MOUSE RELEASED FUNCTION-USE TO RESET FLAGS AFTER THE BUTTON IS NO LONGER PRESSED
void mouseReleased(){
  button_menu_save_flag=0;
  button_menu_load_flag=0;
  button_menu_print_flag=0;
  button_menu_polygon_flag=0;
}
//_________________________________________________________________
//THIS FUNCTION WILL PRINT ANYTHING IN THE GCODE FILE REGARDLESS OF SHAPE COMPLETE
void GCODE_PRINT2(){
    String line_txt[] = loadStrings("GCODE/GCODE.txt");
    for(int i=0;i<line_txt.length;i++){
      Port1.write(line_txt[i]);
      delay(500);
      //GCODE_FILE_SETUP();
    }
  }  
//_________________________________________________________________
//This function updates the PRINT button
void button_menu_print_update(){
  if(button_menu_print_flag==0&&button_menu_update_flag==0&&(mousePressed==true)&&(mouseButton==LEFT)&&(mouseX>window_sketch_x+200)&&(mouseX<window_sketch_x+button_menu_width+200)&&(mouseY>window_sketch_y+window_sketch_height+20)&&mouseY<window_sketch_y+window_sketch_height+20+button_menu_height){
   button_menu_create_print_ON();
   GCODE.flush();
   GCODE.close();
   GCODE_PRINT2();
   button_menu_print_flag=1;
  }
  else{if(button_menu_update_flag==0){
   button_menu_create_print_OFF();
  }
 }
}
//_________________________________________________________________
//This function will create a PRINT button in ON state
void button_menu_create_print_ON(){
  button_menu_create(window_sketch_x+200,window_sketch_y+window_sketch_height+20,button_menu_width,button_menu_height,button_title_print,button_color_true[0],button_color_true[1],button_color_true[2]);
}
//_________________________________________________________________
//This Function will create a PRINT button in OFF state
void button_menu_create_print_OFF(){
  button_menu_create(window_sketch_x+200,window_sketch_y+window_sketch_height+20,button_menu_width,button_menu_height,button_title_print,button_color_false[0],button_color_false[1],button_color_false[2]);
}
//_________________________________________________________________
//This function opens a dialog box to load GCODE from a saved file
void fileSelected_to_load(File selection) {
  if (selection == null) {
    println("Window was closed or the user hit cancel.");
  } else {
    println("User selected " + selection.getAbsolutePath());
    String lines[] = loadStrings(selection.getAbsolutePath());
    GCODE=createWriter("GCODE/GCODE.txt");
    for(int i=0;i<lines.length;i++){
    GCODE.println(lines[i]);
    }
    GCODE.flush();
    GCODE.close();
  }
}

//_________________________________________________________________
//This function updates the LOAD button
void button_menu_load_update(){
  if(button_menu_load_flag==0&&button_menu_update_flag==0&&(mousePressed==true)&&(mouseButton==LEFT)&&(mouseX>window_sketch_x+100)&&(mouseX<window_sketch_x+button_menu_width+100)&&(mouseY>window_sketch_y+window_sketch_height+20)&&mouseY<window_sketch_y+window_sketch_height+20+button_menu_height){
   button_menu_create_load_ON();
   selectInput("Select a file to process:", "fileSelected_to_load");
   button_menu_load_flag=1;
  }
  else{if(button_menu_update_flag==0){
    button_menu_create_load_OFF();
  }
 }
}
//_________________________________________________________________
//This function will create a LOAD button in ON state
void button_menu_create_load_ON(){
  button_menu_create(window_sketch_x+100,window_sketch_y+window_sketch_height+20,button_menu_width,button_menu_height,button_title_load,button_color_true[0],button_color_true[1],button_color_true[2]);
}
//_________________________________________________________________
//This Function will create a LOAD button in OFF state
void button_menu_create_load_OFF(){
  button_menu_create(window_sketch_x+100,window_sketch_y+window_sketch_height+20,button_menu_width,button_menu_height,button_title_load,button_color_false[0],button_color_false[1],button_color_false[2]);
}
//_________________________________________________________________
//This function opens a dialog box to select an output file to save
void fileSelected(File selection) {
  if (selection == null) {
    println("Window was closed or the user hit cancel.");
  } else {
    println("User selected " + selection.getAbsolutePath());
    String lines[] = loadStrings("GCODE/GCODE.txt");
    GCODE=createWriter(selection.getAbsolutePath()+".txt");
    for(int i=0;i<lines.length;i++){
      GCODE.println(lines[i]);
   }
    GCODE.flush();
    GCODE.close();
  }
}
//_________________________________________________________________
//This function updates the SAVE button
void button_menu_save_update(){
  if(button_menu_update_flag==0&&button_menu_save_flag==0&&(mousePressed==true)&&(mouseButton==LEFT)&&(mouseX>window_sketch_x)&&(mouseX<window_sketch_x+button_menu_width)&&(mouseY>window_sketch_y+window_sketch_height+20)&&mouseY<window_sketch_y+window_sketch_height+20+button_menu_height){
   button_menu_create_save_ON();
   selectOutput("Select a file to write to:", "fileSelected");
   button_menu_save_flag=1;
  }
  else{if(button_menu_update_flag==0){
    button_menu_create_save_OFF();
  }
 }
}
//_________________________________________________________________
//This function will create a SAVE button in ON state
void button_menu_create_save_ON(){
  button_menu_create(window_sketch_x,window_sketch_y+window_sketch_height+20,button_menu_width,button_menu_height,button_title_save,button_color_true[0],button_color_true[1],button_color_true[2]);
}
//_________________________________________________________________
//This Function will create a SAVE button in OFF state
void button_menu_create_save_OFF(){
  button_menu_create(window_sketch_x,window_sketch_y+window_sketch_height+20,button_menu_width,button_menu_height,button_title_save,button_color_false[0],button_color_false[1],button_color_false[2]);
}
//_________________________________________________________________
//THIS FUNCTION WILL CREATE A MENU BUTTON. NON-TOGGLE STYLE.
void button_menu_create(int x, int y, int button_menu_width, int button_menu_height, String button_menu_title,int color1,int color2, int color3){
  fill(color1,color2,color3);
  rect(x,y,button_menu_width,button_menu_height,button_corner);
  fill(0,0,0);
  textSize(24);
  text(button_menu_title,x+10,y+30);
}

//_________________________________________________________________
//THIS FUNCTION WILL PRINT ANYTHING IN THE GCODE FILE
void GCODE_PRINT(){
  if(shape_complete==2){
    String line_txt[] = loadStrings("GCODE/GCODE.txt");
    for(int i=0;i<line_txt.length;i++){
      println(line_txt[i]);
      delay(500);
      shape_complete=0;
      //GCODE_FILE_SETUP();
    }
  }  
}

//_________________________________________________________________
//This function creates a text file with GCODE. ANY Setup GCODE Commands must be sent here.
void GCODE_FILE_SETUP(){
 GCODE=createWriter("GCODE/GCODE.txt");
 GCODE.println("G20");//INCHES
 GCODE.println("G28");//Go Home
 GCODE.println("G90");//USE ABSOLUTE COORDINATES
}

//_________________________________________________________________
//This function generates GCODE from the Vertex Array
void GCODE(){
  if(shape_complete==1){
    for(int i=0;i<sketch_array_x.length;i++){
      GCODE.print("G00");
      GCODE.print(" X");
      float x_value=sketch_array_x[i];
      x_value=map(x_value,window_sketch_x,window_sketch_x+window_sketch_width,0,6);
      float y_value=sketch_array_y[i];
      y_value=map(y_value,window_sketch_y,window_sketch_height,4,0);
      GCODE.print(x_value);
      GCODE.print(" Y");
      GCODE.println(y_value);
   }
       sketch_array_x=new int[1];
       sketch_array_y=new int[1];
       sketch_array_x2=new int[1];
       sketch_array_y2=new int[1];
       shape_complete=2;
       GCODE.flush();
       GCODE.close();
}
}

//_________________________________________________________________
//This Function builds an array of coordinates from a polygon 
void sketch(){
  //if the mouse is inside the sketch area and clicked the left button
  if((mousePressed==true)&&(mouseButton==LEFT)&&(mouseX>window_sketch_x)&&(mouseX<window_sketch_bounds_x)&&(mouseY>window_sketch_y)&&mouseY<window_sketch_bounds_y){
    //if the sketch flag is zero begin a new shape with no fill set sketch flag to one
    if(window_sketch_flag==0){
      button_menu_update_flag=1;
      noFill();
      beginShape();
    }
  //set the vertex to the coordinates of the mouse
    window_sketch_coordinate_x=mouseX;
    window_sketch_coordinate_y=mouseY;
    fill(0);
    arc(window_sketch_coordinate_x,window_sketch_coordinate_y,5,5,0,TWO_PI);
    noFill();
    vertex(window_sketch_coordinate_x, window_sketch_coordinate_y);
    //if this is a new sketch start building a new array of coordinates
    if(window_sketch_flag==0){
      sketch_array_x[0]=window_sketch_coordinate_x;
      sketch_array_y[0]=window_sketch_coordinate_y;
      window_sketch_coordinate_x_close=window_sketch_coordinate_x;
      window_sketch_coordinate_y_close=window_sketch_coordinate_y; 
  }
    //if this is a sketch that has already started continue building the array
    if(window_sketch_flag==1){
      sketch_array_x2[0]=window_sketch_coordinate_x;
      sketch_array_y2[0]=window_sketch_coordinate_y;
      if ((window_sketch_coordinate_x!=window_sketch_coordinate_x_previous)&&(window_sketch_coordinate_y!=window_sketch_coordinate_y_previous)){
      sketch_array_x=append(sketch_array_x,sketch_array_x2[0]);  
      sketch_array_y=append(sketch_array_y,sketch_array_y2[0]); 
      }
  }
    window_sketch_flag=1;
  }
  //if the right mouse button is pressed close the shape and finish building the array
    if((mousePressed==true)&&(mouseButton==RIGHT)&&(mouseX>window_sketch_x)&&(mouseX<window_sketch_bounds_x)&&(mouseY>window_sketch_y)&&mouseY<window_sketch_bounds_y&&shape_complete==0){
    noFill();
    endShape(CLOSE);
    button_menu_update_flag=0;
    window_sketch_flag=0;
    sketch_array_x2[0]=window_sketch_coordinate_x_close;
    sketch_array_y2[0]=window_sketch_coordinate_y_close;
    sketch_array_x=append(sketch_array_x,sketch_array_x2[0]);
    sketch_array_y=append(sketch_array_y,sketch_array_y2[0]);
    shape_complete=1;
    button_menu_polygon_status_flag=0;
    }
    window_sketch_coordinate_x_previous=window_sketch_coordinate_x;
    window_sketch_coordinate_y_previous=window_sketch_coordinate_y;
}
//______________________________________________________________
//create a sketch window to draw inside
void sketch_window_create(){
  text(window_sketch_title, 300, window_sketch_y-20);
  rect(window_sketch_x,window_sketch_y,window_sketch_width,window_sketch_height,button_corner);
  window_sketch_bounds_x=window_sketch_x+window_sketch_width;
  window_sketch_bounds_y=window_sketch_y+window_sketch_height;
}

//_______________________________________________________________
//update the button picture on the screen
void button_update(){
  fill(255,255,255);
  switch(button_status){//toggle the button ON and OFF if the mouse is pressed on the button
    case 0:
    if ((mouseX>button_pos_x&&mouseX<(button_pos_x+button_width))&&(mouseY>button_pos_y&&mouseY<(button_pos_y+button_height))&&(mousePressed==true)){
    button_create_ON(button_pos_x,button_pos_y,button_width,button_height,button_title1,button_corner);
  delay(150);  
  }
    break;
    case 1:
    if ((mouseX>button_pos_x&&mouseX<(button_pos_x+button_width))&&(mouseY>button_pos_y&&mouseY<(button_pos_y+button_height))&&(mousePressed==true)){
    button_create_OFF(button_pos_x,button_pos_y,button_width,button_height,button_title1,button_corner);
  delay(150);  
  }
    break;
}
}

//_______________________________________________________________
//create a new button that is ON
void button_create_ON(int x, int y, int button_width, int button_height, String button_title,int button_corner){
  button_check=createShape(GROUP);
  button_slot=createShape(RECT, x, y, button_width, button_height,button_corner);
  button_slot.setFill(color(0,128,255));
  button_indicator=createShape(ARC, x+12, y+12, 20, 20, 0, TWO_PI);
  button_check.addChild(button_slot);
  button_check.addChild(button_indicator);
  shape(button_check);
  button_status=1;
  text(button_title,button_width+x+15,y+15);
}

//_________________________________________________________________
//create a new button that is OFF
void button_create_OFF(int x,int y,int button_width, int button_height,String button_title, int button_corner){
  button_check=createShape(GROUP);
  button_slot=createShape(RECT, x, y, button_width,button_height,button_corner);
  button_slot.setFill(color(0,128,255));
  button_indicator=createShape(ARC, x+38, y+12, 20, 20, 0, TWO_PI);
  button_check.addChild(button_slot);
  button_check.addChild(button_indicator);
  shape(button_check);
  button_status=0;
  text(button_title,button_width+x+15,y+15);
}
  