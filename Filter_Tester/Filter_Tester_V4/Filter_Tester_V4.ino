#include <SPI.h>
#include <Wire.h>
#include <SFE_LSM9DS0.h>
#include <LiquidCrystal.h>

#define LSM9DS0_XM 0x1D
#define LSM9DS0_G 0x6B

LSM9DS0 dof(MODE_I2C, LSM9DS0_G, LSM9DS0_XM);
LiquidCrystal lcd(12,11,5,4,3,2);

const byte INT1XM = 7;
const byte INT2XM = 6;  //Define the pins where these are attached
const byte DRDYG = 8;
float abias[3]={0,0,0},gbias[3]={0,0,0};

long now=0;
long lastUpdate=0;
long  count=0;
float ax,ay,az,gx,gy,gz,mx,my,mz,pitch,roll;
float heading,temperature;

void setup(){
  Serial.begin(9600);
  pinMode(INT1XM,INPUT);
  pinMode(INT2XM,INPUT);
  pinMode(DRDYG,INPUT); 
  uint32_t status = dof.begin();
  lcd.begin(16,2);
  dof.setAccelScale(dof.A_SCALE_2G);
  dof.setGyroScale(dof.G_SCALE_245DPS);
  dof.setMagScale(dof.M_SCALE_2GS);
  
  dof.setAccelODR(dof.A_ODR_200);
  dof.setAccelABW(dof.A_ABW_50);
  
  dof. setGyroODR(dof.G_ODR_190_BW_125);
  
  dof.setMagODR(dof.M_ODR_125);
    
  dof.calLSM9DS0(gbias, abias);
  
  now=micros();
  
}
void loop(){
     if(digitalRead(DRDYG)){
    dof.readGyro();
    gx=dof.calcGyro(dof.gx)-gbias[0];
    gy=dof.calcGyro(dof.gy)-gbias[1];
    gz=dof.calcGyro(dof.gz)-gbias[2];
   } 
   if(digitalRead(INT1XM)){
    dof.readAccel();
    ax = dof.calcAccel(dof.ax)-abias[0];
    ay = dof.calcAccel(dof.ay)-abias[1];
    az = dof.calcAccel(dof.az)-abias[2]; 
   }
   if(digitalRead(INT2XM)){
    dof.readMag();
    mx=dof.calcMag(dof.mx);
    my=dof.calcMag(dof.my);
    mz=dof.calcMag(dof.mz);
    
    dof.readTemp();
    temperature = 21.0f+(float)dof.temperature/8;
   }
   lastUpdate=now;
   now=micros();
  long dt=now-lastUpdate;
  
  pitch=.5*(pitch+gy*((float)dt/1000000))+.5*(asin(ax)*(180/PI));
  roll=.5*(roll+gx*((float)dt/1000000))+.5*(asin(ay)*(180/PI));
  
  if (my > 0){
    heading = 90 - (atan(mx / my) * (180 / PI));
  }else if (my < 0) {
    heading = - (atan(mx / my) * (180 / PI));
  }
  else{
    if (mx < 0) heading = 180;
    else heading = 0;
  }
  

  //ax=filterX(ax,pitch);
  //ay=filterY(ay,roll);
  //az=filterZ(az);
  if((millis()-count)>50){
    Serial.println((ax+cos(pitch*(PI/180)))*1000,9);  
    lcd.clear();
    lcd.print(ax*1000);
    lcd.setCursor(0,2);
    lcd.print(ay*1000);
    count=millis();
  }
}
float xs[5]={0,0,0,0,0};
float filterX(float x,float angle){
  float holder[5];

  x=ax-cos(angle);
    //x=((floor(abs(x*100)))*x/abs(x))/100;
    holder[0]=x;
  for(int i=1;i<5;i++){
   holder[i]=xs[i]; 
  }
  for(int i=0;i<5;i++){
   xs[i]=holder[i]; 
  }
  x=(xs[0]+xs[1]+xs[2]+xs[3]+xs[4])/5;
  return x;
}
float ys[5]={0,0,0,0,0};
float filterY(float y,float angle){
  float holder[5];
  y=y-sin(angle*(PI/180));
  //y=((floor(abs(y*100)))*y/abs(y))/100;
  holder[0]=y;
  for(int i=1;i<5;i++){
   holder[i]=ys[i]; 
  }
  for(int i=0;i<5;i++){
   ys[i]=holder[i]; 
  }
    y=(ys[0]+ys[1]+ys[2]+ys[3]+ys[4])/5;
  return y;
}
float filterZ(float x){
  return x;
}

