import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import processing.serial.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class Filter_Tester_Processing extends PApplet {



int lf=10; // the ASCII value of the '\n' character
String reading=null;
Serial arduino; //create serial port
PFont f; //Create a font for the text to be displayed
public void setup(){
 println(Serial.list());
 //WARNING!!! THIS LINE IS SPECIFIC TO YOUR COMPUTER
 //READ OUTPUT OF Serial.list THEN EDIT THIS LINE BEFORE CONTINUING
 //!!                     VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
  arduino=new Serial(this,Serial.list()[Serial.list().length-1],9600);
 //!!                     ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ 
 arduino.clear();
 reading=arduino.readStringUntil(lf);
 reading=null;//throw out first value incase we start in the middle of a line
  
 size(1000,300);// set the window size
 f=createFont("Arial",16,true); //Creates font arial 16pt ant-aliased

 String message="Reading Values";
 char[] msg=message.toCharArray();
 for(int i=0;i<msg.length;i++){
    arduino.write(msg[i]); 
 }
}
int oldVals[]=new int[1000]; //Create containers for old values
int oldValfs[]=new int[1000];

public void draw(){
 if(arduino.available()>0){
  
  reading=arduino.readStringUntil(lf);
  
  if(reading!=null && reading!=""){
    stroke(255);
  fill(255);            //clear screen by drawing a white rectangle
  rect(0,0,1000,300); //the size of the whole screen
  String input[]=split(reading,','); //break into 2 values;
  float measured=Float.parseFloat(input[0]);
  float filtered=Float.parseFloat(input[1]);//Turn strings into Float values
  float vx=Float.parseFloat(input[2]);
  float x=Float.parseFloat(input[3]); 
   
   textFont(f); //Set the Created font as the text font
   text("Velocity: "+vx,900,10);
   text("Position: "+x,900,30);  
  
  int val=(int)((measured+2)*150)-150; //Scale values to window
  int valf=(int)((filtered+2)*150)-150;
 
  //Here val in unfiltered and valf is filtered
  
  int vals[]=new int[1000];
  int valfs[]=new int[1000];
  
  vals[0]=val; // add the new measurement to the 
  valfs[0]=valf;// front of the array
  
  for(int i=1;i<vals.length;i++){ // These for loops populate
    vals[i]=oldVals[i-1];         // the rest of the value 
  }                               // arrays from preveious 
  for(int i=1;i<valfs.length;i++){ // measurements
    valfs[i]=oldValfs[i-1];
  }
  
  stroke(200); //Set measured line to grey
  for(int i=0;i<vals.length-1;i++){
     line(i,vals[i],i+1,vals[i+1]);
  }
  stroke(0xffFF0000); //Set filtered to red
  for(int i=0;i<valfs.length-1;i++){
    line(i,valfs[i],i+1,valfs[i+1]);
  }
  oldVals=vals;
  oldValfs=valfs;
  }
  
 } 
}
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "Filter_Tester_Processing" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
