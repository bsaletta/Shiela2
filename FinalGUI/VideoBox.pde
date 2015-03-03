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
  frameRate(30);// Set the framerate to 30 frames per second to get accurate timing
  cmdLines.add("Paused,Click Video to Play");
}
void drawVideoBox() {
  stroke(0);//set line color
  //fill(0,0);
  rect(vidx, vidy, vidh, vidw);//draw bounding rectangle
  if (video!=null) {
    image(video, vidx, vidy, vidh, vidw);//draw the next frame of the video to the video box
  }
  
}
void selectedFile(File selection) {//handle movie file selection
  if (selection==null || selection.getAbsolutePath().indexOf(".mov")==-1) {
    //make sure that a file is selected and that it is a .mov file.
    video=new Movie(this, "TestVid.mov");
  } else {
    video=new Movie(this, selection.getAbsolutePath());
  }
}
int count;
void movieEvent(Movie m) {
  m.read();
  if (count%15==0) { // % gives the remainder, test if the remainder of the number of frames/15 is zero
    cmdLines.add(data[count/15]);//print out the data to the command box
  } 
  updateCmd=true;//make sure that the command box is updated
  count++;
}
void videoClick() {//handle a click on the video box
  if (isPlaying) {
    video.pause();
    cmdLines.add("Paused,Click Video to Play");
    isPlaying=false;
  } else {
    video.play();
    cmdLines.add("Playing, Click Video to Pause");
    isPlaying=true;
  }
  updateCmd=true;
}

