int cmdx,cmdy,cmdh,cmdw;
PFont mono;
void setupCommandBox(int x,int y,int h,int w){
  cmdx=x;cmdy=y;cmdh=h;cmdw=w;
  mono = createFont("Courier",12);
  textFont(mono);
  
}
void drawCommandBox(){
  int lineheight=12;
  stroke(0);//set line color
  fill(0);
  rect(cmdx,cmdy,cmdh,cmdw);//draw bounding rectangle
  fill(0,255,0);
  text("Console Initated...",cmdx+5,cmdy+lineheight*1);
  text("Position: "+position[0]+","+position[1]+","+position[2],cmdx+5,cmdy+lineheight*2);
  text("Orientation: "+orientation[0]+","+orientation[1]+","+orientation[2],cmdx+5,cmdy+lineheight*3);
  if(isPlaying){
   text("Playing, Click Video to Pause",cmdx+5,cmdy+lineheight*4); 
  }else{
   text("Paused,Click Video to Play",cmdx+5,cmdy+lineheight*4); 
  }
  fill(255);
}
void comandClick(){
 //Handle A click in the commandBox 
}
