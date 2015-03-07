String[] data;
boolean updateCmd=true;
boolean updateOrient=true;
boolean updateMap=true;
int[] Position={0,0,0};
int[] Orientation={0,0,0};
void setup(){
  frame.setTitle("Edna's Adventure"); //SEt the title of the box
  size(900,600,OPENGL); //Set the intiial dimentions of the box
  selectInput("Select Data File","dataFileSelected"); //Open a fileselection dialog to find data file
  setupCommandBox(455,305,435,285);     //  Thse 4 functions call the setup functions   
  setupMapBox(10,305,435,285);          //  for each of the subprograms. The numbers are the  
  setupOrientationBox(455,10,435,285);  //  (x,y) position of the top left corner of the containing box
  setupVideoBox(10,10,435,285);         //   the next two are the height and width of the box
}
void draw(){ //the main draw loop
  if(updateCmd){ //if the updateCmd flag is true then the command box's draw loop is run
    drawCommandBox();
  }
  drawMapBox(); //Draws the map box
  if(updateOrient){
    drawOrientationBox(); //Waits for the orientation box to need an update then updates it
  }
  drawVideoBox(); //Draws the video box. Everything is synched to the timing of the movie
}
void mouseClicked(){ //Delegate mouse clicks to the box they belong to.
  int posX=mouseX;
  int posY=mouseY;
  if(posX>10 && posX<445){// This breaks down the click and delegates is based on position
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
void dataFileSelected(File f){ //This is called when a data file is selected
  cmdLines.add("Loading data from "+f.getAbsolutePath()); //Adds a line to the command box
  updateCmd=true;
  data=loadStrings(f.getAbsolutePath()); //load the data from the file.
  cmdLines.add(data.length+" Datapoints Loaded");
}
