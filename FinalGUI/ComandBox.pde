int cmdx,cmdy,cmdh,cmdw;
PFont mono;
ArrayList<String> cmdLines=new ArrayList<String>();

void setupCommandBox(int x,int y,int h,int w){
  cmdx=x;cmdy=y;cmdh=h;cmdw=w;
  mono = createFont("Courier",12);
  textFont(mono);
  cmdLines.add("Console Initated...");
  cmdLines.add("Position: "+Position[0]+","+Position[1]+","+Position[2]);
  cmdLines.add("Orientation: "+Orientation[0]+","+Orientation[1]+","+Orientation[2]);
  if(isPlaying){
    cmdLines.remove("Paused,Click Video to Play");
    cmdLines.add("Playing, Click Video to Pause"); 
  }else{
    cmdLines.remove("Playing, Click Video to Pause");
    cmdLines.add("Paused,Click Video to Play");
  }
}

void drawCommandBox(){
  int lineheight=12;
  stroke(0);//set line color
  fill(0);
  rect(cmdx,cmdy,cmdh,cmdw);//draw bounding rectangle
  fill(0,255,0);

  for(int i=0;i<cmdLines.size();i++){
   text(cmdLines.get(i),cmdx+5,cmdy+(i+1)*12); 
  }
  fill(255);
}
void comandClick(){
  updateCmd=true;
 //Handle A click in the commandBox 
}
