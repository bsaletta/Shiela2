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
  Serial.begin(115200);
  lcd.begin(16,2);
    if (!lsm.begin()){
    while(1){
    Serial.println("Oops ... unable to initialize the LSM9DS0. Check your wiring!");
    }
    setupSensor();
    }
    oldTime=millis();
  }
float oldapts[5];//old acceleration points
void loop(){
  sensors_event_t accel,mag,gyro,temp;
  lsm.getEvent(&accel,&mag,&gyro,&temp); 
  
  float dty=gyro.gyro.y;
  float ax=accel.acceleration.x;
  
  pitch=.5*(pitch+dty*((float)dt/1000))+.5*(asin(ax)*(180/3.14157));//filter and calculate the pitch angle
  
  float netaccel=(ax)-sin(pitch*3.14157/180);//Compensate for gravity
  
  float apts[5];
   for(int i=1;i<5;i++){
    apts[i]=oldapts[i-1];
  }
  for(int i=0;i<5;i++){
     oldapts[i]=apts[i]; 
  }
  asum
  
  
}
