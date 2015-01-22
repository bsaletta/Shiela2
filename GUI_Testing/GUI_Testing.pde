PShape s;
void setup(){
  size(640,480,P3D);
 s=loadShape("Test.obj"); 
}
void draw(){
 background(204);
 translate(width/2,height/2);
  shape(s,0,0);//Not working yet
}
