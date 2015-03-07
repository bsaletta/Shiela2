import processing.video.*;
//import processing.io.*;
Movie video;
int vidx, vidy, vidh, vidw;
boolean isPlaying=false;
void setupVideoBox(int x, int y, int h, int w) {
  selectInput("Select Video File", "selectedFile");
  vidx=x;
  vidy=y;
  vidw=w;
  vidh=h;
  frameRate(30); //set framerate so that the data can sync with the data
}
void drawVideoBox() {
  stroke(0);//set line color
  //fill(0,0);
  rect(vidx, vidy, vidh, vidw);//draw bounding rectangle
  if (video!=null) {
    image(video, vidx, vidy, vidh, vidw);
  }
}
void selectedFile(File selection) {
  if (selection==null || selection.getAbsolutePath().indexOf(".mov")==-1) {//make sure the vido is selected and has the right format
    video=new Movie(this, "TestVid.mov"); //Default test movie
  } else {
    video=new Movie(this, selection.getAbsolutePath()); //Open selected movie
  }
}
int count=0;
void movieEvent(Movie m) {
  m.read(); //read the next frame
  if ((count%15)==0) { //Check to see if the remainder of count/15 == 0, make sure were a multipul of 15
    String[] h=splitTokens(data[count/15], ","); //Break CSV into array
    for (int i=0; i<3; i++) {
      Position[i]=Integer.parseInt(h[i]);
      Orientation[i]=Integer.parseInt(h[3+i]);
     
    }
     updatePath();
      cmdLines.add("Position: "+Position[0]+","+Position[1]+","+Position[2]);
      cmdLines.add("Orientation: "+Orientation[0]+","+Orientation[1]+","+Orientation[2]);//put the position and orientation data on the command screen
    updateCmd=true;
    updateOrient=true; //Update the orientation and comand boxes
    updateMap=true;
  }
  count++;
}
void videoClick() {
  if (isPlaying) {
    video.pause();//Put data into the comand line

      cmdLines.add("Paused,Click Video to Play");
    isPlaying=false;
  } else {
    if (mapReady) {
      video.play();

      cmdLines.add("Playing, Click Video to Pause");

      isPlaying=true;
    } else {
      cmdLines.add("Please Select An Initial Position First"); 
      updateCmd=true;
    }
  }
}

