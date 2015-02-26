import de.fhpotsdam.unfolding.*;
import de.fhpotsdam.unfolding.geo.*;
import de.fhpotsdam.unfolding.utils.*; 
import de.fhpotsdam.unfolding.providers.Microsoft;
import de.fhpotsdam.unfolding.marker.*;
UnfoldingMap map;
int mapx,mapy,maph,mapw;
Location cpp;
SimplePointMarker cppMarker;
void setupMapBox(int x,int y,int h,int w){
  mapx=x;mapy=y;maph=h;mapw=w;
 map=new UnfoldingMap(this,"Map",x,y,h,w,true,false,new Microsoft.AerialProvider());
 map.setTweening(true);
 MapUtils.createDefaultEventDispatcher(this,map);
  cpp=new Location(34.0564,-117.8217);
  cppMarker=new SimplePointMarker(cpp);
  map.addMarker(cppMarker);
  map.zoomAndPanTo(cpp,12);
}
void drawMapBox(){
   stroke(0);//set line color
  rect(mapx,mapy,maph,mapw);//draw bounding rectangle
  //map.move(mapx,mapy);
  map.draw();
 
  //println(map.getTopLeftBorder());
}
void mapClick(){
 //handle a click in the map box 
}
