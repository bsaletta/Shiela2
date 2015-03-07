import de.fhpotsdam.unfolding.*;
import de.fhpotsdam.unfolding.geo.*;
import de.fhpotsdam.unfolding.utils.*; 
import de.fhpotsdam.unfolding.providers.Microsoft;
import de.fhpotsdam.unfolding.marker.*;
import de.fhpotsdam.unfolding.ui.BarScaleUI;
import g4p_controls.*;
//Import the library for the maps unfoldingmaps.org
boolean mapReady=false;
UnfoldingMap map;
BarScaleUI bar;
int mapx, mapy, maph, mapw;
Location initialPosition=null;
SimplePointMarker cppMarker;
ArrayList<Location> dataPoints=new ArrayList<Location>();
void setupMapBox(int x, int y, int h, int w) {

  mapx=x;
  mapy=y;
  maph=h;
  mapw=w;
  setupInitialLocationBox();
  map=new UnfoldingMap(this, "Map", x, y, h, w, true, false, new Microsoft.AerialProvider());
  //bar=BarScaleUI(this,map);
  //create a map of the appropreate size in the right location and make it a satilite map
  map.setTweening(true);//<- not sure but it wont work with out it
  MapUtils.createDefaultEventDispatcher(this, map); //what part of the program handles the map? this one!

  //Location cpp=new Location(34.0564, -117.8217); //geolocation of Cal poly Pomona
  //cppMarker=new SimplePointMarker(cpp);//Add a marker at this location
  //cppMarker.setColor(color(255, 0, 0));
  //map.addMarker(cppMarker);
  // map.zoomAndPanTo(cpp, 2);//go to that location
}
void drawMapBox() {
  stroke(0);//set line color
  fill(0);
  rect(mapx, mapy, maph, mapw);//draw bounding rectangle
  //map.move(mapx,mapy);
  //keep the map updated
  //bar.draw();
  if (initialPosition==null) {
    drawInitialLocationBox();
    //map.addMarker(new SimplePointMarker(initialPosition));
    //map.zoomAndPanTo(initialPosition,10);
  } else {
    map.draw();
    G4P.setWindowAlpha(this, 0);
    if (updateMap) {
      for (int i=0; i<dataPoints.size (); i++) {
        SimplePointMarker pt=new SimplePointMarker(dataPoints.get(i));
        pt.setColor(color(255,0,0));
        map.addMarker(pt);
      }
      updateMap=false;
    }
  }
  //println(map.getTopLeftBorder());
}
void mapClick() {
  println("Map Clicked!");
}

GTextField latField;
GTextField longField;
GButton submit;
void setupInitialLocationBox() {
  latField=new GTextField(this, mapx+25, mapy+95, 100, 20);
  longField=new GTextField(this, mapx+230, mapy+95, 100, 20);
  submit=new GButton(this, mapx+160, mapy+120, 80, 20, "Submit");
  latField.setText("34.241389");
  longField.setText("-118.254167");
}
void updatePath() {
  float xOffset=(float)Position[0]/362778;
  float yOffset=(float)Position[1]/362778;
  dataPoints.add(new Location(initialPosition.getLat()+xOffset, initialPosition.getLon()+yOffset));
  updateMap=true;
}
void drawInitialLocationBox() {
  fill(255);
  rect(mapx+15, mapy+75, 400, 90);
  fill(0);
  text("Please Enter Inital Latitude and Longitude", mapx+80, mapy+90);
}
void handleButtonEvents(GButton button, GEvent event) {
  initialPosition=new Location(Float.parseFloat(latField.getText()), Float.parseFloat(longField.getText()));
  //map.zoomAndPanTo(initialPosition,10);
  SimplePointMarker initial=new SimplePointMarker(initialPosition);//Add a marker at this location
  initial.setColor(color(255, 0, 0));
  map.addMarker(initial);
  mapReady=true;
  dataPoints.add(initialPosition);
  map.zoomAndPanTo(initialPosition,10);
}

