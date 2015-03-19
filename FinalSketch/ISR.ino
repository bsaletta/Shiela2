boolean motorOn=false;
//boolean bcAtMax=false;
//boolean bcAtMin=false;
long isrTimer;
long isrLastTime=0;//Initialize the counter
int motorDtCount=0;//create the holder
void doISR(){//this is in the main loop so it is like our isr LOOP function
  isrTimer=millis();
  int isrDt=isrLastTime-isrTimer; //calculate the time difference
  //monitorBC();
  if(motorOn){
    if(motorDtCount>1000){ //time delay to turn it off
      motorDtCount=0;
      ST.motor(1,0);
      motorOn=false;
    }else{
      motorDtCount+=isrDt;
    }
  }else{
    if(motorDtCount>1000){ //time delay to turn it on again
      motorDtCount=0;
      ST.motor(1,127);
      motorOn=true;
    }else{
      motorDtCount+=isrDt;
    }
  }
  
 isrLastTime=isrTimer;
}

//void bouyancyControl(boolean up){
// if(up){
//  if(!bcAtMax){
//   ST.motor(2,127);
//  } 
// }else{
//  if(!bcAtMin){
//   ST.motor(2,-127);
//  } 
// }
//}
//void monitorBC(){
//  if(digitalRead(buoyancyInTrigger)){
//      bcAtMin=true;
//      ST.motor(2,0);
//  }else if(digitalRead(buoyancyOutTrigger)){
//     bcAtMax=true; 
//     ST.motor(2,0);
//  }
//}
