#include <LiquidCrystal.h>
#include <Wire.h>
#include<SPI.h>
#include <Adafruit_LSM9DS0.h>;
#include <Adafruit_Sensor.h>;


Adafruit_LSM9DS0 lsm = Adafruit_LSM9DS0();


LiquidCrystal lcd(12,11,5,4,3,2);
void setupSensor(){
   
  lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_2G);
  lsm.setupMag(lsm.LSM9DS0_MAGGAIN_2GAUSS);
  lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_245DPS);
}

long oldTime;

void setup(){
  Serial.begin(9600);
  lcd.begin(16,2);
 
  lcd.print("    Process");
  lcd.setCursor(0,2);
  lcd.print("   Initiated");
  if (!lsm.begin()){
    while(1){
    Serial.println("Oops ... unable to initialize the LSM9DS0. Check your wiring!");
    }
    setupSensor();
  }
  oldTime=millis();
    sensors_event_t accel,mag,gyro,temp; //Clear old reading from the board
  lsm.getEvent(&accel,&mag,&gyro,&temp);
}

float oldPoints[5]={0,0,0,0,0};
float oldVelocities[5]={0,0,0};
float pitch=0;
float x=0;
float vx=0;
float oldnetaccel=0;
int count=0;
void loop(){
  sensors_event_t accel,mag,gyro,temp;
  lsm.getEvent(&accel,&mag,&gyro,&temp);
  long dt=millis()-oldTime;
  oldTime=millis();
  //Angle Calculations
  float dty=gyro.gyro.y;
  float dtx=gyro.gyro.x;
  float dtz=gyro.gyro.z;
  
  float ax=accel.acceleration.x;
  float ay=accel.acceleration.y;
  float az=accel.acceleration.z;
  
  float mx=mag.magnetic.x;
  float my=mag.magnetic.y;
  float mz=mag.magnetic.z;
  
  pitch=.5*(pitch+dty*((float)dt/1000))+.5*(asin(ax)*(180/3.14157));
  
  
  
  //linear approximation
  float points[5];
  points[0]=ax;
  for(int i=1;i<5;i++){
    points[i]=oldPoints[i-1];
  }
  for(int i=0;i<5;i++){
     oldPoints[i]=points[i]; 
  }
  float xsum;
  xsum=points[0]+points[1]+points[2]+points[3]+points[4];
  float netaccel=(xsum/5)-sin(pitch*3.14157/180);
  //float error=netaccel*1000;
  netaccel=(float)(floor(abs(netaccel*100)))/100*(netaccel/abs(netaccel));
  if(count>5){
    
    float vxsample=((netaccel))*(float)dt/1000*32.174;
    if(vxsample!=0){
      vx+=(float)(floor(abs(vxsample*1000)))/1000*(vxsample/abs(vxsample));
    }else{
      if(vx!=0){
        vx-=vx/abs(vx)*.0001;//return to zero tendancy
      }
    }
    float velocities[3];
    velocities[0]=vx;
    for(int i=1;i<3;i++){
      velocities[i]=oldVelocities[i-1];
    }
    for(int i=0;i<3;i++){
      oldVelocities[i]=velocities[i]; 
    }
    float vxsum=velocities[0]+velocities[1]+velocities[2];
    vx=vxsum/3;
    if(count>8){
      
      x+=vx*(float)dt/1000;
    }
  }
  oldnetaccel=netaccel;
  lcd.clear();
  lcd.print("X: ");
  lcd.print(x*12);
  lcd.print(" in");
  lcd.setCursor(0,2);
    lcd.print("VX: ");
  lcd.print(vx*12);
  lcd.print(" in/s");
  Serial.print(ax);
  Serial.print(',');
  Serial.print(netaccel);
  Serial.print(',');
  Serial.print(vx);
  Serial.print(',');
  Serial.println(x);
  if(Serial.available()>0){
    delay(100);
    lcd.clear();
    while(Serial.available()>0){
      lcd.write(Serial.read());
    }
  }
  count++;
  delay(150);
}
