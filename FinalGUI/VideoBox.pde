import processing.video.*;
//import processing.io.*;
Movie video;
int vidx,vidy,vidh,vidw;
boolean isPlaying=false;
void setupVideoBox(int x, int y, int h, int w){
  selectInput("Select Video File","selectedFile");
  vidx=x;vidy=y;vidw=w;vidh=h;
  
}
void drawVideoBox(){
  stroke(0);//set line color
  //fill(0,0);
  rect(vidx,vidy,vidh,vidw);//draw bounding rectangle
  if(video!=null){
    image(video,vidx,vidy,vidh,vidw);
}
}
void selectedFile(File selection){
  if(selection==null || selection.getAbsolutePath().indexOf(".mov")==-1){
    video=new Movie(this,"TestVid.mov");
  }else{
   video=new Movie(this,selection.getAbsolutePath()); 
  }
}
void movieEvent(Movie m){
 m.read(); 
}
void videoClick(){
  if(isPlaying){
   video.pause();
    isPlaying=false; 
  }else{
    video.play();
    isPlaying=true;
  }
}
