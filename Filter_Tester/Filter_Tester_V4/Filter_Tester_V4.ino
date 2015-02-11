#include <SPI.h>
#include <Wire.h>
#include <SFE_LSM9DS0.h>
//#include <LiquidCrystal.h>

#define LSM9DS0_XM 0x1D
#define LSM9DS0_G 0x6B

LSM9DS0 dof(MODE_I2C, LSM9DS0_G, LSM9DS0_XM);
//LiquidCrystal lcd(12,11,5,4,3,2);

const byte INT1XM = 7;
const byte INT2XM = 6;  //Define the pins where these are attached
const byte DRDYG = 8;
float abias[3]={0,0,0},gbias[3]={0,0,0};

long now=0;
long lastUpdate=0;
long gyroNow=0;
long gyroLastUpdate=0;
long  count=0;
float a[3]={0,0,0},g[3]={0,0,0},m[3]={0,0,0};
float P[3]={0,0,0},V[3]={0,0,0},o[3]={0,0,0},A[3]={0,0,0};
float G[3]={0,0,0},D[3]={0,0,0};
float dTheta[3]={0,0,0};
//float gravity=1.033;
float temperature;

boolean aFlag=false,gFlag=false,mFlag=false;
 int test=0;
void setup(){
  Serial.begin(9600);
  pinMode(INT1XM,INPUT);
  pinMode(INT2XM,INPUT);
  pinMode(DRDYG,INPUT); 
  uint32_t status = dof.begin();
  //lcd.begin(16,2);
  dof.setAccelScale(dof.A_SCALE_2G);
  dof.setGyroScale(dof.G_SCALE_245DPS);
  dof.setMagScale(dof.M_SCALE_2GS);
  
  dof.setAccelODR(dof.A_ODR_200);
  dof.setAccelABW(dof.A_ABW_50);
  
  dof. setGyroODR(dof.G_ODR_190_BW_125);
  
  dof.setMagODR(dof.M_ODR_125);
    
  dof.calLSM9DS0(gbias, abias);
  
  now=micros();
  gyroNow=now;
  
}
float aMag=0;
void loop(){
   if(digitalRead(DRDYG)){//Prepare for inturrupts
      readGyro();
   } 
   if(digitalRead(INT1XM)){
      readAccel(); 
   }
   if(digitalRead(INT2XM)){
      readMag();
   }
   if(gFlag){
       updateGyro();
   }
   if(aFlag && gFlag && mFlag){
     updatePosition(); 
   }
  
  if((millis()-count)>100){
    Serial.print(aMag,5);
    Serial.print(",");
    Serial.print(D[0],5);
    Serial.print(",");
    Serial.print(D[1],5);
    Serial.print(",");
    Serial.println(D[2],5);  
    test=0;
    count=millis();
  }
}

void updateGyro(){
  gyroLastUpdate=gyroNow;
  gyroNow=micros();
  long dt=gyroNow/gyroLastUpdate;
  
  G[0]+=g[0]*(float)dt/1000;
  G[1]+=g[1]*(float)dt/1000;
  G[2]+=g[2]*(float)dt/1000;
}
void updatePosition(){
  lastUpdate=now;
  now=micros();
  long dt=now-lastUpdate;
  //calculate orientation
  o[0]=(o[0]+G[0]);//+.1*(asin(a[0])*(180/PI));
  o[1]=(o[1]+G[1]);//+.1*(asin(a[1])*(180/PI));
  
  float oldZtheta=o[2];
   if (m[1] > 0){  
    o[2] = 90 - (atan(m[0] / m[1]) * (180 / PI));
  }else if (m[1] < 0) {
    o[2] = - (atan(m[0] / m[1]) * (180 / PI));
  }
  else{
    if (m[0] < 0) m[2] = 180;
    else o[2] = 0;
  }
  
  o[2]=.5*(oldZtheta+G[2])+.5*o[2];
  G[0]=0;
  G[1]=0;
  G[2]=0;
  //Force 
  
  //remove Gravity
  float gravX=sin(o[1]*PI/180);
  float gravY=sin(o[0]*PI/180);
  a[0]=a[0]-gravX;
  a[1]=a[1]-gravY;
  a[2]=a[2]-(1-(gravX+gravY));
  
  aMag=sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
  D[0]=atan(a[1]/a[2])*180/PI;
  D[1]=atan(a[2]/a[0])*180/PI;
  D[2]=atan(a[1]/a[0])*180/PI;
  
  //Transform to Global Coordinates
  float sx=sin((o[0]+D[0])*PI/180);
  float cx=cos((o[0]+D[0])*PI/180);
  float sy=sin((o[1]+D[1])*PI/180);
  float cy=cos((o[1]+D[1])*PI/180);  
  float sz=sin((o[2]+D[2])*PI/180);
  float cz=cos((o[2]+D[2])*PI/180);
  A[0]=cy*cx*a[0]+(sz*sy*cx-cz*sx)*a[1]+(cz*sy*cx+sz*sx)*a[2];
  A[1]=cy*sx*a[0]+(sz*sy*sx+cz*cx)*a[1]+(cz*sy*sx-sz*cz)*a[2];
  A[2]=-sy*a[0]+sz*cy*a[1]+cz*cy*a[2];
  
  //Calculate velocity
  V[0]+=A[0]*(float)dt/1000000;
  V[1]+=A[1]*(float)dt/1000000;
  V[2]+=A[2]*(float)dt/1000000;
  
  //Calculate position
  P[0]=V[0]*(float)dt/1000000;
  P[1]=V[1]*(float)dt/1000000;
  P[2]=V[2]*(float)dt/1000000;
  
  //Start timer
  now=micros();
  aFlag=false;
  gFlag=false;
  mFlag=false;
}




float aHolder[10][3]={{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
int aCount=0;
int aBufferSize=5;
void readAccel(){
    dof.readAccel();
    aHolder[aCount][0] = dof.calcAccel(dof.ax)-abias[0];
    aHolder[aCount][1] = dof.calcAccel(dof.ay)-abias[1];
    aHolder[aCount][2] = dof.calcAccel(dof.az)-abias[2];
    aCount++;
    if(aCount==5){

       float axSum=0,aySum=0,azSum=0;
       for(int i=0;i<aBufferSize;i++){
             axSum+=aHolder[i][0];
             aySum+=aHolder[i][1];
             azSum+=aHolder[i][2];
             test+=1;
       }
       a[0]=axSum/aCount;
       a[1]=aySum/aCount;
       a[2]=azSum/aCount;
       aCount=0;
       aFlag=true;
    }
}
//float gHolder[10][3]={{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
//int gCount=0;

void readGyro(){
    dof.readGyro();
    g[0] = dof.calcGyro(dof.gx)-gbias[0];
    g[1] = dof.calcGyro(dof.gy)-gbias[1];
    g[2] = dof.calcGyro(dof.gz)-gbias[2];
   // gCount++;
   // if(gCount==10){
   //    gCount=0;
   //    float gxSum=0,gySum=0,gzSum=0;
   //    for(int i=0;i<10;i++){
   //          gxSum+=gHolder[i][0];
   //          gySum+=gHolder[i][1];
   //          gzSum+=gHolder[i][2];
   //    }
   //    g[0]=gxSum/10;
    //   g[1]=gySum/10;
    //   g[2]=gzSum/10;
       
       gFlag=true;
    //}
}
float mHolder[10][3]={{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
float tHolder[10]={0,0,0,0,0,0,0,0,0,0};
int mCount=0;

void readMag(){
    dof.readMag();    
    dof.readTemp();
    tHolder[mCount] = 21.0f+(float)dof.temperature/8;
    mHolder[mCount][0] = dof.calcMag(dof.mx);
    mHolder[mCount][1] = dof.calcMag(dof.my);
    mHolder[mCount][2] = dof.calcMag(dof.mz);
    mCount++;
    if(mCount==10){
       mCount=0;
       float mxSum=0,mySum=0,mzSum=0,tSum=0;
       for(int i=0;i<10;i++){
             mxSum+=mHolder[i][0];
             mySum+=mHolder[i][1];
             mzSum+=mHolder[i][2];
             tSum+=tHolder[i];
       }
       m[0]=mxSum/10;
       m[1]=mySum/10;
       m[2]=mzSum/10;
       temperature=tSum/10;
       mFlag=true;
    }
}

