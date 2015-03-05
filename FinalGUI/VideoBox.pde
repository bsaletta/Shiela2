import processing.video.*;
//import processing.io.*;
Movie video;
int vidx,vidy,vidh,vidw;
boolean isPlaying=false;
void setupVideoBox(int x, int y, int h, int w){
  selectInput("Select Video File","selectedFile");
  vidx=x;vidy=y;vidw=w;vidh=h;
  frameRate(30);
  
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
int count=0;
void movieEvent(Movie m){
 m.read();
 if(count==15){
    String[] h=splitTokens(data[count/15],",");
    for(int i=0;i<3;i++){
      Position[i]=Integer.parseInt(h[i]);
      Orientation[i]=Integer.parseInt(h[3+i]);
    }
    updateCmd=true;
    updateOrient=true;
 }
count++; 
}
void videoClick(){
  if(isPlaying){
   video.pause();
        cmdLines.remove("Playing, Click Video to Pause");
    cmdLines.add("Paused,Click Video to Play");
    isPlaying=false; 
  }else{
    video.play();
    cmdLines.remove("Paused,Click Video to Play");
    cmdLines.add("Playing, Click Video to Pause");

    isPlaying=true;
  }
}
