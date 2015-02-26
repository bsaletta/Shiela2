float[] position={0,0,0};
float[] orientation={0,0,0};
void setup(){
  frame.setTitle("Edna's Adventure");
  size(900,600,OPENGL);
  
  setupCommandBox(455,305,435,285);
  setupMapBox(10,305,435,285);
  setupOrientationBox(455,10,435,285);
  setupVideoBox(10,10,435,285);
}
void draw(){
  drawCommandBox();
  drawMapBox();
  drawOrientationBox();
  drawVideoBox();
}
void mouseClicked(){
  int posX=mouseX;
  int posY=mouseY;
  if(posX>10 && posX<445){
   if(posY>10 && posY<295){
     videoClick();
   }else if(posY>305 && posY<590){
     orientationClick();
   }
  }else if(posX>455 && posX<890){
       if(posY>10 && posY<295){
     mapClick();
   }else if(posY>305 && posY<590){
     comandClick();
   }
  }
}
