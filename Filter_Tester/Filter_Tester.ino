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
}

float oldPoints[5]={0,0,0,0,0};
float oldVelocities[5]={0,0,0};
float pitch=0;
float x=0;
float vx=0;
float oldnetaccel=0;
void loop(){
  sensors_event_t accel,mag,gyro,temp;
  lsm.getEvent(&accel,&mag,&gyro,&temp);
  long dt=millis()-oldTime;
  oldTime=millis();
  //Angle Calculations
  float dty=gyro.gyro.y;
  float ax=accel.acceleration.x;
  
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
  
  //netaccel=(float)(floor(netaccel*1000))/1000;
  vx+=((oldnetaccel+netaccel)/2)*(float)dt/1000*32.174;
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
  x+=vx*(float)dt/1000;
  oldnetaccel=netaccel;
  lcd.clear();
  lcd.print("X: ");
  lcd.print(x*12);
  lcd.print(" in");
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
  delay(50);
}
