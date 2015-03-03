import de.fhpotsdam.unfolding.*;
import de.fhpotsdam.unfolding.geo.*;
import de.fhpotsdam.unfolding.utils.*; 
import de.fhpotsdam.unfolding.providers.Microsoft;
import de.fhpotsdam.unfolding.marker.*;
//Import the library for the maps unfoldingmaps.org

UnfoldingMap map;
int mapx, mapy, maph, mapw;
Location cpp;
SimplePointMarker cppMarker;

void setupMapBox(int x, int y, int h, int w) {
  mapx=x;
  mapy=y;
  maph=h;
  mapw=w;
  map=new UnfoldingMap(this, "Map", x, y, h, w, true, false, new Microsoft.AerialProvider());
   //create a map of the appropreate size in the right location and make it a satilite map
  map.setTweening(true);//<- not sure but it wont work with out it
  MapUtils.createDefaultEventDispatcher(this, map); //what part of the program handles the map? this one!
  cpp=new Location(34.0564, -117.8217); //geolocation of Cal poly Pomona
  cppMarker=new SimplePointMarker(cpp);//Add a marker at this location
  map.addMarker(cppMarker);
  map.zoomAndPanTo(cpp, 12);//go to that location
}
void drawMapBox() {
  stroke(0);//set line color
  rect(mapx, mapy, maph, mapw);//draw bounding rectangle
  //map.move(mapx,mapy);
  map.draw();//keep the map updated

  //println(map.getTopLeftBorder());
}
void mapClick() {
  //handle a click in the map box
}

