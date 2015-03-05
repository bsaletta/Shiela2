int cmdx,cmdy,cmdh,cmdw;
PFont mono;
ArrayList<String> cmdLines=new ArrayList<String>(); //create a holder for the command lines

void setupCommandBox(int x,int y,int h,int w){
  cmdx=x;cmdy=y;cmdh=h;cmdw=w;
  mono = createFont("Courier",12);//create a font
  textFont(mono);//set the created font to the 
  cmdLines.add("Console Initated...");
}

void drawCommandBox(){
  int lineheight=12;
  stroke(0);//set line color
  fill(0);
  rect(cmdx,cmdy,cmdh,cmdw);//draw bounding rectangle
  fill(0,255,0);
  cmdLines.add("Position: "+Position[0]+","+Position[1]+","+Position[2]);
  cmdLines.add("Orientation: "+Orientation[0]+","+Orientation[1]+","+Orientation[2]);//put the position and orientation data on the command screen
  while(cmdLines.size()*12>cmdh/1.6){
   cmdLines.remove(0); 
  }
  for(int i=0;i<cmdLines.size();i++){
   text(cmdLines.get(i),cmdx+5,cmdy+(i+1)*lineheight); 
  }
  fill(255);
  updateCmd=false;
}
void comandClick(){
  updateCmd=true;
 //Handle A click in the commandBox 
}
