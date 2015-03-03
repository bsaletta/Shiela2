int cmdx,cmdy,cmdh,cmdw;
PFont mono;
ArrayList<String> lines=new ArrayList<String>();

void setupCommandBox(int x,int y,int h,int w){
  cmdx=x;cmdy=y;cmdh=h;cmdw=w;
  mono = createFont("Courier",12);
  textFont(mono);
  lines.add("Console Initated...");
  lines.add("Position: "+position[0]+","+position[1]+","+position[2]);
  lines.add("Orientation: "+orientation[0]+","+orientation[1]+","+orientation[2]);
  if(isPlaying){
    lines.remove("Paused,Click Video to Play");
    lines.add("Playing, Click Video to Pause"); 
  }else{
    lines.remove("Playing, Click Video to Pause");
    lines.add("Paused,Click Video to Play");
  }
}

void drawCommandBox(){
  int lineheight=12;
  stroke(0);//set line color
  fill(0);
  rect(cmdx,cmdy,cmdh,cmdw);//draw bounding rectangle
  fill(0,255,0);

  for(int i=0;i<lines.size();i++){
   text(lines.get(i),cmdx+5,cmdy+(i+1)*12); 
  }
  fill(255);
}
void comandClick(){
 //Handle A click in the commandBox 
}
