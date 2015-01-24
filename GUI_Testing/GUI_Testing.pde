PShape s;
void setup(){
  size(640,480,P3D);
 s=loadShape("Test.obj"); 
}
void draw(){
 background(204);
 translate(width/2,height/2);
 lights();// Create a light source so contrast can be seen
 scale(10);//Scale the drawing to 10%
 rotateZ(PI);
 float thetaX=(mouseY)*PI/480;
 float thetaY=(mouseX)*PI/480;
 rotateX(thetaX);//Have the monkey move with the mouse
 rotateY(thetaY);//
 shape(s,0,0);//Draw the thing
 
}
