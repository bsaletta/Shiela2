int cmdx,cmdy,cmdh,cmdw;
ArrayList<String> cmdLines; //create a holder for the lines to be writen to the comand line
PFont mono; //establish a font
void setupCommandBox(int x,int y,int h,int w){
  cmdx=x;cmdy=y;cmdh=h;cmdw=w;
  mono = createFont("Courier",12); //create the font
  textFont(mono);    //assign the created font to the text
  cmdLines=new ArrayList<String>(); //initate the container, using an array list because they are dynamicly sized
  cmdLines.add("Console Initated...");
  if(data!=null){
    cmdLines.add(data.length+" Data Points Loaded");  
  }else{
   cmdLines.add("No Data To Report"); 
  }
  
}
void drawCommandBox(){ //This is the main loop for the comand box function
  int lineheight=12;// 12 pt font means that the line heigth should be about 12 px
  stroke(0);//set line color
  fill(0);
  rect(cmdx,cmdy,cmdh,cmdw);//draw bounding rectangle
  fill(0,255,0);
  while((1.6*lineheight*cmdLines.size())>cmdh){ //if there are more lines than can fit in the command box remove the oldest ones
   cmdLines.remove(0); 
  }
  for(int i=0;i<cmdLines.size();i++){ //Write all of the lines in the container to the box
    text(cmdLines.get(i),cmdx+5,cmdy+lineheight*(i+1));
  }
  fill(255);
  updateCmd=false; //set the update flag to false because the box has just been updated
}
void comandClick(){
 //Handle A click in the commandBox
updateCmd=true; //Update if the box is clicked.
}
