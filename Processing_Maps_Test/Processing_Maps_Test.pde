import de.fhpotsdam.unfolding.mapdisplay.*;
import de.fhpotsdam.unfolding.utils.*;
import de.fhpotsdam.unfolding.marker.*;
import de.fhpotsdam.unfolding.tiles.*;
import de.fhpotsdam.unfolding.interactions.*;
import de.fhpotsdam.unfolding.ui.*;
import de.fhpotsdam.unfolding.*;
import de.fhpotsdam.unfolding.core.*;
import de.fhpotsdam.unfolding.mapdisplay.shaders.*;
import de.fhpotsdam.unfolding.data.*;
import de.fhpotsdam.unfolding.geo.*;
import de.fhpotsdam.unfolding.texture.*;
import de.fhpotsdam.unfolding.events.*;
import de.fhpotsdam.utils.*;
import de.fhpotsdam.unfolding.providers.*;
//Download from unfoldingmaps.org
UnfoldingMap map;

void setup(){
 size(800,600);
  map=new UnfoldingMap(this, new Microsoft.AerialProvider());
  
 MapUtils.createDefaultEventDispatcher(this,map);
 
 map.draw();
 
 map.zoomAndPanTo(new Location(34.0608f,-117.7558f),50);
}
void draw(){
 map.draw(); 
}
