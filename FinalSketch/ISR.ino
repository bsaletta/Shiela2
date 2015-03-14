boolean motorOn=false;
boolean bcAtMax=false;
boolean bcAtMin=false;
long isrTimer;
long isrLastTime=0;
void doISR(){
  isrTimer=millis();
  int isrDT=isrLastTime-isrTimer;
  monitorBC();
  if(motorOn){
    
  }else{
   ST.motor(1,127);
   bouyacnyControl(false);
   motorOn=true; 
  }
  
  isrLastTime=isrTimer;
}

void bouyancyControl(boolean up){
 if(up){
  if(!bcAtMax){
   ST.motor(2,127);
  } 
 }else{
  if(!bcAtMin){
   ST.motor(2,-127);
  } 
 }
}
void monitorBC(){
  if(digitalRead(buoyancyInTrigger)){
      bcAtMin=true;
      ST.motor(2,0);
  }else if(digitalRead(buoyancyOutTrigger)){
     bcAtMax=true; 
     ST.motor(2,0);
  }
}
