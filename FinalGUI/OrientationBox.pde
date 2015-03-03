int orx,ory,orh,orw;
PShape model;
void setupOrientationBox(int x, int y,int h, int w){
  orx=x;ory=y;orh=h;orw=w;
  model=loadShape("Test.obj");//load the shape, must be in the data folder, or have a more specific path
  stroke(0);//set line color
  fill(3,113,156);
  rect(orx,ory,orh,orw);//draw bounding rectangle
}
void drawOrientationBox(){
  
  lights();//turn on the lights so contrast is visible
  fill(204);
  model.translate(orw/2,orh/2,10); //translate the model to the origin
  model.scale(10); //Scale it to 10% so it fits in the box
  model.rotateZ(PI); //Rotate it so it's looking the right way
  shape(model,orx,ory); //draw the shape
  fill(255);
  updateOrient=false;//orientation has been update so this should be set to false
}
void orientationClick(){
 updateOrient=true;//update on click
}
