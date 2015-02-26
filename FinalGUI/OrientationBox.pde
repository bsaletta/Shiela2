int orx,ory,orh,orw;
PShape model;
void setupOrientationBox(int x, int y,int h, int w){
  orx=x;ory=y;orh=h;orw=w;
  model=loadShape("Test.obj");
  stroke(0);//set line color
  fill(3,113,156);
  rect(orx,ory,orh,orw);//draw bounding rectangle
}
void drawOrientationBox(){
  
  lights();
  fill(204);
  model.translate(orw/2,orh/2,10);
  model.scale(10);
  model.rotateZ(PI);
  shape(model,orx,ory);
  fill(255);
}
void orientationClick(){
 //handle a click in the orientation box 
}
