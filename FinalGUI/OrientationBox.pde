int orx,ory,orh,orw;
import processing.opengl.*;
//import saito.objloader.*;
int[] orientation={0,0,0};
//OBJModel model;
PShape model;

void setupOrientationBox(int x, int y,int h, int w){
  orx=x;ory=y;orh=h;orw=w;
  //model = new OBJModel(this, "Full Vehicle Processing.obj",OBJModel.RELATIVE);
  model=loadShape("Full Vehicle Processing.obj");
  model.scale(.2);
  noStroke();
  fill(color(#006994));
  rect(orx,ory,orh,orw);//draw bounding rectangle
  fill(255);
  
  // model.translate(orw,orh,10);
}
void drawOrientationBox(){
  
  lights();//turn on the lights so contrast is visible
  fill(204);
  //translate(orw/2,orh/2,10); 
  int i = 0;
  fill(color(#006994));
  rect(orx,ory,orh,orw);//draw bounding rectangle
  fill(255);
 shape(model,orx+orw/2,ory+orw/2);
    //PVector translation=new PVector(orx+orw/2,ory+orh/2,0);
     //translates model to origin of orientation box
   //translate(width/1.6,height/2.7,0.0f);
    float x = Orientation[0];
    float y = Orientation[1];
    float z = Orientation[2];
    //PVector rotation=new PVector(x,y,z);    
    //model.setNormal(0,rotation);
    model.rotateX(radians(x));
    model.rotateY(radians(y));
    model.rotateZ(radians(z));
    
    //rotateX(radians(-x));
    //rotateY(radians(-y));
    //rotateZ(radians(-z));
    i += 1;
  updateOrient=false;//orientation has been update so this should be set to false
}

void orientationClick(){
 updateOrient=true;//update on click
}

