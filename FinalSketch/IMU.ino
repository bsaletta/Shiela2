
void updateGyro(){
  gyroLastUpdate=gyroNow;
  gyroNow=micros();
  long dt=gyroNow-gyroLastUpdate;
  if(abs(g[0])>gMin){
    G[0]+=g[0]*(float)dt/1000000;
  }
  if(abs(g[1])>gMin){
    G[1]+=g[1]*(float)dt/1000000;
  }
  if(abs(g[2])>gMin){
    G[2]+=g[2]*(float)dt/1000000;
  }
}

float oldHeading;
void updateMag(){
 oldHeading=heading;
 float holder;
  if (m[1] > 0){  
     holder = 90 - (atan(m[0] / m[1]) * (180 / PI));
  }
  else if (m[1] < 0) {
    holder = - (atan(m[0] / m[1]) * (180 / PI));
  }
  else{
    if (m[0] < 0) holder = 180;
    holder = 0;
  }
  holder+=declination;
  if(abs(oldHeading-holder)<headingTolerance){
    heading=holder;
  }else{
    heading=oldHeading;
  } 
  
}

void updatePosition(){
  lastUpdate=now;
  now=micros();
  long dt=now-lastUpdate;
  //Check if acceleration is prettymuch just gravity
  aMag=sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
  float accelOrient[3]={0,0,0};
  if(abs(1-aMag)<gDeviation){
    gravity=aMag;
      accelOrient[1] = atan2(a[0], sqrt(a[1] * a[1]) + (a[2] * a[2]))*180.0 / PI;
      accelOrient[0] = atan2(a[1], sqrt(a[0] * a[0]) + (a[2] * a[2]))*180.0 / PI;
    //accelOrient[2]=atan(a[1]/a[0])*180/PI;
    o[0]=.8*(o[0]+G[0])+.2*accelOrient[0];
    o[1]=.8*(o[1]+G[1])+.2*accelOrient[1];
 }else{
  //calculate orientation
   o[0]=(o[0]+G[0]);
   o[1]=(o[1]+G[1]);
  }
  o[2]=heading;

  //o[2]=.9*(oldZtheta+G[2])+.1*o[2];
  G[0]=0;
  G[1]=0;
  G[2]=0; 

 //Transform to Global Coordinates
  float sx=sin((o[0])*PI/180);
  float cx=cos((o[0])*PI/180);
  float sy=sin((o[1])*PI/180);
  float cy=cos((o[1])*PI/180);  
  float sz=sin((o[2])*PI/180);
  float cz=cos((o[2])*PI/180);
  
  A[0]=cy*cx*a[0]+(sz*sy*cx-cz*sx)*a[1]+(cz*sy*cx+sz*sx)*a[2];
  A[1]=cy*sx*a[0]+(sz*sy*sx+cz*cx)*a[1]+(cz*sy*sx-sz*cz)*a[2];
  A[2]=-sy*a[0]+sz*cy*a[1]+cz*cy*a[2];

  //remove Gravity
  float aSph[3]={aMag,atan(A[1]/A[0]),atan(sqrt(A[0]*A[0]+A[1]*A[1])/A[2])};
  

float gravSph[3]={gravity,0,PI};
  for(int i=0;i<3;i++){
     aSph[i]=aSph[i]-gravSph[i]; 
  }
  
  
  
  A[0]=aSph[0]*sin(aSph[2])*cos(aSph[1]);
  A[1]=aSph[0]*sin(aSph[2])*sin(aSph[1]);
  A[2]=-aSph[0]*cos(aSph[2]);
  
  //Truncate values eliminate the trailing inaccuracy 
  A[0]=(float)(round(A[0]*truncateValue))/truncateValue;
  A[1]=(float)(round(A[1]*truncateValue))/truncateValue;
  A[2]=(float)(round(A[2]*truncateValue))/truncateValue;
 
  //Calculate velocity
  V[0]+=A[0]*Gc*(float)dt/1000000;
  V[1]+=A[1]*Gc*(float)dt/1000000;
  V[2]+=A[2]*Gc*(float)dt/1000000;
  
   
  for(int i=0;i<3;i++){
     //V[i]=V[i]-abias[i]*(float)dt/10000;
     if(V[i]==oldV[i])V[i]=0;
     if(V[i]>=maxVelocity)V[i]=0;
     
    oldV[i]=V[i]; 
  }
  

  //Calculate position
  P[0]+=V[0]*(float)dt/1000000;
  P[1]+=V[1]*(float)dt/1000000;
  P[2]+=V[2]*(float)dt/1000000;
  //}
  //Start timer
  now=micros();
  aFlag=false;
  gFlag=false;
  mFlag=false;
}


float aHolder[abuffer][3];
int aCount=0;

void readAccel(){
  dof.readAccel();
  aHolder[aCount][0] = ((dof.calcAccel(dof.ax)-abias[0])>maxAccel)?0:dof.calcAccel(dof.ax)-abias[0];
  aHolder[aCount][1] =  ((dof.calcAccel(dof.ay)-abias[1])>maxAccel)?0:dof.calcAccel(dof.ay)-abias[1];
  aHolder[aCount][2] =  ((dof.calcAccel(dof.az)-abias[2])>maxAccel)?0:dof.calcAccel(dof.az)-abias[2];
  aCount++;
  if(aCount==abuffer){

    float axSum=0,aySum=0,azSum=0;
    for(int i=0;i<abuffer;i++){
      axSum+=aHolder[i][0];
      aySum+=aHolder[i][1];
      azSum+=aHolder[i][2];
    }
    a[0]=axSum/aCount;
    a[1]=aySum/aCount;
    a[2]=azSum/aCount;
    aCount=0;
    aFlag=true;
  }
}

void readGyro(){
  dof.readGyro();
  g[0] = dof.calcGyro(dof.gx)-gbias[0];
  g[1] = dof.calcGyro(dof.gy)-gbias[1];
  g[2] = dof.calcGyro(dof.gz)-gbias[2];

  gFlag=true;
  //}
}
float mHolder[mbuffer][3];
float tHolder[mbuffer];
int mCount=0;

void readMag(){
  dof.readMag();    
  dof.readTemp();
  tHolder[mCount] = 21.0f+(float)dof.temperature/8;
  mHolder[mCount][0] = dof.calcMag(dof.mx);
  mHolder[mCount][1] = dof.calcMag(dof.my);
  mHolder[mCount][2] = dof.calcMag(dof.mz);
  mCount++;
  if(mCount==mbuffer){
    mCount=0;
    float mxSum=0,mySum=0,mzSum=0,tSum=0;
    for(int i=0;i<mbuffer;i++){
      mxSum+=mHolder[i][0];
      mySum+=mHolder[i][1];
      mzSum+=mHolder[i][2];
      tSum+=tHolder[i];
    }
    m[0]=mxSum/mbuffer;
    m[1]=mySum/mbuffer;
    m[2]=mzSum/mbuffer;
    temperature=tSum/mbuffer;
    mFlag=true;
  }
}

